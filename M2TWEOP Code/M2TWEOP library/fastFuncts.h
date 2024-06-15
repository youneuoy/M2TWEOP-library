#pragma once
#include"headersSTD.h"
#include "realGameTypes.h"
#include "settlement.h"

namespace fastFuncts
{
	factionStruct* GetCurrentFaction();
	std::string GetModPath();

	float GetMovepointsForReachNearTile(int x, int y, int destX, int destY);

	void revealTile(factionStruct* faction, int x, int y);
	void hideRevealedTile(factionStruct* faction, int x, int y);

	int8_t getTileVisibility(factionStruct* faction, int x, int y);
	void setTileVisibility(factionStruct* faction, int x, int y, int8_t vis);

	void GetGameTileCoordsWithCursor(int& x, int& y);
	void ViewTacticalMap(int x, int y);
	bool IsStratMap();

	UINT32 getTileRegionID(int x,int y);
	oneTile* getTileStruct(int x,int y);
	regionStruct* getRegionByID(UINT32 regionID);
	factionStruct* getRegionOwner(int regionID);
	void toggleDeveloperMode();

	//season(0-summer,1-winter)
	UINT32 getSeason();

	UINT32 getYear();
	//set character as heir
	void setHeir(characterRecord* gen, bool isJustSet);

	//count of factions in game
	UINT32 getFactionsCount();

	//array of pointers to factions
	factionStruct** getFactionsList();
	
	stackStruct* spawnArmy(
		factionStruct* faction,
		const char* name,
		const char* name2,
		int characterType,
		const char* label,
		const char* portrait,
		int x,
		int y,
		int age,
		bool family,
		int subFaction,
		int unitIndex,
		int exp,
		int wpn,
		int armour
		);

	//number of passed turns
	UINT32 getPassedTurnsNum();

	//-1, -1 if not recognized
	std::pair<int, int> getArmyCoords(stackStruct* army);
	//find fort on coords
	stackStruct* findArmy(int x, int y);
	//find fort on coords
	fortStruct* findFort(int x, int y);
	//find port on coords
	portBuildingStruct* findPort(int x, int y);
	//find resource on coords
	resStrat* findResource(int x, int y);
	//find settlement on coords
	settlementStruct* findSettlement(int x, int y);
	//find watchtower on coords
	watchTowerStruct* findWatchTower(int x, int y);
	coordPair* findValidTileNearTile(coordPair* coords, int charType);
	bool isTileValidForCharacterType(int charType, coordPair* coords);
	campaignDb* getCampaignDb();
	campaignDbExtra* getCampaignDbExtra();
	//move stratmap camera slow
	void moveStratCameraSlow(int x, int y);
	//move stratmap camera fast
	void moveStratCameraFast(int x, int y);
	//zoom stratmap camera
	void zoomStratCamera(float zoom);
	void setBodyguardStart(character* gen, unit* un);
	void loadSaveGame(const char* saveName);

	factionRecord* GetFactSmDescrById(int id);
	//find ancillary in anc list
	ancillary* findAncillary(const char* ancName);
	void NuullifyMovepoints(stackStruct* army);

	//kills
	stackStruct* createArmy(character* character);
	stackStruct* createArmyInSettlement(settlementStruct* sett);
	
	//returns true if we have siege before
	bool StopSiege(stackStruct* army);
	bool StopBlockPort(stackStruct* army);
	int addUnitToArmy(stackStruct* army, unit* un);

	void AddToSettlement(stackStruct*army,settlementStruct* set);
	void AddToFort(stackStruct*army,fortStruct* fort);


	void UngarisonSetOrFort(void* setOrFort);


	ModelDbEntry* findBattleModel(const char* modelName);

	//allocate mem for game class/etc
	DWORD allocateGameMem(size_t amount);
	
	void logFuncError(const std::string& funcName, const std::string& error);

	//attacker or defender
	bool autoWin(const char* winnerSide);


	void autoResolve();

	bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer);


	bool useButton(const char* buttonName);
	uiElement* getUiElement(const char* elementName);
	void useUiElement(uiElement* element);


	void mergeArmies(stackStruct* army, stackStruct* targetArmy);
	DWORD* getTileObject(const oneTile* tile, int type);

};

