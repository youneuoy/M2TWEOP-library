#pragma once
#include <vector>
#include <memory>
#include <string>

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

#define GAME_FUNC(funcType, funcAddr) reinterpret_cast<funcType>(codes::offsets.funcAddr)
#define GAME_FUNC_RAW(funcType, funcAddr) reinterpret_cast<funcType>(funcAddr)


template<typename classType, typename returnType, typename ... TArgs>
returnType callClassFunc(classType instance, DWORD offset, TArgs ... args)
{
	const DWORD vtable = *reinterpret_cast<DWORD*>(instance);
	DWORD vFunc = *reinterpret_cast<DWORD*>(vtable + offset);
	return GAME_FUNC_RAW(returnType(__thiscall*)(classType, TArgs...), vFunc)(instance, args...);
}

struct scriptCommand
{
	DWORD vftable /*VFT*/;
	int field_4 = 0;
	int lineNumber = 0;
	const char *filePath = "luaCommand";
	int filePathHash = 35062250;
	const char *className = nullptr;
	scriptCommand(const char* name);
};

struct gameLogCommand
{
	scriptCommand command = scriptCommand("gameLog");
	int level = 4000;
	const char* message = nullptr;
	int messageHash = 0;
	gameLogCommand(const char* msg) : message(msg) {}
};

struct stringWithHash
{
public:
	char* name; //0x0000
	int32_t hash; //0x0004
}; //Size


struct UNICODE_STRING {
	USHORT something;//idk
	USHORT Length;//idk
	USHORT something2;//idk
	PWSTR Buffer;//y
};

