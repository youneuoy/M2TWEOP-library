#pragma once
#include "battle.h"
#include "..\gameStringHelpers.h"
#include "realGameTypes.h"
#include "lua/sol.hpp"

enum unitBattleProperties :int
	{
		guardMode = 1,
		fireAtWill = 2,
		skirmish = 4,
	};

enum class unitCategory
{
	infantry,
	cavalry,
	siege,
	nonCombatant,
	ship,
	animalHandler,
};

inline bool operator ==(int a, unitCategory b)
{
	return static_cast<unitCategory>(a) == b;
}

inline bool operator !=(int a, unitCategory b)
{
	return static_cast<unitCategory>(a) != b;
}
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

enum class unitClass
{
	heavy,
	light,
	skirmish,
	spearmen,
	missile
};

inline bool operator ==(int a, unitClass b)
{
	return static_cast<unitClass>(a) == b;
}

inline bool operator !=(int a, unitClass b)
{
	return static_cast<unitClass>(a) != b;
}
	
/* 2193 */
struct unitVerification
{
	unitVerificationTable *unitVerificationTable;
	unitVerificationElement *tablePos;
	int unitVerField_8;
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
	basicStringGame mount;
	basicStringGame primary;
	basicStringGame secondary;
	gameStdVector<basicStringGame> primaryAttachments;
	gameStdVector<basicStringGame> secondaryAttachments;
}; //Size: 0x0078


struct modelDbEntry
{
public:
	basicStringGame type;
	float scale; //0x001C
	gameStdVector<void*> meshes;
	gameStdVector<void*> textures;
	gameStdVector<void*> attTextures;
	gameStdVector<modelDbAnims> skeletons;
	int32_t torchAttachBone; //0x0060
	float N00000857; //0x0064
	float N00000858; //0x0068
	float N00000859; //0x006C
	float N0000085A; //0x0070
	float N0000085B; //0x0074
	float N0000085C; //0x0078
	char pad_007C[4]; //0x007C
}; //Size: 0x0080

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

struct generalStats
{
public:
	struct soldierInBattle* generalBgSoldier; //0x0000
	struct soldierInBattle* generalBgMount; //0x0004
}; //Size: 0x0008

struct gfxEntityStruct
{
public:
	void* gfxEntityMeshes; //0x0000
	void* sharedPointer; //0x0004
	float float_8; //0x0008
	float float_C; //0x000C
	float float_10; //0x0010
	float float_14; //0x0014
	struct modelDbEntry *bmdbEntry; //0x0018
	char (*spritePaths)[4]; //0x001C
	int32_t field_20; //0x0020
	int32_t field_24; //0x0024
	int32_t field_28; //0x0028
	int32_t field_2C; //0x002C
	int32_t field_30; //0x0030
	int32_t field_34; //0x0034
	int32_t field_38; //0x0038
	int32_t field_3C; //0x003C
	int32_t field_40; //0x0040
	int32_t field_44; //0x0044
	int32_t field_48; //0x0048
	int32_t field_4C; //0x004C
	int32_t field_50; //0x0050
	int32_t field_54; //0x0054
	int32_t field_58; //0x0058
	int32_t field_5C; //0x005C
	int32_t field_60; //0x0060
	int32_t field_64; //0x0064
	int32_t field_68; //0x0068
	int32_t field_6C; //0x006C
	int32_t field_70; //0x0070
	int32_t field_74; //0x0074
	int32_t field_78; //0x0078
	int32_t field_7C; //0x007C
	int32_t field_80; //0x0080
	int32_t field_84; //0x0084
	int32_t field_88; //0x0088
	int32_t field_8C; //0x008C
	int32_t field_90; //0x0090
	int32_t field_94; //0x0094
	int8_t byte_98; //0x0098
	int8_t byte_99; //0x0099
	int8_t byte_9a; //0x009A
	int8_t byte_9b; //0x009B
	void *sprite; //0x009C
	int32_t field_A0; //0x00A0
	int32_t field_A4; //0x00A4
	int32_t field_A8; //0x00A8
	int32_t field_AC; //0x00AC
	int32_t field_B0; //0x00B0
}; //Size: 0x00B4

struct moraleEffect
{
	int id;
	int modAmount;
	int amount;
};

struct moraleEffects
{
	moraleEffect effects[34];
};

struct moraleStruct
{
	struct unit *thisUnit;
	int moraleLevel;
	int moraleThreshold;
	int moraleBerserkCounter;
	int surpriseCounter;
	int routingFinishCounter;
	int routingRallyCounter;
	int waveringTimer;
	int chargeTimer;
	moraleEffects effects;
	moraleEffects *effectsPtr;
	int effectCount;
	int moraleBonus;
	int bloodBonus;
	int8_t inMoraleShock;
	int8_t isRallied;
	int8_t processRally;
	int8_t forceRout;
	int8_t lockedMorale;
	int8_t enemyCurseEffectCounter;
	int8_t curseEffectCounter;
	int8_t enemyChantEffectCounter;
	int8_t chantEffectCounter;
	int8_t routingCounter;
	int8_t byte_1d6;
	int8_t byte_1d7;
	int aeCowCarcass;
	int ae_holy_inspiration;
	int minMorale;
	int maxMorale;
	int8_t minMoraleSet;
	int8_t maxMoraleSet;
	int8_t byte_1ea[2];
	int forceRallyStatus;
	int minRoutDelay;
};

struct nearbyEnemyUnit
{
	unit *unit;
	int attackPosition;
	int8_t checked;
	int8_t neighbouring;
	int8_t pada[2];
	int timedOut;
	int attackMode;
};

struct unitTaskInterface
{
	void *vftable /*VFT*/;
	struct unitTaskMem *taskMem;
};

