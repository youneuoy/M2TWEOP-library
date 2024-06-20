///
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"

#include "casModelsDrawer.h"
#include "console.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include "settlementConversionLvlSetter.h"
#include "smallFuncs.h"
#include "stratModelsChange.h"
#include "tilesChange.h"
#include "realGameTypes.h"
#include <windows.h>
#include "gameHelpers.h"
#include "m2tweopHelpers.h"
#include "technicalHelpers.h"
#include "gameSTDUIHelpers.h"
#include "character.h"
#include "settlement.h"
#include "characterRecord.h"
#include "faction.h"
#include "unit.h"
#include "army.h"
#include "battle.h"
#include "campaign.h"
#include "campaignDb.h"
#include "DeveloperMode.h"
#include "eopBuildings.h"
#include "strategyMap.h"
std::vector<std::string> luaP::logS;
std::vector<std::string> luaP::logCommands;


static int ourP(lua_State* l)
{
	const int n = lua_gettop(l);  /* number of arguments */
	lua_getglobal(l, "tostring");
	for (int i = 1; i <= n; i++) {
		std::string newS;
		lua_pushvalue(l, -1);  /* function to be called */
		lua_pushvalue(l, i);   /* value to print */
		lua_call(l, 1, 1);
		const char* s = lua_tostring(l, -1);  /* get result */
		if (s == nullptr)
			return luaL_error(l, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		if (i > 1)
			newS += '\t';
		newS += s;
		luaP::logS.push_back(newS);
		lua_pop(l, 1);  /* pop result */
	}
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

void luaP::runScriptS(const std::string* script)
{
	logS.emplace_back("\n== Output ==");
	sol::load_result scriptResult = luaState.load(*script);
	if (!scriptResult.valid()) {
		const sol::error luaError = scriptResult;
		logS.emplace_back("\n== Error ==\n");
		logS.emplace_back(luaError.what());
		return;
	}
	if (const sol::protected_function_result result = scriptResult(); !result.valid()) {
		const sol::error luaError = result;
		logS.emplace_back("\n== Error ==\n");
		logS.emplace_back(luaError.what());
	}
}

bool luaP::checkVar(const char* gName, int variable)
{
	if (sol::optional<int> scriptVar = luaState[gName])
		return scriptVar.value() == variable;
	return false;
}

sol::state* luaP::init(std::string& luaFilePath, std::string& modPath)
{
	struct
	{
		sol::table M2TWEOPTable;
		sol::table stratmapTable;
		sol::table objectsTable;
		sol::table cameraTable;
		sol::table gameTable;
		sol::table gameUITable;
	}tables;

	struct
	{
		sol::usertype<uiElement>uiElement;
		sol::usertype<settlementInfoScroll>settlementInfoScroll;
		sol::usertype<settlementTextStrings>settlementTextStrings;
		sol::usertype<uiString>uiString;
	}types;
	
	luaState = {};
	luaPath = modPath + R"(\youneuoy_Data\plugins\lua)";
	luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::debug);

	std::string packagePS = "package.path = '";
	packagePS += modPath;
	packagePS += R"(\youneuoy_Data\plugins\lua\?.lua;')";

	packagePS += "..'";
	packagePS += modPath;
	packagePS += R"(\youneuoy_Data\plugins\lua\redist\?.lua;'..package.path ;)";

	packagePS += "package.cpath = '";
	packagePS += modPath;
	packagePS += R"(\youneuoy_Data\plugins\lua\redist\?.dll;'..package.cpath ;)";


	std::string f = "\\";
	std::string r = "/";
	replaceAll2(packagePS, f, r);

	luaState["print"] = &ourP;
	if (auto funcResult = luaState.script(packagePS); !funcResult.valid())
	{
		sol::error luaError = funcResult;
		MessageBoxA(nullptr, luaError.what(), "Lua package error!", NULL);
		return nullptr;
	}
	
	// This checks the syntax of the script, but does not execute it
	sol::load_result fileRes = luaState.load_file(luaFilePath);
	if (!fileRes.valid())
	{ 
		sol::error luaError = fileRes;
		MessageBoxA(nullptr, luaError.what(), "Lua syntax error!", NULL);
		return nullptr;
	}
	if (sol::protected_function_result result1 = fileRes(); !result1.valid())
	{
		sol::error luaError = result1;
		MessageBoxA(nullptr, luaError.what(), "Lua execution error!", NULL);
		return nullptr;
	}

	characterHelpers::addToLua(luaState);
	characterRecordHelpers::addToLua(luaState);
	factionHelpers::addToLua(luaState);
	settlementHelpers::addToLua(luaState);
	fortHelpers::addToLua(luaState);
	armyHelpers::addToLua(luaState);

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
	@tfield getTileVisibility getTileVisibility
	@tfield setGuildCooldown setGuildCooldown
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
	@tfield getTileMoveCost getTileMoveCost
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

	tables.M2TWEOPTable.set_function("toggleDeveloperMode", &toggleDeveloperMode);
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
	tables.M2TWEOPTable.set_function("logGame", &m2tweopHelpers::logStringGame);
	/***
	Load a savegame.
	@function M2TWEOP.loadGame
	@tparam string path starting from mods
	@usage
	 M2TWEOP.loadGame("mods/bare_geomod/saves/test.sav");
	*/
	tables.M2TWEOPTable.set_function("loadGame", &m2tweopHelpers::loadSaveGame);

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
	tables.M2TWEOPTable.set_function("setEDUUnitsSize", &smallFuncs::setMaxUnitSize);

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
	tables.M2TWEOPTable.set_function("getBattleCamCoords", &battleHelpers::getBattleCamCoords);

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
	tables.M2TWEOPTable.set_function("isTileFree", &stratMapHelpers::isTileFreeLua);
	/***
	Get the selected tile coords.
	@function M2TWEOP.getGameTileCoordsWithCursor
	@treturn int x
	@treturn int y
	@usage
	local x,y=M2TWEOP.getGameTileCoordsWithCursor();
	*/
	tables.M2TWEOPTable.set_function("getGameTileCoordsWithCursor", &m2tweopHelpers::getGameTileCoordsWithCursor);
	tables.M2TWEOPTable.set_function("getTileRegionID", &stratMapHelpers::getTileRegionID);
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
	tables.M2TWEOPTable.set_function("getCampaignDb", &campaignHelpers::getCampaignDb);
	/***
	Get the campaign options.
	@function M2TWEOP.getCampaignDbExtra
	@treturn campaignDbExtra options
	@usage
	local options = M2TWEOP.getCampaignDbExtra();
	*/
	tables.M2TWEOPTable.set_function("getCampaignDbExtra", &campaignHelpers::getCampaignDbExtra);
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
	@function M2TWEOP.getTileMoveCost
	@tparam int originX
	@tparam int originY
	@tparam int targetX only adjacent tiles! Does not calculate paths just the cost of moving from one tile to another.
	@tparam int targetY only adjacent tiles! Does not calculate paths just the cost of moving from one tile to another.
	@treturn float moveCost
	@usage
	local moveCost = M2TWEOP.getTileMoveCost(153, 245, 154, 245);
	*/
	tables.M2TWEOPTable.set_function("getTileMoveCost", &stratMapHelpers::getTileMoveCost);

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

	tables.gameUITable.set_function("getUiElement", &gameSTDUIHelpers::getUiElement);

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
	types.uiElement.set("execute", &gameSTDUIHelpers::useUiElement);


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
	tables.cameraTable.set_function("move", &stratMapHelpers::moveStratCameraSlow);
	/***
	Quickly move the camera to the specified tile.
	@function stratmap.camera.jump
	@tparam int xCoord
	@tparam int yCoord
	@usage
	stratmap.camera.jump(1,2);
	*/
	tables.cameraTable.set_function("jump", &stratMapHelpers::moveStratCameraFast);
	/***
	Set the zoom level of the camera.
	@function stratmap.camera.zoom
	@tparam float distance
	@usage
	stratmap.camera.zoom(0.12);
	*/
	tables.cameraTable.set_function("zoom", &stratMapHelpers::zoomStratCamera);

	///Game
	//@section gameTable


	/***
	Basic stratmap.game table

	@tfield createCharacterByString createCharacterByString
	@tfield createArmy createArmy
	@tfield spawnArmy spawnArmy
	@tfield createArmyInSettlement createArmyInSettlement
	@tfield getScriptCounter getScriptCounter
	@tfield setScriptCounter setScriptCounter
	@tfield historicEvent historicEvent
	@tfield scriptCommand scriptCommand
	@tfield callConsole callConsole

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
	tables.gameTable.set_function("getFactionsCount", &campaignHelpers::getFactionsCount);
	tables.gameTable.set_function("getFaction", &campaignHelpers::getFaction);

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
	@treturn armyStruct newArmy
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
	tables.gameTable.set_function("spawnArmy", &armyHelpers::spawnArmy);
	
	/***
	Legacy code to spawn an army for a created character. Use spawnArmy instead!
	@function stratmap.game.createArmy
	@tparam character ourGeneral Character class, not named character class!
	@treturn armyStruct army
	@usage
	army=stratmap.game.createArmy(gen);
	*/
	tables.gameTable.set_function("createArmy", &armyHelpers::createArmy);
	/***
	Create an army in a settlement (don't need a character). Used to add units to an empty settlement.
	@function stratmap.game.createArmyInSettlement
	@tparam settlementStruct settlement
	@treturn armyStruct army
	@usage
	army=stratmap.game.createArmyInSettlement(STRAT_MAP:getSettlement("London"));
	*/
	tables.gameTable.set_function("createArmyInSettlement", &armyHelpers::createArmyInSettlement);
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
	
	return &luaState;
}

void luaP::checkLuaFunc(sol::function** lRef)
{
	if ((*lRef)->valid() == false)
		*lRef = nullptr;
}

void luaP::fillHashMaps()
{
	const campaign* campaign = campaignHelpers::getCampaignData();
	if (!campaign)
		return;
	for (int i = 0; i < campaign->factionCount; i++)
	{
		if (const auto faction = campaign->factionsSortedByTurnOrder[i])
			factions.insert_or_assign(std::string(faction->factionRecord->facName), faction->factionID);
	}
	const auto stratMap = stratMapHelpers::getStratMap();
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
	m2tweopHelpers::logStringGame("Hashmaps filled");
}
