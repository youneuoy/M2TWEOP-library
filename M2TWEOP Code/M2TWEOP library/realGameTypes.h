#pragma once
#include <string>
#include <cstdint>
#include <windows.h>

typedef unsigned char undefined;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
#pragma pack(push,1)

#define FLOAT_EQUAL(_a, _b) (fabs(_a - _b) < 0.0001f)

/*----------------------------------------------------------------------------------------------------------------*\
									Firing game function helpers
\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Game function helpers

#define GAME_FUNC(funcType, funcAddr) reinterpret_cast<funcType>(codes::offsets.funcAddr)
#define GAME_FUNC_RAW(funcType, funcAddr) reinterpret_cast<funcType>(funcAddr)
template<unsigned int IIdx, typename TRet, typename ... TArgs>
TRet callVFunc(void* thisPtr, TArgs ... argList)
{
	using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
	return (*static_cast<Fn**>(thisPtr))[IIdx](thisPtr, argList...);
}
template<typename ClassType, typename ReturnType, typename ... TArgs>
ReturnType callClassFunc(ClassType instance, const DWORD offset, TArgs ... args)
{
	const DWORD vtable = *reinterpret_cast<DWORD*>(instance);
	DWORD vFunc = *reinterpret_cast<DWORD*>(vtable + offset);
	return GAME_FUNC_RAW(ReturnType(__thiscall*)(ClassType, TArgs...), vFunc)(instance, args...);
}
#pragma endregion Game function helpers

/*----------------------------------------------------------------------------------------------------------------*\
									 Game standard types and structures
\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Game standard types

struct basicStringGame
{
	char pad[0x4];
	union
	{
		char* long_string;
		char short_string[16];
	};
	int stringLength;
	int stringCapacity;
public:
	char* getData () { return stringCapacity < 16 ? short_string : long_string; }
	std::string getString() { return { getData()};}
	void setString(const std::string& str)
	{
		stringLength = str.length();
		stringCapacity = str.capacity();
		if (stringCapacity < 16)
		{
			std::memset(short_string, 0, 16);
			strcpy(short_string, str.c_str());
		}
		else
		{
			delete long_string;
			long_string = new char[stringCapacity];
			strcpy(long_string, str.c_str());
		}
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
		const int num = size();
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
	int size() const
	{
		return size();
	}
	bool empty()
	{
		return currentCount == 0;
	}
	bool empty() const
	{
		return empty();
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
	T* get(int index) const
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

template<typename T>
struct gameLinkedNode
{
	T* element;
	gameLinkedNode<T>* prev;
	gameLinkedNode<T>* next;
};

template<typename T>
struct gameLinkedList
{
	gameLinkedNode<T>* head;
	gameLinkedNode<T>* tail;
	int size()
	{
		int total = 0;
		gameLinkedNode<T>* current = head;
		while (current)
		{
			total++;
			current = current->next;
		}
		return total;
	}
	bool empty()
	{
		return head == nullptr;
	}
	T* get(int index)
	{
		gameLinkedNode<T>* current = head;
		while (current && index > 0)
		{
			index--;
			current = current->next;
		}
		if (current)
			return current->element;
		return nullptr;
	}
	T& operator[](int index)
	{
		return *get(index);
	}
};

template<typename T, const int N>
struct gameCircularBuffer
{
	T elements[N];
	int head;
	int tail;
	int num;

	T& operator[](const int index)
	{
		return elements[(head + index) % N];
	}

	const T&  operator[](const int index) const
	{
		return elements[(head + index) % N];
	}

	bool empty()
	{
		return num == 0;
	}

	T& first()
	{
		return elements[head];
	}

	T& last()
	{
		return elements[tail];
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
	[[nodiscard]] std::string getString() const { return {name}; }
	void setString(const std::string& str);
}; //Size

struct UNICODE_STRING {
	USHORT something;//idk
	USHORT Length;//idk
	USHORT something2;//idk
	PWSTR Buffer;//y
};

struct boostLoggerImpl
{
	gameStdVector<void*> appenders;
	gameStdVector<void*> modifiers;
	basicStringGame name;
	unsigned int level;
	bool isCompileTime;
	char pad[0x3];
	char threadingMutex[0x18];
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

struct coordPair
{
public:
	int xCoord; //0x0000
	int yCoord; //0x0004
	explicit coordPair(int x, int y) : xCoord(x), yCoord(y) {}
	coordPair() : xCoord(0), yCoord(0) {}
}; //Size: 0x0008

struct trackedCharacter
{
public:
	void* vtbl; //0x0000
	struct character* character; //0x0004
}; //Size: 0x0008

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

struct oneSiege {
	void* vtable;
	struct siegeS* siege;
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

struct spyingInfo
{
public:
	DWORD *spyingInfo_vtbl; //0x0000
	int8_t spyingInfoFactionArray[31]; //0x0004
	char pad_0023[1]; //0x0023
	int randomNumber; 
	int randomSeed; 
}; //Size: 0x002C

#pragma endregion Game standard types

/*----------------------------------------------------------------------------------------------------------------*\
									 Strat models
\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Strat models

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
	DWORD models[8]; //0x0000
	float lodRanges[8]; //0x0020
	bool builtModels[8]; //0x0020
	struct casTextures modelTexturesFactionArray[31]; //0x0048
	int32_t altTextures[31]; //0x0140
	int32_t diffuse; //0x01C4
	int32_t gloss; //0x01C4
	float red;
	float green;
	float blue;
	float triRange; //0x01D8
	DWORD skeleton[2]; //0x01D4
	int32_t modelCount; //0x01DC
	float indivRangeSquared; //0x01E0
	DWORD shadowModels[8]; //0x01E4
	float shadowModelRanges[8]; //0x0204
	int32_t shadowModelCount; //0x0224
	void* attachmentDb; //0x0228
	char* typeName; //0x022C
	int32_t typeNameHash; //0x0230
}; //Size: 0x0234

struct stratModelListEntry
{
public:
	stringWithHash modelName; //0x0000
	struct stratModelArrayEntry* stratModelEntry; //0x0008
}; //Size: 0x000C

struct descrCharacterStratModelArray
{
public:
	struct stratModelListEntry stratModelsArray[16]; //0x0000
}; //Size: 0x00C0

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
	undefined field_0x0[0x164];
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

#pragma endregion Strat models

/*----------------------------------------------------------------------------------------------------------------*\
									 Game Data All Struct
\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Game Data

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
	struct battleTerrainData* battleTerrainDataPtr;
	struct battleTerrainData* battleTerrainData2;
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
	struct selectionInfo* selectInfo;
	struct selectionInfo** selectionInfoPtr;
	struct selectionInfo** selectionInfoPtr2;
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
#pragma endregion Game Data

/*----------------------------------------------------------------------------------------------------------------*\
									 Shared stuff for units
\*----------------------------------------------------------------------------------------------------------------*/
#pragma region unit structs

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
	float width; //0x0020
	float height; //0x0024
	float centreX; //0x0028
	float centreY; //0x002C
	float minRadians; //0x0030
	float maxRadians; //0x0034
}; //Size: 0x0044

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

