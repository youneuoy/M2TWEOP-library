#pragma once
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include"headersSTD.h"
#include "headersMEM.h"
#include "realGameTypes.h"
#include "techFuncs.h"
#include "exportHeader.h"
#include "globals.h"

namespace fastFuncts
{
	//set new crypted string
	//alloc memory, copy string to it
	//copy crypt to next 4 bytes after pointer
	NOINLINE EOP_EXPORT void setCryptedString(char* targetS, const char* newS);

	//count of factions in game
	NOINLINE EOP_EXPORT UINT32 getFactionsCount();
	//array of pointers to factions
	NOINLINE EOP_EXPORT factionStruct** getFactionsList();

	//number of passed turns
	NOINLINE EOP_EXPORT UINT32 getPassedTurnsNum();



	//find fort on coords
	NOINLINE EOP_EXPORT fortStruct* findFort(int x, int y);
	//find port on coords
	NOINLINE EOP_EXPORT portBuildingStruct* findPort(int x, int y);
	//find resource on coords
	NOINLINE EOP_EXPORT resStrat* findResource(int x, int y);

	//move stratmap camera slow
	NOINLINE EOP_EXPORT void moveStratCameraSlow(int x, int y);
	//move stratmap camera fast
	NOINLINE EOP_EXPORT void moveStratCameraFast(int x, int y);
	//zoom stratmap camera
	NOINLINE EOP_EXPORT void zoomStratCamera(float zoom);


	//teleport character
	NOINLINE EOP_EXPORT void  teleportCharacter(general* gen,int x, int y);

	//add anchillary to character
	NOINLINE EOP_EXPORT int addAnchillary(generalCharacterictics* character, anchillary* anch);
	//remove anchillary from character
	NOINLINE EOP_EXPORT void removeAnchillary(generalCharacterictics* character, anchillary* anch);
	//find anchillary in anch list
	NOINLINE EOP_EXPORT anchillary* findAnchillary(char* anchName);

	//edit unit characteristics
	NOINLINE EOP_EXPORT void setUnitParams(unit* un, int count, int exp, int armor, int weap);
	NOINLINE EOP_EXPORT void setSoldiersCount(unit* un,int count);

	//kills
	NOINLINE EOP_EXPORT void killUnit(unit* un);
	NOINLINE EOP_EXPORT void killCharacter(general* gen);


	NOINLINE EOP_EXPORT general* createCharacter(char*type,factionStruct*fac,int age,char*name,char*name2,int subFaction,char*portrait,int x,int y);

	NOINLINE EOP_EXPORT stackStruct* createArmy(general*character);


	//facNum - dipnum
	NOINLINE EOP_EXPORT unit* createUnitN(const char* type, int facNum, int exp, int arm, int weap);
	NOINLINE EOP_EXPORT unit* createUnitIdx(int index, int facNum, int exp, int arm, int weap);

	NOINLINE EOP_EXPORT int addUnitToArmy(stackStruct* army, unit* un);
	NOINLINE EOP_EXPORT void setBodyguard(general*gen,unit* un);

	//allocate mem for game class/etc
	NOINLINE EOP_EXPORT DWORD allocateGameMem(size_t amount);
};
