#pragma once
#include <lua/sol.hpp>

#include "realGameTypes.h"
#define generalStruct_abilityID 1

struct portBuildingStruct;
struct factionStruct;
struct fortStruct;
struct building;
struct characterMoveData;
struct characterMovementExtents;

struct genMod {
	int type; /* 0-spy, 2 - diplomat, etc */
	struct descrCharacterStratModelArray* stratInfo;
	int modelCountArrayMax;
	int modelCount;
	char* card;
	int cardHash;
	char* portrait;
	int portraitHash;
	char* rebelCard;
	int rebelCardHash;
	char* rebelPortrait;
	int rebelPortraitHash;
	struct eduOfficer* battleMod;
	bool actions[34];
	char pad[2];
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

struct movePath
{
public:
	struct coordPair* tileCoords; //0x0000
	int32_t tileCoordsSize; //0x0004
	int32_t tileCoordsCount; //0x0008
}; //Size: 0x000C

struct character
{ 
	DWORD* vtbl;
	void* obj;
	void* prev;
	int xCoord;
	int yCoord;
	int fade;
	bool render;
	float opacity;
	bool highlighted;
	char pad_0x1D[3];
	trackedObject trackedObject;
	spyingInfo spyInfo;
	void* crusadeFinishListener;
	int fieldx60;
	int fieldx64;
	int fieldx68;
	int fieldx6C;
	int fieldx70;
	int fieldx74;
	int fieldx78;
	int fieldx7C;
	struct characterRecord* characterRecord; /* many important info about character */
	factionStruct** faction;
	struct genMod* genType;
	undefined field9_0x8c[24];
	bool ifMarkedToKill;
	bool deathAction;
	bool inEnemyZOC; //0x00A6
	bool inEnemyTerritory;
	int32_t rally;
	float reinforcementTime;
	int16_t sedentaryTurns;
	bool isPlagued;
	int8_t padb3;
	int32_t plaguedDuration;
	int actionType;
	int32_t actionsThisTurn; //0x00BC
	int32_t passiveState;
	bool unusedMovePoints;
	bool infiniteMovePoints;
	int8_t multiTurnMoveThisTurn;
	int8_t padc7;
	float movePointsCharacter;
    float totalMpCost;
	uchar isStopCharacterNeeded; /* set to 1 if character moving now and he stops */
    int8_t padD1[3];
	int32_t numTurnsIdle; //0x00D4
	int32_t regionID; //0x00D8
	float percentCharacterReligionInRegion; //0x00DC
	float popConvertedThisTurn; //0x00E0
	int32_t timeInRegion; //0x00E4
	int32_t timeWithArmy;
	void *cadClass;
	struct settlementStruct *residence;
	portBuildingStruct* dockedPort;
	portBuildingStruct* blockadedPort;
	DWORD armyLeadedVtbl;
	struct armyStruct* armyLeaded; /* army of the general */
	DWORD bodyguardsVtbl;
	struct unit* bodyguards; /* unit of general */
	struct armyStruct* armyNotLeaded; /* army, if not leader */
	character *surrenderToCharacter;
	character *surrenderTile;
	factionStruct *captor;
	int8_t isHostage;
	int8_t turnExecuting;
	int16_t additionalRot;
	int timer;
	char skeletonState[0xB0];
	float stratPosX;
	float stratPosZ;
	float stratPosY;
	int16_t angle;
	int8_t pushBackToNavy;
	int8_t pad1e3;
	float movePointsModifier;
	float movePointsMax;
	float movePointsArmy;
	float movePointsMaxArmy; //0x01F0
	struct crusade* crusade; //0x01F4
	int32_t turnJoinedCrusade; //0x01F8
	int32_t currentTurn; //0x01FC
	float distanceToCrusadeTarget[10];
	int32_t crusadeNoProgressTurns;
	int8_t noCrusadeProgressThisTurn;
	int8_t isInCrusadeRegion;
	char pad_0xx[2];
	char* ability;
	int32_t abilityHash;
	struct character* thisCharacter; //0x0238
	char pad_023C[16]; //0x023C
	struct character* thisCharacter2; //0x024C
	int32_t xCoordLast; //0x0250
	int32_t yCoordLast; //0x0254
	int32_t xCoordCurrent; //0x0258
	int32_t yCoordCurrent; //0x025C
	struct settlementStruct* besiegingSettlement; //0x0260
	char pad_0264[12]; //0x0264
	struct character* besiegingCharacter; //0x0270
	int32_t N00032B12; //0x0274
	int32_t N00032B00; //0x0278
	int32_t N00032B13; //0x027C
	int32_t N00032B14; //0x0280
	char pad_0284[296]; //0x0284
	void* tilePath; //0x03AC
	int32_t tilePathSize; //0x03B0
	int32_t tilePathCount; //0x03B4
	struct movePath* multiTurnMovePaths; //0x03B8
	int8_t multiTurnMovePathsSize; //0x03BC
	int8_t multiTurnMovePathsCount; //0x03BD
	int8_t padEnd[2]; //0x03BE
public:
	//methods
	factionStruct* getFaction()
	{
		return *faction;
	}
	settlementStruct* getSettlement();
	fortStruct* getFort();
	int getTypeID();
	bool hasFreeTilesToMove();
	void setTypeID(int charType);
	characterMovementExtents* getMoveExtents(int searchType, int numTurns);
	std::shared_ptr<characterMoveData> createMoveData(int searchType, int numTurns);
};

namespace characterHelpers
{
	void sendOffMap(character* gen);
	character* createCharacterWithoutSpawning(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y);
	character* createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y);
	void diplomacyFort(character* gen, fortStruct* targetFort);
	void bribeFort(character* gen, fortStruct* targetFort);
	void spyFort(character* gen, fortStruct* targetFort);
	void bribeSettlement(character* gen, settlementStruct* targetSettlement);
	void spySettlement(character* gen, settlementStruct* targetSettlement);
	void sabotageSettlement(character* gen, settlementStruct* targetSettlement);
	void siegeSettlement(character* gen, settlementStruct* sett, bool isAttack);
	void siegeFort(character* gen, fortStruct* fort, bool isAttack);
	void moveNormal(character* gen, int x,int y);
	void attackCharacter(character* attacker, character* defender);
	void diplomacyCharacter(character* gen, character* targetCharacter);
	void diplomacySettlement(character* gen, settlementStruct* targetSettlement);
	void targetCharacterAction(character* gen, character* targetCharacter, characterAction type);
	void targetFortAction(character* gen, fortStruct* targetFort, characterAction type);
	void targetSettlementAction(character* gen, settlementStruct* targetSettlement, characterAction type);
	void assassinate(character* gen, character* targetCharacter);
	void marry(character* gen, character* targetCharacter);
	void spyCharacter(character* gen, character* targetCharacter);
	void denounce(character* gen, character* targetCharacter);
	void bribe(character* gen, character* targetCharacter);
	void acquire(character* gen, character* targetCharacter);
	void sabotageBuilding(character* gen, building* build);
	const char* getTypeName(const character* gen);
	void setMovePoints(character* gen, float movePoints);
	float getMovePoints(const character* gen);
	int getTypeID(const character* gen);
	void setTypeID(character* gen, int typeID);
	void teleportCharacter(character* gen, int x, int y);
	bool teleportCharacterClose(character* gen, int x, int y);
	void killCharacter(character* gen);
	fortStruct* createFort(character* gen);
	void setCharacterType(character* character, int typeID, int subFaction, int factionDipNum);
	void setBodyguard(character* gen, unit* un);
	std::string getStringPropertyGen(const character* gen);
	void setStringPropertyGen(character* gen, const std::string& newS);
	void addToLua(sol::state& luaState);
}
