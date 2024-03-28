#pragma once
template<unsigned int IIdx, typename TRet, typename ... TArgs>
TRet CallVFunc(void* thisptr, TArgs ... argList)
{
	using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
	return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
}

#include <cstdint>
#include <windows.h>
#include <basetsd.h>
typedef unsigned char   undefined;
typedef unsigned int    uint;
typedef unsigned char    uchar;
typedef unsigned short    ushort;
#pragma pack(push,1)
typedef struct stackStruct stackStruct, * PstackStruct;
typedef struct settlementStruct settlementStruct, * PsettlementStruct;


struct UNICODE_STRING {
	USHORT something;//idk
	USHORT Length;//idk
	USHORT something2;//idk
	PWSTR Buffer;//y
};

struct rcString
{
public:
	char pad_0000[12]; //0x0000
	wchar_t* string; //0x000C
	char pad_0010[104]; //0x0010
}; //Size: 0x0078

struct generalStats
{
public:
	struct soldierInBattle* generalBgSoldier; //0x0000
	struct soldierInBattle* generalBgMount; //0x0004
}; //Size: 0x0008

struct legioString
{
public:
	char* name; //0x0000
	int32_t nameHash; //0x0004
	struct UNICODE_STRING** localizedLegoName; //0x0008
	int32_t recruitCount; //0x000C
	char pad_0010[4]; //0x0010
}; //Size: 0x0014

struct cultureModels
{
public:
	char pad_0000[5584]; //0x0000
}; //Size: 0x15D0

struct cultureAgent
{
public:
	char* cardName; //0x0000
	int32_t cardNameHash; //0x0004
	char* infoCardName; //0x0008
	int32_t infoCardNameHash; //0x000C
	char* cardName2; //0x0010
	int32_t cardName2Hash; //0x0014
	int16_t index1; //0x0018
	char pad_001A[2]; //0x001A
	int16_t index2; //0x001C
	char pad_001E[2]; //0x001E
	int16_t cost; //0x0020
	int16_t time; //0x0022
	int16_t popCost; //0x0024
	int16_t someOtherRomeShit; //0x0026
	char pad_0028[4]; //0x0028
}; //Size: 0x002C



struct culture
{
public:
	int32_t cultureID; //0x0000
	char* cultureName; //0x0004
	int32_t cultureNameHash; //0x0008
	char pad_000C[1072]; //0x000C
	int32_t fortCost; //0x043C
	int32_t watchTowerCost; //0x0440
	char pad_0444[24]; //0x0444
	struct cultureModels cultureModelCrap; //0x045C
	struct cultureAgent cultureAgents[6]; //0x1A2C
	char pad_1B34[4]; //0x1B34
}; //Size: 0x1B38

struct coordPair
{
public:
	int32_t xCoord; //0x0000
	int32_t yCoord; //0x0004
}; //Size: 0x0008

struct movePath
{
public:
	struct coordPair* tileCoords; //0x0000
	int32_t tileCoordsSize; //0x0004
	int32_t tileCoordsCount; //0x0008
}; //Size: 0x000C


struct eventTrigger
{
	void* eventTrigger_vtbl; //0x0000
	char pad_0004[28]; //0x0004
};

struct buildingBattle
{
public:
	char pad_0000[24]; //0x0000
	int32_t type; //0x0018
	char pad_001C[8]; //0x001C
	float posX; //0x0024
	float posZ; //0x0028
	float posY; //0x002C
	char pad_0030[4]; //0x0030
	int32_t endHealth; //0x0034
	int32_t currentHealth; //0x0038
	int32_t startHealth; //0x003C
	char pad_0040[4]; //0x0040
	int32_t isDefenderControlled; //0x0044
	char pad_0048[8]; //0x0048
}; //Size: 0x0050



struct religionDatabase
{
public:
	void* religionDatabase_vtbl;//0x0000
	struct rcString** religionNames; //0x0004
	int32_t religionNamesSize; //0x0008
	int32_t religionCount; //0x000C
	struct rcString** religionPips; //0x0010
	int32_t religionPipsSize; //0x0014
	int32_t religionPipsCount; //0x0018
	void* hashedStringTable; //0x001C

}; //Size: 0x0020

struct capturedCharacter
{
	struct namedCharacter* namedChar;
	int32_t capturedValue;
};

struct capturedUnit
{
	struct unit* unit;
	int32_t capturedSoldiers;
	int32_t capturedValue;
};

struct capturedFactionInfo
{
	int32_t targetFactionID;
	int32_t factionID;
	struct namedCharacter* character;
	struct namedCharacter* targetCharacter;
	int32_t bitMap;
	struct capturedCharacter* capturedCharacters;
	uint32_t capturedCharactersEnd;
	uint32_t capturedCharactersEnd2;
	char* logString;
	struct capturedUnit* capturedUnits;
	uint32_t capturedUnitsEnd;
	int32_t capturedUnitsEnd2;
	int32_t ransomValue;
};

struct floatsSeaConnect
{
public:
	uint32_t tileId; //0x0000
	float someFloat; //0x0004
}; //Size: 0x0008

struct seaConnectedRegion
{
public:
	int32_t regionID; //0x0000
	char pad_0004[4]; //0x0004
	float angle; //0x0008
	float distance; //0x000C
	int32_t seaExportValue; //0x0010
	int32_t seaImportValue; //0x0014
	struct floatsSeaConnect* tilesReachable; //0x0018
	int32_t tilesReachableSize; //0x001C
	int32_t tilesReachableCount; //0x0020
	struct coordPair* seaTradeLanePath; //0x0024
	int32_t seaTradeLanePathSize; //0x0028
	int32_t seaTradeLanePathCount; //0x002C
	char pad_0030[4]; //0x0030
}; //Size: 0x0034

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
	struct stackStruct** armies; //0x002C
	int32_t armyCountArraySize; //0x0030
	int32_t stacksNum; //0x0034
	struct fortStruct** forts; //0x0038
	int32_t fortCountArraySize; //0x003C
	int32_t fortsNum; //0x0040
	struct watchTowerStruct** watchtowers; //0x0044
	int32_t watchtowerCountArraySize; //0x0048
	int32_t watchtowersNum; //0x004C
	int8_t isSea; //0x0050
	int8_t hasLake; //0x0051
	char pad_0052[2]; //0x0052
	struct landMass* landMass; //0x0054
	struct roadStruct* roadToPort; //0x0054
	struct seaConnectedRegion* seaConnectedRegions; //0x005C
	int32_t seaConnectedRegionsCountArraySize; //0x0060
	int32_t seaConnectedRegionsCount; //0x0064
	int32_t loyaltyFactionID; //0x0068
	struct seaConnectedRegion* seaExportRegion; //0x006C
	struct seaConnectedRegion* seaExportRegion2; //0x0070 --disk/dlc only
	struct seaConnectedRegion* seaExportRegion3; //0x0074 --disk/dlc only
	struct seaConnectedRegion* seaExportRegion4; //0x0078 --disk/dlc only
	struct seaConnectedRegion** seaImportRegions; //0x007C
	int32_t seaImportRegionsSize; //0x0080
	int32_t seaImportRegionsCount; //0x0084
	int32_t tradeFleetCount; //0x0088
	struct mercPool* mercPool;//0x008C
	int32_t* regionSeaEdges; //0x0090
	int32_t regionSeaEdgesSize; //0x0094
	int32_t regionSeaEdgesCount; //0x0098
	char pad_009C[12]; //0x009C
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
	struct resStrat** resources;//0x01B8
	int32_t resourcesSize; //0x01BC
	int32_t resourcesNum; //0x01C0
	int32_t N00008CB2; //0x01C4
	uint32_t resourceTypesBitMap; //0x01C8
	int32_t N00008CB4; //0x01CC
	uint32_t hiddenResources1; //0x01D0
	uint32_t hiddenResources2; //0x01D4
	char pad_01D8[4]; //0x01D8
	int32_t settlementXCoord; //0x01DC
	int32_t settlementYCoord; //0x01E0
	int32_t portEntranceXCoord; //0x01E4
	int32_t portEntranceYCoord; //0x01E8
	uint8_t colorBlue; //0x01EC
	uint8_t colorGreen; //0x01ED
	uint8_t colorRed; //0x01EE
	char pad_01EF[1]; //0x01EF
	struct factionStruct* factionOwner;//0x01F0
	char pad_01F4[4]; //0x01F4
	UNICODE_STRING** localizedRegionName; //0x01F8
	UNICODE_STRING** localizedSettlementName; //0x01FC
	UNICODE_STRING** localizedRebelsName; //0x0200
	char* rebelType; //0x0204
	int32_t rebelTypeHash; //0x0208
	int32_t harvestSuccess; //0x020C
	int32_t triumphValue; //0x0210
	int32_t N00008CC6; //0x0214
	uint32_t totalSeaTradeValue; //0x0218
};//Size: 0x021C

struct neighbourRegion
{
public:
	struct regionStruct* region; //0x0000
	int32_t regionID; //0x0004
	float value; //0x0008
	int8_t alliedRegion; //0x000C
	char pad_000D[3]; //0x000D
	struct roadStruct* connectingRoad; //0x0010
	int32_t tradeValue; //0x0014
	int32_t* bordertiles; //0x0018
	int32_t borderTilessize; //0x001C
	int32_t borderTilesCount; //0x0020
	char pad_0024[52]; //0x0024
}; //Size: 0x0058

struct landMass
{
public:
	int32_t index; //0x0000
	int32_t* regions; //0x0004
	int32_t regionsSize; //0x0008
	int32_t regionsNum; //0x000C
}; //Size: 0x0010

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
	uint8_t hasWinter; //0x0010
	char pad_0011[3]; //0x0011
	void* treeModelsSummerSparse; //0x0014
	int32_t treeModelsSummerSparseSize; //0x0018
	int32_t treeModelsSummerSparseNum; //0x001C
	void* treeModelsSummerDense; //0x0020
	int32_t treeModelsSummerDenseSize; //0x0024
	int32_t treeModelsSummerDenseNum; //0x0028
	void* treeModelsSummerCanopy; //0x002C
	int32_t treeModelsSummerCanopysize; //0x0030
	int32_t treeModelsSummerCanopyNum; //0x0034
	void* treeModelsWinterSparse; //0x0038
	int32_t treeModelsWinterSparsesize; //0x003C
	int32_t treeModelsWinterSparseNum; //0x0040
	void* treeModelsWinterDense; //0x0044
	int32_t treeModelsWinterDensesize; //0x0048
	int32_t treeModelsWinterDenseNum; //0x004C
	void* treeModelsWinterCanopy; //0x0050
	int32_t treeModelsWinterCanopySize; //0x0054
	int32_t treeModelsWinterCanopyNum; //0x0058
}; //Size: 0x005C

struct climates
{
public:
	char pad_0000[4]; //0x0000
	struct climate* climateArray; //0x0004
	int32_t climatesSize; //0x0008
	int32_t climatesNum; //0x000C
}; //Size: 0x0010

struct oneTileDouble
{
public:
	float height; //0x0000
	int8_t climate; //0x0004
	int8_t groundType; //0x0005
	int8_t Nx0006; //0x0006
	int8_t Nx0007; //0x0007
}; //Size: 0x0008

struct stratMap {
	undefined field_0x0[4];
	struct UNICODE_STRING** campaignName; //0x0004
	struct UNICODE_STRING*** campaignPath; //0x0008
	float minRoughness; //0x000C
	float maxRoughness; //0x0010
	float maxLandHeight; //0x0014
	float minSeaHeight; //0x0018
	float fractalMultiplier; //0x001C
	float lattitudeMin; //0x0020
	float lattitudeMax; //0x0024
	climates* climates; //0x0028
	int mapWidth;
	int mapHeight;
	float N00008987; //0x0034
	uint32_t N00008988; //0x0038
	char pad_003C[4]; //0x003C
	void* storesMapRoughness; //0x0040
	struct oneTileDouble* climateTileArray; //0x0044
	struct oneTile* tilesArr;
	DWORD arraySmthing; //0x004C
	undefined field_0x50[20];
	struct coordPair* volcanoes; //0x0064
	int32_t volcanoesSize; //0x0068
	int32_t volcanoesNum; //0x006C
	struct coordPair* landMassConnections; //0x0070
	int32_t landMassConnectionsSize; //0x0074
	int32_t landMassConnectionsNum; //0x0078
	struct regionStruct regions[200];
	int regionsNum;
	struct landMass landMass[20]; //0x1A660
	int32_t landMassNum; //0x1A7A0
};
struct 	mercPoolUnit
{
public:
	struct eduEntry* eduEntry; //0x0000
	int32_t experience; //0x0004
	int32_t cost; //0x0008
	float replenishMin; //0x000C
	float replenishMax; //0x0010
	int32_t maxUnits; //0x0014
	float currentPool; //0x0018
	float startYear; //0x001C
	float endYear; //0x0020
	char pad_0024[4]; //0x0024
	int* religionsList; //0x0028
	int* religionsListEnd; //0x002C
	int* religionsListEnd2; //0x0030
	int32_t crusading; //0x0034
	char pad_0038[4]; //0x0038
	int* eventsList; //0x003C
	int* eventsListEnd; //0x0040
	int* eventsListEnd2; //0x0044
	int16_t poolIndex; //0x0048
	int16_t mercPoolUnitIndex; //0x004A
	struct mercPool* mercPool; //0x004C
}; //Size: 0x0050

struct mercPoolUnits
{
public:
	struct mercPoolUnit mercPoolUnit[64]; //0x0000
};

struct mercPoolUnitsPtr
{
public:
	struct mercPoolUnit* mercPoolUnits;
	struct mercPoolUnitsPtr* nextUnitsPtr;
	struct mercPoolUnit* prevPoolUnits;
	int Maxpool;
	int currentPool;
};

struct mercPool
{
public:
	char* name; //0x0000
	char pad_0004[4]; //0x0004
	struct regionStruct* regions; //0x008
	char pad_000C[16]; //0x000C
	struct mercPoolUnitsPtr firstUnits; //0x001C
}; //Size: 0x0180

struct mercPoolList
{
public:
	struct mercPool* mercPools; //0x0000
	struct mercPoolList* nextMercPools; //0x0004
	struct mercPoolList* previousMercPools; //0x0008
	int32_t currentSize; //0x000C
	int32_t currentCount; //0x0010
}; //Size: 0x0014

struct uiElement {
	undefined field_0x0[16];
	int xSize;
	int ySize;
	undefined field_0x18[4];
	char* elementName; /* can be nullptr */
	undefined field_0x20[32];
	int xPos;
	int yPos;
	undefined field_0x48[4];
	struct uiElement** subElements;
	undefined field_0x50[4];
	int subElementsNum;
	undefined field_0x58[112];
};

struct roadStruct
{
public:
	struct coordPair* coords; //0x0000
	int32_t coordsSize; //0x0004
	int32_t coordsNum; //0x0008
	void* someOtherArray; //0x000C
	int32_t someOtherArraySize; //0x0010
	int32_t someOtherArrayCount; //0x0014
	int32_t regionIdStart; //0x0018
	int32_t regionIdEnd; //0x001C
	int32_t tradeValue; //0x0020
	char pad_0020[20]; //0x0020
}; //Size: 0x0038

struct oneTile {
	DWORD* object; //can be character, resource, fort, 
	struct roadStruct* road; //0x0004
	undefined field_0x8[2];
	int8_t isLand;
	undefined field_0xB[1];
	int8_t terrainModel;
	int8_t ModelIsHills;
	undefined field_0xE[2];
	int groundType;
	int regionId;
	undefined field_0x18[4];
	int8_t factionId;
	int8_t borderingRiver;
	int8_t borderingSettlement;
	undefined field_0x1F[1];
	int8_t hasRoad; //bit 1 has devastation status
	int8_t border;
	undefined field_0x22[2];
	int8_t objectTypes; //bit87_character6_ship5_watchtower4_port3_bit32_fort1_settlement0
	int8_t passable;
	int8_t N0002227F; //0x0026
	int8_t N00022271; //0x0027
	uint32_t armiesNearTile; //0x0028
	uint32_t charactersOnTile; //0x002C
	float mpModifier; //0x0030
};

struct aiSiegeEquipStruct
{
	int type;
	int8_t byte4;
	int8_t byte5_init1;
	int8_t byte6;
	int8_t byte7;
	int normalCost;
	int cost;
	int int10;
};

struct siegeS {
	void* vTable;
	struct oneSiege* sieges; //0x0004
	int8_t field_8;
	char pad_0009[3]; //0x0009
	struct settlementStruct* goal; /* settlement or fort */
	struct stackStruct* army;
	struct aiSiegeEquipStruct aiEquipmentQueue[25];
	int someCount;
	int field_20C;
	int field_210;
	int field_214;
	int field_218;
	int field_21C;
	int field_220;
	int field_224;
	int field_228;
	int siegeTurns;
	int field_230;
	int field_234;
	int soldierCount;
	int8_t gotCaptured;
	char pad_x[3];
};

struct stringWithHash
{
public:
	char* name; //0x0000
	int32_t hash; //0x0004
}; //Size: 0x0008