struct eduOfficer
{
	char* name;
	int nameHash;
	int modelIndex;
	int modelGroup;
	struct modelDbEntry* modelEntry;
};
#pragma endregion unit structs

/*----------------------------------------------------------------------------------------------------------------*\
									 Game Enums
\*----------------------------------------------------------------------------------------------------------------*/
#pragma region Game Enums

enum class characterTypeStrat : uint8_t
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

inline bool operator >= (int a, characterTypeStrat b)
{
	return static_cast<characterTypeStrat>(a) >= b;
}

inline bool operator > (int a, characterTypeStrat b)
{
	return static_cast<characterTypeStrat>(a) > b;
}

inline bool operator <= (int a, characterTypeStrat b)
{
	return static_cast<characterTypeStrat>(a) <= b;
}

inline bool operator < (int a, characterTypeStrat b)
{
	return static_cast<characterTypeStrat>(a) < b;
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
	unitIdle = 19,
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

enum class unitStatus									
{
	idle,									
	hiding,									
	taunting,								
	celebrating,
	ready,									
	reforming,								
	moving,									
	withdrawing,							
	missilesFiring,							
	missilesReloading,						
	charging,								
	fighting,								
	pursuing,								
	routing,								
	fightingBacksToTheWalls,				
	runningAmok,							
	berserk,								
	rallying,								
	dead,									
	leavingBattle,							
	enteringBattle,							
	leftBattle,								
	bracing,								
	infighting,								
};

inline bool operator ==(int a, unitStatus b)
{
	return static_cast<unitStatus>(a) == b;
}

inline bool operator !=(int a, unitStatus b)
{
	return static_cast<unitStatus>(a) != b;
}

enum charAttributes
{
	command = 0,
	influence = 1,
	subterfuge = 2,
	loyalty = 3,
	charm = 4,
	finance = 5,
	piety = 6,
	magic = 7,
	chivalry = 8,
	authority = 9,
	level = 10,
	unorthodoxy = 11,
	heresyImmunity = 12,
	assassination = 13,
	sabotage = 14,
	eligibility = 15,
	purity = 16,
	violence = 17,
	disposition = 18,
	boldness = 19,
	generosity = 20,
	management = 21,
	bodyguardSize = 22,
	troopMorale = 23,
	movementPoints = 24,
	attack = 25,
	defence = 26,
	siegeAttack = 27,
	siegeDefence = 28,
	ambush = 29,
	navalCommand = 30,
	siegeEngineering = 31,
	nightBattle = 32,
	personalSecurity = 33,
	publicSecurity = 34,
	bribery = 35,
	briberyResistance = 36,
	electability = 37,
	lineOfSight = 38,
	trainingUnits = 39,
	trainingAgents = 40,
	construction = 41,
	trading = 42,
	localPopularity = 43,
	footInTheDoor = 44,
	farming = 45,
	mining = 46,
	taxCollection = 47,
	fertility = 48,
	cavalryCommand = 49,
	infantryCommand = 50,
	gunpowderCommand = 51,
	artilleryCommand = 52,
	startReligionCombat = 53,
	combatVReligion0 = startReligionCombat,
	combatVReligion1,
	combatVReligion2,
	combatVReligion3,
	combatVReligion4,
	combatVReligion5,
	combatVReligion6,
	combatVReligion7,
	combatVReligion8,
	combatVReligion9,
	endReligionCombat = combatVReligion9,
	startFactionCombat,
	combatVFaction0 = startFactionCombat,
	combatVFaction1,
	combatVFaction2,
	combatVFaction3,
	combatVFaction4,
	combatVFaction5,
	combatVFaction6,
	combatVFaction7,
	combatVFaction8,
	combatVFaction9,
	combatVFaction10,
	combatVFaction11,
	combatVFaction12,
	combatVFaction13,
	combatVFaction14,
	combatVFaction15,
	combatVFaction16,
	combatVFaction17,
	combatVFaction18,
	combatVFaction19,
	combatVFaction20,
	combatVFaction21,
	combatVFaction22,
	combatVFaction23,
	combatVFaction24,
	combatVFaction25,
	combatVFaction26,
	combatVFaction27,
	combatVFaction28,
	combatVFaction29,
	combatVFaction30,
	combatVFaction31,
	endFactionCombat = combatVFaction31,
	health,
	squalor,
	unrest,
	law,
	looting,
	bodyguardValour,
	hitPoints,
	trainingAnimalUnits,
	battleSurgery,
	attributeNum
};

inline bool operator ==(int a, charAttributes b)
{
	return static_cast<charAttributes>(a) == b;
}

inline bool operator !=(int a, charAttributes b)
{
	return static_cast<charAttributes>(a) != b;
}

inline bool operator >=(int a, charAttributes b)
{
	return static_cast<charAttributes>(a) >= b;
}

inline bool operator >(int a, charAttributes b)
{
	return static_cast<charAttributes>(a) > b;
}

inline bool operator <= (int a, charAttributes b)
{
	return static_cast<charAttributes>(a) <= b;
}

inline bool operator < (int a, charAttributes b)
{
	return static_cast<charAttributes>(a) < b;
}

#pragma endregion Game Enums

#pragma pack(pop)