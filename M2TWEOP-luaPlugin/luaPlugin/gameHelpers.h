#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include <vector>

#include "lua/sol.hpp"
namespace gameHelpers
{
	UINT32 getFactionsCount();
	std::string callConsole(std::string cmdName, sol::variadic_args va);
	factionStruct* getFaction(int index);
	guild* getGuild(unsigned char index);
	unit* getEventAttackingUnit(eventTrigger* eventData);
	unit* getEventDefendingUnit(eventTrigger* eventData);
	general* getEventCharacter(eventTrigger* eventData);
	namedCharacter* getEventNamedCharacter(eventTrigger* eventData);
	namedCharacter* getEventNamedCharacter2(eventTrigger* eventData);
	namedCharacter* getEventTargetNamedCharacter(eventTrigger* eventData);
	settlementStruct* getEventSettlement(eventTrigger* eventData);
	settlementStruct* getEventTargetSettlement(eventTrigger* eventData);
	fortStruct* getEventFort(eventTrigger* eventData);
	factionStruct* getEventFaction(eventTrigger* eventData);
	factionStruct* getEventTargetFaction(eventTrigger* eventData);
	stackStruct* getEventArmy(eventTrigger* eventData);
	int getEventRegion(eventTrigger* eventData);
	int getEventTargetRegion(eventTrigger* eventData);
	unit* getEventPlayerUnit(eventTrigger* eventData);
	unit* getEventEnemyUnit(eventTrigger* eventData);
	buildingBattle* getBuildingBattle(eventTrigger* eventData);
	buildingInQueue* getPriorBuild(eventTrigger* eventData);
	const char* getResourceDescription(eventTrigger* eventData);
	int getReligionCount();
	int getReligionN(const char* name);
	int getEventCharacterType(eventTrigger* eventData);
	int getEventTargetCharacterType(eventTrigger* eventData);
	const char* getEventType(eventTrigger* eventData);
	const char* getMissionSuccessLevel(eventTrigger* eventData);
	int getMissionProbability(eventTrigger* eventData);
	const char* getMissionDetails(eventTrigger* eventData);
	int getEventID(eventTrigger* eventData);
	guild* getEventGuild(eventTrigger* eventData);
	const char* getEventCounter(eventTrigger* eventData);
	coordPair* getPosition(eventTrigger* eventData);
	int getReligion(eventTrigger* eventData);
	int getTargetReligion(eventTrigger* eventData);
	float getAmount(eventTrigger* eventData);
	crusade* getCrusade(eventTrigger* eventData);
	capturedFactionInfo* getCapturedFactionInfo(eventTrigger* eventData);
	const char* getRansomType(eventTrigger* eventData);
	unit* getUnit(eventTrigger* eventData);
	bool condition(std::string condition, const eventTrigger* eventData);
	eduEntry* getEduEntry(eventTrigger* eventData);


