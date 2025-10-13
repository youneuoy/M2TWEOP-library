#pragma once
#include "eopBuildings.h"
#include "realGameTypes.h"
#include "lua/sol.hpp"

struct regionStrengths;
struct oneTile;
struct portBuildingStruct;

enum class strategyObject :int
{
	character = 28,
	settlement = 29,
	fort = 30,
	port = 31,
	watchtower = 32,
	sundry = 33,
	rallyPointSundry = 34,
	floatingGeneral = 35,
	battleSiteMarker = 36,
};

inline bool operator ==(int a, strategyObject b)
{
	return static_cast<strategyObject>(a) == b;
}

inline bool operator !=(int a, strategyObject b)
{
	return static_cast<strategyObject>(a) != b;
}

enum class strategyGroundType
{
	lowFertility = 0,
	mediumFertility = 1,
	highFertility = 2,
	wilderness = 3,
	highMountains = 4,
	lowMountains = 5,
	hills = 6,
	denseForest = 7,
	woodland = 8,
	swamp = 9,
	ocean = 10,
	deepSea = 11,
	shallowSea = 12,
	coast = 13,
	impassableLand = 14,
	impassableSea = 15,
};

inline bool operator ==(int a, strategyGroundType b)
{
	return static_cast<strategyGroundType>(a) == b;
}

inline bool operator !=(int a, strategyGroundType b)
{
	return static_cast<strategyGroundType>(a) != b;
}

inline bool operator >(int a, strategyGroundType b)
{
	return static_cast<strategyGroundType>(a) > b;
}

inline bool operator <(int a, strategyGroundType b)
{
	return static_cast<strategyGroundType>(a) < b;
}

inline bool operator >=(int a, strategyGroundType b)
{
	return static_cast<strategyGroundType>(a) >= b;
}

inline bool operator <=(int a, strategyGroundType b)
{
	return static_cast<strategyGroundType>(a) <= b;
}

struct landingTile
{
public:
    uint32_t tileId; //0x0000
    float moveCost; //0x0004
	oneTile* getTile();
}; //Size: 0x0008

struct customTile {
	char* name;
	int nameCrypt;
	int x;
	int y;
	char** pathToModel_UNICODE;
	char* weather;
	int weatherCrypt;
	int timeCode;
};

struct customTilesDb
{
public:
	DWORD* vTable; //0x0000
	struct customTile customTiles[100]; //0x0004
	int32_t numberOfTiles; //0x0C84
	int32_t hashTableSize; //0x0C88
	void *lookupTable; //0x0C8C
	char pad_0C90[820]; //0x0C90
	int32_t field_0FC4; //0x0FC4
	void *field_0FC8; //0x0FC8
	char pad_0FCC[24]; //0x0FCC
	int32_t field_0FE4; //0x0FE4
}; //Size: 0x0FE8

struct seaConnectedRegion
{
public:
    int32_t regionID; //0x0000
    int32_t seaRegionID; //0x0004
    float moveCostBestLandTile; //0x0008
    float moveCostPort; //0x000C
    int32_t seaExportValue; //0x0010
    int32_t seaImportValue; //0x0014
    struct landingTile* tilesReachable; //0x0018
    int32_t tilesReachableSize; //0x001C
    int32_t tilesReachableCount; //0x0020
    struct coordPair* seaTradeLanePath; //0x0024
    int32_t seaTradeLanePathSize; //0x0028
    int32_t seaTradeLanePathCount; //0x002C
    void* aerialMap; //0x0030
public:
    landingTile* getReachableTile(const int index)
    {
        if (index < 0 || index >= tilesReachableCount)
            return nullptr;
        return &tilesReachable[index];
    }
    coordPair* getTradeLanePath(const int index)
    {
        if (index < 0 || index >= seaTradeLanePathCount)
            return nullptr;
        return &seaTradeLanePath[index];
    }
}; //Size: 0x0034

struct landingPoint
{
    int tileIndex;
    float moveCost;
    int seaTileIndex;
public:
	struct oneTile* getTile();
	struct oneTile* getSeaTile();
};

struct legioString
{
public:
    char* name; //0x0000
    int32_t nameHash; //0x0004
    struct UNICODE_STRING** localizedLegoName; //0x0008
    int32_t recruitCount; //0x000C
}; //Size: 0x0014

