///
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "pch.h"
#include "luaPlugin.h"
#include "console.h"
#include "settlementConversionLvlSetter.h"
#include "stratModelsChange.h"
#include "gameHelpers.h"
#include "character.h"
#include "settlement.h"
#include "characterRecord.h"
#include "faction.h"
#include "army.h"
#include "battle.h"
#include "campaign.h"
#include "campaignDb.h"
#include "cultures.h"
#include "developerMode.h"
#include "eopBuildings.h"
#include "fort.h"
#include "gameUi.h"
#include "graphicsD3D.h"
#include "mapImage.h"
#include "strategyMap.h"
#include "campaignAi.h"
#include "imgui/sol_ImGui.h"
#include "rebelFactions.h"
#include "graphicsEvents.h"

#include "eopVideos.h"
plugData::pDat plugData::data;
std::vector<std::string> luaPlugin::logS;
std::vector<std::string> luaPlugin::logCommands;

int initLuaPlugin(bool isReload)
{
	std::string luaFile = globals::dataS.modPath + R"(\eopData\eopScripts\luaPluginScript.lua)";
	sol::state* luaState = plugData::data.luaAll.init(luaFile, globals::dataS.modPath);
	plugData::data.luaAll.initCampaign();
	plugData::data.luaAll.initLuaEnums();
	plugData::data.luaAll.initCampaignDb();
	plugData::data.luaAll.initUnits();
	plugData::data.luaAll.initEopFbx();
	plugData::data.luaAll.initEop3dObjects();
	plugData::data.luaAll.initEopSounds();
	eopVideos::addToLua(*luaState);
	sol_ImGui::Init(*luaState);

	gameEvents::initEvents();
	if (luaState == nullptr)
	{
		MessageBoxA(nullptr, "LUA loading error!", "Error!", NULL);
		exit(0);
	}
	plugData::data.luaAll.addLegacy();
	plugData::data.luaAll.onPluginLoadF();

	if (isReload) {
		auto bMsg = ImGuiToast(ImGuiToastType_Info, 1000);
		bMsg.set_title("Restarted Lua plugin!");
		bMsg.set_content("");
		ImGui::InsertNotification(bMsg);
	}

	return 1;
}

void luaPlugin::loadLuaScript(std::string& luaFilePath)
{
	UINT defaultFlags = MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION;  // Default type with exclamation icon
	// This checks the syntax of the script, but does not execute it
	sol::load_result fileRes = luaState.load_file(luaFilePath);
	if (!fileRes.valid())
	{
		sol::error luaError = fileRes;
		gameHelpers::logStringGame(luaError.what());
		int result = MessageBoxA(nullptr, luaError.what(), "Lua syntax error!", defaultFlags);
		console::handleMessageBoxResult(result);
		return;
	}
	if (sol::protected_function_result result1 = fileRes(); !result1.valid())
	{
		sol::error luaError = result1;
		gameHelpers::logStringGame(luaError.what());
		int result = MessageBoxA(nullptr, luaError.what(), "Lua execution error!", defaultFlags);
		console::handleMessageBoxResult(result);
		return;
	}
}
void reloadLua()
{
	const std::string luaFile = globals::dataS.modPath + R"(\eopData\eopScripts\luaPluginScript.lua)";
	auto script = plugData::data.luaAll.luaState.load_file(luaFile);
	script();

	auto bMsg = ImGuiToast(ImGuiToastType_Info, 1000);
	bMsg.set_title("Reloaded Lua script!");
	bMsg.set_content("");
	ImGui::InsertNotification(bMsg);
}

//Print function
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
		luaPlugin::logS.push_back(newS);
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

void luaPlugin::runScriptS(const std::string* script)
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

bool luaPlugin::checkVar(const char* gName, int variable)
{
	if (sol::optional<int> scriptVar = luaState[gName])
		return scriptVar.value() == variable;
	return false;
}

struct M2TW
{
	
};

