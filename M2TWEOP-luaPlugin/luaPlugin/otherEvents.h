#pragma once
#include "pluginM2TWEOP.h"
extern "C" PLUGINM2TWEOP_API void onMultiTurnMove(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onSettlementSelected(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onSettlementUpgraded(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onSettlementConverted(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onCityRiots(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onUngarrisonedSettlement(settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onUngarrisonedFort(fortStruct * frt);

extern "C" PLUGINM2TWEOP_API void onGiveSettlement(settlementStruct * sett, factionStruct * fac, factionStruct * fac2);

extern "C" PLUGINM2TWEOP_API void onOccupySettlement(generalCharacterictics * gen, factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onExterminatePopulation(generalCharacterictics * gen, factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onSackSettlement(generalCharacterictics * gen, factionStruct * fac);

extern "C" PLUGINM2TWEOP_API void onAddedToBuildingQueue(settlementStruct * sett, const char* build);
extern "C" PLUGINM2TWEOP_API void onBuildingDestroyed(settlementStruct * sett, const char* build);
extern "C" PLUGINM2TWEOP_API void onBuildingCompleted(factionStruct * fac, settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onEventCounter(const char* str);



extern "C" PLUGINM2TWEOP_API void onFactionExcommunicated(factionStruct * fac);

extern "C" PLUGINM2TWEOP_API void onDisaster(int eventType);

extern "C" PLUGINM2TWEOP_API void onHordeFormed(factionStruct * fac);


extern "C" PLUGINM2TWEOP_API void onAddedToTrainingQueue(settlementStruct * sett, const char* unitName);
extern "C" PLUGINM2TWEOP_API void onUnitDisbanded(factionStruct * fac, unit * un);

extern "C" PLUGINM2TWEOP_API void onUnitTrained(factionStruct * fac, settlementStruct * sett, unit * un);
extern "C" PLUGINM2TWEOP_API void onAgentCreated(generalCharacterictics * pers, int type, settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onObjSeen(factionStruct * fac, factionStruct * fac2, int x, int y);
extern "C" PLUGINM2TWEOP_API void onTileSeen(factionStruct * fac, int x, int y);


extern "C" PLUGINM2TWEOP_API void onGameReloaded(int something);
extern "C" PLUGINM2TWEOP_API void onTransgression(factionStruct * fac, const char* str, factionStruct * fac2);

extern "C" PLUGINM2TWEOP_API void onGuildUpgraded(settlementStruct * sett, const char* resourceDescription);
extern "C" PLUGINM2TWEOP_API void onGuildDestroyed(settlementStruct * sett, unsigned char guildID);

extern "C" PLUGINM2TWEOP_API void onBrotherAdopted(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onBirth(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onCharacterComesOfAge(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onCharacterMarries(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onCharacterMarriesPrincess(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onCharacterBecomesAFather(generalCharacterictics * gen);

extern "C" PLUGINM2TWEOP_API void onNewAdmiralCreated(generalCharacterictics * pers, settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onShortcutTriggered(const char* str);

extern "C" PLUGINM2TWEOP_API void onBecomesFactionLeader(generalCharacterictics * gen);
extern "C" PLUGINM2TWEOP_API void onBecomesFactionHeir(generalCharacterictics * gen);
