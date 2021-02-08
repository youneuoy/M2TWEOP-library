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


}