struct holdRegionsWinCondition
{
public:
	char pad_0000[4]; //0x0000
	uint32_t stratTableStringIndex; //0x0004
	struct stringWithHash* regionsToHold; //0x0008
	int32_t regionsToHoldSize; //0x000C
	int32_t regionsToHoldCount; //0x0010
	uint32_t factionsToOutliveBitfield; //0x0014
	int32_t numberOfRegions; //0x0018
	char pad_001C[4]; //0x001C
	int32_t* holdRegionLengths; //0x0020
	int32_t holdRegionLengthsSize; //0x0024
	int32_t holdRegionLengthsCount; //0x0028
	char pad_002C[12]; //0x002C
}; //Size: 0x0080

struct battleSettlement
{
public:
	char pad_0000[4]; //0x0000
	struct settlementStruct* settlement; //0x0004
	char pad_0008[4]; //0x0008
	struct battleResidence* battleResidence; //0x000C
	char pad_0010[304]; //0x0010
}; //Size: 0x0140

struct settlementBuildings
{
public:
	struct buildingBattle** buildingList; //0x0000
	int32_t buildingListSize; //0x0004
	int32_t buildingNum; //0x0008
}; //Size: 0x000C

struct battleBuildings
{
public:
	char pad_0000[20]; //0x0000
	struct buildingBattle** allBuildings; //0x0014
	int32_t allBuildingsSize; //0x0020
	int32_t allBuildingsNum; //0x0020
	int32_t* allBuildingsInt; //0x0020
	char pad_0024[8]; //0x0024
	struct settlementBuildings* settlementBuildings; //0x002C
}; //Size: 0x0444

struct battleResidence
{
public:
	char pad_0000[28]; //0x0000
	struct settlementStruct* settlement; //0x001C
	char pad_0020[8]; //0x0020
	struct factionStruct** faction; //0x0028
	char pad_002C[4]; //0x002C
	int32_t isFortBattle; //0x0030
	struct battleBuildings* battleBuildings; //0x0034
	char pad_0038[4]; //0x0038
	float N000203C6; //0x003C
	float N000203C7; //0x0040
	char pad_0044[40]; //0x0044
	int8_t settlementWallsBreached; //0x006C
	int8_t settlementGateDestroyed; //0x006D
	char pad_006E[2]; //0x006E
	int8_t N0003A33C; //0x0070
	char pad_0071[7]; //0x0071
	float N0003A33E; //0x0078
	char pad_007C[200]; //0x007C
}; //Size: 0x0144

struct factionRanking
{
public:
	float totalRanking; //0x0000
	float militaryRanking; //0x0004
	float productionRanking; //0x0008
	float territoryRanking; //0x000C
	float FinancialRanking; //0x0010
	float populationRanking; //0x0014
}; //Size: 0x0018

struct factionStratMapDescrS { /* see descr_sm_factions.txt */
	int id;
	char* facName;
	DWORD facNameHash;
	int shadowedByID;
	char* shadowedBy;
	char pad_0014[4];
	int32_t shadowingID;
	wchar_t* shadowing;
	char pad_0020[4];
	int32_t spawnsOnRevoltID;
	char* spawnsOnRevolt;
	char pad_002C[4];
	int32_t otherId;
	char pad_0034[12];
	struct culture* culture;
	int religionID;
	struct model_Rigid* symbol;
	char* symbolPath;
	DWORD symbolPathHash;
	struct model_Rigid* rebel_symbol;
	char* rebel_symbolPath;
	DWORD rebel_symbolPathHash;
	undefined field_0x60[8];
	char* loading_logo;
	DWORD loading_logoHash;
	uchar primary_colour_blue;
	uchar primary_colour_green;
	uchar primary_colour_red;
	uchar secondary_colour_blue;
	uchar secondary_colour_green;
	uchar secondary_colour_red;
	undefined field_0x76[2];
	int triumph_value;
	int standard_index;
	int logo_index;
	int small_logo_index;
	int8_t roman;
	int8_t barbarian;
	int8_t eastern;
	int8_t slave;
	struct stringWithHash* hordeUnitNames;
	int32_t hordeUnitNamesSize;
	int32_t hordeUnitNamesCount;
	int32_t hordeMinUnits;
	int32_t hordeMaxUnits; //0x009C
	int32_t reductionPerHorde;
	int32_t hordeUnitPerSettlementPop;
	int32_t hordeMinNamedCharacters;
	int32_t hordeMaxPercentArmyStack;
	void* N00008DBD;
	int32_t N00008DBE;
	int32_t N00008DBF;
	uint8_t customBattleAvailability; //0x00BC
	char pad_00BD[3]; //0x00BD
	uint32_t periodsUnavailableInCustomBattle; //0x00C0
	uint8_t N00008DC2; //0x00C4
	uint8_t canSap; //0x00C5
	char pad_00C6[2]; //0x00C6
	int32_t prefersNavalInvasions; //0x00C8
	uint8_t canHavePrincess; //0x00CC
	char pad_00CD[3]; //0x00CD
	char* specialfactionType; //0x00D0
	char pad_00D4[4]; //0x00D4
	uint8_t hasFamilyTree; //0x00D8
	uint8_t teutonic; //0x00D9
	uint8_t disbandToPools; //0x00DA
	uint8_t canBuildSiegeTowers; //0x00DB
	uint8_t canTransmitPlague; //0x00DC
	char pad_00DD[3]; //0x00DD
};

struct watchTowerModel {
	struct model_Rigid* modelP;
	undefined field_0x4[26];
};

//watchtower
struct watchTowerStruct {
	void* watchTowerVtable; //0x0000
	DWORD* object; //0x0004
	struct general* character; //0x0008
	int xCoord;
	int yCoord;
	char pad_0014[4]; //0x0014
	int8_t N000021C7; //0x0018
	char pad_0019[3]; //0x0019
	float N000021C8; //0x001C
	int8_t N000021C9; //0x0020
	char pad_0021[3]; //0x0021
	struct watchTowerModel* model;
	int32_t regionID; //0x0028
	struct factionStruct* faction; //0x002C
	struct settlementStruct* settlement; //0x0030
	void* trackedPointerArmyVtable; //0x0034
	struct stackStruct* blockingArmy; //0x0038
	int32_t factionID; //0x003C
	char pad_0040[8]; //0x0040
};

struct worldRecord {
	undefined field_0x0[4];
	char recordName[16]; /* pointer if allocatedNameLen>15 */
	int nameLen;
	int allocatedNameLen;
	undefined field_0x1c[4];
	char recordGroup[16]; /* pointer if allocatedGroupLen>15 */
	int groupLen;
	int allocatedGroupLen;
	undefined field_0x38[40];
};

struct crusade
{
public:
	char pad_0000[20]; //0x0000
	void* N0000121D; //0x0014
	char pad_0018[28]; //0x0018
	int32_t startTurn; //0x0034
	int32_t endTurn; //0x0038
	struct settlementStruct* targetSettlement; //0x003C
	int32_t length; //0x0040
	int32_t outcome; //0x0044
	char pad_0048[252]; //0x0048
	void* N00001269; //0x0144
	char pad_0148[44]; //0x0148
}; //Size: 0x0174



struct jihad
{
public:
	char pad_0000[52]; //0x0000
	int32_t startTurn; //0x0034
	int32_t endTurn; //0x0038
	struct settlementStruct* targetSettlement; //0x003C
	int32_t length; //0x0040
	int32_t outcome; //0x0044
	char pad_0048[292]; //0x0048
}; //Size: 0x016C

struct settlementList
{
	struct settlementStruct** settlements; //0x0000
	struct settlementList* nextSettlements; //0x0004
	struct settlementList* previousSettlements; //0x0008
	int32_t settlementsSize; //0x000C
	int32_t settlementsNum; //0x0010
};


struct factionDiplomacy
{
public:
	char pad_0000[4]; //0x0000
	int state;  /* diplomatic state(war, alliance, peace)(600/0/200) */
	int trade; /* trade rights(0 or 1) */
	int protectorate; /* protectorate or not(15 or 6) */
	float factionStanding; //0x0010
	char pad_0014[4]; //0x0014
	int32_t numTurnsAllied; //0x0018
	int32_t numTurnsWar; //0x001C
	int32_t numTurnsPeace; //0x0020
	int32_t numTurnsTrade; //0x0024
	int32_t numTurnsAccess; //0x0028
	char pad_002C[72]; //0x002C
	int32_t turnsSinceMapInfoGiven; //0x0074 -no start at 0 but like 10 on campaign start
	char pad_0078[8]; //0x0078
	int32_t turnsSinceMapInfoTaken; //0x0080 -no start at 0 but like 10 on campaign start
	int32_t numTurnsCeasefire; //0x0084
}; //Size: 0x0088

struct trackedCharacter
{
public:
	struct N0001F08D* N0001F061; //0x0000
	struct general* character; //0x0004
}; //Size: 0x0008

struct cardinal
{
public:
	struct trackedCharacter* trackedCharacter; //0x0000
	struct N0001F079* cardinalInfo; //0x0004
}; //Size: 0x0008

struct cardinalArray
{
public:
	struct cardinal cardinals[16]; //0x0000
}; //Size: 0x0008

struct collegeOfCardinals
{
public:
	char pad_0000[4]; //0x0000
	struct namedCharacter* pope; //0x0004
	char pad_0008[4]; //0x0008
	struct cardinalArray* cardinalsArray; //0x000C
	int32_t cardinalsArraySize; //0x0010
	int32_t cardinalNum; //0x0014
	int8_t N0001D9D6; //0x0018
	int8_t getsUsedForCountVotes; //0x0019
	int8_t N0001F1B6; //0x001A
	int8_t N0001F1B4; //0x001B
	struct N0001F0B9* someOtherArray; //0x001C
	int32_t someOtherArraySize; //0x0020
	int32_t someOtherArrayCount; //0x0024
	struct UNICODE_STRING** localizedCardinalNames; //0x0028
	char pad_002C[32]; //0x002C
}; //Size: 0x004C

struct campaign {
	undefined field0_0x0[4];
	undefined field1_0x4[36];
	int32_t type; //0x0024
	char pad_0028[4]; //0x0028
	uint32_t playableFactionsBitmap; //0x002C
	char pad_0030[4]; //0x0030
	int playerFactionId;
	undefined field3_0x3c[4];
	int hotSeatPlayer2FactionId;
	undefined field5_0x44[232];
	int humanPlayers; /* number of player-controlled factions */
	int factionIdPow; /* pow(2,factionId) */
	int32_t campaignDifficultyFaction[31];
	int32_t battleDifficultyFaction[31];
	struct factionStruct* factionsSortedByDescrStrat[31];
	struct factionStruct* factionsSortedByID[31];
	int numberOfFactionsWithSlave;
	uint32_t existingFactionsBitMap; //0x0324
	int32_t nonPlayableFactionsNum; //0x0328
	char pad_032C[12]; //0x032C
	struct collegeOfCardinals* collegeOfCardinals; //0x033C
	struct factionStruct* papalFaction; //0x0340
	struct factionStruct* currentFactionTurn;
	int factionTurnID; /* FactionTurn? */
	UNICODE_STRING** currentDescrFile; /* path to descr_battle.txt or to descr_strat.txt */
	int32_t fogOfWar; //0x0350
	undefined field20_0x354[24];
	int tickCount; /* TickCount? */
	int millisecondCount;
	float secondCount;
	int TurnNumber;
	int field25_0x37c; /* PauseForBattleDialog? */
	float TimeScale;
	undefined field27_0x384[4];
	struct settlementStruct* rome;
	undefined field29_0x38c[4];
	struct settlementStruct* constantinople;
	char pad_0390[12]; //0x0390
	void* unkArray1; //0x039C
	int32_t unkArray1Size; //0x03A0
	int32_t unkArray1Count; //0x03A4
	void* unkArray2; //0x03A8
	int32_t unkArray2Size; //0x03AC
	int32_t unkArray2Count; //0x03B0
	void* script; //0x03B4
	void* scriptEnd; //0x03B8
	char pad_03BC[8]; //0x03BC
	void* pointerToScript; //0x03C4
	void* pointerToScriptEnd; //0x03C8
	int8_t N00001208; //0x03CC
	int8_t forceAllowManageAllSetts; //0x03CD
	char pad_03CE[18]; //0x03CE
	int8_t marianReformsActive; //0x03E0
	int8_t marianReformsDisabled; //0x03E1
	int8_t rebellingCharactersActive; //0x03E2
	int8_t gladiatorUprisingDisabled; //0x03E3
	int8_t nightBattlesEnabled; //0x03E4
	int8_t N00038FC9; //0x03E5
	int8_t N00038FCC; //0x03E6
	char pad_03E7[1]; //0x03E7
	float BrigandSpawnValue;
	float PirateSpawnValue;
	int8_t N00001211; //0x03F4
	int8_t showDateAsTurns; //0x03F5
	int8_t restrictAutoResolve; //0x03F6
	char pad_03F7[1]; //0x03F7
	int FreeUpkeepForts;
	int ActiveFactions;	/* number factions minus slave */
	undefined field40_0x400[12];
	int lastrandomseed;
	struct crusade crusade; //0x0410
	struct jihad jihad; //0x0584
	struct UNICODE_STRING** someUniStringMaybePassword; //0x06F0
	int8_t isAdminPasswordExist; //0x06F4
	int8_t saveEnabled; //0x06F5
	undefined field45_0x6f6[6];
	int8_t hotseatConsole;
	int8_t hotseatConsoleEnabledWithLogon;
	undefined field48_0x6fe[14];
	float currentDate;
	int season; /* season(0-summer,1-winter) */
	float startDate;
	int startSeason;
	float endDate;
	int endSeason;
	undefined field55_0x724[4];
	float timeAtStartBattle;
	int daysInBattle;
	float currentTimeInBattle; /* 24 max, so calc as daysInBattle*24+currentTimeInBattle */
	char pad_0734[28]; //0x0734
	float clockTime; //0x0750
	char pad_0754[260]; //0x0754
	int32_t interFactionMarriage[31][31]; //0x0858
	struct factionDiplomacy diplomaticStandings[31][31]; //0x175C
	struct settlementList settlementList; //0x215E4
	char* introVid; //0x215F8
	char pad_215FC[16]; //0x215FC
	struct fortStruct** fortsArray;
	int32_t fortsSize; //0x21610
	int32_t fortsNum; //0x21614
	struct portBuildingStruct** portsBuildings;
	int32_t portsBuildingsSize; //0x2161C
	int32_t portsBuildingsNum; //0x21620
	struct watchTowerStruct** watchtowers;
	int32_t watchtowersSize; //0x21628
	int32_t watchtowersNum; //0x2162C
	char pad[16]; //0x21638
	struct mercPoolList allMercPools; //0x2163C
	void* unk3; //0x21650
	void* unk4; //0x21654
};

struct uiUnitCard
{
public:
	char pad_0000[900]; //0x0000
	unit* unit; //0x0384
};

struct uiCardManager
{
public:
	char pad_0000[16]; //0x0000
	struct uiUnitCard** selectedUnitCards; //0x0010
	int32_t selectedUnitCardsSize; //0x0014
	int32_t selectedUnitCardsCount; //0x0018
	char pad_001C[24]; //0x001C
	struct uiUnitCard** unitCards; //0x0034
	int32_t unitCardsSize; //0x0038
	int32_t unitCardsCount; //0x003C
	char pad_0060[328]; //0x0060
	struct settlementStruct* selectedSettlement; //0x0188
	char pad_018C[12]; //0x018C
	struct general* selectedCharacter; //0x0198
	char pad_019C[12]; //0x019C
	struct fortStruct* selectedFort; //0x01A8
};

struct gameDataAllStruct {
	undefined field_0x0[40];
	struct campaign* campaignData;
	undefined field_0x2c[8];
	struct stratMap* stratMap;
	struct campaign* field_0x38;
	undefined field_0x3c[28];
	struct battleDataS* battleHandler;
	int* localFactionID2;
	int* localFactionID;
	undefined field_0x64[28];
	struct battleSettlement* battleSettlement;
	char pad[112];
	struct uiCardManager* uiCardManager;
	char pad2[84];
};

struct battleUnit
{
public:
	struct unit* unit; //0x0000
	struct UNICODE_STRING** localizedName; //0x0004
	float valuePerSoldier; //0x0008
	int32_t soldiersLost; //0x000C
	int32_t soldiersKilled; //0x0010
	int32_t takenPrisoner; //0x0014
	int32_t prisonersCaught; //0x0018
	int32_t soldiersStart; //0x001C
	uint32_t unitsRouted; //0x0020
	int32_t soldiersHealed; //0x0024
	int32_t unitsRoutedEnd; //0x0028
	int32_t soldiersEnd; //0x002C
	int32_t friendlyFireCasualties; //0x0030
	int32_t expStart; //0x0034
	int32_t expGained; //0x0038
	int8_t isGeneral; //0x003C
	int8_t hasWithdrawn; //0x003D
	uint16_t N000400D6; //0x003E
	int32_t N0001FD2C; //0x0040
	int32_t N0001FDB3; //0x0044
}; //Size: 0x0048

