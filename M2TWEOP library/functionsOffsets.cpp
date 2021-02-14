#include "pch.h"
#include "functionsOffsets.h"
codes::offsetsT codes::offsets;
void codes::initCodes(int gameVer)
{
	if (gameVer == 1)
	{
		offsets.stringCryptFunc = 0x00d4cfd0;
		offsets.loadStratCasModelFunc = 0x00a04380;
		offsets.allocMemFunc = 0x012418d8;

		offsets.createSiegeSettlementGarrisonedFunc = 0x005b56c0;
		offsets.getReadySiegeCharacterGarrisonedFunc = 0x005a18e0;
		offsets.finalyzeActionStratmapFunc = 0x0059e790;


		offsets.getReadyForMovingFunc = 0x005b0060;
		offsets.getReadyForMoving2Func = 0x005a0870;
		offsets.getGeneralReadyForMovingFunc = 0x004cd460;


		offsets.makeTrackedPointerToAttackFunc = 0x004e6000;
		offsets.makeCadAttackFunc = 0x005b5420;
		offsets.somethingWithTrackedPointerAttackFunc = 0x004cb6d0;



		//CAMERA
		offsets.moveStratCameraSlowFunc = 0x0098d940;
		offsets.moveStratCameraFastFunc = 0x0098d4b0;
		offsets.zoomStratCameraFunc = 0x0098d6a0;


		offsets.teleportCharacterFunc = 0x0059ba70;



		offsets.replenishUnitFunc = 0x00748e20;
		offsets.setUnitArmorFunc = 0x0073bce0;
		offsets.setUnitWeapFunc = 0x0073bb00;


		offsets.createCharacterFunc = 0x008b5d60;
		offsets.spawnCreatedCharacterFunc = 0x004cd240;
		offsets.doSomeWithCharacterFunc = 0x004bafa0;

		offsets.createArmyFunc = 0x004f40d0;
		offsets.createUnitFunc = 0x008edf60;
		offsets.addUnitToArmyFunc = 0x00719380;
		offsets.setBodyguard = 0x00598f10;

		offsets.killCharStratMapFunc = 0x00592900;
		offsets.killUnitStratMapFunc = 0x007417e0;
	}
	else if (gameVer == 2)
	{
		offsets.stringCryptFunc = 0x00d47330;
		offsets.loadStratCasModelFunc = 0x00a04f00;
		offsets.allocMemFunc = 0x011d318a;

		offsets.createSiegeSettlementGarrisonedFunc = 0x005b5ba0;
		offsets.getReadySiegeCharacterGarrisonedFunc = 0x005a1dc0;
		offsets.finalyzeActionStratmapFunc = 0x0059ec70;



		offsets.getReadyForMovingFunc = 0x005b0540;
		offsets.getReadyForMoving2Func = 0x005a0d50;
		offsets.getGeneralReadyForMovingFunc = 0x004cda20;


		offsets.makeTrackedPointerToAttackFunc = 0x004e6600;
		offsets.makeCadAttackFunc = 0x005b5900;
		offsets.somethingWithTrackedPointerAttackFunc = 0x004cbc90;

		//CAMERA
		offsets.moveStratCameraSlowFunc = 0x0098e4c0;
		offsets.moveStratCameraFastFunc = 0x0098e030;
		offsets.zoomStratCameraFunc = 0x0098e220;


		offsets.teleportCharacterFunc = 0x0059b580;


		offsets.replenishUnitFunc = 0x007495a0;
		offsets.setUnitArmorFunc = 0x0073c470;
		offsets.setUnitWeapFunc = 0x0073c290;


		offsets.createCharacterFunc = 0x008b6750;
		offsets.spawnCreatedCharacterFunc = 0x004cd800;
		offsets.doSomeWithCharacterFunc = 0x004bb530;

		offsets.createArmyFunc = 0x004f46a0;
		offsets.createUnitFunc = 0x008ee9e0;
		offsets.addUnitToArmyFunc = 0x00719ab0;
		offsets.setBodyguard = 0x00599400;


		offsets.killCharStratMapFunc = 0x00592e00;
		offsets.killUnitStratMapFunc = 0x00741F70;
		offsets.killUnitStratMapFunc = 0x00741F70;

	}
}