struct landMass
{
public:
    int32_t index; //0x0000
    int32_t* regions; //0x0004
    int32_t regionsSize; //0x0008
    int32_t regionsNum; //0x000C
public:
    int getRegion(const int i)
    {
        if (i < 0 || i >= regionsNum)
            return -1;
        return regions[i];
    }
}; //Size: 0x0010

struct roadStruct
{
public:
    struct coordPair* coords; //0x0000
    int32_t coordsSize; //0x0004
    int32_t coordsNum; //0x0008
    struct coordPair* destroyedCoords; //0x0000
    int32_t destroyedCoordsSize; //0x0004
    int32_t destroyedCoordsNum; //0x0008
    int32_t regionIdStart; //0x0018
    int32_t regionIdEnd; //0x001C
    int32_t tradeValue; //0x0020
    bool onlyHalf; //0x0024
    char pad_0025[3]; //0x0025
    int id; //0x0028
    void* aerialMap; //0x002C
public:
    coordPair* getCoord(const int index)
    {
        if (index < 0 || index >= coordsNum)
            return nullptr;
        return &coords[index];
    }
}; //Size: 0x0038

struct pathFindingNode
{
	float f;
	float h;
	float g;
	int tileIndex;
	int xCoord;
	int yCoord;
	pathFindingNode* parent;
	pathFindingNode* children[16];
};

struct stratMovePath
{
	std::vector<coordPair> tiles{};
	float totalCost{};
	int getTileCount()
	{
		return static_cast<int>(tiles.size());
	}
	coordPair* getTile(const int index)
	{
		if (index < 0 || index >= getTileCount())
			return nullptr;
		return &tiles[index];
	}
};



struct neighbourRegion
{
public:
	struct regionStruct* region; //0x0000
	int32_t regionID; //0x0004
	float moveCost; //0x0008
	bool isBlocked; //0x000C
	char pad_000D[3]; //0x000D
	struct roadStruct* connectingRoad; //0x0010
	int32_t tradeValue; //0x0014
	int32_t* borderTiles; //0x0018
	int32_t borderTilesSize; //0x001C
	int32_t borderTilesCount; //0x0020
	int32_t* frontierTiles; //0x0018
	int32_t frontierTilesSize; //0x001C
	int32_t frontierTilesCount; //0x0020
	int32_t* ambushTiles; //0x0018
	int32_t ambushTilesSize; //0x001C
	int32_t ambushTilesCount; //0x0020
	int32_t* deepFrontierTiles; //0x0018
	int32_t deepFrontierTilesSize; //0x001C
	int32_t deepFrontierTilesCount; //0x0020
	int32_t* passableTiles; //0x0018
	int32_t passableTilesSize; //0x001C
	int32_t passableTilesCount; //0x0020
	bool isSeaFord;
	char pad_0025[3]; //0x0025
public:
	oneTile* getBorderTile(int index);
	oneTile* getFrontierTile(int index);
	oneTile* getAmbushTile(int index);
	oneTile* getDeepFrontierTile(int index);
	oneTile* getBorderCrossing(int index);
}; //Size: 0x0058


struct climate
{
public:
	struct rcString* rcString; //0x0000
	struct UNICODE_STRING*** localizedName; //0x0004
	uint8_t colorBlue; //0x0008
	uint8_t colorGreen; //0x0009
	uint8_t colorRed; //0x000A
	char pad_000B[1]; //0x000B
	int32_t heatValue; //0x000C
	bool hasWinter; //0x0010
	char pad_0011[3]; //0x0011
	void* treeModelsSummerSparse; //0x0014
	int32_t treeModelsSummerSparseSize; //0x0018
	int32_t treeModelsSummerSparseNum; //0x001C
	void* treeModelsSummerDense; //0x0020
	int32_t treeModelsSummerDenseSize; //0x0024
	int32_t treeModelsSummerDenseNum; //0x0028
	void* treeModelsSummerCanopy; //0x002C
	int32_t treeModelsSummerCanopySize; //0x0030
	int32_t treeModelsSummerCanopyNum; //0x0034
	void* treeModelsWinterSparse; //0x0038
	int32_t treeModelsWinterSparseSize; //0x003C
	int32_t treeModelsWinterSparseNum; //0x0040
	void* treeModelsWinterDense; //0x0044
	int32_t treeModelsWinterDenseSize; //0x0048
	int32_t treeModelsWinterDenseNum; //0x004C
	void* treeModelsWinterCanopy; //0x0050
	int32_t treeModelsWinterCanopySize; //0x0054
	int32_t treeModelsWinterCanopyNum; //0x0058
}; //Size: 0x005C

