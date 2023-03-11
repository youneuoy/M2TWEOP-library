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

		DWORD makeTrackedPointerToAttackFunc = NULL;
		DWORD makeCadAttackFunc = NULL;
		DWORD somethingWithTrackedPointerAttackFunc = NULL;


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
		DWORD createArmyInSettlementFunc = NULL;
		DWORD createUnitFunc = NULL;
		DWORD addUnitToArmyFunc = NULL;

		DWORD setBodyguard = NULL;

		//kill character on stratmap
		DWORD killCharStratMapFunc = NULL;
		//kill unit on stratmap
		DWORD killUnitStratMapFunc = NULL;

		//destroy all building of type in settlement
		DWORD destroyBuildingFunc = NULL;
		//create building
		DWORD createBuildingFunc = NULL;
		DWORD autoWinFunc = NULL;
		DWORD getStringFromTable = NULL;


		DWORD autoResolveFunc = NULL;


		DWORD useButtonFunc = NULL;
		DWORD getUiElementFunc = NULL;

		DWORD getSkeleton = NULL;
		DWORD createModelFlexi = NULL;
		DWORD createModelFlexiShadow = NULL;
		DWORD assignCasToFlexi = NULL;
		DWORD assignShadowCasToFlexi = NULL;
		DWORD loadStratCharModel = NULL;
		DWORD readTgaFile = NULL;

		DWORD readEDUEntryFunc = NULL;
		DWORD deleteFortFuncOne = NULL;
		DWORD deleteFortFuncTwo = NULL;
		DWORD createFortFunc = NULL;
		DWORD mercEOPValue = NULL;
	}offsets;

	static void initCodes(int gameVer);
};