struct options1
{
	void* cursor; //0x0000
	int8_t N0003DD45; //0x0004
	int8_t widescreen; //0x0005
	char pad_0006[2]; //0x0006
	int64_t antiAliasMode; //0x0008
	int16_t subtitles; //0x0010
	int8_t english; //0x0012
	int8_t noBattleTimeLimit; //0x0013
	char pad_0014[4]; //0x0014
	int32_t useNewCursorActions; //0x0018
	int32_t campaignNumTimesPlay; //0x001C
	int8_t uiWinConditions; //0x0020
	int8_t isScenario; //0x0021
	char pad_0022[2]; //0x0022
	int32_t isHotseatEnabled; //0x0024
	int8_t hotseatAutosave; //0x0028
	int8_t email; //0x0029
	int8_t saveConfig; //0x002A
	int8_t closeAfterSave; //0x002B
	int8_t validateData; //0x002C
	int8_t campaignMapSpeedUp; //0x002D
	char pad_002E[2]; //0x002E
	int32_t skipAiFactions; //0x0030
	int8_t labelCharacters; //0x0034
	int8_t noBackGroundFmv; //0x0035
	int8_t disableArrowMarkers; //0x0036
	int8_t arcadeBattles; //0x0037
	int8_t disableEvents; //0x0038
	int8_t isPrologue; //0x0039
	int8_t updateAiCamera; //0x003A
	int8_t hideCampaign; //0x003B
	int32_t unlimitedMenOnBattlefield; //0x003C
	char pad_0040[4]; //0x0040
	void* prefFactionsPlayed; //0x0044
	int32_t tgaReserveSpace; //0x0048
	int32_t keysetUsed; //0x004C
	class N0003DD67* descrShortcuts; //0x0050
	int8_t muteAdvisor; //0x0054
	int8_t advancedStatsAlways; //0x0055
	int8_t microManageAllSettlements; //0x0056
	int8_t blindAdvisor; //0x0057
	int32_t terrainQuality; //0x0058
	int32_t vegetationQuality; //0x005C
	int8_t useQuickChat; //0x0060
	int8_t someBoolNetworkManager; //0x0061
	char pad_0062[1]; //0x0062
	int8_t someBoolOnlinePlay; //0x0063
	int32_t graphicsAdaptor; //0x0064
	int8_t byte_2C6D86C; //0x0068
	int8_t showDemeanour; //0x0069
	char pad_006A[2]; //0x006A
	int32_t radar; //0x006C
	int32_t unitCards; //0x0070
	int32_t sa_cards; //0x0074
	int32_t buttons; //0x0078
	int8_t tutorialBattlePlayed; //0x007C
	int8_t disableVnVs; //0x007D
	int8_t allUsers; //0x007E
	char pad_007F[29]; //0x007F
	int32_t unk_2C6D8A0; //0x009C
	char pad_00A0[28]; //0x00A0
	char* audioProvider; //0x00BC
	char pad_00C0[20]; //0x00C0
	struct UNICODE_STRING** campaignName; //0x00D4
	char pad_00D8[4]; //0x00D8
	struct UNICODE_STRING*** N0003DA7C; //0x00DC
	struct UNICODE_STRING*** maybeGameSpyUsername; //0x00E0
	struct UNICODE_STRING*** gameSpyPassword; //0x00E4
	char pad_00E8[4]; //0x00E8
	char* gameName; //0x00EC
	char pad_00F0[24]; //0x00F0
	char adminPassword[28]; //0x0108
	char* tutorialPath; //0x0124
}; //Size: 0x0128


struct options2
{
	uint16_t defaultCampaignResolutionX; //0x0000
	uint16_t defaultCampaignResolutionY; //0x0002
	uint16_t campaignResolutionX; //0x0004
	uint16_t campaignResolutionY; //0x0006
	uint16_t defaultBattleResolutionX; //0x0008
	uint16_t defaultBattleResolutionY; //0x000A
	uint16_t battleResolutionX; //0x000C
	uint16_t battleResolutionY; //0x000E
	int8_t idk; //0x0010
	int8_t vSync; //0x0011
	char pad_0012[6]; //0x0012
	int8_t uiIconBarCheck; //0x0018
	int8_t uiRadarCheck; //0x0019
	int8_t useMorale; //0x001A
	int8_t uiAmmoCheck; //0x001B
	int8_t useFatigue; //0x001C
	int8_t uiSupplyCheck; //0x001D
	int8_t toggleFowState; //0x001E
	int8_t cameraRestrict; //0x001F
	int8_t eventCutscenes; //0x0020
	char pad_0021[3]; //0x0021
	int32_t defaultCameraInBattle; //0x0024
	int8_t splashes; //0x0028
	int8_t vegetationVideo; //0x0029
	int8_t byte_1639F1A; //0x002A
	int8_t stencilShadows; //0x002B
	int8_t byte_1639F1C; //0x002C
	int8_t audioEnable; //0x002D
	int8_t speechEnable; //0x002E
	int8_t firstTimePlay; //0x002F
	char* audioProviderName; //0x0030
	char pad_0034[1]; //0x0034
	int8_t byte_1639F25; //0x0035
	char pad_0036[1]; //0x0036
	int8_t toggleAutoSave; //0x0037
	int8_t showBanners; //0x0038
	int8_t passwords; //0x0039
	int8_t hotseatTurns; //0x003A
	int8_t hotseatScroll; //0x003B
	int32_t allowValidationFeatures; //0x003C
	int32_t campaignSpeed; //0x0040
	int8_t labelSettlements; //0x0044
	int8_t disablePapalElections; //0x0045
	int8_t autoresolveAllBattles; //0x0046
	int8_t savePrefs; //0x0047
	int8_t disableConsole; //0x0048
	int8_t validateDiplomacy; //0x0049
	char pad_004A[2]; //0x004A
	float someFloat; //0x004C
	int32_t unitDetail; //0x0050
	int32_t buildingDetail; //0x0054
	int32_t maxSoldiersOnBattlefield; //0x0058
	int32_t unitSize; //0x005C
	int32_t cameraRotateSpeed; //0x0060
	int32_t cameraMoveSpeed; //0x0064
	float cameraSmoothing; //0x0068
	int32_t somethingBasedOnBuildingDetail; //0x006C
	int32_t masterVolume; //0x0070
	int32_t musicVolume; //0x0074
	int32_t speechVolume; //0x0078
	int32_t sfxVolume; //0x007C
	int32_t subFactionAccents; //0x0080
	int32_t playerFactionId; //0x0084
	int32_t campaignDifficulty; //0x0088
	int32_t battleDifficulty; //0x008C
	int32_t tgaWidth; //0x0090
	float tgaAspect; //0x0094
	int32_t tgaInputScale; //0x0098
	int32_t scrollMinZoom; //0x009C
	int32_t scrollMaxZoom; //0x00A0
	int32_t advisorVerbosity; //0x00A4
	int8_t useSomethingTgaTextures; //0x00A8
	int8_t byte_1639F99; //0x00A9
	char pad_00AA[2]; //0x00AA
	int32_t effectQuality; //0x00AC
	int32_t EnableCameraCampaignSmoothing; //0x00B0
	int32_t chatMsgDuration; //0x00B4
	int8_t N0003DDF9; //0x00B8
	int8_t saveGameSpyPassword; //0x00B9
	int8_t addDateToLogs; //0x00BA
	int8_t showToolTips; //0x00BB
	int8_t isNormalHud; //0x00BC
	int8_t showPackageLitter; //0x00BD
	char pad_00BE[2]; //0x00BE
	char* normal; //0x00C0
	char pad_00C4[8]; //0x00C4
	int32_t effectNormal; //0x00CC
	char pad_00D0[8]; //0x00D0
	char* low; //0x00D8
	char pad_00DC[8]; //0x00DC
	int32_t effectLow; //0x00E4
	char pad_00E8[8]; //0x00E8
	char* high; //0x00F0
	char pad_00F4[8]; //0x00F4
	int32_t effectHigh; //0x00FC
	char pad_0100[8]; //0x0100
	char* lowest; //0x0108
	char pad_010C[8]; //0x010C
	int32_t effectLowest; //0x0114
	char pad_0118[8]; //0x0118
	char* highest; //0x0120
	char pad_0124[8]; //0x0124
	int32_t effectHighest; //0x012C
	char pad_0130[8]; //0x0130
	char* custom; //0x0138
	char pad_013C[8]; //0x013C
	int32_t effectCustom; //0x0144
	char pad_0148[8]; //0x0148
	char* lower; //0x0150
	char pad_0154[8]; //0x0154
	int32_t effectLower; //0x015C
	char pad_0160[8]; //0x0160
	char* higher; //0x0168
	char pad_016C[8]; //0x016C
	int32_t effectHigher; //0x0174
	char pad_0178[8]; //0x0178
	char* off; //0x0180
	char pad_0184[8]; //0x0184
	int32_t effectOff; //0x018C
	char pad_0190[32]; //0x0190
	int32_t maxSizeEduMaybe1; //0x01B0
	float unitMultDefault; //0x01B4
	int32_t unitMultDefaultId; //0x01B8
	int32_t maxSizeEduMaybe2; //0x01BC
	float unitSizeMultiplierLow; //0x01C0
	int32_t unitMult1ID; //0x01C4
	int32_t maxSizeEduMaybe3; //0x01C8
	float unitSizeMultiplierMedium; //0x01CC
	int32_t unitMult2ID; //0x01D0
	int32_t maxSizeEduMaybe4; //0x01D4
	float unitSizeMultiplierLarge; //0x01D8
	int32_t unitMult3ID; //0x01DC
	char pad_01E0[12]; //0x01E0
	uint16_t word_163A0DC; //0x01EC
	char pad_01EE[10]; //0x01EE
	uint16_t word_163A0E8; //0x01F8
	char pad_01FA[2]; //0x01FA
	uint16_t word_163A0EC; //0x01FC
	char pad_01FE[54]; //0x01FE
	int16_t currentHotseatPlayerOrSomething; //0x0234
	char pad_0236[2]; //0x0236
	class N0003DF44* keysetPointer; //0x0238
	char pad_023C[4]; //0x023C
	int32_t keySetUsed; //0x0240
	int32_t preferencesWereLoaded; //0x0244
};


struct armyAndCharacter { /* in battle leader and leader army */
	struct stackStruct* army;//0x0000
	struct general* character;//0x0004
	int32_t generalNumKillsBattle; //0x0008
	float generalHPRatioLost; //0x000C
	float totalValue; //0x0010
	float battleOdds; //0x0014
	char pad_0018[12]; //0x0018
	struct general* killedGenerals; //0x0024
	int32_t killedGeneralsSize; //0x0028
	int32_t numKilledGenerals; //0x002C
	char pad_0030[24]; //0x0030
	struct general* generalsDied; //0x0048
	int32_t generalsDiedSize; //0x004C
	int32_t generalsDiedCount; //0x0050
	struct general* killedCaptains; //0x0054
	int32_t killedCaptainsSize; //0x0058
	int32_t killedCaptainsCount; //0x005C
	char pad_005C[60]; //0x005C
	struct battleUnit* units; //0x009C
	int32_t unitArraySize; //0x00A0
	int32_t unitCount; //0x00A4
	char pad_00A8[20]; //0x00A8
};//Size: 0x00BC

struct deploymentAreaS {
	undefined field_0x0[4];
	float* coordsPairs;
	undefined field_0x8[4];
	int coordsNum;
};

struct trackedPointerArmy {
	undefined field_0x0[4];//0x0000
	struct stackStruct* stack;//0x0004
	undefined field_0x8[44];//0x0008
	struct deploymentAreaS* deploymentArea;//0x0048
	undefined field_0x38[40];
};

struct aiDetachmentUnit
{
public:
	struct unit* unit; //0x0000
	DWORD objective; //0x0004
	char pad_0008[4]; //0x0008
}; //Size: 0x000C


struct aiDetachment
{
public:
	char pad_0000[4]; //0x0000
	struct AIBattleObjectiveBase* aiObjective; //0x0004
	struct aiDetachmentUnit** aiDetachUnits; //0x0008
	int32_t aiDetachUnitsSize; //0x000C
	int32_t aiDetachUnitsCount; //0x0010
	char pad_0014[8]; //0x0014
	DWORD objPhases; //0x001C
	int32_t objPhasesSize; //0x0020
	int32_t objPhasesCount; //0x0024
}; //Size: 0x0028


struct AIBattleObjectiveBase
{
public:
	char pad_0000[8]; //0x0000
	struct aiDetachment** aiDetachments; //0x0008
	int32_t aiDetachmentsSize; //0x000C
	int32_t aiDetachmentsCount; //0x0010
	struct unit** aiUnits; //0x0014
	int32_t aiUnitsSize; //0x0018
	int32_t aiUnitsCount; //0x001C
	char pad_0020[12]; //0x0020
	int32_t priority; //0x002C
	char pad_0030[24]; //0x0030
	DWORD battleFieldNode; //0x0048
	char pad_004C[20]; //0x004C
	struct aiDetachment** activeDetachments; //0x0060
	uint32_t activeDetachmentsEnd; //0x0064
	uint32_t activeDetachmentsEnd2; //0x0068
	char pad_006C[12]; //0x006C
}; //Size: 0x0078

struct aiObjectiveArrayBattle
{
public:
	struct AIBattleObjectiveBase* aIBattleObjective[50]; //0x0000
	char pad_00C8[16]; //0x00C8
}; //Size: 0x00D8

struct battleAI
{
public:
	char pad_0000[4]; //0x0000
	uint8_t newPlan; //0x0004
	char pad_0005[3]; //0x0005
	uint32_t currentAIPlan; //0x0008
	char pad_000C[8]; //0x000C
	char pad_0014[8]; //0x0014
	int32_t somethingHasUnitsagain; //0x001C
	char pad_0020[8]; //0x0020
	struct unit** aiUnitArray; //0x0028
	int32_t aiUnitMax; //0x002C
	int32_t aiUnitCount; //0x0030
	char pad_0034[4]; //0x0034
	struct unit** aiUnitArrayReversed; //0x0038
	int32_t aiUnitMaxReversed; //0x003C
	int32_t aiUnitCountReversed; //0x0040
	char pad_0044[340]; //0x0044
	struct unit** enemyUnitArray; //0x0198
	int32_t enemyUnitArraySize; //0x019C
	int32_t enemyUnitCount; //0x01A0
	char pad_01A4[4]; //0x01A4
	struct unit** enemyUnitArrayReversed; //0x01A8
	int32_t enemyUnitArraySizeReversed; //0x01AC
	int32_t enemyUnitCountReversed; //0x01B0
	char pad_01B4[1420]; //0x01B4
	struct unit** units; //0x0740
	int32_t unitArraySize; //0x0744
	int32_t unitCount; //0x0748
	char pad_074C[12]; //0x074C
	struct AIBattleObjectiveBase** currentObjectives; //0x0758
	int32_t currentObjectivesSize; //0x075C
	int32_t currentObjectivesCount; //0x0760
	char pad_0764[4]; //0x0764
	struct AIBattleObjectiveBase** addedObjectives; //0x0768
	int32_t addedObjectivesSize; //0x076C
	int32_t addedObjectivesCount; //0x0770
	char pad_0774[44]; //0x0774
	DWORD aiAnalysers; //0x07A0
	char pad_07A4[96]; //0x07A4
}; //Size: 0x0804

struct battleSide {
	bool isDefender;//0x0000
	uchar field_0x1;//0x0001
	bool isCanDeploy;//0x0002
	char pad_0003[1]; //0x0003
	uint32_t factionBitMap; //0x0004
	char pad_0008[4]; //0x0008
	DWORD winConditions[4];//0x000C
	int32_t winConditionCount; //0x001C
	char pad_0020[4]; //0x0020
	int32_t wonBattle; //0x0024 2 means won, 0 means lost, 1 is draw maybe
	int32_t battleSuccess; //0x0028 0 = close, 1 = average, 2 = clear, 3 = crushing
	char pad_002C[4]; //0x002C
	struct armyAndCharacter* forces;//0x0030
	int32_t armyArraySize; //0x0034
	int armiesNum;//0x0038
	char pad_003C[12]; //0x003C
	int32_t alliance; //0x0048
	struct battleAI* battleAIPlan; //0x004C
	char pad_0050[8]; //0x0050
	struct trackedPointerArmy armies[64];//0x0058
	int32_t armyAICount; //0x1858
	struct factionStruct* factions[8]; //0x185C
	int32_t factionCount; //0x187C
	char pad_1880[4]; //0x1880
	struct unit** units; //0x1884
	int32_t unitSize; //0x1888
	int32_t unitNum; //0x188C
	struct stackStruct** armies2; //0x1890
	int32_t armies2Size; //0x1894
	int32_t armies2Num; //0x1898 --often broken dont bind to lua
	char pad_189C[104]; //0x189C
	int32_t totalStrength; //0x1904
	int32_t totalValue; //0x1908
	char pad_190C[4]; //0x190C
	int32_t totalStrength2; //0x1910
	int32_t totalStrength3; //0x1914
	int32_t soldierCount; //0x1918
	float battleOdds; //0x191C
	float battleOdds2; //0x1920
	float someFloat; //0x1924
	char pad_1928[4]; //0x1928
};

struct playerArmy
{
public:
	struct stackStruct* army; //0x0000
	char pad_0004[20]; //0x0004
}; //Size: 0x0018

