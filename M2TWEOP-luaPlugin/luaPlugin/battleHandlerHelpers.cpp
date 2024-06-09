#include "battleHandlerHelpers.h"

#include "gameDataAllHelper.h"
#include "plugData.h"
namespace battleHandlerHelpers
{
	std::string getWinConditionS(DWORD condition)
	{
		int condCode = (*(*plugData::data.funcsBattle.getBattleCondCode))(condition);

		switch (condCode)
		{
		case 0: return "destroy_or_rout_enemy"; break;
		case 1: return "balance_of_strength_percent"; break;
		case 2: return "destroy_enemy_strength_percent"; break;
		case 3: return "capture_location"; break;
		case 4: return "destroy_character"; break;
		case 5: return "capture_major_settlement"; break;
		case 6: return "capture_army_settlement"; break;
		default: return "unknown_condition"; break;
		}
	}

	armyAndCharacter* getBattleArmy(const battleSide* side, int index)
	{
		return &side->forces[index];
	}

	battleUnit* getBattleUnit(const armyAndCharacter* battleArmy, int index)
	{
		return &battleArmy->units[index];
	}

	stackStruct* getPlayerArmy(const battleDataS* battleData, int index)
	{
		return battleData->playerArmies[index].army;
	}

	int getBattleTileIndex(float xCoord, float yCoord)
	{
		return 1000 * (int((xCoord + 1000.0) - 0.5) >> 1) + (int((yCoord + 1000.0) - 0.5) >> 1);
	}

	int getTowerStatCount(towerEntry* entry)
	{
		return (entry->someListEnd - reinterpret_cast<DWORD>(entry->towerStats)) / sizeof(towerStats);
	}

	towerStats* getTowerStats(towerEntry* entry, int index)
	{
		return &entry->towerStats[index];
	}

	battleTerrainData* getBattleTerrainData()
	{
		return gameDataAllHelper::get()->battleTerrainDataPtr;
	}

	battleTile* getBattleTile(float xCoord, float yCoord)
	{
		const auto terrainData = getBattleTerrainData();
		if (!terrainData) return nullptr;
		const auto index = getBattleTileIndex(xCoord, yCoord);
		if (index < 0 || index >= 1000000) return nullptr;
		return &terrainData->battleTiles[index];
	}

	int getGroundType(const battleTile* tile)
	{
		if (!tile) return -1;
		return 0b11111111 & (tile->physicalGroundType >> 4);
	}

	float getGroundHeight(const battleTile* tile)
	{
		if (!tile) return -1;
		const auto terrainData = getBattleTerrainData();
		if (!terrainData) return 0;
		return (tile->height * 0.1f) + terrainData->heightOffset;
	}

	float getWaterHeight(const battleTile* tile)
	{
		if (!tile) return -1;
		const auto terrainData = getBattleTerrainData();
		if (!terrainData) return 0;
		return tile->waterHeight * 0.1 + terrainData->heightOffset;
	}

	terrainFeatureHill* getHill(const hillVector* hills, const int index)
	{
		if (index < 0 || index >= hills->hillsNum) return nullptr;
		return &hills->hills[index];
	}

	int getStreetNum(const battleStreets* streets)
	{
		if (!streets)
			return 0;
		const int end = reinterpret_cast<DWORD>(streets->streetsEnd);
		const int start = reinterpret_cast<DWORD>(streets->streets);
		return (end - start) / sizeof(roadNode);
	}

	perimeterBuildings* getPerimeter(const battleBuildings* battleBuildings, int index)
	{
		return &battleBuildings->perimeters[index];
	}

	roadNode* getStreetNode(const battleStreets* streets, const int index)
	{
		if (!streets)
			return nullptr;
		return &streets->streets[index];
	}

	battlePerimeters* getBattlePerimeters()
	{
		auto perimeters = reinterpret_cast<battlePerimeters*>(0x016A7428);
		if (m2tweopHelpers::getGameVersion() == 1)
			perimeters = reinterpret_cast<battlePerimeters*>(0x16F0600);
		return perimeters;
	}

	battleStreets* getBattleStreets()
	{
		battlePerimeters* perimeters = getBattlePerimeters();
		if (!perimeters) return nullptr;
		return &perimeters->battleStreets;
	}

