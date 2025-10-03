#include "pch.h"
#include "functionsOffsets.h"
codes::offsetsT codes::offsets;
void codes::initCodes(int gameVer)
{
	if (gameVer == 1) //kingdoms
	{
		offsets.stringCryptFunc = 0x00d4cfd0;
		offsets.loadStratCasModelFunc = 0x00a04380;
		offsets.allocMemFunc = 0x012418d8;
		offsets.createEduEntry = 0x008EECA0;

		offsets.createSiegeSettlementGarrisonedFunc = 0x005b56c0;
		offsets.createCadAttackSettlementFunc = 0x005B55B0;
		offsets.createCadAssaultSettlementFunc = 0x005B57D0;
		offsets.createCadSiegeSettlementFunc = 0x005B56C0;
		offsets.createCadMovingNormalFunc = 0x005B0060;
		offsets.getReadySiegeCharacterGarrisonedFunc = 0x005a18e0;
		offsets.decideEquipmentFunc = 0x0071C920;
		offsets.getEquipmentPreferencesFunc = 0x00521520;
		offsets.finalizeActionStrat = 0x0059e790;
		offsets.saveGame = 0x00434da0;
		offsets.parseCondition = 0x874920;

		offsets.createIdleActionFunc = 0x005aff40;

		offsets.getRandomNameFunc = 0x008DC920;
		offsets.getNameIndexFunc = 0x005A43D0;
		offsets.getCharacterName = 0x008DBFE0;
		offsets.respawnOffMapCharacterFunc = 0x008B4450;
		offsets.setFactionLeaderFunc = 0x004E8FE0;
		offsets.findParentForAdoptionFunc = 0x004EA4E0;
		offsets.setArmyRegionEntriesFunc = 0x004A40E0;
		offsets.factionResurrectStuffFunc = 0x004F4C80;
		offsets.doHordeStuff = 0x0050B170;
		offsets.doHordeStuff2 = 0x004EAA10;
		offsets.doHordeStuff3 = 0x00C5CBC0;
		
		offsets.switchCharacterFaction = 0x005A1C20;
		offsets.switchNamedCharacterFaction = 0x00594370;
		offsets.switchArmyFaction = 0x004F4520;
		offsets.changeCharacterTileStuff = 0x00592EB0;
		offsets.initPlaceCharacter = 0x00599410;
		
		offsets.getResidenceCharacterNum = 0x004BFCC0;
		offsets.getResidenceCharacterAtIndex = 0x004BFEB0;
		
		
		
		offsets.getReadyForMovingFunc = 0x005b0060;
		offsets.getReadyForMoving2Func = 0x005a0870;
		offsets.getGeneralReadyForMovingFunc = 0x004cd460;

		offsets.getStringFromTable = 0x00CDE400;

		offsets.makeTrackedPointerToAttackFunc = 0x004e6000;
		offsets.makeCadAttackFunc = 0x005b5420;
		offsets.somethingWithTrackedPointerAttackFunc = 0x004cb6d0;
		offsets.makeCadDiplomacyFunc = 0x005B60D0;
		offsets.makeCadDiplomacySettlementFunc = 0x005B61F0;



		//CAMERA
		offsets.moveStratCameraSlowFunc = 0x0098d940;
		offsets.moveStratCameraFastFunc = 0x0098d4b0;
		offsets.zoomStratCameraFunc = 0x0098d6a0;
		
		offsets.createCadAttackFortFunc = 0x005B5950;
		offsets.createCadAssaultFortFunc = 0x005B5B90;
		offsets.createCadSiegeFortFunc = 0x005B5A70;
		offsets.makeTrackedPointerFortFunc = 0x004E61F0;
		offsets.somethingWithTrackedPointerFortFunc = 0x004EB5B0;


		offsets.teleportCharacterFunc = 0x0059b580;
		offsets.removeSieges = 0x004bf340;



		offsets.replenishUnitFunc = 0x00748e20;
		offsets.setUnitArmorFunc = 0x0073bce0;
		offsets.setUnitWeaponFunc = 0x0073bb00;


		offsets.createCharacterFunc = 0x008b5d60;
		offsets.spawnCreatedObject = 0x004cd240;
		offsets.doSomeWithCharacterFunc = 0x004bafa0;

		offsets.createArmyFunc = 0x004f40d0;
		offsets.createArmyInSettlementFunc = 0x005dbdd0;
		offsets.createUnitFunc = 0x008edf60;
		offsets.addUnitToArmyFunc = 0x00719380;
		offsets.setBodyguard = 0x00598f10;
		offsets.makeConditionFunc = 0x00874920;
		offsets.makeBuildingCondition = 0x008A6B30;

		offsets.killCharStratMapFunc = 0x00592900;
		offsets.killUnitStratMapFunc = 0x007417e0;

		offsets.destroyBuildingFunc = 0x005e4f10;
		offsets.createBuildingFunc = 0x00d32cd0;

		offsets.autoWinFunc = 0x00d2dcf0;
		offsets.historicEventFunc = 0x00C5BD50;
		offsets.scriptCommandOne = 0x00A945F0;
		offsets.scriptCommandTwo = 0x00A93570;

		offsets.autoResolveFunc = 0x00aa6bc0;


		offsets.useButtonFunc = 0x00b0daf0;
		offsets.getUiElementFunc = 0x00b4bff0;

		offsets.getSkeleton = 0x007403B0;
		offsets.createModelFlexi = 0x0093AB40;
		offsets.createModelFlexiShadow = 0x00936750;
		offsets.assignCasToFlexi = 0x0093B1E0;
		offsets.assignShadowCasToFlexi = 0x00933F40;
		offsets.loadStratCharModel = 0x00D51A40;
		offsets.readTgaFile = 0x008FF780;

		offsets.readEDUEntryFunc = 0x008ef2d0;
		offsets.deleteFortFuncOne = 0x00BB5B80;
		offsets.deleteFortFuncTwo = 0x004BCA80;
		offsets.createFortFunc = 0x00714760;
		offsets.createsFort = 0x8B8C00;
		offsets.addToFortsArray = 0x4BD330;
		offsets.mercEOPValue = 0x0190B928;
		
		offsets.getUnitByLabel = 0x0A945A0;
		offsets.getGroupByLabel = 0x00A92160;
		offsets.getUnitByLabel2 = 0x0A91780;
		
		offsets.unitOrderMove = 0x00A6DAA0;
		offsets.unitImmediatePlace = 0x00A6D310;
		offsets.unitAttackClosestUnit = 0x00A789C0;
		offsets.unitAttackUnit = 0x00A6E920;
		offsets.unitDeployStakes = 0x00A77E00;
		offsets.unitChangeFormation = 0x00A6F040;
		offsets.unitHalt = 0x00A6D680;
		offsets.getMapHeight = 0x00443E70;
		offsets.unitMoveToOrientation = 0x0A6E140;
		offsets.moveRelative = 0x0A79810;
		offsets.moveToMissileRange = 0x0A6F400;
		offsets.unitTurn = 0x0A6F940;
		offsets.unitTaunt = 0x0A723F0;
		offsets.useSpecialAbility = 0x0A72630;
		offsets.getBuildingAngle = 0x006EAB10;
		offsets.attackBuilding = 0x00757750;
		offsets.createCADTargetCharacter = 0x00AAB0C0;
		offsets.createCADTargetFort = 0x00AAB1A0;
		offsets.createCADTargetSettlement = 0x00AAB130;
		offsets.executeCADTargetCharacter = 0x00AAD080;
		offsets.getTileCharactersFunc = 0x004D0000;
		offsets.getValidRegionTile = 0x004A5420;
		offsets.isTileValidForCharacter = 0x004CCA10;
		offsets.deleteMoveExtents = 0x004B6250;
		offsets.someSelectionStuff = 0x004DAE80;
		offsets.clearPaths = 0x4DAC10;

		//Group commands
		offsets.groupUnitChangeFormation = 0x00A78FA0;
		offsets.groupMoveToRangeOfGroup = 0x00A758C0;
		
		offsets.groupMoveToRangeOfUnit = 0x00A752D0;
		offsets.groupAttackGroup = 0x00A76310;
		offsets.groupChangeFormation = 0x00A76820;
		offsets.groupHalt = 0x00A73AC0;
		offsets.groupMoveFormed = 0x00A73F50;
		offsets.groupMoveUnformed = 0x00A744B0;
		offsets.groupRelativeMoveFormed = 0x00A749B0;
		offsets.groupRelativeMoveUnformed = 0x00A74E80;
		offsets.groupTurn = 0x00A76BF0;
		offsets.sendCharacterOffMap = 0x4EEBC0;
		offsets.assaultObject = 0x00718F50;
		offsets.buildWatchtower = 0x0714A50;
		
		offsets.checkOwnershipCustom = 0x8EE450;
		offsets.getUnitCard = 0x008EB550;
		offsets.addToCardArray = 0x464610;
		
		//offsets.unitPlace = 0x00770A70;
		offsets.unitMove = 0x0759B70;
		offsets.gameLogCommand = 0xA8FAC0;
		offsets.sabotageBuilding = 0xAAB280;
		offsets.setCharacterType = 0x008b3810;
		offsets.addAncillary = 0x005a5870;
		offsets.getTrait = 0x008b6680;
		offsets.addTrait = 0x005a7710;
		offsets.removeTrait = 0x005a53a0;
		offsets.removeAncillary = 0x005a5990;
		offsets.issueMoveOrder = 0x692CD0;
		offsets.issueFormationOrder = 0x692230;
		offsets.repairBuilding = 0x005F89D0;
		offsets.addBuildingToQueue = 0x5E4190;
		offsets.addUnitToQueue = 0x5E40A0;
		offsets.createBuildInSett = 0x008A32F0;
		offsets.upgradeBuildInSett = 0x005F7CB0;
		offsets.upgradeSett = 0x005DB560;
		offsets.getRecruitmentOptions = 0x005E7A40;
		offsets.getRetrainingOptions = 0x005E6140;
		offsets.resetBuildingsMem = 0x005FA320;
		offsets.getConstructionOptions = 0x005E1730;
		offsets.makeWatchTower = 0x008B8DC0;
		offsets.addToWatchtowerList = 0x004DD390;
		offsets.updateBlockades = 0x004DCD40;
		offsets.createCadDisembark = 0x5B01F0;
		offsets.setCadClass = 0xAAB050;
		offsets.canArmySplit = 0x4D5C30;
		offsets.splitArmy = 0x4F9740;
		offsets.setUnitMovePoints = 0x00597770;
		offsets.automateGroup = 0x00721410;
		offsets.placeGroup = 0x0071DEE0;
		offsets.clearUnitGroup = 0x0071E590;
		offsets.removeUnusedLabel = 0x00A92320;
		offsets.defineUnitGroup = 0x00A91BD0;
		offsets.findAncillary = 0x008b1340;
		offsets.mergeArmies = 0x00714EF0;
		
		offsets.stopBlockade = 0x00711310;
		offsets.stopSiege = 0x007110f0;
		offsets.addToSettlement = 0x005e18f0;
		offsets.addToFort = 0x004bc690;
		offsets.ungarrisonResidence = 0x004bf9c0;
		offsets.attackArmy = 0x718E80;
		offsets.getTileMoveCost = 0x004c7620;
		offsets.revealTile = 0x004ba910;
		offsets.setFactionTrade = 0x00502EE0;
		offsets.setProtectorate = 0x00504980;
		offsets.isTileFree = 0x004c8c70;
		offsets.hideRevealedTile = 0x004ba9f0;
		offsets.findBattleModel = 0x010e86c0;
		offsets.isZoneValid = 0x672EC0;
		offsets.getZonePerimeter = 0xE0DC00;
		offsets.getZoneID = 0xDF6050;
		offsets.createUniString = 0x00f018e0;
		offsets.getScriptCounter = 0x00489360;
		offsets.setScriptCounter = 0x0048c8e0;
		offsets.getUnitFullMovePoints = 0x00742380;
		offsets.createMoveExtents = 0x4B60E0;
		offsets.createSettlement = 0x005E4500;
		offsets.createRallyPointSundry = 0x005D8C80;
		offsets.initRallyPoint = 0x005D8D00;
		offsets.settAttachFaction = 0x005DB000;
		offsets.areaOfInfluence = 0x004C9600;
		offsets.addToSettlementList = 0x005F0480;
		offsets.residenceTileCharacterCheck = 0x004BF880;
		offsets.recalculateSettlement = 0x005F41D0;
		offsets.setPop = 0x005F0EE0;
		offsets.addToArrayInt = 0x00464820;
		offsets.changeWatchTowerFaction = 0x004DCE80;
		offsets.setRegionFaction = 0x0049FA50;
		offsets.clearAiRegionData = 0x533760;
		offsets.createRegionInArray = 0x533660;
		offsets.getRegionIsolationScore = 0x514AC0;
		offsets.calculateRegionStrengths = 0x5160B0;
		offsets.calculateBackupRegionStrengths = 0x00516140;
		offsets.isTrustedAlly = 0x0053CA60;
		offsets.resetRegionGroupControllers = 0x00518D20;
		offsets.addNavalTargetRegion = 0x00534B00;
		offsets.removeNavalTargetRegion = 0x00534C40;
		offsets.createHordeController = 0x00536720;
		offsets.createRegionController = 0x00550030;
		offsets.addToRegionControllers = 0x00533850;
		offsets.releaseResource = 0x00551B50;
		offsets.getRequiredGarrisonStrength = 0x00515580;
		offsets.recalcRegionGroupStrengths = 0x518DA0;
		offsets.manageRegionGroups = 0x533EE0;
		offsets.initGsdFactionData = 0x532CF0;
		offsets.clearTransports = 0x005363E0;
		offsets.addGsdRegion = 0x005335C0;
		offsets.aiCrusadeController = 0x00535770;
		offsets.addCampaignController = 0x005452F0;
		offsets.createCampaignController = 0x0051AC70;
		offsets.sortControllers = 0x005454E0;
		offsets.calculateCampaignControllerStrengths = 0x0051BE00;
		offsets.manageNavalTransports = 0x00537280;
		offsets.balanceRegionGroups = 0x00537390;
		offsets.initNavalController = 0x0054A770;
		offsets.destroyFaction = 0x55F4A0;
		offsets.reduceUnitProduction = 0x0055F610;
		offsets.reduceEconomicProduction = 0x55F6D0;
		offsets.attachResourceSett = 0x4C4250;
		offsets.updateFowTableObject = 0x4BB330;
		offsets.updateFowTable = 0x4BB790;
		offsets.updateTerrain = 0x98D640;
		offsets.updateRadar = 0xBB4E40;
		offsets.clearMapPath = 0x98DC00;
		offsets.canMerge = 0x717C60;
		offsets.hasFreeTilesToMove = 0x596420;
		offsets.canStartSiege = 0x005212F0;
		offsets.canStartAssault = 0x711620;
		offsets.getNumEnginesCanPenetrateWalls = 0x710D00;
		offsets.canSeeCharacter = 0x4D3A90;
		offsets.getArmyCoords = 0x712FA0;
		offsets.generalJoinCrusade = 0x5A0AA0;
		offsets.generalLeaveCrusade = 0x5A0CB0;
		offsets.addTraitPoints = 0x5A6710;
		offsets.incEventCounter = 0x48C810;
		offsets.createEventAcceptObj = 0xA7FBE0;
		offsets.createUnitFunc2 = 0x8EC560;
		offsets.switchFactionLeader = 0x4E8FE0;
		offsets.modifyWithSettMechanics = 0x8E6390;
		offsets.setCharacterName = 0x005A3D20;
		offsets.officerDestructor = 0x740A90;
		offsets.getEventController = 0x488BD0;
		offsets.deleteScriptEvent = 0x48AAD0; // 48AAD0
		offsets.getRelativePath = 0x41B9A0; // 48AAD0
		offsets.parseCas = 0xD519A0;
		offsets.createModelRigid = 0x9336C0;
		offsets.appendTexturesFolder = 0x9FFBE0;
		offsets.loadModelRigid = 0x933280;
		offsets.getCasModel = 0xD4B960;
		offsets.closeCas = 0xD518D0;
		offsets.simpleCasDestructor = 0xD51970;
		offsets.createCultureSettlement = 0x8BB560;
		offsets.releaseCultureSettlement = 0x8B9C50;
		offsets.createCultureFort = 0x8B9B10;
		offsets.createCultureCasEntry = 0x8B88C0;
		offsets.getEntriesByFaction = 0x8A8300;
		offsets.createSettlementConditionContext = 0x609690;
		offsets.isBuildingTypePresent = 0x6089C0;
		offsets.canSettlementUpgrade = 0x5DB410;
		offsets.isLevelAvailableForLevel = 0x5FE0D0;
		offsets.evaluateBuildingGuard = 0x5FCC60;
		offsets.createConstructionOption = 0x5FAD70;
		offsets.createConvertSett = 0x5FAC00;
		offsets.createBuildingSett = 0x5FAB60;
		offsets.buildQueueConflictTest = 0x5EB100;
		offsets.removeBuildingSelection = 0x5FA3A0;
		offsets.getNextWord = 0xD4F060;
		offsets.modelRigidDestructor = 0x933640;
		offsets.generateSprite = 0xE7E220;
		offsets.initGameScriptLog = 0xD39670;
		offsets.createSpCountedBase = 0x403A60;
		offsets.createLogger = 0x4224C0;
		offsets.createUnitGroupDeployer = 0x6D6640;
		offsets.buildingBattleGetFacing = 0x6EA3F0;
		offsets.findDeployPosition = 0x6D6D20;
		offsets.createSallyForthObjective = 0x51D3D0;
		offsets.freeHashString = 0xD4B9E0;
		offsets.getMovePath = 0x4D7DF0;
		offsets.removeTrackedReference = 0xD6F200;
		offsets.resetLtgd = 0x53C1A0;
		offsets.setLtgdNavalTarget = 0x53F520;
		offsets.getRegionGroup = 0x518E40;
		offsets.getFactionRegionGroupNum = 0x518E90;
		offsets.isLoneRegionGroup = 0x5190E0;
		offsets.setCapital = 0x4FB500;
		offsets.setSettlementTaxLevel = 0x52D070;
		offsets.usePersonalityType = 0x530940;
		offsets.usePersonalityName = 0x530DC0;
		offsets.isMicroManageAll = 0x467730;
		offsets.addSettlementPolicy = 0x5316E0;
		offsets.createAiProductionController = 0x52E020;
		offsets.prodSetPriorities = 0x52E3B0;
		offsets.addToProductionControllers = 0x5315F0;
		offsets.ParseDescrMountedEngineEntry = 0x8CD4E0;
		offsets.formatStringAdd = 0xD386E0;
		offsets.getParserFileName = 0xD4D270;
		offsets.getDataInvalidLogger = 0xD388C0;
		offsets.createEnabledLogger = 0x403D60;
		offsets.destroyLogObject = 0x403DF0;
		offsets.createMountedEngineRecord = 0x8CE1B0;
		offsets.dbHashTableGet = 0x437DF0;
		offsets.dbHashTableSet = 0x437890;
		offsets.createCharacterRecord = 0x4F3F70;
		offsets.birthLog = 0x832F70;
		offsets.marryCharacters = 0x5A7CB0;
	}


	
	/////////////////// STEAM ///////////////////////////////////
	else if (gameVer == 2) //steam
	{
		offsets.stringCryptFunc = 0x00d47330;
		offsets.loadStratCasModelFunc = 0x00a04f00;
		offsets.allocMemFunc = 0x011d318a;
		offsets.createEduEntry = 0x008EF720;

		offsets.createSiegeSettlementGarrisonedFunc = 0x005b5ba0;
		offsets.getReadySiegeCharacterGarrisonedFunc = 0x005a1dc0;
		offsets.switchCharacterFaction = 0x005A2100;
		offsets.switchNamedCharacterFaction = 0x00594870;
		offsets.switchArmyFaction = 0x004F4AF0;
		offsets.changeCharacterTileStuff = 0x005933B0;
		offsets.initPlaceCharacter = 0x00599900;
		offsets.getResidenceCharacterNum = 0x004C0250;
		offsets.getResidenceCharacterAtIndex = 0x004C0440;
		offsets.removeSieges = 0x004bf8d0;
		offsets.doHordeStuff = 0x0050B750;
		offsets.doHordeStuff2 = 0x004EB020;
		offsets.doHordeStuff3 = 0x00C56FC0;
		offsets.saveGame = 0x004350e0;
		
		offsets.createCadAttackSettlementFunc = 0x00005B5A90;
		offsets.createCadAssaultSettlementFunc = 0x005B5CB0;
		offsets.createCadSiegeSettlementFunc = 0x005B5BA0;
		offsets.createCadMovingNormalFunc = 0x005B0540;
		offsets.decideEquipmentFunc = 0x0071D050;
		offsets.getEquipmentPreferencesFunc = 0x00521B10;
		
		offsets.finalizeActionStrat = 0x0059ec70;
		

		offsets.createIdleActionFunc = 0x005b0420;

		offsets.getRandomNameFunc = 0x008DD390;
		offsets.getNameIndexFunc = 0x005A48B0;
		offsets.getCharacterName = 0x008DCA50;
		offsets.respawnOffMapCharacterFunc = 0x008B4E40;
		offsets.setFactionLeaderFunc = 0x004E95F0;
		offsets.findParentForAdoptionFunc = 0x004EAAF0;
		offsets.setArmyRegionEntriesFunc = 0x004A4580;
		offsets.factionResurrectStuffFunc = 0x004F5250;

		offsets.getReadyForMovingFunc = 0x005b0540;
		offsets.getReadyForMoving2Func = 0x005a0d50;
		offsets.getGeneralReadyForMovingFunc = 0x004cda20;

		offsets.getStringFromTable = 0x00CD87D0;


		offsets.makeTrackedPointerToAttackFunc = 0x004e6600;
		offsets.makeCadAttackFunc = 0x005b5900;
		offsets.somethingWithTrackedPointerAttackFunc = 0x004cbc90;
		offsets.makeCadDiplomacyFunc = 0x005B65B0;
		offsets.makeCadDiplomacySettlementFunc = 0x005B66D0;

		//CAMERA
		offsets.moveStratCameraSlowFunc = 0x0098e4c0;
		offsets.moveStratCameraFastFunc = 0x0098e030;
		offsets.zoomStratCameraFunc = 0x0098e220;
		
		offsets.createCadAttackFortFunc = 0x005B5E30;
		offsets.createCadAssaultFortFunc = 0x005B6070;
		offsets.createCadSiegeFortFunc = 0x005B5F50;
		offsets.makeTrackedPointerFortFunc = 0x004E67F0;
		offsets.somethingWithTrackedPointerFortFunc = 0x004EBBC0;


		offsets.teleportCharacterFunc = 0x0059ba70;
		offsets.getTileCharactersFunc = 0x004D05B0;
		offsets.getValidRegionTile = 0x004A5970;
		offsets.isTileValidForCharacter = 0x004CCFD0;
		offsets.deleteMoveExtents = 0x004B67E0;
		offsets.someSelectionStuff = 0x004DB420;
		offsets.clearPaths = 0x4DB1B0;
		

		offsets.replenishUnitFunc = 0x007495a0;
		offsets.setUnitArmorFunc = 0x0073c470;
		offsets.setUnitWeaponFunc = 0x0073c290;


		offsets.createCharacterFunc = 0x008b6750;
		offsets.spawnCreatedObject = 0x004cd800;
		offsets.doSomeWithCharacterFunc = 0x004bb530;

		offsets.createArmyFunc = 0x004f46a0;
		offsets.createArmyInSettlementFunc = 0x005dc290;
		offsets.createUnitFunc = 0x008ee9e0;
		offsets.createUnitFunc2 = 0x8ECFE0;
		offsets.addUnitToArmyFunc = 0x00719ab0;
		offsets.setBodyguard = 0x00599400;
		offsets.makeConditionFunc = 0x00875310;
		offsets.makeBuildingCondition = 0x008A7510;


		offsets.killCharStratMapFunc = 0x00592e00;
		offsets.killUnitStratMapFunc = 0x00741F70;

		offsets.destroyBuildingFunc = 0x005e5380;
		offsets.createBuildingFunc = 0x00d2d050;


		offsets.autoWinFunc = 0x00d28070;
		offsets.historicEventFunc = 0x0C56150;
		offsets.scriptCommandOne = 0x00A95650;
		offsets.scriptCommandTwo = 0x00A945D0;


		offsets.autoResolveFunc = 0x00aa7bf0;


		offsets.useButtonFunc = 0x00b07e00;
		offsets.getUiElementFunc = 0x00b46340;

		offsets.getSkeleton = 0x0740B40;
		offsets.createModelFlexi = 0x0093B610;
		offsets.createModelFlexiShadow = 0x00937220;
		offsets.assignCasToFlexi = 0x0093BCB0;
		offsets.assignShadowCasToFlexi = 0x00934A10;
		offsets.loadStratCharModel = 0x00D4BDA0;
		offsets.readTgaFile = 0x00900210;


		offsets.readEDUEntryFunc = 0x008efd50;
		offsets.deleteFortFuncOne = 0x00bafef0;
		offsets.deleteFortFuncTwo = 0x004bd010;
		offsets.createFortFunc = 0x00714E60;
		offsets.createsFort = 0x8B95F0;
		offsets.addToFortsArray = 0x4BD8C0;
		offsets.mercEOPValue = 0x018C27C8;
		
		offsets.getUnitByLabel = 0x0A95600;
		offsets.getGroupByLabel = 0x00A931C0;
		offsets.getUnitByLabel2 = 0x0A927E0;

		//unit commands
		offsets.unitOrderMove = 0x00A6EB00;
		offsets.unitImmediatePlace = 0x00A6E370;
		offsets.unitAttackClosestUnit = 0x00A79A20;
		offsets.unitAttackUnit = 0x00A6F980;
		offsets.unitDeployStakes = 0x00A78E60;
		offsets.unitChangeFormation = 0x00A700A0;
		offsets.unitHalt = 0x00A6E6E0;
		offsets.getMapHeight = 0x004441B0;
		offsets.unitMoveToOrientation = 0x0A6F1A0;
		offsets.moveRelative = 0x00A7A870;
		offsets.moveToMissileRange = 0x00A70460;
		offsets.unitTurn = 0x00A709A0;
		offsets.unitTaunt = 0x00A73450;
		offsets.useSpecialAbility = 0x00A73690;
		offsets.getBuildingAngle = 0x006EB0F0;
		offsets.attackBuilding = 0x00757EE0;
		offsets.createCADTargetCharacter = 0x00AAC0F0;
		offsets.createCADTargetFort = 0x00AAC1D0;
		offsets.createCADTargetSettlement = 0x00AAC160;
		offsets.executeCADTargetCharacter = 0x00AAE0B0;

		//Group commands
		offsets.groupUnitChangeFormation = 0x00A7A000;
		offsets.groupMoveToRangeOfGroup = 0x00A76920;
		offsets.groupMoveToRangeOfUnit = 0x00A76330;
		offsets.groupAttackGroup = 0x00A77370;
		offsets.groupChangeFormation = 0x00A77880;
		offsets.groupHalt = 0x00A74B20;
		offsets.groupMoveFormed = 0x00A74FB0;
		offsets.groupMoveUnformed = 0x00A75510;
		offsets.groupRelativeMoveFormed = 0x00A75A10;
		offsets.groupRelativeMoveUnformed = 0x00A75EE0;
		offsets.groupTurn = 0x00A77C50;
		offsets.sendCharacterOffMap = 0x004EF1D0;
		offsets.assaultObject = 0x00719670;
		offsets.checkOwnershipCustom = 0x8EEED0;
		offsets.getUnitCard = 0x008EBFD0;
		offsets.addToCardArray = 0x464990;
		offsets.buildWatchtower = 0x0715150;
		
		//offsets.unitPlace = 0x00770A70;
		offsets.unitMove = 0x075A320;
		offsets.gameLogCommand = 0x0A90B20;
		offsets.sabotageBuilding = 0xAAC2B0;
		offsets.setCharacterType = 0x008b4200;
		offsets.addAncillary = 0x005a5d50;
		offsets.getTrait = 0x008b7070;
		offsets.addTrait = 0x005a7bf0;
		offsets.removeTrait = 0x005a5880;
		offsets.removeAncillary = 0x005a5e70;
		offsets.issueMoveOrder = 0x6931C0;
		offsets.issueFormationOrder = 0x692720;
		offsets.repairBuilding = 0x005F8DC0;
		offsets.addBuildingToQueue = 0x005E4600;
		offsets.addUnitToQueue = 0x005E4510;
		offsets.createBuildInSett = 0x008A3CE0;
		offsets.upgradeBuildInSett = 0x005F80A0;
		offsets.upgradeSett = 0x005DBA20;
		offsets.getRecruitmentOptions = 0x005E7EA0;
		offsets.getRetrainingOptions = 0x005E65A0;
		offsets.resetBuildingsMem = 0x5FA710;
		offsets.getConstructionOptions = 0x005E1C00;
		offsets.makeWatchTower = 0x008B97B0;
		offsets.addToWatchtowerList = 0x004DD940;
		offsets.updateBlockades = 0x004DD2F0;
		offsets.createCadDisembark = 0x005B06D0;
		offsets.setCadClass = 0xAAC080;
		offsets.canArmySplit = 0x4D61F0;
		offsets.splitArmy = 0x4F9C90;
		offsets.setUnitMovePoints = 0x00597c60;
		offsets.automateGroup = 0x00721B50;
		offsets.placeGroup = 0x0071E620;
		offsets.clearUnitGroup = 0x0071ECD0;
		offsets.removeUnusedLabel = 0x00A93380;
		offsets.defineUnitGroup = 0x00A92C30;
		offsets.findAncillary = 0x008b1d30;
		offsets.mergeArmies = 0x007155F0;
		
		offsets.stopBlockade = 0x00711a50;
		offsets.stopSiege = 0x00711830;
		offsets.addToSettlement = 0x005e1dc0;
		offsets.addToFort = 0x004bcc20;
		offsets.ungarrisonResidence = 0x004bff50;
		offsets.attackArmy = 0x007195A0;
		offsets.getTileMoveCost = 0x004c7bd0;
		offsets.revealTile = 0x004baea0;
		offsets.setFactionTrade = 0x00503480;
		offsets.setProtectorate = 0x00504F20;
		offsets.isTileFree = 0x004c9220;
		offsets.hideRevealedTile = 0x004baf80;
		offsets.findBattleModel = 0x010e8ee0;
		offsets.isZoneValid = 0x6733A0;
		offsets.getZonePerimeter = 0xE08290;
		offsets.getZoneID = 0xDF0680;
		offsets.createUniString = 0x00f01eb0;
		offsets.getScriptCounter = 0x00489760;
		offsets.setScriptCounter = 0x0048cce0;
		offsets.getUnitFullMovePoints = 0x00742b10;
		offsets.parseCondition = 0x00875310;
		offsets.createMoveExtents = 0x004B6670;
		offsets.createSettlement = 0x005E4970;
		offsets.createRallyPointSundry = 0x005D9140;
		offsets.initRallyPoint = 0x005D91C0;
		offsets.settAttachFaction = 0x005DB4C0;
		offsets.areaOfInfluence = 0x004C9BB0;
		offsets.addToSettlementList = 0x005F0870;
		offsets.residenceTileCharacterCheck = 0x004BFE10;
		offsets.recalculateSettlement = 0x005F45C0;
		offsets.setPop = 0x005F12D0;
		offsets.addToArrayInt = 0x00464BA0;
		offsets.changeWatchTowerFaction = 0x004DD430;
		offsets.setRegionFaction = 0x0049FEC0;
		offsets.clearAiRegionData = 0x00533D80;
		offsets.createRegionInArray = 0x533C80;
		offsets.getRegionIsolationScore = 0x5150B0;
		offsets.calculateRegionStrengths = 0x005166A0;
		offsets.calculateBackupRegionStrengths = 0x00516730;
		offsets.isTrustedAlly = 0x0053CE80;
		offsets.resetRegionGroupControllers = 0x00519310;
		offsets.addNavalTargetRegion = 0x00535130;
		offsets.removeNavalTargetRegion = 0x00535270;
		offsets.createHordeController = 0x00536D50;
		offsets.createRegionController = 0x005504C0;
		offsets.addToRegionControllers = 0x00533E70;
		offsets.releaseResource = 0x00551FE0;
		offsets.getRequiredGarrisonStrength = 0x00515B70;
		offsets.recalcRegionGroupStrengths = 0x519390;
		offsets.manageRegionGroups = 0x534500;
		offsets.initGsdFactionData = 0x533310;
		offsets.clearTransports = 0x00536A10;
		offsets.addGsdRegion = 0x00533BE0;
		offsets.aiCrusadeController = 0x00535DA0;
		offsets.addCampaignController = 0x00545730;
		offsets.createCampaignController = 0x0051B260;
		offsets.sortControllers = 0x00545920;
		offsets.calculateCampaignControllerStrengths = 0x0051C3F0;
		offsets.manageNavalTransports = 0x005378B0;
		offsets.balanceRegionGroups = 0x005379C0;
		offsets.initNavalController = 0x0054ABF0;
		offsets.destroyFaction = 0x55F930;
		offsets.reduceUnitProduction = 0x0055FAA0;
		offsets.reduceEconomicProduction = 0x55FB60;
		offsets.attachResourceSett = 0x4C47F0;
		offsets.updateFowTableObject = 0x4BB8C0;
		offsets.updateFowTable = 0x4BBD20;
		offsets.updateTerrain = 0x98E1C0;
		offsets.updateRadar = 0xBAF1B0;
		offsets.clearMapPath = 0x98E780;
		offsets.canMerge = 0x718380;
		offsets.hasFreeTilesToMove = 0x00596910;
		offsets.canStartSiege = 0x005218E0;
		offsets.canStartAssault = 0x711D60;
		offsets.getNumEnginesCanPenetrateWalls = 0x711440;
		offsets.canSeeCharacter = 0x4D4040;
		offsets.getArmyCoords = 0x7136A0;
		offsets.generalJoinCrusade = 0x5A0F80;
		offsets.generalLeaveCrusade = 0x5A1190;
		offsets.addTraitPoints = 0x5A6BF0;
		offsets.incEventCounter = 0x48CC10;
		offsets.createEventAcceptObj = 0xA80C40;
		offsets.switchFactionLeader = 0x4E95F0;
		offsets.modifyWithSettMechanics = 0x8E6E10;
		offsets.setCharacterName = 0x005A4200;
		offsets.officerDestructor = 0x741220;
		offsets.getEventController = 0x488FD0;
		offsets.deleteScriptEvent = 0x48AED0; // 48AAD0
		offsets.getRelativePath = 0x41BCC0;
		offsets.parseCas = 0xD4BD00;
		offsets.createModelRigid = 0x00934190;
		offsets.appendTexturesFolder = 0xA00760;
		offsets.loadModelRigid = 0x933D50;
		offsets.getCasModel = 0xD45CC0;
		offsets.closeCas = 0xD4BC30;
		offsets.simpleCasDestructor = 0xD4BCD0;
		offsets.createCultureSettlement = 0x8BBF50;
		offsets.releaseCultureSettlement = 0x8BA640;
		offsets.createCultureFort = 0x8BA500;
		offsets.createCultureCasEntry = 0x8B92B0;
		offsets.getEntriesByFaction = 0x8A8CE0;
		offsets.createSettlementConditionContext = 0x00609A40;
		offsets.isBuildingTypePresent = 0x00608D70;
		offsets.canSettlementUpgrade = 0x005DB8D0;
		offsets.isLevelAvailableForLevel = 0x005FE4C0;
		offsets.evaluateBuildingGuard = 0x005FD050;
		offsets.createConstructionOption = 0x005FB160;
		offsets.createConvertSett = 0x005FAFF0;
		offsets.createBuildingSett = 0x005FAF50;
		offsets.buildQueueConflictTest = 0x005EB560;
		offsets.removeBuildingSelection = 0x005FA790;
		offsets.getNextWord = 0xD493C0;
		offsets.modelRigidDestructor = 0x934110;
		offsets.generateSprite = 0xE78C00;
		offsets.clearCharacterStuff = 0x5A0BE0;
		offsets.initGameScriptLog = 0x00D339F0;
		offsets.createSpCountedBase = 0x403B00;
		offsets.createLogger = 0x4227F0;
		offsets.createUnitGroupDeployer = 0x6D6BF0;
		offsets.buildingBattleGetFacing = 0x6EA9D0;
		offsets.findDeployPosition = 0x6D72D0;
		offsets.createSallyForthObjective = 0x51D9C0;
		offsets.freeHashString = 0xD45D40;
		offsets.getMovePath = 0x4D8390;
		offsets.removeTrackedReference = 0xD69580;
		offsets.resetLtgd = 0x53C5C0;
		offsets.setLtgdNavalTarget = 0x53F940;
		offsets.getRegionGroup = 0x519430;
		offsets.getFactionRegionGroupNum = 0x519480;
		offsets.isLoneRegionGroup = 0x5196D0;
		offsets.setCapital = 0x4FBA60;//4FB500
		offsets.setSettlementTaxLevel = 0x52D670;
		offsets.usePersonalityType = 0x530F50;
		offsets.usePersonalityName = 0x5313D0;
		offsets.isMicroManageAll = 0x467AB0;
		offsets.addSettlementPolicy = 0x531CF0;
		offsets.createAiProductionController = 0x52E620;
		offsets.prodSetPriorities = 0x0052E9B0;
		offsets.addToProductionControllers = 0x531C00;
		offsets.ParseDescrMountedEngineEntry = 0x8CDEF0;
		offsets.formatStringAdd = 0xD32A60;
		offsets.getParserFileName = 0xD475D0;
		offsets.getDataInvalidLogger = 0xD32C40;
		offsets.createEnabledLogger = 0x403E00;
		offsets.destroyLogObject = 0x403E90;
		offsets.createMountedEngineRecord = 0x8CEBC0;
		offsets.dbHashTableGet = 0x438130;
		offsets.dbHashTableSet = 0x437BD0;
		offsets.createCharacterRecord = 0x4F4540;
		offsets.birthLog = 0x8337E0;
		offsets.marryCharacters = 0x5A8190;
	}
}
