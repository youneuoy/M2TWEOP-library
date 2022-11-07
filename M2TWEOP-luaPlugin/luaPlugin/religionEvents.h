#pragma once
#include "pluginM2TWEOP.h"
extern "C" PLUGINM2TWEOP_API void onPopeAcceptsCrusadeTarget(void* crusade, settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onCrusadeCalled(void* crusade, settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onCrusadeEnds(void* crusade, settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onPopeRejectsCrusadeTarget(void* crusade, settlementStruct * sett);
extern "C" PLUGINM2TWEOP_API void onArmyTakesCrusadeTarget(void* crusade, settlementStruct * sett, stackStruct * army);
extern "C" PLUGINM2TWEOP_API void onUnitsDesertCrusade(void* crusade, settlementStruct * sett, factionStruct * fac);

extern "C" PLUGINM2TWEOP_API void onPopeElected(factionStruct * fac, factionStruct * fac2);
extern "C" PLUGINM2TWEOP_API void onVotedForPope(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onAssassinCaughtAttackingPope(factionStruct * fac);
extern "C" PLUGINM2TWEOP_API void onInquisitorAppointed(factionStruct * fac, settlementStruct * sett);