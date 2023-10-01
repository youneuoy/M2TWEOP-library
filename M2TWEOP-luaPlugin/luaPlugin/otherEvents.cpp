#include "otherEvents.h"

void onMultiTurnMove(namedCharacter* gen)
{
	plugData::data.luaAll.onMultiTurnMove(gen);
}

void onSettlementSelected(settlementStruct* sett)
{
	plugData::data.luaAll.onSettlementSelected(sett);
}

void onShortcutTriggered(const char* str)
{
	if (plugData::data.luaAll.onShortcutTriggeredFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onShortcutTriggeredFunc)(str))
	}
}
