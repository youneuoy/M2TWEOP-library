///
//![Lua logo](../Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"
#include "gameDataAllHelper.h"
#include "battleHandlerHelpers.h"



void luaP::initCampaign()
{
	struct
	{
		sol::usertype<campaign> campaignTable;
		sol::usertype<stratMap> stratMap;
		sol::usertype<oneTile> tileStruct;
		sol::usertype<regionStruct> region;
		sol::usertype<mercPool> mercPool;
		sol::usertype<mercPoolUnit> mercPoolUnit;
	}typeAll;
	using namespace campaignEnums;

	/// Campaign Enums
	//@section campaignEnums

	/***
	Enum with a list of types of diplomatic relations.

	@tfield int war
	@tfield int peace
	@tfield int alliance
	@tfield int suzerain
	@tfield int trade

	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	local isInWar=campaign:checkDipStance(dipRelType.war,fac1,fac2);

	@table dipRelType
	*/
	luaState.new_enum(
		"dipRelType",
		"war", dipRelEnum::war,
		"peace", dipRelEnum::peace,
		"alliance", dipRelEnum::alliance,
		"suzerain", dipRelEnum::suzerain,
		"trade", dipRelEnum::trade
	);

	/// Campaign
	//@section campaignStruct

	/***
	Basic campaign table.

	@tfield factionStruct[31] factionsSortedByDescrStrat Table of factionStruct[31], indexing starts at 1.
	@tfield int numberOfFactions
	@tfield int numberHumanFactions Number of player-controlled factions.
	@tfield factionStruct currentFaction Faction whose turn it is at the moment, can be set.
	@tfield int passedTurnsNum
	@tfield float timescale Factor for number of turns per year, see descr\_strat.txt
	@tfield settlementStruct romeSettlement
	@tfield settlementStruct constantinopleSettlement
	@tfield float BrigandSpawnValue Lower values increase spawn rate.
	@tfield float PirateSpawnValue Lower values increase spawn rate.
	@tfield int FreeUpkeepForts Number of units who get free upkeep in forts.
	@tfield float currentDate
	@tfield int currentseason season (0=summer, 1=winter)
	@tfield float startDate
	@tfield int startSeason season (0=summer, 1=winter)
	@tfield float endDate
	@tfield int endSeason season (0=summer, 1=winter)
	@tfield int daysInBattle
	@tfield float currentTimeInBattle 24 max, so calculate as daysInBattle*24+currentTimeInBattle.
	@tfield checkDipStance checkDipStance
	@tfield setDipStance setDipStance
	@tfield GetUnitSize GetUnitSize

	@table gameDataAll.campaignStruct
	*/
	typeAll.campaignTable = luaState.new_usertype<campaign>("campaignStruct");
	typeAll.campaignTable.set("factionsSortedByDescrStrat", sol::property([](campaign& self) { return std::ref(self.factionsSortedByDescrStrat); }));
	typeAll.campaignTable.set("numberOfFactions", &campaign::numberOfFactionsWithSlave);
	typeAll.campaignTable.set("numberHumanFactions", &campaign::humanPlayers);
	typeAll.campaignTable.set("currentFaction", &campaign::currentFactionTurn);

	typeAll.campaignTable.set("passedTurnsNum", &campaign::TurnNumber);

	typeAll.campaignTable.set("timescale", &campaign::TimeScale);
	typeAll.campaignTable.set("romeSettlement", &campaign::rome);
	typeAll.campaignTable.set("constantinopleSettlement", &campaign::constantinople);

	typeAll.campaignTable.set("BrigandSpawnValue", &campaign::BrigandSpawnValue);
	typeAll.campaignTable.set("BrigandSpawnValue", &campaign::PirateSpawnValue);
	typeAll.campaignTable.set("FreeUpkeepForts", &campaign::FreeUpkeepForts);

	typeAll.campaignTable.set("currentDate", &campaign::currentDate);
	typeAll.campaignTable.set("currentseason", &campaign::season);

	typeAll.campaignTable.set("startDate", &campaign::startDate);
	typeAll.campaignTable.set("startSeason", &campaign::startSeason);

	typeAll.campaignTable.set("endDate", &campaign::endDate);
	typeAll.campaignTable.set("endSeason", &campaign::endSeason);

	typeAll.campaignTable.set("daysInBattle", &campaign::daysInBattle);
	typeAll.campaignTable.set("currentTimeInBattle", &campaign::currentTimeInBattle);
	/***
	Check if a diplomatic relation between two factions.
	@function campaignStruct:checkDipStance
	@tparam dipRelType checkType Example: dipRelType.war
	@tparam factionStruct fac1 A faction.
	@tparam factionStruct fac2 Another faction.
	@treturn bool checkResult
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	local isInWar=campaign:checkDipStance(dipRelType.war,fac1,fac2);
	*/
	typeAll.campaignTable.set_function("checkDipStance", &m2tweopHelpers::checkDipStance);
	/***
	Set a diplomatic relation between two factions.
	@function campaignStruct:setDipStance
	@tparam dipRelType relType Example: dipRelType.war
	@tparam factionStruct fac1 A faction.
	@tparam factionStruct fac2 Another faction.
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	campaign:setDipStance(dipRelType.war,fac1,fac2);
	*/
	typeAll.campaignTable.set_function("setDipStance", &m2tweopHelpers::setDipStance);
	/***
	Get size of unit(i.e. small or medium, etc). Numbers from 0 to 3
	@function campaignStruct:GetUnitSize
	@treturn int unitSize
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local unitSize=campaign:GetUnitSize();
	*/
	typeAll.campaignTable.set_function("GetUnitSize", &m2tweopHelpers::GetUnitSize);

	/// Strat Map
	//@section stratMap

	/***
	Basic strat map table.

	@tfield int mapWidth
	@tfield int mapHeight
	@tfield int regionsNum
	@tfield getRegion getRegion
	@tfield getTile getTile

	@table gameDataAll.stratMap
	*/
	typeAll.stratMap = luaState.new_usertype<stratMap>("stratMap");
	typeAll.stratMap.set("mapWidth", &stratMap::mapWidth);
	typeAll.stratMap.set("mapHeight", &stratMap::mapHeight);
	typeAll.stratMap.set("regionsNum", &stratMap::regionsNum);

	/***
	Get a specific region by index.
	@function stratMap.getRegion
	@tparam int index
	@treturn region region
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	*/
	typeAll.stratMap.set_function("getRegion", &gameHelpers::getRegion);

	/***
	Get a specific tile by it's coordinates.
	@function stratMap.getTile
	@tparam int x
	@tparam int y
	@treturn tileStruct tile
	@usage
	local sMap = gameDataAll.get().stratMap;
	local tile = sMap.getTile(182, 243);
	*/
	typeAll.stratMap.set_function("getTile", &gameHelpers::getTile);
	///Tile
	//@section tileStruct

	/***
	Basic tile table, not everything very known and understood yet, you can research with these fields.

	@tfield int isLand (1 = land, 0 = sea)
	@tfield int groundType
	@tfield int regionID
	@tfield int factionID (Doesn't work if tile is river).
	@tfield int borderingSettlement Not 0 means bordering settlement.
	@tfield int hasRoad Not 0 means it has a road.
	@tfield int border Not 0 means it is on border of region.

	@table tileStruct
	*/
	typeAll.tileStruct = luaState.new_usertype<oneTile>("tileStruct");
	typeAll.tileStruct.set("isLand", &oneTile::isLand);
	typeAll.tileStruct.set("groundType", &oneTile::groundType);
	typeAll.tileStruct.set("regionID", &oneTile::regionId);
	typeAll.tileStruct.set("factionID", &oneTile::factionId);
	typeAll.tileStruct.set("borderingSettlement", &oneTile::borderingSettlement);
	typeAll.tileStruct.set("hasRoad", &oneTile::hasRoad);
	typeAll.tileStruct.set("border", &oneTile::border);

	///Region
	//@section Region

	/***
	Basic region table.

	@tfield string regionName
	@tfield string localizedName
	@tfield string settlementName
	@tfield string legioName
	@tfield int regionID
	@tfield int stacksNum
	@tfield int fortsNum
	@tfield int watchtowersNum
	@tfield int isSea
	@tfield int hasLake (fully enclosed by region)
	@tfield mercPool mercPool
	@tfield settlementStruct settlement
	@tfield int tileCount
	@tfield int neighbourRegionsNum
	@tfield int resourcesNum
	@tfield int hiddenResources1 (bitmap with 32 first hidden resources), needs to be converted to binary and then use bitwise operators from lua.
	@tfield int hiddenResources2 (bitmap last 32 first hidden resources), needs to be converted to binary and then use bitwise operators from lua.
	@tfield int settlementXCoord
	@tfield int settlementYCoord
	@tfield int portEntranceXCoord
	@tfield int portEntranceYCoord
	@tfield factionStruct faction
	@tfield string rebelType
	@tfield string localizedRebelsName
	@tfield int triumphValue
	@tfield getStack getStack
	@tfield getFort getFort
	@tfield getWatchtower getWatchtower
	@tfield getResource getResource
	@tfield getNeighbour getNeighbour
	@tfield getHiddenResource getHiddenResource
	@tfield setHiddenResource setHiddenResource

	@table region
	*/
	typeAll.region = luaState.new_usertype<regionStruct>("region");
	typeAll.region.set("regionName", &regionStruct::regionName);
	typeAll.region.set("settlementName", &regionStruct::settlementName);
	typeAll.region.set("localizedName", sol::property(
		&gameHelpers::getRegionName, &gameHelpers::changeRegionName
		));
	typeAll.region.set("legioName", &regionStruct::legioName);
	typeAll.region.set("regionID", &regionStruct::regionID);
	typeAll.region.set("stacksNum", &regionStruct::stacksNum);
	typeAll.region.set("fortsNum", &regionStruct::fortsNum);
	typeAll.region.set("watchtowersNum", &regionStruct::watchtowersNum);
	typeAll.region.set("isSea", &regionStruct::isSea);
	typeAll.region.set("hasLake", &regionStruct::hasLake);
	typeAll.region.set("mercPool", &regionStruct::mercPool);
	typeAll.region.set("settlement", &regionStruct::settlement);
	typeAll.region.set("tileCount", &regionStruct::tileCount);
	typeAll.region.set("neighbourRegionsNum", &regionStruct::neighbourRegionsNum);
	typeAll.region.set("resourcesNum", &regionStruct::resourcesNum);
	typeAll.region.set("hiddenResources1", &regionStruct::hiddenResources1);
	typeAll.region.set("hiddenResources2", &regionStruct::hiddenResources2);
	typeAll.region.set("settlementXCoord", &regionStruct::settlementXCoord);
	typeAll.region.set("settlementYCoord", &regionStruct::settlementYCoord);
	typeAll.region.set("portEntranceXCoord", &regionStruct::portEntranceXCoord);
	typeAll.region.set("portEntranceYCoord", &regionStruct::portEntranceYCoord);
	typeAll.region.set("faction", &regionStruct::factionOwner);
	typeAll.region.set("rebelType", &regionStruct::rebelType);
	typeAll.region.set("localizedRebelsName", sol::property(
		&gameHelpers::getRebelsName, &gameHelpers::changeRebelsName
		));
	typeAll.region.set("triumphValue", &regionStruct::triumphValue);

	/***
	Get an army by it's index.
	@function region:getStack
	@tparam int index
	@treturn stackStruct army
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local army = region:getStack(0)
	*/
	typeAll.region.set_function("getStack", &gameHelpers::getStack);

	/***
	Get a fort by it's index.
	@function region:getFort
	@tparam int index
	@treturn fortStruct fort
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local fort = region:getFort(0)
	*/
	typeAll.region.set_function("getFort", &gameHelpers::getFort);

	/***
	Get a watchtower by it's index.
	@function region:getWatchtower
	@tparam int index
	@treturn watchtowerStruct watchtower
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local watch = region:getWatchtower(0)
	*/
	typeAll.region.set_function("getWatchtower", &gameHelpers::getWatchtower);

	/***
	Get a resource by it's index.
	@function region:getResource
	@tparam int index
	@treturn tradeResource resource
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local res = region:getResource(0)
	*/
	typeAll.region.set_function("getResource", &gameHelpers::getResource);

	/***
	Get a neighbour region by it's index.
	@function region:getNeighbour
	@tparam int index
	@treturn region nRegion
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	region:getNeighbour(0)
	*/
	typeAll.region.set_function("getNeighbour", &gameHelpers::getNeighbour);

	/***
	Check if a region has a hidden resource.
	@function region:getHiddenResource
	@tparam int index
	@treturn bool hr
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	region:getHiddenResource(0)
	*/
	typeAll.region.set_function("getHiddenResource", &gameHelpers::getHiddenResource);

	/***
	Set a region's hidden resource (reset on game restart).
	@function region:setHiddenResource
	@tparam int index
	@tparam bool enable
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	region:setHiddenResource(0, false)
	*/
	typeAll.region.set_function("setHiddenResource", &gameHelpers::setHiddenResource);

	///Merc Pools
	//@section mercPool

	/***
	Basic mercenary pool table.

	@tfield string name
	@tfield getMercUnitNum getMercUnitNum
	@tfield addMercUnit addMercUnit
	@tfield getMercUnit getMercUnit

	@table mercPool
	*/
	typeAll.mercPool = luaState.new_usertype<mercPool>("mercPool");
	typeAll.mercPool.set("name", &mercPool::name);

	/***
	Get amount of mercenary units a region has.
	@function mercPool:getMercUnitNum
	@treturn int mercUnitNum
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercNum = mercpool:getMercUnitNum();
	*/
	typeAll.mercPool.set_function("getMercUnitNum", &gameHelpers::getMercUnitNum);

	/***
	Add a new mercenary unit to a pool.
	@function mercPool:addMercUnit
	@tparam int idx EDU index, supports EOP units.
	@tparam int exp Starting experience.
	@tparam int cost
	@tparam float repmin Minimum replenishment rate.
	@tparam float repmax Maximum replenishment rate.
	@tparam int maxunits Maximum Pool.
	@tparam float startpool Starting pool.
	@tparam int startyear (0 to disable) Use 0 if the startyear is before the year you introduce the merc, not an earlier startyear!
	@tparam int endyear (0 to disable)
	@tparam int crusading
	@treturn mercPoolUnit mercunit
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercUnit = mercpool:addMercUnit(1907, 0, 570, 0.15, 0.35, 3, 3, 0, 0, 0);
	*/
	typeAll.mercPool.set_function("addMercUnit", &gameHelpers::addMercUnit);

	/***
	Get a mercenary unit from a pool by index.
	@function mercPool:getMercUnit
	@tparam int idx
	@treturn mercPoolUnit mercUnit
	@usage
	local sMap = gameDataAll.get().stratMap;
	local region = sMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercNum = mercPool:getMercUnitNum();
	for i = 0, mercNum-1 do
		local mercUnit = mercpool:getMercUnit(i);
	end
	*/
	typeAll.mercPool.set_function("getMercUnit", &gameHelpers::getMercUnit);


	///Mercenary units
	//@section mercPoolUnit

	/***
	Basic mercenary unit table.

	@tfield EduEntry eduEntry
	@tfield int experience
	@tfield int cost
	@tfield float replenishMin
	@tfield float replenishMax
	@tfield int maxUnits
	@tfield float currentPool
	@tfield int startYear
	@tfield int endYear
	@tfield int crusading
	@tfield int poolIndex
	@tfield int mercPoolUnitIndex
	@tfield mercPool mercPool
	@tfield setMercReligion setMercReligion

	@table mercPoolUnit
	*/
	typeAll.mercPoolUnit = luaState.new_usertype<mercPoolUnit>("mercPoolUnit");
	typeAll.mercPoolUnit.set("eduEntry", &mercPoolUnit::eduEntry);
	typeAll.mercPoolUnit.set("experience", &mercPoolUnit::experience);
	typeAll.mercPoolUnit.set("cost", &mercPoolUnit::cost);
	typeAll.mercPoolUnit.set("replenishMin", &mercPoolUnit::replenishMin);
	typeAll.mercPoolUnit.set("replenishMax", &mercPoolUnit::replenishMax);
	typeAll.mercPoolUnit.set("maxUnits", &mercPoolUnit::maxUnits);
	typeAll.mercPoolUnit.set("currentPool", &mercPoolUnit::currentPool);
	typeAll.mercPoolUnit.set("startYear", &mercPoolUnit::startYear);
	typeAll.mercPoolUnit.set("endYear", &mercPoolUnit::endYear);
	typeAll.mercPoolUnit.set("crusading", &mercPoolUnit::crusading);
	typeAll.mercPoolUnit.set("poolIndex", &mercPoolUnit::poolIndex);
	typeAll.mercPoolUnit.set("mercPoolUnitIndex", &mercPoolUnit::mercPoolUnitIndex);
	typeAll.mercPoolUnit.set("mercPool", &mercPoolUnit::mercPool);

	/***
	Set or remove a religion requirement for a mercenary unit.
	@function mercPoolUnit:setMercReligion
	@tparam int religion
	@tparam bool set True means enable this religion requirement, False means disable.
	@usage
	local stratmap = gameDataAll.get().stratMap;
	local region = stratMap.getRegion(2);
	local mercpool = region.mercPool;
	local mercNum = mercPool:getMercUnitNum();
	for i = 0, mercNum-1 do
		local mercUnit = mercPool:getMercUnit(i);
		mercUnit:setMercReligion(3, true)
	end
	*/
	typeAll.mercPoolUnit.set_function("setMercReligion", &gameHelpers::setMercReligion);


}
void luaP::initP2()
{
	struct
	{
		//global game table
		sol::usertype<gameDataAllStruct> gameDataAllTable;
		//this inside gameDataAll table
		sol::usertype<battleDataS> battleTable;
		sol::usertype<battleSide> battleSideTable;
		sol::usertype<trackedPointerArmy> trackedPointerArmyTable;
		sol::usertype<deploymentAreaS> deploymentAreaTable;
		sol::usertype<battleAI> battleAI;
		sol::usertype<armyAndCharacter> battleArmy;
		sol::usertype<battleUnit> battleUnit;

	}typeAll;
	///gameDataAll
	//@section gameDataAll

	/***
	Basic gameDataAll table

	@tfield get get
	@tfield battleStruct battleStruct battle data
	@tfield campaignStruct campaignStruct campaign data

	@table gameDataAll
	*/
	typeAll.gameDataAllTable = luaState.new_usertype<gameDataAllStruct>("gameDataAll");
	/***
	Call at the start of the script, this is a static object and the pointer to it doesn't change.
	@function gameDataAll.get
	@treturn gameDataAllStruct gameDataAll
	@usage
	gameData=gameDataAll.get();
	battleXCoord=gameData.battleStruct.xCoord;
	*/
	typeAll.gameDataAllTable.set_function("get", &gameDataAllHelper::get);
	typeAll.gameDataAllTable.set("battleStruct", &gameDataAllStruct::battleHandler);
	typeAll.gameDataAllTable.set("campaignStruct", &gameDataAllStruct::campaignData);
	typeAll.gameDataAllTable.set("stratMap", &gameDataAllStruct::stratMap);



	///battleStruct
	//@section battleStruct

	/***
	basic battleStruct table

	@tfield int battleState 0-not in battle,5-active battle,9-results screen,etc
	@tfield int battleType 3 = siege, 4 = sally out, rest unknown for now, you can experiment.
	@tfield int isNightBattle
	@tfield int xCoord
	@tfield int yCoord
	@tfield int attackerXCoord
	@tfield int attackerYCoord
	@tfield int defenderXCoord
	@tfield int defenderYCoord
	@tfield int paused
	@tfield float battleSpeed
	@tfield float secondsPassed
	@tfield int sidesNum Returns a battleSide[8]. Maximum: 8.
	@tfield battleSide[8] sides
	@tfield factionSide[31] faction alliance array, -1 if not in battle

	@table gameDataAll.battleStruct
	*/
	typeAll.battleTable = luaState.new_usertype<battleDataS>("battleStruct");
	typeAll.battleTable.set("battleState", &battleDataS::battleState);
	typeAll.battleTable.set("battleType", &battleDataS::battleType);
	typeAll.battleTable.set("isNightBattle", &battleDataS::isNightBattle);
	typeAll.battleTable.set("xCoord", &battleDataS::xCoord);
	typeAll.battleTable.set("yCoord", &battleDataS::yCoord);
	typeAll.battleTable.set("attackerXCoord", &battleDataS::attackerXCoord);
	typeAll.battleTable.set("attackerYCoord", &battleDataS::attackerYCoord);
	typeAll.battleTable.set("defenderXCoord", &battleDataS::defenderXCoord);
	typeAll.battleTable.set("defenderYCoord", &battleDataS::defenderYCoord);
	typeAll.battleTable.set("paused", &battleDataS::paused);
	typeAll.battleTable.set("battleSpeed", &battleDataS::speed);
	typeAll.battleTable.set("secondsPassed", &battleDataS::secondsPassed);
	typeAll.battleTable.set("sidesNum", &battleDataS::sidesNum);
	typeAll.battleTable.set("sides", sol::property([](battleDataS& self) { return std::ref(self.sides); }));
	typeAll.battleTable.set("factionSide", sol::property([](battleDataS& self) { return std::ref(self.factionSide); }));
	///battleSide
	//@section battleSide

	/***
	Basic battleSide table

	@tfield bool isDefender
	@tfield bool isCanDeploy
	@tfield int wonBattle 0 = lose, 1 = draw, 2 = win
	@tfield int battleSuccess 0 = close, 1 = average, 2 = clear, 3 = crushing
	@tfield int[4] winConditions Returns an int index of a wincondition.
	@tfield getWinConditionString getWinConditionString
	@tfield int armiesNum
	@tfield int alliance
	@tfield int soldierCount
	@tfield int totalStrenght
	@tfield battleAI battleAIPlan
	@tfield getBattleArmy getBattleArmy
	@tfield trackedPointerArmy[8] Returns a table of trackedPointerArmy. Maximum: 8.



	@table battleStruct.battleSide
	*/
	typeAll.battleSideTable = luaState.new_usertype<battleSide>("battleSide");
	typeAll.battleSideTable.set("isDefender", &battleSide::isDefender);
	typeAll.battleSideTable.set("isCanDeploy", &battleSide::isCanDeploy);
	typeAll.battleSideTable.set("wonBattle", &battleSide::wonBattle);
	typeAll.battleSideTable.set("battleSuccess", &battleSide::battleSuccess);
	typeAll.battleSideTable.set("alliance", &battleSide::alliance);
	typeAll.battleSideTable.set("soldierCount", &battleSide::soldierCount);
	typeAll.battleSideTable.set("totalStrenght", &battleSide::totalStrenght);
	typeAll.battleSideTable.set("battleAIPlan", &battleSide::battleAIPlan);
	typeAll.battleSideTable.set("winConditions", sol::property([](battleSide& self) { return std::ref(self.winConditions); }));
	/***
	Get win condition string, for example: destroy\_or\_rout_enemy
	@function battleSide.getWinConditionString
	@tparam int condition
	@treturn string winCondition destroy\_or\_rout\_enemy, balance\_of\_strength\_percent, destroy\_enemy\_strength\_percent, capture\_location, destroy\_character, capture\_major\_settlement, capture\_army\_settlement, unknown\_condition
	@usage
	function onPostBattle(namedChar, faction, region, characterType, religion)
		getBattleData()
	end

	function getBattleData()
		local thisBattle, battleList = gameData.battleStruct, "Function: getBattleData()"
		for i = 1, thisBattle.sidesNum, 1 do
			local thisSide = thisBattle.sides[i]
			battleList = battleList.."\n\tSide "..i.."\n\t\tisDefender: "..tostring(thisSide.isDefender).."\n\t\tisCanDeploy: "..tostring(thisSide.isCanDeploy).."\n\t\tarmiesNum: "..thisSide.armiesNum.."\n\t\twinConditions:"
			for j = 1, 4, 1 do
				local thisWinCond = thisSide.winConditions[j]
				if thisWinCond ~= 0 then
					battleList = battleList.."\n\t\t\t"..thisSide.getWinConditionString(thisWinCond)
				end
			end
			if thisSide.armies[1] ~= nil then
				local k = 1
				repeat
					local thisArmy = thisSide.armies[k].army
					if thisArmy ~= nil then
						battleList = battleList.."\n\t\tArmy "..k.."\n\t\t\tFaction: "..thisArmy.faction:getFactionName()
						if thisArmy.leader ~= nil then
							battleList = battleList.."\n\t\t\tLeader: "..thisArmy.leader.namedCharacter.fullName
						end
					end
					k = k + 1
				until thisSide.armies[k] == nil
			end
		end
		print(battleList)
	end
	*/
	typeAll.battleSideTable.set_function("getWinConditionString", &battleHandlerHelpers::getWinConditionS);
	typeAll.battleSideTable.set("armiesNum", &battleSide::armiesNum);
	typeAll.battleSideTable.set("armies", sol::property([](battleSide& self) { return std::ref(self.armies); }));
	/***
	Get a battle army by it's index.
	@function getBattleArmy
	@tparam battleSideTable side
	@tparam int index
	@treturn battleArmy army
	@usage

		unit = side:getBattleArmy(0)

	*/
	typeAll.battleSideTable.set_function("getBattleArmy", &battleHandlerHelpers::getBattleArmy);


	///trackedPointerArmy
	//@section trackedPointerArmy

	/***
	Basic trackedPointerArmy table

	@tfield stackStruct army
	@tfield deploymentAreaS deploymentArea



	@table battleSide.trackedPointerArmy
	*/
	typeAll.trackedPointerArmyTable = luaState.new_usertype<trackedPointerArmy>("trackedPointerArmy");
	typeAll.trackedPointerArmyTable.set("army", &trackedPointerArmy::stack);
	typeAll.trackedPointerArmyTable.set("deploymentArea", &trackedPointerArmy::deploymentArea);

	///deploymentAreaS
	//@section deploymentAreaS

	/***
	Basic trackedPointerArmy table

	@tfield int coordsNum
	@tfield getCoordPair getCoordPair



	@table trackedPointerArmy.deploymentAreaS
	*/
	typeAll.deploymentAreaTable = luaState.new_usertype<deploymentAreaS>("deploymentAreaS");
	typeAll.deploymentAreaTable.set("coordsNum", &deploymentAreaS::coordsNum);
	/***
	Get pair of coords with index.
	@function deploymentAreaS:getCoordPair
	@treturn float xCoord X coordinate of the area.
	@treturn float yCoord Y coordinate of the area.
	@usage
	gameData=gameDataAll.get();
	battleS=gameData.battleStruct;
	side1=gameData.battleStruct.sides[1];
	deployArea1=side1.armies[1].deploymentArea;

	for i = 0, deployArea1.coordsNum-1 do
		xCoord,yCoord=side1.armies[1].deploymentArea:getCoordPair(i);
		print(xCoord,yCoord);
	end
	*/
	typeAll.deploymentAreaTable.set("getCoordPair", [](deploymentAreaS& self, int pairNum) { return std::make_tuple(self.coordsPairs[0 + pairNum], self.coordsPairs[1 + pairNum]); });

	///battleAI
	//@section battleAI

	/***
	Basic Battle AI table

	@tfield int gtaPlan
	@tfield int unitCount
	@tfield int enemyUnitCount

	@table battleAI
	*/
	typeAll.battleAI = luaState.new_usertype<battleAI>("battleAI");
	typeAll.battleAI.set("gtaPlan", &battleAI::currentAIPlan);
	typeAll.battleAI.set("unitCount", &battleAI::unitCount);
	typeAll.battleAI.set("enemyUnitCount", &battleAI::enemyUnitCount);
	///battleArmy
	//@section battleArmy

	/***
	Basic battleArmy table

	@tfield stackStruct army
	@tfield character character
	@tfield int generalNumKillsBattle
	@tfield float generalHPRatioLost
	@tfield float battleOdds
	@tfield int numKilledGenerals
	@tfield int unitCount
	@tfield getBattleUnit getBattleUnit

	@table battleArmy
	*/
	typeAll.battleArmy = luaState.new_usertype<armyAndCharacter>("battleArmy");
	typeAll.battleArmy.set("army", &armyAndCharacter::army);
	typeAll.battleArmy.set("character", &armyAndCharacter::character);
	typeAll.battleArmy.set("generalNumKillsBattle", &armyAndCharacter::generalNumKillsBattle);
	typeAll.battleArmy.set("generalHPRatioLost", &armyAndCharacter::generalHPRatioLost);
	typeAll.battleArmy.set("battleOdds", &armyAndCharacter::battleOdds);
	typeAll.battleArmy.set("numKilledGenerals", &armyAndCharacter::numKilledGenerals);
	typeAll.battleArmy.set("unitCount", &armyAndCharacter::unitCount);
	/***
	Get a battle unit by it's index.
	@function getBattleUnit
	@tparam battleArmy army
	@tparam int index
	@treturn battleUnit unit
	@usage

		unit = battleArmy:getBattleUnit(0)

	*/
	typeAll.battleArmy.set_function("getBattleUnit", &battleHandlerHelpers::getBattleUnit);
	///battleUnit
	//@section battleUnit

	/***
	Basic battleUnit table

	@tfield unit unit
	@tfield int soldiersLost
	@tfield int soldiersStart
	@tfield int unitsRouted
	@tfield boolean hasRouted

	@table battleUnit
	*/
	typeAll.battleUnit = luaState.new_usertype<battleUnit>("battleUnit");
	typeAll.battleUnit.set("unit", &battleUnit::unit);
	typeAll.battleUnit.set("soldiersLost", &battleUnit::soldiersLost);
	typeAll.battleUnit.set("soldiersStart", &battleUnit::soldiersStart);
	typeAll.battleUnit.set("unitsRouted", &battleUnit::unitsRouted);
	typeAll.battleUnit.set("hasRouted", &battleUnit::hasRouted);
}