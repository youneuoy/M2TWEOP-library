///
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"

#include "actionsStrat.h"
#include "casModelsDrawer.h"
#include "console.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include "settlementConversionLvlSetter.h"
#include "smallFuncs.h"
#include "stratModelsChange.h"
#include "tilesChange.h"
#include "unitActions.h"
#include "realGameTypes.h"
#include <windows.h>
#include "luaGetSetFuncs.h"
#include "factionHelpers.h"
#include "resourcesHelpers.h"
#include "gameHelpers.h"
#include "stackStructHelpers.h"
#include "unitHelpers.h"
#include "m2tweopHelpers.h"
#include "generalCharactericticsHelpers.h"
#include "technicalHelpers.h"
#include "eopEduHelpers.h"
#include "siegeHelpers.h"
#include "gameSTDUIHelpers.h"
#include "character.h"
#include "settlement.h"
#include "characterRecord.h"
#include "eopBuildings.h"
std::vector<std::string> luaP::logS;
std::vector<std::string> luaP::logCommands;


static int ourP(lua_State* L) {
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		std::string newS;
		const char* s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1)
		{
			newS += '\t';
			//fputs("\t", stdout);
		}
		newS += s;
		luaP::logS.push_back(newS);
		//fputs(s, stdout);
		lua_pop(L, 1);  /* pop result */
	}
	//luaP::logS += "\n";
	//fputs("\n", stdout);
	return 0;
}