struct battleDataS {
	undefined field_0x0[4];//0x0000
	int battleState; /* 0-not in battle,5-active battle,9-results screen,etc 0x0004 */
	uint32_t battleType; //0x0008
	int32_t isNightBattle; //0x000C
	char pad_0010[4]; //0x0010
	int32_t battleResult; //0x0014
	int xCoord;//0x0018
	int yCoord;//0x001C
	undefined somethingRelatedToBattleType[16];//0x0020
	int attackerXCoord;//0x0030
	int attackerYCoord;//0x0034
	int defenderXCoord;//0x0038
	int defenderYCoord;//0x003C
	float N000037ED; //0x0040
	float N000037EE; //0x0044
	char pad_0048[4]; //0x0048
	int8_t N000037F0; //0x004C
	int8_t N0003A507; //0x004D
	int8_t inBattle; //0x004E
	int8_t N0003A508; //0x004F
	int32_t N000037F1; //0x0050
	int32_t paused; //0x0054
	float speed; //0x0058
	char pad_005C[8]; //0x005C
	float secondsPassed; //0x0064
	int32_t secondsSinceBattleLoaded; //0x0068
	char pad_006C[16]; //0x006C
	float mapWidthDoubled; //0x007C
	float mapHeightDoubled; //0x0080
	char pad_0084[12]; //0x0084
	int32_t hidingEnabledSet; //0x0090
	char pad_0094[4]; //0x0094
	struct UNICODE_STRING** someUniString; //0x0098
	struct battleSide sides[8]; //0x009C
	int32_t factionSide[31]; //0xC9FC
	int sidesNum;//0xCA78
	char pad_CA7C[4]; //0xCA7C
	int32_t soldiersLeft; //0xCA80
	char pad_CA84[36]; //0xCA84
	struct playerArmy playerArmies[30]; //0xCAA8
	char pad_CD78[4]; //0xCD78
	int32_t playerArmyCount; //0xCD7C
	char pad_CD80[84]; //0xCD80
	int32_t N0001D4DC; //0xCDD4
	int32_t N0001D4DD; //0xCDD8
	char pad_CDDC[20]; //0xCDDC
	int32_t N0001D4E3; //0xCDF0
	int32_t N0001D4E4; //0xCDF4
	char pad_CDF8[4]; //0xCDF8
	int32_t N0001D4E6; //0xCDFC
	int32_t N0001D4E7; //0xCE00
	char pad_CE04[44]; //0xCE04
	struct battleDataS* thisStruct; //0xCE30
};//Size: 0xCA7C

struct stratPortModel {
	struct model_Rigid* model_rigid;
	undefined field_0x4[4];
	char* modelPath;
	int modelPathCrypt;
};

//dock (water tile)
struct portDockStrat {
	undefined field_0x0[8];
	struct general* character; /* character on port (enemy or not, army or not) */
	int xCoord;
	int yCoord;
	undefined field_0x14[28];
	struct settlementStruct* settlement;
	undefined field_0x34[4];
	int tradeFleets;
	char pad_0040[8]; //0x0040
	struct stackStruct* occupantsArmy; /* if port block by army then it here */
	int32_t numTurnsBlocked; //0x004C
	int32_t N00008216; //0x0050
	int32_t N00008217; //0x0054
	int32_t rallyCoordX; //0x0058
	int32_t rallyCoordY; //0x005C
	void* rallyPoint; //0x0060
	struct factionStruct* fac; /* blockading faction becomes owner */
	undefined field_0x68[4];
	struct stackStruct* occupantsArmy2; /* if port block by army then it here */
	int fieldx70;
	struct stratPortModel* dockStratModel;
	struct portBuildingStruct* port;
	void* trackedPointerArmyVtbl3;
	struct stackStruct* someArmy3;
};

struct custom_tile {
	char* name;
	int nameCrypt;
	int x;
	int y;
	char** pathToModel_UNICODE;
	char* weather;
	int weatherCrypt;
	int timeCode;
};

struct modelFlexiMixed
{
	char structsize[0x12c];
};

struct modelFlexi
{
	char structsize[0x11c];
};

struct casTextures
{
	int textureIndex;
	int glossTextureIndex;
};

struct stratModelArrayEntry
{
public:
	DWORD model_flexi_m; //0x0000
	int32_t zeropoint; //0x0004
	int32_t zeroarray[6]; //0x0008
	float lodRange; //0x0020
	int32_t zeroarray2[9]; //0x0024
	struct casTextures modelTexturesFactionArray[31]; //0x0048
	int32_t somearray[33]; //0x0140
	int32_t somearray2[3]; //0x01C4
	float lod2; //0x01D0
	DWORD skeleton; //0x01D4
	float scale; //0x01D8
	int32_t modelCountNotShadow; //0x01DC
	float indevRangeSquared; //0x01E0
	DWORD shadow_model_flexi; //0x01E4
	int32_t zeroarray3[7]; //0x01E8
	float lod3; //0x0204
	int32_t zeroarray4[7]; //0x0208
	int32_t modelCountShadow; //0x0224
	int32_t zeroint; //0x0228
	char* typeName; //0x022C
	int32_t N54788969; //0x0230

}; //Size: 0x0234

struct stratModelListEntry
{
public:
	char* modelName; //0x0000
	int32_t crypt; //0x0004
	struct stratModelArrayEntry* stratModelEntry; //0x0008
}; //Size: 0x000C

struct descrCharacterStratModelArray
{
public:
	struct stratModelListEntry stratModelsArray[16]; //0x0000
}; //Size: 0x00C0


struct genMod {
	int type; /* 0-spy, 2 - diplomat, etc */
	struct descrCharacterStratModelArray* stratInfo;
	int modelCountArrayMax;
	int modelCount;
	char* card;
	undefined field_0x14[5];
	char* portrait;
	undefined field_0x1d[3];
	char* card_rebel;
	undefined field_0x24[4];
	char* portrait_rebel;
	undefined field_0x2c[4];
	struct genBattleMod* battleMod;
	undefined field_0x34[4];
};

//fort model struct on stratmap
struct stratFortMod {
	struct model_Rigid* centerModel;
	struct model_Rigid* wallsModel;
	char* centerModelPath;
	int centerModelPathCrypt;
	char* wallsModelPath;
	int wallsModelPathCrypt;
};

//resource model struct on stratmap
struct stratResMod {
	struct model_Rigid* model;
	char* patchToModel;
	undefined field_0x8[4];
	int resource_num;
	int resource_cost;
	char* tga;
	undefined field_0x18[4];
	int hasMine;
};

/* rigid model(cas model for "static" objects - settlements, resources, etc) on stratmap! */
struct model_Rigid { /* rigid model(cas model for "static" objects - settlements, resources, etc) on stratmap! */
	undefined field_0x0[4];
};

struct descrCharacterEntry
{
public:
	char* type; //0x0000
	char pad_0004[4]; //0x0004
	int32_t dictionary; //0x0008
	struct genMod* ptrsToDescrCharacterFactionEntries[31]; //0x000C
	struct genMod factionEntries[31]; //0x0088
	int32_t factionCount; //0x0750
	int8_t hasAction[40]; //0x0754
	int32_t wageBase; //0x077C
	float startingActionPoints; //0x0780
}; //Size: 0x0784

struct descrCharacterArray
{
public:
	int32_t entryCount; //0x0000
	char pad_0004[4]; //0x0004
	int32_t defaultStartingActionPoints; //0x0008
	char pad_000C[8]; //0x000C
	int16_t N0000416D; //0x0014
	char pad_0016[2]; //0x0016
	struct descrCharacterEntry entries[12]; //0x0018
}; //Size: 0x5A48


//trading resource on stratmap
struct resStrat {
	undefined field_0x0[12];
	int xCoord;
	int yCoord;
	undefined field_0x14[16];
	struct stratResMod* stratMod;
	struct settlementStruct* settlement;
	int regionID;
	int resourceID;
	undefined field_0x2c[12];
};

//settlement model struct
struct settMod {
	struct model_Rigid* town;
	struct model_Rigid* castle;
	undefined field_0x8[924];
};

//models on the stratmap (settlements, resources, etc)
struct stratMod {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
};

//port
struct portBuildingStruct {
	undefined field_0x0[8];
	struct general* character; /* character on port (enemy or not, army or not) */
	int xCoord;
	int yCoord;
	undefined field_0x14[28];
	struct settlementStruct* settlement;
	undefined field_0x34[4];
	int tradeFleets;
	char pad_0040[8]; //0x0040
	struct stackStruct* occupantsArmy; /* if port block by army then it here */
	int32_t numTurnsBlocked; //0x004C
	int32_t N00008216; //0x0050
	int32_t N00008217; //0x0054
	int32_t rallyCoordX; //0x0058
	int32_t rallyCoordY; //0x005C
	void* rallyPoint; //0x0060
	struct factionStruct* fac; /* blockading faction becomes owner */
	undefined field_0x68[4];
	struct stackStruct* occupantsArmy2; /* if port block by army then it here */
	int fieldx70;
	struct stratPortModel* portStratModel;
	struct portDockStrat* portDock;
	undefined field_0x7c[4];
};


struct BuildingPicEntry
{
public:
	char* buildingPicPath; //0x0000
	char pad_0004[4]; //0x0004
}; //Size: 0x0008

//building draw info(pics, etc)
struct buildingLevel { /* (name, tga's, models, etc) */
	char* name;
	int32_t buildingLevelNameHash; //0x0004
	UNICODE_STRING*** buildingName[31]; //0x0008
	UNICODE_STRING*** buildingDescr[31]; //0x0084
	UNICODE_STRING*** buildingDescrShort[31]; //0x0100
	struct BuildingPicEntry buildingPic[7]; //0x017C
	char* GenericBuildingPic; //0x01B4
	int32_t GenericBuildingPicHash; //0x01B8
	struct BuildingPicEntry buildingPicConstructed[7]; //0x01BC
	char* GenericbuildingPicConstructed; //0x01F4
	int32_t GenericbuildingPicConstructedHash; //0x01F8
	struct BuildingPicEntry buildingPicConstruction[7]; //0x01FC
	char* GenericbuildingPicConstruction; //0x0234
	int32_t GenericbuildingPicConstructionHash; //0x0238
	void* buildingLevelCondition; //0x023C
	int16_t buildCost; //0x0240
	uint8_t buildTime; //0x0242
	char pad_0243[1]; //0x0243
	uint32_t settlementMinLvl; //0x0244
	int8_t cityOneCastleMinusOne; //0x0248
	char pad_0249[19]; //0x0249
	struct BuildingLvlCapability* capabilities; //0x025C
	struct recruitPool* recruitPools; //0x0260
	int32_t factionCapabilities; //0x0264
	void* upgrades; //0x0268
};


struct BuildingLvlCapability
{
public:
	int32_t capabilityType; //0x0000
	int16_t capabilityLvl; //0x0004
	int16_t bonus; //0x0006
	int32_t capabilityID; //0x0008
	DWORD funcPointer;
	struct buildingLevel* buildingLevel;
	DWORD EDBpointer;
	void* buildingLevelCondition; //0x0018
	struct BuildingLvlCapability* nextCapability; //0x001C
}; //Size: 0x0020


struct recruitPool
{
public:
	int32_t capabilityType; //0x0000
	int32_t capabilityLvlorExp; //0x0004
	int32_t unitID; //0x0006
	float initialSize;
	float gainPerTurn;
	float maxSize;
	void* buildingLevelCondition; //0x0018
	struct recruitPool* nextPool; //0x001C
}; //Size: 0x0020


//siege engine
struct siegeEngine {
	undefined field_0x0[172];
	int type; /* 2-RAM,4-ladder,1-tower */
};

//building data
struct edbEntry { /* SETTLEMENT_CONDITION_SETTLEMENT_FACTION */
	char pad_0000[20]; //0x0000
	int32_t buildingID; //0x0014
	int32_t classification; //0x0018
	char pad_001C[68]; //0x001C
	int8_t isCoreBuilding; //0x0060
	int8_t isPort; //0x0061
	int8_t isCoreBuilding2; //0x0062
	int8_t hasReligion; //0x0063
	int32_t religionID; //0x0064
	int8_t isHinterland; //0x0068
	int8_t isFarm; //0x0069
	char pad_006A[6]; //0x006A
	UNICODE_STRING** localizedName;
	void* convertTo; //0x0074
	char pad_0078[8]; //0x0078
	char* type; /* type of building (core_building,barracks)  */
	int typeHash;
	struct buildingLevel* buildingLevel; /* name of building(stone_wall), tga's, etc */
	char pad_008C[4]; //0x008C
	int32_t buildingLevelCount; //0x0090
	char pad_0094[8]; //0x0094
};

//building
struct building { /* building structure */
	undefined field_0x0[56];
	struct edbEntry* edbEntry; /* many info (type, name, etc), exportDescrBuildingEntry */
	int8_t level;
	undefined field_0x3d[27];
	int hp; /* health points of the building */
	struct settlementStruct* settlement;
};

struct buildingInQueue { /* building in settlement queue */
	int someID;
	struct edbEntry* edbEntry;
	struct building* existsBuilding; /* 0 if building dont exist yet */
	struct settlementStruct* settlement;
	int currentLevel;
	int pastLevel;
	int buildCost;
	int something2;
	int turnsToBuild;
	int buildTurnsPassed;
	int percentBuild;
	int8_t turnNumber;
	char pad[3];
	int buildCost2;
	int turnsToBuild2;
	void* texture;
};

struct buildingsQueue {
	struct buildingInQueue items[6];
	int firstIndex;
	int lastIndex;
	int buildingsInQueue;
	int currentBuildingIndex;
};

struct hiddenResource
{
public:
	char* hiddenResName; //0x0000
	int32_t hiddenResNameHash; //0x0004
}; //Size: 0x0008

struct buildingsArray
{
public:
	struct edbEntry buildings[64]; //0x0000
};

struct buildingListPointer
{
public:
	struct buildingsArray* buildingsArray; //0x0000
	struct buildingListPointer* nextBuildingsListPointer; //0x0004
	struct buildingListPointer* prevBuildingsListPointer; //0x0008
	int32_t arraySize; //0x000C
	int32_t arrayCount; //0x0010
}; //Size: 0x0014

struct exportDescrBuildings
{
public:
	struct hiddenResource hiddenResources[64]; //0x0000
	int32_t hiddenResourceCount; //0x0200
	char pad_0204[68]; //0x0204
	struct edbEntry* cityPort; //0x0248
	struct edbEntry* castlePort; //0x024C
	struct edbEntry* coreCityBuilding; //0x0250
	struct edbEntry* coreCastleBuilding; //0x0254
	char pad_0258[40]; //0x0258
	struct buildingListPointer buildingsList; //0x0280
	char pad_0294[188]; //0x0294
}; //Size: 0x0350


struct oneSiege {
	void* vtable;
	struct siegeS* siege;
};

struct unitRQ {
public:
	struct eduEntry* eduEntry; //0x0000
	int32_t FourtySeven; //0x0004
	struct settlementStruct* settlement; //0x0008
	int8_t N0001082F; //0x000C
	int8_t recruitType; //0x000D
	int8_t experience; //0x000E
	int8_t armourUpg; //0x000F
	int8_t weaponUpgrade; //0x0010
	int8_t Minus1; //0x0011
	int8_t turnsTrainedAlready; //0x0012
	int8_t percentFinished; //0x0013
	int8_t costUpkeep; //0x0014
	int8_t turnsToTrain; //0x0015
	int16_t cost; //0x0016
	int16_t soldierCount; //0x0018
	int16_t turnNumber; //0x001A
	int8_t fieldx1C; //0x001C
	int8_t isNotFrozen; //0x001D
	int16_t recruitmentPoolSizeBeforeOrRetrainingNumbersBefore; //0x001E
	int8_t isRetraining; //0x0020
	char pad_0021[3]; //0x0021
}; //Size: 0x0024


struct RallyPointSundry {
	undefined field0_0x0[4];
	void* object;
	undefined field2_0x8[56];
	void* object2;
	undefined field4_0x44[16];
};

struct settlementCapability
{
public:
	int16_t value; //0x0000
	int16_t bonus; //0x0002
}; //Size: 0x0004

struct recruitCapRome
{
public:
	char pad[8];
}; //Size: 0x0008

struct recruitmentCapability
{
public:
	int32_t eduIndex; //0x0000
	int32_t xp; //0x0004
	float initialSize; //0x0008
	float replenishRate; //0x000C
	float maxSize; //0x0010
}; //Size: 0x0014

struct settlementRecruitmentPool
{
public:
	int32_t eduIndex; //0x0000
	float availablePool; //0x0004
}; //Size: 0x0008


struct recruitRome
{
	char pad[8];
};

struct capabilityStruct
{
public:
	struct settlementCapability settlementCapabilities[57]; //0x0000
	struct settlementCapability settlementCapabilitiesAgent[6]; //0x00E4
	struct settlementCapability settlementCapabilitiesAgentLimit[6]; //0x00FC
	struct settlementCapability settlementCapabilitiesAncillaryROME[4]; //0x0114
	struct recruitRome settlementCapabilitiesRecruitROME[64]; //0x0124
	int32_t recruitCapabilityCount; //0x0324
	struct recruitmentCapability recruitmentCapabilities[64]; //0x0328
	int32_t recruitmentCapabilityCount; //0x0828
}; //Size: 0x082C


