#pragma once
#include <string>
#include "realGameTypes.h"
namespace battleHandlerHelpers
{
	std::string getWinConditionS(DWORD condition);
	armyAndCharacter* getBattleArmy(const battleSide* side, int index);
	battleUnit* getBattleUnit(const armyAndCharacter* battleArmy, int index);
	stackStruct* getPlayerArmy(const battleDataS* battleData, int index);
	stackStruct* getReinforcementArmy(const battleSide* side, int index);
	factionStruct* getFaction(const battleSide* side, int index);
	battleResidence* getBattleResidence();
	battlefieldEngines* getBattlefieldEngines();
	siegeEngine* getSiegeEngine(const battlefieldEngines* engineDb, const int index);
	battleDataS* getBattleData();
	bool inBattle();
	battlePos* getCoordPair(const deploymentAreaS* deployArea, const int index);
	int getEngineType(const siegeEngine* engine);
	AIBattleObjectiveBase* getObjective(const battleAI* battleAi, int index);
	int getObjectiveType(AIBattleObjectiveBase* objective);
	unit* getUnit(const AIBattleObjectiveBase* objective, int index);
	buildingBattle* getBattleBuilding(const battleBuildings* buildings, int index);
	buildingBattle* getPerimeterBuilding(const perimeterBuildings* buildings, int index);
	int getBattleTileIndex(float xCoord, float yCoord);
	int getTowerStatCount(towerEntry* entry);
	towerStats* getTowerStats(towerEntry* entry, int index);
	battleTerrainData* getBattleTerrainData();
	battleTile* getBattleTile(float xCoord, float yCoord);
	int getGroundType(const battleTile* tile);
	float getGroundHeight(const battleTile* tile);
	float getWaterHeight(const battleTile* tile);
	int getStreetNum(const battleStreets* streets);
	perimeterBuildings* getPerimeter(const battleBuildings* battleBuildings, int index);
	battleStreets* getBattleStreets();
	int getZoneID(float x, float y);
	int getZonePerimeter(float x, float y);
	bool isZoneValid(int zoneID);
	roadNode* getStreetNode(const battleStreets* streets, const int index);
	terrainFeatureHill* getHill(const hillVector* hills, const int index);
	terrainLineSegment* getTerrainLine(const terrainSegmentVector* segments, const int index);
};

