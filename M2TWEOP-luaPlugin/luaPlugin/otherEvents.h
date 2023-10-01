#pragma once
#include "pluginM2TWEOP.h"
extern "C" PLUGINM2TWEOP_API void onMultiTurnMove(namedCharacter * gen);
extern "C" PLUGINM2TWEOP_API void onSettlementSelected(settlementStruct * sett);

extern "C" PLUGINM2TWEOP_API void onShortcutTriggered(const char* str);