sol::state* luaPlugin::init(std::string& luaFilePath, std::string& modPath)
{
	struct
	{
		sol::table M2TWEOP;
		sol::usertype<M2TW> M2TW;
	}tables;
	
	luaState = {};
	luaPath = modPath + R"(\eopData\eopScripts)";
	luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::debug);

	std::string packagePS = "package.path = '";
	packagePS += modPath;
	packagePS += R"(\eopData\eopScripts\?.lua;')";

	packagePS += "..'";
	packagePS += modPath;
	packagePS += R"(\eopData\eopScripts\redist\?.lua;'..package.path ;)";

	packagePS += "package.cpath = '";
	packagePS += modPath;
	packagePS += R"(\eopData\eopScripts\redist\?.dll;'..package.cpath ;)";


	std::string f = "\\";
	std::string r = "/";
	replaceAll2(packagePS, f, r);
	UINT defaultFlags = MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION;  // Default type with exclamation icon

	luaState["print"] = &ourP;
	if (auto funcResult = luaState.script(packagePS); !funcResult.valid())
	{
		sol::error luaError = funcResult;
		int result = MessageBoxA(nullptr, luaError.what(), "Lua package error!", defaultFlags);
		console::handleMessageBoxResult(result);
		return nullptr;
	}

	plugData::data.luaAll.loadLuaScript(luaFilePath);
	
	///M2TWEOP
	//@section m2tweopTable

	/***
	Basic M2TWEOP table

	@tfield getModPath getModPath
	@tfield getModFolderName getModFolderName
	@tfield toggleConsole toggleConsole
	@tfield reloadScript reloadScript
	@tfield restartLua restartLua
	@tfield toggleDeveloperMode toggleDeveloperMode
	@tfield saveGame saveGame
	@tfield copyFile copyFile
	@tfield getAncillary getAncillary
	@tfield getAncillaryByIndex getAncillaryByIndex
	@tfield getAncillaryCount getAncillaryCount
	@tfield getTrait getTrait
	@tfield getTraitByIndex getTraitByIndex
	@tfield getTraitCount getTraitCount
	@tfield setExpandedString setExpandedString
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
	@tfield getStratHoveredCoords getStratHoveredCoords
	@tfield getBattleHoveredCoords getBattleHoveredCoords
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
	@tfield addModelToGame addModelToGame
	@tfield setModel setModel
	@tfield callConsole callConsole
	@tfield setScriptCounter setScriptCounter
	@tfield getScriptCounter getScriptCounter
	@tfield scriptCommand scriptCommand
	@tfield getEopAiConfig getEopAiConfig
	@tfield getRebelFaction getRebelFaction
	@tfield addBanner addBanner
	@tfield getFactionRecord getFactionRecord
	@tfield getFactionRecordNum getFactionRecordNum
	@tfield hideUnknownUnitTooltips hideUnknownUnitTooltips
	@tfield handleUnitCards handleUnitCards
	@tfield setWatchTowerRange setWatchTowerRange
	@tfield enableFamilyEventsWithoutTree enableFamilyEventsWithoutTree
	@tfield useEopFrontiers useEopFrontiers
	@tfield setKhakiTextColor setKhakiTextColor
	@tfield getMinorSettlementBalance getMinorSettlementBalance
	@tfield generateSprite generateSprite
	@tfield getGroundTypeMoveCost getGroundTypeMoveCost
	@tfield setGroundTypeMoveCost setGroundTypeMoveCost
	@table M2TWEOP
	*/
	
	tables.M2TWEOP = luaState.create_table("M2TWEOP");


	/***
	Function to return the path to the mod folder, starts from the drive not the game folder.
	@function M2TWEOP.getModPath
	@treturn string mod path
	@usage
	mPath=M2TWEOP.getModPath();
	print(mPath);
	*/
	tables.M2TWEOP.set_function("getModPath", &gameHelpers::getModPath);
	
	/***
	Get the name of the mod folder.
	@function M2TWEOP.getModFolderName
	@treturn string mod path
	@usage
	mPath=M2TWEOP.getModFolderName();
	print(mPath);
	*/
	tables.M2TWEOP.set_function("getModFolderName", &gameHelpers::getModFolderName);
	
	/***
	Open/close the lua console.
	@function M2TWEOP.toggleConsole
	@usage
	    M2TWEOP.toggleConsole();
	*/
	tables.M2TWEOP.set_function("toggleConsole", &console::toggleConsole);
	
	/***
	Toggle developer mode.
	@function M2TWEOP.toggleDeveloperMode
	@usage
	    M2TWEOP.toggleDeveloperMode();
	*/
	tables.M2TWEOP.set_function("toggleDeveloperMode", &developerMode::toggleDeveloperMode);
	
	/***
	Reload the lua script (without restarting the plugin itself, onPluginLoad doesn't fire).
	@function M2TWEOP.reloadScript
	@usage
	    M2TWEOP.reloadScript();
	*/
	tables.M2TWEOP.set_function("reloadScript", &reloadScript);
	
	/***
	Restart the lua plugin (onPluginLoad fires).
	@function M2TWEOP.restartLua
	@usage
	    M2TWEOP.restartLua();
	*/
	tables.M2TWEOP.set_function("restartLua", &restartLua);
	
	/***
	Save the game.
	@function M2TWEOP.saveGame
	@tparam string path (start from mods)
	@usage
	M2TWEOP.saveGame("mods/bare_geomod/saves/newsave.sav");
	*/
	tables.M2TWEOP.set_function("saveGame", &gameHelpers::saveGame);
	
	/***
	Get an ancillary by name.
	@function M2TWEOP.getAncillary
	@tparam string name
	@treturn ancillary anc
	@usage
	    local anc = M2TWEOP.getAncillary("ancillary_name");
	*/
	tables.M2TWEOP.set_function("getAncillary", &characterRecordHelpers::findAncillary);
	
	/***
	Get an ancillary by index.
	@function M2TWEOP.getAncillaryByIndex
	@tparam int index
	@treturn ancillary anc
	@usage
	    local anc = M2TWEOP.getAncillaryByIndex(0);
	*/
	tables.M2TWEOP.set_function("getAncillaryByIndex", &characterRecordHelpers::getAncillaryByIndex);
	
	/***
	Get ancillary count.
	@function M2TWEOP.getAncillaryCount
	@treturn int ancNum
	@usage
		local ancNum = M2TWEOP.getAncillaryCount();
	    for i = 0, ancNum - 1 do
	        local anc = M2TWEOP.getAncillary(i)
	    end
	*/
	tables.M2TWEOP.set_function("getAncillaryCount", &characterRecordHelpers::getAncillaryCount);
	
	/***
	Get a trait by name.
	@function M2TWEOP.getTrait
	@tparam string name
	@tparam traitEntry trait
	@usage
	    local trait = M2TWEOP.getTrait("trait_name");
	*/
	tables.M2TWEOP.set_function("getTrait", &characterRecordHelpers::findTrait);

	/***
	Get a trait by index.
	@function M2TWEOP.getTraitByIndex
	@tparam int index
	@treturn traitEntry trait
	@usage
	    local trait = M2TWEOP.getTraitByIndex(0)
	 */
	tables.M2TWEOP.set_function("getTraitByIndex", &characterRecordHelpers::getTraitByIndex);
	
	/***
	Get trait count.
	@function M2TWEOP.getTraitCount
	@treturn int traitNum
	@usage
		local traitNum = M2TWEOP.getTraitCount();
		for i = 0, traitNum - 1 do
			local trait = M2TWEOP.getTraitByIndex(i)
		end
	*/
	tables.M2TWEOP.set_function("getTraitCount", &characterRecordHelpers::getTraitCount);
	
	/***
	Copy a file.
	@function M2TWEOP.copyFile
	@tparam string from
	@tparam string to
	@usage
	    M2TWEOP.copyFile(M2TWEOP.getModPath().."/saves/newsave.sav",  M2TWEOP.getModPath().."/isSaveBackupEnabled/newsave.sav");
	*/
	tables.M2TWEOP.set_function("copyFile", &gameHelpers::copyFileLua);
	
	/***
	Set a string in expanded string table.
	@function M2TWEOP.setExpandedString
	@tparam string key
	@tparam string value
	@usage
	    M2TWEOP.setExpandedString("EMT_TURKS_FACTION_LEADER", "High King");
	*/
	tables.M2TWEOP.set_function("setExpandedString", &gameHelpers::setExpandedString);
	
	/***
	Function to get the game version.
	@function M2TWEOP.getGameVersion
	@treturn int gamever (1 = disk 2 = steam)
	@usage
	M2TWEOP.getGameVersion();
	*/
	tables.M2TWEOP.set_function("getGameVersion", &gameHelpers::getGameVersion);
	
	/***
	Set perfect spy.
	@function M2TWEOP.setPerfectSpy
	@tparam bool set
	@usage
		M2TWEOP.setPerfectSpy(true);
	*/
	tables.M2TWEOP.set_function("setPerfectSpy", &campaignHelpers::setPerfectSpy);
	
	/***
	Get local faction ID.
	@function M2TWEOP.getLocalFactionID
	@treturn int localFaction
	@usage
		local localFaction = M2TWEOP.getLocalFactionID();
	*/
	tables.M2TWEOP.set_function("getLocalFactionID", &campaignHelpers::getLocalFactionID);
	
	/***
	Function to return the path to the plugin (location of your LUA files).
	@function M2TWEOP.getPluginPath
	@treturn string plugin path
	@usage
	plPath=M2TWEOP.getPluginPath();
	print(plPath);
	*/
	tables.M2TWEOP.set_function("getPluginPath", &gameHelpers::getLuaPath);
	
	/***
	Log something to the game log.
	@function M2TWEOP.logGame
	@tparam string message
	@usage
	 M2TWEOP.logGame("log message");
	*/
	tables.M2TWEOP.set_function("logGame", &gameHelpers::logStringLua);
	
	/***
	Load a savegame.
	@function M2TWEOP.loadGame
	@tparam string path starting from mods
	@usage
	 M2TWEOP.loadGame("mods/bare_geomod/saves/test.sav");
	*/
	tables.M2TWEOP.set_function("loadGame", &gameHelpers::loadSaveGame);

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

	tables.M2TWEOP.set_function("loadTexture", &graphicsExport::loadTextureToGame);
	
	/***
	Unload d3d texture
	@function M2TWEOP.unloadTexture
	@tparam int id of the image
	@usage
	local testImage = { x = 0, y = 0, img = nil};
	testImage.x, testImage.y, testImage.img=M2TWEOP.loadTexture(M2TWEOP.getModPath().."/youneuoy_textures/test.dds");
	M2TWEOP.unloadTexture(testImage.img);
	*/
	tables.M2TWEOP.set_function("unloadTexture", &graphicsExport::unloadTextureFromGame);
	
	/***
	Sets the limit of ancillaries per character on the stratmap.
	@function M2TWEOP.setAncillariesLimit
	@tparam int newLimit default: 8, maximum: 127
	@usage
	M2TWEOP.setAncillariesLimit(15);
	*/
	tables.M2TWEOP.set_function("setAncillariesLimit", &gameHelpers::setAncLimit);

	/***
	Unlocks all console commands, also allows the use of the 'control' command to change factions in singleplayer campaigns.
	@function M2TWEOP.unlockGameConsoleCommands
	@usage
	M2TWEOP.unlockGameConsoleCommands();
	*/
	tables.M2TWEOP.set_function("unlockGameConsoleCommands", &gameHelpers::unlockConsoleCommands);
	
	/***
	Sets the maximum amount of soldiers a general's bodyguard unit can replenish to. The value is multiplied by the unit size modifiers (e.g Huge = 2.5 multiplier)
	@function M2TWEOP.setMaxBgSize
	@tparam int newSize
	@usage
	M2TWEOP.setMaxBgSize(100) -- On huge unit size, 100*2.5 = 250 max bodyguard size
	M2TWEOP.setMaxBgSize(150) -- On huge unit size, 150*2.5 = 300 max bodyguard size
	M2TWEOP.setMaxBgSize(50)  -- On huge unit size, 50*2.5 = 125 max bodyguard size
	*/
	tables.M2TWEOP.set_function("setMaxBgSize", &gameHelpers::setMaxBgSize);

	/***
	Sets the new maximum soldier count.
	@function M2TWEOP.setEDUUnitsSize
	@tparam int minSize maximum: 300
	@tparam int maxSize maximum: 300
	@usage
	M2TWEOP.setEDUUnitsSize(1,300);
	*/
	tables.M2TWEOP.set_function("setEDUUnitsSize", &gameHelpers::setMaxUnitSize);

	/***
	Set a size limit for UI texture cache. The new maximum size is 2048 x 2048. This also changes the maximum size for map_regions.tga. Don't use regions that are too gigantic!   
	@function M2TWEOP.setTextureCacheSize
	@usage
	M2TWEOP.setTextureCacheSize();
	*/
	tables.M2TWEOP.set_function("setTextureCacheSize", &gameHelpers::setTextureCacheSize);

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
	tables.M2TWEOP.set_function("getSettlementInfoScroll", &gameUiHelpers::getSettlementInfoScroll);

	/***
	* Sets the new maximum amount of building levels within a chain.
	* @function M2TWEOP.setBuildingChainLimit
	* @tparam int limit default: 9, maximum: 56
	* @usage
	* M2TWEOP.setBuildingChainLimit(12);
	*/
	tables.M2TWEOP.set_function("setBuildingChainLimit", &gameHelpers::setBuildingChainLimit);

	/***
	* Sets which level of castle converts to which level of city.
	* @function M2TWEOP.setConversionLvlFromCastle
	* @tparam int castleLvl 0-5
	* @tparam int convertToLvl 0-6, 6 means conversion from that level is disabled
	* @usage
	* M2TWEOP.setConversionLvlFromCastle(3,3)
	*/
	tables.M2TWEOP.set_function("setConversionLvlFromCastle", &settlementConversionLvlSetter::setConversionLvlFromCastle);

	/***
	* Sets which level of city converts to which level of castle.
	* @function M2TWEOP.setConversionLvlFromCity
	* @tparam int cityLvl 0-5
	* @tparam int convertToLvl 0-6, 6 means conversion from that level is disabled
	* @usage
	* M2TWEOP.setConversionLvlFromCity(3,3)
	*/
	tables.M2TWEOP.set_function("setConversionLvlFromCity", &settlementConversionLvlSetter::setConversionLvlFromCity);

	/***
	* Sets the minimum number of turns until the next guild offer after a rejection by the player.
	* @function M2TWEOP.setGuildCooldown
	* @tparam int turns default: 10
	* @usage
	* M2TWEOP.setGuildCooldown(5)
	*/
	tables.M2TWEOP.set_function("setGuildCooldown", &gameHelpers::setGuildCooldown);
	
	/***
	Toggle the highlighting of units on the tactical map.
	@function M2TWEOP.toggleUnitsBMapHighlight
	@usage
	M2TWEOP.toggleUnitsBMapHighlight();
	*/
	tables.M2TWEOP.set_function("toggleUnitsBMapHighlight", &gameHelpers::toggleUnitHighlight);

	/***
	Get the current x, y and z coords of the battlemap camera
	@function M2TWEOP.getBattleCamCoords
	@treturn battleCameraStruct Camera struct
	@usage
	local cameraCoords = M2TWEOP.getBattleCamCoords();
	-- Zoom out the camera beyond it's normal range
	cameraCoords.zCoord = 500;
	*/
	tables.M2TWEOP.set_function("getBattleCamCoords", &battleHelpers::getBattleCamCoords);

	/***
	Set the maximum number of religions in the mod (per descr\_religions.txt). Do not use religions > 10 in CombatVsReligion attributes!
	@function M2TWEOP.setReligionsLimit
	@tparam int newLimit maximum: 127
	@usage
	M2TWEOP.setReligionsLimit(25);
	*/
	tables.M2TWEOP.set_function("setReligionsLimit", &gameHelpers::setReligionsLimit);

	/***
	Set the siege points required to make different siege equipment.
	@function M2TWEOP.setEquipmentCosts
	@tparam int equipmentType 0 = ram, 1 = ladder, 2 = siege tower
	@tparam int newCost
	@usage
		M2TWEOP.setEquipmentCosts(0, 25);
	*/
	tables.M2TWEOP.set_function("setEquipmentCosts", &gameHelpers::setEquipmentCosts);

	/***
	Checks if a tile is free.
	@function M2TWEOP.isTileFree
	@tparam int X coordinate of the tile.
	@tparam int Y coordinate of the tile.
	@return boolean isFree
	@usage
		M2TWEOP.isTileFree(55,25);
	*/
	tables.M2TWEOP.set_function("isTileFree", &stratMapHelpers::isTileFreeLua);
	
	/***
	Get the hovered tile coordinates on the strategy map.
	@function M2TWEOP.getStratHoveredCoords
	@treturn int x
	@treturn int y
	@usage
		local x,y=M2TWEOP.getStratHoveredCoords();
	*/
	tables.M2TWEOP.set_function("getStratHoveredCoords", &stratMapHelpers::getStratHoveredCoords);
	
	
	/***
	Get the hovered position coordinates on the battle map.
	@function M2TWEOP.getBattleHoveredCoords
	@treturn float x
	@treturn float y
	@treturn float z
	@usage
		local x,y,z = M2TWEOP.getBattleHoveredCoords();
	*/
	tables.M2TWEOP.set_function("getBattleHoveredCoords", &stratMapHelpers::getBattleHoveredCoords);
	tables.M2TWEOP.set_function("getGameTileCoordsWithCursor", &stratMapHelpers::getStratHoveredCoords);
	tables.M2TWEOP.set_function("getTileRegionID", &stratMapHelpers::getTileRegionID);
	
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
	tables.M2TWEOP.set_function("getTileVisibility", &factionHelpers::getTileVisibility);
	tables.M2TWEOP.set_function("getRegionOwner", &stratMapHelpers::getRegionOwner);
	
	/***
	Get religion name by index.
	@function M2TWEOP.getReligionName
	@tparam int index
	@treturn string name
	@usage
	local religionName = M2TWEOP.getReligionName(1);
	*/
	tables.M2TWEOP.set_function("getReligionName", &gameHelpers::getReligionName);
	
	/***
	Get the amount of religions.
	@function M2TWEOP.getReligionCount
	@treturn int religionCount
	@usage
	local religionCount = M2TWEOP.getReligionCount();
	*/
	tables.M2TWEOP.set_function("getReligionCount", &gameHelpers::getReligionCount);
	
	/***
	Get culture name by index.
	@function M2TWEOP.getCultureName
	@tparam int index
	@treturn string name
	@usage
	local name = M2TWEOP.getCultureName(1);
	*/
	tables.M2TWEOP.set_function("getCultureName", &gameHelpers::getCultureName);
	
	/***
	Get climate name by index.
	@function M2TWEOP.getClimateName
	@tparam int index
	@treturn string name
	@usage
	local name = M2TWEOP.getClimateName(1);
	*/
	tables.M2TWEOP.set_function("getClimateName", &gameHelpers::getClimateName);
	
	/***
	Get religion ID by name.
	@function M2TWEOP.getReligion
	@tparam string name
	@treturn int index
	@usage
	local religion = M2TWEOP.getReligion("catholic");
	*/
	tables.M2TWEOP.set_function("getReligion", &gameHelpers::getReligionN);
	
	/***
	Get culture ID by name.
	@function M2TWEOP.getCultureID
	@tparam string name
	@treturn int index
	@usage
	local cultureID = M2TWEOP.getCultureID("southern_european");
	*/
	tables.M2TWEOP.set_function("getCultureID", &gameHelpers::getCultureN);
	
	/***
	Get climate ID by name.
	@function M2TWEOP.getClimateID
	@tparam string name
	@treturn int index
	@usage
	local climateID = M2TWEOP.getClimateID("sandy_desert");
	*/
	tables.M2TWEOP.set_function("getClimateID", &gameHelpers::getClimateN);
	
	/***
	Get some game options.
	@function M2TWEOP.getOptions1
	@treturn options1 options
	@usage
	local options = M2TWEOP.getOptions1();
	*/
	tables.M2TWEOP.set_function("getOptions1", &gameHelpers::getOptions1);
	
	/***
	Get some game options.
	@function M2TWEOP.getOptions2
	@treturn options2 options
	@usage
		local options = M2TWEOP.getOptions2();
	*/
	tables.M2TWEOP.set_function("getOptions2", &gameHelpers::getOptions2);
	
	/***
	Get the campaign difficulty modifiers.
	@function M2TWEOP.getCampaignDifficulty1
	@treturn campaignDifficulty1 options
	@usage
		local modifiers = M2TWEOP.getCampaignDifficulty1();
	*/
	tables.M2TWEOP.set_function("getCampaignDifficulty1", &campaignHelpers::getCampaignDifficulty1);
	
	/***
	Get the campaign difficulty modifiers.
	@function M2TWEOP.getCampaignDifficulty2
	@treturn campaignDifficulty2 options
	@usage
		local modifiers = M2TWEOP.getCampaignDifficulty2();
	*/
	tables.M2TWEOP.set_function("getCampaignDifficulty2", &campaignHelpers::getCampaignDifficulty2);
	
	/***
	Get the campaign options.
	@function M2TWEOP.getCampaignDb
	@treturn campaignDb options
	@usage
		local options = M2TWEOP.getCampaignDb();
	*/
	tables.M2TWEOP.set_function("getCampaignDb", &campaignHelpers::getCampaignDb);
	
	/***
	Get the campaign options.
	@function M2TWEOP.getCampaignDbExtra
	@treturn campaignDbExtra options
	@usage
		local options = M2TWEOP.getCampaignDbExtra();
	*/
	tables.M2TWEOP.set_function("getCampaignDbExtra", &campaignHelpers::getCampaignDbExtra);
	
	/***
	Add a new .cas campaign strategy model to the game with a unique ID. This should be called during onPluginLoad()
	@function M2TWEOP.addModelToGame
	@tparam string path Relative path from the modfolder (starting with "data/").
	@tparam int modelId  Unique ID to use the model later.
	@tparam bool isSettlement optional
	@usage
		M2TWEOP.addModelToGame("data/models_strat/residences/invisible.CAS",1);
	*/
	tables.M2TWEOP.set_function("addModelToGame", sol::overload(
				sol::resolve<void(const std::string&, uint32_t)>(stratModelsChange::addModelToGameNoBool),
				sol::resolve<void(const std::string&, uint32_t, bool)>(stratModelsChange::addModelToGame)
			));
	
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
	tables.M2TWEOP.set_function("condition", &gameHelpers::conditionLua);
	
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
	tables.M2TWEOP.set_function("getTileMoveCost", &stratMapHelpers::getTileMoveCost);
	
	/***
	Set the strategy model for object at specified coordinates, works only for supported object types
	@function M2TWEOP.setModel
	@tparam int xCoord
	@tparam int yCoord
	@tparam int modelId used for: watchtower, resource, settlement, fort, port
	@tparam int modelId2 used for: fort wall (use fort coords), dock (use port coords)
	@usage
		M2TWEOP.addModelToGame("data/models_strat/residences/invisible.CAS",1)
		M2TWEOP.setModel(288,257,1,1)
	*/
	tables.M2TWEOP.set_function("setModel", sol::overload(&stratModelsChange::setModel,&stratModelsChange::setModelOneVar));

	/***
	Execute a Medieval II console command.
	@function M2TWEOP.callConsole
	@tparam string command
	@tparam string args
	@treturn string error Note: string can be empty but not nil
	@usage
	-- Creating units, adding money
		function onCharacterSelected(eventData)
			local selectedChar = eventData.character
			local err = M2TWEOP.callConsole("add_money", "2321")
			local err2 = M2TWEOP.callConsole("create_unit", "testcharacter 'Cool Unit' 4 1 1 1")
			print(err)
			print(err2)
		end
	*/
	tables.M2TWEOP.set_function("callConsole", &gameHelpers::callConsole);
	
	/***
	Set an event\_counter value, does not work for counters, only event\_counters.
	@function M2TWEOP.setScriptCounter
	@tparam string counterName
	@tparam int value
	@usage
		M2TWEOP.setScriptCounter("SomeCounter", 25)
	*/
	tables.M2TWEOP.set_function("setScriptCounter", &gameHelpers::setScriptCounter);
	
	/***
	Get a script counter value, works for counters and for event\_counters
	@function M2TWEOP.getScriptCounter
	@tparam string counterName The name of the counter
	@treturn int counterValue Returns the value of the counter
	@usage
 	---@param eventData eventTrigger
		function onEventCounter(eventData)
    			-- get the name and value of the triggered counter
    			local counterName = eventData.eventCounter
    			local counterValue =  M2TWEOP.getScriptCounter(counterName)
    			print('counter : '..counterName..' has been set to : '..counterValue)
		end
	*/
	tables.M2TWEOP.set_function("getScriptCounter", &gameHelpers::getScriptCounterNoBool);
	
	/***
	Get AI config.
	@function M2TWEOP.getEopAiConfig
	@treturn eopAiConfig config
	@usage
		local config = M2TWEOP.getEopAiConfig()
	*/
	tables.M2TWEOP.set_function("getEopAiConfig", &globalEopAiConfig::getInstanceLua);
	
	/***
	Get a rebel faction.
	@function M2TWEOP.getRebelFaction
	@tparam string name
	@treturn eopRebelFaction rebelFac
	@usage
		local bandits = M2TWEOP.getRebelFaction("Bandits")
	*/
	tables.M2TWEOP.set_function("getRebelFaction", &eopRebelFactionDb::getRebelFactionLua);
	
	/***
	Get a faction record.
	@function M2TWEOP.getFactionRecord
	@tparam int factionID
	@treturn factionRecord facRecord
	@usage
		local facRecord = M2TWEOP.getFactionRecord(sett.creatorFactionID)
	*/
	tables.M2TWEOP.set_function("getFactionRecord", &factionHelpers::getFactionRecord);
	
	/***
	Get amount of factions in descr_sm_factions.
	@function M2TWEOP.getFactionRecordNum
	@treturn int facNum
	@usage
		local facNum = M2TWEOP.getFactionRecordNum()
	*/
	tables.M2TWEOP.set_function("getFactionRecordNum", &factionHelpers::getFactionRecordNum);
	
	/***
	Hides tooltips for unknown units, only use if you use empty card instead of question mark as the UI. Disabled by default.
	@function M2TWEOP.hideUnknownUnitTooltips
	@tparam bool set
	@usage
		M2TWEOP.hideUnknownUnitTooltips(true)
	*/
	tables.M2TWEOP.set_function("hideUnknownUnitTooltips", &m2tweopOptions::setHideUnknownUnitTooltips);
	
	/***
	Recalculates frontier tiles for the AI using EOP algorithms.
	@function M2TWEOP.useEopFrontiers
	@tparam bool set
	@usage
		M2TWEOP.useEopFrontiers(true)
	*/
	tables.M2TWEOP.set_function("useEopFrontiers", &m2tweopOptions::setUseEopFrontiers);
	
	/***
	Faction specific unit cards are always chosen if found. Enabled by default.
	@function M2TWEOP.handleUnitCards
	@tparam bool set
	@usage
		M2TWEOP.handleUnitCards(true)
	*/
	tables.M2TWEOP.set_function("handleUnitCards", &m2tweopOptions::setHandleUnitCards);

	/***
	Set watchtower range. Default: 10.
	@function M2TWEOP.setWatchTowerRange
	@tparam int newRange
	@usage
		M2TWEOP.setWatchTowerRange(20)
	*/
	tables.M2TWEOP.set_function("setWatchTowerRange", &m2tweopOptions::setWatchTowerRange);
	
	/***
	Factions without a family tree or a teutonic one still get marriage offers and produce children. Enabled by default.
	@function M2TWEOP.enableFamilyEventsWithoutTree
	@tparam bool set
	@usage
		M2TWEOP.enableFamilyEventsWithoutTree(true)
	*/
	tables.M2TWEOP.set_function("enableFamilyEventsWithoutTree", &m2tweopOptions::setEnableFamilyEventsForTeutonic);
	
	/***
	Change color of default khaki text.
	@function M2TWEOP.setKhakiTextColor
	@tparam int r
	@tparam int g
	@tparam int b
	@usage
		M2TWEOP.setKhakiTextColor(0,0,0)
	*/
	tables.M2TWEOP.set_function("setKhakiTextColor", &m2tweopOptions::setKhakiTextColor);
	
	/***
	Get modifiers for minor settlements for income and growth.
	@function M2TWEOP.getMinorSettlementBalance
	@treturn minorSettlementBalance modifiers
	@usage
		local modifiers = M2TWEOP.getMinorSettlementBalance()
	*/
	tables.M2TWEOP.set_function("getMinorSettlementBalance", &minorSettlementDb::getModifiers);
	
	/***
	Generate sprites for a battle model entry in the bmdb. There needs to be a export/unit_sprites folder in your vanilla Medieval II directory. The game will close after generating the sprites.
	@function M2TWEOP.generateSprite
	@tparam string modelName
	@usage
		M2TWEOP.generateSprite("peasants")
	*/
	tables.M2TWEOP.set_function("generateSprite", &gameHelpers::generateSprite);
	
	/***
	Get the move cost of a ground type.
	@function M2TWEOP.getGroundTypeMoveCost
	@tparam int groundType
	@treturn float moveCost
	@usage
		local cost = M2TWEOP.getGroundTypeMoveCost(strategyGroundType.hills)
	*/
	tables.M2TWEOP.set_function("getGroundTypeMoveCost", &stratMapHelpers::getGroundTypeMoveCost);
	
	/***
	Set the move cost of a ground type.
	@function M2TWEOP.setGroundTypeMoveCost
	@tparam int groundType
	@tparam float moveCost
	@usage
		M2TWEOP.setGroundTypeMoveCost(strategyGroundType.hills, 11.0)
	*/
	tables.M2TWEOP.set_function("setGroundTypeMoveCost", &stratMapHelpers::setGroundTypeMoveCost);
	
	/***
	Add a banner symbol. Add them onCampaignMapLoaded or later!
	@function M2TWEOP.addBanner
	@tparam string name
	@tparam string filePath starting from mods, path to the tga file containing the symbol. You need to add this file to descr_standards.txt so the game loads it!
	@tparam float topLeftX As percentage of the picture width, for example 0.5 for the middle
	@tparam float topLeftY As percentage of the picture width, for example 0.5 for the middle
	@tparam float bottomRightX As percentage of the picture width, for example 0.5 for the middle
	@tparam float bottomRightY As percentage of the picture width, for example 0.5 for the middle
	@usage
		   M2TWEOP.addBanner("Bandits_Banner", "mods/ago_beta/data/banners/symbols10.tga", 0.0, 0.0, 0.5, 0.5)
	*/
	tables.M2TWEOP.set_function("addBanner", &eopBannerSymbols::addBanner);
	
	/***
	Fire any script command available from the game. It is always just 2 parameters in the function, the command name and all the arguments as 1 string in the second parameter.
	Do not use inc_counter, set_counter, declare_counter! they crash!
	@function M2TWEOP.scriptCommand
	@tparam string command
	@tparam string args
	@usage
	M2TWEOP.scriptCommand("give_everything_to_faction", "france england false")
	M2TWEOP.scriptCommand("send_character_off_map", "Rufus")

	-- Multiline commands and using variables in command strings
	local facName="hre"
	M2TWEOP.scriptCommand("set_faction_banner", string.format([[

		faction england

		banner %s

	end_set_faction_banner]], facName))
	*/
	tables.M2TWEOP.set_function("scriptCommand", &gameHelpers::scriptCommandLua);
	
	///M2TW
	//@section M2TW

	/***
	Global game structures.

	@tfield battleStruct battle
	@tfield campaignStruct campaign
	@tfield uiCardManager uiCardManager
	@tfield stratMap stratMap
	@tfield selectionInfo selectionInfo
	
	@table M2TW
	*/
	tables.M2TW = luaState.new_usertype<M2TW>("M2TW");
	tables.M2TW.set("battle", sol::property(&battleHelpers::getBattleData));
	tables.M2TW.set("campaign", sol::property(&campaignHelpers::getCampaignData));
	tables.M2TW.set("stratMap", sol::property(&stratMapHelpers::getStratMap));
	tables.M2TW.set("uiCardManager", sol::property(&gameUiHelpers::getUiCardManager));
	tables.M2TW.set("selectionInfo", sol::property(&campaignHelpers::getSelectionInfo));

	characterHelpers::addToLua(luaState);
	characterRecordHelpers::addToLua(luaState);
	factionHelpers::addToLua(luaState);
	settlementHelpers::addToLua(luaState);
	fortHelpers::addToLua(luaState);
	armyHelpers::addToLua(luaState);
	campaignAi::addToLua(luaState);
	rebels::addToLua(luaState);
	return &luaState;
}

