#pragma once
#include <string>
#include "realGameTypes.h"
#include "settlement.h"
#include "character.h"
#include "strategyMap.h"
#include "characterRecord.h"

#include "lua/sol.hpp"
struct eventTrigger;

struct loadGameHandler
{
	void* vtable;
	struct UNICODE_STRING*** saveGameName;
};

struct eventAcceptDecline
{
	DWORD vfTable;
	char* name;
	int hash;
};

/* I_CompareCounter script command */
struct compareCounter { /* I_CompareCounter script command */
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	char* commandName; /* name of command(I_CompareCounter) */
	int operation;
	char* operandName; /* name of checked counter */
	int operandNameCrypt; /* maybe crypt of the counter name */
	int checkedValue; /* value for check */
};

struct console_command { /* structure of console command */
	bool(_stdcall** function)(const char*, char*);
	char* name;
	char* description;
	int type;
	int idk;
};

struct consoleCommands {
	struct console_command** commands;
	int reservedElements;
	int size;
};

struct religionDatabase
{
public:
	void* religionDatabase_vtbl;//0x0000
	struct rcString** religionNames; //0x0004
	int32_t religionNamesSize; //0x0008
	int32_t religionCount; //0x000C
	struct rcString** religionPips; //0x0010
	int32_t religionPipsSize; //0x0014
	int32_t religionPipsCount; //0x0018
	void* hashedStringTable; //0x001C
}; //Size: 0x0020

struct options1
{
	void* cursor; //0x0000
	int8_t N0003DD45; //0x0004
	int8_t widescreen; //0x0005
	char pad_0006[2]; //0x0006
	int64_t antiAliasMode; //0x0008
	int16_t subtitles; //0x0010
	int8_t english; //0x0012
	int8_t noBattleTimeLimit; //0x0013
	char pad_0014[4]; //0x0014
	int32_t useNewCursorActions; //0x0018
	int32_t campaignNumTimesPlay; //0x001C
	int8_t uiWinConditions; //0x0020
	int8_t isScenario; //0x0021
	char pad_0022[2]; //0x0022
	int32_t isHotseatEnabled; //0x0024
	int8_t hotseatAutosave; //0x0028
	int8_t email; //0x0029
	int8_t saveConfig; //0x002A
	int8_t closeAfterSave; //0x002B
	int8_t validateData; //0x002C
	int8_t campaignMapSpeedUp; //0x002D
	char pad_002E[2]; //0x002E
	int32_t skipAiFactions; //0x0030
	int8_t labelCharacters; //0x0034
	int8_t noBackGroundFmv; //0x0035
	int8_t disableArrowMarkers; //0x0036
	int8_t arcadeBattles; //0x0037
	int8_t disableEvents; //0x0038
	int8_t isPrologue; //0x0039
	int8_t updateAiCamera; //0x003A
	int8_t hideCampaign; //0x003B
	int32_t unlimitedMenOnBattlefield; //0x003C
	char pad_0040[4]; //0x0040
	void* prefFactionsPlayed; //0x0044
	int32_t tgaReserveSpace; //0x0048
	int32_t keysetUsed; //0x004C
	class N0003DD67* descrShortcuts; //0x0050
	int8_t muteAdvisor; //0x0054
	int8_t advancedStatsAlways; //0x0055
	int8_t microManageAllSettlements; //0x0056
	int8_t blindAdvisor; //0x0057
	int32_t terrainQuality; //0x0058
	int32_t vegetationQuality; //0x005C
	int8_t useQuickChat; //0x0060
	int8_t someBoolNetworkManager; //0x0061
	char pad_0062[1]; //0x0062
	int8_t someBoolOnlinePlay; //0x0063
	int32_t graphicsAdaptor; //0x0064
	int8_t byte_2C6D86C; //0x0068
	int8_t showDemeanour; //0x0069
	char pad_006A[2]; //0x006A
	int32_t radar; //0x006C
	int32_t unitCards; //0x0070
	int32_t sa_cards; //0x0074
	int32_t buttons; //0x0078
	int8_t tutorialBattlePlayed; //0x007C
	int8_t disableVnVs; //0x007D
	int8_t allUsers; //0x007E
	char pad_007F[29]; //0x007F
	int32_t unk_2C6D8A0; //0x009C
	char pad_00A0[28]; //0x00A0
	char* audioProvider; //0x00BC
	char pad_00C0[20]; //0x00C0
	struct UNICODE_STRING** campaignName; //0x00D4
	char pad_00D8[4]; //0x00D8
	struct UNICODE_STRING*** N0003DA7C; //0x00DC
	struct UNICODE_STRING*** maybeGameSpyUsername; //0x00E0
	struct UNICODE_STRING*** gameSpyPassword; //0x00E4
	char pad_00E8[4]; //0x00E8
	char* gameName; //0x00EC
	char pad_00F0[24]; //0x00F0
	char adminPassword[28]; //0x0108
	char* tutorialPath; //0x0124
}; //Size: 0x0128


