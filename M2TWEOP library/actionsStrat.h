#pragma once
#include "headersSTD.h"
#include "realGameTypes.h"
#include "fastFuncts.h"
#include "exportHeader.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
namespace actionsStrat
{
	//Siege helpers
	DWORD getReadySiegeActionSett(DWORD allocatedMem, settlementStruct* sett);
	void getReadySiegeActionChar(DWORD allocatedMem, general* gen);

	//moving helpers
	DWORD getReadyForMoving(DWORD allocatedMem,general* gen,int x, int y);
	void getGeneralReadyForMoving(general* gen);
	
	//helpers
	void finalizeAction(DWORD allocatedMem, general* gen);

	NOINLINE EOP_EXPORT void siegeSettlement(general* gen, settlementStruct* sett);
	NOINLINE EOP_EXPORT void moveNormal(general* gen, int x,int y);
	NOINLINE EOP_EXPORT void attackArmy(general* attacker, general* defender);
};