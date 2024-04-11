///
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"

#include "console.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
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
		sol::usertype<general>character;
		sol::usertype<namedCharacter>namedCharacter;
		sol::usertype<capturedFactionInfo>capturedFactionInfo;
		sol::usertype<capturedUnit>capturedUnit;
		sol::usertype<capturedCharacter>capturedCharacter;
		sol::usertype<ancillary>ancillary;
		sol::usertype<traitContainer>traitContainerT;
		sol::usertype<eduEntry>eduEntry;
		sol::usertype<factionStruct>factionStruct;
		sol::usertype<factionStratMapDescrS>factionStratMapStruct;
		sol::usertype<watchTowerStruct>watchtowerStruct;
		sol::usertype<fortStruct>fortStruct;
		sol::usertype<portBuildingStruct>portStruct;
		sol::usertype<portDockStrat>dockStruct;
		sol::usertype<settlementStruct>settlementStruct;
		sol::usertype<guild>guild;
		sol::usertype<resStrat>tradeResource;
		sol::usertype<stackStruct>stackStruct;
		sol::usertype<building>building;
		sol::usertype<buildingsQueue>buildingsQueue;
		sol::usertype<buildingInQueue>buildingInQueue;
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
		sol::usertype<settlementCapability>settlementCapability;
		sol::usertype<recruitmentCapability>recruitmentCapability;
		sol::usertype<settlementRecruitmentPool>settlementRecruitmentPool;
		sol::usertype<battleFactionCounter>battleFactionCounter;
		sol::usertype<eventTrigger> eventTrigger;
		sol::usertype<descrMountEntry> mountStruct;
		sol::usertype<projectile> projectileStruct;
		sol::usertype<settlementStats> settlementStats;
		sol::usertype<aiFaction> aiFaction;
		sol::usertype<aiLongTermGoalDirector> aiLongTermGoalDirector;
		sol::usertype<aiPersonalityValues> aiPersonality;
		sol::usertype<aiGlobalStrategyDirector> aiGlobalStrategyDirector;
		sol::usertype<decisionValuesLTGD> decisionValuesLTGD;
		sol::usertype<aiProductionController> aiProductionController;
		sol::usertype<ltgdFactionValues> aiFactionValues;
		sol::usertype<interFactionLTGD> interFactionLTGD;
		sol::usertype<trait> traitStruct;
		sol::usertype<traitEntry> traitEntry;
		sol::usertype<traitLevel> traitLevel;
		sol::usertype<traitEffect> traitEffect;
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
	@tfield getSettlementInfoScroll getSettlementInfoScroll
	@tfield getReligion getReligion
	@tfield condition condition
	@tfield getOptions1 getOptions1
	@tfield getOptions2 getOptions2
	@tfield getCampaignDifficulty1 getCampaignDifficulty1
	@tfield getCampaignDifficulty2 getCampaignDifficulty2
	@tfield setConversionLvlFromCastle setConversionLvlFromCastle
	@tfield setConversionLvlFromCity setConversionLvlFromCity
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

	tables.M2TWEOPTable.set_function("toggleDeveloperMode", &m2tweopHelpers::toggleDeveloperMode);
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
	M2TWEOP.saveGame(mods/bare_geomod/saves/newsave.sav);
	*/
	tables.M2TWEOPTable.set_function("saveGame", &gameHelpers::saveGame);
	/***
	Function to get the game version.
	@function M2TWEOP.getGameVersion
	@treturn int gamever (1 = disk 2 = steam)
	@usage
	M2TWEOP.getGameVersion();
	*/
	tables.M2TWEOPTable.set_function("getGameVersion", &m2tweopHelpers::getGameVersion);
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
	tables.M2TWEOPTable.set_function("setAncillariesLimit", &m2tweopHelpers::setAncLimit);
	/***
	Unlocks all console commands, also allows the use of the 'control' command to change factions in singleplayer campaigns.
	@function M2TWEOP.unlockGameConsoleCommands
	@usage
	M2TWEOP.unlockGameConsoleCommands();
	*/
	tables.M2TWEOPTable.set_function("unlockGameConsoleCommands", &m2tweopHelpers::unlockGameConsoleCommands);
	/***
	Sets the maximum amount of soldiers a general's bodyguard unit can replenish to. The value is multiplied by the unit size modifiers (e.g Huge = 2.5 multiplier)
	@function M2TWEOP.setMaxBgSize
	@tparam int newSize
	@usage
	M2TWEOP.setMaxBgSize(100) -- On huge unit size, 100*2.5 = 250 max bodyguard size
	M2TWEOP.setMaxBgSize(150) -- On huge unit size, 150*2.5 = 300 max bodyguard size
	M2TWEOP.setMaxBgSize(50)  -- On huge unit size, 50*2.5 = 125 max bodyguard size
	*/
	tables.M2TWEOPTable.set_function("setMaxBgSize", &m2tweopHelpers::setMaxBgSize);

	/***
	Sets the new maximum soldier count.
	@function M2TWEOP.setEDUUnitsSize
	@tparam int minSize maximum: 300
	@tparam int maxSize maximum: 300
	@usage
	M2TWEOP.setEDUUnitsSize(1,300);
	*/
	tables.M2TWEOPTable.set_function("setEDUUnitsSize", &m2tweopHelpers::setEDUUnitsSize);

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

	tables.M2TWEOPTable.set_function("setBuildingChainLimit", &m2tweopHelpers::setBuildingChainLimit);

	/***
	* Sets which level of castle converts to which level of city.
	* @function M2TWEOP.setConversionLvlFromCastle
	* @tparam int castleLvl 0-5
	* @tparam int convertToLvl 0-6, 6 means conversion from that level is disabled
	* @usage
	* M2TWEOP.setConversionLvlFromCastle(3,3)
	*/
	tables.M2TWEOPTable.set_function("setConversionLvlFromCastle", &m2tweopHelpers::setConversionLvlFromCastle);

	/***
	* Sets which level of city converts to which level of castle.
	* @function M2TWEOP.setConversionLvlFromCity
	* @tparam int cityLvl 0-5
	* @tparam int convertToLvl 0-6, 6 means conversion from that level is disabled
	* @usage
	* M2TWEOP.setConversionLvlFromCity(3,3)
	*/
	tables.M2TWEOPTable.set_function("setConversionLvlFromCity", &m2tweopHelpers::setConversionLvlFromCity);

	/***
	* Sets the minimum number of turns until the next guild offer after a rejection by the player.
	* @function M2TWEOP.setGuildCooldown
	* @tparam int turns default: 10
	* @usage
	* M2TWEOP.setGuildCooldown(5)
	*/
	tables.M2TWEOPTable.set_function("setGuildCooldown", &m2tweopHelpers::setGuildCooldown);


	/***
	Toggle the highlighting of units on the tactical map.
	@function M2TWEOP.toggleUnitsBMapHighlight
	@usage
	M2TWEOP.toggleUnitsBMapHighlight();
	*/
	tables.M2TWEOPTable.set_function("toggleUnitsBMapHighlight", &m2tweopHelpers::toggleUnitsBMapHighlight);

	/***
	Get the current x, y and z coords of the battlemap camera
	@function M2TWEOP.getBattleCamCoords
	@treturn battleCameraStruct Camera struct
	@usage
	local cameraCoords = M2TWEOP.getBattleCamCoords();
	-- Zoom out the camera beyond it's normal range
	cameraCoords.zCoord = 500;
	*/
	tables.M2TWEOPTable.set_function("getBattleCamCoords", &m2tweopHelpers::getBattleCamCoords);

	/***
	Set the maximum number of religions in the mod (per descr\_religions.txt). Do not use religions > 10 in CombatVsReligion attributes!
	@function M2TWEOP.setReligionsLimit
	@tparam int newLimit maximum: 127
	@usage
	M2TWEOP.setReligionsLimit(25);
	*/
	tables.M2TWEOPTable.set_function("setReligionsLimit", &m2tweopHelpers::seReligionsLimit);

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
	tables.M2TWEOPTable.set_function("getTileRegionID", &m2tweopHelpers::getTileRegionID);
	/***
	Get a specific tile's visibility according to faction (i.e can a faction see a tile) Note: Once the tile has been seen by a faction, it will always return true. e.g If you have spotted a settlement but it is now outside of the fog of war, it will still be classed as visible. 
	@function M2TWEOP.getTileVisibility
	@tparam factionStruct faction Faction to check
	@tparam int xCoord x coord of the tile
	@tparam int yCoord y coord of the tile
	@treturn bool isVisible true = visible, false = not visible
	@usage
	local faction = stratmap.game.getFaction(2);
	local isVisible = M2TWEOP.getTileVisibility(faction, xCoord, yCoord)
	if isVisible == true then 
		print("Tile is visible to faction "..faction:getFactionName())
	else
		print("Tile is not visible to faction "..faction:getFactionName())
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
	Get religion ID by name.
	@function M2TWEOP.getReligion
	@tparam string name
	@treturn int index
	@usage
	local religion = M2TWEOP.getReligion("catholic");
	*/
	tables.M2TWEOPTable.set_function("getReligion", &gameHelpers::getReligionN);
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
	@function uiElementStruct.getSubElement
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
	tables.objectsTable.set_function("startDrawModelAt", &objectsHelpers::startDrawModelAt);
	/***
	Stop drawing .cas campaign strategy model with a unique ID. Can be used at any time.
	@function stratmap.objects.stopDrawModel
	@tparam int modelId Unique ID
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS", 1);
	stratmap.objects.startDrawModelAt(1, 50, 150, 2.25);
	stratmap.objects.stopDrawModel(1);
	*/
	tables.objectsTable.set_function("stopDrawModel", &objectsHelpers::stopDrawModel);
	/***
	Add a new .cas campaign strategy model to the game with a unique ID. This should be called during onPluginLoad()
	@function stratmap.objects.addModelToGame
	@tparam string path Relative path from the modfolder (starting with "data/").
	@tparam int modelId  Unique ID to use the model later.
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS",1);
	*/
	tables.objectsTable.set_function("addModelToGame", &objectsHelpers::addModelToGame);
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
	tables.objectsTable.set_function("addCharacterCas", &generalHelpers::addCharacterCas);
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
	tables.objectsTable.set_function("setModel", sol::overload(&objectsHelpers::setModel,
		&objectsHelpers::setModelOneVar));
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
	tables.objectsTable.set_function("replaceTile", &objectsHelpers::replaceTile);

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
	tables.cameraTable.set_function("move", &cameraHelpers::moveStratCamera);
	/***
	Quickly move the camera to the specified tile.
	@function stratmap.camera.jump
	@tparam int xCoord
	@tparam int yCoord
	@usage
	stratmap.camera.jump(1,2);
	*/
	tables.cameraTable.set_function("jump", &cameraHelpers::snapStratCamera);
	/***
	Set the zoom level of the camera.
	@function stratmap.camera.zoom
	@tparam float distance
	@usage
	stratmap.camera.zoom(0.12);
	*/
	tables.cameraTable.set_function("zoom", &cameraHelpers::zoomStratCamera);

	///Game
	//@section gameTable


	/***
	Basic stratmap.game table

	@tfield getFactionsCount getFactionsCount
	@tfield getFaction getFaction
	@tfield createCharacterByString createCharacterByString
	@tfield createArmy createArmy
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
	Get the amount of factions.
	@function stratmap.game.getFactionsCount
	@treturn int facNumber Amount of factions
	@usage
	local facNum=stratmap.game.getFactionsCount();
	*/
	tables.gameTable.set_function("getFactionsCount", &gameHelpers::getFactionsCount);
	/***
	Get a faction by the index, commonly used when iterating over all factions using getFactionsCount()
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
	tables.gameTable.set_function("getGuild", &gameHelpers::getGuild);

	/***
	Create a new character at the specified coordinates.
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
	newCharacter=stratmap.game.createCharacterByString("named character",stratmap.game.getFaction(0),18,"Name1","Name2",31,"custom_portrait_name",character.character.xCoord+5,character.character.yCoord);
	*/
	tables.gameTable.set_function("createCharacterByString", &gameHelpers::createCharacter);
	/***
	Create an army for a character. Commonly used after spawning a new character to set it's bodyguard unit.
	@function stratmap.game.createArmy
	@tparam character ourGeneral Character class, not named character class!
	@treturn stackStruct army
	@usage
	army=stratmap.game.createArmy(gen);
	*/
	tables.gameTable.set_function("createArmy", &gameHelpers::createArmy);
	/***
	Create an army in a settlement (don't need a character). Used to add units to an empty settlement.
	@function stratmap.game.createArmyInSettlement
	@tparam settlementStruct settlement
	@treturn stackStruct army
	@usage
	army=stratmap.game.createArmyInSettlement(sett);
	*/
	tables.gameTable.set_function("createArmyInSettlement", &gameHelpers::createArmyInSettlement);
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
	tables.gameTable.set_function("setScriptCounter", &gameHelpers::setScriptCounter);
	/***
	Fire a game event message. Picture needs to be provided in the ui folders as default.
	@function stratmap.game.historicEvent
	@tparam string eventName
	@tparam string title
	@tparam string body
	@usage
	stratmap.game.historicEvent("my_event", "my title", "my description")
	*/
	tables.gameTable.set_function("historicEvent", &gameHelpers::historicEvent);

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
	@tfield int fatigue (battle)
	@tfield int maxAmmo (battle)
	@tfield int currentAmmo (battle)
	@tfield float battlePosX 2d position on battlemap useful for getting distance etc (battle)
	@tfield float battlePosY 2d position on battlemap useful for getting distance etc (battle)
	@tfield character character
	@tfield unitPositionData unitPositionData
	@tfield stackStruct army
	@tfield int siegeEngineNum
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
	@tfield isEngaged isEngaged
	@tfield isUnderFire isUnderFire
	@tfield getMountClass getMountClass


	@table unit
	*/
	types.unit = luaState.new_usertype<unit>("unit");
	types.unit.set("eduEntry", &unit::eduEntry);
	types.unit.set("aiActiveSet", &unit::aiActiveSet);
	types.unit.set("movePoints", sol::property(&unitHelpers::getMovepoints, &unitHelpers::setMovepoints));
	types.unit.set("soldierCountStratMap", sol::property(&unitHelpers::getsoldierCountStratMap, &unitHelpers::setSoldiersCount));
	types.unit.set("exp", sol::property(&unitHelpers::getExp, &unitHelpers::setExp));
	types.unit.set("armourLVL", sol::property(&unitHelpers::getarmourLVL, &unitHelpers::setarmourLVL));
	types.unit.set("weaponLVL", sol::property(&unitHelpers::getweaponLVL, &unitHelpers::setweaponLVL));
	types.unit.set("soldierCountStratMapMax", sol::property(&unitHelpers::getMaxSoldiersCount));
	types.unit.set("soldierCountBattleMap", &unit::SoldierCountBattlemap);
	types.unit.set("unitPositionData", &unit::unitPositionData);
	types.unit.set("character", &unit::general);
	types.unit.set("isCloseFormation", &unit::isCloseFormation);
	types.unit.set("moraleLevel", &unit::moraleLevel);
	types.unit.set("battlePosX", &unit::positionX);
	types.unit.set("battlePosY", &unit::positionY);
	types.unit.set("fatigue", &unit::fatigue);
	types.unit.set("maxAmmo", &unit::maxAmmo);
	types.unit.set("currentAmmo", &unit::currentAmmo);
	types.unit.set("siegeEngineNum", &unit::siegeEnNum);
	types.unit.set("army", &unit::army);
	/***
	Kill this unit
	@function unit:kill
	@usage
	unit:kill();
	*/
	types.unit.set_function("kill", &unitHelpers::killUnit);
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
	types.unitPositionData.set("unit", &unitPositionData::unit);
	types.unitPositionData.set("isOnWallsCount", &unitPositionData::isOnWallsCount);
	types.unitPositionData.set("isInTowerCount", &unitPositionData::isInTowerCount);
	types.unitPositionData.set("isInGateHouseCount", &unitPositionData::isInGateHouseCount);
	types.unitPositionData.set("targetsDone", &unitPositionData::targetsDone);
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
	@tfield int isBodyPiercing

	@table projectileStruct
	*/
	types.projectileStruct = luaState.new_usertype<projectile>("projectileStruct");
	types.projectileStruct.set("name", &projectile::name);
	types.projectileStruct.set("accuracy", &projectile::accuracyVsUnits);
	types.projectileStruct.set("isBodyPiercing", &projectile::accuracyVsUnits);


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
	@tfield mountStruct mount
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
	types.eduEntry.set("class", &eduEntry::Class);
	types.eduEntry.set("categoryClassCombo", &eduEntry::categoryClassCombinationForAI);
	types.eduEntry.set("recruitPriorityOffset", &eduEntry::RecruitPriorityOffsetTimes4);
	types.eduEntry.set("crusadingUpkeepModifier", &eduEntry::CrusadingUpkeepModifier);
	types.eduEntry.set("aiUnitValuePerSoldier", &eduEntry::aiUnitValuePerSoldier);
	types.eduEntry.set("aiUnitValue", &eduEntry::aiUnitValue);


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


	///Character
	//@section Character Table

	/***
	characters as they exist on the strategy map - dead characters, wives, children, and off-map characters do not have these fields.

	@tfield int xCoord
	@tfield int yCoord
	@tfield namedCharacter namedCharacter
	@tfield unit bodyguards
	@tfield stackStruct armyLeaded
	@tfield stackStruct armyNotLeaded in the stack but not leading it
	@tfield int inEnemyZOC
	@tfield int ambushState
	@tfield int regionID
	@tfield int isMarkedToKill can check if the character died before the game updates he is dead like post battle event
	@tfield int doNotSpendMovePoints
	@tfield float movePointsCharacter
	@tfield float movePointsModifier
	@tfield float movePointsMaxCharacter
	@tfield float movePointsMaxArmy
	@tfield float movePointsArmy
	@tfield int turnJoinedCrusade
	@tfield int numTurnsIdle
	@tfield float percentCharacterReligionInRegion 0 to 1
	@tfield float popConvertedThisTurn
	@tfield int timeInRegion
	@tfield int timeWithArmy for auxiliary generals, not leading general
	@tfield crusadeStruct crusade
	@tfield int currentTurn
	@tfield float distanceToCrusadeTarget
	@tfield int disbandProgress
	@tfield int isCrusadeDisbandActive
	@tfield settlementStruct besiegingSettlement
	@tfield character besiegingCharacter
	@tfield string ability see descr\_hero\_abilities.xml
	@tfield getTypeID getTypeID
	@tfield getTypeName getTypeName
	@tfield setTypeID setTypeID
	@tfield moveToTile moveToTile
	@tfield reposition reposition
	@tfield kill kill
	@tfield createFort createFort
	@tfield setBodyguardUnit setBodyguardUnit
	@tfield setCharacterModel setCharacterModel

	@table character
	*/
	types.character = luaState.new_usertype<general>("character");
	types.character.set("xCoord", &general::xCoord);
	types.character.set("yCoord", &general::yCoord);
	types.character.set("namedCharacter", &general::genChar);
	types.character.set("bodyguards", &general::bodyguards);
	types.character.set("armyLeaded", &general::armyLeaded);
	types.character.set("armyNotLeaded", &general::armyNotLeaded);
	types.character.set("isMarkedToKill", &general::ifMarkedToKill);
	types.character.set("inEnemyZOC", &general::inEnemyZOC);
	types.character.set("ambushState", &general::ambushState);
	types.character.set("doNotSpendMovePoints", &general::doNotSpendMovePoints);
	types.character.set("turnJoinedCrusade", &general::turnJoinedCrusade);
	types.character.set("numTurnsIdle", &general::numTurnsIdle);
	types.character.set("percentCharacterReligionInRegion", &general::percentCharacterReligionInRegion);
	types.character.set("popConvertedThisTurn", &general::popConvertedThisTurn);
	types.character.set("timeInRegion", &general::timeInRegion);
	types.character.set("timeWithArmy", &general::timeWithArmy);
	types.character.set("regionID", &general::regionID);
	types.character.set("crusade", &general::crusade);
	types.character.set("turnJoinedCrusade", &general::turnJoinedCrusade);
	types.character.set("currentTurn", &general::currentTurn);
	types.character.set("distanceToCrusadeTarget", &general::distanceToCrusadeTarget);
	types.character.set("disbandProgress", &general::disbandProgress);
	types.character.set("isCrusadeDisbandActive", &general::isCrusadeDisbandActive);
	types.character.set("besiegingSettlement", &general::besiegingSettlement);
	types.character.set("besiegingCharacter", &general::besiegingCharacter);
	types.character.set("movePointsCharacter", &general::movePointsCharacter);
	types.character.set("movePointsModifier", &general::movePointsModifier);
	types.character.set("movePointsMaxArmy", &general::movePointsMaxArmy);
	types.character.set("movePointsMaxCharacter", &general::movePointsMax);
	types.character.set("movePointsArmy", sol::property(&generalHelpers::getMovepoints, &generalHelpers::setMovepoints));
	types.character.set("ability", sol::property(&luaGetSetFuncs::getStringPropertyGen<generalStruct_abilityID>, &luaGetSetFuncs::setStringPropertyGen<generalStruct_abilityID>));
	/***
	Get the character type. See hint below for the types.
	0-spy
	1-assassin
	2-diplomat
	3-admiral
	4-merchant
	5-priest
	6-general
	7-named character
	8-princess
	9-heretic
	10-witch
	11-inquisitor
	13-pope
	@function character:getTypeID
	@treturn int typeId
	@usage
	local ourType=ourCharacter:getTypeID();
	*/
	types.character.set_function("getTypeID", &generalHelpers::getTypeID);
	/***
	Get the character type. See hint below for the types.
	0-spy
	1-assassin
	2-diplomat
	3-admiral
	4-merchant
	5-priest
	6-general
	7-named character
	8-princess
	9-heretic
	10-witch
	11-inquisitor
	13-pope
	@function character:getTypeName
	@treturn string type
	@usage
	local ourType=ourCharacter:getTypeName();
	*/
	types.character.set_function("getTypeName", &generalHelpers::getTypeName);
	/***
	Set the character type. See hint below for the types.
	0-spy
	1-assassin
	2-diplomat
	3-admiral
	4-merchant
	5-priest
	6-general
	7-named character
	8-princess
	9-heretic
	10-witch
	11-inquisitor
	13-pope
	@function character:setTypeID
	@tparam int typeId
	@usage
	ourCharacter:setTypeID(2);
	*/
	types.character.set_function("setTypeID", &generalHelpers::setTypeID);	/***
	Issue regular move command, character must have movement points.
	@function character:moveToTile
	@tparam int xCoord
	@tparam int yCoord
	@usage
	ourCharacter:moveToTile(11,25);
	*/
	types.character.set_function("moveToTile", &generalHelpers::moveToTile);
	/***
	Instantly teleport character to the coordinates.
	@function character:reposition
	@tparam int xCoord
	@tparam int yCoord
	@usage
	ourCharacter:reposition(11,25);
	*/
	types.character.set_function("reposition", &generalHelpers::reposition);
	/***
	Delete this character
	@function character:kill
	@usage
	ourCharacter:kill();
	*/
	types.character.set_function("kill", &generalHelpers::killGeneral);
	/***
	Create a fort at the character's coordinates
	@function character:createFort
	@usage
	ourCharacter:createFort();
	*/
	types.character.set_function("createFort", &factionHelpers::createFort);
	/***
	Set bodyguard. Do this only for characters without it, such as immediately after creating a character.
	@function character:setBodyguardUnit
	@tparam unit unit
	@usage
	ourCharacter:setBodyguardUnit(unit);
	*/
	types.character.set_function("setBodyguardUnit", &generalHelpers::setBodyguard);
	/***
	Set a character's model to a new one.
	@function character:setCharacterModel
	@tparam string model
	@usage
	ourCharacter:setCharacterModel("saruman");
	*/
	types.character.set_function("setCharacterModel", &generalHelpers::setCharacterModel);



	///NamedCharacter
	//@section namedCharacterTable

	/***
	Basic namedCharacter table. All named characters have these fields including dead characters, wives, children, and off-map characters.

	@tfield int index
	@tfield character character
	@tfield string shortName Internal name.
	@tfield string fullName Internal name including surname.
	@tfield string localizedDisplayName Display name, resets upon reloading a save.
	@tfield string label
	@tfield string portrait Wives (who have never been princesses) and children do not have anything for this field. Path to 'young' portrait used starting from 'mods' folder. Resets upon reloading a save.
	@tfield string portrait2 Wives (who have never been princesses) and children do not have anything for this field. Path to 'old' portrait used starting from 'mods' folder. Resets upon reloading a save.
	@tfield string portrait_custom Wives (who have never been princesses) and children do not have anything for this field. Folder name in ui/custom_portraits folder.
	@tfield string modelName Battle model (needs battle_models.modeldb entry).
	@tfield int status 5-leader,2 - heir, 0 - ordinary character, read only, do not set value
	@tfield setAsHeir setAsHeir
	@tfield isAlive isAlive
	@tfield bool isFamily
	@tfield bool isMale
	@tfield isOffMap isOffMap
	@tfield isChild isChild
	@tfield int age
	@tfield float yearOfBirth For example with 4 turns per year, the yearOfBirth could be 1840.25
	@tfield factionStruct faction
	@tfield int subFaction
	@tfield namedCharacter parent
	@tfield namedCharacter spouse
	@tfield namedCharacter[4] childs Maximum 4.
	@tfield getTraits getTraits
	@tfield addTrait addTrait
	@tfield removeTrait removeTrait
	@tfield int ancNum
	@tfield getAncillary getAncillary
	@tfield addAncillary addAncillary
	@tfield removeAncillary removeAncillary
	@tfield int level
	@tfield int authority
	@tfield int command
	@tfield int chivalryAndDread positive = Chivalry, negative = Dread
	@tfield int loyalty
	@tfield int piety
	@tfield int ambush
	@tfield int artilleryCommand
	@tfield int assassination
	@tfield int attack
	@tfield int battleSurgery
	@tfield int bodyguardSize
	@tfield int bodyguardValour
	@tfield int boldness
	@tfield int bribeResistance
	@tfield int bribery
	@tfield int cavalryCommand
	@tfield int charm
	@tfield int construction
	@tfield int defence
	@tfield int disposition
	@tfield int electability
	@tfield int eligibility
	@tfield int farming
	@tfield int fertility
	@tfield int finance
	@tfield int footInTheDoor
	@tfield int generosity
	@tfield int gunpowerCommand
	@tfield int health
	@tfield int heresyImmunity
	@tfield int hitpoints
	@tfield int infantryCommand
	@tfield int influence
	@tfield int law
	@tfield int lineOfSight
	@tfield int localPopularity
	@tfield int looting
	@tfield int magic
	@tfield int management
	@tfield int mining
	@tfield int movementPointsBonus
	@tfield int navalCommand
	@tfield int nightBattle
	@tfield int personalSecurity
	@tfield int publicSecurity
	@tfield int purity
	@tfield int sabotage
	@tfield int siegeAttack
	@tfield int siegeDefense
	@tfield int siegeEngineering
	@tfield int squalor
	@tfield int subterfuge
	@tfield int taxCollection
	@tfield int trading
	@tfield int trainingAgents
	@tfield int trainingAnimalUnits
	@tfield int trainingUnits
	@tfield int troopMorale
	@tfield int unorthodoxy
	@tfield int unrest
	@tfield int violence
	@tfield int[10] combatVsReligion Maximum 10. EVEN IF YOU SET RELIGION LIMIT.
	@tfield int[31] combatVsFaction Maximum 31.

	@table namedCharacter
	*/
	types.namedCharacter = luaState.new_usertype<namedCharacter>("namedCharacter");
	types.namedCharacter.set("index", &namedCharacter::index);
	types.namedCharacter.set("character", &namedCharacter::gen);
	types.namedCharacter.set("shortName", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_shortName>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_shortName>));
	types.namedCharacter.set("fullName", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_fullName>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_fullName>));
	types.namedCharacter.set("localizedDisplayName", sol::property(&technicalHelpers::namedCharUniStringToStr<namedChar_localizedFullName>, &technicalHelpers::namedCharSetLocalizedFullName));
	types.namedCharacter.set("label", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_label>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_label>));
	types.namedCharacter.set("portrait", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_portrait>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_portrait>));
	types.namedCharacter.set("portrait2", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_portrait2>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_portrait2>));
	types.namedCharacter.set("portrait_custom", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_portrait_custom>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_portrait_custom>));
	types.namedCharacter.set("modelName", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_modelName>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_modelName>));
	types.namedCharacter.set("status", &namedCharacter::status);
	/***
	Sets the named character as the faction heir.
	@function namedCharacter:setAsHeir
	@tparam bool onlyHeir True = this character will be the only heir, false = add another heir (faction can appear to have multiple heirs but only one will become leader).
	@usage
	ourcharacter:setAsHeir(true)
	*/
	types.namedCharacter.set_function("setAsHeir", &generalCharactericticsHelpers::setAsHeir);
	/***
	Checks if character is alive, read only, do not set this value.
	@function namedCharacter:isAlive
	@treturn bool liveStatus true = alive, false = dead
	@usage
	if ourcharacter:isAlive() == true then
		ourcharacter.character:kill()
	end
	*/
	types.namedCharacter.set_function("isAlive", &generalCharactericticsHelpers::isAlive);
	/***
	Checks if character is off map, read only, do not set this value.
	@function namedCharacter:isOffMap
	@treturn bool offMap
	@usage
	if ourcharacter:isOffMap() == true then
		--do something
	end
	*/
	types.namedCharacter.set_function("isOffMap", &generalCharactericticsHelpers::isOffMap);
	/***
	Checks if character is child, read only, do not set this value.
	@function namedCharacter:isChild
	@treturn bool isChild
	@usage
	if ourcharacter:isChild() == true then
		--do something
	end
	*/
	types.namedCharacter.set_function("isChild", &generalCharactericticsHelpers::isChild);
	types.namedCharacter.set("isMale", sol::property(&generalCharactericticsHelpers::getIsMale, &generalCharactericticsHelpers::setIsMale));
	types.namedCharacter.set("isFamily", sol::property(&generalCharactericticsHelpers::isFamily, &generalCharactericticsHelpers::setAsFamily));
	types.namedCharacter.set("age", sol::property(&generalCharactericticsHelpers::getAge, &generalCharactericticsHelpers::setAge));
	types.namedCharacter.set("yearOfBirth", &namedCharacter::yearOfBirth);
	types.namedCharacter.set("faction", &namedCharacter::faction);
	types.namedCharacter.set("subFaction", &namedCharacter::subFaction);
	types.namedCharacter.set("parent", &namedCharacter::parent);
	types.namedCharacter.set("spouse", &namedCharacter::spouse);
	types.namedCharacter.set("childs", sol::property([](namedCharacter& self) { return std::ref(self.childs); }));
	types.namedCharacter.set("combatVsReligion", sol::property([](namedCharacter& self) { return std::ref(self.combatVsReligion); }));
	types.namedCharacter.set("combatVsFaction", sol::property([](namedCharacter& self) { return std::ref(self.combatVsFaction); }));
	/***
	Get the pointer to the character's traits container.
	@function namedCharacter:getTraits
	@treturn traitContainer The character's traits.
	@usage
	local thisTrait, traitsList, index = selectedChar:getTraits(), selectedChar.fullName.." traits:", 0
	while thisTrait ~= nil do
		traitsList, thisTrait, index = traitsList.."\n\tTrait "..index.." - Name: "..thisTrait.name.." - Level: "..thisTrait.level, thisTrait.nextTrait, index + 1
	end
	print(traitsList)
	*/
	types.namedCharacter.set_function("getTraits", &generalCharactericticsHelpers::getTraits);
	/***
	Add a trait to the character.
	@function namedCharacter:addTrait
	@tparam string traitName Trait's internal name per export\_descr\_character\_traits.txt
	@tparam int traitLevel Trait's level.
	@usage
	ourNamedCharacter:addTrait("GoodCommander", 2)
	*/
	types.namedCharacter.set_function("addTrait", &generalCharactericticsHelpers::addTrait);
	/***
	Remove a trait from the character.
	@function namedCharacter:removeTrait
	@tparam string traitName Trait's internal name per export\_descr\_character\_traits.txt
	@usage
	ourNamedCharacter:removeTrait("GoodCommander");
	*/
	types.namedCharacter.set_function("removeTrait", &generalCharactericticsHelpers::removeTrait);
	types.namedCharacter.set("ancNum", &namedCharacter::ancNum);
	/***
	Get the pointer to the ancillary using it's index. You can iterate over a character's ancillaries for example by going from index 0 to ancNum - 1.
	@function namedCharacter:getAncillary
	@tparam int index
	@treturn ancillary ancillary
	@usage
	ancillary = ourNamedCharacter:getAncillary(2)
	*/
	types.namedCharacter.set_function("getAncillary", &generalCharactericticsHelpers::getAncillary);
	/***
	Add an ancillary to the named character using the name per export\_descr\_ancillaries.txt.
	@function namedCharacter:addAncillary
	@tparam string ancillaryName
	@usage
	ourNamedCharacter:addAncillary("VeryVeryGoodMan");
	*/
	types.namedCharacter.set_function("addAncillary", &generalCharactericticsHelpers::addAncillary);
	/***
	Remove an ancillary from the named character using it's pointer. Use getAncillary function to get the specific ancillary.
	@function namedCharacter:removeAncillary
	@tparam ancillary ancillary
	@usage
	ourAnc=ourNamedCharacter:getAncillary(2);
	ourNamedCharacter:removeAncillary(ourAnc);
	*/
	types.namedCharacter.set_function("removeAncillary", &generalCharactericticsHelpers::removeAncillary);
	types.namedCharacter.set("level", &namedCharacter::level);
	types.namedCharacter.set("authority", &namedCharacter::leaderAutority);
	types.namedCharacter.set("command", &namedCharacter::authority);
	types.namedCharacter.set("chivalryAndDread", &namedCharacter::nobility);
	types.namedCharacter.set("loyalty", &namedCharacter::loyality);
	types.namedCharacter.set("piety", &namedCharacter::piety);
	types.namedCharacter.set("influence", &namedCharacter::influence);
	types.namedCharacter.set("subterfuge", &namedCharacter::subterfuge);
	types.namedCharacter.set("charm", &namedCharacter::charm);
	types.namedCharacter.set("finance", &namedCharacter::finance);
	types.namedCharacter.set("magic", &namedCharacter::magic);
	types.namedCharacter.set("unorthodoxy", &namedCharacter::unorthodoxy);
	types.namedCharacter.set("heresyImmunity", &namedCharacter::heresyImmunity);
	types.namedCharacter.set("assassination", &namedCharacter::assassination);
	types.namedCharacter.set("sabotage", &namedCharacter::sabotage);
	types.namedCharacter.set("eligibility", &namedCharacter::eligibility);
	types.namedCharacter.set("purity", &namedCharacter::purity);
	types.namedCharacter.set("violence", &namedCharacter::violence);
	types.namedCharacter.set("disposition", &namedCharacter::disposition);
	types.namedCharacter.set("boldness", &namedCharacter::boldness);
	types.namedCharacter.set("generosity", &namedCharacter::generosity);
	types.namedCharacter.set("management", &namedCharacter::management);
	types.namedCharacter.set("bodyguardSize", &namedCharacter::bodyguardSize);
	types.namedCharacter.set("troopMorale", &namedCharacter::troopMorale);
	types.namedCharacter.set("movementPointsBonus", &namedCharacter::movementPointsBonus);
	types.namedCharacter.set("attack", &namedCharacter::attack);
	types.namedCharacter.set("defence", &namedCharacter::defence);
	types.namedCharacter.set("siegeAttack", &namedCharacter::siegeAttack);
	types.namedCharacter.set("siegeDefense", &namedCharacter::siegeDefense);
	types.namedCharacter.set("ambush", &namedCharacter::ambush);
	types.namedCharacter.set("navalCommand", &namedCharacter::navalCommand);
	types.namedCharacter.set("siegeEngineering", &namedCharacter::siegeEngineering);
	types.namedCharacter.set("nightBattle", &namedCharacter::nightBattle);
	types.namedCharacter.set("personalSecurity", &namedCharacter::personalSecurity);
	types.namedCharacter.set("publicSecurity", &namedCharacter::publicSecurity);
	types.namedCharacter.set("bribery", &namedCharacter::bribery);
	types.namedCharacter.set("bribeResistance", &namedCharacter::bribeResistance);
	types.namedCharacter.set("electability", &namedCharacter::electability);
	types.namedCharacter.set("lineOfSight", &namedCharacter::lineOfSight);
	types.namedCharacter.set("trainingUnits", &namedCharacter::trainingUnits);
	types.namedCharacter.set("trainingAgents", &namedCharacter::trainingAgents);
	types.namedCharacter.set("construction", &namedCharacter::construction);
	types.namedCharacter.set("trading", &namedCharacter::trading);
	types.namedCharacter.set("localPopularity", &namedCharacter::localPopularity);
	types.namedCharacter.set("footInTheDoor", &namedCharacter::footInTheDoor);
	types.namedCharacter.set("farming", &namedCharacter::farming);
	types.namedCharacter.set("mining", &namedCharacter::mining);
	types.namedCharacter.set("taxCollection", &namedCharacter::taxCollection);
	types.namedCharacter.set("fertility", &namedCharacter::fertility);
	types.namedCharacter.set("cavalryCommand", &namedCharacter::cavalryCommand);
	types.namedCharacter.set("infantryCommand", &namedCharacter::infantryCommand);
	types.namedCharacter.set("gunpowerCommand", &namedCharacter::gunpowerCommand);
	types.namedCharacter.set("artilleryCommand", &namedCharacter::artilleryCommand);
	types.namedCharacter.set("health", &namedCharacter::health);
	types.namedCharacter.set("squalor", &namedCharacter::squalor);
	types.namedCharacter.set("unrest", &namedCharacter::unrest);
	types.namedCharacter.set("law", &namedCharacter::law);
	types.namedCharacter.set("looting", &namedCharacter::looting);
	types.namedCharacter.set("bodyguardValour", &namedCharacter::bodyguardValour);
	types.namedCharacter.set("hitpoints", &namedCharacter::hitpoints);
	types.namedCharacter.set("trainingAnimalUnits", &namedCharacter::trainingAnimalUnits);
	types.namedCharacter.set("battleSurgery", &namedCharacter::battleSurgery);


	///capturedFactionInfo
	//@section capturedFactionInfoTable

	/***
	Basic capturedFactionInfo table.

	@tfield int targetFactionID
	@tfield int factionID
	@tfield namedCharacter namedChar
	@tfield namedCharacter targetCharacter
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

	@tfield namedCharacter namedChar
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
	@tfield string ai_label
	@tfield string name
	@tfield string localizedName
	@tfield settlementStruct capital
	@tfield namedCharacter leader
	@tfield namedCharacter heir
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
	@tfield factionStratMapStruct facStrat
	@tfield int numOfNamedCharacters includes literally all characters without distinction (so also wives, children, dead and those sent off map)
	@tfield getNamedCharacter getNamedCharacter
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

	@table factionStruct
	*/
	types.factionStruct = luaState.new_usertype<factionStruct>("factionStruct");
	types.factionStruct.set("dipNum", &factionStruct::dipNum);
	types.factionStruct.set("factionID", &factionStruct::dipNum);
	types.factionStruct.set("aiPersonalityType", &factionStruct::AIPersonalityType);
	types.factionStruct.set("aiPersonalityName", &factionStruct::AIPersonalityName);
	/***
	Get the faction's internal name
	@function factionStruct:getFactionName
	@treturn string facName
	@usage
	ourFac = stratmap.game.getFaction(0);
	ourFacName = ourFac:getFactionName();
	if ourFacName == "england" then
		ourFac.money = ourFac.money + (ourFac.fortsNum * 500)
	end
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
		&factionHelpers::getLocalizedFactionName, &factionHelpers::changeFactionName
		));
	types.factionStruct.set("capital", &factionStruct::capital);
	types.factionStruct.set("leader", &factionStruct::leader);
	types.factionStruct.set("heir", &factionStruct::heir);
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
	types.factionStruct.set("numOfNamedCharacters", &factionStruct::numOfCharactersAll);
	/***
	Get named character using it's index.
	@function factionStruct:getNamedCharacter
	@tparam int number
	@treturn namedCharacter retNamedCharacter
	@usage
	ourNamedCharacter = stratmap.game.getFaction(0):getNamedCharacter(0)
	if ourNamedCharacter.command > 5 then
		ourFac.money = ourFac.money - (ourNamedCharacter.command * 10)
	end
	*/
	types.factionStruct.set_function("getNamedCharacter", &factionHelpers::getCharacterFromFullList);
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
		local factionsNum = stratmap.game.getFactionsCount();
		for i = 0, factionsNum - 1 do
			local faction = stratmap.game.getFaction(i);
			if not faction then
				return nil;
			end
			local armiesNum = faction.stacksNum;
			for j = 0, armiesNum - 1 do
				local army = faction:getStack(j);
				if (army.xCoord == x and army.yCoord == y) then
					return army;
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
	ourSett = stratmap.game.getFaction(0):getSettlement(0)
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
	ourFort = stratmap.game.getFaction(0):getFort(0)
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
	types.factionStruct.set_function("deleteFort", &factionHelpers::deleteFort);

	/***
	Create a fort at the specified coordinates.
	@function factionStruct:createFortXY
	@tparam int X
	@tparam int Y
	@usage
	fac:createFortXY(193, 283)
	*/
	types.factionStruct.set_function("createFortXY", &factionHelpers::createFortXY);

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
	@tfield int wantBeProtect
	@tfield int wantOfferProtect
	@tfield int allianceAgainst
	@tfield int ptsDesire
	@tfield int ptsAlliance
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


	///aiProductionController
	//@section aiProductionController

	/***
	Basic aiProductionController table

	@tfield aiFaction aiFaction
	@tfield int regionID
	@tfield settlementStruct settlement
	@tfield int autoManagePolicy
	@tfield int isAutoManaged
	@tfield int isAutoManagedRecruitment
	@tfield int isAutoManagedConstruction
	@tfield int spyBias
	@tfield int assassinBias
	@tfield int diplomatBias
	@tfield int admiralBias
	@tfield int priestBias
	@tfield int merchantBias
	@tfield setConstructionValue setConstructionValue
	@tfield setRecruitmentValue setRecruitmentValue
	@tfield getConstructionValue getConstructionValue
	@tfield getRecruitmentValue getRecruitmentValue

	@table aiProductionController
	*/
	types.aiProductionController = luaState.new_usertype<aiProductionController>("aiProductionController");
	types.aiProductionController.set("aiFaction", &aiProductionController::aiFaction);
	types.aiProductionController.set("regionID", &aiProductionController::regionID);
	types.aiProductionController.set("settlement", &aiProductionController::settlement);
	types.aiProductionController.set("autoManagePolicy", &aiProductionController::autoManagePolicy);
	types.aiProductionController.set("isAutoManaged", &aiProductionController::isAutoManaged);
	types.aiProductionController.set("isAutoManagedRecruitment", &aiProductionController::isAutoManagedRecruitment);
	types.aiProductionController.set("isAutoManagedConstruction", &aiProductionController::isAutoManagedConstruction);
	types.aiProductionController.set("spyBias", &aiProductionController::spyBias);
	types.aiProductionController.set("assassinBias", &aiProductionController::assassinBias);
	types.aiProductionController.set("diplomatBias", &aiProductionController::diplomatBias);
	types.aiProductionController.set("admiralBias", &aiProductionController::admiralBias);
	types.aiProductionController.set("priestBias", &aiProductionController::priestBias);
	types.aiProductionController.set("merchantBias", &aiProductionController::merchantBias);
	/***
	Set bias value of the ai personality for a capability.
	@function aiProductionController:setConstructionValue
	@tparam int type use building capabilities enum
	@tparam int value
	@usage
		 aiProductionController:setConstructionValue(buildingCapability.law_bonus, 100)
	*/
	types.aiProductionController.set_function("setConstructionValue", &factionHelpers::setConstructionValueSett);
	/***
	Set bias value of the ai personality for a recruitment class.
	@function aiProductionController:setRecruitmentValue
	@tparam int type use unitCategoryClass enum
	@tparam int value
	@usage
		 aiProductionController:setRecruitmentValue(unitCategoryClass.heavyCavalry, 100)
	*/
	types.aiProductionController.set_function("setRecruitmentValue", &factionHelpers::setRecruitmentValueSett);
	/***
	Get bias value of the ai personality for a capability.
	@function aiProductionController:getConstructionValue
	@tparam int type use building capabilities enum
	@treturn int value
	@usage
		local value = aiProductionController:setConstructionValue(buildingCapability.law_bonus)
	*/
	types.aiProductionController.set_function("getConstructionValue", &factionHelpers::getConstructionValueSett);
	/***
	Get bias value of the ai personality for a recruitment class.
	@function aiProductionController:getRecruitmentValue
	@tparam int type use unitCategoryClass enum
	@treturn int value
	@usage
		local value = aiProductionController:setRecruitmentValue(unitCategoryClass.heavyCavalry)
	*/
	types.aiProductionController.set_function("getRecruitmentValue", &factionHelpers::getRecruitmentValueSett);


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
	@tfield factionStruct faction
	@tfield settlementStruct settlement
	@tfield stackStruct blockingArmy

	@table watchtowerStruct
	*/
	types.watchtowerStruct = luaState.new_usertype<watchTowerStruct>("watchtowerStruct");
	types.watchtowerStruct.set("xCoord", &watchTowerStruct::xCoord);
	types.watchtowerStruct.set("yCoord", &watchTowerStruct::yCoord);
	types.watchtowerStruct.set("factionID", &watchTowerStruct::factionID);
	types.watchtowerStruct.set("faction", &watchTowerStruct::faction);
	types.watchtowerStruct.set("settlement", &watchTowerStruct::settlement);
	types.watchtowerStruct.set("blockingArmy", &watchTowerStruct::blockingArmy);

	///FortStruct
	//@section fortStructTable

	/***
	Basic fortStruct table

	@tfield int xCoord
	@tfield int yCoord
	@tfield character governor
	@tfield stackStruct army
	@tfield factionStruct ownerFaction
	@tfield int siegeNum
	@tfield int siegeHoldoutTurns
	@tfield int turnsSieged
	@tfield int plagued
	@tfield int subFactionID
	@tfield int factionID
	@tfield int cultureID
	@tfield int fortFortificationLevel
	@tfield getSiege getSiege
	
	@table fortStruct
	*/
	types.fortStruct = luaState.new_usertype<fortStruct>("fortStruct");
	types.fortStruct.set("xCoord", &fortStruct::xCoord);
	types.fortStruct.set("yCoord", &fortStruct::yCoord);
	types.fortStruct.set("governor", &fortStruct::gubernator);
	types.fortStruct.set("army", &fortStruct::army);
	types.fortStruct.set("ownerFaction", &fortStruct::faction);
	types.fortStruct.set("siegeNum", &fortStruct::siegeNum);
	types.fortStruct.set("siegeHoldoutTurns", &fortStruct::siegeHoldoutTurns);
	types.fortStruct.set("turnsSieged", &fortStruct::turnsSieged);
	types.fortStruct.set("plagued", &fortStruct::plagued);
	types.fortStruct.set("subFactionID", &fortStruct::subFactionID);
	types.fortStruct.set("factionID", &fortStruct::factionID);
	types.fortStruct.set("cultureID", &fortStruct::cultureID);
	types.fortStruct.set("fortFortificationLevel", &fortStruct::fortFortificationLevel);

	/***
	Get a specific siege by it's index
	@function fortStruct:getSiege
	@tparam int siegeIdx
	@treturn siegeStruct siege
	@usage
	for i = 0, currSet.siegesNum-1 do
	   local siege=currFort:getSiege(i);
	   --etc
	end
	*/
	types.fortStruct.set_function("getSiege", &settlementHelpers::getSiege);



	///PortStruct
	//@section portStructTable

	/***
	Basic portStruct table

	@tfield int xCoord land tile, Note: setting this only moves port's strat model.
	@tfield int yCoord land tile, Note: setting this only moves port's strat model.
	@tfield factionStruct ownerFaction Note: port's ownership changes to blockading faction (army on port)
	@tfield settlementStruct settlement
	@tfield character character Character standing on the port tile, only the first one, check for nil.
	@tfield stackStruct blockadingArmy Enemy army blockading the port, by standing on it's tile, check for nil.
	@tfield dockStruct dock water tile, only upgraded ports have this, check for nil.


	@table portStruct
	*/
	types.portStruct = luaState.new_usertype<portBuildingStruct>("portStruct");
	types.portStruct.set("xCoord", &portBuildingStruct::xCoord);
	types.portStruct.set("yCoord", &portBuildingStruct::yCoord);
	types.portStruct.set("ownerFaction", &portBuildingStruct::fac);
	types.portStruct.set("settlement", &portBuildingStruct::settlement);
	types.portStruct.set("character", &portBuildingStruct::character);
	types.portStruct.set("blockadingArmy", &portBuildingStruct::occupantsArmy);
	types.portStruct.set("dock", &portBuildingStruct::portDock);


	///DockStruct
	//@section dockStructTable

	/***
	Basic dockStruct table

	@tfield int xCoord water tile, Note: setting only moves dock strat model
	@tfield int yCoord water tile, Note: setting only moves dock strat model

	@table dockStruct
	*/
	types.dockStruct = luaState.new_usertype<portDockStrat>("dockStruct");
	types.dockStruct.set("xCoord", &portDockStrat::xCoord);
	types.dockStruct.set("yCoord", &portDockStrat::yCoord);


	///SettlementStruct
	//@section settlementStructTable

	/***
	Basic settlementStruct table

	@tfield int xCoord
	@tfield int yCoord
	@tfield character governor
	@tfield stackStruct army
	@tfield string name internal name of settlement
	@tfield string localizedName
	@tfield factionStruct ownerFaction
	@tfield changeOwner changeOwner
	@tfield int creatorFactionID
	@tfield int regionID
	@tfield int level
	@tfield int isCastle
	@tfield int siegeHoldoutTurns
	@tfield int turnsSieged
	@tfield int subFactionID
	@tfield int yearFounded
	@tfield int isCapital
	@tfield int harvestSuccess
	@tfield int baseFertility
	@tfield int rebelFactionChance
	@tfield int plagued
	@tfield int plagueDeaths
	@tfield int turnsOwned start at 10 for settlements owned at game start without specification in descr_strat
	@tfield int populationSiegeStart
	@tfield int settlementTaxLevel
	@tfield int recruitmentPoolCount
	@tfield int recruitmentCapabilityNum
	@tfield int freezeRecruitmentPool
	@tfield int spiesInRecruitmentQueue
	@tfield int assassinsInRecruitmentQueue
	@tfield int diplomatsInRecruitmentQueue
	@tfield int admiralsInRecruitmentQueue
	@tfield int merchantsInRecruitmentQueue
	@tfield int priestsInRecruitmentQueue
	@tfield aiProductionController aiProductionController
	@tfield int turmoil
	@tfield int isProvokedRebellion
	@tfield int populationSize
	@tfield getReligion getReligion
	@tfield setReligion setReligion
	@tfield getGuildStanding getGuildStanding
	@tfield setGuildStanding setGuildStanding
	@tfield int buildingsNum
	@tfield getBuilding getBuilding
	@tfield createBuilding createBuilding
	@tfield destroyBuilding destroyBuilding
	@tfield buildingsQueue buildingsQueue
	@tfield int resourcesNum
	@tfield settlementStats settlementStats
	@tfield settlementStats settlementStatsLastTurn
	@tfield getResource getResource
	@tfield int siegesNum
	@tfield getSiege getSiege
	@tfield getSettlementCapability getSettlementCapability
	@tfield getAgentCapability getAgentCapability
	@tfield getAgentLimitCapability getAgentLimitCapability
	@tfield getRecruitmentCapability getRecruitmentCapability
	@tfield getSettlementRecruitmentPool getSettlementRecruitmentPool

	@table settlementStruct
	*/
	types.settlementStruct = luaState.new_usertype<settlementStruct>("settlementStruct");
	types.settlementStruct.set("xCoord", &settlementStruct::xCoord);
	types.settlementStruct.set("yCoord", &settlementStruct::yCoord);
	types.settlementStruct.set("governor", &settlementStruct::governor);
	types.settlementStruct.set("army", &settlementStruct::army);
	types.settlementStruct.set("name", sol::property(
		&settlementHelpers::getStringProperty<settlementStruct_name>, &settlementHelpers::setStringProperty<settlementStruct_name>
		));
	types.settlementStruct.set("localizedName", sol::property(
		&settlementHelpers::getSettlementName, &settlementHelpers::changeSettlementName
		));
	types.settlementStruct.set("ownerFaction", &settlementStruct::faction);
	/***
	Change owner faction of settlement. All agents, armies etc. leave the settlement.
	@function settlementStruct:changeOwner
	@tparam factionStruct newOwner Faction to change ownership to.
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	currSet:changeOwner(fac1);
	end
	*/
	types.settlementStruct.set_function("changeOwner", &settlementHelpers::changeOwner);
	types.settlementStruct.set("creatorFactionID", &settlementStruct::fac_creatorModNum);
	types.settlementStruct.set("regionID", &settlementStruct::regionID);
	types.settlementStruct.set("level", &settlementStruct::level);
	types.settlementStruct.set("isCastle", &settlementStruct::isCastle);
	types.settlementStruct.set("settlementTaxLevel", &settlementStruct::settlementTaxLevel);
	types.settlementStruct.set("siegeHoldoutTurns", &settlementStruct::siegeHoldoutTurns);
	types.settlementStruct.set("turnsSieged", &settlementStruct::turnsSieged);
	types.settlementStruct.set("subFactionID", &settlementStruct::subFactionID);
	types.settlementStruct.set("yearFounded", &settlementStruct::yearFounded);
	types.settlementStruct.set("isCapital", &settlementStruct::isCapital);
	types.settlementStruct.set("aiProductionController", &settlementStruct::aiProductionController);
	types.settlementStruct.set("harvestSuccess", &settlementStruct::harvestSuccess);
	types.settlementStruct.set("baseFertility", &settlementStruct::baseFertilityValue);
	types.settlementStruct.set("rebelFactionChance", &settlementStruct::rebelFactionChance);
	types.settlementStruct.set("plagued", &settlementStruct::plagued);
	types.settlementStruct.set("populationSiegeStart", &settlementStruct::populationSiegeStart);
	types.settlementStruct.set("isProvokedRebellion", &settlementStruct::isProvokedRebellion);
	types.settlementStruct.set("populationSize", sol::property(settlementHelpers::getPopulation));
	types.settlementStruct.set("recruitmentPoolCount", &settlementStruct::recruitmentPoolCount);
	types.settlementStruct.set("freezeRecruitmentPool", &settlementStruct::freezeRecruitmentPool);
	types.settlementStruct.set("spiesInRecruitmentQueue", &settlementStruct::spiesInRecruitmentQueue);
	types.settlementStruct.set("assassinsInRecruitmentQueue", &settlementStruct::assassinsInRecruitmentQueue);
	types.settlementStruct.set("diplomatsInRecruitmentQueue", &settlementStruct::diplomatsInRecruitmentQueue);
	types.settlementStruct.set("admiralsInRecruitmentQueue", &settlementStruct::admiralsInRecruitmentQueue);
	types.settlementStruct.set("merchantsInRecruitmentQueue", &settlementStruct::merchantsInRecruitmentQueue);
	types.settlementStruct.set("priestsInRecruitmentQueue", &settlementStruct::priestsInRecruitmentQueue);
	types.settlementStruct.set("settlementStats", &settlementStruct::settlementStats);
	types.settlementStruct.set("settlementStatsLastTurn", &settlementStruct::settlementStatsLastTurn);
	types.settlementStruct.set("turmoil", &settlementStruct::turmoil);
	/***
	Get the settlement's specific regligion's value
	@function settlementStruct:getReligion
	@tparam int religionID In order of descr\_religions.txt, starting from 0
	@treturn float religionValue from 0 to 1
	@usage
	local firstRelVal = settlementStruct:getReligion(0) --get float of religion with ID 0
	*/
	types.settlementStruct.set_function("getReligion", &settlementHelpers::getReligion);
	/***
	Set the settlement's specific religion's value, make sure the sum of all religion values does not exceed 1.0!
	@function settlementStruct:setReligion
	@tparam int religionID in order of descr\_religions.txt, starting from 0
	@tparam float religionValue from 0 to 1
	@usage
	settlementStruct:setReligion(0, 0.5) --set religion with ID 0 as 50%
	*/
	types.settlementStruct.set_function("setReligion", &settlementHelpers::setReligion);
	/***
	Get a settlement's standing points with a specific guild by ID
	@function settlementStruct:getGuildStanding
	@tparam int guild_id
	@usage
	ourGuildStanding = settlementStruct:getGuildStanding(0)
	*/
	types.settlementStruct.set_function("getGuildStanding", &settlementHelpers::getGuildStanding);
	/***
	Set the settlement's standing points with specific guild.
	@function settlementStruct:setGuildStanding
	@tparam int guild_id
	@tparam int standing
	@usage
	settlementStruct:setGuildStanding(0, 300)
	*/
	types.settlementStruct.set_function("setGuildStanding", &settlementHelpers::setGuildStanding);
	types.settlementStruct.set("buildingsNum", &settlementStruct::buildingsNum);
	/***
	Get a specific building by it's index.
	@function settlementStruct:getBuilding
	@tparam int number
	@treturn building build
	@usage
	ourBuilding=settlementStruct:getBuilding(0);
	if(ourBuilding.level>1)
	then
		print("test");
	end
	*/
	types.settlementStruct.set_function("getBuilding", &settlementHelpers::getBuilding);
	/***
	Create a building in the settlement.
	@function settlementStruct:createBuilding
	@tparam string building_level_id
	@usage
	settlementStruct:createBuilding("some_build1");
	*/
	types.settlementStruct.set_function("createBuilding", &settlementHelpers::createBuilding);
	/***
	Destroy a building of a specified type in the settlement.
	@function settlementStruct:destroyBuilding
	@tparam string typeName Type of building.
	@tparam bool isReturnMoney Should money be returned to the faction like with a manual desctruction.
	@usage
	settlementStruct:destroyBuilding("some_buildType",false);
	*/
	types.settlementStruct.set_function("destroyBuilding", &settlementHelpers::destroyBuilding);
	types.settlementStruct.set("buildingsQueue", &settlementStruct::buildingsQueueArray);
	types.settlementStruct.set("resourcesNum", &settlementStruct::resourcesNum);
	/***
	Get a specific resource by it's index.
	@function settlementStruct:getResource
	@tparam int number
	@treturn tradeResource resource
	@usage
	ourResource = settlementStruct:getResource(0)
	*/
	types.settlementStruct.set_function("getResource", &settlementHelpers::getResource);
	types.settlementStruct.set("siegesNum", &settlementStruct::siegesNumber);
	/***
	Get a specific siege by it's index
	@function settlementStruct:getSiege
	@tparam int siegeIdx
	@treturn siegeStruct siege
	@usage
	for i = 0, currSet.siegesNum-1 do
	   local siege=currSet:getSiege(i);
	   --etc
	end
	*/
	types.settlementStruct.set_function("getSiege", &settlementHelpers::getSiege);
	/***
	Get a capability by capability type.
	@function settlementStruct:getSettlementCapability
	@tparam int capabilityType
	@treturn settlementCapability capability
	@usage
	local incomeBonus = settlementStruct:getSettlementCapability(55)
	*/
	types.settlementStruct.set_function("getSettlementCapability", &settlementHelpers::getSettlementCapability);
	/***
	Get an agent capability by agent type (only recruitable agents) 0 = spy, 1 = assassin, 2 = diplomat, 3 = princess, 4 = merchant, 5 = priest.
	@function settlementStruct:getAgentCapability
	@tparam int agentType
	@treturn settlementCapability capability
	@usage
	local spyCap = settlementStruct:getAgentCapability(0)
	*/
	types.settlementStruct.set_function("getAgentCapability", &settlementHelpers::getAgentCapability);
	/***
	Get an agent limit capability by agent type (only recruitable agents) 0 = spy, 1 = assassin, 2 = diplomat, 3 = princess, 4 = merchant, 5 = priest.
	@function settlementStruct:getAgentLimitCapability
	@tparam int agentType
	@treturn settlementCapability capability
	@usage
	local spyCapLimit = settlementStruct:getAgentLimitCapability(0)
	*/
	types.settlementStruct.set_function("getAgentLimitCapability", &settlementHelpers::getAgentCapability);
	/***
	Get a recruitment capability by index (max 64!).
	@function settlementStruct:getRecruitmentCapability
	@tparam int index
	@treturn recruitmentCapability capability
	@usage
	local capability = settlementStruct:getRecruitmentCapability(0)
	*/
	types.settlementStruct.set_function("getRecruitmentCapability", &settlementHelpers::getRecruitmentCapability);
	types.settlementStruct.set("recruitmentCapabilityNum", sol::property(settlementHelpers::getRecruitmentCapabilityNum));
	/***
	Get a recruitment pool by index.
	@function settlementStruct:getSettlementRecruitmentPool
	@tparam int index
	@treturn settlementRecruitmentPool pool
	@usage
	local pool = settlementStruct:getSettlementRecruitmentPool(0)
	*/
	types.settlementStruct.set_function("getSettlementRecruitmentPool", &settlementHelpers::getSettlementRecruitmentPool);


	///settlementStats
	//@section settlementStats

	/***
	Basic settlementStats table

	@tfield int PopGrowthBaseFarm - Get only
	@tfield int population
	@tfield int PopGrowthFarms - Get only
	@tfield int PopGrowthHealth - Get only
	@tfield int PopGrowthBuildings - Get only
	@tfield int PopGrowthTaxBonus - Get only
	@tfield int PopGrowthEntertainment - Get only
	@tfield int PopGrowthTrade - Get only
	@tfield int PopGrowthGovernorInfluence - Get only
	@tfield int PopGrowthSqualor - Get only
	@tfield int PopGrowthPlague - Get only
	@tfield int PopGrowthTaxPenalty - Get only
	@tfield int PublicOrderGarrison - Get only
	@tfield int PublicOrderLaw - Get only
	@tfield int PublicOrderBuildingsEntertainment - Get only
	@tfield int PublicOrderGovernorInfluence - Get only
	@tfield int PublicOrderTaxBonus - Get only
	@tfield int PublicOrderTriumph - Get only
	@tfield int PublicOrderPopulationBoom - Get only
	@tfield int PublicOrderEntertainment - Get only
	@tfield int PublicOrderHealth - Get only
	@tfield int PublicOrderGarrisonTwo - Get only
	@tfield int PublicOrderFear - Get only
	@tfield int PublicOrderGlory - Get only
	@tfield int PublicOrderSqualor - Get only
	@tfield int PublicOrderDistanceToCapital - Get only
	@tfield int PublicOrderNoGovernance - Get only
	@tfield int PublicOrderTaxPenalty - Get only
	@tfield int PublicOrderUnrest - Get only
	@tfield int PublicOrderBesieged - Get only
	@tfield int PublicOrderBlockaded - Get only
	@tfield int PublicOrderCulturalUnrest - Get only
	@tfield int PublicOrderExcommunication - Get only
	@tfield int PublicOrder - Get only
	@tfield int FarmsIncome - Get only
	@tfield int TaxesIncome - Get only
	@tfield int MiningIncome - Get only
	@tfield int TradeIncome - Get only
	@tfield int DiplomaticIncome - Get only
	@tfield int DemolitionIncome - Get only
	@tfield int LootingIncome - Get only
	@tfield int BuildingsIncome - Get only
	@tfield int AdminIncome - Get only
	@tfield int ConstructionExpense - Get only
	@tfield int RecruitmentExpense - Get only
	@tfield int DiplomaticExpense - Get only
	@tfield int CorruptionExpense - Get only
	@tfield int EntertainmentExpense - Get only
	@tfield int DevastationExpense - Get only
	@tfield int TotalIncomeWithoutAdmin - Get only
	@tfield int majorityReligionID

	@table settlementStats
	*/
	types.settlementStats = luaState.new_usertype<settlementStats>("settlementStats");
	types.settlementStats.set("population", &settlementStats::population);
	types.settlementStats.set("PopGrowthBaseFarm", &settlementStats::PopGrowthBaseFarm);
	types.settlementStats.set("PopGrowthFarms", &settlementStats::PopGrowthFarms);
	types.settlementStats.set("PopGrowthHealth", &settlementStats::PopGrowthHealth);
	types.settlementStats.set("PopGrowthBuildings", &settlementStats::PopGrowthBuildings);
	types.settlementStats.set("PopGrowthTaxBonus", &settlementStats::PopGrowthTaxBonus);
	types.settlementStats.set("PopGrowthEntertainment", &settlementStats::PopGrowthEntertainment);
	types.settlementStats.set("PopGrowthTrade", &settlementStats::PopGrowthTrade);
	types.settlementStats.set("PopGrowthGovernorInfluence", &settlementStats::PopGrowthGovernorInfluence);
	types.settlementStats.set("PopGrowthSqualor", &settlementStats::PopGrowthSqualor);
	types.settlementStats.set("PopGrowthPlague", &settlementStats::PopGrowthPlague);
	types.settlementStats.set("PopGrowthTaxPenalty", &settlementStats::PopGrowthTaxPenalty);
	types.settlementStats.set("PublicOrderGarrison", &settlementStats::PublicOrderGarrison);
	types.settlementStats.set("PublicOrderLaw", &settlementStats::PublicOrderLaw);
	types.settlementStats.set("PublicOrderBuildingsEntertainment", &settlementStats::PublicOrderBuildingsEntertainment);
	types.settlementStats.set("PublicOrderGovernorInfluence", &settlementStats::PublicOrderGovernorInfluence);
	types.settlementStats.set("PublicOrderTaxBonus", &settlementStats::PublicOrderTaxBonus);
	types.settlementStats.set("PublicOrderTriumph", &settlementStats::PublicOrderTriumph);
	types.settlementStats.set("PublicOrderPopulationBoom", &settlementStats::PublicOrderPopulationBoom);
	types.settlementStats.set("PublicOrderEntertainment", &settlementStats::PublicOrderEntertainment);
	types.settlementStats.set("PublicOrderHealth", &settlementStats::PublicOrderHealth);
	types.settlementStats.set("PublicOrderGarrisonTwo", &settlementStats::PublicOrderGarrisonTwo);
	types.settlementStats.set("PublicOrderFear", &settlementStats::PublicOrderFear);
	types.settlementStats.set("PublicOrderGlory", &settlementStats::PublicOrderGlory);
	types.settlementStats.set("PublicOrderSqualor", &settlementStats::PublicOrderSqualor);
	types.settlementStats.set("PublicOrderDistanceToCapital", &settlementStats::PublicOrderDistanceToCapital);
	types.settlementStats.set("PublicOrderNoGovernance", &settlementStats::PublicOrderNoGovernance);
	types.settlementStats.set("PublicOrderTaxPenalty", &settlementStats::PublicOrderTaxPenalty);
	types.settlementStats.set("PublicOrderUnrest", &settlementStats::PublicOrderUnrest);
	types.settlementStats.set("PublicOrderBesieged", &settlementStats::PublicOrderBesieged);
	types.settlementStats.set("PublicOrderBlockaded", &settlementStats::PublicOrderBlockaded);
	types.settlementStats.set("PublicOrderCulturalUnrest", &settlementStats::PublicOrderCulturalUnrest);
	types.settlementStats.set("PublicOrderExcommunication", &settlementStats::PublicOrderExcommunication);
	types.settlementStats.set("PublicOrder", &settlementStats::PublicOrder);
	types.settlementStats.set("FarmsIncome", &settlementStats::FarmsIncome);
	types.settlementStats.set("TaxesIncome", &settlementStats::TaxesIncome);
	types.settlementStats.set("MiningIncome", &settlementStats::MiningIncome);
	types.settlementStats.set("TradeIncome", &settlementStats::TradeIncome);
	types.settlementStats.set("DiplomaticIncome", &settlementStats::DiplomaticIncome);
	types.settlementStats.set("DemolitionIncome", &settlementStats::DemolitionIncome);
	types.settlementStats.set("LootingIncome", &settlementStats::LootingIncome);
	types.settlementStats.set("BuildingsIncome", &settlementStats::BuildingsIncome);
	types.settlementStats.set("AdminIncome", &settlementStats::AdminIncome);
	types.settlementStats.set("ConstructionExpense", &settlementStats::ConstructionExpense);
	types.settlementStats.set("RecruitmentExpense", &settlementStats::RecruitmentExpense);
	types.settlementStats.set("DiplomaticExpense", &settlementStats::DiplomaticExpense);
	types.settlementStats.set("CorruptionExpense", &settlementStats::CorruptionExpense);
	types.settlementStats.set("EntertainmentExpense", &settlementStats::EntertainmentExpense);
	types.settlementStats.set("DevastationExpense", &settlementStats::DevastationExpense);
	types.settlementStats.set("TotalIncomeWithoutAdmin", &settlementStats::TotalIncomeWithoutAdmin);
	types.settlementStats.set("majorityReligionID", &settlementStats::TotalIncomeWithoutAdmin);


	///settlementCapability
	//@section settlementCapability

	/***
	Basic settlementCapability table

	@tfield int value
	@tfield int bonus

	@table settlementCapability
	*/
	types.settlementCapability = luaState.new_usertype<settlementCapability>("settlementCapability");
	types.settlementCapability.set("value", &settlementCapability::value);
	types.settlementCapability.set("bonus", &settlementCapability::bonus);

	///recruitmentCapability
	//@section recruitmentCapability

	/***
	Basic recruitmentCapability table

	@tfield int eduIndex
	@tfield int xp
	@tfield float initialSize
	@tfield float replenishRate
	@tfield float maxSize

	@table recruitmentCapability
	*/
	types.recruitmentCapability = luaState.new_usertype<recruitmentCapability>("recruitmentCapability");
	types.recruitmentCapability.set("eduIndex", &recruitmentCapability::eduIndex);
	types.recruitmentCapability.set("xp", &recruitmentCapability::xp);
	types.recruitmentCapability.set("initialSize", &recruitmentCapability::initialSize);
	types.recruitmentCapability.set("replenishRate", &recruitmentCapability::replenishRate);
	types.recruitmentCapability.set("maxSize", &recruitmentCapability::maxSize);

	///settlementRecruitmentPool
	//@section settlementRecruitmentPool

	/***
	Basic settlementRecruitmentPool table

	@tfield int eduIndex
	@tfield float availablePool

	@table settlementRecruitmentPool
	*/
	types.settlementRecruitmentPool = luaState.new_usertype<settlementRecruitmentPool>("settlementRecruitmentPool");
	types.settlementRecruitmentPool.set("eduIndex", &settlementRecruitmentPool::eduIndex);
	types.settlementRecruitmentPool.set("availablePool", &settlementRecruitmentPool::availablePool);


	///Building
	//@section buildingTable

	/***
	Basic building table

	@tfield int level
	@tfield int hp
	@tfield settlementStruct settlement
	@tfield edbEntry edbEntry
	@tfield getType getType
	@tfield getName getName

	@table building
	*/
	types.building = luaState.new_usertype<building>("building");
	types.building.set("level", &building::level);
	types.building.set("hp", &building::hp);
	types.building.set("settlement", &building::settlement);
	types.building.set("edbEntry", &building::edbEntry);
	/***
	Get the name of the building type (the building chain in export\_descr\_buildings.txt).

	@function building:getType
	@treturn string buildingType (building chain name)
	@usage
	if building:getType() == "core_building" then
		--do stuff
	end
	*/
	types.building.set_function("getType", &buildingStructHelpers::getType);
	/***
	Get name of building level (as per export\_descr\_buildings.txt).

	@function building:getName
	@treturn string buildingName
	@usage
	if building:getName() == "large_stone_wall" then
		--do stuff
	end
	*/
	types.building.set_function("getName", &buildingStructHelpers::getName);

	//types.building.set_function("addCapability", &buildingStructHelpers::addCapability);


	///BuildingsQueue
	//@section buildingsQueueTable

	/***
	Basic buildingsQueue table

	@tfield int currentlyBuilding position in queue of building currently under construction, usually 1
	@tfield int numBuildingsInQueue maximum is 6
	@tfield getBuildingInQueue getBuildingInQueue by position in queue (1-6)

	@table buildingsQueue
	*/
	types.buildingsQueue = luaState.new_usertype<buildingsQueue>("buildingsQueue");
	types.buildingsQueue.set("currentlyBuilding", &buildingsQueue::currentBuildingIndex);
	types.buildingsQueue.set("numBuildingsInQueue", &buildingsQueue::buildingsInQueue);
	/***
	Get building in queue by position

	@function buildingsQueue:getBuildingInQueue
	@tparam int position
	@treturn buildingInQueue buildingInQueue
	@usage
	if ourQueue.numBuildingsInQueue > 0 then
		local result = "ourQueue:\n\t"
		for i = 1, ourQueue.numBuildingsInQueue, 1 do
			local ourQueueBuld = ourQueue:getBuildingInQueue(i)
			result = result..i.." "..ourQueueBuld:getQueueBuildingName().."\n\t"
		end
		print(result)
	end
	*/
	types.buildingsQueue.set_function("getBuildingInQueue", &buildingStructHelpers::getBuildingInQueue);


	///BuildingInQueue
	//@section buildingInQueueTable

	/***
	Basic buildingInQueue table

	@tfield building building Is nil if building doesn't exist yet.
	@tfield settlementStruct settlement
	@tfield int currentLevel
	@tfield int previousLevel
	@tfield int buildCost
	@tfield int buildTurnsPassed
	@tfield int buildTurnsRemaining
	@tfield int percentBuilt
	@tfield getQueueBuildingType getQueueBuildingType
	@tfield getQueueBuildingName getQueueBuildingName

	@table buildingInQueue
	*/
	types.buildingInQueue = luaState.new_usertype<buildingInQueue>("buildingInQueue");
	types.buildingInQueue.set("building", &buildingInQueue::existsBuilding);
	types.buildingInQueue.set("settlement", &buildingInQueue::settlement);
	types.buildingInQueue.set("currentLevel", &buildingInQueue::currentLevel);
	types.buildingInQueue.set("previousLevel", &buildingInQueue::pastLevel);
	types.buildingInQueue.set("buildCost", &buildingInQueue::buildCost);
	types.buildingInQueue.set("buildTurnsPassed", &buildingInQueue::buildTurnsPassed);
	types.buildingInQueue.set("buildTurnsRemaining", &buildingInQueue::turnsToBuild);
	types.buildingInQueue.set("percentBuilt", &buildingInQueue::percentBuild);
	/***
	Get name of building in queue type (chain)

	@function buildingInQueue:getQueueBuildingType
	@treturn string buildingType (building chain name)
	@usage
	if ourQueueBld:getQueueBuildingType() = "core_building" then
		--do stuff
	end
	*/
	types.buildingInQueue.set_function("getQueueBuildingType", &buildingStructHelpers::getQueueType);
	/***
	Get name of building in queue level

	@function buildingInQueue:getQueueBuildingName
	@treturn string buildingName
	@usage
	if ourQueueBld:getQueueBuildingName() = "wooden_pallisade" then
		--do stuff
	end
	*/
	types.buildingInQueue.set_function("getQueueBuildingName", &buildingStructHelpers::getQueueName);


	///Guild
	//@section guildTable

	/***
	Basic guild table

	@tfield string name
	@tfield int id
	@tfield int level1
	@tfield int level2
	@tfield int level3

	@table guild
	*/
	types.guild = luaState.new_usertype<guild>("guild");
	types.guild.set("name", sol::property(
		&guildHelpers::getStringProperty<guild_name>, &guildHelpers::setStringProperty<guild_name>));
	types.guild.set("id", &guild::id);
	types.guild.set("level1", &guild::level1Threshold);
	types.guild.set("level2", &guild::level2Threshold);
	types.guild.set("level3", &guild::level3Threshold);

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
	-- ID -> Internal Name
	0   = "gold";
	1   = "silver";
	2   = "fish";
	3   = "furs";
	4   = "grain";
	5   = "timber";
	6   = "iron";
	7   = "ivory";
	8   = "wine";
	9   = "slaves";
	10  = "chocolate";
	11  = "marble";
	12  = "textiles";
	13  = "dyes";
	14  = "tobacco";
	15  = "silk";
	16  = "sugar";
	17  = "sulfur";
	18  = "tin";
	19  = "spices";
	20  = "cotton";
	21  = "amber";
	22  = "coal";
	23  = "wool";
	24  = "elephants";
	25  = "camels";
	26  = "dogs";
	27  = "generic";
	--
	if tradeResource:getResourceID() == 23 then --wool
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
	@tfield int isBoarded
	@tfield int ladders
	@tfield int rams
	@tfield int towers
	@tfield getCharacter getCharacter
	@tfield int numOfCharacters Includes Auxiliary generals and agents (i.e all characters excluding the leading general)
	@tfield stackStruct boardedArmy army embarked on this fleet stack
	@tfield stackStruct shipArmy fleet that this army stack is embarked on
	@tfield portStruct blockedPort
	@tfield character leader Returns nil if stack is inside residence (fleet, settlement, fort).
	@tfield findInSettlement findInSettlement
	@tfield findInFort findInFort
	@tfield int totalStrength
	@tfield float reform_point_x X coordinate to which the retreating units will go.
	@tfield float reform_point_y Y coordinate to which the retreating units will go.
	@tfield createEOPUnit createEOPUnit
	@tfield createUnit createUnit
	@tfield sortStack sortStack
	@tfield mergeArmies mergeArmies
	@tfield createUnitByIDX createUnitByIDX
	@tfield siegeSettlement siegeSettlement
	@tfield siegeFort siegeFort
	@tfield attackArmy attackArmy
	@tfield siegeStruct siege Current siege.


	@table stackStruct
	*/
	types.stackStruct = luaState.new_usertype<stackStruct>("stackStruct");
	types.stackStruct.set("faction", &stackStruct::faction);
	types.stackStruct.set("isBoarded", &stackStruct::isBoarded);
	types.stackStruct.set("ladders", &stackStruct::ladders);
	types.stackStruct.set("rams", &stackStruct::rams);
	types.stackStruct.set("towers", &stackStruct::towers);

	/***
	Sort units in a stack. Use the sortType enum to specify the sorting mode.
	@function stackStruct:sortStack
	@tparam int sortMode
	@tparam int sortMode2
	@tparam int sortMode3
	@usage
	-- Note: Generals will always remain at the start of the stack
	-- 1 = EDU Type
	-- 2 = Category
	-- 3 = Class
	-- 4 = Soldier Count
	-- 5 = Experience
	-- 6 = Category + Class
	-- 7 = AI unit value

	function onFactionTurnStart(eventData)
	local faction = eventData.faction
    -- If it's not the players turn, don't sort
    if faction.isPlayerControlled == 0 then return end;

    -- Sort all the stacks on the map right before the turn starts
	-- Note: Generals will always remain at the start of the stack
	-- 1 = EDU Type
	-- 2 = Category
	-- 3 = Class
	-- 4 = Soldier Count
	-- 5 = Experience
	-- 6 = Category + Class
	-- 7 = AI unit value

	function onFactionTurnStart(eventData)
	local faction = eventData.faction
	-- If it's not the players turn, don't sort
	if faction.isPlayerControlled == 0 then return end;

	-- Sort all the stacks on the map right before the turn starts
	local factionsNum = stratmap.game.getFactionsCount();
	for i = 0, factionsNum - 1 do
		local faction = stratmap.game.getFaction(i);
		for j = 0, faction.stacksNum - 1 do
			local stack = faction:getStack(j);
			if stack then
				-- Debug Info
				-- print("\n\n")
				-- print("-- Unsorted Stack --")
				-- for k = 0, stack.numOfUnits - 1 do
				--     local unit = stack:getUnit(k);
				--     if unit.eduEntry.Type then
				--         print(unit.eduEntry.Type)
				--     end
				-- end

				-- Sort the stack by category + class, then by soldier count, then by experience
				stack:sortStack(sortType.categoryClass, sortType.soldierCount, sortType.experience)

				-- print("\n\n")
				-- print("-- Sorted Stack --")
				-- for k = 0, stack.numOfUnits - 1 do
				--     local unit = stack:getUnit(k);
				--     if unit.eduEntry.Type then
				--         print(unit.eduEntry.Type)
				--     end
				-- end
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
	@usage
	army:mergeArmies(anotherArmy);
	*/
	types.stackStruct.set_function("mergeArmies", &stackStructHelpers::mergeArmies);

	/***
	Besiege the specified settlement, or attack it if already besieging it. Requires movement points.
	@function stackStruct:siegeSettlement
	@tparam settlementStruct settlement
	@tparam bool isAttack if this is false it makes the army maintain a siege
	@usage
	stackStruct:siegeSettlement(settlement);
	*/
	types.stackStruct.set_function("siegeSettlement", &stackStructHelpers::siegeSettlement);

	/***
	Besiege the specified fort, or attack it if already besieging it. Requires movement points.
	@function stackStruct:siegeFort
	@tparam fortStruct fort
	@tparam bool isAttack if this is false it makes the army maintain a siege
	@usage
	stackStruct:siegeFort(fort);
	*/
	types.stackStruct.set_function("siegeFort", &stackStructHelpers::siegeFort);
	
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

	types.stackStruct.set("siege", &stackStruct::siege);
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
		const auto faction = campaign->factionsSortedByID[i];
		if (faction)
			factions.insert_or_assign(faction->factSmDescr->facName, i);
	}
	const auto stratMap = gameData->stratMap;
	if (!stratMap)
		return;
	for (int i = 0; i < stratMap->regionsNum; i++)
	{
		const auto region = &stratMap->regions[i];
		if (region)
			regions.insert_or_assign(region->regionName, i);
		if(region->settlement)
			settlements.insert_or_assign(region->settlement->name, i);
	}
	for (int i = 0; i < gameHelpers::getReligionCount(); i++)
	{
		auto religionName = gameHelpers::getReligionName(i);
		if (religionName)
			religionNames.insert_or_assign(i, religionName);
	}
	hashLoaded = true;
}
