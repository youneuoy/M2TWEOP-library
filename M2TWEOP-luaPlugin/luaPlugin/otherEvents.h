#pragma once
#include "pluginM2TWEOP.h"
extern "C" PLUGINM2TWEOP_API void onMultiTurnMove(namedCharacter * gen);
extern "C" PLUGINM2TWEOP_API void onSettlementSelected(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onSettlementUpgraded(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onSettlementConverted(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onCityRiots(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onUngarrisonedSettlement(settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onGiveSettlement(settlementStruct * sett, factionStruct * fac, factionStruct * fac2);

extern "C" PLUGINM2TWEOP_API void onOccupySettlement(namedCharacter * gen, factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onExterminatePopulation(namedCharacter * gen, factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onSackSettlement(namedCharacter * gen, factionStruct * fac);

extern "C" PLUGINM2TWEOP_API void onAddedToBuildingQueue(settlementStruct * sett, const char* build);
extern "C" PLUGINM2TWEOP_API void onBuildingDestroyed(settlementStruct * sett, const char* build);
extern "C" PLUGINM2TWEOP_API void onBuildingCompleted(factionStruct * fac, settlementStruct * sett);


extern "C" PLUGINM2TWEOP_API void onAddedToTrainingQueue(settlementStruct * sett, const char* unitName);

extern "C" PLUGINM2TWEOP_API void onUnitTrained(factionStruct * fac, settlementStruct * sett, unit * un);
extern "C" PLUGINM2TWEOP_API void onAgentCreated(namedCharacter * pers, int type, settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onGuildUpgraded(settlementStruct * sett, const char* resourceDescription);
extern "C" PLUGINM2TWEOP_API void onGuildDestroyed(settlementStruct * sett, unsigned char guildID);

extern "C" PLUGINM2TWEOP_API void onNewAdmiralCreated(namedCharacter * pers, settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onShortcutTriggered(const char* str);