struct climates
{
public:
	bool isOpen;
	char pad[3];
	struct climate* climateArray; //0x0004
	int32_t climatesSize; //0x0008
	int32_t climatesNum; //0x000C
	void* stringTable;
}; //Size: 0x0010

struct oneTileDouble
{
public:
	float height; //0x0000
	int8_t climate; //0x0004
	int8_t groundType; //0x0005
	uint16_t fixed : 1;
	uint16_t cultivated	: 2;
	uint16_t pad : 13;
}; //Size: 0x0008

struct coastLine
{
	int seaRegionID;
	int* regions;
	int regionsSize;
	int regionsNum;
};

struct mapRegion
{
	char* name; //0x0000
	int nameHash; //0x0004
	uint8_t blue; //0x0008
	uint8_t green; //0x0009
	uint8_t red; //0x000A
	uint8_t pad; //0x000B
	UNICODE_STRING*** regionName; //0x000C
	char* legio; //0x0010
	int legioHash; //0x0014
	char* settlementName; //0x0018
	int settlementNameHash; //0x001C
	UNICODE_STRING*** localSettlementName; //0x0020
	char* rebelName; //0x0024
	int rebelNameHash; //0x0028
	int factionId; //0x002C
	uint32_t padResMask; //0x0030
	uint32_t resourceMask; //0x0034
	uint32_t padHResMask; //0x0038
	uint32_t hiddenResourceMask1; //0x003C
	uint32_t hiddenResourceMask2; //0x0040
	uint32_t padHResMask2; //0x0044
	int triumph; //0x0048
	int baseFarm; //0x004C
	float* religions; //0x0050
};

struct gameStratRect
{
	int minX;
	int minY;
	int maxX;
	int maxY;
};

