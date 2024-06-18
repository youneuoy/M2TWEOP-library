#include "campaign.h"

#include "eopdu.h"
#include "gameDataAllHelper.h"
#include "gameHelpers.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "stratModelsChange.h"
#include "techFuncs.h"
#include "technicalHelpers.h"
#include "army.h"
#include "fort.h"
#include "settlement.h"
#include "character.h"
#include "strategyMap.h"
#include "unit.h"


void resourceStruct::setModel(UINT32 modelId)
{
	stratModelsChange::setModel(xCoord, yCoord, modelId, modelId);
}

void campaign::setDipStance(campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2)
{
	using namespace campaignEnums;
	if (!fac1 || !fac2 || !fac1->factionRecord || !fac2->factionRecord)
		return;
	if (dipType == suzerain)
		return setFactionProtectorate(fac1, fac2);
	if (dipType == trade)
		return setFactionTrade(fac1, fac2);
	const auto facOneName = std::string(fac1->factionRecord->facName);
	const auto facTwoName = std::string(fac2->factionRecord->facName);
	const std::string command = "diplomatic_stance " + facOneName + " " + facTwoName + " ";
	if (dipType == war)
		smallFuncs::scriptCommand("console_command", (command + "war").c_str());
	else if (dipType == peace)
		smallFuncs::scriptCommand("console_command", (command + "neutral").c_str());
	else if (dipType == alliance)
		smallFuncs::scriptCommand("console_command", (command + "allied").c_str());
}

	
bool campaign::checkDipStance(const campaignEnums::dipRelEnum dipType, const factionStruct* fac1, const factionStruct* fac2)
{
	using namespace campaignEnums;
	if (!fac1 || !fac2)
		return false;
	const auto facDiplomacy = diplomaticStandings[fac1->factionID][fac2->factionID];
	if (dipType == trade)
		return facDiplomacy.hasTradeRights;
	const auto state = facDiplomacy.state;
	if (dipType == war)
		return state == warState;
	if (dipType == peace)
		return state == peaceState;
	if (dipType == alliance)
		return state == allianceState;
	if (dipType == suzerain)
		return facDiplomacy.isProtectorate;
	return false;
}

void campaign::setFactionTrade(factionStruct* factionOne, factionStruct* factionTwo)
{
	DWORD funcAddr = codes::offsets.setFactionTrade;
	auto diplomaticStuff = &diplomaticStandings;
	int facIdOne = factionOne->factionID;
	int facIdTwo = factionTwo->factionID;
	auto set = diplomaticStandings[facIdOne][facIdTwo].hasTradeRights == 0;
	_asm
	{
		push set
		push facIdTwo
		push facIdOne
		mov ecx, diplomaticStuff
		mov eax, funcAddr
		call eax
	}
}

std::string campaign::getCampaignPath()
{
	return technicalHelpers::uniStringToStr(currentDescrFile);
}

settlementStruct* campaign::getSettlementByName(const char* name)
{
	const auto stratMap = stratMapHelpers::getStratMap();
	if (const auto sett = stratMapHelpers::getSettlement(stratMap, name); sett)
		return sett;
	const int settlementCount = settlements.size();
	for (int i = 0; i < settlementCount; i++)
	{
		if (const auto sett = *settlements.get(i); strcmp(sett->name, name) == 0)
			return sett;
	}
	return nullptr;
}

factionStruct* campaign::getFactionHashed(const std::string& name)
{
	if (!plugData::data.luaAll.hashLoaded)
		plugData::data.luaAll.fillHashMaps();
	const auto factionId = plugData::data.luaAll.factions.find(name);
	if (factionId == plugData::data.luaAll.factions.end()) 
		return nullptr;
	return factionsSortedByID[factionId->second];
}

void campaign::setFactionProtectorate(factionStruct* factionOne, factionStruct* factionTwo)
{
	DWORD funcAddr = codes::offsets.setProtectorate;
	auto diplomaticStuff = &diplomaticStandings;
	int facIdOne = factionOne->factionID;
	int facIdTwo = factionTwo->factionID;
	_asm
	{
		push facIdTwo
		push facIdOne
		mov ecx, diplomaticStuff
		mov eax, funcAddr
		call eax
	}
}

mercPoolUnit* mercPool::addMercUnit(const int idx, const int exp, const int cost, const float repMin, const float repMax,
	const int maxUnits, const float startPool, const float startYear, const float endYear, const int crusading)
{
	int16_t mercPoolUnitIndex = mercenaryUnits.currentCount;
	int16_t poolIndex = 0;
	if (const auto unit = &mercenaryUnits.elements[0]; !unit)
	{
		const auto campaign =  campaignHelpers::getCampaignData();
		const auto num = campaign->mercPools.size();
		for (int i = 0; i < num; i++)
		{
			if (strcmp(campaign->mercPools.get(i)->name , name) == 0)
				break;
			poolIndex++;
		}
	}
	else
		poolIndex = unit->poolIndex;
	if (const auto nextUnitsPtr = mercenaryUnits.next; nextUnitsPtr && nextUnitsPtr->currentCount )
		mercPoolUnitIndex = mercenaryUnits.currentCount + campaignHelpers::getPoolIndex(nextUnitsPtr);
	const auto newMerc = campaignHelpers::getNewMercUnit(&mercenaryUnits);
	newMerc->mercPoolUnitIndex = mercPoolUnitIndex;
	newMerc->poolIndex = poolIndex;
	newMerc->mercPool = this;
	eduEntry* entry = eopDu::getEduEntry(idx);
	newMerc->eduEntry = entry;
	newMerc->experience = exp;
	newMerc->cost = cost;
	newMerc->replenishMin = repMin;
	newMerc->replenishMax = repMax;
	newMerc->maxUnits = maxUnits;
	newMerc->currentPool = startPool;
	newMerc->startYear = startYear;
	newMerc->endYear = endYear;
	newMerc->crusading = crusading;
	return newMerc;
}

