#include "uiEvents.h"

void onSettlementPanelOpen(settlementStruct* sett)
{
	if (plugData::data.luaAll.onSettlementPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onSettlementPanelOpenFunc)(sett));
	}
}

void onFinancesPanelOpen(factionStruct* fac)
{
	if (plugData::data.luaAll.onFinancesPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onFinancesPanelOpenFunc)(fac));
	}
}

void onFactionSummaryPanelOpen(factionStruct* fac)
{
	if (plugData::data.luaAll.onFactionSummaryPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onFactionSummaryPanelOpenFunc)(fac));
	}
}

void onFamilyTreePanelOpen(factionStruct* fac)
{
	if (plugData::data.luaAll.onFamilyTreePanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onFamilyTreePanelOpenFunc)(fac));
	}
}

void onDiplomaticStandingPanelOpen()
{
	if (plugData::data.luaAll.onDiplomaticStandingPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onDiplomaticStandingPanelOpenFunc)());
	}
}

void onDiplomacyPanelOpen(factionStruct* fac)
{
	if (plugData::data.luaAll.onDiplomacyPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onDiplomacyPanelOpenFunc)(fac));
	}
}

void onPreBattlePanelOpen(factionStruct* fac)
{
	if (plugData::data.luaAll.onPreBattlePanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onPreBattlePanelOpenFunc)(fac));
	}

}

void onNavalAutoResolvePanelOpen()
{
	if (plugData::data.luaAll.onNavalAutoResolvePanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onNavalAutoResolvePanelOpenFunc)());
	}
}

void onCharacterPanelOpen(generalCharacterictics* pers)
{
	if (plugData::data.luaAll.onCharacterPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCharacterPanelOpenFunc)(pers));
	}
}

void onTradePanelOpen(settlementStruct* sett)
{
	if (plugData::data.luaAll.onTradePanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onTradePanelOpenFunc)(sett));
	}
}

void onRequestBuildingAdvice(settlementStruct* sett)
{
	if (plugData::data.luaAll.onRequestBuildingAdviceFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onRequestBuildingAdviceFunc)(sett));
	}
}

void onRequestTrainingAdvice(settlementStruct* sett, EduEntry* recommend)
{
	if (plugData::data.luaAll.onRequestTrainingAdviceFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onRequestTrainingAdviceFunc)(sett, recommend));
	}
}

void onMessageOpen(factionStruct* fac, int type)
{
	if (plugData::data.luaAll.onMessageOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onMessageOpenFunc)(fac, type));
	}
}

void onIncomingMessage(factionStruct* fac, int type)
{
	if (plugData::data.luaAll.onIncomingMessageFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onIncomingMessageFunc)(fac, type));
	}
}

void onMessageClosed(int type)
{
	if (plugData::data.luaAll.onMessageClosedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onMessageClosedFunc)(type));
	}
}

void onButtonPressed(const char* butName)
{
	if (plugData::data.luaAll.onButtonPressedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onButtonPressedFunc)(butName));
	}
}

void onScrollClosed(const char* scrName)
{
	if (plugData::data.luaAll.onScrollClosedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onScrollClosedFunc)(scrName));
	}
}

void onScrollOpened(const char* scrName)
{
	if (plugData::data.luaAll.onScrollOpenedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onScrollOpenedFunc)(scrName));
	}
}

void onUIElementVisible(const char* elName)
{
	if (plugData::data.luaAll.onUIElementVisibleFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onUIElementVisibleFunc)(elName));
	}
}

void onScrollAdviceRequested(const char* scrName)
{
	if (plugData::data.luaAll.onScrollAdviceRequestedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onScrollAdviceRequestedFunc)(scrName));
	}
}

void onSettlementScrollAdviceRequested(settlementStruct* sett, const char* scrName)
{
	if (plugData::data.luaAll.onSettlementScrollAdviceRequestedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onSettlementScrollAdviceRequestedFunc)(sett,scrName));
	}
}

void onPreBattleScrollAdviceRequested()
{
	if (plugData::data.luaAll.onPreBattleScrollAdviceRequestedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onPreBattleScrollAdviceRequestedFunc)());
	}
}

void onNavalPreBattleScrollAdviceRequested()
{
	if (plugData::data.luaAll.onNavalPreBattleScrollAdviceRequestedFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onNavalPreBattleScrollAdviceRequestedFunc)());
	}
}

void onCollegeOfCardinalsPanelOpen(void* college)
{
	if (plugData::data.luaAll.onCollegeOfCardinalsPanelOpenFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCollegeOfCardinalsPanelOpenFunc)((int)college));
	}
}
