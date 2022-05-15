#include "actionsStrat.h"
namespace actionsStrat {

	NOINLINE EOP_EXPORT void siegeSettlement(general* gen, settlementStruct* sett)
	{
		DWORD mem = fastFuncts::allocateGameMem(0x3024);
		if (mem == 0)return;

		mem = getReadySiegeActionSett(mem, sett);
		getReadySiegeActionChar(mem, gen);
		finalizeAction(mem, gen);
	}

	NOINLINE EOP_EXPORT void moveNormal(general* gen, int x, int y)
	{
		DWORD mem = fastFuncts::allocateGameMem(0x3024);
		if (mem == 0)return;

		getReadyForMoving(mem, gen,x,y);
		getGeneralReadyForMoving(gen);
		finalizeAction(mem, gen);

	}

	NOINLINE EOP_EXPORT void attackCharacter(general* attacker, general* defender)
	{

		if (attacker->armyLeaded == nullptr || defender->armyLeaded == nullptr)
		{
			return;
		}

		DWORD mem = fastFuncts::allocateGameMem(0x302c);
		if (mem == 0)return;



		int trackedObject[2]{0};

		DWORD makeTrackedPointerToAttackFunc= codes::offsets.makeTrackedPointerToAttackFunc;
		DWORD makeCadAttackFunc = codes::offsets.makeCadAttackFunc;
		DWORD somethingWithTrackedPointerAttackFunc = codes::offsets.somethingWithTrackedPointerAttackFunc;

		_asm
		{
			push defender
			lea ecx, trackedObject
			mov eax, makeTrackedPointerToAttackFunc
			call eax


			push 0
			push eax
			push 0
			mov ecx,mem
			mov eax, makeCadAttackFunc
			call eax

			lea ecx, trackedObject
			mov eax, somethingWithTrackedPointerAttackFunc
			call eax
		}
		finalizeAction(mem, attacker);


	}


}