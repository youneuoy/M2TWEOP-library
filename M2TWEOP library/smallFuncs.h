#pragma once
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include"headersSTD.h"
#include "headersMEM.h"
#include "realGameTypes.h"
#include "techFuncs.h"
#include "exportHeader.h"
#include "globals.h"
namespace smallFuncs
{
	NOINLINE EOP_EXPORT void setAncLimit(unsigned char limit);
	NOINLINE EOP_EXPORT void setMaxBgSize(unsigned char size);
	NOINLINE EOP_EXPORT void unlockConsoleCommands();
	NOINLINE EOP_EXPORT int getBattleCondCode(DWORD condObject);


	NOINLINE EOP_EXPORT gameDataAllStruct* getGameDataAll();
};

