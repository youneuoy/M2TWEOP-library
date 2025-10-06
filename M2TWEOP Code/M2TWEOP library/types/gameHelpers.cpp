///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "gameHelpers.h"

#include "campaign.h"
#include "luaPlugin.h"
#include "dataOffsets.h"
#include "events.h"
#include "globals.h"
#include "MemWork.h"
#include "techFuncs.h"

bool m2tweopOptions::hideUnknownUnitTooltips = false;
bool m2tweopOptions::eopHandleUnitCards = true;
bool m2tweopOptions::enableFamilyEventsForTeutonic = true;
bool m2tweopOptions::useEopFrontiers = true;
int m2tweopOptions::watchTowerRange = 10;
uint8_t m2tweopOptions::khakiTextRed = 0x80;
uint8_t m2tweopOptions::khakiTextGreen = 0x77;
uint8_t m2tweopOptions::khakiTextBlue = 0x61;

scriptCommand::scriptCommand(const char* name) : className(name)
{
	this->vftable = dataOffsets::offsets.scriptCommandVFT;
}

eopLogging* eopLogging::m_Instance = new eopLogging();

char* descrParser::getFileName()
{
	return GAME_FUNC(char*(__thiscall*)(descrParser*), getParserFileName)(this);
}

void eopLogging::createEopLoggers()
{
	eopLog = techFuncs::createGameClass<boostLogger>();
	basicStringGame target{};
	target.setString(eopLogName);
	GAME_FUNC(boostLogger*(__thiscall*)(boostLogger*, basicStringGame*), createLogger)(eopLog, &target);
	eopLuaLog = techFuncs::createGameClass<boostLogger>();
	basicStringGame target2{};
	target2.setString(eopLuaLogName);
	GAME_FUNC(boostLogger*(__thiscall*)(boostLogger*, basicStringGame*), createLogger)(eopLuaLog, &target2);
	m_LoggersCreated = true;
}

namespace gameHelpers
{
	using namespace std;

	std::string callConsole(const std::string& cmdName, sol::variadic_args va)
	{
		char buffer[100]{};
		buffer[0] = '\0';
		if (va.size() == 0)
			callGameConsoleCommand(cmdName.c_str(), "", buffer);
		else
			callGameConsoleCommand(cmdName.c_str(), va.begin()->as<std::string>().c_str(), buffer);
		if (buffer[0] == '\0')
			sprintf_s(buffer, "error");
		return buffer;
	}
	
	bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer)
	{
		if (!name)
			return false;
		const auto cmd = dataOffsets::offsets.consoleCommands;
		for (int i = 0; i < cmd->size; i++)
		{
			const auto currCom = cmd->commands[i];
			if (strcmp(currCom->name, name) != 0)
				continue;
			return (**currCom->function)(arg, errorBuffer);
		}
		logStringGame("Command not found: " + std::string(name));
		return false;
	}
	
	int getScriptCounter(const char* counterName, bool& success)
	{
		if (!counterName)
			return 0;
		const auto eventsObject = reinterpret_cast<countersObjectS*>(dataOffsets::offsets.scriptCounters);
		stringWithHash* cryptS = gameStringHelpers::createHashedStringGame(counterName);
		const auto retS = GAME_FUNC(counterS*(__thiscall*)(countersObjectS*, stringWithHash*), getScriptCounter)(eventsObject, cryptS);
		gameStringHelpers::freeHashString(cryptS);
		if (retS == eventsObject->testCounterSValue)
			success = false;
		else
		{
			if (retS != nullptr)
			{
				if (retS->nameCrypt == cryptS->hash && retS->counterName && strcmp(retS->counterName, counterName) == 0)
				{
					success = true;
					return retS->counterValue;
				}
				success = false;
			}
			else
				success = false;
		}
		return 0;
	}
	
	void setScriptCounter(const char* counterName, const int counterValue)
	{
		if (!counterName)
			return;
		bool isExist = false;
		const int value = getScriptCounter(counterName, isExist);
		if (!isExist)
		{
			const std::string commandArgs = std::string(counterName) + " " + std::to_string(counterValue);
			scriptCommand("set_event_counter", commandArgs.c_str());
			return;
		}
		if (value == counterValue)
			return;
		const DWORD eventsObject = dataOffsets::offsets.scriptCountersSet;
		const auto hashedName = gameStringHelpers::createHashedStringGame(counterName);
		GAME_FUNC(void(__thiscall*)(DWORD, stringWithHash*, int), setScriptCounter)(eventsObject, hashedName, counterValue);
		gameStringHelpers::freeHashString(hashedName);
	}
	
	std::tuple<bool, int> getScriptCounterLua(const char* type)
	{
		bool isExist = false;
		int counterValue = getScriptCounter(type, isExist);
		return std::make_tuple(isExist, counterValue);
	}
	
	int getScriptCounterNoBool(const char* type)
	{
		bool isExist = false;
		const int counterValue = getScriptCounter(type, isExist);
		return isExist ? counterValue : 0;
	}
	
	bool conditionLua(const std::string& condition, const eventTrigger* eventData)
	{
		const char* conditionC = condition.c_str();
		return gameHelpers::condition(conditionC, eventData);
	}

	int getGameVersion()
	{
		return globals::dataS.gameVersion;
	}

	void generateSprite(const std::string& model)
	{
		void* unitSprites = nullptr;
		return GAME_FUNC(void(__thiscall*)(void*, const char*), generateSprite)(unitSprites, model.c_str());
	}

	void scriptCommandLua(const std::string& command, sol::variadic_args va)
	{
		const char* commandC = command.c_str();
		if (va.size() == 0)
			scriptCommand(commandC, "");
		else
			scriptCommand(commandC, va.begin()->as<std::string>().c_str());
	}
	
	void saveGame(const char* path)
	{
		DWORD funcAddr = codes::offsets.saveGame;
		DWORD offS = dataOffsets::offsets.saveGameHandler;
		auto uni = new UNICODE_STRING*;
		gameStringHelpers::createUniString(uni, path);
		UNICODE_STRING*** ptrUni = &uni;
		_asm
		{
			push 0
			push ptrUni
			mov ecx, offS
			mov eax, funcAddr
			call eax
		}
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
	    const auto stratMap = stratMapHelpers::getStratMap();
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
		if (!plugData::data.luaAll.hashLoaded || plugData::data.luaAll.religionNames.empty())
			plugData::data.luaAll.fillHashMaps();
		const auto religionName = plugData::data.luaAll.religionNames.find(index);
		if (religionName == plugData::data.luaAll.religionNames.end())
		{
			logStringGame("getReligionName: Could not find religion name for index " + std::to_string(index));
			return nullptr;
		}
		return religionName->second;
	}

	const char* getClimateName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded || plugData::data.luaAll.climateNames.empty())
			plugData::data.luaAll.fillHashMaps();
		const auto name = plugData::data.luaAll.climateNames.find(index);
		if (name == plugData::data.luaAll.climateNames.end())
		{
			logStringGame("getClimateName: Could not find climate name for index " + std::to_string(index));
			return nullptr;
		}
		return name->second;
	}

	const char* getCultureName(const int index)
	{
		if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.cultureNames.empty())
			plugData::data.luaAll.fillHashMapsNonCampaign();
		const auto name = plugData::data.luaAll.cultureNames.find(index);
		if (name == plugData::data.luaAll.cultureNames.end())
		{
			logStringGame("getCultureName: Could not find culture name for index " + std::to_string(index));
			return nullptr;
		}
		return name->second;
	}

	religionDatabase* getReligionDatabase()
	{
		return *reinterpret_cast <religionDatabase**>(dataOffsets::offsets.religionDatabase);
	}

	int getReligionCount()
	{
		const auto* religionDb = getReligionDatabase();
		return religionDb->religionCount;
	}

	int getReligionN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.religionIndex.empty())
			plugData::data.luaAll.fillHashMapsNonCampaign();
		const auto religionIndex = plugData::data.luaAll.religionIndex.find(name);
		if (religionIndex == plugData::data.luaAll.religionIndex.end())
		{
			logStringGame("getReligionN: Could not find religion index for name " + name);
			return -1;
		}
		return religionIndex->second;
	}

	int getClimateN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded || plugData::data.luaAll.climateIndex.empty())
			plugData::data.luaAll.fillHashMaps();
		const auto index = plugData::data.luaAll.climateIndex.find(name);
		if (index == plugData::data.luaAll.climateIndex.end())
		{
			logStringGame("getClimateN: Could not find climate index for name " + name);
			return -1;
		}
		return index->second;
	}

	int getCultureN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.cultureIndex.empty())
			plugData::data.luaAll.fillHashMapsNonCampaign();
		const auto index = plugData::data.luaAll.cultureIndex.find(name);
		if (index == plugData::data.luaAll.cultureIndex.end())
		{
			logStringGame("getCultureN: Could not find culture index for name " + name);
			return -1;
		}
		return index->second;
	}
	
	DWORD getScriptCommandByName(const char* cmdName)
	{
		DWORD func1 = codes::offsets.scriptCommandOne;
		DWORD func2 = codes::offsets.scriptCommandTwo;
		DWORD result = 0x0;
		DWORD cmdNamePtr = reinterpret_cast<DWORD>(&cmdName);

		_asm {
			mov eax, func1
			call eax
			mov ecx, eax
			push cmdNamePtr
			mov eax, func2
			call eax
			mov result, eax
		}
		return result;
	}
	
	bool condition(const char* condition, const eventTrigger* eventData)
	{
		const auto fakeText = std::make_unique<fakeTextInput>(fakeTextInput(condition, 0));
		const auto rawText = fakeText.get();
		void* result = GAME_FUNC(void*(__cdecl*)(fakeTextInput*), parseCondition)(rawText);
		if (result == nullptr)
		{
			logStringGame("Condition: Could not create condition for " + std::string(condition));
			return false;
		}
		return callVFunc<1, bool>(result, eventData);
	}

	void scriptCommand(const char* command, const char* args)
	{
		DWORD scriptClass = getScriptCommandByName(command);
		if (scriptClass == 0x0)
		{
			return;
		}
		std::string fullCommand = std::string(command) + " " + args;
		size_t start = strlen(command) + static_cast<int8_t>(0x8);
		auto fakeText = std::make_shared<fakeTextInput>(fakeTextInput(fullCommand.c_str(), start));
		DWORD classPointer = 0x0;
		_asm
		{
			mov eax, scriptClass
			mov eax, [eax]
			mov classPointer, eax
		}
		fakeText->classPointer = classPointer;
		DWORD funcAddr = scriptClass + static_cast<int8_t>(0x4);
		DWORD scriptObject = 0x0;
		_asm
		{
			push fakeText
			mov eax, funcAddr
			mov eax, [eax]
			call eax
			mov scriptObject, eax
			add esp, 0x4
		}
		if (scriptObject == 0x0)
		{
			logStringGame("ScriptCommand: Could not create script object for " + std::string(command));
			return;
		}
		_asm
		{
			mov ecx, scriptObject
			mov eax, [ecx]
			mov eax, [eax]
			call eax
		}
	}
	
	void fireGameScriptFunc(void* scriptStruct, DWORD offset)
	{
		void* scriptStructPtr = scriptStruct;
		DWORD func = offset;
		_asm
		{
			mov ecx, scriptStructPtr
			mov eax, func
			call eax
		}
	}
	
	void logStringLua(const std::string& msg)
	{
		logStringGame(msg, true);
	}
	
	void logStringGame(const std::string& msg, const bool fromLua)
	{
		const auto eopLog = eopLogging::get();
		if (!eopLog->isInitialized())
			eopLog->createEopLoggers();
		const auto flushRate = reinterpret_cast<int*>(dataOffsets::offsets.logFlushRate);
		const int oldRate = *flushRate;
		*flushRate = 1;
		auto logger = *reinterpret_cast<boostLoggerImpl**>(dataOffsets::offsets.gameScriptLogger);
		if (!logger)
		{
			GAME_FUNC(void(__cdecl*)(), initGameScriptLog)();
			logger = *reinterpret_cast<boostLoggerImpl**>(dataOffsets::offsets.gameScriptLogger);
		}
		if (!eopLog->backupLogger)
		{
			eopLog->backupLogger = techFuncs::createGameClass<boostLoggerImpl>();
			*eopLog->backupLogger = *logger;
		}
		if (logger)
		{
			if (fromLua)
				*reinterpret_cast<boostLoggerImpl**>(dataOffsets::offsets.gameScriptLogger) = eopLog->eopLuaLog->impl.px;
			else
				*reinterpret_cast<boostLoggerImpl**>(dataOffsets::offsets.gameScriptLogger) = eopLog->eopLog->impl.px;
		}
		const auto order = std::make_shared<gameLogCommand>(msg.c_str());
		fireGameScriptFunc(order.get(), codes::offsets.gameLogCommand);
		*flushRate = oldRate;
		if (logger)
			*reinterpret_cast<boostLoggerImpl**>(dataOffsets::offsets.gameScriptLogger) = eopLog->backupLogger;
	}
	
	void logFuncError(const std::string& funcName, const std::string& error)
	{
		logStringGame(funcName + " error: " + error);
	}
	
	void loadSaveGame(const char* saveName)
	{
		const auto nameMem = new UNICODE_STRING*;
		const auto loadName = reinterpret_cast<UNICODE_STRING***>(dataOffsets::offsets.loadGameHandler + 0x4);
		*loadName = nameMem;
		gameStringHelpers::createUniString(*loadName, saveName);
		const auto currentHandler = reinterpret_cast<DWORD*>(dataOffsets::offsets.currentGameHandler);
		*currentHandler = dataOffsets::offsets.loadGameHandler;
	}

	stringWithHash* LOOKUP_EVENT = new stringWithHash();
	
	int incrementEventCounter(const std::string& name, const int value)
	{
		gameStringHelpers::setHashedString(&LOOKUP_EVENT->name, name.c_str());
		const int newVal = value;
		return GAME_FUNC(int(__thiscall*)(DWORD, stringWithHash*, int)
			, incEventCounter)(dataOffsets::offsets.eventManager, LOOKUP_EVENT, newVal);
	}

	eventAcceptDecline* createEventAcceptDecline(const std::string& name)
	{
		gameStringHelpers::setHashedString(&LOOKUP_EVENT->name, name.c_str());
		const auto obj = techFuncs::createGameClass<eventAcceptDecline>();
		GAME_FUNC(eventAcceptDecline*(__thiscall*)(eventAcceptDecline*, const char*, int)
			, createEventAcceptObj)(obj, LOOKUP_EVENT->name, LOOKUP_EVENT->hash);
		return obj;
	}

	void historicEvent(const std::string& name, const std::string& title, const std::string& description, const bool isChoice, const int xCoord,
					   const int yCoord, const sol::table& factions)
	{
		uint32_t facMask = 0;
		if (const int facCount = factions.size(); facCount)
		{
			for (int i{ 1 }; i <= facCount; i++)
			{
				const auto& fac = factions.get<sol::optional<std::string>>(i);
				if (!fac)
					break;
				auto faction = fac.value_or("");
				if (faction.empty())
					break;
				if (const auto foundFac = campaignHelpers::getCampaignData()->getFactionHashed(faction); foundFac)
					facMask |= 1 << foundFac->factionID;
			}
		}
		else
		{
			facMask = 0x3FFFFFFF;
		}
		historicEventRaw(name.c_str(), title.c_str(), description.c_str(), isChoice, facMask, xCoord, yCoord);
	}
	
	void historicEventRaw(const char* name, const char* title, const char* description, bool isChoice, uint32_t factions, int xCoord, int yCoord)
	{
		DWORD funcAddr = codes::offsets.historicEventFunc;
		incrementEventCounter(name, 1);
		
		UNICODE_STRING** titleUni = techFuncs::createGameClass<UNICODE_STRING*>();
		gameStringHelpers::createUniString(titleUni, title);

		UNICODE_STRING** bodyUni = techFuncs::createGameClass<UNICODE_STRING*>();
		gameStringHelpers::createUniString(bodyUni, description);

		UNICODE_STRING*** titleUniP = &titleUni;
		UNICODE_STRING*** bodyUniP = &bodyUni;

		eventAcceptDecline* obj = nullptr;
		if (isChoice)
			obj = createEventAcceptDecline(name);
		int x = xCoord;
		int y = yCoord;
		uint32_t facMask = factions;
		_asm
		{
			push facMask
			push 0x0
			push obj
			push yCoord
			push xCoord
			push name
			push bodyUniP
			push titleUniP
			mov eax, funcAddr
			call eax
			add esp, 0x20
		}
	}

	bool isPlayingDlc()
	{
		if (getGameVersion() == 1)
			return true;
		return *reinterpret_cast<bool*>(dataOffsets::offsets.isDLC);
	}

	void setPlayingDlc(const bool value)
	{
		if (getGameVersion() == 1)
			return;
		*reinterpret_cast<bool*>(dataOffsets::offsets.isDLC) = value;
	}
	
	void historicEvent(const std::string& name, const std::string& title, const std::string& description, const bool isChoice)
	{
		historicEventRaw(name.c_str(), title.c_str(), description.c_str(), isChoice, 0x3FFFFFFF, -1, -1);
	}
	
	void historicEvent(const std::string& name, const std::string& title, const std::string& description, const bool isChoice, const int xCoord,
					   const int yCoord)
	{
		historicEventRaw(name.c_str(), title.c_str(), description.c_str(), isChoice, 0x3FFFFFFF, xCoord, yCoord);
	}
	
	void historicEvent(const std::string& name, const std::string& title, const std::string& description)
	{
		historicEventRaw(name.c_str(), title.c_str(), description.c_str(), false, 0x3FFFFFFF, -1, -1);
	}
	
	std::string getModPath()
	{
		return  globals::dataS.modPath;
	}
	std::string getLuaPath()
	{
		return plugData::data.luaAll.luaPath;
	}
	
	std::string getModString(const std::string& path)
	{
		std::string ret;
		const size_t pos = path.find("/mods/", 0);
		for (UINT32 i = 0; i < path.size(); i++)
		{
			if (i > static_cast<UINT32>(pos))
			{
				ret.push_back(path[i]);
			}
		}
		return ret;
	}
	
	std::string getModFolderName()
	{
		const std::string path = getModPath();
		size_t pos = path.length();
		if (path.back() == '/' || path.back() == '\\')
			pos--;
		for (; pos > 0; pos--)
		{
			if (path[pos] == '/' || path[pos] == '\\')
				break;
		}
		return path.substr(pos + 1, path.length() - pos);
	}

	void copyFileLua(const std::string& file, const std::string& to)
	{
		//CopyFileA(file.c_str(), to.c_str(), FALSE);
		if (!std::filesystem::exists(file))
		{
			logStringGame("M2TWEOP.copyFile: File not found: " + file);
			return;
		}
		std::filesystem::copy(file, to, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
	}

	UNICODE_STRING*** getHashedUniString(void* stringTable, const std::string& key)
	{
		auto uniString = new UNICODE_STRING();
		UNICODE_STRING** lookup = &uniString;
		gameStringHelpers::createUniString(lookup, key.c_str());
		const auto value =  callClassFunc<void*, UNICODE_STRING***, UNICODE_STRING***>(stringTable, 0x4, &lookup);
		return value;
	}

	void setExpandedString(const std::string& key, const std::string& value)
	{
		const auto stringTable = *reinterpret_cast<void**>(dataOffsets::offsets.expandedBinTable);
		const auto uniString = getHashedUniString(stringTable, key);
		if (!uniString)
			return;
		gameStringHelpers::createUniString(*uniString, value.c_str());
	}

	void setEquipmentCosts(const int equipType, const int cost)
	{
		struct equipmentCosts
		{
			int ram;
			int ladder;
			int siegeTower;
		};
		const auto costs = reinterpret_cast<equipmentCosts*>(dataOffsets::offsets.equipmentCosts);
		switch (equipType)
		{
		case 0:
			costs->ram = cost;
			break;
		case 1:
			costs->ladder = cost;
			break;
		case 2:
			costs->siegeTower = cost;
			break;
		default:
			logStringGame("setEquipmentCosts: Invalid equipment type " + std::to_string(equipType));
			break;
		}
	}

	options1* getOptions1()
	{
		return reinterpret_cast<options1*>(dataOffsets::offsets.options1);
	}

	options2* getOptions2()
	{
		return reinterpret_cast<options2*>(dataOffsets::offsets.options2);
	}

	void addToIntArray(int** array, int* value)
	{
		GAME_FUNC(void(__thiscall*)(int**, int*), addToArrayInt)(array, value);
	}
	
	struct intArray
	{
		int* elements;
		int capacity;
		int count;
	};

	bool intArrayContains(int** array, const int value)
	{
		const auto arr = reinterpret_cast<intArray*>(array);
		for (int i = 0; i < arr->count; i++)
		{
			if (arr->elements[i] == value)
				return true;
		}
		return false;
	}

	void removeFromIntArray(int** array, const int index)
	{
		const auto arr = reinterpret_cast<intArray*>(array);
		if (!arr || index < 0 || index >= arr->count)
		{
			logStringGame("removeFromIntArray: Invalid index " + std::to_string(index));
			return;
		}
		arr->count--;
		int next;
		int *curr;
		for (int i = index; i < arr->count; *curr = next)
		{
			next = arr->elements[i + 1];
			curr = &arr->elements[i++];
		}
	}

	void setAncLimit(uint8_t limit)
	{
		const DWORD ancillariesOffset = dataOffsets::offsets.ancLimit;
		MemWork::WriteData(&limit, ancillariesOffset, 1);
	}

	void setMaxUnitSize(signed short min, signed short max)
	{
		DWORD codeOffset = dataOffsets::offsets.maxUnitSize;
		codeOffset += 0x82C;
		MemWork::WriteData(&min, codeOffset, 2);
		codeOffset += 6;
		MemWork::WriteData(&max, codeOffset, 2);
	}
	
	void setMaxBgSize(unsigned char size)
	{
		const DWORD cmpAdr = dataOffsets::offsets.maxBgSize1 + 2;
		const DWORD retAdr = dataOffsets::offsets.maxBgSize2 + 1;
		MemWork::WriteData(&size, cmpAdr, 1);
		MemWork::WriteData(&size, retAdr, 1);
	}

	void fixReligionTrigger()
	{
		int8_t fixValue = -3;
		MemWork::WriteData(&fixValue, dataOffsets::offsets.religionTriggerBug, 1);
	}

	gameDataAllStruct* getGameDataAll()
	{
		return dataOffsets::offsets.gameDataAllOffset;
	}
	
	void unlockConsoleCommands()
	{
		uchar nops[2] = { 0x90,0x90 };
		DWORD cmd = dataOffsets::offsets.unlockConsoleCommands1;
		//check checking code and change all jmps to nops
		for (int i = 0; i < 53; i++, cmd++)
		{
			uchar ch;
			MemWork::ReadData(cmd, &ch, 1);
			if (ch == 0x74)
				MemWork::WriteData(nops, cmd, 2);
		}
		//unlock change_faction
		uchar nops1[6] = { 0x90,0x90,0x90,0x90,0x90,0x90 };
		cmd = dataOffsets::offsets.unlockConsoleCommands2;
		MemWork::WriteData(nops1, cmd, 6);
	}
	
	bool HIGHLIGHT_ON = false;
	void toggleUnitHighlight()
	{
		static unsigned char highLightOn = 0x8f;
		static unsigned char highLightOff = 0x8e;
		const DWORD codeAdr = dataOffsets::offsets.highlightUnits;
		if (!HIGHLIGHT_ON)
		{
			MemWork::WriteData(&highLightOn, codeAdr + 0x1, 1);
			HIGHLIGHT_ON = true;
		}
		else
		{
			MemWork::WriteData(&highLightOff, codeAdr + 0x1, 1);
			HIGHLIGHT_ON = false;
		}
	}
	
	void setReligionsLimit(unsigned char limit)
	{
		MemWork::WriteData(&limit, dataOffsets::offsets.religionLimit, 1);
	}
	
	void setBuildingChainLimit(unsigned int limit)
	{
		limit++;
		const DWORD codeAdr = dataOffsets::offsets.buildingChainLimit;
		MemWork::WriteData(&limit, codeAdr, 4);
	}

	void setGuildCooldown(unsigned char turns)
	{
		const DWORD codeAdr = dataOffsets::offsets.guildCooldown;
		MemWork::WriteData(&turns, codeAdr, 1);
	}

	int getUnitSize()
	{
		return *dataOffsets::offsets.gameUnit_size;
	}
	
	void setTextureCacheSize()
	{
		const DWORD mapCacheLimit1 = dataOffsets::offsets.mapCacheSize1;
		int fix1 = 0x10;
		MemWork::WriteData(&fix1, mapCacheLimit1, 1);

		const DWORD mapCacheLimit2 = dataOffsets::offsets.mapCacheSize2;
		int fix2 = 0x08;
		MemWork::WriteData(&fix2, mapCacheLimit2, 1);
	}

	void unlockWeaponLimit()
	{
		int fix1 = 0x03;
		int fix2 = 0x04;

		const DWORD limit1 = dataOffsets::offsets.weaponLimit1;
		MemWork::WriteData(&fix1, limit1, 1);

		const DWORD limit2 = dataOffsets::offsets.weaponLimit2;
		MemWork::WriteData(&fix1, limit2, 1);

		const DWORD limit3 = dataOffsets::offsets.weaponLimit3;
		MemWork::WriteData(&fix1, limit3, 1);

		const DWORD limit4 = dataOffsets::offsets.weaponLimit4;
		MemWork::WriteData(&fix1, limit4, 1);

		const DWORD limit5 = dataOffsets::offsets.weaponLimit5;
		MemWork::WriteData(&fix2, limit5, 1);
	}

	void addToLua(sol::state& luaState)
	{
		struct
		{
			sol::usertype<options1> options1;
			sol::usertype<options2> options2;
		}typeAll;
		
		///Game Options
		//@section options1

		/***
		Basic options1 table

		@tfield int widescreen
		@tfield int antiAliasMode
		@tfield int subtitles
		@tfield int english
		@tfield int noBattleTimeLimit
		@tfield int useNewCursorActions
		@tfield int campaignNumTimesPlay
		@tfield int uiWinConditions
		@tfield int isScenario
		@tfield int isHotseatEnabled
		@tfield int hotseatAutosave
		@tfield int email
		@tfield int saveConfig
		@tfield int closeAfterSave
		@tfield int validateData
		@tfield int campaignMapSpeedUp
		@tfield int skipAiFactions
		@tfield int labelCharacters
		@tfield int noBackGroundFmv
		@tfield int disableArrowMarkers
		@tfield int arcadeBattles
		@tfield int disableEvents
		@tfield int isPrologue
		@tfield int updateAiCamera
		@tfield int hideCampaign
		@tfield int unlimitedMenOnBattlefield
		@tfield int tgaReserveSpace
		@tfield int keysetUsed
		@tfield int muteAdvisor
		@tfield int advancedStatsAlways
		@tfield int microManageAllSettlements
		@tfield int blindAdvisor
		@tfield int terrainQuality
		@tfield int vegetationQuality
		@tfield int useQuickChat
		@tfield int graphicsAdaptor
		@tfield int showDemeanour
		@tfield int radar
		@tfield int unitCards
		@tfield int sa_cards
		@tfield int buttons
		@tfield int tutorialBattlePlayed
		@tfield int disableVnVs
		@tfield int allUsers
		@tfield bool isPlayingDlc

		@table options1
		*/
		typeAll.options1 = luaState.new_usertype<options1>("options1");
		typeAll.options1.set("widescreen", &options1::widescreen);
		typeAll.options1.set("antiAliasMode", &options1::antiAliasMode);
		typeAll.options1.set("subtitles", &options1::subtitles);
		typeAll.options1.set("english", &options1::english);
		typeAll.options1.set("noBattleTimeLimit", &options1::noBattleTimeLimit);
		typeAll.options1.set("useNewCursorActions", &options1::useNewCursorActions);
		typeAll.options1.set("campaignNumTimesPlay", &options1::campaignNumTimesPlay);
		typeAll.options1.set("uiWinConditions", &options1::uiWinConditions);
		typeAll.options1.set("isScenario", &options1::isScenario);
		typeAll.options1.set("isHotseatEnabled", &options1::isHotseatEnabled);
		typeAll.options1.set("hotseatAutosave", &options1::hotseatAutosave);
		typeAll.options1.set("email", &options1::email);
		typeAll.options1.set("isPlayingDlc", sol::property(&isPlayingDlc, &setPlayingDlc));
		typeAll.options1.set("saveConfig", &options1::saveConfig);
		typeAll.options1.set("closeAfterSave", &options1::closeAfterSave);
		typeAll.options1.set("validateData", &options1::validateData);
		typeAll.options1.set("campaignMapSpeedUp", &options1::campaignMapSpeedUp);
		typeAll.options1.set("skipAiFactions", &options1::skipAiFactions);
		typeAll.options1.set("labelCharacters", &options1::labelCharacters);
		typeAll.options1.set("noBackGroundFmv", &options1::noBackGroundFmv);
		typeAll.options1.set("disableArrowMarkers", &options1::disableArrowMarkers);
		typeAll.options1.set("arcadeBattles", &options1::arcadeBattles);
		typeAll.options1.set("disableEvents", &options1::disableEvents);
		typeAll.options1.set("isPrologue", &options1::isPrologue);
		typeAll.options1.set("updateAiCamera", &options1::updateAiCamera);
		typeAll.options1.set("hideCampaign", &options1::hideCampaign);
		typeAll.options1.set("unlimitedMenOnBattlefield", &options1::unlimitedMenOnBattlefield);
		typeAll.options1.set("tgaReserveSpace", &options1::tgaReserveSpace);
		typeAll.options1.set("keysetUsed", &options1::keysetUsed);
		typeAll.options1.set("muteAdvisor", &options1::muteAdvisor);
		typeAll.options1.set("advancedStatsAlways", &options1::advancedStatsAlways);
		typeAll.options1.set("microManageAllSettlements", &options1::microManageAllSettlements);
		typeAll.options1.set("blindAdvisor", &options1::blindAdvisor);
		typeAll.options1.set("terrainQuality", &options1::terrainQuality);
		typeAll.options1.set("vegetationQuality", &options1::vegetationQuality);
		typeAll.options1.set("useQuickChat", &options1::useQuickChat);
		typeAll.options1.set("graphicsAdaptor", &options1::graphicsAdaptor);
		typeAll.options1.set("showDemeanour", &options1::showDemeanour);
		typeAll.options1.set("radar", &options1::radar);
		typeAll.options1.set("unitCards", &options1::unitCards);
		typeAll.options1.set("sa_cards", &options1::sa_cards);
		typeAll.options1.set("buttons", &options1::buttons);
		typeAll.options1.set("tutorialBattlePlayed", &options1::tutorialBattlePlayed);
		typeAll.options1.set("disableVnVs", &options1::disableVnVs);
		typeAll.options1.set("allUsers", &options1::allUsers);


		/***
		Basic options2 table
		
		@tfield int campaignResolutionX
		@tfield int campaignResolutionY
		@tfield int battleResolutionX
		@tfield int battleResolutionY
		@tfield int vSync
		@tfield int uiIconBarCheck
		@tfield int uiRadarCheck
		@tfield int useMorale
		@tfield int uiAmmoCheck
		@tfield int useFatigue
		@tfield int uiSupplyCheck
		@tfield int toggleFowState this does not toggle fow just remembers if it was on or off
		@tfield int cameraRestrict
		@tfield int eventCutscenes
		@tfield int defaultCameraInBattle
		@tfield int splashes
		@tfield int stencilShadows
		@tfield int audioEnable
		@tfield int speechEnable
		@tfield int firstTimePlay
		@tfield int toggleAutoSave
		@tfield int showBanners
		@tfield int passwords
		@tfield int hotseatTurns
		@tfield int hotseatScroll
		@tfield int allowValidationFeatures
		@tfield int campaignSpeed
		@tfield int labelSettlements
		@tfield int disablePapalElections
		@tfield int autoresolveAllBattles
		@tfield int savePrefs
		@tfield int disableConsole
		@tfield int validateDiplomacy
		@tfield int unitDetail
		@tfield int buildingDetail
		@tfield int maxSoldiersOnBattlefield if limited
		@tfield int unitSize
		@tfield int cameraRotateSpeed
		@tfield int cameraMoveSpeed
		@tfield float cameraSmoothing
		@tfield int masterVolume
		@tfield int musicVolume
		@tfield int speechVolume
		@tfield int sfxVolume
		@tfield int subFactionAccents
		@tfield int tgaWidth
		@tfield float tgaAspect
		@tfield int tgaInputScale
		@tfield int scrollMinZoom
		@tfield int scrollMaxZoom
		@tfield int advisorVerbosity
		@tfield int effectQuality
		@tfield int EnableCameraCampaignSmoothing
		@tfield int chatMsgDuration
		@tfield int saveGameSpyPassword
		@tfield int addDateToLogs
		@tfield int playerFactionID
		@tfield int campaignDifficulty
		@tfield int battleDifficulty
		@tfield int showToolTips
		@tfield int isNormalHud
		@tfield int showPackageLitter
		@tfield float unitSizeMultiplierLow
		@tfield float unitSizeMultiplierMedium
		@tfield float unitSizeMultiplierLarge
		@tfield isHotseatPlayer isHotseatPlayer
		@tfield setHotseatPlayer setHotseatPlayer

		@table options2
		*/
		typeAll.options2 = luaState.new_usertype<options2>("options2");
		typeAll.options2.set("campaignResolutionX", &options2::campaignResolutionX);
		typeAll.options2.set("campaignResolutionY", &options2::campaignResolutionY);
		typeAll.options2.set("battleResolutionX", &options2::battleResolutionX);
		typeAll.options2.set("battleResolutionY", &options2::battleResolutionY);
		typeAll.options2.set("vSync", &options2::vSync);
		typeAll.options2.set("uiIconBarCheck", &options2::uiIconBarCheck);
		typeAll.options2.set("uiRadarCheck", &options2::uiRadarCheck);
		typeAll.options2.set("useMorale", &options2::useMorale);
		typeAll.options2.set("uiAmmoCheck", &options2::uiAmmoCheck);
		typeAll.options2.set("useFatigue", &options2::useFatigue);
		typeAll.options2.set("uiSupplyCheck", &options2::uiSupplyCheck);
		typeAll.options2.set("toggleFowState", &options2::toggleFowState);
		typeAll.options2.set("cameraRestrict", &options2::cameraRestrict);
		typeAll.options2.set("eventCutscenes", &options2::eventCutscenes);
		typeAll.options2.set("defaultCameraInBattle", &options2::defaultCameraInBattle);
		typeAll.options2.set("splashes", &options2::splashes);
		typeAll.options2.set("stencilShadows", &options2::stencilShadows);
		typeAll.options2.set("audioEnable", &options2::audioEnable);
		typeAll.options2.set("speechEnable", &options2::speechEnable);
		typeAll.options2.set("firstTimePlay", &options2::firstTimePlay);
		typeAll.options2.set("toggleAutoSave", &options2::toggleAutoSave);
		typeAll.options2.set("showBanners", &options2::showBanners);
		typeAll.options2.set("passwords", &options2::passwords);
		typeAll.options2.set("hotseatTurns", &options2::hotseatTurns);
		typeAll.options2.set("hotseatScroll", &options2::hotseatScroll);
		typeAll.options2.set("allowValidationFeatures", &options2::allowValidationFeatures);
		typeAll.options2.set("campaignSpeed", &options2::campaignSpeed);
		typeAll.options2.set("labelSettlements", &options2::labelSettlements);
		typeAll.options2.set("disablePapalElections", &options2::disablePapalElections);
		typeAll.options2.set("autoresolveAllBattles", &options2::autoresolveAllBattles);
		typeAll.options2.set("savePrefs", &options2::savePrefs);
		typeAll.options2.set("disableConsole", &options2::disableConsole);
		typeAll.options2.set("validateDiplomacy", &options2::validateDiplomacy);
		typeAll.options2.set("unitDetail", &options2::unitDetail);
		typeAll.options2.set("buildingDetail", &options2::buildingDetail);
		typeAll.options2.set("maxSoldiersOnBattlefield", &options2::maxSoldiersOnBattlefield);
		typeAll.options2.set("unitSize", &options2::unitSize);
		typeAll.options2.set("cameraRotateSpeed", &options2::cameraRotateSpeed);
		typeAll.options2.set("cameraMoveSpeed", &options2::cameraMoveSpeed);
		typeAll.options2.set("cameraSmoothing", &options2::cameraSmoothing);
		typeAll.options2.set("masterVolume", &options2::masterVolume);
		typeAll.options2.set("musicVolume", &options2::musicVolume);
		typeAll.options2.set("speechVolume", &options2::speechVolume);
		typeAll.options2.set("sfxVolume", &options2::sfxVolume);
		typeAll.options2.set("subFactionAccents", &options2::subFactionAccents);
		typeAll.options2.set("playerFactionID", &options2::playerFactionId);
		typeAll.options2.set("campaignDifficulty", &options2::campaignDifficulty);
		typeAll.options2.set("battleDifficulty", &options2::battleDifficulty);
		typeAll.options2.set("tgaWidth", &options2::tgaWidth);
		typeAll.options2.set("tgaAspect", &options2::tgaAspect);
		typeAll.options2.set("tgaInputScale", &options2::tgaInputScale);
		typeAll.options2.set("scrollMinZoom", &options2::scrollMinZoom);
		typeAll.options2.set("scrollMaxZoom", &options2::scrollMaxZoom);
		typeAll.options2.set("advisorVerbosity", &options2::advisorVerbosity);
		typeAll.options2.set("effectQuality", &options2::effectQuality);
		typeAll.options2.set("EnableCameraCampaignSmoothing", &options2::EnableCameraCampaignSmoothing);
		typeAll.options2.set("chatMsgDuration", &options2::chatMsgDuration);
		typeAll.options2.set("saveGameSpyPassword", &options2::saveGameSpyPassword);
		typeAll.options2.set("addDateToLogs", &options2::addDateToLogs);
		typeAll.options2.set("showToolTips", &options2::showToolTips);
		typeAll.options2.set("isNormalHud", &options2::isNormalHud);
		typeAll.options2.set("showPackageLitter", &options2::showPackageLitter);
		typeAll.options2.set("unitSizeMultiplierLow", &options2::unitSizeMultiplierLow);
		typeAll.options2.set("unitSizeMultiplierMedium", &options2::unitSizeMultiplierMedium);
		typeAll.options2.set("unitSizeMultiplierLarge", &options2::unitSizeMultiplierLarge);
		
		/***
		Check if a faction was selected on faction select screen.
		@function options2:isHotseatPlayer
		@tparam int factionID
		@treturn bool selected
		@usage
			local selected = options2:isHotseatPlayer(1)
		*/
		typeAll.options2.set_function("isHotseatPlayer", &options2::isHotseatPlayer);
		
		/***
		Select a faction on faction select screen.
		@function options2:setHotseatPlayer
		@tparam int factionID
		@tparam bool set
		@usage
			options2:setHotseatPlayer(1, true)
		*/
		typeAll.options2.set_function("setHotseatPlayer", &options2::setHotseatPlayer);
	
	}
	
}