struct settlementStats
{
	struct settlementStruct* settlement;
	int32_t PopGrowthBaseFarm;
	int32_t PopGrowthFarms;
	int32_t PopGrowthHealth;
	int32_t PopGrowthBuildings;
	int32_t PopGrowthTaxBonus;
	int32_t PopGrowthEntertainment;
	int32_t PopGrowthTrade;
	int32_t PopGrowthGovernorInfluence;
	int32_t PopGrowthSqualor;
	int32_t PopGrowthPlague;
	int32_t PopGrowthTaxPenalty;
	int32_t population;
	int32_t PublicOrderGarrison;
	int32_t PublicOrderLaw;
	int32_t PublicOrderBuildingsEntertainment;
	int32_t PublicOrderGovernorInfluence;
	int32_t PublicOrderTaxBonus;
	int32_t PublicOrderTriumph;
	int32_t PublicOrderPopulationBoom;
	int32_t PublicOrderEntertainment;
	int32_t PublicOrderHealth;
	int32_t PublicOrderGarrisonTwo;
	int32_t PublicOrderFear;
	int32_t PublicOrderGlory;
	int32_t PublicOrderSqualor;
	int32_t PublicOrderDistanceToCapital;
	int32_t PublicOrderNoGovernance;
	int32_t PublicOrderTaxPenalty;
	int32_t PublicOrderUnrest;
	int32_t PublicOrderBesieged;
	int32_t PublicOrderBlockaded;
	int32_t PublicOrderCulturalUnrest;
	int32_t PublicOrderExcommunication;
	int32_t PublicOrder;
	int32_t FarmsIncome;
	int32_t TaxesIncome;
	int32_t MiningIncome;
	int32_t TradeIncome;
	int32_t DiplomaticIncome;
	int32_t DemolitionIncome;
	int32_t LootingIncome;
	int32_t BuildingsIncome;
	int32_t AdminIncome;
	int32_t N000025DD;
	int32_t upkeepExpense;
	int32_t ConstructionExpense;
	int32_t RecruitmentExpense;
	int32_t DiplomaticExpense;
	int32_t CorruptionExpense;
	int32_t EntertainmentExpense;
	int32_t DevastationExpense;
	int32_t TotalIncomeWithoutAdmin;
	int32_t* religionPercentages;
	int32_t religionPercentagesSize;
	int32_t religionPercentagesNum;
	int32_t majorityReligionID;
};

//settlement
struct settlementStruct {
	void* vTable;
	undefined field1_0x4[8];
	UINT32 xCoord; /* x coordinate */
	UINT32 yCoord; /* y coordinate */
	undefined field4_0x14[32];
	float bannerPosX;
	float bannerPosZ;
	float bannerPosY;
	struct trackedPointerArmy_vtbl* trackedPointerArmyVtable;
	struct stackStruct* army; /* army on the settlement */
	struct oneSiege sieges[8];
	BYTE siegesNumber;
	undefined pad_3[3];
	int siegeHoldoutTurns;
	int turnsSieged;
	int fieldx94;
	int populationSiegeStart;
	int32_t fieldx9C;
	int32_t somethingPlagueRelated;
	BYTE plagued;
	int8_t bytexA5;
	int8_t bytexA6;
	int8_t bytexA7;
	int32_t fieldxA8;
	struct descrRebelEntry* descrRebel; //0x00AC
	int32_t subFactionID; //0x00B0
	char pad_00B4[152]; //0x00B4
	struct settMod* model;
	int cultureID;
	undefined field11_0x154[12];
	struct portBuildingStruct* port;
	char* name; /* internal name of settlement */
	int nameCrypt;
	UNICODE_STRING** localizedName;
	struct factionStruct* faction; /* faction of the owner */
	undefined field16_0x174[36];
	int level; /* level of the settlement/castle */
	int fac_creatorModNum;
	undefined field19_0x1a0[4];
	BYTE isCastle; /* castle or settlement */
	undefined field21_0x1a5[1];
	BYTE turnsOwned; /* start at 10 for settlements owned at game start without specification in descr_strat */
	undefined field21_0x1a7[1];
	UINT32 regionID; /* number of region */
	int32_t factionID; //0x01AC
	int32_t yearFounded; //0x01B0
	int32_t triumph; //0x01B4
	int queueCost;
	undefined field24_0xBC[24];
	struct unitRQ unitQueue[9];
	int startIndexRQ;
	int endIndexRQ;
	int countRQ;
	undefined field24_0x324[272];
	struct buildingsQueue buildingsQueueArray;
	undefined field26_0x5ac[520];
	struct building* buildings[128];
	int buildingsNum; /* number of the buildings in the settlement */
	undefined field29_0x9b8[1080];
	int32_t rallyPointX; //0x0DF0
	int32_t rallyPointY; //0x0DF4
	void* rallyPointPointer; //0x0DF8
	struct aiProductionController* aiProductionController; //0x0DFC
	int32_t plagueDeaths; //0x0E00
	int8_t scriptRebel; //0x0E04
	uchar isProvokedRebellion;//0x0E05
	uchar field31_0xe06[10];//0x0E06
	int8_t isCapital; //0x0E10
	char pad_0E11[15]; //0x0E11
	struct general* governor;//0x0E20
	char pad_0E24[16]; //0x0E24
	int32_t somthingPO; //0x0E34
	int32_t publicOrderLastTurnEnd; //0x0E38
	int32_t populationLastTurnEnd; //0x0E3C
	int32_t harvestSuccess; //0x0E40
	struct resStrat** resources;
	int32_t resourceSize; //0x0E48
	int resourcesNum;
	int32_t settlementTaxLevel; //0x0E50
	struct capabilityStruct capabilities; //0x0E54
	int8_t N000089C8; //0x1680
	char pad_1681[3]; //0x1681
	struct settlementStruct* thisSett; //0x1684
	struct settlementRecruitmentPool* recruitmentPools; //0x1688
	int32_t recruitmentPoolsSize; //0x168C
	int32_t recruitmentPoolCount; //0x1690
	char pad_1694[12]; //0x1694
	int8_t freezeRecruitmentPool; //0x16A0
	char pad_16A1[3]; //0x16A1
	int32_t spiesInRecruitmentQueue; //0x16A4
	int32_t assassinsInRecruitmentQueue; //0x16A8
	int32_t diplomatsInRecruitmentQueue; //0x16AC
	int32_t admiralsInRecruitmentQueue; //0x16B0
	int32_t merchantsInRecruitmentQueue; //0x16B4
	int32_t priestsInRecruitmentQueue; //0x16B8
	char pad_16BC[8]; //0x16BC
	intptr_t guildStandings;
	char pad_16C8[48]; //0x16C8
	int32_t baseFertilityValue; //0x16F8
	int32_t rebelFactionChance; //0x16FC
	int32_t turmoil; //0x1700
	int32_t N00002567; //0x1704
	int32_t N00002568; //0x1708
	int32_t N00002569; //0x170C
	int32_t N0000256A; //0x1710
	int32_t N0000256B; //0x1714
	int32_t N0000256C; //0x1718
	int32_t N0000256D; //0x171C
	int32_t N0000256E; //0x1720
	int32_t diplomacyIncome; //0x1724
	int32_t otherIncome1; //0x1728
	int32_t wages; //0x172C
	int32_t upkeep; //0x1730
	int fieldx1734;
	int fieldx1738;
	int diplomacyExpense;
	int fieldx1740;
	int fieldx1744;
	struct settlementStats settlementStatsLastTurn;
	struct settlementStats settlementStats;
	int32_t alternateTradeCalc; //0x1910
};

struct campaignDifficulty1
{
	int orderFromGrowth;
	int considerWarWithPlayer;
	float difficultyFloat1_unused;
	float brigandChanceAi;
	float brigandChancePlayer;
	int forceAttackDelay;
	float taxIncomeModifierPlayer;
	float farmingIncomeModifierPlayer;
	float incomeModifierAi;
	float playerRegionValueModifier;
};

struct campaignDifficulty2
{
public:
	int32_t popGrowthBonusAi; //0x0000
	int32_t publicOrderBonusAi; //0x0004
	int32_t experienceBonusAi; //0x0008
	int32_t difficultyInt4_unused; //0x000C
	int32_t incomeBonusAi; //0x0010
	int8_t wantsTargetPlayer; //0x0014
	int8_t wantsTargetPlayerNaval; //0x0015
	char pad_0016[6]; //0x0016
	int32_t autoAttackPlayerIfCrusadeTarget; //0x001C
	char pad_0020[32]; //0x0020
}; //Size: 0x0040



struct guild
{
	char* name; //0x0000
	DWORD nameHash; //0x0004
	uint8_t id; //0x0008
	char pad_0009[3]; //0x0009
	void* building; //0x000C
	int32_t level1Threshold; //0x0010
	int32_t level2Threshold; //0x0014
	int32_t level3Threshold; //0x0018
	char pad_001C[4]; //0x001C
	void* excludedGuildIds; //0x0020
	char pad_0024[40]; //0x0024
}; //Size: 0x004C

/* I_CompareCounter script command */
struct CompareCounter { /* I_CompareCounter script command */
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	char* commandName; /* name of command(I_CompareCounter) */
	int operation;
	char* operandName; /* name of checked counter */
	int operandNameCrypt; /* maybe crypt of the counter name */
	int checkedValue; /* value for check */
};

//main character data(coordinates, etc)
struct general { /* character on the stratmap, who has a unit in a stack */
	undefined field0_0x0[4];
	void* obj;
	undefined field2_0x8[4];
	int xCoord; /* number of x-coord of unit fosition */
	int yCoord; /* number of y-coord of unit fosition */
	undefined field5_0x14[108];
	struct namedCharacter* genChar; /* many important info about character */
	undefined field7_0x84[4];
	struct genMod* genType;
	undefined field9_0x8c[24];
	uchar ifMarkedToKill;
	undefined field11_0xa5[1];
	int8_t inEnemyZOC; //0x00A6
	char pad_00A7[17]; //0x00A7
	int ambushState;
	char pad_00B9[3]; //0x00B9
	int32_t actionsThisTurn; //0x00BC
	char pad_00C0[4]; //0x00C0
	int8_t N000013B1; //0x00C4
	int8_t doNotSpendMovePoints; //0x00C5
	char pad_00C6[2]; //0x00C6
	float movePointsCharacter;
	undefined field15_0xcc[4];
	uchar isStopCharacterNeeded; /* set to 1 if character moving now and he stops */
	int32_t numTurnsIdle; //0x00D4
	int32_t regionID; //0x00D8
	float percentCharacterReligionInRegion; //0x00DC
	float popConvertedThisTurn; //0x00E0
	int32_t timeInRegion; //0x00E4
	int32_t timeWithArmy; //0x00E8
	char pad_00EC[20]; //0x00EC
	struct stackStruct* armyLeaded; /* army of the general */
	undefined field19_0x104[4];
	struct unit* bodyguards; /* unit of general */
	struct stackStruct* armyNotLeaded; /* army, if not leader */
	undefined field22_0x110[208];
	undefined field23_0x1e0[4];
	float movePointsModifier;
	float movePointsMax;
	float movePointsArmy;
	float movePointsMaxArmy; //0x01F0
	struct crusade* crusade; //0x01F4
	int32_t turnJoinedCrusade; //0x01F8
	int32_t currentTurn; //0x01FC
	float distanceToCrusadeTarget;
	float floatx204_startMinus1;
	float floatx208_startMinus1;
	float floatx20C_startMinus1;
	float floatx210_startMinus1;
	float floatx214_startMinus1;
	float floatx218_startMinus1;
	float floatx21C_startMinus1;
	float floatx220_startMinus1;
	float floatx224_startMinus1;
	int32_t disbandProgress;
	int8_t isCrusadeDisbandActive;
	int8_t bytex22D;
	char pad_0xx[2];
	char* ability;
	int32_t fieldx234;
	struct general* thisCharacter; //0x0238
	char pad_023C[16]; //0x023C
	struct general* thisCharacter2; //0x024C
	int32_t xCoordLast; //0x0250
	int32_t yCoordLast; //0x0254
	int32_t xCoordCurrent; //0x0258
	int32_t yCoordCurrent; //0x025C
	struct settlementStruct* besiegingSettlement; //0x0260
	char pad_0264[12]; //0x0264
	struct general* besiegingCharacter; //0x0270
	int32_t N00032B12; //0x0274
	int32_t N00032B00; //0x0278
	int32_t N00032B13; //0x027C
	int32_t N00032B14; //0x0280
	char pad_0284[296]; //0x0284
	void* someCoordStruct; //0x03AC
	int32_t someCoordStructSize; //0x03B0
	int32_t someCoordStructCount; //0x03B4
	struct movePath* multiTurnMovePaths; //0x03B8
	int8_t multiTurnMovePathsSize; //0x03BC
	int8_t multiTurnMovePathsCount; //0x03BD
	int8_t N00032E71; //0x03BE
	int8_t N00032E76; //0x03BF
};

//additional character data(name,label,traits, etc)
struct namedCharacter { /* many important info about character */
	UINT32 index; /* index of character */
	UNICODE_STRING** localizedFullName; /* displaying name */
	UNICODE_STRING** localizedNameForSave; /* saved to save file */
	UNICODE_STRING** localizedNextNameForSave; /* saved to save file */
	UNICODE_STRING** localizedNicknameForSave; /* saved to save file */
	char* shortName; /* not a full name) */
	undefined field_0x18[12];
	char* fullName; /* full name of character) */
	undefined field_0x28[4];
	char* label; /* label of character */
	UINT32 labelCrypt; /* crypt  of the label */
	char* modelName; /* custom model */
	undefined field_0x38[4];
	uchar status; /* 5-leader,2 - heir, 0 - ordinary man */
	undefined field_0x3d[7];
	int32_t authority; /* command */
	int32_t influence;
	int32_t subterfuge;
	int32_t loyality;
	int32_t charm;
	int32_t finance;
	int32_t piety;
	int32_t magic;
	int32_t nobility; /* chivalry/dread */
	int32_t leaderAutority; /* authority of leader */
	int32_t level;
	int32_t unorthodoxy; //0x0070
	int32_t heresyImmunity; //0x0074
	int32_t assassination; //0x0078
	int32_t sabotage; //0x007C
	int32_t eligibility; //0x0080
	int32_t purity; //0x0084
	int32_t violence; //0x0088
	int32_t disposition; //0x008C
	int32_t boldness; //0x0090
	int32_t generosity; //0x0094
	int32_t management; //0x0098
	int32_t bodyguardSize; //0x009C
	int32_t troopMorale; //0x00A0
	int32_t movementPointsBonus; //0x00A4
	int32_t attack; //0x00A8
	int32_t defence; //0x00AC
	int32_t siegeAttack; //0x00B0
	int32_t siegeDefense; //0x00B4
	int32_t ambush; //0x00B8
	int32_t navalCommand; //0x00BC
	int32_t siegeEngineering; //0x00C0
	int32_t nightBattle; //0x00C4
	int32_t personalSecurity; //0x00C8
	int32_t publicSecurity; //0x00CC
	int32_t bribery; //0x00D0
	int32_t bribeResistance; //0x00D4
	int32_t electability; //0x00D8
	int32_t lineOfSight; //0x00DC
	int32_t trainingUnits; //0x00E0
	int32_t trainingAgents; //0x00E4
	int32_t construction; //0x00E8
	int32_t trading; //0x00EC
	int32_t localPopularity; //0x00F0
	int32_t footInTheDoor; //0x00F4
	int32_t farming; //0x00F8
	int32_t mining; //0x00FC
	int32_t taxCollection; //0x0100
	int32_t fertility; //0x0104
	int32_t cavalryCommand; //0x0108
	int32_t infantryCommand; //0x010C
	int32_t gunpowerCommand; //0x0110
	int32_t artilleryCommand; //0x0114
	int32_t combatVsReligion[10]; //0x0118
	int32_t combatVsFaction[31]; //0x0140
	int32_t field_01BC; //0x01BC
	int32_t health; //0x01C0
	int32_t squalor; //0x01C4
	int32_t unrest; //0x01C8
	int32_t law; //0x01CC
	int32_t looting; //0x01D0
	int32_t bodyguardValour; //0x01D4
	int32_t hitpoints; //0x01D8
	int32_t trainingAnimalUnits; //0x01DC
	int32_t battleSurgery; //0x01E0
	struct traitContainer* traits; /* names at [item number] -0x4-here. Not have number, read it in while(traits != 0){this->m_memory->Read(traits + 0x08, 4).toINT32();} */
	undefined field_0x1e8[4];
	struct ancData** ancillaries; /* pointers to character ancillaries, names at  [item number] -0-0c-here) */
	undefined field_0x1f0[4];
	UINT32 ancNum; /* number of character  ancillaries */
	struct general* gen; /* on stratmap */
	undefined field_0x1fc[8];
	float yearOfBirth; /* yearOfBirth */
	undefined field_0x208[16];
	struct factionStruct* faction;
	int subFaction;
	undefined field_0x220[4];
	struct namedCharacter* parent; /* father */
	struct namedCharacter* spouse;
	struct namedCharacter* childs[4]; /* children, womens wont have children */
	char pad_023C[16];
	int32_t isDead;
	char* portrait;
	undefined field_0x254[4];
	char* portrait2;
	undefined field_0x25c[8];
	char* portrait_custom; /* custom portrait name */
	undefined field_0x268[4];
	int age;
};

//ancillary of character
struct ancillary { /* structure of ancillary */
	UINT32 index;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	char* ancName;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	char* patchToAncImage;
	char pad_0018[4]; //0x0018
	int8_t isUnique; //0x001C
	char pad_001D[23]; //0x001D
	uint32_t excludedCultures; //0x0034
};

struct trait { /* traits of the character */
	struct traitEntry* traitEntry;
	struct traitLevel* level; /* level of trait */
	int traitPoints;
	int someInt;
	int somePointer;
};

struct traitEffect
{
	int32_t effectID;
	int32_t value;
};