struct regionStruct {
	char* regionName;//0x0000
	int regionNameHash;//0x0004
	char* settlementName; /* null for sea *///0x0008
	int settlementNameHash;//0x000C
	char* legioName; /* null for sea *///0x0010
	int legioNameHash;//0x0014
	struct legioString* legioPointer; //0x0018
	int regionID;//0x001C
	int32_t roadLevel; //0x0020
	int32_t farmingLevel; //0x0024
	int32_t famineThreat; //0x0028
	struct armyStruct** armies; //0x002C
	int32_t armyCountArraySize; //0x0030
	int32_t stacksNum; //0x0034
	struct fortStruct** forts; //0x0038
	int32_t fortCountArraySize; //0x003C
	int32_t fortsNum; //0x0040
	struct watchTowerStruct** watchtowers; //0x0044
	int32_t watchtowerCountArraySize; //0x0048
	int32_t watchtowersNum; //0x004C
	bool isSea; //0x0050
	bool peninsular; //0x0051
	char pad_0052[2]; //0x0052
	struct landMass* landMass; //0x0054
	struct roadStruct* roadToPort; //0x0054
	struct seaConnectedRegion* seaConnectedRegions; //0x005C
	int32_t seaConnectedRegionsCountArraySize; //0x0060
	int32_t seaConnectedRegionsCount; //0x0064
	int32_t loyaltyFactionID; //0x0068
	struct seaConnectedRegion* seaExportRegions[4]; //0x006C
	struct seaConnectedRegion** seaImportRegions; //0x007C
	int32_t seaImportRegionsSize; //0x0080
	int32_t seaImportRegionsCount; //0x0084
	int32_t tradeFleetCount; //0x0088
	struct mercPool* mercPool;//0x008C
	int32_t* patrolPoints; //0x0090
	int32_t patrolPointsSize; //0x0094
	int32_t patrolPointsCount; //0x0098
	landingPoint* landingPoints; //0x009C
	int32_t landingPointsSize; //0x00A0
	int32_t landingPointsCount; //0x00A4
	int32_t* devastatedTiles; //0x00A8
	int32_t devastatedTilesSize; //0x00AC
	int32_t devastatedTilesCount; //0x00B0
	float* religionsARR;//0x00B4
	float* religionHistory[20];
	uint32_t* tilesBorderingEdgeOfMap; //0x0108
	int32_t tilesBorderingEdgeOfMapSize; //0x010C
	int32_t tilesBorderingEdgeOfMapCount; //0x0110
	int32_t armiesHostileToArrayIndexIDStrength[31]; //0x0114
	struct settlementStruct* settlement;//0x0190
	int* tiles; //0x0194
	int32_t tilesSize; //0x0198
	int32_t tileCount; //0x019C
	int* fertileTiles; //0x01A0
	int32_t fertileTilesSize; //0x01A4
	int32_t fertileTilesCount; //0x01A8
	struct neighbourRegion* neighbourRegions;//0x01AC
	int32_t neighbourRegionsCountArraySize; //0x01B0
	int32_t neighbourRegionsNum; //0x01B4
	struct resourceStruct** resources;//0x01B8
	int32_t resourcesSize; //0x01BC
	int32_t resourcesNum; //0x01C0
	int32_t field_1c4; //0x01C4
	uint32_t resourceTypesBitMap; //0x01C8
	int32_t field_1cC; //0x01CC
	int32_t hiddenResources;
	int32_t hiddenResources2;
	int32_t field_1d8; //0x01D8
	int32_t settlementXCoord; //0x01DC
	int32_t settlementYCoord; //0x01E0
	int32_t portEntranceXCoord; //0x01E4
	int32_t portEntranceYCoord; //0x01E8
	uint8_t colorBlue; //0x01EC
	uint8_t colorGreen; //0x01ED
	uint8_t colorRed; //0x01EE
	char pad_01EF[1]; //0x01EF
	struct factionStruct* factionOwner;//0x01F0
	bool hasTradeRoute; //0x01F4
	char pad_01F5[3]; //0x01F5
	UNICODE_STRING** localizedRegionName; //0x01F8
	UNICODE_STRING** localizedSettlementName; //0x01FC
	UNICODE_STRING** localizedRebelsName; //0x0200
	char* rebelType; //0x0204
	int32_t rebelTypeHash; //0x0208
	int32_t harvestSuccess; //0x020C
	int32_t triumphValue; //0x0210
	int32_t totalLandValue; //0x0214
	int32_t totalTradeValue; //0x0218
public:
	std::string getLocalizedName();
	std::string getLocalizedRebelsName();
	int settlementCount();
	int getResourcesValue();
	bool neighboursFaction(int factionId);
	void calculateRegionStrengths(int factionId, regionStrengths* strengths);
	void calculateBackupRegionStrengths(regionStrengths* strengths, int* enemyNum, int* neutralNum);
	settlementStruct* getSettlement(int index);
	bool hasFaction(int factionId);
	bool hasAlliesToFaction(int factionId, bool trustedOnly);
	int getEnemySettsToFaction(int factionId);
	int getNeutralSettsToFaction(int factionId);
	settlementStruct* getTargetSettForFaction(factionStruct* faction);
	void changeRegionName(const char* newName);
	void changeRegionSettlementName(const char* newName);
	void changeRebelsName(const char* newName);
	void fixReligionLevels();
	oneTile* getPatrolPoint(int index);
	oneTile* getDevastatedTile(int index);
	oneTile* getTile(int index);
	oneTile* getFertileTile(int index);
	oneTile* getTileBorderingEdgeOfMap(int index);
	armyStruct* getArmy(const int index)
	{
		if (index < 0 || index >= stacksNum)
			return nullptr;
		return armies[index];
	}
	fortStruct* getFort(const int index)
	{
		if (index < 0 || index >= fortsNum)
			return nullptr;
		return forts[index];
	}
	watchTowerStruct* getWatchTower(const int index)
	{
		if (index < 0 || index >= watchtowersNum)
			return nullptr;
		return watchtowers[index];
	}
	seaConnectedRegion* getSeaConnectedRegion(const int index)
	{
		if (index < 0 || index >= seaConnectedRegionsCount)
			return nullptr;
		return &seaConnectedRegions[index];
	}
	seaConnectedRegion* getSeaExportRegion(const int index)
	{
		if (index < 0 || index >= tradeFleetCount)
			return nullptr;
		return seaExportRegions[index];
	}
	seaConnectedRegion* getSeaImportRegion(const int index)
	{
		if (index < 0 || index >= seaImportRegionsCount)
			return nullptr;
		return seaImportRegions[index];
	}
	landingPoint* getLandingPoint(const int index)
	{
		if (index < 0 || index >= landingPointsCount)
			return nullptr;
		return &landingPoints[index];
	}
	neighbourRegion* getNeighbourRegion(const int index)
	{
		if (index < 0 || index >= neighbourRegionsNum)
			return nullptr;
		return &neighbourRegions[index];
	}
	bool hasResourceType(const int resourceType)
	{
		return resourceTypesBitMap & (1 << resourceType);
	}
	resourceStruct* getResource(const int index)
	{
		if (index < 0 || index >= resourcesNum)
			return nullptr;
		return resources[index];
	}
	bool hasHiddenResource(const std::string& name);
	void setHiddenResource(const std::string& name, bool enable);
	bool hasHiddenResourceId(const int index)
	{
		if (index < 0)
			return false;
		return eopHiddenResources::hasHiddenResource(regionID, index);
	}
	void setHiddenResourceId(const int index, const bool enable)
	{
		if (index < 0)
			return;
		if (index < 64)
		{
			const auto hiddenResourcesPtr = reinterpret_cast<uint64_t*>(&hiddenResources);
			if (enable)
				*hiddenResourcesPtr |= (1ULL << index);
			else
				*hiddenResourcesPtr &= ~(1ULL << index);
		}
	}
	int getHostileArmiesStrength(const int factionId)
	{
		return armiesHostileToArrayIndexIDStrength[factionId];
	}
	float getReligionHistory(const int religionId, const int turnsAgo)
	{
		if (turnsAgo > 19)
			return 0.0f;
		if (turnsAgo == 0)
			return religionsARR[religionId];
		return religionHistory[turnsAgo][religionId];
	}
};//Size: 0x021C