struct unitTaskMem
{
	void *vftable /*VFT*/;
	unit *unit;
	void *unitTask;
	int32_t actionStatus;
	int field_10;
	int field_14;
	int field_18;
	int32_t field_1C;
	float float_20;
	float float_24;
	char pad_0634[36];
	int8_t byte_4c;
	char pad_0659[71];
	int32_t getsComparedToSoldierIndexAtFormedCharge;
	int field_06A4;
	int32_t getsCheckedForZeroInFormedCharge;
	char pad_00a0[4524];
	int32_t isGeneral;
	char pad_1250[1472];
	int field_1810;
	char byte_1814;
	char field_1815;
	char field_1816;
	char field_1817;
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

struct unitAiGroupData
{
	struct aiUnitGroup *unitGroup;
	int32_t unitAiStatus;
	bool inMelee;
	bool releasedFromMelee;
	char pad[2];
	struct aiBattleObjective *battleObjective;
	bool isAvailable;
	char pad2[3];
	int meleeAnalyzerDetails;
	float targetX;
	float targetY;
};

struct soldierData
{
	unsigned __int32 name : 6;
	unsigned __int32 years : 6;
	unsigned __int32 experience : 4;
	unsigned __int32 numKills : 4;
	unsigned __int32 scale : 3;
	unsigned __int32 armour : 3;
	unsigned __int32 weapon : 3;
public:
	int getName()
	{
		return name;
	}
	int getYears()
	{
		return years;
	}
	int getExperience()
	{
		return experience;
	}
	int getNumKills()
	{
		return numKills;
	}
	int getScale()
	{
		return scale;
	}
	int getArmour()
	{
		return armour;
	}
	int getWeapon()
	{
		return weapon;
	}
	void setName(int newName)
	{
		name = newName;
	}
	void setYears(int newYears)
	{
		years = newYears;
	}
	void setExperience(int newExp)
	{
		experience = newExp;
	}
	void setNumKills(int newKills)
	{
		numKills = newKills;
	}
	void setScale(int newScale)
	{
		scale = newScale;
	}
	void setArmour(int newArmour)
	{
		armour = newArmour;
	}
	void setWeapon(int newWeapon)
	{
		weapon = newWeapon;
	}
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
	struct characterRecord* namedChar; //0x0008
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

struct heroAbility
{
	char *name; //0x0000
	char pad_0004[4]; //0x0004
	void* generalStuff; //0x0008
	int32_t usesLeft; //0x000C
	int32_t cooldown; //0x0010
	int32_t abilityTimer; //0x0014
	int32_t duration; //0x0018
};

struct unitMoveStruct
{
	int xCoord;
	int yCoord;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int noDraggedFormation;
	int16_t angle;
	int16_t short_1e;
	float float_20;
	int field_24;
	float somethingDistanceRelated;
	int widthInMen;
	char isClose;
	char byte_31;
	char byte_32;
	char byte_33;
	int field_34;
	char isFormed;
	char byte_39;
	char byte_3a;
	char byte_3b;
	float float_3C;
	int8_t byte_40;
	int8_t hasSetAngle;
	int8_t byte_42;
	int8_t hasSetWidth;
	int8_t hasCloseSet;
	int8_t byte_45;
	int8_t byte_46;
	int8_t byte_47;
};

struct targetPos
{
public:
	int32_t actionType;
	int groupType;
	unitVerification targetVerification;
	unitMoveStruct moveStruct;
	float targetPosXEnd; //0x005C
	float targetPosYEnd; //0x0060
	int8_t run;
	int8_t pad_65[3];
	void *worldData;
	int someIdForSiege;
	float targetCoordXEngine;
	float targetCoordZEngine;
	float targetCoordYEngine;
	siegeEngine *siegeEngine;
	int32_t meleeStateBitfield;
	int32_t formationId;
	int32_t field_88;
	int32_t field_8C;
	int8_t byte_90;
	int8_t pad_91[3];
	int field_94;
	int16_t frontRankSoldierCount;
	int16_t short_98;
	float float_9C;
	float float_a0;
	int16_t targetAngle;
	int8_t formationClose;
	int8_t byte_a7;
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
	unit *unit;
	int32_t engagedSoldiers;
	int32_t engagedDogs;
	int32_t engagedPigs;
	int32_t flamingPigs;
	int32_t spearPoints;
	float engagedRatio;
}; //Size: 0x001C

struct firingUnit
{
	public:
	struct unit* unit; //0x0000
	int32_t timer; //0x0004
};

struct firingBuilding
{
	public:
	struct buildingBattle* attacker; //0x0000
	int32_t timer; //0x0004
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
	struct soldierInBattle *engagedSoldier;
    void *engagedSpear;
	int soldiersEngaged;
	int timeEngaged;
	struct unit *thisUnit;
	bool forcedToBlock;
	bool isBlocked;
	int8_t pad_2E[2];
    unit *blockedBy;
	int area;
	struct unit *closestEnemyUnit;
    float closestEnemyUnitDist;
	float xCoord;
	float zCoord;
	float yCoord;
	int16_t angle;
	int8_t pad_4E[2];
	struct battleMapPosition* isOnWalls; //0x0050
	int32_t isOnWallsSize; //0x0054
	int32_t isOnWallsCount; //0x0058
	struct battleMapPosition* isInTower; //0x005C
	int32_t isInTowerSize; //0x0060
	int32_t isInTowerCount; //0x0064
	struct battleMapPosition* isInGateHouse; //0x0068
	int32_t isInGateHouseSize; //0x006C
	int32_t isInGateHouseCount; //0x0070
	struct battleMapPosition* areas; //0x0074
	int32_t areasSize; //0x0078
	int32_t areasCount; //0x007C
	struct battleMapPosition* areaLinks; //0x0080
	int32_t areaLinksSize; //0x0084
	int32_t areaLinksCount; //0x0088
	struct battleMapPosition* perimeters; //0x008C
	int32_t perimetersSize; //0x0090
	int32_t perimetersCount; //0x0094
	struct battleMapPosition* zones; //0x0098
	int32_t zonesSize; //0x009C
	int32_t zonesCount; //0x00A0
	int soldierCountGround;
	int soldierCountWater;
	int soldierCountTotal;
	struct floatPosData floatPosData;
	int16_t floatPosDataAngle;
	char pad_00EA[2]; //0x00EA
	int facingX;
	int facingY;
	int wallEntityID;
	struct targetPos targetArray[16];
	int32_t startIndex;
	int32_t endIndex;
	int32_t currentIndex;
	bool hasTargets;
	bool isHalted;
	char pad_B86[2];
	char isValid;
	char pad_B89[3];
	float lastTargetCoordX; //0x0B8C
	float lastTargetCoordY; //0x0B90
	int16_t lastTargetAngle;
	char pad_B96[2];
	int32_t formationType; //0x0B98
	int8_t formationClose; //0x0B9C
	char pad_0B9D[3]; //0x0B9D
	int32_t width; //0x0BA0
	int32_t platform; //0x0BA4
	struct floatPosData destination;
	bool isValid2;
	char padvalid[3];
	int lastTargetCoordX2;
	int lastTargetCoordY2;
	__int16 lastTargetAngle2;
	char pad_B962[2];
	bool skirmishing;
	bool firingAtWill;
	bool engagingAtWill;
	bool waitingToMove;
	bool firingWhileMoving;
	bool hasDefeatedEnemy;
	bool hidden;
	char pad_hidden[1];
	int pathHandle;
	int alliance;
	firingBuilding* towersUnderFireFrom; //0x0C00
	int32_t towersUnderFireFromSize; //0x0C04
	int32_t towersUnderFireFromCount; //0x0C08
	struct firingUnit* unitsUnderFireFrom; //0x0C0C
	int32_t unitsUnderFireFromSize; //0x0C10
	int32_t unitsUnderFireFromCount; //0x0C14
	firingBuilding* towersUnderFireFromArtillery; //0x0C18
	int32_t towersUnderFireFromArtillerySize; //0x0C1C
	int32_t towersUnderFireFromArtilleryCount; //0x0C20
	firingUnit *artilleryUnderFireFrom;
	int32_t artilleryUnderFireFromSize;
	int32_t artilleryUnderFireFromNum;
	int attackedFire;
	int attackedGunpowder;
	int combatStatus;
	int unitTurnData[12];
	int turnDataStart;
	int turnDataEnd;
	int turnDataIndex;
	float casualtiesAll;
	float casualtiesFour;
	float killsAll;
	float killsFour;
	int lastSoldierKilled;
	int centreSoldier;
	int outOfCombatCounter;
	struct unit *targetUnit;
	int *engageMode;
    float engageDistance;
	struct unit *targetUnitMissile;
    armyStruct *reinforcingArmy;
	char pathHistory[240];
	int pathHistoryStart;
	int pathHistoryEnd;
	int pathHistoryIndex;
	unit* getUnitUnderFireFrom(int index)
	{
		if (index < unitsUnderFireFromCount)
		{
			return unitsUnderFireFrom[index].unit;
		}
		return nullptr;
	}
	unit* getEngagedUnit(int index)
	{
		if (index < engagedUnitsNum)
		{
			return engagedUnits[index].unit;
		}
		return nullptr;
	}
	unit* getTargetUnit()
	{
		if (!battleHelpers::inBattle())
			return nullptr;
		if (!hasTargets || isHalted)
		{
			return nullptr;
		}
		if (targetArray[currentIndex].targetVerification.tablePos)
		{
			return targetArray[currentIndex].targetVerification.tablePos->unit;
		}
		return nullptr;
	}
}; //Size: 0x0C3C
	
struct siegeEngine
{
	void *vftable /*VFT*/;
	int field_4;
	int type;
	int bitfield;
	float mass;
	float inverseMass;
	float posX;
	float posZ;
	float posY;
	struct soldierInBattle *soldier;
	float someRadius1;
	float someRadius2;
	float someRadius3;
	float markerRadius;
	float height;
	int field_3C_initMinus1;
	int field_40;
	__int16 angle;
	__int16 pad46;
	int velocityX;
	int velocityZ;
	int velocityY;
	int field_54;
	battleTile *battleTile;
	int field_5C;
	void *vftbl_5_0000000001317C08;
	int engineID;
	int field_68;
	void *meshStuff;
	int field_70;
	int field_74;
	int ID;
	int health;
	struct engineRecord* engineRec;
	struct unit* currentUnit;
	struct unit* lastUnit;
	unitStats stats;
	int proxyDiameter;
	int proxies;
	int currentModel;
	char skelState[176];
	char wheelChassis[248];
	float centreX;
	float centreZ;
	float centreY;
	float radius;
	float visualRadius;
	int adjustedRotation[4];
	int field_2BC;
	int field_2C0;
	int field_2C4;
	int field_2C8;
	int field_2CC;
	int field_2D0;
	int field_2D4;
	int field_2D8;
	int field_2DC;
	int field_2E0;
	int field_2E4;
	int field_2E8;
	int field_2EC;
	int field_2F0;
	int field_2F4;
	int field_2F8;
	float destX;
	float destY;
	int field_304;
	int field_308;
	int field_30C;
	int16_t short_310;
	int16_t short_312;
	int field_314;
	int field_318;
	int field_31C;
	float float_320;
	int field_324;
	char gap328[60];
	int field_364;
	char gap368[172];
	int target;
	int idleAnim;
	int field_41C;
	void *field_420;
	int field_424;
	char gap428[24];
	uint32_t bitfield440;
	float float_444;
	int field_448;
	int field_44C;
	int field_450;
	int field_454;
	int field_458;
	int field_45C;
	int field_460;
	int field_464;
	int field_468;
	int field_46C;
	int field_470;
	int field_474;
	int field_478;
	int field_47C;
	int field_480;
	char gap484[88];
	int field_4DC;
	char gap4E0[8];
	void *vftbl_85_0000000001317B60;
	char gap4EC[8];
	void *vftbl_86_0000000001317B84;
	char gap4F8[92];
	char byte_554;
	char byte_555;
	char byte_556;
	char byte_557;
	int field_558;
	int field_55C;
	int field_560;
	int field_564;
	int field_568;
	int field_56C;
	int moveStatus;
	bool wasMoving;
	bool isMoving;
	char byte_576;
	char byte_577;
	float lastPosX;
	float lastPosY;
	int16_t lastAngle;
	int16_t short_582;
	int fireLevel;
	bool isBurning;
	char byte_589[3];
	int fireEffect;
	int fireSound;
	int effects;
	int effectsSize;
	int effectsNum;
	int infoPtrs;
	int infoPtrsSize;
	int infoPtrsNum;
	char byte_5AC;
	char byte_5Ad;
	char byte_5Ae;
	char byte_5Af;
	int field_5B0;
	int bitfield_5B4;
	int field_5B8;
	int field_5BC;
	int field_5C0;
	char byte_5C4;
	char byte_5C5;
	char byte_5C6;
	char byte_5C7;
	int field_5C8;
	int field_5CC;
	int field_5D0;
	int field_5D4;
	int field_5D8;
	int field_5DC;
	int totalAmmo;
	int totalMaxAmmo;
	char ammoStacksCount;
	char byte_5E9[3];
	int areaEffect;
	int debugShowProxies;
	int deployingFaction;
	int field_5F8;
	int field_5FC;
	int field_600;
public:
	int getEngineType()
	{
		return engineRec->classID;
	}
};

struct mountEffect
{
	int mountClass;
	int mountIndex;
	int bonus;
};

//type of unit from EDU
struct eduEntry {
	char* eduType;
	uint32_t typeHash;
	DWORD index;
	DWORD unitCreatedCounter;
	char* unitCardTga;
	uint32_t unitCardHash;
	char* infoCardTga;
	uint32_t unitInfoCardHash;
	char* cardPicDir;
	uint32_t cardPicDirHash;
	char* infoPicDir;
	uint32_t infoPicDirHash;
	UNICODE_STRING*** localizedName;
	UNICODE_STRING*** localizedDescr;
	UNICODE_STRING*** localizedDescrShort;
	uint32_t category;
	uint32_t unitClass;
	uint32_t unitProductionClass;
	DWORD voiceType;
	char *accent;
	uint32_t accentHash;
	char *bannerFaction;
	uint32_t bannerFactionHash;
	char *bannerUnit;
	uint32_t bannerUnitHash;
	char *bannerHoly;
	uint32_t bannerHolyHash;
	char *bannerMarine;
	uint32_t bannerMarineHash;
	char *bannerSecondary;
	uint32_t bannerSecondaryHash;
	char *bannerMini;
	uint32_t bannerMiniHash;
	DWORD trained;
	DWORD discipline;
	bool moraleLocked;
	int8_t morale;
	int8_t statHealth;
	int8_t statHealthAnimal;
	int8_t statHeat;
	int8_t statScrub;
	int8_t statSand;
	int8_t statForest;
	int8_t statSnow;
	char pad_0095[1];
	ushort statFood1;
	ushort statFood2;
	ushort statFireDelay;
	DWORD statStl;
	float chargeDistance;
	mountEffect mountEffects[3];
	DWORD mountEffectCount;
	ushort recruitTime;
	ushort recruitCost;
	ushort upkeepCost;
	ushort weaponCost;
	ushort armourCost;
	ushort customBattleCost;
	ushort customBattleIncrease;
	ushort customBattleLimit;
	float crusadingUpkeepModifier;
	DWORD recruitPriorityOffset;
	int8_t formationHorde;
	int8_t formationColumn;
	int8_t formationSquare;
	int8_t formationSquareHollow;
	int8_t formationWedge;
	int8_t formationPhalanx;
	int8_t formationSchiltrom;
	int8_t formationShieldWall;
	int8_t formationWall;
	int8_t formationMovingThrough;
	char pad_00EE[2];
	uint32_t defaultRanks;
	float unitSpacingFrontToBackClose;
	float unitSpacingSideToSideClose;
	float unitSpacingFrontToBackLoose;
	float unitSpacingSideToSideLoose;
	char* soldier;
	int soldierHash;
    int modelIndex;
	void* modelGroup;
	struct modelDbEntry* modelEntry;
	uint16_t soldierCount;
	uint16_t pad11a;
	float mass;
	float width;
	float height;
	statPri priStats;
	statPri secStats;
	statArmour statPriArmour;
	eduOfficer officers[3];
	DWORD officerCount;
	gameStdVector<int8_t>armourUpgradeLevels{};
	gameStdVector<stringWithHash>armourUpgradeModels{};
	int extrasCount;
	struct engineRecord *engineRec;
	int engineExtras;
	statPri engineStats;
	statPri terStats;
	statArmour statSecArmour;
	struct descrMountEntry* mount;
	struct modelDbEntry* mountModel;
	statPri mountStats;
	int mountIndex; 
	statArmour statArmourMount;
	void *descrAnimal;
	modelDbEntry *animalBmdbEntry;
	statPri animalStats; 
	statArmour statArmourAnimal;
	void* mountedEngine;
	void* ship;
	uint32_t ownership;
	gameStdVector<uint32_t> eraOwnerShips{};
	float aiUnitValuePerSoldier;
	float aiUnitValue;
	gameStdVector<stringWithHash>attributes{};
	uint32_t canSap : 1;
	uint32_t canSetupEngines : 1;
	uint32_t canWithdraw : 1;
	uint32_t formedCharge : 1;
	uint32_t feignRout : 1;
	uint32_t runsAmok : 1;
	uint32_t canSwim : 1;
	uint32_t canHorde : 1;
	uint32_t hideForest : 1;
	uint32_t hideImprovedForest : 1;
	uint32_t hideLongGrass : 1;
	uint32_t hideAnywhere : 1;
	uint32_t powerCharge : 1;
	uint32_t seaFaring : 1;
	uint32_t command : 1;
	uint32_t heavyUnit : 1;
	uint32_t hardy : 1;
	uint32_t veryHardy : 1;
	uint32_t slave : 1;
	uint32_t frightenFoot : 1;
	uint32_t frightenMounted : 1;
	uint32_t freeUpkeepUnit : 1;
	uint32_t gunpowderUnit : 1;
	uint32_t gunpowderArtillery	: 1;
	uint32_t fireByRank : 1;
	uint32_t startNotSkirmishing : 1;
	uint32_t mercenaryUnit : 1;
	uint32_t noCustom : 1;
	uint32_t isPeasant : 1;
	uint32_t generalUnit : 1;
	uint32_t generalUnitUpgrade : 1;
	uint32_t cantabrianCircle : 1;
	uint32_t druid : 1;
	uint32_t screechingWomen : 1;
	uint32_t warCry	: 1;
	uint32_t stakes	: 1;
	uint32_t legio : 1;
	uint32_t canSkirmish : 1;
	float moveSpeedMod;
	basicStringGame	areaEffectAttribute;
	int wagonFortBitfield;
	float priSkeletonCompFactor;
	float secSkeletonCompFactor;
	float terSkeletonCompFactor;
public:
	int getArmourLevelNum()
	{
		return armourUpgradeLevels.size();
	}
	int getArmourLevel(int i)
	{
		if (i < armourUpgradeLevels.size())
		{
			return armourUpgradeLevels[i];
		}
		return -1;
	}
	void setArmourLevel(int i, int8_t newLevel)
	{
		if (i < armourUpgradeLevels.size())
		{
			armourUpgradeLevels[i] = newLevel;
		}
	}
	int getArmourModelNum()
	{
		return armourUpgradeModels.size();
	}
	std::string getArmourModel(int i)
	{
		if (i < armourUpgradeModels.size())
		{
			return std::string(armourUpgradeModels[i].name);
		}
		return "";
	}
	void setArmourModel(int i, const std::string& newModel)
	{
		if (i < armourUpgradeModels.size())
		{
			gameStringHelpers::setHashedString(const_cast<char**>(&armourUpgradeModels[i].name), newModel.c_str());
		}
	}
	bool hasAttribute(const std::string& attr)
	{
		for (int i = 0; i < attributes.size(); i++)
		{
			if (strcmp(attributes[i].name, attr.c_str()) == 0)
				return true;
		}
		return false;
	}
	bool hasOwnership(uint32_t factionID)
	{
		return ownership & (1 << factionID);
	}
	void setOwnership(uint32_t factionID, bool set)
	{
		if (set)
			ownership |= (1 << factionID);
		else
			ownership &= ~(1 << factionID);
	}
	