	general* createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y);
	stackStruct* createArmy(general* character);
	stackStruct* createArmyInSettlement(settlementStruct* sett);

	std::tuple<bool, int> getScriptCounter(const char* type);
	void setScriptCounter(const char* type, int value);
	regionStruct* getRegion(int index);
	oneTile* getTile(int x, int y);
	stackStruct* getStack(const regionStruct* region, int index);
	fortStruct* getFort(const regionStruct* region, int index);
	watchTowerStruct* getWatchtower(const regionStruct* region, int index);
	resStrat* getResource(const regionStruct* region, int index);
	neighbourRegion* getNeighbour(regionStruct* region, const int index);
	oneTile* getBorderTile(const neighbourRegion* region, const int index);
	bool getHiddenResource(regionStruct* region, int index);
	void setHiddenResource(regionStruct* region, int index, bool enable);
	void changeRegionName(regionStruct* region, const char* newName);
	std::string getRegionName(regionStruct* region);
	void changeRebelsName(regionStruct* region, const char* newName);
	general* getCardinal(const collegeOfCardinals* college, const int index);
	fortStruct* getFortAll(const campaign* campaign, const int index);
	portBuildingStruct* getPortAll(const campaign* campaign, const int index);
	watchTowerStruct* getWatchTowerAll(const campaign* campaign, const int index);
	settlementStruct* getSettlementByName(campaign* campaign, const char* name);
	std::string getRebelsName(regionStruct* region);
	coordPair* getVolcanoCoords(const stratMap* map, const int index);
	coordPair* getLandConnection(const stratMap* map, const int index);
	landMass* getLandMass(stratMap* map, const int index);
	int getRegionID(const landMass* landMass, const int index);
	coordPair* getRoadCoord(const roadStruct* road, const int index);
	int getTileFactionID(const oneTile* tile);
	bool tileHasRiver(const oneTile* tile);
	bool tileHasCrossing(const oneTile* tile);
	int tileBorderType(const oneTile* tile);
	bool tileHasCharacter(const oneTile* tile);
	bool tileHasShip(const oneTile* tile);
	bool tileHasWatchtower(const oneTile* tile);
	bool tileHasPort(const oneTile* tile);
	bool tileHasFort(const oneTile* tile);
	bool tileHasSettlement(const oneTile* tile);
	bool factionHasArmyNearTile(const oneTile* tile, int factionID);
	bool factionHasCharacterOnTile(const oneTile* tile, int factionID);
	int getHostileArmiesStrength(const regionStruct* region, const int factionID);
	coordPair* convertTileCoords(DWORD arrayIndex);
	coordPair* getTileCoords(const oneTile* tile);
	seaConnectedRegion* getSeaConnectedRegion(const regionStruct* region, const int index);
	seaConnectedRegion* getSeaImportRegion(const regionStruct* region, const int index);
	oneTile* getRegionSeaEdge(const regionStruct* region, const int index);
	oneTile* getDevastatedTile(const regionStruct* region, const int index);
	float getReligionHistory(const regionStruct* region, const int religionID, int turnsAgo);
	oneTile* getTileBorderingEdgeOfMap(const regionStruct* region, const int index);
	oneTile* getTileRegion(const regionStruct* region, const int index);
	oneTile* getFertileTile(const regionStruct* region, const int index);
	bool hasResourceType(const regionStruct* region, const int resourceType);
	oneTile* getReachableTile(const seaConnectedRegion* region, int index);
	coordPair* getTradeLaneCoord(const seaConnectedRegion* region, int index);
	bool isDevastated(const oneTile* tile);
	oneTileDouble* tileToDoubleTile(const oneTile* tile);
	float getTileHeight(const oneTile* tile);
	int getTileClimate(const oneTile* tile);
	int getTileHeatValue(const oneTile* tile);
	DWORD* getTileObject(const oneTile* tile, int type);
	resStrat* getTileResource(const oneTile* tile);
	general* getTileCharacter(const oneTile* tile);
	settlementStruct* getTileSettlement(const oneTile* tile);
	fortStruct* getTileFort(const oneTile* tile);
	portBuildingStruct* getTilePort(const oneTile* tile);
	watchTowerStruct* getTileWatchtower(const oneTile* tile);

	int getMercUnitNum(mercPool* mercPool);
	mercPoolUnit* getMercUnit(mercPool* pool, int index);
	void setMercReligion(mercPoolUnit* unit, int religion, bool set);
	mercPoolUnit* addMercUnit(mercPool* mercPool, int idx, int exp, int cost, float repmin, float repmax, int maxunits, float startpool, float startyear, float endyear, int crusading);
	void saveGame(const char* path);
	void historicEvent(const char* name, const char* title, const char* description);
	void scriptCommand(std::string command, const char* args);
	const char* getReligionName(const int index);
	unit* getSelectedUnitCard(const uiCardManager* cardManager, const int index);
	unit* getUnitCard(const uiCardManager* cardManager, const int index);
};

