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
		
		DWORD createEduEntry = NULL;
		DWORD uniStrCustomTile = NULL;



		DWORD createIdleActionFunc = NULL;
		DWORD makeCadDiplomacyFunc = NULL;
		DWORD makeCadDiplomacySettlementFunc = NULL;
		DWORD createCADTargetCharacter = NULL;
		DWORD createCADTargetFort = NULL;
		DWORD createCADTargetSettlement = NULL;


		DWORD createSiegeSettlementGarrisonedFunc = NULL;
		DWORD getReadySiegeCharacterGarrisonedFunc = NULL;
		DWORD createCadAssaultSettlementFunc = NULL;
		DWORD createCadSiegeSettlementFunc = NULL;
		DWORD createCadAttackSettlementFunc = NULL;
		DWORD finalizeActionStrat = NULL;


		DWORD getReadyForMovingFunc = NULL;
		DWORD getReadyForMoving2Func = NULL;
		DWORD getGeneralReadyForMovingFunc = NULL;

		DWORD makeTrackedPointerToAttackFunc = NULL;
		DWORD makeCadAttackFunc = NULL;
		DWORD somethingWithTrackedPointerAttackFunc = NULL;
		
		DWORD somethingWithTrackedPointerFortFunc = NULL;


		//strat camera functions
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
		DWORD setUnitWeaponFunc = NULL;

		//create character
		DWORD createCharacterFunc = NULL;
		DWORD spawnCreatedObject = NULL;
		DWORD doSomeWithCharacterFunc = NULL;

		//create army
		DWORD createArmyFunc = NULL;
		DWORD createArmyInSettlementFunc = NULL;
		DWORD createUnitFunc = NULL;
		DWORD addUnitToArmyFunc = NULL;
		DWORD makeBuildingCondition = NULL;

		DWORD setBodyguard = NULL;

		//kill character on strat map
		DWORD killCharStratMapFunc = NULL;
		//kill unit on strat map
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
		DWORD factionResurrectStuffFunc = NULL;

		DWORD unitMove = NULL;
		DWORD unitOrderMove = NULL;
		DWORD unitPlace = NULL;
		DWORD getUnitByLabel = NULL;
		DWORD getUnitByLabel2 = NULL;
		DWORD getGroupByLabel = NULL;
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
		
		DWORD groupUnitChangeFormation = NULL;
		DWORD groupMoveToRangeOfGroup = NULL;
		DWORD groupMoveToRangeOfUnit = NULL;
		DWORD groupAttackGroup = NULL;
		DWORD groupChangeFormation = NULL;
		DWORD groupHalt = NULL;
		DWORD groupMoveFormed = NULL;
		DWORD groupMoveUnformed = NULL;
		DWORD groupRelativeMoveFormed = NULL;
		DWORD groupRelativeMoveUnformed = NULL;
		DWORD groupTurn = NULL;
		DWORD gameLogCommand = NULL;
		DWORD executeCADTargetCharacter = NULL;
		DWORD switchCharacterFaction = NULL;
		DWORD switchNamedCharacterFaction = NULL;
		DWORD switchArmyFaction = NULL;
		DWORD changeCharacterTileStuff = NULL;
		DWORD initPlaceCharacter = NULL;
		DWORD getResidenceCharacterNum = NULL;
		DWORD getResidenceCharacterAtIndex = NULL;
		DWORD removeSieges = NULL;
		DWORD doHordeStuff = NULL;
		DWORD getTileCharactersFunc = NULL;
		DWORD getValidRegionTile = NULL;
		DWORD isTileValidForCharacter = NULL;
		DWORD deleteMoveExtents = NULL;
		DWORD someSelectionStuff = NULL;
		DWORD doHordeStuff2 = NULL;
		DWORD doHordeStuff3 = NULL;
		DWORD sendCharacterOffMap = NULL;
		DWORD assaultObject = NULL;
		DWORD checkOwnershipCustom = NULL;
		DWORD getUnitCard = NULL;
		DWORD addToCardArray = NULL;
		DWORD buildWatchtower = NULL;
		DWORD sabotageBuilding = NULL;
		DWORD setCharacterType = NULL;
		DWORD addAncillary = NULL;
		DWORD getTrait = NULL;
		DWORD addTrait = NULL;
		DWORD removeTrait = NULL;
		DWORD removeAncillary = NULL;
		DWORD issueMoveOrder = NULL;
		DWORD issueFormationOrder = NULL;
		DWORD repairBuilding = NULL;
		DWORD addBuildingToQueue = NULL;
		DWORD addUnitToQueue = NULL;
		DWORD createBuildInSett = NULL;
		DWORD upgradeBuildInSett = NULL;
		DWORD upgradeSett = NULL;
		DWORD getRecruitmentOptions = NULL;
		DWORD getRetrainingOptions = NULL;
		DWORD resetBuildingsMem = NULL;
		DWORD getConstructionOptions = NULL;
		DWORD makeWatchTower = NULL;
		DWORD addToWatchtowerList = NULL;
		DWORD updateBlockades = NULL;
		DWORD createCadDisembark = NULL;
		DWORD setCadClass = NULL;
		DWORD canArmySplit = NULL;
		DWORD splitArmy = NULL;
		DWORD setUnitMovePoints = NULL;
		DWORD automateGroup = NULL;
		DWORD placeGroup = NULL;
		DWORD clearUnitGroup = NULL;
		DWORD removeUnusedLabel = NULL;
		DWORD defineUnitGroup = NULL;
		DWORD findAncillary = NULL;
		DWORD mergeArmies = NULL;
		DWORD stopBlockade = NULL;
		DWORD stopSiege = NULL;
		DWORD addToSettlement = NULL;
		DWORD addToFort = NULL;
		DWORD ungarrisonResidence = NULL;
		DWORD attackArmy = NULL;
		DWORD getTileMoveCost = NULL;
		DWORD revealTile = NULL;
		DWORD setFactionTrade = NULL;
		DWORD setProtectorate = NULL;
		DWORD isTileFree = NULL;
		DWORD hideRevealedTile = NULL;
		DWORD findBattleModel = NULL;
	}offsets;

	static void initCodes(int gameVer);
};