struct oneTile {
	DWORD* object; //can be character, resource, fort, 
	struct roadStruct* road; //0x0004
	uint16_t tileIncome;
	int8_t isLand;
	int8_t field_0xB;
	uint16_t modelIndex;
	int8_t modelSeed;
	int8_t pad;
	int groundType;
	int regionId;
	int32_t choke; //choke value like in toggle_terrain
	uint32_t factionId : 5;
	uint32_t riverSource : 1;
	uint32_t river : 1;
	uint32_t crossing : 1;
	uint32_t cliff : 3;
	uint32_t volcano : 1;
	uint32_t seaCrossing : 3;
	uint32_t roadLevel : 3;
	uint32_t wallLevel : 2;
	uint32_t earthQuake : 3;
	uint32_t flood : 3;
	uint32_t storm : 3;
	uint32_t volcanoStrength : 3;
	uint32_t scorched : 1;
	uint32_t tradeRoute : 1;
	uint32_t devastationLevel : 3;
	uint32_t deforested : 1;
	uint32_t highlightExtents : 4;
	uint32_t border : 1;
	uint32_t landCoast : 1;
	uint32_t seaCost : 1;
	bool settlement : 1;
	bool fort : 1;
	bool dock : 1;
	bool port : 1;
	bool watchtower : 1;
	bool navy : 1;
	bool army : 1;
	int8_t nonPassable;
	uint32_t armiesNearTile; //0x0028
	uint32_t charactersOnTile; //0x002C
	float mpModifier; //0x0030
public:
	oneTileDouble* tileToDoubleTile();
	int getTileX();
	int getTileY();
	std::pair<int, int> getTileCoords();
	int getTileCharacterCount();
	character* getTileCharacterAtIndex(int index);
	int getTileHeatValue();
	float getTileHeight()
	{
		const auto doubleTile = tileToDoubleTile();
		if (!doubleTile)
			return 0.0f;
		return doubleTile->height;
	}
	void setTileHeight(float height);
	DWORD* findObject(const int type)
	{
		DWORD* thisPtr = object;
		while (thisPtr)
		{
			if (const int objectType = callVFunc<4, int>(thisPtr); objectType == type || (type == 0 && objectType < 28))
				return thisPtr;
			thisPtr = *reinterpret_cast<DWORD**>((reinterpret_cast<DWORD>(thisPtr) + 0x4));  // NOLINT(performance-no-int-to-ptr)
		}
		return nullptr;
	}
	template<typename T>
	T* getTileObject(const strategyObject type)
	{
		return reinterpret_cast<T*>(findObject(static_cast<int>(type)));
	}
	template<typename T>
	T* getTileObject(const int type)
	{
		return reinterpret_cast<T*>(findObject(type));
	}
	armyStruct* getArmy(bool onlyLead = false);
	resourceStruct* getResource()
	{
		return getTileObject<resourceStruct>(0);
	}
	character* getCharacter()
	{
		return getTileObject<character>(strategyObject::character);
	}
	fortStruct* getFort()
	{
		return getTileObject<fortStruct>(strategyObject::fort);
	}
	settlementStruct* getSettlement()
	{
		return getTileObject<settlementStruct>(strategyObject::settlement);
	}
	portBuildingStruct* getPort()
	{
		return getTileObject<portBuildingStruct>(strategyObject::port);
	}
	watchTowerStruct* getWatchtower()
	{
		return getTileObject<watchTowerStruct>(strategyObject::watchtower);
	}
	int getTileClimate()
	{
		const auto doubleTile = tileToDoubleTile();
		if (!doubleTile)
			return 0;
		return doubleTile->climate;
	}
	void setTileClimate(int climate);
	void setTileGroundType(int ground);
	int getTileGroundType()
	{
		return groundType;
	}
	int getFactionId()
	{
		return factionId;
	}
	bool isRiverSource()
	{
		return riverSource;
	}
	bool isRiver()
	{
		return river | riverSource | crossing;
	}
	bool isCrossing()
	{
		return crossing;
	}
	bool isCliff()
	{
		return cliff;
	}
	bool isVolcano()
	{
		return volcano;
	}
	bool isSeaCrossing()
	{
		return seaCrossing;
	}
	bool isRoad()
	{
		return roadLevel;
	}
	int getRoadLevel()
	{
		return roadLevel;
	}
	int getEarthQuake()
	{
		return earthQuake;
	}
	int getFlood()
	{
		return flood;
	}
	int getStorm()
	{
		return storm;
	}
	int getVolcanoStrength()
	{
		return volcanoStrength;
	}
	bool isScorched()
	{
		return scorched;
	}
	bool isTradeRoute()
	{
		return tradeRoute;
	}
	int getDevastationLevel()
	{
		return devastationLevel;
	}
	void setDevastationLevel(const int value)
	{
		int newValue = value;
		if (value > 7)
			newValue = 7;
		devastationLevel = newValue;
	}
	bool isDeforested()
	{
		return deforested;
	}
	void setDeforested(const bool value)
	{
		deforested = value;
	}
	int getHighlightExtents()
	{
		return highlightExtents;
	}
	bool isBorder()
	{
		return border;
	}
	bool isLandCoast()
	{
		return landCoast;
	}
	bool isSeaCoast()
	{
		return seaCost;
	}
	bool hasSettlement()
	{
		return settlement;
	}
	bool hasFort()
	{
		return fort;
	}
	bool hasDock()
	{
		return dock;
	}
	bool hasPort()
	{
		return port;
	}
	bool hasWatchtower()
	{
		return watchtower;
	}
	bool hasNavy()
	{
		return navy;
	}
	bool hasArmy()
	{
		return army;
	}
    bool isImpassible()
	{
		return nonPassable == -1;
	}
    bool isDevastated()
	{
		return devastationLevel;
	}
	bool factionHasArmyNearTile(int factionID)
	{
		return armiesNearTile & (1 << factionID);
	}
	bool factionHasCharacterOnTile(int factionID)
	{
		return charactersOnTile & (1 << factionID);
	}
};

