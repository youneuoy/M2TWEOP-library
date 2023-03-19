#include "religionEvents.h"


void onAssassinCaughtAttackingPope(factionStruct* fac)
{
	if (plugData::data.luaAll.onAssassinCaughtAttackingPopeFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onAssassinCaughtAttackingPopeFunc)(fac));
	}
}
