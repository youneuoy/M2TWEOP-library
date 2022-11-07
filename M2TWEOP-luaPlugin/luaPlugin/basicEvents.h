#pragma once
#include "pluginM2TWEOP.h"


extern "C" PLUGINM2TWEOP_API void onReadGameDbsAtStart();
extern "C" PLUGINM2TWEOP_API void onEndSiege(int x, int y);
extern "C" PLUGINM2TWEOP_API void onStartSiege(int x, int y);


extern "C" PLUGINM2TWEOP_API void onClickAtTile(int x,int y);
extern "C" PLUGINM2TWEOP_API std::string * onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
extern "C" PLUGINM2TWEOP_API int onfortificationlevelS(settlementStruct* settlement, bool* isCastle, bool* isChanged);
extern "C" PLUGINM2TWEOP_API void onChangeTurnNum(int num);
extern "C" PLUGINM2TWEOP_API void onNewGameStart();
extern "C" PLUGINM2TWEOP_API void onCampaignMapLoaded();
extern "C" PLUGINM2TWEOP_API void onLoadGamePl(std::vector<std::string>* saveFiles);
extern "C" PLUGINM2TWEOP_API std::vector<std::string>*onSaveGamePl(UNICODE_STRING * *&savePath);



extern "C" PLUGINM2TWEOP_API void onPreFactionTurnStart(factionStruct* fac);
extern "C" PLUGINM2TWEOP_API void onFactionTurnStart(factionStruct* fac);
extern "C" PLUGINM2TWEOP_API void onFactionTurnEnd(factionStruct* fac);
extern "C" PLUGINM2TWEOP_API void onFactionNewCapital(factionStruct* fac);
extern "C" PLUGINM2TWEOP_API void onFactionWarDeclared(factionStruct* fac, factionStruct * fac2);
extern "C" PLUGINM2TWEOP_API void onFactionAllianceDeclared(factionStruct* fac, factionStruct * fac2);
extern "C" PLUGINM2TWEOP_API void onFactionTradeAgreementMade(factionStruct* fac, factionStruct * fac2);
extern "C" PLUGINM2TWEOP_API void onFactionBreakAlliance(factionStruct* fac, factionStruct * fac2);

extern "C" PLUGINM2TWEOP_API void onGiveMoney(factionStruct* fac, factionStruct * fac2, float amount);
extern "C" PLUGINM2TWEOP_API void onUpdateAttitude(factionStruct* fac, factionStruct * fac2);
extern "C" PLUGINM2TWEOP_API void onDemeanour(factionStruct * fac, factionStruct * fac2, float amount);

extern "C" PLUGINM2TWEOP_API void onCharacterSelected(generalCharacterictics * gen);

extern "C" PLUGINM2TWEOP_API void onGeneralAssaultsGeneral(generalCharacterictics * gen, generalCharacterictics * gen2);

//if sett==null, then you need use frt, else use sett
extern "C" PLUGINM2TWEOP_API void onGeneralAssaultsResidence(generalCharacterictics * gen, settlementStruct * sett,fortStruct* frt);
extern "C" PLUGINM2TWEOP_API void onGeneralCaptureSettlement(generalCharacterictics * gen, settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onGeneralCaptureResidence(generalCharacterictics * gen);

//if sett==null, then you need use frt, else use sett
extern "C" PLUGINM2TWEOP_API void onSiegeEquipmentCompleted(settlementStruct* sett, fortStruct* frt);
extern "C" PLUGINM2TWEOP_API void onPostBattle(generalCharacterictics* gen);

