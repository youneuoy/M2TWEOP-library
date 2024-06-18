#pragma once
#include <string>
#include "realGameTypes.h"
#include "settlement.h"
#include "character.h"
#include "strategyMap.h"
#include "characterRecord.h"

#include "lua/sol.hpp"
struct eventTrigger;
namespace gameHelpers
{
	UINT32 getFactionsCount();
	std::string callConsole(const std::string& cmdName, sol::variadic_args va);
	factionStruct* getFaction(int index);
	const char* getClimateName2(const int index);
	bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer);
	int getReligionCount();
	int getReligionN(const std::string& name);
	int getClimateN(const std::string& name);
	int getCultureN(const std::string& name);
	bool condition(const std::string& condition, const eventTrigger* eventData);
	std::tuple<bool, int> getScriptCounter(const char* type);
	void scriptCommand(const std::string& command, sol::variadic_args va);
	const char* getReligionName(const int index);
	const char* getCultureName(const int index);
	const char* getClimateName(const int index);
	const char* getReligionName2(const int index);
	unit* getSelectedUnitCard(const uiCardManager* cardManager, const int index);
	unit* getUnitCard(const uiCardManager* cardManager, const int index);
};

