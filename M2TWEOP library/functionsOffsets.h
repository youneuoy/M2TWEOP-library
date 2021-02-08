#pragma once
#include"headersSTD.h"

class codes
{
public:
	static struct offsetsT
	{
		DWORD stringCryptFunc = NULL;
		DWORD loadStratCasModelFunc = NULL;

		DWORD allocMemFunc = NULL;



		DWORD createSiegeSettlementGarrisonedFunc = NULL;
		DWORD getReadySiegeCharacterGarrisonedFunc = NULL;
		DWORD finalyzeActionStratmapFunc = NULL;


		DWORD getReadyForMovingFunc = NULL;
		DWORD getReadyForMoving2Func = NULL;
		DWORD getGeneralReadyForMovingFunc = NULL;



		//strat camera funcs
		//params - x,y at stack
		//restore stack after call it
		DWORD moveStratCameraSlowFunc = NULL;
		DWORD moveStratCameraFastFunc = NULL;
		DWORD zoomStratCameraFunc = NULL;

		//ecx - general*
		//stack 8 - y
		//stack 4 - x
		DWORD teleportCharacterFunc = NULL;


		//@replenishUnit(unit *un,int howManyAdd,int exp)
		DWORD replenishUnitFunc = NULL;
		DWORD setUnitArmorFunc = NULL;
		DWORD setUnitWeapFunc = NULL;

		//create character
		DWORD createCharacterFunc = NULL;
		DWORD spawnCreatedCharacterFunc = NULL;
		DWORD doSomeWithCharacterFunc = NULL;

		//create army
		DWORD createArmyFunc = NULL;
		DWORD createUnitFunc = NULL;
		DWORD addUnitToArmyFunc = NULL;

		DWORD setBodyguard = NULL;

		//kill character on stratmap
		DWORD killCharStratMapFunc;
		//kill unit on stratmap
		DWORD killUnitStratMapFunc;

	}offsets;

	static void initCodes(int gameVer);;
};