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



		DWORD createIdleActionFunc = NULL;


		DWORD createSiegeSettlementGarrisonedFunc = NULL;
		DWORD getReadySiegeCharacterGarrisonedFunc = NULL;
		DWORD createCadAssaultSettlementFunc = NULL;
		DWORD createCadSiegeSettlementFunc = NULL;
		DWORD createCadAttackSettlementFunc = NULL;
		DWORD finalyzeActionStratmapFunc = NULL;


		DWORD getReadyForMovingFunc = NULL;
		DWORD getReadyForMoving2Func = NULL;
		DWORD getGeneralReadyForMovingFunc = NULL;

		DWORD makeTrackedPointerToAttackFunc = NULL;
		DWORD makeCadAttackFunc = NULL;
		DWORD somethingWithTrackedPointerAttackFunc = NULL;
		
		DWORD somethingWithTrackedPointerFortFunc = NULL;


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
		DWORD makeBuildingCondition = NULL;

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
		DWORD historicEventFunc = NULL;
		DWORD getStringFromTable = NULL;
		DWORD scriptCommandOne = NULL;
		DWORD scriptCommandTwo = NULL;


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
		DWORD makeConditionFunc = NULL;
		DWORD getEquipmentPreferencesFunc = NULL;
		DWORD decideEquipmentFunc = NULL;
		DWORD createCadMovingNormalFunc = NULL;
		DWORD createCadAttackFortFunc = NULL;
		DWORD createCadAssaultFortFunc = NULL;
		DWORD createCadSiegeFortFunc = NULL;
		DWORD makeTrackedPointerFortFunc = NULL;
		DWORD getRandomNameFunc = NULL;
		DWORD getNameIndexFunc = NULL;
		DWORD getCharacterName = NULL;
		DWORD respawnOffMapCharacterFunc = NULL;
		DWORD setFactionLeaderFunc = NULL;
		DWORD findParentForAdoptionFunc = NULL;
		DWORD setArmyRegionEntriesFunc = NULL;
		DWORD factionRessurectStuffFunc = NULL;

		DWORD unitMove = NULL;
		DWORD unitOrderMove = NULL;
		DWORD unitPlace = NULL;
		DWORD getUnitByLabel = NULL;
		DWORD getUnitByLabel2 = NULL;
		DWORD unitImmediatePlace = NULL;
		DWORD unitAttackClosestUnit = NULL;
		DWORD unitAttackUnit = NULL;
		DWORD unitDeployStakes = NULL;
		DWORD unitChangeFormation = NULL;
		DWORD unitHalt = NULL;
		DWORD getMapHeight = NULL;
		DWORD unitMoveToOrientation = NULL;
		DWORD moveRelative = NULL;
		DWORD moveToMissileRange = NULL;
		DWORD unitTurn = NULL;
		DWORD unitTaunt = NULL;
		DWORD useSpecialAbility = NULL;
		DWORD getBuildingAngle = NULL;
		DWORD attackBuilding = NULL;
	}offsets;

	static void initCodes(int gameVer);
};