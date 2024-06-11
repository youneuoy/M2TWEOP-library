#pragma once
#include "headersSTD.h"
#include "realGameTypes.h"
namespace smallFuncs
{
	void* GetMainStratObject(void*baseObj);
	void setAncLimit(unsigned char limit);
	void setEDUUnitsSize(signed short min, signed short max);
	void setMaxBgSize(unsigned char size);
	void unlockConsoleCommands();
	int getBattleCondCode(DWORD condObject);
	std::string getWinConditionS(DWORD condObject);
	int getGameVersion();
	void saveGame(const char* path);

	void createUniString(UNICODE_STRING**& newUniStringPointer, const char* nonUniStr);
	void changeSettlementName(settlementStruct* sett, const char* newName);
	void changeFactionName(factionStruct* fac, const char* newName);
	UNICODE_STRING** getFactionName(factionStruct* fac);
	void historicEvent(const char* name, const char* title, const char* description);
	DWORD getScriptCommandByName(const char* cmdName);
	void scriptCommand(const char* command, const char* args);
	void changeRegionName(regionStruct* region, const char* newName);
	void changeRebelsName(regionStruct* region, const char* newName);
	bool condition(const char* condition, const eventTrigger* eventData);

	gameDataAllStruct* getGameDataAll();

	//toggle the highlighting of units on the tactical map.
	void toggleUnitHighlight();
	battleCameraStruct* getBattleCamCoords();
	stratMap* getStratMap();

	void setReligionsLimit(unsigned char limit);

	bool isTileFree(int* xy);

	int getScriptCounter(const char* counterName, bool& isFinded);
	void setScriptCounter(const char* counterName, int counterValue);

	void setBuildingChainLimit(unsigned int limit);

	void setGuildCooldown(unsigned char turns);


	int GetUnitSize();

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

