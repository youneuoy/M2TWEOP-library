#pragma once
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

struct regionStruct {
	char* regionName;
	int regionNameHash;
	char* settlementName; /* null for sea */
	int settlementNameHash;
	undefined field_0x10[12];
	int regionID;
	undefined field_0x20[368];
	struct settlementStruct* settlement;
	undefined field_0x194[92];
	struct factionStruct* factionOwner;
	undefined field_0x1f4[40];
};

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
struct tilesS {
	undefined field_0x0[44];
	int xBound;
	int yBound;
	undefined field_0x34[20];
	struct oneTile* tilesArr;
	undefined field_0x4c[4];
	undefined field_0x50[44];
	struct regionStruct regionsArr[200];
	int regionsNum;
};

struct oneTile {
	undefined field_0x0[20];
	int regionId;
	undefined field_0x18[28];
};
struct UNICODE_STRING {
	USHORT something;//idk
	USHORT Length;//idk
	USHORT something2;//idk
	PWSTR Buffer;//y
};
struct siegeS {
	undefined field_0x0[12];
	struct settlementStruct* goal; /* settlement or fort */
	struct stackStruct* army;
};
//diplomacy data of faction to another faction
struct factionDiplomacy {
	undefined field_0x0[16];
	int state; /* diplomatic state(war, alliance, peace)(600/0/200) */
	int trade; /* trade rights(0 or 1) */
	int protectorate; /* protectorate or not(15 or 6) */
	undefined field_0x1c[108];
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

struct campaign {
	undefined field_0x0[4];
	undefined field_0x4[52];
	int playerFactionId;
	undefined field_0x3c[4];
	int hotSeatPlayer2FactionId;
	undefined field_0x44[232];
	int humanPlayers;
	int factionIdPow; /* pow(2,factionId) */
	int campaignDifficultyFaction[8];
	undefined field_0x154[120];
	int battleDifficulty07;
	undefined field_0x1d0[88];
	int battleDifficulty30;
	struct factionStruct* factionsSortedByDescrStrat[31];
	struct factionStruct* factionsSortedByID[31];
	int numberOfFactionsWithSlave;
	undefined field_0x328[28];
	struct factionStruct* currentFactionTurn;
	int field_0x348; /* FactionTurn? */
	UNICODE_STRING** currentDescrFile; /* path to descr_battle.txt or to descr_strat.txt */
	undefined field_0x350[28];
	int field_0x36c; /* TickCount? */
	int MillisecondCount;
	float SecondCount;
	int TurnNumber;
	int field_0x37c; /* PauseForBattleDialog? */
	float TimeScale;
	undefined field_0x384[4];
	struct settlementStruct* rome;
	undefined field_0x38c[4];
	struct settlementStruct* constantinople;
	undefined field_0x394[60];
	int8_t field_0x3d0;
	int8_t field_0x3d1;
	undefined field_0x3d2[26];
	float BrigandSpawnValue;
	float PirateSpawnValue;
	undefined field_0x3f4[4];
	int FreeUpkeepForts;
	int ActiveFactions;
	undefined field_0x400[12];
	int lastrandomseed;
	undefined field_0x410[744];
	int8_t isAdminPasswordExist;
	int8_t isHotseatLogon;
	undefined field_0x6fa[2];
	int8_t hotseatConsole;
	int8_t hotseatConsole2;
	undefined field_0x6fe[14];
	float currentDate;
	int season; /* season(0-summer,1-winter) */
	float startDate;
	int startSeason;
	float endDate;
	int endSeason;
	undefined field_0x724[4];
	float timeAtStartBattle;
	int daysInBattle;
	float currentTimeInBattle; /* 24 max, so calc as daysInBattle*24+currentTimeInBattle */
	undefined field_0x734[4128];
	struct factionDiplomacy dipArray[31][31];
};
struct gameDataAllStruct {
	undefined field_0x0[40];
	struct campaign* campaignData;
	undefined field_0x2c[8];
	struct tilesS* stratMap;
	struct campaign* field_0x38;
	undefined field_0x3c[28];
	struct battleDataS* battleHandler;
	undefined field_0x5c[164];
};
struct armyAndCharacter { /* in battle leader and leader army */
	struct stackStruct* army;
	struct general* character;
};
struct deploymentAreaS {
	undefined field_0x0[4];
	float* coordsPairs;
	undefined field_0x8[4];
	int coordsNum;
};
struct trackedPointerArmy {
	undefined field_0x0[4];
	struct stackStruct* stack;
	undefined field_0x8[44];
	struct deploymentAreaS* deploymentArea;
	undefined field_0x38[40];
};


struct battleSide {
	bool isDefender;
	uchar field_0x1;
	bool isCanDeploy;
	undefined field_0x3[8];
	undefined field_0xb;
	DWORD winConditions[4];
	undefined field_0x1c[20];
	struct armyAndCharacter* forces;
	undefined field_0x34[4];
	int armiesNum;
	undefined field_0x3c[28];
	struct trackedPointerArmy armies[8];
	undefined field_0x358[5588];
};

struct battleDataS {
	undefined field_0x0[4];
	int battleState; /* 0-not in battle,5-active battle,9-results screen,etc */
	undefined field_0xc[16];
	int xCoord;
	int yCoord;
	undefined somethingRelatedToBattleType[16];
	int attackerXCoord;
	int attackerYCoord;
	int defenderXCoord;
	int defenderYCoord;
	undefined field_0x40[92];
	struct battleSide sides[8];
	undefined field_0xc9fc[124];
	int sidesNum;
};


struct stratPortModel {
	struct model_Rigid* model_rigid;
	undefined field_0x4[4];
	char* modelPath;
	int modelPathCrypt;
};


struct portDockStrat {
	undefined field_0x0[12];
	int x;
	int y;
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

struct genMod {
	int type; /* 0-spy, 2 - diplomat, etc */
	int stratInfo;
	undefined field_0x8[8];
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
/* traiding resource on stratmap */
struct resStrat { /* traiding resource on stratmap */
	undefined field_0x0[12];
	int xCoord;
	int yCoord;
	undefined field_0x14[16];
	struct stratResMod* stratMod;
	struct settlementStruct* settlement;
	undefined field_0x2c[20];
};
//settlement model struct
struct settMod {
	struct model_Rigid* town;
	struct model_Rigid* castle;
	undefined field_0x8[924];
};


//models on the stratmap(settlements, resources, etc)
struct stratMod {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
};
struct portBuildingStruct {
	undefined field_0x0[8];
	struct general* character; /* charactr on port(enemy or not, army or not) */
	int x;
	int y;
	undefined field_0x14[28];
	struct settlementStruct* settlement;
	undefined field_0x34[20];
	struct stackStruct* occupantsArmy; /* if port block by army then it here */
	undefined field_0x4c[24];
	struct factionStruct* fac;
	undefined field_0x68[12];
	struct stratPortModel* portStratModel;
	struct portDockStrat* portDock;
	undefined field_0x7c[4];
};
//building draw info(pics, etc)
struct buildingDrawInfo { /* name, tga`s, models, etc) */
	char* name;
	undefined field_0x4[616];
};
//siege engine
struct siegeEngine {
	undefined field_0x0[172];
	int type; /* 2-RAM,4-ladder,1-tower */
};
//building data
struct building_data { /* SETTLEMENT_CONDITION_SETTLEMENT_FACTION */
	undefined field_0x0[98];
	char isDestroyAllowed;
	undefined field_0x63[29];
	char* type; /* type of building(core_building,barracks)  */
	int typeHash;
	struct buildingDrawInfo* drawInfo; /* name of building(stone_wall), tga`s, etc */
};
//building
struct building { /* building structure */
	undefined field_0x0[56];
	struct building_data* bData; /* many info(type,name, etc). exportDescrBuildingEntry */
	int8_t level;
	undefined field_0x3d[27];
	int hp; /* health points of the building */
	struct settlementStruct* settlement;
};

struct buildingInQueue { /* building in settlement queue */
	int something;
	struct building_data* buildingData;
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
//settlement
struct settlementStruct {
	undefined field_0x0[4];
	struct general* gubernator; /* ruler of the city */
	undefined field_0x8[4];
	UINT32 xCoord; /* x coordinate */
	UINT32 yCoord; /* y coordinate */
	undefined field_0x14[48];
	struct stackStruct* army; /* army on the settlement */
	undefined field_0x48[4];
	struct siegeS* siege;
	undefined field_0x50[252];
	struct settMod* model;
	int descr_culturestxt;
	undefined field_0x154[16];
	char* name; /* name  of the province */
	int nameCrypt;
	UNICODE_STRING** localizedName;
	struct factionStruct* ownerFac; /* faction of the owner */
	undefined field_0x174[36];
	int level; /* level of the settlement/castle */
	int fac_creatorModNum;
	undefined field_0x1a0[4];
	BYTE isCastle; /* castle or settlement */
	undefined field_0x1a5[3];
	UINT32 regionNumber; /* number of region */
	undefined field_0x1ac[644];
	undefined field_0x430[4];
	struct buildingsQueue buildingsQueueArray;
	undefined field_0x5ac[520];
	struct building* buildings[128];
	int buildingsNum; /* number of the buildings in the settlement */
	undefined field_0x9b8[1100];
	uchar isProvokedRebellion;
	uchar field_0xe05[3];
	undefined field_0xe08[60];
	struct resStrat** resources; // 0xE44
	undefined field_0xe48[4];
	int resourcesNum; // 0x0E4C
	undefined field_0xe50[2164];
	intptr_t guildStandings; // 0x16C4
	undefined field_0x16c8[404]; // 0x16C8
	int populationSize; // 0x185C
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
	undefined field_0x0[4];
	struct settlementStruct* settlement;
	undefined field_0x8[4];
	int xCoord; /* number of x-coord of unit fosition */
	int yCoord; /* number of y-coord of unit fosition */
	undefined field_0x14[108];
	struct generalCharacterictics* genChar; /* many important info about character */
	undefined field_0x84[4];
	struct genMod* genType;
	undefined field_0x8c[24];
	uchar ifMarkedToKill;
	undefined field_0xa5[35];
	float movepoints1;
	undefined field_0xcc[52];
	struct stackStruct* armyLeaded; /* army of the general */
	undefined field_0x104[4];
	struct unit* bodyguards; /* unit of general */
	struct stackStruct* armyNotLeaded; /* army, if not leader */
	undefined field_0x110[208];
	undefined field_0x1e0[4];
	float movepointsModifier;
	float movepointsMax;
	float movepoints2;
	undefined field_0x1f0[64];
	char* ability; /* custom ability */
};

//additional character data(name,label,traits, etc)
struct generalCharacterictics { /* many important info about character */
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
	int authority; /* authority of the general */
	int influence;
	int subterfuge;
	int loyality; /* loyality of the general */
	int charm;
	int finance;
	int piety; /* piety of the general */
	int magic;
	int nobility; /* nobility of the general */
	int leaderAutority; /* autority of the leader */
	int level;
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
	struct anchData** anchillaries; /* pointers to character anchillaries, names at  [item number] -0-0c-here) */
	undefined field_0x1f0[4];
	UINT32 anchNum; /* number of character  anchillaries */
	struct general* gen; /* on stratmap */
	undefined field_0x1fc[8];
	float yearOfBirth; /* yearOfBirth */
	undefined field_0x208[16];
	struct factionStruct* faction;
	int subFaction;
	undefined field_0x220[4];
	struct generalCharacterictics* parent; /* father */
	struct generalCharacterictics* spouse;
	struct generalCharacterictics* childs[4]; /* childs, womens wont have childs */
	undefined field_0x23c[20];
	char* portrait;
	undefined field_0x254[4];
	char* portrait2;
	undefined field_0x25c[8];
	char* portrait_custom; /* custom portrait name */
	undefined field_0x268[4];
	int age;
};
//anchillary of character
struct anchillary { /* structure of anchillary */
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
	undefined field_0x0[24];
	float xCoord;
	float zCoord;
	float yCoord;
	undefined field_0x24[1440];
};

//unit data
struct unit {
	undefined field_0x0[4];
	struct trackedPointerUnit** trackedUnitPointerP;
	undefined field_0x8[636];
	struct stackStruct* army;
	undefined field_0x288[12];
	struct EduEntry* eduEntry;
	undefined field_0x298[616];
	int expScreen; /* screen expierence */
	struct general* general;
	float movePoints;
	int number; /* number of soldiers */
	int numberTact; /* number of soldiers on tactical map */
	int numberMax; /* max number of soldiers */
	undefined field_0x518[216];
	UINT32 stats; /* def/atk/etc */
	struct soldierInBattle** soldiersBattleArr; /* array of soldiers battle data */
	struct soldierData* soldiersArr; /* array of soldiers data */
	undefined field_0x5fc[6780];
	UNICODE_STRING** alias; /* Legio string etc */
	undefined field_0x207c[44];
	struct siegeEngine** siegeEngine;
	undefined field_0x20ac[4];
	int siegeEnNum; /* number of siege engines */
	undefined field_0x20b4[36];
	struct engineRecord* engineRec;
	undefined field_0x20dc[4];
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
struct factionStruct {
	undefined field_0x0[180];
	int dipNum; /* number of faction in diplomatic array */
	undefined field_0xb8[4];
	char* ai_label; /* ai_label of faction */
	undefined field_0xc0[4];
	struct settlementStruct* capital; /* capital of the faction */
	struct generalCharacterictics* leader; /* faction leader */
	struct generalCharacterictics* heir; /* faction heir */
	struct factionStratMapDescrS* factSmDescr;
	int isPlayerControlled; /* is faction a controlled by player */
	UINT32* ai_assistant;
	undefined field_0xdc[20];
	UINT32 holdRegions;
	undefined field_0xf4[4];
	struct generalCharacterictics** charactersAll; /* all characrters, died, alive, etc */
	undefined field_0xfc[4];
	int numOfCharactersAll; /* all characrters, died, alive, etc */
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
	struct watchTowerStruct** watchTowers;
	undefined field_0x144[4];
	uint wathtowersNum;
	struct portBuildingStruct** portBuildings; /* port buildings */
	undefined field_0x150[4];
	int portBuildingsNum;
	undefined field_0x158[68];
	int someForSpawnCharacter;
	undefined field_0x1a0[2208];
	UINT32 religion; /* number of religion */
	undefined field_0xa44[12];
	undefined field_0xa50[156];
	int money; /* money of the faction */
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
//tupe of unit from edu
struct EduEntry {
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
	int8_t HasPrimaryWeapon;
	int8_t nothing3;
	int8_t AttackValueBitshiftedleftby18;
	int8_t ChargeBonusBitshiftedleftby24;
	DWORD Ammunition;
	DWORD MissleRange;
	float MissleRangeSquared;
	DWORD Unknown4;
	void* StatPriMissle;
	DWORD WeaponType;
	DWORD TechType;
	DWORD DamageType;
	DWORD SoundType;
	DWORD AttackMinDelay;
	char pad_0154[4];
	char WeaponShootEffect[20];
	char pad_016C[4];
	int8_t HasSecondaryWeapon;
	int8_t nothing4;
	int8_t SecAttackValueBitshiftedleftby18;
	int8_t SecChargeBonusBitshiftedleftby24;
	DWORD SecAmmunition;
	DWORD SecMissleRange;
	float SecMissleRangeSquared;
	DWORD Unknown2;
	void* StatSecMissle;
	DWORD SecWeaponType;
	DWORD SecTechType;
	DWORD SecDamageType;
	DWORD SecSoundType;
	DWORD SecAttackMinDelay;
	char pad_019C[28];
	DWORD PriDefenseSkillAndShield;
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
	void* ArmorUpgrade;
	void* ArmorUpgrad1;
	void* ArmorUpgrade2;
	char pad_0210[4];
	void* ArmorUpgrade3;
	void* ArmorUpgrade4;
	void* ArmorUpgrade5;
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
	void* Attributes;
	void* EndOfAttributes;
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
	struct EduEntry unitTupes[500];
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
//pointer to anchillary
struct anchData { /* data of the anchillarie */
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
	undefined field_0xe4[4];
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


#pragma pack(pop)