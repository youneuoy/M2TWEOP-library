#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>

struct statPri;
struct modelDbEntry;

template<unsigned int IIdx, typename TRet, typename ... TArgs>
TRet callVFunc(void* thisptr, TArgs ... argList)
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

struct basicStringGame
{
	char pad[0x1C];
public:
	std::string getString()
	{
		return *reinterpret_cast<std::string*>(this);
	}
	void setString(const std::string& str)
	{
		*reinterpret_cast<std::string*>(this) = str;
	}
};

template <typename T>
struct gameStdVector
{
public:
	DWORD field0; //0x0000
	T* elements; //0x0004
	T* end; //0x0008
	DWORD reserved; //0x000C
public:
	int size()
	{
		return end - elements;
	}
	bool empty()
	{
		return size() == 0;
	}
	T& operator[](int index)
	{
		return elements[index];
	}
	bool contains(T* thing)
	{
		int num = size();
		for (int i = 0; i < num; i++)
		{
			if (elements[i] == thing)
				return true;
		}
		return false;
	}
};

template<typename T>
struct gameList
{
	T* elements;
	gameList<T>* next;
	gameList<T>* prev;
	int32_t currentSize;
	int32_t currentCount;
	int size()
	{
		int total = 0;
		gameList<T>* current = this;
		while (current)
		{
			total += current->currentCount;
			current = current->next;
		}
		return total;
	}
	bool empty()
	{
		return currentCount == 0;
	}
	T* get(int index)
	{
		gameList<T>* current = this;
		while (current && index >= current->currentCount)
		{
			index -= current->currentCount;
			current = current->next;
		}
		if (current)
			return &current->elements[index];
		return nullptr;
	}
};

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
	const char* name; //0x0000
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

