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

class religionDatabase
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

struct regionStruct {
	char* regionName;//0x0000
	int regionNameHash;//0x0004
	char* settlementName; /* null for sea *///0x0008
	int settlementNameHash;//0x000C
	char* legioName; /* null for sea *///0x0010
	int legioNameHash;//0x0014
	undefined field4_0x10[4];//0x0018
	int regionID;//0x001C
	char pad_0020[12]; //0x0020
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
	char pad_0052[58]; //0x0052
	struct mercPool* mercPool;//0x008C
	undefined field6_0x90[36];//0x0090
	float* religionsARR;//0x00B4
	int* religionsEnd; //0x00B8
	undefined field8_0xbc[212];//0x00BC
	struct settlementStruct* settlement;//0x0190
	char pad_0194[8]; //0x0194
	int32_t tileCount; //0x019C
	char pad_0177[12]; //0x0194
	struct neighbourRegion* neighbourRegions;//0x01AC
	int32_t neighbourRegionsCountArraySize; //0x01B0
	int32_t neighbourRegionsNum; //0x01B4
	struct resStrat** resources;//0x01B8
	int32_t resourceCountArrayZie; //0x01BC
	int32_t resourcesNum; //0x01C0
	char pad_01C4[12]; //0x01C4
	uint32_t hiddenResources1; //0x01D0
	uint32_t hiddenResources2; //0x01D4
	char pad_01D8[4]; //0x01D8
	int32_t settlementXCoord; //0x01DC
	int32_t settlementYCoord; //0x01E0
	int32_t portEntranceXCoord; //0x01E4
	int32_t portEntranceYCoord; //0x01E8
	char pad_01EC[4]; //0x01EC
	struct factionStruct* factionOwner;//0x01F0
	char pad_01F4[4]; //0x01F4
	UNICODE_STRING** localizedRegionName; //0x01F8
	UNICODE_STRING** localizedSettlementName; //0x01FC
	UNICODE_STRING** localizedRebelsName; //0x0200
	char* rebelType; //0x0204
	int32_t rebelTypeHash; //0x0208
	int32_t getsCopiedToSettlementAtRebelEndTurn; //0x020C
	int32_t triumphValue; //0x0210
	char pad_0214[8]; //0x0214
};//Size: 0x021C

struct neighbourRegion
{
public:
	struct regionStruct* region; //0x0000
	int32_t regionID; //0x0004
	float distance; //0x0008
	char pad_000C[4]; //0x000C
	void* tileStuff; //0x0010
	char pad_0014[68]; //0x0014
}; //Size: 0x0058

