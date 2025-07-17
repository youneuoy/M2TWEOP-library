#pragma once
#include"headersSTD.h"

class codes
{
public:
	static struct offsetsT
	{
		DWORD stringCryptFunc = NULL;
		DWORD freeHashString = NULL;
		DWORD loadStratCasModelFunc = NULL;
		DWORD getRelativePath = NULL;
		DWORD parseCas = NULL;
		DWORD createModelRigid = NULL;
		DWORD appendTexturesFolder = NULL;
		DWORD loadModelRigid = NULL;
		DWORD getCasModel = NULL;
		DWORD closeCas = NULL;
		DWORD simpleCasDestructor = NULL;
		DWORD createCultureSettlement = NULL;
		DWORD releaseCultureSettlement = NULL;
		DWORD createCultureFort = NULL;
		DWORD createCultureCasEntry = NULL;

		DWORD allocMemFunc = NULL;
		
		DWORD createEduEntry = NULL;
		DWORD createUniString = NULL;
		DWORD getScriptCounter = NULL;
		DWORD setScriptCounter = NULL;
		DWORD getUnitFullMovePoints = NULL;
		DWORD createMoveExtents = NULL;
		DWORD setCharacterName = NULL;
		DWORD birthLog = NULL;
		DWORD marryCharacters = NULL;

		DWORD createIdleActionFunc = NULL;
		DWORD makeCadDiplomacyFunc = NULL;
		DWORD makeCadDiplomacySettlementFunc = NULL;
		DWORD createCADTargetCharacter = NULL;
		DWORD createCADTargetFort = NULL;
		DWORD createCADTargetSettlement = NULL;
		DWORD saveGame = NULL;


		DWORD createSiegeSettlementGarrisonedFunc = NULL;
		DWORD getReadySiegeCharacterGarrisonedFunc = NULL;
		DWORD createCadAssaultSettlementFunc = NULL;
		DWORD createCadSiegeSettlementFunc = NULL;
		DWORD createCadAttackSettlementFunc = NULL;
		DWORD finalizeActionStrat = NULL;
		DWORD hasFreeTilesToMove = NULL;
		DWORD canStartSiege = NULL;
		DWORD canStartAssault = NULL;
		DWORD getNumEnginesCanPenetrateWalls = NULL;
		DWORD canSeeCharacter = NULL;
		DWORD getArmyCoords = NULL;
		DWORD officerDestructor = NULL;
		DWORD getEventController = NULL;
		DWORD deleteScriptEvent = NULL;


		DWORD getReadyForMovingFunc = NULL;
		DWORD getReadyForMoving2Func = NULL;
		DWORD getGeneralReadyForMovingFunc = NULL;

