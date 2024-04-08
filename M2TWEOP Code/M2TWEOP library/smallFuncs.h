#pragma once
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include "headersSTD.h"
#include "headersMEM.h"
#include "realGameTypes.h"

#include "exportHeader.h"
#include "globals.h"
namespace smallFuncs
{
	void* GetMainStratObject(void*baseObj);
	NOINLINE EOP_EXPORT void setAncLimit(unsigned char limit);
	NOINLINE EOP_EXPORT void setEDUUnitsSize(signed short min, signed short max);
	NOINLINE EOP_EXPORT void setMaxBgSize(unsigned char size);
	NOINLINE EOP_EXPORT void unlockConsoleCommands();
	NOINLINE EOP_EXPORT int getBattleCondCode(DWORD condObject);
	std::string getWinConditionS(DWORD condObject);
	NOINLINE EOP_EXPORT int getGameVersion();
	NOINLINE EOP_EXPORT void saveGame(const char* path);

	NOINLINE EOP_EXPORT void createUniString(UNICODE_STRING**& newUniStringPointer, const char* nonUniStr);
	NOINLINE EOP_EXPORT void changeSettlementName(settlementStruct* sett, const char* newName);
	NOINLINE EOP_EXPORT void changeFactionName(factionStruct* fac, const char* newName);
	NOINLINE EOP_EXPORT UNICODE_STRING** getFactionName(factionStruct* fac);
	NOINLINE EOP_EXPORT void historicEvent(const char* name, const char* title, const char* description);
	DWORD getScriptCommandByName(const char* cmdName);
	NOINLINE EOP_EXPORT void scriptCommand(const char* command, const char* args);
	NOINLINE EOP_EXPORT void changeRegionName(regionStruct* region, const char* newName);
	NOINLINE EOP_EXPORT void changeRebelsName(regionStruct* region, const char* newName);
	NOINLINE EOP_EXPORT bool condition(const char* condition, const eventTrigger* eventData);

	NOINLINE EOP_EXPORT gameDataAllStruct* getGameDataAll();

	//toggle the highlighting of units on the tactical map.
	NOINLINE EOP_EXPORT void swUnBMapHighlight();
	NOINLINE EOP_EXPORT battleCameraStruct* getBattleCamCoords();

	NOINLINE EOP_EXPORT void setReligionsLimit(unsigned char limit);

	NOINLINE EOP_EXPORT bool isTileFree(int* xy);

	NOINLINE EOP_EXPORT int getScriptCounter(const char* counterName, bool& isFinded);
	NOINLINE EOP_EXPORT void setScriptCounter(const char* counterName, int counterValue);

	NOINLINE EOP_EXPORT void setBuildingChainLimit(unsigned int limit);

	NOINLINE EOP_EXPORT void setGuildCooldown(unsigned char turns);


	NOINLINE EOP_EXPORT int GetUnitSize();

	float GetMinimumPossibleMovepointsForArmy(stackStruct* army);
	float GetDistanceInTiles(int x, int y, int destX, int destY);
};


struct fakeTextInput
{
	explicit fakeTextInput(const char* condition, size_t start)
	{
		const std::string scriptPath = "luaCommand";
		unicodePointerPointer = new UNICODE_STRING*;
		smallFuncs::createUniString(unicodePointerPointer, scriptPath.c_str());
		textBuffer = condition;
		byteSize = start;
		endString = condition + strlen(condition);
		currRead = condition;
		currLine = condition;
		lineNumber = 1;
		N1814981889 = 1814981889;
		N0 = 0;
		classPointer = 0;
	}
	const char* textBuffer; //0x0000
	uint32_t byteSize; //0x0004
	const char* endString; //0x0008
	UNICODE_STRING** unicodePointerPointer; //0x000C
	const char* currRead; //0x0010
	const char* currLine; //0x0014
	uint32_t lineNumber; //0x0018
	int32_t N1814981889; //0x001C
	int32_t N0; //0x0020
	DWORD classPointer; //0x0024
}; //Size: 0x0028