struct stratMap {
	bool isOpen;
	char pad[3];
	struct UNICODE_STRING** campaignName; //0x0004
	struct UNICODE_STRING*** campaignPath; //0x0008
	float minRoughness; //0x000C
	float maxRoughness; //0x0010
	float maxLandHeight; //0x0014
	float minSeaHeight; //0x0018
	float fractalMultiplier; //0x001C
	float minLatitude; //0x0020
	float maxLatitude; //0x0024
	climates* climates; //0x0028
	int mapWidth;
	int mapHeight;
	float tileSize; //0x0034
	float maxHeight;
	float minHeight; //0x003C
	void* storesMapRoughness; //0x0040
	struct oneTileDouble* climateTileArray; //0x0044
	struct oneTile* tilesArr;
	coordPair* riverSources;
	int32_t riverSourcesSize;
	int32_t riverSourcesNum;
	coordPair* crossings;
	int32_t crossingsSize;
	int32_t crossingsNum;
	struct coordPair* volcanoes; //0x0064
	int32_t volcanoesSize; //0x0068
	int32_t volcanoesNum; //0x006C
	struct coordPair* landMassConnections; //0x0070
	int32_t landMassConnectionsSize; //0x0074
	int32_t landMassConnectionsNum; //0x0078
	struct regionStruct regions[200];
	int regionsNum;
	struct landMass landMasses[20]; //0x1A660
	int32_t landMassNum; //0x1A7A0
	int maxChoke;
	int minDeadEnd;
	int runMode;
	struct customTilesDb customTilesDb; //0x1A7B0
	coastLine* coastLines;
	int coastLinesSize;
	int coastLinesNum;
	gameStdVector<int> restrictedCursorRegions;
	gameStdVector<gameStratRect> restrictedCursorPositions;
	gameStratRect cameraRestriction;
	bool radarRestricted;
	char padEnd[3];
public:
	regionStruct* getRegion(const int index)
	{
		if (!isOpen)
			return nullptr;
		if (index < 0 || index >= regionsNum)
			return nullptr;
		return &regions[index];
	}
	oneTile* getTile(const int x, const int y)
	{
		if (!isOpen)
			return nullptr;
		if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
			return nullptr;
		return &tilesArr[y * mapWidth + x];
	}
	coordPair* getVolcanoCoords(const int index)
	{
		if (!isOpen)
			return nullptr;
		return &volcanoes[index];
	}
	coordPair* getLandConnection(const int index)
	{
		if (!isOpen)
			return nullptr;
		return &landMassConnections[index];
	}
	landMass* getLandMass(const int index)
	{
		if (!isOpen)
			return nullptr;
		return &landMasses[index];
	}
	bool isInBounds(const int x, const int y)
	{
		return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight;
	}
	void fixAllReligionLevels();
};