struct trackedObject
{
	DWORD* vtbl;
	void* reference;
	bool destroying;
	char pad[3];
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

struct floatPosData
{
public:
	float boundUpRightX; //0x0000
	float boundUpRightY; //0x0004
	float boundDownRightX; //0x0008
	float boundDownRightY; //0x000C
	float boundUpLeftX; //0x0010
	float boundUpLeftY; //0x0014
	float boundDownLeftX; //0x0018
	float boundDownLeftY; //0x001C
	float float_20; //0x0020
	float float_24; //0x0024
	float rotation1; //0x0028
	float rotation2; //0x002C
	float coord1_1; //0x0030
	float coord1_2; //0x0034
}; //Size: 0x0044

struct smObject
{
	DWORD* vftable;
	smObject* next;
	smObject* previous;
	int xCoord;
	int yCoord;
	int fadeMode;
	bool render;
	float opacity;
	bool highlighted;
	char pad[3];
};

struct rcString
{
public:
	char pad_0000[12]; //0x0000
	wchar_t* string; //0x000C
	char pad_0010[104]; //0x0010
}; //Size: 0x0078


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

struct battleTile
{
	void *object;
	__int16 physicalGroundType;
	__int16 height;
	__int16 waterHeight;
	__int16 fielda;
	__int16 fieldC;
	__int16 fielde;
};

struct terrainLineSegment
{
	float startX;
	float startZ;
	float startY;
	float endX;
	float endZ;
	float endY;
	terrainLineSegment *previousSegment;
	terrainLineSegment *nextSegment;
	float radius1;
	float radius2;
	int16_t angle;
	int16_t field_2a;
};

struct terrainSegmentVector
{
	struct terrainLineSegment *lineSegments;
	struct terrainSegmentVector *nextSegments;
	struct terrainSegmentVector *previousSegments;
	int32_t lineSegmentsSize;
	int32_t lineSegmentsNum;
};


struct terrainFeatureHill
{
	void* vfTable;
	float xCoord;
	float zCoord;
	float yCoord;
	float radius;
	terrainLineSegment *terrainLineSegmentStart;
	terrainLineSegment *terrainLineSegmentEnd;
	float area;
};

struct hillVector
{
	struct terrainFeatureHill *hills;
	struct hillVector *nextHills;
	struct hillVector *previousHills;
	int32_t hillsSize;
	int32_t hillsNum;
};

struct plazaStuff 
{
	int soldiersAlliance0;
	int soldiersAlliance1;
	void *field8;
	void *field8End;
	int field10;
	int field14;
	void *someFloats;
	int field1C;
	int factionID1;
	int factionID2;
	UNICODE_STRING ***localizedName;
	int8_t byte2c;
	int8_t byte2d;
	int8_t pad2e[2];
	int settType;
	float plazaXcoord;
	float plazaYCoord;
	float center2;
	float center;
	int field44;
	int field48;
	int field4c;
	int field50;
	float sizeX;
	float sizeY;
	int field5c;
	int short_54Residence;
	int field5C_residence;
	int8_t byte68;
	int8_t pad69[3];
	int alliancePlaza;
	int field70;
	float plazaMaxTime;
	float plazaControlPerSecond;
	float plazaTimer;
	int8_t byte80;
	int8_t pad81[3];
	int field84;
	int field88;
	int field8c;
	int field90;
	int field94;
	int field98;
	int field9c;
	int fielda0;
	int fielda4;
	int fielda8;
	int fieldac;
};

struct crossing
{
	int32_t field0;
	int32_t someID;
	void *startArea;
	float startPosX;
	float startPosY;
	float field14;
	float field18;
	float field1C;
	float field20;
	void *endArea;
	float endPosX;
	float endPosY;
	float field30;
	float field34;
	float field38;
	float field3C;
	float field40;
	int angle;
	float field48;
	float field4C;
	float field50;
	void *linkProxy;
	int32_t proxyNum;
	float field5C;
};


struct battleAreas
{
	char byte_0;
	char byte_1;
	char gap2[2];
	void *areas;
	void *wholeArea;
	crossing *riverCrossings;
	void *offmapModels;
	void *outlinePoints;
	void *wholeAreaOutline;
	void* outlineLists;
	int field_30;
	int numberOfRivers;
	int field_38;
	int field_3C;
	char byte_40;
	char byte_41;
	char byte_42;
	char byte_43;
};


struct battleTerrainData
{
	battleTile battleTiles[1000000];
	float heightOffset;
	int field_F42404;
	int field_F42408;
	int field_F4240C;
	int field_F42410;
	int field_F42414;
	int field_F42418;
	int16_t short_F4241C;
	int16_t short_F4241E;
};

struct cultureAgent
{
public:
	char *cardName;
	int32_t cardNameHash;
	char *infoCardName;
	int32_t infoCardNameHash;
	char *cardName2;
	int32_t cardName2Hash;
	int index1;
	int index2;
	int16_t cost;
	int8_t time;
	int8_t pad23;
	int16_t popCost;
	int16_t someOtherRomeShit;
	int recruitBiasValue;
}; //Size: 0x002C

struct underlay
{
	char *name;
	int nameHash;
	char *path;
	int pathHash;
	float float10;
	int field14;
};

struct cultureCasEntry
{
	int field_0;
	int field_4;
	char *casName;
	int casNameHash;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	underlay *underlay;
};

struct cultureFort
{
	void *modelRigid;
	void *stratModelArrayEntry;
	stringWithHash fortCasName;
	stringWithHash fortWallsCasName;
	int array_18[31];
	stringWithHash stringsHash_94[31];
	char *strings_18c[31];
	stringWithHash stringsHash_208[31];
	stringWithHash uiGenericFort;
	underlay *underlay;
};

struct smthingCult2
{
	int field_0;
	int field_4;
	stringWithHash field_8;
	stringWithHash field_10;
	int field_18;
};

struct cultureSettlement
{
	int field_0;
	int field_4;
	char *casNameCity;
	int casNameCityHash;
	void *casNameCastle;
	int casNameCastleHash;
	int field_18;
	char *strings1C[31];
	char *strings98[31];
	stringWithHash stringHash114[31];
	stringWithHash stringHash20c[31];
	smthingCult2 field_304[5];
	char *tgaFileCity;
	int field_394;
	char *tgaFileCastle;
	int field_39C;
	underlay *underlay;
};

struct culture
{
	int32_t cultureID;
	char *cultureName;
	int32_t cultureNameHash;
	cultureFort fort;
	cultureCasEntry fishingVillage;
	cultureCasEntry portWalls[3];
	cultureCasEntry portBuildings[3];
	cultureCasEntry watchTower;
	int cultureCasEntryCount;
	int32_t fortCost;
	int32_t watchTowerCost;
	int field_444;
	char *portraitMapping;
	int field_44C;
	int rebelStandardIndex;
	struct cultureSettlement cultureSettlements[6];
	struct cultureAgent cultureAgents[6];
	int maxLevel;
};

struct culturesDB
{
	struct culture *cultures;
	int32_t culturesSize;
	int32_t culturesCount;
};

struct portraitsEntry
{
	int32_t count{}; //0x0000
	int32_t *portraitIndexes{}; //0x0004
	int32_t usageCount{}; //0x0008
};

struct portraitDdCharacterEntry
{
public:
	portraitsEntry ages[3]; //0x0000
}; //Size: 0x0024

struct portraitDbEntry
{
public:
	char *cultureName{}; //0x0000
	int32_t cultureNameHash{}; //0x0004
	struct portraitDdCharacterEntry portraits[9]; //0x0008
}; //Size: 0x014C

struct portraitDb
{
public:
	struct portraitDbEntry cultures[7]; //0x0000
}; //Size: 0x0914


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

/* 1869 */
struct gateEntry
{
	char *name;
	int32_t nameHash;
	int32_t health;
	float height;
	float pursuitLockoutRadius;
	float blockedLockoutRadous;
};

/* 1872 */
struct wallEntry
{
	char *name;
	int32_t nameHash;
	int32_t flammability;
	int32_t impactDamage;
	int32_t health;
	float controlAreaRadius;
	int8_t manned;
	int8_t isSelectable;
	int8_t healthExcluded;
	int8_t byte_1b;
	struct UNICODE_STRING **localizedName;
};

/* 1873 */
struct gateWayEntry : wallEntry
{
	int32_t field20;
	struct towerStats *towerStats;
	int32_t field_28;
	int32_t field_2C;
	struct gateEntry **gateEntries;
	int32_t gateEntrySize;
	int32_t gateEntryCount;
	int32_t projectileImpactsAllHitGate;
};

/* 1874 */
struct towerEntry : wallEntry
{
	int32_t field20;
	struct towerStats *towerStats;
	int32_t someListEnd;
	int32_t someListEnd2;
};

/* 1875 */
struct gateHouseEntry : wallEntry
{
	int32_t field20;
	struct towerStats *someList;
	int32_t someListEnd;
	int32_t someListEnd2;
	int32_t field30;
	int32_t field34;
	int32_t field38;
	int32_t field3C;
};

struct descrWalls
{
	struct descrWallEntry *walls;
	int32_t wallsSize;
	int32_t wallNum;
	struct gateEntry *gates;
	int32_t field10;
	int32_t field14;
	int32_t gatesSize;
	int32_t gatesNum;
};



struct descrWallEntry
{
	int32_t siegeTowerSize;
	int32_t level;
	struct wallEntry wall;
	struct gateWayEntry gateway;
	struct towerEntry tower;
	struct gateHouseEntry gatehouse;
	int32_t hasGatehouse;
};

struct eventTrigger
{
	void* eventTrigger_vtbl; //0x0000
	char pad_0004[28]; //0x0004
};

struct smthingBattleBuilding
{
public:
	char pad_0000[100]; //0x0000
	void *worldData; //0x0064
	char pad_0068[220]; //0x0068
}; //Size: 0x0144

struct weaponFX
{
	int field0;
	char *name;
	int field8;
	int fieldC;
	int field10;
	int field14;
	int field18;
};

struct fireRate
{
	int normal;
	int flaming;
};

struct towerStats
{
	int32_t statBitfield;
	int ammoBitfield;
	int missileRange;
	float missileRangeSquared;
	int hasPrimary;
	struct projectile *missile;
	int32_t weaponType;
	int32_t techType;
	int damageType;
	int soundType;
	int32_t attackMinDelay;
	weaponFX weaponEffect;
	struct weaponFX shot_sfx;
	struct weaponFX shot_gfx;
	float fireAngle;
	float slotYawX;
	float slotYawY;
	float slotPitchX;
	float slotPitchY;
	fireRate fireRatesNormal[4];
};

struct buildingBattle
{
public:
	void *vftable /*VFT*/;
	int field4;
	int field8;
	int fieldC;
	int field10;
	void *field_14;
	int type;
	int field_1C;
	int factionId;
	float xCoord;
	float zCoord;
	float yCoord;
	int16_t angle;
	char byte_32;
	char byte_33;
	int endHealth;
	int currentHealth;
	int startHealth;
	struct factionStruct *faction;
	int alliance;
	struct battleResidence *battleResidence;
	int field_4C;
	struct towerEntry *descrWallsEntry;
	int field_54;
	char field_58;
	char pad_59[3];
	int field_5C;
	char field_60;
	char field_61;
	char pad_62[2];
	int field_64;
	int field_68;
	void *fireAmbientRegister;
	void *fireAmbientRegisterSP;
	float float_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84_init1;
	int field_88;
	void *worldData;
	int field_90;
	int index;
	int field_98;
	char field_9C;
	char pad_9D[3];
	int field_A0;
	int field_A4;
	towerStats towerStats;
	int field_15C;
	int32_t statBitfield;
	int ammoBitfield;
	int missileRange;
	float missileRangeSquared;
	int hasPrimary;
	struct projectile *missile;
	int32_t weaponType;
	int32_t techType;
	int damageType;
	int soundType;
	int32_t attackMinDelay;
	weaponFX weaponEffect;
	int32_t statPriArmour;
	int32_t armourSoundType;
	int8_t attackInBattle;
	int8_t armourInBattle;
	int8_t pad52;
	int8_t byte_53;
	int8_t byte_54;
	int8_t byte_55;
	int8_t byte_56;
	int8_t byte_57;
	int field_1B8;
	int field_1BC;
	int field_1C0;
	int field_1C4;
	char byte_1C8;
	char byte_1C9;
	char pad_1CA[2];
	int field_1CC;
	int field_1D0;
	int array_1D4;
	int array_1D4Size;
	int array_1D4Num;
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
	uint16_t field_0x8;
	int8_t isLand;
	int8_t field_0xB;
	int8_t terrainModel;
	int8_t ModelIsHills;
	uint16_t field_0xE;
	int groundType;
	int regionId;
	int32_t choke; //choke value like in toggle_terrain
	uint32_t factionId; //see below
	/*
	 * bit 0 - 4 - faction id
	 * bit 5 - river source
	 * bit 6 - river
	 * bit 7 - crossing
	 * 
	 * bit 8 - cliff
	 * bit 9 - coastal water
	 * bit 10 - cliff not directly bordering water
	 * bit 11
	 * bit 12 - land connection
	 * bit 13 - land connection
	 * bit 14 - sea crossing
	 * bit 15 - road level 1
	 * 
	 * bit 16 - road level 2
	 * bit 17 - road level 3
	 * all rest - unknown or unused
	 */
	uint8_t hasRoad; //bit 0 - devastation status, bit 5 - has road, all rest - unknown or unused
	uint8_t border; //bit 0-2 unknown, bit 3 - land border, bit 4 - sea border (land), bit 5 - sea border (water), bit 6 - unknown, bit 7 - unknown
	uint16_t otherField;
	int8_t objectTypes; //unk character ship watchtower port upgraded_port fort settlement
	int8_t nonPassable;
	int16_t field_0x1E;
	uint32_t armiesNearTile; //0x0028
	uint32_t charactersOnTile; //0x002C
	float mpModifier; //0x0030
};



/* 2190 */
struct unitVerificationElement
{
	unitVerificationElement *nextElementIfEmpty;
	struct unit *unit;
	int32_t field_8;
	int32_t index;
	int32_t field_10;
};

/* 2191 */
struct unitVerificationSection
{
	struct unitVerificationSection *nextSection;
	struct unitVerificationElement unitVerificationElements[128];
};

/* 2192 */
struct unitVerificationTable
{
	struct unitVerificationSection *firstSection;
	struct unitVerificationSection *lastSection;
	struct unitVerificationElement *freeSpot;
	struct unitVerificationElement *lastElement;
	int32_t count;
	int8_t hasElements;
	int8_t pad_15[3];
	int32_t field_18;
	char *name;
};


struct color
{
	int8_t r = 0;
	int8_t g = 0;
	int8_t b = 0;
};

#define GETBLUE(color) ((color) & 0xFF)
#define GETGREEN(color) (((color) >> 8) & 0xFF)
#define GETRED(color) (((color) >> 16) & 0xFF)
#define GETALPHA(color) (((color) >> 24) & 0xFF)
#define SETBLUE(color, red) ((color) = ((color) & 0xFFFFFF00) | (red))
#define SETGREEN(color, green) ((color) = ((color) & 0xFFFF00FF) | ((green) << 8))
#define SETRED(color, blue) ((color) = ((color) & 0xFF00FFFF) | ((blue) << 16))
#define SETALPHA(color, alpha) ((color) = ((color) & 0x00FFFFFF) | ((alpha) << 24))
#define MAKECOLOR(r, g, b, a) ((b) | ((g) << 8) | ((r) << 16) | ((a) << 24))

struct tileColor
{
	tileColor() : color(0), coords({ 0,0 }) {}
	tileColor(uint32_t colorA, int x, int y) : color(colorA), coords({ x,y }) {}
	uint32_t color;
	struct coordPair coords;
};

struct mapImage
{
	std::vector<tileColor> tiles = {};
	bool useBlur = true;
	float blurStrength = 1.0f;
	bool adaptiveBlur = false;
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


struct perimeterBuildings
{
public:
	struct buildingBattle** buildingList; //0x0000
	int32_t buildingListSize; //0x0004
	int32_t buildingNum; //0x0008
}; //Size: 0x000C

struct battleBuildings
{
public:
	int field0;
	int field4;
	int field8;
	int fieldc;
	int field10;
	struct buildingBattle** allBuildings; //0x0014
	int32_t allBuildingsSize; //0x0020
	int32_t allBuildingsNum; //0x0020
	int32_t* allBuildingsInt; //0x0020
	int allBuildingsIntSize;
	int allBuildingsIntNum;
	struct perimeterBuildings *perimeters;
	int perimetersSize;
	int perimitersNum;
	void *array38;
	int array38Size;
	int array38Num;
}; //Size: 0x0444

struct battleResidence
{
public:
	int8_t gateStrength;
	int8_t pad1[3];
	float defendOffset1;
	int field8;
	float defendOffset2;
	int attackerDirection;
	int field18;
	void *ptr_1c;
	struct settlementStruct *settlement;
	struct fortBattleInfo *fort;
	int field24;
	struct factionStruct *faction;
	char byte_2c;
	char byte_2d;
	char byte_2e;
	char byte_2f;
	int32_t fortFortificationLevel;
	struct battleBuildings *battleBuildings;
	int field_38_init2;
	float float_3c;
	float float_40;
	float float_44;
	float float_48;
	float float_4C;
	float float_50;
	float float_54;
	float float_58;
	float float_5C;
	struct plazaStuff *plaza;
	void *worldData;
	int someGroundTypeBitfield;
	int8_t settlementWallsBreached;
	int8_t settlementGateDestroyed;
	char pad_006E[2];
	int8_t alliance;
	char pad_0071[3];
	int32_t field_74;
	void *list_78;
	void *list_78end;
	void *list_78end2;
}; //Size: 0x0144

struct smthingBattSettlement
{
	char pad_0000[120];
};

struct battleSettlementPad
{
	char pad_0000[2000];
};

struct roadNode
{
	int16_t short_0;
	int16_t short_2;
	float xCoord;
	float yCoord;
	int field_C;
	int field_10;
	int8_t byte_14;
	int8_t byte_15;
	int8_t byte_16;
	int8_t byte_17;
};

struct battleStreets
{
	int field0;
	roadNode *streets;
	roadNode *streetsEnd;
	roadNode *streetsEnd2;
};

struct battlePerimeters
{
public:
	char pad_0000[36]; //0x0000
	void *N0008F6BF; //0x0024
	char pad_0028[28]; //0x0028
	void *bmapProperties; //0x0044
	char pad_0048[4]; //0x0048
	void *N0008F6C9; //0x004C
	char pad_0050[28]; //0x0050
	void *N0008FBD7; //0x006C
	char pad_0070[20]; //0x0070
	void *N0008FBDD; //0x0084
	char pad_0088[12]; //0x0088
	void *battlestuff94; //0x0094
	char pad_0098[8]; //0x0098
	void *N0008FBE4; //0x00A0
	char pad_00A4[12]; //0x00A4
	void *N0008FBE8; //0x00B0
	char pad_00B4[12]; //0x00B4
	void *nodes; //0x00C0
	char pad_00C4[52]; //0x00C4
	void *N0008FBFA; //0x00F8
	char pad_00FC[68]; //0x00FC
	void *N0008FC0C; //0x0140
	char pad_0144[8]; //0x0144
	battleStreets battleStreets; //0x014C
	char pad_0154[4]; //0x015C
	void *N0008FC14; //0x0160
	char pad_0164[8]; //0x0164
}; //Size: 0x016C



struct battleSettlement
{
	int32_t field_0;
	struct settlementStruct *settlement;
	struct fortBattleInfo *fortBattleInfo;
	struct battleResidence *battleResidence;
	int32_t field_10;
	struct smthingBattSettlement smthingBattSettlement;
	int32_t field_8c;
	int32_t field_90;
	struct battleResidence battleResidences[212];
	int8_t field_6de4;
	int8_t field_6de5;
	char pad_6DE6[2];
	int32_t xCoord;
	int32_t yCoord;
	int32_t field_6DF0;
	int32_t field_6DF4;
	int32_t field_6DF8;
	int32_t field_6DFC;
	int32_t field_6e00;
	void *array_6e04;
	int32_t array_6e04Size;
	int32_t array_6e04Num;
	struct battleSettlementPad smthing;
	int32_t field_75E0;
	int32_t field_75E4;
	int32_t field_75E8;
	int32_t field_75EC;
	int32_t field_75F0;
	int32_t field_75F4;
	int32_t field_75F8;
	int32_t field_75FC;
	int32_t field_7600;
	int32_t field_7604;
	void *array_7608;
	int32_t field_760c;
	int32_t field_7610;
	int32_t array_7608Sizw;
	int32_t array_7608Num;
	int32_t field_761C;
};

struct projectile
{
	char pad_0000[4];
	char *name;
	int32_t nameHash;
	int32_t specialType;
	struct projectile *flamingProjectile;
	int8_t shatterDust;
	int8_t shatterDebris;
	int8_t vanishDust;
	int8_t vanishDebris;
	int8_t fiery;
	int8_t particleTrail;
	int8_t aimed;
	int8_t invertModelZ;
	int8_t spin;
	int8_t rocket;
	int8_t explosive;
	int8_t bool1f;
	float spinAmount;
	void *ptr24;
	float minAngle;
	float maxAngle;
	int8_t preferHigh;
	int8_t byte31;
	int8_t byte32;
	int8_t byte33;
	char pad_0034[4];
	float maxVelocity;
	char pad_003C[12];
	float minVelocity;
	float float4c;
	float float50;
	char pad_0054[20];
	float radius;
	float mass;
	float area;
	float accuracyVsUnits;
	float accuracyVsBuildings;
	float accuracyVsTowers;
	int8_t affectedByRain;
	int8_t damageToTroops;
	int8_t groundShatter;
	int8_t bounce;
	float bounceFloat1;
	float bounceFloat2;
	float bounceFloat3;
	float bounceFloat4;
	int8_t erratic;
	int8_t isBodyPiercing;
	int16_t short96;
	int8_t destroyMaxRange;
	int destroyMaxRangeVariation;
	char pad_0098[20];
	int32_t damage;
	char end_effect1[32];
	char *endEffect;
	char pad_00DC[24];
	char *endManEffect;
	char pad_00F8[24];
	char *endPackageEffect;
	char pad_0114[24];
	char *endShatterPackageEffect;
	char pad_0130[24];
	char *endShatterManEffect;
	char pad_014C[24];
	char *endShatterEffect;
	char pad_0168[48];
	char areaEffect[24];
	char pad_01B0[48];
	float EffectOffset;
	char pad_01E4[588];
};

struct watchTowerModel {
	struct model_Rigid* modelP;
	undefined field_0x4[26];
};

//watchtower
struct watchTowerStruct {
	void* watchTowerVtable; //0x0000
	DWORD* nextObject; //0x0004
	DWORD* previousObject; //0x0008
	int xCoord;
	int yCoord;
	int somethingIdle;
	bool inSettOrNotArmyLead;
	char pad19[3];
	float floatCheckedForLessThen1ForMove;
	bool fieldx20;
	char pad_0021[3]; //0x0021
	struct watchTowerModel* model;
	int32_t regionID; //0x0028
	struct factionStruct* faction; //0x002C
	struct settlementStruct* settlement; //0x0030
	void* trackedPointerArmyVtable; //0x0034
	struct stackStruct* blockingArmy; //0x0038
	int32_t factionID; //0x003C
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

struct movePoint
{
	float float1;
	int32_t field4;
	float float2;
	int32_t tileIndex;
	int32_t xCoord;
	int32_t yCoord;
	struct movePoint *nextCoords;
};

struct movementExtentTile
{
	int tileIndex;
	float movePoints;
	int smthingUseMovepoints;
};

struct characterMovementExtents
{
	void * vtbl;
	int tilesMoved;
	int fieldx8_init7;
	int tileIndex;
	int xCoord1;
	int yCoord1;
	int xCoord2;
	int yCoord2;
	int fieldx20;
	movementExtentTile* tiles;
	int tilesEnd;
	int tilesEnd2;
	struct character *character;
	int boolx34;
};

struct trackedPointerCharacter
{
	void *vtbl /*VFT*/;
	struct character *character;
};

struct sundryStruct
{
	void* vtable; //0x0000
	DWORD* nextObject; //0x0004
	DWORD* previousObject; //0x0008
	int xCoord;
	int yCoord;
	int somethingIdle;
	bool inSettOrNotArmyLead;
	char pad19[3];
	float floatCheckedForLessThen1ForMove;
	bool fieldx20;
	char pad_0021[3]; //0x0021
	int selectIndicators;
	int field_28;
	float xCoordFloat;
	int field_30;
	float yCoordFloat;
	int8_t gap38[4];
	struct character *selectedCharacter;
	struct settlementStruct *selectedSettlement;
	struct fortStruct *selectedFort;
	int field_48;
	float field_4C_rapidChange;
	int field_50_init1_changeToMinusOne;
	int selectionType;
};

struct rallyPointStruct : sundryStruct
{
   int field_58_initMinus1;
};


struct selectionInfo
{
  float floats[119];
  struct sundryStruct *selectedLandUnitCard;
  struct sundryStruct *selectedShipUnitCard;
  struct sundryStruct *selectedCharacter;
  struct sundryStruct *hoveredCharacter;
  struct sundryStruct *selectedSettlement;
  struct sundryStruct *hoveredSettlement;
  struct sundryStruct *selectedRallyPoint;
  struct sundryStruct *hoveredRallyPoint;
  struct sundryStruct *selectedEnemyCharacter;
  struct sundryStruct *selectedEnemySettlement;
  struct rallyPointStruct **rallyPoints;
  int32_t rallyPointsSize;
  int32_t rallyPointsNum;
  int32_t N0002DC9B;
  void *array22;
  int32_t array22Size;
  int32_t array22Count;
};

struct stratPathFinding
{
	float mpRelated;
	struct trackedPointerCharacter trackedPointerCharacter;
	int characterType;
	struct factionStruct *faction;
	int invalidOrReached;
	int8_t noSpeedUp;
	int8_t bytex19;
	int8_t bytex1A;
	int8_t bytex1B;
	int stratWaterCreatedMaybe;
	int8_t bytex20;
	int8_t someBoolCheckForCaptureStuff;
	int8_t bytex22;
	int8_t bytex23;
	void *floatingGeneral1;
	void *floatingGeneral2;
	void *charVerification;
	void *charVerificationPos;
	int fieldx34;
	int somethingBattleCapturedStatus;
	int fieldx3C;
	int fieldx40;
	int fieldx44;
	int fieldx48;
	int fieldx4C;
	int fieldx50;
	int fieldx54;
	int fieldx58_12;
	int8_t fieldx5C;
	int8_t fieldx5D;
	int8_t fieldx5E;
	int8_t fieldx5F;
	int fieldx60;
	int fieldx64;
	int fieldx68;
	int fieldx6C;
	struct character **characterArray1;
	int characterArray1Size;
	int characterArray1Num;
	struct character **characterArray2;
	int characterArray2Size;
	int characterArray2Num;
	int fieldx88;
	characterMovementExtents **characterMovementExtents;
	int characterMovementExtentsEnd;
	int characterMovementExtentsEnd2;
	int fieldx98;
	int fieldx9C;
	int fieldxA0;
	int fieldxA4;
	struct coordPair *coords;
	int coordsSize;
	int coordsNum;
};


struct factionDiplomacy
{
public:
	char pad_0000[4]; //0x0000
	int state;  /* diplomatic state(war, alliance, peace)(600/0/200) */
	int trade; /* trade rights(0 or 1) */
	int protectorate; /* protectorate or not(15 or 6) */
	float factionStanding; //0x0010
	int32_t protectoratePayment; //0x0014
	int32_t numTurnsAllied; //0x0018
	int32_t numTurnsWar; //0x001C
	int32_t numTurnsPeace; //0x0020
	int32_t numTurnsTrade; //0x0024
	int32_t numTurnsAccess; //0x0028
	int field_2c;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int32_t TurnsSinceLastDemand;
	int field_0048;
	int giveMoneyAmount;
	int field_0050;
	int field_0054;
	int field_0058;
	int field_005C;
	int field_0060;
	int field_0064;
	int field_0068;
	int field_006C;
	int field_0070;
	int32_t turnsSinceMapInfoGiven; //0x0074 -no start at 0 but like 10 on campaign start
	int32_t acceptRejectOfferRatio;
	int16_t N0001196B;
	int16_t N0001F222;
	int32_t turnsSinceMapInfoTaken; //0x0080 -no start at 0 but like 10 on campaign start
	int32_t numTurnsCeasefire; //0x0084
}; //Size: 0x0088

struct trackedCharacter
{
public:
	void* vtbl; //0x0000
	struct character* character; //0x0004
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
	struct characterRecord* pope; //0x0004
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

struct factionHordeInfo
{
	factionStruct *faction;
	int someFactionID;
	int xCoord;
	int yCoord;
	int hordeUnitCount;
	char field_14;
	char field_15;
	char field_16;
	char field_17;
	int amountTimesHorded;
	int array1c;
	int array1cSize;
	int array1cNum;
	char isHorde;
	char field_29;
	char field_2a;
	char field_2b;
	int hordeMinUnits2;
};

struct campaign {
	undefined field1_0x0[40];
	int32_t type; //0x0024
	char pad_0028[4]; //0x0028
	uint32_t playableFactionsBitmap; //0x002C
	char pad_0030[4]; //0x0030
	int playerFactionId;
	undefined field3_0x3c[4];
	int hotSeatPlayer2FactionId;
	undefined field5_0x44[232];
	int humanPlayers; /* number of player-controlled factions */
	int playerFacBitMap; /* pow(2,factionId) */
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
	int32_t slaveFactionID;
	void *someArray_objSize0x80;
	int32_t someArraySize;
	int32_t someArrayCount;
	int lastrandomseed;
	struct crusade crusade; //0x0410
	struct jihad jihad; //0x0584
	struct UNICODE_STRING** someUniStringMaybePassword; //0x06F0
	int8_t isAdminPasswordExist; //0x06F4
	int8_t saveEnabled; //0x06F5
	undefined field45_0x6f6[6];
	int8_t hotseatConsole;
	int8_t hotseatConsoleEnabledWithLogon;
	int8_t byte_6fa;
	int8_t byte_6fb;
	int8_t field_06FC_init1;
	int8_t field_06Fd_init52;
	int8_t field_06Fe;
	int8_t field_06Ff;
	int campaignWinnerID;
	int8_t byte_0704;
	int8_t byte_0705;
	int8_t byte_0706;
	int8_t byte_0707;
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
	roadStruct **roads;
	int32_t roadsSize;
	int32_t roadsNum;
	int8_t byte_21638;
	int8_t pad_21639;
	int8_t pad_2163a;
	int8_t pad_2163b;
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
	struct character* selectedCharacter; //0x0198
	char pad_019C[12]; //0x019C
	struct fortStruct* selectedFort; //0x01A8
};

struct gameDataAllStruct {
	undefined field_0x0[40];
	struct campaign* campaignData;
	void* battleData;
	void* uiData;
	struct stratMap* stratMap;
	struct stratMap* stratMap2;
	struct stratMap* stratMap3;
	struct stratMap** stratMapPtrPtr;
	struct campaign* campaign2;
	struct campaign** campaignPtrPTr;
	struct campaign* campaign3;
	struct battleDataS* battleHandler;
	struct battleDataS** battleHandlerPtr;
	struct battleDataS* battleHandler2;
	int* localFactionID2;
	int* localFactionID;
	struct stratPathFinding* stratPathFinding;
	struct stratPathFinding* stratPathFinding2;
	struct stratPathFinding** stratPathFindingPtr;
	void* field_40BCCPtr;
	battleTerrainData* battleTerrainDataPtr;
	battleTerrainData* battleTerrainData2;
	struct battleSettlement* battleSettlement;
	struct battleSettlement* battleSettlement2;
	void* struct_40bd0Ptr;
	void* struct_40bd0Ptr2;
	void* battleGameHandler;
	void* battleGameHandler2;
	void* rtmHandler;
	void* rtmHandler2;
	void* stratMapGameHandler;
	void* stratMapGameHandler2;
	void* menuHandler;
	void* menuHandler2;
	void* battleMapStuff;
	void* battleMapStuff2;
	selectionInfo* selectInfo;
	selectionInfo** selectionInfoPtr;
	selectionInfo** selectionInfoPtr2;
	void* gameOptions;
	void* gameOptions2;
	void* someArray;
	void* someArray2;
	void* resources;
	void* resources2;
	void* nonNetworkGameManager;
	void* networkGameManager;
	void* replayGameManager;
	void* camControl;
	void* waypointsDisplay;
	struct uiCardManager* uiCardManager;
	void* uiShortcuts;
	struct uiCardManager* uiCardManager2;
	void* uiMessageHandler;
	struct uiManager* uiManager;
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
	struct character* character;//0x0004
	int32_t generalNumKillsBattle; //0x0008
	float generalHPRatioLost; //0x000C
	float totalValue; //0x0010
	float battleOdds; //0x0014
	char pad_0018[12]; //0x0018
	struct character* killedGenerals; //0x0024
	int32_t killedGeneralsSize; //0x0028
	int32_t numKilledGenerals; //0x002C
	char pad_0030[24]; //0x0030
	struct character* generalsDied; //0x0048
	int32_t generalsDiedSize; //0x004C
	int32_t generalsDiedCount; //0x0050
	struct character* killedCaptains; //0x0054
	int32_t killedCaptainsSize; //0x0058
	int32_t killedCaptainsCount; //0x005C
	char pad_005C[60]; //0x005C
	struct battleUnit* units; //0x009C
	int32_t unitArraySize; //0x00A0
	int32_t unitCount; //0x00A4
	char pad_00A8[20]; //0x00A8
};//Size: 0x00BC

struct battlePos
{
	float xCoord;
	float yCoord;
};

struct deploymentAreaS {
	void* vftable;
	battlePos* coordsPairs;
	int coordsSize;
	int coordsNum;
	int outLine;
	int numOutlineSegs;
	int width;
	int height;
	float centreX;
	float centreY;
	float facingX;
	float facingY;
};

struct battleSideArmy {
	undefined field_0x0[4];//0x0000
	struct stackStruct* stack;//0x0004
	int field_0x8;
	int8_t isReinforcement;
	int8_t byte_0xD;
	int8_t byte_0xE;
	int8_t byte_0xF;
	int isAiControlled;
	int8_t byte_0x14;
	int8_t byte_0x15;
	int8_t byte_0x16;
	int8_t byte_0x17;
	int field_0x18;
	int field_0x1C;
	int field_0x20;
	int field_0x24;
	int field_0x28;
	int field_0x2C;
	int8_t byte_0x30;
	int8_t byte_0x31;
	int8_t byte_0x32;
	int8_t byte_0x33;
	struct deploymentAreaS* deploymentArea;//0x0048
	undefined field_0x38[40];
};

struct aiDetachmentTactic
{
	DWORD *vftable /*VFT*/;
	struct aiDetachment *aiDetachment;
	char hasBeenConsidered;
	char isExecuting;
	char hasExecuted;
	char field_B;
	int phaseStarted;
	int arrayOfTenRandomValues[10];
	char startedTactic;
	char differentUnitsThisUpdate;
	unit **units;
	int unitsSize;
	int unitNum;
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
	struct aiDetachmentUnit* aiDetachUnits; //0x0008
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

struct reinforcingArmy
{
	stackStruct *army;
	stackStruct *commandingArmy;
	int field_8;
};


struct battleSide {
	bool isDefender;//0x0000
	uchar canWithdraw;//0x0001
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
	struct battleSideArmy armies[64];//0x0058 
	int32_t battleSideArmyNum; //0x1858
	struct factionStruct* factions[8]; //0x185C
	int32_t factionCount; //0x187C
	char pad_1880[4]; //0x1880
	struct unit** enemyUnits; //0x1884
	int32_t enemyUnitsSize; //0x1888
	int32_t enemyUnitsNum; //0x188C
	struct stackStruct** enemyArmies; //0x1890
	int32_t enemyArmiesSize; //0x1894
	int32_t enemyArmiesNum; //0x1898
	reinforcingArmy reinforcementArmies[8];
	int reinforceArmyCount;
	int balanceOfPower;
	int32_t activeArmyStrength;
	int32_t aiUnitStrengthTotal;
	int armyPower;
	int32_t totalStrength;
	int32_t previousArmyStrength;
	int32_t soldierCount;
	float battleOdds;
	float lastBattleOdds;
	float someFloatUnitCostGetDeducedFrom;
	int totalSoldierCustomCostValues;
};

struct playerArmy
{
public:
	int8_t inBattle;
	int8_t deploymentTimer;
	int8_t field2;
	int8_t field3;
	uint32_t factionId;
	int field8;
	int fieldC;
	int field10;
	stackStruct *army;
}; //Size: 0x0018

struct autoResolveData
{
	int int1;
	int int2_checkForFiveOrFour;
	int int3;
	int int4_Init6;
	int array_Size0x100;
	int array_Size0x100Size;
	int array_Size0x100Num;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	stackStruct *array2Armies;
	int array2Size;
	int array2Num;
	int array3;
	int array3Size;
	int array3Num;
	int wallEquipmentAndInfantry;
	int field_48;
	int8_t byte_4C;
	int8_t hasWonBattle;
	int8_t byte_4E;
	int8_t maybeIsNotNaval;
	int8_t isPlayerControlled;
	int8_t alsoIsPlayerControlled;
	int8_t byte_52;
	int8_t byte_53;
	int8_t byte_54;
	int8_t byte_55;
	int8_t byte_56;
	int8_t byte_57;
	int gate_defence_strength_oil;
	int gate_defence_strength_arrow;
	int gate_defence_strength_default;
	int sett_defence_strength_arrow;
	int sett_defence_strength_default;
	int8_t hasRams;
	int8_t byte_6D;
	int8_t byte_6E;
	int8_t byte_6F;
};

struct fortBattleInfo
{
	struct fortStruct *fort;
	struct stackStruct *garrison;
	struct factionStruct *faction;
	int ownerFactionID;
	int creatorFactionID;
	int fortFortificationLevel;
};

struct terrainFeatures
{
	terrainSegmentVector terrainLines;
	hillVector hills;
	float width;
	float widthHalf;
	float height;
	float heightHalf;
	float widthOnePercent;
	float widthOnePercentInverse;
	float heightOnePercent;
	float heightOnePercentInverse;
	int giantArrayNum;
	int giantArray[1048576];
};

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
	int8_t battleAiEnabled; //0x004C
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
	int32_t totalSoldierCount; //0xCA80
	terrainFeatures *terrainFeatures;
	void *array_CA88_objSize0x120;
	int array_CA88Size;
	int array_CA88Num;
	struct playerArmy playerArmies[31];
	int32_t playerArmyNum;
	int8_t byte_CD80;
	int8_t isFortBattle;
	int8_t byte_CD82;
	int8_t byte_CD83;
	struct fortBattleInfo fortInfo;
	int8_t byte_CD9C;
	int8_t byte_CD9D;
	int8_t byte_CD9E;
	int8_t byte_CD9F;
	int field_CDA0;
	int field_CDA4;
	int field_CDA8;
	int field_CDAC;
	int field_CDB0;
	int unitSize;
	int8_t byte_CDB8;
	int8_t byte_CDB9;
	int8_t byte_CDba;
	int8_t byte_CDbb;
	int tideOfBattle;
	struct autoResolveData autoResolveData;
	struct battleDataS *thisData;
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
	struct character* character; /* character on port (enemy or not, army or not) */
	int xCoord;
	int yCoord;
	undefined field_0x14[28];
	struct settlementStruct* settlement;
	int regionID;
	int angle;
	int tradeFleets;
    int subFactionCultureID;
	void *trackedPointerArmyVtbl;
	struct stackStruct* occupantsArmy; /* if port block by army then it here */
	int32_t numTurnsBlocked; //0x004C
	int fieldx50;
	int fieldx54;
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
	struct stackStruct* dockedArmy;
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

struct campaignDbRecruitment
{
	int32_t recruitmentSlots; //0x0000
	int32_t retrainingSlots; //0x0004
	bool deplenishPoolsWithCaps; //0x0008
	char pad_0009[3]; //0x0009
	float deplenishMultiplier; //0x000C
	float deplenishOffset; //0x0010
	bool addDisbandNoCaps; //0x0014
	char pad_0015[3]; //0x0015
	int32_t percentagePoolReductionLost; //0x0018
	int32_t percentagePoolReductionOccupy; //0x001C
	int32_t percentagePoolReductionSack; //0x0020
	int32_t percentagePoolReductionExterminate; //0x0024
	int32_t maxAgentsPerTurn; //0x0028
};

struct campaignDbReligion
{
	int32_t maxWitchesPerRegion; //0x002C
	int32_t maxWitches; //0x0030
	int32_t maxHereticsPerRegion; //0x0034
	int32_t maxHeretics; //0x0038
	int32_t maxInquisitorsPerRegion; //0x003C
	int32_t maxInquisitors; //0x0040
	float maxHereticsConversionModifier; //0x0044
	float hereticConversionRateModifier; //0x0048
	float hereticConversionRateOffset; //0x004C
	float witchConversionRateOffset; //0x0050
	float inquisitorConversionRateModifier; //0x0054
	float inquisitorConversionRateOffset; //0x0058
	float priestConversionRateModifier; //0x005C
	float priestConversionRateOffset; //0x0060
	float witchChanceModifier; //0x0064
	float hereticChanceModifier; //0x0068
	float inquisitorChanceModifier; //0x006C
	int32_t minCardinalPiety; //0x0070
	float convertToHereticBaseModifier; //0x0074
	float convertToHereticUnorthodoxModifier; //0x0078
	int32_t inquisitorTurnStart; //0x007C
};

struct campaignDbBribery
{
	float baseCharacterChance; //0x0080
	float religionModifier; //0x0084
	float combinedAttributeModifier; //0x0088
	float briberAttributeDivisor; //0x008C
	float bribeeAttributeDivisor; //0x0090
	float armySizeModifier; //0x0094
	float baseSettlementChance; //0x0098
	float settlementLoyaltyModifier; //0x009C
	float settlementPopulationModifier; //0x00A0
	float factionStandingDivisor; //0x00A4
	float maxBribeChance; //0x00A8
	float minBribeChance; //0x00AC
	float bribeChanceModifier; //0x00B0
};

struct campaignDbFamilyTree
{
	int32_t maxAge; //0x00B4
	int32_t maxAgeForMarriageMale; //0x00B8
	int32_t maxAgeForMarriageForFemale; //0x00BC
	int32_t maxAgeBeforeDeath; //0x00C0
	int32_t maxAgeOfChild; //0x00C4
	int32_t oldAge; //0x00C8
	int32_t ageOfManhood; //0x00CC
	int32_t daughtersAgeOfConsent; //0x00D0
	int32_t daughtersRetirementAge; //0x00D4
	int32_t ageDifferenceMin; //0x00D8
	int32_t ageDifferenceMax; //0x00DC
	int32_t parentToChildMinAgeDiff; //0x00E0
	int32_t minAdoptionAge; //0x00E4
	int32_t maxAdoptionAge; //0x00E8
	int32_t maxAgeForConception; //0x00EC
	int32_t ageOfManhoodClose; //0x00F0
	int32_t maxNumberOfChildren; //0x00F4
};

struct campaignDbDiplomacy
{
	int32_t maxDiplomacyItems; //0x00F8
	int32_t nullMissionScore; //0x00FC
};

struct campaignDbDisplay
{
	float characterSelectionRadius; //0x0100
	float characterSelectionHeight; //0x0104
	float characterSelectionHeightCrouching; //0x0108
	float diplomacyScrollHeight; //0x010C
	float factionStandingMin; //0x0110
	float factionStandingMax; //0x0114
	bool UseOrigRebelFactionModels; //0x0118
	char pad_0119[3]; //0x0119
	int32_t hudTabTextOffset; //0x011C
	bool useFactionCreatorSettModels; //0x0120
	char pad_0121[3]; //0x0121
	int32_t standardSoldierLimit; //0x0124
	int32_t standardSoldierLevelScale; //0x0128
	bool clearBattleModelsOnNewFaction; //0x012C
	char pad_012D[3]; //0x012D
};

struct campaignDbRansom
{
	float captorReleaseChanceBase; //0x0130
	float captorReleaseChanceChivMod; //0x0134
	float captorRansomChanceBase; //0x0138
	float captorRansomChanceChivMod; //0x013C
	float captorRansomChanceTmMod; //0x0140
	float captiveRansomChanceBase; //0x0144
	float captiveRansomChanceChivMod; //0x0148
	float captiveRansomChanceTmMod; //0x014C
	float captiveRansomChanceMsmMod; //0x0150
};

struct campaignDbAutoresolve
{
	float minCapturePercent; //0x0154
	float maxCapturePercent; //0x0158
	float lopsidedThresh; //0x015C
	float lopsidedHnMod; //0x0160
	int32_t separationMissileAdd; //0x0164
	float navalSinkModifier; //0x0168
	float navalSinkOffset; //0x016C
	float navalSinkMax; //0x0170
	float sallyAtDefDrawDivisor; //0x0174
	bool useNewSettAutoResolve; //0x0178
	char pad_0179[3]; //0x0179
	int32_t gateDefenceNumOilAttacks; //0x017C
	float gateDefenceStrengthOilBase; //0x0180
	int32_t gateDefenceNumArrowAttacks; //0x0184
	float gateDefenceStrengthArrowBase; //0x0188
	float gateDefenceStrengthArrowLevelModifier; //0x018C
	int32_t gateDefenceNumDefaultAttacks; //0x0190
	float gateDefenceStrengthDefaultBase; //0x0194
	float gateDefenceStrengthDefaultLevelModifier; //0x0198
	int32_t settDefenceNumArrowAttacks; //0x019C
	float settDefenceStrengthArrowBase; //0x01A0
	float settDefenceStrengthArrowModifier; //0x01A4
	float settDefenceStrengthDefaultBase; //0x01A8
	float settDefenceStrengthDefaultModifier; //0x01AC
	float displayStrengthOil; //0x01B0
	float displayStrengthArrow; //0x01B4
	float displayStrengthDefault; //0x01B8
};
struct campaignDbSettlement
{
	float sackMoneyModifier; //0x01BC
	float exterminateMoneyModifier; //0x01C0
	float chivSpfModifier; //0x01C4
	float chivSofModifier; //0x01C8
	float dreadSofModifier; //0x01CC
	float pietyCorruptionModifier; //0x01D0
	float pietyAdminSifModifier; //0x01D4
	float portToPortMpMin; //0x01D8
	float heresyUnrestNodifier; //0x01DC
	float religionUnrestModifier; //0x01E0
	int32_t siegeGearRequiredForCityLevel; //0x01E4
	int32_t noTowersOnlyForCityLevel; //0x01E8
	int32_t minTurnKeepRebelGarrison; //0x01EC
	bool destroyEmptyForts; //0x01F0
	bool canBuildForts; //0x01F1
	char pad_01F2[2]; //0x01F2
	float raceGameCostsModifier; //0x01F4
	float altRelAlliedModifier; //0x01F8
	float altRelGovModifierBase; //0x01FC
	float altRelGovCoefficient; //0x0200
};

struct campaignDbRevolt
{
	float endTurnModifier; //0x0204
	float excommunicatedModifier; //0x0208
	float newLeaderModifier; //0x020C
	float maxEffectiveLoyalty; //0x0210
	float rebelRegionModifier; //0x0214
	float shadowRegionModifier; //0x0218
	float rebelBorderModifier; //0x021C
	float shadowBorderModifier; //0x0220
	float numUnitsModifier; //0x0224
	float captainModifier; //0x0228
	float minRevoltChance; //0x022C
	float maxRevoltChance; //0x0230
	float aiRevoltModifier; //0x0234
	float shadowAuthorityModifier; //0x0238
	float shadowAuthorityModifierSett; //0x023C
};

struct campaignDbHorde
{
	int32_t endTargetFactionBonus; //0x0240
	int32_t startTargetFactionBonus; //0x0244
	int32_t farmingLevelBonus; //0x0248
	int32_t sharedTargetBonus; //0x024C
	int32_t disbandingHordeBonus; //0x0250
	int32_t hordeStartingRegionBonus; //0x0254
	int32_t hordeTargetResourceBonus; //0x0258
};

struct campaignDbMerchants
{
	float baseIncomeModifier; //0x025C
	float tradeBonusOffset; //0x0260
};

struct campaignDbAgents
{
	float denounceInquisitorBaseChance; //0x0264
	float DenouncePriestBaseChance; //0x0268
	float denounceAttackModifier; //0x026C
	float denounceDefenceModifier; //0x0270
	int32_t denounceChanceMax; //0x0274
	float assassinateBaseChance; //0x0278
	float assassinateAttackModifier; //0x027C
	float assassinateDefenceModifier; //0x0280
	float assassinatePublicModifier; //0x0284
	float assassinatePersonalModifier; //0x0288
	float assassinateCounterSpyModifier; //0x028C
	float assassinateAgentModifier; //0x0290
	float assassinateOwnRegionModifier; //0x0294
	float assassinateAssassinateAttrModifier; //0x0298
	int32_t assassinateChanceMin; //0x029C
	int32_t assassinateChanceMax; //0x02A0
	float denounceHereticAttemptModifier; //0x02A4
	float denounceCharacterAttemptModifier; //0x02A8
	float acquisitionBaseChance; //0x02AC
	float acquisitionLevelModifier; //0x02B0
	float acquisitionAttackTradeRightsModifier; //0x02B4
	float acquisitionDefenceTradeRightsModifier; //0x02B8
	int32_t acquisitionChanceMin; //0x02BC
	int32_t acquisitionChanceMax; //0x02C0
	float inquisitorCrtHeresyDivisor; //0x02C4
	float inquisitorCrtPfpModifier; //0x02C8
	float inquisitorCrtPfpModifierMin; //0x02CC
	float inquisitorCrtPfpModifierMax; //0x02D0
	float inquisitorCrtChanceMax; //0x02D4
	float spyBaseChance; //0x02D8
	float spyLevelModifier; //0x02DC
	float notSpyLevelModifier; //0x02E0
	float spyPublicModifier; //0x02E4
	float spyCounterSpyModifier; //0x02E8
	float spyDistanceModifier; //0x02EC
	float spySecretAgentTargetModifier; //0x02F0
	float spySedentaryTurnsModifier; //0x02F4
	float spyAllianceModifier; //0x02F8
	float spyTargetEngagedModifier; //0x02FC
	float spyInSettlementModifier; //0x0300
	float spyWatchtowerModifier; //0x0304
	float spyInOwnRegionModifier; //0x0308
	int32_t spyChanceMin; //0x030C
	int32_t spyChanceMax; //0x0310
};

struct campaignDbCrusades
{
	int32_t requiredJihadPiety; //0x0314
	float maxDisbandProgress; //0x0318
	float nearTargetNoDisbandDistance; //0x031C
	int32_t disbandProgressWindow; //0x0320
	int32_t crusadeCalledStartTurn; //0x0324
	int32_t jihadCalledStartTurn; //0x0328
	float movementPointsModifier; //0x032C
};

struct campaignDbAi
{
	float priestReligionMin; //0x0330
	float priestReligionMax; //0x0334
	float priestHeresyMin; //0x0338
	float priestHeresyMax; //0x033C
	float priestReligionExport; //0x0340
	float priestMaxProdTurns; //0x0344
	int32_t merchantMinSurvivalAcquire; //0x0348
	float attStrModifier; //0x034C
	float siegeAttStrModifier; //0x0350
	float crusadeAttStrModifier; //0x0354
	float sallyAttStrModifier; //0x0358
	float ambushAttStrModifier; //0x035C
	float strLimitWeak; //0x0360
	float strLimitStrong; //0x0364
};

struct campaignDbMisc
{
	int32_t fortDevastationDistance; //0x0368
	int32_t armyDevastationDistance; //0x036C
	float fortDevastationModifier; //0x0370
	float armyDevastationModifier; //0x0374
	bool allowEnemyForts; //0x0378
	char pad_0379[3]; //0x0379
	float siegeMovementPointsModifier; //0x037C
	float cavalryMovementPointsModifier; //0x0380
};

struct campaignDb
{
	campaignDbRecruitment campaignDbRecruitment;
	campaignDbReligion campaignDbReligion;
	campaignDbBribery campaignDbBribery;
	campaignDbFamilyTree campaignDbFamilyTree;
	campaignDbDiplomacy campaignDbDiplomacy;
	campaignDbDisplay campaignDbDisplay;
	campaignDbRansom campaignDbRansom;
	campaignDbAutoresolve campaignDbAutoresolve;
	campaignDbSettlement campaignDbSettlement;
	campaignDbRevolt campaignDbRevolt;
	campaignDbHorde campaignDbHorde;
	campaignDbMerchants campaignDbMerchants;
	campaignDbAgents campaignDbAgents;
	campaignDbCrusades campaignDbCrusades;
	campaignDbAi campaignDbAi;
	campaignDbMisc campaignDbMisc;
}; //Size: 0x0580

struct campaignDbExtra
{
	bool clearPoolsWithCaps; //0x0000
	bool addInitialWithCaps; //0x0001
	bool forceClampToMax; //0x0002
	char pad_0003[1]; //0x0003
	float witchConversionRateModifier; //0x0004
	bool inquisitorTargetCrusades; //0x0008
	char pad_0009[3]; //0x0009
	float foundingConversionDefaultRate; //0x000C
	float ownerConversionDefaultRate; //0x0010
	float neighbourNormaliseWeight; //0x0014
	float governorConversionRateOffset; //0x0018
	float governorConversionRateModifier; //0x001C
	float spyConversionRateOffset; //0x0020
	float spyConversionRateModifier; //0x0024
	float spyConversionRateForeignModifier; //0x0028
	bool bribeToFamilyTree; //0x002C
	bool enemiesRejectGifts; //0x002D
	bool useBalanceOwed; //0x002E
	bool recruitmentSortSimple; //0x002F
	bool keepOriginalHereticPortraits; //0x0030
	bool altSettOrderColors; //0x0031
	bool separateGamesRaces; //0x0032
	char pad_0033[1]; //0x0033
	int32_t chivalryDisplayThreshold; //0x0034
	bool captiveRansomForSlave; //0x0038
	bool switchableDefenceExposed; //0x0039
	char pad_003A[2]; //0x003A
	float gateDefenceStrengthOilLevelModifier; //0x003C
	int32_t settDefenceStrengthNumDefaultAttacks; //0x0040
	int32_t siegeGearRequiredForCastleLevel; //0x0044
	int32_t noTowersOnlyForCastleLevel; //0x0048
	int32_t fortFortificationLevel; //0x004C
	bool alternativeReligiousUnrest; //0x0050
	bool revoltAdditionalArmies; //0x0051
	bool revoltCrusadingArmies; //0x0052
	bool agentsCanHide; //0x0053
	int32_t denounceChanceMin; //0x0054
	float inquisitorCrtChanceMin; //0x0058
	bool inquisitorTargetLeaders; //0x005C
	bool inquisitorTargetHeirs; //0x005D
	bool spyRescaleChance; //0x005E
	bool allowResourceForts; //0x005F
	bool enableHotseatMessages; //0x0060
	bool enableBananaRepublicCheat; //0x0061
	bool enableUnitAccentOverrides; //0x0062
	char pad_0063[1]; //0x0063
}; //Size: 0x0064



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
	struct character* character; /* character on port (enemy or not, army or not) */
	int xCoord;
	int yCoord;
	undefined field_0x14[28];
	struct settlementStruct* settlement;
	int regionID;
	int angle;
	int tradeFleets;
	int subFactionCultureID;
	void *trackedPointerArmyVtbl;
	struct stackStruct* occupantsArmy; /* if port block by army then it here */
	int32_t numTurnsBlocked; //0x004C
	int fieldx50;
	int fieldx54;
	int32_t rallyCoordX; //0x0058
	int32_t rallyCoordY; //0x005C
	void* rallyPoint; //0x0060
	struct factionStruct* fac; /* blockading faction becomes owner */
	undefined field_0x68[4];
	struct stackStruct* occupantsArmy2; /* if port block by army then it here */
	int fieldx70;
	struct stratPortModel* portStratModel;
	struct portDockStrat* portDock;
};


struct BuildingPicEntry
{
public:
	char* buildingPicPath; //0x0000
	int picHash;
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

struct battlefieldEngines
{
public:
	struct siegeEngine **engines; //0x0000
	int32_t enginesSize; //0x0004
	int32_t enginesNum; //0x0008
	int32_t enginesNum2; //0x000C
	int32_t enginesNum3; //0x0010
}; //Size: 0x0014

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

struct lookupVariantNamesList
{
public:
	struct stringWithHash *lookupVariantNames; //0x0000
	struct lookupVariantNamesList *next; //0x0004
	struct lookupVariantNamesList *previous; //0x0008
	int32_t lookupVariantNamesSize; //0x000C
	int32_t lookupVariantNamesNum; //0x0010
}; //Size: 0x0014

struct lookupVariant
{
public:
	lookupVariantNamesList names;
	char *name; //0x0014
	int32_t nameHash; //0x0018
}; //Size: 0x001C

struct lookupVariantsVector
{
public:
	struct lookupVariant *lookupVariants; //0x0000
	struct lookupVariantsVector *next; //0x0004
	struct lookupVariantsVector *prev; //0x0008
	int32_t lookupVariantsSize; //0x000C
	int32_t lookupVariantsNum; //0x0010
}; //Size: 0x0014

struct battleBuildingVector
{
public:
	struct buildingBattleEntry *battleBuildings; //0x0000
	struct battleBuildingVector *next; //0x0004
	struct battleBuildingVector *prev; //0x0008
	int32_t battleBuildingsSize; //0x000C
	int32_t battleBuildingsNum; //0x0010
}; //Size: 0x0014

struct buildingBattleWallsVector
{
public:
	struct buildingBattleWallEntry *buildingBattleWalls; //0x0000
	struct buildingBattleWallsVector *next; //0x0004
	struct buildingBattleWallsVector *prev; //0x0008
	int32_t buildingBattleWallsSize; //0x000C
	int32_t buildingBattleWallsNum; //0x0010
}; //Size: 0x0014

struct exportDescrBuildings
{
public:
	struct hiddenResource hiddenresources[64]; //0x0000
	int32_t hiddenResourceCount; //0x0200
	int32_t field_204; //0x0204
	void *stringLookupTable; //0x0208
	int32_t field_20c; //0x020C
	int32_t field_210; //0x0210
	int32_t field_214; //0x0214
	void *array_218; //0x0218
	int32_t array_218Next; //0x021C
	int32_t array_218NextPRev; //0x0220
	int32_t array_218Size; //0x0224
	int32_t array_218SizeNum; //0x0228
	struct lookupVariantsVector lookupVariantsVector; //0x022C
	void *stringTable1; //0x0240
	void *stringTable2; //0x0244
	struct edbEntry *port; //0x0248
	struct edbEntry *castlePort; //0x024C
	struct edbEntry *coreCityBuilding; //0x0250
	struct edbEntry *coreCastleBuilding; //0x0254
	struct battleBuildingVector battleBuildingVector; //0x0258
	void *array_26c; //0x026C
	void*array_26cNext; //0x0270
	void*array_26cPrev; //0x0274
	int32_t array_26cSize; //0x0278
	int32_t array_26cNum; //0x027C
	struct buildingListPointer buildingsList; //0x0280
	void *array_294; //0x0294
	void *array_294NExt; //0x0298
	void*array_294Prev; //0x029C
	int32_t array_294Size; //0x02A0
	int32_t array_294Num; //0x02A4
	struct buildingBattleWallsVector buildingBattleWallsVector; //0x02A8
	void *uniRepairString1; //0x02BC
	void*uniRepairString2; //0x02C0
	void *uniRepairString3; //0x02C4
	void*uniRepairString4; //0x02C8

}; //Size: 0x0350


struct groupLabel
{
public:
	char *name; //0x0000
	int32_t nameHash; //0x0004
	struct unitGroup *group; //0x0008
}; //Size: 0x000C


struct groupLabels
{
public:
	int32_t count; //0x0000
	struct groupLabel *labels; //0x0004
}; //Size: 0x0008



struct oneSiege {
	void* vtable;
	struct siegeS* siege;
};


struct RallyPointSundry {
	undefined field0_0x0[4];
	void* object;
	undefined field2_0x8[56];
	void* object2;
	undefined field4_0x44[16];
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

struct loadGameHandler
{
	void* vtable;
	struct UNICODE_STRING*** saveGameName;
};

struct trackedPointerUnit {
	undefined field_0x0[4];
	struct unit* unit;
	undefined field_0x8[88];
};

struct trackedUnit {
	undefined field_0x0[4];
	struct unit* unit;
};

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

struct spyingInfo
{
public:
	void *spyingInfo_vtbl; //0x0000
	int8_t spyingInfoFactionArray[31]; //0x0004
	char pad_0023[1]; //0x0023
	int randomNumber; 
	int randomSeed; 
}; //Size: 0x002C


//army data
struct stackStruct { /* structure of stack */
	undefined field_0x0[76];
	struct factionStruct* faction;
	int regionID;
	struct unit** units;
	int32_t unitsSize; //0x0058
	int numOfUnits;
	struct unit **deadUnits;
	int deadUnitsSize;
	int deadUnitsNum;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int somethingModifiesEquipCosts2;
	int characterTypeMaybeModifiyEquimentCost;
	int field_8C;
	bool isAdmiral;
	char pad_0091[7]; //0x0091
	struct stackStruct* boardedArmy; /* if fleet, here army */
	undefined field_0x9c[4];
	struct stackStruct* shipArmy; /* here fleet what contain our army */
	undefined field_0xa4[8];
	struct siegeS* siege;
	struct portDockStrat* blockedPort; /* if port blocked by this army when it here */
	char battleLadders;
	char battleRams;
	char battleTowers;
	char ladders;
	char rams;
	char towers;
	char pad_ba[2];
	struct unitGroup *unitGroups;
	int maxUnitGroups;
	int32_t tilesMovedThisTurnDoesntStatAtZero; //0x00C4
	char pad_00C8[4]; //0x00C8
	uint32_t upkeepModifier; //0x00CC
	char pad_00D0[4]; //0x00D0
	struct character* gen; /* 0 if in settlement/fort */
	struct unit* generalsUnit; //0x00D8
	struct generalInfo* generalInfo; //0x00DC
    float generalCommandRadius; //0x00E0
	int32_t generalBattleCommand;
	int32_t fieldxE8;
	int32_t fieldxEC;
	struct character** characters; //0x00F0
	undefined field_0xf4[4]; //0x00F4
	int charactersNum; //0x00F8
	undefined field_0xfc[4]; //0x00FC
	struct settlementStruct* settlement; //0x0100
	int pad_104;
	stackStruct *commandingArmy;
	char aiControlled;
	char inBattle;
	char hasWithdrawedPreBattle;
	char byte_10F;
	void *trackedPointerArmy_vtbl3;
	struct stackStruct* enemyArmy;
	int32_t inBattle2; //0x0118
	int8_t isInactiveBattlefield; //0x011C
	int8_t N0003D211[3]; //0x011D
    int totalStrengthStart;
	int totalStrength;
	float reform_point_x;
	float reform_point_y;
	int32_t battleSide; //0x0130
	char pad_0134[16]; //0x0134
	int32_t battleAllianceOrSomething; //0x0144
	int32_t battleDifficulty; //0x0148
	int8_t isHalfDestroyed; //0x014C
	char byte_14D;
	char pad_14e[2];
	int winningCombatRelated2;
	int winningCombatRelated1;
	stackStruct *commandingArmyThatGivesCommands;
	struct descrRebelEntry* descrRebel; //0x015C
	int32_t subFactionID; //0x0160
};

struct coords {
	int xCoord;
	int yCoord;
};
struct aiResourcePrivate
{
	void *vftable /*VFT*/;
	int number;
	void* trackedCharVtbl;
	character* character;
	void* trackedArmyVtbl;
	stackStruct* army;
	int field_18;
	void *aiController;
	int field_20;
	int intSometimesTen;
	char field_28;
	char field_29;
	char field_2A;
	char hasToBeTrueForAttackOpen;
	char field_2C;
	char pad_2d[3];
	void *aiController2;
	int integerx34;
	void *aiController3;
	int integer3C;
};



struct animSetModelDB
{
public:
	char pad_0000[4]; //0x0000
	char *name; //0x0004
	char pad_0008[24]; //0x0008
}; //Size: 0x0020

struct modelDbAnims
{
public:
	char pad_0000[4]; //0x0000
	char N000028CE[4]; //0x0004
	char N000028CF[4]; //0x0008
	char pad_000C[20]; //0x000C
	char *N00022FB0; //0x0020
	char pad_0024[52]; //0x0024
	struct animSetModelDB *primaryAnim; //0x0058
	void *N0000092A; //0x005C
	void *N0000092B; //0x0060
	char pad_0064[4]; //0x0064
	struct animSetModelDB *secondaryAnim; //0x0068
	char pad_006C[12]; //0x006C
}; //Size: 0x0078


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
	struct ModelDbMesh* mesh; //0x0024
	char pad_0028[12]; //0x0028
	struct ModelDbEntryTextures* textures; //0x0034
	char pad_0038[12]; //0x0038
	struct ModelDbEntryTextures* attTextures; //0x0044
	char pad_0048[12]; //0x0048
	struct modelDbAnims* animations; //0x0054
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

struct descrMountEntry
{
public:
	char *name; //0x0000
	char pad_0004[4]; //0x0004
	int32_t mountClass; //0x0008
	char *modelName; //0x000C
	char pad_0010[12]; //0x0010
	float radius; //0x001C
	float xRadius; //0x0020
	float yRadius; //0x0024
	float yOffset; //0x0028
	float height; //0x002C
	float mass; //0x0030
	float bannerHeight; //0x0034
	float bouyancyOffset; //0x0038
	float elephantRootNodeHeight; //0x003C
	float elephantAttackDelay; //0x0040
	float elephantDeadRadius; //0x0044
	float elephantTuskZ; //0x0048
	float elephantTuskRadius; //0x004C
	int32_t elephantNumberOfRiders; //0x0050
	float elephantRiderOffset1X; //0x0054
	float elephantRiderOffset1Y; //0x0058
	float elephantRiderOffset1Z; //0x005C
	char pad_0060[168]; //0x0060
	float rootNodeHeight; //0x0108
	float riderOffSetX; //0x010C
	float riderOffSetY; //0x0110
	float riderOffsetZ; //0x0114
	char pad_0118[8]; //0x0118
	char *waterTrailEffect; //0x0120
	char pad_0124[20]; //0x0124
}; //Size: 0x0138

struct basicStringGame
{
	char pad[0x18];
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
	char *Accent;
	uint32_t accentHash;
	char *BannerFaction;
	uint32_t bannerFactionHash;
	char *BannerUnit;
	uint32_t bannerUnitHash;
	char *BannerHoly;
	uint32_t bannerHolyHash;
	char *BannerMarine;
	uint32_t bannerMarineHash;
	char *BannerSecondary;
	uint32_t bannerSecondaryHash;
	uint32_t field_7c;
	uint32_t field_80;
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
	int8_t testudo;
	int8_t hasSquareFormation;
	char pad_00EE[2];
	uint32_t defaultRanks;
	float UnitSpacingFrontToBackClose;
	float UnitSpacingSideToSideClose;
	float UnitSpacingFrontToBackLoose;
	float UnitSpacingSideToSideLoose;
	char* Soldier;
	char pad_0108[12];
	struct ModelDbEntry* ModelDBEntry;
	uint16_t SoldierCount;
	uint16_t pad11a;
	float Mass;
	float Width;
	float Height;
	uint32_t StatPri;
	DWORD Ammunition;
	DWORD MissleRange;
	float MissleRangeSquared;
	DWORD HasPrimary;
	projectile* StatPriMissle;
	DWORD WeaponType;
	DWORD TechType;
	DWORD DamageType;
	DWORD SoundType;
	DWORD AttackMinDelay;
	char pad_0154[4];
	char WeaponShootEffect[20];
	int pad_016C;
	uint32_t StatSec;
	DWORD SecAmmunition;
	DWORD SecMissleRange;
	float SecMissleRangeSquared;
	DWORD HasSecondary;
	projectile* StatSecMissle;
	DWORD SecWeaponType;
	DWORD SecTechType;
	DWORD SecDamageType;
	DWORD SecSoundType;
	DWORD SecAttackMinDelay;
	basicStringGame secWeaponShootEffect;
	int field_pad;
	uint32_t StatPriArmour;
    int armourMaterial;
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
    int field_200;
	int8_t* ArmorUpgradeLevels;
	int8_t* ArmorUpgradesEnd;
	int8_t* ArmorUpgrade2;
    int field_210;
	struct ArmourUpgModels* ArmorUpgradeModels;
	int* ArmorUpgradeModelsEnd;
	int* ArmorUpgrade5;
	int extrasCount;
	struct engineRecord *engineRec;
	int engineExtras;
	DWORD engineStats;
	DWORD engineAmmunition;
	DWORD engineRange;
	float engineRangeSquared;
	DWORD engineHasPrimary;
	projectile *engineMissle;
	DWORD engineWeaponType;
	DWORD engineTechType;
	DWORD engineDamageType;
	DWORD engineSoundType;
	DWORD engineAttackMinDelay;
	basicStringGame engineWeaponShootEffect;
	int field_270;
	uint32_t statTer;
	DWORD terAmmunition;
	DWORD terMissileRange;
	float terMissileRangeSquared;
	DWORD hasTertiary;
	projectile *statTerMissile;
	DWORD terWeaponType;
	DWORD terTechType;
	DWORD terDamageType;
	DWORD terSoundType;
	DWORD terAttackMinDelay;
	basicStringGame terWeaponShootEffect;
	int field_2B8;
	uint32_t statSecArmour;
	int secArmourMaterial;
	struct descrMountEntry* mount;
	struct ModelDbEntry* mountModel;
	uint32_t statMount;
	DWORD mountAmmunition;
	DWORD mountMissileRange;
	float mountMissileRangeSquared;
	DWORD hasMountStats;
	projectile *statMountMissile;
	DWORD mountWeaponType;
	DWORD mountTechType;
	DWORD mountDamageType;
	DWORD mountSoundType;
	DWORD mountAttackMinDelay;
	basicStringGame mountWeaponShootEffect;
	int field_310;
	int field_314;
	int statArmourMount;
	int mountMaterialType;
	void *descrAnimal;
	void *animalBmdbEntry;
	uint32_t statAnimal;
	DWORD animalAmmunition;
	DWORD animalMissileRange;
	float animalMissileRangeSquared;
	DWORD hasAnimalStats;
	projectile *statAnimalMissile;
	DWORD animalWeaponType;
	DWORD animalTechType;
	DWORD animalDamageType;
	DWORD animalSoundType;
	DWORD animalAttackMinDelay;
	basicStringGame animalWeaponShootEffect;
	int field_36C;
	DWORD statArmourAnimal;
	DWORD animalMaterialType;
	void* mountedEngine;
	void* Ship;
	uint32_t ownership;
	int32_t eraAmount;
	void* eraOwnerships;
	int eraOwnershipsEnd;
	int eraOwnershipsEnd2;
	float aiUnitValuePerSoldier;
	float aiUnitValue;
    int field_39c;
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
	struct factionRecord* facDescrs;
	int capacity;
	int size;
};

struct uiManager
{
public:
	char pad_0000[12]; //0x0000
	char *N00009D87; //0x000C
	char pad_0010[32]; //0x0010
	void *dragObjectContainer; //0x0030
	struct stratUIStruct *stratUI; //0x0034
}; //Size: 0x00A0
struct buildingInfoScroll
{
	char pad[812];
	settlementStruct* settlement;
	char pad2[12];
	struct building* building;
	char pad3[12];
	edbEntry* entry;
};
struct unitInfoScroll
{
	char pad[772];
	unit* unit;
	char pad2[20];
	eduEntry* entry;
};

// Settlement UI Stuff
struct stratUIStruct
{
public:
	char pad_0000[84]; //0x0000
	struct settlementInfoScroll *settlementInfoScroll; //0x0054
	int pad_0058[6]; //0x0000
	buildingInfoScroll* buildingInfoScroll;
	unitInfoScroll* unitInfoScroll;
}; //Size: 0x0164


struct settlementInfoScroll
{
public:
	char pad_0000[40]; //0x0000
	uint32_t N000218A6; //0x0028
	char pad_002C[136]; //0x002C
	float N000218C9; //0x00B4
	float N000218CA; //0x00B8
	float N000218CB; //0x00BC
	float N000218CC; //0x00C0
	char pad_00C4[576]; //0x00C4
	struct settlementStruct *settlement; //0x0304
	char pad_0308[12]; //0x0308
	void *N00021961; //0x0314
	char pad_0318[16]; //0x0318
	void *uiTable; //0x0328
	void *uiTable2; //0x032C
	char pad_0330[12]; //0x0330
	void *uiSettlementFrame; //0x033C
	char pad_0340[4]; //0x0340
	void *characterInfoFrame; //0x0344
	void *uiCycleListTax; //0x0348
	void *uiCycleList0; //0x034C
	void *uiCycleList; //0x0350
	char pad_0354[16]; //0x0354
	void *uiCheckBox; //0x0364
	void *uiCheckBox2; //0x0368
	void *uiCheckBox3; //0x036C
	void *N00021978; //0x0370
	char pad_0374[16]; //0x0374
	void *settlementRenameElement; //0x0384
	struct settlementStatsTable *settlementStatsTable; //0x0388
}; //Size: 0x0840

struct settlementStatsTable
{
public:
	char pad_0000[76]; //0x0000
	struct settlementTextStrings *settlementTextStrings; //0x004C
}; //Size: 0x0844

struct settlementTextStrings
{
public:
	struct uiString *incomeString; //0x0000
	struct uiString *incomeValue; //0x0004
	struct uiString *publicOrderString; //0x0008
	struct uiString *publicOrderValue; //0x000C
	struct uiString *populationString; //0x0010
	struct uiString *populationValue; //0x0014
	struct uiString *populationGrowthString; //0x0018
	struct uiString *populationGrowthValue; //0x001C
}; //Size: 0x0020

struct uiString
{
public:
	char pad_0000[72]; //0x0000
	void *N00023379; //0x0048
	void *N0002337A; //0x004C
	char pad_0050[8]; //0x0050
	void *N0002337D; //0x0058
	char pad_005C[4]; //0x005C
	void *settlementText; //0x0060
	char pad_0064[4]; //0x0064
	uint8_t thickness; //0x0068
	uint8_t blue; //0x0069
	uint8_t green; //0x006A
	uint8_t red; //0x006B
	char pad_006C[36]; //0x006C
}; //Size: 0x0090



struct characterArray
{
	character** characters;
	int charactersSize = 8;
	int charactersNum = 0;
	characterArray()
	{
		characters = new character*[8];
	}
	~characterArray()
	{
		delete[] characters;
	}
};


enum class characterTypeStrat
{
	spy = 0,
	assassin = 1,
	diplomat = 2,
	admiral = 3,
	merchant = 4,
	priest = 5,
	general = 6,
	namedCharacter = 7,
	princess = 8,
	heretic = 9,
	witch = 10,
	inquisitor = 11,
	invalid = 12,
	pope = 13
};

inline bool operator ==(int a, characterTypeStrat b)
{
	return static_cast<characterTypeStrat>(a) == b;
}

inline bool operator !=(int a, characterTypeStrat b)
{
	return static_cast<characterTypeStrat>(a) != b;
}


enum class characterAction
{
	movingNormal = 0,
	movingQuickMarch = 1,
	fleeing = 2,
	constructingRoad = 3,
	constructingBridge = 4,
	constructingWall = 5,
	landRallyPath = 6,
	seaRallyPath = 7,
	besiege = 8,
	entrench = 9,
	ambush = 10,
	captureResidence = 11,
	captureTile = 12,
	assault = 13,
	buildingFort = 15,
	buildingWatchtower = 16,
	engineering = 17,
	sapping = 18,
	infiltrate = 19,
	diplomacy = 20,
	bribe = 21,
	quickSail = 22,
	blockade = 23,
	disembark = 24,
	spy = 25,
	assassinate = 26,
	insurrection = 27,
	sabotage = 28,
	dying = 29,
	deploy = 30,
	delayBattle = 31,
	exchange = 32,
	acquire = 33,
	marry = 34,
	denounce = 35,
	invalid = 38,
	idle = -1,
};

inline bool operator ==(int a, characterAction b)
{
	return static_cast<characterAction>(a) == b;
}

inline bool operator !=(int a, characterAction b)
{
	return static_cast<characterAction>(a) != b;
}
    
enum class engineType
{
	catapult = 0,
	trebuchet = 1,
	ballista = 2,
	bombard = 3,
	grandBombard = 4,
	hugeBombard = 5,
	culverin = 6,
	basilisk = 7,
	cannon = 8,
	mortar = 9,
	scorpion = 10,
	serpentine = 11,
	rocketLauncher = 12,
	ribault = 13,
	monsterRibault = 14,
	mangonel = 15,
	tower = 17,
	ram = 18,
	ladder = 19,
	holy_cart = 20,
};

inline bool operator ==(int a, engineType b)
{
	return static_cast<engineType>(a) == b;
}

inline bool operator !=(int a, engineType b)
{
	return static_cast<engineType>(a) != b;
}
enum class formation
{
	horde = 0,
	column = 1,
	square = 2,
	wedge = 3,
	squareHollow = 4,
	phalanx = 5,
	schiltrom = 6,
	shieldWall = 7,
	wall = 8,
	movingThrough = 9,
};

inline bool operator ==(int a, formation b)
{
	return static_cast<formation>(a) == b;
}

inline bool operator !=(int a, formation b)
{
	return static_cast<formation>(a) != b;
}

enum class unitActionType
{
	unitChangeFormation = 0,
	unitChangeFormationClose = 1,
	unitChangeFormationWidth = 2,
	unitChangeSpeed = 3,
	unitChangeMeleeState = 4,
	unitSetupEngine = 5,
	unitCollectEngine = 6,
	unitDropEngine = 7,
	unitHide = 8,
	unitMove = 9,
	UnitMoveIntoPlayableArea = 10,
	unitTurn = 11,
	unitAttackUnit = 12,
	unitAttackBuilding = 13,
	unitAttackEngine = 14,
	unitWithdraw = 15,
	unitFeignRout = 16,
	unitUseAbility = 17,
	unitHalt = 18,
};

inline bool operator ==(int a, unitActionType b)
{
	return static_cast<unitActionType>(a) == b;
}

inline bool operator !=(int a, unitActionType b)
{
	return static_cast<unitActionType>(a) != b;
}

enum aiGtaObjective : int
{
	gtaObjective_INVALID = 0,
	gtaObjective_MOVE_TO_POINT = 1,
	gtaObjective_ATTACK_ENEMY_BATTLEGROUP = 2,
	gtaObjective_DEFEND_TERRAIN_HILL = 3,
	gtaObjective_DEFEND_TERRAIN_FOREST = 4,
	gtaObjective_DEFEND_TERRAIN_AREA = 5,
	gtaObjective_DEFEND_CROSSING = 6,
	gtaObjective_ASSAULT_CROSSING = 7,
	gtaObjective_DEFEND_LINE = 8,
	gtaObjective_SCOUT = 9,
	gtaObjective_WITHDRAW = 10,
	gtaObjective_DEFEND_SETTLEMENT = 11,
	gtaObjective_SUPPORT_DEFEND_SETTLEMENT = 12,
	gtaObjective_ATTACK_SETTLEMENT = 13,
	gtaObjective_SKIRMISH = 14,
	gtaObjective_BOMBARD = 15,
	gtaObjective_ATTACK_MODEL = 16,
	gtaObjective_SALLY_OUT = 17,
	gtaObjective_AMBUSH = 18,
	gtaObjective_ERROR = 19,
};


#pragma pack(pop)