struct options2
{
	uint16_t defaultCampaignResolutionX; //0x0000
	uint16_t defaultCampaignResolutionY; //0x0002
	uint16_t campaignResolutionX; //0x0004
	uint16_t campaignResolutionY; //0x0006
	uint16_t defaultBattleResolutionX; //0x0008
	uint16_t defaultBattleResolutionY; //0x000A
	uint16_t battleResolutionX; //0x000C
	uint16_t battleResolutionY; //0x000E
	int8_t idk; //0x0010
	int8_t vSync; //0x0011
	char pad_0012[6]; //0x0012
	int8_t uiIconBarCheck; //0x0018
	int8_t uiRadarCheck; //0x0019
	int8_t useMorale; //0x001A
	int8_t uiAmmoCheck; //0x001B
	int8_t useFatigue; //0x001C
	int8_t uiSupplyCheck; //0x001D
	int8_t toggleFowState; //0x001E
	int8_t cameraRestrict; //0x001F
	int8_t eventCutscenes; //0x0020
	char pad_0021[3]; //0x0021
	int32_t defaultCameraInBattle; //0x0024
	int8_t splashes; //0x0028
	int8_t vegetationVideo; //0x0029
	int8_t byte_1639F1A; //0x002A
	int8_t stencilShadows; //0x002B
	int8_t byte_1639F1C; //0x002C
	int8_t audioEnable; //0x002D
	int8_t speechEnable; //0x002E
	int8_t firstTimePlay; //0x002F
	char* audioProviderName; //0x0030
	char pad_0034[1]; //0x0034
	int8_t byte_1639F25; //0x0035
	char pad_0036[1]; //0x0036
	int8_t toggleAutoSave; //0x0037
	int8_t showBanners; //0x0038
	int8_t passwords; //0x0039
	int8_t hotseatTurns; //0x003A
	int8_t hotseatScroll; //0x003B
	int32_t allowValidationFeatures; //0x003C
	int32_t campaignSpeed; //0x0040
	int8_t labelSettlements; //0x0044
	int8_t disablePapalElections; //0x0045
	int8_t autoresolveAllBattles; //0x0046
	int8_t savePrefs; //0x0047
	int8_t disableConsole; //0x0048
	int8_t validateDiplomacy; //0x0049
	char pad_004A[2]; //0x004A
	float someFloat; //0x004C
	int32_t unitDetail; //0x0050
	int32_t buildingDetail; //0x0054
	int32_t maxSoldiersOnBattlefield; //0x0058
	int32_t unitSize; //0x005C
	int32_t cameraRotateSpeed; //0x0060
	int32_t cameraMoveSpeed; //0x0064
	float cameraSmoothing; //0x0068
	int32_t somethingBasedOnBuildingDetail; //0x006C
	int32_t masterVolume; //0x0070
	int32_t musicVolume; //0x0074
	int32_t speechVolume; //0x0078
	int32_t sfxVolume; //0x007C
	int32_t subFactionAccents; //0x0080
	int32_t playerFactionId; //0x0084
	int32_t campaignDifficulty; //0x0088
	int32_t battleDifficulty; //0x008C
	int32_t tgaWidth; //0x0090
	float tgaAspect; //0x0094
	int32_t tgaInputScale; //0x0098
	int32_t scrollMinZoom; //0x009C
	int32_t scrollMaxZoom; //0x00A0
	int32_t advisorVerbosity; //0x00A4
	int8_t useSomethingTgaTextures; //0x00A8
	int8_t byte_1639F99; //0x00A9
	char pad_00AA[2]; //0x00AA
	int32_t effectQuality; //0x00AC
	int32_t EnableCameraCampaignSmoothing; //0x00B0
	int32_t chatMsgDuration; //0x00B4
	int8_t N0003DDF9; //0x00B8
	int8_t saveGameSpyPassword; //0x00B9
	int8_t addDateToLogs; //0x00BA
	int8_t showToolTips; //0x00BB
	int8_t isNormalHud; //0x00BC
	int8_t showPackageLitter; //0x00BD
	char pad_00BE[2]; //0x00BE
	char* normal; //0x00C0
	char pad_00C4[8]; //0x00C4
	int32_t effectNormal; //0x00CC
	char pad_00D0[8]; //0x00D0
	char* low; //0x00D8
	char pad_00DC[8]; //0x00DC
	int32_t effectLow; //0x00E4
	char pad_00E8[8]; //0x00E8
	char* high; //0x00F0
	char pad_00F4[8]; //0x00F4
	int32_t effectHigh; //0x00FC
	char pad_0100[8]; //0x0100
	char* lowest; //0x0108
	char pad_010C[8]; //0x010C
	int32_t effectLowest; //0x0114
	char pad_0118[8]; //0x0118
	char* highest; //0x0120
	char pad_0124[8]; //0x0124
	int32_t effectHighest; //0x012C
	char pad_0130[8]; //0x0130
	char* custom; //0x0138
	char pad_013C[8]; //0x013C
	int32_t effectCustom; //0x0144
	char pad_0148[8]; //0x0148
	char* lower; //0x0150
	char pad_0154[8]; //0x0154
	int32_t effectLower; //0x015C
	char pad_0160[8]; //0x0160
	char* higher; //0x0168
	char pad_016C[8]; //0x016C
	int32_t effectHigher; //0x0174
	char pad_0178[8]; //0x0178
	char* off; //0x0180
	char pad_0184[8]; //0x0184
	int32_t effectOff; //0x018C
	char pad_0190[32]; //0x0190
	int32_t maxSizeEduMaybe1; //0x01B0
	float unitMultDefault; //0x01B4
	int32_t unitMultDefaultId; //0x01B8
	int32_t maxSizeEduMaybe2; //0x01BC
	float unitSizeMultiplierLow; //0x01C0
	int32_t unitMult1ID; //0x01C4
	int32_t maxSizeEduMaybe3; //0x01C8
	float unitSizeMultiplierMedium; //0x01CC
	int32_t unitMult2ID; //0x01D0
	int32_t maxSizeEduMaybe4; //0x01D4
	float unitSizeMultiplierLarge; //0x01D8
	int32_t unitMult3ID; //0x01DC
	char pad_01E0[12]; //0x01E0
	uint16_t word_163A0DC; //0x01EC
	char pad_01EE[10]; //0x01EE
	uint16_t word_163A0E8; //0x01F8
	char pad_01FA[2]; //0x01FA
	uint16_t word_163A0EC; //0x01FC
	char pad_01FE[54]; //0x01FE
	uint32_t currentHotseatPlayersMask; //0x0234
	class N0003DF44* keysetPointer; //0x0238
	char pad_023C[4]; //0x023C
	int32_t keySetUsed; //0x0240
	int32_t preferencesWereLoaded; //0x0244
	bool isHotseatPlayer(int id)
	{
		return currentHotseatPlayersMask & (1 << id);
	}
	void setHotseatPlayer(int id, bool value)
	{
		if (value)
			currentHotseatPlayersMask |= (1 << id);
		else
			currentHotseatPlayersMask &= ~(1 << id);
	}
};

