#pragma once
#include "pluginM2TWEOP.h"
extern "C" PLUGINM2TWEOP_API void onSettlementPanelOpen(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onFinancesPanelOpen(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onFactionSummaryPanelOpen(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onFamilyTreePanelOpen(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onDiplomaticStandingPanelOpen();
extern "C" PLUGINM2TWEOP_API void onDiplomacyPanelOpen(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onPreBattlePanelOpen(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onNavalAutoResolvePanelOpen();
extern "C" PLUGINM2TWEOP_API void onCharacterPanelOpen(namedCharacter * pers);
extern "C" PLUGINM2TWEOP_API void onTradePanelOpen(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onRequestBuildingAdvice(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onRequestTrainingAdvice(settlementStruct * sett, eduEntry * recommend);
extern "C" PLUGINM2TWEOP_API void onRequestTrainingAdvice(settlementStruct * sett, eduEntry * recommend);

extern "C" PLUGINM2TWEOP_API void onMessageOpen(factionStruct * fac, int type);
extern "C" PLUGINM2TWEOP_API void onIncomingMessage(factionStruct * fac, int type);
extern "C" PLUGINM2TWEOP_API void onMessageClosed(int type);

extern "C" PLUGINM2TWEOP_API void onButtonPressed(const char* butName);
extern "C" PLUGINM2TWEOP_API void onScrollClosed(const char* scrName);
extern "C" PLUGINM2TWEOP_API void onScrollOpened(const char* scrName);
extern "C" PLUGINM2TWEOP_API void onUIElementVisible(const char* elName);
extern "C" PLUGINM2TWEOP_API void onScrollAdviceRequested(const char* scrName);

extern "C" PLUGINM2TWEOP_API void onSettlementScrollAdviceRequested(settlementStruct * sett, const char* scrName);

extern "C" PLUGINM2TWEOP_API void onPreBattleScrollAdviceRequested();
extern "C" PLUGINM2TWEOP_API void onNavalPreBattleScrollAdviceRequested();
extern "C" PLUGINM2TWEOP_API void onCollegeOfCardinalsPanelOpen(void* college);