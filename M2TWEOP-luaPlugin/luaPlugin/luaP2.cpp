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

	@tfield factionStruct[31] factionsSortedByDescrStrat indexing starts at 1
	@tfield int numberOfFactions
	@tfield int numberHumanFactions number of player-controlled factions
	@tfield factionStruct currentFaction faction whose turn is at the moment, can be set
	@tfield int passedTurnsNum
	@tfield float timescale factor for number turns per year, see descr_strat.txt
	@tfield settlementStruct romeSettlement
	@tfield settlementStruct constantinopleSettlement
	@tfield float BrigandSpawnValue lower values increase spawn rate
	@tfield float PirateSpawnValue lower values increase spawn rate
	@tfield int FreeUpkeepForts number units with free_upkeep_unit EDU attribute who can get free upkeep in forts
	@tfield float currentDate
	@tfield int currentseason season (0=summer, 1=winter)
	@tfield float startDate
	@tfield int startSeason season (0=summer, 1=winter)
	@tfield float endDate
	@tfield int endSeason season (0=summer, 1=winter)
	@tfield int daysInBattle
	@tfield float currentTimeInBattle 24 max, so calc as daysInBattle*24+currentTimeInBattle
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
	Check diplomatic relations between factions
	@function campaignStruct:checkDipStance
	@tparam dipRelType checkType
	@tparam factionStruct fac1
	@tparam factionStruct fac2
	@treturn bool checkResult
	@usage
	local campaign=gameDataAll.get().campaignStruct;
	local fac1=campaign.factionsSortedByDescrStrat[1];
	local fac2=campaign.factionsSortedByDescrStrat[2];
	local isInWar=campaign:checkDipStance(dipRelType.war,fac1,fac2);
	*/
	typeAll.campaignTable.set_function("checkDipStance", &m2tweopHelpers::checkDipStance);
	/***
	Set diplomatic relations between factions
	@function campaignStruct:setDipStance
	@tparam dipRelType relType
	@tparam factionStruct fac1
	@tparam factionStruct fac2
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
	Call at start of script, this is a static object and pointer to it doesn't change
	@function gameDataAll.get
	@treturn gameDataAllStruct gameDataAll
	@usage
	gameData=gameDataAll.get();
	battleXCoord=gameData.battleStruct.xCoord;
	*/
	typeAll.gameDataAllTable.set_function("get", &gameDataAllHelper::get);
	typeAll.gameDataAllTable.set("battleStruct", &gameDataAllStruct::battleHandler);
	typeAll.gameDataAllTable.set("campaignStruct", &gameDataAllStruct::campaignData);



	///battleStruct
	//@section battleStruct

	/***
	basic battleStruct table

	@tfield int xCoord
	@tfield int yCoord
	@tfield int attackerXCoord
	@tfield int attackerYCoord
	@tfield int defenderXCoord
	@tfield int defenderYCoord
	@tfield int sidesNum
	@tfield battleSide[8] sides

	@table gameDataAll.battleStruct
	*/
	typeAll.battleTable = luaState.new_usertype<battleDataS>("battleStruct");
	typeAll.battleTable.set("xCoord", &battleDataS::xCoord);
	typeAll.battleTable.set("yCoord", &battleDataS::yCoord);
	typeAll.battleTable.set("attackerXCoord", &battleDataS::attackerXCoord);
	typeAll.battleTable.set("attackerYCoord", &battleDataS::attackerYCoord);
	typeAll.battleTable.set("defenderXCoord", &battleDataS::defenderXCoord);
	typeAll.battleTable.set("defenderYCoord", &battleDataS::defenderYCoord);
	typeAll.battleTable.set("sidesNum", &battleDataS::sidesNum);
	typeAll.battleTable.set("sides", sol::property([](battleDataS& self) { return std::ref(self.sides); }));


	///battleSide
	//@section battleSide

	/***
	basic battleSide table

	@tfield bool isDefender
	@tfield bool isCanDeploy
	@tfield int[4] winConditions
	@tfield getWinConditionString getWinConditionString
	@tfield int armiesNum
	@tfield trackedPointerArmy[8] armies



	@table battleStruct.battleSide
	*/
	typeAll.battleSideTable = luaState.new_usertype<battleSide>("battleSide");
	typeAll.battleSideTable.set("isDefender", &battleSide::isDefender);
	typeAll.battleSideTable.set("isCanDeploy", &battleSide::isCanDeploy);
	typeAll.battleSideTable.set("winConditions", sol::property([](battleSide& self) { return std::ref(self.winConditions); }));
	/***
	Get win condition string, for example: destroy\_or\_rout_enemy
	@function battleSide.getWinConditionString
	@tparam int condition
	@treturn string winCondition destroy\_or\_rout\_enemy, balance\_of\_strength\_percent, destroy\_enemy\_strength\_percent, capture\_location, destroy\_character, capture\_major\_settlement, capture\_army\_settlement, unknown\_condition
	@usage
	function onPostBattle(character)
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


	///trackedPointerArmy
	//@section trackedPointerArmy

	/***
	basic trackedPointerArmy table

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
	basic trackedPointerArmy table

	@tfield int coordsNum
	@tfield getCoordPair getCoordPair



	@table trackedPointerArmy.deploymentAreaS
	*/
	typeAll.deploymentAreaTable = luaState.new_usertype<deploymentAreaS>("deploymentAreaS");
	typeAll.deploymentAreaTable.set("coordsNum", &deploymentAreaS::coordsNum);
	/***
	Get pair of coords with number
	@function deploymentAreaS:getCoordPair
	@treturn float xCoord x coordinate of area
	@treturn float yCoord y coordinate of area
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
}