		DWORD makeTrackedPointerToAttackFunc = NULL;
		DWORD makeCadAttackFunc = NULL;
		DWORD somethingWithTrackedPointerAttackFunc = NULL;
		DWORD clearCharacterStuff = NULL;
		
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
		DWORD createUnitFunc2 = NULL;
		DWORD addUnitToArmyFunc = NULL;
		DWORD makeBuildingCondition = NULL;
		DWORD switchFactionLeader = NULL;

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
		DWORD parseCondition = NULL;
		DWORD getStringFromTable = NULL;
		DWORD scriptCommandOne = NULL;
		DWORD scriptCommandTwo = NULL;
		DWORD getEntriesByFaction = NULL;
		DWORD createSettlementConditionContext = NULL;
		DWORD isBuildingTypePresent = NULL;
		DWORD canSettlementUpgrade = NULL;
		DWORD isLevelAvailableForLevel = NULL;
		DWORD evaluateBuildingGuard = NULL;
		DWORD createConstructionOption = NULL;
		DWORD createConvertSett = NULL;
		DWORD createBuildingSett = NULL;
		DWORD buildQueueConflictTest = NULL;
		DWORD removeBuildingSelection = NULL;


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
		DWORD dbHashTableGet = NULL;
		DWORD dbHashTableSet = NULL;
		DWORD deleteFortFuncTwo = NULL;
		DWORD createFortFunc = NULL;
		DWORD createsFort = NULL;
		DWORD addToFortsArray = NULL;
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
		DWORD modifyWithSettMechanics = NULL;
		DWORD getMovePath = NULL;

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
		DWORD initGameScriptLog = NULL;
		DWORD createSpCountedBase = NULL;
		DWORD createLogger = NULL;
		DWORD groupTurn = NULL;
		DWORD gameLogCommand = NULL;
		DWORD executeCADTargetCharacter = NULL;
		DWORD switchCharacterFaction = NULL;
		DWORD switchNamedCharacterFaction = NULL;
		DWORD switchArmyFaction = NULL;
		DWORD modelRigidDestructor = NULL;
		DWORD generateSprite = NULL;
		DWORD changeCharacterTileStuff = NULL;
		DWORD initPlaceCharacter = NULL;
		DWORD getNextWord = NULL;
		DWORD getResidenceCharacterNum = NULL;
		DWORD getResidenceCharacterAtIndex = NULL;
		DWORD removeSieges = NULL;
		DWORD doHordeStuff = NULL;
		DWORD createCharacterRecord = NULL;
		DWORD getTileCharactersFunc = NULL;
		DWORD getValidRegionTile = NULL;
		DWORD isTileValidForCharacter = NULL;
		DWORD deleteMoveExtents = NULL;
		DWORD someSelectionStuff = NULL;
		DWORD clearPaths = NULL;
		DWORD clearMapPath = NULL;
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
		DWORD addTraitPoints = NULL;
		DWORD addTrait = NULL;
		DWORD removeTrait = NULL;
		DWORD removeAncillary = NULL;
		DWORD issueMoveOrder = NULL;
		DWORD createUnitGroupDeployer = NULL;
		DWORD buildingBattleGetFacing = NULL;
		DWORD findDeployPosition = NULL;
		DWORD issueFormationOrder = NULL;
		DWORD repairBuilding = NULL;
		DWORD addBuildingToQueue = NULL;
		DWORD addUnitToQueue = NULL;
		DWORD createBuildInSett = NULL;
		DWORD upgradeBuildInSett = NULL;
		DWORD upgradeSett = NULL;
		DWORD createSettlement = NULL;
		DWORD createSallyForthObjective = NULL;
		DWORD createRallyPointSundry = NULL;
		DWORD initRallyPoint = NULL;
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
		DWORD setCapital = NULL;
		DWORD resetLtgd = NULL;
		DWORD setLtgdNavalTarget = NULL;
		DWORD getRegionGroup = NULL;
		DWORD getFactionRegionGroupNum = NULL;
		DWORD isLoneRegionGroup = NULL;
		DWORD isZoneValid = NULL;
		DWORD removeTrackedReference = NULL;
		DWORD getZonePerimeter = NULL;
		DWORD getZoneID = NULL;
		DWORD settAttachFaction = NULL;
		DWORD areaOfInfluence = NULL;
		DWORD addToSettlementList = NULL;
		DWORD residenceTileCharacterCheck = NULL;
		DWORD recalculateSettlement = NULL;
		DWORD setPop = NULL;
		DWORD addToArrayInt = NULL;
		DWORD changeWatchTowerFaction = NULL;
		DWORD setRegionFaction = NULL;
		DWORD clearAiRegionData = NULL;
		DWORD createRegionInArray = NULL;
		DWORD getRegionIsolationScore = NULL;
		DWORD calculateRegionStrengths = NULL;
		DWORD calculateBackupRegionStrengths = NULL;
		DWORD isTrustedAlly = NULL;
		DWORD resetRegionGroupControllers = NULL;
		DWORD setSettlementTaxLevel = NULL;
		DWORD usePersonalityType = NULL;
		DWORD isMicroManageAll = NULL;
		DWORD usePersonalityName = NULL;
		DWORD addSettlementPolicy = NULL;
		DWORD addNavalTargetRegion = NULL;
		DWORD removeNavalTargetRegion = NULL;
		DWORD createSettCondContext = NULL;
		DWORD createHordeController = NULL;
		DWORD createRegionController = NULL;
		DWORD addToRegionControllers = NULL;
		DWORD releaseResource = NULL;
		DWORD getRequiredGarrisonStrength = NULL;
		DWORD recalcRegionGroupStrengths = NULL;
		DWORD manageRegionGroups = NULL;
		DWORD initGsdFactionData = NULL;
		DWORD clearTransports = NULL;
		DWORD addGsdRegion = NULL;
		DWORD aiCrusadeController = NULL;
		DWORD addCampaignController = NULL;
		DWORD createCampaignController = NULL;
		DWORD sortControllers = NULL;
		DWORD calculateCampaignControllerStrengths = NULL;
		DWORD manageNavalTransports = NULL;
		DWORD balanceRegionGroups = NULL;
		DWORD initNavalController = NULL;
		DWORD destroyFaction = NULL;
		DWORD reduceUnitProduction = NULL;
		DWORD reduceEconomicProduction = NULL;
		DWORD prodSetPriorities = NULL;
		DWORD addToProductionControllers = NULL;
		DWORD attachResourceSett = NULL;
		DWORD updateFowTableObject = NULL;
		DWORD updateFowTable = NULL;
		DWORD ParseDescrMountedEngineEntry = NULL;
		DWORD updateTerrain = NULL;
		DWORD updateRadar = NULL;
		DWORD canMerge = NULL;
		DWORD generalJoinCrusade = NULL;
		DWORD generalLeaveCrusade = NULL;
		DWORD incEventCounter = NULL;
		DWORD createEventAcceptObj = NULL;
		DWORD formatStringAdd = NULL;
		DWORD getParserFileName = NULL;
		DWORD getDataInvalidLogger = NULL;
		DWORD createEnabledLogger = NULL;
		DWORD destroyLogObject = NULL;
		DWORD createMountedEngineRecord = NULL;
		DWORD createAiProductionController = NULL;
	}offsets;

	static void initCodes(int gameVer);
};