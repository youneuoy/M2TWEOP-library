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
int m2tweopOptions::weaponBonusModifier = 3;
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
		if (globals::dataS.modFolderName.empty())
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
			globals::dataS.modFolderName = path.substr(pos + 1, path.length() - pos);
		}
		return globals::dataS.modFolderName;
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
		{
			logStringGame("setExpandedString: Could not find key " + key);
			return;
		}
		gameStringHelpers::createUniString(*uniString, value.c_str());
	}

	enum stratTextEnum
	{
		SMT_END_TURN,
		SMT_SABOTAGE,
		SMT_ASSASSINATE,
		SMT_SETTLEMENTS,
		SMT_CAPITAL,
		SMT_ALLIES,
		SMT_ENEMIES,
		SMT_SUSPICIOUS,
		SMT_NEUTRAL,
		SMT_HOSTILE,
		SMT_AT_WAR,
		SMT_NIGHT_ATTACK,
		SMT_TRADE_RIGHTS,
		SMT_SIEGE,
		SMT_ASSAULT,
		SMT_END_SIEGE,
		SMT_AGE,
		SMT_LOYALTY,
		SMT_GOVERNOR,
		SMT_GARRISON,
		SMT_RECRUITMENT_COST,
		SMT_RECRUITMENT_TIME,
		SMT_RETRAIN_TIME,
		SMT_RETRAIN_COST,
		SMT_FIGHT_BATTLE,
		SMT_WITHDRAW,
		SMT_AUTORESOLVE,
		SMT_DISBAND,
		SMT_DOCTOR_DESCR,
		SMT_QUARTERMASTER_DESCR,
		SMT_PRIEST_DESCR,
		SMT_SCOUT_DESCR,
		SMT_ENGINEER_DESCR,
		SMT_SPY_DESCR,
		SMT_ASSASSIN_DESCR,
		SMT_DIPLOMAT_DESCR,
		SMT_ADMIRAL_DESCR,
		SMT_ARMY_UPKEEP,
		SMT_LOYALTY_REVOLTING,
		SMT_LOYALTY_RIOTING,
		SMT_LOYALTY_DISILLUSIONED,
		SMT_LOYALTY_CONTENT,
		SMT_LOYALTY_HAPPY,
		SMT_FORTIFICATION_NONE,
		SMT_FORTIFICATION_0,
		SMT_FORTIFICATION_1,
		SMT_FORTIFICATION_2,
		SMT_FORTIFICATION_3,
		SMT_FORTIFICATION_4,
		SMT_MISSION_INFILTRATE,
		SMT_MISSION_DIPLOMACY,
		SMT_MISSION_BRIBE,
		SMT_MISSION_SPYING,
		SMT_MISSION_ASSASSINATE,
		SMT_MISSION_SABOTAGE,
		SMT_NAME,
		SMT_FORT,
		SMT_FACTION,
		SMT_PORT,
		SMT_POPULATION,
		SMT_FATIGUE,
		SMT_EXPERIENCE,
		SMT_MILD,
		SMT_MASSIVE,
		SMT_CATASTROPHIC,
		SMT_SMALL,
		SMT_MEDIUM,
		SMT_LARGE,
		SMT_HUGE,
		SMT_CURRENTLY_BUILDING,
		SMT_HEALTHY,
		SMT_TIRED,
		SMT_ILL,
		SMT_DYING,
		SMT_ARMY_DETAILS,
		SMT_ENEMY_ARMY_DETAILS,
		SMT_ALLIED_ARMY_DETAILS,
		SMT_NEUTRAL_ARMY_DETAILS,
		SMT_NAVY_DETAILS,
		SMT_ENEMY_NAVY_DETAILS,
		SMT_ALLIED_NAVY_DETAILS,
		SMT_NEUTRAL_NAVY_DETAILS,
		SMT_SETTLEMENT_DETAILS,
		SMT_ENEMY_SETTLEMENT_DETAILS,
		SMT_NEUTRAL_SETTLEMENT_DETAILS,
		SMT_ALLIED_SETTLEMENT_DETAILS,
		SMT_FORT_DETAILS,
		SMT_ENEMY_FORT_DETAILS,
		SMT_ALLIED_FORT_DETAILS,
		SMT_NEUTRAL_FORT_DETAILS,
		SMT_SET_HEIR,
		SMT_PREV_CITY,
		SMT_NEXT_CITY,
		SMT_NEXT_CHARACTER,
		SMT_PREV_CHARACTER,
		SMT_NEXT_UNIT,
		SMT_PREV_UNIT,
		SMT_PREV_FORT,
		SMT_NEXT_FORT,
		SMT_FOOD_SUPPLIES,
		SMT_STATE_REVOLTING,
		SMT_STATE_NOT_REVOLTING,
		SMT_STATE_UNDER_SIEGE,
		SMT_STATE_NOT_UNDER_SIEGE,
		SMT_STATE_HOLDING_GAMES,
		SMT_STATE_NOT_HOLDING_GAMES,
		SMT_STATE_PLAGUE,
		SMT_STATE_NOT_PLAGUED,
		SMT_FERTILE_LOW,
		SMT_FERTILE_MEDIUM,
		SMT_FERTILE_HIGH,
		SMT_WILDERNESS,
		SMT_MOUNTAINS_HIGH,
		SMT_MOUNTAINS_LOW,
		SMT_HILLS,
		SMT_FOREST_DENSE,
		SMT_FOREST_SPARSE,
		SMT_SWAMP,
		SMT_OCEAN,
		SMT_SEA_DEEP,
		SMT_SEA_SHALLOW,
		SMT_BEACH,
		SMT_IMPASSABLE_LAND,
		SMT_IMPASSABLE_SEA,
		SMT_FAMINE_THREAT_OK,
		SMT_FAMINE_THREAT_HUNGRY,
		SMT_FAMINE_THREAT_FAMINE_1,
		SMT_FAMINE_THREAT_FAMINE_2,
		SMT_FAMINE_THREAT_FAMINE_3,
		SMT_FAMINE_THREAT,
		SMT_HARVEST_LEVEL,
		SMT_FARMING_LEVEL,
		SMT_TAX_LEVEL_LOW,
		SMT_TAX_LEVEL_NORMAL,
		SMT_TAX_LEVEL_HIGH,
		SMT_TAX_LEVEL_VERY_HIGH,
		SMT_GOVERNOR_POLICY_BALANCED,
		SMT_GOVERNOR_POLICY_FINANCIAL,
		SMT_GOVERNOR_POLICY_MILITARY,
		SMT_GOVERNOR_POLICY_STRONGHOLD,
		SMT_GOVERNOR_POLICY_CULTURAL,
		SMT_NO_GOVERNOR,
		SMT_NO_GOVERNOR_DESCR,
		SMT_SHOW_GARRISON,
		SMT_SHOW_UNITS,
		SMT_SHOW_UNITS_NAVY,
		SMT_SHOW_BUILDINGS,
		SMT_SHOW_VISITORS,
		SMT_SHOW_TRAVELLING_CHARACTERS,
		SMT_SHOW_TRAVELLING_CHARACTERS_NAVY,
		SMT_SHOW_PASSENGERS,
		SMT_RCLICK_SHOW_SETTLEMENTS,
		SMT_RCLICK_SHOW_CHARACTERS,
		SMT_RCLICK_SHOW_ARMIES,
		SMT_LOYALTY_DECREASING,
		SMT_LOYALTY_INCREASING,
		SMT_REMOVE_RALLY_POINT,
		SMT_SET_RALLY_POINT,
		SMT_1_TURN,
		SMT_X_TURNS,
		SMT_SELECT_UNIT_TO_RECRUIT,
		SMT_SELECT_UNIT_TO_RETRAIN,
		SMT_SET_TO_RECRUIT_MODE,
		SMT_SET_TO_RETRAIN_MODE,
		SMT_MERGE_SELECTED_UNITS,
		SMT_X_PERCENT_COMPLETE,
		SMT_DIPLOMACY_OFFER_ALLIANCE,
		SMT_DIPLOMACY_OFFER_CANCEL_ALLIANCE,
		SMT_DIPLOMACY_OFFER_CEASEFIRE,
		SMT_DIPLOMACY_OFFER_MILITARY_ACCESS,
		SMT_DIPLOMACY_OFFER_CANCEL_MILITARY_ACCESS,
		SMT_DIPLOMACY_OFFER_TRADE_RIGHTS,
		SMT_DIPLOMACY_OFFER_CANCEL_TRADE_RIGHTS,
		SMT_DIPLOMACY_OFFER_ATTACK_FACTION,
		SMT_DIPLOMACY_OFFER_CEDE_REGION,
		SMT_DIPLOMACY_OFFER_SINGLE_PAYMENT,
		SMT_DIPLOMACY_OFFER_YEARLY_PAYMENT,
		SMT_DIPLOMACY_OFFER_CANCEL_YEARLY_PAYMENT,
		SMT_DIPLOMACY_OFFER_THREAT_OF_ATTACK,
		SMT_DIPLOMACY_OFFER_DEMAND_NO_ATTACK,
		SMT_DIPLOMACY_OFFER_MAP_INFORMATION,
		SMT_DIPLOMACY_OFFER_BRIBE,
		SMT_DIPLOMACY_OFFER_PROTECTORATE,
		SMT_MAKE_DECLARATION,
		SMT_MAKE_OFFER,
		SMT_MAKE_DEMAND,
		SMT_YOUR_DECLARATIONS,
		SMT_YOUR_DEMANDS,
		SMT_YOUR_OFFERS,
		SMT_THEIR_DECLARATIONS,
		SMT_THEIR_OFFERS,
		SMT_THEIR_DEMANDS,
		SMT_GIVE_X_DENARI,
		SMT_WE_WILL_GIVE_X_DENARI,
		SMT_GIVE_YEARLY_TRIBUTE_OF_X_DENARI,
		SMT_OFFER_YEARLY_TRIBUTE_OF_X_DENARI,
		SMT_CANCEL_YEARLY_TRIBUTE_OF_X_DENARI,
		SMT_DIP_ACCEPT_OFFER,
		SMT_DIP_DECLINE_OFFER,
		SMT_DIP_COUNTER_OFFER,
		SMT_DIP_NEW_OFFER,
		SMT_DIP_INIT_OFFER,
		SMT_DIP_MAKE_GIFT,
		SMT_DIP_INFORM,
		SMT_ABILITIES_AT_A_GLANCE,
		SMT_DESCRIPTION,
		SMT_UPGRADES,
		SMT_X_MEN,
		SMT_YOUR_ARMY_HAS_X_BUILD_POINTS,
		SMT_SIEGE_ACTIONS,
		SMT_BUILT,
		SMT_ZOOM_TO_SETTLEMENT,
		SMT_ZOOM_TO_UNIT,
		SMT_ZOOM_TO_CHARACTER,
		SMT_OPEN_CONSTRUCTION_WINDOW,
		SMT_NOTHING_TO_CONSTRUCT,
		SMT_OPEN_TRAINING_WINDOW,
		SMT_NOTHING_TO_TRAIN,
		SMT_CURRENTLY_TRAINING,
		SMT_OPEN_OPTIONS_WINDOW,
		SMT_OPEN_FINANCE_WINDOW,
		SMT_OPEN_DIPLOMACY_WINDOW,
		SMT_FACTION_BUTTON_TOOLTIP,
		SMT_SELECTED_ITEM_TOOLTIP,
		SMT_BUILD_QUEUE_CONFLICT,
		SMT_BUILD_QUEUE_FULL,
		SMT_BUILD_QUEUE_INSUFFICIENT_FUNDS,
		SMT_TRAINING_INSUFFICIENT_POPULACE,
		SMT_TRAINING_INSUFFICIENT_FUNDS,
		SMT_MAX_UNIT_POOL,
		SMT_TRAINING_QUEUE_FULL,
		SMT_WE_HAVE_MILITARY_ACCESS,
		SMT_THEY_HAVE_MILITARY_ACCESS,
		SMT_PAYING_YOU_TRIBUTE_X_PER_TURN,
		SMT_PAYING_THEM_TRIBUTE_X_PER_TURN,
		SMT_ASSISTANCE_REQUESTED_AGAINST,
		SMT_ASSISTANCE_PROVIDED_AGAINST,
		SMT_CONFIRM_OFFER,
		SMT_CANCEL_OFFER,
		SMT_OFFER_MONEY_TITLE,
		SMT_DEMAND_MONEY_TITLE,
		SMT_AMOUNT_OF_DENARI,
		SMT_OFFER_TRIBUTE_TITLE,
		SMT_DEMAND_TRIBUTE,
		SMT_NUMBER_OF_YEARS,
		SMT_OR_UNTIL_FURTHER_NOTICE,
		SMT_UNTIL_FURTHER_NOTICE,
		SMT_DEMAND_ATTACK_ON_FACTION,
		SMT_OFFER_ATTACK_ON_FACTION,
		SMT_WITHIN,
		SMT_YEARS,
		SMT_GIVE,
		SMT_DEMAND_SETTLEMENT_TITLE,
		SMT_OFFER_SETTLEMENT_TITLE,
		SMT_CITY,
		SMT_CHARACTERS,
		SMT_STATUS,
		SMT_CHARACTER_PERFORMED_TURNS_ACTION,
		SMT_TARGET_TILE_IS_IMPASSABLE,
		SMT_CANNOT_LEAVE_SIEGED_FORT,
		SMT_CANNOT_LEAVE_SIEGED_SETTLEMENT,
		SMT_CANNOT_LEAVE_ENEMIES_ZOC,
		SMT_CHARACTER_DETAILS,
		SMT_ENEMY_CHARACTER_DETAILS,
		SMT_NEUTRAL_CHARACTER_DETAILS,
		SMT_ALLIED_CHARACTER_DETAILS,
		SMT_VICES,
		SMT_FACTION_SUMMARY,
		SMT_FACTION_LEADER,
		SMT_HEIRS,
		SMT_REGIONS_GAINED,
		SMT_BATTLES_WON,
		SMT_BATTLES_LOST,
	    SMT_GREATEST_GENERAL,
	    SMT_GENERALS,
	    SMT_CITIES,
	    SMT_CASTLES,
	    SMT_YEAR,
	    SMT_TURN_NUMBER,
	    SMT_TURNS_REMAINING,
		SMT_SHOW_FAMILY_TREE,
		SMT_SHOW_EVENT_LOG,
		SMT_FINANCIAL_SUMMARY,
		SMT_INCOME,
		SMT_EXPENDITURE,
		SMT_FARMING,
		SMT_RESOURCES,
		SMT_TRADE,
	    SMT_MERCHANT_TRADE,
	    SMT_TAXES,
		SMT_WAGES,
		SMT_PROJECTED_INCOME,
		SMT_PROJECTED_EXPENDITURE,
		SMT_PROJECTED_PROFIT,
		SMT_TOTAL_INCOME,
		SMT_TOTAL_EXPENDITURE,
		SMT_TOTAL_PROFIT,
	    SMT_LEVEL,
	    SMT_EOT_FINANCIAL_SUMMARY,
	    SMT_EOT_MILITARY_SUMMARY,
	    SMT_EOT_BALANCE_OF_POWER,
	    SMT_EOT_BATTLES_FOUGHT,
	    SMT_EOT_BATTLES_WON,
	    SMT_EOT_TROOPS_RECRUITED,
	    SMT_EOT_TROOPS_LOST,
	    SMT_EOT_REGIONS_CAPTURED,
	    SMT_EOT_REGIONS_LOST,
	    SMT_EOT_YOUR_RATING,
	    SMT_EOT_CURRENT_LEADER,
	    SMT_EOT_MILITARY,
	    SMT_EOT_FINANCIAL,
	    SMT_EOT_PRODUCTION,
	    SMT_EOT_POPULATION,
	    SMT_EOT_OVERAL,
	    SMT_EOT_PERCENTAGE,
		SMT_ARMY_RECRUIT,
		SMT_CONSTRUCTION,
		SMT_DIPLOMACY,
		SMT_TRIBUTE,
		SMT_OTHER,
		SMT_EVENT_LOG,
		SMT_CHARACTER,
		SMT_TIME_REMAINING,
		SMT_TARGETS,
		SMT_TARGET,
		SMT_SETTLEMENT,
		SMT_TRADE_DETAILS,
		SMT_SHOW_TRADE_SUMMARY,
		SMT_RESOURCE_GENERIC,
		SMT_RESOURCE_GOLD,
		SMT_RESOURCE_SILVER,
		SMT_RESOURCE_IRON,
		SMT_RESOURCE_FISH,
		SMT_RESOURCE_FURS,
		SMT_RESOURCE_GRAIN,
		SMT_RESOURCE_TIMBER,
		SMT_RESOURCE_ELEPHANT,
		SMT_RESOURCE_CAMEL,
		SMT_RESOURCE_DOG,
		SMT_RESOURCE_COAL,
	    SMT_RESOURCE_WOOL,
		SMT_RESOURCE_IVORY,
		SMT_RESOURCE_WINE,
		SMT_RESOURCE_SLAVES,
		SMT_RESOURCE_CHOCOLATE,
		SMT_RESOURCE_MARBLE,
		SMT_RESOURCE_TEXTILES,
		SMT_RESOURCE_DYES,
		SMT_RESOURCE_TOBACCO,
		SMT_RESOURCE_SILK,
		SMT_RESOURCE_SUGAR,
		SMT_RESOURCE_SULFUR,
		SMT_RESOURCE_TIN,
		SMT_RESOURCE_SPICES,
		SMT_RESOURCE_COTTON,
		SMT_RESOURCE_AMBER,
		SMT_HARVEST_POOR,
		SMT_HARVEST_NORMAL,
		SMT_HARVEST_GOOD,
		SMT_HARVEST_EXCELLENT,
		SMT_MINING_INCOME,
		SMT_FARM_INCOME,
		SMT_FIND_SELECTED_CHARACTER,
		SMT_FAMILY_TREE_TITLE,
		SMT_OWNED_BY,
		SMT_WEAPON_ARMOUR_PIERCING,
		SMT_WEAPON_BODY_PIERCING,
		SMT_WEAPON_PRECURSOR,
		SMT_WEAPON_AREA,
		SMT_UNIT_IS_GENERAL,
		SMT_UNIT_IS_COMMAND,
		SMT_UNIT_CAN_CONCEAL,
		SMT_UNIT_IS_HARDY,
		SMT_UNIT_IS_VERY_HARDY,
		SMT_UNIT_IS_HEAVY,
		SMT_CAN_DO_TESTUDO,
		SMT_CAN_DO_PHALANX,
		SMT_GOOD_ON_SCRUB,
		SMT_GOOD_ON_SAND,
		SMT_GOOD_ON_FOREST,
		SMT_GOOD_ON_SNOW,
		SMT_BAD_ON_SCRUB,
		SMT_BAD_ON_SAND,
		SMT_BAD_ON_FOREST,
		SMT_BAD_ON_SNOW,
		SMT_UNCOVERED_HIDDEN_AGENT_IN_SETTLEMENT_TITLE,
		SMT_UNCOVERED_HIDDEN_AGENT_IN_SETTLEMENT_DESCRIPTION,
		SMT_HIDDEN_AGENT_IN_SETTLEMENT_COMPROMISED_TITLE,
		SMT_HIDDEN_AGENT_IN_SETTLEMENT_COMPROMISED_DESCRIPTION,
		SMT_APPOINT_TO_GOVERNOR_TITLE,
		SMT_APPOINT_TO_GOVERNOR_DESCRIPTION,
		SMT_SETTLEMENT_BESIEGE_TITLE,
		SMT_SETTLEMENT_BESIEGE_DESCRIPTION,
		SMT_SETTLEMENT_REVOLT_TITLE,
		SMT_SETTLEMENT_REVOLT_DESCRIPTION,
		SMT_SETTLEMENT_LOST_TITLE,
		SMT_SETTLEMENT_LOST_DESCRIPTION,
		SMT_SETTLEMENT_GAINED_TITLE,
		SMT_SETTLEMENT_GAINED_DESCRIPTION,
		SMT_NAVY_BLOCKADED_TITLE,
		SMT_NAVY_BLOCKADED_DESCRIPTION,
		SMT_BIRTH_TITLE,
		SMT_BIRTH_DESCRIPTION,
		SMT_ADOPTION_TITLE,
		SMT_ADOPTION_DESCRIPTION,
		SMT_COMING_OF_AGE_TITLE,
		SMT_COMING_OF_AGE_DESCRIPTION,
		SMT_MARRIAGE_TITLE,
		SMT_MARRIAGE_DESCRIPTION,
		SMT_NATURAL_DEATH_TITLE,
		SMT_NATURAL_DEATH_DESCRIPTION,
		SMT_NEW_FACTION_LEADER_TITLE,
		SMT_NEW_FACTION_LEADER_DESCRIPTION,
		SMT_NEW_FACTION_HEIR_TITLE,
		SMT_NEW_FACTION_HEIR_DESCRIPTION,
		SMT_FACTION_DEFEATED_TITLE,
		SMT_FACTION_DEFEATED_DESCRIPTION,
		SMT_UNKNOWN,
		SMT_UNKNOWN_UNIT,
		SMT_UNKNOWN_BUILDING,
		SMT_ARMY_BREAKDOWN,
		SMT_SHOW_ARMY_DETAILS,
		SMT_SHOW_VNV_DETAILS,
		SMT_TRAITS_UKNOWN,
		SMT_STATUS_UNKNOWN,
		SMT_SPOTTED_BY,
		SMT_WATCHTOWER,
		SMT_EXCHANGE_COMMANDING_OFFICER,
		SMT_EXCHANGE_NO_MOVEMENT_POINTS,
		SMT_EXCHANGE_NO_NAVY_PRESENT,
		SMT_EXCHANGE_SELECTED,
		SMT_EXCHANGE_UNSELECTED,
		SMT_EXCHANGE_NO_SELECTION,
		SMT_EXCHANGE_MERGE_OK,
		SMT_EXCHANGE_CAPACITY_EXCEEDED,
		SMT_SHOW_MISSION_TARGET,
		SMT_RALLY_POINTS,
		SMT_RALLY_POINT,
		SMT_CANNOT_MERGE_THIS_UNIT_SELECTION,
		SMT_CANNOT_MERGE_THIS_CHARACTER_SELECTION,
		SMT_ZOOM_DOWN_TO_CITY,
		SMT_DESTROY_BUILDING,
		SMT_RETAINERS,
		SMT_RETAINERS_UNKNOWN,
		SMT_SUCCESS_RATE,
		SMT_SHOW_CHARACTER_TARGETS,
		SMT_SHOW_BUILDING_TARGETS,
		SMT_ACCEPT_MISSION,
		SMT_BUILDING,
		SMT_MILITARY_FORCES,
		SMT_NUM_SOLDIERS,
		SMT_ATTACK,
		SMT_ATTACK_CHARGE,
		SMT_ARMOUR,
		SMT_DEFENCE,
		SMT_SHIELD,
		SMT_HP,
		SMT_CREATION_COST,
		SMT_ARMY_STRENGTH_RATIO,
		SMT_NAVY_STRENGTH_RATIO,
		SMT_PROTECTORATES,
		SMT_YOU_ARE_PROTECTOR,
		SMT_ENABLE_AUTO_TRAIN,
		SMT_ENABLE_AUTO_CONSTRUCT,
		SMT_AUTOMATE_ALL,
		SMT_AI_SPEND_POLICY,
		SMT_SAVE,
		SMT_SPEND,
		SMT_CONSTRUCTION_OPTIONS,
		SMT_CONSTRUCTION_QUEUE,
		SMT_LAND_EXPORTS,
		SMT_SEA_EXPORTS,
		SMT_IMPORTS,
		SMT_TRADE_INCOME,
		SMT_RECRUITMENT_QUEUE,
		SMT_DEFENSES,
		SMT_SIEGE_DETAILS,
		SMT_SETTLEMENT_HAS_FOOD_FOR_X_YEARS,
		SMT_FORT_HAS_FOOD_FOR_X_YEARS,
		SMT_SETTLEMENT_HAS_FOOD_FOR_1_YEAR,
		SMT_FORT_HAS_FOOD_FOR_1_YEAR,
		SMT_SETTLEMENT_HAS_NO_FOOD,
		SMT_FORT_HAS__NO_FOOD,
		SMT_AUTOMANAGE_ALL_TOOLTIP,
		SMT_ALLOW_BUILDING_TOOLTIP,
		SMT_ALLOW_TROOP_PRODUCTION_TOOLTIP,
		SMT_SPEND_SLIDER_TOOLTIP,
		SMT_BUILDING_ADVICE,
		SMT_RECRUITMENT_ADVICE,
		SMT_DAMAGE,
		SMT_INVALID_RALLY_POINT_POSITION,
		SMT_CANNOT_PERFORM_MISSIONS_WITH_MULTIPLE_CHARACTERS,
		SMT_OPEN_MERCENARY_RECRUITMENT,
		SMT_AVAILABLE_MERCENARIES,
		SMT_HIRE_ALL_QUEUED_UNITS,
		SMT_NO_ROOM_FOR_MERCENARIES,
		SMT_NO_MERCENARIES_AVAILABLE,
		SMT_CANNOT_AFFORD_THIS_MERCENARY,
		SMT_ONLY_FAMILY_CAN_RECRUIT_MERCENARIES,
		SMT_ASSISTANCE_REQUESTED,
		SMT_WHICH_ALLY,
		SMT_WHICH_ARMY_REQUESTED,
		SMT_ACCEPT_ASSISTANCE_REQUEST,
		SMT_ATTACKING_THIS_SETTLEMENT_WILL_BREAK_ALLIANCE,
		SMT_ATTACKING_THIS_FORT_WILL_BREAK_ALLIANCE,
		SMT_ATTACKING_THIS_PORT_WILL_BREAK_ALLIANCE,
		SMT_ATTACKING_THIS_ARMY_WILL_BREAK_ALLIANCE,
		SMT_ATTACKING_THIS_NAVY_WILL_BREAK_ALLIANCE,
		SMT_CANNOT_ATTACK_ALLY,
		SMT_CONFIRM_ATTACK_ON_ALLY,
	    SMT_NONE,
	    SMT_BALANCE_OF_POWER,
		SMT_YOUR_FORCES,
		SMT_ENEMY_FORCES,
		SMT_AI_REINFORCEMENT,
		SMT_REINFORCEMENTS,
		SMT_BATTLE_DEPLOYMENT,
		SMT_YOUR_ARMY_SALLIES_FORTH,
		SMT_ENEMY_SALLIES_FORTH,
		SMT_RIGHT_CLICK_FOR_CHARACTER_INFO,
		SMT_FIELD_CONSTRUCTION,
		SMT_PROVINCE,
		SMT_WATCHTOWER_DESCRIPTION,
		SMT_FORT_DESCRIPTION,
		SMT_CANNOT_TRANSFER_GENERALS_UNIT,
		SMT_FAMOUS_BATTLE_SITE,
		SMT_BATTLE_YEAR,
		SMT_VICTOR,
		SMT_LOSER,
		SMT_GAMES_FREQUENCY_YEARLY,
		SMT_GAMES_FREQUENCY_MONTHLY,
		SMT_GAMES_FREQUENCY_DAILY,
		SMT_RACES_FREQUENCY_YEARLY,
		SMT_RACES_FREQUENCY_MONTHLY,
		SMT_RACES_FREQUENCY_DAILY,
		SMT_GAMES_AND_RACES_FREQUENCY_YEARLY,
		SMT_GAMES_AND_RACES_FREQUENCY_MONTHLY,
		SMT_GAMES_AND_RACES_FREQUENCY_DAILY,
		SMT_SHOW_SETTLEMENT_STATS,
		SMT_AUTOMANAGE,
		SMT_SETTLEMENT_STATISTICS,
		SMT_PUBLIC_ORDER,
		SMT_POPULATION_GROWTH,
		SMT_SET_POLICIES,
		SMT_POPULATION_REQUIRED,
		SMT_CONSTRUCTION_TIME,
		SMT_CONSTRUCTION_COST,
		SMT_ANCILLARIES,
		SMT_TRAITS,
		SMT_RECRUITMENT_OPTIONS,
		SMT_MEN_IN_GARRISON,
		SMT_MEN_IN_ARMY,
		SMT_SELECTED_MERCENARIES,
		SMT_SHOW_FACTION_RANKINGS,
		SMT_FACTION_RANKINGS,
		SMT_RANKING_SCORE,
		SMT_RANKING_MILITARY,
		SMT_RANKING_PRODUCTION,
		SMT_RANKING_REGIONS,
		SMT_RANKING_FINANCIAL,
		SMT_RANKING_POPULATION,
		SMT_FACTION_FILTER,
		SMT_OWN_FACTION,
		SMT_ALL_FACTIONS,
		SMT_TOP_5_FACTIONS,
		SMT_CUSTOM_SELECTION,
		SMT_FILTER_NEIGHBOURS,
		SMT_TURNS,
		SMT_VICTORY,
		SMT_DEFEAT,
		SMT_VICTORY_BODY,
		SMT_DEFEAT_BODY,
		SMT_DEFEATED_BY_BODY,
		SMT_DEFEAT_BODY_PROLOGUE,
		SMT_DEFEAT_GAME_OVER,
		SMT_CONTINUE_CAMPAIGN,
		SMT_EXIT_CAMPAIGN,
		SMT_CAPTURED_SETTLEMENT,
	    SMT_OCCUPATION_OPTIONS,
		SMT_DENARI_GAINED_FROM_LOOTING,
		SMT_POPULATION_KILLED,
		SMT_LOOT_SETTLEMENT,
		SMT_SACK_SETTLEMENT,
		SMT_EXTERMINATE,
		SMT_SETTLEMENT_LOOT_BODY_STRING,
		SMT_BUILDINGS_NEEDING_REPAIR,
		SMT_SHOW_NEW_BUILDINGS,
		SMT_SHOW_REPAIRABLE_BUILDINGS,
		SMT_REPAIR_TIME,
		SMT_REPAIR_COST,
		SMT_BUILD_POINTS,
		SMT_ALWAYS_SHOW_ADVANCED_STATS,
		SMT_ADVANCED_STATS,
		SMT_SHOW_TECH_TREE,
		SMT_BUILDING_BROWSER,
		SMT_STATE_ICON_FARMING_LEVEL,
		SMT_STATE_ICON_FARMS_BUILT,
		SMT_STATE_ICON_HEALTH,
		SMT_STATE_ICON_BUILDINGS,
		SMT_STATE_ICON_TAX_RATE_BONUS,
		SMT_STATE_ICON_SQUALOUR,
		SMT_STATE_ICON_TAX_RATE_PENALTY,
		SMT_STATE_ICON_GARRISON,
		SMT_STATE_ICON_BUILDINGS_LAW,
		SMT_STATE_ICON_BUILDINGS_FUN,
		SMT_STATE_ICON_GOVERNORS_INFLUENCE,
		SMT_STATE_ICON_TAX_BONUS,
		SMT_STATE_ICON_TRIUMPH,
		SMT_STATE_ICON_BOOM,
		SMT_STATE_ICON_DISTANCE_TO_CAPITAL,
		SMT_STATE_ICON_NO_GOVERNANCE,
		SMT_STATE_ICON_TAX_PENALTY,
		SMT_STATE_ICON_TURMOIL,
		SMT_STATE_ICON_BESIEGED,
		SMT_STATE_ICON_BLOCKADED,
		SMT_STATE_ICON_ENTERTAINED,
		SMT_STATE_ICON_FARMS,
		SMT_STATE_ICON_TAXES,
		SMT_STATE_ICON_RESOURCES,
		SMT_STATE_ICON_TRADE,
		SMT_STATE_ICON_DIPLOMATIC_INCOME,
		SMT_STATE_ICON_DEMOLITION,
		SMT_STATE_ICON_LOOTING,
		SMT_STATE_ICON_ADMIN,
		SMT_STATE_ICON_WAGES,
		SMT_STATE_ICON_UPKEEP,
		SMT_STATE_ICON_CONSTRUCTION,
		SMT_STATE_ICON_RECRUITMENT,
		SMT_STATE_ICON_DIPLOMATIC_OUTGOINGS,
		SMT_STATE_ICON_CORRUPTION,
		SMT_TECH_TREE_THREAD_ALREADY_BUILT,
		SMT_TECH_TREE_THREAD_AVAILABLE_TO_BUILD,
		SMT_TECH_TREE_THREAD_NOT_AVAILABLE_TO_BUILD,
		SMT_TURNS_UNTIL_SURRENDER,
		SMT_NO_ASSASSINATION_TARGETS,
		SMT_NO_SABOTAGE_TARGETS,
		SMT_MISSION_TARGET,
		SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE,
		SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE,
		SMT_GARRISON_BREAKDOWN,
		SMT_NAVY_BREAKDOWN,
		SMT_INFANTRY,
		SMT_CAVALRY,
		SMT_ARTILLERY,
		SMT_SHIPS,
		SMT_ARMY_FULL,
		SMT_NAVY_FULL,
		SMT_NO_GARRISON_COMMANDER,
		SMT_SAILORS,
		SMT_CAPABILITY_POPULATION_GROWTH_BONUS,
		SMT_CAPABILITY_POPULATION_LOYALTY_BONUS,
		SMT_CAPABILITY_POPULATION_HEALTH_BONUS,
		SMT_CAPABILITY_TRADE_BASE_INCOME_BONUS,
		SMT_CAPABILITY_TRADE_LEVEL_BONUS,
		SMT_CAPABILITY_TRADE_FLEET,
		SMT_CAPABILITY_TAXABLE_INCOME_BONUS,
		SMT_CAPABILITY_MINE_RESOURCE,
		SMT_CAPABILITY_FARMING_LEVEL,
		SMT_CAPABILITY_ROAD_LEVEL,
		SMT_CAPABILITY_GATE_STRENGTH,
		SMT_CAPABILITY_GATE_DEFENSES,
		SMT_CAPABILITY_TOWER_LEVEL,
		SMT_CAPABILITY_ARMOUR,
		SMT_CAPABILITY_STAGE_GAMES,
		SMT_CAPABILITY_STAGE_RACES,
		SMT_CAPABILITY_POPULATION_FIRE_RISK_BONUS,
	    SMT_CAPABILITY_WEAPON_MELEE_SIMPLE,		
	    SMT_CAPABILITY_WEAPON_MELEE_BLADE,		
	    SMT_CAPABILITY_WEAPON_MISSILE_MECHANICAL,
	    SMT_CAPABILITY_WEAPON_MISSILE_GUNPOWDER,
	    SMT_CAPABILITY_WEAPON_ARTILLERY_MECHANICAL,
	    SMT_CAPABILITY_WEAPON_ARTILLERY_GUNPOWDER,
	    SMT_CAPABILITY_WEAPON_NAVAL_GUNPOWDER,
		SMT_CAPABILITY_BODYGUARD,
		SMT_CAPABILITY_RECRUITS_MORALE_BONUS,
		SMT_CAPABILITY_RECRUITS_EXPERIENCE_BONUS,
		SMT_CAPABILITY_HAPPINESS_BONUS,
		SMT_CAPABILITY_LAW_BONUS,
		SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_MILITARY,
		SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_RELIGIOUS,
		SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_DEFENSIVE,
		SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_OTHER,
		SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_MILITARY,
		SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_RELIGIOUS,
		SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_DEFENSIVE,
		SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_OTHER,
	    SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_WOODEN,
	    SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_STONE, 
	    SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_WOODEN,
	    SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_STONE, 
	    SMT_CAPABILITY_ARCHER_EXP_BONUS,
	    SMT_CAPABILITY_CAVALRY_EXP_BONUS,
	    SMT_CAPABILITY_HEAVY_CAVALRY_EXP_BONUS,
	    SMT_CAPABILITY_GUN_EXP_BONUS,
	    SMT_CAPABILITY_NAVY_EXP_BONUS,
	    SMT_CAPABILITY_RECRUITS_COST_BONUS_NAVAL,
	    SMT_CAPABILITY_RETRAIN_COST_BONUS,
	    SMT_CAPABILITY_WEAPON_PROJECTILE,
	    SMT_CAPABILITY_INCOME_BONUS,
		SMT_CAPS_LOC_MAX_POOL,
		SMT_CAPS_LOC_TRAINS_EXPERIENCE,
		SMT_CAPS_LOC_TRAINS,
		SMT_CAPS_LOC_TRAINS_BONUS,
		SMT_CAPS_LOC_RECRUITS_LEVEL,
		SMT_CAPS_LOC_RECRUITS,
		SMT_CAPS_LOC_RECRUITS_BONUS,
		SMT_CAPS_LOC_PROVIDES,
		SMT_CAPS_LOC_PROVIDES_BONUS,
		SMT_CAPS_LOC_REQUIRES,
		SMT_CAPS_LOC_CONDITION_NOT,
		SMT_CAPS_LOC_CONDITION_AND,
		SMT_CAPS_LOC_CONDITION_OR,
		SMT_CAPS_LOC_CONDITION_BUILDING_PRESENT_MIN_LEVEL,
		SMT_CAPS_LOC_CONDITION_CAPABILITY_VALUE,
	    SMT_CAPS_GLOBAL,
		SMT_AUTOMANAGE_SETTLEMENT,
		SMT_CANNOT_RECRUIT_WHILST_IN_BATTLE,
		SMT_VICTORY_CONDITION_LAND_OWNER_TITLE,
		SMT_VICTORY_CONDITION_LAND_OWNER_DESCR,
		SMT_VICTORY_CONDITION_TYCOON_TITLE,
		SMT_VICTORY_CONDITION_TYCOON_DESCR,
		SMT_VICTORY_CONDITION_TIME_LIMIT_TITLE,
		SMT_VICTORY_CONDITION_TIME_LIMIT_DESCR,
		SMT_VICTORY_CONDITION_OUTLIVE_FACTIONS_TITLE,
		SMT_VICTORY_CONDITION_OUTLIVE_FACTIONS_DESCR,
		SMT_CAMPAIGN_VICTORY_CONDITIONS,
		SMT_CAMPAIGN_VICTORY_CONDITIONS_COMPLETE,
		SMT_ENEMY_AMBUSH_FAILED,
		SMT_ENEMY_AMBUSH_SUCCESS,
		SMT_YOUR_AMBUSH_FAILED,
		SMT_YOUR_AMBUSH_SUCCESS,
		SMT_NO_UNITS_IN_NAVY,
		SMT_NO_UNITS_IN_ARMY,
		SMT_NO_CHARACTERS_WITH_NAVY,
		SMT_NO_CHARACTERS_WITH_ARMY,
		SMT_NO_UNITS_TRANSPORTED,
		SMT_NO_UNITS_IN_GARRISON,
		SMT_NO_BUILDINGS_IN_SETTLEMENT,
		SMT_NO_VISITORS_IN_SETTLEMENT,
		SMT_NO_VISITORS_IN_FORT,
		SMT_LAND_IMPORTS,
		SMT_DIED,
		SMT_NO_BUILDING_IN_NEED_OF_REPAIR,
		SMT_NO_UNITS_NEED_RETRAINING,
		SMT_SHOW_VISITORS_FORT,
		SMT_DECLINE_MISSION,
		SMT_SELECT_UNIT_TO_REPAIR,
		SMT_CANNOT_SET_NAVAL_RALLY_POINT_WITH_NO_PORT,
		SMT_DIED_IN_BATTLE_TITLE,
		SMT_DIED_IN_BATTLE_DESCRIPTION,
	    SMT_BREAK_SIEGE_TITLE,
		SMT_BREAK_SIEGE_CONFIRMATION,
		SMT_NO_MORE_OFFERS_ALLOWED,
		SMT_NO_MORE_DEMANDS_ALLOWED,
		SMT_UNITS,
		SMT_SIEGE_EQUIPMENT,
		SMT_SET_AS_CAPITAL,
		SMT_ATTACKING_THIS_CHARACTER_WILL_BREAK_ALLIANCE,
		SMT_ATTACKING_THIS_NAVY_DECLARATION_OF_WAR,
		SMT_ATTACKING_THIS_ARMY_DECLARATION_OF_WAR,
		SMT_ATTACKING_THIS_CHARACTER_DECLARATION_OF_WAR,
		SMT_ATTACKING_THIS_SETTLEMENT_DECLARATION_OF_WAR,
		SMT_ATTACKING_THIS_FORT_DECLARATION_OF_WAR,
		SMT_ATTACKING_THIS_PORT_DECLARATION_OF_WAR,
		SMT_CONFIRM_ATTACK_ON_NEUTRAL,
		SMT_ARMY,
		SMT_NAVY,
		SMT_AGENTS,
		SMT_FLEET,
		SMT_PRIVATE_DEALINGS,
		SMT_CURRENT_TREATIES,
		SMT_FACTION_OVERVIEW,
	    SMT_ROSTERS,
		SMT_FINANCIAL,
		SMT_RADAR_ZOOM_IN,
		SMT_RADAR_ZOOM_OUT,
		SMT_RECRUITMENT,
		SMT_REPAIR,
		SMT_RETRAIN,
		SMT_ASSAULT_NO_INFANTRY,
		SMT_ASSAULT_NO_EQUIPMENT,
		SMT_NO_POPULATION_TO_EXTERMINATE,
		SMT_TOTAL,
		SMT_PROJECTED,
		SMT_STARTING_TREASURY,
		SMT_PROJECTED_END_TREASURY,
		SMT_END_TREASURY,
		SMT_DIPLOMACY_TRIBUTES,
		SMT_DIPLOMACY_CHANGE_YEARLY_PAYMENT,
		SMT_ATTACK_VS_TROOPS,
		SMT_ATTACK_VS_BUILDINGS,
		SMT_MELEE_ATTACK,
		SMT_MISSILE_ATTACK,
		SMT_WEAPON_TYPE,
	    SMT_MELEE_SIMPLE,			
	    SMT_MELEE_BLADE,			
	    SMT_MISSILE_MECHANICAL,
	    SMT_MISSILE_GUNPOWDER,
	    SMT_ARTILLERY_MECHANICAL,
	    SMT_ARTILLERY_GUNPOWDER,
	    SMT_NAVAL_GUNPOWDER,
		SMT_NO_UPGRADE,
		SMT_TOTAL_DEFENCE,
		SMT_STATE_ICON_PLAGUE,
		SMT_SUBTERFUGE_COLON,
		SMT_REWARD_UNITS_AT_YOUR_CAPITAL,
		SMT_CHARACTER_PLAGUED,
		SMT_LOCATE_MISSION_TARGET,
		SMT_REPAIRED,
		SMT_RETRAINED,
		SMT_VARIES,
		SMT_GATES_OPEN_SETTLEMENT,
		SMT_GATES_OPEN_FORT,
		SMT_CORRUPTION_AND_OTHER,
		SMT_SELECT_FORT_OR_WATCHTOWER,
		SMT_FAMILY_MEMBERS,
		SMT_CAPITAL_TITLE,
		SMT_GARRISON_COMMANDER,
		SMT_BLOCKADED,
		SMT_WITHIN_X_TURNS,
		SMT_CHANCE_OF_SUCCESS_X_PERCENT,
		SMT_TIME_TO_PROCESS_SIEGE_QUEUE,
		SMT_CHARACTER_BRIBED,
		SMT_SETTLEMENT_BRIBED,
		SMT_VISITORS_BRIBED,
		SMT_TRAVELLERS_BRIBED,
		SMT_ENTIRE_ARMY_BRIBED,
		SMT_ENTIRE_ARMY_DISBANDED,
		SMT_UNITS_BRIBED,
		SMT_UNITS_DISBANDED,
		SMT_TAX,
		SMT_AUTOMANAGE_TAXES_TOOLTIP,
		SMT_AUTOMATE_TAXES,
		SMT_QUICKSAVE,
		SMT_LOCATE_BATTLE,
		SMT_STATE_ICON_DEVASTATION,
		SMT_INAPPROPRIATE_UNITS_FOR_ASSAULT,
		SMT_EXCHANGE_IN_ZOC,
		SMT_OPEN_DEMANDS,
		SMT_OPEN_OFFERS,
		SMT_THIS_CHARACTER_IN_YOUR_FACTION,
		SMT_1_REGION_LEFT,
		SMT_NEED_GOVERNOR_TO_MANAGE,
		SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_FORT,
		SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_FORT,
		SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_FORT_1_TURN,
		SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_FORT_1_TURN,
		SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_1_TURN,
		SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_1_TURN,
		SMT_SHOW_CHARACTER_INFO,
		SMT_STATE_ICON_UPKEEP_NEW,
		SMT_BY,
		SMT_ON,
		SMT_ALLY,
		SMT_ENEMY,
		SMT_GATES_OPEN_SETTLEMENT_ACTUAL,
		SMT_GATES_OPEN_FORT_ACTUAL,
		SMT_DESERTED,
		SMT_DIED_TRAGIC,
		SMT_DIED_BATTLE,
		SMT_DIED_ASSASSINATED,
		SMT_EXECUTED,
		SMT_BORN,
		SMT_DIED_PLAGUE,
		SMT_NEXT,
		SMT_1_YEAR_REMAINING,
		SMT_X_YEARS_REMAINING,
		SMT_REWARD_ONE_TRIARII_DESCRIPTION,
		SMT_REWARD_TWO_TRIARII_DESCRIPTION,
		SMT_SHOW_FACTION_LISTS,
		SMT_OCCUPY_SETTLEMENT,
		SMT_RAZE_SETTLEMENT,
		SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_1,
		SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_2,
		SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_3,
		SMT_KINGS_PURSE,
		SMT_DOMINANT_RELIGION,
		SMT_BELIEFS,
		SMT_CURRENT_COMMITMENT,
		SMT_PREDICTED_COMMITMENT,
		SMT_ABANDON_SETTLEMENT,
		SMT_CANNOT_FIGHT_AT_NIGHT,
		SMT_TECH_TREE_THREAD_REQUIRES_TEMPLE_DESTRUCTION,
		SMT_SUPRESSES_RELIGIOUS_UNREST,
		SMT_SUPRESSES_RELIGIOUS_CONVERSION,
		SMT_LIVING_FACTIONS,
		SMT_CANNOT_ABANDON_SETTLEMENT_UNDER_SIEGE,
		SMT_CANNOT_ABANDON_SETTLEMENT_NOT_LAST_SETTLEMENT,
		SMT_CANNOT_ABANDON_SETTLEMENT_ALREADY_ABANDONED,
		SMT_CANNOT_ABANDON_SETTLEMENT_HORDE_UNITS,
		SMT_CURRENT_CONVERSTION_STRENGTH,
		SMT_PREDICTED_CONVERSION_STRENGTH,
		SMT_PASSENGERS_CANT_BE_MOVED,
		SMT_CANNOT_OCCUPY_SACKED_SETTLEMENT,
		SMT_DISBAND_IMPOSSIBLE_IN_BATTLE,
		SMT_DISBAND_IMPOSSIBLE_NAMED_GENERAL,
		SMT_DISBAND_IMPOSSIBLE_WHILST_SIEGING,
		SMT_DISBAND_IMPOSSIBLE_DOCKED,
		SMT_DISBAND_IMPOSSIBLE_NOT_ON_LAND,
		SMT_DISBAND_IMPOSSIBLE_WHILST_EXCHANGING,
		SMT_MERCENARY_PAY_FROM_LOOTING,
		SMT_DOMINATION_VICTORY,
		SMT_MEET_ENEMY,
		SMT_ENEMY_MEETS_YOU,
		SMT_MERCHANT_DESCR,
		SMT_DIPLOMACY_OFFER_MARRY_LEADER_TO_PRINCESS,
		SMT_DIPLOMACY_OFFER_MARRY_HEIR_TO_PRINCESS,
		SMT_DIPLOMACY_OFFER_MARRY_LEADER,
		SMT_DIPLOMACY_OFFER_MARRY_HEIR,
		SMT_ACQUIRE,
		SMT_NO_ACQUISITION_TARGETS,
		SMT_DENOUNCE,
		SMT_NO_DENOUNCEMENT_TARGETS,
		SMT_DENOUNCED,
		SMT_RETIRED,
		SMT_DISGRACED,
		SMT_MARRY_GENERALS,
		SMT_MARRIAGE,
		SMT_MARRY,
		SMT_RELIGION,
		SMT_NO_MARRIAGE_TARGETS,
		SMT_AGENT_LIMIT_REACHED,
		SMT_CAPABILITY_RELIGION_LEVEL,
	    SMT_CAPABILITY_RELIGION_LEVEL_AMPLIFIER,
		SMT_FLEE,
		SMT_REMAIN,
		SMT_THE_POPE,
		SMT_POPES_NAME,
		SMT_POPES_DETAILS,
		SMT_COLLEGE_OF_CARDINALS,
		SMT_RANK,
		SMT_EMPTY_SEATS,
		SMT_SHOW_COLLEGE_OF_CARDINALS,
		SMT_PAPAL_ELECTION,
		SMT_PAPAL_ELECTION_RESULTS,
		SMT_STATE_ICON_RELIGIOUS_UNREST,
	    SMT_CASTLE,
	    SMT_CONFIRM_CONVERT_SETTLEMENT,
	    SMT_CONVERT_CASTLE_TO_CITY_FOREVER,
	    SMT_CONVERT_CASTLE_TO_CITY,
	    SMT_CONVERT_CITY_TO_CASTLE,
	    SMT_CRUSADE_TARGETS_TITLE,
	    SMT_JIHAD_TARGETS_TITLE,
	    SMT_CRUSADE_TARGETS_BODY,
	    SMT_JIHAD_TARGETS_BODY,
	    SMT_CRUSADE_NO_TARGETS,
	    SMT_JIHAD_NO_TARGETS,
	    SMT_CONFIRM_JOIN_CRUSADE_AGAINST_ALLY,
	    SMT_CONFIRM_JOIN_JIHAD_AGAINST_ALLY,
	    SMT_JOIN_CRUSADE_WILL_BREAK_ALLIANCE,
	    SMT_JOIN_JIHAD_WILL_BREAK_ALLIANCE,
	    SMT_CONFIRM_JOIN_CRUSADE_AGAINST_NEUTRAL,
	    SMT_CONFIRM_JOIN_JIHAD_AGAINST_NEUTRAL,
	    SMT_JOIN_CRUSADE_DECLARATION_OF_WAR,
	    SMT_JOIN_JIHAD_DECLARATION_OF_WAR,
		SMT_PAPAL_STANDING,
		SMT_PAPAL_STANDING_DESCR,
		SMT_PAPAL_STANDING_0,
		SMT_PAPAL_STANDING_1,
		SMT_PAPAL_STANDING_2,
		SMT_PAPAL_STANDING_3,
		SMT_PAPAL_STANDING_4,
		SMT_PAPAL_STANDING_5,
		SMT_PAPAL_STANDING_6,
		SMT_PAPAL_STANDING_7,
		SMT_PAPAL_STANDING_8,
		SMT_PAPAL_STANDING_9,
		SMT_PAPAL_STANDING_10,
		SMT_RECRUITMENT_BUILDING_DAMAGED,
		SMT_SELECTED_UNIT_COST,
		SMT_SELECTED_UNITS,
		SMT_RECRUITMENT_POOL_EMPTY,
		SMT_RETRAIN_POOL_EMPTY,
		SMT_RECRUITMENT_ORDER_FULL,
		SMT_REPAIR_QUEUE,
		SMT_RETRAINING_QUEUE,
		SMT_PURCHASE_RECRUITMENT,
		SMT_CAPABILITY_FREE_UPKEEP,
		SMT_FREE_UPKEEP,
		SMT_NO_PORT,
		SMT_RECRUITMENT_NON_COAST,
		SMT_TURN_X,
		SMT_1_TURN_REMAINING,
		SMT_X_TURNS_REMAINING,
		SMT_MISSIONS,
		SMT_MISSION_SOURCE,
		SMT_MISSION_DESCRIPTION,
		SMT_NO_MISSIONS,
		SMT_REWARD,
		SMT_PENALTY,
		SMT_PRE_REWARD_MONEY,
		SMT_PRE_PENALTY_MONEY,
		SMT_REWARD_MONEY,
		SMT_PENALTY_MONEY,
		SMT_PRE_PENALTY_INQUISITION,
		SMT_PENALTY_INQUISITION,
		SMT_PRE_PENALTY_EXCOMMUNICATION,
		SMT_PENALTY_EXCOMMUNICATION,
		SMT_PRE_REWARD_INCOME,
		SMT_PRE_PENALTY_INCOME,
		SMT_PENALTY_INCOME,
		SMT_REWARD_INCOME,
		SMT_PRE_REWARD_GUILD_MONEY,
		SMT_PRE_PENALTY_GUILD_MONEY,
		SMT_REWARD_GUILD_MONEY,
		SMT_PENALTY_GUILD_MONEY,
		SMT_PRE_PENALTY_ASSASSINATION,
		SMT_PENALTY_ASSASSINATION,
		SMT_PRE_REWARD_BUILDABLE_UNIT,
		SMT_PRE_REWARD_BUILDABLE_UNITS,
		SMT_PRE_REWARD_BEST_BUILDABLE_UNIT,
		SMT_PRE_REWARD_BEST_BUILDABLE_UNITS,
		SMT_PRE_REWARD_BEST_FACTION_UNIT,
		SMT_PRE_REWARD_BEST_FACTION_UNITS,
		SMT_REWARD_CANNOT_BUILD_A_UNIT,
		SMT_MISSION_RETURN_PAPAL_SETTLEMENT_SUCCESS,
		SMT_MISSION_RETURN_PAPAL_SETTLEMENT_FAILURE,
		SMT_MISSION_RETURN_PAPAL_SETTLEMENT_TITLE,
		SMT_MISSION_RETURN_PAPAL_SETTLEMENT_PROPOSAL,
		SMT_MISSION_JOIN_CRUSADE_SUCCESS,
		SMT_MISSION_JOIN_CRUSADE_EXPIRED_FINISHED,
		SMT_MISSION_JOIN_CRUSADE_EXPIRED_DEATH,
		SMT_MISSION_JOIN_CRUSADE_EXPIRED_EXCOMMUNICATED,
		SMT_MISSION_JOIN_CRUSADE_FAILURE,
		SMT_MISSION_JOIN_CRUSADE_TITLE,
		SMT_MISSION_JOIN_CRUSADE_HEIR_TITLE,
		SMT_MISSION_JOIN_CRUSADE_LEADER_TITLE,
		SMT_MISSION_JOIN_CRUSADE_CHARACTER_PROPOSAL,
		SMT_MISSION_JOIN_CRUSADE_GENERAL_PROPOSAL,
		SMT_MISSION_BREAK_ALLIANCE_TITLE,
		SMT_MISSION_BREAK_ALLIANCE_PROPOSAL,
		SMT_MISSION_BREAK_ALLIANCE_EXPIRY_DEATH,
		SMT_MISSION_BREAK_ALLIANCE_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_BREAK_ALLIANCE_EXPIRY_RECOMMUNICATED,
		SMT_MISSION_BREAK_ALLIANCE_FAILURE,
		SMT_MISSION_BREAK_ALLIANCE_SUCCESS,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_TITLE,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_PROPOSAL,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_SETTLEMENT_TAKEN,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_RECOMMUNICATED,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_BLOCKADED,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_FAILURE,
		SMT_MISSION_PAPAL_BLOCKADE_PORT_SUCCESS,
		SMT_MISSION_ASSASSINATE_CARDINAL_TITLE,
		SMT_MISSION_ASSASSINATE_CARDINAL_PROPOSAL,
		SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_DEATH,
		SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_HIDING,
		SMT_MISSION_ASSASSINATE_CARDINAL_FAILURE,
		SMT_MISSION_ASSASSINATE_CARDINAL_SUCCESS,
		SMT_MISSION_ASSASSINATE_WITCH_TITLE,
		SMT_MISSION_ASSASSINATE_WITCH_PROPOSAL,
		SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_DEATH,
		SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_HIDING,
		SMT_MISSION_ASSASSINATE_WITCH_FAILURE,
		SMT_MISSION_ASSASSINATE_WITCH_SUCCESS,
		SMT_MISSION_CEASE_HOSTILITIES_TITLE,
		SMT_MISSION_CEASE_HOSTILITIES_PROPOSAL,
		SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_DEATH,
		SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_TARGET_EXCOMMUNICATED,
		SMT_MISSION_CEASE_HOSTILITIES_FAILURE,
		SMT_MISSION_CEASE_HOSTILITIES_SUCCESS,
		SMT_MISSION_PAPAL_BUILD_CHURCH_TITLE,
		SMT_MISSION_PAPAL_BUILD_CHURCH_PROPOSAL,
		SMT_MISSION_PAPAL_BUILD_CHURCH_EXPIRY_LOST,
		SMT_MISSION_PAPAL_BUILD_CHURCH_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_PAPAL_BUILD_CHURCH_FAILURE,
		SMT_MISSION_PAPAL_BUILD_CHURCH_SUCCESS,
		SMT_MISSION_CONVERT_TITLE,
		SMT_MISSION_CONVERT_PROPOSAL,
		SMT_MISSION_CONVERT_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_CONVERT_FAILURE,
		SMT_MISSION_CONVERT_SUCCESS,
		SMT_MISSION_GIFT_FROM_POPE_TITLE,
		SMT_MISSION_GIFT_FROM_POPE_PROPOSAL,
		SMT_MISSION_PAPAL_GIVE_CASH_TITLE,
		SMT_MISSION_PAPAL_GIVE_CASH_PROPOSAL,
		SMT_MISSION_PAPAL_GIVE_CASH_FAILURE,
		SMT_MISSION_PAPAL_GIVE_CASH_SUCCESS,
		SMT_MISSION_PAPAL_GIVE_ANCILLARY_TITLE,
		SMT_MISSION_PAPAL_GIVE_ANCILLARY_PROPOSAL,
		SMT_MISSION_PAPAL_GIVE_ANCILLARY_FAILURE,
		SMT_MISSION_PAPAL_GIVE_ANCILLARY_SUCCESS,
		SMT_MISSION_RECRUIT_PRIEST_TITLE,
		SMT_MISSION_RECRUIT_PRIEST_PROPOSAL,
		SMT_MISSION_RECRUIT_PRIEST_EXPIRY_LOST,
		SMT_MISSION_RECRUIT_PRIEST_EXPIRY_EXCOMMUNICATED,
		SMT_MISSION_RECRUIT_PRIEST_FAILURE,
		SMT_MISSION_RECRUIT_PRIEST_SUCCESS,
		SMT_MISSION_ASSASSINATE_GENERAL_TITLE,
		SMT_MISSION_ASSASSINATE_GENERAL_PROPOSAL,
		SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_DEATH,
		SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_ALLIED,
		SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_HIDING,
		SMT_MISSION_ASSASSINATE_GENERAL_FAILURE,
		SMT_MISSION_ASSASSINATE_GENERAL_SUCCESS,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_TITLE,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_PROPOSAL,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_SETTLEMENT_TAKEN,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_ALLIED,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_BLOCKADED,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_FAILURE,
		SMT_MISSION_COUNCIL_BLOCKADE_PORT_SUCCESS,
		SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_TITLE,
		SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_PROPOSAL,
		SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_EXPIRY_LOST,
		SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_FAILURE,
		SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_SUCCESS,
		SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_TITLE,
		SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_PROPOSAL,
		SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_EXPIRY_LOST,
		SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_EXPIRY_ALLIED,
		SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_FAILURE,
		SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_SUCCESS,
		SMT_MISSION_GUILD_TAKE_SETTLEMENT_TITLE,
		SMT_MISSION_GUILD_TAKE_SETTLEMENT_PROPOSAL,
		SMT_MISSION_GUILD_TAKE_SETTLEMENT_EXPIRY_LOST,
		SMT_MISSION_GUILD_TAKE_SETTLEMENT_EXPIRY_ALLIED,
		SMT_MISSION_GUILD_TAKE_SETTLEMENT_FAILURE,
		SMT_MISSION_GUILD_TAKE_SETTLEMENT_SUCCESS,
		SMT_MISSION_GUILD_RECRUIT_AGENT_TITLE,
		SMT_MISSION_GUILD_RECRUIT_AGENT_PROPOSAL,
		SMT_MISSION_GUILD_RECRUIT_AGENT_EXPIRY_LOST,
		SMT_MISSION_GUILD_RECRUIT_AGENT_FAILURE,
		SMT_MISSION_GUILD_RECRUIT_AGENT_SUCCESS,
		SMT_MISSION_GIFT_FROM_COUNCIL_TITLE,
		SMT_MISSION_GIFT_FROM_COUNCIL_PROPOSAL,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_TITLE,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_PROPOSAL,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_GENERAL_DEATH,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_PRINCESS_DEATH,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_TARGET_MARRIED,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_FAILURE_PRINCESS_MARRIED,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_PRINCESS_RETIRED,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_FAILURE,
		SMT_MISSION_COUNCIL_MARRY_PRINCESS_SUCCESS,
		SMT_MISSION_ACQUISITION_TITLE,
		SMT_MISSION_ACQUISITION_PROPOSAL,
		SMT_MISSION_ACQUISITION_EXPIRY_DEATH,
		SMT_MISSION_ACQUISITION_EXPIRY_HIDING,
		SMT_MISSION_ACQUISITION_FAILURE,
		SMT_MISSION_ACQUISITION_SUCCESS,
		SMT_MISSION_TRADE_AGREEMENT_TITLE,
		SMT_MISSION_TRADE_AGREEMENT_PROPOSAL,
		SMT_MISSION_TRADE_AGREEMENT_EXPIRY_DEATH,
		SMT_MISSION_TRADE_AGREEMENT_EXPIRY_WAR,
		SMT_MISSION_TRADE_AGREEMENT_FAILURE,
		SMT_MISSION_TRADE_AGREEMENT_SUCCESS,
		SMT_MISSION_ANNEX_SETTLEMENT_TITLE,
		SMT_MISSION_ANNEX_SETTLEMENT_PROPOSAL,
		SMT_MISSION_ANNEX_SETTLEMENT_EXPIRY_DEATH,
		SMT_MISSION_ANNEX_SETTLEMENT_EXPIRY_LOST,
		SMT_MISSION_ANNEX_SETTLEMENT_FAILURE,
		SMT_MISSION_ANNEX_SETTLEMENT_SUCCESS,
		SMT_MISSION_SEND_EMISSARY_TITLE,
		SMT_MISSION_SEND_EMISSARY_PROPOSAL,
		SMT_MISSION_SEND_EMISSARY_EXPIRY_DEATH,
		SMT_MISSION_SEND_EMISSARY_FAILURE,
		SMT_MISSION_SEND_EMISSARY_SUCCESS,
		SMT_MISSION_REINFORCE_REGION_TITLE,
		SMT_MISSION_REINFORCE_REGION_PROPOSAL,
		SMT_MISSION_REINFORCE_REGION_EXPIRY_LOST,
		SMT_MISSION_REINFORCE_REGION_FAILURE,
		SMT_MISSION_REINFORCE_REGION_SUCCESS,
		SMT_MISSION_GUILD_BUILD_TITLE,
		SMT_MISSION_GUILD_BUILD_PROPOSAL,
		SMT_MISSION_REGICIDE_TITLE,
		SMT_MISSION_REGICIDE_PROPOSAL,
		SMT_MISSION_REGICIDE_EXPIRY_DEATH,
		SMT_MISSION_REGICIDE_EXPIRY_HIDING,
		SMT_MISSION_REGICIDE_FAILURE,
		SMT_MISSION_REGICIDE_SUCCESS,
		SMT_MISSION_CRUSADE_TITLE,
		SMT_MISSION_CRUSADE_PROPOSAL,
		SMT_MISSION_CRUSADE_TARGET_PROPOSAL,
		SMT_MISSION_JIHAD_TITLE,
		SMT_MISSION_JIHAD_PROPOSAL,
		SMT_MISSION_JIHAD_TARGET_PROPOSAL,
		SMT_VOTE,
		SMT_PREFERATI,
		SMT_SUPPORTERS,
		SMT_UNDECIDED_VOTES,
		SMT_CLICK_DIPLOMACY,
		SMT_VOTES,
		SMT_PAPAL_STANDING_ELECTION_0,
		SMT_PAPAL_STANDING_ELECTION_1,
		SMT_PAPAL_STANDING_ELECTION_2,
		SMT_PAPAL_STANDING_ELECTION_3,
		SMT_PAPAL_STANDING_ELECTION_4,
		SMT_PAPAL_STANDING_ELECTION_5,
		SMT_PAPAL_STANDING_ELECTION_6,
		SMT_PAPAL_STANDING_ELECTION_7,
		SMT_PAPAL_STANDING_ELECTION_8,
		SMT_PAPAL_STANDING_ELECTION_9,
		SMT_PAPAL_STANDING_ELECTION_10,
		SMT_POPE_ELECTED,
		SMT_VOTED_POPE,
		SMT_VOTED_NO_POPE,
		SMT_NO_POPE,
		SMT_EXCOMMUNICATE,
		SMT_ACCEPT_ELECTION,
		SMT_DIPLOMACY_OFFER_RECOMMUNICATION_DEMAND,
		SMT_DIPLOMACY_OFFER_RECOMMUNICATION_OFFER,
		SMT_DIPLOMACY_OFFER_CARDINAL_VOTE_DEMAND,
		SMT_DIPLOMACY_OFFER_CARDINAL_VOTE_OFFER,
		SMT_STATE_ICON_FEAR,
		SMT_STATE_ICON_GLORY,
		SMT_STATE_ICON_EXCOMMUNICATION,
		SMT_ATTACKING_THIS_CRUSADE_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_NAVY_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_ARMY_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_CHARACTER_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_SETTLEMENT_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_FORT_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_PORT_WILL_EXCOMMUNICATE,
		SMT_ATTACKING_THIS_CRUSADE_WILL_EXCOMMUNICATE_ASSASSIN,
		SMT_ATTACKING_THIS_CHARACTER_WILL_EXCOMMUNICATE_ASSASSIN,
		SMT_ATTACKING_THIS_CHARACTER_WILL_BREAK_ALLIANCE_ASSASSIN,
		SMT_ATTACKING_THIS_CHARACTER_DECLARATION_OF_WAR_ASSASSIN,
		SMT_ATTACKING_POPE_WILL_EXCOMMUNICATE_ASSASSIN,
		SMT_ATTACKING_POPE_WILL_EXCOMMUNICATE,
		SMT_CONFIRM_EXCOMMUNICATION_ATTACK,
		SMT_CAPABILITY_RECRUITMENT_SLOTS,
		SMT_SLOTS_REMAINING,
		SMT_PRINCESS_MERGE,
		SMT_PRINCESS_DIPLOMACY,
	    SMT_FINANCE_BUTTON,
		SMT_MISSION_BUTTON,
		SMT_MAKE_OFFER_BUTTON,
		SMT_PRISONERS_TITLE_CAPTOR,
		SMT_PRISONERS_TITLE_CAPTIVE,
		SMT_RANSOM_BUTTON_TITLE_CAPTOR,
		SMT_RANSOM_BUTTON_TITLE_CAPTIVE,
		SMT_TROOPS_CAPTURED_CAPTOR,
		SMT_TROOPS_CAPTURED_CAPTIVE,
		SMT_TOTAL_RANSOM_VALUE,
		SMT_RANSOM_BUTTON,
		SMT_RELEASE_BUTTON,
		SMT_EXECUTE_BUTTON,
		SMT_ACCEPT_RANSOM_BUTTON,
		SMT_REJECT_RANSOM_BUTTON,
		SMT_CHARACTER_RANK,
		SMT_UNIT,
		SMT_MEN_CAPTURED,
		SMT_RANSOM_VALUE,
		SMT_RANSOM_ACCEPTED_TITLE,
		SMT_RANSOM_REJECTED_TITLE,
		SMT_RANSOM_COULD_NOT_PAY_TITLE,
		SMT_RANSOM_ACCEPTED_BODY,
		SMT_RANSOM_REJECTED_BODY,
		SMT_RANSOM_COULD_NOT_PAY_BODY,
		SMT_EXECUTED_PRISONER,
		SMT_RELEASED_X_PRISONERS,
		SMT_EXECUTED_X_PRISONERS,
		SMT_HUMAN,
		SMT_AI,
		SMT_NEXT_ARMOUR_LEVEL,
		SMT_HOTSEAT_TITLE,
		SMT_ENTER_PASSWORD,
		SMT_VERIFY_PASSWORD,
		SMT_PASSWORD_MISMATCH,
		SMT_INCORRECT_PASSWORD,
		SMT_HOTSEAT_START_TURN,
		SMT_INVALID_PASSWORD_ASCII,
		SMT_INVALID_PASSWORD_LENGTH,
		SMT_SAVEGAME_STATUS,
		SMT_SAVEGAME_STATUS_GOOD,
		SMT_SAVEGAME_STATUS_TAMPERED,
		SMT_SAVEGAME_VERSION,
		SMT_SAVEGAME_VERSION_DETAILS,
		SMT_SAVEGAME_VERSION_DETAILS_UNKNOWN,
		SMT_ADMIN_PASSWORD,
		SMT_NUM_HUMAN_PLAYERS,
		SMT_AUTORESOLVE_BATTLES,
		SMT_CONSOLE_DISABLED,
		SMT_SAVED_SETTINGS,
		SMT_PLAY_BY_EMAIL,
		SMT_EMAIL_ADDRESS,
		SMT_HUMAN_FACTIONS,
		SMT_AI_FACTIONS,
		SMT_HS_GAME_SETTINGS,
		SMT_HS_HOTSEAT_SETTINGS,
		SMT_HS_INTEGRITY_SETTINGS,
		SMT_HS_GAME_NAME,
		SMT_HS_VICTOR,
		SMT_HS_DATA_STATUS,
		SMT_HS_DATA_STATUS_GOOD,
		SMT_HS_DATA_STATUS_BAD,
		SMT_HS_ALLOW_INTERGITY_FAILURE,
		SMT_HS_CURRENT_GAME_VERSION,
		SMT_HS_QUIT_ON_SAVE,
		SMT_HS_PASSWORD_INSTRUCTIONS_BLANK,
		SMT_HS_PASSWORD_INSTRUCTIONS_SET,
		SMT_HS_START_TURN,
		SMT_PLAYER_CONTROL,
		SMT_ASSAULT_INVALID_SIEGE_TOWERS,
		SMT_HOTSEAT_MESSAGE_TITLE,
		SMT_HS_CLEAR_BUTTON,
		SMT_HS_CANCEL_BUTTON,
		SMT_HS_SEND_BUTTON,
		SMT_RELIGION_RECRUITMENT_REQUIREMENT,
		SMT_STATE_HOLDING_RACES,
		SMT_STATE_NOT_HOLDING_RACES,
		SMT_SETTLEMENT_INFILTRATED,
		STRAT_TEXT_COUNT
	};

	const std::unordered_map<std::string, stratTextEnum> stratTextEnumMap = {
	    {"SMT_END_TURN", SMT_END_TURN},
	    {"SMT_SABOTAGE", SMT_SABOTAGE},
	    {"SMT_ASSASSINATE", SMT_ASSASSINATE},
	    {"SMT_SETTLEMENTS", SMT_SETTLEMENTS},
	    {"SMT_CAPITAL", SMT_CAPITAL},
	    {"SMT_ALLIES", SMT_ALLIES},
	    {"SMT_ENEMIES", SMT_ENEMIES},
	    {"SMT_SUSPICIOUS", SMT_SUSPICIOUS},
	    {"SMT_NEUTRAL", SMT_NEUTRAL},
	    {"SMT_HOSTILE", SMT_HOSTILE},
	    {"SMT_AT_WAR", SMT_AT_WAR},
	    {"SMT_NIGHT_ATTACK", SMT_NIGHT_ATTACK},
	    {"SMT_TRADE_RIGHTS", SMT_TRADE_RIGHTS},
	    {"SMT_SIEGE", SMT_SIEGE},
	    {"SMT_ASSAULT", SMT_ASSAULT},
	    {"SMT_END_SIEGE", SMT_END_SIEGE},
	    {"SMT_AGE", SMT_AGE},
	    {"SMT_LOYALTY", SMT_LOYALTY},
	    {"SMT_GOVERNOR", SMT_GOVERNOR},
	    {"SMT_GARRISON", SMT_GARRISON},
	    {"SMT_RECRUITMENT_COST", SMT_RECRUITMENT_COST},
	    {"SMT_RECRUITMENT_TIME", SMT_RECRUITMENT_TIME},
	    {"SMT_RETRAIN_TIME", SMT_RETRAIN_TIME},
	    {"SMT_RETRAIN_COST", SMT_RETRAIN_COST},
	    {"SMT_FIGHT_BATTLE", SMT_FIGHT_BATTLE},
	    {"SMT_WITHDRAW", SMT_WITHDRAW},
	    {"SMT_AUTORESOLVE", SMT_AUTORESOLVE},
	    {"SMT_DISBAND", SMT_DISBAND},
	    {"SMT_DOCTOR_DESCR", SMT_DOCTOR_DESCR},
	    {"SMT_QUARTERMASTER_DESCR", SMT_QUARTERMASTER_DESCR},
	    {"SMT_PRIEST_DESCR", SMT_PRIEST_DESCR},
	    {"SMT_SCOUT_DESCR", SMT_SCOUT_DESCR},
	    {"SMT_ENGINEER_DESCR", SMT_ENGINEER_DESCR},
	    {"SMT_SPY_DESCR", SMT_SPY_DESCR},
	    {"SMT_ASSASSIN_DESCR", SMT_ASSASSIN_DESCR},
	    {"SMT_DIPLOMAT_DESCR", SMT_DIPLOMAT_DESCR},
	    {"SMT_ADMIRAL_DESCR", SMT_ADMIRAL_DESCR},
	    {"SMT_ARMY_UPKEEP", SMT_ARMY_UPKEEP},
	    {"SMT_LOYALTY_REVOLTING", SMT_LOYALTY_REVOLTING},
	    {"SMT_LOYALTY_RIOTING", SMT_LOYALTY_RIOTING},
	    {"SMT_LOYALTY_DISILLUSIONED", SMT_LOYALTY_DISILLUSIONED},
	    {"SMT_LOYALTY_CONTENT", SMT_LOYALTY_CONTENT},
	    {"SMT_LOYALTY_HAPPY", SMT_LOYALTY_HAPPY},
	    {"SMT_FORTIFICATION_NONE", SMT_FORTIFICATION_NONE},
	    {"SMT_FORTIFICATION_0", SMT_FORTIFICATION_0},
	    {"SMT_FORTIFICATION_1", SMT_FORTIFICATION_1},
	    {"SMT_FORTIFICATION_2", SMT_FORTIFICATION_2},
	    {"SMT_FORTIFICATION_3", SMT_FORTIFICATION_3},
	    {"SMT_FORTIFICATION_4", SMT_FORTIFICATION_4},
	    {"SMT_MISSION_INFILTRATE", SMT_MISSION_INFILTRATE},
	    {"SMT_MISSION_DIPLOMACY", SMT_MISSION_DIPLOMACY},
	    {"SMT_MISSION_BRIBE", SMT_MISSION_BRIBE},
	    {"SMT_MISSION_SPYING", SMT_MISSION_SPYING},
	    {"SMT_MISSION_ASSASSINATE", SMT_MISSION_ASSASSINATE},
	    {"SMT_MISSION_SABOTAGE", SMT_MISSION_SABOTAGE},
	    {"SMT_NAME", SMT_NAME},
	    {"SMT_FORT", SMT_FORT},
	    {"SMT_FACTION", SMT_FACTION},
	    {"SMT_PORT", SMT_PORT},
	    {"SMT_POPULATION", SMT_POPULATION},
	    {"SMT_FATIGUE", SMT_FATIGUE},
	    {"SMT_EXPERIENCE", SMT_EXPERIENCE},
	    {"SMT_MILD", SMT_MILD},
	    {"SMT_MASSIVE", SMT_MASSIVE},
	    {"SMT_CATASTROPHIC", SMT_CATASTROPHIC},
	    {"SMT_SMALL", SMT_SMALL},
	    {"SMT_MEDIUM", SMT_MEDIUM},
	    {"SMT_LARGE", SMT_LARGE},
	    {"SMT_HUGE", SMT_HUGE},
	    {"SMT_CURRENTLY_BUILDING", SMT_CURRENTLY_BUILDING},
	    {"SMT_HEALTHY", SMT_HEALTHY},
	    {"SMT_TIRED", SMT_TIRED},
	    {"SMT_ILL", SMT_ILL},
	    {"SMT_DYING", SMT_DYING},
	    {"SMT_ARMY_DETAILS", SMT_ARMY_DETAILS},
	    {"SMT_ENEMY_ARMY_DETAILS", SMT_ENEMY_ARMY_DETAILS},
	    {"SMT_ALLIED_ARMY_DETAILS", SMT_ALLIED_ARMY_DETAILS},
	    {"SMT_NEUTRAL_ARMY_DETAILS", SMT_NEUTRAL_ARMY_DETAILS},
	    {"SMT_NAVY_DETAILS", SMT_NAVY_DETAILS},
	    {"SMT_ENEMY_NAVY_DETAILS", SMT_ENEMY_NAVY_DETAILS},
	    {"SMT_ALLIED_NAVY_DETAILS", SMT_ALLIED_NAVY_DETAILS},
	    {"SMT_NEUTRAL_NAVY_DETAILS", SMT_NEUTRAL_NAVY_DETAILS},
	    {"SMT_SETTLEMENT_DETAILS", SMT_SETTLEMENT_DETAILS},
	    {"SMT_ENEMY_SETTLEMENT_DETAILS", SMT_ENEMY_SETTLEMENT_DETAILS},
	    {"SMT_NEUTRAL_SETTLEMENT_DETAILS", SMT_NEUTRAL_SETTLEMENT_DETAILS},
	    {"SMT_ALLIED_SETTLEMENT_DETAILS", SMT_ALLIED_SETTLEMENT_DETAILS},
	    {"SMT_FORT_DETAILS", SMT_FORT_DETAILS},
	    {"SMT_ENEMY_FORT_DETAILS", SMT_ENEMY_FORT_DETAILS},
	    {"SMT_ALLIED_FORT_DETAILS", SMT_ALLIED_FORT_DETAILS},
	    {"SMT_NEUTRAL_FORT_DETAILS", SMT_NEUTRAL_FORT_DETAILS},
	    {"SMT_SET_HEIR", SMT_SET_HEIR},
	    {"SMT_PREV_CITY", SMT_PREV_CITY},
	    {"SMT_NEXT_CITY", SMT_NEXT_CITY},
	    {"SMT_NEXT_CHARACTER", SMT_NEXT_CHARACTER},
	    {"SMT_PREV_CHARACTER", SMT_PREV_CHARACTER},
	    {"SMT_NEXT_UNIT", SMT_NEXT_UNIT},
	    {"SMT_PREV_UNIT", SMT_PREV_UNIT},
	    {"SMT_PREV_FORT", SMT_PREV_FORT},
	    {"SMT_NEXT_FORT", SMT_NEXT_FORT},
	    {"SMT_FOOD_SUPPLIES", SMT_FOOD_SUPPLIES},
	    {"SMT_STATE_REVOLTING", SMT_STATE_REVOLTING},
	    {"SMT_STATE_NOT_REVOLTING", SMT_STATE_NOT_REVOLTING},
	    {"SMT_STATE_UNDER_SIEGE", SMT_STATE_UNDER_SIEGE},
	    {"SMT_STATE_NOT_UNDER_SIEGE", SMT_STATE_NOT_UNDER_SIEGE},
	    {"SMT_STATE_HOLDING_GAMES", SMT_STATE_HOLDING_GAMES},
	    {"SMT_STATE_NOT_HOLDING_GAMES", SMT_STATE_NOT_HOLDING_GAMES},
	    {"SMT_STATE_PLAGUE", SMT_STATE_PLAGUE},
	    {"SMT_STATE_NOT_PLAGUED", SMT_STATE_NOT_PLAGUED},
	    {"SMT_FERTILE_LOW", SMT_FERTILE_LOW},
	    {"SMT_FERTILE_MEDIUM", SMT_FERTILE_MEDIUM},
	    {"SMT_FERTILE_HIGH", SMT_FERTILE_HIGH},
	    {"SMT_WILDERNESS", SMT_WILDERNESS},
	    {"SMT_MOUNTAINS_HIGH", SMT_MOUNTAINS_HIGH},
	    {"SMT_MOUNTAINS_LOW", SMT_MOUNTAINS_LOW},
	    {"SMT_HILLS", SMT_HILLS},
	    {"SMT_FOREST_DENSE", SMT_FOREST_DENSE},
	    {"SMT_FOREST_SPARSE", SMT_FOREST_SPARSE},
	    {"SMT_SWAMP", SMT_SWAMP},
	    {"SMT_OCEAN", SMT_OCEAN},
	    {"SMT_SEA_DEEP", SMT_SEA_DEEP},
	    {"SMT_SEA_SHALLOW", SMT_SEA_SHALLOW},
	    {"SMT_BEACH", SMT_BEACH},
	    {"SMT_IMPASSABLE_LAND", SMT_IMPASSABLE_LAND},
	    {"SMT_IMPASSABLE_SEA", SMT_IMPASSABLE_SEA},
	    {"SMT_FAMINE_THREAT_OK", SMT_FAMINE_THREAT_OK},
	    {"SMT_FAMINE_THREAT_HUNGRY", SMT_FAMINE_THREAT_HUNGRY},
	    {"SMT_FAMINE_THREAT_FAMINE_1", SMT_FAMINE_THREAT_FAMINE_1},
	    {"SMT_FAMINE_THREAT_FAMINE_2", SMT_FAMINE_THREAT_FAMINE_2},
	    {"SMT_FAMINE_THREAT_FAMINE_3", SMT_FAMINE_THREAT_FAMINE_3},
	    {"SMT_FAMINE_THREAT", SMT_FAMINE_THREAT},
	    {"SMT_HARVEST_LEVEL", SMT_HARVEST_LEVEL},
	    {"SMT_FARMING_LEVEL", SMT_FARMING_LEVEL},
	    {"SMT_TAX_LEVEL_LOW", SMT_TAX_LEVEL_LOW},
	    {"SMT_TAX_LEVEL_NORMAL", SMT_TAX_LEVEL_NORMAL},
	    {"SMT_TAX_LEVEL_HIGH", SMT_TAX_LEVEL_HIGH},
	    {"SMT_TAX_LEVEL_VERY_HIGH", SMT_TAX_LEVEL_VERY_HIGH},
	    {"SMT_GOVERNOR_POLICY_BALANCED", SMT_GOVERNOR_POLICY_BALANCED},
	    {"SMT_GOVERNOR_POLICY_FINANCIAL", SMT_GOVERNOR_POLICY_FINANCIAL},
	    {"SMT_GOVERNOR_POLICY_MILITARY", SMT_GOVERNOR_POLICY_MILITARY},
	    {"SMT_GOVERNOR_POLICY_STRONGHOLD", SMT_GOVERNOR_POLICY_STRONGHOLD},
	    {"SMT_GOVERNOR_POLICY_CULTURAL", SMT_GOVERNOR_POLICY_CULTURAL},
	    {"SMT_NO_GOVERNOR", SMT_NO_GOVERNOR},
	    {"SMT_NO_GOVERNOR_DESCR", SMT_NO_GOVERNOR_DESCR},
	    {"SMT_SHOW_GARRISON", SMT_SHOW_GARRISON},
	    {"SMT_SHOW_UNITS", SMT_SHOW_UNITS},
	    {"SMT_SHOW_UNITS_NAVY", SMT_SHOW_UNITS_NAVY},
	    {"SMT_SHOW_BUILDINGS", SMT_SHOW_BUILDINGS},
	    {"SMT_SHOW_VISITORS", SMT_SHOW_VISITORS},
	    {"SMT_SHOW_TRAVELLING_CHARACTERS", SMT_SHOW_TRAVELLING_CHARACTERS},
	    {"SMT_SHOW_TRAVELLING_CHARACTERS_NAVY", SMT_SHOW_TRAVELLING_CHARACTERS_NAVY},
	    {"SMT_SHOW_PASSENGERS", SMT_SHOW_PASSENGERS},
	    {"SMT_RCLICK_SHOW_SETTLEMENTS", SMT_RCLICK_SHOW_SETTLEMENTS},
	    {"SMT_RCLICK_SHOW_CHARACTERS", SMT_RCLICK_SHOW_CHARACTERS},
	    {"SMT_RCLICK_SHOW_ARMIES", SMT_RCLICK_SHOW_ARMIES},
	    {"SMT_LOYALTY_DECREASING", SMT_LOYALTY_DECREASING},
	    {"SMT_LOYALTY_INCREASING", SMT_LOYALTY_INCREASING},
	    {"SMT_REMOVE_RALLY_POINT", SMT_REMOVE_RALLY_POINT},
	    {"SMT_SET_RALLY_POINT", SMT_SET_RALLY_POINT},
	    {"SMT_1_TURN", SMT_1_TURN},
	    {"SMT_X_TURNS", SMT_X_TURNS},
	    {"SMT_SELECT_UNIT_TO_RECRUIT", SMT_SELECT_UNIT_TO_RECRUIT},
	    {"SMT_SELECT_UNIT_TO_RETRAIN", SMT_SELECT_UNIT_TO_RETRAIN},
	    {"SMT_SET_TO_RECRUIT_MODE", SMT_SET_TO_RECRUIT_MODE},
	    {"SMT_SET_TO_RETRAIN_MODE", SMT_SET_TO_RETRAIN_MODE},
	    {"SMT_MERGE_SELECTED_UNITS", SMT_MERGE_SELECTED_UNITS},
	    {"SMT_X_PERCENT_COMPLETE", SMT_X_PERCENT_COMPLETE},
	    {"SMT_DIPLOMACY_OFFER_ALLIANCE", SMT_DIPLOMACY_OFFER_ALLIANCE},
	    {"SMT_DIPLOMACY_OFFER_CANCEL_ALLIANCE", SMT_DIPLOMACY_OFFER_CANCEL_ALLIANCE},
	    {"SMT_DIPLOMACY_OFFER_CEASEFIRE", SMT_DIPLOMACY_OFFER_CEASEFIRE},
	    {"SMT_DIPLOMACY_OFFER_MILITARY_ACCESS", SMT_DIPLOMACY_OFFER_MILITARY_ACCESS},
	    {"SMT_DIPLOMACY_OFFER_CANCEL_MILITARY_ACCESS", SMT_DIPLOMACY_OFFER_CANCEL_MILITARY_ACCESS},
	    {"SMT_DIPLOMACY_OFFER_TRADE_RIGHTS", SMT_DIPLOMACY_OFFER_TRADE_RIGHTS},
	    {"SMT_DIPLOMACY_OFFER_CANCEL_TRADE_RIGHTS", SMT_DIPLOMACY_OFFER_CANCEL_TRADE_RIGHTS},
	    {"SMT_DIPLOMACY_OFFER_ATTACK_FACTION", SMT_DIPLOMACY_OFFER_ATTACK_FACTION},
	    {"SMT_DIPLOMACY_OFFER_CEDE_REGION", SMT_DIPLOMACY_OFFER_CEDE_REGION},
	    {"SMT_DIPLOMACY_OFFER_SINGLE_PAYMENT", SMT_DIPLOMACY_OFFER_SINGLE_PAYMENT},
	    {"SMT_DIPLOMACY_OFFER_YEARLY_PAYMENT", SMT_DIPLOMACY_OFFER_YEARLY_PAYMENT},
	    {"SMT_DIPLOMACY_OFFER_CANCEL_YEARLY_PAYMENT", SMT_DIPLOMACY_OFFER_CANCEL_YEARLY_PAYMENT},
	    {"SMT_DIPLOMACY_OFFER_THREAT_OF_ATTACK", SMT_DIPLOMACY_OFFER_THREAT_OF_ATTACK},
	    {"SMT_DIPLOMACY_OFFER_DEMAND_NO_ATTACK", SMT_DIPLOMACY_OFFER_DEMAND_NO_ATTACK},
	    {"SMT_DIPLOMACY_OFFER_MAP_INFORMATION", SMT_DIPLOMACY_OFFER_MAP_INFORMATION},
	    {"SMT_DIPLOMACY_OFFER_BRIBE", SMT_DIPLOMACY_OFFER_BRIBE},
	    {"SMT_DIPLOMACY_OFFER_PROTECTORATE", SMT_DIPLOMACY_OFFER_PROTECTORATE},
	    {"SMT_MAKE_DECLARATION", SMT_MAKE_DECLARATION},
	    {"SMT_MAKE_OFFER", SMT_MAKE_OFFER},
	    {"SMT_MAKE_DEMAND", SMT_MAKE_DEMAND},
	    {"SMT_YOUR_DECLARATIONS", SMT_YOUR_DECLARATIONS},
	    {"SMT_YOUR_DEMANDS", SMT_YOUR_DEMANDS},
	    {"SMT_YOUR_OFFERS", SMT_YOUR_OFFERS},
	    {"SMT_THEIR_DECLARATIONS", SMT_THEIR_DECLARATIONS},
	    {"SMT_THEIR_OFFERS", SMT_THEIR_OFFERS},
	    {"SMT_THEIR_DEMANDS", SMT_THEIR_DEMANDS},
	    {"SMT_GIVE_X_DENARI", SMT_GIVE_X_DENARI},
	    {"SMT_WE_WILL_GIVE_X_DENARI", SMT_WE_WILL_GIVE_X_DENARI},
	    {"SMT_GIVE_YEARLY_TRIBUTE_OF_X_DENARI", SMT_GIVE_YEARLY_TRIBUTE_OF_X_DENARI},
	    {"SMT_OFFER_YEARLY_TRIBUTE_OF_X_DENARI", SMT_OFFER_YEARLY_TRIBUTE_OF_X_DENARI},
	    {"SMT_CANCEL_YEARLY_TRIBUTE_OF_X_DENARI", SMT_CANCEL_YEARLY_TRIBUTE_OF_X_DENARI},
	    {"SMT_DIP_ACCEPT_OFFER", SMT_DIP_ACCEPT_OFFER},
	    {"SMT_DIP_DECLINE_OFFER", SMT_DIP_DECLINE_OFFER},
	    {"SMT_DIP_COUNTER_OFFER", SMT_DIP_COUNTER_OFFER},
	    {"SMT_DIP_NEW_OFFER", SMT_DIP_NEW_OFFER},
	    {"SMT_DIP_INIT_OFFER", SMT_DIP_INIT_OFFER},
	    {"SMT_DIP_MAKE_GIFT", SMT_DIP_MAKE_GIFT},
	    {"SMT_DIP_INFORM", SMT_DIP_INFORM},
	    {"SMT_ABILITIES_AT_A_GLANCE", SMT_ABILITIES_AT_A_GLANCE},
	    {"SMT_DESCRIPTION", SMT_DESCRIPTION},
	    {"SMT_UPGRADES", SMT_UPGRADES},
	    {"SMT_X_MEN", SMT_X_MEN},
	    {"SMT_YOUR_ARMY_HAS_X_BUILD_POINTS", SMT_YOUR_ARMY_HAS_X_BUILD_POINTS},
	    {"SMT_SIEGE_ACTIONS", SMT_SIEGE_ACTIONS},
	    {"SMT_BUILT", SMT_BUILT},
	    {"SMT_ZOOM_TO_SETTLEMENT", SMT_ZOOM_TO_SETTLEMENT},
	    {"SMT_ZOOM_TO_UNIT", SMT_ZOOM_TO_UNIT},
	    {"SMT_ZOOM_TO_CHARACTER", SMT_ZOOM_TO_CHARACTER},
	    {"SMT_OPEN_CONSTRUCTION_WINDOW", SMT_OPEN_CONSTRUCTION_WINDOW},
	    {"SMT_NOTHING_TO_CONSTRUCT", SMT_NOTHING_TO_CONSTRUCT},
	    {"SMT_OPEN_TRAINING_WINDOW", SMT_OPEN_TRAINING_WINDOW},
	    {"SMT_NOTHING_TO_TRAIN", SMT_NOTHING_TO_TRAIN},
	    {"SMT_CURRENTLY_TRAINING", SMT_CURRENTLY_TRAINING},
	    {"SMT_OPEN_OPTIONS_WINDOW", SMT_OPEN_OPTIONS_WINDOW},
	    {"SMT_OPEN_FINANCE_WINDOW", SMT_OPEN_FINANCE_WINDOW},
	    {"SMT_OPEN_DIPLOMACY_WINDOW", SMT_OPEN_DIPLOMACY_WINDOW},
	    {"SMT_FACTION_BUTTON_TOOLTIP", SMT_FACTION_BUTTON_TOOLTIP},
	    {"SMT_SELECTED_ITEM_TOOLTIP", SMT_SELECTED_ITEM_TOOLTIP},
	    {"SMT_BUILD_QUEUE_CONFLICT", SMT_BUILD_QUEUE_CONFLICT},
	    {"SMT_BUILD_QUEUE_FULL", SMT_BUILD_QUEUE_FULL},
	    {"SMT_BUILD_QUEUE_INSUFFICIENT_FUNDS", SMT_BUILD_QUEUE_INSUFFICIENT_FUNDS},
	    {"SMT_TRAINING_INSUFFICIENT_POPULACE", SMT_TRAINING_INSUFFICIENT_POPULACE},
	    {"SMT_TRAINING_INSUFFICIENT_FUNDS", SMT_TRAINING_INSUFFICIENT_FUNDS},
	    {"SMT_MAX_UNIT_POOL", SMT_MAX_UNIT_POOL},
	    {"SMT_TRAINING_QUEUE_FULL", SMT_TRAINING_QUEUE_FULL},
	    {"SMT_WE_HAVE_MILITARY_ACCESS", SMT_WE_HAVE_MILITARY_ACCESS},
	    {"SMT_THEY_HAVE_MILITARY_ACCESS", SMT_THEY_HAVE_MILITARY_ACCESS},
	    {"SMT_PAYING_YOU_TRIBUTE_X_PER_TURN", SMT_PAYING_YOU_TRIBUTE_X_PER_TURN},
	    {"SMT_PAYING_THEM_TRIBUTE_X_PER_TURN", SMT_PAYING_THEM_TRIBUTE_X_PER_TURN},
	    {"SMT_ASSISTANCE_REQUESTED_AGAINST", SMT_ASSISTANCE_REQUESTED_AGAINST},
	    {"SMT_ASSISTANCE_PROVIDED_AGAINST", SMT_ASSISTANCE_PROVIDED_AGAINST},
	    {"SMT_CONFIRM_OFFER", SMT_CONFIRM_OFFER},
	    {"SMT_CANCEL_OFFER", SMT_CANCEL_OFFER},
	    {"SMT_OFFER_MONEY_TITLE", SMT_OFFER_MONEY_TITLE},
	    {"SMT_DEMAND_MONEY_TITLE", SMT_DEMAND_MONEY_TITLE},
	    {"SMT_AMOUNT_OF_DENARI", SMT_AMOUNT_OF_DENARI},
	    {"SMT_OFFER_TRIBUTE_TITLE", SMT_OFFER_TRIBUTE_TITLE},
	    {"SMT_DEMAND_TRIBUTE", SMT_DEMAND_TRIBUTE},
	    {"SMT_NUMBER_OF_YEARS", SMT_NUMBER_OF_YEARS},
	    {"SMT_OR_UNTIL_FURTHER_NOTICE", SMT_OR_UNTIL_FURTHER_NOTICE},
	    {"SMT_UNTIL_FURTHER_NOTICE", SMT_UNTIL_FURTHER_NOTICE},
	    {"SMT_DEMAND_ATTACK_ON_FACTION", SMT_DEMAND_ATTACK_ON_FACTION},
	    {"SMT_OFFER_ATTACK_ON_FACTION", SMT_OFFER_ATTACK_ON_FACTION},
	    {"SMT_WITHIN", SMT_WITHIN},
	    {"SMT_YEARS", SMT_YEARS},
	    {"SMT_GIVE", SMT_GIVE},
	    {"SMT_DEMAND_SETTLEMENT_TITLE", SMT_DEMAND_SETTLEMENT_TITLE},
	    {"SMT_OFFER_SETTLEMENT_TITLE", SMT_OFFER_SETTLEMENT_TITLE},
	    {"SMT_CITY", SMT_CITY},
	    {"SMT_CHARACTERS", SMT_CHARACTERS},
	    {"SMT_STATUS", SMT_STATUS},
	    {"SMT_CHARACTER_PERFORMED_TURNS_ACTION", SMT_CHARACTER_PERFORMED_TURNS_ACTION},
	    {"SMT_TARGET_TILE_IS_IMPASSABLE", SMT_TARGET_TILE_IS_IMPASSABLE},
	    {"SMT_CANNOT_LEAVE_SIEGED_FORT", SMT_CANNOT_LEAVE_SIEGED_FORT},
	    {"SMT_CANNOT_LEAVE_SIEGED_SETTLEMENT", SMT_CANNOT_LEAVE_SIEGED_SETTLEMENT},
	    {"SMT_CANNOT_LEAVE_ENEMIES_ZOC", SMT_CANNOT_LEAVE_ENEMIES_ZOC},
	    {"SMT_CHARACTER_DETAILS", SMT_CHARACTER_DETAILS},
	    {"SMT_ENEMY_CHARACTER_DETAILS", SMT_ENEMY_CHARACTER_DETAILS},
	    {"SMT_NEUTRAL_CHARACTER_DETAILS", SMT_NEUTRAL_CHARACTER_DETAILS},
	    {"SMT_ALLIED_CHARACTER_DETAILS", SMT_ALLIED_CHARACTER_DETAILS},
	    {"SMT_VICES", SMT_VICES},
	    {"SMT_FACTION_SUMMARY", SMT_FACTION_SUMMARY},
	    {"SMT_FACTION_LEADER", SMT_FACTION_LEADER},
	    {"SMT_HEIRS", SMT_HEIRS},
	    {"SMT_REGIONS_GAINED", SMT_REGIONS_GAINED},
	    {"SMT_BATTLES_WON", SMT_BATTLES_WON},
	    {"SMT_BATTLES_LOST", SMT_BATTLES_LOST},
	    {"SMT_GREATEST_GENERAL", SMT_GREATEST_GENERAL},
	    {"SMT_GENERALS", SMT_GENERALS},
	    {"SMT_CITIES", SMT_CITIES},
	    {"SMT_CASTLES", SMT_CASTLES},
	    {"SMT_YEAR", SMT_YEAR},
	    {"SMT_TURN_NUMBER", SMT_TURN_NUMBER},
	    {"SMT_TURNS_REMAINING", SMT_TURNS_REMAINING},
	    {"SMT_SHOW_FAMILY_TREE", SMT_SHOW_FAMILY_TREE},
	    {"SMT_SHOW_EVENT_LOG", SMT_SHOW_EVENT_LOG},
	    {"SMT_FINANCIAL_SUMMARY", SMT_FINANCIAL_SUMMARY},
	    {"SMT_INCOME", SMT_INCOME},
	    {"SMT_EXPENDITURE", SMT_EXPENDITURE},
	    {"SMT_FARMING", SMT_FARMING},
	    {"SMT_RESOURCES", SMT_RESOURCES},
	    {"SMT_TRADE", SMT_TRADE},
	    {"SMT_MERCHANT_TRADE", SMT_MERCHANT_TRADE},
	    {"SMT_TAXES", SMT_TAXES},
	    {"SMT_WAGES", SMT_WAGES},
	    {"SMT_PROJECTED_INCOME", SMT_PROJECTED_INCOME},
	    {"SMT_PROJECTED_EXPENDITURE", SMT_PROJECTED_EXPENDITURE},
	    {"SMT_PROJECTED_PROFIT", SMT_PROJECTED_PROFIT},
	    {"SMT_TOTAL_INCOME", SMT_TOTAL_INCOME},
	    {"SMT_TOTAL_EXPENDITURE", SMT_TOTAL_EXPENDITURE},
	    {"SMT_TOTAL_PROFIT", SMT_TOTAL_PROFIT},
	    {"SMT_LEVEL", SMT_LEVEL},
	    {"SMT_EOT_FINANCIAL_SUMMARY", SMT_EOT_FINANCIAL_SUMMARY},
	    {"SMT_EOT_MILITARY_SUMMARY", SMT_EOT_MILITARY_SUMMARY},
	    {"SMT_EOT_BALANCE_OF_POWER", SMT_EOT_BALANCE_OF_POWER},
	    {"SMT_EOT_BATTLES_FOUGHT", SMT_EOT_BATTLES_FOUGHT},
	    {"SMT_EOT_BATTLES_WON", SMT_EOT_BATTLES_WON},
	    {"SMT_EOT_TROOPS_RECRUITED", SMT_EOT_TROOPS_RECRUITED},
	    {"SMT_EOT_TROOPS_LOST", SMT_EOT_TROOPS_LOST},
	    {"SMT_EOT_REGIONS_CAPTURED", SMT_EOT_REGIONS_CAPTURED},
	    {"SMT_EOT_REGIONS_LOST", SMT_EOT_REGIONS_LOST},
	    {"SMT_EOT_YOUR_RATING", SMT_EOT_YOUR_RATING},
	    {"SMT_EOT_CURRENT_LEADER", SMT_EOT_CURRENT_LEADER},
	    {"SMT_EOT_MILITARY", SMT_EOT_MILITARY},
	    {"SMT_EOT_FINANCIAL", SMT_EOT_FINANCIAL},
	    {"SMT_EOT_PRODUCTION", SMT_EOT_PRODUCTION},
	    {"SMT_EOT_POPULATION", SMT_EOT_POPULATION},
	    {"SMT_EOT_OVERAL", SMT_EOT_OVERAL},
	    {"SMT_EOT_PERCENTAGE", SMT_EOT_PERCENTAGE},
	    {"SMT_ARMY_RECRUIT", SMT_ARMY_RECRUIT},
	    {"SMT_CONSTRUCTION", SMT_CONSTRUCTION},
	    {"SMT_DIPLOMACY", SMT_DIPLOMACY},
	    {"SMT_TRIBUTE", SMT_TRIBUTE},
	    {"SMT_OTHER", SMT_OTHER},
	    {"SMT_EVENT_LOG", SMT_EVENT_LOG},
	    {"SMT_CHARACTER", SMT_CHARACTER},
	    {"SMT_TIME_REMAINING", SMT_TIME_REMAINING},
	    {"SMT_TARGETS", SMT_TARGETS},
	    {"SMT_TARGET", SMT_TARGET},
	    {"SMT_SETTLEMENT", SMT_SETTLEMENT},
	    {"SMT_TRADE_DETAILS", SMT_TRADE_DETAILS},
	    {"SMT_SHOW_TRADE_SUMMARY", SMT_SHOW_TRADE_SUMMARY},
	    {"SMT_RESOURCE_GENERIC", SMT_RESOURCE_GENERIC},
	    {"SMT_RESOURCE_GOLD", SMT_RESOURCE_GOLD},
	    {"SMT_RESOURCE_SILVER", SMT_RESOURCE_SILVER},
	    {"SMT_RESOURCE_IRON", SMT_RESOURCE_IRON},
	    {"SMT_RESOURCE_FISH", SMT_RESOURCE_FISH},
	    {"SMT_RESOURCE_FURS", SMT_RESOURCE_FURS},
	    {"SMT_RESOURCE_GRAIN", SMT_RESOURCE_GRAIN},
	    {"SMT_RESOURCE_TIMBER", SMT_RESOURCE_TIMBER},
	    {"SMT_RESOURCE_ELEPHANT", SMT_RESOURCE_ELEPHANT},
	    {"SMT_RESOURCE_CAMEL", SMT_RESOURCE_CAMEL},
	    {"SMT_RESOURCE_DOG", SMT_RESOURCE_DOG},
	    {"SMT_RESOURCE_COAL", SMT_RESOURCE_COAL},
	    {"SMT_RESOURCE_WOOL", SMT_RESOURCE_WOOL},
	    {"SMT_RESOURCE_IVORY", SMT_RESOURCE_IVORY},
	    {"SMT_RESOURCE_WINE", SMT_RESOURCE_WINE},
	    {"SMT_RESOURCE_SLAVES", SMT_RESOURCE_SLAVES},
	    {"SMT_RESOURCE_CHOCOLATE", SMT_RESOURCE_CHOCOLATE},
	    {"SMT_RESOURCE_MARBLE", SMT_RESOURCE_MARBLE},
	    {"SMT_RESOURCE_TEXTILES", SMT_RESOURCE_TEXTILES},
	    {"SMT_RESOURCE_DYES", SMT_RESOURCE_DYES},
	    {"SMT_RESOURCE_TOBACCO", SMT_RESOURCE_TOBACCO},
	    {"SMT_RESOURCE_SILK", SMT_RESOURCE_SILK},
	    {"SMT_RESOURCE_SUGAR", SMT_RESOURCE_SUGAR},
	    {"SMT_RESOURCE_SULFUR", SMT_RESOURCE_SULFUR},
	    {"SMT_RESOURCE_TIN", SMT_RESOURCE_TIN},
	    {"SMT_RESOURCE_SPICES", SMT_RESOURCE_SPICES},
	    {"SMT_RESOURCE_COTTON", SMT_RESOURCE_COTTON},
	    {"SMT_RESOURCE_AMBER", SMT_RESOURCE_AMBER},
	    {"SMT_HARVEST_POOR", SMT_HARVEST_POOR},
	    {"SMT_HARVEST_NORMAL", SMT_HARVEST_NORMAL},
	    {"SMT_HARVEST_GOOD", SMT_HARVEST_GOOD},
	    {"SMT_HARVEST_EXCELLENT", SMT_HARVEST_EXCELLENT},
	    {"SMT_MINING_INCOME", SMT_MINING_INCOME},
	    {"SMT_FARM_INCOME", SMT_FARM_INCOME},
	    {"SMT_FIND_SELECTED_CHARACTER", SMT_FIND_SELECTED_CHARACTER},
	    {"SMT_FAMILY_TREE_TITLE", SMT_FAMILY_TREE_TITLE},
	    {"SMT_OWNED_BY", SMT_OWNED_BY},
	    {"SMT_WEAPON_ARMOUR_PIERCING", SMT_WEAPON_ARMOUR_PIERCING},
	    {"SMT_WEAPON_BODY_PIERCING", SMT_WEAPON_BODY_PIERCING},
	    {"SMT_WEAPON_PRECURSOR", SMT_WEAPON_PRECURSOR},
	    {"SMT_WEAPON_AREA", SMT_WEAPON_AREA},
	    {"SMT_UNIT_IS_GENERAL", SMT_UNIT_IS_GENERAL},
	    {"SMT_UNIT_IS_COMMAND", SMT_UNIT_IS_COMMAND},
	    {"SMT_UNIT_CAN_CONCEAL", SMT_UNIT_CAN_CONCEAL},
	    {"SMT_UNIT_IS_HARDY", SMT_UNIT_IS_HARDY},
	    {"SMT_UNIT_IS_VERY_HARDY", SMT_UNIT_IS_VERY_HARDY},
	    {"SMT_UNIT_IS_HEAVY", SMT_UNIT_IS_HEAVY},
	    {"SMT_CAN_DO_TESTUDO", SMT_CAN_DO_TESTUDO},
	    {"SMT_CAN_DO_PHALANX", SMT_CAN_DO_PHALANX},
	    {"SMT_GOOD_ON_SCRUB", SMT_GOOD_ON_SCRUB},
	    {"SMT_GOOD_ON_SAND", SMT_GOOD_ON_SAND},
	    {"SMT_GOOD_ON_FOREST", SMT_GOOD_ON_FOREST},
	    {"SMT_GOOD_ON_SNOW", SMT_GOOD_ON_SNOW},
	    {"SMT_BAD_ON_SCRUB", SMT_BAD_ON_SCRUB},
	    {"SMT_BAD_ON_SAND", SMT_BAD_ON_SAND},
	    {"SMT_BAD_ON_FOREST", SMT_BAD_ON_FOREST},
	    {"SMT_BAD_ON_SNOW", SMT_BAD_ON_SNOW},
	    {"SMT_UNCOVERED_HIDDEN_AGENT_IN_SETTLEMENT_TITLE", SMT_UNCOVERED_HIDDEN_AGENT_IN_SETTLEMENT_TITLE},
	    {"SMT_UNCOVERED_HIDDEN_AGENT_IN_SETTLEMENT_DESCRIPTION", SMT_UNCOVERED_HIDDEN_AGENT_IN_SETTLEMENT_DESCRIPTION},
	    {"SMT_HIDDEN_AGENT_IN_SETTLEMENT_COMPROMISED_TITLE", SMT_HIDDEN_AGENT_IN_SETTLEMENT_COMPROMISED_TITLE},
	    {"SMT_HIDDEN_AGENT_IN_SETTLEMENT_COMPROMISED_DESCRIPTION", SMT_HIDDEN_AGENT_IN_SETTLEMENT_COMPROMISED_DESCRIPTION},
	    {"SMT_APPOINT_TO_GOVERNOR_TITLE", SMT_APPOINT_TO_GOVERNOR_TITLE},
	    {"SMT_APPOINT_TO_GOVERNOR_DESCRIPTION", SMT_APPOINT_TO_GOVERNOR_DESCRIPTION},
	    {"SMT_SETTLEMENT_BESIEGE_TITLE", SMT_SETTLEMENT_BESIEGE_TITLE},
	    {"SMT_SETTLEMENT_BESIEGE_DESCRIPTION", SMT_SETTLEMENT_BESIEGE_DESCRIPTION},
	    {"SMT_SETTLEMENT_REVOLT_TITLE", SMT_SETTLEMENT_REVOLT_TITLE},
	    {"SMT_SETTLEMENT_REVOLT_DESCRIPTION", SMT_SETTLEMENT_REVOLT_DESCRIPTION},
	    {"SMT_SETTLEMENT_LOST_TITLE", SMT_SETTLEMENT_LOST_TITLE},
	    {"SMT_SETTLEMENT_LOST_DESCRIPTION", SMT_SETTLEMENT_LOST_DESCRIPTION},
	    {"SMT_SETTLEMENT_GAINED_TITLE", SMT_SETTLEMENT_GAINED_TITLE},
	    {"SMT_SETTLEMENT_GAINED_DESCRIPTION", SMT_SETTLEMENT_GAINED_DESCRIPTION},
	    {"SMT_NAVY_BLOCKADED_TITLE", SMT_NAVY_BLOCKADED_TITLE},
	    {"SMT_NAVY_BLOCKADED_DESCRIPTION", SMT_NAVY_BLOCKADED_DESCRIPTION},
	    {"SMT_BIRTH_TITLE", SMT_BIRTH_TITLE},
	    {"SMT_BIRTH_DESCRIPTION", SMT_BIRTH_DESCRIPTION},
	    {"SMT_ADOPTION_TITLE", SMT_ADOPTION_TITLE},
	    {"SMT_ADOPTION_DESCRIPTION", SMT_ADOPTION_DESCRIPTION},
	    {"SMT_COMING_OF_AGE_TITLE", SMT_COMING_OF_AGE_TITLE},
	    {"SMT_COMING_OF_AGE_DESCRIPTION", SMT_COMING_OF_AGE_DESCRIPTION},
	    {"SMT_MARRIAGE_TITLE", SMT_MARRIAGE_TITLE},
	    {"SMT_MARRIAGE_DESCRIPTION", SMT_MARRIAGE_DESCRIPTION},
	    {"SMT_NATURAL_DEATH_TITLE", SMT_NATURAL_DEATH_TITLE},
	    {"SMT_NATURAL_DEATH_DESCRIPTION", SMT_NATURAL_DEATH_DESCRIPTION},
	    {"SMT_NEW_FACTION_LEADER_TITLE", SMT_NEW_FACTION_LEADER_TITLE},
	    {"SMT_NEW_FACTION_LEADER_DESCRIPTION", SMT_NEW_FACTION_LEADER_DESCRIPTION},
	    {"SMT_NEW_FACTION_HEIR_TITLE", SMT_NEW_FACTION_HEIR_TITLE},
	    {"SMT_NEW_FACTION_HEIR_DESCRIPTION", SMT_NEW_FACTION_HEIR_DESCRIPTION},
	    {"SMT_FACTION_DEFEATED_TITLE", SMT_FACTION_DEFEATED_TITLE},
	    {"SMT_FACTION_DEFEATED_DESCRIPTION", SMT_FACTION_DEFEATED_DESCRIPTION},
	    {"SMT_UNKNOWN", SMT_UNKNOWN},
	    {"SMT_UNKNOWN_UNIT", SMT_UNKNOWN_UNIT},
	    {"SMT_UNKNOWN_BUILDING", SMT_UNKNOWN_BUILDING},
	    {"SMT_ARMY_BREAKDOWN", SMT_ARMY_BREAKDOWN},
	    {"SMT_SHOW_ARMY_DETAILS", SMT_SHOW_ARMY_DETAILS},
	    {"SMT_SHOW_VNV_DETAILS", SMT_SHOW_VNV_DETAILS},
	    {"SMT_TRAITS_UKNOWN", SMT_TRAITS_UKNOWN},
	    {"SMT_STATUS_UNKNOWN", SMT_STATUS_UNKNOWN},
	    {"SMT_SPOTTED_BY", SMT_SPOTTED_BY},
	    {"SMT_WATCHTOWER", SMT_WATCHTOWER},
	    {"SMT_EXCHANGE_COMMANDING_OFFICER", SMT_EXCHANGE_COMMANDING_OFFICER},
	    {"SMT_EXCHANGE_NO_MOVEMENT_POINTS", SMT_EXCHANGE_NO_MOVEMENT_POINTS},
	    {"SMT_EXCHANGE_NO_NAVY_PRESENT", SMT_EXCHANGE_NO_NAVY_PRESENT},
	    {"SMT_EXCHANGE_SELECTED", SMT_EXCHANGE_SELECTED},
	    {"SMT_EXCHANGE_UNSELECTED", SMT_EXCHANGE_UNSELECTED},
	    {"SMT_EXCHANGE_NO_SELECTION", SMT_EXCHANGE_NO_SELECTION},
	    {"SMT_EXCHANGE_MERGE_OK", SMT_EXCHANGE_MERGE_OK},
	    {"SMT_EXCHANGE_CAPACITY_EXCEEDED", SMT_EXCHANGE_CAPACITY_EXCEEDED},
	    {"SMT_SHOW_MISSION_TARGET", SMT_SHOW_MISSION_TARGET},
	    {"SMT_RALLY_POINTS", SMT_RALLY_POINTS},
	    {"SMT_RALLY_POINT", SMT_RALLY_POINT},
	    {"SMT_CANNOT_MERGE_THIS_UNIT_SELECTION", SMT_CANNOT_MERGE_THIS_UNIT_SELECTION},
	    {"SMT_CANNOT_MERGE_THIS_CHARACTER_SELECTION", SMT_CANNOT_MERGE_THIS_CHARACTER_SELECTION},
	    {"SMT_ZOOM_DOWN_TO_CITY", SMT_ZOOM_DOWN_TO_CITY},
	    {"SMT_DESTROY_BUILDING", SMT_DESTROY_BUILDING},
	    {"SMT_RETAINERS", SMT_RETAINERS},
	    {"SMT_RETAINERS_UNKNOWN", SMT_RETAINERS_UNKNOWN},
	    {"SMT_SUCCESS_RATE", SMT_SUCCESS_RATE},
	    {"SMT_SHOW_CHARACTER_TARGETS", SMT_SHOW_CHARACTER_TARGETS},
	    {"SMT_SHOW_BUILDING_TARGETS", SMT_SHOW_BUILDING_TARGETS},
	    {"SMT_ACCEPT_MISSION", SMT_ACCEPT_MISSION},
	    {"SMT_BUILDING", SMT_BUILDING},
	    {"SMT_MILITARY_FORCES", SMT_MILITARY_FORCES},
	    {"SMT_NUM_SOLDIERS", SMT_NUM_SOLDIERS},
	    {"SMT_ATTACK", SMT_ATTACK},
	    {"SMT_ATTACK_CHARGE", SMT_ATTACK_CHARGE},
	    {"SMT_ARMOUR", SMT_ARMOUR},
	    {"SMT_DEFENCE", SMT_DEFENCE},
	    {"SMT_SHIELD", SMT_SHIELD},
	    {"SMT_HP", SMT_HP},
	    {"SMT_CREATION_COST", SMT_CREATION_COST},
	    {"SMT_ARMY_STRENGTH_RATIO", SMT_ARMY_STRENGTH_RATIO},
	    {"SMT_NAVY_STRENGTH_RATIO", SMT_NAVY_STRENGTH_RATIO},
	    {"SMT_PROTECTORATES", SMT_PROTECTORATES},
	    {"SMT_YOU_ARE_PROTECTOR", SMT_YOU_ARE_PROTECTOR},
	    {"SMT_ENABLE_AUTO_TRAIN", SMT_ENABLE_AUTO_TRAIN},
	    {"SMT_ENABLE_AUTO_CONSTRUCT", SMT_ENABLE_AUTO_CONSTRUCT},
	    {"SMT_AUTOMATE_ALL", SMT_AUTOMATE_ALL},
	    {"SMT_AI_SPEND_POLICY", SMT_AI_SPEND_POLICY},
	    {"SMT_SAVE", SMT_SAVE},
	    {"SMT_SPEND", SMT_SPEND},
	    {"SMT_CONSTRUCTION_OPTIONS", SMT_CONSTRUCTION_OPTIONS},
	    {"SMT_CONSTRUCTION_QUEUE", SMT_CONSTRUCTION_QUEUE},
	    {"SMT_LAND_EXPORTS", SMT_LAND_EXPORTS},
	    {"SMT_SEA_EXPORTS", SMT_SEA_EXPORTS},
	    {"SMT_IMPORTS", SMT_IMPORTS},
	    {"SMT_TRADE_INCOME", SMT_TRADE_INCOME},
	    {"SMT_RECRUITMENT_QUEUE", SMT_RECRUITMENT_QUEUE},
	    {"SMT_DEFENSES", SMT_DEFENSES},
	    {"SMT_SIEGE_DETAILS", SMT_SIEGE_DETAILS},
	    {"SMT_SETTLEMENT_HAS_FOOD_FOR_X_YEARS", SMT_SETTLEMENT_HAS_FOOD_FOR_X_YEARS},
	    {"SMT_FORT_HAS_FOOD_FOR_X_YEARS", SMT_FORT_HAS_FOOD_FOR_X_YEARS},
	    {"SMT_SETTLEMENT_HAS_FOOD_FOR_1_YEAR", SMT_SETTLEMENT_HAS_FOOD_FOR_1_YEAR},
	    {"SMT_FORT_HAS_FOOD_FOR_1_YEAR", SMT_FORT_HAS_FOOD_FOR_1_YEAR},
	    {"SMT_SETTLEMENT_HAS_NO_FOOD", SMT_SETTLEMENT_HAS_NO_FOOD},
	    {"SMT_FORT_HAS__NO_FOOD", SMT_FORT_HAS__NO_FOOD},
	    {"SMT_AUTOMANAGE_ALL_TOOLTIP", SMT_AUTOMANAGE_ALL_TOOLTIP},
	    {"SMT_ALLOW_BUILDING_TOOLTIP", SMT_ALLOW_BUILDING_TOOLTIP},
	    {"SMT_ALLOW_TROOP_PRODUCTION_TOOLTIP", SMT_ALLOW_TROOP_PRODUCTION_TOOLTIP},
	    {"SMT_SPEND_SLIDER_TOOLTIP", SMT_SPEND_SLIDER_TOOLTIP},
	    {"SMT_BUILDING_ADVICE", SMT_BUILDING_ADVICE},
	    {"SMT_RECRUITMENT_ADVICE", SMT_RECRUITMENT_ADVICE},
	    {"SMT_DAMAGE", SMT_DAMAGE},
	    {"SMT_INVALID_RALLY_POINT_POSITION", SMT_INVALID_RALLY_POINT_POSITION},
	    {"SMT_CANNOT_PERFORM_MISSIONS_WITH_MULTIPLE_CHARACTERS", SMT_CANNOT_PERFORM_MISSIONS_WITH_MULTIPLE_CHARACTERS},
	    {"SMT_OPEN_MERCENARY_RECRUITMENT", SMT_OPEN_MERCENARY_RECRUITMENT},
	    {"SMT_AVAILABLE_MERCENARIES", SMT_AVAILABLE_MERCENARIES},
	    {"SMT_HIRE_ALL_QUEUED_UNITS", SMT_HIRE_ALL_QUEUED_UNITS},
	    {"SMT_NO_ROOM_FOR_MERCENARIES", SMT_NO_ROOM_FOR_MERCENARIES},
	    {"SMT_NO_MERCENARIES_AVAILABLE", SMT_NO_MERCENARIES_AVAILABLE},
	    {"SMT_CANNOT_AFFORD_THIS_MERCENARY", SMT_CANNOT_AFFORD_THIS_MERCENARY},
	    {"SMT_ONLY_FAMILY_CAN_RECRUIT_MERCENARIES", SMT_ONLY_FAMILY_CAN_RECRUIT_MERCENARIES},
	    {"SMT_ASSISTANCE_REQUESTED", SMT_ASSISTANCE_REQUESTED},
	    {"SMT_WHICH_ALLY", SMT_WHICH_ALLY},
	    {"SMT_WHICH_ARMY_REQUESTED", SMT_WHICH_ARMY_REQUESTED},
	    {"SMT_ACCEPT_ASSISTANCE_REQUEST", SMT_ACCEPT_ASSISTANCE_REQUEST},
	    {"SMT_ATTACKING_THIS_SETTLEMENT_WILL_BREAK_ALLIANCE", SMT_ATTACKING_THIS_SETTLEMENT_WILL_BREAK_ALLIANCE},
	    {"SMT_ATTACKING_THIS_FORT_WILL_BREAK_ALLIANCE", SMT_ATTACKING_THIS_FORT_WILL_BREAK_ALLIANCE},
	    {"SMT_ATTACKING_THIS_PORT_WILL_BREAK_ALLIANCE", SMT_ATTACKING_THIS_PORT_WILL_BREAK_ALLIANCE},
	    {"SMT_ATTACKING_THIS_ARMY_WILL_BREAK_ALLIANCE", SMT_ATTACKING_THIS_ARMY_WILL_BREAK_ALLIANCE},
	    {"SMT_ATTACKING_THIS_NAVY_WILL_BREAK_ALLIANCE", SMT_ATTACKING_THIS_NAVY_WILL_BREAK_ALLIANCE},
	    {"SMT_CANNOT_ATTACK_ALLY", SMT_CANNOT_ATTACK_ALLY},
	    {"SMT_CONFIRM_ATTACK_ON_ALLY", SMT_CONFIRM_ATTACK_ON_ALLY},
	    {"SMT_NONE", SMT_NONE},
	    {"SMT_BALANCE_OF_POWER", SMT_BALANCE_OF_POWER},
	    {"SMT_YOUR_FORCES", SMT_YOUR_FORCES},
	    {"SMT_ENEMY_FORCES", SMT_ENEMY_FORCES},
	    {"SMT_AI_REINFORCEMENT", SMT_AI_REINFORCEMENT},
	    {"SMT_REINFORCEMENTS", SMT_REINFORCEMENTS},
	    {"SMT_BATTLE_DEPLOYMENT", SMT_BATTLE_DEPLOYMENT},
	    {"SMT_YOUR_ARMY_SALLIES_FORTH", SMT_YOUR_ARMY_SALLIES_FORTH},
	    {"SMT_ENEMY_SALLIES_FORTH", SMT_ENEMY_SALLIES_FORTH},
	    {"SMT_RIGHT_CLICK_FOR_CHARACTER_INFO", SMT_RIGHT_CLICK_FOR_CHARACTER_INFO},
	    {"SMT_FIELD_CONSTRUCTION", SMT_FIELD_CONSTRUCTION},
	    {"SMT_PROVINCE", SMT_PROVINCE},
	    {"SMT_WATCHTOWER_DESCRIPTION", SMT_WATCHTOWER_DESCRIPTION},
	    {"SMT_FORT_DESCRIPTION", SMT_FORT_DESCRIPTION},
	    {"SMT_CANNOT_TRANSFER_GENERALS_UNIT", SMT_CANNOT_TRANSFER_GENERALS_UNIT},
	    {"SMT_FAMOUS_BATTLE_SITE", SMT_FAMOUS_BATTLE_SITE},
	    {"SMT_BATTLE_YEAR", SMT_BATTLE_YEAR},
	    {"SMT_VICTOR", SMT_VICTOR},
	    {"SMT_LOSER", SMT_LOSER},
	    {"SMT_GAMES_FREQUENCY_YEARLY", SMT_GAMES_FREQUENCY_YEARLY},
	    {"SMT_GAMES_FREQUENCY_MONTHLY", SMT_GAMES_FREQUENCY_MONTHLY},
	    {"SMT_GAMES_FREQUENCY_DAILY", SMT_GAMES_FREQUENCY_DAILY},
	    {"SMT_RACES_FREQUENCY_YEARLY", SMT_RACES_FREQUENCY_YEARLY},
	    {"SMT_RACES_FREQUENCY_MONTHLY", SMT_RACES_FREQUENCY_MONTHLY},
	    {"SMT_RACES_FREQUENCY_DAILY", SMT_RACES_FREQUENCY_DAILY},
	    {"SMT_GAMES_AND_RACES_FREQUENCY_YEARLY", SMT_GAMES_AND_RACES_FREQUENCY_YEARLY},
	    {"SMT_GAMES_AND_RACES_FREQUENCY_MONTHLY", SMT_GAMES_AND_RACES_FREQUENCY_MONTHLY},
	    {"SMT_GAMES_AND_RACES_FREQUENCY_DAILY", SMT_GAMES_AND_RACES_FREQUENCY_DAILY},
	    {"SMT_SHOW_SETTLEMENT_STATS", SMT_SHOW_SETTLEMENT_STATS},
	    {"SMT_AUTOMANAGE", SMT_AUTOMANAGE},
	    {"SMT_SETTLEMENT_STATISTICS", SMT_SETTLEMENT_STATISTICS},
	    {"SMT_PUBLIC_ORDER", SMT_PUBLIC_ORDER},
	    {"SMT_POPULATION_GROWTH", SMT_POPULATION_GROWTH},
	    {"SMT_SET_POLICIES", SMT_SET_POLICIES},
	    {"SMT_POPULATION_REQUIRED", SMT_POPULATION_REQUIRED},
	    {"SMT_CONSTRUCTION_TIME", SMT_CONSTRUCTION_TIME},
	    {"SMT_CONSTRUCTION_COST", SMT_CONSTRUCTION_COST},
	    {"SMT_ANCILLARIES", SMT_ANCILLARIES},
	    {"SMT_TRAITS", SMT_TRAITS},
	    {"SMT_RECRUITMENT_OPTIONS", SMT_RECRUITMENT_OPTIONS},
	    {"SMT_MEN_IN_GARRISON", SMT_MEN_IN_GARRISON},
	    {"SMT_MEN_IN_ARMY", SMT_MEN_IN_ARMY},
	    {"SMT_SELECTED_MERCENARIES", SMT_SELECTED_MERCENARIES},
	    {"SMT_SHOW_FACTION_RANKINGS", SMT_SHOW_FACTION_RANKINGS},
	    {"SMT_FACTION_RANKINGS", SMT_FACTION_RANKINGS},
	    {"SMT_RANKING_SCORE", SMT_RANKING_SCORE},
	    {"SMT_RANKING_MILITARY", SMT_RANKING_MILITARY},
	    {"SMT_RANKING_PRODUCTION", SMT_RANKING_PRODUCTION},
	    {"SMT_RANKING_REGIONS", SMT_RANKING_REGIONS},
	    {"SMT_RANKING_FINANCIAL", SMT_RANKING_FINANCIAL},
	    {"SMT_RANKING_POPULATION", SMT_RANKING_POPULATION},
	    {"SMT_FACTION_FILTER", SMT_FACTION_FILTER},
	    {"SMT_OWN_FACTION", SMT_OWN_FACTION},
	    {"SMT_ALL_FACTIONS", SMT_ALL_FACTIONS},
	    {"SMT_TOP_5_FACTIONS", SMT_TOP_5_FACTIONS},
	    {"SMT_CUSTOM_SELECTION", SMT_CUSTOM_SELECTION},
	    {"SMT_FILTER_NEIGHBOURS", SMT_FILTER_NEIGHBOURS},
	    {"SMT_TURNS", SMT_TURNS},
	    {"SMT_VICTORY", SMT_VICTORY},
	    {"SMT_DEFEAT", SMT_DEFEAT},
	    {"SMT_VICTORY_BODY", SMT_VICTORY_BODY},
	    {"SMT_DEFEAT_BODY", SMT_DEFEAT_BODY},
	    {"SMT_DEFEATED_BY_BODY", SMT_DEFEATED_BY_BODY},
	    {"SMT_DEFEAT_BODY_PROLOGUE", SMT_DEFEAT_BODY_PROLOGUE},
	    {"SMT_DEFEAT_GAME_OVER", SMT_DEFEAT_GAME_OVER},
	    {"SMT_CONTINUE_CAMPAIGN", SMT_CONTINUE_CAMPAIGN},
	    {"SMT_EXIT_CAMPAIGN", SMT_EXIT_CAMPAIGN},
	    {"SMT_CAPTURED_SETTLEMENT", SMT_CAPTURED_SETTLEMENT},
	    {"SMT_OCCUPATION_OPTIONS", SMT_OCCUPATION_OPTIONS},
	    {"SMT_DENARI_GAINED_FROM_LOOTING", SMT_DENARI_GAINED_FROM_LOOTING},
	    {"SMT_POPULATION_KILLED", SMT_POPULATION_KILLED},
	    {"SMT_LOOT_SETTLEMENT", SMT_LOOT_SETTLEMENT},
	    {"SMT_SACK_SETTLEMENT", SMT_SACK_SETTLEMENT},
	    {"SMT_EXTERMINATE", SMT_EXTERMINATE},
	    {"SMT_SETTLEMENT_LOOT_BODY_STRING", SMT_SETTLEMENT_LOOT_BODY_STRING},
	    {"SMT_BUILDINGS_NEEDING_REPAIR", SMT_BUILDINGS_NEEDING_REPAIR},
	    {"SMT_SHOW_NEW_BUILDINGS", SMT_SHOW_NEW_BUILDINGS},
	    {"SMT_SHOW_REPAIRABLE_BUILDINGS", SMT_SHOW_REPAIRABLE_BUILDINGS},
	    {"SMT_REPAIR_TIME", SMT_REPAIR_TIME},
	    {"SMT_REPAIR_COST", SMT_REPAIR_COST},
	    {"SMT_BUILD_POINTS", SMT_BUILD_POINTS},
	    {"SMT_ALWAYS_SHOW_ADVANCED_STATS", SMT_ALWAYS_SHOW_ADVANCED_STATS},
	    {"SMT_ADVANCED_STATS", SMT_ADVANCED_STATS},
	    {"SMT_SHOW_TECH_TREE", SMT_SHOW_TECH_TREE},
	    {"SMT_BUILDING_BROWSER", SMT_BUILDING_BROWSER},
	    {"SMT_STATE_ICON_FARMING_LEVEL", SMT_STATE_ICON_FARMING_LEVEL},
	    {"SMT_STATE_ICON_FARMS_BUILT", SMT_STATE_ICON_FARMS_BUILT},
	    {"SMT_STATE_ICON_HEALTH", SMT_STATE_ICON_HEALTH},
	    {"SMT_STATE_ICON_BUILDINGS", SMT_STATE_ICON_BUILDINGS},
	    {"SMT_STATE_ICON_TAX_RATE_BONUS", SMT_STATE_ICON_TAX_RATE_BONUS},
	    {"SMT_STATE_ICON_SQUALOUR", SMT_STATE_ICON_SQUALOUR},
	    {"SMT_STATE_ICON_TAX_RATE_PENALTY", SMT_STATE_ICON_TAX_RATE_PENALTY},
	    {"SMT_STATE_ICON_GARRISON", SMT_STATE_ICON_GARRISON},
	    {"SMT_STATE_ICON_BUILDINGS_LAW", SMT_STATE_ICON_BUILDINGS_LAW},
	    {"SMT_STATE_ICON_BUILDINGS_FUN", SMT_STATE_ICON_BUILDINGS_FUN},
	    {"SMT_STATE_ICON_GOVERNORS_INFLUENCE", SMT_STATE_ICON_GOVERNORS_INFLUENCE},
	    {"SMT_STATE_ICON_TAX_BONUS", SMT_STATE_ICON_TAX_BONUS},
	    {"SMT_STATE_ICON_TRIUMPH", SMT_STATE_ICON_TRIUMPH},
	    {"SMT_STATE_ICON_BOOM", SMT_STATE_ICON_BOOM},
	    {"SMT_STATE_ICON_DISTANCE_TO_CAPITAL", SMT_STATE_ICON_DISTANCE_TO_CAPITAL},
	    {"SMT_STATE_ICON_NO_GOVERNANCE", SMT_STATE_ICON_NO_GOVERNANCE},
	    {"SMT_STATE_ICON_TAX_PENALTY", SMT_STATE_ICON_TAX_PENALTY},
	    {"SMT_STATE_ICON_TURMOIL", SMT_STATE_ICON_TURMOIL},
	    {"SMT_STATE_ICON_BESIEGED", SMT_STATE_ICON_BESIEGED},
	    {"SMT_STATE_ICON_BLOCKADED", SMT_STATE_ICON_BLOCKADED},
	    {"SMT_STATE_ICON_ENTERTAINED", SMT_STATE_ICON_ENTERTAINED},
	    {"SMT_STATE_ICON_FARMS", SMT_STATE_ICON_FARMS},
	    {"SMT_STATE_ICON_TAXES", SMT_STATE_ICON_TAXES},
	    {"SMT_STATE_ICON_RESOURCES", SMT_STATE_ICON_RESOURCES},
	    {"SMT_STATE_ICON_TRADE", SMT_STATE_ICON_TRADE},
	    {"SMT_STATE_ICON_DIPLOMATIC_INCOME", SMT_STATE_ICON_DIPLOMATIC_INCOME},
	    {"SMT_STATE_ICON_DEMOLITION", SMT_STATE_ICON_DEMOLITION},
	    {"SMT_STATE_ICON_LOOTING", SMT_STATE_ICON_LOOTING},
	    {"SMT_STATE_ICON_ADMIN", SMT_STATE_ICON_ADMIN},
	    {"SMT_STATE_ICON_WAGES", SMT_STATE_ICON_WAGES},
	    {"SMT_STATE_ICON_UPKEEP", SMT_STATE_ICON_UPKEEP},
	    {"SMT_STATE_ICON_CONSTRUCTION", SMT_STATE_ICON_CONSTRUCTION},
	    {"SMT_STATE_ICON_RECRUITMENT", SMT_STATE_ICON_RECRUITMENT},
	    {"SMT_STATE_ICON_DIPLOMATIC_OUTGOINGS", SMT_STATE_ICON_DIPLOMATIC_OUTGOINGS},
	    {"SMT_STATE_ICON_CORRUPTION", SMT_STATE_ICON_CORRUPTION},
	    {"SMT_TECH_TREE_THREAD_ALREADY_BUILT", SMT_TECH_TREE_THREAD_ALREADY_BUILT},
	    {"SMT_TECH_TREE_THREAD_AVAILABLE_TO_BUILD", SMT_TECH_TREE_THREAD_AVAILABLE_TO_BUILD},
	    {"SMT_TECH_TREE_THREAD_NOT_AVAILABLE_TO_BUILD", SMT_TECH_TREE_THREAD_NOT_AVAILABLE_TO_BUILD},
	    {"SMT_TURNS_UNTIL_SURRENDER", SMT_TURNS_UNTIL_SURRENDER},
	    {"SMT_NO_ASSASSINATION_TARGETS", SMT_NO_ASSASSINATION_TARGETS},
	    {"SMT_NO_SABOTAGE_TARGETS", SMT_NO_SABOTAGE_TARGETS},
	    {"SMT_MISSION_TARGET", SMT_MISSION_TARGET},
	    {"SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE", SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE},
	    {"SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE", SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE},
	    {"SMT_GARRISON_BREAKDOWN", SMT_GARRISON_BREAKDOWN},
	    {"SMT_NAVY_BREAKDOWN", SMT_NAVY_BREAKDOWN},
	    {"SMT_INFANTRY", SMT_INFANTRY},
	    {"SMT_CAVALRY", SMT_CAVALRY},
	    {"SMT_ARTILLERY", SMT_ARTILLERY},
	    {"SMT_SHIPS", SMT_SHIPS},
	    {"SMT_ARMY_FULL", SMT_ARMY_FULL},
	    {"SMT_NAVY_FULL", SMT_NAVY_FULL},
	    {"SMT_NO_GARRISON_COMMANDER", SMT_NO_GARRISON_COMMANDER},
	    {"SMT_SAILORS", SMT_SAILORS},
	    {"SMT_CAPABILITY_POPULATION_GROWTH_BONUS", SMT_CAPABILITY_POPULATION_GROWTH_BONUS},
	    {"SMT_CAPABILITY_POPULATION_LOYALTY_BONUS", SMT_CAPABILITY_POPULATION_LOYALTY_BONUS},
	    {"SMT_CAPABILITY_POPULATION_HEALTH_BONUS", SMT_CAPABILITY_POPULATION_HEALTH_BONUS},
	    {"SMT_CAPABILITY_TRADE_BASE_INCOME_BONUS", SMT_CAPABILITY_TRADE_BASE_INCOME_BONUS},
	    {"SMT_CAPABILITY_TRADE_LEVEL_BONUS", SMT_CAPABILITY_TRADE_LEVEL_BONUS},
	    {"SMT_CAPABILITY_TRADE_FLEET", SMT_CAPABILITY_TRADE_FLEET},
	    {"SMT_CAPABILITY_TAXABLE_INCOME_BONUS", SMT_CAPABILITY_TAXABLE_INCOME_BONUS},
	    {"SMT_CAPABILITY_MINE_RESOURCE", SMT_CAPABILITY_MINE_RESOURCE},
	    {"SMT_CAPABILITY_FARMING_LEVEL", SMT_CAPABILITY_FARMING_LEVEL},
	    {"SMT_CAPABILITY_ROAD_LEVEL", SMT_CAPABILITY_ROAD_LEVEL},
	    {"SMT_CAPABILITY_GATE_STRENGTH", SMT_CAPABILITY_GATE_STRENGTH},
	    {"SMT_CAPABILITY_GATE_DEFENSES", SMT_CAPABILITY_GATE_DEFENSES},
	    {"SMT_CAPABILITY_TOWER_LEVEL", SMT_CAPABILITY_TOWER_LEVEL},
	    {"SMT_CAPABILITY_ARMOUR", SMT_CAPABILITY_ARMOUR},
	    {"SMT_CAPABILITY_STAGE_GAMES", SMT_CAPABILITY_STAGE_GAMES},
	    {"SMT_CAPABILITY_STAGE_RACES", SMT_CAPABILITY_STAGE_RACES},
	    {"SMT_CAPABILITY_POPULATION_FIRE_RISK_BONUS", SMT_CAPABILITY_POPULATION_FIRE_RISK_BONUS},
	    {"SMT_CAPABILITY_WEAPON_MELEE_SIMPLE", SMT_CAPABILITY_WEAPON_MELEE_SIMPLE},
	    {"SMT_CAPABILITY_WEAPON_MELEE_BLADE", SMT_CAPABILITY_WEAPON_MELEE_BLADE},
	    {"SMT_CAPABILITY_WEAPON_MISSILE_MECHANICAL", SMT_CAPABILITY_WEAPON_MISSILE_MECHANICAL},
	    {"SMT_CAPABILITY_WEAPON_MISSILE_GUNPOWDER", SMT_CAPABILITY_WEAPON_MISSILE_GUNPOWDER},
	    {"SMT_CAPABILITY_WEAPON_ARTILLERY_MECHANICAL", SMT_CAPABILITY_WEAPON_ARTILLERY_MECHANICAL},
	    {"SMT_CAPABILITY_WEAPON_ARTILLERY_GUNPOWDER", SMT_CAPABILITY_WEAPON_ARTILLERY_GUNPOWDER},
	    {"SMT_CAPABILITY_WEAPON_NAVAL_GUNPOWDER", SMT_CAPABILITY_WEAPON_NAVAL_GUNPOWDER},
	    {"SMT_CAPABILITY_BODYGUARD", SMT_CAPABILITY_BODYGUARD},
	    {"SMT_CAPABILITY_RECRUITS_MORALE_BONUS", SMT_CAPABILITY_RECRUITS_MORALE_BONUS},
	    {"SMT_CAPABILITY_RECRUITS_EXPERIENCE_BONUS", SMT_CAPABILITY_RECRUITS_EXPERIENCE_BONUS},
	    {"SMT_CAPABILITY_HAPPINESS_BONUS", SMT_CAPABILITY_HAPPINESS_BONUS},
	    {"SMT_CAPABILITY_LAW_BONUS", SMT_CAPABILITY_LAW_BONUS},
	    {"SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_MILITARY", SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_MILITARY},
	    {"SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_RELIGIOUS", SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_RELIGIOUS},
	    {"SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_DEFENSIVE", SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_DEFENSIVE},
	    {"SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_OTHER", SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_OTHER},
	    {"SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_MILITARY", SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_MILITARY},
	    {"SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_RELIGIOUS", SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_RELIGIOUS},
	    {"SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_DEFENSIVE", SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_DEFENSIVE},
	    {"SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_OTHER", SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_OTHER},
	    {"SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_WOODEN", SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_WOODEN},
	    {"SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_STONE", SMT_CAPABILITY_CONSTRUCTION_COST_BONUS_STONE},
	    {"SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_WOODEN", SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_WOODEN},
	    {"SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_STONE", SMT_CAPABILITY_CONSTRUCTION_TIME_BONUS_STONE},
	    {"SMT_CAPABILITY_ARCHER_EXP_BONUS", SMT_CAPABILITY_ARCHER_EXP_BONUS},
	    {"SMT_CAPABILITY_CAVALRY_EXP_BONUS", SMT_CAPABILITY_CAVALRY_EXP_BONUS},
	    {"SMT_CAPABILITY_HEAVY_CAVALRY_EXP_BONUS", SMT_CAPABILITY_HEAVY_CAVALRY_EXP_BONUS},
	    {"SMT_CAPABILITY_GUN_EXP_BONUS", SMT_CAPABILITY_GUN_EXP_BONUS},
	    {"SMT_CAPABILITY_NAVY_EXP_BONUS", SMT_CAPABILITY_NAVY_EXP_BONUS},
	    {"SMT_CAPABILITY_RECRUITS_COST_BONUS_NAVAL", SMT_CAPABILITY_RECRUITS_COST_BONUS_NAVAL},
	    {"SMT_CAPABILITY_RETRAIN_COST_BONUS", SMT_CAPABILITY_RETRAIN_COST_BONUS},
	    {"SMT_CAPABILITY_WEAPON_PROJECTILE", SMT_CAPABILITY_WEAPON_PROJECTILE},
	    {"SMT_CAPABILITY_INCOME_BONUS", SMT_CAPABILITY_INCOME_BONUS},
	    {"SMT_CAPS_LOC_MAX_POOL", SMT_CAPS_LOC_MAX_POOL},
	    {"SMT_CAPS_LOC_TRAINS_EXPERIENCE", SMT_CAPS_LOC_TRAINS_EXPERIENCE},
	    {"SMT_CAPS_LOC_TRAINS", SMT_CAPS_LOC_TRAINS},
	    {"SMT_CAPS_LOC_TRAINS_BONUS", SMT_CAPS_LOC_TRAINS_BONUS},
	    {"SMT_CAPS_LOC_RECRUITS_LEVEL", SMT_CAPS_LOC_RECRUITS_LEVEL},
	    {"SMT_CAPS_LOC_RECRUITS", SMT_CAPS_LOC_RECRUITS},
	    {"SMT_CAPS_LOC_RECRUITS_BONUS", SMT_CAPS_LOC_RECRUITS_BONUS},
	    {"SMT_CAPS_LOC_PROVIDES", SMT_CAPS_LOC_PROVIDES},
	    {"SMT_CAPS_LOC_PROVIDES_BONUS", SMT_CAPS_LOC_PROVIDES_BONUS},
	    {"SMT_CAPS_LOC_REQUIRES", SMT_CAPS_LOC_REQUIRES},
	    {"SMT_CAPS_LOC_CONDITION_NOT", SMT_CAPS_LOC_CONDITION_NOT},
	    {"SMT_CAPS_LOC_CONDITION_AND", SMT_CAPS_LOC_CONDITION_AND},
	    {"SMT_CAPS_LOC_CONDITION_OR", SMT_CAPS_LOC_CONDITION_OR},
	    {"SMT_CAPS_LOC_CONDITION_BUILDING_PRESENT_MIN_LEVEL", SMT_CAPS_LOC_CONDITION_BUILDING_PRESENT_MIN_LEVEL},
	    {"SMT_CAPS_LOC_CONDITION_CAPABILITY_VALUE", SMT_CAPS_LOC_CONDITION_CAPABILITY_VALUE},
	    {"SMT_CAPS_GLOBAL", SMT_CAPS_GLOBAL},
	    {"SMT_AUTOMANAGE_SETTLEMENT", SMT_AUTOMANAGE_SETTLEMENT},
	    {"SMT_CANNOT_RECRUIT_WHILST_IN_BATTLE", SMT_CANNOT_RECRUIT_WHILST_IN_BATTLE},
	    {"SMT_VICTORY_CONDITION_LAND_OWNER_TITLE", SMT_VICTORY_CONDITION_LAND_OWNER_TITLE},
	    {"SMT_VICTORY_CONDITION_LAND_OWNER_DESCR", SMT_VICTORY_CONDITION_LAND_OWNER_DESCR},
	    {"SMT_VICTORY_CONDITION_TYCOON_TITLE", SMT_VICTORY_CONDITION_TYCOON_TITLE},
	    {"SMT_VICTORY_CONDITION_TYCOON_DESCR", SMT_VICTORY_CONDITION_TYCOON_DESCR},
	    {"SMT_VICTORY_CONDITION_TIME_LIMIT_TITLE", SMT_VICTORY_CONDITION_TIME_LIMIT_TITLE},
	    {"SMT_VICTORY_CONDITION_TIME_LIMIT_DESCR", SMT_VICTORY_CONDITION_TIME_LIMIT_DESCR},
	    {"SMT_VICTORY_CONDITION_OUTLIVE_FACTIONS_TITLE", SMT_VICTORY_CONDITION_OUTLIVE_FACTIONS_TITLE},
	    {"SMT_VICTORY_CONDITION_OUTLIVE_FACTIONS_DESCR", SMT_VICTORY_CONDITION_OUTLIVE_FACTIONS_DESCR},
	    {"SMT_CAMPAIGN_VICTORY_CONDITIONS", SMT_CAMPAIGN_VICTORY_CONDITIONS},
	    {"SMT_CAMPAIGN_VICTORY_CONDITIONS_COMPLETE", SMT_CAMPAIGN_VICTORY_CONDITIONS_COMPLETE},
	    {"SMT_ENEMY_AMBUSH_FAILED", SMT_ENEMY_AMBUSH_FAILED},
	    {"SMT_ENEMY_AMBUSH_SUCCESS", SMT_ENEMY_AMBUSH_SUCCESS},
	    {"SMT_YOUR_AMBUSH_FAILED", SMT_YOUR_AMBUSH_FAILED},
	    {"SMT_YOUR_AMBUSH_SUCCESS", SMT_YOUR_AMBUSH_SUCCESS},
	    {"SMT_NO_UNITS_IN_NAVY", SMT_NO_UNITS_IN_NAVY},
	    {"SMT_NO_UNITS_IN_ARMY", SMT_NO_UNITS_IN_ARMY},
	    {"SMT_NO_CHARACTERS_WITH_NAVY", SMT_NO_CHARACTERS_WITH_NAVY},
	    {"SMT_NO_CHARACTERS_WITH_ARMY", SMT_NO_CHARACTERS_WITH_ARMY},
	    {"SMT_NO_UNITS_TRANSPORTED", SMT_NO_UNITS_TRANSPORTED},
	    {"SMT_NO_UNITS_IN_GARRISON", SMT_NO_UNITS_IN_GARRISON},
	    {"SMT_NO_BUILDINGS_IN_SETTLEMENT", SMT_NO_BUILDINGS_IN_SETTLEMENT},
	    {"SMT_NO_VISITORS_IN_SETTLEMENT", SMT_NO_VISITORS_IN_SETTLEMENT},
	    {"SMT_NO_VISITORS_IN_FORT", SMT_NO_VISITORS_IN_FORT},
	    {"SMT_LAND_IMPORTS", SMT_LAND_IMPORTS},
	    {"SMT_DIED", SMT_DIED},
	    {"SMT_NO_BUILDING_IN_NEED_OF_REPAIR", SMT_NO_BUILDING_IN_NEED_OF_REPAIR},
	    {"SMT_NO_UNITS_NEED_RETRAINING", SMT_NO_UNITS_NEED_RETRAINING},
	    {"SMT_SHOW_VISITORS_FORT", SMT_SHOW_VISITORS_FORT},
	    {"SMT_DECLINE_MISSION", SMT_DECLINE_MISSION},
	    {"SMT_SELECT_UNIT_TO_REPAIR", SMT_SELECT_UNIT_TO_REPAIR},
	    {"SMT_CANNOT_SET_NAVAL_RALLY_POINT_WITH_NO_PORT", SMT_CANNOT_SET_NAVAL_RALLY_POINT_WITH_NO_PORT},
	    {"SMT_DIED_IN_BATTLE_TITLE", SMT_DIED_IN_BATTLE_TITLE},
	    {"SMT_DIED_IN_BATTLE_DESCRIPTION", SMT_DIED_IN_BATTLE_DESCRIPTION},
	    {"SMT_BREAK_SIEGE_TITLE", SMT_BREAK_SIEGE_TITLE},
	    {"SMT_BREAK_SIEGE_CONFIRMATION", SMT_BREAK_SIEGE_CONFIRMATION},
	    {"SMT_NO_MORE_OFFERS_ALLOWED", SMT_NO_MORE_OFFERS_ALLOWED},
	    {"SMT_NO_MORE_DEMANDS_ALLOWED", SMT_NO_MORE_DEMANDS_ALLOWED},
	    {"SMT_UNITS", SMT_UNITS},
	    {"SMT_SIEGE_EQUIPMENT", SMT_SIEGE_EQUIPMENT},
	    {"SMT_SET_AS_CAPITAL", SMT_SET_AS_CAPITAL},
	    {"SMT_ATTACKING_THIS_CHARACTER_WILL_BREAK_ALLIANCE", SMT_ATTACKING_THIS_CHARACTER_WILL_BREAK_ALLIANCE},
	    {"SMT_ATTACKING_THIS_NAVY_DECLARATION_OF_WAR", SMT_ATTACKING_THIS_NAVY_DECLARATION_OF_WAR},
	    {"SMT_ATTACKING_THIS_ARMY_DECLARATION_OF_WAR", SMT_ATTACKING_THIS_ARMY_DECLARATION_OF_WAR},
	    {"SMT_ATTACKING_THIS_CHARACTER_DECLARATION_OF_WAR", SMT_ATTACKING_THIS_CHARACTER_DECLARATION_OF_WAR},
	    {"SMT_ATTACKING_THIS_SETTLEMENT_DECLARATION_OF_WAR", SMT_ATTACKING_THIS_SETTLEMENT_DECLARATION_OF_WAR},
	    {"SMT_ATTACKING_THIS_FORT_DECLARATION_OF_WAR", SMT_ATTACKING_THIS_FORT_DECLARATION_OF_WAR},
	    {"SMT_ATTACKING_THIS_PORT_DECLARATION_OF_WAR", SMT_ATTACKING_THIS_PORT_DECLARATION_OF_WAR},
	    {"SMT_CONFIRM_ATTACK_ON_NEUTRAL", SMT_CONFIRM_ATTACK_ON_NEUTRAL},
	    {"SMT_ARMY", SMT_ARMY},
	    {"SMT_NAVY", SMT_NAVY},
	    {"SMT_AGENTS", SMT_AGENTS},
	    {"SMT_FLEET", SMT_FLEET},
	    {"SMT_PRIVATE_DEALINGS", SMT_PRIVATE_DEALINGS},
	    {"SMT_CURRENT_TREATIES", SMT_CURRENT_TREATIES},
	    {"SMT_FACTION_OVERVIEW", SMT_FACTION_OVERVIEW},
	    {"SMT_ROSTERS", SMT_ROSTERS},
	    {"SMT_FINANCIAL", SMT_FINANCIAL},
	    {"SMT_RADAR_ZOOM_IN", SMT_RADAR_ZOOM_IN},
	    {"SMT_RADAR_ZOOM_OUT", SMT_RADAR_ZOOM_OUT},
	    {"SMT_RECRUITMENT", SMT_RECRUITMENT},
	    {"SMT_REPAIR", SMT_REPAIR},
	    {"SMT_RETRAIN", SMT_RETRAIN},
	    {"SMT_ASSAULT_NO_INFANTRY", SMT_ASSAULT_NO_INFANTRY},
	    {"SMT_ASSAULT_NO_EQUIPMENT", SMT_ASSAULT_NO_EQUIPMENT},
	    {"SMT_NO_POPULATION_TO_EXTERMINATE", SMT_NO_POPULATION_TO_EXTERMINATE},
	    {"SMT_TOTAL", SMT_TOTAL},
	    {"SMT_PROJECTED", SMT_PROJECTED},
	    {"SMT_STARTING_TREASURY", SMT_STARTING_TREASURY},
	    {"SMT_PROJECTED_END_TREASURY", SMT_PROJECTED_END_TREASURY},
	    {"SMT_END_TREASURY", SMT_END_TREASURY},
	    {"SMT_DIPLOMACY_TRIBUTES", SMT_DIPLOMACY_TRIBUTES},
	    {"SMT_DIPLOMACY_CHANGE_YEARLY_PAYMENT", SMT_DIPLOMACY_CHANGE_YEARLY_PAYMENT},
	    {"SMT_ATTACK_VS_TROOPS", SMT_ATTACK_VS_TROOPS},
	    {"SMT_ATTACK_VS_BUILDINGS", SMT_ATTACK_VS_BUILDINGS},
	    {"SMT_MELEE_ATTACK", SMT_MELEE_ATTACK},
	    {"SMT_MISSILE_ATTACK", SMT_MISSILE_ATTACK},
	    {"SMT_WEAPON_TYPE", SMT_WEAPON_TYPE},
	    {"SMT_MELEE_SIMPLE", SMT_MELEE_SIMPLE},
	    {"SMT_MELEE_BLADE", SMT_MELEE_BLADE},
	    {"SMT_MISSILE_MECHANICAL", SMT_MISSILE_MECHANICAL},
	    {"SMT_MISSILE_GUNPOWDER", SMT_MISSILE_GUNPOWDER},
	    {"SMT_ARTILLERY_MECHANICAL", SMT_ARTILLERY_MECHANICAL},
	    {"SMT_ARTILLERY_GUNPOWDER", SMT_ARTILLERY_GUNPOWDER},
	    {"SMT_NAVAL_GUNPOWDER", SMT_NAVAL_GUNPOWDER},
	    {"SMT_NO_UPGRADE", SMT_NO_UPGRADE},
	    {"SMT_TOTAL_DEFENCE", SMT_TOTAL_DEFENCE},
	    {"SMT_STATE_ICON_PLAGUE", SMT_STATE_ICON_PLAGUE},
	    {"SMT_SUBTERFUGE_COLON", SMT_SUBTERFUGE_COLON},
	    {"SMT_REWARD_UNITS_AT_YOUR_CAPITAL", SMT_REWARD_UNITS_AT_YOUR_CAPITAL},
	    {"SMT_CHARACTER_PLAGUED", SMT_CHARACTER_PLAGUED},
	    {"SMT_LOCATE_MISSION_TARGET", SMT_LOCATE_MISSION_TARGET},
	    {"SMT_REPAIRED", SMT_REPAIRED},
	    {"SMT_RETRAINED", SMT_RETRAINED},
	    {"SMT_VARIES", SMT_VARIES},
	    {"SMT_GATES_OPEN_SETTLEMENT", SMT_GATES_OPEN_SETTLEMENT},
	    {"SMT_GATES_OPEN_FORT", SMT_GATES_OPEN_FORT},
	    {"SMT_CORRUPTION_AND_OTHER", SMT_CORRUPTION_AND_OTHER},
	    {"SMT_SELECT_FORT_OR_WATCHTOWER", SMT_SELECT_FORT_OR_WATCHTOWER},
	    {"SMT_FAMILY_MEMBERS", SMT_FAMILY_MEMBERS},
	    {"SMT_CAPITAL_TITLE", SMT_CAPITAL_TITLE},
	    {"SMT_GARRISON_COMMANDER", SMT_GARRISON_COMMANDER},
	    {"SMT_BLOCKADED", SMT_BLOCKADED},
	    {"SMT_WITHIN_X_TURNS", SMT_WITHIN_X_TURNS},
	    {"SMT_CHANCE_OF_SUCCESS_X_PERCENT", SMT_CHANCE_OF_SUCCESS_X_PERCENT},
	    {"SMT_TIME_TO_PROCESS_SIEGE_QUEUE", SMT_TIME_TO_PROCESS_SIEGE_QUEUE},
	    {"SMT_CHARACTER_BRIBED", SMT_CHARACTER_BRIBED},
	    {"SMT_SETTLEMENT_BRIBED", SMT_SETTLEMENT_BRIBED},
	    {"SMT_VISITORS_BRIBED", SMT_VISITORS_BRIBED},
	    {"SMT_TRAVELLERS_BRIBED", SMT_TRAVELLERS_BRIBED},
	    {"SMT_ENTIRE_ARMY_BRIBED", SMT_ENTIRE_ARMY_BRIBED},
	    {"SMT_ENTIRE_ARMY_DISBANDED", SMT_ENTIRE_ARMY_DISBANDED},
	    {"SMT_UNITS_BRIBED", SMT_UNITS_BRIBED},
	    {"SMT_UNITS_DISBANDED", SMT_UNITS_DISBANDED},
	    {"SMT_TAX", SMT_TAX},
	    {"SMT_AUTOMANAGE_TAXES_TOOLTIP", SMT_AUTOMANAGE_TAXES_TOOLTIP},
	    {"SMT_AUTOMATE_TAXES", SMT_AUTOMATE_TAXES},
	    {"SMT_QUICKSAVE", SMT_QUICKSAVE},
	    {"SMT_LOCATE_BATTLE", SMT_LOCATE_BATTLE},
	    {"SMT_STATE_ICON_DEVASTATION", SMT_STATE_ICON_DEVASTATION},
	    {"SMT_INAPPROPRIATE_UNITS_FOR_ASSAULT", SMT_INAPPROPRIATE_UNITS_FOR_ASSAULT},
	    {"SMT_EXCHANGE_IN_ZOC", SMT_EXCHANGE_IN_ZOC},
	    {"SMT_OPEN_DEMANDS", SMT_OPEN_DEMANDS},
	    {"SMT_OPEN_OFFERS", SMT_OPEN_OFFERS},
	    {"SMT_THIS_CHARACTER_IN_YOUR_FACTION", SMT_THIS_CHARACTER_IN_YOUR_FACTION},
	    {"SMT_1_REGION_LEFT", SMT_1_REGION_LEFT},
	    {"SMT_NEED_GOVERNOR_TO_MANAGE", SMT_NEED_GOVERNOR_TO_MANAGE},
	    {"SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_FORT", SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_FORT},
	    {"SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_FORT", SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_FORT},
	    {"SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_FORT_1_TURN", SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_FORT_1_TURN},
	    {"SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_FORT_1_TURN", SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_FORT_1_TURN},
	    {"SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_1_TURN", SMT_SIEGE_DURATION_WHILST_UNDER_SIEGE_1_TURN},
	    {"SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_1_TURN", SMT_SIEGE_DURATION_WHILST_NOT_UNDER_SIEGE_1_TURN},
	    {"SMT_SHOW_CHARACTER_INFO", SMT_SHOW_CHARACTER_INFO},
	    {"SMT_STATE_ICON_UPKEEP_NEW", SMT_STATE_ICON_UPKEEP_NEW},
	    {"SMT_BY", SMT_BY},
	    {"SMT_ON", SMT_ON},
	    {"SMT_ALLY", SMT_ALLY},
	    {"SMT_ENEMY", SMT_ENEMY},
	    {"SMT_GATES_OPEN_SETTLEMENT_ACTUAL", SMT_GATES_OPEN_SETTLEMENT_ACTUAL},
	    {"SMT_GATES_OPEN_FORT_ACTUAL", SMT_GATES_OPEN_FORT_ACTUAL},
	    {"SMT_DESERTED", SMT_DESERTED},
	    {"SMT_DIED_TRAGIC", SMT_DIED_TRAGIC},
	    {"SMT_DIED_BATTLE", SMT_DIED_BATTLE},
	    {"SMT_DIED_ASSASSINATED", SMT_DIED_ASSASSINATED},
	    {"SMT_EXECUTED", SMT_EXECUTED},
	    {"SMT_BORN", SMT_BORN},
	    {"SMT_DIED_PLAGUE", SMT_DIED_PLAGUE},
	    {"SMT_NEXT", SMT_NEXT},
	    {"SMT_1_YEAR_REMAINING", SMT_1_YEAR_REMAINING},
	    {"SMT_X_YEARS_REMAINING", SMT_X_YEARS_REMAINING},
	    {"SMT_REWARD_ONE_TRIARII_DESCRIPTION", SMT_REWARD_ONE_TRIARII_DESCRIPTION},
	    {"SMT_REWARD_TWO_TRIARII_DESCRIPTION", SMT_REWARD_TWO_TRIARII_DESCRIPTION},
	    {"SMT_SHOW_FACTION_LISTS", SMT_SHOW_FACTION_LISTS},
	    {"SMT_OCCUPY_SETTLEMENT", SMT_OCCUPY_SETTLEMENT},
	    {"SMT_RAZE_SETTLEMENT", SMT_RAZE_SETTLEMENT},
	    {"SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_1", SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_1},
	    {"SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_2", SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_2},
	    {"SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_3", SMT_VICTORY_CONDITION_HOLD_REGIONS_DESCR_3},
	    {"SMT_KINGS_PURSE", SMT_KINGS_PURSE},
	    {"SMT_DOMINANT_RELIGION", SMT_DOMINANT_RELIGION},
	    {"SMT_BELIEFS", SMT_BELIEFS},
	    {"SMT_CURRENT_COMMITMENT", SMT_CURRENT_COMMITMENT},
	    {"SMT_PREDICTED_COMMITMENT", SMT_PREDICTED_COMMITMENT},
	    {"SMT_ABANDON_SETTLEMENT", SMT_ABANDON_SETTLEMENT},
	    {"SMT_CANNOT_FIGHT_AT_NIGHT", SMT_CANNOT_FIGHT_AT_NIGHT},
	    {"SMT_TECH_TREE_THREAD_REQUIRES_TEMPLE_DESTRUCTION", SMT_TECH_TREE_THREAD_REQUIRES_TEMPLE_DESTRUCTION},
	    {"SMT_SUPRESSES_RELIGIOUS_UNREST", SMT_SUPRESSES_RELIGIOUS_UNREST},
	    {"SMT_SUPRESSES_RELIGIOUS_CONVERSION", SMT_SUPRESSES_RELIGIOUS_CONVERSION},
	    {"SMT_LIVING_FACTIONS", SMT_LIVING_FACTIONS},
	    {"SMT_CANNOT_ABANDON_SETTLEMENT_UNDER_SIEGE", SMT_CANNOT_ABANDON_SETTLEMENT_UNDER_SIEGE},
	    {"SMT_CANNOT_ABANDON_SETTLEMENT_NOT_LAST_SETTLEMENT", SMT_CANNOT_ABANDON_SETTLEMENT_NOT_LAST_SETTLEMENT},
	    {"SMT_CANNOT_ABANDON_SETTLEMENT_ALREADY_ABANDONED", SMT_CANNOT_ABANDON_SETTLEMENT_ALREADY_ABANDONED},
	    {"SMT_CANNOT_ABANDON_SETTLEMENT_HORDE_UNITS", SMT_CANNOT_ABANDON_SETTLEMENT_HORDE_UNITS},
	    {"SMT_CURRENT_CONVERSTION_STRENGTH", SMT_CURRENT_CONVERSTION_STRENGTH},
	    {"SMT_PREDICTED_CONVERSION_STRENGTH", SMT_PREDICTED_CONVERSION_STRENGTH},
	    {"SMT_PASSENGERS_CANT_BE_MOVED", SMT_PASSENGERS_CANT_BE_MOVED},
	    {"SMT_CANNOT_OCCUPY_SACKED_SETTLEMENT", SMT_CANNOT_OCCUPY_SACKED_SETTLEMENT},
	    {"SMT_DISBAND_IMPOSSIBLE_IN_BATTLE", SMT_DISBAND_IMPOSSIBLE_IN_BATTLE},
	    {"SMT_DISBAND_IMPOSSIBLE_NAMED_GENERAL", SMT_DISBAND_IMPOSSIBLE_NAMED_GENERAL},
	    {"SMT_DISBAND_IMPOSSIBLE_WHILST_SIEGING", SMT_DISBAND_IMPOSSIBLE_WHILST_SIEGING},
	    {"SMT_DISBAND_IMPOSSIBLE_DOCKED", SMT_DISBAND_IMPOSSIBLE_DOCKED},
	    {"SMT_DISBAND_IMPOSSIBLE_NOT_ON_LAND", SMT_DISBAND_IMPOSSIBLE_NOT_ON_LAND},
	    {"SMT_DISBAND_IMPOSSIBLE_WHILST_EXCHANGING", SMT_DISBAND_IMPOSSIBLE_WHILST_EXCHANGING},
	    {"SMT_MERCENARY_PAY_FROM_LOOTING", SMT_MERCENARY_PAY_FROM_LOOTING},
	    {"SMT_DOMINATION_VICTORY", SMT_DOMINATION_VICTORY},
	    {"SMT_MEET_ENEMY", SMT_MEET_ENEMY},
	    {"SMT_ENEMY_MEETS_YOU", SMT_ENEMY_MEETS_YOU},
	    {"SMT_MERCHANT_DESCR", SMT_MERCHANT_DESCR},
	    {"SMT_DIPLOMACY_OFFER_MARRY_LEADER_TO_PRINCESS", SMT_DIPLOMACY_OFFER_MARRY_LEADER_TO_PRINCESS},
	    {"SMT_DIPLOMACY_OFFER_MARRY_HEIR_TO_PRINCESS", SMT_DIPLOMACY_OFFER_MARRY_HEIR_TO_PRINCESS},
	    {"SMT_DIPLOMACY_OFFER_MARRY_LEADER", SMT_DIPLOMACY_OFFER_MARRY_LEADER},
	    {"SMT_DIPLOMACY_OFFER_MARRY_HEIR", SMT_DIPLOMACY_OFFER_MARRY_HEIR},
	    {"SMT_ACQUIRE", SMT_ACQUIRE},
	    {"SMT_NO_ACQUISITION_TARGETS", SMT_NO_ACQUISITION_TARGETS},
	    {"SMT_DENOUNCE", SMT_DENOUNCE},
	    {"SMT_NO_DENOUNCEMENT_TARGETS", SMT_NO_DENOUNCEMENT_TARGETS},
	    {"SMT_DENOUNCED", SMT_DENOUNCED},
	    {"SMT_RETIRED", SMT_RETIRED},
	    {"SMT_DISGRACED", SMT_DISGRACED},
	    {"SMT_MARRY_GENERALS", SMT_MARRY_GENERALS},
	    {"SMT_MARRIAGE", SMT_MARRIAGE},
	    {"SMT_MARRY", SMT_MARRY},
	    {"SMT_RELIGION", SMT_RELIGION},
	    {"SMT_NO_MARRIAGE_TARGETS", SMT_NO_MARRIAGE_TARGETS},
	    {"SMT_AGENT_LIMIT_REACHED", SMT_AGENT_LIMIT_REACHED},
	    {"SMT_CAPABILITY_RELIGION_LEVEL", SMT_CAPABILITY_RELIGION_LEVEL},
	    {"SMT_CAPABILITY_RELIGION_LEVEL_AMPLIFIER", SMT_CAPABILITY_RELIGION_LEVEL_AMPLIFIER},
	    {"SMT_FLEE", SMT_FLEE},
	    {"SMT_REMAIN", SMT_REMAIN},
	    {"SMT_THE_POPE", SMT_THE_POPE},
	    {"SMT_POPES_NAME", SMT_POPES_NAME},
	    {"SMT_POPES_DETAILS", SMT_POPES_DETAILS},
	    {"SMT_COLLEGE_OF_CARDINALS", SMT_COLLEGE_OF_CARDINALS},
	    {"SMT_RANK", SMT_RANK},
	    {"SMT_EMPTY_SEATS", SMT_EMPTY_SEATS},
	    {"SMT_SHOW_COLLEGE_OF_CARDINALS", SMT_SHOW_COLLEGE_OF_CARDINALS},
	    {"SMT_PAPAL_ELECTION", SMT_PAPAL_ELECTION},
	    {"SMT_PAPAL_ELECTION_RESULTS", SMT_PAPAL_ELECTION_RESULTS},
	    {"SMT_STATE_ICON_RELIGIOUS_UNREST", SMT_STATE_ICON_RELIGIOUS_UNREST},
	    {"SMT_CASTLE", SMT_CASTLE},
	    {"SMT_CONFIRM_CONVERT_SETTLEMENT", SMT_CONFIRM_CONVERT_SETTLEMENT},
	    {"SMT_CONVERT_CASTLE_TO_CITY_FOREVER", SMT_CONVERT_CASTLE_TO_CITY_FOREVER},
	    {"SMT_CONVERT_CASTLE_TO_CITY", SMT_CONVERT_CASTLE_TO_CITY},
	    {"SMT_CONVERT_CITY_TO_CASTLE", SMT_CONVERT_CITY_TO_CASTLE},
	    {"SMT_CRUSADE_TARGETS_TITLE", SMT_CRUSADE_TARGETS_TITLE},
	    {"SMT_JIHAD_TARGETS_TITLE", SMT_JIHAD_TARGETS_TITLE},
	    {"SMT_CRUSADE_TARGETS_BODY", SMT_CRUSADE_TARGETS_BODY},
	    {"SMT_JIHAD_TARGETS_BODY", SMT_JIHAD_TARGETS_BODY},
	    {"SMT_CRUSADE_NO_TARGETS", SMT_CRUSADE_NO_TARGETS},
	    {"SMT_JIHAD_NO_TARGETS", SMT_JIHAD_NO_TARGETS},
	    {"SMT_CONFIRM_JOIN_CRUSADE_AGAINST_ALLY", SMT_CONFIRM_JOIN_CRUSADE_AGAINST_ALLY},
	    {"SMT_CONFIRM_JOIN_JIHAD_AGAINST_ALLY", SMT_CONFIRM_JOIN_JIHAD_AGAINST_ALLY},
	    {"SMT_JOIN_CRUSADE_WILL_BREAK_ALLIANCE", SMT_JOIN_CRUSADE_WILL_BREAK_ALLIANCE},
	    {"SMT_JOIN_JIHAD_WILL_BREAK_ALLIANCE", SMT_JOIN_JIHAD_WILL_BREAK_ALLIANCE},
	    {"SMT_CONFIRM_JOIN_CRUSADE_AGAINST_NEUTRAL", SMT_CONFIRM_JOIN_CRUSADE_AGAINST_NEUTRAL},
	    {"SMT_CONFIRM_JOIN_JIHAD_AGAINST_NEUTRAL", SMT_CONFIRM_JOIN_JIHAD_AGAINST_NEUTRAL},
	    {"SMT_JOIN_CRUSADE_DECLARATION_OF_WAR", SMT_JOIN_CRUSADE_DECLARATION_OF_WAR},
	    {"SMT_JOIN_JIHAD_DECLARATION_OF_WAR", SMT_JOIN_JIHAD_DECLARATION_OF_WAR},
	    {"SMT_PAPAL_STANDING", SMT_PAPAL_STANDING},
	    {"SMT_PAPAL_STANDING_DESCR", SMT_PAPAL_STANDING_DESCR},
	    {"SMT_PAPAL_STANDING_0", SMT_PAPAL_STANDING_0},
	    {"SMT_PAPAL_STANDING_1", SMT_PAPAL_STANDING_1},
	    {"SMT_PAPAL_STANDING_2", SMT_PAPAL_STANDING_2},
	    {"SMT_PAPAL_STANDING_3", SMT_PAPAL_STANDING_3},
	    {"SMT_PAPAL_STANDING_4", SMT_PAPAL_STANDING_4},
	    {"SMT_PAPAL_STANDING_5", SMT_PAPAL_STANDING_5},
	    {"SMT_PAPAL_STANDING_6", SMT_PAPAL_STANDING_6},
	    {"SMT_PAPAL_STANDING_7", SMT_PAPAL_STANDING_7},
	    {"SMT_PAPAL_STANDING_8", SMT_PAPAL_STANDING_8},
	    {"SMT_PAPAL_STANDING_9", SMT_PAPAL_STANDING_9},
	    {"SMT_PAPAL_STANDING_10", SMT_PAPAL_STANDING_10},
	    {"SMT_RECRUITMENT_BUILDING_DAMAGED", SMT_RECRUITMENT_BUILDING_DAMAGED},
	    {"SMT_SELECTED_UNIT_COST", SMT_SELECTED_UNIT_COST},
	    {"SMT_SELECTED_UNITS", SMT_SELECTED_UNITS},
	    {"SMT_RECRUITMENT_POOL_EMPTY", SMT_RECRUITMENT_POOL_EMPTY},
	    {"SMT_RETRAIN_POOL_EMPTY", SMT_RETRAIN_POOL_EMPTY},
	    {"SMT_RECRUITMENT_ORDER_FULL", SMT_RECRUITMENT_ORDER_FULL},
	    {"SMT_REPAIR_QUEUE", SMT_REPAIR_QUEUE},
	    {"SMT_RETRAINING_QUEUE", SMT_RETRAINING_QUEUE},
	    {"SMT_PURCHASE_RECRUITMENT", SMT_PURCHASE_RECRUITMENT},
	    {"SMT_CAPABILITY_FREE_UPKEEP", SMT_CAPABILITY_FREE_UPKEEP},
	    {"SMT_FREE_UPKEEP", SMT_FREE_UPKEEP},
	    {"SMT_NO_PORT", SMT_NO_PORT},
	    {"SMT_RECRUITMENT_NON_COAST", SMT_RECRUITMENT_NON_COAST},
	    {"SMT_TURN_X", SMT_TURN_X},
	    {"SMT_1_TURN_REMAINING", SMT_1_TURN_REMAINING},
	    {"SMT_X_TURNS_REMAINING", SMT_X_TURNS_REMAINING},
	    {"SMT_MISSIONS", SMT_MISSIONS},
	    {"SMT_MISSION_SOURCE", SMT_MISSION_SOURCE},
	    {"SMT_MISSION_DESCRIPTION", SMT_MISSION_DESCRIPTION},
	    {"SMT_NO_MISSIONS", SMT_NO_MISSIONS},
	    {"SMT_REWARD", SMT_REWARD},
	    {"SMT_PENALTY", SMT_PENALTY},
	    {"SMT_PRE_REWARD_MONEY", SMT_PRE_REWARD_MONEY},
	    {"SMT_PRE_PENALTY_MONEY", SMT_PRE_PENALTY_MONEY},
	    {"SMT_REWARD_MONEY", SMT_REWARD_MONEY},
	    {"SMT_PENALTY_MONEY", SMT_PENALTY_MONEY},
	    {"SMT_PRE_PENALTY_INQUISITION", SMT_PRE_PENALTY_INQUISITION},
	    {"SMT_PENALTY_INQUISITION", SMT_PENALTY_INQUISITION},
	    {"SMT_PRE_PENALTY_EXCOMMUNICATION", SMT_PRE_PENALTY_EXCOMMUNICATION},
	    {"SMT_PENALTY_EXCOMMUNICATION", SMT_PENALTY_EXCOMMUNICATION},
	    {"SMT_PRE_REWARD_INCOME", SMT_PRE_REWARD_INCOME},
	    {"SMT_PRE_PENALTY_INCOME", SMT_PRE_PENALTY_INCOME},
	    {"SMT_PENALTY_INCOME", SMT_PENALTY_INCOME},
	    {"SMT_REWARD_INCOME", SMT_REWARD_INCOME},
	    {"SMT_PRE_REWARD_GUILD_MONEY", SMT_PRE_REWARD_GUILD_MONEY},
	    {"SMT_PRE_PENALTY_GUILD_MONEY", SMT_PRE_PENALTY_GUILD_MONEY},
	    {"SMT_REWARD_GUILD_MONEY", SMT_REWARD_GUILD_MONEY},
	    {"SMT_PENALTY_GUILD_MONEY", SMT_PENALTY_GUILD_MONEY},
	    {"SMT_PRE_PENALTY_ASSASSINATION", SMT_PRE_PENALTY_ASSASSINATION},
	    {"SMT_PENALTY_ASSASSINATION", SMT_PENALTY_ASSASSINATION},
	    {"SMT_PRE_REWARD_BUILDABLE_UNIT", SMT_PRE_REWARD_BUILDABLE_UNIT},
	    {"SMT_PRE_REWARD_BUILDABLE_UNITS", SMT_PRE_REWARD_BUILDABLE_UNITS},
	    {"SMT_PRE_REWARD_BEST_BUILDABLE_UNIT", SMT_PRE_REWARD_BEST_BUILDABLE_UNIT},
	    {"SMT_PRE_REWARD_BEST_BUILDABLE_UNITS", SMT_PRE_REWARD_BEST_BUILDABLE_UNITS},
	    {"SMT_PRE_REWARD_BEST_FACTION_UNIT", SMT_PRE_REWARD_BEST_FACTION_UNIT},
	    {"SMT_PRE_REWARD_BEST_FACTION_UNITS", SMT_PRE_REWARD_BEST_FACTION_UNITS},
	    {"SMT_REWARD_CANNOT_BUILD_A_UNIT", SMT_REWARD_CANNOT_BUILD_A_UNIT},
	    {"SMT_MISSION_RETURN_PAPAL_SETTLEMENT_SUCCESS", SMT_MISSION_RETURN_PAPAL_SETTLEMENT_SUCCESS},
	    {"SMT_MISSION_RETURN_PAPAL_SETTLEMENT_FAILURE", SMT_MISSION_RETURN_PAPAL_SETTLEMENT_FAILURE},
	    {"SMT_MISSION_RETURN_PAPAL_SETTLEMENT_TITLE", SMT_MISSION_RETURN_PAPAL_SETTLEMENT_TITLE},
	    {"SMT_MISSION_RETURN_PAPAL_SETTLEMENT_PROPOSAL", SMT_MISSION_RETURN_PAPAL_SETTLEMENT_PROPOSAL},
	    {"SMT_MISSION_JOIN_CRUSADE_SUCCESS", SMT_MISSION_JOIN_CRUSADE_SUCCESS},
	    {"SMT_MISSION_JOIN_CRUSADE_EXPIRED_FINISHED", SMT_MISSION_JOIN_CRUSADE_EXPIRED_FINISHED},
	    {"SMT_MISSION_JOIN_CRUSADE_EXPIRED_DEATH", SMT_MISSION_JOIN_CRUSADE_EXPIRED_DEATH},
	    {"SMT_MISSION_JOIN_CRUSADE_EXPIRED_EXCOMMUNICATED", SMT_MISSION_JOIN_CRUSADE_EXPIRED_EXCOMMUNICATED},
	    {"SMT_MISSION_JOIN_CRUSADE_FAILURE", SMT_MISSION_JOIN_CRUSADE_FAILURE},
	    {"SMT_MISSION_JOIN_CRUSADE_TITLE", SMT_MISSION_JOIN_CRUSADE_TITLE},
	    {"SMT_MISSION_JOIN_CRUSADE_HEIR_TITLE", SMT_MISSION_JOIN_CRUSADE_HEIR_TITLE},
	    {"SMT_MISSION_JOIN_CRUSADE_LEADER_TITLE", SMT_MISSION_JOIN_CRUSADE_LEADER_TITLE},
	    {"SMT_MISSION_JOIN_CRUSADE_CHARACTER_PROPOSAL", SMT_MISSION_JOIN_CRUSADE_CHARACTER_PROPOSAL},
	    {"SMT_MISSION_JOIN_CRUSADE_GENERAL_PROPOSAL", SMT_MISSION_JOIN_CRUSADE_GENERAL_PROPOSAL},
	    {"SMT_MISSION_BREAK_ALLIANCE_TITLE", SMT_MISSION_BREAK_ALLIANCE_TITLE},
	    {"SMT_MISSION_BREAK_ALLIANCE_PROPOSAL", SMT_MISSION_BREAK_ALLIANCE_PROPOSAL},
	    {"SMT_MISSION_BREAK_ALLIANCE_EXPIRY_DEATH", SMT_MISSION_BREAK_ALLIANCE_EXPIRY_DEATH},
	    {"SMT_MISSION_BREAK_ALLIANCE_EXPIRY_EXCOMMUNICATED", SMT_MISSION_BREAK_ALLIANCE_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_BREAK_ALLIANCE_EXPIRY_RECOMMUNICATED", SMT_MISSION_BREAK_ALLIANCE_EXPIRY_RECOMMUNICATED},
	    {"SMT_MISSION_BREAK_ALLIANCE_FAILURE", SMT_MISSION_BREAK_ALLIANCE_FAILURE},
	    {"SMT_MISSION_BREAK_ALLIANCE_SUCCESS", SMT_MISSION_BREAK_ALLIANCE_SUCCESS},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_TITLE", SMT_MISSION_PAPAL_BLOCKADE_PORT_TITLE},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_PROPOSAL", SMT_MISSION_PAPAL_BLOCKADE_PORT_PROPOSAL},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_SETTLEMENT_TAKEN", SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_SETTLEMENT_TAKEN},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_EXCOMMUNICATED", SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_RECOMMUNICATED", SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_RECOMMUNICATED},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_BLOCKADED", SMT_MISSION_PAPAL_BLOCKADE_PORT_EXPIRY_BLOCKADED},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_FAILURE", SMT_MISSION_PAPAL_BLOCKADE_PORT_FAILURE},
	    {"SMT_MISSION_PAPAL_BLOCKADE_PORT_SUCCESS", SMT_MISSION_PAPAL_BLOCKADE_PORT_SUCCESS},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_TITLE", SMT_MISSION_ASSASSINATE_CARDINAL_TITLE},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_PROPOSAL", SMT_MISSION_ASSASSINATE_CARDINAL_PROPOSAL},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_DEATH", SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_DEATH},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_EXCOMMUNICATED", SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_HIDING", SMT_MISSION_ASSASSINATE_CARDINAL_EXPIRY_HIDING},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_FAILURE", SMT_MISSION_ASSASSINATE_CARDINAL_FAILURE},
	    {"SMT_MISSION_ASSASSINATE_CARDINAL_SUCCESS", SMT_MISSION_ASSASSINATE_CARDINAL_SUCCESS},
	    {"SMT_MISSION_ASSASSINATE_WITCH_TITLE", SMT_MISSION_ASSASSINATE_WITCH_TITLE},
	    {"SMT_MISSION_ASSASSINATE_WITCH_PROPOSAL", SMT_MISSION_ASSASSINATE_WITCH_PROPOSAL},
	    {"SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_DEATH", SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_DEATH},
	    {"SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_EXCOMMUNICATED", SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_HIDING", SMT_MISSION_ASSASSINATE_WITCH_EXPIRY_HIDING},
	    {"SMT_MISSION_ASSASSINATE_WITCH_FAILURE", SMT_MISSION_ASSASSINATE_WITCH_FAILURE},
	    {"SMT_MISSION_ASSASSINATE_WITCH_SUCCESS", SMT_MISSION_ASSASSINATE_WITCH_SUCCESS},
	    {"SMT_MISSION_CEASE_HOSTILITIES_TITLE", SMT_MISSION_CEASE_HOSTILITIES_TITLE},
	    {"SMT_MISSION_CEASE_HOSTILITIES_PROPOSAL", SMT_MISSION_CEASE_HOSTILITIES_PROPOSAL},
	    {"SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_DEATH", SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_DEATH},
	    {"SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_EXCOMMUNICATED", SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_TARGET_EXCOMMUNICATED", SMT_MISSION_CEASE_HOSTILITIES_EXPIRY_TARGET_EXCOMMUNICATED},
	    {"SMT_MISSION_CEASE_HOSTILITIES_FAILURE", SMT_MISSION_CEASE_HOSTILITIES_FAILURE},
	    {"SMT_MISSION_CEASE_HOSTILITIES_SUCCESS", SMT_MISSION_CEASE_HOSTILITIES_SUCCESS},
	    {"SMT_MISSION_PAPAL_BUILD_CHURCH_TITLE", SMT_MISSION_PAPAL_BUILD_CHURCH_TITLE},
	    {"SMT_MISSION_PAPAL_BUILD_CHURCH_PROPOSAL", SMT_MISSION_PAPAL_BUILD_CHURCH_PROPOSAL},
	    {"SMT_MISSION_PAPAL_BUILD_CHURCH_EXPIRY_LOST", SMT_MISSION_PAPAL_BUILD_CHURCH_EXPIRY_LOST},
	    {"SMT_MISSION_PAPAL_BUILD_CHURCH_EXPIRY_EXCOMMUNICATED", SMT_MISSION_PAPAL_BUILD_CHURCH_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_PAPAL_BUILD_CHURCH_FAILURE", SMT_MISSION_PAPAL_BUILD_CHURCH_FAILURE},
	    {"SMT_MISSION_PAPAL_BUILD_CHURCH_SUCCESS", SMT_MISSION_PAPAL_BUILD_CHURCH_SUCCESS},
	    {"SMT_MISSION_CONVERT_TITLE", SMT_MISSION_CONVERT_TITLE},
	    {"SMT_MISSION_CONVERT_PROPOSAL", SMT_MISSION_CONVERT_PROPOSAL},
	    {"SMT_MISSION_CONVERT_EXPIRY_EXCOMMUNICATED", SMT_MISSION_CONVERT_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_CONVERT_FAILURE", SMT_MISSION_CONVERT_FAILURE},
	    {"SMT_MISSION_CONVERT_SUCCESS", SMT_MISSION_CONVERT_SUCCESS},
	    {"SMT_MISSION_GIFT_FROM_POPE_TITLE", SMT_MISSION_GIFT_FROM_POPE_TITLE},
	    {"SMT_MISSION_GIFT_FROM_POPE_PROPOSAL", SMT_MISSION_GIFT_FROM_POPE_PROPOSAL},
	    {"SMT_MISSION_PAPAL_GIVE_CASH_TITLE", SMT_MISSION_PAPAL_GIVE_CASH_TITLE},
	    {"SMT_MISSION_PAPAL_GIVE_CASH_PROPOSAL", SMT_MISSION_PAPAL_GIVE_CASH_PROPOSAL},
	    {"SMT_MISSION_PAPAL_GIVE_CASH_FAILURE", SMT_MISSION_PAPAL_GIVE_CASH_FAILURE},
	    {"SMT_MISSION_PAPAL_GIVE_CASH_SUCCESS", SMT_MISSION_PAPAL_GIVE_CASH_SUCCESS},
	    {"SMT_MISSION_PAPAL_GIVE_ANCILLARY_TITLE", SMT_MISSION_PAPAL_GIVE_ANCILLARY_TITLE},
	    {"SMT_MISSION_PAPAL_GIVE_ANCILLARY_PROPOSAL", SMT_MISSION_PAPAL_GIVE_ANCILLARY_PROPOSAL},
	    {"SMT_MISSION_PAPAL_GIVE_ANCILLARY_FAILURE", SMT_MISSION_PAPAL_GIVE_ANCILLARY_FAILURE},
	    {"SMT_MISSION_PAPAL_GIVE_ANCILLARY_SUCCESS", SMT_MISSION_PAPAL_GIVE_ANCILLARY_SUCCESS},
	    {"SMT_MISSION_RECRUIT_PRIEST_TITLE", SMT_MISSION_RECRUIT_PRIEST_TITLE},
	    {"SMT_MISSION_RECRUIT_PRIEST_PROPOSAL", SMT_MISSION_RECRUIT_PRIEST_PROPOSAL},
	    {"SMT_MISSION_RECRUIT_PRIEST_EXPIRY_LOST", SMT_MISSION_RECRUIT_PRIEST_EXPIRY_LOST},
	    {"SMT_MISSION_RECRUIT_PRIEST_EXPIRY_EXCOMMUNICATED", SMT_MISSION_RECRUIT_PRIEST_EXPIRY_EXCOMMUNICATED},
	    {"SMT_MISSION_RECRUIT_PRIEST_FAILURE", SMT_MISSION_RECRUIT_PRIEST_FAILURE},
	    {"SMT_MISSION_RECRUIT_PRIEST_SUCCESS", SMT_MISSION_RECRUIT_PRIEST_SUCCESS},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_TITLE", SMT_MISSION_ASSASSINATE_GENERAL_TITLE},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_PROPOSAL", SMT_MISSION_ASSASSINATE_GENERAL_PROPOSAL},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_DEATH", SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_DEATH},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_ALLIED", SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_ALLIED},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_HIDING", SMT_MISSION_ASSASSINATE_GENERAL_EXPIRY_HIDING},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_FAILURE", SMT_MISSION_ASSASSINATE_GENERAL_FAILURE},
	    {"SMT_MISSION_ASSASSINATE_GENERAL_SUCCESS", SMT_MISSION_ASSASSINATE_GENERAL_SUCCESS},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_TITLE", SMT_MISSION_COUNCIL_BLOCKADE_PORT_TITLE},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_PROPOSAL", SMT_MISSION_COUNCIL_BLOCKADE_PORT_PROPOSAL},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_SETTLEMENT_TAKEN", SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_SETTLEMENT_TAKEN},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_ALLIED", SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_ALLIED},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_BLOCKADED", SMT_MISSION_COUNCIL_BLOCKADE_PORT_EXPIRY_BLOCKADED},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_FAILURE", SMT_MISSION_COUNCIL_BLOCKADE_PORT_FAILURE},
	    {"SMT_MISSION_COUNCIL_BLOCKADE_PORT_SUCCESS", SMT_MISSION_COUNCIL_BLOCKADE_PORT_SUCCESS},
	    {"SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_TITLE", SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_TITLE},
	    {"SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_PROPOSAL", SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_PROPOSAL},
	    {"SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_EXPIRY_LOST", SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_EXPIRY_LOST},
	    {"SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_FAILURE", SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_FAILURE},
	    {"SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_SUCCESS", SMT_MISSION_COUNCIL_TAKE_REBEL_SETTLEMENT_SUCCESS},
	    {"SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_TITLE", SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_TITLE},
	    {"SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_PROPOSAL", SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_PROPOSAL},
	    {"SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_EXPIRY_LOST", SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_EXPIRY_LOST},
	    {"SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_EXPIRY_ALLIED", SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_EXPIRY_ALLIED},
	    {"SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_FAILURE", SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_FAILURE},
	    {"SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_SUCCESS", SMT_MISSION_COUNCIL_TAKE_SETTLEMENT_SUCCESS},
	    {"SMT_MISSION_GUILD_TAKE_SETTLEMENT_TITLE", SMT_MISSION_GUILD_TAKE_SETTLEMENT_TITLE},
	    {"SMT_MISSION_GUILD_TAKE_SETTLEMENT_PROPOSAL", SMT_MISSION_GUILD_TAKE_SETTLEMENT_PROPOSAL},
	    {"SMT_MISSION_GUILD_TAKE_SETTLEMENT_EXPIRY_LOST", SMT_MISSION_GUILD_TAKE_SETTLEMENT_EXPIRY_LOST},
	    {"SMT_MISSION_GUILD_TAKE_SETTLEMENT_EXPIRY_ALLIED", SMT_MISSION_GUILD_TAKE_SETTLEMENT_EXPIRY_ALLIED},
	    {"SMT_MISSION_GUILD_TAKE_SETTLEMENT_FAILURE", SMT_MISSION_GUILD_TAKE_SETTLEMENT_FAILURE},
	    {"SMT_MISSION_GUILD_TAKE_SETTLEMENT_SUCCESS", SMT_MISSION_GUILD_TAKE_SETTLEMENT_SUCCESS},
	    {"SMT_MISSION_GUILD_RECRUIT_AGENT_TITLE", SMT_MISSION_GUILD_RECRUIT_AGENT_TITLE},
	    {"SMT_MISSION_GUILD_RECRUIT_AGENT_PROPOSAL", SMT_MISSION_GUILD_RECRUIT_AGENT_PROPOSAL},
	    {"SMT_MISSION_GUILD_RECRUIT_AGENT_EXPIRY_LOST", SMT_MISSION_GUILD_RECRUIT_AGENT_EXPIRY_LOST},
	    {"SMT_MISSION_GUILD_RECRUIT_AGENT_FAILURE", SMT_MISSION_GUILD_RECRUIT_AGENT_FAILURE},
	    {"SMT_MISSION_GUILD_RECRUIT_AGENT_SUCCESS", SMT_MISSION_GUILD_RECRUIT_AGENT_SUCCESS},
	    {"SMT_MISSION_GIFT_FROM_COUNCIL_TITLE", SMT_MISSION_GIFT_FROM_COUNCIL_TITLE},
	    {"SMT_MISSION_GIFT_FROM_COUNCIL_PROPOSAL", SMT_MISSION_GIFT_FROM_COUNCIL_PROPOSAL},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_TITLE", SMT_MISSION_COUNCIL_MARRY_PRINCESS_TITLE},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_PROPOSAL", SMT_MISSION_COUNCIL_MARRY_PRINCESS_PROPOSAL},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_GENERAL_DEATH", SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_GENERAL_DEATH},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_PRINCESS_DEATH", SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_PRINCESS_DEATH},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_TARGET_MARRIED", SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_TARGET_MARRIED},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_FAILURE_PRINCESS_MARRIED", SMT_MISSION_COUNCIL_MARRY_PRINCESS_FAILURE_PRINCESS_MARRIED},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_PRINCESS_RETIRED", SMT_MISSION_COUNCIL_MARRY_PRINCESS_EXPIRY_PRINCESS_RETIRED},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_FAILURE", SMT_MISSION_COUNCIL_MARRY_PRINCESS_FAILURE},
	    {"SMT_MISSION_COUNCIL_MARRY_PRINCESS_SUCCESS", SMT_MISSION_COUNCIL_MARRY_PRINCESS_SUCCESS},
	    {"SMT_MISSION_ACQUISITION_TITLE", SMT_MISSION_ACQUISITION_TITLE},
	    {"SMT_MISSION_ACQUISITION_PROPOSAL", SMT_MISSION_ACQUISITION_PROPOSAL},
	    {"SMT_MISSION_ACQUISITION_EXPIRY_DEATH", SMT_MISSION_ACQUISITION_EXPIRY_DEATH},
	    {"SMT_MISSION_ACQUISITION_EXPIRY_HIDING", SMT_MISSION_ACQUISITION_EXPIRY_HIDING},
	    {"SMT_MISSION_ACQUISITION_FAILURE", SMT_MISSION_ACQUISITION_FAILURE},
	    {"SMT_MISSION_ACQUISITION_SUCCESS", SMT_MISSION_ACQUISITION_SUCCESS},
	    {"SMT_MISSION_TRADE_AGREEMENT_TITLE", SMT_MISSION_TRADE_AGREEMENT_TITLE},
	    {"SMT_MISSION_TRADE_AGREEMENT_PROPOSAL", SMT_MISSION_TRADE_AGREEMENT_PROPOSAL},
	    {"SMT_MISSION_TRADE_AGREEMENT_EXPIRY_DEATH", SMT_MISSION_TRADE_AGREEMENT_EXPIRY_DEATH},
	    {"SMT_MISSION_TRADE_AGREEMENT_EXPIRY_WAR", SMT_MISSION_TRADE_AGREEMENT_EXPIRY_WAR},
	    {"SMT_MISSION_TRADE_AGREEMENT_FAILURE", SMT_MISSION_TRADE_AGREEMENT_FAILURE},
	    {"SMT_MISSION_TRADE_AGREEMENT_SUCCESS", SMT_MISSION_TRADE_AGREEMENT_SUCCESS},
	    {"SMT_MISSION_ANNEX_SETTLEMENT_TITLE", SMT_MISSION_ANNEX_SETTLEMENT_TITLE},
	    {"SMT_MISSION_ANNEX_SETTLEMENT_PROPOSAL", SMT_MISSION_ANNEX_SETTLEMENT_PROPOSAL},
	    {"SMT_MISSION_ANNEX_SETTLEMENT_EXPIRY_DEATH", SMT_MISSION_ANNEX_SETTLEMENT_EXPIRY_DEATH},
	    {"SMT_MISSION_ANNEX_SETTLEMENT_EXPIRY_LOST", SMT_MISSION_ANNEX_SETTLEMENT_EXPIRY_LOST},
	    {"SMT_MISSION_ANNEX_SETTLEMENT_FAILURE", SMT_MISSION_ANNEX_SETTLEMENT_FAILURE},
	    {"SMT_MISSION_ANNEX_SETTLEMENT_SUCCESS", SMT_MISSION_ANNEX_SETTLEMENT_SUCCESS},
	    {"SMT_MISSION_SEND_EMISSARY_TITLE", SMT_MISSION_SEND_EMISSARY_TITLE},
	    {"SMT_MISSION_SEND_EMISSARY_PROPOSAL", SMT_MISSION_SEND_EMISSARY_PROPOSAL},
	    {"SMT_MISSION_SEND_EMISSARY_EXPIRY_DEATH", SMT_MISSION_SEND_EMISSARY_EXPIRY_DEATH},
	    {"SMT_MISSION_SEND_EMISSARY_FAILURE", SMT_MISSION_SEND_EMISSARY_FAILURE},
	    {"SMT_MISSION_SEND_EMISSARY_SUCCESS", SMT_MISSION_SEND_EMISSARY_SUCCESS},
	    {"SMT_MISSION_REINFORCE_REGION_TITLE", SMT_MISSION_REINFORCE_REGION_TITLE},
	    {"SMT_MISSION_REINFORCE_REGION_PROPOSAL", SMT_MISSION_REINFORCE_REGION_PROPOSAL},
	    {"SMT_MISSION_REINFORCE_REGION_EXPIRY_LOST", SMT_MISSION_REINFORCE_REGION_EXPIRY_LOST},
	    {"SMT_MISSION_REINFORCE_REGION_FAILURE", SMT_MISSION_REINFORCE_REGION_FAILURE},
	    {"SMT_MISSION_REINFORCE_REGION_SUCCESS", SMT_MISSION_REINFORCE_REGION_SUCCESS},
	    {"SMT_MISSION_GUILD_BUILD_TITLE", SMT_MISSION_GUILD_BUILD_TITLE},
	    {"SMT_MISSION_GUILD_BUILD_PROPOSAL", SMT_MISSION_GUILD_BUILD_PROPOSAL},
	    {"SMT_MISSION_REGICIDE_TITLE", SMT_MISSION_REGICIDE_TITLE},
	    {"SMT_MISSION_REGICIDE_PROPOSAL", SMT_MISSION_REGICIDE_PROPOSAL},
	    {"SMT_MISSION_REGICIDE_EXPIRY_DEATH", SMT_MISSION_REGICIDE_EXPIRY_DEATH},
	    {"SMT_MISSION_REGICIDE_EXPIRY_HIDING", SMT_MISSION_REGICIDE_EXPIRY_HIDING},
	    {"SMT_MISSION_REGICIDE_FAILURE", SMT_MISSION_REGICIDE_FAILURE},
	    {"SMT_MISSION_REGICIDE_SUCCESS", SMT_MISSION_REGICIDE_SUCCESS},
	    {"SMT_MISSION_CRUSADE_TITLE", SMT_MISSION_CRUSADE_TITLE},
	    {"SMT_MISSION_CRUSADE_PROPOSAL", SMT_MISSION_CRUSADE_PROPOSAL},
	    {"SMT_MISSION_CRUSADE_TARGET_PROPOSAL", SMT_MISSION_CRUSADE_TARGET_PROPOSAL},
	    {"SMT_MISSION_JIHAD_TITLE", SMT_MISSION_JIHAD_TITLE},
	    {"SMT_MISSION_JIHAD_PROPOSAL", SMT_MISSION_JIHAD_PROPOSAL},
	    {"SMT_MISSION_JIHAD_TARGET_PROPOSAL", SMT_MISSION_JIHAD_TARGET_PROPOSAL},
	    {"SMT_VOTE", SMT_VOTE},
	    {"SMT_PREFERATI", SMT_PREFERATI},
	    {"SMT_SUPPORTERS", SMT_SUPPORTERS},
	    {"SMT_UNDECIDED_VOTES", SMT_UNDECIDED_VOTES},
	    {"SMT_CLICK_DIPLOMACY", SMT_CLICK_DIPLOMACY},
	    {"SMT_VOTES", SMT_VOTES},
	    {"SMT_PAPAL_STANDING_ELECTION_0", SMT_PAPAL_STANDING_ELECTION_0},
	    {"SMT_PAPAL_STANDING_ELECTION_1", SMT_PAPAL_STANDING_ELECTION_1},
	    {"SMT_PAPAL_STANDING_ELECTION_2", SMT_PAPAL_STANDING_ELECTION_2},
	    {"SMT_PAPAL_STANDING_ELECTION_3", SMT_PAPAL_STANDING_ELECTION_3},
	    {"SMT_PAPAL_STANDING_ELECTION_4", SMT_PAPAL_STANDING_ELECTION_4},
	    {"SMT_PAPAL_STANDING_ELECTION_5", SMT_PAPAL_STANDING_ELECTION_5},
	    {"SMT_PAPAL_STANDING_ELECTION_6", SMT_PAPAL_STANDING_ELECTION_6},
	    {"SMT_PAPAL_STANDING_ELECTION_7", SMT_PAPAL_STANDING_ELECTION_7},
	    {"SMT_PAPAL_STANDING_ELECTION_8", SMT_PAPAL_STANDING_ELECTION_8},
	    {"SMT_PAPAL_STANDING_ELECTION_9", SMT_PAPAL_STANDING_ELECTION_9},
	    {"SMT_PAPAL_STANDING_ELECTION_10", SMT_PAPAL_STANDING_ELECTION_10},
	    {"SMT_POPE_ELECTED", SMT_POPE_ELECTED},
	    {"SMT_VOTED_POPE", SMT_VOTED_POPE},
	    {"SMT_VOTED_NO_POPE", SMT_VOTED_NO_POPE},
	    {"SMT_NO_POPE", SMT_NO_POPE},
	    {"SMT_EXCOMMUNICATE", SMT_EXCOMMUNICATE},
	    {"SMT_ACCEPT_ELECTION", SMT_ACCEPT_ELECTION},
	    {"SMT_DIPLOMACY_OFFER_RECOMMUNICATION_DEMAND", SMT_DIPLOMACY_OFFER_RECOMMUNICATION_DEMAND},
	    {"SMT_DIPLOMACY_OFFER_RECOMMUNICATION_OFFER", SMT_DIPLOMACY_OFFER_RECOMMUNICATION_OFFER},
	    {"SMT_DIPLOMACY_OFFER_CARDINAL_VOTE_DEMAND", SMT_DIPLOMACY_OFFER_CARDINAL_VOTE_DEMAND},
	    {"SMT_DIPLOMACY_OFFER_CARDINAL_VOTE_OFFER", SMT_DIPLOMACY_OFFER_CARDINAL_VOTE_OFFER},
	    {"SMT_STATE_ICON_FEAR", SMT_STATE_ICON_FEAR},
	    {"SMT_STATE_ICON_GLORY", SMT_STATE_ICON_GLORY},
	    {"SMT_STATE_ICON_EXCOMMUNICATION", SMT_STATE_ICON_EXCOMMUNICATION},
	    {"SMT_ATTACKING_THIS_CRUSADE_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_CRUSADE_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_NAVY_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_NAVY_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_ARMY_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_ARMY_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_CHARACTER_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_CHARACTER_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_SETTLEMENT_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_SETTLEMENT_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_FORT_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_FORT_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_PORT_WILL_EXCOMMUNICATE", SMT_ATTACKING_THIS_PORT_WILL_EXCOMMUNICATE},
	    {"SMT_ATTACKING_THIS_CRUSADE_WILL_EXCOMMUNICATE_ASSASSIN", SMT_ATTACKING_THIS_CRUSADE_WILL_EXCOMMUNICATE_ASSASSIN},
	    {"SMT_ATTACKING_THIS_CHARACTER_WILL_EXCOMMUNICATE_ASSASSIN", SMT_ATTACKING_THIS_CHARACTER_WILL_EXCOMMUNICATE_ASSASSIN},
	    {"SMT_ATTACKING_THIS_CHARACTER_WILL_BREAK_ALLIANCE_ASSASSIN", SMT_ATTACKING_THIS_CHARACTER_WILL_BREAK_ALLIANCE_ASSASSIN},
	    {"SMT_ATTACKING_THIS_CHARACTER_DECLARATION_OF_WAR_ASSASSIN", SMT_ATTACKING_THIS_CHARACTER_DECLARATION_OF_WAR_ASSASSIN},
	    {"SMT_ATTACKING_POPE_WILL_EXCOMMUNICATE_ASSASSIN", SMT_ATTACKING_POPE_WILL_EXCOMMUNICATE_ASSASSIN},
	    {"SMT_ATTACKING_POPE_WILL_EXCOMMUNICATE", SMT_ATTACKING_POPE_WILL_EXCOMMUNICATE},
	    {"SMT_CONFIRM_EXCOMMUNICATION_ATTACK", SMT_CONFIRM_EXCOMMUNICATION_ATTACK},
	    {"SMT_CAPABILITY_RECRUITMENT_SLOTS", SMT_CAPABILITY_RECRUITMENT_SLOTS},
	    {"SMT_SLOTS_REMAINING", SMT_SLOTS_REMAINING},
	    {"SMT_PRINCESS_MERGE", SMT_PRINCESS_MERGE},
	    {"SMT_PRINCESS_DIPLOMACY", SMT_PRINCESS_DIPLOMACY},
	    {"SMT_FINANCE_BUTTON", SMT_FINANCE_BUTTON},
	    {"SMT_MISSION_BUTTON", SMT_MISSION_BUTTON},
	    {"SMT_MAKE_OFFER_BUTTON", SMT_MAKE_OFFER_BUTTON},
	    {"SMT_PRISONERS_TITLE_CAPTOR", SMT_PRISONERS_TITLE_CAPTOR},
	    {"SMT_PRISONERS_TITLE_CAPTIVE", SMT_PRISONERS_TITLE_CAPTIVE},
	    {"SMT_RANSOM_BUTTON_TITLE_CAPTOR", SMT_RANSOM_BUTTON_TITLE_CAPTOR},
	    {"SMT_RANSOM_BUTTON_TITLE_CAPTIVE", SMT_RANSOM_BUTTON_TITLE_CAPTIVE},
	    {"SMT_TROOPS_CAPTURED_CAPTOR", SMT_TROOPS_CAPTURED_CAPTOR},
	    {"SMT_TROOPS_CAPTURED_CAPTIVE", SMT_TROOPS_CAPTURED_CAPTIVE},
	    {"SMT_TOTAL_RANSOM_VALUE", SMT_TOTAL_RANSOM_VALUE},
	    {"SMT_RANSOM_BUTTON", SMT_RANSOM_BUTTON},
	    {"SMT_RELEASE_BUTTON", SMT_RELEASE_BUTTON},
	    {"SMT_EXECUTE_BUTTON", SMT_EXECUTE_BUTTON},
	    {"SMT_ACCEPT_RANSOM_BUTTON", SMT_ACCEPT_RANSOM_BUTTON},
	    {"SMT_REJECT_RANSOM_BUTTON", SMT_REJECT_RANSOM_BUTTON},
	    {"SMT_CHARACTER_RANK", SMT_CHARACTER_RANK},
	    {"SMT_UNIT", SMT_UNIT},
	    {"SMT_MEN_CAPTURED", SMT_MEN_CAPTURED},
	    {"SMT_RANSOM_VALUE", SMT_RANSOM_VALUE},
	    {"SMT_RANSOM_ACCEPTED_TITLE", SMT_RANSOM_ACCEPTED_TITLE},
	    {"SMT_RANSOM_REJECTED_TITLE", SMT_RANSOM_REJECTED_TITLE},
	    {"SMT_RANSOM_COULD_NOT_PAY_TITLE", SMT_RANSOM_COULD_NOT_PAY_TITLE},
	    {"SMT_RANSOM_ACCEPTED_BODY", SMT_RANSOM_ACCEPTED_BODY},
	    {"SMT_RANSOM_REJECTED_BODY", SMT_RANSOM_REJECTED_BODY},
	    {"SMT_RANSOM_COULD_NOT_PAY_BODY", SMT_RANSOM_COULD_NOT_PAY_BODY},
	    {"SMT_EXECUTED_PRISONER", SMT_EXECUTED_PRISONER},
	    {"SMT_RELEASED_X_PRISONERS", SMT_RELEASED_X_PRISONERS},
	    {"SMT_EXECUTED_X_PRISONERS", SMT_EXECUTED_X_PRISONERS},
	    {"SMT_HUMAN", SMT_HUMAN},
	    {"SMT_AI", SMT_AI},
	    {"SMT_NEXT_ARMOUR_LEVEL", SMT_NEXT_ARMOUR_LEVEL},
	    {"SMT_HOTSEAT_TITLE", SMT_HOTSEAT_TITLE},
	    {"SMT_ENTER_PASSWORD", SMT_ENTER_PASSWORD},
	    {"SMT_VERIFY_PASSWORD", SMT_VERIFY_PASSWORD},
	    {"SMT_PASSWORD_MISMATCH", SMT_PASSWORD_MISMATCH},
	    {"SMT_INCORRECT_PASSWORD", SMT_INCORRECT_PASSWORD},
	    {"SMT_HOTSEAT_START_TURN", SMT_HOTSEAT_START_TURN},
	    {"SMT_INVALID_PASSWORD_ASCII", SMT_INVALID_PASSWORD_ASCII},
	    {"SMT_INVALID_PASSWORD_LENGTH", SMT_INVALID_PASSWORD_LENGTH},
	    {"SMT_SAVEGAME_STATUS", SMT_SAVEGAME_STATUS},
	    {"SMT_SAVEGAME_STATUS_GOOD", SMT_SAVEGAME_STATUS_GOOD},
	    {"SMT_SAVEGAME_STATUS_TAMPERED", SMT_SAVEGAME_STATUS_TAMPERED},
	    {"SMT_SAVEGAME_VERSION", SMT_SAVEGAME_VERSION},
	    {"SMT_SAVEGAME_VERSION_DETAILS", SMT_SAVEGAME_VERSION_DETAILS},
	    {"SMT_SAVEGAME_VERSION_DETAILS_UNKNOWN", SMT_SAVEGAME_VERSION_DETAILS_UNKNOWN},
	    {"SMT_ADMIN_PASSWORD", SMT_ADMIN_PASSWORD},
	    {"SMT_NUM_HUMAN_PLAYERS", SMT_NUM_HUMAN_PLAYERS},
	    {"SMT_AUTORESOLVE_BATTLES", SMT_AUTORESOLVE_BATTLES},
	    {"SMT_CONSOLE_DISABLED", SMT_CONSOLE_DISABLED},
	    {"SMT_SAVED_SETTINGS", SMT_SAVED_SETTINGS},
	    {"SMT_PLAY_BY_EMAIL", SMT_PLAY_BY_EMAIL},
	    {"SMT_EMAIL_ADDRESS", SMT_EMAIL_ADDRESS},
	    {"SMT_HUMAN_FACTIONS", SMT_HUMAN_FACTIONS},
	    {"SMT_AI_FACTIONS", SMT_AI_FACTIONS},
	    {"SMT_HS_GAME_SETTINGS", SMT_HS_GAME_SETTINGS},
	    {"SMT_HS_HOTSEAT_SETTINGS", SMT_HS_HOTSEAT_SETTINGS},
	    {"SMT_HS_INTEGRITY_SETTINGS", SMT_HS_INTEGRITY_SETTINGS},
	    {"SMT_HS_GAME_NAME", SMT_HS_GAME_NAME},
	    {"SMT_HS_VICTOR", SMT_HS_VICTOR},
	    {"SMT_HS_DATA_STATUS", SMT_HS_DATA_STATUS},
	    {"SMT_HS_DATA_STATUS_GOOD", SMT_HS_DATA_STATUS_GOOD},
	    {"SMT_HS_DATA_STATUS_BAD", SMT_HS_DATA_STATUS_BAD},
	    {"SMT_HS_ALLOW_INTERGITY_FAILURE", SMT_HS_ALLOW_INTERGITY_FAILURE},
	    {"SMT_HS_CURRENT_GAME_VERSION", SMT_HS_CURRENT_GAME_VERSION},
	    {"SMT_HS_QUIT_ON_SAVE", SMT_HS_QUIT_ON_SAVE},
	    {"SMT_HS_PASSWORD_INSTRUCTIONS_BLANK", SMT_HS_PASSWORD_INSTRUCTIONS_BLANK},
	    {"SMT_HS_PASSWORD_INSTRUCTIONS_SET", SMT_HS_PASSWORD_INSTRUCTIONS_SET},
	    {"SMT_HS_START_TURN", SMT_HS_START_TURN},
	    {"SMT_PLAYER_CONTROL", SMT_PLAYER_CONTROL},
	    {"SMT_ASSAULT_INVALID_SIEGE_TOWERS", SMT_ASSAULT_INVALID_SIEGE_TOWERS},
	    {"SMT_HOTSEAT_MESSAGE_TITLE", SMT_HOTSEAT_MESSAGE_TITLE},
	    {"SMT_HS_CLEAR_BUTTON", SMT_HS_CLEAR_BUTTON},
	    {"SMT_HS_CANCEL_BUTTON", SMT_HS_CANCEL_BUTTON},
	    {"SMT_HS_SEND_BUTTON", SMT_HS_SEND_BUTTON},
	    {"SMT_RELIGION_RECRUITMENT_REQUIREMENT", SMT_RELIGION_RECRUITMENT_REQUIREMENT},
	    {"SMT_STATE_HOLDING_RACES", SMT_STATE_HOLDING_RACES},
	    {"SMT_STATE_NOT_HOLDING_RACES", SMT_STATE_NOT_HOLDING_RACES},
	    {"SMT_SETTLEMENT_INFILTRATED", SMT_SETTLEMENT_INFILTRATED},
	    {"STRAT_TEXT_COUNT", STRAT_TEXT_COUNT},
	};

	void setStratString(const std::string& key, const std::string& value)
	{
		const auto stringTable = *reinterpret_cast<void**>(dataOffsets::offsets.stratBinTable);
		if (stratTextEnumMap.find(key) == stratTextEnumMap.end())
		{
			logStringGame("setStratString: Could not find key " + key);
			return;
		}
		const auto enumVal = stratTextEnumMap.at(key);
		const auto uniString =  callClassFunc<void*, UNICODE_STRING***, int>(stringTable, 0x8, enumVal);
		if (!uniString)
		{
			logStringGame("setStratString: Could not get string for key " + key);
			return;
		}
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

		//campaign
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

		//custom
		const DWORD limit6 = dataOffsets::offsets.weaponLimit6;
		MemWork::WriteData(&fix1, limit6, 1);

		const DWORD limit7 = dataOffsets::offsets.weaponLimit7;
		MemWork::WriteData(&fix1, limit7, 1);

		const DWORD limit8 = dataOffsets::offsets.weaponLimit8;
		MemWork::WriteData(&fix1, limit8, 1);

		const DWORD limit9 = dataOffsets::offsets.weaponLimit9;
		MemWork::WriteData(&fix2, limit9, 1);

		//historic battle?
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
