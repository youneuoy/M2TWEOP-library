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
		offsets.finalyzeActionStratmapFunc = 0x0059e790;

		offsets.createIdleActionFunc = 0x005aff40;

		offsets.getRandomNameFunc = 0x008DC920;
		offsets.getNameIndexFunc = 0x005A43D0;
		offsets.getCharacterName = 0x008DBFE0;
		offsets.respawnOffMapCharacterFunc = 0x008B4450;
		offsets.setFactionLeaderFunc = 0x004E8FE0;
		offsets.findParentForAdoptionFunc = 0x004EA4E0;
		offsets.setArmyRegionEntriesFunc = 0x004A40E0;
		offsets.factionRessurectStuffFunc = 0x004F4C80;
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
		offsets.setUnitWeapFunc = 0x0073bb00;


		offsets.createCharacterFunc = 0x008b5d60;
		offsets.spawnCreatedCharacterFunc = 0x004cd240;
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
		offsets.mercEOPValue = 0x0190B928;
		
		offsets.getUnitByLabel = 0x0A945A0;
		offsets.getGroupByLabel = 0x00A945C0;
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
		offsets.executeCADTargetCharacter = 0x00AAD080;
		offsets.getTileCharactersFunc = 0x004D0000;
		offsets.getValidRegionTile = 0x004A5420;
		offsets.isTileValidForCharacter = 0x004CCA10;
		offsets.deleteMoveExtents = 0x004B6250;
		offsets.someSelectionStuff = 0x004DAE80;

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
		
		//offsets.unitPlace = 0x00770A70;
		offsets.unitMove = 0x0759B70;
		offsets.gameLogCommand = 0xA8FAC0;
	}
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
		
		offsets.createCadAttackSettlementFunc = 0x00005B5A90;
		offsets.createCadAssaultSettlementFunc = 0x005B5CB0;
		offsets.createCadSiegeSettlementFunc = 0x005B5BA0;
		offsets.createCadMovingNormalFunc = 0x005B0540;
		offsets.decideEquipmentFunc = 0x0071D050;
		offsets.getEquipmentPreferencesFunc = 0x00521B10;
		
		offsets.finalyzeActionStratmapFunc = 0x0059ec70;
		

		offsets.createIdleActionFunc = 0x005b0420;

		offsets.getRandomNameFunc = 0x008DD390;
		offsets.getNameIndexFunc = 0x005A48B0;
		offsets.getCharacterName = 0x008DCA50;
		offsets.respawnOffMapCharacterFunc = 0x008B4E40;
		offsets.setFactionLeaderFunc = 0x004E95F0;
		offsets.findParentForAdoptionFunc = 0x004EAAF0;
		offsets.setArmyRegionEntriesFunc = 0x004A4580;
		offsets.factionRessurectStuffFunc = 0x004F5250;

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
		

		offsets.replenishUnitFunc = 0x007495a0;
		offsets.setUnitArmorFunc = 0x0073c470;
		offsets.setUnitWeapFunc = 0x0073c290;


		offsets.createCharacterFunc = 0x008b6750;
		offsets.spawnCreatedCharacterFunc = 0x004cd800;
		offsets.doSomeWithCharacterFunc = 0x004bb530;

		offsets.createArmyFunc = 0x004f46a0;
		offsets.createArmyInSettlementFunc = 0x005dc290;
		offsets.createUnitFunc = 0x008ee9e0;
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
		
		//offsets.unitPlace = 0x00770A70;
		offsets.unitMove = 0x075A320;
		offsets.gameLogCommand = 0x0A90B20;
	}
}