void replaceAll2(std::string& s, const std::string& search, const std::string& replace) {
	for (size_t pos = 0; ; pos += replace.length())
	{
		pos = s.find(search, pos);
		if (pos == std::string::npos) break;

		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}


void luaP::runScriptS(std::string* script)
{
	const char* retS = nullptr;
	plugData::data.luaAll.logS.push_back("\n== Output ==");
	auto funcResult = luaState.script(*script);
	if (!funcResult.valid())
	{
		sol::error luaError = funcResult;
		plugData::data.luaAll.logS.push_back("\n== Error ==\n");
		luaP::logS.push_back(luaError.what());
	}
	return;
}

bool luaP::checkVar(const char* gName, int variable)
{
	sol::optional<int> scriptVar = luaState[gName];
	if (scriptVar)
	{
		return scriptVar.value() == variable;
	}

	return false;
}



sol::state* luaP::init(std::string& luaFilePath, std::string& modPath)
{



	struct
	{
		sol::table M2TWEOPTable;



		sol::table stratmapTable;
		//this inside stratmap table
		sol::table objectsTable;
		sol::table cameraTable;
		sol::table gameTable;


		sol::table gameUITable;
	}tables;

	struct
	{
		sol::usertype<uiElement>uiElement;
		sol::usertype<unit>unit;
		sol::usertype<capturedFactionInfo>capturedFactionInfo;
		sol::usertype<capturedUnit>capturedUnit;
		sol::usertype<capturedCharacter>capturedCharacter;
		sol::usertype<ancillary>ancillary;
		sol::usertype<traitContainer>traitContainerT;
		sol::usertype<eduEntry>eduEntry;
		sol::usertype<factionStruct>factionStruct;
		sol::usertype<factionStratMapDescrS>factionStratMapStruct;
		sol::usertype<watchTowerStruct>watchtowerStruct;
		sol::usertype<portBuildingStruct>portStruct;
		sol::usertype<portDockStrat>dockStruct;
		sol::usertype<resStrat>tradeResource;
		sol::usertype<stackStruct>stackStruct;
		sol::usertype<siegeS>siege;
		sol::usertype<buildingLevel>buildingLevel;
		sol::usertype<battleCameraStruct>battleCameraStruct;
		sol::usertype<settlementInfoScroll>settlementInfoScroll;
		sol::usertype<settlementTextStrings>settlementTextStrings;
		sol::usertype<uiString>uiString;
		sol::usertype<unitPositionData>unitPositionData;
		sol::usertype<factionEconomy>factionEconomy;
		sol::usertype<factionRanking>factionRanking;
		sol::usertype<holdRegionsWinCondition>holdRegionsWinCondition;
		sol::usertype<battleFactionCounter>battleFactionCounter;
		sol::usertype<eventTrigger> eventTrigger;
		sol::usertype<descrMountEntry> mountStruct;
		sol::usertype<projectile> projectileStruct;
		sol::usertype<aiFaction> aiFaction;
		sol::usertype<aiLongTermGoalDirector> aiLongTermGoalDirector;
		sol::usertype<aiPersonalityValues> aiPersonality;
		sol::usertype<aiGlobalStrategyDirector> aiGlobalStrategyDirector;
		sol::usertype<decisionValuesLTGD> decisionValuesLTGD;
		sol::usertype<ltgdFactionValues> aiFactionValues;
		sol::usertype<interFactionLTGD> interFactionLTGD;
		sol::usertype<trait> traitStruct;
		sol::usertype<traitEntry> traitEntry;
		sol::usertype<traitLevel> traitLevel;
		sol::usertype<traitEffect> traitEffect;
		sol::usertype<unitGroup> unitGroup;
	}types;
	luaState = {};
	luaPath = modPath + "\\youneuoy_Data\\plugins\\lua";
	luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::debug);

	std::string packagePS = "package.path = '";
	packagePS += modPath;
	packagePS += "\\youneuoy_Data\\plugins\\lua\\?.lua;'";

	packagePS += "..'";
	packagePS += modPath;
	packagePS += "\\youneuoy_Data\\plugins\\lua\\redist\\?.lua;'..package.path ;";

	packagePS += "package.cpath = '";
	packagePS += modPath;
	packagePS += "\\youneuoy_Data\\plugins\\lua\\redist\\?.dll;'..package.cpath ;";


	std::string f = "\\";
	std::string r = "/";
	replaceAll2(packagePS, f, r);

	luaState["print"] = &ourP;
	auto funcResult = luaState.script(packagePS.c_str());
	if (!funcResult.valid())
	{
		sol::error luaError = funcResult;
		MessageBoxA(NULL, luaError.what(), "Lua package error!", NULL);
		return nullptr;
	}

	sol::load_result fileRes = luaState.load_file(luaFilePath);
	if (!fileRes.valid()) { // This checks the syntax of your script, but does not execute it
		sol::error luaError = fileRes;
		MessageBoxA(NULL, luaError.what(), "Lua syntax error!", NULL);
		return nullptr;
	}
	sol::protected_function_result result1 = fileRes(); // this causes the script to execute
	if (!result1.valid()) {
		sol::error luaError = result1;
		MessageBoxA(NULL, luaError.what(), "Lua execution error!", NULL);
		return nullptr;
	}

	characterHelpers::addToLua(luaState);
	characterRecordHelpers::addToLua(luaState);
	settlementHelpers::addToLua(luaState);
	fortHelpers::addToLua(luaState);

	///M2TWEOP
	//@section m2tweopTable

	/***
	Basic M2TWEOP table

	@tfield getModPath getModPath
	@tfield toggleConsole toggleConsole
	@tfield reloadScript reloadScript
	@tfield restartLua restartLua
	@tfield toggleDeveloperMode toggleDeveloperMode
	@tfield saveGame saveGame
	@tfield getGameVersion getGameVersion
	@tfield setPerfectSpy setPerfectSpy
	@tfield getLocalFactionID getLocalFactionID
	@tfield getPluginPath  getPluginPath
	@tfield loadTexture loadTexture
	@tfield unloadTexture unloadTexture
	@tfield setAncillariesLimit setAncillariesLimit
	@tfield unlockGameConsoleCommands unlockGameConsoleCommands
	@tfield setMaxBgSize setMaxBgSize
	@tfield toggleUnitsBMapHighlight toggleUnitsBMapHighlight
	@tfield getBattleCamCoords getBattleCamCoords
	@tfield setReligionsLimit setReligionsLimit
	@tfield setEquipmentCosts setEquipmentCosts
	@tfield isTileFree isTileFree
	@tfield getGameTileCoordsWithCursor getGameTileCoordsWithCursor
	@tfield getTileRegionID getTileRegionID
	@tfield getTileVisibility getTileVisibility
	@tfield setGuildCooldown setGuildCooldown
	@tfield getRegionOwner getRegionOwner
	@tfield setEDUUnitsSize setEDUUnitsSize
	@tfield setBuildingChainLimit setBuildingChainLimit
	@tfield getReligionName getReligionName
	@tfield getReligionCount getReligionCount
	@tfield getCultureName getCultureName
	@tfield getClimateName getClimateName
	@tfield getSettlementInfoScroll getSettlementInfoScroll
	@tfield getReligion getReligion
	@tfield getClimateID getClimateID
	@tfield getCultureID getCultureID
	@tfield condition condition
	@tfield getOptions1 getOptions1
	@tfield getOptions2 getOptions2
	@tfield CalculateTileMovementCost CalculateTileMovementCost
	@tfield getCampaignDifficulty1 getCampaignDifficulty1
	@tfield getCampaignDifficulty2 getCampaignDifficulty2
	@tfield setConversionLvlFromCastle setConversionLvlFromCastle
	@tfield setConversionLvlFromCity setConversionLvlFromCity
	@tfield loadGame loadGame
	@tfield logGame logGame
	@tfield getCampaignDb getCampaignDb
	@tfield getCampaignDbExtra getCampaignDbExtra
	@table M2TWEOP
	*/



	tables.M2TWEOPTable = luaState.create_table("M2TWEOP");


	/***
	Function to return the path to the mod folder, starts from the drive not the game folder.


	@function M2TWEOP.getModPath
	@treturn string mod path

	@usage
	mPath=M2TWEOP.getModPath();
	print(mPath);
	*/

	tables.M2TWEOPTable.set_function("getModPath", &m2tweopHelpers::getModPath);
	/***
	Open/close the lua console.


	@function M2TWEOP.toggleConsole

	@usage
	    M2TWEOP.toggleConsole();
	*/

	tables.M2TWEOPTable.set_function("toggleConsole", &console::toggleConsole);
	/***
	Toggle developer mode.


	@function M2TWEOP.toggleDeveloperMode

	@usage
	    M2TWEOP.toggleDeveloperMode();
	*/

	tables.M2TWEOPTable.set_function("toggleDeveloperMode", &fastFuncts::toggleDeveloperMode);
	/***
	Reload the lua script (without restarting the plugin itself, onPluginLoad doesn't fire).


	@function M2TWEOP.reloadScript

	@usage
	    M2TWEOP.reloadScript();
	*/

	tables.M2TWEOPTable.set_function("reloadScript", &console::reloadScript);
	/***
	Restart the lua plugin (onPluginLoad fires).


	@function M2TWEOP.restartLua

	@usage
	    M2TWEOP.restartLua();
	*/

	tables.M2TWEOPTable.set_function("restartLua", &console::restartLua);
	/***
	Save the game.
	@function M2TWEOP.saveGame
	@tparam string path (start from mods)
	@usage
	M2TWEOP.saveGame("mods/bare_geomod/saves/newsave.sav");
	*/
	tables.M2TWEOPTable.set_function("saveGame", &smallFuncs::saveGame);
	/***
	Function to get the game version.
	@function M2TWEOP.getGameVersion
	@treturn int gamever (1 = disk 2 = steam)
	@usage
	M2TWEOP.getGameVersion();
	*/
	tables.M2TWEOPTable.set_function("getGameVersion", &smallFuncs::getGameVersion);
	/***
	Set perfect spy.
	@function M2TWEOP.setPerfectSpy
	@tparam bool set
	@usage
		M2TWEOP.setPerfectSpy(true);
	*/
	tables.M2TWEOPTable.set_function("setPerfectSpy", &m2tweopHelpers::setPerfectSpy);
	/***
	Get local faction ID.
	@function M2TWEOP.getLocalFactionID
	@treturn int localFaction
	@usage
		local localFaction = M2TWEOP.getLocalFactionID();
	*/
	tables.M2TWEOPTable.set_function("getLocalFactionID", &m2tweopHelpers::getLocalFactionID);
	/***
	Function to return the path to the plugin (location of your LUA files).
	@function M2TWEOP.getPluginPath
	@treturn string plugin path
	@usage
	plPath=M2TWEOP.getPluginPath();
	print(plPath);
	*/
	tables.M2TWEOPTable.set_function("getPluginPath", &m2tweopHelpers::getLuaPath);
	/***
	Log something to the game log.
	@function M2TWEOP.logGame
	@tparam string message
	@usage
	 M2TWEOP.logGame("log message");
	*/
	tables.M2TWEOPTable.set_function("logGame", &unitActions::logStringGame);
	/***
	Load a savegame.
	@function M2TWEOP.loadGame
	@tparam string path starting from mods
	@usage
	 M2TWEOP.loadGame("mods/bare_geomod/saves/test.sav");
	*/
	tables.M2TWEOPTable.set_function("loadGame", &fastFuncts::loadSaveGame);

	/***
	Load d3d texture
	@function M2TWEOP.loadTexture
	@tparam string path full path to texture
	@treturn int x size of the image
	@treturn int y size of the image
	@treturn int id of the image
	@usage
	-- This function supports the following file formats: .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga.
	-- Recommended to use .dds for best performance
	-- Note: Doing image scaling and format conversion at load time can be slow. Store images in the format and resolution they will be used.
	-- More info: https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxcreatetexturefromfileex
	local testImage = { x = 0, y = 0, img = nil};
	testImage.x, testImage.y, testImage.img=M2TWEOP.loadTexture(M2TWEOP.getModPath().."/youneuoy_textures/test.dds");
	*/

	tables.M2TWEOPTable.set_function("loadTexture", &m2tweopHelpers::loadTextureToGame);
	/***
	Unload d3d texture
	@function M2TWEOP.unloadTexture
	@tparam int id of the image
	@usage
	local testImage = { x = 0, y = 0, img = nil};
	testImage.x, testImage.y, testImage.img=M2TWEOP.loadTexture(M2TWEOP.getModPath().."/youneuoy_textures/test.dds");
	M2TWEOP.unloadTexture(testImage.img);
	*/
	tables.M2TWEOPTable.set_function("unloadTexture", &m2tweopHelpers::unloadTextureFromGame);
	/***
	Sets the limit of ancillaries per character on the stratmap.
	@function M2TWEOP.setAncillariesLimit
	@tparam int newLimit default: 8, maximum: 127
	@usage
	M2TWEOP.setAncillariesLimit(15);
	*/
	tables.M2TWEOPTable.set_function("setAncillariesLimit", &smallFuncs::setAncLimit);
	/***
	Unlocks all console commands, also allows the use of the 'control' command to change factions in singleplayer campaigns.
	@function M2TWEOP.unlockGameConsoleCommands
	@usage
	M2TWEOP.unlockGameConsoleCommands();
	*/
	tables.M2TWEOPTable.set_function("unlockGameConsoleCommands", &smallFuncs::unlockConsoleCommands);
	/***
	Sets the maximum amount of soldiers a general's bodyguard unit can replenish to. The value is multiplied by the unit size modifiers (e.g Huge = 2.5 multiplier)
	@function M2TWEOP.setMaxBgSize
	@tparam int newSize
	@usage
	M2TWEOP.setMaxBgSize(100) -- On huge unit size, 100*2.5 = 250 max bodyguard size
	M2TWEOP.setMaxBgSize(150) -- On huge unit size, 150*2.5 = 300 max bodyguard size
	M2TWEOP.setMaxBgSize(50)  -- On huge unit size, 50*2.5 = 125 max bodyguard size
	*/
	tables.M2TWEOPTable.set_function("setMaxBgSize", &smallFuncs::setMaxBgSize);

	/***
	Sets the new maximum soldier count.
	@function M2TWEOP.setEDUUnitsSize
	@tparam int minSize maximum: 300
	@tparam int maxSize maximum: 300
	@usage
	M2TWEOP.setEDUUnitsSize(1,300);
	*/
	tables.M2TWEOPTable.set_function("setEDUUnitsSize", &smallFuncs::setEDUUnitsSize);

	/***
	Gets a struct containing color information about the settlement info scroll.
	@function M2TWEOP.getSettlementInfoScroll
	@treturn settlementInfoScroll infoScroll
	@usage
	// Make the words "Income" on the Settlement Info scroll red
	settlementInfoScroll = M2TWEOP.getSettlementInfoScroll();
	settlementInfoScroll:getUIStrings.incomeString.red = 255
	settlementInfoScroll:getUIStrings.incomeString.green = 0
	settlementInfoScroll:getUIStrings.incomeString.blue = 0
	*/
	tables.M2TWEOPTable.set_function("getSettlementInfoScroll", &m2tweopHelpers::getSettlementInfoScroll);

	/***
	* Sets the new maximum amount of building levels within a chain.
	* @function M2TWEOP.setBuildingChainLimit
	* @tparam int limit default: 9, maximum: 57
	* @usage
	* M2TWEOP.setBuildingChainLimit(12);
	*/

	tables.M2TWEOPTable.set_function("setBuildingChainLimit", &smallFuncs::setBuildingChainLimit);

	/***
	* Sets which level of castle converts to which level of city.
	* @function M2TWEOP.setConversionLvlFromCastle
	* @tparam int castleLvl 0-5
	* @tparam int convertToLvl 0-6, 6 means conversion from that level is disabled
	* @usage
	* M2TWEOP.setConversionLvlFromCastle(3,3)
	*/
	tables.M2TWEOPTable.set_function("setConversionLvlFromCastle", &settlementConversionLvlSetter::setConversionLvlFromCastle);

	/***
	* Sets which level of city converts to which level of castle.
	* @function M2TWEOP.setConversionLvlFromCity
	* @tparam int cityLvl 0-5
	* @tparam int convertToLvl 0-6, 6 means conversion from that level is disabled
	* @usage
	* M2TWEOP.setConversionLvlFromCity(3,3)
	*/
	tables.M2TWEOPTable.set_function("setConversionLvlFromCity", &settlementConversionLvlSetter::setConversionLvlFromCity);

	/***
	* Sets the minimum number of turns until the next guild offer after a rejection by the player.
	* @function M2TWEOP.setGuildCooldown
	* @tparam int turns default: 10
	* @usage
	* M2TWEOP.setGuildCooldown(5)
	*/
	tables.M2TWEOPTable.set_function("setGuildCooldown", &smallFuncs::setGuildCooldown);


	/***
	Toggle the highlighting of units on the tactical map.
	@function M2TWEOP.toggleUnitsBMapHighlight
	@usage
	M2TWEOP.toggleUnitsBMapHighlight();
	*/
	tables.M2TWEOPTable.set_function("toggleUnitsBMapHighlight", &smallFuncs::toggleUnitHighlight);

	/***
	Get the current x, y and z coords of the battlemap camera
	@function M2TWEOP.getBattleCamCoords
	@treturn battleCameraStruct Camera struct
	@usage
	local cameraCoords = M2TWEOP.getBattleCamCoords();
	-- Zoom out the camera beyond it's normal range
	cameraCoords.zCoord = 500;
	*/
	tables.M2TWEOPTable.set_function("getBattleCamCoords", &smallFuncs::getBattleCamCoords);

	/***
	Set the maximum number of religions in the mod (per descr\_religions.txt). Do not use religions > 10 in CombatVsReligion attributes!
	@function M2TWEOP.setReligionsLimit
	@tparam int newLimit maximum: 127
	@usage
	M2TWEOP.setReligionsLimit(25);
	*/
	tables.M2TWEOPTable.set_function("setReligionsLimit", &smallFuncs::setReligionsLimit);

	/***
	Set the siege points required to make different siege equipment.
	@function M2TWEOP.setEquipmentCosts
	@tparam int equipmentType 0 = ram, 1 = ladder, 2 = siege tower
	@tparam int newCost
	@usage
		M2TWEOP.setEquipmentCosts(0, 25);
	*/
	tables.M2TWEOPTable.set_function("setEquipmentCosts", &m2tweopHelpers::setEquipmentCosts);

	/***
	Checks if a tile is free.
	@function M2TWEOP.isTileFree
	@tparam int X coordinate of the tile.
	@tparam int Y coordinate of the tile.
	@return boolean isFree
	@usage
	M2TWEOP.isTileFree(55,25);
	*/
	tables.M2TWEOPTable.set_function("isTileFree", &m2tweopHelpers::isTileFree);
	/***
	Get the selected tile coords.
	@function M2TWEOP.getGameTileCoordsWithCursor
	@treturn int x
	@treturn int y
	@usage
	local x,y=M2TWEOP.getGameTileCoordsWithCursor();
	*/
	tables.M2TWEOPTable.set_function("getGameTileCoordsWithCursor", &m2tweopHelpers::getGameTileCoordsWithCursor);
	/***
	Get the RegionID of a tile.
	@function M2TWEOP.getTileRegionID
	@tparam int x
	@tparam int y
	@treturn int regionID
	@usage
	local regionID=M2TWEOP.getTileRegionID(55,25);
	*/
	tables.M2TWEOPTable.set_function("getTileRegionID", &fastFuncts::getTileRegionID);
	/***
	Get a specific tile's visibility according to faction (i.e can a faction see a tile) Note: Once the tile has been seen by a faction, it will always return true. e.g If you have spotted a settlement but it is now outside of the fog of war, it will still be classed as visible. 
	@function M2TWEOP.getTileVisibility
	@tparam factionStruct faction Faction to check
	@tparam int xCoord x coord of the tile
	@tparam int yCoord y coord of the tile
	@treturn bool isVisible true = visible, false = not visible
	@usage
	local faction = CAMPAIGN:getFaction("england")
	local isVisible = M2TWEOP.getTileVisibility(faction, xCoord, yCoord)
	if isVisible == true then 
		print("Tile is visible to faction "..faction.name)
	else
		print("Tile is not visible to faction "..faction.name)
	end
	*/
	tables.M2TWEOPTable.set_function("getTileVisibility", &m2tweopHelpers::getTileVisibility);
	/***
	Get the owner of a region by RegionID.
	@function M2TWEOP.getRegionOwner
	@tparam int regionID
	@treturn factionStruct owner
	@usage
	local regionID = M2TWEOP.getTileRegionID(55,25);
	local ownerFac = M2TWEOP.getRegionOwner(regionID);
	*/
	tables.M2TWEOPTable.set_function("getRegionOwner", &m2tweopHelpers::getRegionOwner);
	/***
	Get religion name by index.
	@function M2TWEOP.getReligionName
	@tparam int index
	@treturn string name
	@usage
	local religionName = M2TWEOP.getReligionName(1);
	*/
	tables.M2TWEOPTable.set_function("getReligionName", &gameHelpers::getReligionName);
	/***
	Get the amount of religions.
	@function M2TWEOP.getReligionCount
	@treturn int religionCount
	@usage
	local religionCount = M2TWEOP.getReligionCount();
	*/
	tables.M2TWEOPTable.set_function("getReligionCount", &gameHelpers::getReligionCount);
	/***
	Get culture name by index.
	@function M2TWEOP.getCultureName
	@tparam int index
	@treturn string name
	@usage
	local name = M2TWEOP.getCultureName(1);
	*/
	tables.M2TWEOPTable.set_function("getCultureName", &gameHelpers::getCultureName);
	/***
	Get climate name by index.
	@function M2TWEOP.getClimateName
	@tparam int index
	@treturn string name
	@usage
	local name = M2TWEOP.getClimateName(1);
	*/
	tables.M2TWEOPTable.set_function("getClimateName", &gameHelpers::getClimateName);
	/***
	Get religion ID by name.
	@function M2TWEOP.getReligion
	@tparam string name
	@treturn int index
	@usage
	local religion = M2TWEOP.getReligion("catholic");
	*/
	tables.M2TWEOPTable.set_function("getReligion", &gameHelpers::getReligionN);
	/***
	Get culture ID by name.
	@function M2TWEOP.getCultureID
	@tparam string name
	@treturn int index
	@usage
	local cultureID = M2TWEOP.getCultureID("southern_european");
	*/
	tables.M2TWEOPTable.set_function("getCultureID", &gameHelpers::getCultureN);
	/***
	Get climate ID by name.
	@function M2TWEOP.getClimateID
	@tparam string name
	@treturn int index
	@usage
	local climateID = M2TWEOP.getClimateID("sandy_desert");
	*/
	tables.M2TWEOPTable.set_function("getClimateID", &gameHelpers::getClimateN);
	/***
	Get some game options.
	@function M2TWEOP.getOptions1
	@treturn options1 options
	@usage
	local options = M2TWEOP.getOptions1();
	*/
	tables.M2TWEOPTable.set_function("getOptions1", &m2tweopHelpers::getOptions1);
	/***
	Get some game options.
	@function M2TWEOP.getOptions2
	@treturn options2 options
	@usage
	local options = M2TWEOP.getOptions2();
	*/
	tables.M2TWEOPTable.set_function("getOptions2", &m2tweopHelpers::getOptions2);
	/***
	Get the campaign difficulty modifiers.
	@function M2TWEOP.getCampaignDifficulty1
	@treturn campaignDifficulty1 options
	@usage
	local modifiers = M2TWEOP.getCampaignDifficulty1();
	*/
	tables.M2TWEOPTable.set_function("getCampaignDifficulty1", &m2tweopHelpers::getCampaignDifficulty1);
	/***
	Get the campaign difficulty modifiers.
	@function M2TWEOP.getCampaignDifficulty2
	@treturn campaignDifficulty2 options
	@usage
	local modifiers = M2TWEOP.getCampaignDifficulty2();
	*/
	tables.M2TWEOPTable.set_function("getCampaignDifficulty2", &m2tweopHelpers::getCampaignDifficulty2);
	/***
	Get the campaign options.
	@function M2TWEOP.getCampaignDb
	@treturn campaignDb options
	@usage
	local options = M2TWEOP.getCampaignDb();
	*/
	tables.M2TWEOPTable.set_function("getCampaignDb", &gameHelpers::getCampaignDb);
	/***
	Get the campaign options.
	@function M2TWEOP.getCampaignDbExtra
	@treturn campaignDbExtra options
	@usage
	local options = M2TWEOP.getCampaignDbExtra();
	*/
	tables.M2TWEOPTable.set_function("getCampaignDbExtra", &gameHelpers::getCampaignDbExtra);
	/***
	Check game condition.
	@function M2TWEOP.condition
	@tparam string condition
	@tparam eventTrigger|nil eventData
	@treturn bool isTrue
	@usage
	if M2TWEOP.condition("InEnemyLands", eventData) then
		--do stuff
	end
	*/
	tables.M2TWEOPTable.set_function("condition", &gameHelpers::condition);
	/***
	Calculate movement point cost between two adjacent tiles.
	@function M2TWEOP.CalculateTileMovementCost
	@tparam int originX
	@tparam int originY
	@tparam int targetX only adjacent tiles! Does not calculate paths just the cost of moving from one tile to another.
	@tparam int targetY only adjacent tiles! Does not calculate paths just the cost of moving from one tile to another.
	@treturn float moveCost
	@usage
	local moveCost = M2TWEOP.CalculateTileMovementCost(153, 245, 154, 245);
	*/
	tables.M2TWEOPTable.set_function("CalculateTileMovementCost", &fastFuncts::GetMovepointsForReachNearTile);


	/// BattleCamera
	//@section gameSTDUITable

	/***
	Get information about the camera in a battle
	@tfield float xCoord 
	@tfield float yCoord 
	@tfield float zCoord 
	@table battleCameraStruct
	*/
	types.battleCameraStruct = luaState.new_usertype<battleCameraStruct>("battleCameraStruct");
	types.battleCameraStruct.set("xCoord", &battleCameraStruct::xCoord);
	types.battleCameraStruct.set("yCoord", &battleCameraStruct::yCoord);
	types.battleCameraStruct.set("zCoord", &battleCameraStruct::zCoord);


	/// GameUI
	//@section gameSTDUITable

	/***
	Basic gameSTDUI table

	@tfield getUiElement getUiElement

	@table gameSTDUI
	*/



	tables.gameUITable = luaState.create_table("gameSTDUI");
	/***
	Get a game UI element, element must be opened.
	@function gameSTDUI.getUiElement
	@tparam string elementName
	@treturn uiElementStruct element
	@usage
	function analyzeScroll(scrollName)
		local scroll, uiList = gameSTDUI.getUiElement(scrollName), "UI elements of scroll: "..scrollName
		if scroll ~= nil then
			for i = 0, scroll.subElementsNum - 1, 1 do
				uiList = uiList.."\n\t"..i.." "..scroll:getSubElement(i).elementName
			end
			print(uiList)
		end
	end
	*/

	tables.gameUITable.set_function("getUiElement", &fastFuncts::getUiElement);

	/// UIElement
	//@section uiElementStructTable

	/***
	Basic uiElementStruct table

	@tfield string elementName
	@tfield int xSize
	@tfield int ySize
	@tfield int xPos
	@tfield int yPos
	@tfield int subElementsNum
	@tfield getSubElement getSubElement
	@tfield execute execute

	@table uiElementStruct
	*/
	types.uiElement = luaState.new_usertype<uiElement>("uiElementStruct");
	types.uiElement.set("elementName", sol::property(
		&gameSTDUIHelpers::getUIElementName));
	types.uiElement.set("xSize", &uiElement::xSize);
	types.uiElement.set("ySize", &uiElement::ySize);
	types.uiElement.set("xPos", &uiElement::xPos);
	types.uiElement.set("yPos", &uiElement::yPos);
	types.uiElement.set("subElementsNum", &uiElement::subElementsNum);
	/***
	Get a subelement of an UI element using the index.
	@function uiElementStruct:getSubElement
	@tparam int index Starts from 0.
	@treturn uiElementStruct subelement
	@usage
	local financeScroll = gameSTDUI.getUiElement("finance_scroll");
	local subElement1 = financeScroll:getSubElement(5);
	*/
	types.uiElement.set("getSubElement", &gameSTDUIHelpers::getSubElement);
	/***
	execute standard game UI element, use only for buttons
	@function uiElementStruct:execute
	@usage
	local financeScroll = gameSTDUI.getUiElement("finance_scroll");
	--at index 5 we have faction_listviews_button
	local subElement1 = financeScroll:getSubElement(5);
	subElement1:execute();
	*/
	types.uiElement.set("execute", &fastFuncts::useUiElement);


	///StratmapObjects
	//@section objectsTable


	/***
	Basic stratmap.objects table

	@tfield addModelToGame addModelToGame
	@tfield addCharacterCas addCharacterCas
	@tfield setModel setModel
	@tfield replaceTile replaceTile
	@tfield startDrawModelAt startDrawModelAt
	@tfield stopDrawModel stopDrawModel

	@table stratmap.objects
	*/
	tables.objectsTable = luaState.create_table();

	/***
	Start drawing .cas campaign strategy model with a unique ID in some coords. Can be used at any time.
	@function stratmap.objects.startDrawModelAt
	@tparam int modelId Unique ID
	@tparam int x
	@tparam int y
	@tparam int sizeMultiplier 1 is value with what it draw as same size as game objects
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS", 1);
	stratmap.objects.startDrawModelAt(1, 50, 150, 2.25);
	*/
	tables.objectsTable.set_function("startDrawModelAt", &casModelsDrawer::addCasModelToDrawList);
	/***
	Stop drawing .cas campaign strategy model with a unique ID. Can be used at any time.
	@function stratmap.objects.stopDrawModel
	@tparam int modelId Unique ID
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS", 1);
	stratmap.objects.startDrawModelAt(1, 50, 150, 2.25);
	stratmap.objects.stopDrawModel(1);
	*/
	tables.objectsTable.set_function("stopDrawModel", &casModelsDrawer::removeCasModelFromDrawList);
	/***
	Add a new .cas campaign strategy model to the game with a unique ID. This should be called during onPluginLoad()
	@function stratmap.objects.addModelToGame
	@tparam string path Relative path from the modfolder (starting with "data/").
	@tparam int modelId  Unique ID to use the model later.
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS",1);
	*/
	tables.objectsTable.set_function("addModelToGame", &stratModelsChange::addModelToGame);
	/***
	Add a new .cas character strategy model to the game with a unique name. Only add it after loading to campaign map!
	@function stratmap.objects.addCharacterCas
	@tparam string skeleton name of skeleton used.
	@tparam string caspath Relative path from the mods folder (starting with "mods/").
	@tparam string shadowcaspath Relative path from the mods folder (starting with "mods/").
	@tparam string typename Name of the new model used to assign.
	@tparam string texturepath Relative path from the mods folder (starting with "mods/").
	@tparam float scale
	@usage
	stratmap.objects.addCharacterCas(
		"strat_named_with_army",
		"mods/Bare_Geomod/data/models_strat/islamic_general2.cas",
		"mods/Bare_Geomod/data/models_strat/shadow_sword2.cas",
		"islamic_general2",
		"mods/Bare_Geomod/data/models_strat/textures/islamic_general_turks.tga",
		0.7
	);
	*/
	tables.objectsTable.set_function("addCharacterCas", &stratModelsChange::addCharacterCas);
	/***
	Set the strategy model for object at specified coordinates, works only for supported object types
	@function stratmap.objects.setModel
	@tparam int xCoord
	@tparam int yCoord
	@tparam int modelId used for: watchtower, resource, settlement, fort, port
	@tparam int modelId2 used for: fort wall (use fort coords), dock (use port coords)
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS",1);
	stratmap.objects.setModel(288,257,1,1);
	*/
	tables.objectsTable.set_function("setModel", sol::overload(&stratModelsChange::setModel,
		&stratModelsChange::setModelOneVar));
	/***
	Replace a custom tile. Change the custom battlefield on the specified coordinates.
	@function stratmap.objects.replaceTile
	@tparam string label  Identifier.
	@tparam int xCoord  X coordinate of tile.
	@tparam int yCoord  Y coordinate of tile.
	@tparam string filename  Just the name, not full path (.wfc)
	@tparam string weather Weather on the battle map.
	@tparam string dayTime Time of day.
	@usage
	stratmap.objects.replaceTile("Helms-Deep_Province",167,158,"hornburg_amb.wfc","clear","midday");
	*/
	tables.objectsTable.set_function("replaceTile", &tilesChange::replaceTile);

	///Camera
	//@section cameraTable


	/***
	Basic stratmap.camera table

	@tfield move move
	@tfield jump  jump
	@tfield zoom  zoom

	@table stratmap.camera
	*/
	tables.cameraTable = luaState.create_table();
	/***
	Slowly move the camera to the specified tile.
	@function stratmap.camera.move
	@tparam int xCoord
	@tparam int yCoord
	@usage
	stratmap.camera.move(1,2);
	*/
	tables.cameraTable.set_function("move", &fastFuncts::moveStratCameraSlow);
	/***
	Quickly move the camera to the specified tile.
	@function stratmap.camera.jump
	@tparam int xCoord
	@tparam int yCoord
	@usage
	stratmap.camera.jump(1,2);
	*/
	tables.cameraTable.set_function("jump", &fastFuncts::moveStratCameraFast);
	/***
	Set the zoom level of the camera.
	@function stratmap.camera.zoom
	@tparam float distance
	@usage
	stratmap.camera.zoom(0.12);
	*/
	tables.cameraTable.set_function("zoom", &fastFuncts::zoomStratCamera);

	///Game
	//@section gameTable


	/***
	Basic stratmap.game table

	@tfield getFactionsCount getFactionsCount
	@tfield getFaction getFaction
	@tfield createCharacterByString createCharacterByString
	@tfield createArmy createArmy
	@tfield spawnArmy spawnArmy
	@tfield createArmyInSettlement createArmyInSettlement
	@tfield getScriptCounter getScriptCounter
	@tfield setScriptCounter setScriptCounter
	@tfield historicEvent historicEvent
	@tfield scriptCommand scriptCommand
	@tfield callConsole callConsole
	@tfield getGuild getGuild

	@table stratmap.game
*/
	tables.gameTable = luaState.create_table();
	/***
	Execute a Medieval II console command.
	@function stratmap.game.callConsole
	@tparam string command
	@tparam string args
	@treturn string error Note: string can be empty but not nil
	@usage
	-- Creating units, adding money
	function onCharacterSelected(eventData)
		local selectedChar = eventData.character
		local err = stratmap.game.callConsole("add_money", "2321")
		local err2 = stratmap.game.callConsole("create_unit", "testcharacter 'Cool Unit' 4 1 1 1")
		print(err)
		print(err2)
	end
	*/
	tables.gameTable.set_function("callConsole", &gameHelpers::callConsole);
	/***
	Get the amount of factions. Just returns campaignStruct.numberOfFactions field.
	@function stratmap.game.getFactionsCount
	@treturn int facNumber Amount of factions
	@usage
	local facNum=stratmap.game.getFactionsCount();
	*/
	tables.gameTable.set_function("getFactionsCount", &gameHelpers::getFactionsCount);
	/***
	Get a faction by the turn order. Same thing as doing campaignStruct.factionsSortedByDescrStrat[index + 1].
	@function stratmap.game.getFaction
	@tparam int Index of the faction.
	@treturn factionStruct faction
	@usage
	faction=stratmap.game.getFaction(2);
	*/
	tables.gameTable.set_function("getFaction", &gameHelpers::getFaction);
	/***
	Get a guild by the index.
	@function stratmap.game.getGuild
	@tparam int index
	@treturn guild guild
	@usage
	ourGuild=stratmap.game.getGuild(1);
	*/
	tables.gameTable.set_function("getGuild", &eopBuildings::getGuild);

	/***
	Create a new character at the specified coordinates. If you are not spawning an agent it is preferred to use spawnArmy instead.
	@function stratmap.game.createCharacterByString
	@tparam string type Character type, for example "named character".
	@tparam factionStruct Faction the new character belongs to.
	@tparam int age The character's age
	@tparam string name The short name of the character.
	@tparam string name2 The full name of the character.
	@tparam int subFaction Set to 31 to disable.
	@tparam string portrait_custom cannot be nil Name of the folder inside 'data/ui/custom_portraits folder. Can not be nil!
	@tparam int xCoord X coordinate of the new character
	@tparam int yCoord Y coordinate of the new character
	@treturn character newCharacter Returns a character class, not a named character class!
	@usage
	newCharacter=stratmap.game.createCharacterByString("named character",CAMPAIGN:getFaction("england"),18,"Name1","Name2",31,"custom_portrait_name",character.character.xCoord+5,character.character.yCoord);
	*/
	tables.gameTable.set_function("createCharacterByString", &characterHelpers::createCharacter);

	/***
	Create a new army at the specified coordinates. Works similarly to the script command spawn_army. You can respawn off-map characters using it. You can not re-use labels!
	@function stratmap.game.spawnArmy
	@tparam factionStruct Faction the new character belongs to.
	@tparam string name The short name of the character. Use random_name to pick a random name.
	@tparam string name2 The full name of the character.
	@tparam int type characterType.named_character or characterType.general or characterType.admiral.
	@tparam string label label of the character, has to be unique!. Can be nil.
	@tparam string portrait Name of the folder inside 'data/ui/custom_portraits folder.
	@tparam int x X coordinate of the new character
	@tparam int y Y coordinate of the new character
	@tparam int age The character's age
	@tparam bool family should character be auto adopted?
	@tparam int subFaction Set to 31 to disable.
	@tparam int unitIndex Index of the unit in the unit list. Can be EOP or normal.
	@tparam int exp
	@tparam int wpn
	@tparam int armour
	@treturn stackStruct newArmy
	@usage
	local army = stratmap.game.spawnArmy(
	CAMPAIGN:getFaction("england"),
	"Rufus",
	"",
	characterType.named_character,
	"rufus_1",
	"",
	106, 149,
	18, false, 31,
	M2TWEOPDU.getEduIndexByType("Peasants"), 3, 0, 0
    )
	*/
	tables.gameTable.set_function("spawnArmy", &fastFuncts::spawnArmy);
	
	/***
	Legacy code to spawn an army for a created character. Use spawnArmy instead!
	@function stratmap.game.createArmy
	@tparam character ourGeneral Character class, not named character class!
	@treturn stackStruct army
	@usage
	army=stratmap.game.createArmy(gen);
	*/
	tables.gameTable.set_function("createArmy", &fastFuncts::createArmy);
	/***
	Create an army in a settlement (don't need a character). Used to add units to an empty settlement.
	@function stratmap.game.createArmyInSettlement
	@tparam settlementStruct settlement
	@treturn stackStruct army
	@usage
	army=stratmap.game.createArmyInSettlement(STRAT_MAP:getSettlement("London"));
	*/
	tables.gameTable.set_function("createArmyInSettlement", &fastFuncts::createArmyInSettlement);
	/***
	Get a script counter value, works for counters and for event\_counters
	@function stratmap.game.getScriptCounter
	@tparam string counterName The name of the counter
	@treturn bool isExist Returns true if the counter exists i.e it has been used at least once in any way in the campaign\_script
	@treturn int counterValue Returns the value of the counter
	@usage
	isExist, counterValue = stratmap.game.getScriptCounter("SomeCounter")
	*/
	tables.gameTable.set_function("getScriptCounter", &gameHelpers::getScriptCounter);
	/***
	Set an event\_counter value, does not work for counters, only event\_counters.
	@function stratmap.game.setScriptCounter
	@tparam string counterName
	@tparam int value
	@usage
	stratmap.game.setScriptCounter("SomeCounter", 25)
	*/
	tables.gameTable.set_function("setScriptCounter", &smallFuncs::setScriptCounter);
	/***
	Fire a game event message. Picture needs to be provided in the ui folders as default.
	@function stratmap.game.historicEvent
	@tparam string eventName
	@tparam string title
	@tparam string body
	@usage
	stratmap.game.historicEvent("my_event", "my title", "my description")
	*/
	tables.gameTable.set_function("historicEvent", &smallFuncs::historicEvent);

	/***
	Fire any script command available from the game. It is always just 2 parameters in the function, the command name and all the arguments as 1 string in the second parameter.
	Do not use inc_counter, set_counter, declare_counter! they crash!
	@function stratmap.game.scriptCommand
	@tparam string command
	@tparam string args
	@usage
	stratmap.game.scriptCommand("give_everything_to_faction", "france england false")
	stratmap.game.scriptCommand("send_character_off_map", "Rufus")

	-- Multiline commands and using variables in command strings
	local facName="hre"
	stratmap.game.scriptCommand("set_faction_banner", string.format([[

		faction england

		banner %s

	end_set_faction_banner]], facName))
	*/
	tables.gameTable.set_function("scriptCommand", &gameHelpers::scriptCommand);
	///Stratmap
	//@section stratmapTable

	/***
	Basic stratmap table

	@tfield stratmap.objects objects
	@tfield stratmap.camera camera
	@tfield stratmap.game game


	@table stratmap
	*/
	tables.stratmapTable = luaState.create_table("stratmap");


	tables.stratmapTable.set("objects", tables.objectsTable);
	tables.stratmapTable.set("camera", tables.cameraTable);
	tables.stratmapTable.set("game", tables.gameTable);

	///Unit
	//@section unitTable

	/***
	Unit table

	@tfield eduEntry eduEntry
	@tfield float movePoints
	@tfield int aiActiveSet  0 means inactive, 1 means active, 2 means labelled unit (for Battle)
	@tfield int soldierCountStratMap soldiers count. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int exp soldiers expierence. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int armourLVL soldiers armour. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int weaponLVL soldiers weapon. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int soldierCountStratMapMax Read only
	@tfield int soldierCountBattleMap Read only
	@tfield int moraleLevel use moraleStatus enum
	@tfield int isCloseFormation
	@tfield int ID
	@tfield int nearbyFriendlyUnitsNum
	@tfield int nearbyEnemyUnitsNum
	@tfield int missileRange
	@tfield projectileStruct missile
	@tfield int weaponType
	@tfield int techType
	@tfield int armourInBattle
	@tfield int attackInBattle
	@tfield settlementStruct retrainingSettlement
	@tfield unitGroup unitGroup
	@tfield int fatigue (battle)
	@tfield float angle (battle)
	@tfield int maxAmmo (battle)
	@tfield int currentAmmo (battle)
	@tfield float battlePosZ (battle)
	@tfield float battlePosX (battle)
	@tfield float battlePosY (battle)
	@tfield character character
	@tfield unitPositionData unitPositionData
	@tfield stackStruct army
	@tfield int siegeEngineNum (only infantry and artillery units!)
	@tfield kill kill
	@tfield setParams setParams change soldierCountStratMap, exp, armourLVL, weaponLVL at one time.
	@tfield hasAttribute hasAttribute Check if unit has edu attribute.
	@tfield string alias
	@tfield hasBattleProperty hasBattleProperty
	@tfield setBattleProperty setBattleProperty
	@tfield getActionStatus getActionStatus
	@tfield isMovingFastSet isMovingFastSet
	@tfield setMovingFastSet setMovingFastSet
	@tfield isOnWalls isOnWalls
	@tfield int supplies
	@tfield isEngaged isEngaged
	@tfield isUnderFire isUnderFire
	@tfield getMountClass getMountClass
	@tfield moveToPosition moveToPosition
	@tfield immediatePlace immediatePlace
	@tfield getFormation getFormation
	@tfield attackClosestUnit attackClosestUnit
	@tfield attackUnit attackUnit
	@tfield deployStakes deployStakes
	@tfield changeFormation changeFormation
	@tfield halt halt
	@tfield moveToOrientation moveToOrientation
	@tfield moveRelative moveRelative
	@tfield moveToMissileRange moveToMissileRange
	@tfield turn turn
	@tfield taunt taunt
	@tfield useSpecialAbility useSpecialAbility
	@tfield getSiegeEngine getSiegeEngine
	@tfield attackBuilding attackBuilding
	@tfield collectEngine collectEngine
	@tfield getNearbyFriendlyUnit getNearbyFriendlyUnit
	@tfield getNearbyEnemyUnit getNearbyEnemyUnit
	@tfield releaseUnit releaseUnit

	@table unit
	*/
	types.unit = luaState.new_usertype<unit>("unit");
	types.unit.set("eduEntry", &unit::eduEntry);
	types.unit.set("aiActiveSet", &unit::aiActiveSet);
	types.unit.set("movePoints", sol::property(&unitHelpers::getMovepoints, &fastFuncts::setUnitMovepoints));
	types.unit.set("soldierCountStratMap", sol::property(&unitHelpers::getsoldierCountStratMap, &fastFuncts::setSoldiersCount));
	types.unit.set("exp", sol::property(&unitHelpers::getExp, &unitHelpers::setExp));
	types.unit.set("armourLVL", sol::property(&unitHelpers::getarmourLVL, &unitHelpers::setarmourLVL));
	types.unit.set("weaponLVL", sol::property(&unitHelpers::getweaponLVL, &unitHelpers::setweaponLVL));
	types.unit.set("soldierCountStratMapMax", sol::property(&unitHelpers::getMaxSoldiersCount));
	types.unit.set("soldierCountBattleMap", &unit::SoldierCountBattlemap);
	types.unit.set("unitPositionData", &unit::unitPositionData);
	types.unit.set("character", &unit::general);
	types.unit.set("isCloseFormation", &unit::isCloseFormation);
	types.unit.set("moraleLevel", sol::property(&unitHelpers::getMoraleLevel, &unitHelpers::setMoraleLevel));
	types.unit.set("battlePosX", &unit::positionX);
	types.unit.set("battlePosZ", &unit::positionZ);
	types.unit.set("battlePosY", &unit::positionY);
	types.unit.set("angle", sol::property(&unitHelpers::getUnitAngle, &unitHelpers::setUnitAngle));
	types.unit.set("unitGroup", &unit::unitGroup);
	types.unit.set("fatigue", &unit::fatigue);
	types.unit.set("maxAmmo", &unit::maxAmmo);
	types.unit.set("ID", &unit::ID);
	types.unit.set("currentAmmo", &unit::currentAmmo);
	types.unit.set("siegeEngineNum", sol::property(&unitHelpers::getSiegeEngineNum));
	types.unit.set("army", &unit::army);
	types.unit.set("retrainingSettlement", &unit::retrainingSettlement);
	types.unit.set("supplies", &unit::foodRequirement);
	types.unit.set("nearbyFriendlyUnitsNum", &unit::nearbyFriendlyUnitsNum);
	types.unit.set("nearbyEnemyUnitsNum", &unit::nearbyEnemyUnitsNum);
	types.unit.set("missileRange", &unit::missileRange);
	types.unit.set("missile", &unit::missile);
	types.unit.set("weaponType", &unit::weaponType);
	types.unit.set("techType", &unit::weaponTecType);
	types.unit.set("armourInBattle", &unit::armourInBattle);
	types.unit.set("attackInBattle", &unit::attackInBattle);
	/***
	Kill this unit
	@function unit:kill
	@usage
	unit:kill();
	*/
	types.unit.set_function("kill", &fastFuncts::killUnit);
	/***
	Set unit basic parameters
	@function unit:setParams
	@tparam int exp Experience. Maximum: 9
	@tparam int armor Armour level.
	@tparam int weapon Weapon Upgrade. Maximum: 1
	@usage
	unit:setParams(0,0,0);
	*/
	types.unit.set_function("setParams", &unitHelpers::setUnitParams);
	types.unit.set("alias", sol::property(&technicalHelpers::unitUniStringToStr, &technicalHelpers::setUnitUniStr));
	/***
	Check if unit has edu attribute.
	@function unit:hasAttribute
	@usage
	local hasAttr = unit:hasAttribute("sea_faring");
	*/
	types.unit.set_function("hasAttribute", &eopEduHelpers::hasAttribute);
	/***
	Check if unit has guard mode, skirmish or fire at will on.
	@function unit:hasBattleProperty
	@tparam int property use enum: unitBattleProperties.guardMode, unitBattleProperties.skirmish, unitBattleProperties.fireAtWill
	@treturn bool hasProperty
	@usage
	local hasProp = unit:hasBattleProperty(unitBattleProperties.guardMode);
	*/
	types.unit.set_function("hasBattleProperty", &unitHelpers::hasBattleProperty);
	/***
	Set a unit battle property (guard mode, skirmish or fire at will).
	@function unit:setBattleProperty
	@tparam int property use enum: unitBattleProperties.guardMode, unitBattleProperties.skirmish, unitBattleProperties.fireAtWill
	@tparam bool value
	@usage
	unit:setBattleProperty(unitBattleProperties.skirmish, true);
	*/
	types.unit.set_function("setBattleProperty", &unitHelpers::setBattleProperty);
	/***
	Get unit action status in battle ( idling, hiding, ready, reforming, moving, withdrawing, missiles\_firing, missiles\_reloading, charging, fighting, pursuing, routing, fighting\_backs\_to\_the\_walls, running\_amok, rallying, dead, leaving\_battle, entering\_battle, left\_battle, go\_berserk, taunting, bracing, infighting).
	@function unit:getActionStatus
	@treturn string actionStatus
	@usage
	local status = unit:getActionStatus();
	*/
	types.unit.set_function("getActionStatus", &unitHelpers::getActionStatus);
	/***
	Is unit set to run?
	@function unit:isMovingFastSet
	@treturn bool movingFastSet
	@usage
	local isRunning = unit:isMovingFastSet();
	*/
	types.unit.set_function("isMovingFastSet", &unitHelpers::isMovingFastSet);
	/***
	Toggle unit running.
	@function unit:setMovingFastSet
	@tparam bool movingFastSet
	@usage
	unit:setMovingFastSet(true);
	*/
	types.unit.set_function("setMovingFastSet", &unitHelpers::setMovingFastSet);
	/***
	Is unit on walls?
	@function unit:isOnWalls
	@treturn bool isOnWalls
	@usage
	local isOnWalls = unit:isOnWalls();
	*/
	types.unit.set_function("isOnWalls", &unitHelpers::isOnWalls);
	/***
	Is unit engaged in melee?
	@function unit:isEngaged
	@treturn bool isEngaged
	@usage
	local isEngaged = unit:isEngaged();
	*/
	types.unit.set_function("isEngaged", &unitHelpers::isEngaged);
	/***
	Is unit under fire?
	@function unit:isUnderFire
	@treturn bool isUnderFire
	@usage
	local isUnderFire = unit:isUnderFire();
	*/
	types.unit.set_function("isUnderFire", &unitHelpers::isUnderFire);
	/***
	Get unit mount class.
	@function unit:getMountClass
	@treturn int mountClass
	@usage
	local mountClass = unit:getMountClass();
	*/
	types.unit.set_function("getMountClass", &unitHelpers::getMountClass);
	/***
	Get unit formation type.
	@function unit:getFormation
	@treturn int formationType
	@usage
	local formationType = unit:getFormation();
	*/
	types.unit.set_function("getFormation", &unitActions::getUnitFormation);
	/***
	Orders the unit to move to the specified position.
	@function unit:moveToPosition
	@tparam float xCoord
	@tparam float yCoord
	@tparam bool run
	@usage
		unit:moveToPosition(150, -300, true);
	*/
	types.unit.set_function("moveToPosition", &unitActions::unitMovetoPosition);
	/***
	Immediately positions the unit at the given location.
	@function unit:immediatePlace
	@tparam float xCoord
	@tparam float yCoord
	@tparam int angle
	@tparam int width
	@usage
		unit:immediatePlace(150, -300, 60, 0);
	*/
	types.unit.set_function("immediatePlace", &unitActions::placeUnit);
	/***
	Searches an arc in front of the unit, and attacks the closest enemy found.
	@function unit:attackClosestUnit
	@tparam int angle
	@tparam bool run
	@usage
		unit:attackClosestUnit(0, true);
	*/
	types.unit.set_function("attackClosestUnit", &unitActions::unitAttackClosest);
	/***
	Orders the unit to attack another unit.
	@function unit:attackUnit
	@tparam unit targetUnit
	@tparam bool run
	@usage
		unit:attackUnit(target, true);
	*/
	types.unit.set_function("attackUnit", &unitActions::attackUnit);
	/***
	Deploys stakes.
	@function unit:deployStakes
	@usage
		unit:deployStakes();
	*/
	types.unit.set_function("deployStakes", &unitActions::deployStakes);
	/***
	Changes the unit's formation.
	@function unit:changeFormation
	@tparam int formationType use formationType enum
	@usage
		unit:changeFormation();
	*/
	types.unit.set_function("changeFormation", &unitActions::changeUnitFormation);
	/***
	Orders the unit to stop it's orders.
	@function unit:halt
	@usage
		unit:halt();
	*/
	types.unit.set_function("halt", &unitActions::haltUnit);
	/***
	Orders the unit to move to the specified position with a specified rotation and orientation.
	@function unit:moveToOrientation
	@tparam float xCoord
	@tparam float yCoord
	@tparam int widthInMen
	@tparam int angle
	@tparam bool run
	@usage
		unit:moveToOrientation(150, 105, 100, 60, true);
	*/
	types.unit.set_function("moveToOrientation", &unitActions::moveToOrientation);
	/***
	Orders the unit to move to the specified position.
	@function unit:moveRelative
	@tparam float xCoord
	@tparam float yCoord
	@tparam bool run
	@usage
		unit:moveRelative(150, 105, true);
	*/
	types.unit.set_function("moveRelative", &unitActions::moveRelative);
	/***
	Orders the attacker to move into missile range of the target.
	@function unit:moveToMissileRange
	@tparam unit targetUnit
	@tparam bool run
	@usage
		unit:moveToMissileRange(targetUnit, true);
	*/
	types.unit.set_function("moveToMissileRange", &unitActions::moveToMissileRange);
	/***
	Turn to an absolute angle or by an angle relative to it's current rotation.
	@function unit:turn
	@tparam int angle
	@tparam bool isRelative
	@usage
		unit:turn(90, true);
	*/
	types.unit.set_function("turn", &unitActions::unitTurn);
	/***
	Makes the unit taunt.
	@function unit:taunt
	@usage
		unit:taunt();
	*/
	types.unit.set_function("taunt", &unitActions::taunt);
	/***
	Makes the unit perform their special ability.
	@function unit:useSpecialAbility
	@usage
		unit:useSpecialAbility();
	*/
	types.unit.set_function("useSpecialAbility", &unitActions::useSpecialAbility);
	/***
	Get Siege engine.
	@function unit:getSiegeEngine
	@tparam int index
	@treturn siegeEngineStruct engine
	@usage
		local engine = unit:getSiegeEngine(0);
	*/
	types.unit.set_function("getSiegeEngine", &unitHelpers::getSiegeEngine);
	/***
	Order a unit with appropriate siege equipment to attack an appropriate building (including docking towers/ladders to walls).
	@function unit:attackBuilding
	@tparam buildingBattle building
	@usage
		unit:attackBuilding(building);
	*/
	types.unit.set_function("attackBuilding", &unitActions::attackBuilding);
	/***
	Order an infantry unit to collect an engine (ram/ladder/tower).
	@function unit:collectEngine
	@tparam siegeEngineStruct engine
	@usage
		unit:collectEngine(engine);
	*/
	types.unit.set_function("collectEngine", &unitActions::collectEngine);
	/***
	Get Nearby friendly unit.
	@function unit:getNearbyFriendlyUnit
	@tparam int index
	@treturn unit nearUnit
	@usage
		local nearUnit = unit:getNearbyFriendlyUnit(0);
	*/
	types.unit.set_function("getNearbyFriendlyUnit", &unitHelpers::getNearbyUnit);
	/***
	Get Nearby Enemy unit.
	@function unit:getNearbyEnemyUnit
	@tparam int index
	@treturn unit nearUnit
	@usage
		local nearUnit = unit:getNearbyEnemyUnit(0);
	*/
	types.unit.set_function("getNearbyEnemyUnit", &unitHelpers::getNearbyEnemyUnit);
	/***
	Set ai active set to on or off depending on if the unit is player controlled.
	@function unit:releaseUnit
	@usage
		unit:releaseUnit();
	*/
	types.unit.set_function("releaseUnit", &unitHelpers::releaseUnit);
	
	///Unit Position Data
	//@section unitPositionData

	/***

	@tfield int engagedUnitsNum
	@tfield unit unit
	@tfield int isOnWallsCount (amount of walls
	@tfield int isInTowerCount (amount of towers)
	@tfield int isInGateHouseCount (amount of gatehouses)
	@tfield int targetsDone dont set
	@tfield int additionalTargetsOverOne dont set
	@tfield int targetsToGo dont set
	@tfield int hasTargets dont set
	@tfield int isHalted
	@tfield int combatStatus use enum: combatStatus
	@tfield float lastTargetCoord1 dont set
	@tfield float lastTargetCoord2 dont set
	@tfield int towersUnderFireFromCount
	@tfield int unitsUnderFireFromCount
	@tfield getUnitUnderFireFrom getUnitUnderFireFrom
	@tfield getEngagedUnit getEngagedUnit
	@tfield getTargetUnit getTargetUnit

	@table unitPositionData
	*/
	types.unitPositionData = luaState.new_usertype<unitPositionData>("unitPositionData");
	types.unitPositionData.set("engagedUnitsNum", &unitPositionData::engagedUnitsNum);
	types.unitPositionData.set("unit", &unitPositionData::thisUnit);
	types.unitPositionData.set("isOnWallsCount", &unitPositionData::isOnWallsCount);
	types.unitPositionData.set("isInTowerCount", &unitPositionData::isInTowerCount);
	types.unitPositionData.set("isInGateHouseCount", &unitPositionData::isInGateHouseCount);
	types.unitPositionData.set("targetsDone", &unitPositionData::currIndex);
	types.unitPositionData.set("additionalTargetsOverOne", &unitPositionData::additionalTargetsOverOne);
	types.unitPositionData.set("targetsToGo", &unitPositionData::targetsToGo);
	types.unitPositionData.set("hasTargets", &unitPositionData::hasTargets);
	types.unitPositionData.set("combatStatus", &unitPositionData::combatStatus);
	types.unitPositionData.set("isHalted", &unitPositionData::isHalted);
	types.unitPositionData.set("lastTargetCoord1", &unitPositionData::lastTargetCoord1);
	types.unitPositionData.set("lastTargetCoord2", &unitPositionData::lastTargetCoord2);
	types.unitPositionData.set("towersUnderFireFromCount", &unitPositionData::towersUnderFireFromCount);
	types.unitPositionData.set("unitsUnderFireFromCount", &unitPositionData::unitsUnderFireFromCount);

	/***
	Get unit that is firing at this unit.
	@function unitPositionData:getUnitUnderFireFrom
	@tparam int index
	@treturn unit unit
	@usage
	local enemyUnit = unit.unitPositionData:getUnitUnderFireFrom(0);
	*/
	types.unitPositionData.set_function("getUnitUnderFireFrom", &unitHelpers::getUnitUnderFireFrom);

	/***
	Get unit that is fighting this unit in melee.
	@function unitPositionData:getEngagedUnit
	@tparam int index
	@treturn unit unit
	@usage
	local enemyUnit = unit.unitPositionData:getEngagedUnit(0);
	*/
	types.unitPositionData.set_function("getEngagedUnit", &unitHelpers::getEngagedUnit);

	/***
	Get the unit this unit is currently targeting.
	@function unitPositionData:getTargetUnit
	@treturn unit unit
	@usage
	local enemyUnit = unit.unitPositionData:getTargetUnit();
	*/
	types.unitPositionData.set_function("getTargetUnit", &unitHelpers::getTargetUnit);


	///Projectile
	//@section projectileStruct

	/***

	@tfield string name
	@tfield float accuracy
	@tfield float accuracyVsBuildings
	@tfield float accuracyVsTowers
	@tfield int damage
	@tfield int damageToTroops
	@tfield int isBodyPiercing
	@tfield int fiery
	@tfield int affectedByRain

	@table projectileStruct
	*/
	types.projectileStruct = luaState.new_usertype<projectile>("projectileStruct");
	types.projectileStruct.set("name", &projectile::name);
	types.projectileStruct.set("accuracy", &projectile::accuracyVsUnits);
	types.projectileStruct.set("accuracyVsBuildings", &projectile::accuracyVsBuildings);
	types.projectileStruct.set("accuracyVsTowers", &projectile::accuracyVsTowers);
	types.projectileStruct.set("damage", &projectile::damage);
	types.projectileStruct.set("damageToTroops", &projectile::damageToTroops);
	types.projectileStruct.set("affectedByRain", &projectile::affectedByRain);
	types.projectileStruct.set("isBodyPiercing", &projectile::accuracyVsUnits);
	types.projectileStruct.set("fiery", &projectile::fiery);


	///Mount
	//@section mountStruct

	/***

	@tfield string name
	@tfield int mountClass
	@tfield float radius
	@tfield float mass
	@tfield float elephantDeadRadius
	@tfield float elephantTuskRadius

	@table mountStruct
	*/
	types.mountStruct = luaState.new_usertype<descrMountEntry>("mountStruct");
	types.mountStruct.set("name", &descrMountEntry::name);
	types.mountStruct.set("mountClass", &descrMountEntry::mountClass);
	types.mountStruct.set("radius", &descrMountEntry::radius);
	types.mountStruct.set("mass", &descrMountEntry::radius);
	types.mountStruct.set("elephantDeadRadius", &descrMountEntry::elephantDeadRadius);
	types.mountStruct.set("elephantTuskRadius", &descrMountEntry::elephantTuskRadius);

	///EduEntry
	//@section eduEntryTable

	/***
	Basic eduEntry table

	@tfield string eduType - Get only
	@tfield string soldier - Get only
	@tfield string unitCardTga - Get only
	@tfield string infoCardTga - Get only
	@tfield string localizedName
	@tfield string localizedDescription
	@tfield string localizedDescriptionShort
	@tfield int index - Get only
	@tfield int unitCreatedCounter - Get only
	@tfield int category - Get only
	@tfield int class - Get only
	@tfield int categoryClassCombo - Get only
	@tfield int recruitPriorityOffset times 4
	@tfield int crusadingUpkeepModifier
	@tfield number aiUnitValuePerSoldier
	@tfield number aiUnitValue
	@tfield int soldierCount
	@tfield float mass
	@tfield float width
	@tfield float height
	@tfield bool haveAttributeLegio
	@tfield float moveSpeedMod
	@tfield float unitSpacingFrontToBackClose
	@tfield float unitSpacingSideToSideClose
	@tfield float unitSpacingFrontToBackLoose
	@tfield float unitSpacingSideToSideLoose
	@tfield int statHealth
	@tfield int statHealthAnimal
	@tfield int statHeat
	@tfield int statScrub
	@tfield int statSand
	@tfield int statForest
	@tfield int statSnow
	@tfield int recruitTime
	@tfield int recruitCost
	@tfield int upkeepCost
	@tfield int weaponCost
	@tfield int armourCost
	@tfield int customBattleCost
	@tfield int customBattleIncrease
	@tfield int customBattleLimit
	@tfield int training
	@tfield int discipline
	@tfield int canPhalanx
	@tfield int morale
	@tfield int moraleLocked
	@tfield int statFood1
	@tfield int statFood2
	@tfield int ammunition
	@tfield int range
	@tfield projectileStruct projectile
	@tfield int weaponType
	@tfield int techType
	@tfield int damageType
	@tfield int soundType
	@tfield int attackMinDelay
	@tfield int secAmmunition
	@tfield int secRange
	@tfield projectileStruct secProjectile
	@tfield int secWeaponType
	@tfield int secTechType
	@tfield int secDamageType
	@tfield int secSoundType
	@tfield int secAttackMinDelay
	@tfield int engineAmmunition
	@tfield int engineRange
	@tfield projectileStruct engineProjectile
	@tfield int engineWeaponType
	@tfield int engineTechType
	@tfield int engineDamageType
	@tfield int engineSoundType
	@tfield int engineAttackMinDelay
	@tfield int armourMaterial
	@tfield mountStruct mount
	@tfield string primaryAnim
	@tfield string secondaryAnim
	@tfield hasOwnership hasOwnership
	@tfield hasAttribute hasAttribute
	@tfield setOwnerShip setOwnerShip

	@table eduEntry
	*/
	types.eduEntry = luaState.new_usertype<eduEntry>("eduEntry");
	types.eduEntry.set("eduType", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_Type>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_Type>
		));
	types.eduEntry.set("soldier", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_Soldier>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_Soldier>
		));
	types.eduEntry.set("unitCardTga", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_UnitCardTga>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_UnitCardTga>
		));
	types.eduEntry.set("infoCardTga", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_InfoCardTga>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_InfoCardTga>
		));
	types.eduEntry.set("localizedName", sol::property(unitHelpers::getLocalizedUnitName, unitHelpers::setUnitName));
	types.eduEntry.set("localizedDescription", sol::property(unitHelpers::getLocalizedUnitDescr, unitHelpers::setUnitDescr));
	types.eduEntry.set("localizedDescriptionShort", sol::property(unitHelpers::getLocalizedUnitDescrShort, unitHelpers::setUnitDescrShort));
	types.eduEntry.set("index", &eduEntry::Index);
	types.eduEntry.set("unitCreatedCounter", &eduEntry::UnitCreatedCounter);
	types.eduEntry.set("soldierCount", &eduEntry::SoldierCount);
	types.eduEntry.set("mass", &eduEntry::Mass);
	types.eduEntry.set("width", &eduEntry::Width);
	types.eduEntry.set("height", &eduEntry::Height);
	types.eduEntry.set("range", &eduEntry::MissleRange);
	types.eduEntry.set("training", &eduEntry::Trained);
	types.eduEntry.set("discipline", &eduEntry::StatMentalDicipline);
	types.eduEntry.set("range", &eduEntry::MissleRange);
	types.eduEntry.set("projectile", &eduEntry::StatPriMissle);
	types.eduEntry.set("mount", &eduEntry::mount);
	types.eduEntry.set("canPhalanx", &eduEntry::formationPhalanx);
	types.eduEntry.set("haveAttributeLegio", sol::property(&eopEduHelpers::haveAttributeLegioGet, &eopEduHelpers::haveAttributeLegioSet));
	types.eduEntry.set("moveSpeedMod", &eduEntry::MoveSpeedMod);
	types.eduEntry.set("unitSpacingFrontToBackClose", &eduEntry::UnitSpacingFrontToBackClose);
	types.eduEntry.set("unitSpacingSideToSideClose", &eduEntry::UnitSpacingSideToSideClose);
	types.eduEntry.set("unitSpacingFrontToBackLoose", &eduEntry::UnitSpacingFrontToBackLoose);
	types.eduEntry.set("unitSpacingSideToSideLoose", &eduEntry::UnitSpacingSideToSideLoose);
	types.eduEntry.set("statHealth", &eduEntry::StatHealth);
	types.eduEntry.set("statHealthAnimal", &eduEntry::StatHealthAnimal);
	types.eduEntry.set("statHeat", &eduEntry::StatHeat);
	types.eduEntry.set("statScrub", &eduEntry::StatGround1);
	types.eduEntry.set("statSand", &eduEntry::StatGround2);
	types.eduEntry.set("statForest", &eduEntry::StatGround3);
	types.eduEntry.set("statSnow", &eduEntry::StatGround4);
	types.eduEntry.set("recruitTime", &eduEntry::StatCost1);
	types.eduEntry.set("recruitCost", &eduEntry::StatCost2);
	types.eduEntry.set("upkeepCost", &eduEntry::StatCost3);
	types.eduEntry.set("weaponCost", &eduEntry::StatCost4);
	types.eduEntry.set("armourCost", &eduEntry::StatCost5);
	types.eduEntry.set("customBattleCost", &eduEntry::StatCost6);
	types.eduEntry.set("customBattleIncrease", &eduEntry::StatCost7);
	types.eduEntry.set("customBattleLimit", &eduEntry::StatCost8);
	types.eduEntry.set("morale", &eduEntry::Morale);
	types.eduEntry.set("moraleLocked", &eduEntry::MoraleLocked);
	types.eduEntry.set("statFood1", &eduEntry::StatFood1);
	types.eduEntry.set("statFood2", &eduEntry::StatFood2);
	types.eduEntry.set("ammunition", &eduEntry::Ammunition);
	types.eduEntry.set("category", &eduEntry::Category);
	types.eduEntry.set("primaryAnim", sol::property(eopEduHelpers::getPrimaryAnim));
	types.eduEntry.set("secondaryAnim", sol::property(eopEduHelpers::getSecondaryAnim));
	types.eduEntry.set("class", &eduEntry::Class);
	types.eduEntry.set("categoryClassCombo", &eduEntry::categoryClassCombinationForAI);
	types.eduEntry.set("recruitPriorityOffset", &eduEntry::RecruitPriorityOffsetTimes4);
	types.eduEntry.set("crusadingUpkeepModifier", &eduEntry::CrusadingUpkeepModifier);
	types.eduEntry.set("aiUnitValuePerSoldier", &eduEntry::aiUnitValuePerSoldier);
	types.eduEntry.set("aiUnitValue", &eduEntry::aiUnitValue);
	types.eduEntry.set("weaponType", &eduEntry::WeaponType);
	types.eduEntry.set("techType", &eduEntry::TechType);
	types.eduEntry.set("damageType", &eduEntry::DamageType);
	types.eduEntry.set("soundType", &eduEntry::SoundType);
	types.eduEntry.set("attackMinDelay", &eduEntry::AttackMinDelay);
	types.eduEntry.set("secAmmunition", &eduEntry::SecAmmunition);
	types.eduEntry.set("secRange", &eduEntry::SecMissleRange);
	types.eduEntry.set("secProjectile", &eduEntry::StatSecMissle);
	types.eduEntry.set("secWeaponType", &eduEntry::SecWeaponType);
	types.eduEntry.set("secTechType", &eduEntry::SecTechType);
	types.eduEntry.set("secDamageType", &eduEntry::SecDamageType);
	types.eduEntry.set("secSoundType", &eduEntry::SecSoundType);
	types.eduEntry.set("secAttackMinDelay", &eduEntry::SecAttackMinDelay);
	types.eduEntry.set("engineAmmunition", &eduEntry::engineAmmunition);
	types.eduEntry.set("engineRange", &eduEntry::engineRange);
	types.eduEntry.set("engineProjectile", &eduEntry::engineMissle);
	types.eduEntry.set("engineWeaponType", &eduEntry::engineWeaponType);
	types.eduEntry.set("engineTechType", &eduEntry::engineTechType);
	types.eduEntry.set("engineDamageType", &eduEntry::engineDamageType);
	types.eduEntry.set("engineSoundType", &eduEntry::engineSoundType);
	types.eduEntry.set("engineAttackMinDelay", &eduEntry::engineAttackMinDelay);
	types.eduEntry.set("armourMaterial", &eduEntry::armourMaterial);


	/***
	Check if a faction has ownership of this entry.
	@function eduEntry:hasOwnership
	@tparam int factionID
	@treturn bool hasOwnership
	@usage
	local hasOwnership = unit.eduEntry:hasOwnership(2);
	*/
	types.eduEntry.set_function("hasOwnership", &eopEduHelpers::hasOwnership);

	/***
	Set if a faction has ownership of this entry.
	@function eduEntry:setOwnerShip
	@tparam int factionID
	@tparam bool setOwnership
	@usage
		unit.eduEntry:setOwnerShip(2, true);
	*/
	types.eduEntry.set_function("setOwnerShip", &eopEduHelpers::setOwnerShip);


	/***
	Check if the entry has an attribute.
	@function eduEntry:hasAttribute
	@tparam string attributeName
	@treturn bool hasAttribute
	@usage
	local hasAttribute = entry:hasAttribute("sea_faring");
	*/
	types.eduEntry.set_function("hasAttribute", &eopEduHelpers::hasAttributeEdu);
	
	///capturedFactionInfo
	//@section capturedFactionInfoTable

	/***
	Basic capturedFactionInfo table.

	@tfield int targetFactionID
	@tfield int factionID
	@tfield characterRecord namedChar
	@tfield characterRecord targetCharacter
	@tfield getCapturedCharacter getCapturedCharacter
	@tfield int capturedCharactersNum
	@tfield getCapturedUnit getCapturedUnit
	@tfield int capturedUnitsNum
	@tfield int ransomValue

	@table capturedFactionInfo
	*/
	types.capturedFactionInfo = luaState.new_usertype<capturedFactionInfo>("capturedFactionInfo");
	types.capturedFactionInfo.set("targetFactionID", &capturedFactionInfo::targetFactionID);
	types.capturedFactionInfo.set("factionID", &capturedFactionInfo::factionID);
	types.capturedFactionInfo.set("namedChar", &capturedFactionInfo::character);
	types.capturedFactionInfo.set("targetCharacter", &capturedFactionInfo::targetCharacter);
	types.capturedFactionInfo.set("capturedCharactersNum", sol::property(generalCharactericticsHelpers::getCapturedCharacterNum));
	types.capturedFactionInfo.set("capturedUnitsNum", sol::property(generalCharactericticsHelpers::getCapturedUnitNum));
	types.capturedFactionInfo.set("ransomValue", &capturedFactionInfo::ransomValue);

	/***
	Get a captured unit by it's index.
	@function capturedFactionInfo:getCapturedUnit
	@tparam int index
	@treturn capturedUnit capUnit
	@usage
	capUnit = capInfo:getCapturedUnit(0);
	*/
	types.capturedFactionInfo.set_function("getCapturedUnit", &generalCharactericticsHelpers::getCapturedUnit);

	/***
	Get a captured character by it's index.
	@function capturedFactionInfo:getCapturedCharacter
	@tparam int index
	@treturn capturedCharacter capChar
	@usage
	capUnit = capInfo:getCapturedCharacter(0);
	*/
	types.capturedFactionInfo.set_function("getCapturedCharacter", &generalCharactericticsHelpers::getCapturedCharacter);

	///capturedUnit
	//@section capturedUnitTable

	/***
	Basic capturedUnit table.

	@tfield unit unit
	@tfield int capturedSoldiers
	@tfield int capturedValue

	@table capturedUnit
	*/
	types.capturedUnit = luaState.new_usertype<capturedUnit>("capturedUnit");
	types.capturedUnit.set("unit", &capturedUnit::unit);
	types.capturedUnit.set("capturedSoldiers", &capturedUnit::capturedSoldiers);
	types.capturedUnit.set("capturedValue", &capturedUnit::capturedValue);


	///settlementTextStrings
	//@section settlementTextStrings

	/***
	Basic settlementTextStrings table.

	@tfield uiString incomeString
	@tfield uiString incomeValue
	@tfield uiString publicOrderString
	@tfield uiString publicOrderValue
	@tfield uiString populationString
	@tfield uiString populationValue
	@tfield uiString populationGrowthString
	@tfield uiString populationGrowthValue

	@table settlementTextStrings
	*/
	types.settlementTextStrings = luaState.new_usertype<settlementTextStrings>("settlementTextStrings");
	types.settlementTextStrings.set("incomeString", &settlementTextStrings::incomeString);
	types.settlementTextStrings.set("incomeValue", &settlementTextStrings::incomeValue);
	types.settlementTextStrings.set("publicOrderString", &settlementTextStrings::publicOrderString);
	types.settlementTextStrings.set("publicOrderValue", &settlementTextStrings::publicOrderValue);
	types.settlementTextStrings.set("populationString", &settlementTextStrings::populationString);
	types.settlementTextStrings.set("populationValue", &settlementTextStrings::populationValue);
	types.settlementTextStrings.set("populationGrowthString", &settlementTextStrings::populationGrowthString);
	types.settlementTextStrings.set("populationGrowthValue", &settlementTextStrings::populationGrowthValue);

	///uiString
	//@section uiString

	/***
	Basic uiString table.

	@tfield int thickness (0-255)
	@tfield int blue (0-255)
	@tfield int green (0-255)
	@tfield int red (0-255)

	@table uiString
	*/
	types.uiString = luaState.new_usertype<uiString>("uiString");
	types.uiString.set("thickness", &uiString::thickness);
	types.uiString.set("blue", &uiString::blue);
	types.uiString.set("green", &uiString::green);
	types.uiString.set("red", &uiString::red);

	///settlementInfoScroll
	//@section settlementInfoScroll

	/***
	Basic settlementInfoScroll table.

	@tfield settlementStruct settlement
	@tfield getUIStrings getUIStrings

	@table settlementInfoScroll
	*/
	types.settlementInfoScroll = luaState.new_usertype<settlementInfoScroll>("settlementInfoScroll");
	types.settlementInfoScroll.set("settlement", &settlementInfoScroll::settlement);

	/***
	Get settlement text color info.
	@function settlementInfoScroll:getUIStrings
	@treturn settlementTextStrings getUIStrings
	@usage
	function onScrollOpened(eventData)
    	local settlementInfoScroll = M2TWEOP.getSettlementInfoScroll();
		if settlementInfoScroll then
			settlementInfoScroll:getUIStrings().incomeString.red = 0
			settlementInfoScroll:getUIStrings().incomeString.green = 0
			settlementInfoScroll:getUIStrings().incomeString.blue = 0
			settlementInfoScroll:getUIStrings().incomeString.thickness = 0

			settlementInfoScroll:getUIStrings().incomeValue.red = 0
			settlementInfoScroll:getUIStrings().incomeValue.green = 0
			settlementInfoScroll:getUIStrings().incomeValue.blue = 0
		end
	end
	*/
	types.settlementInfoScroll.set_function("getUIStrings", &m2tweopHelpers::getUIStrings);



	///capturedCharacter
	//@section capturedCharacterTable

	/***
	Basic capturedCharacter table.

	@tfield characterRecord namedChar
	@tfield int capturedValue

	@table capturedCharacter
	*/
	types.capturedCharacter = luaState.new_usertype<capturedCharacter>("capturedCharacter");
	types.capturedCharacter.set("namedChar", &capturedCharacter::namedChar);
	types.capturedCharacter.set("capturedValue", &capturedCharacter::capturedValue);

	///Ancillary
	//@section ancillaryTable

	/***
	Basic ancillary table

	@tfield int index
	@tfield string name
	@tfield string imagePath

	@table ancillary
	*/
	types.ancillary = luaState.new_usertype<ancillary>("ancillary");
	types.ancillary.set("index", &ancillary::index);
	types.ancillary.set("name", sol::property(
		&luaGetSetFuncs::getStringPropertyAnc<ancillaryStruct_name>, &luaGetSetFuncs::setStringPropertyAnc<ancillaryStruct_name>
		));
	types.ancillary.set("imagePath", sol::property(
		&luaGetSetFuncs::getStringPropertyAnc<ancillaryStruct_imagePath>, &luaGetSetFuncs::setStringPropertyAnc<ancillaryStruct_imagePath>
		));


	/// Trait Container
	//@section traitsTable

	/***
	Basic traits table

	@tfield int level
	@tfield string name
	@tfield traitContainer nextTrait
	@tfield traitContainer prevTrait
	@tfield traitStruct traitInfo

	@table traitContainer
	*/
	types.traitContainerT = luaState.new_usertype<traitContainer>("traitContainer");
	types.traitContainerT.set("level", sol::property(&luaGetSetFuncs::getTraitLevel));
	types.traitContainerT.set("name", sol::property(&luaGetSetFuncs::getTraitName));
	types.traitContainerT.set("nextTrait", sol::property(&luaGetSetFuncs::getNextTrait));
	types.traitContainerT.set("prevTrait", sol::property(&luaGetSetFuncs::getPrevTrait));
	types.traitContainerT.set("traitInfo", &traitContainer::trait);

	/// Trait Struct
	//@section traitStruct

	/***
	Basic trait table

	@tfield traitEntry traitEntry
	@tfield traitLevel levelEntry
	@tfield int traitPoints

	@table traitStruct
	*/
	types.traitStruct = luaState.new_usertype<trait>("traitStruct");
	types.traitStruct.set("traitEntry", &trait::traitEntry);
	types.traitStruct.set("levelEntry", &trait::level);
	types.traitStruct.set("traitPoints", &trait::traitPoints);

	/// Trait Entry Struct
	//@section traitEntry

	/***
	Basic traitEntry table

	@tfield int index
	@tfield string name
	@tfield traitLevel[10] levels Maximum: 10
	@tfield int levelCount
	@tfield traitEntry[20] antiTraits Maximum: 20
	@tfield int antiTraitCount
	@tfield int noGoingBackLevel
	@tfield int hidden

	@table traitEntry
	*/
	types.traitEntry = luaState.new_usertype<traitEntry>("traitEntry");
	types.traitEntry.set("index", &traitEntry::index);
	types.traitEntry.set("name", &traitEntry::name);
	types.traitEntry.set("levels", sol::property([](traitEntry& self) { return std::ref(self.levels); }));
	types.traitEntry.set("antiTraits", sol::property([](traitEntry& self) { return std::ref(self.antiTraits); }));
	types.traitEntry.set("levelCount", &traitEntry::levelCount);
	types.traitEntry.set("antiTraitCount", &traitEntry::antiTraitCount);
	types.traitEntry.set("noGoingBackLevel", &traitEntry::noGoingBackLevel);
	types.traitEntry.set("hidden", &traitEntry::hidden);

	/// Trait Level Struct
	//@section traitLevel

	/***
	Basic traitLevel table

	@tfield int level
	@tfield int threshold
	@tfield int effectsCount
	@tfield getTraitEffect getTraitEffect

	@table traitLevel
	*/
	types.traitLevel = luaState.new_usertype<traitLevel>("traitLevel");
	types.traitLevel.set("level", &traitLevel::level);
	types.traitLevel.set("threshold", &traitLevel::threshold);
	types.traitLevel.set("effectsCount", &traitLevel::effectsCount);
	/***
	Get trait effect.
	@function traitLevel:getTraitEffect
	@tparam int index
	@treturn traitEffect effect
	@usage
		local effect = level:getTraitEffect(0);
	*/
	types.traitLevel.set_function("getTraitEffect", &luaGetSetFuncs::getTraitEffect);

	/// Trait Effect Struct
	//@section traitEffect

	/***
	Basic traitEffect table

	@tfield int id
	@tfield int value

	@table traitEffect
	*/
	types.traitEffect = luaState.new_usertype<traitEffect>("traitEffect");
	types.traitEffect.set("id", &traitEffect::effectID);
	types.traitEffect.set("value", &traitEffect::value);

	///FactionStruct
	//@section factionStructTable

	/***
	Basic factionStruct table

	@tfield int factionID
	@tfield getFactionName getFactionName
	@tfield int cultureID
	@tfield int aiPersonalityType
	@tfield int aiPersonalityName
	@tfield aiFaction aiFaction
	@tfield string ai_label
	@tfield string name
	@tfield string localizedName
	@tfield settlementStruct capital
	@tfield characterRecord leader
	@tfield characterRecord heir
	@tfield int isPlayerControlled 0=AI, 1=player
	@tfield int neighBourFactionsBitmap
	@tfield int religion
	@tfield int isUndiscovered
	@tfield int missionCount
	@tfield int freezeFactionAI
	@tfield int treasuryTurnStart
	@tfield int incomeDoubled
	@tfield int battlesWon
	@tfield int battlesLost
	@tfield int settlementsCaptured
	@tfield int settlementsLost
	@tfield int otherFactionCount
	@tfield int money
	@tfield int kingsPurse
	@tfield int regionsOwnedStart
	@tfield factionStratMapStruct facStrat
	@tfield int characterRecordNum includes literally all characters without distinction (so also wives, children, dead and those sent off map)
	@tfield getCharacterRecord getCharacterRecord
	@tfield int numOfCharacters includes all the characters present on the strat map
	@tfield getCharacter getCharacter
	@tfield int stacksNum
	@tfield getStack getStack
	@tfield int settlementsNum
	@tfield getSettlement getSettlement
	@tfield int fortsNum
	@tfield getFort getFort
	@tfield int portsNum
	@tfield getPort getPort
	@tfield int watchtowersNum
	@tfield getWatchtower getWatchtower
	@tfield int neighBourRegionsNum
	@tfield deleteFort deleteFort
	@tfield createFortXY createFortXY
	@tfield createWatchtower createWatchtower
	@tfield hasMilitaryAccess hasMilitaryAccess
	@tfield setMilitaryAccess setMilitaryAccess
	@tfield getFactionStanding getFactionStanding
	@tfield setFactionStanding setFactionStanding
	@tfield getFactionRanking getFactionRanking
	@tfield getFactionEconomy getFactionEconomy
	@tfield getNeighbourRegionID getNeighbourRegionID
	@tfield getBattleVsFactionStats getBattleVsFactionStats
	@tfield setCharacterNameFaction setCharacterNameFaction
	@tfield isNeighbourFaction isNeighbourFaction
	@tfield getAiFactionValues getAiFactionValues
	@tfield getInterFactionLTGD getInterFactionLTGD
	@tfield splitArmy splitArmy

	@table factionStruct
	*/
	types.factionStruct = luaState.new_usertype<factionStruct>("factionStruct");
	types.factionStruct.set("dipNum", &factionStruct::dipNum);
	types.factionStruct.set("factionID", &factionStruct::dipNum);
	types.factionStruct.set("aiFaction", &factionStruct::aiFaction);
	types.factionStruct.set("aiPersonalityType", &factionStruct::AIPersonalityType);
	types.factionStruct.set("aiPersonalityName", &factionStruct::AIPersonalityName);
	/***
	Outdated legacy function. Use faction.name instead.
	@function factionStruct:getFactionName
	@treturn string facName
	@usage
	*/
	types.factionStruct.set_function("getFactionName", &factionHelpers::getFactionName);
	types.factionStruct.set("cultureID", &factionStruct::cultureID);
	types.factionStruct.set("ai_label", sol::property(
		&factionHelpers::getStringProperty<factionStruct_ai_label>, &factionHelpers::setStringProperty<factionStruct_ai_label>
		));
	types.factionStruct.set("name", sol::property(
		&factionHelpers::getStringProperty<factionStruct_name>, &factionHelpers::setStringProperty<factionStruct_name>
		));
	types.factionStruct.set("localizedName", sol::property(
		&factionHelpers::getLocalizedFactionName, &smallFuncs::changeFactionName
		));
	types.factionStruct.set("regionsOwnedStart", &factionStruct::regionsOwnedStart);
	types.factionStruct.set("capital", &factionStruct::capital);
	types.factionStruct.set("leader", &factionStruct::leader);
	types.factionStruct.set("heir", &factionStruct::heir);
	types.factionStruct.set("neighBourFactionsBitmap", &factionStruct::neighBourFactionsBitmap);
	types.factionStruct.set("isPlayerControlled", &factionStruct::isPlayerControlled);
	types.factionStruct.set("religion", &factionStruct::religion);
	types.factionStruct.set("missionCount", &factionStruct::missionCount);
	types.factionStruct.set("isUndiscovered", &factionStruct::isUndiscovered);
	types.factionStruct.set("neighBourRegionsNum", &factionStruct::neighBourRegionsNum);
	types.factionStruct.set("money", &factionStruct::money);
	types.factionStruct.set("kingsPurse", &factionStruct::KingsPurse);
	types.factionStruct.set("facStrat", &factionStruct::factSmDescr);
	types.factionStruct.set("freezeFactionAI", &factionStruct::freezeFactionAI);
	types.factionStruct.set("treasuryTurnStart", &factionStruct::treasuryTurnStart);
	types.factionStruct.set("incomeDoubled", &factionStruct::incomeDoubled);
	types.factionStruct.set("battlesWon", &factionStruct::battlesWon);
	types.factionStruct.set("battlesLost", &factionStruct::battlesLost);
	types.factionStruct.set("settlementsCaptured", &factionStruct::settlementsCaptured);
	types.factionStruct.set("settlementsLost", &factionStruct::settlementsLost);
	types.factionStruct.set("otherFactionCount", &factionStruct::otherFactionCount);
	types.factionStruct.set("numOfNamedCharacters", &factionStruct::characterRecordNum);
	types.factionStruct.set("characterRecordNum", &factionStruct::characterRecordNum);
	types.factionStruct.set_function("getNamedCharacter", &factionHelpers::getCharacterFromFullList);
	/***
	Get a character record using it's index.
	@function factionStruct:getCharacterRecord
	@tparam int number
	@treturn characterRecord retCharacter
	@usage
	ourNamedCharacter = stratmap.game.getFaction(0):getCharacterRecord(0)
	if ourNamedCharacter.command > 5 then
		ourFac.money = ourFac.money - (ourNamedCharacter.command * 10)
	end
	*/
	types.factionStruct.set_function("getCharacterRecord", &factionHelpers::getCharacterFromFullList);
	types.factionStruct.set("numOfCharacters", &factionStruct::numOfCharacters);
	/***
	Get a character using it's index.
	@function factionStruct:getCharacter
	@tparam int number
	@treturn character retCharacter
	@usage
	ourCharacter = stratmap.game.getFaction(0):getCharacter(0)
	if ourCharacter.xCoord > 150 and ourCharacter.movePoints < 10 then
		ourCharacter:kill()
	end
	*/
	types.factionStruct.set_function("getCharacter", &factionHelpers::getCharacterFromGeneralsList);
	types.factionStruct.set("stacksNum", &factionStruct::stackNum);
	/***
	Get an army using it's index.
	@function factionStruct:getStack
	@tparam int number
	@treturn stackStruct army
	@usage
	function FindArmy(x,y)
		CAMPAIGN = gameDataAll.get().campaignStruct
		for i = 1, #CAMPAIGN.numberOfFactions do
			local faction = CAMPAIGN.factionsSortedByDescrStrat[i]
			if not faction then
				return nil
			end
			local armiesNum = faction.stacksNum
			for j = 0, armiesNum - 1 do
				local army = faction:getStack(j)
				local x2, y2 = 0, 0
				if (army.leader) then
					x2 = army.leader.xCoord
					y2 = army.leader.yCoord
				elseif army:findInSettlement() then
					x2 = army:findInSettlement().xCoord
					y2 = army:findInSettlement().yCoord
				elseif army:findInFort() then
					x2 = army:findInFort().xCoord
					y2 = army:findInFort().yCoord
				elseif army.shipArmy and army.shipArmy.leader then
					x2 = army.shipArmy.leader.xCoord
					y2 = army.shipArmy.leader.yCoord
				else
					return nil
				end
				if (x2 == x and y2 == y) then
					return army
				end
			end
		end
		return nil
	end
	*/
	types.factionStruct.set_function("getStack", &factionHelpers::getStack);
	types.factionStruct.set("settlementsNum", &factionStruct::settlementsNum);
	/***
	Get a settlement using it's index.
	@function factionStruct:getSettlement
	@tparam int number
	@treturn settlementStruct settlement
	@usage
	ourSett = CAMPAIGN:getFaction("byzantium"):getSettlement(0)
	if ourSett.isProvokedRebellion ~= 0 then
		ourFac.money = ourFac.money - (ourSett.level*5000)
	end
	*/
	types.factionStruct.set_function("getSettlement", &factionHelpers::getSettlement);
	types.factionStruct.set("fortsNum", &factionStruct::fortsNum);
	/***
	Get a fort using it's index.
	@function factionStruct:getFort
	@tparam int number
	@treturn fortStruct fort
	@usage
	ourFort = CAMPAIGN:getFaction("byzantium"):getFort(0)
	if ourFort then
		print(ourFort.xCoord..", "..ourFort.yCoord)
	end
	*/
	types.factionStruct.set_function("getFort", &factionHelpers::getFort);
	types.factionStruct.set("portsNum", &factionStruct::portBuildingsNum);
	/***
	Get a port using it's index.
	@function factionStruct:getPort
	@tparam int number
	@treturn portStruct port
	@usage
	local thisFac, portList = gameDataAll.get().campaignStruct.currentFaction, "Ports:"
	for i = 0, thisFac.portsNum - 1, 1 do
		local thisPort = thisFac:getPort(i)
		local thisDock = thisPort.dock
		portList = portList.."\n\t"..i.." "..thisPort.settlement.name.." ("..thisPort.xCoord..", "..thisPort.yCoord..") "
		if thisDock then
			portList = portList.."\n\t\tDock: ("..thisDock.xCoord..", "..thisDock.yCoord..")"
		end
	end
	print(portList)
	*/
	types.factionStruct.set_function("getPort", &factionHelpers::getPort);
	types.factionStruct.set("watchtowersNum", &factionStruct::watchtowersNum);
	/***
	Get a watchtower using it's index.
	@function factionStruct:getWatchtower
	@tparam int number
	@treturn watchtowerStruct watchtower
	@usage
	local thisFac, watchtowerList = gameDataAll.get().campaignStruct.currentFaction, "Watchtowers:"
	for i = 0, thisFac.watchtowersNum - 1, 1 do
		local thisTower = thisFac:getWatchtower(i)
		watchtowerList = watchtowerList.."\n\t"..i.." ("..thisTower.xCoord..", "..thisTower.yCoord..") "
	end
	print(watchtowerList)
	*/
	types.factionStruct.set_function("getWatchtower", &factionHelpers::getWatchtower);


	/***
	Delete a specific fort.
	@function factionStruct:deleteFort
	@tparam fortStruct fort
	@usage
	fac:deleteFort(someFort)
	*/
	types.factionStruct.set_function("deleteFort", &fortHelpers::deleteFort);

	/***
	Create a fort at the specified coordinates.
	@function factionStruct:createFortXY
	@tparam int X
	@tparam int Y
	@usage
	fac:createFortXY(193, 283)
	*/
	types.factionStruct.set_function("createFortXY", &fortHelpers::createFortXY);

	/***
	Create a watchtower at the specified coordinates.
	@function factionStruct:createWatchtower
	@tparam int X
	@tparam int Y
	@treturn watchtowerStruct watchTower
	@usage
	     fac:createWatchtower(193, 283)
	*/
	types.factionStruct.set_function("createWatchtower", &factionHelpers::spawnWatchtower);

	/***
	Check if a faction has military access to another faction.
	@function factionStruct:hasMilitaryAccess
	@tparam factionStruct targetFaction
	@treturn bool hasMilitaryAccess
	@usage
	local hasAccess = fac:hasMilitaryAccess(targetFac)
	*/
	types.factionStruct.set_function("hasMilitaryAccess", &factionHelpers::hasMilitaryAccess);

	/***
	Set if a faction has military access to another faction.
	@function factionStruct:setMilitaryAccess
	@tparam factionStruct targetFaction
	@tparam bool hasMilitaryAccess
	@usage
	fac:setMilitaryAccess(targetFac, true)
	*/
	types.factionStruct.set_function("setMilitaryAccess", &factionHelpers::setMilitaryAccess);

	/***
	Get the faction standing between 2 factions (a faction with itself returns global standing).
	@function factionStruct:getFactionStanding
	@tparam factionStruct targetFaction
	@treturn float factionStanding
	@usage
	local standing = fac:getFactionStanding(targetFac)
	*/
	types.factionStruct.set_function("getFactionStanding", &factionHelpers::getFactionStanding);
	/***
	Set the faction standing between 2 factions (a faction with itself sets global standing).
	@function factionStruct:setFactionStanding
	@tparam factionStruct targetFaction
	@tparam float factionStanding
	@usage
	fac:setFactionStanding(targetFac, 0.5)
	*/
	types.factionStruct.set_function("setFactionStanding", &factionHelpers::setFactionStanding);

	/***
	Get the faction ranking scores.
	@function factionStruct:getFactionRanking
	@tparam int turnNumber
	@treturn factionRanking factionRanking
	@usage
	local rankings = fac:getFactionRanking(30)
	*/
	types.factionStruct.set_function("getFactionRanking", &factionHelpers::getFactionRanking);

	/***
	Get the faction ecomomy table, specified number of turns ago (max 10).
	@function factionStruct:getFactionEconomy
	@tparam int turnsAgo
	@treturn factionEconomy factionEconomy
	@usage
	local economyTable = fac:getFactionEconomy(0)
	*/
	types.factionStruct.set_function("getFactionEconomy", &factionHelpers::getFactionEconomy);

	/***
	Check if 2 factions are neighbours.
	@function factionStruct:isNeighbourFaction
	@tparam factionStruct targetFaction
	@treturn bool isNeighbour
	@usage
	local isNeighbour = fac:isNeighbourFaction(targetFac)
	*/
	types.factionStruct.set_function("isNeighbourFaction", &factionHelpers::isNeighbourFaction);

	/***
	Get a region ID of a neighbouring region by index.
	@function factionStruct:getNeighbourRegionID
	@tparam int index
	@treturn int nRegionID
	@usage
	local nRegionID = fac:getNeighbourRegionID(0)
	*/
	types.factionStruct.set_function("getNeighbourRegionID", &factionHelpers::getNeighbourRegionID);

	/***
	Get stats versus a specific other faction.
	@function factionStruct:getBattleVsFactionStats
	@tparam int targetFactionID
	@treturn battleFactionCounter battleStats
	@usage
	local battleStats = fac:getBattleVsFactionStats(2)
	*/
	types.factionStruct.set_function("getBattleVsFactionStats", &factionHelpers::getBattleVsFactionStats);

	/***
	Get various statistics the long term goal director uses.
	@function factionStruct:getAiFactionValues
	@treturn aiFactionValues aiFactionValues
	@usage
	local aiFacValues = fac:getAiFactionValues()
	*/
	types.factionStruct.set_function("getAiFactionValues", &factionHelpers::getAiFactionValues);

	/***
	Get various statistics the long term goal director uses versus a target faction.
	@function factionStruct:getInterFactionLTGD
	@tparam factionStruct targetFaction
	@treturn interFactionLTGD interFactionLTGD
	@usage
	local iLTGD = fac:getInterFactionLTGD(targetFac)
	*/
	types.factionStruct.set_function("getInterFactionLTGD", &factionHelpers::getInterFactionLTGD);

	/***
	Set the faction characters of a specific type draw their names from.
	@function factionStruct:setCharacterNameFaction
	@tparam int characterTypeIndex
	@tparam int factionID
	@usage
	     fac:setCharacterNameFaction(characterType.spy, 2)
	*/
	types.factionStruct.set_function("setCharacterNameFaction", &factionHelpers::setCharacterNameFaction);

	/***
	Split an army.
	@function factionStruct:splitArmy
	@tparam table units
	@tparam int targetX
	@tparam int targetY
	@usage
	     local units = {}
	     for i = 0, myStack.unitsNum / 2 - 1 do
	         table.insert(units, myStack:getUnit(i))
	     end
	     fac:splitArmy(units, 154, 84)
	*/
	types.factionStruct.set_function("splitArmy", &factionHelpers::splitArmy);

	///aiFaction
	//@section aiFaction

	/***
	Basic aiFaction table

	@tfield factionStruct faction
	@tfield int factionID
	@tfield aiLongTermGoalDirector LTGD
	@tfield aiPersonality aiPersonality

	@table aiFaction
	*/
	types.aiFaction = luaState.new_usertype<aiFaction>("aiFaction");
	types.aiFaction.set("faction", &aiFaction::faction);
	types.aiFaction.set("factionID", &aiFaction::factionID);
	types.aiFaction.set("LTGD", &aiFaction::aiLongTermGoalDirector);
	types.aiFaction.set("aiPersonality", &aiFaction::aiProductionControllers);

	///aiLongTermGoalDirector
	//@section aiLongTermGoalDirector

	/***
	Basic aiLongTermGoalDirector table

	@tfield factionStruct faction
	@tfield aiFaction aiFaction
	@tfield int trustedAllyEnemiesBitfield
	@tfield int freeStrengthEnemy
	@tfield int freeStrengthEnemyBalance
	@tfield int consideringNavalInvasion
	@tfield int navalTargetRegionID
	@tfield int navalTargetRegionPriority
	@tfield getlongTermGoalValues getlongTermGoalValues

	@table aiLongTermGoalDirector
	*/
	types.aiLongTermGoalDirector = luaState.new_usertype<aiLongTermGoalDirector>("aiLongTermGoalDirector");
	types.aiLongTermGoalDirector.set("faction", &aiLongTermGoalDirector::faction);
	types.aiLongTermGoalDirector.set("aiFaction", &aiLongTermGoalDirector::aiFaction);
	types.aiLongTermGoalDirector.set("trustedAllyEnemiesBitfield", &aiLongTermGoalDirector::trustedAllyEnemies);
	types.aiLongTermGoalDirector.set("freeStrengthEnemy", &aiLongTermGoalDirector::freeStrengthEnemy);
	types.aiLongTermGoalDirector.set("freeStrengthEnemyBalance", &aiLongTermGoalDirector::freeStrengthEnemyBalance);
	types.aiLongTermGoalDirector.set("consideringNavalInvasion", &aiLongTermGoalDirector::consideringNavalInvasion);
	types.aiLongTermGoalDirector.set("navalTargetRegionID", &aiLongTermGoalDirector::navalTargetRegionID);
	types.aiLongTermGoalDirector.set("navalTargetRegionPriority", &aiLongTermGoalDirector::navalTargetRegionPriority);

	/***
	Get long term goal director values versus a specific other faction.
	@function aiLongTermGoalDirector:getlongTermGoalValues
	@tparam int targetFactionID
	@treturn decisionValuesLTGD longTermGoalValues
	@usage
	local longTermGoalValues = LTGD:getlongTermGoalValues(2)
	*/
	types.aiLongTermGoalDirector.set_function("getlongTermGoalValues", &factionHelpers::getlongTermGoalValues);

	///decisionValuesLTGD
	//@section decisionValuesLTGD

	/***
	Basic decisionValuesLTGD table

	@tfield int defendType
	@tfield int defendPriority
	@tfield int invasionType
	@tfield int invadePriority
	@tfield int atWar
	@tfield int wantPeace
	@tfield int wantAlly
	@tfield int forceInvade
	@tfield int wantBeProtect
	@tfield int wantOfferProtect
	@tfield int allianceAgainst
	@tfield int ptsDesire
	@tfield int ptsAlliance
	@tfield int pointsInvasion
	@tfield int pointsDefense
	@tfield int canForceInvade

	@table decisionValuesLTGD
	*/
	types.decisionValuesLTGD = luaState.new_usertype<decisionValuesLTGD>("decisionValuesLTGD");
	types.decisionValuesLTGD.set("defendType", &decisionValuesLTGD::defendType);
	types.decisionValuesLTGD.set("defendPriority", &decisionValuesLTGD::defendPriority);
	types.decisionValuesLTGD.set("invasionType", &decisionValuesLTGD::invasionType);
	types.decisionValuesLTGD.set("invadePriority", &decisionValuesLTGD::invadePriority);
	types.decisionValuesLTGD.set("atWar", &decisionValuesLTGD::atWar);
	types.decisionValuesLTGD.set("wantPeace", &decisionValuesLTGD::wantPeace);
	types.decisionValuesLTGD.set("wantAlly", &decisionValuesLTGD::wantAlly);
	types.decisionValuesLTGD.set("wantBeProtect", &decisionValuesLTGD::wantBeProtect);
	types.decisionValuesLTGD.set("wantOfferProtect", &decisionValuesLTGD::wantOfferProtect);
	types.decisionValuesLTGD.set("allianceAgainst", &decisionValuesLTGD::allianceAgainst);
	types.decisionValuesLTGD.set("ptsDesire", &decisionValuesLTGD::ptsDesire);
	types.decisionValuesLTGD.set("ptsAlliance", &decisionValuesLTGD::ptsAlliance);
	types.decisionValuesLTGD.set("canForceInvade", &decisionValuesLTGD::canForceInvade);
	types.decisionValuesLTGD.set("forceInvade", &decisionValuesLTGD::forceInvade);
	types.decisionValuesLTGD.set("pointsInvasion", &decisionValuesLTGD::pointsInvasion);
	types.decisionValuesLTGD.set("pointsDefense", &decisionValuesLTGD::pointsDefense);

	///aiFactionValues
	//@section aiFactionValues

	/***
	Basic aiFactionValues table

	@tfield int totalPopulation
	@tfield int tileCount
	@tfield int averagePopulation
	@tfield int productionValue
	@tfield int nonAlliedBorderLength
	@tfield int enemyBorderLength
	@tfield int fleetCount
	@tfield int navalPowerPerFleet
	@tfield int navalStrength
	@tfield int armyCount
	@tfield int strengthPerArmy
	@tfield int totalStrength
	@tfield int freeStrength
	@tfield int neighbourEnemyNum
	@tfield int enemyStrength
	@tfield int protectorateOf

	@table aiFactionValues
	*/
	types.aiFactionValues = luaState.new_usertype<ltgdFactionValues>("aiFactionValues");
	types.aiFactionValues.set("totalPopulation", &ltgdFactionValues::totalPopulation);
	types.aiFactionValues.set("tileCount", &ltgdFactionValues::tileCount);
	types.aiFactionValues.set("averagePopulation", &ltgdFactionValues::averagePopulation);
	types.aiFactionValues.set("productionValue", &ltgdFactionValues::productionValue);
	types.aiFactionValues.set("nonAlliedBorderLength", &ltgdFactionValues::nonAlliedBorderLength);
	types.aiFactionValues.set("enemyBorderLength", &ltgdFactionValues::enemyBorderLength);
	types.aiFactionValues.set("fleetCount", &ltgdFactionValues::fleetCount);
	types.aiFactionValues.set("navalPowerPerFleet", &ltgdFactionValues::navalPowerPerFleet);
	types.aiFactionValues.set("navalStrength", &ltgdFactionValues::navalStrength);
	types.aiFactionValues.set("armyCount", &ltgdFactionValues::armyCount);
	types.aiFactionValues.set("strengthPerArmy", &ltgdFactionValues::strengthPerArmy);
	types.aiFactionValues.set("totalStrength", &ltgdFactionValues::totalStrength);
	types.aiFactionValues.set("freeStrength", &ltgdFactionValues::freeStrength);
	types.aiFactionValues.set("neighbourEnemyNum", &ltgdFactionValues::neighbourEnemyNum);
	types.aiFactionValues.set("enemyStrength", &ltgdFactionValues::enemyStrength);
	types.aiFactionValues.set("protectorateOf", &ltgdFactionValues::protectorateOf);


	///interFactionLTGD
	//@section interFactionLTGD

	/***
	Basic interFactionLTGD table

	@tfield int borderTiles
	@tfield int frontLineBalance
	@tfield int hasAllianceAgainst
	@tfield int isStrongestNeighbour
	@tfield int isWeakestNeighbour

	@table interFactionLTGD
	*/
	types.interFactionLTGD = luaState.new_usertype<interFactionLTGD>("interFactionLTGD");
	types.interFactionLTGD.set("borderTiles", &interFactionLTGD::borderTiles);
	types.interFactionLTGD.set("frontLineBalance", &interFactionLTGD::frontLineBalance);
	types.interFactionLTGD.set("hasAllianceAgainst", &interFactionLTGD::hasAllianceAgainst);
	types.interFactionLTGD.set("isStrongestNeighbour", &interFactionLTGD::isStrongestNeighbour);
	types.interFactionLTGD.set("isWeakestNeighbour", &interFactionLTGD::isWeakestNeighbour);


	///aiPersonality
	//@section aiPersonality

	/***
	Basic aiPersonality table

	@tfield aiFaction aiFaction
	@tfield int aiProductionControllersNum
	@tfield int aiPersonalityType
	@tfield int aiPersonalityName
	@tfield int spyBias
	@tfield int assassinBias
	@tfield int diplomatBias
	@tfield int admiralBias
	@tfield int priestBias
	@tfield int merchantBias
	@tfield int balancedPolicyNum
	@tfield int financialPolicyNum
	@tfield int militaryPolicyNum
	@tfield int growthPolicyNum
	@tfield int culturalPolicyNum
	@tfield setConstructionValue setConstructionValue
	@tfield setRecruitmentValue setRecruitmentValue
	@tfield getConstructionValue getConstructionValue
	@tfield getRecruitmentValue getRecruitmentValue
	@tfield getProductionController getProductionController

	@table aiPersonality
	*/
	types.aiPersonality = luaState.new_usertype<aiPersonalityValues>("aiPersonality");
	types.aiPersonality.set("aiFaction", &aiPersonalityValues::aiFaction);
	types.aiPersonality.set("aiProductionControllersNum", &aiPersonalityValues::aiProductionControllersNum);
	types.aiPersonality.set("aiPersonalityType", &aiPersonalityValues::AIPersonalityType);
	types.aiPersonality.set("aiPersonalityName", &aiPersonalityValues::AIPersonalityName);
	types.aiPersonality.set("spyBias", &aiPersonalityValues::spyBias);
	types.aiPersonality.set("assassinBias", &aiPersonalityValues::assassinBias);
	types.aiPersonality.set("diplomatBias", &aiPersonalityValues::diplomatBias);
	types.aiPersonality.set("admiralBias", &aiPersonalityValues::admiralBias);
	types.aiPersonality.set("priestBias", &aiPersonalityValues::priestBias);
	types.aiPersonality.set("merchantBias", &aiPersonalityValues::merchantBias);
	types.aiPersonality.set("balancedPolicyNum", &aiPersonalityValues::balancedPolicyNum);
	types.aiPersonality.set("financialPolicyNum", &aiPersonalityValues::financialPolicyNum);
	types.aiPersonality.set("militaryPolicyNum", &aiPersonalityValues::militaryPolicyNum);
	types.aiPersonality.set("growthPolicyNum", &aiPersonalityValues::growthPolicyNum);
	types.aiPersonality.set("culturalPolicyNum", &aiPersonalityValues::culturalPolicyNum);
	/***
	Set bias value of the ai personality for a capability.
	@function aiPersonality:setConstructionValue
	@tparam int type use building capabilities enum
	@tparam int value
	@usage
	     aiPersonality:setConstructionValue(buildingCapability.law_bonus, 100)
	*/
	types.aiPersonality.set_function("setConstructionValue", &factionHelpers::setConstructionValue);
	/***
	Set bias value of the ai personality for a recruitment class.
	@function aiPersonality:setRecruitmentValue
	@tparam int type use unitCategoryClass enum
	@tparam int value
	@usage
		 aiPersonality:setRecruitmentValue(unitCategoryClass.heavyCavalry, 100)
	*/
	types.aiPersonality.set_function("setRecruitmentValue", &factionHelpers::setRecruitmentValue);
	/***
	Get bias value of the ai personality for a capability.
	@function aiPersonality:getConstructionValue
	@tparam int type use building capabilities enum
	@treturn int value
	@usage
		local value = aiPersonality:getConstructionValue(buildingCapability.law_bonus)
	*/
	types.aiPersonality.set_function("getConstructionValue", &factionHelpers::getConstructionValue);
	/***
	Get bias value of the ai personality for a recruitment class.
	@function aiPersonality:getRecruitmentValue
	@tparam int type use unitCategoryClass enum
	@treturn int value
	@usage
		local value = aiPersonality:getRecruitmentValue(unitCategoryClass.heavyCavalry)
	*/
	types.aiPersonality.set_function("getRecruitmentValue", &factionHelpers::getRecruitmentValue);
	/***
	Get a production controller by index.
	@function aiPersonality:getProductionController
	@tparam int index
	@treturn aiProductionController controller
	@usage
		local prodController = aiPersonality:getProductionController(0)
	*/
	types.aiPersonality.set_function("getProductionController", &factionHelpers::getProductionController);


	///battleFactionCounter
	//@section battleFactionCounter

	/***
	Basic battleFactionCounter table

	@tfield int battlesWon
	@tfield int battlesLost

	@table battleFactionCounter
	*/
	types.battleFactionCounter = luaState.new_usertype<battleFactionCounter>("battleFactionCounter");
	types.battleFactionCounter.set("battlesWon", &battleFactionCounter::battlesWon);
	types.battleFactionCounter.set("battlesLost", &battleFactionCounter::battlesLost);

	///holdRegionsWinCondition
	//@section holdRegionsWinCondition

	/***
	Basic holdRegionsWinCondition table

	@tfield int regionsToHoldCount
	@tfield int numberOfRegions
	@tfield getRegionToHoldName getRegionToHoldName
	@tfield getRegionToHoldLength getRegionToHoldLength
	
	@table holdRegionsWinCondition
	*/
	types.holdRegionsWinCondition = luaState.new_usertype<holdRegionsWinCondition>("holdRegionsWinCondition");
	types.holdRegionsWinCondition.set("regionsToHoldCount", &holdRegionsWinCondition::regionsToHoldCount);
	types.holdRegionsWinCondition.set("numberOfRegions", &holdRegionsWinCondition::numberOfRegions);

	/***
	Get the name of the region that has to be held to win the campaign.
	@function holdRegionsWinCondition:getRegionToHoldName
	@tparam int index
	@treturn string regionName
	@usage
	local regionName = fac.winCondition:getRegionToHoldName(0)
	*/
	types.holdRegionsWinCondition.set_function("getRegionToHoldName", &factionHelpers::getRegionToHoldName);

	/***
	Get the number of turns the region has to be held to win the campaign.
	@function holdRegionsWinCondition:getRegionToHoldLength
	@tparam int index
	@treturn int turnsToHold
	@usage
	local turnsToHold = fac.winCondition:getRegionToHoldLength(0)
	*/
	types.holdRegionsWinCondition.set_function("getRegionToHoldLength", &factionHelpers::getRegionToHoldLength);


	///FactionEconomy
	//@section factionEconomy

	/***
	Basic factionEconomy table

	@tfield int farmingIncome
	@tfield int taxesIncome
	@tfield int miningIncome
	@tfield int tradeIncome
	@tfield int merchantIncome
	@tfield int constructionIncome
	@tfield int otherIncome1
	@tfield int otherIncome2
	@tfield int diplomacyIncome
	@tfield int tributesIncome
	@tfield int adminIncome
	@tfield int kingsPurseIncome
	@tfield int wagesExpense
	@tfield int upkeepExpense
	@tfield int constructionExpenseBuildings
	@tfield int constructionExpenseField
	@tfield int recruitmentExpenseBuildings
	@tfield int recruitmentExpenseMercs
	@tfield int corruptionExpense
	@tfield int diplomacyExpense
	@tfield int tributesExpense
	@tfield int otherExpense1
	@tfield int otherExpense2
	@tfield int devastationExpense

	@table factionEconomy
	*/
	types.factionEconomy = luaState.new_usertype<factionEconomy>("factionEconomy");
	types.factionEconomy.set("farmingIncome", &factionEconomy::farmingIncome);
	types.factionEconomy.set("taxesIncome", &factionEconomy::taxesIncome);
	types.factionEconomy.set("miningIncome", &factionEconomy::miningIncome);
	types.factionEconomy.set("tradeIncome", &factionEconomy::tradeIncome);
	types.factionEconomy.set("merchantIncome", &factionEconomy::merchantIncome);
	types.factionEconomy.set("constructionIncome", &factionEconomy::constructionIncome);
	types.factionEconomy.set("otherIncome1", &factionEconomy::otherIncome1);
	types.factionEconomy.set("otherIncome2", &factionEconomy::otherIncome2);
	types.factionEconomy.set("diplomacyIncome", &factionEconomy::diplomacyIncome);
	types.factionEconomy.set("tributesIncome", &factionEconomy::tributesIncome);
	types.factionEconomy.set("adminIncome", &factionEconomy::adminIncome);
	types.factionEconomy.set("kingsPurseIncome", &factionEconomy::kingsPurseIncome);
	types.factionEconomy.set("wagesExpense", &factionEconomy::wagesExpense);
	types.factionEconomy.set("upkeepExpense", &factionEconomy::upkeepExpense);
	types.factionEconomy.set("constructionExpenseBuildings", &factionEconomy::constructionExpenseBuildings);
	types.factionEconomy.set("constructionExpenseField", &factionEconomy::constructionExpenseField);
	types.factionEconomy.set("recruitmentExpenseBuildings", &factionEconomy::recruitmentExpenseBuildings);
	types.factionEconomy.set("recruitmentExpenseMercs", &factionEconomy::recruitmentExpenseMercs);
	types.factionEconomy.set("corruptionExpense", &factionEconomy::corruptionExpense);
	types.factionEconomy.set("diplomacyExpense", &factionEconomy::diplomacyExpense);
	types.factionEconomy.set("tributesExpense", &factionEconomy::tributesExpense);
	types.factionEconomy.set("otherExpense1", &factionEconomy::otherExpense1);
	types.factionEconomy.set("otherExpense2", &factionEconomy::otherExpense2);
	types.factionEconomy.set("devastationExpense", &factionEconomy::devastationExpense);

	///FactionRankings
	//@section factionRanking

	/***
	Basic factionRanking table

	@tfield float totalRankingScore
	@tfield float militaryRankingScore
	@tfield float productionRankingScore
	@tfield float territoryRankingScore
	@tfield float financialRankingScore
	@tfield float populationRankingScore

	@table factionRanking
	*/
	types.factionRanking = luaState.new_usertype<factionRanking>("factionRanking");
	types.factionRanking.set("totalRankingScore", &factionRanking::totalRanking);
	types.factionRanking.set("militaryRankingScore", &factionRanking::militaryRanking);
	types.factionRanking.set("productionRankingScore", &factionRanking::productionRanking);
	types.factionRanking.set("territoryRankingScore", &factionRanking::territoryRanking);
	types.factionRanking.set("financialRankingScore", &factionRanking::financialRanking);
	types.factionRanking.set("populationRankingScore", &factionRanking::populationRanking);

	///FactionStratMapStruct
	//@section factionStratMapStructTable

	/***
	Basic factionStratMapStruct table

	@tfield int primaryColorRed Warning: resets on reload.
	@tfield int primaryColorGreen Warning: resets on reload.
	@tfield int primaryColorBlue Warning: resets on reload.
	@tfield int secondaryColorRed Warning: resets on reload.
	@tfield int secondaryColorGreen Warning: resets on reload.
	@tfield int secondaryColorBlue Warning: resets on reload.
	@tfield int triumphValue Usage unknown.
	@tfield int religionID
	@tfield int standardIndex Warning: resets on reload.
	@tfield int logoIndex Warning: resets on reload.
	@tfield int smallLogoIndex Warning: resets on reload.
	@tfield int customBattleAvailability
	@tfield int periodsUnavailableInCustomBattle
	@tfield int canSap shouldnt do anything in med 2, but could potentially use flag to store some other info about this faction
	@tfield int prefersNavalInvasions
	@tfield int canHavePrincess
	@tfield int hasFamilyTree
	@tfield int teutonic
	@tfield int disbandToPools
	@tfield int canBuildSiegeTowers
	@tfield int canTransmitPlague
	@tfield int shadowedByID
	@tfield int shadowingID
	@tfield int spawnsOnRevoltID
	@tfield int roman
	@tfield int barbarian
	@tfield int eastern
	@tfield int slave
	@tfield int hordeMinUnits
	@tfield int hordeMaxUnits
	@tfield int reductionPerHorde
	@tfield int hordeUnitPerSettlementPop
	@tfield int hordeMinNamedCharacters
	@tfield int hordeMaxPercentArmyStack

	@table factionStratMapStruct
	*/
	types.factionStratMapStruct = luaState.new_usertype<factionStratMapDescrS>("factionStratMapStruct");
	types.factionStratMapStruct.set("primaryColorRed", &factionStratMapDescrS::primary_colour_red);
	types.factionStratMapStruct.set("primaryColorGreen", &factionStratMapDescrS::primary_colour_green);
	types.factionStratMapStruct.set("primaryColorBlue", &factionStratMapDescrS::primary_colour_blue);
	types.factionStratMapStruct.set("secondaryColorRed", &factionStratMapDescrS::secondary_colour_red);
	types.factionStratMapStruct.set("secondaryColorGreen", &factionStratMapDescrS::secondary_colour_green);
	types.factionStratMapStruct.set("secondaryColorBlue", &factionStratMapDescrS::secondary_colour_blue);
	types.factionStratMapStruct.set("triumphValue", &factionStratMapDescrS::triumph_value);
	types.factionStratMapStruct.set("standardIndex", &factionStratMapDescrS::standard_index);
	types.factionStratMapStruct.set("logoIndex", &factionStratMapDescrS::logo_index);
	types.factionStratMapStruct.set("smallLogoIndex", &factionStratMapDescrS::small_logo_index);
	types.factionStratMapStruct.set("religionID", &factionStratMapDescrS::religionID);
	types.factionStratMapStruct.set("customBattleAvailability", &factionStratMapDescrS::customBattleAvailability);
	types.factionStratMapStruct.set("periodsUnavailableInCustomBattle", &factionStratMapDescrS::periodsUnavailableInCustomBattle);
	types.factionStratMapStruct.set("canSap", &factionStratMapDescrS::canSap);
	types.factionStratMapStruct.set("prefersNavalInvasions", &factionStratMapDescrS::prefersNavalInvasions);
	types.factionStratMapStruct.set("canHavePrincess", &factionStratMapDescrS::canHavePrincess);
	types.factionStratMapStruct.set("hasFamilyTree", &factionStratMapDescrS::hasFamilyTree);
	types.factionStratMapStruct.set("teutonic", &factionStratMapDescrS::teutonic);
	types.factionStratMapStruct.set("disbandToPools", &factionStratMapDescrS::disbandToPools);
	types.factionStratMapStruct.set("canBuildSiegeTowers", &factionStratMapDescrS::canBuildSiegeTowers);
	types.factionStratMapStruct.set("canTransmitPlague", &factionStratMapDescrS::canTransmitPlague);
	types.factionStratMapStruct.set("shadowedByID", &factionStratMapDescrS::shadowedByID);
	types.factionStratMapStruct.set("shadowingID", &factionStratMapDescrS::shadowingID);
	types.factionStratMapStruct.set("spawnsOnRevoltID", &factionStratMapDescrS::spawnsOnRevoltID);
	types.factionStratMapStruct.set("roman", &factionStratMapDescrS::roman);
	types.factionStratMapStruct.set("barbarian", &factionStratMapDescrS::barbarian);
	types.factionStratMapStruct.set("eastern", &factionStratMapDescrS::eastern);
	types.factionStratMapStruct.set("slave", &factionStratMapDescrS::slave);
	types.factionStratMapStruct.set("hordeMinUnits", &factionStratMapDescrS::hordeMinUnits);
	types.factionStratMapStruct.set("hordeMaxUnits", &factionStratMapDescrS::hordeMaxUnits);
	types.factionStratMapStruct.set("reductionPerHorde", &factionStratMapDescrS::reductionPerHorde);
	types.factionStratMapStruct.set("hordeUnitPerSettlementPop", &factionStratMapDescrS::hordeUnitPerSettlementPop);
	types.factionStratMapStruct.set("hordeMinNamedCharacters", &factionStratMapDescrS::hordeMinNamedCharacters);
	types.factionStratMapStruct.set("hordeMaxPercentArmyStack", &factionStratMapDescrS::hordeMaxPercentArmyStack);


	///WatchtowerStruct
	//@section watchtowerStructTable

	/***
	Basic watchtowerStruct table

	@tfield int xCoord
	@tfield int yCoord
	@tfield int factionID
	@tfield int regionID
	@tfield factionStruct faction
	@tfield settlementStruct settlement
	@tfield stackStruct blockingArmy

	@table watchtowerStruct
	*/
	types.watchtowerStruct = luaState.new_usertype<watchTowerStruct>("watchtowerStruct");
	types.watchtowerStruct.set("xCoord", &watchTowerStruct::xCoord);
	types.watchtowerStruct.set("yCoord", &watchTowerStruct::yCoord);
	types.watchtowerStruct.set("factionID", &watchTowerStruct::factionID);
	types.watchtowerStruct.set("regionID", &watchTowerStruct::regionID);
	types.watchtowerStruct.set("faction", &watchTowerStruct::faction);
	types.watchtowerStruct.set("settlement", &watchTowerStruct::settlement);
	types.watchtowerStruct.set("blockingArmy", &watchTowerStruct::blockingArmy);




	///PortStruct
	//@section portStructTable

	/***
	Basic portStruct table

	@tfield int xCoord land tile, Note: setting this only moves port's strat model.
	@tfield int yCoord land tile, Note: setting this only moves port's strat model.
	@tfield factionStruct ownerFaction Note: port's ownership changes to blockading faction (army on port)
	@tfield settlementStruct settlement
	@tfield int regionID
	@tfield int cultureID
	@tfield int numTurnsBlocked
	@tfield stackStruct blockadingArmy Enemy army blockading the port, by standing on it's tile, check for nil.
	@tfield dockStruct dock water tile, only upgraded ports have this, check for nil.


	@table portStruct
	*/
	types.portStruct = luaState.new_usertype<portBuildingStruct>("portStruct");
	types.portStruct.set("xCoord", &portBuildingStruct::xCoord);
	types.portStruct.set("yCoord", &portBuildingStruct::yCoord);
	types.portStruct.set("ownerFaction", &portBuildingStruct::fac);
	types.portStruct.set("settlement", &portBuildingStruct::settlement);
	types.portStruct.set("blockadingArmy", &portBuildingStruct::occupantsArmy);
	types.portStruct.set("dock", &portBuildingStruct::portDock);
	types.portStruct.set("regionID", &portBuildingStruct::regionID);
	types.portStruct.set("numTurnsBlocked", &portBuildingStruct::numTurnsBlocked);
	types.portStruct.set("cultureID", &portBuildingStruct::subFactionCultureID);


	///DockStruct
	//@section dockStructTable

	/***
	Basic dockStruct table

	@tfield int xCoord water tile, Note: setting only moves dock strat model
	@tfield int yCoord water tile, Note: setting only moves dock strat model
	@tfield int regionID
	@tfield factionStruct ownerFaction Note: port's ownership changes to blockading faction (army on port)
	@tfield settlementStruct settlement
	@tfield int cultureID
	@tfield int numTurnsBlocked
	@tfield portStruct port
	@tfield stackStruct dockedArmy
	

	@table dockStruct
	*/
	types.dockStruct = luaState.new_usertype<portDockStrat>("dockStruct");
	types.dockStruct.set("xCoord", &portDockStrat::xCoord);
	types.dockStruct.set("yCoord", &portDockStrat::yCoord);
	types.dockStruct.set("regionID", &portDockStrat::regionID);
	types.dockStruct.set("ownerFaction", &portDockStrat::fac);
	types.dockStruct.set("settlement", &portDockStrat::settlement);
	types.dockStruct.set("cultureID", &portDockStrat::subFactionCultureID);
	types.dockStruct.set("numTurnsBlocked", &portDockStrat::numTurnsBlocked);
	types.dockStruct.set("port", &portDockStrat::port);
	types.dockStruct.set("dockedArmy", &portDockStrat::dockedArmy);

	///TradeResource
	//@section tradeResource

	/***
	Basic tradeResource table

	@tfield int xCoord
	@tfield int yCoord
	@tfield int regionID
	@tfield int resourceID
	@tfield settlementStruct settlement
	@tfield setStratModel setStratModel
	@tfield getResourceID getResourceID
	@tfield getResourceValue getResourceValue
	@tfield getResourceHasMine getResourceHasMine
	@tfield getResourceImage getResourceImage

	@table tradeResource
	*/
	types.tradeResource = luaState.new_usertype<resStrat>("tradeResource");
	types.tradeResource.set("xCoord", &resStrat::xCoord);
	types.tradeResource.set("yCoord", &resStrat::yCoord);
	types.tradeResource.set("regionID", &resStrat::regionID);
	types.tradeResource.set("resourceID", &resStrat::resourceID);
	types.tradeResource.set("settlement", &resStrat::settlement);
	/***
	Set the resource's strat model.
	@function tradeResource:setStratModel
	@tparam int modelId Added with stratmap.objects.addModelToGame
	@usage
	tradeResource:setStratModel(5);
	*/
	types.tradeResource.set_function("setStratModel", &resourcesHelpers::setModel);
	/***
	Get the resource's ID.
	@function tradeResource:getResourceID
	@treturn int ID
	@usage
	
	--
	if tradeResource:getResourceID() == resourceType.wool then --wool
		--do stuff
	end
	*/
	types.tradeResource.set_function("getResourceID", &resourcesHelpers::getResourceCode);
	/***
	Get the resource's trade value.
	@function tradeResource:getResourceValue
	@treturn int value
	@usage
	if tradeResource:getResourceValue() == 5 then
		--do stuff
	end
	*/
	types.tradeResource.set_function("getResourceValue", &resourcesHelpers::getResourceCost);
	/***
	Check if the resource has a mine.
	@function tradeResource:getResourceHasMine
	@treturn int hasMine 0=no mine, 1=mine
	@usage
	if tradeResource:getResourceHasMine() == 1 then
		--do stuff
	end
	*/
	types.tradeResource.set_function("getResourceHasMine", &resourcesHelpers::getResourceHasMine);
	/***
	Get the resource's image (icon) relative path.
	@function tradeResource:getResourceImage
	@treturn string imagePath
	@usage
	function onSettlementSelected(eventData)
		local selectedSett = eventData.settlement
		local resList = ""
		for i = 0, selectedSett.resourcesNum - 1, 1 do
			local thisRes = selectedSett:getResource(i)
			resList = resList.."Resource ID: "..thisRes:getResourceID().."\n\t("..thisRes.xCoord..", "..thisRes.yCoord..")\n\tTrade Value: "..thisRes:getResourceValue().."\n\thasMine: "..thisRes:getResourceHasMine().."\n\timage: "..thisRes:getResourceImage().."\n\n"
		end
		print(selectedSett.name.." resource list\n\n"..resList)
	end
	*/
	types.tradeResource.set_function("getResourceImage", &resourcesHelpers::getImage);

	///StackStruct
	//@section stackStructTable

	/***
	Basic stackStruct table

	@tfield factionStruct faction
	@tfield getUnit getUnit
	@tfield int numOfUnits
	@tfield int deadUnitsNum
	@tfield int isAdmiral
	@tfield int ladders
	@tfield int rams
	@tfield int towers
	@tfield getCharacter getCharacter
	@tfield int numOfCharacters Includes Auxiliary generals and agents (i.e all characters excluding the leading general)
	@tfield stackStruct boardedArmy army embarked on this fleet stack
	@tfield stackStruct shipArmy fleet that this army stack is embarked on
	@tfield dockStruct blockedPort
	@tfield character leader Returns nil if stack is inside residence (fleet, settlement, fort).
	@tfield findInSettlement findInSettlement
	@tfield findInFort findInFort
	@tfield int totalStrength
	@tfield int subFactionID
	@tfield int regionID
	@tfield int totalStrengthStart
	@tfield int alliance
	@tfield int isHalfDestroyed
	@tfield int generalBattleCommand
	@tfield number generalCommandRadius
	@tfield stackStruct commandingArmy if not leading army but reinforcement
	@tfield stackStruct commandingArmyThatGivesCommands reinforcement and listening to player commands
	@tfield int inBattle
	@tfield int maxGroups
	@tfield float reform_point_x X coordinate to which the retreating units will go.
	@tfield float reform_point_y Y coordinate to which the retreating units will go.
	@tfield createEOPUnit createEOPUnit
	@tfield createUnit createUnit
	@tfield sortStack sortStack
	@tfield mergeArmies mergeArmies
	@tfield createUnitByIDX createUnitByIDX
	@tfield siegeSettlement siegeSettlement
	@tfield siegeFort siegeFort
	@tfield blockadePort blockadePort
	@tfield attackArmy attackArmy
	@tfield getGroup getGroup
	@tfield siegeStruct siege Current siege.
	@tfield defineUnitGroup defineUnitGroup
	@tfield getDeadUnit getDeadUnit
	@tfield setAiActiveSet setAiActiveSet
	@tfield releaseUnits releaseUnits
	@tfield buildWatchTower buildWatchTower


	@table stackStruct
	*/
	types.stackStruct = luaState.new_usertype<stackStruct>("stackStruct");
	types.stackStruct.set("faction", &stackStruct::faction);
	types.stackStruct.set("isAdmiral", &stackStruct::isAdmiral);
	types.stackStruct.set("deadUnitsNum", &stackStruct::deadUnitsNum);
	types.stackStruct.set("ladders", &stackStruct::ladders);
	types.stackStruct.set("rams", &stackStruct::rams);
	types.stackStruct.set("maxGroups", &stackStruct::maxUnitGroups);
	types.stackStruct.set("towers", &stackStruct::towers);
	types.stackStruct.set("siege", &stackStruct::siege);
	types.stackStruct.set("regionID", &stackStruct::regionID);
	types.stackStruct.set("subFactionID", &stackStruct::subFactionID);
	types.stackStruct.set("totalStrengthStart", &stackStruct::totalStrengthStart);
	types.stackStruct.set("totalStrengthStart", &stackStruct::totalStrengthStart);
	types.stackStruct.set("inBattle", &stackStruct::inBattle);
	types.stackStruct.set("generalCommandRadius", &stackStruct::generalCommandRadius);
	types.stackStruct.set("generalBattleCommand", &stackStruct::generalBattleCommand);
	types.stackStruct.set("alliance", &stackStruct::battleSide);
	types.stackStruct.set("isHalfDestroyed", &stackStruct::isHalfDestroyed);
	types.stackStruct.set("isHalfDestroyed", &stackStruct::isHalfDestroyed);
	types.stackStruct.set("commandingArmy", &stackStruct::commandingArmy);
	types.stackStruct.set("commandingArmyThatGivesCommands", &stackStruct::commandingArmyThatGivesCommands);

	/***
	Sort units in a stack. Use the sortType enum to specify the sorting mode.
	@function stackStruct:sortStack
	@tparam int sortMode
	@tparam int sortMode2
	@tparam int sortMode3
	@usage

	function onFactionTurnStart(eventData)
	local faction = eventData.faction
    -- If it's not the players turn, don't sort
    if faction.isPlayerControlled == 0 then return end;

	function onFactionTurnStart(eventData)
		CAMPAIGN = gameDataAll.get().campaignStruct
		local faction = eventData.faction
		-- If it's not the players turn, don't sort
		if faction.isPlayerControlled == 0 then return end;

		-- Sort all the stacks on the map right before the turn starts
		local factionsNum = CAMPAIGN.numberOfFactions;
		for i = 1, #factionsNum do
			local fac = CAMPAIGN.factionsSortedByDescrStrat[i];
			for j = 0, fac.stacksNum - 1 do
				local stack = fac:getStack(j);
				if stack then
					-- Sort the stack by category + class, then by soldier count, then by experience
					stack:sortStack(sortType.categoryClass, sortType.soldierCount, sortType.experience)
				end
			end
		end
	end
	
	*/
	types.stackStruct.set_function("sortStack", &stackStructHelpers::sortStack);

	/***
	Get a unit by it's index.
	@function stackStruct:getUnit
	@tparam int number
	@treturn unit retUnit
	@usage
	ourUnit=stackStruct:getUnit(0);
	ourUnit:kill();
	*/
	types.stackStruct.set_function("getUnit", &stackStructHelpers::getUnit);
	types.stackStruct.set("numOfUnits", &stackStruct::numOfUnits);
	/***
	Get a character (agent or non-leading named character) by it's index.
	@function stackStruct:getCharacter
	@tparam int number
	@treturn character retCharacter
	@usage
	ourChar=stackStruct:getCharacter(0);
	ourChar:kill();
	*/
	types.stackStruct.set_function("getCharacter", &stackStructHelpers::getCharacter);
	types.stackStruct.set("numOfCharacters", &stackStruct::charactersNum);
	types.stackStruct.set("boardedArmy", &stackStruct::boardedArmy);
	types.stackStruct.set("shipArmy", &stackStruct::shipArmy);
	types.stackStruct.set("blockedPort", &stackStruct::blockedPort);
	types.stackStruct.set("leader", &stackStruct::gen);
	/***
	Find the settlement in which the army is located. Returns nil if the army is not in a settlement.

	Returns nil if the army is not in the settlement.

	@function stackStruct:findInSettlement
	@treturn  settlementStruct  settlement
	@usage
	ourSett=stackStruct:findInSettlement();
	if(ourSett~=nil)
	then
		--something
	end
	*/
	types.stackStruct.set_function("findInSettlement", &stackStructHelpers::findInSettlement);
	/***
	Find the fort in which the army is located. Returns nil if the army is not in a fort.

	Returns nil if the army is not in the fort.

	@function stackStruct:findInFort
	@treturn  fortStruct  fort
	@usage
	ourFort=stackStruct:findInFort();
	if(ourFort~=nil)
	then
		--something
	end
	*/
	types.stackStruct.set_function("findInFort", &stackStructHelpers::findInFort);
	types.stackStruct.set("totalStrength", &stackStruct::totalStrength);
	types.stackStruct.set("reform_point_x", &stackStruct::reform_point_x);
	types.stackStruct.set("reform_point_y", &stackStruct::reform_point_y);

	/***
	Create a unit in the army by index from M2TWEOP units DB (M2TWEOPDU).
	@function stackStruct:createEOPUnit
	@tparam int index
	@tparam int exp
	@tparam int armor
	@tparam int weapon
	@treturn unit newUnit
	@usage
	local newUnit=stackStruct:createEOPUnit(1000,1,1,1);
	*/
	types.stackStruct.set_function("createEOPUnit", &stackStructHelpers::createEOPUnit);
	/***
	Create a unit in the army by type from export\_descr\_unit.txt
	@function stackStruct:createUnit
	@tparam string type
	@tparam int exp Experience. Maximum: 9.
	@tparam int armor Armour level.
	@tparam int weapon Weapon upgrade. Maximum: 1.
	@treturn unit newUnit
	@usage
	local newUnit=stackStruct:createUnit("Axemen of Lossarnach",1,1,1);
	*/
	types.stackStruct.set_function("createUnit", &stackStructHelpers::createUnit);

	/***
	Create a unit in the army by index from export\_descr\_unit.txt
	@function stackStruct:createUnitByIDX
	@tparam int index Index (order in export\_descr\_unit.txt)
	@tparam int exp Experience. Maximum: 9.
	@tparam int armor Armour level.
	@tparam int weapon Weapon upgrade. Maximum: 1.
	@treturn unit newUnit
	@usage
	local newUnit=stackStruct:createUnitByIDX(255,1,1,1);
	*/
	types.stackStruct.set_function("createUnitByIDX", &stackStructHelpers::createUnitByIDX);


	/***
	Merge 2 armies on the strat map. Does nothing if the total size of the new army exceeds 20 units.
	@function stackStruct:mergeArmies
	@tparam stackStruct targetArmy
	@tparam bool force optional
	@usage
	army:mergeArmies(anotherArmy);
	*/
	types.stackStruct.set_function("mergeArmies", sol::overload(
			sol::resolve<void(stackStruct*, stackStruct*)>(stackStructHelpers::mergeArmies),
			sol::resolve<void(stackStruct*, stackStruct*, bool)>(stackStructHelpers::mergeArmies)
		));

	/***
	Besiege the specified settlement, or attack it if already besieging it. Requires movement points.
	@function stackStruct:siegeSettlement
	@tparam settlementStruct settlement
	@tparam bool isAttack if this is false it makes the army maintain a siege
	@usage
	stackStruct:siegeSettlement(settlement);
	*/
	types.stackStruct.set_function("siegeSettlement", &characterHelpers::siegeSettlement);

	/***
	Besiege the specified fort, or attack it if already besieging it. Requires movement points.
	@function stackStruct:siegeFort
	@tparam fortStruct fort
	@tparam bool isAttack if this is false it makes the army maintain a siege
	@usage
	stackStruct:siegeFort(fort);
	*/
	types.stackStruct.set_function("siegeFort", &characterHelpers::siegeFort);

	/***
	Blockade a port.
	@function stackStruct:blockadePort
	@tparam portStruct port
	@treturn bool success
	@usage
	local success = stackStruct:blockadePort(port);
	*/
	types.stackStruct.set_function("blockadePort", &actionsStrat::blockadePort);
	
	/***
	Attack another army. Requires movement points.

	@function stackStruct:attackArmy
	@tparam stackStruct defender
	@treturn  int Success Failed = 0.
	@usage
	sucess=stackStruct:attackArmy(defenderArmy);
	if(sucess~=0)
	then
		--something
	end
	*/
	types.stackStruct.set_function("attackArmy", &stackStructHelpers::attackArmy);
	
	/***
	Get dead unit at index.
	@function stackStruct:getDeadUnit
	@tparam int index
	@treturn unit deadUnit
	@usage
	    local unit =stackStruct:getDeadUnit(0);
	*/
	types.stackStruct.set_function("getDeadUnit", &stackStructHelpers::getDeadUnit);
	
	/***
	Get unit group at index.
	@function stackStruct:getGroup
	@tparam int index
	@treturn unitGroup group
	@usage
	    local group =stackStruct:getGroup(0);
	*/
	types.stackStruct.set_function("getGroup", &stackStructHelpers::getGroup);
	
	/***
	Define a new unit group with a label.
	@function stackStruct:defineUnitGroup
	@tparam string name
	@tparam unit unit
	@treturn unitGroup group
	@usage
	    local group =stackStruct:defineUnitGroup("group1", unit);
	*/
	types.stackStruct.set_function("defineUnitGroup", &unitHelpers::defineUnitGroup);
	
	/***
	Set ai active set to a value for the whole army (0 = inacitve, 1 = active, 2 = script controlled)
	@function stackStruct:setAiActiveSet
	@tparam int activeSet
	@usage
	     stackStruct:setAiActiveSet(1);
	*/
	types.stackStruct.set_function("setAiActiveSet", &stackStructHelpers::setAiActiveSet);
	
	/***
	Set ai active set to on or off depending if army is player controlled
	@function stackStruct:releaseUnits
	@usage
	     stackStruct:releaseUnits();
	*/
	types.stackStruct.set_function("releaseUnits", &stackStructHelpers::releaseUnits);
	
	/***
	Build a watchtower (payment applies)
	@function stackStruct:buildWatchTower
	@usage
	     stackStruct:buildWatchTower();
	*/
	types.stackStruct.set_function("buildWatchTower", &actionsStrat::buildWatchTower);
	
	///unitGroup
	//@section Unit Group

	/***
	Basic unitGroup table

	@tfield int unitsInFormationNum
	@tfield int unitsNotInFormationNum
	@tfield int unitNumTotal
	@tfield float xCoord
	@tfield float yCoord
	@tfield int angle
	@tfield int automationType 1 = defend, 2 = attack
	@tfield float defendXCoord if automated defend (1)
	@tfield float defendYCoord if automated defend (1)
	@tfield float defendRadius if automated defend (1)
	@tfield unit targetUnit if automated attack (2)
	@tfield int newAutomationType 1 = defend, 2 = attack (setting this passes info from new fields to active fields)
	@tfield float newDefendXCoord if automated defend (1)
	@tfield float newDefendYCoord if automated defend (1)
	@tfield float newDefendRadius if automated defend (1)
	@tfield unit newTargetUnit if automated attack (2)
	@tfield getUnitInFormation getUnitInFormation
	@tfield getUnitNotInFormation getUnitNotInFormation
	@tfield addUnit addUnit
	@tfield removeUnit removeUnit
	@tfield undefine undefine
	@tfield automate automate
	@tfield automateAttack automateAttack
	@tfield automateDefense automateDefense
	@tfield changeUnitFormation changeUnitFormation
	@tfield moveToMissileRangeOfGroup moveToMissileRangeOfGroup
	@tfield moveToMissileRangeOfUnit moveToMissileRangeOfUnit
	@tfield attackGroup attackGroup
	@tfield halt halt
	@tfield place place
	@tfield moveFormed moveFormed
	@tfield moveUnformed moveUnformed
	@tfield moveRelativeFormed moveRelativeFormed
	@tfield moveRelativeUnformed moveRelativeUnformed
	@tfield turn turn
	

	@table unitGroup
	*/
	types.unitGroup = luaState.new_usertype<unitGroup>("unitGroup");
	types.unitGroup.set("unitsInFormationNum", &unitGroup::unitsInFormationNum);
	types.unitGroup.set("unitsNotInFormationNum", &unitGroup::unitsNotInFormationNum);
	types.unitGroup.set("unitNumTotal", &unitGroup::unitsNumTotal);
	types.unitGroup.set("xCoord", &unitGroup::xCoord);
	types.unitGroup.set("yCoord", &unitGroup::yCoord);
	types.unitGroup.set("angle", &unitGroup::angle);
	types.unitGroup.set("automationType", &unitGroup::automationType);
	types.unitGroup.set("defendXCoord", &unitGroup::defendXCoord);
	types.unitGroup.set("defendYCoord", &unitGroup::defendYCoord);
	types.unitGroup.set("defendRadius", &unitGroup::defendRadius);
	types.unitGroup.set("targetUnit", &unitGroup::targetUnit);
	types.unitGroup.set("newAutomationType", &unitGroup::newAutomationType);
	types.unitGroup.set("newDefendXCoord", &unitGroup::newDefendXCoord);
	types.unitGroup.set("newDefendYCoord", &unitGroup::newDefendYCoord);
	types.unitGroup.set("newDefendRadius", &unitGroup::newDefendRadius);
	types.unitGroup.set("newTargetUnit", &unitGroup::newTargetUnit);
	
	/***
	Add a unit to the group (won't add if unit already in group).
	@function unitGroup:addUnit
	@tparam unit unit
	@usage
			unitGroup:addUnit(unit);
	*/
	types.unitGroup.set_function("addUnit", &unitHelpers::addUnitToGroup);
	
	/***
	Remove a unit from a group.
	@function unitGroup:removeUnit
	@tparam unit unit
	@usage
			unitGroup:removeUnit(unit);
	*/
	types.unitGroup.set_function("removeUnit", &unitHelpers::removeUnitFromGroup);
	
	/***
	Undefine a unit group.
	@function unitGroup:undefine
	@usage
			unitGroup:undefine();
	*/
	types.unitGroup.set_function("undefine", &unitHelpers::undefineUnitGroup);
	
	/***
	Toggle group automation.
	@function unitGroup:automate
	@tparam bool automate
	@usage
			unitGroup:automate(true);
	*/
	types.unitGroup.set_function("automate", &unitHelpers::automateGroup);
	
	/***
	Automate group attack.
	@function unitGroup:automateAttack
	@tparam unit targetUnit
	@usage
			unitGroup:automateAttack(targetUnit);
	*/
	types.unitGroup.set_function("automateAttack", &unitHelpers::automateAttack);
	
	/***
	Automate group defense.
	@function unitGroup:automateDefense
	@tparam float xCoord
	@tparam float yCoord
	@tparam float radius
	@usage
			unitGroup:automateDefense(180, 283, 50);
	*/
	types.unitGroup.set_function("automateDefense", &unitHelpers::automateDefense);
	
	/***
	Get a unit in the group's formation (not given individual commands). Once you give a unit a command once it seems they will always be in the other array.
	@function unitGroup:getUnitInFormation
	@tparam int index
	@treturn unit unit
	@usage
			local un = unitGroup:getUnitInFormation(0);
	*/
	types.unitGroup.set_function("getUnitInFormation", &unitHelpers::getUnitInFormation);
	
	/***
	Get a unit not in the group's formation (not given individual commands). Once you give a unit a command once it seems they will always be in the other array.
	@function unitGroup:getUnitNotInFormation
	@tparam int index
	@treturn unit unit
	@usage
			local un = unitGroup:getUnitNotInFormation(0);
	*/
	types.unitGroup.set_function("getUnitNotInFormation", &unitHelpers::getUnitNotInFormation);
	
	/***
	Place a group at a location.
	@function unitGroup:place
	@tparam float xCoord
	@tparam float yCoord
	@tparam float angle
	@usage
			unitGroup:place(150, 127, 0);
	*/
	types.unitGroup.set_function("place", &unitHelpers::placeGroup);
	
	/***
	Change the group's units formations.
	@function unitGroup:changeUnitFormation
	@tparam int formationType use enum
	@usage
			unitGroup:changeUnitFormation(formationType.phalanx);
	*/
	types.unitGroup.set_function("changeUnitFormation", &unitActions::changeGroupUnitFormation);
	
	/***
	Move to put an enemy group inside your missile range.
	@function unitGroup:moveToMissileRangeOfGroup
	@tparam unitGroup targetGroup
	@tparam bool run
	@usage
			unitGroup:moveToMissileRangeOfGroup(otherGroup, true);
	*/
	types.unitGroup.set_function("moveToMissileRangeOfGroup", &unitActions::moveToRangeOfGroup);
	
	/***
	Move to put an enemy unit inside your missile range.
	@function unitGroup:moveToMissileRangeOfUnit
	@tparam unit targetUnit
	@tparam bool run
	@usage
			unitGroup:moveToMissileRangeOfUnit(targetUnit, true);
	*/
	types.unitGroup.set_function("moveToMissileRangeOfUnit", &unitActions::moveGroupToRangeOfUnit);
	
	/***
	Attack another group.
	@function unitGroup:attackGroup
	@tparam unitGroup targetGroup
	@tparam bool run
	@usage
			unitGroup:attackGroup(targetGroup, true);
	*/
	types.unitGroup.set_function("attackGroup", &unitActions::groupAttackGroup);
	
	/***
	Halt the group's orders.
	@function unitGroup:halt
	@usage
			unitGroup:halt();
	*/
	types.unitGroup.set_function("halt", &unitActions::groupHalt);
	
	/***
	Move to the specified location in formation.
	@function unitGroup:moveFormed
	@tparam float xCoord
	@tparam float yCoord
	@tparam bool run
	@usage
			unitGroup:moveFormed(182, 333, true);
	*/
	types.unitGroup.set_function("moveFormed", &unitActions::groupMoveFormed);
	
	/***
	Move to the specified location not in formation.
	@function unitGroup:moveUnformed
	@tparam float xCoord
	@tparam float yCoord
	@tparam bool run
	@usage
			unitGroup:moveUnformed(182, 333, true);
	*/
	types.unitGroup.set_function("moveUnformed", &unitActions::groupMoveUnformed);
	
	/***
	Move to the specified location in formation.
	@function unitGroup:moveRelativeFormed
	@tparam float xCoord
	@tparam float yCoord
	@tparam bool run
	@usage
			unitGroup:moveRelativeFormed(182, 333, true);
	*/
	types.unitGroup.set_function("moveRelativeFormed", &unitActions::groupMoveFormedRelative);
	
	/***
	Move to the specified location not in formation.
	@function unitGroup:moveRelativeUnformed
	@tparam float xCoord
	@tparam float yCoord
	@tparam bool run
	@usage
			unitGroup:moveRelativeUnformed(182, 333, true);
	*/
	types.unitGroup.set_function("moveRelativeUnformed", &unitActions::groupMoveUnformedRelative);
	
	/***
	Turn the group either relative or absolute.
	@function unitGroup:turn
	@tparam int angle
	@tparam bool isRelative
	@usage
			unitGroup:turn(90, true);
	*/
	types.unitGroup.set_function("turn", &unitActions::groupTurn);
	
	
	///SiegeStruct
	//@section siegeStruct

	/***
	Basic siegeStruct table

	@tfield stackStruct besieger
	@tfield settlementStruct besiegedSettlement
	@tfield fortStruct besiegedFort
	@tfield integer siegeTurns
	@tfield integer soldierCount

	@table siegeStruct
	*/
	types.siege = luaState.new_usertype<siegeS>("siegeStruct");
	types.siege.set("besieger", &siegeS::army);
	types.siege.set("siegeTurns", &siegeS::siegeTurns);
	types.siege.set("soldierCount", &siegeS::soldierCount);
	types.siege.set("besiegedSettlement", sol::property(
		&siegeHelpers::getSiegedSettlement));
	types.siege.set("besiegedFort", sol::property(
		&siegeHelpers::getSiegedFort));





	return &luaState;
}

