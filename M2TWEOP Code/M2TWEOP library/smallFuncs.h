#pragma once
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include"headersSTD.h"
#include "headersMEM.h"
#include "realGameTypes.h"

#include "exportHeader.h"
#include "globals.h"
namespace smallFuncs
{
	NOINLINE EOP_EXPORT void setAncLimit(unsigned char limit);
	NOINLINE EOP_EXPORT void setEDUUnitsSize(signed short min, signed short max);
	NOINLINE EOP_EXPORT void setMaxBgSize(unsigned char size);
	NOINLINE EOP_EXPORT void unlockConsoleCommands();
	NOINLINE EOP_EXPORT int getBattleCondCode(DWORD condObject);
	std::string getWinConditionS(DWORD condObject);
	NOINLINE EOP_EXPORT int getGameVersion();
	NOINLINE EOP_EXPORT void saveGame(const char* path);
	NOINLINE EOP_EXPORT void mergeArmies(stackStruct* army, stackStruct* targetArmy);

	NOINLINE EOP_EXPORT void createUniString(UNICODE_STRING**& newUniStringPointer, const char* nonUniStr);
	NOINLINE EOP_EXPORT void changeSettlementName(settlementStruct* sett, const char* newName);
	NOINLINE EOP_EXPORT void changeFactionName(factionStruct* fac, const char* newName);
	NOINLINE EOP_EXPORT UNICODE_STRING** getFactionName(factionStruct* fac);
	NOINLINE EOP_EXPORT void historicEvent(const char* name, const char* title, const char* description);
	DWORD getScriptCommandByName(const char* cmdName);
	NOINLINE EOP_EXPORT void scriptCommand(const char* command, const char* args);
	NOINLINE EOP_EXPORT void changeRegionName(regionStruct* region, const char* newName);
	NOINLINE EOP_EXPORT void changeRebelsName(regionStruct* region, const char* newName);

	NOINLINE EOP_EXPORT gameDataAllStruct* getGameDataAll();

	//toggle the highlighting of units on the tactical map.
	NOINLINE EOP_EXPORT void swUnBMapHighlight();

	NOINLINE EOP_EXPORT void setReligionsLimit(unsigned char limit);

	NOINLINE EOP_EXPORT bool isTileFree(int* xy);

	NOINLINE EOP_EXPORT int getScriptCounter(const char* counterName, bool& isFinded);
	NOINLINE EOP_EXPORT void setScriptCounter(const char* counterName, int counterValue);

	NOINLINE EOP_EXPORT void setBuildingChainLimit(unsigned int limit);

	NOINLINE EOP_EXPORT void setGuildCooldown(unsigned char turns);


	NOINLINE EOP_EXPORT int GetUnitSize();

	float GetMinimumPossibleMovepointsForArmy(stackStruct* army);

};