#define resource_dataStruct_type 1
namespace campaignHelpers
{
	//stratResMod
	template <char fieldIndex>
	std::string getStringPropertyBD(const stratResMod* stratMod)
	{
		char* retS = nullptr;
		if (fieldIndex == resource_dataStruct_type)
		{
			retS = stratMod->tga;
		}
		if (retS != nullptr)
		{
			return std::string(retS);
		}
		else
		{
			return std::string("");
		}
	}
	template <char fieldIndex>
	void setStringPropertyBD(stratResMod* stratMod, std::string newS)
	{
		if (fieldIndex == resource_dataStruct_type)
			fastFunctsHelpers::setCryptedString(&stratMod->tga, newS.c_str());
	}
	
	int getPoolIndex(const gameList<mercPoolUnit> *unitPtr)
	{
		if (const auto nextUnitsPtr = unitPtr->next; nextUnitsPtr && nextUnitsPtr->currentCount )
			return unitPtr->currentCount + getPoolIndex(nextUnitsPtr);
		return unitPtr->currentCount;
	}

	mercPoolUnit* getNewMercUnit(gameList<mercPoolUnit>* unitPtr)
	{
		const auto currentPool = unitPtr->currentCount;
		if (currentPool >= unitPtr->currentSize)
		{
			if (!unitPtr->next)
			{
				const auto newPtr = reinterpret_cast<gameList<mercPoolUnit>*>(techFuncs::allocateGameMem(0x14));
				newPtr->currentSize = unitPtr->currentSize * 2;
				newPtr->currentCount = 0;
				newPtr->elements = reinterpret_cast<mercPoolUnit*>(techFuncs::allocateGameMem(0x50 * newPtr->currentSize));
				unitPtr->next = newPtr;
				newPtr->prev = unitPtr;
			}
			return getNewMercUnit(unitPtr->next);
		}
		const auto unit = &unitPtr->elements[currentPool];
		unitPtr->currentCount++;
		unit->eduEntry = nullptr;
		unit->experience = 0;
		unit->replenishMin = 0;
		unit->replenishMax = 0;
		unit->maxUnits = 0;
		unit->currentPool = 0;
		unit->startYear = 0;
		unit->endYear = 0;
		unit->crusading = 0;
		unit->religions = gameStdVector<int>{};
		unit->events = gameStdVector<stringWithHash>{};
		unit->poolIndex = 0;
		unit->mercPoolUnitIndex = 0;
		unit->mercPool = nullptr;
		return unit;
	}

	UINT32 getFactionsCount()
	{
		const campaign* campaign = getCampaignData();
		if (!campaign)
			return 0;
		return campaign->factionCount;
	}

	factionStruct* getFaction(const int index)
	{
		const campaign* campaign =  getCampaignData();
		if (!campaign || index < 0 || index >= campaign->factionCount)
			return nullptr;
		return campaign->factionsSortedByTurnOrder[index];
	}
	
