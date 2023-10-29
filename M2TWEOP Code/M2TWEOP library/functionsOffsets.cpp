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

		offsets.createSiegeSettlementGarrisonedFunc = 0x005b56c0;
		offsets.getReadySiegeCharacterGarrisonedFunc = 0x005a18e0;
		offsets.finalyzeActionStratmapFunc = 0x0059e790;

		offsets.createIdleActionFunc = 0x005aff40;


		offsets.getReadyForMovingFunc = 0x005b0060;
		offsets.getReadyForMoving2Func = 0x005a0870;
		offsets.getGeneralReadyForMovingFunc = 0x004cd460;

		offsets.getStringFromTable = 0x00CDE400;

		offsets.makeTrackedPointerToAttackFunc = 0x004e6000;
		offsets.makeCadAttackFunc = 0x005b5420;
		offsets.somethingWithTrackedPointerAttackFunc = 0x004cb6d0;



		//CAMERA
		offsets.moveStratCameraSlowFunc = 0x0098d940;
		offsets.moveStratCameraFastFunc = 0x0098d4b0;
		offsets.zoomStratCameraFunc = 0x0098d6a0;


		offsets.teleportCharacterFunc = 0x0059b580;



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
	}
	else if (gameVer == 2) //steam
	{
		offsets.stringCryptFunc = 0x00d47330;
		offsets.loadStratCasModelFunc = 0x00a04f00;
		offsets.allocMemFunc = 0x011d318a;

		offsets.createSiegeSettlementGarrisonedFunc = 0x005b5ba0;
		offsets.getReadySiegeCharacterGarrisonedFunc = 0x005a1dc0;
		offsets.finalyzeActionStratmapFunc = 0x0059ec70;

		offsets.createIdleActionFunc = 0x005b0420;



		offsets.getReadyForMovingFunc = 0x005b0540;
		offsets.getReadyForMoving2Func = 0x005a0d50;
		offsets.getGeneralReadyForMovingFunc = 0x004cda20;

		offsets.getStringFromTable = 0x00CD87D0;


		offsets.makeTrackedPointerToAttackFunc = 0x004e6600;
		offsets.makeCadAttackFunc = 0x005b5900;
		offsets.somethingWithTrackedPointerAttackFunc = 0x004cbc90;

		//CAMERA
		offsets.moveStratCameraSlowFunc = 0x0098e4c0;
		offsets.moveStratCameraFastFunc = 0x0098e030;
		offsets.zoomStratCameraFunc = 0x0098e220;


		offsets.teleportCharacterFunc = 0x0059ba70;

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
	}
}