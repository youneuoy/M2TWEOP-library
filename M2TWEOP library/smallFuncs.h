#pragma once
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include"headersSTD.h"
#include "headersMEM.h"
#include "realGameTypes.h"

#include "exportHeader.h"
#include "globalData.h"
namespace smallFuncs
{	
	NOINLINE EOP_EXPORT void setAncLimit(unsigned char limit);
	NOINLINE EOP_EXPORT void setEDUUnitsSize(signed short min,signed short max);
	NOINLINE EOP_EXPORT void setMaxBgSize(unsigned char size);
	NOINLINE EOP_EXPORT void unlockConsoleCommands();
	NOINLINE EOP_EXPORT int getBattleCondCode(DWORD condObject);
	std::string getWinConditionS(DWORD condObject);

	NOINLINE EOP_EXPORT void createUniString(UNICODE_STRING**& newUniStringPointer,const char*nonUniStr);


	NOINLINE EOP_EXPORT gameDataAllStruct* getGameDataAll();

	//toggle the highlighting of units on the tactical map.
	NOINLINE EOP_EXPORT void swUnBMapHighlight();

	NOINLINE EOP_EXPORT void setReligionsLimit(unsigned char limit);

	NOINLINE EOP_EXPORT bool isTileFree(int* xy);

	NOINLINE EOP_EXPORT int getScriptCounter(const char*counterName,bool&isFinded);
	NOINLINE EOP_EXPORT void setScriptCounter(const char*counterName,int counterValue);
};