struct extentColor
{
	uint8_t red{};
	uint8_t green{};
	uint8_t blue{};
	uint8_t alpha{};
	uint8_t borderAlpha{};
};

class extentColors
{
public:
	static extentColor getOwnColor() { return m_Own; }
	static extentColor getEnemyColor() { return m_Enemy; }
	static extentColor getZocColor() { return m_Zoc; }
	static void setOwnColorFull(const extentColor& color) { m_Own = color; }
	static void setEnemyColorFull(const extentColor& color) { m_Enemy = color; }
	static uint32_t getOwnColorHex()
	{
		return (m_Own.alpha << 24) | (m_Own.red << 16) | (m_Own.green << 8) | m_Own.blue;
	}
	static uint32_t getEnemyColorHex()
	{
		return (m_Enemy.alpha << 24) | (m_Enemy.red << 16) | (m_Enemy.green << 8) | m_Enemy.blue;
	}
	static uint32_t getZocColorHex()
	{
		return (m_Zoc.alpha << 24) | (m_Zoc.red << 16) | (m_Zoc.green << 8) | m_Zoc.blue;
	}
	static void setOwnColorHex(const uint32_t color)
	{
		m_Own.alpha = (color >> 24) & 0xFF;
		m_Own.red = (color >> 16) & 0xFF;
		m_Own.green = (color >> 8) & 0xFF;
		m_Own.blue = color & 0xFF;
	}
	static void setEnemyColorHex(const uint32_t color)
	{
		m_Enemy.alpha = (color >> 24) & 0xFF;
		m_Enemy.red = (color >> 16) & 0xFF;
		m_Enemy.green = (color >> 8) & 0xFF;
		m_Enemy.blue = color & 0xFF;
	}
	static void setOwnColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha, const uint8_t borderAlpha)
	{
		m_Own.red = red;
		m_Own.green = green;
		m_Own.blue = blue;
		m_Own.alpha = alpha;
		m_Own.borderAlpha = borderAlpha;
	}
	static void setZocColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
	{
		m_Zoc.red = red;
		m_Zoc.green = green;
		m_Zoc.blue = blue;
		m_Zoc.borderAlpha = alpha;
	}
	static void setEnemyColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha, const uint8_t borderAlpha)
	{
		m_Enemy.red = red;
		m_Enemy.green = green;
		m_Enemy.blue = blue;
		m_Enemy.alpha = alpha;
		m_Enemy.borderAlpha = borderAlpha;
	}