    campaign* getCampaignData()
    {
        const auto gameData = gameDataAllHelper::get();
        if (!gameData)
            return nullptr;
        return gameData->campaignStruct;
    }

    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::usertype<campaign> campaignTable;
            sol::usertype<collegeOfCardinals> collegeOfCardinals;
            sol::usertype<cardinal> cardinal;
            sol::usertype<crusade> crusadeStruct;
            sol::usertype<crusadeReward> crusadeReward;
            sol::usertype<jihad> jihadStruct;
		    sol::usertype<watchTowerStruct>watchtowerStruct;
		    sol::usertype<portBuildingStruct>portStruct;
		    sol::usertype<portDockStrat>dockStruct;
            sol::usertype<resourceStruct>tradeResource;
            sol::usertype<mercPool> mercPool;
            sol::usertype<mercPoolUnit> mercPoolUnit;
            sol::usertype<factionDiplomacy> factionDiplomacy;
        }typeAll;
    	
		///Campaign
		//@section campaignStruct

		/***
		Basic campaign table.

		@tfield int[31] campaignDifficultyFaction Indexing starts at 1, so add 1 to faction ID. Maximum 31.
		@tfield int[31] battleDifficultyFaction Indexing starts at 1, so add 1 to faction ID. Maximum 31.
		@tfield factionStruct[31] factionsSortedByDescrStrat Table of factionStruct[31], indexing starts at 1. Maximum 31. Slightly misleading name, sorted by the turn order of the factions. Player controlled faction is at index 0 in single player.
		@tfield factionStruct[31] factionsSortedByID Table of factionStruct[31], indexing starts at 1, so add 1 to faction ID. Maximum 31.
		@tfield int numberOfFactions
		@tfield int numberOfHumanFactions Number of player-controlled factions.
		@tfield factionStruct currentFaction Faction whose turn it is at the moment, can be set.
		@tfield collegeOfCardinals collegeOfCardinals
		@tfield factionStruct papalFaction
		@tfield bool fogOfWar
		@tfield bool isPaused
		@tfield bool followMovement
		@tfield bool marianReformsActive
		@tfield bool marianReformsDisabled
		@tfield factionStruct slaveFaction
		@tfield int factionTurnID
		@tfield int tickCount
		@tfield int campaignWinnerID
		@tfield int millisecondCount
		@tfield float secondCount
		@tfield int turnNumber
		@tfield float timescale Factor for number of turns per year, see descr\_strat.txt
		@tfield settlementStruct romeSettlement
		@tfield settlementStruct constantinopleSettlement
		@tfield crusadeStruct crusade
		@tfield jihadStruct jihad
		@tfield float brigandSpawnValue Lower values increase spawn rate.
		@tfield float pirateSpawnValue Lower values increase spawn rate.
		@tfield int restrictAutoResolve
		@tfield int saveEnabled
		@tfield int freeUpkeepForts Number of units who get free upkeep in forts.
		@tfield float currentDate
		@tfield int currentSeason season (0=summer, 1=winter)
		@tfield float startDate
		@tfield int startSeason season (0=summer, 1=winter)
		@tfield float endDate
		@tfield int endSeason season (0=summer, 1=winter)
		@tfield int daysInBattle
		@tfield float currentTimeInBattle 24 max, so calculate as daysInBattle*24+currentTimeInBattle.
		@tfield int fortsNum
		@tfield int portsBuildingsNum
		@tfield int watchTowerNum
		@tfield int slaveFactionID
		@tfield int roadsNum
		@tfield int mercPoolNum
		@tfield int settlementNum
		@tfield bool nightBattlesEnabled
		@tfield bool rebellingCharactersActive
		@tfield checkDipStance checkDipStance
		@tfield setDipStance setDipStance
		@tfield getUnitSize getUnitSize
		@tfield getPlayerFaction getPlayerFaction
		@tfield getCampaignPath getCampaignPath
		@tfield getFort getFort
		@tfield getPort getPort
		@tfield getWatchTower getWatchTower
		@tfield getFaction getFaction
		@tfield getSettlementByName getSettlementByName
		@tfield getMercPool getMercPool
		@tfield getMercPoolByName getMercPoolByName
		@tfield getRoad getRoad
		@tfield getFactionByID getFactionByID
		@tfield getFactionByOrder getFactionByOrder
		@tfield isPlayableFaction isPlayableFaction
		@tfield isPlayerFaction isPlayerFaction
		@tfield isExistingFaction isExistingFaction
		@tfield getNumberOfMarriages getNumberOfMarriages
		@tfield getSettlement getSettlement

		@table campaignStruct
		*/
		typeAll.campaignTable = luaState.new_usertype<campaign>("campaignStruct");
		typeAll.campaignTable.set("campaignDifficultyFaction", sol::property([](campaign& self) { return std::ref(self.campaignDifficultyFaction); }));
		typeAll.campaignTable.set("battleDifficultyFaction", sol::property([](campaign& self) { return std::ref(self.battleDifficultyFaction); }));
		typeAll.campaignTable.set("factionsSortedByDescrStrat", sol::property([](campaign& self) { return std::ref(self.factionsSortedByTurnOrder); }));
		typeAll.campaignTable.set("factionsSortedByID", sol::property([](campaign& self) { return std::ref(self.factionsSortedByID); }));
		typeAll.campaignTable.set("numberOfFactions", &campaign::factionCount);
		typeAll.campaignTable.set("mercPoolNum", sol::property(&campaign::getMercPoolNum));
		typeAll.campaignTable.set("settlementNum", sol::property(&campaign::getSettlementNum));
		typeAll.campaignTable.set("papalFaction", &campaign::papalFaction);
		typeAll.campaignTable.set("numberOfHumanFactions", &campaign::humanPlayers);
		typeAll.campaignTable.set("collegeOfCardinals", &campaign::collegeOfCardinals);
		typeAll.campaignTable.set("currentFaction", &campaign::currentFactionTurn);
		typeAll.campaignTable.set("factionTurnID", &campaign::factionTurnID);
		typeAll.campaignTable.set("marianReformsActive", &campaign::marianReformsActive);
		typeAll.campaignTable.set("marianReformsDisabled", &campaign::marianReformsDisabled);
		typeAll.campaignTable.set("tickCount", &campaign::tickCount);
		typeAll.campaignTable.set("millisecondCount", &campaign::millisecondCount);
		typeAll.campaignTable.set("slaveFaction", sol::property(&campaign::getSlaveFaction));
		typeAll.campaignTable.set("secondCount", &campaign::secondCount);
		typeAll.campaignTable.set("fogOfWar", &campaign::fogOfWar);
		typeAll.campaignTable.set("isPaused", &campaign::isPaused);
		typeAll.campaignTable.set("followMovement", &campaign::followMovement);
		typeAll.campaignTable.set("passedTurnsNum", &campaign::turnNumber);
		typeAll.campaignTable.set("turnNumber", &campaign::turnNumber);
		typeAll.campaignTable.set("crusade", &campaign::crusade);
		typeAll.campaignTable.set("campaignWinnerID", &campaign::campaignWinnerID);
		typeAll.campaignTable.set("jihad", &campaign::jihad);

		typeAll.campaignTable.set("timescale", &campaign::timeScale);
		typeAll.campaignTable.set("slaveFactionID", &campaign::slaveFactionID);
		typeAll.campaignTable.set("romeSettlement", &campaign::rome);
		typeAll.campaignTable.set("constantinopleSettlement", &campaign::constantinople);

		typeAll.campaignTable.set("brigandSpawnValue", &campaign::brigandSpawnValue);
		typeAll.campaignTable.set("pirateSpawnValue", &campaign::pirateSpawnValue);
		typeAll.campaignTable.set("restrictAutoResolve", &campaign::restrictAutoResolve);
		typeAll.campaignTable.set("saveEnabled", &campaign::saveEnabled);
		typeAll.campaignTable.set("freeUpkeepForts", &campaign::freeUpkeepForts);
		typeAll.campaignTable.set("rebellingCharactersActive", &campaign::rebellingCharactersActive);
		typeAll.campaignTable.set("nightBattlesEnabled", &campaign::nightBattlesEnabled);

		typeAll.campaignTable.set("currentDate", &campaign::currentDate);
		typeAll.campaignTable.set("currentSeason", &campaign::season);

		typeAll.campaignTable.set("startDate", &campaign::startDate);
		typeAll.campaignTable.set("startSeason", &campaign::startSeason);

		typeAll.campaignTable.set("endDate", &campaign::endDate);
		typeAll.campaignTable.set("endSeason", &campaign::endSeason);

		typeAll.campaignTable.set("daysInBattle", &campaign::daysInBattle);
		typeAll.campaignTable.set("currentTimeInBattle", &campaign::currentTimeInBattle);
		typeAll.campaignTable.set("fortsNum", &campaign::fortsNum);
		typeAll.campaignTable.set("roadsNum", &campaign::roadsNum);
		typeAll.campaignTable.set("portsBuildingsNum", &campaign::portsBuildingsNum);
		typeAll.campaignTable.set("watchTowerNum", &campaign::watchtowersNum);
		/***
		Check if a diplomatic relation between two factions.
		@function campaignStruct:checkDipStance
		@tparam dipRelType checkType Example: dipRelType.war
		@tparam factionStruct fac1 A faction.
		@tparam factionStruct fac2 Another faction.
		@treturn bool checkResult
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local england = campaign:getFaction("england")
		local france = campaign:getFaction("france")
		local isAtWar = campaign:checkDipStance(dipRelType.war, england, france);
		*/
		typeAll.campaignTable.set_function("checkDipStance", &campaign::checkDipStance);
		/***
		Set a diplomatic relation between two factions.
		@function campaignStruct:setDipStance
		@tparam dipRelType relType Example: dipRelType.war
		@tparam factionStruct fac1 A faction.
		@tparam factionStruct fac2 Another faction.
    	@usage
		local campaign = gameDataAll.get().campaignStruct;
		local england = campaign:getFaction("england")
		local france = campaign:getFaction("france")
		campaign:setDipStance(dipRelType.war, england, france);
		*/
		typeAll.campaignTable.set_function("setDipStance", &campaign::setDipStance);
		/***
		Get size of unit(i.e. small or medium, etc). Numbers from 0 to 3
		@function campaignStruct.getUnitSize
		@treturn int unitSize
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local unitSize = campaign.getUnitSize();
		*/
		typeAll.campaignTable.set_function("getUnitSize", &smallFuncs::GetUnitSize);
		/***
		Get fort by index.
		@function campaignStruct:getFort
		@tparam int index
		@treturn fortStruct fort
		@usage
    	local campaign = gameDataAll.get().campaignStruct
		for i = 0, campaign.fortNum - 1 do
			local fort = campaign:getFort(i)
		end
		*/
		typeAll.campaignTable.set_function("getFort", &campaign::getFort);
		/***
		Get port by index.
		@function campaignStruct:getPort
		@tparam int index
		@treturn portStruct port
		@usage
    	local campaign = gameDataAll.get().campaignStruct
		for i = 0, campaign.portNum - 1 do
			local port = campaign:getPort(i)
		end
		*/
		typeAll.campaignTable.set_function("getPort", &campaign::getPort);
		/***
		Get watchtower by index.
		@function campaignStruct:getWatchTower
		@tparam int index
		@treturn watchtowerStruct watchtower
		@usage
		local campaign = gameDataAll.get().campaignStruct
		for i = 0, campaign.watchTowerNum - 1 do
			local watchtower = campaign:getWatchTower(i)
		end
		*/
		typeAll.campaignTable.set_function("getWatchTower", &campaign::getWatchTower);
		/***
		Get settlement by internal name.
		@function campaignStruct:getSettlementByName
		@tparam string name
		@treturn settlementStruct settlement
		@usage
		local campaign = gameDataAll.get().campaignStruct
		local sett = campaign:getSettlementByName("London")
		*/
		typeAll.campaignTable.set_function("getSettlementByName", &campaign::getSettlementByName);
		/***
		Get a faction by it's internal name.
		@function campaignStruct:getFaction
		@tparam string name
		@treturn factionStruct faction
		@usage
		local CAMPAIGN = gameDataAll.get().campaignStruct
		local fac = CAMPAIGN:getFaction("england")
		*/
		typeAll.campaignTable.set_function("getFaction", &campaign::getFactionHashed);
		/***
		Get path to the current descr\_strat file used.
		@function campaignStruct:getCampaignPath
		@treturn string path
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local path = campaign:getCampaignPath();
		*/
		typeAll.campaignTable.set_function("getCampaignPath", &campaign::getCampaignPath);
		/***
		Get a mercenary pool by index.
		@function campaignStruct:getMercPool
		@tparam int index
		@treturn mercPool pool
		@usage
    	local campaign = gameDataAll.get().campaignStruct;
		for i = 0, campaign.mercPoolNum - 1 do
			local pool = campaign:getMercPool(i)
		end
		*/
		typeAll.campaignTable.set_function("getMercPool", &campaign::getMercPool);
		/***
		Get a mercenary pool by name.
		@function campaignStruct:getMercPoolByName
		@tparam string name
		@treturn mercPool pool
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local pool = campaign:getMercPoolByName("mercpool1");
		*/
		typeAll.campaignTable.set_function("getMercPoolByName", &campaign::getMercPoolByName);
		/***
		Get a road by index.
		@function campaignStruct:getRoad
		@tparam int index
		@treturn roadStruct road
		@usage
    	local campaign = gameDataAll.get().campaignStruct
		for i = 0, campaign.roadsNum - 1 do
			local road = campaign:getRoad(i)
		end
		*/
		typeAll.campaignTable.set_function("getRoad", &campaign::getRoad);
		/***
		Get a human faction by index.
		@function campaignStruct:getPlayerFaction
		@tparam int index
		@treturn factionStruct playerFac
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local fac = campaign:getPlayerFaction(0);
		*/
		typeAll.campaignTable.set_function("getPlayerFaction", &campaign::getPlayerFaction);
		/***
		Get a faction by faction ID.
		@function campaignStruct:getFactionByID
		@tparam int factionID
		@treturn factionStruct fac
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local fac = campaign:getFactionByID(0);
		*/
		typeAll.campaignTable.set_function("getFactionByID", &campaign::getFactionById);
		/***
		Get a faction by turn order (best way to iterate over factions on the campaign map).
		@function campaignStruct:getFactionByOrder
		@tparam int index
		@treturn factionStruct fac
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		for i = 0, campaign.numberOfFactions - 1 do
			local fac = campaign:getFactionByOrder(i);
		end
		*/
		typeAll.campaignTable.set_function("getFactionByOrder", &campaign::getFactionByOrder);
		/***
		Check if a faction is playable.
		@function campaignStruct:isPlayableFaction
		@tparam int factionID
		@treturn bool isPlayable
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		for i = 0, campaign.numberOfFactions - 1 do
			local fac = campaign:getFactionByOrder(i);
    	    local playable = campaign:isPlayableFaction(fac.factionID);
		end
		*/
		typeAll.campaignTable.set_function("isPlayableFaction", &campaign::isPlayableFaction);
		/***
		Check if a faction is player controlled.
		@function campaignStruct:isPlayerFaction
		@tparam int factionID
		@treturn bool isPlayer
    	@usage
		local campaign = gameDataAll.get().campaignStruct;
		for i = 0, campaign.numberOfFactions - 1 do
			local fac = campaign:getFactionByOrder(i);
			local isPlayer = campaign:isPlayerFaction(fac.factionID);
		end
		*/
		typeAll.campaignTable.set_function("isPlayerFaction", &campaign::isPlayerFaction);
		/***
		Check if a faction is currently active in the campaign.
		@function campaignStruct:isExistingFaction
		@tparam int factionID
		@treturn bool exists
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local fac = campaign:isExistingFaction(4);
		*/
		typeAll.campaignTable.set_function("isExistingFaction", &campaign::isExistingFaction);
		/***
		Check how many interfaction marriages have been done between the 2 factions.
		@function campaignStruct:getNumberOfMarriages
		@tparam int factionIDOne
		@tparam int factionIDTwo
		@treturn int marriageNum
		@usage
		local campaign = gameDataAll.get().campaignStruct;
		local marriages = campaign:getNumberOfMarriages(4, 8);
		*/
		typeAll.campaignTable.set_function("getNumberOfMarriages", &campaign::getNumberOfMarriages);
		/***
		Get settlement by index.
		@function campaignStruct:getSettlement
		@tparam int index
		@treturn settlementStruct sett
		@usage
    	local campaign = gameDataAll.get().campaignStruct;
		for i = 0, campaign.settlementNum - 1 do
			local sett = campaign:getSettlement(i);
		end
		*/
		typeAll.campaignTable.set_function("getSettlement", &campaign::getSettlement);

		///factionDiplomacy
		//@section Faction diplomacy

		/***
		Basic faction diplomacy table.
		@tfield int previousState
		@tfield int state (don't set, use setDipStance so all proper functionality fires)
		@tfield bool hasTradeRights (don't set, use setDipStance so all proper functionality fires)
		@tfield bool hasMilitaryAccess
		@tfield bool isTrustWorthy
		@tfield bool isProtectorate (don't set, use setDipStance so all proper functionality fires)
		@tfield bool isKnown
		@tfield bool lastDiplomaticResult
		@tfield float factionStanding
		@tfield int protectoratePayment
		@tfield int numTurnsAllied
		@tfield int numTurnsWar
		@tfield int numTurnsState
		@tfield int numTurnsTrade
		@tfield int numTurnsAccess
		@tfield int numTurnsProtectorate
		@tfield int numTreaties
		@tfield int numAssists
		@tfield int numAssistFailures
		@tfield int numEmptyThreats
		@tfield int numValidThreats
		@tfield int leaveTerritoryTimer
		@tfield int winsInBattle
		@tfield int tributeAmount
		@tfield int tributeDuration
		@tfield int turnsSinceLastMeeting
		@tfield int diplomaticSuccess
		@tfield int diplomaticBalance
		@tfield int lastMapInfoTrade
		@tfield int numTurnsCeasefire

		@table factionDiplomacy
		*/
		typeAll.factionDiplomacy = luaState.new_usertype<factionDiplomacy>("factionDiplomacy");
		typeAll.factionDiplomacy.set("previousState", &factionDiplomacy::prevState);
		typeAll.factionDiplomacy.set("state", &factionDiplomacy::state);
		typeAll.factionDiplomacy.set("hasTradeRights", sol::property(&factionDiplomacy::getHasTradeRights));
		typeAll.factionDiplomacy.set("hasMilitaryAccess", sol::property(
		&factionDiplomacy::getHasMilitaryAccess, &factionDiplomacy::setHasMilitaryAccess));
    	typeAll.factionDiplomacy.set("isTrustWorthy", sol::property(
			&factionDiplomacy::getIsTrustWorthy, &factionDiplomacy::setIsTrustWorthy));
    	typeAll.factionDiplomacy.set("isProtectorate", sol::property(&factionDiplomacy::getIsProtectorate));
    	typeAll.factionDiplomacy.set("isKnown", sol::property(&factionDiplomacy::getIsKnown, &factionDiplomacy::setIsKnown));
    	typeAll.factionDiplomacy.set("lastDiplomaticResult", sol::property(
    		&factionDiplomacy::getLastDiplomaticResult, &factionDiplomacy::setLastDiplomaticResult));
    	typeAll.factionDiplomacy.set("factionStanding", &factionDiplomacy::factionStanding);
    	typeAll.factionDiplomacy.set("protectoratePayment", &factionDiplomacy::protectoratePayment);
    	typeAll.factionDiplomacy.set("numTurnsAllied", &factionDiplomacy::numTurnsAllied);
    	typeAll.factionDiplomacy.set("numTurnsWar", &factionDiplomacy::numTurnsWar);
    	typeAll.factionDiplomacy.set("numTurnsState", &factionDiplomacy::numTurnsState);
    	typeAll.factionDiplomacy.set("numTurnsTrade", &factionDiplomacy::numTurnsTrade);
    	typeAll.factionDiplomacy.set("numTurnsAccess", &factionDiplomacy::numTurnsAccess);
    	typeAll.factionDiplomacy.set("numTurnsProtectorate", &factionDiplomacy::numTurnsProtectorate);
    	typeAll.factionDiplomacy.set("numTreaties", &factionDiplomacy::numTreaties);
    	typeAll.factionDiplomacy.set("numAssists", &factionDiplomacy::numAssists);
    	typeAll.factionDiplomacy.set("numAssistFailures", &factionDiplomacy::numAssistFailures);
    	typeAll.factionDiplomacy.set("numEmptyThreats", &factionDiplomacy::numEmptyThreats);
    	typeAll.factionDiplomacy.set("numValidThreats", &factionDiplomacy::numValidThreats);
    	typeAll.factionDiplomacy.set("leaveTerritoryTimer", &factionDiplomacy::leaveTerritoryTimer);
    	typeAll.factionDiplomacy.set("winsInBattle", &factionDiplomacy::winsInBattle);
    	typeAll.factionDiplomacy.set("tributeAmount", &factionDiplomacy::tributeAmount);
    	typeAll.factionDiplomacy.set("tributeDuration", &factionDiplomacy::tributeDuration);
    	typeAll.factionDiplomacy.set("turnsSinceLastMeeting", &factionDiplomacy::turnsSinceLastMeeting);
    	typeAll.factionDiplomacy.set("diplomaticSuccess", &factionDiplomacy::diplomaticSuccess);
        typeAll.factionDiplomacy.set("diplomaticBalance", &factionDiplomacy::diplomaticBalance);
    	typeAll.factionDiplomacy.set("lastMapInfoTrade", &factionDiplomacy::lastMapInfoTrade);
    	typeAll.factionDiplomacy.set("numTurnsCeasefire", &factionDiplomacy::numTurnsCeasefire);
    	
    	
		///CollegeofCardinals
		//@section College of cardinals

		/***
		Basic College of Cardinals table.
		@tfield characterRecord pope
		@tfield int cardinalNum
		@tfield int electionResultsCount
		@tfield bool popeIsOld
		@tfield bool inElection
		@tfield int popePreviousFactionID
		@tfield int maxCardinals
		@tfield int maxPreferati
		@tfield getCardinal getCardinal
		@tfield getElectionResult getElectionResult

		@table collegeOfCardinals
		*/
		typeAll.collegeOfCardinals = luaState.new_usertype<collegeOfCardinals>("collegeOfCardinals");
		typeAll.collegeOfCardinals.set("pope", &collegeOfCardinals::pope);
		typeAll.collegeOfCardinals.set("cardinalNum", &collegeOfCardinals::cardinalNum);
		typeAll.collegeOfCardinals.set("electionResultsCount", &collegeOfCardinals::electionResultsCount);
		typeAll.collegeOfCardinals.set("popeIsOld", &collegeOfCardinals::popeOld);
		typeAll.collegeOfCardinals.set("inElection", &collegeOfCardinals::inElection);
		typeAll.collegeOfCardinals.set("maxCardinals", &collegeOfCardinals::maxCardinals);
		typeAll.collegeOfCardinals.set("maxPreferati", &collegeOfCardinals::maxPreferati);
		typeAll.collegeOfCardinals.set("popePreviousFactionID", &collegeOfCardinals::popePreviousFaction);

		/***
		Get a specific cardinal by index.
		@function collegeOfCardinals:getCardinal
		@tparam int index
		@treturn cardinal cardinal
		@usage
		local college = campaign.collegeOfCardinals;
		local cardinal = college:getCardinal(2);
		*/
		typeAll.collegeOfCardinals.set_function("getCardinal", &collegeOfCardinals::getCardinal);

		/***
		Get a election result by index.
		@function collegeOfCardinals:getElectionResult
		@tparam int index
		@treturn cardinal electionResult
		@usage
		local college = campaign.collegeOfCardinals;
		local cardinal = college:getElectionResult(2);
		*/
		typeAll.collegeOfCardinals.set_function("getElectionResult", &collegeOfCardinals::getElectionResult);
    	
		/***
		Basic cardinal table.
		@tfield character character
		@tfield int factionID
		@tfield int voteFactionID
		@tfield bool isPreferati

		@table cardinal
		*/
		typeAll.cardinal = luaState.new_usertype<cardinal>("cardinal");
		typeAll.cardinal.set("character", sol::property(&cardinal::getCharacter));
		typeAll.cardinal.set("factionID", &cardinal::factionID);
    	typeAll.cardinal.set("voteFactionID", &cardinal::voteFactionID);
    	typeAll.cardinal.set("isPreferati", &cardinal::isPreferati);

		/// Crusade
		//@section Crusades

		/***
		Basic crusade table.

		@tfield int startTurn
		@tfield int endTurn
		@tfield settlementStruct targetSettlement
		@tfield int length
		@tfield int outcome
		@tfield int orthodoxID
		@tfield int catholicID
		@tfield int hereticID
		@tfield factionJoined factionJoined
		@tfield factionJoinedLastTurn factionJoinedLastTurn
		@tfield factionFought factionFought
		@tfield factionInTarget factionInTarget
		@tfield getReward getReward

		@table crusadeStruct
		*/
		typeAll.crusadeStruct = luaState.new_usertype<crusade>("crusadeStruct");
		typeAll.crusadeStruct.set("startTurn", &crusade::startTurn);
		typeAll.crusadeStruct.set("endTurn", &crusade::endTurn);
		typeAll.crusadeStruct.set("targetSettlement", &crusade::targetSettlement);
		typeAll.crusadeStruct.set("length", &crusade::length);
		typeAll.crusadeStruct.set("outcome", &crusade::outcome);
		typeAll.crusadeStruct.set("orthodoxID", &crusade::orthodoxId);
		typeAll.crusadeStruct.set("catholicID", &crusade::catholicId);
    	typeAll.crusadeStruct.set("hereticID", &crusade::hereticId);

    	/***
		Check if a faction has joined this crusade.
		@function crusadeStruct:factionJoined
		@tparam int factionID
		@treturn bool hasJoined
		@usage
		local joined = crusadeStruct:factionJoined(2);
		*/
    	typeAll.crusadeStruct.set_function("factionJoined", &crusade::factionJoined);

    	/***
		Check if a faction has joined this crusade last turn.
		@function crusadeStruct:factionJoinedLastTurn
		@tparam int factionID
		@treturn bool hasJoined
		@usage
		local joined = crusadeStruct:factionJoinedLastTurn(2);
		*/
    	typeAll.crusadeStruct.set_function("factionJoinedLastTurn", &crusade::factionJoinedLastTurn);

    	/***
		Check if a faction has fought in this crusade.
		@function crusadeStruct:factionFought
		@tparam int factionID
		@treturn bool hasFought
		@usage
		local fought = crusadeStruct:factionFought(2);
		*/
    	typeAll.crusadeStruct.set_function("factionFought", &crusade::factionFought);

    	/***
		Check if a faction is in the crusade target region.
		@function crusadeStruct:factionInTarget
		@tparam int factionID
		@treturn bool inTarget
		@usage
		local inTarget = crusadeStruct:factionInTarget(2);
		*/
    	typeAll.crusadeStruct.set_function("factionInTarget", &crusade::factionInTarget);

    	/***
		Get reward a faction will get for winning this crusade.
		@function crusadeStruct:getReward
		@tparam int factionID
		@treturn crusadeReward reward
		@usage
		local reward = crusadeStruct:getReward(2);
		*/
    	typeAll.crusadeStruct.set_function("getReward", &crusade::getReward);
    	
		/***
		Basic jihad table.

		@tfield int startTurn
		@tfield int endTurn
		@tfield settlementStruct targetSettlement
		@tfield int length
		@tfield int outcome
		@tfield int muslimID
		@tfield factionJoined factionJoined
		@tfield factionJoinedLastTurn factionJoinedLastTurn
		@tfield factionFought factionFought
		@tfield factionInTarget factionInTarget
		@tfield getReward getReward

		@table jihadStruct
		*/
		typeAll.jihadStruct = luaState.new_usertype<jihad>("jihadStruct");
		typeAll.jihadStruct.set("startTurn", &jihad::startTurn);
		typeAll.jihadStruct.set("endTurn", &jihad::endTurn);
		typeAll.jihadStruct.set("targetSettlement", &jihad::targetSettlement);
		typeAll.jihadStruct.set("length", &jihad::length);
    	typeAll.jihadStruct.set("outcome", &jihad::outcome);
    	typeAll.jihadStruct.set("muslimID", &jihad::muslimId);

    	/***
		Check if a faction has joined this jihad.
		@function jihadStruct:factionJoined
		@tparam int factionID
		@treturn bool hasJoined
		@usage
		local joined = jihadStruct:factionJoined(2);
		*/
    	typeAll.jihadStruct.set_function("factionJoined", &jihad::factionJoined);

    	/***
		Check if a faction has joined this jihad last turn.
		@function jihadStruct:factionJoinedLastTurn
		@tparam int factionID
		@treturn bool hasJoined
		@usage
		local joined = jihadStruct:factionJoinedLastTurn(2);
		*/
    	typeAll.jihadStruct.set_function("factionJoinedLastTurn", &jihad::factionJoinedLastTurn);

    	/***
		Check if a faction has fought in this jihad.
		@function jihadStruct:factionFought
		@tparam int factionID
		@treturn bool hasFought
		@usage
		local fought = jihadStruct:factionFought(2);
		*/
    	typeAll.jihadStruct.set_function("factionFought", &jihad::factionFought);

    	/***
		Check if a faction is in the jihad target region.
		@function jihadStruct:factionInTarget
		@tparam int factionID
		@treturn bool inTarget
		@usage
		local inTarget = jihadStruct:factionInTarget(2);
		*/
    	typeAll.jihadStruct.set_function("factionInTarget", &jihad::factionInTarget);

    	/***
		Get reward a faction will get for winning this jihad.
		@function jihadStruct:getReward
		@tparam int factionID
		@treturn crusadeReward reward
		@usage
		local reward = jihadStruct:getReward(2);
		*/
    	typeAll.jihadStruct.set_function("getReward", &jihad::getReward);
    	
    	/***
		Basic crusadeReward table.

		@tfield int money
		@tfield int experience

		@table crusadeReward
		*/
		typeAll.crusadeReward = luaState.new_usertype<crusadeReward>("crusadeReward");
    	typeAll.crusadeReward.set("money", &crusadeReward::money);
    	typeAll.crusadeReward.set("experience", &crusadeReward::experience);
    	
		///MercPools
		//@section Mercenaries

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
		typeAll.mercPool.set_function("getMercUnitNum", &mercPool::getMercUnitNum);

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
		typeAll.mercPool.set_function("addMercUnit", &mercPool::addMercUnit);

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
		typeAll.mercPool.set_function("getMercUnit", &mercPool::getMercUnit);


		///MercenaryUnits
		//@section mercPoolUnit

		/***
		Basic mercenary unit table.

		@tfield eduEntry eduEntry
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
		typeAll.mercPoolUnit.set_function("setMercReligion", &mercPoolUnit::setMercReligion);
	
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
		@tfield armyStruct blockingArmy

		@table watchtowerStruct
		*/
		typeAll.watchtowerStruct = luaState.new_usertype<watchTowerStruct>("watchtowerStruct");
		typeAll.watchtowerStruct.set("xCoord", &watchTowerStruct::xCoord);
		typeAll.watchtowerStruct.set("yCoord", &watchTowerStruct::yCoord);
		typeAll.watchtowerStruct.set("factionID", &watchTowerStruct::factionID);
		typeAll.watchtowerStruct.set("regionID", &watchTowerStruct::regionID);
		typeAll.watchtowerStruct.set("faction", &watchTowerStruct::faction);
		typeAll.watchtowerStruct.set("settlement", &watchTowerStruct::settlement);
		typeAll.watchtowerStruct.set("blockingArmy", &watchTowerStruct::blockingArmy);

			
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
		@tfield armyStruct blockadingArmy Enemy army blockading the port, by standing on it's tile, check for nil.
		@tfield dockStruct dock water tile, only upgraded ports have this, check for nil.


		@table portStruct
		*/
		typeAll.portStruct = luaState.new_usertype<portBuildingStruct>("portStruct");
		typeAll.portStruct.set("xCoord", &portBuildingStruct::xCoord);
		typeAll.portStruct.set("yCoord", &portBuildingStruct::yCoord);
		typeAll.portStruct.set("ownerFaction", &portBuildingStruct::fac);
		typeAll.portStruct.set("settlement", &portBuildingStruct::settlement);
		typeAll.portStruct.set("blockadingArmy", &portBuildingStruct::occupantsArmy);
		typeAll.portStruct.set("dock", &portBuildingStruct::portDock);
		typeAll.portStruct.set("regionID", &portBuildingStruct::regionID);
		typeAll.portStruct.set("numTurnsBlocked", &portBuildingStruct::numTurnsBlocked);
		typeAll.portStruct.set("cultureID", &portBuildingStruct::subFactionCultureID);


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
		@tfield armyStruct dockedArmy
		

		@table dockStruct
		*/
		typeAll.dockStruct = luaState.new_usertype<portDockStrat>("dockStruct");
		typeAll.dockStruct.set("xCoord", &portDockStrat::xCoord);
		typeAll.dockStruct.set("yCoord", &portDockStrat::yCoord);
		typeAll.dockStruct.set("regionID", &portDockStrat::regionID);
		typeAll.dockStruct.set("ownerFaction", &portDockStrat::fac);
		typeAll.dockStruct.set("settlement", &portDockStrat::settlement);
		typeAll.dockStruct.set("cultureID", &portDockStrat::subFactionCultureID);
		typeAll.dockStruct.set("numTurnsBlocked", &portDockStrat::numTurnsBlocked);
		typeAll.dockStruct.set("port", &portDockStrat::port);
		typeAll.dockStruct.set("dockedArmy", &portDockStrat::dockedArmy);

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
		typeAll.tradeResource = luaState.new_usertype<resourceStruct>("tradeResource");
		typeAll.tradeResource.set("xCoord", &resourceStruct::xCoord);
		typeAll.tradeResource.set("yCoord", &resourceStruct::yCoord);
		typeAll.tradeResource.set("regionID", &resourceStruct::regionID);
		typeAll.tradeResource.set("resourceID", &resourceStruct::resourceID);
		typeAll.tradeResource.set("settlement", &resourceStruct::settlement);
		/***
		Set the resource's strat model.
		@function tradeResource:setStratModel
		@tparam int modelId Added with stratmap.objects.addModelToGame
		@usage
		tradeResource:setStratModel(5);
		*/
		typeAll.tradeResource.set_function("setStratModel", &resourceStruct::setModel);
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
		typeAll.tradeResource.set_function("getResourceID", &resourceStruct::getResourceCode);
		/***
		Get the resource's trade value.
		@function tradeResource:getResourceValue
		@treturn int value
		@usage
		if tradeResource:getResourceValue() == 5 then
			--do stuff
		end
		*/
		typeAll.tradeResource.set_function("getResourceValue", &resourceStruct::getResourceCost);
		/***
		Check if the resource has a mine.
		@function tradeResource:getResourceHasMine
		@treturn int hasMine 0=no mine, 1=mine
		@usage
		if tradeResource:getResourceHasMine() == 1 then
			--do stuff
		end
		*/
		typeAll.tradeResource.set_function("getResourceHasMine", &resourceStruct::getResourceHasMine);
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
		typeAll.tradeResource.set_function("getResourceImage", &resourceStruct::getImage);
    }
};