void luaP::checkLuaFunc(sol::function** lRef)
{
	if ((*lRef)->valid() == false)
	{
		*lRef = nullptr;
	}
}

void luaP::onChangeTurnNum(int num)
{
	if (onChangeTurnNumFunc != nullptr)
	{
		tryLua((*onChangeTurnNumFunc)(num));
	}
}


void luaP::fillHashMaps()
{
	const gameDataAllStruct* gameData = gameDataAllHelper::get();
	const campaign* campaign = gameData->campaignData;
	if (!campaign)
		return;
	for (int i = 0; i < campaign->numberOfFactionsWithSlave; i++)
	{
		if (const auto faction = campaign->factionsSortedByDescrStrat[i])
			factions.insert_or_assign(std::string(faction->factSmDescr->facName), faction->dipNum);
	}
	const auto stratMap = gameData->stratMap;
	if (!stratMap)
		return;
	for (int i = 0; i < stratMap->regionsNum; i++)
	{
		const auto region = &stratMap->regions[i];
		if (region)
			regions.insert_or_assign(std::string(region->regionName), i);
		if(region->settlement)
			settlements.insert_or_assign(std::string(region->settlement->name), i);
	}
	for (int i = 0; i < gameHelpers::getReligionCount(); i++)
	{
		if (auto religionName = gameHelpers::getReligionName2(i))
		{
			religionNames.insert_or_assign(i, religionName);
			religionIndex.insert_or_assign(std::string(religionName), i);
		}
	}
	const auto cultureDb = m2tweopHelpers::getCultureDb();
	for (int i = 0; i < cultureDb->culturesCount; i++)
	{
		const auto culture = &cultureDb->cultures[i];
		cultureNames.insert_or_assign(culture->cultureID, culture->cultureName);
		cultureIndex.insert_or_assign(std::string(culture->cultureName), culture->cultureID);
	}
	for (int i = 0; i < stratMap->climates->climatesNum; i++)
	{
		if (auto climateName = gameHelpers::getClimateName2(i))
		{
			climateNames.insert_or_assign(i, climateName);
			climateIndex.insert_or_assign(std::string(climateName), i);
		}
	}
	hashLoaded = true;
}
