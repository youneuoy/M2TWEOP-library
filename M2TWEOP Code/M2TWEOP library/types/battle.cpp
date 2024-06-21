///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "battle.h"

#include "dataOffsets.h"
#include "functionsOffsets.h"
#include "globals.h"
#include "fort.h"
#include "settlement.h"
#include "army.h"
#include "unit.h"
#include "character.h"
#include "gameHelpers.h"


float battleTile::getGroundHeight()
{
	const auto terrainData = battleHelpers::getBattleTerrainData();
	if (!terrainData)
		return 0;
	return height * 0.1f + terrainData->heightOffset;
}

int towerStats::getFireRateFlaming()
{
	const auto battleData = battleHelpers::getBattleData();
	if (!battleData)
		return 0;
	return fireRatesNormal[battleData->unitSize].flaming;
}

int towerStats::getFireRate()
{
	const auto battleData = battleHelpers::getBattleData();
	if (!battleData)
		return 0;
	return fireRatesNormal[battleData->unitSize].normal;
}

void towerStats::setFireRateFlaming(const int rate)
{
	const auto battleData = battleHelpers::getBattleData();
	if (!battleData)
		return;
	fireRatesNormal[battleData->unitSize].flaming = rate;
}

void towerStats::setFireRate(const int rate)
{
	const auto battleData = battleHelpers::getBattleData();
	if (!battleData)
		return;
	fireRatesNormal[battleData->unitSize].normal = rate;
}

float battleTile::getWaterHeight()
{
	const auto terrainData = battleHelpers::getBattleTerrainData();
	if (!terrainData)
		return 0;
	return waterHeight * 0.1f + terrainData->heightOffset;
}

namespace battleHelpers
{
	battleResidence* getBattleResidence()
	{
		const auto gameData = gameHelpers::getGameDataAll();
		if (const auto battleSettlement = gameData->battleSettlement; battleSettlement)
			return battleSettlement->battleResidence;
		return nullptr;
	}
	
	float getBattleMapHeight(const float xCoord, const float yCoord)
	{
		const DWORD* offset = reinterpret_cast<DWORD*>(dataOffsets::offsets.battleMapData);
		return GAME_FUNC(float(__thiscall*)(DWORD, float, float, float), getMapHeight)(*offset, xCoord, yCoord, 0.0f);
	}

	battlefieldEngines* getBattlefieldEngines()
	{
		const auto battleFieldEngines = *reinterpret_cast<battlefieldEngines**>(dataOffsets::offsets.battlefieldEngines);
		return battleFieldEngines;
	}

	int getBattleCondCode(const DWORD condObject)
	{
		if (condObject == 0)
			return -1;
		const DWORD* objectPtr = reinterpret_cast<DWORD*>(condObject);
		const DWORD vtablePtr = *objectPtr;
		
		//steam version
		if (globals::dataS.gameVersion == 2)
		{
			switch (vtablePtr)
			{
			    case 0x012ed814: return 0; //destroy_or_rout_enemy
			    case 0x012fe30c: return 1; //balance_of_strength_percent
			    case 0x012fe334: return 2; //destroy_enemy_strength_percent
			    case 0x012fe2bc: return 3; //capture_location
			    case 0x012fe9c4: return 4; //destroy_character
			    case 0x012ed7ec: return 5; //capture_major_settlement
			    case 0x012fe2e4: return 6; //capture_army_settlement
			    default: return -1;
			} 
		}
		
		//disk version
		switch (vtablePtr)
		{
		    case 0x01332834: return 0; //destroy_or_rout_enemy
		    case 0x0134332c: return 1; //balance_of_strength_percent
		    case 0x01343354: return 2; //destroy_enemy_strength_percent
		    case 0x013432dc: return 3; //capture_location
		    case 0x013439e4: return 4; //destroy_character
		    case 0x0133280c: return 5; //capture_major_settlement
		    case 0x01343304: return 6; //capture_army_settlement
		    default: return -1;
		}
	}

