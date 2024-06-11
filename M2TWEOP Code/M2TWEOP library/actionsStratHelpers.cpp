
#include "realGameTypes.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include "actionsStrat.h"

namespace actionsStrat 
{
	DWORD getReadyIdleAction(DWORD allocatedMem)
	{
		DWORD adrFunc = codes::offsets.createIdleActionFunc;
		_asm
		{
			push 0x0
			mov ecx, allocatedMem
			mov eax, adrFunc
			call eax

			mov allocatedMem, eax
		}

		return allocatedMem;
	}
	DWORD getReadySiegeActionSett(DWORD allocatedMem,settlementStruct* sett)
	{
		DWORD adrFunc = codes::offsets.createSiegeSettlementGarrisonedFunc;
		_asm 
		{
			push 0x0
			push sett
			mov ecx, allocatedMem
			mov eax, adrFunc
			call eax

			mov allocatedMem,eax
		}

		return allocatedMem;
	}
	void getReadySiegeActionChar(DWORD allocatedMem, general* gen)
	{
		DWORD adrFunc = codes::offsets.getReadySiegeCharacterGarrisonedFunc;
		_asm
		{
			push 0x0
			push allocatedMem
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}

		return;
	}

	void finalizeAction(DWORD allocatedMem, general* gen)
	{
		DWORD adrFunc = codes::offsets.finalyzeActionStratmapFunc;
		_asm
		{
			push allocatedMem
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}

		return;
	}
	DWORD getReadyForMoving(DWORD allocatedMem,general* gen,int x, int y)
	{
		DWORD res = 0;
		DWORD adrFunc = codes::offsets.getReadyForMovingFunc;
		_asm
		{
			push 0
			mov ecx, allocatedMem
			mov eax, adrFunc
			call eax
		}

		adrFunc = codes::offsets.getReadyForMoving2Func;
		_asm
		{
			push 0
			push 0
			push y
			push x
			push allocatedMem
			mov ecx, gen
			mov eax, adrFunc
			call eax
			mov res,eax
		}
	
		return res;
	}
	void getGeneralReadyForMoving(general* gen)
	{
		DWORD adrFunc = codes::offsets.getGeneralReadyForMovingFunc;
		DWORD arg = dataOffsets::offsets.stratMapAllOffsetStart;
		_asm
		{
			push gen
			mov ecx, [arg]
			mov eax, adrFunc
			call eax
		}
	}
}