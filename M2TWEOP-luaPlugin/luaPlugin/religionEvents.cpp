#include "religionEvents.h"

void onPopeAcceptsCrusadeTarget(void* crusade, settlementStruct* sett)
{
	if (plugData::data.luaAll.onPopeAcceptsCrusadeTargetFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onPopeAcceptsCrusadeTargetFunc)((int)crusade, sett));
	}
}

void onCrusadeCalled(void* crusade, settlementStruct* sett)
{
	if (plugData::data.luaAll.onCrusadeCalledFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCrusadeCalledFunc)((int)crusade, sett));
	}
}

void onCrusadeEnds(void* crusade, settlementStruct* sett)
{
	if (plugData::data.luaAll.onCrusadeEndsFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCrusadeEndsFunc)((int)crusade, sett));
	}
}

void onPopeRejectsCrusadeTarget(void* crusade, settlementStruct* sett)
{
	if (plugData::data.luaAll.onPopeRejectsCrusadeTargetFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onPopeRejectsCrusadeTargetFunc)((int)crusade, sett));
	}
}

void onArmyTakesCrusadeTarget(void* crusade, settlementStruct* sett, stackStruct* army)
{
	if (plugData::data.luaAll.onArmyTakesCrusadeTargetFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onArmyTakesCrusadeTargetFunc)((int)crusade, sett, army));
	}
}

void onUnitsDesertCrusade(void* crusade, settlementStruct* sett, factionStruct* fac)
{
	if (plugData::data.luaAll.onUnitsDesertCrusadeFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onUnitsDesertCrusadeFunc)((int)crusade, sett, fac));
	}
}

void onPopeElected(factionStruct* fac, factionStruct* fac2)
{
	if (plugData::data.luaAll.onPopeElectedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onPopeElectedFunc)(fac,fac2));
	}
}

void onVotedForPope(factionStruct* fac)
{
	if (plugData::data.luaAll.onVotedForPopeFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onVotedForPopeFunc)(fac));
	}
}

void onAssassinCaughtAttackingPope(factionStruct* fac)
{
	if (plugData::data.luaAll.onAssassinCaughtAttackingPopeFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onAssassinCaughtAttackingPopeFunc)(fac));
	}
}

void onInquisitorAppointed(factionStruct* fac, settlementStruct* sett)
{
	if (plugData::data.luaAll.onInquisitorAppointedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onInquisitorAppointedFunc)(fac,sett));
	}
}
