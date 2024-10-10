///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "fort.h"

#include "dataOffsets.h"
#include "functionsOffsets.h"
#include "luaPlugin.h"
#include "faction.h"
#include "unit.h"
#include "character.h"
#include "characterRecord.h"
#include "army.h"
#include "campaignDb.h"
#include "cultures.h"
#include "gameHelpers.h"
#include "strategyMap.h"
#include "rebelFactions.h"

namespace fortHelpers
{
	
	void changeOwner(fortStruct* fort, factionStruct* newFaction, bool convertGarrison)
	{
		if (convertGarrison)
		{
			const int charNum = GAME_FUNC(int(__thiscall*)(fortStruct*, int), getResidenceCharacterNum)(fort, 1);
			for (int i = 0; i < charNum; i++)
			{
				const auto thisChar = GAME_FUNC(character*(__thiscall*)(fortStruct*, int, int), getResidenceCharacterAtIndex)(fort, i, 1);
				if (thisChar->genType->type != characterTypeStrat::spy || thisChar->characterRecord->faction == fort->faction)
				{
					DWORD funcAddr = codes::offsets.switchCharacterFaction;
					_asm
						{
						push 0
						push 0
						push newFaction
						mov ecx, thisChar
						mov eax, funcAddr
						call eax
						}
				}
				GAME_FUNC(void(__thiscall*)(character*), changeCharacterTileStuff)(thisChar);

				if (thisChar->genType->type == characterTypeStrat::namedCharacter)
				{
					DWORD funcAddr2 = codes::offsets.switchNamedCharacterFaction;
					_asm
						{
						push 0
						push newFaction
						mov ecx, thisChar
						mov eax, funcAddr2
						call eax
						}
				}
				GAME_FUNC(void(__thiscall*)(character*), initPlaceCharacter)(thisChar);
			}
			if (auto stack = fort->army)
			{
				auto origFaction = stack->faction;
				DWORD funcAddr3 = codes::offsets.switchArmyFaction;
				_asm
					{
					push newFaction
					push stack
					mov ecx, origFaction
					mov eax, funcAddr3
					call eax
					}
			}
		}
		
		callClassFunc<fortStruct*, void, int, factionStruct*, int>(fort, 0x9c, 0, newFaction, 7);
		
		GAME_FUNC(void(__thiscall*)(fortStruct*, int), removeSieges)(fort, 0);
		if (newFaction->factionHordeInfo && newFaction->factionHordeInfo->isHorde)
		{
			auto globalFort = reinterpret_cast<fortStruct*>(dataOffsets::offsets.globalSett);
			globalFort = fort;
			auto hordeInfo = newFaction->factionHordeInfo;
			DWORD funcAddr4 = codes::offsets.doHordeStuff;
			_asm
			{
				mov ecx, hordeInfo
				mov eax, funcAddr4
				call eax
			}
		}
			
		if (!convertGarrison && fort->army)
		{
			if (const auto [newX, newY] = stratMapHelpers::findValidTileNearTile(fort->xCoord, fort->yCoord, 7);
				stratMapHelpers::isTileValidForCharacterType(static_cast<int>(characterTypeStrat::namedCharacter), newX, newY))
			{
				sol::table units = sol::state_view(plugData::data.luaAll.luaState).create_table();
				for (int i = 0; i < fort->army->numOfUnits; i++)
					units.add(fort->army->units[i]);
				const auto faction = fort->army->faction;
				if (const auto army = factionHelpers::splitArmy(faction, units, newX, newY); !army)
				{
					if (fort->army)
					{
						for (int i = 0; i < fort->army->numOfUnits; i++)
							unitHelpers::killUnit(fort->army->units[i]);
					}
				}
				units.clear();	
			}
		}
	}

	void deleteFort(const factionStruct* fac, fortStruct* fort)
	{
		fortStruct* delFort = fort;
		const factionStruct* delFaction = fac;
		DWORD funcB = codes::offsets.deleteFortFuncOne;
		DWORD funcC = codes::offsets.deleteFortFuncTwo;
		DWORD facOffset = dataOffsets::offsets.factionOffsetsStart + 0x4;
		_asm
		{
			mov edi, delFort
			mov esi, delFaction
			push edi
			mov eax, funcB
			call eax
			push edi
			push ebp
			mov ebp, facOffset
			lea ecx, [ebp + 0x21608]
			pop ebp
			mov eax, funcC
			call eax
		}
	}
	
