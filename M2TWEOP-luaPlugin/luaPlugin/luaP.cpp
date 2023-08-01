///
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
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
	auto funcResult = luaState.script(*script);
	if (!funcResult.valid())
	{
		sol::error luaError = funcResult;
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
		sol::usertype<generalCharacterictics>namedCharacter;
		sol::usertype<anchillary>ancillary;
		sol::usertype<traitContainer>traitContainerT;
		sol::usertype<EduEntry>EduEntry;
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
		MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);
		return nullptr;
	}



	sol::load_result fileRes = luaState.load_file(luaFilePath);
	if (!fileRes.valid()) { // This checks the syntax of your script, but does not execute it
		sol::error luaError = fileRes;
		MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);
		return nullptr;
	}
	sol::protected_function_result result1 = fileRes(); // this causes the script to execute
	if (!result1.valid()) {
		sol::error luaError = result1;
		MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);
		return nullptr;
	}


	//luaState.new_enum(
	//	"buildingCap",
	//    "population_growth_bonus", buildingStructHelpers::buildingCap::population_growth_bonus,
	//    "population_loyalty_bonus", buildingStructHelpers::buildingCap::population_loyalty_bonus,
	//    "population_health_bonus", buildingStructHelpers::buildingCap::population_health_bonus,
	//    "trade_base_income_bonus", buildingStructHelpers::buildingCap::trade_base_income_bonus,
	//    "trade_level_bonus", buildingStructHelpers::buildingCap::trade_level_bonus,
	//    "trade_fleet", buildingStructHelpers::buildingCap::trade_fleet,
	//    "taxable_income_bonus", buildingStructHelpers::buildingCap::taxable_income_bonus,
	//    "mine_resource", buildingStructHelpers::buildingCap::mine_resource,
	//    "farming_level", buildingStructHelpers::buildingCap::farming_level,
	//    "road_level", buildingStructHelpers::buildingCap::road_level,
	//    "gate_strength", buildingStructHelpers::buildingCap::gate_strength,
	//    "gate_defences", buildingStructHelpers::buildingCap::gate_defences,
	//    "wall_level", buildingStructHelpers::buildingCap::wall_level,
	//    "tower_level", buildingStructHelpers::buildingCap::tower_level,
	//    "armour", buildingStructHelpers::buildingCap::armour,
	//    "stage_games", buildingStructHelpers::buildingCap::stage_games,
	//    "stage_races", buildingStructHelpers::buildingCap::stage_races,
	//    "fire_risk", buildingStructHelpers::buildingCap::fire_risk,
	//    "weapon_melee_simple", buildingStructHelpers::buildingCap::weapon_melee_simple,
	//    "weapon_melee_blade", buildingStructHelpers::buildingCap::weapon_melee_blade,
	//    "weapon_missile_mechanical", buildingStructHelpers::buildingCap::weapon_missile_mechanical,
	//    "weapon_missile_gunpowder", buildingStructHelpers::buildingCap::weapon_missile_gunpowder,
	//    "weapon_artillery_mechanical", buildingStructHelpers::buildingCap::weapon_artillery_mechanical,
	//    "weapon_artillery_gunpowder", buildingStructHelpers::buildingCap::weapon_artillery_gunpowder,
	//    "weapon_naval_gunpowder", buildingStructHelpers::buildingCap::weapon_naval_gunpowder,
	//    "upgrade_bodyguard", buildingStructHelpers::buildingCap::upgrade_bodyguard,
	//    "recruits_morale_bonus", buildingStructHelpers::buildingCap::recruits_morale_bonus,
	//    "recruits_exp_bonus", buildingStructHelpers::buildingCap::recruits_exp_bonus,
	//    "happiness_bonus", buildingStructHelpers::buildingCap::happiness_bonus,
	//    "law_bonus", buildingStructHelpers::buildingCap::law_bonus,
	//    "construction_cost_bonus_military", buildingStructHelpers::buildingCap::construction_cost_bonus_military,
	//    "construction_cost_bonus_religious", buildingStructHelpers::buildingCap::construction_cost_bonus_religious,
	//    "construction_cost_bonus_defensive", buildingStructHelpers::buildingCap::construction_cost_bonus_defensive,
	//    "construction_cost_bonus_other", buildingStructHelpers::buildingCap::construction_cost_bonus_other,
	//    "construction_time_bonus_military", buildingStructHelpers::buildingCap::construction_time_bonus_military,
	//    "construction_time_bonus_religious", buildingStructHelpers::buildingCap::construction_time_bonus_religious,
	//    "construction_time_bonus_defensive", buildingStructHelpers::buildingCap::construction_time_bonus_defensive,
	//    "construction_time_bonus_other", buildingStructHelpers::buildingCap::construction_time_bonus_other,
	//    "construction_cost_bonus_wooden", buildingStructHelpers::buildingCap::construction_cost_bonus_wooden,
	//    "construction_cost_bonus_stone", buildingStructHelpers::buildingCap::construction_cost_bonus_stone,
	//    "construction_time_bonus_wooden", buildingStructHelpers::buildingCap::construction_time_bonus_wooden,
	//    "construction_time_bonus_stone", buildingStructHelpers::buildingCap::construction_time_bonus_stone,
	//    "free_upkeep", buildingStructHelpers::buildingCap::free_upkeep,
	//    "pope_approval", buildingStructHelpers::buildingCap::pope_approval,
	//    "pope_disapproval", buildingStructHelpers::buildingCap::pope_disapproval,
	//    "religion_level", buildingStructHelpers::buildingCap::religion_level,
	//    "amplify_religion_level", buildingStructHelpers::buildingCap::amplify_religion_level,
	//    "archer_bonus", buildingStructHelpers::buildingCap::archer_bonus,
	//    "cavalry_bonus", buildingStructHelpers::buildingCap::cavalry_bonus,
	//    "heavy_cavalry_bonus", buildingStructHelpers::buildingCap::heavy_cavalry_bonus,
	//    "gun_bonus", buildingStructHelpers::buildingCap::gun_bonus,
	//    "navy_bonus", buildingStructHelpers::buildingCap::navy_bonus,
	//    "recruitment_cost_bonus_naval", buildingStructHelpers::buildingCap::recruitment_cost_bonus_naval,
	//    "retrain_cost_bonus", buildingStructHelpers::buildingCap::retrain_cost_bonus,
	//    "weapon_projectile", buildingStructHelpers::buildingCap::weapon_projectile,
	//    "income_bonus", buildingStructHelpers::buildingCap::income_bonus,
	//    "recruitment_slots", buildingStructHelpers::buildingCap::recruitment_slots
	//);

	///M2TWEOP
	//@section m2tweopTable

	/***
	Basic M2TWEOP table

	@tfield getModPath getModPath
	@tfield saveGame saveGame
	@tfield getGameVersion getGameVersion
	@tfield getPluginPath  getPluginPath
	@tfield loadTexture loadTexture
	@tfield unloadTexture unloadTexture
	@tfield setAncillariesLimit setAncillariesLimit
	@tfield unlockGameConsoleCommands unlockGameConsoleCommands
	@tfield setMaxBgSize setMaxBgSize
	@tfield toggleUnitsBMapHighlight toggleUnitsBMapHighlight
	@tfield setReligionsLimit setReligionsLimit
	@tfield isTileFree isTileFree
	@tfield getGameTileCoordsWithCursor getGameTileCoordsWithCursor
	@tfield getTileRegionID getTileRegionID
	@tfield getTileVisibility getTileVisibility
	@tfield getRegionOwner getRegionOwner
	@tfield setEDUUnitsSize setEDUUnitsSize
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
	Set the maximum number of religions in the mod (per descr\_religions.txt)
	@function M2TWEOP.setReligionsLimit
	@tparam int newLimit maximum: 127
	@usage
	M2TWEOP.setReligionsLimit(25);
	*/
	tables.M2TWEOPTable.set_function("setReligionsLimit", &m2tweopHelpers::seReligionsLimit);

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
	Get a specific tile's visibility according to faction
	@function stratMap.getTileVisibility
	@tparam factionStruct faction
	@tparam xCoord x coord of the tile
	@tparam yCoord y coord of the tile
	@treturn isVisible 0 = not visible, 1 = visible
	@usage
	local tile = sMap.getTile(182, 243);
	local faction = stratmap.game.getFaction(2);
	local isVisible = tile:getTileVisibility(faction)
	*/
	typeAll.M2TWEOPTable.set_function("getTileVisibility", &m2tweopHelpers::getTileVisibility);
	/***
	Get the owner of a region by RegionID.
	@function M2TWEOP.getRegionOwner
	@tparam int regionID
	@treturn factionStruct owner
	@usage
	local regionID=M2TWEOP.getTileRegionID(55,25);
	local ownerFac=M2TWEOP.getRegionOwner(regionID);
	*/
	tables.M2TWEOPTable.set_function("getRegionOwner", &m2tweopHelpers::getRegionOwner);

	/// gameSTDUITable
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

	///uiElementStruct
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
	@function gameSTDUI.getSubElement
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


	///Strat Map Objects
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
	@function objects.startDrawModelAt
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
	@function objects.stopDrawModel
	@tparam int modelId Unique ID
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS", 1);
	stratmap.objects.startDrawModelAt(1, 50, 150, 2.25);
	stratmap.objects.stopDrawModel(1);
	*/
	tables.objectsTable.set_function("stopDrawModel", &objectsHelpers::stopDrawModel);
	/***
	Add a new .cas campaign strategy model to the game with a unique ID. This should be called during onPluginLoad()
	@function objects.addModelToGame
	@tparam string path Relative path from the modfolder (starting with "data/").
	@tparam int modelId  Unique ID to use the model later.
	@usage
	stratmap.objects.addModelToGame("data/models_strat/residences/invisible.CAS",1);
	*/
	tables.objectsTable.set_function("addModelToGame", &objectsHelpers::addModelToGame);
	/***
	Add a new .cas character strategy model to the game with a unique name. Only add it after loading to campaign map!
	@function objects.addCharacterCas
	@tparam string skeleton name of skeleton used.
	@tparam string caspath Relative path from the mods folder (starting with "mods/").
	@tparam string shadowcaspath Relative path from the mods folder (starting with "mods/").
	@tparam string typename Name of the new model used to assign.
	@tparam string texturepath Relative path from the mods folder (starting with "mods/").
	@usage
	stratmap.objects.addCharacterCas("strat_named_with_army","mods/Bare_Geomod/data/models_strat/islamic_general2.cas","mods/Bare_Geomod/data/models_strat/shadow_sword2.cas","islamic_general2","mods/Bare_Geomod/data/models_strat/textures/islamic_general_turks.tga");
	*/
	tables.objectsTable.set_function("addCharacterCas", &generalHelpers::addCharacterCas);
	/***
	Set the strategy model for object at specified coordinates, works only for supported object types
	@function objects.setModel
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
	@function objects.replaceTile
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
	@function camera.move
	@tparam int xCoord
	@tparam int yCoord
	@usage
	stratmap.camera.move(1,2);
	*/
	tables.cameraTable.set_function("move", &cameraHelpers::moveStratCamera);
	/***
	Quickly move the camera to the specified tile.
	@function camera.jump
	@tparam int xCoord
	@tparam int yCoord
	@usage
	stratmap.camera.jump(1,2);
	*/
	tables.cameraTable.set_function("jump", &cameraHelpers::snapStratCamera);
	/***
	Set the zoom level of the camera.
	@function camera.zoom
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

	@table stratmap.game
*/
	tables.gameTable = luaState.create_table();
	/***
	Execute a Medieval II console command.
	@function game.callConsole
	@treturn string error Note: string can be empty but not nil
	@usage
	-- Creating units, adding money
	function onCharacterSelected(selectedChar)
		local err = stratmap.game.callConsole("add_money", "2321")
		local err2 = stratmap.game.callConsole("create_unit", "testcharacter 'Cool Unit' 4 1 1 1")
		print(err)
		print(err2)
	end
	*/
	tables.gameTable.set_function("callConsole", &gameHelpers::callConsole);
	/***
	Get the amount of factions
	@function game.getFactionsCount
	@treturn int facNumber Amount of factions
	@usage
	local facNum=stratmap.game.getFactionsCount();
	*/
	tables.gameTable.set_function("getFactionsCount", &gameHelpers::getFactionsCount);
	/***
	Get a faction by the index, commonly used when iterating over all factions using getFactionsCount()
	@function game.getFaction
	@tparam int Index of the faction.
	@treturn factionStruct faction
	@usage
	faction=stratmap.game.getFaction(2);
	*/
	tables.gameTable.set_function("getFaction", &gameHelpers::getFaction);
	/***
	Get a guild by the index.
	@function game.getGuild
	@tparam int index
	@treturn guild guild
	@usage
	ourGuild=stratmap.game.getGuild(1);
	*/
	tables.gameTable.set_function("getGuild", &gameHelpers::getGuild);

	/***
	Create a new character at the specified coordinates.
	@function game.createCharacterByString
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
	@function game.createArmy
	@tparam character ourGeneral Character class, not named character class!
	@treturn stackStruct army
	@usage
	army=stratmap.game.createArmy(gen);
	*/
	tables.gameTable.set_function("createArmy", &gameHelpers::createArmy);
	/***
	Create an army in a settlement (don't need a character). Used to add units to an empty settlement.
	@function game.createArmyInSettlement
	@tparam settlementStruct settlement
	@treturn stackStruct army
	@usage
	army=stratmap.game.createArmyInSettlement(sett);
	*/
	tables.gameTable.set_function("createArmyInSettlement", &gameHelpers::createArmyInSettlement);
	/***
	Get a script counter value, works for counters and for event\_counters
	@function game.getScriptCounter
	@tparam string counterName The name of the counter
	@treturn bool isExist Returns true if the counter exists i.e it has been used at least once in any way in the campaign\_script
	@treturn int counterValue Returns the value of the counter
	@usage
	isExist, counterValue = stratmap.game.getScriptCounter("SomeCounter")
	*/
	tables.gameTable.set_function("getScriptCounter", &gameHelpers::getScriptCounter);
	/***
	Set an event\_counter value, does not work for counters, only event\_counters.
	@function game.setScriptCounter
	@tparam string counterName
	@tparam int value
	@usage
	stratmap.game.setScriptCounter("SomeCounter", 25)
	*/
	tables.gameTable.set_function("setScriptCounter", &gameHelpers::setScriptCounter);
	/***
	Fire a game event message. Picture needs to be provided in the ui folders as default.
	@function game.historicEvent
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
	@function game.scriptCommand
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

	@tfield objects objects
	@tfield camera camera
	@tfield game game


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
	@tfield int soldierCountStratMap soldiers count. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int exp soldiers expierence. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int armourLVL soldiers armour. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int weaponLVL soldiers weapon. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
	@tfield int soldierCountStratMapMax Read only
	@tfield int soldierCountBattleMap Read only
	@tfield character character
	@tfield stackStruct army
	@tfield kill kill
	@tfield setParams setParams change soldierCountStratMap, exp, armourLVL, weaponLVL at one time.
	@tfield string alias


	@table unit
	*/
	types.unit = luaState.new_usertype<unit>("unit");
	types.unit.set("eduEntry", &unit::eduEntry);
	types.unit.set("movePoints", sol::property(&unitHelpers::getMovepoints, &unitHelpers::setMovepoints));
	types.unit.set("soldierCountStratMap", sol::property(&unitHelpers::getsoldierCountStratMap, &unitHelpers::setSoldiersCount));
	types.unit.set("exp", sol::property(&unitHelpers::getExp, &unitHelpers::setExp));
	types.unit.set("armourLVL", sol::property(&unitHelpers::getarmourLVL, &unitHelpers::setarmourLVL));
	types.unit.set("weaponLVL", sol::property(&unitHelpers::getweaponLVL, &unitHelpers::setweaponLVL));
	types.unit.set("soldierCountStratMapMax", sol::property(&unitHelpers::getMaxSoldiersCount));
	types.unit.set("soldierCountBattleMap", &unit::numberTact);
	types.unit.set("character", &unit::general);
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


	///EduEntry
	//@section eduEntryTable

	/***
	Basic eduEntry table

	@tfield string Type - Get only
	@tfield string Soldier - Get only
	@tfield string UnitCardTga - Get only
	@tfield string InfoCardTga - Get only
	@tfield int Index - Get only
	@tfield int UnitCreatedCounter - Get only
	@tfield int SoldierCount
	@tfield float Mass
	@tfield float Width
	@tfield float Height
	@tfield bool haveAttributeLegio
	@tfield float MoveSpeedMod
	@tfield float UnitSpacingFrontToBackClose
	@tfield float UnitSpacingSideToSideClose
	@tfield float UnitSpacingFrontToBackLoose
	@tfield float UnitSpacingSideToSideLoose
	@tfield int StatHealth
	@tfield int StatHealthAnimal
	@tfield int StatHeat
	@tfield int StatGround1
	@tfield int StatGround2
	@tfield int StatGround3
	@tfield int StatGround4
	@tfield int StatCost1
	@tfield int StatCost2
	@tfield int StatCost3
	@tfield int StatCost4
	@tfield int StatCost5
	@tfield int StatCost6
	@tfield int StatCost7
	@tfield int StatCost8
	@tfield int Morale
	@tfield int MoraleLocked
	@tfield int StatFood1
	@tfield int StatFood2
	@tfield int Ammunition

	@table eduEntry
	*/
	types.EduEntry = luaState.new_usertype<EduEntry>("eduEntry");
	types.EduEntry.set("Type", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_Type>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_Type>
		));
	types.EduEntry.set("Soldier", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_Soldier>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_Soldier>
		));
	types.EduEntry.set("UnitCardTga", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_UnitCardTga>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_UnitCardTga>
		));
	types.EduEntry.set("InfoCardTga", sol::property(
		&luaGetSetFuncs::getStringPropertyEDU<EduEntryStruct_InfoCardTga>, &luaGetSetFuncs::setStringPropertyEDU<EduEntryStruct_InfoCardTga>
		));
	types.EduEntry.set("Index", &EduEntry::Index);
	types.EduEntry.set("UnitCreatedCounter", &EduEntry::UnitCreatedCounter);
	types.EduEntry.set("SoldierCount", &EduEntry::SoldierCount);
	types.EduEntry.set("Mass", &EduEntry::Mass);
	types.EduEntry.set("Width", &EduEntry::Width);
	types.EduEntry.set("Height", &EduEntry::Height);
	types.EduEntry.set("haveAttributeLegio", sol::property(&eopEduHelpers::haveAttributeLegioGet, &eopEduHelpers::haveAttributeLegioSet));
	types.EduEntry.set("MoveSpeedMod", &EduEntry::MoveSpeedMod);
	types.EduEntry.set("UnitSpacingFrontToBackClose", &EduEntry::UnitSpacingFrontToBackClose);
	types.EduEntry.set("UnitSpacingSideToSideClose", &EduEntry::UnitSpacingSideToSideClose);
	types.EduEntry.set("UnitSpacingFrontToBackLoose", &EduEntry::UnitSpacingFrontToBackLoose);
	types.EduEntry.set("UnitSpacingSideToSideLoose", &EduEntry::UnitSpacingSideToSideLoose);
	types.EduEntry.set("StatHealth", &EduEntry::StatHealth);
	types.EduEntry.set("StatHealthAnimal", &EduEntry::StatHealthAnimal);
	types.EduEntry.set("StatHeat", &EduEntry::StatHeat);
	types.EduEntry.set("StatGround1", &EduEntry::StatGround1);
	types.EduEntry.set("StatGround2", &EduEntry::StatGround2);
	types.EduEntry.set("StatGround3", &EduEntry::StatGround3);
	types.EduEntry.set("StatGround4", &EduEntry::StatGround4);
	types.EduEntry.set("StatCost1", &EduEntry::StatCost1);
	types.EduEntry.set("StatCost2", &EduEntry::StatCost2);
	types.EduEntry.set("StatCost3", &EduEntry::StatCost3);
	types.EduEntry.set("StatCost4", &EduEntry::StatCost4);
	types.EduEntry.set("StatCost5", &EduEntry::StatCost5);
	types.EduEntry.set("StatCost6", &EduEntry::StatCost6);
	types.EduEntry.set("StatCost7", &EduEntry::StatCost7);
	types.EduEntry.set("StatCost8", &EduEntry::StatCost8);
	types.EduEntry.set("Morale", &EduEntry::Morale);
	types.EduEntry.set("MoraleLocked", &EduEntry::MoraleLocked);
	types.EduEntry.set("StatFood1", &EduEntry::StatFood1);
	types.EduEntry.set("StatFood2", &EduEntry::StatFood2);
	types.EduEntry.set("Ammunition", &EduEntry::Ammunition);



	///Character
	//@section characterTable

	/***
	characters as they exist on the strategy map - dead characters, wives, children, and off-map characters do not have these fields.

	@tfield int xCoord
	@tfield int yCoord
	@tfield namedCharacter namedCharacter
	@tfield unit bodyguards
	@tfield stackStruct armyLeaded
	@tfield stackStruct armyNotLeaded in the stack but not leading it
	@tfield float movePointsMax
	@tfield float movePointsModifier
	@tfield float movePoints
	@tfield string ability see descr\_hero\_abilities.xml
	@tfield getTypeID getTypeID
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
	types.character.set("movePointsMax", &general::movepointsMax);
	types.character.set("movePointsModifier", &general::movepointsModifier);
	types.character.set("movePoints", sol::property(&generalHelpers::getMovepoints, &generalHelpers::setMovepoints));
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
	@tfield bool isMale
	@tfield int age
	@tfield float yearOfBirth For example with 4 turns per year, the yearOfBirth could be 1840.25
	@tfield factionStruct faction
	@tfield int subFaction
	@tfield namedCharacter parent
	@tfield namedCharacter spouse
	@tfield namedCharacter[4] childs example: ourChar.childs[2].fullName
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

	@table namedCharacter
	*/
	types.namedCharacter = luaState.new_usertype<generalCharacterictics>("namedCharacter");
	types.namedCharacter.set("index", &generalCharacterictics::index);
	types.namedCharacter.set("character", &generalCharacterictics::gen);
	types.namedCharacter.set("shortName", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_shortName>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_shortName>));
	types.namedCharacter.set("fullName", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_fullName>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_fullName>));
	types.namedCharacter.set("localizedDisplayName", sol::property(&technicalHelpers::namedCharUniStringToStr<namedChar_localizedFullName>, &technicalHelpers::namedCharSetLocalizedFullName));
	types.namedCharacter.set("label", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_label>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_label>));
	types.namedCharacter.set("portrait", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_portrait>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_portrait>));
	types.namedCharacter.set("portrait2", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_portrait2>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_portrait2>));
	types.namedCharacter.set("portrait_custom", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_portrait_custom>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_portrait_custom>));
	types.namedCharacter.set("modelName", sol::property(&luaGetSetFuncs::getStringPropertyGenChar<generalCharactericticsStruct_modelName>, &luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_modelName>));
	types.namedCharacter.set("status", &generalCharacterictics::status);
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
	@treturn int liveStatus true = alive, false = dead
	@usage
	if ourcharacter:isAlive() == true then
		ourcharacter.character:kill()
	end
	*/
	types.namedCharacter.set_function("isAlive", &generalCharactericticsHelpers::isAlive);
	types.namedCharacter.set("isMale", sol::property(&generalCharactericticsHelpers::getIsMale, &generalCharactericticsHelpers::setIsMale));
	types.namedCharacter.set("age", sol::property(&generalCharactericticsHelpers::getAge, &generalCharactericticsHelpers::setAge));
	types.namedCharacter.set("yearOfBirth", &generalCharacterictics::yearOfBirth);
	types.namedCharacter.set("faction", &generalCharacterictics::faction);
	types.namedCharacter.set("subFaction", &generalCharacterictics::subFaction);
	types.namedCharacter.set("parent", &generalCharacterictics::parent);
	types.namedCharacter.set("spouse", &generalCharacterictics::spouse);
	types.namedCharacter.set("childs", sol::property([](generalCharacterictics& self) { return std::ref(self.childs); }));
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
	types.namedCharacter.set("ancNum", &generalCharacterictics::anchNum);
	/***
	Get the pointer to the ancillary using it's index. You can iterate over a character's ancillaries for example by going from index 0 to ancNum - 1.
	@function namedCharacter:getAncillary
	@tparam int index
	@treturn ancillary ancillary
	@usage
	ancillary = ourNamedCharacter:getAncillary(2)
	*/
	types.namedCharacter.set_function("getAncillary", &generalCharactericticsHelpers::getAnchillary);
	/***
	Add an ancillary to the named character using the name per export\_descr\_ancillaries.txt.
	@function namedCharacter:addAncillary
	@tparam string ancillaryName
	@usage
	ourNamedCharacter:addAncillary("VeryVeryGoodMan");
	*/
	types.namedCharacter.set_function("addAncillary", &generalCharactericticsHelpers::addAnchillary);
	/***
	Remove an ancillary from the named character using it's pointer. Use getAncillary function to get the specific ancillary.
	@function namedCharacter:removeAncillary
	@tparam ancillary ancillary
	@usage
	ourAnc=ourNamedCharacter:getAncillary(2);
	ourNamedCharacter:removeAncillary(ourAnc);
	*/
	types.namedCharacter.set_function("removeAncillary", &generalCharactericticsHelpers::removeAnchillary);
	types.namedCharacter.set("level", &generalCharacterictics::level);
	types.namedCharacter.set("authority", &generalCharacterictics::leaderAutority);
	types.namedCharacter.set("command", &generalCharacterictics::authority);
	types.namedCharacter.set("chivalryAndDread", &generalCharacterictics::nobility);
	types.namedCharacter.set("loyalty", &generalCharacterictics::loyality);
	types.namedCharacter.set("piety", &generalCharacterictics::piety);
	types.namedCharacter.set("influence", &generalCharacterictics::influence);
	types.namedCharacter.set("subterfuge", &generalCharacterictics::subterfuge);
	types.namedCharacter.set("charm", &generalCharacterictics::charm);
	types.namedCharacter.set("finance", &generalCharacterictics::finance);
	types.namedCharacter.set("magic", &generalCharacterictics::magic);
	types.namedCharacter.set("unorthodoxy", &generalCharacterictics::unorthodoxy);
	types.namedCharacter.set("heresyImmunity", &generalCharacterictics::heresyImmunity);
	types.namedCharacter.set("assassination", &generalCharacterictics::assassination);
	types.namedCharacter.set("sabotage", &generalCharacterictics::sabotage);
	types.namedCharacter.set("eligibility", &generalCharacterictics::eligibility);
	types.namedCharacter.set("purity", &generalCharacterictics::purity);
	types.namedCharacter.set("violence", &generalCharacterictics::violence);
	types.namedCharacter.set("disposition", &generalCharacterictics::disposition);
	types.namedCharacter.set("boldness", &generalCharacterictics::boldness);
	types.namedCharacter.set("generosity", &generalCharacterictics::generosity);
	types.namedCharacter.set("management", &generalCharacterictics::management);
	types.namedCharacter.set("bodyguardSize", &generalCharacterictics::bodyguardSize);
	types.namedCharacter.set("troopMorale", &generalCharacterictics::troopMorale);
	types.namedCharacter.set("movementPointsBonus", &generalCharacterictics::movementPointsBonus);
	types.namedCharacter.set("attack", &generalCharacterictics::attack);
	types.namedCharacter.set("defence", &generalCharacterictics::defence);
	types.namedCharacter.set("siegeAttack", &generalCharacterictics::siegeAttack);
	types.namedCharacter.set("siegeDefense", &generalCharacterictics::siegeDefense);
	types.namedCharacter.set("ambush", &generalCharacterictics::ambush);
	types.namedCharacter.set("navalCommand", &generalCharacterictics::navalCommand);
	types.namedCharacter.set("siegeEngineering", &generalCharacterictics::siegeEngineering);
	types.namedCharacter.set("nightBattle", &generalCharacterictics::nightBattle);
	types.namedCharacter.set("personalSecurity", &generalCharacterictics::personalSecurity);
	types.namedCharacter.set("publicSecurity", &generalCharacterictics::publicSecurity);
	types.namedCharacter.set("bribery", &generalCharacterictics::bribery);
	types.namedCharacter.set("bribeResistance", &generalCharacterictics::bribeResistance);
	types.namedCharacter.set("electability", &generalCharacterictics::electability);
	types.namedCharacter.set("lineOfSight", &generalCharacterictics::lineOfSight);
	types.namedCharacter.set("trainingUnits", &generalCharacterictics::trainingUnits);
	types.namedCharacter.set("trainingAgents", &generalCharacterictics::trainingAgents);
	types.namedCharacter.set("construction", &generalCharacterictics::construction);
	types.namedCharacter.set("trading", &generalCharacterictics::trading);
	types.namedCharacter.set("localPopularity", &generalCharacterictics::localPopularity);
	types.namedCharacter.set("footInTheDoor", &generalCharacterictics::footInTheDoor);
	types.namedCharacter.set("farming", &generalCharacterictics::farming);
	types.namedCharacter.set("mining", &generalCharacterictics::mining);
	types.namedCharacter.set("taxCollection", &generalCharacterictics::taxCollection);
	types.namedCharacter.set("fertility", &generalCharacterictics::fertility);
	types.namedCharacter.set("cavalryCommand", &generalCharacterictics::cavalryCommand);
	types.namedCharacter.set("infantryCommand", &generalCharacterictics::infantryCommand);
	types.namedCharacter.set("gunpowerCommand", &generalCharacterictics::gunpowerCommand);
	types.namedCharacter.set("artilleryCommand", &generalCharacterictics::artilleryCommand);
	types.namedCharacter.set("health", &generalCharacterictics::health);
	types.namedCharacter.set("squalor", &generalCharacterictics::squalor);
	types.namedCharacter.set("unrest", &generalCharacterictics::unrest);
	types.namedCharacter.set("law", &generalCharacterictics::law);
	types.namedCharacter.set("looting", &generalCharacterictics::looting);
	types.namedCharacter.set("bodyguardValour", &generalCharacterictics::bodyguardValour);
	types.namedCharacter.set("hitpoints", &generalCharacterictics::hitpoints);
	types.namedCharacter.set("trainingAnimalUnits", &generalCharacterictics::trainingAnimalUnits);
	types.namedCharacter.set("battleSurgery", &generalCharacterictics::battleSurgery);


	///Ancillary
	//@section ancillaryTable

	/***
	Basic ancillary table

	@tfield int index
	@tfield string name
	@tfield string imagePath

	@table ancillary
	*/
	types.ancillary = luaState.new_usertype<anchillary>("ancillary");
	types.ancillary.set("index", &anchillary::index);
	types.ancillary.set("name", sol::property(
		&luaGetSetFuncs::getStringPropertyAnc<anchillaryStruct_name>, &luaGetSetFuncs::setStringPropertyAnc<anchillaryStruct_name>
		));
	types.ancillary.set("imagePath", sol::property(
		&luaGetSetFuncs::getStringPropertyAnc<anchillaryStruct_imagePath>, &luaGetSetFuncs::setStringPropertyAnc<anchillaryStruct_imagePath>
		));


	///traitContainer
	//@section traitsTable

	/***
	Basic traits table

	@tfield int level
	@tfield string name
	@tfield traitContainer nextTrait
	@tfield traitContainer prevTrait

	@table traitContainer
	*/
	types.traitContainerT = luaState.new_usertype<traitContainer>("traitContainer");
	types.traitContainerT.set("level", sol::property(&luaGetSetFuncs::getTraitLevel));
	types.traitContainerT.set("name", sol::property(&luaGetSetFuncs::getTraitName));
	types.traitContainerT.set("nextTrait", sol::property(&luaGetSetFuncs::getNextTrait));
	types.traitContainerT.set("prevTrait", sol::property(&luaGetSetFuncs::getPrevTrait));


	///FactionStruct
	//@section factionStructTable

	/***
	Basic factionStruct table

	@tfield int dipNum
	@tfield getFactionName getFactionName
	@tfield int cultureID
	@tfield string ai_label
	@tfield string name
	@tfield string localizedName
	@tfield settlementStruct capital
	@tfield namedCharacter leader
	@tfield namedCharacter heir
	@tfield int isPlayerControlled 0=AI, 1=player
	@tfield int religion
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
	@tfield deleteFort deleteFort
	@tfield createFortXY createFortXY

	@table factionStruct
	*/
	types.factionStruct = luaState.new_usertype<factionStruct>("factionStruct");
	types.factionStruct.set("dipNum", &factionStruct::dipNum);
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
	types.factionStruct.set("money", &factionStruct::money);
	types.factionStruct.set("kingsPurse", &factionStruct::KingsPurse);
	types.factionStruct.set("facStrat", &factionStruct::factSmDescr);
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
	types.factionStruct.set("watchtowersNum", &factionStruct::wathtowersNum);
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
	@tfield int standardIndex Warning: resets on reload.
	@tfield int logoIndex Warning: resets on reload.
	@tfield int smallLogoIndex Warning: resets on reload.

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


	///WatchtowerStruct
	//@section watchtowerStructTable

	/***
	Basic watchtowerStruct table

	@tfield int xCoord
	@tfield int yCoord

	@table watchtowerStruct
	*/
	types.watchtowerStruct = luaState.new_usertype<watchTowerStruct>("watchtowerStruct");
	types.watchtowerStruct.set("xCoord", &watchTowerStruct::xCoord);
	types.watchtowerStruct.set("yCoord", &watchTowerStruct::yCoord);

	///FortStruct
	//@section fortStructTable

	/***
	Basic fortStruct table

	@tfield int xCoord
	@tfield int yCoord
	@tfield character governor
	@tfield stackStruct army
	@tfield factionStruct ownerFaction
	@tfield siegeStruct siege

	@table fortStruct
	*/
	types.fortStruct = luaState.new_usertype<fortStruct>("fortStruct");
	types.fortStruct.set("xCoord", &fortStruct::xCoord);
	types.fortStruct.set("yCoord", &fortStruct::yCoord);
	types.fortStruct.set("governor", &fortStruct::gubernator);
	types.fortStruct.set("army", &fortStruct::army);
	types.fortStruct.set("ownerFaction", &fortStruct::faction);
	types.fortStruct.set("siege", &fortStruct::siege);



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
	@tfield dockStruct Dock water tile, only upgraded ports have this, check for nil.


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
	@tfield int fac_creatorNum
	@tfield int regionNumber
	@tfield int level
	@tfield int isCastle
	@tfield int isProvokedRebellion
	@tfield int populationSize
	@tfield int PopGrowthBaseFarm - Get only
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
	@tfield getResource getResource
	@tfield int siegesNum
	@tfield getSiege getSiege

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
	types.settlementStruct.set("ownerFaction", &settlementStruct::ownerFac);
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
	types.settlementStruct.set("fac_creatorNum", &settlementStruct::fac_creatorModNum);
	types.settlementStruct.set("regionNumber", &settlementStruct::regionNumber);
	types.settlementStruct.set("level", &settlementStruct::level);
	types.settlementStruct.set("isCastle", &settlementStruct::isCastle);
	types.settlementStruct.set("isProvokedRebellion", &settlementStruct::isProvokedRebellion);
	types.settlementStruct.set("populationSize", &settlementStruct::populationSize);
	types.settlementStruct.set("PopGrowthBaseFarm", &settlementStruct::PopGrowthBaseFarm);
	types.settlementStruct.set("PopGrowthFarms", &settlementStruct::PopGrowthFarms);
	types.settlementStruct.set("PopGrowthHealth", &settlementStruct::PopGrowthHealth);
	types.settlementStruct.set("PopGrowthBuildings", &settlementStruct::PopGrowthBuildings);
	types.settlementStruct.set("PopGrowthTaxBonus", &settlementStruct::PopGrowthTaxBonus);
	types.settlementStruct.set("PopGrowthEntertainment", &settlementStruct::PopGrowthEntertainment);
	types.settlementStruct.set("PopGrowthTrade", &settlementStruct::PopGrowthTrade);
	types.settlementStruct.set("PopGrowthGovernorInfluence", &settlementStruct::PopGrowthGovernorInfluence);
	types.settlementStruct.set("PopGrowthSqualor", &settlementStruct::PopGrowthSqualor);
	types.settlementStruct.set("PopGrowthPlague", &settlementStruct::PopGrowthPlague);
	types.settlementStruct.set("PopGrowthTaxPenalty", &settlementStruct::PopGrowthTaxPenalty);
	types.settlementStruct.set("PublicOrderGarrison", &settlementStruct::PublicOrderGarrison);
	types.settlementStruct.set("PublicOrderLaw", &settlementStruct::PublicOrderLaw);
	types.settlementStruct.set("PublicOrderBuildingsEntertainment", &settlementStruct::PublicOrderBuildingsEntertainment);
	types.settlementStruct.set("PublicOrderGovernorInfluence", &settlementStruct::PublicOrderGovernorInfluence);
	types.settlementStruct.set("PublicOrderTaxBonus", &settlementStruct::PublicOrderTaxBonus);
	types.settlementStruct.set("PublicOrderTriumph", &settlementStruct::PublicOrderTriumph);
	types.settlementStruct.set("PublicOrderPopulationBoom", &settlementStruct::PublicOrderPopulationBoom);
	types.settlementStruct.set("PublicOrderEntertainment", &settlementStruct::PublicOrderEntertainment);
	types.settlementStruct.set("PublicOrderHealth", &settlementStruct::PublicOrderHealth);
	types.settlementStruct.set("PublicOrderGarrisonTwo", &settlementStruct::PublicOrderGarrisonTwo);
	types.settlementStruct.set("PublicOrderFear", &settlementStruct::PublicOrderFear);
	types.settlementStruct.set("PublicOrderGlory", &settlementStruct::PublicOrderGlory);
	types.settlementStruct.set("PublicOrderSqualor", &settlementStruct::PublicOrderSqualor);
	types.settlementStruct.set("PublicOrderDistanceToCapital", &settlementStruct::PublicOrderDistanceToCapital);
	types.settlementStruct.set("PublicOrderNoGovernance", &settlementStruct::PublicOrderNoGovernance);
	types.settlementStruct.set("PublicOrderTaxPenalty", &settlementStruct::PublicOrderTaxPenalty);
	types.settlementStruct.set("PublicOrderUnrest", &settlementStruct::PublicOrderUnrest);
	types.settlementStruct.set("PublicOrderBesieged", &settlementStruct::PublicOrderBesieged);
	types.settlementStruct.set("PublicOrderBlockaded", &settlementStruct::PublicOrderBlockaded);
	types.settlementStruct.set("PublicOrderCulturalUnrest", &settlementStruct::PublicOrderCulturalUnrest);
	types.settlementStruct.set("PublicOrderExcommunication", &settlementStruct::PublicOrderExcommunication);
	types.settlementStruct.set("PublicOrder", &settlementStruct::PublicOrder);
	types.settlementStruct.set("FarmsIncome", &settlementStruct::FarmsIncome);
	types.settlementStruct.set("TaxesIncome", &settlementStruct::TaxesIncome);
	types.settlementStruct.set("MiningIncome", &settlementStruct::MiningIncome);
	types.settlementStruct.set("TradeIncome", &settlementStruct::TradeIncome);
	types.settlementStruct.set("DiplomaticIncome", &settlementStruct::DiplomaticIncome);
	types.settlementStruct.set("DemolitionIncome", &settlementStruct::DemolitionIncome);
	types.settlementStruct.set("LootingIncome", &settlementStruct::LootingIncome);
	types.settlementStruct.set("BuildingsIncome", &settlementStruct::BuildingsIncome);
	types.settlementStruct.set("AdminIncome", &settlementStruct::AdminIncome);
	types.settlementStruct.set("ConstructionExpense", &settlementStruct::ConstructionExpense);
	types.settlementStruct.set("RecruitmentExpense", &settlementStruct::RecruitmentExpense);
	types.settlementStruct.set("DiplomaticExpense", &settlementStruct::DiplomaticExpense);
	types.settlementStruct.set("CorruptionExpense", &settlementStruct::CorruptionExpense);
	types.settlementStruct.set("EntertainmentExpense", &settlementStruct::EntertainmentExpense);
	types.settlementStruct.set("DevastationExpense", &settlementStruct::DevastationExpense);
	types.settlementStruct.set("TotalIncomeWithoutAdmin", &settlementStruct::TotalIncomeWithoutAdmin);
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
	types.buildingInQueue.set("percentBuilt", &buildingInQueue::petcentBuilded);
	/***
	Get name of building in queue type (chain)

	@function building:getQueueBuildingType
	@treturn string buildingType (building chain name)
	@usage
	if ourQueueBld:getQueueBuildingType() = "core_building" then
		--do stuff
	end
	*/
	types.buildingInQueue.set_function("getQueueBuildingType", &buildingStructHelpers::getQueueType);
	/***
	Get name of building in queue level

	@function building:getQueueBuildingName
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
	function onSettlementSelected(selectedSett)
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
	@tfield mergeArmies mergeArmies
	@tfield createUnitByIDX createUnitByIDX
	@tfield siegeSettlement siegeSettlement Call it twice to initiate an assault.
	@tfield attackArmy attackArmy
	@tfield siegeStruct siege Current siege.


	@table stackStruct
	*/
	types.stackStruct = luaState.new_usertype<stackStruct>("stackStruct");
	types.stackStruct.set("faction", &stackStruct::faction);
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
	Merge 2 armies on the strat map.
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
	@usage
	stackStruct:siegeSettlement(settlement);
	*/
	types.stackStruct.set_function("siegeSettlement", &stackStructHelpers::siegeSettlement);
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
	///siegeStruct
	//@section siegeStruct

	/***
	Basic siegeStruct table

	@tfield stackStruct besieger
	@tfield settlementStruct besiegedSettlement
	@tfield fortStruct besiegedFort

	@table siegeStruct
	*/
	types.siege = luaState.new_usertype<siegeS>("siegeStruct");
	types.siege.set("besieger", &siegeS::army);
	types.siege.set("siegedSettlement", sol::property(
		&siegeHelpers::getSiegedSettlement));
	types.siege.set("siegedFort", sol::property(
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

void luaP::onCharacterSelected(generalCharacterictics* gen)
{
	if (onCharacterSelectedFunc != nullptr)
	{
		tryLua((*onCharacterSelectedFunc)(gen));
	}
}

void luaP::onPreFactionTurnStart(factionStruct* fac)
{
	if (onPreFactionTurnStartFunc != nullptr)
	{
		tryLua((*onPreFactionTurnStartFunc)(fac));
	}
}

void luaP::onFactionTurnStart(factionStruct* fac)
{
	if (onFactionTurnStartFunc != nullptr)
	{
		tryLua((*onFactionTurnStartFunc)(fac));
	}
}

void luaP::onGeneralDevastatesTile(generalCharacterictics* gen)
{
	if (onGeneralDevastatesTileFunc != nullptr)
	{
		tryLua((*onGeneralDevastatesTileFunc)(gen));
	}
}

void luaP::onFactionTurnEnd(factionStruct* fac)
{
	if (onFactionTurnEndFunc != nullptr)
	{
		tryLua((*onFactionTurnEndFunc)(fac));
	}
}

void luaP::onFactionNewCapital(factionStruct* fac)
{
	if (onFactionNewCapitalFunc != nullptr)
	{
		tryLua((*onFactionNewCapitalFunc)(fac));
	}
}

void luaP::onFactionWarDeclared(factionStruct* fac, factionStruct* fac2)
{
	if (onFactionWarDeclaredFunc != nullptr)
	{
		tryLua((*onFactionWarDeclaredFunc)(fac, fac2));
	}
}

void luaP::onFactionAllianceDeclared(factionStruct* fac, factionStruct* fac2)
{
	if (onFactionAllianceDeclaredFunc != nullptr)
	{
		tryLua((*onFactionAllianceDeclaredFunc)(fac, fac2));
	}
}

void luaP::onFactionTradeAgreementMade(factionStruct* fac, factionStruct* fac2)
{
	if (onFactionTradeAgreementMadeFunc != nullptr)
	{
		tryLua((*onFactionTradeAgreementMadeFunc)(fac, fac2));
	}
}

void luaP::onFactionBreakAlliance(factionStruct* fac, factionStruct* fac2)
{
	if (onFactionBreakAllianceFunc != nullptr)
	{
		tryLua((*onFactionBreakAllianceFunc)(fac, fac2));
	}
}

void luaP::onGiveMoney(factionStruct* fac, factionStruct* fac2, float amount)
{
	if (onGiveMoneyFunc != nullptr)
	{
		tryLua((*onGiveMoneyFunc)(fac, fac2, amount));
	}
}

void luaP::onUpdateAttitude(factionStruct* fac, factionStruct* fac2)
{
	if (onUpdateAttitudeFunc != nullptr)
	{
		tryLua((*onUpdateAttitudeFunc)(fac, fac2));
	}
}

void luaP::onDemeanour(factionStruct* fac, factionStruct* fac2, float amount)
{
	if (onDemeanourFunc != nullptr)
	{
		tryLua((*onDemeanourFunc)(fac, fac2, amount));
	}
}

void luaP::onGeneralAssaultsGeneral(generalCharacterictics* gen, generalCharacterictics* gen2)
{
	if (onGeneralAssaultsGeneralFunc != nullptr)
	{
		tryLua((*onGeneralAssaultsGeneralFunc)(gen, gen2));
	}
}

void luaP::onGeneralAssaultsResidence(generalCharacterictics* gen, settlementStruct* sett, fortStruct* frt)
{
	if (onGeneralAssaultsResidenceFunc != nullptr)
	{
		tryLua((*onGeneralAssaultsResidenceFunc)(gen, sett, frt));
	}
}

void luaP::onGeneralCaptureSettlement(generalCharacterictics* gen, settlementStruct* sett)
{
	if (onGeneralCaptureSettlementFunc != nullptr)
	{
		tryLua((*onGeneralCaptureSettlementFunc)(gen, sett));
	}
}

void luaP::onGeneralCaptureResidence(generalCharacterictics* gen)
{
	if (onGeneralCaptureResidenceFunc != nullptr)
	{
		tryLua((*onGeneralCaptureResidenceFunc)(gen));
	}
}

void luaP::onSiegeEquipmentCompleted(settlementStruct* sett, fortStruct* frt)
{
	if (onSiegeEquipmentCompletedFunc != nullptr)
	{
		tryLua((*onSiegeEquipmentCompletedFunc)(sett, frt));
	}
}

void luaP::onPostBattle(generalCharacterictics* gen)
{
	if (onPostBattleFunc != nullptr)
	{
		tryLua((*onPostBattleFunc)(gen));
	}
}

void luaP::onMultiTurnMove(generalCharacterictics* gen)
{
	if (onMultiTurnMoveFunc != nullptr)
	{
		tryLua((*onMultiTurnMoveFunc)(gen));
	}
}

void luaP::onSettlementSelected(settlementStruct* sett)
{
	if (onSettlementSelectedFunc != nullptr)
	{
		tryLua((*onSettlementSelectedFunc)(sett));
	}
}

void luaP::onSettlementUpgraded(settlementStruct* sett)
{
	if (onSettlementUpgradedFunc != nullptr)
	{
		tryLua((*onSettlementUpgradedFunc)(sett));
	}
}

void luaP::onSettlementConverted(settlementStruct* sett)
{
	if (onSettlementConvertedFunc != nullptr)
	{
		tryLua((*onSettlementConvertedFunc)(sett));
	}
}
