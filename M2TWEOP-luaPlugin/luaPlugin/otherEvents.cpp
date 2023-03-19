#include "otherEvents.h"

void onMultiTurnMove(namedCharacter* gen)
{
	plugData::data.luaAll.onMultiTurnMove(gen);
}

void onSettlementSelected(settlementStruct* sett)
{
	plugData::data.luaAll.onSettlementSelected(sett);
}

void onSettlementUpgraded(settlementStruct* sett)
{
	plugData::data.luaAll.onSettlementUpgraded(sett);
}

void onSettlementConverted(settlementStruct* sett)
{
	plugData::data.luaAll.onSettlementConverted(sett);
}

void onCityRiots(settlementStruct* sett)
{
	if (plugData::data.luaAll.onCityRiotsFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCityRiotsFunc)(sett));
	}
}

void onUngarrisonedSettlement(settlementStruct* sett)
{
	if (plugData::data.luaAll.onUngarrisonedSettlementFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onUngarrisonedSettlementFunc)(sett));
	}
}

void onGiveSettlement(settlementStruct* sett, factionStruct* fac, factionStruct* fac2)
{
	if (plugData::data.luaAll.onGiveSettlementFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onGiveSettlementFunc)(sett, fac, fac2));
	}
}

void onOccupySettlement(namedCharacter* gen, factionStruct* fac)
{
	if (plugData::data.luaAll.onOccupySettlementFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onOccupySettlementFunc)(gen, fac));
	}
}

void onExterminatePopulation(namedCharacter* gen, factionStruct* fac)
{
	if (plugData::data.luaAll.onExterminatePopulationFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onExterminatePopulationFunc)(gen, fac));
	}
}

void onSackSettlement(namedCharacter* gen, factionStruct* fac)
{
	if (plugData::data.luaAll.onSackSettlementFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onSackSettlementFunc)(gen, fac));
	}
}

void onAddedToBuildingQueue(settlementStruct* sett, const char* build)
{
	if (plugData::data.luaAll.onAddedToBuildingQueueFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onAddedToBuildingQueueFunc)(sett, build));
	}
}

void onBuildingDestroyed(settlementStruct* sett, const char* build)
{
	if (plugData::data.luaAll.onBuildingDestroyedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onBuildingDestroyedFunc)(sett, build));
	}
}

void onBuildingCompleted(factionStruct* fac, settlementStruct* sett)
{
	if (plugData::data.luaAll.onBuildingCompletedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onBuildingCompletedFunc)(fac, sett));
	}
}

void onAddedToTrainingQueue(settlementStruct* sett, const char* unitName)
{
	if (plugData::data.luaAll.onAddedToTrainingQueueFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onAddedToTrainingQueueFunc)(sett, unitName));
	}
}

void onUnitTrained(factionStruct* fac, settlementStruct* sett, unit* un)
{
	if (plugData::data.luaAll.onUnitTrainedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onUnitTrainedFunc)(fac, sett, un));
	}
}

void onAgentCreated(namedCharacter* pers, int type, settlementStruct* sett)
{
	if (plugData::data.luaAll.onAgentCreatedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onAgentCreatedFunc)(pers, type, sett));
	}
}

void onGuildUpgraded(settlementStruct* sett, const char* resourceDescription)
{
	if (plugData::data.luaAll.onGuildUpgradedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onGuildUpgradedFunc)(sett, resourceDescription))
	}
}

void onGuildDestroyed(settlementStruct* sett, unsigned char guildID)
{
	if (plugData::data.luaAll.onGuildDestroyedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onGuildDestroyedFunc)(sett, guildID))
	}
}

void onNewAdmiralCreated(namedCharacter* pers, settlementStruct* sett)
{
	if (plugData::data.luaAll.onNewAdmiralCreatedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onNewAdmiralCreatedFunc)(pers, sett))
	}
}

void onShortcutTriggered(const char* str)
{
	if (plugData::data.luaAll.onShortcutTriggeredFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onShortcutTriggeredFunc)(str))
	}
}