void luaPlugin::initCampaign()
{
	stratMapHelpers::addToLua(luaState);
	campaignHelpers::addToLua(luaState);
	buildingHelpers::addToLua(luaState);
	battleHelpers::addToLua(luaState);
	gameUiHelpers::addToLua(luaState);
	gameHelpers::addToLua(luaState);
	mapImageManager::addToLua(luaState);
}

void luaPlugin::checkLuaFunc(sol::function** lRef)
{
	if ((*lRef)->valid() == false)
		*lRef = nullptr;
}

void luaPlugin::fillHashMaps()
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
	if (!stratMap || !stratMap->isOpen)
		return;
	for (int i = 0; i < stratMap->regionsNum; i++)
	{
		const auto region = &stratMap->regions[i];
		if (region)
			regions.insert_or_assign(std::string(region->regionName), i);
		const int settCount = region->settlementCount();
		for (int j = 0; j < settCount; j++)
		{
			if (const auto settlement = region->getSettlement(j); settlement && settlement->name)
				settlements.insert_or_assign(std::string(settlement->name), i);
		}
	}
	for (int i = 0; i < stratMap->climates->climatesNum; i++)
	{
		if (auto climateName = gameHelpers::getClimateName2(i))
		{
			climateNames.insert_or_assign(i, climateName);
			climateIndex.insert_or_assign(std::string(climateName), i);
		}
	}
	fillHashMapsNonCampaign();
	hashLoaded = true;
	gameHelpers::logStringGame("Hashmaps filled");
}

