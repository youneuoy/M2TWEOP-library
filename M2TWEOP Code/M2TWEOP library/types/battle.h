#pragma once
#include "realGameTypes.h"
#include "lua/sol.hpp"

struct streetPosition;
struct towerStats;

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

struct battleGroup
{
	int index;
	struct unit **units;
	int unitsSize;
	int unitCount;
	bool infoValid;
	char field_11[3];
	float midPointX;
	float midPointY;
	float prevMidPointX;
	float prevMidPointY;
	int radius;
	int16_t facing;
	int16_t facpad;
	struct floatPosData floatPosDataStart;
	float velocityX;
	float velocityY;
	int8_t gap6C_positionFloats[172];
	char field_118;
	char field_119;
	char field_11a;
	char field_11b;
	int16_t someAngle;
	int16_t pad11e;
	float speed;
	bool onHill;
	bool allInSameArea;
	char field_126[2];
	int positionID;
	int cavalryRatio;
	int missileRatio;
	int routingRatio;
	int forestRatio;
	int soldierCounts;
	int cavalryRatioVisible;
	int missileRatioVisible;
	int routingRatioVisible;
	int forestRatioVisible;
	int soldierCountsVisible;
	int unitNumVisible;
};

struct arrayInUnitGroup
{
	int order;
	float xCoord;
	float yCoord;
	int16_t angle;
	bool run;
	int8_t byte_e;
	int movementType;
	bool changeWidth;
	bool changeFormation;
	bool changeAngle;
	int8_t byte_17;
	int pathHandle;
	void* startWaypoint;
	void* endWaypoint;
	int8_t isAiAutomated;
	int8_t byte_25[3];
	int isAiFormation;
	int positionType;
	int platform;
	float pointX;
	float pointZ;
	float pointY;
	int groupFormationID;
	int attackType;
};

struct unitGroup
{
	void *vftable /*VFT*/;
	bool useGroupFormation;
	char field_5[3];
	struct unit **unitsInFormation;
	int32_t unitsSize;
	int32_t unitsInFormationNum;
	int unitsNumTotal;
	void *blockPriorities;
	int32_t blockPrioritiesSize;
	int32_t blockPrioritiesNum;
	bool *assignedUnits;
	int32_t assignedUnitsSize;
	int32_t assignedUnitsNum;
	struct smthingUnitGroup *formationBlocks;
	int32_t formationBlocksSize;
	int32_t formationBlocksNum;
	floatPosData *boundingBoxes;
	int32_t boundingBoxesSize;
	int32_t boundingBoxesNum;
	void *descrFormationEntry;
	char hasFormation;
	char field_4d[3];
	int formationState;
	float width;
	float height;
	float distFrontToCentre;
	float scaleFactorX;
	float scaleFactorY;
	float xCoord;
	float yCoord;
	float midPointX;
	float midPointY;
	int16_t angle;
	int16_t field_7a;
	float slowestSpeed;
	float quickestSpeed;
	char canRun;
	char field_85[3];
	int pendingFormation;
	int alliance;
	int status;
	struct arrayInUnitGroup arrayInUnitGroupThing[8];
	int groupOrdersStart;
	int groupOrdersEnd;
	int groupOrdersIndex;
	unit **unitsNotInFormation;
	int unitsNotInFormationSize;
	int unitsNotInFormationNum;
	float pathWidth;
	unitGroup *thisGroup;
	void *groupAutomationTasks;
	int taskNum;
	char gap2FC[4096];
	char moveFast;
	char orderStarted;
	char orderCleared;
	char field_12FF;
	char automationEnabled;
	char field_1301[3];
	int automationType;
	float defendXCoord;
	float defendYCoord;
	float defendRadius;
	void* targetWorldData;
	float targetX;
	float targetZ;
	float targetY;
	unit* targetUnit;
	int newAutomationType;
	float newDefendXCoord;
	float newDefendYCoord;
	float newDefendRadius;
	void* newField_targetWorldData;
	float newField_targetX;
	float newField_targetZ;
	float newField_targetY;
	unit *newTargetUnit;
	struct battleResidence *targetSettlement;
	void *targetCrossing;
	int defensiveArea;
	struct terrainFeatureHill *targetHill;
public:
	unit* getUnitInFormation(const int index)
	{
		if (index < 0 || index >= unitsInFormationNum)
			return nullptr;
		return unitsInFormation[index];
	}
	unit* getUnitNotInFormation(const int index)
	{
		if (index < 0 || index >= unitsNotInFormationNum)
			return nullptr;
		return unitsNotInFormation[index];
	}
};
struct aiUnitGroup : unitGroup
{
	void *detachment;
	struct aiDetachmentTactic *detachmentTactic;
	unit **unitsInMelee;
	int unitsInMeleeSize;
	int unitsInMeleeNum;
	battleGroup battleAiUnitGroup;
	char gap14c8[92];
	float centreFrontX;
	float centreFrontY;
	float centreX;
	float centreY;
	__int16 avgFacing;
	__int16 field_1536;
	int percentFormed;
public:
	
};
struct aiTacticAssault
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
	char pad[2];
	unit **units;
	int unitsSize;
	int unitNum;
	void* fsm;
	DWORD fsmState;
	struct aiDetachment *detachment;
	struct aiUnitGroup siegeUnitGroup;
	struct aiUnitGroup aiUnitGroup;
	bool started;
	char field_2ACd[3];
	struct buildingBattle *building;
	struct unit *attackUnit;
	int formationIndex;
	float advanceX;
	float advanceY;
	int16_t angle;
	int16_t angle2;
	int advanceDirectionX;
	float advanceDirectionY;
	char firstAdvanceOrders;
	char docking;
	char engineLeft;
	char finished;
	float priority;
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
	gameStdVector<towerStats> stats;
	struct gateEntry **gateEntries;
	int32_t gateEntrySize;
	int32_t gateEntryCount;
	int32_t projectileImpactsAllHitGate;
};