struct traitLevel
{
	int32_t level;
	struct UNICODE_STRING*** localizedName;
	struct UNICODE_STRING*** localizedDescription;
	struct UNICODE_STRING*** localizedEpithetDescription;
	struct UNICODE_STRING*** localizedGainDescription;
	struct UNICODE_STRING*** localizedLoseDescription;
	int32_t threshold;
	struct traitEffect* effects;
	int32_t effectsSize;
	int32_t effectsCount;
	struct UNICODE_STRING*** localizedEffectsDescription;
};

struct traitEntry { /* char* at 0x4 */
	int32_t index;
	char* name;
	int32_t nameHash;
	struct traitLevel levels[10];
	int32_t levelCount;
	struct traitEntry* antiTraits[20];
	int32_t antiTraitCount;
	int32_t characterTypeNum;
	uint32_t characterType;
	int32_t noGoingBackLevel;
	int32_t excludeCulturesNum;
	uint32_t excludeCulturesStart;
	int32_t hidden;
	struct stringWithHash antiTraitNames[20];
	int32_t antiTraitNameCount;
};

struct traitContainer {
	struct trait* trait;
	struct traitContainer* prev;
	struct traitContainer* next;
	int hasEpithet;
};

struct trackedPointerUnit {
	undefined field_0x0[4];
	struct unit* unit;
	undefined field_0x8[88];
};

struct engineRecord {
	undefined field_0x0[4];
	char* type;
	undefined field_0x8[172];
	int classID;
	undefined field_0xb8[564];
};

struct soldierInBattle {
	undefined field_0x0[8];
	int32_t unitCategory; //0x0008
	int16_t someFlags; //0x000C
	int8_t N00008AC5; //0x000E
	int8_t N00008AC7; //0x000F
	float mass; //0x0010
	float inverseMass; //0x0014
	float xCoord;
	float zCoord;
	float yCoord;
	struct soldierInBattle* self; //0x0024
	float someRadius1; //0x0028
	float someRadius2; //0x002C
	float someRadius3; //0x0030
	float markerRadius; //0x0034
	float height; //0x0038
	char pad_003C[8]; //0x003C
	int16_t rotation; //0x0044
	char pad_0046[2]; //0x0046
	float velocity1;
	float velocity2;
	float velocity3;
	char pad_0054[196]; //0x0054
	struct soldierInBattle* self2; //0x0118
	char pad_011C[4]; //0x011C
	float speed; //0x0120
	char pad_0124[4]; //0x0124
	float xCoord_writable;
	float zCoord_writable;
	float yCoord_writable;
	uint16_t angle_writable; //0x0134
	int8_t N0000713A; //0x0136
	int8_t N00009525; //0x0137
	float velocityModifier; //0x0138
	int32_t currentStateGroupID; //0x013C
	int32_t currentStateStateID; //0x0140
	int32_t pendingStateGroupID; //0x0144
	int32_t pendingStateStateID; //0x0148
	char pad_014C[4]; //0x014C
	float xCoord_destination;
	float zCoord_destination;
	float yCoord_destination;
	int16_t destinationAngle; //0x015C
	int16_t thisIsAngleCauseTwoByteMaybe; //0x015E
	float destinationRadius; //0x0160
	char pad_0164[16]; //0x0164
	int16_t N00001DC6; //0x0174
	char pad_0176[2]; //0x0176
	int32_t targetStateGroupID; //0x0178
	int32_t targetStateStateID; //0x017C (One = Standing, 2 = Walking, 3 = running)
	int32_t moveStateGroupID; //0x0180
	int32_t moveStateStateID; //0x0184 (2 = walking, 3 = running)
	int32_t finishStateGroupID; //0x0188
	int32_t finishStateStateID; //0x018C
	int32_t options; //0x0190
	int32_t pendingAction; //0x0194
	float stoppingModifier; //0x0198
	float slideRotation; //0x019C
	int32_t numTicks; //0x01A0
	char pad_01A4[20]; //0x01A4
	int32_t surfaceID; //0x01B8
	int16_t pathHistoryAndObstacleDirection; //0x01BC
	int16_t unk; //0x01BE
	char pad_01C0[16]; //0x01C0
	int16_t waypoint; //0x01D0
	char pad_01D2[18]; //0x01D2
	int16_t cone; //0x01E4
	int16_t directionOffset; //0x01E6
	int16_t blockedCounter; //0x01E8
	int16_t N0000954E; //0x01EA
	float radiusModifier; //0x01EC
	float speedModifier; //0x01F0
	int32_t deflectionCounter; //0x01F4
	float stoppingDistance; //0x01F8
	float externalModifier; //0x01FC
	uint32_t locomotionFlags; //0x0200
	char pad[960]; //0x01D2

};

struct generalInfo
{
public:
	char** generalModelName; //0x0000
	struct unit* unit; //0x0004
	struct namedCharacter* namedChar; //0x0008
	char pad_000C[48]; //0x000C
	int8_t N00010FEA; //0x003C
	char pad_003D[43]; //0x003D
	struct soldierInBattle* soldier; //0x0068
	int8_t isCharacter; //0x006C
	int8_t alive; //0x006D
	char pad_006E[2]; //0x006E
	struct heroAbility* ability; //0x0070
	char pad_0074[144]; //0x0074
}; //Size: 0x0104

struct floatPosData
{
public:
	float rotStart; //0x0000
	float coord2_1; //0x0004
	float coord2_2; //0x0008
	float coord3_2; //0x000C
	float coord3_1; //0x0010
	float coord0_2; //0x0014
	float coord0_1; //0x0018
	float unk1; //0x001C
	float unk2; //0x0020
	float unk; //0x0024
	float rotation1; //0x0028
	float rotation2; //0x002C
	float coord1_1; //0x0030
	float coord1_2; //0x0034
	float val15; //0x0038
	float rotValue1; //0x003C
	float rotValue2; //0x0040
}; //Size: 0x0044

struct targetUnitStruct
{
public:
	char pad_0000[4]; //0x0000
	struct unit* target; //0x0004
	char pad_0008[4]; //0x0008
	int32_t targetUnitIndex; //0x000C
	char pad_0010[4]; //0x0010
}; //Size: 0x0014

struct targetPos
{
public:
	int32_t typeOrSomething; //0x0000
	int32_t N0001B8DA; //0x0004
	void* unitVerificationTable; //0x0008
	struct targetUnitStruct* targetUnit; //0x000C
	char pad_0010[4]; //0x0010
	float targetPosXStart; //0x0014
	float targetPosYStart; //0x0018
	char pad_001C[16]; //0x001C
	int32_t noDraggedFormation; //0x002C
	float N0001B8E5; //0x0030
	float N0001B8E6; //0x0034
	char pad_0038[4]; //0x0038
	float somethingDistanceRelated; //0x003C
	int32_t ranks; //0x0040
	float N0001B8EA; //0x0044
	char pad_0048[4]; //0x0048
	float N0001B8EC; //0x004C
	char pad_0050[4]; //0x0050
	int8_t N0001B8EE; //0x0054
	int8_t draggedFormation1; //0x0055
	int8_t N0003D819; //0x0056
	int8_t draggedFormation2; //0x0057
	int8_t N0001B8EF; //0x0058
	int8_t N0003D80B; //0x0059
	int8_t N0003D80E; //0x005A
	int8_t N0003D80C; //0x005B
	float targetPosXEnd; //0x005C
	float targetPosYEnd; //0x0060
	int8_t N0001B8F4; //0x0064
	int8_t N0003D7F4; //0x0065
	int8_t N0003D7F7; //0x0066
	int8_t N0003D7F5; //0x0067
	void* N0001B8F5; //0x0068
	int8_t N0001B8F6; //0x006C
	int8_t N0003D7EA; //0x006D
	int8_t N0003D7ED; //0x006E
	int8_t N0003D7EB; //0x006F
	float targetCoordXEngine; //0x0070
	float targetCoordZEngine; //0x0074
	float targetCoordYEngine; //0x0078
	struct siegeEngine* siegeEngine; //0x007C
	int32_t N0001B8FB; //0x0080
	int32_t N0001B8FC; //0x0084
	char pad_0088[8]; //0x0088
	float N0001B8FF; //0x0090
	char pad_0094[16]; //0x0094
	float N0003A5AB; //0x00A4
}; //Size: 0x00A8

struct battleMapPosition
{
public:
	int32_t positionIDMaybe; //0x0000
	int32_t numSoldiers; //0x0004
}; //Size: 0x0008

struct engagedUnit
{
public:
	struct unit* unit; //0x0000
	int32_t intValue; //0x0004
	int32_t intValue2; //0x0008
	int32_t intValue3; //0x000C
	int32_t intValue4; //0x0010
	int32_t intValue5; //0x0014
	float floatValue; //0x0018
}; //Size: 0x001C

struct firingUnit
{
public:
	struct unit* unit; //0x0000
	int32_t intValue; //0x0004
};

struct unitPositionData
{
public:
	struct engagedUnit* engagedUnits; //0x0000
	int32_t engagedUnitsSize; //0x0004
	int32_t engagedUnitsNum; //0x0008
	struct engagedUnit* engagedUnits2; //0x000C
	int32_t engagedUnits2Size; //0x0010
	int32_t engagedUnits2Num; //0x0014
	char pad_0018[16]; //0x0018
	struct unit* unit; //0x0028
	char pad_002C[4]; //0x002C
	void* N00002C71; //0x0030
	char pad_0034[4]; //0x0034
	struct unit* N00002C73; //0x0038
	float coord0; //0x003C
	float coord1; //0x0040
	float coord2; //0x0044
	float coord3; //0x0048
	float rotation; //0x004C
	struct battleMapPosition* isOnWalls; //0x0050
	int32_t isOnWallsSize; //0x0054
	int32_t isOnWallsCount; //0x0058
	struct battleMapPosition* isInTower; //0x005C
	int32_t isInTowerSize; //0x0060
	int32_t isInTowerCount; //0x0064
	struct battleMapPosition* isInGateHouse; //0x0068
	int32_t isInGateHouseSize; //0x006C
	int32_t isInGateHouseCount; //0x0070
	struct battleMapPosition* positions1; //0x0074
	int32_t positions1Size; //0x0078
	int32_t positions1Count; //0x007C
	struct battleMapPosition* positions2; //0x0080
	int32_t positions2Size; //0x0084
	int32_t positions2Count; //0x0088
	struct battleMapPosition* positions3; //0x008C
	int32_t positions3Size; //0x0090
	int32_t positions3Count; //0x0094
	struct battleMapPosition* positions4; //0x0098
	int32_t positions4Size; //0x009C
	int32_t positions4Count; //0x00A0
	char pad_00A4[12]; //0x00A4
	struct floatPosData floatPosData; //0x00B0
	char pad_00F4[4]; //0x00F4
	struct targetPos targetArray[16]; //0x00F8
	int32_t targetsDone; //0x0B78
	int32_t additionalTargetsOverOne; //0x0B7C
	int32_t targetsToGo; //0x0B80
	int8_t hasTargets; //0x0B84
	int8_t isHalted; //0x0B85
	char pad_0B86[6]; //0x0B86
	float lastTargetCoord1; //0x0B8C
	float lastTargetCoord2; //0x0B90
	char pad_0B94[20]; //0x0B94
	float N00002F68; //0x0BA8
	float N00002F69; //0x0BAC
	float N00002F6A; //0x0BB0
	float N00002F6B; //0x0BB4
	float N00002F6C; //0x0BB8
	float N00002F6D; //0x0BBC
	float N00002F6E; //0x0BC0
	float N00002F6F; //0x0BC4
	float N00002F70; //0x0BC8
	float N00002F71; //0x0BCC
	float N00002F72; //0x0BD0
	float N00002F73; //0x0BD4
	float N00002F74; //0x0BD8
	float N00002F75; //0x0BDC
	float N00002F76; //0x0BE0
	float N00002F77; //0x0BE4
	float N00002F78; //0x0BE8
	char pad_0BEC[20]; //0x0BEC
	void** towersUnderFireFrom; //0x0C00
	int32_t towersUnderFireFromSize; //0x0C04
	int32_t towersUnderFireFromCount; //0x0C08
	struct firingUnit* unitsUnderFireFrom; //0x0C0C
	int32_t unitsUnderFireFromSize; //0x0C10
	int32_t unitsUnderFireFromCount; //0x0C14
	void** underFireSomething2; //0x0C18
	int32_t underFireSomething2Size; //0x0C1C
	int32_t underFireSomething2Count; //0x0C20
	void** underFireSomething3; //0x0C24
	int32_t underFireSomething3Size; //0x0C28
	int32_t underFireSomething3Count; //0x0C2C
	char pad_0C30[8]; //0x0C30
	int32_t combatStatus; //0x0C38
}; //Size: 0x0C3C

struct rebelUnitName
{
public:
	char* unitName; //0x0000
	char pad_0004[20]; //0x0004
}; //Size: 0x0018

struct descrRebelEntry
{
public:
	int32_t index; //0x0000
	char* name; //0x0004
	int32_t nameHash; //0x0008
	int32_t category; //0x000C
	struct N00022A30* localizedName; //0x0010
	int32_t chance; //0x0014
	struct rebelUnitName unitNames[121]; //0x0018
	int32_t unitCount; //0x0B70
}; //Size: 0x0B74

//unit data
struct unit {
	undefined field0_0x0[4];
	struct trackedPointerUnit** trackedUnitPointerP;
	undefined field2_0x8[52];
	int32_t aiActiveSet; //0x003C
	undefined field_0040[24];
	struct modelEntry* bmdbEntry; //0x0058
	undefined field3_0x5C[548];//0x005C
	struct unitPositionData* unitPositionData; //0x0280
	struct stackStruct* army;//0x0284
	undefined field4_0x288[12];
	struct eduEntry* eduEntry;
	int ID; //0x0298
	int battlemapTeam; //0x029C
	int actionStatus; //0x02A0
	char pad_02A4[24]; //0x02A4
	void* ThirtyTwoByteObject; //0x02BC
	char pad_02C0[8]; //0x02C0
	void* hundredSixtyByteObject; //0x02C8
	char pad_02CC[8]; //0x02CC
	int fatigue; //0x02D4
	float killChanceModifier; //0x02D8
	struct Unit* this2; //0x02DC
	int moraleLevel; //0x02E0
	int moraleStatusSum; //0x02E4
	char pad_02E8[24]; //0x02E8
	int moraleValue1; //0x0300
	int moraleValue2; //0x0304
	int moraleValue3; //0x0308
	int moraleValue4; //0x030C
	int moraleValue5; //0x0310
	int moraleValue6; //0x0314
	int moraleValue7; //0x0318
	int moraleValue8; //0x031C
	int moraleValue9; //0x0320
	char pad_0324[372]; //0x0324
	struct unitMorale* moralePartOfUnitLookAbove; //0x0498
	char pad_049C[4]; //0x049C
	int someMoraleValue; //0x04A0
	char pad_04A4[4]; //0x04A4
	int16_t N0000271D; //0x04A8
	int8_t N00009770; //0x04AA
	int8_t skipsMoraleCalcIFNotZero; //0x04AB
	int8_t lockedMorale; //0x04AC
	char pad_04AD[11]; //0x04AD
	int moraleAreaEffectEnd; //0x04B8
	char pad_04BC[60]; //0x04BC
	int hasActiveEffects; //0x04F8
	int activeEffectsOnThisUnit; //0x04FC
	int expScreen; /* screen experience */
	struct general* general;
	float movePoints;
	int SoldierCountStrat; /* number of soldiers */
	int SoldierCountMaxStrat; /* number of soldiers on tactical map */
	int SoldierCountBattlemap; /* max number of soldiers */
	int soldierCountMaxBattlemap; /* max number of soldiers */
	char pad_051C[64]; //0x051C
	struct generalInfo* officer1; //0x055C
	struct generalInfo* officer2; //0x0560
	struct generalInfo* officer3; //0x0560
	struct generalInfo* generalInfo; //0x0568
	struct soldierInBattle* generalSoldier; //0x056C
	struct soldierInBattle* officer1Soldier; //0x0570
	struct soldierInBattle* officer2Soldier; //0x0574
	struct soldierInBattle* officer3Soldier; //0x0578
	int32_t numOfficer; //0x057C
	int32_t numOfficerInBattle; //0x0580
	int32_t N00002755; //0x0584
	int32_t N00002756; //0x0588
	int32_t N00002757; //0x058C
	int32_t N00002758; //0x0590
	int32_t statPri; //0x0594
	char pad_0598[20]; //0x0598
	int32_t weaponType; //0x05AC
	int32_t weaponTecType; //0x05B0
	char pad_05B4[8]; //0x05B4
	int32_t minAttackDelayPri; //0x05BC
	char pad_05C0[28]; //0x05C0
	int32_t statPriArmour; //0x05DC
	int32_t valueAfterInEdu; //0x05E0
	int8_t attackInBattle; //0x05E4
	int16_t armourInBattle; //0x05E5
	char pad_05E7[1]; //0x05E7
	int8_t N0000276E; //0x05E8
	int8_t N00010E95; //0x05E9
	char pad_05EA[6]; //0x05EA
	UINT32 stats; /* def/atk/etc */
	struct soldierInBattle** soldiersBattleArr; /* array of soldiers battle data */
	struct soldierData* soldiersArr; /* array of soldiers data */
	void* soldierIndices; //0x05FC
	void* generalOrMount; //0x0600
	int32_t attachmentCount; //0x0604
	char pad_0608[6172]; //0x0608
	void* UNIT_TASK_INTERFACE; //0x1E24
	char pad_1E28[28]; //0x1E28
	int32_t unitBattleProperties; //0x1E44
	char pad_1E48[4]; //0x1E48
	void* formationsArray; //0x1E4C
	char pad_1E50[38]; //0x1E50
	int16_t ranks; //0x1E76
	char pad_1E78[12]; //0x1E78
	int8_t isCloseFormation; //0x1E84
	char pad_1E85[59]; //0x1E85
	float positionX; //0x1EC0
	float N00002FB7; //0x1EC4
	float positionY; //0x1EC8
	char pad_1ECC[400]; //0x1ECC
	int32_t maxAmmo; //0x205C
	int32_t currentAmmo; //0x2060
	uint8_t statusField1; //0x2064
	uint8_t statusField2; //0x2065
	uint8_t statusField3; //0x2066
	char pad_2067[1]; //0x2067
	int8_t rallyHornCountdown; //0x2068
	char pad_2069[15];; //0x2069
	UNICODE_STRING** alias; /* Legio string etc */
	undefined field20_0x207c[44];
	struct siegeEngine** siegeEngine;
	undefined field22_0x20ac[4];
	int siegeEnNum; /* number of siege engines */
	undefined field24_0x20b4[36];
	struct engineRecord* engineRec;
	undefined field26_0x20dc[4];
};