struct smObject
{
	DWORD* vftable;
	smObject* next;
	smObject* previous;
	int xCoord;
	int yCoord;
	int fadeMode;
	bool render;
	char pad0[3];
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

struct cultureModels
{
public:
	char pad_0000[5584]; //0x0000
}; //Size: 0x15D0

struct battleTile
{
	void *object;
	uint16_t area : 4;
	uint16_t physicalGroundType : 4;
	uint16_t secondaryGroundType : 4;
	uint16_t vegetation : 1;
	uint16_t bridge : 1;
	uint16_t height;
	uint16_t waterHeight;
	__int16 fieldA;
	__int16 fieldC;
	uint16_t smthing : 12;
	uint16_t smthing2 : 4;
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
	int allianceSoldiers[8];
	int controllingFaction;
	int ownerFaction;
	UNICODE_STRING ***localizedName;
	bool isRound;
	bool halfwayCaptured;
	int8_t pad2e[2];
	int settType;
	float plazaXCoord;
	float plazaYCoord;
	float radius;
	float radius2;
	float heightX;
	float heightY;
	float widthX;
	float widthY;
	float sizeX;
	float sizeY;
	int flagIndex;
	int minAllies;
	int maxEnemies;
	bool capturing;
	int8_t pad69[3];
	int alliancePlaza;
	int tickCaptured;
	float plazaMaxTime;
	float plazaControlPerSecond;
	float plazaTimer;
	bool isCaptured;
	int8_t pad81[3];
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


struct statPri
{
	unsigned __int32 isValid : 1;
	unsigned __int32 isMissile : 1;
	unsigned __int32 isSpear : 1;
	unsigned __int32 isLightSpear : 1;
	unsigned __int32 isPrec : 1;
	unsigned __int32 isAP : 1;
	unsigned __int32 isBP : 1;
	unsigned __int32 isArea : 1;
	unsigned __int32 isFire : 1;
	unsigned __int32 isBodyLaunching : 1;
	unsigned __int32 isThrown : 1;
	unsigned __int32 isShortPike : 1;
	unsigned __int32 isLongPike : 1;
	unsigned __int32 isSpearBonus12 : 1;
	unsigned __int32 isSpearBonus10 : 1;
	unsigned __int32 isSpearBonus8 : 1;
	unsigned __int32 isSpearBonus6 : 1;
	unsigned __int32 isSpearBonus4 : 1;
	unsigned __int32 attack : 6;
	unsigned __int32 charge : 6;
	unsigned __int32 ammo : 7;
	int32_t missileRange; //0x0008
	float missileRangeSquared; //0x000C
	int32_t missileClose; //0x0010
	struct projectile *missile; //0x0014
	int32_t weaponType; //0x05AC
	int32_t weaponTecType; //0x05B0
	int32_t damageType; //0x0020
	int32_t soundType; //0x0024
	int32_t minAttackDelayPri; //0x05BC
	struct basicStringGame weaponFX; //0x05C0
public:
	bool getIsValid()
	{
		return isValid;
	}
	bool getIsMissile()
	{
		return isMissile;
	}
	bool getIsSpear()
	{
		return isSpear;
	}
	bool getIsLightSpear()
	{
		return isLightSpear;
	}
	bool getIsPrec()
	{
		return isPrec;
	}
	bool getIsAP()
	{
		return isAP;
	}
	bool getIsBP()
	{
		return isBP;
	}
	bool getIsArea()
	{
		return isArea;
	}
	bool getIsFire()
	{
		return isFire;
	}
	bool getIsBodyLaunching()
	{
		return isBodyLaunching;
	}
	bool getIsThrown()
	{
		return isThrown;
	}
	bool getIsShortPike()
	{
		return isShortPike;
	}
	bool getIsLongPike()
	{
		return isLongPike;
	}
	bool getIsSpearBonus12()
	{
		return isSpearBonus12;
	}
	bool getIsSpearBonus10()
	{
		return isSpearBonus10;
	}
	bool getIsSpearBonus8()
	{
		return isSpearBonus8;
	}
	bool getIsSpearBonus6()
	{
		return isSpearBonus6;
	}
	bool getIsSpearBonus4()
	{
		return isSpearBonus4;
	}
	int getAttack()
	{
		return attack;
	}
	int getCharge()
	{
		return charge;
	}
	int getAmmo()
	{
		return ammo;
	}
	void setIsValid(bool newIsValid)
	{
		isValid = newIsValid;
	}
	void setIsMissile(bool newIsMissile)
	{
		isMissile = newIsMissile;
	}
	void setIsSpear(bool newIsSpear)
	{
		isSpear = newIsSpear;
	}
	void setIsLightSpear(bool newIsLightSpear)
	{
		isLightSpear = newIsLightSpear;
	}
	void setIsPrec(bool newIsPrec)
	{
		isPrec = newIsPrec;
	}
	void setIsAP(bool newIsAP)
	{
		isAP = newIsAP;
	}
	void setIsBP(bool newIsBP)
	{
		isBP = newIsBP;
	}
	void setIsArea(bool newIsArea)
	{
		isArea = newIsArea;
	}
	void setIsFire(bool newIsFire)
	{
		isFire = newIsFire;
	}
	void setIsBodyLaunching(bool newIsBodyLaunching)
	{
		isBodyLaunching = newIsBodyLaunching;
	}
	void setIsThrown(bool newIsThrown)
	{
		isThrown = newIsThrown;
	}
	void setIsShortPike(bool newIsShortPike)
	{
		isShortPike = newIsShortPike;
	}
	void setIsLongPike(bool newIsLongPike)
	{
		isLongPike = newIsLongPike;
	}
	void setIsSpearBonus12(bool newIsSpearBonus12)
	{
		isSpearBonus12 = newIsSpearBonus12;
	}
	void setIsSpearBonus10(bool newIsSpearBonus10)
	{
		isSpearBonus10 = newIsSpearBonus10;
	}
	void setIsSpearBonus8(bool newIsSpearBonus8)
	{
		isSpearBonus8 = newIsSpearBonus8;
	}
	void setIsSpearBonus6(bool newIsSpearBonus6)
	{
		isSpearBonus6 = newIsSpearBonus6;
	}
	void setIsSpearBonus4(bool newIsSpearBonus4)
	{
		isSpearBonus4 = newIsSpearBonus4;
	}
	void setAttack(int newAttack)
	{
		attack = newAttack & 0x3F;
	}
	void setCharge(int newCharge)
	{
		charge = newCharge & 0x3F;
	}
	void setAmmo(int newAmmo)
	{
		ammo = newAmmo & 0x7F;
	}
};

struct statArmour
{
	unsigned __int32 isValid : 1;
	unsigned __int32 armour : 6;
	unsigned __int32 defense : 6;
	unsigned __int32 shield : 5;
	int32_t armourMaterial;
public:
	int getIsValid()
	{
		return isValid;
	}
	int getArmour()
	{
		return armour;
	}
	int getDefense()
	{
		return defense;
	}
	int getShield()
	{
		return shield;
	}
	void setIsValid(bool newIsValid)
	{
		isValid = newIsValid;
	}
	void setArmour(int newArmour)
	{
		armour = newArmour & 0x3F;
	}
	void setDefense(int newDefense)
	{
		defense = newDefense & 0x3F;
	}
	void setShield(int newShield)
	{
		shield = newShield & 0x1F;
	}
};

struct unitStats
{
	statPri priStats;
	statArmour armourStats;
	int8_t attackInBattle;
	int8_t armourInBattle;
	int8_t formationDefBonus;
	int8_t formationAttBonus;
	unsigned __int8 chargeBonus : 6;
	unsigned __int8 chargeDecay : 2;
	int8_t generalsBonus;
	int8_t byte_56[2];
public:
	int getChargeBonus()
	{
		return chargeBonus;
	}
	int getChargeDecay()
	{
		return chargeDecay;
	}
	void setChargeBonus(int newChargeBonus)
	{
		chargeBonus = newChargeBonus & 0x3F;
	}
	void setChargeDecay(int newChargeDecay)
	{
		chargeDecay = newChargeDecay & 0x3;
	}
};

struct towerStats
{
	statPri stats;
	basicStringGame shot_sfx;
	basicStringGame shot_gfx;
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
	float topLeftX;
	float topLeftY;
	float bottomRightX;
	float bottomRightY;
	void *outline;
	int type;
	int creatorId;
	int factionId;
	float xCoord;
	float zCoord;
	float yCoord;
	int16_t angle;
	int8_t damageLevel;
	char byte_33;
	int currentHealth;
	int tempHealth;
	int startHealth;
	struct factionStruct *faction;
	int alliance;
	struct battleResidence *battleResidence;
	int buildingIndex;
	struct towerEntry *descrWallsEntry;
	float fireDamage;
	bool isBurning;
	char pad_59[3];
	float fireLevel;
	bool breached;
	bool dockable;
	char pad_62[2];
	int fireSoundRegister;
	int fireSoundRegisterSP;
	void *fireAmbientRegister;
	void *fireAmbientRegisterSP;
	float float_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84_init1;
	int field_88;
	void *worldData;
	void* buildingDamage;
	int index;
	int perimeter;
	bool canBeBreached;
	char pad_9D[3];
	int field_A0;
	int towerType;
	towerStats towerStats;
	int upgradeLevel;
	unitStats unitStats;
	gameStdVector<void*> entities;
	bool groundZoned;
	bool manned;
	char pad_1CA[2];
	int ticks;
	struct unit* controllingUnit;
	void* slotData;
	int slotDataSize;
	int slotDataNum;
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
	gameList<void> spawnPositions; //0x0000
	struct buildingBattle** allBuildings; //0x0014
	int32_t allBuildingsSize; //0x0020
	int32_t allBuildingsNum; //0x0020
	int32_t* allBuildingsInt; //0x0020
	int allBuildingsIntSize;
	int allBuildingsIntNum;
	struct perimeterBuildings *perimeters;
	int perimetersSize;
	int perimetersNum;
	void *perimeterZones;
	int perimeterZonesSize;
	int perimeterZonesNum;
}; //Size: 0x0444

struct battleResidence
{
public:
	bool isUsed;
	int8_t pad1[3];
	float defendOffsetX;
	float defendOffsetZ;
	float defendOffsetY;
	int attackerDirection;
	float radius;
	void *outline;
	struct settlementStruct *settlement;
	struct fortBattleInfo *fort;
	struct armyStruct *army;
	struct factionStruct *faction;
	int32_t alliesBitfield;
	int32_t residenceType;
	struct battleBuildings *battleBuildings;
	int targetType;
	float targetWidth;
	float targetHeight;
	float targetRadius;
	float targetX;
	float targetZ;
	float targetY;
	int targetMinAllies;
	int targetMaxEnemy;
	float targetHoldTime;
	struct plazaStuff *plaza;
	void *worldData;
	int areaId;
	int8_t settlementWallsBreached;
	int8_t settlementGateDestroyed;
	char pad_006E[2];
	int alliance;
	gameStdVector<void*> perimeterStuff;
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
	int16_t angle;
	int16_t short_2;
	float xCoord;
	float yCoord;
	int perimeter;
	float projection;
	bool available;
	int8_t byte_15[3];
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
	gameStdVector<roadNode> streetDeploymentPositions;
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
	bool isOpen;
	bool isPrepared;
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
	DWORD* Vtable;
	void* nextObject;
	void* prevObject;
	UINT32 xCoord;
	UINT32 yCoord;
	int fade;
	bool render;
	char pad0[3];
	float opacity;
	bool highlighted;
	char pad1[3];
	struct watchTowerModel* model;
	int32_t regionID; //0x0028
	struct factionStruct* faction; //0x002C
	struct settlementStruct* settlement; //0x0030
	void* trackedPointerArmyVtable; //0x0034
	struct armyStruct* blockingArmy; //0x0038
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
	int8_t turns;
	int8_t inOpenList;
	char pad[2];
};

struct characterMovementExtents
{
	void * vtbl;
	int totalTiles;
	int searchType;
	int tileIndex;
	int xCoordMin;
	int yCoordMin;
	int xCoordMax;
	int yCoordMax;
	gameStdVector<movementExtentTile> movementExtentTiles;
	struct character *character;
	int turns;
	movementExtentTile* getTile(int xCoord, int yCoord)
	{
		if (xCoord < xCoordMin || xCoord > xCoordMax || yCoord < yCoordMin || yCoord > yCoordMax)
			return nullptr;
		return &movementExtentTiles[(yCoord - yCoordMin) * (xCoordMax - xCoordMin + 1) + (xCoord - xCoordMin)];
	}
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

struct trackedCharacter
{
public:
	void* vtbl; //0x0000
	struct character* character; //0x0004
}; //Size: 0x0008

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
	void* campaignData;
	void* battleData;
	void* uiData;
	struct stratMap* stratMap;
	struct stratMap* stratMap2;
	struct stratMap* stratMap3;
	struct stratMap** stratMapPtrPtr;
	struct campaign* campaignStruct;
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


struct battleArmy { /* in battle leader and leader army */
	struct armyStruct* army;//0x0000
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

struct deployData {
	deploymentAreaS area;
	void* deployUnits;
	int deployUnitsSize;
	int deployUnitsCount;
	battleResidence* residence;
	bool isInSettlement;
	int8_t byte_0x1D[3];
};

struct battleSideArmy {
	int trackedArmyVtbl;
	struct armyStruct* stack;//0x0004
	int alliance;
	bool isReinforcement;
	int8_t byte_0xD[3];
	int isAiControlled;
	bool canChangeReinforceMode;
	bool canWithdraw;
	int8_t byte_0x16[2];
	void* worldData;
	float worldDataX;
	float worldDataY;
	int worldDataRot;
	float reformX;
	float reformY;
	bool hasReformPoint;
	int8_t byte_0x31[3];
	gameList<deployData> deploymentAreas;
	void* lostSoldiers;
	int lostSoldiersSize;
	int lostSoldiersCount;
	void* fledSoldiers;
	int fledSoldiersSize;
	int fledSoldiersCount;
	deploymentAreaS* getMainDeployArea()
	{
		return &deploymentAreas.get(0)->area;
	}
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
	armyStruct *army;
	armyStruct *commandingArmy;
	int field_8;
};


struct battleSide {
	bool isDefender;//0x0000
	uchar canWithdraw;//0x0001
	bool canWinSiege;//0x0002
	char pad_0003[1]; //0x0003
	uint32_t factionBitMap; //0x0004
	int condOperator;
	DWORD winConditions[4];//0x000C
	int32_t winConditionCount; //0x001C
	bool conditionMet;
	char pad_cond[3];
	int32_t wonBattle; //0x0024 2 means won, 0 means lost, 1 is draw maybe
	int32_t battleSuccess; //0x0028 0 = close, 1 = average, 2 = clear, 3 = crushing
	float score;
	struct battleArmy* forces;//0x0030 
	int32_t armyArraySize; //0x0034 
	int armiesNum;//0x0038 
	int unitsStart;
	int unitsKilled;
	int unitsRouted;
	int32_t alliance; //0x0048
	struct battleAI* battleAIPlan; //0x004C
	bool canWait;
	bool canDeploy;
	bool canView;
	char pad_view;
	int deployStage;
	struct battleSideArmy armies[64];//0x0058 
	int32_t battleSideArmyNum; //0x1858
	struct factionStruct* factions[8]; //0x185C
	int32_t factionCount; //0x187C
	int blockedReinforcerNum;
	struct unit** visibleEnemyUnits; //0x1884
	int32_t visibleEnemyUnitsSize; //0x1888
	int32_t visibleEnemyUnitsNum; //0x188C
	struct armyStruct** visibleEnemyArmies; //0x1890
	int32_t visibleEnemyArmiesSize; //0x1894
	int32_t visibleEnemyArmiesNum; //0x1898
	reinforcingArmy reinforcementArmies[8];
	int reinforceArmyCount;
	int balanceOfPower;
	int32_t activeArmyStrength;
	int32_t armyStrengthStratMap;
	int armyStrengthBattleMap;
	int32_t totalStrength;
	int32_t previousArmyStrength;
	int32_t soldierCount;
	float battleOdds;
	float lastBattleOdds;
	float startingAllianceScore;
	float allianceScore;
};

struct playerArmy
{
public:
	int8_t inBattle;
	int8_t completeDeployment;
	int8_t wantsToLeave;
	int8_t field3;
	uint32_t factionId;
	float speedSet;
	bool hasPaused;
	char pad[3];
	int battleId;
	armyStruct *army;
}; //Size: 0x0018

struct autoResolveData
{
	int capturePercent;
	int autoResolveBattleType;
	int duration;
	int soldierRatio;
	int autoResolveArmies;
	int autoResolveArmiesSize;
	int autoResolveArmiesNum;
	int mainAttackerIndex;
	int attackerNum;
	int mainDefenderIndex;
	int defenderNum;
	struct armyStruct **reinforcers;
	int reinforcersSize;
	int reinforcersNum;
	int fights;
	int fightsSize;
	int fightsNum;
	int wallEquipmentAndInfantry;
	int gatesForced;
	bool isAutoWin;
	bool attackerWin;
	bool autoRout;
	bool maybeIsNotNaval;
	bool attackerPlayerControlled;
	bool defenderPlayerControlled;
	bool attackerWon;
	bool attackerInitiative;
	bool defendersExposed;
	bool gatesOpen;
	int8_t byte_56[2];
	int gate_defence_strength_oil;
	int gate_defence_strength_arrow;
	int gate_defence_strength_default;
	int sett_defence_strength_arrow;
	int sett_defence_strength_default;
	bool hasRams;
	bool experienceForAttackers;
	bool experienceForDefenders;
	int8_t byte_6F;
};

struct fortBattleInfo
{
	struct fortStruct *fort;
	struct armyStruct *garrison;
	struct factionStruct *faction;
	int ownerFactionID;
	int creatorFactionID;
	int fortFortificationLevel;
};

struct terrainFeatures
{
	gameList<terrainLineSegment> lines;
	gameList<terrainFeatureHill> hills;
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
	bool isNightBattle; //0x000C
	char pad_00D[3]; //0x000D
	int battleWinType;
	int32_t battleResult; //0x0014
	int xCoord;//0x0018
	int yCoord;//0x001C
	int battleCampaignType;
	bool isValidCampaign;
	char pad[3];
	int attackingSide;
	int defendingSide;
	int attackerXCoord;//0x0030
	int attackerYCoord;//0x0034
	int defenderXCoord;//0x0038
	int defenderYCoord;//0x003C
	float attackDirX; //0x0040
	float attackDirY; //0x0044
	int residenceOwnerFacId;
	bool battleAiEnabled; //0x004C
	bool started; //0x004D
	bool inBattle; //0x004E
	bool defenderWithdrawn; //0x004F
	int32_t duration; //0x0050
	bool paused; //0x0054
	char pad_0055[3]; //0x0055
	float speed; //0x0058
	int ticks;
	int ticksMilliSec;
	float secondsPassed; //0x0064
	int32_t secondsSinceBattleLoaded; //0x0068
	int finishTimer;
	int32_t autoResolveMode;
	float centreX;
	float centreY;
	float mapWidth; //0x007C
	float mapHeight; //0x0080
	bool hasOverrides;
	int8_t byte_0x85[3];
	int speechTimer;
	bool inDelayState;
	int8_t byte_0x8D[3];
	uint8_t hidingEnabledSet;
	uint8_t byte_0x91[3];
	uint32_t deadSoldiers;
	struct UNICODE_STRING** someUniString; //0x0098
	struct battleSide sides[8]; //0x009C
	int32_t factionSide[31]; //0xC9FC
	int sidesNum;//0xCA78
	int timeWinningAlliance;
	int32_t totalSoldierCount; //0xCA80
	terrainFeatures *terrainFeatures;
	void *lights;
	int lightsSize;
	int lightsNum;
	struct playerArmy playerArmies[31];
	int32_t playerArmyNum;
	bool isAutoResolve;
	bool isFortBattle;
	int8_t byte_CD82[2];
	struct fortBattleInfo fortInfo;
	bool canPlaceWagonFort;
	bool hasWagonFort;
	int8_t byte_CD9E[2];
	int totalArmiesAdded;
	gameStdVector<void*> cursorRestrictions;
	int unitSize;
	bool tutorial;
	bool weatherChanged;
	int8_t byte_CDba[2];
	int tideOfBattle;
	struct autoResolveData autoResolveData;
};//Size: 0xCA7C

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


struct eduOfficer
{
	char* name;
	int nameHash;
	int modelIndex;
	int modelGroup;
	struct modelDbEntry* modelEntry;
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
	int pathHash;
	int resource_num;
	int resource_cost;
	char* tga;
	int tgaPath;
	bool hasMine;
	char pad[3];
};

/* rigid model(cas model for "static" objects - settlements, resources, etc) on stratmap! */
struct model_Rigid { /* rigid model(cas model for "static" objects - settlements, resources, etc) on stratmap! */
	undefined field_0x0[4];
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
	armyStruct* army;
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

struct tradingResource {
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
	struct tradingResource resources[27];
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
	struct eduEntry* entry;
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

enum class buildingCapabilities
{
	population_growth_bonus = 0,
	population_loyalty_bonus = 1,
	population_health_bonus = 2,
	trade_base_income_bonus = 3,
	trade_level_bonus = 4,
	trade_fleet = 5,
	taxable_income_bonus = 6,
	mine_resource = 7,
	farming_level = 8,
	road_level = 9,
	gate_strength = 10,
	gate_defences = 11,
	wall_level = 12,
	tower_level = 13,
	armour = 14,
	stage_games = 15,
	stage_races = 16,
	fire_risk = 17,
	weapon_melee_simple = 18,
	weapon_melee_blade = 19,
	weapon_missile_mechanical = 20,
	weapon_missile_gunpowder = 21,
	weapon_artillery_mechanical = 22,
	weapon_artillery_gunpowder = 23,
	weapon_naval_gunpowder = 24,
	upgrade_bodyguard = 25,
	recruits_morale_bonus = 26,
	recruits_exp_bonus = 27,
	happiness_bonus = 28,
	law_bonus = 29,
	construction_cost_bonus_military = 30,
	construction_cost_bonus_religious = 31,
	construction_cost_bonus_defensive = 32,
	construction_cost_bonus_other = 33,
	construction_time_bonus_military = 34,
	construction_time_bonus_religious = 35,
	construction_time_bonus_defensive = 36,
	construction_time_bonus_other = 37,
	construction_cost_bonus_wooden = 38,
	construction_cost_bonus_stone = 39,
	construction_time_bonus_wooden = 40,
	construction_time_bonus_stone = 41,
	free_upkeep = 42,
	pope_approval = 43,
	pope_disapproval = 44,
	religion_level = 45,
	amplify_religion_level = 46,
	archer_bonus = 47,
	cavalry_bonus = 48,
	heavy_cavalry_bonus = 49,
	gun_bonus = 50,
	navy_bonus = 51,
	recruitment_cost_bonus_naval = 52,
	retrain_cost_bonus = 53,
	weapon_projectile = 54,
	income_bonus = 55,
	recruitment_slots = 56,
};

inline bool operator ==(int a, buildingCapabilities b)
{
	return static_cast<buildingCapabilities>(a) == b;
}

inline bool operator !=(int a, buildingCapabilities b)
{
	return static_cast<buildingCapabilities>(a) != b;
}


#pragma pack(pop)