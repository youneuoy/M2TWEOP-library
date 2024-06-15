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
#include "realGameTypes.h"
#include <windows.h>
#include "luaGetSetFuncs.h"
#include "resourcesHelpers.h"
#include "gameHelpers.h"
#include "stackStructHelpers.h"
#include "m2tweopHelpers.h"
#include "technicalHelpers.h"
#include "eopEduHelpers.h"
#include "siegeHelpers.h"
#include "gameSTDUIHelpers.h"
#include "character.h"
#include "settlement.h"
#include "characterRecord.h"
#include "faction.h"
#include "unit.h"
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
		sol::usertype<eduEntry>eduEntry;
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
		sol::usertype<eventTrigger> eventTrigger;
		sol::usertype<descrMountEntry> mountStruct;
		sol::usertype<projectile> projectileStruct;
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
	factionHelpers::addToLua(luaState);
	unitHelpers::addToLua(luaState);
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
	tables.M2TWEOPTable.set_function("logGame", &m2tweopHelpers::logStringGame);
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
			factions.insert_or_assign(std::string(faction->factionRecord->facName), faction->factionID);
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