//army data
struct stackStruct { /* structure of stack */
	undefined field_0x0[76];
	struct factionStruct* faction;
	int regionID;
	struct unit** units;
	int32_t unitsSize; //0x0058
	int numOfUnits;
	undefined field_0x60[48];
	int8_t isBoarded; //0x0090
	char pad_0091[7]; //0x0091
	struct stackStruct* boardedArmy; /* if fleet, here army */
	undefined field_0x9c[4];
	struct stackStruct* shipArmy; /* here fleet what contain our army */
	undefined field_0xa4[8];
	struct siegeS* siege;
	struct portBuildingStruct* blockedPort; /* if port blocked by this army when it here */
	undefined field_0xb4[3];
	int8_t ladders; //0x00B7
	int8_t rams; //0x00B8
	int8_t towers; //0x00B9
	char pad_00BA[10]; //0x00BA
	int32_t tilesMovedThisTurnDoesntStatAtZero; //0x00C4
	char pad_00C8[4]; //0x00C8
	uint32_t upkeepModifier; //0x00CC
	char pad_00D0[4]; //0x00D0
	struct general* gen; /* 0 if in settlement/fort */
	struct unit* generalsUnit; //0x00D8
	struct generalInfo* generalInfo; //0x00DC
	float armyStrengthForAutoResolveMaybe; //0x00E0
	char pad_00E4[12]; //0x00E4
	struct general** characters; //0x00F0
	undefined field_0xf4[4]; //0x00F4
	int charactersNum; //0x00F8
	undefined field_0xfc[4]; //0x00FC
	struct settlementStruct* settlement; //0x0100
	undefined field_0x104[16];
	struct stackStruct* enemyArmy;
	int32_t inBattle; //0x0118
	int8_t N00001BCA; //0x011C
	int8_t N0003D211; //0x011D
	uint16_t N0003D214; //0x011E
	char pad_0120[4]; //0x0120
	int totalStrength;
	float reform_point_x;
	float reform_point_y;
	int32_t battleSide; //0x0130
	char pad_0134[16]; //0x0134
	int32_t battleAllianceOrSomething; //0x0144
	int32_t battleDifficulty; //0x0148
	int8_t isHalfDestroyed; //0x014C
	char pad_014D[15]; //0x014D
	struct descrRebelEntry* descrRebel; //0x015C
	int32_t subFactionID; //0x0160
};

struct coords {
	int xCoord;
	int yCoord;
};

struct factionTileStruct {
	int8_t* tilesVisiblity;
	int tilesXBound;
	int tilesYBound;
	undefined field3_0xc[28];
	void* revealedTiles;
	int revealedTilesContainerAllocatedSize;
	int revealedTilesNumber;
	undefined field7_0x34[24];
};

struct factionEconomy
{
public:
	int32_t farmingIncome; //0x0000
	int32_t taxesIncome; //0x0004
	int32_t miningIncome; //0x0008
	int32_t tradeIncome; //0x000C
	int32_t merchantIncome; //0x0010
	int32_t constructionIncome; //0x0014
	int32_t otherIncome1; //0x0018
	int32_t otherIncome2; //0x001C
	int32_t diplomacyIncome; //0x0020
	int32_t tributesIncome; //0x0024
	int32_t adminIncome; //0x0028
	int32_t kingsPurseIncome; //0x002C
	int32_t wagesExpense; //0x0030
	int32_t upkeepExpense; //0x0034
	int32_t constructionExpenseBuildings; //0x0038
	int32_t constructionExpenseField; //0x003C
	int32_t recruitmentExpenseBuildings; //0x0040
	int32_t recruitmentExpenseMercs; //0x0044
	int32_t corruptionExpense; //0x0048
	int32_t diplomacyExpense; //0x004C
	int32_t tributesExpense; //0x0050
	int32_t otherExpense1; //0x0054
	int32_t otherExpense2; //0x0058
	int32_t devastationExpense; //0x005C
}; //Size: 0x0060

struct battleFactionCounter
{
public:
	int32_t battlesWon; //0x0000
	int32_t battlesLost; //0x0004
	int32_t wonLostRatio; //0x0008
}; //Size: 0x000C

struct factionDataStrategy
{
	int32_t intx0;
	int32_t intx4;
	int intx8;
	int intxC;
	int somethingSubterFugeTasks;
	float someOtherStrengthModifier;
	float strengthLimitModifier;
	int8_t atWar;
	int8_t notWantCeaseFire;
	int8_t canAttack;
	int8_t pad;
};

struct aiRegionData
{
	void* __vftable /*VFT*/;
	int fieldx4;
	bool boolx8;
	char pad1[3];
	int factionID;
	int regionID;
	bool boolx14;
	bool boolx15;
	char pad2[2];
	int regionValue;
	int fieldx1C;
	int hasNeighbouringEnemies;
	int hasNeighbouringOwnArmies;
	int fieldx28;
	int ownArmiesTotalStrength;
	int ownArmiesTotalCount;
	int enemyArmiesTotalStrength;
	int enemyArmiesTotalCount;
	int neutralArmiesTotalStrength;
	int neutralArmiesTotalCount;
	int neighbourRegionsOwnArmyStrength;
	int neighbourRegionsOwnArmyNum;
	int neighbourRegionsEnemyArmyStrength;
	int neighbourRegionsEnemyArmyNum;
	int neighbourRegionsNeutralArmyStrength;
	int neighbourRegionsNeutralArmyNum;
	int regionRisk;
	int fieldx60;
};

struct someStructInGS
{
	struct aiFaction* aiFaction;
	struct factionStruct* faction;
	void* arrayx8_objSize20;
	int32_t arrayx8Size;
	int32_t arrayx8Num;
	void* arrayx14_objSize20;
	int32_t arrayx14Size;
	int32_t arrayx14Num;
	void* arrayx20_objSize20;
	int32_t arrayx20Size;
	int32_t arrayx20Num;
	void* aiBrigandControllers;
	int32_t aiBrigandControllersSize;
	int32_t aiBrigandControllersNum;
	int32_t fieldx38;
	void* aiMilitaryControllerWorldWide;
};

struct aiPersonalityValues
{
	struct aiFaction* aiFaction;
	struct aiProductionController** aiProductionControllers;
	int32_t aiProductionControllersSize;
	int32_t aiProductionControllersNum;
	int32_t AIPersonalityType;
	int32_t AIPersonalityName;
	int32_t buildingValues[257];
	int32_t unkBiasCultural1;
	int32_t unkBiasCultural2;
	int32_t unkBiasCultural3;
	int32_t unkBiasCultural4;
	int32_t spyBiasLarge;
	int32_t assassinBiasLarge;
	int32_t diplomatBiasLarge;
	int32_t admiralBiasLarge;
	int32_t priestBiasLarge;
	int32_t merchantBiasLarge;
	int32_t recruitmentValuesLarge[11];
	int32_t populationBias;
	int32_t spyBias;
	int32_t assassinBias;
	int32_t diplomatBias;
	int32_t admiralBias;
	int32_t priestBias;
	int32_t merchantBias;
	int32_t recruitmentValues[11];
	int32_t balancedPolicyNum;
	int32_t financialPolicyNum;
	int32_t militaryPolicyNum;
	int32_t growthPolicyNum;
	int32_t culturalPolicyNum;
	struct settlementPolicies* settlementPolicies;
	int32_t settlementPoliciesSize;
	int32_t settlementPoliciesCount;
	int8_t autoManagedRecruitment;
	int8_t autoManagedConstruction;
	int8_t isAiControlled;
	char pad_01BB[9];
};

struct aiGlobalStrategyDirector
{
	void* _vftable /*VFT*/;
	struct aiFaction* aiFaction;
	struct factionStruct* faction;
	struct factionDataStrategy someFactionData[31];
	struct aiRegionData* ownRegions;
	int32_t ownRegionsSize;
	int32_t ownRegionsCount;
	struct aiRegionData* allNeighbourRegions;
	int32_t allNeighbourRegionsSize;
	int32_t allNeighbourRegionsCount;
	struct aiRegionData* targetRegions;
	int32_t targetRegionsSize;
	int32_t targetRegionsCount;
	struct aiRegionData targetRegionCrusade;
	void* arrayx474_objsize380_maybeNavalTargets;
	int32_t arrayx474Size;
	int32_t arrayx474Num;
	void* arrayx480_objsize180;
	int32_t arrayx480Size;
	int32_t freeShipsMaybe;
	void* regionControllers;
	int32_t arrayx48CSize;
	int32_t regionControllersNum;
	struct someStructInGS someStructInGS;
	void* aiNavalControllers;
	int32_t aiNavalControllersSize;
	int32_t aiNavalControllersNum;
	void* arrayx4F8_objsize20;
	int32_t arrayx4F8Size;
	int32_t arrayx4F8Num;
	int fieldx504_initMinusOne;
	int fieldx508;
	int fieldx50C_init31;
	int fieldx510_init31;
	void* crusadeController;
};

struct settlementPolicies
{
	int32_t regionID;
	int32_t autoManageBuildPolicy;
	int32_t secondaryPolicy;
	int8_t autoManagedConstruction;
	int8_t autoManagedRecruitment;
	char pad_000E[2];
};

struct aiProductionController
{
	void* __vftable /*VFT*/;
	struct aiFaction* aiFaction;
	int regionID;
	struct settlementStruct* settlement;
	int32_t autoManagePolicy;
	int secondaryPolicy;
	int regionRiskLevel1;
	int regionRiskLevel2;
	int regionRiskLevel3;
	int regionRiskLevel4;
	int increaseUnitBias5;
	int increaseTradeStuff;
	int increaseGrowthStuff;
	int increaseSpyBias;
	int increaseAssasinBias;
	int increasePriestBias;
	int increaseMerchantBias;
	int increaseDiplomatBias;
	int increaseShipBias;
	int generalBuildRecruitDesire;
	int minimumUnitValue;
	int fieldx54;
	int fieldx58;
	int startLevel;
	int controllerIndex;
	int8_t isAutoManaged;
	int8_t isAutoManagedRecruitment;
	int8_t isAutoManagedConstruction;
	int8_t isAutoManagedTaxes;
	int8_t hasGrowthBuildPolicy;
	char pad_x[3];
	int fieldx6C;
	int fieldx70;
	int fieldx74;
	int fieldx78;
	int fieldx7C;
	int fieldx80;
	int fieldx84;
	int fieldx88;
	int fieldx8C;
	int fieldx90;
	struct settlementStruct* settlement2;
	struct unitRQ unitInQueue;
	struct buildingInQueue buildingInQueue;
	int randomIncreaseLocation;
	int fieldxFC;
	bool isRandomIncreased;
	bool boolx101;
	char pad_102[2];
	int buildingBias[257];
	int fieldx1E8_unkBiasCultural;
	int fieldx1EC_unkBiasCultural;
	int fieldx1F0_unkBiasCultural;
	int fieldx1F4_unkBiasCultural;
	int spyBiasTurn;
	int assassinBiasTurn;
	int diplomatBiasTurn;
	int admiralBiasTurn;
	int priestBiasTurn;
	int merchantBiasTurn;
	int recruitBiasBoosted[11];
	int populationBoost;
	int8_t buildingQueueEmpty;
	int8_t manualConstruction;
	int8_t unitQueueEmpty;
	int8_t pad22;
	struct settlementStruct* settlement3;
	struct unitRQ unitInQueue2;
	struct buildingInQueue buildingInQueue2;
	int unitValue;
	int fieldx2AC;
	bool boolx2B0;
	bool boolx2B1;
	bool boolx2B2;
	bool boolx2B3;
	int spyBias;
	int assassinBias;
	int diplomatBias;
	int admiralBias;
	int priestBias;
	int merchantBias;
	int recruitBias[11];
};


struct militaryValuesLTGD
{
	int32_t totalPopulation; //0x0000
	int32_t tileCount; //0x0004
	int32_t averagePopulation; //0x0008
	int32_t productionValue; //0x000C
	int32_t nonAlliedBorderLength; //0x0010
	int32_t enemyBorderLength; //0x0014
	int32_t fleetCount; //0x0018
	int32_t navalPowerPerFleet; //0x001C
	int32_t navalStrength; //0x0020
	int32_t armyCount; //0x0024
	int32_t strengthPerArmy; //0x0028
	int32_t totalStrength; //0x002C
	int32_t freeStrength; //0x0030
	int32_t neighbourEnemyNum; //0x0034
	int32_t enemyStrength; //0x0038
	int32_t protectorateOf; //0x003C
};

struct strategyValuesLTGD
{
	int32_t borderTiles; //0x0000
	int32_t frontLineBalance; //0x0004
	int8_t hasAllianceAgainst; //0x0008
	int8_t isStrongestNeighbour; //0x0009
	int8_t isWeakestNeighbour; //0x000A
	char pad_000B[1]; //0x000B

};

struct dataLTGD
{
public:
	struct factionStruct* currentFaction; //0x0000
	char pad_0004[8]; //0x0004
	uint32_t N00024EDA; //0x000C
	char pad_0010[4]; //0x0010
	struct militaryValuesLTGD militaryValuesLTGD[31]; //0x0014
	struct strategyValuesLTGD strategyValuesLTGD[31][31]; //0x07D4
};


struct decisionValuesLTGD
{
public:
	int32_t defendType; //0x0000
	int32_t defendPriority; //0x0004
	int32_t invasionType; //0x0008
	int32_t invadePriority; //0x000C
	int8_t atWar; //0x0010
	int8_t wantPeace; //0x0011
	int8_t wantAlly; //0x0012
	int8_t wantBeProtect; //0x0013
	int8_t wantOfferProtect; //0x0014
	char pad_0015[3]; //0x0015
	int32_t allianceAgainst; //0x0018
	int32_t ptsDesire; //0x001C
	int32_t ptsAlliance; //0x0020
	int32_t N0002EF7A; //0x0024
	int32_t N0002EF7B; //0x0028
	int8_t canForceInvade; //0x002C
	char pad_002D[3]; //0x002D
}; //Size: 0x0030

struct aiLongTermGoalDirector
{
public:
	char pad_0000[4]; //0x0000
	struct aiFaction* aiFaction; //0x0004
	struct factionStruct* faction; //0x0008
	uint32_t trustedAllyEnemies; //0x000C --bitfield
	int32_t freeStrengthEnemy; //0x0010
	int32_t freeStrengthEnemyBalance; //0x0014
	int8_t consideringNavalInvasion; //0x0018
	char pad_0019[3]; //0x0019
	int32_t navalTargetRegionID;
	int32_t navalTargetRegionPriority;
	int32_t x24;
	struct decisionValuesLTGD longTermGoalValues[31]; //0x0028
	int32_t N0002EFB5; //0x05F8
	int32_t militaryBalanceLevel; //0x05FC
	char pad_0600[4]; //0x0600
}; //Size: 0x0604

struct aiFaction
{
public:
	char pad_0000[4]; //0x0000
	struct factionStruct* faction; //0x0004
	int32_t factionID; //0x0008
	char pad_000C[4]; //0x000C
	int8_t N00001E8C; //0x0010
	int8_t N0002F25A; //0x0011
	char pad_0012[14]; //0x0012
	struct aiLongTermGoalDirector* aiLongTermGoalDirector; //0x0020
	struct aiDiplomacyManager* aiDiplomacyManager; //0x0024
	struct aiActionRequestController* aiActionRequestController; //0x0028
	struct aiResourceManager* aiResourceManager; //0x002C
	struct AiFinanceManager* AiFinanceManager; //0x0030
	struct aiPersonalityValues* aiProductionControllers; //0x0034
	struct aiGlobalStrategyDirector* aiGlobalStrategyDirector; //0x0038
	struct aiSubterFugeController* aiSubterFugeController; //0x003C
	struct aiNamedCharacterController* aiNamedCharacterController; //0x0040
	struct aiPriestController* aiPriestController; //0x0044
	struct aiMerchantController* aiMerchantController; //0x0048
}; //Size: 0x0058

