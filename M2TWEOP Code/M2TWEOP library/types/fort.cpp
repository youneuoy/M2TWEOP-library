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
#include "strategyMap.h"

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
			auto oldCoords = coordPair{static_cast<int>(fort->xCoord), static_cast<int>(fort->yCoord)};
			if (const auto coords = stratMapHelpers::findValidTileNearTile(&oldCoords, 7);
				stratMapHelpers::isTileValidForCharacterType(static_cast<int>(characterTypeStrat::namedCharacter), coords))
			{
				sol::table units = sol::state_view(plugData::data.luaAll.luaState).create_table();
				for (int i = 0; i < fort->army->numOfUnits; i++)
					units.add(fort->army->units[i]);
				const auto faction = fort->army->faction;
				if (const auto army = factionHelpers::splitArmy(faction, units, coords->xCoord, coords->yCoord); !army)
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
	
	void createFortXY(factionStruct* fac, int x, int y)
	{
		factionStruct* faction = fac;
		character* newgen = characterHelpers::createCharacterWithoutSpawning("named character", faction, 30, "fort", "fort", 31, "default", x, y);
		armyStruct* newarmy = armyHelpers::createArmy(newgen);
		auto cultureDb = cultures::getCultureDb();
		auto culture = cultureDb->cultures[fac->cultureID];
		auto cost = culture.fortCost;
		auto oldMoney = fac->money;
		fac->money = cost;
		auto oldOption = campaignHelpers::getCampaignDb()->campaignDbSettlement.canBuildForts;
		campaignHelpers::getCampaignDb()->campaignDbSettlement.canBuildForts = true;
		DWORD adrFunc = codes::offsets.createFortFunc;
		_asm
		{
			mov ecx, newarmy
			mov eax, adrFunc
			call eax
		}
		characterHelpers::killCharacter(newgen);
		campaignHelpers::getCampaignDb()->campaignDbSettlement.canBuildForts = oldOption;
		fac->money = oldMoney;
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
		@tfield int creatorFactionID
		@tfield int creatorCultureID
		@tfield int gatesAreOpened
		@tfield int fortFortificationLevel
		@tfield getSiege getSiege
		@tfield changeOwner changeOwner
		
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
		Change fort ownership.
		@function fortStruct:changeOwner
		@tparam factionStruct newFaction
		@tparam bool convertGarrison
		@usage
			myFort:changeOwner(otherFac, true)
		*/
		luaType.fortStruct.set_function("changeOwner", &changeOwner);
    	
    }
}