/* 1874 */
struct towerEntry : wallEntry
{
	gameStdVector<towerStats> stats;
public:
	int getTowerStatCount()
	{
		return stats.size();
	}
	towerStats* getTowerStat(const int index)
	{
		if (index < 0 || index >= stats.size())
			return nullptr;
		return &stats[index];
	}
};

/* 1875 */
struct gateHouseEntry : wallEntry
{
	gameStdVector<towerStats> stats;
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

struct fireRate
{
	int normal;
	int flaming;
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
	int getFireRateFlaming();
	int getFireRate();
	void setFireRate(int rate);
	void setFireRateFlaming(int rate);
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

struct perimeterBuildings
{
public:
	struct buildingBattle** buildingList; //0x0000
	int32_t buildingListSize; //0x0004
	int32_t buildingNum; //0x0008
public:
	buildingBattle* getBuilding(const int index)
	{
		if (index < 0 || index >= buildingNum)
			return nullptr;
		return buildingList[index];
	}
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
public:
	perimeterBuildings* getPerimeter(const int index)
	{
		if (index < 0 || index >= perimetersNum)
			return nullptr;
		return &perimeters[index];
	}
	buildingBattle* getBattleBuilding(const int index)
	{
		if (index < 0 || index >= allBuildingsNum)
			return nullptr;
		return allBuildings[index];
	}
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

struct streetPosition
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
	gameStdVector<streetPosition> streetDeploymentPositions;
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


struct diedGeneral
{
	UNICODE_STRING** localizedName;
	int ID;
};

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
public:
	int getGroundType()
	{
		return physicalGroundType;
	}
	float getGroundHeight();
	float getWaterHeight();
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

struct lineData
{
	float xCoordStart;
	float yCoordStart;
	float xCoordEnd;
	float yCoordEnd;
	float angle;
	float length;
	float xMidPoint;
	float yMidPoint;
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
	int getSoldierCount(const int alliance)
	{
		if (alliance < 0 || alliance >= 8)
			return 0;
		return allianceSoldiers[alliance];
	}
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
	uint32_t soldiersRouted; //0x0020
	int32_t soldiersHealed; //0x0024
	int32_t soldiersRallied; //0x0028
	int32_t soldiersEnd; //0x002C
	int32_t friendlyFireCasualties; //0x0030
	int32_t expStart; //0x0034
	int32_t expGained; //0x0038
	int8_t isGeneral; //0x003C
	int8_t hasWithdrawn; //0x003D
	uint16_t pad_3e; //0x003E
	int32_t shipsSunk; //0x0040
	int32_t shipDamage; //0x0044
}; //Size: 0x0048


struct battleArmy { /* in battle leader and leader army */
	struct armyStruct* army;//0x0000
	struct character* character;//0x0004
	int32_t generalNumKillsBattle; //0x0008
	float generalHPRatioLost; //0x000C
	float initialStrength; //0x0010
	float battleOdds; //0x0014
	struct diedGeneral* diedGenerals; //0x0018
	int32_t diedGeneralsSize; //0x001C
	int32_t diedGeneralsNum; //0x0020
	struct diedGeneral* killedGenerals; //0x0024
	int32_t killedGeneralsSize; //0x0028
	int32_t numKilledGenerals; //0x002C
	struct diedGeneral* gotCapturedGenerals; //0x0030
	int32_t gotCapturedSize; //0x0034
	int32_t gotCapturedNum; //0x0038
	struct diedGeneral* capturedGenerals; //0x003C
	int32_t capturedGeneralsSize; //0x0040
	int32_t capturedGeneralsNum; //0x0044
	struct diedGeneral* diedCharacters; //0x0048
	int32_t diedCharactersSize; //0x004C
	int32_t diedCharactersNum; //0x0050
	struct diedGeneral* killedCharacters; //0x0054
	int32_t killedCharactersSize; //0x0058
	int32_t killedCharactersNum; //0x005C
	struct diedGeneral* generalsGotCaptured; //0x0060 
	int32_t generalsGotCapturedSize;  //0x0064
	int32_t generalsGotCapturedCount; //0x0068
	struct diedGeneral* generalsCaptured; //0x006C
	int32_t generalsCapturedSize; //0x0070
	int32_t generalsCapturedCount; //0x0074
	int friendlyStandardsCaptured; //0x0078
	int enemyStandardsCaptured; //0x007C
	bool friendlyCampDestroyed; //0x0080
	bool enemyCampDestroyed; //0x0081
	bool friendlyCampCaptured; //0x0082
	bool enemyCampCaptured; //0x0083
	bool baggageTrainDestroyed; //0x0084
	bool enemyBaggageTrainDestroyed; //0x0085
	bool baggageTrainCaptured; //0x0086
	bool enemyBaggageTrainCaptured; //0x0087
	int soldiersMiscKilled; //0x0088
	int enemySoldiersMiscKilled; //0x008C
	int buildingsDestroyed; //0x0090
	int buildingsCaptured; //0x0094
	int loot; //0x0098
	struct battleUnit* units; //0x009C
	int32_t unitArraySize; //0x00A0
	int32_t unitCount; //0x00A4
	bool transportArmyLost; //0x00A8
	char pad_00A9[3]; //0x00A9
	UNICODE_STRING** transportLostName; //0x00AC
	int transportLostID; //0x00B0
	int giveUpTick; //0x00B4
	int resignTick; //0x00B8
public:
	battleUnit* getBattleUnit(const int index)
	{
		return &units[index];
	}
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
public:
	battlePos* getCoordPair(const int index)
	{
		if (index < 0 || index >= coordsNum)
			return nullptr;
		return &coordsPairs[index];
	}
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
	int getDeployAreaCount()
	{
		return deploymentAreas.size();
	}
	deploymentAreaS* getDeployArea(const int index)
	{
		if (index < 0 || index >= deploymentAreas.size())
			return nullptr;
		return &deploymentAreas.get(index)->area;
	}
};

struct aiDetachmentTactic
{
	DWORD *vftable /*VFT*/;
	struct aiDetachment *aiDetachment;
	bool hasBeenConsidered;
	bool isExecuting;
	bool hasExecuted;
	char field_B;
	int phaseStarted;
	int arrayOfTenRandomValues[10];
	bool startedTactic;
	bool differentUnitsThisUpdate;
	unit **units;
	int unitsSize;
	int unitNum;
};


struct aiDetachmentUnit
{
public:
	struct unit* unit; //0x0000
	DWORD objective; //0x0004
	bool isAssigned;
	char pad_0005[3]; //0x0005
}; //Size: 0x000C


struct aiDetachment
{
public:
	DWORD* vfTable; //0x0000
	struct aiBattleObjective* aiObjective; //0x0004
	struct aiDetachmentUnit* aiDetachUnits; //0x0008
	int32_t aiDetachUnitsSize; //0x000C
	int32_t aiDetachUnitsCount; //0x0010
	int phase;
	int nextPhase;
	aiDetachmentTactic* tactics; //0x001C
	int32_t tacticsSize; //0x0020
	int32_t tacticsCount; //0x0024
	aiDetachmentTactic* surplusTactic;
	aiDetachmentTactic* mainTactic;
}; //Size: 0x0028

struct aiSiegeEquipment
{
	struct armyStruct* army;
	int engineType;
	aiBattleObjective* objective;
	bool available;
	char pad[3];
};

struct aiBattleObjective
{
public:
	DWORD vfTable;
	struct battleAI* battleAi;
	struct aiDetachment** aiDetachments; //0x0008
	int32_t aiDetachmentsSize; //0x000C
	int32_t aiDetachmentsCount; //0x0010
	struct unit** aiUnits; //0x0014
	int32_t aiUnitsSize; //0x0018
	int32_t aiUnitsCount; //0x001C
	aiSiegeEquipment** siegeEquipment; //0x0020
	int32_t siegeEquipmentSize; //0x0024
	int32_t siegeEquipmentCount; //0x0028
	int32_t priority; //0x002C
	int objectiveId; //0x0030
	bool isRequired;
	char pad_0035[3]; //0x0035
	int ticks;
	bool isPlayer;
	char pad_003D[3]; //0x003D
	armyStruct* army;
public:
	unit* getUnit(const int index)
	{
		if (!aiUnits)
			return nullptr;
		if ( index < 0 || index >= aiUnitsCount)
			return nullptr;
		return aiUnits[index];
	}
	int getType()
	{
		return callVFunc<7, int>(this);
	}
};

struct battleGroupController
{
	battleGroup **battleGroups;
	int battleGroupsSize;
	int battleGroupsNum;
	unit **units;
	int unitsSize;
	int unitsNum;
	battleGroup masterGroup;
};

struct aiAnalyzerObjectives
{
	void **objectives;
	int32_t objectivesSize;
	int32_t objectivesNum;
};

struct discardedAnalyzerObjectives
{
	void **discardedObjectives;
	int32_t discardedObjectivesSize;
	int32_t discardedObjectivesNum;
};

struct analyzerUnitsArray
{
	void **analyzerUnits;
	int32_t analyzerUnitsSize;
	int32_t analyzerUnitsNum;
};

struct battleStrengthStruct
{
	int soldierCount;
	int meleeStrength;
	int missileStrength;
	int siegeStrength;
	int infantryMissilePlusMelee;
	int cavalryMeleeStrength;
	int total;
};

struct battleAI
{
public:
	struct battleSide* battleSide;
	bool scriptPlan; //0x0004
	char pad_0005[3]; //0x0005
	uint32_t currentAiPlan; //0x0008
    uint32_t previousAiPlan; //0x000C
	struct battleAI *thisAi;
    int aiAnalyzerPlan;
	int planPriority;
	battleGroupController aiActiveSetGroup;
	battleGroupController visibleEnemyGroup;
	battleGroupController notActiveSetGroup;
	int targetGroupIndex;
	unit **enemyUnits;
	int32_t enemyUnitsSize;
	int32_t enemyUnitsNum;
	battleGroup friendlyUnitsGroup;
	battleGroup enemyUnitsGroup;
	gameList<aiSiegeEquipment> siegeEquipment;
	struct unit **units;
	int32_t unitArraySize;
	int32_t unitCount;
	aiSiegeEquipment** siegeEquipmentArray;
	int32_t siegeEquipmentArraySize;
	int32_t siegeEquipmentArrayCount;
	aiBattleObjective** objectives;
	int32_t objectivesSize;
	int32_t objectivesCount;
	int objectiveTimer;
	aiBattleObjective** playerObjectives;
	int32_t playerObjectivesSize;
	int32_t playerObjectivesCount;
	struct battleSide* side2;
	aiAnalyzerObjectives aiAnalyzerObjectives;
	discardedAnalyzerObjectives discardedAnalyzerObjectives;
	analyzerUnitsArray analyzerUnitsArray;
	void *analyzerUnitsDb;
	void *aiAnalysers;
	int32_t aiAnalysersSize;
	int32_t aiAnalysersNum;
	int unallocatedUnitsAnalyzer;
	int updateTimer3;
	int noUsefulAttackNum;
	int array_07b8RandomValues;
	int array_07b8Size;
	int array_07b8Num;
	int updateTimer;
	int planStartTime;
	bool isDefender;
	bool isAmbush;
	bool isDeploying;
	bool hasDeployed;
	bool wantsToSally;
	int8_t byte_07D1[3];
	int battleDifficulty;
	bool defendAreaSet;
	int8_t byte_07D9[3];
	float defendAreaX;
	float defendAreaZ;
	float defendAreaY;
	float defendRadius;
	lineData defendLineData;
	battleStrengthStruct alliedInitialTotal;
	battleStrengthStruct alliedTotal;
	battleStrengthStruct enemyInitialTotal;
	battleStrengthStruct enemyTotal;
	battleStrengthStruct enemyInitialVisible;
	battleStrengthStruct enemyVisible;
	terrainFeatureHill *defendHill;
	int bestCrossingDefendArea;
	int field_08bC;
	int field_08c0;
public:
	aiBattleObjective* getPlayerObjective(const int index)
	{
		if (!playerObjectives)
			return nullptr;
		if ( index < 0 || index >= playerObjectivesCount)
			return nullptr;
		return playerObjectives[index];
	}
	aiBattleObjective* getAiObjective(const int index)
	{
		if (!objectives)
			return nullptr;
		if ( index < 0 || index >= objectivesCount)
			return nullptr;
		return objectives[index];
	}
	aiBattleObjective* getObjective(const int index)
	{
		if (playerObjectivesCount > 0)
			return getPlayerObjective(index);
		return getAiObjective(index);
	}
	int getObjectiveCount()
	{
		if (playerObjectivesCount > 0)
			return playerObjectivesCount;
		return objectivesCount;
	}
	unit* getVisibleEnemy(const int index)
	{
		if (index < 0 || index >= enemyUnitsNum)
			return nullptr;
		return enemyUnits[index];
	}
	unit* getUnit(const int index)
	{
		if (index < 0 || index >= unitCount)
			return nullptr;
		return units[index];
	}
	void setPlan(const int plan)
	{
		previousAiPlan = currentAiPlan;
		scriptPlan = true;
		currentAiPlan = plan;
	}
	int getPlan()
	{
		return currentAiPlan;
	}
}; //Size: 0x0804

struct reinforcingArmy
{
	armyStruct *army;
	armyStruct *commandingArmy;
	float arrivalTime;
};

struct battleSide {
	bool isDefender;//0x0000
	bool canWithdraw;//0x0001
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
public:
	battleArmy* getBattleArmy(const int index)
	{
		if (index < 0 || index >= armiesNum)
			return nullptr;
		return &forces[index];
	}
	armyStruct* getReinforcementArmy(const int index)
	{
		if (index < 0 || index >= reinforceArmyCount)
			return nullptr;
		return reinforcementArmies[index].army;
	}
	factionStruct* getFaction(const int index)
	{
		if (index < 0 || index >= factionCount)
			return nullptr;
		return factions[index];
	}
	bool hasFaction(const int factionID)
	{
		return factionBitMap & (1 << factionID);
	}
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
public:
	int getLineNum()
	{
		return lines.size();
	}
	int getHillNum()
	{
		return hills.size();
	}
	terrainFeatureHill* getHill(const int index)
	{
		if (index < 0 || index >= hills.size())
			return nullptr;
		return hills.get(index);
	}
	terrainLineSegment* getLine(const int index)
	{
		if (index < 0 || index >= lines.size())
			return nullptr;
		return lines.get(index);
	}
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
public:
	armyStruct* getPlayerArmy(const int index)
	{
		return playerArmies[index].army;
	}
};//Size: 0xCA7C

struct battleCameraStruct
{
public:
	float xCoord; //0x0000
	char pad_0004[4]; //0x0004
	float yCoord; //0x0008
	char pad_000C[20]; //0x000C
	float zCoord; //0x0020
}; //Size: 0x0024

struct battlefieldEngines
{
public:
	struct siegeEngine **engines; //0x0000
	int32_t enginesSize; //0x0004
	int32_t enginesNum; //0x0008
	int32_t enginesNum2; //0x000C
	int32_t enginesNum3; //0x0010
public:
	siegeEngine* getSiegeEngine(const int index)
	{
		if (index < 0 || index >= enginesNum)
			return nullptr;
		return engines[index];
	}
}; //Size: 0x0014

namespace battleHelpers
{
	int getBattleCondCode(DWORD condObject);
	std::string getWinConditionS(DWORD condObject);
	battleDataS* getBattleData();
	battleTerrainData* getBattleTerrainData();
	battlePerimeters* getBattlePerimeters();
	battleCameraStruct* getBattleCamCoords();
	battleTile* getBattleTile(float xCoord, float yCoord);
	battleResidence* getBattleResidence();
	int getBattleTileIndex(float xCoord, float yCoord);
	battlefieldEngines* getBattlefieldEngines();
	streetPosition* getStreetPosition(int index);
	bool isZoneValid(int zoneID);
	int getZonePerimeter(float x, float y);
	int getZoneID(float x, float y);
	float getBattleMapHeight(float xCoord, float yCoord);
	bool inBattle();
	void autoResolve();
	bool autoWin(const char* winnerSide);
    void addToLua(sol::state& luaState);
};