//faction
struct factionStruct {
	undefined field_0x0[180];
	int dipNum; /* number of faction in diplomatic array */
	int cultureID;
	char* ai_label; /* ai_label of faction */
	int32_t AILabelHash; //0x00C0
	struct settlementStruct* capital; /* capital of the faction */
	struct namedCharacter* leader; /* faction leader */
	struct namedCharacter* heir; /* faction heir */
	struct factionStratMapDescrS* factSmDescr;
	int isPlayerControlled; /* is faction a controlled by player */
	struct aiFaction* aiFaction; //0x00D8
	int32_t AIPersonalityType; //0x00DC
	int32_t AIPersonalityName; //0x00E0
	char pad_00E4[12]; //0x00E4
	struct holdRegionsWinCondition* WinConditions; //0x00F0
	int32_t regionsOwnedStart; //0x00F4
	struct namedCharacter** charactersAll; /* all characters, died, alive, etc */
	int32_t namedCharactersSize; //0x00FC
	int numOfCharactersAll; /* all characters, died, alive, etc */
	struct general** characters; /* characters on stratmap */
	int32_t charactersSize; //0x0108
	int numOfCharacters; /* characters on stratmap */
	struct stackStruct** stacks;
	int32_t armiesSize; //0x0114
	int stackNum;
	int* regionsID;
	int32_t regionsSize; //0x0120
	int regionsNum;
	struct settlementStruct** settlements;
	int32_t settlementsSize; //0x012C
	int settlementsNum;
	struct fortStruct** forts;
	int32_t fortsSize; //0x0138
	int fortsNum;
	struct watchTowerStruct** watchTowers; /* watchtowers */
	int32_t watchtowersSize; //0x0144
	int watchtowersNum;
	struct portBuildingStruct** portBuildings; /* port buildings */
	int32_t portBuildingsSize; //0x0150
	int portBuildingsNum;
	uint32_t neighBourFactionsBitmap; //0x0158
	int* neighBourRegions; //0x015C
	int32_t neighBourRegionsSize; //0x0160
	int32_t neighBourRegionsNum; //0x0164
	char pad_0168[44]; //0x0168
	int32_t deadStatus; //0x0194 3 means until resurrected 4 means until emerged
	int8_t reEmergent; //0x0198
	int8_t isUndiscovered; //0x0199
	char pad_019A[2]; //0x019A
	factionTileStruct* tilesFac;
	struct mission** missions; //0x01A0
	int32_t missionsSize; //0x01A4
	int32_t missionCount; //0x01A8
	char pad_01AC[48]; //0x01AC
	struct factionRanking* factionRankings; //0x01DC
	int32_t factionRankingsSize; //0x01E0
	int32_t factionRankingsCount; //0x01E4
	char pad_01E8[12]; //0x01E8
	int32_t triumphValue; //0x01F4
	int8_t someBool;
	int8_t autoManageRecruitment;
	int8_t autoManageBuildings;
	int8_t autoManageTaxes;
	float someEconomyFloat;
	int8_t freezeFactionAI; //0x0200
	char pad_0201[3]; //0x0201
	struct capabilityStruct factionCapabilities;
	int8_t hasSettlementsProcessed; //0x0A30
	char pad_0A31[3]; //0x0A31
	int32_t treasuryTurnStart; //0x0A34
	char pad_0A38[8]; //0x0A38
	UINT32 religion; /* number of religion */
	undefined field_0xa44[16];
	int8_t isFactionExcommunicated; //0x0A54
	char pad_0A55[3]; //0x0A55
	int32_t glory; //0x0A34
	char pad_0A5C[36]; //0x0A5C
	float* resourceModifiers;
	DWORD resourceModifiersEnd;
	DWORD resourceModifiersEnd2;
	char pad_0A8C[12]; //0x0AD4
	UNICODE_STRING** someString;
	UNICODE_STRING** localizedName;
	int32_t factionBannerIndex; //0x0AA0
	int32_t agentNameFactionId[12]; //0x0AA4
	undefined field_0xad4[24];
	int money; /* money of the faction */
	int KingsPurse; /* money of the faction */
	int32_t incomeDoubled; //0x0AF4
	struct factionEconomy factionEconomy[10]; //0x0AF8
	int32_t nextCounterEconomy; //0x0EB8
	int32_t counterEconomy; //0x0EBC
	int32_t maxTurnsTillReset; //0x0EC0
	int32_t upkeepModifier; //0x0E4
	char pad_0EC8[12]; //0x0EC8
	struct battleFactionCounter(*battlesWonVsFaction)[31]; //0x0ED4
	int32_t factionCountStart; //0x0ED8
	int32_t otherFactionCount; //0x0EDC
	int32_t battlesWon; //0x0EE0
	int32_t battlesLost; //0x0EE4
	int32_t settlementsCaptured; //0x0EE8
	int32_t settlementsLost; //0x0EEC
	char pad_0EF0[24]; //0x0EF0
};

struct ModelDbEntry
{
public:
	char pad_0000[4]; //0x0000
	char N00002889[8]; //0x0004
	char pad_000C[8]; //0x000C
	int32_t nameLength; //0x0014
	char pad_0018[4]; //0x0018
	float scale; //0x001C
	char pad_0020[4]; //0x0020
	class ModelDbMesh* Mesh; //0x0024
	char pad_0028[12]; //0x0028
	class ModelDbEntryTextures* Textures; //0x0034
	char pad_0038[12]; //0x0038
	class ModelDbEntryTextures* AttTextures; //0x0044
	char pad_0048[12]; //0x0048
	class ModelDbAnims* Animations; //0x0054
	char pad_0058[8]; //0x0058
	int32_t torchAttachBone; //0x0060
	float N00000857; //0x0064
	float N00000858; //0x0068
	float N00000859; //0x006C
	float N0000085A; //0x0070
	float N0000085B; //0x0074
	float N0000085C; //0x0078
	char pad_007C[4]; //0x007C
}; //Size: 0x0080


struct ArmourUpgModels
{
public:
	char* BaseModel; //0x0000
	int32_t BaseModelHash; //0x0004
	char* UpgradeOne; //0x0008
	int32_t UpgradeOneHash; //0x000C
	char* UpgradeTwo; //0x0010
	int32_t UpgradeTwoHash; //0x0014
	char* UpgradeThree; //0x0018
	int32_t UpgradeThreeHash; //0x001C
};


//type of unit from EDU
struct eduEntry {
	char* Type;
	uint32_t typeHash;
	DWORD Index;
	DWORD UnitCreatedCounter;
	char* UnitCardTga;
	uint32_t unitcardHash;
	char* InfoCardTga;
	uint32_t unitInfoCardHash;
	char* cardPicDir;
	uint32_t cardPicDirHash;
	char* InfoPicDir;
	uint32_t infoPicDirHash;
	UNICODE_STRING*** localizedName;
	UNICODE_STRING*** localizedDescr;
	UNICODE_STRING*** localizedDescrShort;
	uint32_t Category;
	uint32_t Class;
	uint32_t categoryClassCombinationForAI;
	DWORD VoiceType;
	char* Accent;
	uint32_t accentHash;
	char* BannerFaction;
	char pad_0058[4];
	char* BannerUnit;
	char pad_0060[4];
	char* BannerHoly;
	char pad_0068[4];
	char* BannerMarine;
	char pad_0070[4];
	char* BannerSecondary;
	char pad_0078[12];
	DWORD Trained;
	DWORD StatMentalDicipline;
	int8_t MoraleLocked;
	int8_t Morale;
	int8_t StatHealth;
	int8_t StatHealthAnimal;
	int8_t StatHeat;
	int8_t StatGround1;
	int8_t StatGround2;
	int8_t StatGround3;
	int8_t StatGround4;
	char pad_0095[1];
	ushort StatFood1;
	ushort StatFood2;
	ushort StatFireDelay;
	DWORD StatStl;
	float ChargeDistance;
	DWORD mountEffectClass;
	char pad_00A8[4];
	DWORD mountEffectAmount2;
	DWORD mountEffectClass2;
	char pad_00B4[4];
	DWORD mountEffectAmount3;
	DWORD mountEffectClass3;
	char pad_00C0[4];
	DWORD mountEffectAmount4;
	DWORD MountEffectCount;
	ushort StatCost1;
	ushort StatCost2;
	ushort StatCost3;
	ushort StatCost4;
	ushort StatCost5;
	ushort StatCost6;
	ushort StatCost7;
	ushort StatCost8;
	DWORD CrusadingUpkeepModifier;
	DWORD RecruitPriorityOffsetTimes4;
	int8_t formationHorde;
	int8_t formationColumn;
	int8_t formationSquare;
	int8_t formationSquareHollow;
	int8_t formationWedge;
	int8_t formationPhalanx;
	int8_t formationSchiltrom;
	int8_t formationShieldWall;
	int8_t N000000AA;
	int8_t hasSquareFormation;
	char pad_00EE[2];
	uint32_t DefaultNubmerOfRanks;
	float UnitSpacingFrontToBackClose;
	float UnitSpacingSideToSideClose;
	float UnitSpacingFrontToBackLoose;
	float UnitSpacingSideToSideLoose;
	char* Soldier;
	char pad_0108[12];
	struct ModelDbEntry* ModelDBEntry;
	DWORD SoldierCount;
	float Mass;
	float Width;
	float Height;
	uint32_t StatPri;
	DWORD Ammunition;
	DWORD MissleRange;
	float MissleRangeSquared;
	DWORD HasPrimary;
	void* StatPriMissle;
	DWORD WeaponType;
	DWORD TechType;
	DWORD DamageType;
	DWORD SoundType;
	DWORD AttackMinDelay;
	char pad_0154[4];
	char WeaponShootEffect[20];
	char pad_016C[4];
	uint32_t StatSec;
	DWORD SecAmmunition;
	DWORD SecMissleRange;
	float SecMissleRangeSquared;
	DWORD HasSecondary;
	void* StatSecMissle;
	DWORD SecWeaponType;
	DWORD SecTechType;
	DWORD SecDamageType;
	DWORD SecSoundType;
	DWORD SecAttackMinDelay;
	char pad_019C[28];
	uint32_t StatPriArmour;
	char pad_01BC[4];
	char* FirstOfficier;
	void* N000000E0;
	char pad_01C8[8];
	void* N000000E3;
	char* SecondOfficier;
	void* N000000E5;
	char pad_01DC[8];
	void* N000000E8;
	char* ThirdOfficier;
	void* N000000EA;
	char pad_01F0[8];
	void* N000000ED;
	DWORD OfficierCount;
	char pad_0200[4];
	int8_t* ArmorUpgradeLevels;
	int8_t* ArmorUpgradesEnd;
	int8_t* ArmorUpgrade2;
	char pad_0210[4];
	struct ArmourUpgModels* ArmorUpgradeModels;
	int* ArmorUpgradeModelsEnd;
	int* ArmorUpgrade5;
	int extrasCount;
	void* N000000F8;
	char pad_0228[4];
	int8_t N000000FA;
	int8_t N000003B1;
	int8_t EngineMissleAttack;
	int8_t EngineMissleChargeBonus;
	DWORD EngineMissleAmmunition;
	DWORD EngineMissleRange;
	float EngineMissleRangeSquared;
	DWORD Unknown3;
	void* EngineMissle;
	DWORD EngineMissleWeaponType;
	DWORD EngineMissleTechType;
	DWORD EngineMissleDamageType;
	DWORD EngineMissleSoundType;
	DWORD EngineMissleAttackMinDelay;
	char pad_0258[48];
	void* N00000111;
	DWORD N00000112;
	DWORD N00000113;
	DWORD N00000114;
	DWORD N00000115;
	DWORD TerMissleAttackMinDelay;
	char pad_02A0[36];
	void* MountModel;
	void* MountSomerthing;
	DWORD StatSecAndSecArmour;
	char pad_02D0[68];
	DWORD Mount;
	char pad_0318[8];
	void* AnimalModel;
	void* AnimalSomething;
	char pad_0328[84];
	void* Ship;
	uint32_t ownership;
	int32_t eraAmount;
	void* eraOwnerships;
	void* N00000152;
	void* N00000153;
	float aiUnitValuePerSoldier;
	float aiUnitValue;
	char pad_039C[4];
	DWORD* Attributes;
	DWORD* EndOfAttributes;
	void* BytesBeforeNextAttributes;
	int8_t Attributes2;
	int8_t Attributes3;
	int8_t Attributes4;
	int8_t Attributes5;
	DWORD Attributes6;
	float MoveSpeedMod;
	DWORD AttributeAreaEffect;
	char pad_03BC[28];
	float PriSkeletonCompFactor;
	float SecSkeletonCompFactor;
	float TerSkeletonCompFactor;
};

struct eduEntryes { /* structure with all edu entries */
	UINT32 qq;
	struct eduEntry unitTupes[500];
	UINT32 numberOfTupes;
};

//string
struct someFactNameStruct {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	char* name;
};

//pointer to ancillary
struct ancData { /* data of the ancillary */
	struct ancillary* dataAnc;
};

//data of one soldier in unit
#pragma pack(push,1)
struct soldierData { /* one soldier in unit */
	unsigned char field_0x0;
	unsigned char exp; /* expierence of soldier */
	undefined field_0x2;
	undefined field_0x3;
};
#pragma pack(pop)


struct ltgdFactionValues
{
public:
	int32_t totalPopulation; //0x0000
	int32_t tileCount; //0x0004
	int32_t averagePopulation; //0x0008
	int32_t productionValue; //0x000C
	int32_t nonAlliedBorderLength; //0x0010
	int32_t enemyBorderLength; //0x0014
	int32_t fleetCount; //0x0018
	int32_t navalPowerPerFleet; //0x001C
	int32_t navalStrength; //0x0020
	int32_t armyCount; //0x0024
	int32_t strengthPerArmy; //0x0028
	int32_t totalStrength; //0x002C
	int32_t freeStrength; //0x0030
	int32_t neighbourEnemyNum; //0x0034
	int32_t enemyStrength; //0x0038
	int32_t protectorateOf; //0x003C
}; //Size: 0x0040

struct interFactionLTGD
{
public:
	int32_t borderTiles; //0x0000
	int32_t frontLineBalance; //0x0004
	int8_t hasAllianceAgainst; //0x0008
	int8_t isStrongestNeighbour; //0x0009
	int8_t isWeakestNeighbour; //0x000A
	char pad_000B[1]; //0x000B
}; //Size: 0x000C

struct ltgdGlobals
{
public:
	struct factionStruct* currentFaction; //0x0000
	char pad_0004[8]; //0x0004
	uint32_t N00024EDA; //0x000C
	char pad_0010[4]; //0x0010
	struct ltgdFactionValues ltgdFactionValues[31]; //0x0014
	struct interFactionLTGD interFactionLTGD[31][31]; //0x07D4
};


//fort
struct fortStruct {
public:
	void* fortVtable;
	struct general* gubernator;
	undefined field_0x8[4];
	UINT32 xCoord;
	UINT32 yCoord;
	undefined field_0x14[32];
	float bannerPosX;
	float bannerPosZ;
	float bannerPosY;
	struct trackedPointerArmy_vtbl* trackedPointerArmyVtable;
	struct stackStruct* army;
	struct oneSiege sieges[8];
	int8_t siegeNum; //0x0088
	char pad_0089[3]; //0x0089
	int32_t siegeHoldoutTurns;
	int32_t turnsSieged;
	int32_t fieldx94;
	int32_t populationSiegeStart;
	int32_t fieldx9C;
	int32_t somethingPlagueRelated;
	int8_t plagued;
	int8_t setZeroAfterInvaded; //0x00A5
	int8_t bytexA6;
	int8_t bytexA7;
	int32_t fieldxA8;
	struct descrRebelEntry* descrRebel; //0x00AC
	int32_t subFactionID; //0x00B0
	void* spyingInfoBase; //0x00B4
	int8_t spyingInfoFactions[31]; //0x00B8
	char pad_00D7[9]; //0x00D7
	struct stratFortMod* stratModel;
	int regionID;
	struct factionStruct* faction;
	int32_t factionID; //0x00EC
	int32_t cultureID; //0x00F0
	void* localString; //0x00F4
	char* fortType;
	char pad_00FC[4]; //0x00FC
	int32_t fortFortificationLevel; //0x0100
	char pad_0104[20]; //0x0104
};

struct traidingResource {
	char* name;
	int nameCrypt;
	int null1;
	struct model_Rigid* model;
	char* item;
	int itemCrypt;
	int null3;
	int trade_value;
	char* icon;
	int iconCrypt;
	int null4;//
};

struct tradingResources {
	struct traidingResource resources[27];
	char* mine;
	int mineCrypt;
};

struct console_command { /* structure of console command */
	bool(_stdcall** function)(const char*, char*);
	char* name;
	char* description;
	int type;
	int idk;
};

struct consoleCommands {
	struct console_command** commands;
	int reservedElements;
	int size;
};

struct battleCameraStruct
{
public:
	float xCoord; //0x0000
	char pad_0004[4]; //0x0004
	float yCoord; //0x0008
	char pad_000C[20]; //0x000C
	float zCoord; //0x0020
}; //Size: 0x0024

struct settlementUiTextStruct
{
public:
	float ValueColumn; //0x0000
	char pad_0004[4]; //0x0004
	float yCoord; //0x0008
	char pad_000C[20]; //0x000C
	float zCoord; //0x0020
}; //Size: 0x0024


struct descr_sm_factions_list {
	struct factionStratMapDescrS* facDescrs;
	int capacity;
	int size;
};

#pragma pack(pop)