	fortStruct* createFortXY(factionStruct* fac, const int x, const int y)
	{
		return createFortXYCulture(fac, x, y, fac->cultureID);
	}

	stringWithHash* WOODEN_FORT = nullptr;

	bool isInMapBounds(const int x, const int y)
	{
		const auto stratMap = stratMapHelpers::getStratMap();
		return x >= 0 && x < stratMap->mapWidth && y >= 0 && y < stratMap->mapHeight;
	}
	
	fortStruct* createFortXYCulture(factionStruct* fac, const int x, const int y, const int cultureId)
	{
		factionStruct* faction = fac;
		const auto tile = stratMapHelpers::getStratMap()->getTile(x, y);
		auto neighbours = stratMapHelpers::getNeighbourTiles(x, y);
		while (true)
		{
			if (neighbours.empty())
				break;
			const auto [checkX, checkY] = neighbours.front();
			neighbours.pop();
			const auto nTile = stratMapHelpers::getTile(checkX, checkY);
			if (!nTile)
				continue;
			if (nTile->settlement || nTile->fort)
				return nullptr;
		}
		const int regionId = tile->regionId;
		if (!WOODEN_FORT)
		{
			WOODEN_FORT = new stringWithHash();
			gameStringHelpers::setHashedString(&WOODEN_FORT->name, "wooden_fort");
		}
		const auto fort = GAME_FUNC(fortStruct*(__cdecl*)(int, factionStruct*, int, const char*, int)
			, createsFort)(regionId, faction, cultureId, WOODEN_FORT->name, WOODEN_FORT->hash);
		if (!fort)
		{
			gameHelpers::logStringGame("Failed to create fort at x: " + std::to_string(x) + " y: " + std::to_string(y));
			return nullptr;
		}
		coordPair coords { x, y };
		GAME_FUNC(void(__thiscall*)(stratPathFinding*, void*, coordPair*),
			spawnCreatedObject)(campaignHelpers::getStratPathFinding(), fort, &coords);
		tile->fort = true;
		const auto campaign = campaignHelpers::getCampaignData();
		GAME_FUNC(char(__thiscall*)(fortStruct***, fortStruct*), addToFortsArray)(&campaign->fortsArray, fort);
	    GAME_FUNC(char(__thiscall*)(fortStruct*), residenceTileCharacterCheck)(fort);
		return fort;
	}
	