	std::string getPrimaryAnim()
	{
		if (!modelEntry)
			return "";
		if (modelEntry->skeletons.empty())
			return "";
		return modelEntry->skeletons[0].primary.getString();
	}

	std::string getSecondaryAnim()
	{
		if (!modelEntry)
			return "";
		if (modelEntry->skeletons.empty())
			return "";
		return modelEntry->skeletons[0].secondary.getString();
	}
	bool getIsLegio()
	{
		return legio;
	}
	void setIsLegio(bool set)
	{
		legio = set;
	}
	bool getHideForest()
	{
		return hideForest;
	}
	void setHideForest(bool set)
	{
		hideForest = set;
	}
	bool getHideImprovedForest()
	{
		return hideImprovedForest;
	}
	void setHideImprovedForest(bool set)
	{
		hideImprovedForest = set;
	}
	bool getHideLongGrass()
	{
		return hideLongGrass;
	}
	void setHideLongGrass(bool set)
	{
		hideLongGrass = set;
	}
	bool getHideAnywhere()
	{
		return hideAnywhere;
	}
	void setHideAnywhere(bool set)
	{
		hideAnywhere = set;
	}
	bool getCanSwim()
	{
		return canSwim;
	}
	void setCanSwim(bool set)
	{
		canSwim = set;
	}
	bool getCanHorde()
	{
		return canHorde;
	}
	void setCanHorde(bool set)
	{
		canHorde = set;
	}
	bool getPowerCharge()
	{
		return powerCharge;
	}
	void setPowerCharge(bool set)
	{
		powerCharge = set;
	}
	bool getSeaFaring()
	{
		return seaFaring;
	}
	void setSeaFaring(bool set)
	{
		seaFaring = set;
	}
	bool getCommand()
	{
		return command;
	}
	void setCommand(bool set)
	{
		command = set;
	}
	bool getHeavyUnit()
	{
		return heavyUnit;
	}
	void setHeavyUnit(bool set)
	{
		heavyUnit = set;
	}
	bool getHardy()
	{
		return hardy;
	}
	void setHardy(bool set)
	{
		hardy = set;
	}
	bool getVeryHardy()
	{
		return veryHardy;
	}
	void setVeryHardy(bool set)
	{
		veryHardy = set;
	}
	bool getSlave()
	{
		return slave;
	}
	void setSlave(bool set)
	{
		slave = set;
	}
	bool getFrightenFoot()
	{
		return frightenFoot;
	}
	void setFrightenFoot(bool set)
	{
		frightenFoot = set;
	}
	bool getFrightenMounted()
	{
		return frightenMounted;
	}
	void setFrightenMounted(bool set)
	{
		frightenMounted = set;
	}
	bool getFreeUpkeepUnit()
	{
		return freeUpkeepUnit;
	}
	void setFreeUpkeepUnit(bool set)
	{
		freeUpkeepUnit = set;
	}
	bool getGunpowderUnit()
	{
		return gunpowderUnit;
	}
	void setGunpowderUnit(bool set)
	{
		gunpowderUnit = set;
	}
	bool getGunpowderArtillery()
	{
		return gunpowderArtillery;
	}
	void setGunpowderArtillery(bool set)
	{
		gunpowderArtillery = set;
	}
	bool getFireByRank()
	{
		return fireByRank;
	}
	void setFireByRank(bool set)
	{
		fireByRank = set;
	}
	bool getStartNotSkirmishing()
	{
		return startNotSkirmishing;
	}
	void setStartNotSkirmishing(bool set)
	{
		startNotSkirmishing = set;
	}
	bool getMercenaryUnit()
	{
		return mercenaryUnit;
	}
	void setMercenaryUnit(bool set)
	{
		mercenaryUnit = set;
	}
	bool getNoCustom()
	{
		return noCustom;
	}
	void setNoCustom(bool set)
	{
		noCustom = set;
	}
	bool getIsPeasant()
	{
		return isPeasant;
	}
	void setIsPeasant(bool set)
	{
		isPeasant = set;
	}
	bool getGeneralUnit()
	{
		return generalUnit;
	}
	void setGeneralUnit(bool set)
	{
		generalUnit = set;
	}
	bool getGeneralUnitUpgrade()
	{
		return generalUnitUpgrade;
	}
	void setGeneralUnitUpgrade(bool set)
	{
		generalUnitUpgrade = set;
	}
	bool getCantabrianCircle()
	{
		return cantabrianCircle;
	}
	void setCantabrianCircle(bool set)
	{
		cantabrianCircle = set;
	}
	bool getDruid()
	{
		return druid;
	}
	void setDruid(bool set)
	{
		druid = set;
	}
	bool getScreechingWomen()
	{
		return screechingWomen;
	}
	void setScreechingWomen(bool set)
	{
		screechingWomen = set;
	}
	bool getWarCry()
	{
		return warCry;
	}
	void setWarCry(bool set)
	{
		warCry = set;
	}
	bool getStakes()
	{
		return stakes;
	}
	void setStakes(bool set)
	{
		stakes = set;
	}
	bool getCanSkirmish()
	{
		return canSkirmish;
	}
	void setCanSkirmish(bool set)
	{
		canSkirmish = set;
	}
	
};

//unit data
struct unit
{
	DWORD vftable; //0x0000
	struct trackedPointerUnit** trackedUnitPointerP;
	undefined field0_0xC[4];
	spyingInfo spyingInfoUnit;
	void* crusadeFinishedNotification;
	int32_t aiActiveSet; //0x003C
	gfxEntityStruct unitEntity; //0x0040
	gameStdVector<gfxEntityStruct>generalAndOfficerEntities; //0x00F8
	gfxEntityStruct mountEntity; //0x0104
	gfxEntityStruct someOtherEntity; //0x01B8
	void *unitGfxUpdateCallBack; //0x026C
	void *sharedPointerForCallback; //0x0270
	unitVerification unitVerification; //0x0274
	struct unitPositionData* unitPositionData; //0x0280
	struct armyStruct* army;//0x0284
	struct unitGroup *unitGroup; //0x0288
	struct settlementStruct *retrainingSettlement;
	int32_t foodRequirement; //0x0290
	struct eduEntry* eduEntry;
	int ID; //0x0298
	int alliance; //0x029C
	int actionStatus; //0x02A0
	int flankRightState;
	int flankLeftState;
	int flankRearState;
	unit *flankRightThreat;
	unit *flankLeftThreat;
	unit *flankRearThreat;
	unit **nearbyFriendlyUnits;
	int nearbyFriendlyUnitsSize;
	int nearbyFriendlyUnitsNum;
	nearbyEnemyUnit *nearbyEnemyUnits;
	int nearbyEnemyUnitsSize;
	int nearbyEnemyUnitsNum;
	int fatigue; //0x02D4
	float killChanceModifier; //0x02D8
	moraleStruct unitMorale;
	int moraleBonusPermanent;
	int moraleSpecialAbility;
	int canAmok;
	int8_t isAmok;
	int8_t byte_4Dd[3];
	int smthingAmok[4];
	bool isInfighting;
	char byte_4F1[3];
	int infightingCounter;
	char hasActiveEffects;
	char byte_4f9[3];
	int activeEffectsOnThisUnit; //0x04FC
	int expScreen; /* screen experience */
	struct character* general;
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
    int32_t officerID[4];
	unitStats battleStats;
	int32_t generalsBonus;
	unsigned __int32 avgAmmo : 8;
	unsigned __int32 avgWeaponUpg : 5;
	unsigned __int32 avgArmourUpg : 5;
	unsigned __int32 usingPrimary : 1;
	unsigned __int32 usingSecondary : 1;
	struct soldierInBattle** soldiersBattleArr; /* array of soldiers battle data */
	struct soldierData* soldiersArr; /* array of soldiers data */
	void* soldierIndices; //0x05FC
	struct generalInfo* generalArmy; //0x0600
    int32_t numExtras;
    int32_t soldiersFled;
	unitTaskMem unitTaskMem;
	unitTaskInterface unitTaskInterface;
	int taskState;
	int interruptState;
	trackedUnit engagedUnit;
	int8_t interruptPending;
    int8_t byte_1e3D[3];
	int action;
	int32_t meleeStateBitfield; //0x1E44
	int8_t executing;
	int8_t byte_0x1e49[3];
	void* formationsArray; //0x1E4C
	float offsetRowX;
	float offsetRowY;
	float offsetColumnX;
	float offsetColumnY;
	float unitSpacingX;
	float unitSpacingY;
	float rotation;
	float xRadius;
	float yRadius;
	uint16_t soldierCount;
	int16_t frontRankSoldierCount;
	int8_t specialEntries;
	int8_t lastRankSoldierCount;
	int8_t trainingLevel;
	int8_t allowRandom;
	void *auxData;
	int32_t movingThroughCounter;
	int8_t isCloseFormation;
	int8_t byte_1e85[3];
	float avgDisplacement;
	float maxDisplacement;
	int16_t rotDisplacement;
	int16_t pad1e92;
	float formationCentreX;
	float formationCentreY;
    buildingBattle *buildingInside;
	struct unitAiGroupData *unitAiGroupData;
	int32_t engagedSections;
	float extentMaxX;
	float extentMaxZ;
	float extentMaxY;
	float extentMinX;
	float extentMinZ;
	float extentMinY;
	float positionX;
	float positionZ;
	float positionY;
	float lastPositionX;
	float lastPositionZ;
	float lastPositionY;
	int32_t someClock;
	int someTable[40];
	int someTableIndex;
	int ifThisFortyGoNextTable;
	int someCount;
	int tableValuesGetRemovedHere;
	float velocityX;
	float velocityZ;
	float velocityY;
	float velocityXImmediate;
	float velocityZImmediate;
	float velocityYImmediate;
	char pad_1FA4[100];
	char byte_2008;
	char byte_2009;
	char byte_200a;
	char byte_200b;
	int16_t heading;
	int16_t short_200e;
	float radius;
	float radiusAdj;
	float opacity;
	char validDest;
	char byte_201d[3];
	float moveSpeed;
	float velocityMod;
	float animalPosX;
	float animalPosZ;
	float animalPosY;
	float animalRadius;
	void *banner;
	gameStdVector<buildingBattle*> mannedBuildings;
	gameStdVector<void*> mannedBuildingBanners;
	int32_t maxAmmo;
	int32_t currentAmmo;
	unsigned __int32 inPlayableArea : 1;
	unsigned __int32 openBattle : 1;
	unsigned __int32 inGroup : 1;
	unsigned __int32 isHidden : 1;
	unsigned __int32 canBeSeen : 1;
	unsigned __int32 hasBeenSeen : 1;
	unsigned __int32 inFormation : 1;
	unsigned __int32 inStep : 1;
	unsigned __int32 moveFast : 1;
	unsigned __int32 mountsInStep : 1;
	unsigned __int32 ghosted : 1;
	unsigned __int32 reformWhileMoving : 1;
	unsigned __int32 progressiveReshuffle : 1;
	unsigned __int32 doReshuffle : 1;
	unsigned __int32 ambushReveal : 1;
	unsigned __int32 isMercenary : 1;
	unsigned __int32 autoLink : 1;
	unsigned __int32 specialAbilityActive : 1;
	unsigned __int32 stakesUsed : 1;
	unsigned __int32 holyInspired : 1;
	unsigned __int32 dead : 1;
	unsigned __int32 unableToFire : 1;
	unsigned __int32 onLastWaypoint : 1;
	uint32_t abilityTimer;
	int32_t abilityCooldown;
	int32_t abilityChargeUp;
	int8_t nauseaCount;
	int8_t factionID;
	int8_t pad_2076[2];
	struct UNICODE_STRING **alias;
	void *torchAllocator;
	void *currentWaypoint;
	struct crusade *crusade;
	int8_t isOnCrusadeMaybe;
	int8_t markedToKill;
	int8_t taskMelee;
	int8_t taskSkirmish;
	int8_t taskFireAtWill;
	int8_t taskRout;
	int8_t taskBerserk;
	int8_t pad_208f;
	char taskInfighting;
	char pad_2091;
	char pad_2092;
	char pad_2093;
	int nauseaColor;
	float lastIdleNotificationTime;
	char outsidePlayArea;
	char pad_209d;
	char pad_209e;
	char pad_209f;
	int areaEffectPtr;
	int pad_20A4;
	//only unitInfantry has below
	struct siegeEngine** siegeEngines;
	int siegeEnSize;
	int siegeEnNum; /* number of siege engines */
	undefined field24_0x20b4[36];
	struct engineRecord* engineRec;
	undefined field26_0x20dc[4];
public:
	void setUnitParams(int count, int exp, int armor, int weap);
	float getUnitAngle();
	void setUnitAngle(float angle);
	const char* getActionStatus();
	int getUnitFormation()
	{
		return callVFunc<0x1, int>(formationsArray);
	}
	int getExp()
	{
		return expScreen;
	}
	void setExp(const int exp)
	{
		setExperience(exp);
	}
	int getArmourLevel()
	{
		return avgArmourUpg;
	}
	int getWeaponLevel()
	{
		return avgWeaponUpg;
	}
	void setArmour(uint8_t armour);
	void setExperience(uint8_t exp);
	void setWeapon(uint8_t wpn);
	void setarmourLVL(int lvl)
	{
		setUnitParams(SoldierCountStrat, expScreen, lvl, avgWeaponUpg);
	}
	void setweaponLVL(int lvl)
	{
		setUnitParams(SoldierCountStrat, expScreen, avgArmourUpg, lvl);
	}
	void setUnitParamsLua(int exp, int armor, int weap)
	{
		setUnitParams(SoldierCountStrat, exp, armor, weap);
	}
	float getMovepoints()
	{
		return movePoints;
	}
	int getMoraleLevel()
	{
		return unitMorale.moraleLevel;
	}
	void setMoraleLevel(int level)
	{
		unitMorale.moraleLevel = level;
	}
	bool hasBattleProperty(unitBattleProperties prop)
	{
		return (meleeStateBitfield & prop) != 0;
	}
	void setBattleProperty(unitBattleProperties prop, bool value)
	{
		if (value)
		{
			meleeStateBitfield |= prop;
		}
		else
		{
			meleeStateBitfield &= ~prop;
		}
	}
	bool isMovingFastSet()
	{
		return moveFast;
	}
	void setMovingFastSet(bool set)
	{
		if (eduEntry->category == 0 && siegeEnNum > 0
			&& siegeEngines[0]->engineRec->classID != engineType::ladder)
		{
			moveFast = 0;
			return;
		}
		moveFast = set;
	}
	int getSoldierCountStratMap()
	{
		return SoldierCountStrat;
	}
	bool isOnWalls()
	{
		return unitPositionData->isInGateHouseCount + unitPositionData->isInTowerCount + unitPositionData->isOnWallsCount;
	}
	bool isEngaged()
	{
		return unitPositionData->engagedUnitsNum;
	}
	bool isUnderFire()
	{
		return unitPositionData->towersUnderFireFromCount + unitPositionData->unitsUnderFireFromCount
		+ unitPositionData->towersUnderFireFromArtilleryCount + unitPositionData->artilleryUnderFireFromNum;
	}
	int getMountClass()
	{
		return callVFunc<186, int>(this);
	}
	unit* getNearbyUnit(const int index)
	{
		if (index < 0 || index >= nearbyFriendlyUnitsNum)
			return nullptr;
		return nearbyFriendlyUnits[index];
	}
	unit* getNearbyEnemyUnit(const int index)
	{
		if (index < 0 || index >= nearbyEnemyUnitsNum)
			return nullptr;
		return nearbyEnemyUnits[index].unit;
	}
	siegeEngine* getSiegeEngine(const int index)
	{
		const auto category = eduEntry->category;
		if (category != 0 && category != 2)
			return nullptr;
		if (!siegeEngines)
			return nullptr;
		if (index < 0 || index >= siegeEnNum)
			return nullptr;
		return siegeEngines[index];
	}
	void releaseUnit()
	{
		if (!battleHelpers::inBattle())
			return;
		const auto stack = army;
		const auto battleData = battleHelpers::getBattleData();
		for (int i = 0; i < battleData->playerArmyNum; i++)
		{
			const auto playerArmy = battleData->playerArmies[i].army;
			if (playerArmy == stack)
			{
				aiActiveSet = 0;
				return;
			}
		}
		aiActiveSet = 1;
	}
	int getSiegeEngineNum()
	{
		if (eduEntry->category != 0 && eduEntry->category != 2)
			return 0;
		return siegeEnNum;
	}
	bool getInPlayableArea()
	{
		return inPlayableArea;
	}
	void setInPlayableArea(bool set)
	{
		inPlayableArea = set;
	}
	bool getIsHidden()
	{
		return isHidden;
	}
	void setIsHidden(bool set)
	{
		isHidden = set;
	}
	bool getCanBeSeen()
	{
		return canBeSeen;
	}
	void setCanBeSeen(bool set)
	{
		canBeSeen = set;
	}
	bool getHasBeenSeen()
	{
		return hasBeenSeen;
	}
	void setHasBeenSeen(bool set)
	{
		hasBeenSeen = set;
	}
	bool getInFormation()
	{
		return inFormation;
	}
	void setInFormation(bool set)
	{
		inFormation = set;
	}
	bool getIsMercenary()
	{
		return isMercenary;
	}
	void setIsMercenary(bool set)
	{
		isMercenary = set;
	}
	bool getStakesUsed()
	{
		return stakesUsed;
	}
	void setStakesUsed(bool set)
	{
		stakesUsed = set;
	}
	bool getSpecialAbilityActive()
	{
		return specialAbilityActive;
	}
	void setSpecialAbilityActive(bool set)
	{
		specialAbilityActive = set;
	}
	bool getHolyInspired()
	{
		return holyInspired;
	}
	void setHolyInspired(bool set)
	{
		holyInspired = set;
	}
	bool getDead()
	{
		return dead;
	}
	void setDead(bool set)
	{
		dead = set;
	}
	bool getUnableToFire()
	{
		return unableToFire;
	}
	void setUnableToFire(bool set)
	{
		unableToFire = set;
	}
	bool hasAttribute(const char* attributeName)
	{
		return eduEntry->hasAttribute(attributeName);
	}
};

struct unitDb { /* structure with all edu entries */
	UINT32 qq;
	struct eduEntry unitEntries[500];
	UINT32 numberOfEntries;
};

namespace unitHelpers
{
	unitDb* getEdu();
	void initBaseUnitsLookup();
	void setUnitMovePoints(unit* un, float movePoints);
	void setSoldiersCountAndExp(unit* un, int count, int exp);
	void setSoldiersCount(unit* un, int count);
	void killUnit(unit* un);
	int getMaxSoldiersCount(const unit* un);
	groupLabels* getGroupLabels();
	std::string getLocalizedUnitName(const eduEntry* entry);
	std::string getLocalizedUnitDescr(const eduEntry* entry);
	std::string getLocalizedUnitDescrShort(const eduEntry* entry);
	void setUnitName(eduEntry* entry, const std::string& name);
	void setUnitDescr(eduEntry* entry, const std::string& descr);
	void setUnitDescrShort(eduEntry* entry, const std::string& descr);
	float getEngineAngle(const siegeEngine* engine);
	void setEngineAngle(siegeEngine* engine, float angle);
	unitGroup* getEmptyGroup(const armyStruct* army);
	const char* getGroupLabel(const unitGroup* group);
	int16_t angleFloatToShort(float angle);
	float angleShortToFloat(int16_t angle);
	modelDbEntry* findBattleModel(const char* modelName);
	
	unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap);
	unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap, int soldierCount);
	unit* createUnitIdx(int index, int regionID, int facNum, int exp, int arm, int weap);
	unit* createUnitIdx2(int index, int regionId, int facNum, int exp, uint8_t arm, uint8_t weapon, int soldiers);
	unit* createUnitEDB(int edb, int regionID, int facNum, int exp, int arm, int weap);
	int getEduIndex(const char* type);
	eduEntry* getEduEntryByName(const char* type);
	eduEntry* getEDUEntryById(int id);
	
	unitGroup* defineUnitGroup(const armyStruct* army, const char* label, unit* un);
	void addUnitToGroup(unitGroup* group, unit* un);
	unitGroup* getGroupByLabel(const char* label);
	void placeGroup(unitGroup* group, float xCoord, float yCoord, float angle);
	void undefineUnitGroup(const unitGroup* group);
	void removeUnitFromGroup(unitGroup* group, unit* un);
	void automateGroup(const unitGroup* group, bool automate);
	void automateAttack(unitGroup* group, unit* targetUnit);
	void automateDefense(unitGroup* group, float xCoord, float yCoord, float radius);
    
};

namespace unitActions
{
    void unitMovetoPosition(unit* unit, float xCoord, float yCoord, bool run);
    void placeUnit(unit* unit, float xCoord, float yCoord, int16_t angle, int width);
    unit* getUnitByLabel(const char* label);
    void unitAttackClosest(unit* un, int16_t angle, bool run);
    void attackUnit(unit* un, const unit* targetUnit, bool run);
    void deployStakes(const unit* un);
    void changeUnitFormation(const unit* un, int formationType);
    void haltUnit(const unit* un);
    void moveToOrientation(unit* un, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run);
    void moveRelative(unit* un, float xCoord, float yCoord, bool run);
    void moveToMissileRange(unit* un, const unit* targetUnit, bool run);
    void unitTurn(unit* un, int16_t angle, bool isRelative);
    void taunt(const unit* un);
    void useSpecialAbility(const unit* un, bool heroOnly);
    int getSiegeEngineType(const unit* un);
    void attackBuilding(unit* un, buildingBattle* building);
    void collectEngine(unit* un, siegeEngine* engine);

    void changeGroupUnitFormation(const unitGroup* group, int formationType);
    void moveToRangeOfGroup(const unitGroup* group, const unitGroup* targetGroup, bool run);
    void moveGroupToRangeOfUnit(const unitGroup* group, const unit* targetUnit, bool run);
    void groupAttackGroup(const unitGroup* group, const unitGroup* targetGroup, bool run);
    void groupHalt(const unitGroup* group);
    void groupMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run);
    void groupMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run);
    void groupMoveFormedRelative(const unitGroup* group, float xCoord, float yCoord, bool run);
    void groupMoveUnformedRelative(const unitGroup* group, float xCoord, float yCoord, bool run);
    void groupTurn(const unitGroup* group, int16_t angle, bool isRelative);
}

