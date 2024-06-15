#include "actionsStrat.h"

#include "dataOffsets.h"
#include "functionsOffsets.h"
#include "character.h"
#include "characterRecord.h"
#include "faction.h"

namespace actionsStrat {
	
	bool blockadePort(stackStruct* fleet, portBuildingStruct* port)
	{
		if (!fleet || !port )
			return false;
		int success = 0;
		DWORD funcAddr = codes::offsets.assaultObject;
		_asm
		{
			push 0
			push port
			mov ecx, fleet
			mov eax, funcAddr
			call eax
			mov success, eax
		}
		return success != 0;
	}
	
	void buildWatchTower(stackStruct* army)
	{
		if (!army->gen || !army->gen->characterRecord || !army->gen->characterRecord->faction)
			return;
		int cultureID = army->gen->characterRecord->faction->cultureID;
		culturesDB* cultures = reinterpret_cast<culturesDB*>(dataOffsets::offsets.cultureDatabase);
		int cost = cultures->cultures[cultureID].watchTowerCost;
		if (army->gen->characterRecord->faction->money < cost)
			return;
		DWORD funcAddr = codes::offsets.buildWatchtower;
		_asm
		{
			mov ecx, army
			mov eax, funcAddr
			call eax
		}
	}
}