struct stratMap {
	undefined field_0x0[44];
	int mapWidth;
	int mapHeight;
	undefined field_0x34[20];
	struct oneTile* tilesArr;
	undefined field_0x4c[4];
	undefined field_0x50[44];
	struct regionStruct regions[200];
	int regionsNum;
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

struct oneTile {
	void* object;
	undefined field_0x4[6];
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
	int8_t hasRoad;
	int8_t border;
	undefined field_0x22[2];
	int8_t objectTypes;
	int8_t passable;
	undefined field_0x26[14];
};

struct siegeS {
	undefined field_0x0[12];
	struct settlementStruct* goal; /* settlement or fort */
	struct stackStruct* army;
};

struct factionStratMapDescrS { /* see descr_sm_factions.txt */
	int id;
	char* facName;
	DWORD facNameHash;
	undefined field_0xc[60];
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
	undefined field_0x88[88];
};

struct watchTowerModel {
	struct model_Rigid* modelP;
	undefined field_0x4[26];
};

//watchtower
struct watchTowerStruct {
	undefined field_0x0[12];
	int xCoord;
	int yCoord;
	undefined field_0x14[16];
	struct watchTowerModel* model;
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
	char pad_0014[116]; //0x0014
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

struct collegeOfCardinals
{
public:
	char pad_0000[4]; //0x0000
	struct namedCharacter* pope; //0x0004
	char pad_0008[4]; //0x0008
	struct cardinal* cardinalsArray; //0x000C
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
	undefined field1_0x4[52];
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
	char pad_0328[20]; //0x0328
	struct collegeOfCardinals* collegeOfCardinals; //0x033C
	struct factionStruct* papalFaction; //0x0340
	struct factionStruct* currentFactionTurn;
	int factionTurnID; /* FactionTurn? */
	UNICODE_STRING** currentDescrFile; /* path to descr_battle.txt or to descr_strat.txt */
	undefined field20_0x350[28];
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
	undefined field31_0x394[60];
	int8_t field32_0x3d0;
	int8_t field33_0x3d1;
	undefined field34_0x3d2[26];
	float BrigandSpawnValue;
	float PirateSpawnValue;
	int8_t N00001211; //0x03F4
	int8_t smthingDateTurnDisplay; //0x03F5
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
	int8_t hotseatConsole2;
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
};

struct gameDataAllStruct {
	undefined field_0x0[40];
	struct campaign* campaignData;
	undefined field_0x2c[8];
	struct stratMap* stratMap;
	struct campaign* field_0x38;
	undefined field_0x3c[28];
	struct battleDataS* battleHandler;
	undefined field_0x5c[8];
};

struct battleUnit
{
public:
	struct unit* unit; //0x0000
	struct UNICODE_STRING** localizedName; //0x0004
	float someFloat; //0x0008
	int32_t soldiersLost; //0x000C
	char pad_0010[12]; //0x0010
	int32_t soldiersStart; //0x001C
	uint32_t unitsRouted; //0x0020
	char pad_0024[20]; //0x0024
	int8_t hasRouted; //0x0038
	char pad_0039[15]; //0x0039
}; //Size: 0x0048


struct armyAndCharacter { /* in battle leader and leader army */
	struct stackStruct* army;//0x0000
	struct general* character;//0x0004
	int32_t generalNumKillsBattle; //0x0008
	float generalHPRatioLost; //0x000C
	float someFloat; //0x0010
	float battleOdds; //0x0014
	char pad_0018[20]; //0x0018
	int32_t numKilledGenerals; //0x002C
	char pad_0030[36]; //0x0030
	DWORD N0001FC50; //0x0054
	char pad_0058[68]; //0x0058
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
	struct aiObjectiveArrayBattle* aiObjectives; //0x0758
	int32_t aiObjectivesSize; //0x075C
	int32_t aiObjectivesCount; //0x0760
	char pad_0764[4]; //0x0764
	struct aiObjectiveArrayBattle* aiObjectives2; //0x0768
	char pad_076C[4]; //0x076C
	uint32_t objectiveCount; //0x0770
	char pad_0774[44]; //0x0774
	DWORD aiAnalysers; //0x07A0
	char pad_07A4[96]; //0x07A4
}; //Size: 0x0804

struct battleSide {
	bool isDefender;//0x0000
	uchar field_0x1;//0x0001
	bool isCanDeploy;//0x0002
	undefined field_0x3[9];//0x0003
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
	DWORD someArray; //0x1884
	int32_t someArraySize; //0x1888
	int32_t someArrayCount; //0x188C
	DWORD someArray2; //0x1890
	int32_t someArraySize2; //0x1894
	int32_t someArrayCount2; //0x1898
	char pad_189C[104]; //0x189C
	int32_t totalStrenght; //0x1904
	int32_t autoResolveStrenghtMaybe; //0x1908
	char pad_190C[4]; //0x190C
	int32_t totalStrenght2; //0x1910
	int32_t totalStrenght3; //0x1914
	int32_t soldierCount; //0x1918
	float battleOdds; //0x191C
	float battleOdds2; //0x1920
	float someFloat; //0x1924
	char pad_1928[4]; //0x1928
};

struct battleDataS {
	undefined field_0x0[4];//0x0000
	int battleState; /* 0-not in battle,5-active battle,9-results screen,etc 0x0004 */
	uint32_t battleType; //0x0008
	int32_t isNightBattle; //0x000C
	char pad_0010[8]; //0x0010
	int xCoord;//0x0018
	int yCoord;//0x001C
	undefined somethingRelatedToBattleType[16];//0x0020
	int attackerXCoord;//0x0030
	int attackerYCoord;//0x0034
	int defenderXCoord;//0x0038
	int defenderYCoord;//0x003C
	char pad_0040[20]; //0x0040
	int32_t paused; //0x0054
	float speed; //0x0058
	char pad_005C[8]; //0x005C
	float secondsPassed; //0x0064
	char pad_0068[48]; //0x0068
	struct UNICODE_STRING** someUniString; //0x0098
	struct battleSide sides[8]; //0x009C
	int32_t factionSide[31]; //0xC9FC
	int sidesNum;//0xCA78
};//Size: 0xCA7C

struct stratPortModel {
	struct model_Rigid* model_rigid;
	undefined field_0x4[4];
	char* modelPath;
	int modelPathCrypt;
};

//dock (water tile)
struct portDockStrat {
	undefined field_0x0[12];
	int xCoord;
	int yCoord;
	undefined field_0x14[28];
	struct settlementStruct* settlement;
	undefined field_0x34[64];
	struct stratPortModel* dockStratModel;
	struct portBuildingStruct* port;
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
	undefined field_0x34[20];
	struct stackStruct* occupantsArmy; /* if port block by army then it here */
	undefined field_0x4c[24];
	struct factionStruct* fac; /* blockading faction becomes owner */
	undefined field_0x68[12];
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
	int something;
	struct edbEntry* edbEntry;
	struct building* existsBuilding; /* 0 if building dont exist yet */
	struct settlementStruct* settlement;
	int currentLevel;
	int pastLevel;
	int buildCost;
	int something2;
	int turnsToBuild;
	int buildTurnsPassed;
	int petcentBuilded;
	int flags;
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
	int8_t threeIfRetrainingFourIfBoth; //0x000D
	int8_t experience; //0x000E
	char pad_000F[1]; //0x000F
	int8_t N00010830; //0x0010
	int8_t Minus1; //0x0011
	int8_t turnsTrainedAlready; //0x0012
	int8_t procentFinished; //0x0013
	int16_t turnsToTrain; //0x0014
	int16_t cost; //0x0016
	int32_t unitCount; //0x0018
	int8_t N00010833; //0x001C
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

//settlement
struct settlementStruct {
	void* vTable;
	undefined field1_0x4[8];
	UINT32 xCoord; /* x coordinate */
	UINT32 yCoord; /* y coordinate */
	undefined field4_0x14[48];
	struct stackStruct* army; /* army on the settlement */
	struct oneSiege sieges[8];
	BYTE siegesNumber;
	undefined pad_3[3];
	int siegeHoldoutTurns;
	int turnsSieged;
	int N00001EF5;
	int populationSiegeStart;
	undefined pad_8[8];
	BYTE plagued;
	undefined field8_0xA5[167];
	struct settMod* model;
	int descr_culturestxt;
	undefined field11_0x154[16];
	char* name; /* internal name of settlement */
	int nameCrypt;
	UNICODE_STRING** localizedName;
	struct factionStruct* faction; /* faction of the owner */
	undefined field16_0x174[36];
	int level; /* level of the settlement/castle */
	int fac_creatorModNum;
	undefined field19_0x1a0[4];
	BYTE isCastle; /* castle or settlement */
	undefined field21_0x1a5[3];
	UINT32 regionID; /* number of region */
	undefined field_0x1ac[12];
	int priceOfUnitsInRecruitmentSlots;
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
	undefined field29_0x9b8[1100];
	uchar isProvokedRebellion;
	uchar field31_0xe05[3];
	undefined field32_0xe08[24];
	struct general* governor;
	undefined field34_0xe24[32];
	struct resStrat** resources;
	undefined field36_0xe48[4];
	int resourcesNum;
	undefined field38_0xe50[2164];
	intptr_t guildStandings;
	char pad_16C8[56]; //0x16C8
	int32_t Turmoil; //0x1700
	char pad_1704[116]; //0x1704
	int32_t populationLastTurn; //0x1778
	char pad_177C[180]; //0x177C
	int32_t PopGrowthBaseFarm; //0x1830
	int32_t PopGrowthFarms; //0x1834
	int32_t PopGrowthHealth; //0x1838
	int32_t PopGrowthBuildings; //0x183C
	int32_t PopGrowthTaxBonus; //0x1840
	int32_t PopGrowthEntertainment; //0x1844
	int32_t PopGrowthTrade; //0x1848
	int32_t PopGrowthGovernorInfluence; //0x184C
	int32_t PopGrowthSqualor; //0x1850
	int32_t PopGrowthPlague; //0x1854
	int32_t PopGrowthTaxPenalty; //0x1858
	int32_t populationSize; //0x185C
	int32_t PublicOrderGarrison; //0x1860
	int32_t PublicOrderLaw; //0x1864
	int32_t PublicOrderBuildingsEntertainment; //0x1868
	int32_t PublicOrderGovernorInfluence; //0x186C
	int32_t PublicOrderTaxBonus; //0x1870
	int32_t PublicOrderTriumph; //0x1874
	int32_t PublicOrderPopulationBoom; //0x1878
	int32_t PublicOrderEntertainment; //0x187C
	int32_t PublicOrderHealth; //0x1880
	int32_t PublicOrderGarrisonTwo; //0x1884
	int32_t PublicOrderFear; //0x1888
	int32_t PublicOrderGlory; //0x188C
	int32_t PublicOrderSqualor; //0x1890
	int32_t PublicOrderDistanceToCapital; //0x1894
	int32_t PublicOrderNoGovernance; //0x1898
	int32_t PublicOrderTaxPenalty; //0x189C
	int32_t PublicOrderUnrest; //0x18A0
	int32_t PublicOrderBesieged; //0x18A4
	int32_t PublicOrderBlockaded; //0x18A8
	int32_t PublicOrderCulturalUnrest; //0x18AC
	int32_t PublicOrderExcommunication; //0x18B0
	int32_t PublicOrder; //0x18B4
	int32_t FarmsIncome; //0x18B8
	int32_t TaxesIncome; //0x18BC
	int32_t MiningIncome; //0x18C0
	int32_t TradeIncome; //0x18C4
	int32_t DiplomaticIncome; //0x18C8
	int32_t DemolitionIncome; //0x18CC
	int32_t LootingIncome; //0x18D0
	int32_t BuildingsIncome; //0x18D4
	int32_t AdminIncome; //0x18D8
	char pad_18DC[4]; //0x18DC
	int32_t N000025DE; //0x18E0
	int32_t ConstructionExpense; //0x18E4
	int32_t RecruitmentExpense; //0x18E8
	int32_t DiplomaticExpense; //0x18EC
	int32_t CorruptionExpense; //0x18F0
	int32_t EntertainmentExpense; //0x18F4
	int32_t DevastationExpense; //0x18F8
	int32_t TotalIncomeWithoutAdmin; //0x18FC
	char pad_1900[268]; //0x1900
};

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
	struct settlementStruct* settlement;
	undefined field2_0x8[4];
	int xCoord; /* number of x-coord of unit fosition */
	int yCoord; /* number of y-coord of unit fosition */
	undefined field5_0x14[108];
	struct namedCharacter* genChar; /* many important info about character */
	undefined field7_0x84[4];
	struct genMod* genType;
	undefined field9_0x8c[24];
	uchar ifMarkedToKill;
	undefined field11_0xa5[19];
	int ambushState;
	undefined field13_0xbc[12];
	float movepoints1;
	undefined field15_0xcc[4];
	uchar isStopCharacterNeeded; /* set to 1 if character moving now and he stops */
	undefined field17_0xd1[47];
	struct stackStruct* armyLeaded; /* army of the general */
	undefined field19_0x104[4];
	struct unit* bodyguards; /* unit of general */
	struct stackStruct* armyNotLeaded; /* army, if not leader */
	undefined field22_0x110[208];
	undefined field23_0x1e0[4];
	float movepointsModifier;
	float movepointsMax;
	float movepoints2;
	undefined field27_0x1f0[64];
	char* ability; /* custom ability */
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
	undefined pad_0118[168]; //0x0118
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
	struct anchData** anchillaries; /* pointers to character ancillaries, names at  [item number] -0-0c-here) */
	undefined field_0x1f0[4];
	UINT32 anchNum; /* number of character  ancillaries */
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
	undefined field_0x23c[20];
	char* portrait;
	undefined field_0x254[4];
	char* portrait2;
	undefined field_0x25c[8];
	char* portrait_custom; /* custom portrait name */
	undefined field_0x268[4];
	int age;
};

//ancillary of character
struct anchillary { /* structure of ancillary */
	UINT32 index;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	char* anchName;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	char* patchToAnchImage;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
};

struct trait { /* traits of the character */
	struct sometNameStruct* nameS;
	int* level; /* level of trait */
	struct trait* nextTrait;
	undefined field_0xc[8];
};

struct sometNameStruct { /* char* at 0x4 */
	undefined field_0x0[4];
	char* name;
};

struct traitContainer {
	struct trait* trait;
	struct traitContainer* prev;
	struct traitContainer* next;
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
	struct soldierInBattle *self2; //0x0118
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
	void* getsSet0IfHalted; //0x0008
	struct targetUnitStruct* targetUnit; //0x000C
	char pad_0010[4]; //0x0010
	float targetPosX; //0x0014
	float targetPosY; //0x0018
	char pad_001C[24]; //0x001C
	float N0001B8E6; //0x0034
	char pad_0038[4]; //0x0038
	float N0001B8E8; //0x003C
	char pad_0040[4]; //0x0040
	int32_t N0001B8EA; //0x0044
	char pad_0048[96]; //0x0048
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
	struct battleMapPosition *isInTower; //0x005C
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
	struct unit** unitsUnderFireFrom; //0x0C0C
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
	char pad_1E50[28]; //0x1E28
	int16_t ranks; //0x1E76
	char pad_1E78[14]; //0x1E78
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
	undefined field_0x58[4];
	int numOfUnits;
	undefined field_0x60[56];
	struct stackStruct* boardedArmy; /* if fleet, here army */
	undefined field_0x9c[4];
	struct stackStruct* shipArmy; /* here fleet what contain our army */
	undefined field_0xa4[8];
	struct siegeS* siege;
	struct portBuildingStruct* blockedPort; /* if port blocked by this army when it here */
	undefined field_0xb4[32];
	struct general* gen; /* 0 if in settlement/fort */
	undefined field_0xd8[24];
	struct general** characters;
	undefined field_0xf4[4];
	int charactersNum;
	undefined field_0xfc[4];
	struct settlementStruct* settlement;
	undefined field_0x104[32];
	int totalStrength;
	float reform_point_x;
	float reform_point_y;


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
//faction
struct factionStruct {
	undefined field_0x0[180];
	int dipNum; /* number of faction in diplomatic array */
	int cultureID;
	char* ai_label; /* ai_label of faction */
	undefined field_0xc0[4];
	struct settlementStruct* capital; /* capital of the faction */
	struct namedCharacter* leader; /* faction leader */
	struct namedCharacter* heir; /* faction heir */
	struct factionStratMapDescrS* factSmDescr;
	int isPlayerControlled; /* is faction a controlled by player */
	UINT32* ai_assistant;
	undefined field_0xdc[20];
	UINT32 holdRegions;
	undefined field_0xf4[4];
	struct namedCharacter** charactersAll; /* all characters, died, alive, etc */
	undefined field_0xfc[4];
	int numOfCharactersAll; /* all characters, died, alive, etc */
	struct general** characters; /* characters on stratmap */
	undefined field_0x108[4];
	int numOfCharacters; /* characters on stratmap */
	struct stackStruct** stacks;
	undefined field_0x114[4];
	int stackNum;
	int* regionsID;
	undefined field_0x120[4];
	int regionsNum;
	struct settlementStruct** settlements;
	undefined field_0x12c[4];
	int settlementsNum;
	struct fortStruct** forts;
	undefined field_0x138[4];
	int fortsNum;
	struct watchTowerStruct** watchTowers; /* watchtowers */
	undefined field_0x144[4];
	int wathtowersNum;
	struct portBuildingStruct** portBuildings; /* port buildings */
	undefined field_0x150[4];
	int portBuildingsNum;
	undefined field_0x158[68];
	factionTileStruct* tilesFac;
	undefined field_0x1a0[2208];
	UINT32 religion; /* number of religion */
	undefined field_0xa44[84];
	UNICODE_STRING** someString;
	UNICODE_STRING** localizedName;
	undefined field_0xaa0[76];
	int money; /* money of the faction */
	int KingsPurse; /* money of the faction */
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
	char pad_0220[4];
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
	char pad_0380[8];
	void* N00000151;
	void* N00000152;
	void* N00000153;
	char pad_0394[12];
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
struct anchData { /* data of the ancillary */
	struct anchillary* dataAnch;
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

//fort
struct fortStruct {
public:
	undefined field_0x0[4];
	struct general* gubernator;
	undefined field_0x8[4];
	UINT32 xCoord;
	UINT32 yCoord;
	undefined field_0x14[48];
	struct stackStruct* army;
	undefined field_0x48[4];
	struct siegeS* siege;
	undefined field_0x50[144];
	struct stratFortMod* stratModel;
	int regionID;
	struct factionStruct* faction;
	undefined field_0xec[12];
	char* fortType;
	undefined field_0xfc[28];
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

struct descr_sm_factions_list {
	struct factionStratMapDescrS* facDescrs;
	int capacity;
	int size;
};

#pragma pack(pop)