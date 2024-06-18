#include "gameHelpers.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include <vector>
#include "smallFuncs.h"
#include "dataOffsets.h"
#include "events.h"

namespace gameHelpers
{
	using namespace std;

	std::string callConsole(const std::string& cmdName, sol::variadic_args va)
	{
		char buffer[100]{};
		buffer[0] = '\0';
		bool isOk = false;
		if (va.size() == 0)
		{
			callGameConsoleCommand(cmdName.c_str(), "", buffer);
		}
		else
		{
			callGameConsoleCommand(cmdName.c_str(), va.begin()->as<std::string>().c_str(), buffer);
		}

		if (isOk == false && buffer[0] == '\0')
		{
			sprintf_s(buffer, "error");
		}
		return buffer;
	}
	
	bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer)
	{
		auto cmd = dataOffsets::offsets.consoleCommands;
		for (int i = 0; i < cmd->size; i++)
		{
			auto currCom = cmd->commands[i];
			if (strcmp(currCom->name, name) != 0)
			{
				continue;
			}
			return (**currCom->function)(arg, errorBuffer);
		}
		return false;
	}
	
	std::tuple<bool, int> getScriptCounter(const char* type)
	{
		bool isExist = false;
		int counterValue = smallFuncs::getScriptCounter(type, isExist);

		return std::make_tuple(isExist, counterValue);
	}
	
	bool condition(const std::string& condition, const eventTrigger* eventData)
	{
		const char* conditionC = condition.c_str();
		return smallFuncs::condition(conditionC, eventData);
	}
	
	void scriptCommand(const std::string& command, sol::variadic_args va)
	{
		const char* commandC = command.c_str();
		if (va.size() == 0)
		{
			smallFuncs::scriptCommand(commandC, "");
		}
		else
		{
			smallFuncs::scriptCommand(commandC, va.begin()->as<std::string>().c_str());
		}
	}

	unit* getSelectedUnitCard(const uiCardManager* cardManager, const int index)
	{
		return cardManager->selectedUnitCards[index]->unit;
	}

	unit* getUnitCard(const uiCardManager* cardManager, const int index)
	{
		return cardManager->unitCards[index]->unit;
	}

	const char* getReligionName2(const int index)
	{
		const auto* religionDb = *reinterpret_cast <religionDatabase**>(dataOffsets::offsets.religionDatabase);
		const wchar_t* name = religionDb->religionNames[index]->string;
		// Determine the size of the required buffer
		const int size = WideCharToMultiByte(CP_UTF8, 0, name, -1, nullptr, 0, nullptr, nullptr);
		if (size == 0) {
			return nullptr;
		}
		// Allocate a buffer for the converted string
		const auto buffer = new char[size];
		if (buffer == nullptr) {
			// Allocation failed
			return nullptr;
		}
		// Convert the string
		WideCharToMultiByte(CP_UTF8, 0, name, -1, buffer, size, nullptr, nullptr);
		// Return the converted string
		return buffer;
	}

	const char* getClimateName2(const int index)
	{
	    const gameDataAllStruct* gameData = gameDataAllHelper::get();
	    const auto stratMap = gameData->stratMap;
		const wchar_t* name = stratMap->climates->climateArray[index].rcString->string;
		// Determine the size of the required buffer
		const int size = WideCharToMultiByte(CP_UTF8, 0, name, -1, nullptr, 0, nullptr, nullptr);
		if (size == 0) {
			return nullptr;
		}
		// Allocate a buffer for the converted string
		const auto buffer = new char[size];
		if (buffer == nullptr) {
			// Allocation failed
			return nullptr;
		}
		// Convert the string
		WideCharToMultiByte(CP_UTF8, 0, name, -1, buffer, size, nullptr, nullptr);
		// Return the converted string
		return buffer;
	}

	const char* getReligionName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto religionName = plugData::data.luaAll.religionNames.find(index);
		if (religionName == plugData::data.luaAll.religionNames.end()) 
			return nullptr;
		return religionName->second;
	}

	const char* getClimateName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto name = plugData::data.luaAll.climateNames.find(index);
		if (name == plugData::data.luaAll.climateNames.end()) 
			return nullptr;
		return name->second;
	}

	const char* getCultureName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto name = plugData::data.luaAll.cultureNames.find(index);
		if (name == plugData::data.luaAll.cultureNames.end()) 
			return nullptr;
		return name->second;
	}

	int getReligionCount()
	{
		const auto* religionDb = *reinterpret_cast <religionDatabase**>(0x016A0B90);
		if (smallFuncs::getGameVersion() == 1)
		{
			religionDb = *reinterpret_cast <religionDatabase**>(0x016E9DC0);
		}
		return religionDb->religionCount;
	}

	int getReligionN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto religionIndex = plugData::data.luaAll.religionIndex.find(name);
		if (religionIndex == plugData::data.luaAll.religionIndex.end()) 
			return -1;
		return religionIndex->second;
	}

	int getClimateN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto index = plugData::data.luaAll.climateIndex.find(name);
		if (index == plugData::data.luaAll.climateIndex.end()) 
			return -1;
		return index->second;
	}

	int getCultureN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto index = plugData::data.luaAll.cultureIndex.find(name);
		if (index == plugData::data.luaAll.cultureIndex.end()) 
			return -1;
		return index->second;
	}

}
