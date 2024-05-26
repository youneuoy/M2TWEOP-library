#pragma once
#ifndef luaH_DEF
#define luaH_DEF
#pragma comment(lib,"lua/lua5.1.lib")
#pragma comment(lib,"lua/luajit.lib")


#define SOL_SAFE_FUNCTIONS 1
#define SOL_ALL_SAFETIES_ON  0
#define SOL_LUAJIT 1
#include "lua/sol.hpp"


#include <string>
#include "realGameTypes.h"
#include <windows.h>
#include "luaGetSetFuncs.h"
#include "factionHelpers.h"
#include "settlementHelpers.h"
#include "resourcesHelpers.h"
#include "objectsHelpers.h"
#include "gameHelpers.h"
#include "stackStructHelpers.h"
#include "buildingStructHelpers.h"
#include "generalHelpers.h"
#include "unitHelpers.h"
#include "cameraHelpers.h"
#include "m2tweopHelpers.h"
#include "generalCharactericticsHelpers.h"
#include "technicalHelpers.h"
#include "eopEduHelpers.h"
#include "guildHelpers.h"
#include "eopFbxHelpers.h"
#include "siegeHelpers.h"
#include "gameSTDUIHelpers.h"

#define tryLua(luaFunc)  \
auto funcResult = luaFunc;\
if (!funcResult.valid())\
{\
	sol::error luaError = funcResult;\
	MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);\
	if (plugData::data.luaAll.checkVar("terminateAtLuaException", 1) == true)\
	{\
		terminate();\
	}\
}\


#define tryLuaGetRes(luaFunc,result)  \
auto funcResult = luaFunc;\
if (!funcResult.valid())\
{\
	sol::error luaError = funcResult;\
	MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);\
	if (plugData::data.luaAll.checkVar("terminateAtLuaException", 1) == true)\
	{\
		terminate();\
	}\
}\
else\
{\
result=funcResult;	\
}\



class luaP
{
public:
	static std::vector<std::string> logS;
	static std::vector<std::string> logCommands;
	std::string luaPath;
	bool checkVar(const char* gName, int variable);

	//create lua instance, init some basic tables
	sol::state* init(std::string& luaFilePath, std::string& modPath);
	//init part 2 of tables
	void initP2();
	void initCampaign();
	void initTech();
	void initEopEdu();
	void initEopFbx();
	void initEop3dObjects();
	void initEopSounds();
	void runScriptS(std::string* script);

	//lua functions and events controllers
	void onPluginLoadF();
	void onChangeTurnNum(int num);

	sol::state luaState;

	/*
	struct
	{
		sol::table M2TWEOPTable;
		sol::table M2TWEOPEDUTable;
		sol::table M2TWEOPFBXTable;


		sol::table stratmapTable;
		//this inside stratmap table
		sol::table objectsTable;
		sol::table cameraTable;
		sol::table gameTable;


	}tables;
	struct
	{
		//global game table
		sol::usertype<gameDataAllStruct> gameDataAllTable;
		//this inside gameDataAll table
		sol::usertype<battleDataS> battleTable;
		sol::usertype<battleSide> battleSideTable;
		sol::usertype<trackedPointerArmy> trackedPointerArmyTable;
		sol::usertype<deploymentAreaS> deploymentAreaTable;

		sol::usertype<campaign> campaignTable;
	}typeAll;
	struct
	{
		sol::table uniString;
	}techTypes;
	struct
	{
		sol::usertype<unit>unit;
		sol::usertype<general>character;
		sol::usertype<namedCharacter>namedCharacter;
		sol::usertype<ancillary>ancillary;
		sol::usertype<traitContainer>traitContainerT;
		sol::usertype<EduEntry>EduEntry;
		sol::usertype<factionStruct>factionStruct;
		sol::usertype<fortStruct>fortStruct;
		sol::usertype<portBuildingStruct>portBuildingStruct;
		sol::usertype<settlementStruct>settlementStruct;
		sol::usertype<guild>guild;
		sol::usertype<resStrat>resStrat;
		sol::usertype<stackStruct>stackStruct;
		sol::usertype<building>building;
		sol::usertype<siegeS>siege;
		sol::usertype<edbEntry>edbEntry;
		sol::usertype<buildingLevel>buildingLevel;

	}types;
	*/
	void fillHashMaps();
	
	bool hashLoaded = false;

	std::unordered_map<std::string, int> factions = {
	};
	std::unordered_map<std::string, int> regions = {
	};
	std::unordered_map<std::string, int> settlements = {
	};
	std::unordered_map<std::string, int> religionIndex = {
	};
	std::unordered_map<int, const char*> religionNames = {
	};

	sol::function* onCampaignMapLoaded = nullptr;

	sol::function* onReadGameDbsAtStart = nullptr;
	sol::function* onGameInit = nullptr;
	sol::function* onUnloadCampaign = nullptr;
	sol::function* onAiTurn = nullptr;
	sol::function* onEndSiege = nullptr;
	sol::function* onStartSiege = nullptr;
	sol::function* onSelectWorldpkgdesc = nullptr;
	sol::function* onfortificationlevelS = nullptr;
	sol::function* onCalculateUnitValue = nullptr;
	sol::function* onChangeTurnNumFunc = nullptr;
	sol::function* onClickAtTile = nullptr;
	sol::function* onNewGameStart = nullptr;
	sol::function* onPluginLoad = nullptr;
	sol::function* onCreateSaveFile = nullptr;
	sol::function* onLoadSaveFile = nullptr;

	sol::function* drawLuaFunc = nullptr;
	sol::function* resetDXFunc = nullptr;
	sol::function* onLoadingFonts = nullptr;
	sol::function* initDXFunc = nullptr;
	void checkLuaFunc(sol::function** lRef);
	


private:

};



#endif