private:
	static extentColor m_Own;
	static extentColor m_Enemy;
	static extentColor m_Zoc;
};

struct mapArrows
{
	int fogOfWar;
	void* paths;
	int pathsSize;
	uint32_t pathsNum;
	void clearAll();
};

struct mapTilesDb
{
	char pad[0x7188];
	mapArrows* mapArrows;
};

struct minorSettlementBalance
{
	float incomeModifierBase = 0.f;
	float growthModifierBase = 0.25f;
	float incomeModifierAllianceOffset = 0.30f;
	float growthModifierAllianceOffset = 0.40f;
	float incomeModifierNeutralOffset = 0.15f;
	float growthModifierNeutralOffset = 0.20f;
	float incomeModifierTradeRightsOffset = 0.30f;
	float growthModifierTradeRightsOffset = 0.15f;
	float incomeModifierOwnFaction = 0.75f;
	float growthModifierOwnFaction = 0.75f;
};

class minorSettlementDb
{
public:
	static std::vector<settlementStruct*> getMinorSettlements(int regionId);
	static void addToMinorSettlements(int regionId, settlementStruct* settlement);
	static settlementStruct* getSettlement(int regionId, int settlementIndex);
	static int getSettlementCount(int regionId);
	static void clear() { regionMinorSettlements.fill({}); m_Loaded = false;}
	static settlementStruct* getSettlementAtIndex(int regionId, int index);
	static std::array<std::vector<settlementStruct*>, 200> regionMinorSettlements;
	static bool isLoaded() { return m_Loaded; }
	static minorSettlementBalance* getModifiers() { return m_BalanceModifiers; }
	static void load();
private:
	static bool m_Loaded;
	static minorSettlementBalance* m_BalanceModifiers;
};


namespace stratMapHelpers
{
	//move stratmap camera slow
	void moveStratCameraSlow(int x, int y);
	//move stratmap camera fast
	void moveStratCameraFast(int x, int y);
	//zoom stratmap camera
	void zoomStratCamera(float zoom);
	void clearSundries(character* thisChar);
	stratMap* getStratMap();
	std::shared_ptr<stratMovePath> getMovePath(int fromX, int fromY, int destX, int destY);
	void rebuildFrontiers();
	bool isTileFree(int* xy);
	bool isTileFreeLua(int x, int y);
	bool isStratMap();
	void updateTerrain();
	void* getMainStratObject(void* baseObj);
	UINT32 getTileRegionID(int x, int y);
	factionStruct* getRegionOwner(int regionID);
	float getTileMoveCost(int x, int y, int destX, int destY);
	std::pair<int, int> findValidTileNearTile(int x, int y, int charType);
	bool isTileValidForCharacterType(int charType, int x, int y);
	void viewTacticalMap(int x, int y);
	float getGroundTypeMoveCost(int groundType);
	void setGroundTypeMoveCost(int groundType, float newCost);
	void getGameTileCoordsWithCursor(int& x, int& y);
	std::tuple<int, int> getStratHoveredCoords();
	std::tuple<float, float, float> getBattleHoveredCoords();
	settlementStruct* getSettlement(stratMap* map, const std::string& name);
	regionStruct* getRegionByName(stratMap* map, const std::string& name);
	std::pair<int, int> convertTileCoords(const DWORD arrayIndex);
	std::queue<std::pair<int, int>> getNeighbourTiles(int x, int y, int depth = 1);
	oneTile* getTile(int x, int y);
	regionStruct* getRegion(int index);
	float getDistanceInTiles(const int x, const int y, const int destX, const int destY);
    void addToLua(sol::state& luaState);
};
