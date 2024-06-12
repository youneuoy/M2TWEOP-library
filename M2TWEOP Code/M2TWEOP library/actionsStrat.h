#pragma once
#include "realGameTypes.h"
#include "character.h"

namespace actionsStrat
{
	DWORD getReadyIdleAction(DWORD allocatedMem);
	//Siege helpers
	DWORD getReadySiegeActionSett(DWORD allocatedMem, settlementStruct* sett);
	void getReadySiegeActionChar(DWORD allocatedMem, character* gen);
	bool blockadePort(stackStruct* fleet, portBuildingStruct* port);
	//moving helpers
	DWORD getReadyForMoving(DWORD allocatedMem,character* gen,int x, int y);
	//helpers
	void finalizeAction(DWORD allocatedMem, character* gen);
	void buildWatchTower(stackStruct* army);
};