	bool isZoneValid(int zoneID)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters) return false;
		DWORD funcAddr = 0x6733A0;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcAddr = 0x672EC0;
		return GAME_FUNC_RAW(bool(__thiscall*)(battlePerimeters*, int), funcAddr)(perimeters, zoneID);
	}

	int getZonePerimeter(float x, float y)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters) return -1;
		battlePos pos{x,y};
		const auto posPtr = &pos;
		DWORD funcAddr = 0xE08290;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcAddr = 0xE0DC00;
		return GAME_FUNC_RAW(int(__thiscall*)(battlePerimeters*, battlePos*), funcAddr)(perimeters, posPtr);
	}

	int getZoneID(float x, float y)
	{
		const auto perimeters = getBattlePerimeters();
		if (!perimeters) return -1;
		const DWORD offset = reinterpret_cast<DWORD>(perimeters) + 0x48;
		DWORD getZoneAddr = 0xDF0680;
		if (m2tweopHelpers::getGameVersion() == 1)
			getZoneAddr = 0xDF6050;
		battlePos pos{x,y};
		const auto posPtr = &pos;
		return GAME_FUNC_RAW(int(__thiscall*)(DWORD, battlePos*), getZoneAddr)(offset, posPtr);
	}

	terrainLineSegment* getTerrainLine(const terrainSegmentVector* segments, const int index)
	{
		if (index < 0 || index >= segments->lineSegmentsNum) return nullptr;
		return &segments->lineSegments[index];
	}

	stackStruct* getReinforcementArmy(const battleSide* side, int index)
	{
		if (index >= side->reinforceArmyCount)
			return nullptr;
		return side->reinforcementArmies[index].army;
	}

	factionStruct* getFaction(const battleSide* side, int index)
	{
		return side->factions[index];
	}

	AIBattleObjectiveBase* getObjective(const battleAI* battleAi, int index)
	{
		if (!battleAi->addedObjectives) return nullptr;
		if ( (index < 0) || (index >= battleAi->addedObjectivesCount)) return nullptr;
		return battleAi->addedObjectives[index];
	}

	unit* getUnit(const AIBattleObjectiveBase* objective, int index)
	{
		if (!objective->aiUnits) return nullptr;
		if ( (index < 0) || (index >= objective->aiUnitsCount)) return nullptr;
		return objective->aiUnits[index];
	}

	int getObjectiveType(AIBattleObjectiveBase* objective)
	{
		if (!objective) return -1;
		return CallVFunc<7, int>(objective);
	}

	battleResidence* getBattleResidence()
	{
		const auto gameData = gameDataAllHelper::get();
		const auto battleSettlement = gameData->battleSettlement;
		if (battleSettlement)
		{
			return battleSettlement->battleResidence;
		}
		return nullptr;
	}
	
	battleDataS* getBattleData()
	{
		return gameDataAllHelper::get()->battleHandler;
	}
	
	bool inBattle()
	{
		const auto battle = getBattleData();
		if (!battle) return false;
		return battle->inBattle == 1;
	}

	battlePos* getCoordPair(const deploymentAreaS* deployArea, const int index)
	{
		return &deployArea->coordsPairs[index];
	}

	battlefieldEngines* getBattlefieldEngines()
	{
		auto enginesDbPtr = reinterpret_cast<battlefieldEngines**>(0x02C3A254);
		if (m2tweopHelpers::getGameVersion() == 1)
			enginesDbPtr = reinterpret_cast<battlefieldEngines**>(0x02C8329C);
		if (!enginesDbPtr) return nullptr;
		return *enginesDbPtr;
	}

	siegeEngine* getSiegeEngine(const battlefieldEngines* engineDb, const int index)
	{
		if (!engineDb || index < 0 || index >= engineDb->enginesNum) return nullptr;
		return engineDb->engines[index];
	}

	int getEngineType(const siegeEngine* engine)
	{
		if (!engine) return -1;
		const auto engineRec = engine->engineRecord;
		if (!engineRec) return -1;
		return engineRec->classID;
	}

	buildingBattle* getBattleBuilding(const battleBuildings* buildings, int index)
	{
		return buildings->allBuildings[index];
	}

	buildingBattle* getPerimeterBuilding(const perimeterBuildings* buildings, int index)
	{
		return buildings->buildingList[index];
	}
};