struct fakeTextInput
{
	explicit fakeTextInput(const char* condition, size_t start)
	{
		const std::string scriptPath = "luaCommand";
		unicodePointerPointer = new UNICODE_STRING*;
		gameStringHelpers::createUniString(unicodePointerPointer, scriptPath.c_str());
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

struct counterS
{
	counterS* something1;
	counterS* something2;
	counterS* something3;
	char* counterName;
	int nameCrypt;
	int counterValue;
};
struct countersObjectS
{
	void* something;
	void* testCounterSValue;
};

class m2tweopOptions
{
public:
	static void setHideUnknownUnitTooltips(const bool value) { hideUnknownUnitTooltips = value; }
	static bool getHideUnknownUnitTooltips() { return hideUnknownUnitTooltips; }
	static void setEnableFamilyEventsForTeutonic(const bool value) { enableFamilyEventsForTeutonic = value; }
	static bool getEnableFamilyEventsForTeutonic() { return enableFamilyEventsForTeutonic; }
	static void setHandleUnitCards(const bool value) { eopHandleUnitCards = value; }
	static bool getHandleUnitCards() { return eopHandleUnitCards; }
	static void setWatchTowerRange(const int value) { watchTowerRange = value; }
	static int getWatchTowerRange() { return watchTowerRange; }
	static void setUseEopFrontiers(const bool value) { useEopFrontiers = value; }
	static int getUseEopFrontiers() { return useEopFrontiers; }
	static DWORD getColor() { return (static_cast<uint32_t>(0xFF) << 24) | (khakiTextRed << 16) | (khakiTextGreen << 8) | khakiTextBlue; }
	static void setKhakiTextColor(const uint8_t red, const uint8_t green, const uint8_t blue) { khakiTextRed = red; khakiTextGreen = green; khakiTextBlue = blue; }
	static bool hideUnknownUnitTooltips;
	static bool eopHandleUnitCards;
	static bool enableFamilyEventsForTeutonic;
	static bool useEopFrontiers;
	static bool fixHeroAbilityKillChance;
	static uint8_t khakiTextRed;
	static uint8_t khakiTextGreen;
	static uint8_t khakiTextBlue;
	static int watchTowerRange;
};

struct boostLightMutex
{
	int l;
};

struct boostSpCountedBase
{
	DWORD vTable;
	int useCount;
	int weakCount;
	boostLightMutex mutex;
};

struct boostSharedCount
{
	boostSpCountedBase* pi;
	boostSharedCount()
	{
		pi = techFuncs::createGameClass<boostSpCountedBase>();
		GAME_FUNC(boostSpCountedBase*(__thiscall*)(boostSpCountedBase*), createSpCountedBase)(pi);
	}
};

struct boostSharedPtr
{
	boostLoggerImpl* px;
	boostSharedCount pn;
};
	
struct boostLogger
{
	boostSharedPtr impl;
	bool destroyed;
	char pad[0x3];
};

struct loggerAndLevel
{
	boostLogger* logger;
	int level;
	loggerAndLevel(boostLogger* logger, const int level)
		: logger(logger), level(level) {}
};

struct enabledLogger
{
	std::ostringstream* stream;
	boostLogger* logger;
	int level;
};

struct descrParser
{
	char *buffer;
	int size;
	char *end;
	UNICODE_STRING **filename;
	char *walker;
	char *lineStart;
	int lineNumber;
	bool ownBuffer;
	char pad[3];
	char* getFileName();
};

class eopLogging
{
public:
	boostLogger* eopLog = nullptr;
	std::string eopLogName = "M2TWEOP.core";
	boostLogger* eopLuaLog = nullptr;
	std::string eopLuaLogName = "M2TWEOP.lua";
	boostLoggerImpl* backupLogger = nullptr;
	void createEopLoggers();
	bool isInitialized() { return m_LoggersCreated; }
	static eopLogging* get() { return m_Instance; }
private:
	static eopLogging* m_Instance;
	bool m_LoggersCreated = false;
};

namespace gameHelpers
{
	std::string getModPath();
	std::string getLuaPath();
	std::string getModFolderName();
	void copyFileLua(const std::string& file, const std::string& to);
	void setExpandedString(const std::string& key, const std::string& value);
	std::string getModString(const std::string& path);
	int getGameVersion();
	void generateSprite(const std::string& model);
	void addToLua(sol::state& luaState);
	
	std::string callConsole(const std::string& cmdName, sol::variadic_args va);
	bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer);
	DWORD getScriptCommandByName(const char* cmdName);
	bool conditionLua(const std::string& condition, const eventTrigger* eventData);
	bool condition(const char* condition, const eventTrigger* eventData);
	std::tuple<bool, int> getScriptCounterLua(const char* type);
	int getScriptCounter(const char* counterName, bool& success);
	void setScriptCounter(const char* counterName, int counterValue);
	void scriptCommandLua(const std::string& command, sol::variadic_args va);
	void scriptCommand(const char* command, const char* args);
	void fireGameScriptFunc(void* scriptStruct, DWORD offset);
	void logStringGame(const std::string& msg, const bool fromLua = false);
	void logStringLua(const std::string& msg);
	void logFuncError(const std::string& funcName, const std::string& error);
	int getScriptCounterNoBool(const char* type);

	void historicEvent(const std::string& name, const std::string& title, const std::string& description);
	void historicEvent(const std::string& name, const std::string& title, const std::string& description, bool isChoice);
	void historicEvent(const std::string& name, const std::string& title, const std::string& description, bool isChoice, int xCoord, int yCoord);
	void historicEvent(const std::string& name, const std::string& title, const std::string& description, bool isChoice, int xCoord, int yCoord, const sol::table& factions);
	void historicEventRaw(const char* name, const char* title, const char* description, bool isChoice, uint32_t factions, int xCoord, int yCoord);
	eventAcceptDecline* createEventAcceptDecline(const std::string& name);
	int incrementEventCounter(const std::string& name, int value);
	
	options1* getOptions1();
	options2* getOptions2();
	
	void addToIntArray(int** array, int* value);
	bool intArrayContains(int** array, int value);
	void removeFromIntArray(int** array, int index);
	
	void saveGame(const char* path);
	void loadSaveGame(const char* saveName);
	
	int getReligionCount();
	int getReligionN(const std::string& name);
	int getClimateN(const std::string& name);
	int getCultureN(const std::string& name);
	const char* getReligionName(int index);
	const char* getCultureName(int index);
	const char* getClimateName(int index);
	const char* getClimateName2(int index);
	const char* getReligionName2(int index);
	
	void setEquipmentCosts(int equipType, int cost);
	void setAncLimit(uint8_t limit);
	void setMaxUnitSize(signed short min, signed short max);
	void setMaxBgSize(unsigned char size);
	void fixReligionTrigger();
	void unlockConsoleCommands();
	void toggleUnitHighlight();
	void setReligionsLimit(unsigned char limit);
	void setBuildingChainLimit(unsigned int limit);
	void setGuildCooldown(unsigned char turns);
	int getUnitSize();
	void setTextureCacheSize();
	void unlockWeaponLimit();

	religionDatabase* getReligionDatabase();
	gameDataAllStruct* getGameDataAll();
};