void luaPlugin::fillHashMapsNonCampaign()
{
	for (int i = 0; i < gameHelpers::getReligionCount(); i++)
	{
		if (auto religionName = gameHelpers::getReligionName2(i))
		{
			religionNames.insert_or_assign(i, religionName);
			religionIndex.insert_or_assign(std::string(religionName), i);
		}
	}
	const auto cultureDb = cultures::getCultureDb();
	for (int i = 0; i < cultureDb->culturesCount; i++)
	{
		const auto culture = &cultureDb->cultures[i];
		cultureNames.insert_or_assign(culture->cultureID, culture->cultureName);
		cultureIndex.insert_or_assign(std::string(culture->cultureName), culture->cultureID);
	}
	const auto edb = eopBuildings::getEdb();
	const int entryNum = edb->getBuildingNum();
	for (int i = 0; i < entryNum; i++)
	{
		const auto building = edb->buildings.get(i);
		buildings.insert_or_assign(std::string(building->type), building->buildingID);
		const int levelNum = building->buildingLevelCount;
		for (int j = 0; j < levelNum; j++)
		{
			const auto level = &building->levels[j];
			buildingLevels.insert_or_assign(std::string(level->name), j);
			buildingLevelLines.insert_or_assign(std::string(level->name), building->buildingID);
		}
	}
	hashNonCampaignLoaded = true;
}

void luaPlugin::fillHashMapsVnv()
{
	const auto traitsDb = characterRecordHelpers::getTraitDb();
	const auto ancDb = characterRecordHelpers::getAncillaryDb();
	if (!traitsDb || !ancDb)
		return;
	const int traitNum = traitsDb->traitsNum;
	const int ancNum = ancDb->ancillariesNum;
	if (traitNum == 0 || ancNum == 0)
		return;
	for (int i = 0; i < traitNum; i++)
	{
		const auto trait = &traitsDb->traits[i];
		traits.insert_or_assign(std::string(trait->name), i);
	}
	for (int i = 0; i < ancNum; i++)
	{
		const auto anc = &ancDb->ancillaries[i];
		ancillaries.insert_or_assign(std::string(anc->ancName), i);
	}
	hashVnvLoaded = true;
}