	streetPosition* getStreetPosition(const int index)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters)
			return nullptr;
		if (index < 0 || index >= perimeters->streetDeploymentPositions.size())
			return nullptr;
		return &perimeters->streetDeploymentPositions[index];
	}

	int getStreetPositionCount()
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters)
			return 0;
		return perimeters->streetDeploymentPositions.size();
	}

	bool isZoneValid(const int zoneID)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters) return false;
		return GAME_FUNC(bool(__thiscall*)(battlePerimeters*, int), isZoneValid)(perimeters, zoneID);
	}

	int getZonePerimeter(const float x, const float y)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters) return -1;
		battlePos pos{x,y};
		const auto posPtr = &pos;
		return GAME_FUNC(int(__thiscall*)(battlePerimeters*, battlePos*), getZonePerimeter)(perimeters, posPtr);
	}

	int getZoneID(const float x, const float y)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters) return -1;
		const DWORD offset = reinterpret_cast<DWORD>(perimeters) + 0x48;
		battlePos pos{x,y};
		const auto posPtr = &pos;
		return GAME_FUNC(int(__thiscall*)(DWORD, battlePos*), getZoneID)(offset, posPtr);
	}
	
	battlePerimeters* getBattlePerimeters()
	{
		return reinterpret_cast<battlePerimeters*>(dataOffsets::offsets.battlePerimeters);
	}
	
	std::string getWinConditionS(const DWORD condObject)
	{
		switch (getBattleCondCode(condObject))
		{
		    case 0: return "destroy_or_rout_enemy";
		    case 1: return "balance_of_strength_percent";
		    case 2: return "destroy_enemy_strength_percent";
		    case 3: return "capture_location";
		    case 4: return "destroy_character";
		    case 5: return "capture_major_settlement";
		    case 6: return "capture_army_settlement";
		    default: return "unknown_condition";
		}
	}
	
	battleCameraStruct* getBattleCamCoords()
	{
		// If we aren't in a battle
		if (!inBattle())
			return nullptr;
		return reinterpret_cast<battleCameraStruct*>(dataOffsets::offsets.battleCamera);
	}
	
	battleDataS* getBattleData()
	{
		return gameHelpers::getGameDataAll()->battleHandler;
	}

	int getBattleTileIndex(const float xCoord, const float yCoord)
	{
		return static_cast<int>((roundf(xCoord + 1000 - 0.5f) / 2) + 1000 * (roundf(yCoord + 1000 - 0.5f) / 2));
	}
	
	bool inBattle()
	{
		const auto battle = getBattleData();
		if (!battle) return false;
		return battle->inBattle;
	}
	
	void autoResolve()
	{
		DWORD adrFunc = codes::offsets.autoResolveFunc;
		_asm
		{
			mov eax, adrFunc
			call eax
		}
	}
	
	bool autoWin(const char* winnerSide)
	{
		DWORD adrFunc = codes::offsets.autoWinFunc;
		string command = winnerSide;
		char buffer[100]{};
		const char* cmdC = command.c_str();

		bool result = false;
		_asm
		{
			lea eax, buffer
			push eax
			push cmdC
			mov eax, adrFunc
			call eax
			mov result, al
		}
		return result;
	}
	
	battleTerrainData* getBattleTerrainData()
	{
		return gameHelpers::getGameDataAll()->battleTerrainDataPtr;
	}

	battleTile* getBattleTile(const float xCoord, const float yCoord)
	{
		const auto terrainData = getBattleTerrainData();
		if (!terrainData)
			return nullptr;
		const auto index = getBattleTileIndex(xCoord, yCoord);
		if (index < 0 || index >= 1000000)
			return nullptr;
		return &terrainData->battleTiles[index];
	}
	
    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::usertype<battleDataS> battleTable;
            sol::usertype<battleSide> battleSideTable;
            sol::usertype<battleSideArmy> battleSideArmyTable;
            sol::usertype<deploymentAreaS> deploymentAreaTable;
            sol::usertype<battlePos> battlePos;
            sol::usertype<battleAI> battleAI;
            sol::usertype<lineData> lineData;
            sol::usertype<battleStrengthStruct> battleStrengthStruct;
            sol::usertype<battleArmy> battleArmy;
            sol::usertype<battleUnit> battleUnit;
            sol::usertype<battleResidence> battleResidence;
            sol::usertype<aiBattleObjective> battleObjective;
            sol::usertype<buildingBattle> buildingBattle;
            sol::usertype<battleBuildings> battleBuildings;
            sol::usertype<battleTile> battleTile;
            sol::usertype<battlefieldEngines> battlefieldEngines;
            sol::usertype<terrainLineSegment> terrainLine;
            sol::usertype<terrainFeatures> terrainFeatures;
            sol::usertype<terrainFeatureHill> terrainFeatureHill;
            sol::usertype<plazaStuff> plazaData;
            sol::usertype<fortBattleInfo> fortBattleInfo;
            sol::usertype<streetPosition> streetPosition;
            sol::usertype<perimeterBuildings> perimeterBuildings;
            sol::usertype<towerEntry> battleBuildingStats;
            sol::usertype<towerStats> towerStats;
            sol::usertype<battleCameraStruct>battleCameraStruct;
        }typeAll;
    	
		///Battle Data
		//@section Battle Data

		/***
		basic battleStruct table

		@tfield int battleState
		@tfield int battleType
		@tfield bool isNightBattle
		@tfield bool battleAiEnabled
		@tfield bool defenderWithdrawn
		@tfield int xCoord
		@tfield int yCoord
		@tfield int attackingSide
		@tfield int defendingSide
		@tfield int residenceOwnerFacId
		@tfield int attackerXCoord
		@tfield int attackerYCoord
		@tfield int defenderXCoord
		@tfield int defenderYCoord
		@tfield bool paused
		@tfield bool isAutoResolve
		@tfield bool isFortBattle
		@tfield bool inBattle
		@tfield int finishTimer
		@tfield float battleSpeed
		@tfield float secondsPassed
		@tfield int secondsSinceBattleLoaded
		@tfield int hidingEnabledSet
		@tfield fortBattleInfo fortInfo
		@tfield float mapWidth
		@tfield float mapHeight
		@tfield terrainFeatures terrainFeatures
		@tfield int sidesNum
		@tfield int unitSize
		@tfield int tideOfBattle
		@tfield int playerArmyNum
		@tfield int timeWinningAlliance
		@tfield battleSide[8] sides Returns a battleSide[8]. Maximum: 8.
		@tfield int[31] factionSide faction alliance array, -1 if not in battle, start at 1 so faction ID + 1 Maximum 31.
		@tfield getPlayerArmy getPlayerArmy
		@tfield getBattleResidence getBattleResidence
		@tfield getUnitByLabel getUnitByLabel
		@tfield getGroupByLabel getGroupByLabel
		@tfield getBattleMapHeight getBattleMapHeight
		@tfield getBattlefieldEngines getBattlefieldEngines
		@tfield getBattleTile getBattleTile
		@tfield getZoneID getZoneID
		@tfield getPosPerimeter getPosPerimeter
		@tfield isZoneValid isZoneValid

		@table battleStruct
		*/
		typeAll.battleTable = luaState.new_usertype<battleDataS>("battleStruct");
		typeAll.battleTable.set("battleState", &battleDataS::battleState);
		typeAll.battleTable.set("battleType", &battleDataS::battleType);
		typeAll.battleTable.set("isNightBattle", &battleDataS::isNightBattle);
		typeAll.battleTable.set("attackingSide", &battleDataS::attackingSide);
		typeAll.battleTable.set("isAutoResolve", &battleDataS::isAutoResolve);
		typeAll.battleTable.set("defendingSide", &battleDataS::defendingSide);
		typeAll.battleTable.set("timeWinningAlliance", &battleDataS::timeWinningAlliance);
		typeAll.battleTable.set("residenceOwnerFacId", &battleDataS::residenceOwnerFacId);
		typeAll.battleTable.set("defenderWithdrawn", &battleDataS::defenderWithdrawn);
		typeAll.battleTable.set("battleAiEnabled", &battleDataS::battleAiEnabled);
		typeAll.battleTable.set("isFortBattle", &battleDataS::isFortBattle);
		typeAll.battleTable.set("isAutoResolve", &battleDataS::isAutoResolve);
		typeAll.battleTable.set("xCoord", &battleDataS::xCoord);
		typeAll.battleTable.set("finishTimer", &battleDataS::finishTimer);
		typeAll.battleTable.set("fortInfo", &battleDataS::fortInfo);
		typeAll.battleTable.set("yCoord", &battleDataS::yCoord);
		typeAll.battleTable.set("playerArmyNum", &battleDataS::playerArmyNum);
		typeAll.battleTable.set("attackerXCoord", &battleDataS::attackerXCoord);
		typeAll.battleTable.set("attackerYCoord", &battleDataS::attackerYCoord);
		typeAll.battleTable.set("defenderXCoord", &battleDataS::defenderXCoord);
		typeAll.battleTable.set("defenderYCoord", &battleDataS::defenderYCoord);
		typeAll.battleTable.set("paused", &battleDataS::paused);
		typeAll.battleTable.set("unitSize", &battleDataS::unitSize);
		typeAll.battleTable.set("tideOfBattle", &battleDataS::tideOfBattle);
		typeAll.battleTable.set("inBattle", &battleDataS::inBattle);
		typeAll.battleTable.set("battleSpeed", &battleDataS::speed);
		typeAll.battleTable.set("secondsPassed", &battleDataS::secondsPassed);
		typeAll.battleTable.set("secondsSinceBattleLoaded", &battleDataS::secondsSinceBattleLoaded);
		typeAll.battleTable.set("hidingEnabledSet", &battleDataS::hidingEnabledSet);
		typeAll.battleTable.set("mapWidthDoubled", &battleDataS::mapWidth);
		typeAll.battleTable.set("mapHeightDoubled", &battleDataS::mapHeight);
		typeAll.battleTable.set("mapWidth", &battleDataS::mapWidth);
		typeAll.battleTable.set("mapHeight", &battleDataS::mapHeight);
		typeAll.battleTable.set("sidesNum", &battleDataS::sidesNum);
		typeAll.battleTable.set("terrainFeatures", &battleDataS::terrainFeatures);
		typeAll.battleTable.set("sides", sol::property([](battleDataS& self) { return std::ref(self.sides); }));
		typeAll.battleTable.set("factionSide", sol::property([](battleDataS& self) { return std::ref(self.factionSide); }));
		/***
		Get a players army.
		@function battleStruct:getPlayerArmy
		@tparam int index
		@treturn armyStruct army
		@usage

			local playerArmy = battle:getPlayerArmy(0)

		*/
		typeAll.battleTable.set_function("getPlayerArmy", &battleDataS::getPlayerArmy);
		/***
		Get table with certain info about the battle residence.
		@function battleStruct.getBattleResidence
		@treturn battleResidence battleResidence
		@usage

			local battleResidence = battle.getBattleResidence()

		*/
		typeAll.battleTable.set_function("getBattleResidence", &getBattleResidence);
		/***
		Get a unit by it's label.
		@function battleStruct.getUnitByLabel
		@tparam string label
		@treturn unit foundUnit
		@usage
		local unit = BATTLE.getUnitByLabel("a1u1");
		*/
		typeAll.battleTable.set_function("getUnitByLabel", &unitActions::getUnitByLabel);
		/***
		Get a group by it's label.
		@function battleStruct.getGroupByLabel
		@tparam string label
		@treturn unitGroup foundGroup
		@usage
		      local group = BATTLE.getGroupByLabel("group1");
		*/
		typeAll.battleTable.set_function("getGroupByLabel", &unitHelpers::getGroupByLabel);
		
		/***
		Get battlemap height at position.
		@function battleStruct.getBattleMapHeight
		@tparam float xCoord
		@tparam float yCoord
		@treturn float zCoord
		@usage
		local unit = BATTLE.getBattleMapHeight(10,20);
		*/
		typeAll.battleTable.set_function("getBattleMapHeight", &getBattleMapHeight);
		
		/***
		Get battlefield engines.
		@function battleStruct.getBattlefieldEngines
		@treturn battlefieldEngines engines
		@usage
		     local engines = BATTLE.getBattlefieldEngines();
		*/
		typeAll.battleTable.set_function("getBattlefieldEngines", &getBattlefieldEngines);
		
		/***
		Get battlefield tile.
		@function battleStruct.getBattleTile
		@tparam float xCoord
		@tparam float yCoord
		@treturn battleTile tile
		@usage
		     local tile = BATTLE.getBattleTile(103.65, 385.54);
		*/
		typeAll.battleTable.set_function("getBattleTile", &getBattleTile);
		
		/***
		Get zone ID.
		@function battleStruct.getZoneID
		@tparam float xCoord
		@tparam float yCoord
		@treturn int zoneID
		@usage
		     local id = BATTLE.getZoneID(103.65, 385.54);
		*/
		typeAll.battleTable.set_function("getZoneID", &getZoneID);
		
		/***
		Get position perimeter.
		@function battleStruct.getPosPerimeter
		@tparam float xCoord
		@tparam float yCoord
		@treturn int perimeter
		@usage
		     local perimeter = BATTLE.getPosPerimeter(103.65, 385.54);
		*/
		typeAll.battleTable.set_function("getPosPerimeter", &getZonePerimeter);
		
		/***
		Is zone valid.
		@function battleStruct.isZoneValid
		@tparam int zoneID
		@treturn bool valid
		@usage
		     local valid = BATTLE.isZoneValid(3);
		*/
		typeAll.battleTable.set_function("isZoneValid", &isZoneValid);
		
		/***
		Basic battlefieldEngines table

		@tfield int engineNum
		@tfield getEngine getEngine

		@table battlefieldEngines
		*/
		typeAll.battlefieldEngines = luaState.new_usertype<battlefieldEngines>("battlefieldEngines");
		typeAll.battlefieldEngines.set("engineNum", &battlefieldEngines::enginesNum);

		/***
		Get an engine from the battlefield.
		@function battlefieldEngines:getEngine
		@tparam int index
		@treturn siegeEngineStruct engine
		@usage
			local engine = battlefieldEngines:getEngine(0)
		*/
		typeAll.battlefieldEngines.set_function("getEngine", &battlefieldEngines::getSiegeEngine);
		
		/***
		Basic fortBattleInfo table

		@tfield fortStruct fort
		@tfield armyStruct garrison
		@tfield factionStruct faction
		@tfield int ownerFactionID
		@tfield int creatorFactionID
		@tfield int fortFortificationLevel

		@table fortBattleInfo
		*/
		typeAll.fortBattleInfo = luaState.new_usertype<fortBattleInfo>("fortBattleInfo");
		typeAll.fortBattleInfo.set("fort", &fortBattleInfo::fort);
		typeAll.fortBattleInfo.set("garrison", &fortBattleInfo::garrison);
		typeAll.fortBattleInfo.set("faction", &fortBattleInfo::faction);
		typeAll.fortBattleInfo.set("ownerFactionID", &fortBattleInfo::ownerFactionID);
		typeAll.fortBattleInfo.set("creatorFactionID", &fortBattleInfo::creatorFactionID);
		typeAll.fortBattleInfo.set("fortFortificationLevel", &fortBattleInfo::fortFortificationLevel);
		
		///Battle Side
		//@section Battle Side

		/***
		Basic battleSide table

		@tfield bool isDefender
		@tfield bool canWithdraw
		@tfield bool canDeploy
		@tfield int wonBattle 0 = lose, 1 = draw, 2 = win
		@tfield int battleSuccess 0 = close, 1 = average, 2 = clear, 3 = crushing
		@tfield int[4] winConditions Returns an int index of a wincondition. Maximum 4.
		@tfield getWinConditionString getWinConditionString
		@tfield int armiesNum
		@tfield int battleArmyNum
		@tfield int alliance
		@tfield int soldierCountStart
		@tfield int factionCount
		@tfield int totalStrength
		@tfield int reinforceArmyCount
		@tfield float battleOdds
		@tfield int activeArmyStrength
		@tfield battleAI battleAIPlan
		@tfield getBattleArmy getBattleArmy
		@tfield getFaction getFaction
		@tfield hasFaction hasFaction
		@tfield getReinforcementArmy getReinforcementArmy
		@tfield battleSideArmy[64] armies Returns a table of battleSideArmy. Maximum: 64.
		
		@table battleSide
		*/
		typeAll.battleSideTable = luaState.new_usertype<battleSide>("battleSide");
		typeAll.battleSideTable.set("isDefender", &battleSide::isDefender);
		typeAll.battleSideTable.set("canDeploy", &battleSide::canDeploy);
		typeAll.battleSideTable.set("canWithdraw", &battleSide::canWithdraw);
		typeAll.battleSideTable.set("isCanDeploy", &battleSide::canDeploy);
		typeAll.battleSideTable.set("wonBattle", &battleSide::wonBattle);
		typeAll.battleSideTable.set("battleSuccess", &battleSide::battleSuccess);
		typeAll.battleSideTable.set("battleArmyNum", &battleSide::armiesNum);
		typeAll.battleSideTable.set("reinforceArmyCount", &battleSide::reinforceArmyCount);
		typeAll.battleSideTable.set("alliance", &battleSide::alliance);
		typeAll.battleSideTable.set("soldierCountStart", &battleSide::soldierCount);
		typeAll.battleSideTable.set("totalStrength", &battleSide::totalStrength);
		typeAll.battleSideTable.set("activeArmyStrength", &battleSide::activeArmyStrength);
		typeAll.battleSideTable.set("battleOdds", &battleSide::battleOdds);
		typeAll.battleSideTable.set("factionCount", &battleSide::factionCount);
		typeAll.battleSideTable.set("battleAIPlan", &battleSide::battleAIPlan);
		typeAll.battleSideTable.set("winConditions", sol::property([](battleSide& self) { return std::ref(self.winConditions); }));
		/***
		Get win condition string, for example: destroy\_or\_rout_enemy
		@function battleSide.getWinConditionString
		@tparam int condition
		@treturn string winCondition destroy\_or\_rout\_enemy, balance\_of\_strength\_percent, destroy\_enemy\_strength\_percent, capture\_location, destroy\_character, capture\_major\_settlement, capture\_army\_settlement, unknown\_condition
		@usage
		function onPostBattle(eventData)
			getBattleData()
		end

		function getBattleData()
			local thisBattle, battleList = M2TW.battle, "Function: getBattleData()"
			for i = 1, thisBattle.sidesNum, 1 do
				local thisSide = thisBattle.sides[i]
				battleList = battleList.."\n\tSide "..i.."\n\t\tisDefender: "..tostring(thisSide.isDefender).."\n\t\tisCanDeploy: "..tostring(thisSide.canDeploy).."\n\t\tarmiesNum: "..thisSide.armiesNum.."\n\t\twinConditions:"
				for j = 1, 4, 1 do
					local thisWinCond = thisSide.winConditions[j]
					if thisWinCond ~= 0 then
						battleList = battleList.."\n\t\t\t"..thisSide.getWinConditionString(thisWinCond)
					end
				end
				for a = 0, a < thisSide.battleArmyNum - 1 do
					local armyBattle = thisSide:getBattleArmy(a)
					if armyBattle then
						battleList = battleList.."\n\t\t\tArmy "..a.."\n\t\t\t\tFaction: "..armyBattle.army.faction.name
						if armyBattle.character then
							battleList = battleList.."\n\t\t\t\tLeader: "..armyBattle.character.characterRecord.fullName
						end
					end
				end
			end
			print(battleList)
		end
		*/
		typeAll.battleSideTable.set_function("getWinConditionString", &getWinConditionS);
		typeAll.battleSideTable.set("armiesNum", &battleSide::battleSideArmyNum);
		typeAll.battleSideTable.set("armies", sol::property([](battleSide& self) { return std::ref(self.armies); }));
		/***
		Get a battle army by it's index.
		@function battleSide:getBattleArmy
		@tparam int index
		@treturn battleArmy army
		@usage

			local battleArmy = side:getBattleArmy(0)

		*/
		typeAll.battleSideTable.set_function("getBattleArmy", &battleSide::getBattleArmy);
		/***
		Get a faction in this side by it's index.
		@function battleSide:getFaction
		@tparam int index
		@treturn factionStruct faction
		@usage

			local faction = side:getFaction(0)

		*/
		typeAll.battleSideTable.set_function("getFaction", &battleSide::getFaction);
		/***
		Get a reinforcement army in this side by it's index.
		@function battleSide:getReinforcementArmy
		@tparam int index
		@treturn armyStruct army
		@usage

			local army = side:getReinforcementArmy(0)

		*/
		typeAll.battleSideTable.set_function("getReinforcementArmy", &battleSide::getReinforcementArmy);;
		/***
		Check if a side contains a faction.
		@function battleSide:hasFaction
		@tparam int factionID
		@treturn bool containsFac
		@usage

			local containsFac = side:hasFaction(4)

		*/
		typeAll.battleSideTable.set_function("hasFaction", &battleSide::hasFaction);

		/***
		Basic battleSideArmy table

		@tfield armyStruct army
		@tfield bool isReinforcement
		@tfield bool canWithdraw
		@tfield int deployAreaCount
		@tfield getDeployArea getDeployArea

		@table battleSideArmy
		*/
		typeAll.battleSideArmyTable = luaState.new_usertype<battleSideArmy>("battleSideArmy");
		typeAll.battleSideArmyTable.set("army", &battleSideArmy::stack);
		typeAll.battleSideArmyTable.set("canWithdraw", &battleSideArmy::canWithdraw);
		typeAll.battleSideArmyTable.set("isReinforcement", &battleSideArmy::isReinforcement);
		typeAll.battleSideArmyTable.set("deployAreaCount", sol::property(&battleSideArmy::getDeployAreaCount));
		typeAll.battleSideArmyTable.set("deploymentArea", sol::property(&battleSideArmy::getMainDeployArea));
		
		/***
		Get a deployment area.
		@function battleSideArmy:getDeployArea
		@tparam int index
		@treturn deploymentAreaS area
		@usage
			local area = side:getDeployArea(0)
		*/
		typeAll.battleSideArmyTable.set_function("getDeployArea", &battleSideArmy::getDeployArea);

		/***
		Basic DeploymentAreaS table

		@tfield int coordsNum
		@tfield getCoordPair getCoordPair
		@tfield int width
		@tfield int height
		@tfield float centreX
		@tfield float centreY


		@table deploymentAreaS
		*/
		typeAll.deploymentAreaTable = luaState.new_usertype<deploymentAreaS>("deploymentAreaS");
		typeAll.deploymentAreaTable.set("coordsNum", &deploymentAreaS::coordsNum);
		typeAll.deploymentAreaTable.set("width", &deploymentAreaS::width);
		typeAll.deploymentAreaTable.set("height", &deploymentAreaS::height);
		typeAll.deploymentAreaTable.set("centreX", &deploymentAreaS::centreX);
		typeAll.deploymentAreaTable.set("centreY", &deploymentAreaS::centreY);
		
		/***
		Get pair of coords with index.
		@function deploymentAreaS:getCoordPair
		@tparam int index
		@treturn battlePos position
		@usage
		local side1 = M2TW.battle.sides[1];
		local deployArea1 = side1.armies[1]:getDeployArea(0);

		for i = 0, deployArea1.coordsNum-1 do
			local pos = side1.armies[1].deploymentArea:getCoordPair(i);
			print(pos.xCoord, pos.yCoord);
		end
		*/
		typeAll.deploymentAreaTable.set("getCoordPair", &deploymentAreaS::getCoordPair);
		
		/***
		Basic battlePos table

		@tfield float xCoord
		@tfield float yCoord


		@table battlePos
		*/
		typeAll.battlePos = luaState.new_usertype<battlePos>("battlePos");
		typeAll.battlePos.set("xCoord", &battlePos::xCoord);
		typeAll.battlePos.set("yCoord", &battlePos::yCoord);

		///Battle AI
		//@section Battle AI

		/***
		Basic Battle AI table

		@tfield int gtaPlan
			0 = "DO_NOTHING"
			1 = "ATTACK_ALL"
			2 = "DEFEND"
			3 = "DEFEND_FEATURE"
			4 = "HIDE"
			5 = "AMBUSH"
			6 = "SCOUT"
			7 = "WITHDRAW"
			8 = "ATTACK_SETTLEMENT"
			9 = "DEFEND_SETTLEMENT"
			10 = "SALLY_OUT"
		@tfield int previousAiPlan
		@tfield int aiAnalyzerPlan
		@tfield bool scriptPlan
		@tfield int unitCount
		@tfield int visibleEnemyUnitCount
		@tfield int addedObjectivesCount
		@tfield int objectiveCount
		@tfield getObjective getObjective
		@tfield getVisibleEnemy getVisibleEnemy
		@tfield getUnit getUnit
		@tfield int objectiveTimer
		@tfield battleSide battleSide
		@tfield float defendAreaX
		@tfield float defendAreaZ
		@tfield float defendAreaY
		@tfield float defendRadius
		@tfield bool isDefender
		@tfield bool isAmbush
		@tfield bool hasDeployed
		@tfield bool isDeploying
		@tfield bool wantsToSally
		@tfield bool defendAreaSet
		@tfield int battleDifficulty
		@tfield battleStrengthStruct alliedInitialStrength
		@tfield battleStrengthStruct alliedStrength
		@tfield battleStrengthStruct enemyInitialStrength
		@tfield battleStrengthStruct enemyStrength
		@tfield battleStrengthStruct enemyVisibleInitialStrength
		@tfield battleStrengthStruct enemyVisibleStrength
		@tfield terrainFeatureHill defendHill
		
		@table battleAI
		*/

		typeAll.battleAI = luaState.new_usertype<battleAI>("battleAI");
		typeAll.battleAI.set("gtaPlan", sol::property(&battleAI::getPlan, &battleAI::setPlan));
		typeAll.battleAI.set("previousAiPlan", &battleAI::previousAiPlan);
		typeAll.battleAI.set("aiAnalyzerPlan", &battleAI::aiAnalyzerPlan);
		typeAll.battleAI.set("unitCount", &battleAI::unitCount);
		typeAll.battleAI.set("isDefender", &battleAI::isDefender);
		typeAll.battleAI.set("isAmbush", &battleAI::isAmbush);
		typeAll.battleAI.set("isDeploying", &battleAI::isDeploying);
		typeAll.battleAI.set("hasDeployed", &battleAI::hasDeployed);
		typeAll.battleAI.set("scriptPlan", &battleAI::scriptPlan);
		typeAll.battleAI.set("wantsToSally", &battleAI::wantsToSally);
		typeAll.battleAI.set("battleDifficulty", &battleAI::battleDifficulty);
		typeAll.battleAI.set("battleSide", &battleAI::battleSide);
		typeAll.battleAI.set("defendAreaSet", &battleAI::defendAreaSet);
		typeAll.battleAI.set("objectiveTimer", &battleAI::objectiveTimer);
		typeAll.battleAI.set("defendAreaX", &battleAI::defendAreaX);
		typeAll.battleAI.set("defendAreaZ", &battleAI::defendAreaZ);
		typeAll.battleAI.set("defendAreaY", &battleAI::defendAreaY);
		typeAll.battleAI.set("defendRadius", &battleAI::defendRadius);
		typeAll.battleAI.set("defendHill", &battleAI::defendHill);
		typeAll.battleAI.set("visibleEnemyUnitCount", &battleAI::enemyUnitsNum);
		typeAll.battleAI.set("alliedInitialStrength", &battleAI::playerObjectivesCount);
		typeAll.battleAI.set("alliedStrength", &battleAI::alliedInitialTotal);
		typeAll.battleAI.set("enemyInitialStrength", &battleAI::alliedTotal);
		typeAll.battleAI.set("enemyStrength", &battleAI::enemyTotal);
		typeAll.battleAI.set("enemyVisibleInitialStrength", &battleAI::enemyInitialVisible);
		typeAll.battleAI.set("enemyVisibleStrength", &battleAI::enemyVisible);
		typeAll.battleAI.set("addedObjectivesCount", &battleAI::defendHill);
		typeAll.battleAI.set("objectiveCount", sol::property(&battleAI::getObjectiveCount));
		/***
		Get a battle objective by it's index.
		@function battleAI:getObjective
		@tparam int index
		@treturn battleObjective objective
		@usage

			local objective = battleAI:getObjective(0)

		*/
		typeAll.battleAI.set_function("getObjective", &battleAI::getObjective);
		
		/***
		Get a visible enemy unit by index.
		@function battleAI:getVisibleEnemy
		@tparam int index
		@treturn unit enemy
		@usage
			local unit = battleAI:getVisibleEnemy(0)
		*/
		typeAll.battleAI.set_function("getVisibleEnemy", &battleAI::getVisibleEnemy);
		
		/***
		Get a unit by index.
		@function battleAI:getUnit
		@tparam int index
		@treturn unit aiUnit
		@usage
			local unit = battleAI:getUnit(0)
		*/
		typeAll.battleAI.set_function("getUnit", &battleAI::getUnit);
		
		/***
		Basic lineData table

		@tfield float xCoordStart
		@tfield float yCoordStart
		@tfield float xCoordEnd
		@tfield float yCoordEnd
		@tfield float xMidPoint
		@tfield float yMidPoint
		@tfield float length

		@table lineData
		*/
		typeAll.lineData = luaState.new_usertype<lineData>("lineData");
		typeAll.lineData.set("xCoordStart", &lineData::xCoordStart);
		typeAll.lineData.set("yCoordStart", &lineData::yCoordStart);
		typeAll.lineData.set("xCoordEnd", &lineData::xCoordEnd);
		typeAll.lineData.set("yCoordEnd", &lineData::yCoordEnd);
		typeAll.lineData.set("xMidPoint", &lineData::xMidPoint);
		typeAll.lineData.set("yMidPoint", &lineData::yMidPoint);
		typeAll.lineData.set("length", &lineData::length);
		
		/***
		Basic battleStrengthStruct table

		@tfield int soldierCount
		@tfield int meleeStrength
		@tfield int missileStrength
		@tfield int siegeStrength
		@tfield int infantryMissilePlusMelee
		@tfield int cavalryMeleeStrength
		@tfield int total

		@table battleStrengthStruct
		*/
		typeAll.battleStrengthStruct = luaState.new_usertype<battleStrengthStruct>("battleStrengthStruct");
		typeAll.battleStrengthStruct.set("soldierCount", &battleStrengthStruct::soldierCount);
		typeAll.battleStrengthStruct.set("meleeStrength", &battleStrengthStruct::meleeStrength);
		typeAll.battleStrengthStruct.set("missileStrength", &battleStrengthStruct::missileStrength);
		typeAll.battleStrengthStruct.set("siegeStrength", &battleStrengthStruct::siegeStrength);
		typeAll.battleStrengthStruct.set("infantryMissilePlusMelee", &battleStrengthStruct::infantryMissilePlusMelee);
		typeAll.battleStrengthStruct.set("cavalryMeleeStrength", &battleStrengthStruct::cavalryMeleeStrength);
		typeAll.battleStrengthStruct.set("total", &battleStrengthStruct::total);

		/***
		Basic battleObjective table
		
		@tfield int priority
		@tfield battleAI battleAi
		@tfield int unitCount
		@tfield int ticks
		@tfield bool isRequired
		@tfield bool isPlayer
		@tfield getUnit getUnit
		@tfield getType getType

		@table battleObjective
		*/

		typeAll.battleObjective = luaState.new_usertype<aiBattleObjective>("battleObjective");
		typeAll.battleObjective.set("priority", &aiBattleObjective::priority);
		typeAll.battleObjective.set("battleAi", &aiBattleObjective::battleAi);
		typeAll.battleObjective.set("ticks", &aiBattleObjective::ticks);
		typeAll.battleObjective.set("isRequired", &aiBattleObjective::isRequired);
		typeAll.battleObjective.set("isPlayer", &aiBattleObjective::isPlayer);
		typeAll.battleObjective.set("unitCount", &aiBattleObjective::aiUnitsCount);
		
		/***
		Get a unit by it's index.
		@function battleObjective:getUnit
		@tparam int index
		@treturn unit unit
		@usage

			local unit = battleObjective:getUnit(0)

		*/
		typeAll.battleObjective.set_function("getUnit", &aiBattleObjective::getUnit);
		/***
		Get the type of objective.
		@function battleObjective:getType
		@treturn int objectiveType
		@usage

			local objective = battleObjective:getType()

		*/
		typeAll.battleObjective.set_function("getType", &aiBattleObjective::getType);

		///Battle Army
		//@section Battle Army

		/***
		Basic battleArmy table

		@tfield armyStruct army
		@tfield character character
		@tfield int generalNumKillsBattle
		@tfield float initialStrength
		@tfield float battleOdds
		@tfield float generalHPRatioLost
		@tfield int numKilledGenerals (manual battle only)
		@tfield int unitCount
		@tfield getBattleUnit getBattleUnit

		@table battleArmy
		*/
		typeAll.battleArmy = luaState.new_usertype<battleArmy>("battleArmy");
		typeAll.battleArmy.set("army", &battleArmy::army);
		typeAll.battleArmy.set("character", &battleArmy::character);
		typeAll.battleArmy.set("generalNumKillsBattle", &battleArmy::generalNumKillsBattle);
		typeAll.battleArmy.set("generalHPRatioLost", &battleArmy::generalHPRatioLost);
		typeAll.battleArmy.set("numKilledGenerals", &battleArmy::numKilledGenerals);
		typeAll.battleArmy.set("initialStrength", &battleArmy::initialStrength);
		typeAll.battleArmy.set("battleOdds", &battleArmy::battleOdds);
		typeAll.battleArmy.set("unitCount", &battleArmy::unitCount);
		/***
		Get a battle unit by it's index.
		@function battleArmy:getBattleUnit
		@tparam int index
		@treturn battleUnit unit
		@usage

			unit = battleArmy:getBattleUnit(0)

		*/
		typeAll.battleArmy.set_function("getBattleUnit", &battleArmy::getBattleUnit);
		
		/***
		Basic battleUnit table

		@tfield unit unit
		@tfield float valuePerSoldier
		@tfield int soldiersLost
		@tfield int soldiersStart
		@tfield int soldiersRouted
		@tfield int soldiersKilled
		@tfield int takenPrisoner
		@tfield int prisonersCaught
		@tfield int soldiersHealed
		@tfield int soldiersRallied
		@tfield int soldiersEnd
		@tfield int friendlyFireCasualties
		@tfield int expStart
		@tfield int expGained
		@tfield int shipsSunk
		@tfield int shipDamage
		@tfield int isGeneral
		@tfield int hasWithdrawn

		@table battleUnit
		*/
		typeAll.battleUnit = luaState.new_usertype<battleUnit>("battleUnit");
		typeAll.battleUnit.set("unit", &battleUnit::unit);
		typeAll.battleUnit.set("valuePerSoldier", &battleUnit::valuePerSoldier);
		typeAll.battleUnit.set("soldiersLost", &battleUnit::soldiersLost);
		typeAll.battleUnit.set("soldiersStart", &battleUnit::soldiersStart);
		typeAll.battleUnit.set("unitsRouted", &battleUnit::soldiersRouted);
		typeAll.battleUnit.set("soldiersRouted", &battleUnit::soldiersRouted);
		typeAll.battleUnit.set("soldiersKilled", &battleUnit::soldiersKilled);
		typeAll.battleUnit.set("takenPrisoner", &battleUnit::takenPrisoner);
		typeAll.battleUnit.set("prisonersCaught", &battleUnit::prisonersCaught);
		typeAll.battleUnit.set("soldiersHealed", &battleUnit::soldiersHealed);
		typeAll.battleUnit.set("soldiersRallied", &battleUnit::soldiersRallied);
		typeAll.battleUnit.set("soldiersEnd", &battleUnit::soldiersEnd);
		typeAll.battleUnit.set("friendlyFireCasualties", &battleUnit::friendlyFireCasualties);
		typeAll.battleUnit.set("expStart", &battleUnit::expStart);
		typeAll.battleUnit.set("expGained", &battleUnit::expGained);
		typeAll.battleUnit.set("isGeneral", &battleUnit::isGeneral);
		typeAll.battleUnit.set("hasWithdrawn", &battleUnit::isGeneral);
		typeAll.battleUnit.set("shipsSunk", &battleUnit::shipsSunk);
		typeAll.battleUnit.set("shipDamage", &battleUnit::shipDamage);

		///Battle Residence
		//@section Battle Residence

		/***
		Basic battleResidence table

		@tfield settlementStruct settlement
		@tfield battleBuildings battleBuildings
		@tfield factionStruct faction
		@tfield plazaData plazaData
		@tfield fortBattleInfo fortInfo
		@tfield int settlementWallsBreached
		@tfield int settlementGateDestroyed
		@tfield bool isUsed
		@tfield int streetPositionCount
		@tfield getStreetPosition getStreetPosition

		@table battleResidence
		*/
		typeAll.battleResidence = luaState.new_usertype<battleResidence>("battleResidence");
		typeAll.battleResidence.set("settlement", &battleResidence::settlement);
		typeAll.battleResidence.set("faction", &battleResidence::faction);
		typeAll.battleResidence.set("settlementWallsBreached", &battleResidence::settlementWallsBreached);
		typeAll.battleResidence.set("settlementGateDestroyed", &battleResidence::settlementGateDestroyed);
		typeAll.battleResidence.set("battleBuildings", &battleResidence::battleBuildings);
		typeAll.battleResidence.set("fortInfo", &battleResidence::fort);
		typeAll.battleResidence.set("plazaData", &battleResidence::plaza);
		typeAll.battleResidence.set("isUsed", &battleResidence::isUsed);
		typeAll.battleResidence.set("streetPositionCount", sol::property(&getStreetPositionCount));
		
		/***
		Get a street position in a settlement.
		@function battleResidence:getStreetPosition
		@tparam int index
		@treturn streetPosition pos
		@usage
			local pos = battleResidence:getStreetPosition(0)
		*/
		typeAll.battleResidence.set_function("getStreetPosition", &getStreetPosition);
		
		/***
		Basic battleBuildings table

		@tfield int buildingCount
		@tfield int perimeterCount
		@tfield getBuilding getBuilding
		@tfield getPerimeter getPerimeter

		@table battleBuildings
		*/
		typeAll.battleBuildings = luaState.new_usertype<battleBuildings>("battleBuildings");
		typeAll.battleBuildings.set("buildingCount", &battleBuildings::allBuildingsNum);
		typeAll.battleBuildings.set("perimeterCount", &battleBuildings::perimetersNum);

		/***
		Get a battle building by it's index.
		@function battleBuildings:getBuilding
		@tparam int index
		@treturn buildingBattle building
		@usage

			local building = battleBuildings:getBuilding(0)

		*/
		typeAll.battleBuildings.set_function("getBuilding", &battleBuildings::getBattleBuilding);

		/***
		Get a perimeter by it's index.
		@function battleBuildings:getPerimeter
		@tparam int index
		@treturn perimeterBuildings perimeter
		@usage

			local perimeter = battleBuildings:getPerimeter(0)

		*/
		typeAll.battleBuildings.set_function("getPerimeter", &battleBuildings::getPerimeter);

		/***
		Basic perimeterBuildings table

		@tfield int buildingCount
		@tfield getBuilding getBuilding

		@table perimeterBuildings
		*/
		typeAll.perimeterBuildings = luaState.new_usertype<perimeterBuildings>("perimeterBuildings");
		typeAll.perimeterBuildings.set("buildingCount", &perimeterBuildings::buildingNum);

		/***
		Get a battle building in a perimiter by it's index.
		@function perimeterBuildings:getBuilding
		@tparam int index
		@treturn buildingBattle building
		@usage

			local building = perimeterBuildings:getBuilding(0)

		*/
		typeAll.perimeterBuildings.set_function("getBuilding", &perimeterBuildings::getBuilding);

		
		/***
		Basic streetPosition table

		@tfield float xCoord
		@tfield float yCoord
		@tfield int perimeter
		@tfield bool available
		
		@table streetPosition
		*/
		typeAll.streetPosition = luaState.new_usertype<streetPosition>("streetPosition");
		typeAll.streetPosition.set("xCoord", &streetPosition::xCoord);
		typeAll.streetPosition.set("yCoord", &streetPosition::yCoord);
		typeAll.streetPosition.set("perimeter", &streetPosition::perimeter);
		typeAll.streetPosition.set("available", &streetPosition::available);
		
		
		/***
		Basic plazaData table

		@tfield float xCoord
		@tfield float yCoord
		@tfield float sizeX
		@tfield float sizeY
		@tfield int alliance
		@tfield bool capturing
		@tfield bool isCaptured
		@tfield float plazaMaxTime
		@tfield float plazaControlPerSecond
		@tfield float plazaControl
		@tfield getSoldierCount getSoldierCount

		@table plazaData
		*/
		typeAll.plazaData = luaState.new_usertype<plazaStuff>("plazaData");
		typeAll.plazaData.set("xCoord", &plazaStuff::plazaXCoord);
		typeAll.plazaData.set("yCoord", &plazaStuff::plazaYCoord);
		typeAll.plazaData.set("capturing", &plazaStuff::capturing);
		typeAll.plazaData.set("isCaptured", &plazaStuff::isCaptured);
		typeAll.plazaData.set("sizeX", &plazaStuff::sizeX);
		typeAll.plazaData.set("sizeY", &plazaStuff::sizeY);
		typeAll.plazaData.set("alliance", &plazaStuff::alliancePlaza);
		typeAll.plazaData.set("plazaMaxTime", &plazaStuff::plazaMaxTime);
		typeAll.plazaData.set("plazaControlPerSecond", &plazaStuff::plazaControlPerSecond);
		typeAll.plazaData.set("plazaControl", &plazaStuff::plazaTimer);
		
		/***
		Get amount of soldiers on plazaz for an alliance.
		@function plazaData:getSoldierCount
		@tparam int alliance
		@treturn int soldiers
		@usage
			local stats = plazaData:getSoldierCount(0)
		*/
		typeAll.plazaData.set_function("getSoldierCount", &plazaStuff::getSoldierCount);
		
		///Terrain Features
		//@section Terrain Features

		/***
		Basic terrainFeatures table

		@tfield float width
		@tfield float widthHalf
		@tfield float length
		@tfield float lengthHalf
		@tfield float widthOnePercent
		@tfield float lengthOnePercent
		@tfield int terrainLineCount
		@tfield int hillCount
		@tfield getHill getHill
		@tfield getLine getLine
		
		@table terrainFeatures
		*/
		typeAll.terrainFeatures = luaState.new_usertype<terrainFeatures>("terrainFeatures");
		typeAll.terrainFeatures.set("width", &terrainFeatures::width);
		typeAll.terrainFeatures.set("widthHalf", &terrainFeatures::widthHalf);
		typeAll.terrainFeatures.set("length", &terrainFeatures::height);
		typeAll.terrainFeatures.set("lengthHalf", &terrainFeatures::heightHalf);
		typeAll.terrainFeatures.set("widthOnePercent", &terrainFeatures::widthOnePercent);
		typeAll.terrainFeatures.set("lengthOnePercent", &terrainFeatures::heightOnePercent);
		typeAll.terrainFeatures.set("terrainLineCount", sol::property(&terrainFeatures::getLineNum));
		typeAll.terrainFeatures.set("hillCount", sol::property(&terrainFeatures::getHillNum));

		/***
		Get a terrain line by index.
		@function terrainFeatures:getLine
		@tparam int index
		@treturn terrainLine line
		@usage
			local line = terrainFeatures:getLine(0)
		*/
		typeAll.terrainFeatures.set_function("getLine", &terrainFeatures::getLine);

		/***
		Get a terrain hill by index.
		@function terrainFeatures:getHill
		@tparam int index
		@treturn terrainFeatureHill hill
		@usage
			local hill = terrainFeatures:getHill(0)
		*/
		typeAll.terrainFeatures.set_function("getHill", &terrainFeatures::getHill);
		
		/***
		Basic terrainLine table

		@tfield float startX
		@tfield float startZ
		@tfield float startY
		@tfield float endX
		@tfield float endZ
		@tfield float endY
		@tfield terrainLine previousSegment
		@tfield terrainLine nextSegment
		
		@table terrainLine
		*/
		typeAll.terrainLine = luaState.new_usertype<terrainLineSegment>("terrainLine");
		typeAll.terrainLine.set("startX", &terrainLineSegment::startX);
		typeAll.terrainLine.set("startZ", &terrainLineSegment::startZ);
		typeAll.terrainLine.set("startY", &terrainLineSegment::startY);
		typeAll.terrainLine.set("endX", &terrainLineSegment::endX);
		typeAll.terrainLine.set("endZ", &terrainLineSegment::endZ);
		typeAll.terrainLine.set("endY", &terrainLineSegment::endY);
		typeAll.terrainLine.set("previousSegment", &terrainLineSegment::previousSegment);
		typeAll.terrainLine.set("nextSegment", &terrainLineSegment::nextSegment);
		
		/***
		Basic terrainFeatureHill table

		@tfield float xCoord
		@tfield float zCoord
		@tfield float yCoord
		@tfield float radius
		@tfield terrainLine terrainLinesStart
		@tfield float area
		
		@table terrainFeatureHill
		*/
		typeAll.terrainFeatureHill = luaState.new_usertype<terrainFeatureHill>("terrainFeatureHill");
		typeAll.terrainFeatureHill.set("xCoord", &terrainFeatureHill::xCoord);
		typeAll.terrainFeatureHill.set("zCoord", &terrainFeatureHill::zCoord);
		typeAll.terrainFeatureHill.set("yCoord", &terrainFeatureHill::yCoord);
		typeAll.terrainFeatureHill.set("radius", &terrainFeatureHill::radius);
		typeAll.terrainFeatureHill.set("terrainLinesStart", &terrainFeatureHill::terrainLineSegmentStart);
		typeAll.terrainFeatureHill.set("area", &terrainFeatureHill::area);

		///Battle Building
		//@section Battle Building

		/***
		Basic buildingBattle table

		@tfield int type
		@tfield int currentHealth
		@tfield int startHealth
		@tfield int alliance
		@tfield int factionId
		@tfield factionStruct faction
		@tfield battleResidence battleResidence
		@tfield float posX
		@tfield float posZ
		@tfield float posY
		@tfield battleBuildingStats battleBuildingStats
		@tfield int index
		@tfield int perimeter
		@tfield bool breached
		@tfield bool dockable
		@tfield unit controllingUnit
		@tfield bool isBurning
		@tfield float fireDamage
		@tfield float fireLevel

		@table buildingBattle
		*/
		typeAll.buildingBattle = luaState.new_usertype<buildingBattle>("buildingBattle");
		typeAll.buildingBattle.set("type", &buildingBattle::type);
		typeAll.buildingBattle.set("factionId", &buildingBattle::factionId);
		typeAll.buildingBattle.set("posX", &buildingBattle::xCoord);
		typeAll.buildingBattle.set("posZ", &buildingBattle::zCoord);
		typeAll.buildingBattle.set("posY", &buildingBattle::yCoord);
		typeAll.buildingBattle.set("breached", &buildingBattle::breached);
		typeAll.buildingBattle.set("dockable", &buildingBattle::dockable);
		typeAll.buildingBattle.set("perimeter", &buildingBattle::perimeter);
		typeAll.buildingBattle.set("controllingUnit", &buildingBattle::controllingUnit);
		typeAll.buildingBattle.set("faction", &buildingBattle::faction);
		typeAll.buildingBattle.set("currentHealth", &buildingBattle::currentHealth);
		typeAll.buildingBattle.set("startHealth", &buildingBattle::startHealth);
		typeAll.buildingBattle.set("battleResidence", &buildingBattle::battleResidence);
		typeAll.buildingBattle.set("alliance", &buildingBattle::alliance);
		typeAll.buildingBattle.set("index", &buildingBattle::index);
		typeAll.buildingBattle.set("isBurning", &buildingBattle::isBurning);
		typeAll.buildingBattle.set("fireDamage", &buildingBattle::fireDamage);
		typeAll.buildingBattle.set("fireLevel", &buildingBattle::fireLevel);
		typeAll.buildingBattle.set("battleBuildingStats", &buildingBattle::towerStats);
		
		/***
		Basic battleBuildingStats table

		@tfield string name
		@tfield int flammability
		@tfield int impactDamage
		@tfield int health (descr_walls, not currently battle)
		@tfield float controlAreaRadius
		@tfield int manned
		@tfield int isSelectable
		@tfield int healthExcluded
		@tfield getTowerStats getTowerStats
		@tfield int towerStatsCount
		

		@table battleBuildingStats
		*/
		typeAll.battleBuildingStats = luaState.new_usertype<towerEntry>("battleBuildingStats");
		typeAll.battleBuildingStats.set("name", &towerEntry::name);
		typeAll.battleBuildingStats.set("flammability", &towerEntry::flammability);
		typeAll.battleBuildingStats.set("impactDamage", &towerEntry::impactDamage);
		typeAll.battleBuildingStats.set("health", &towerEntry::health);
		typeAll.battleBuildingStats.set("controlAreaRadius", &towerEntry::controlAreaRadius);
		typeAll.battleBuildingStats.set("manned", &towerEntry::manned);
		typeAll.battleBuildingStats.set("healthExcluded", &towerEntry::healthExcluded);
		typeAll.battleBuildingStats.set("isSelectable", &towerEntry::isSelectable);
		typeAll.battleBuildingStats.set("towerStatsCount", sol::property(&towerEntry::getTowerStatCount));

		/***
		Get tower stats by index.
		@function battleBuildingStats:getTowerStats
		@tparam int index
		@treturn towerStats stats
		@usage
			local stats = battleBuildingStats:getTowerStats(0)
		*/
		typeAll.battleBuildingStats.set_function("getTowerStats", &towerEntry::getTowerStat);
		
		/***
		Basic towerStats table

		@tfield attackStats stats
		@tfield float fireAngle
		@tfield float slotYawX
		@tfield float slotYawY
		@tfield float slotPitchX
		@tfield float slotPitchY
		@tfield int fireRate (don't set)
		@tfield int fireRateFlaming (don't set)
		
		@table towerStats
		*/
		typeAll.towerStats = luaState.new_usertype<towerStats>("towerStats");
		typeAll.towerStats.set("stats", &towerStats::stats);
		typeAll.towerStats.set("fireAngle", &towerStats::fireAngle);
		typeAll.towerStats.set("slotYawX", &towerStats::slotYawX);
		typeAll.towerStats.set("slotYawY", &towerStats::slotYawY);
		typeAll.towerStats.set("slotPitchX", &towerStats::slotPitchX);
		typeAll.towerStats.set("slotPitchY", &towerStats::slotPitchY);
		typeAll.towerStats.set("fireRate", sol::property(&towerStats::getFireRate, &towerStats::setFireRate));
		typeAll.towerStats.set("fireRateFlaming", sol::property(&towerStats::getFireRateFlaming, &towerStats::setFireRateFlaming));
		
		///Battle Tile
		//@section Battle Tile

		/***
		Basic battleTile table

		@tfield int physicalGroundType
		@tfield float height
		@tfield float waterHeight

		@table battleTile
		*/
		typeAll.battleTile = luaState.new_usertype<battleTile>("battleTile");
		typeAll.battleTile.set("physicalGroundType", sol::property(&battleTile::getGroundType));
		typeAll.battleTile.set("height", sol::property(&battleTile::getGroundHeight));
		typeAll.battleTile.set("waterHeight", sol::property(&battleTile::getWaterHeight));


		///Battle Camera
		//@section Battle Camera

		/***
		Get information about the camera in a battle
		@tfield float xCoord 
		@tfield float yCoord 
		@tfield float zCoord 
		@table battleCameraStruct
		*/
		typeAll.battleCameraStruct = luaState.new_usertype<battleCameraStruct>("battleCameraStruct");
		typeAll.battleCameraStruct.set("xCoord", &battleCameraStruct::xCoord);
		typeAll.battleCameraStruct.set("yCoord", &battleCameraStruct::yCoord);
		typeAll.battleCameraStruct.set("zCoord", &battleCameraStruct::zCoord);
    }
	



	
};
