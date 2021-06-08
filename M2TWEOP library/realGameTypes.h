#pragma once
#include <cstdint>
#include <windows.h>
#include <basetsd.h>
typedef unsigned char   undefined;
typedef unsigned int    uint;
typedef unsigned char    uchar;
struct gameDataAllStruct {
	undefined field_0x0[88];
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
	undefined field_0x0[24];
	int xCoord;
	int yCoord;
	undefined field_0x20[16];
	int attackerXCoord;
	int attackerYCoord;
	int defenderXCoord;
	int defenderYCoord;
	undefined field_0x40[92];
	struct battleSide sides[8];
	undefined field_0xc9fc[124];
	int sidesNum;
};

#pragma pack(push,1) 
struct UNICODE_STRING {
	USHORT something;//idk
	USHORT Length;//idk
	USHORT something2;//idk
	PWSTR Buffer;//y
};
#pragma pack(pop)
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
	struct building_data* bData; /* many info(type,name, etc) */
	int level;
	undefined field_0x40[24];
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
	undefined field_0x48[260];
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
	int buldingsNum; /* number of the buildings in the settlement */
	undefined field_0x9b8[1100];
	uchar isProvokedRebellion;
	uchar field_0xe05[3];
	undefined field_0xe08[60];
	struct resStrat** resources;
	undefined field_0xe48[4];
	int resourcesNum;
	undefined field_0xe50[2572];
	int populationSize;
};

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
	undefined field_0x48[8];
	int loyality; /* loyality of the general */
	undefined field_0x54[8];
	int piety; /* piety of the general */
	undefined field_0x60[4];
	int nobility; /* nobility of the general */
	int leaderAutority; /* autority of the leader */
	undefined field_0x6c[376];
	struct trait** traits; /* names at [item number] -0x4-here. Not have number, read it in while(traits != 0){this->m_memory->Read(traits + 0x08, 4).toINT32();} */
	undefined field_0x1e8[4];
	struct anchData** anchillaries; /* pointers to character anchillaries, names at  [item number] -0-0c-here) */
	undefined field_0x1f0[4];
	UINT32 anchNum; /* number of character  anchillaries */
	struct general* gen; /* on stratmap */
	undefined field_0x1fc[8];
	int yearOfBirth; /* yearOfBirth */
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
//trait of character
struct trait { /* traits of the character */
	struct sometNameStruct* nameS;
	UINT32* level; /* level of trait */
	struct trait** nextTrait; /* pointer or 0 or 1 */
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
};
//string
struct sometNameStruct { /* char* at 0x4 */
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	char* name;
};
struct trackedPointerUnit {
	undefined field_0x0[4];
	struct unit* unit;
	undefined field_0x8[88];
};
//unit data
struct unit {
	undefined field_0x0[4];
	struct trackedPointerUnit** trackedUnitPointerP;
	undefined field_0x8[652];
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
	undefined field_0x5f4[4];
	struct soldierData* soldiersArr; /* array of soldiers data */
	undefined field_0x5fc[6780];
	UNICODE_STRING** alias; /* Legio string etc */
	undefined field_0x207c[44];
	struct siegeEngine** siegeEngine;
	undefined field_0x20ac[4];
	int siegeEnNum; /* number of siege engines */
};
//army data
struct stackStruct { /* structure of stack */
	undefined field_0x0[76];
	struct factionStruct* faction;
	undefined field_0x50[4];
	struct unit** units;
	undefined field_0x58[4];
	int numOfUnits;
	undefined field_0x60[56];
	struct stackStruct* boardedArmy; /* if fleet, here army */
	undefined field_0x9c[20];
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
//faction data
struct factionStruct {
	undefined field_0x0[180];
	int dipNum; /* number of faction in diplomatic array */
	undefined field_0xb8[4];
	char* ai_label; /* ai_label of faction */
	undefined field_0xc0[4];
	struct settlementStruct* capital; /* capital of the faction */
	struct generalCharacterictics* leader; /* faction leader */
	struct generalCharacterictics* heir; /* faction heir */
	struct someFactNameStruct* factName;
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
	undefined field_0x11c[8];
	int someNum;
	struct settlementStruct** settlements;
	undefined field_0x12c[4];
	int settlementsNum;
	struct fortStruct** forts;
	undefined field_0x138[4];
	int fortsNum;
	int field_0x140;
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

//tupe of unit from edu
struct EduEntry
{
	char* Type; //0x0000
	int32_t Dictionary; //0x0004
	int32_t Index; //0x0008
	int32_t UnitCreatedCounter; //0x000C
	char* UnitCardTga; //0x0010
	char pad_0014[4]; //0x0014
	char* InfoCardTga; //0x0018
	char pad_001C[4]; //0x001C
	int32_t nothing; //0x0020
	int32_t nothing2; //0x0024
	char* InfoPicDir; //0x0028
	int32_t DifferentForShip; //0x002C
	class Description* Description; //0x0030
	void* DescriptionShort; //0x0034
	void* DescriptionName; //0x0038
	int32_t Category; //0x003C
	int32_t Class; //0x0040
	int32_t Unknown; //0x0044
	int32_t VoiceType; //0x0048
	char* Accent; //0x004C
	char pad_0050[4]; //0x0050
	char* BannerFaction; //0x0054
	char pad_0058[4]; //0x0058
	char* BannerUnit; //0x005C
	char pad_0060[4]; //0x0060
	char* BannerHoly; //0x0064
	char pad_0068[4]; //0x0068
	char* BannerMarine; //0x006C
	char pad_0070[4]; //0x0070
	char* BannerSecondary; //0x0074
	char pad_0078[12]; //0x0078
	int32_t Trained; //0x0084
	int32_t StatMentalDicipline; //0x0088
	int8_t MoraleLocked; //0x008C
	int8_t Morale; //0x008D
	int8_t StatHealth; //0x008E
	int8_t StatHealthAnimal; //0x008F
	int8_t StatHeat; //0x0090
	int8_t StatGround1; //0x0091
	int8_t StatGround2; //0x0092
	int8_t StatGround3; //0x0093
	int8_t StatGround4; //0x0094
	char pad_0095[1]; //0x0095
	int16_t StatFood1; //0x0096
	int16_t StatFood2; //0x0098
	int16_t StatFireDelay; //0x009A
	int32_t StatStl; //0x009C
	float ChargeDistance; //0x00A0
	int32_t mountEffectClass; //0x00A4
	char pad_00A8[4]; //0x00A8
	int32_t mountEffectAmount2; //0x00AC
	int32_t mountEffectClass2; //0x00B0
	char pad_00B4[4]; //0x00B4
	int32_t mountEffectAmount3; //0x00B8
	int32_t mountEffectClass3; //0x00BC
	char pad_00C0[4]; //0x00C0
	int32_t mountEffectAmount4; //0x00C4
	int32_t MountEffectCount; //0x00C8
	int16_t StatCost1; //0x00CC
	int16_t StatCost2; //0x00CE
	int16_t StatCost3; //0x00D0
	int16_t StatCost4; //0x00D2
	int16_t StatCost5; //0x00D4
	int16_t StatCost6; //0x00D6
	int16_t StatCost7; //0x00D8
	int16_t StatCost8; //0x00DA
	int32_t CrusadingUpkeepModifier; //0x00DC
	int32_t RecruitPriorityOffsetTimes4; //0x00E0
	int8_t Formation; //0x00E4
	int8_t Formation2; //0x00E5
	int8_t Formation3; //0x00E6
	int8_t Formation4; //0x00E7
	int8_t Formation5; //0x00E8
	int8_t Formation6; //0x00E9
	char pad_00EA[2]; //0x00EA
	int8_t N000000AA; //0x00EC
	int8_t N00000218; //0x00ED
	char pad_00EE[2]; //0x00EE
	int32_t DefaultNubmerOfRanks; //0x00F0
	float UnitSpacingFrontToBackClose; //0x00F4
	float UnitSpacingSideToSideClose; //0x00F8
	float UnitSpacingFrontToBackLoose; //0x00FC
	float UnitSpacingSideToSideLoose; //0x0100
	char* Soldier; //0x0104
	char pad_0108[12]; //0x0108
	class ModelDbEntry* ModelDBEntry; //0x0114
	int32_t SoldierCount; //0x0118
	float Mass; //0x011C
	float Width; //0x0120
	float Height; //0x0124
	int8_t HasPrimaryWeapon; //0x0128
	int8_t nothing3; //0x0129
	int8_t AttackValueBitshiftedleftby18; //0x012A
	int8_t ChargeBonusBitshiftedleftby24; //0x012B
	int32_t Ammunition; //0x012C
	int32_t MissleRange; //0x0130
	float MissleRangeSquared; //0x0134
	int32_t Unknown4; //0x0138
	class Projectile* StatPriMissle; //0x013C
	int32_t WeaponType; //0x0140
	int32_t TechType; //0x0144
	int32_t DamageType; //0x0148
	int32_t SoundType; //0x014C
	int32_t AttackMinDelay; //0x0150
	char pad_0154[4]; //0x0154
	char WeaponShootEffect[20]; //0x0158
	char pad_016C[4]; //0x016C
	int8_t HasSecondaryWeapon; //0x0170
	int8_t nothing4; //0x0171
	int8_t SecAttackValueBitshiftedleftby18; //0x0172
	int8_t SecChargeBonusBitshiftedleftby24; //0x0173
	int32_t SecAmmunition; //0x0174
	int32_t SecMissleRange; //0x0178
	float SecMissleRangeSquared; //0x017C
	int32_t Unknown2; //0x0180
	void* StatSecMissle; //0x0184
	int32_t SecWeaponType; //0x0188
	int32_t SecTechType; //0x018C
	int32_t SecDamageType; //0x0190
	int32_t SecSoundType; //0x0194
	int32_t SecAttackMinDelay; //0x0198
	char pad_019C[28]; //0x019C
	int32_t PriDefenseSkillAndShield; //0x01B8
	char pad_01BC[4]; //0x01BC
	char* FirstOfficier; //0x01C0
	void* N000000E0; //0x01C4
	char pad_01C8[8]; //0x01C8
	void* N000000E3; //0x01D0
	char* SecondOfficier; //0x01D4
	void* N000000E5; //0x01D8
	char pad_01DC[8]; //0x01DC
	void* N000000E8; //0x01E4
	char* ThirdOfficier; //0x01E8
	void* N000000EA; //0x01EC
	char pad_01F0[8]; //0x01F0
	void* N000000ED; //0x01F8
	int32_t OfficierCount; //0x01FC
	char pad_0200[4]; //0x0200
	void* ArmorUpgrade; //0x0204
	void* ArmorUpgrad1; //0x0208
	void* ArmorUpgrade2; //0x020C
	char pad_0210[4]; //0x0210
	void* ArmorUpgrade3; //0x0214
	void* ArmorUpgrade4; //0x0218
	void* ArmorUpgrade5; //0x021C
	char pad_0220[4]; //0x0220
	class Engine* N000000F8; //0x0224
	char pad_0228[4]; //0x0228
	int8_t N000000FA; //0x022C
	int8_t N000003B1; //0x022D
	int8_t EngineMissleAttack; //0x022E
	int8_t EngineMissleChargeBonus; //0x022F
	int32_t EngineMissleAmmunition; //0x0230
	int32_t EngineMissleRange; //0x0234
	float EngineMissleRangeSquared; //0x0238
	int32_t Unknown3; //0x023C
	class Projectile* EngineMissle; //0x0240
	int32_t EngineMissleWeaponType; //0x0244
	int32_t EngineMissleTechType; //0x0248
	int32_t EngineMissleDamageType; //0x024C
	int32_t EngineMissleSoundType; //0x0250
	int32_t EngineMissleAttackMinDelay; //0x0254
	char pad_0258[48]; //0x0258
	class Projectile* N00000111; //0x0288
	int32_t N00000112; //0x028C
	int32_t N00000113; //0x0290
	int32_t N00000114; //0x0294
	int32_t N00000115; //0x0298
	int32_t TerMissleAttackMinDelay; //0x029C
	char pad_02A0[36]; //0x02A0
	void* MountModel; //0x02C4
	void* MountSomerthing; //0x02C8
	int32_t StatSecAndSecArmour; //0x02CC
	char pad_02D0[68]; //0x02D0
	int32_t Mount; //0x0314
	char pad_0318[8]; //0x0318
	void* AnimalModel; //0x0320
	void* AnimalSomething; //0x0324
	char pad_0328[84]; //0x0328
	class Ship* Ship; //0x037C
	char pad_0380[8]; //0x0380
	void* N00000151; //0x0388
	void* N00000152; //0x038C
	void* N00000153; //0x0390
	char pad_0394[12]; //0x0394
	class Attributes* Attributes; //0x03A0
	void* EndOfAttributes; //0x03A4
	void* BytesBeforeNextAttributes; //0x03A8
	uint8_t Attributes2; //0x03AC
	uint8_t Attributes3; //0x03AD
	uint8_t Attributes4; //0x03AE
	uint8_t Attributes5; //0x03AF
	uint32_t Attributes6; //0x03B0
	float MoveSpeedMod; //0x03B4
	int32_t AttributeAreaEffect; //0x03B8
	char pad_03BC[28]; //0x03BC
	float PriSkeletonCompFactor; //0x03D8
	float SecSkeletonCompFactor; //0x03DC
	float TerSkeletonCompFactor; //0x03E0
}; //Size: 0x03E4
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
//diplomacy data of faction to another faction
struct factionDiplomacy {
	undefined field_0x0[12];
	int state; /* diplomatic state(war, alliance, peace) */
	int trade; /* trade rights(0 or 1) */
	int protectorate; /* protectorate or not(15 or 6) */
	undefined field_0x18[112];
};
//fort
struct fortStruct {
	undefined field_0x0[4];
	struct general* gubernator;
	undefined field_0x8[4];
	UINT32 xCoord;
	UINT32 yCoord;
	undefined field_0x14[48];
	struct stackStruct* army;
	undefined field_0x48[152];
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
	int null4;
};
struct tradingResources {
	struct traidingResource resources[27];
	char* mine;
	int mineCrypt;
};



struct console_command { /* structure of console command */
	void** function;
	char* name;
	char* description;
	int type;
	int idk;
};