	void addToLua(sol::state& luaState)
    {
		///Fort
		//@section Fort

		/***
		Basic fortStruct table

		@tfield int xCoord
		@tfield int yCoord
		@tfield character governor
		@tfield armyStruct army
		@tfield factionStruct ownerFaction
		@tfield int siegeNum
		@tfield int maxHoldoutTurns
		@tfield int siegeDuration
		@tfield int preSiegePopulation
		@tfield int factionTradedFrom
		@tfield int plagueDuration
		@tfield int salliedOut
		@tfield int readyToSurrender
		@tfield crusadeStruct takenByCrusade
		@tfield int plagued
		@tfield int subFactionID
		@tfield int regionID
        @tfield rebelFaction rebelEntry
		@tfield int creatorFactionID
		@tfield int creatorCultureID
		@tfield int gatesAreOpened
		@tfield int fortFortificationLevel
		@tfield int characterCount
		@tfield getSiege getSiege
		@tfield getCharacter getCharacter
		@tfield changeOwner changeOwner
		@tfield changeOwner changeOwner
		@tfield isAllyToFaction isAllyToFaction
		@tfield isEnemyToFaction isEnemyToFaction
		
		@table fortStruct
		*/
		struct
		{
			sol::usertype<fortStruct> fortStruct;
		}luaType;
		
		luaType.fortStruct = luaState.new_usertype<fortStruct>("fortStruct");
		luaType.fortStruct.set("xCoord", &fortStruct::xCoord);
		luaType.fortStruct.set("yCoord", &fortStruct::yCoord);
		luaType.fortStruct.set("governor", sol::property(&fortStruct::getGovernor));
		luaType.fortStruct.set("characterCount", sol::property(&fortStruct::characterCount));
		luaType.fortStruct.set("army", &fortStruct::army);
		luaType.fortStruct.set("gatesAreOpened", &fortStruct::gatesAreOpened);
		luaType.fortStruct.set("ownerFaction", &fortStruct::faction);
		luaType.fortStruct.set("siegeNum", &fortStruct::siegeNum);
		luaType.fortStruct.set("maxHoldoutTurns", &fortStruct::maxHoldoutTurns);
		luaType.fortStruct.set("siegeDuration", &fortStruct::siegeDuration);
		luaType.fortStruct.set("preSiegePopulation", &fortStruct::preSiegePopulation);
		luaType.fortStruct.set("factionTradedFrom", &fortStruct::factionTradedFrom);
		luaType.fortStruct.set("plagueDuration", &fortStruct::plagueDuration);
		luaType.fortStruct.set("salliedOut", &fortStruct::salliedOut);
		luaType.fortStruct.set("readyToSurrender", &fortStruct::readyToSurrender);
		luaType.fortStruct.set("takenByCrusade", &fortStruct::takenByCrusade);
		luaType.fortStruct.set("plagued", &fortStruct::plagued);
		luaType.fortStruct.set("subFactionID", &fortStruct::subFactionID);
		luaType.fortStruct.set("regionID", &fortStruct::regionID);
		luaType.fortStruct.set("creatorFactionID", &fortStruct::creatorFactionID);
		luaType.fortStruct.set("creatorCultureID", &fortStruct::creatorCultureID);
		luaType.fortStruct.set("rebelEntry", &fortStruct::descrRebel);
		luaType.fortStruct.set("fortFortificationLevel", &fortStruct::fortFortificationLevel);

		/***
		Get a specific siege by it's index
		@function fortStruct:getSiege
		@tparam int siegeIdx
		@treturn siegeStruct siege
		@usage
		for i = 0, currSet.siegesNum-1 do
		   local siege=currFort:getSiege(i);
		   --etc
		end
		*/
		luaType.fortStruct.set_function("getSiege", &fortStruct::getSiege);

		/***
		Get a specific character by index.
		@function fortStruct:getCharacter
		@tparam int index
		@treturn character foundChar
		@usage
		local characterNum = currFort.characterCount
		for i = 0, characterNum - 1 do
		   local char = currFort:getCharacter(i)
		   --etc
		end
		*/
		luaType.fortStruct.set_function("getCharacter", &fortStruct::getCharacter);

		/***
		Change fort ownership.
		@function fortStruct:changeOwner
		@tparam factionStruct newFaction
		@tparam bool convertGarrison
		@usage
			myFort:changeOwner(otherFac, true)
		*/
		luaType.fortStruct.set_function("changeOwner", &changeOwner);

		/***
		Check if fort is allied to faction.
		@function fortStruct:isAllyToFaction
		@tparam factionStruct otherFac
		@treturn bool isAlly
		@usage
			local isAlly = myFort:isAllyToFaction(otherFac)
		*/
		luaType.fortStruct.set_function("isAllyToFaction", &fortStruct::isAllyToFaction);

		/***
		Check if fort is at war with faction.
		@function fortStruct:isEnemyToFaction
		@tparam factionStruct otherFac
		@treturn bool isEnemy
		@usage
			local isEnemy = myFort:isEnemyToFaction(otherFac)
		*/
		luaType.fortStruct.set_function("isEnemyToFaction", &fortStruct::isEnemyToFaction);
    	
    }
}

bool fortStruct::isAllyToFaction(const factionStruct* otherFac)
{
	if (!faction || !otherFac)
		return false;
	if (faction->factionID == otherFac->factionID)
		return true;
	const auto facDip = campaignHelpers::getCampaignData()->getFactionDiplomacy(faction->factionID, otherFac->factionID);
	if (!facDip)
		return false;
	return facDip->state == dipStance::alliance;
}

bool fortStruct::isEnemyToFaction(const factionStruct* otherFac)
{
	if (!faction || !otherFac)
		return false;
	if (faction->factionID == otherFac->factionID)
		return false;
	const auto facDip = campaignHelpers::getCampaignData()->getFactionDiplomacy(faction->factionID, otherFac->factionID);
	if (!facDip)
		return false;
	return facDip->state == dipStance::war;
}

int fortStruct::characterCount()
{
	const auto tile = stratMapHelpers::getTile(xCoord, yCoord);
	return tile->getTileCharacterCount();
}

character* fortStruct::getCharacter(const int index)
{
	const auto tile = stratMapHelpers::getTile(xCoord, yCoord);
	return tile->getTileCharacterAtIndex(index);
}
