///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "campaign.h"

#include "eopdu.h"
#include "gameHelpers.h"
#include "luaPlugin.h"
#include "stratModelsChange.h"
#include "techFuncs.h"
#include "army.h"
#include "fort.h"
#include "settlement.h"
#include "character.h"
#include "strategyMap.h"
#include "unit.h"

characterMoveData::characterMoveData(character* charPtr, const int searchType, const int turns)
: thisChar(charPtr)
{
	movePoints = charPtr->movePointsArmy;
	xCoord = charPtr->xCoord;
	yCoord = charPtr->yCoord;
	this->searchType = searchType;
	this->turns = turns;
	moveExtents = charPtr->getMoveExtents(searchType, turns);
	auto actualMinX = 100000000;
	auto actualMaxX = -100000000;
	auto actualMinY = 100000000;
	auto actualMaxY = -100000000;
	for (int x = moveExtents->xCoordMin; x <= moveExtents->xCoordMax; x++)
	{
		for (int y = moveExtents->yCoordMin; y <= moveExtents->yCoordMax; y++)
		{
			if (x == xCoord && y == yCoord)
				continue;
			if (const auto tileEx = moveExtents->getTile(x, y); tileEx->movePoints <= 50000)
				tiles.emplace_back(tileEx);
			else
				continue;
			if (x < actualMinX)
				actualMinX = x;
			if (x > actualMaxX)
				actualMaxX = x;
			if (y < actualMinY)
				actualMinY = y;
			if (y > actualMaxY)
				actualMaxY = y;
		}
	}
	for (int x = actualMinX; x <= actualMaxX; x++)
	{
		for (int y = actualMinY; y <= actualMaxY; y++)
		{
			if (x == xCoord && y == yCoord)
				continue;
			const auto tileEx = moveExtents->getTile(x, y);
			if (!tileEx)
				continue;
			const auto tile = stratMapHelpers::getTile(x, y);
			auto comp = [this](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				return stratMapHelpers::getDistanceInTiles(a.first, a.second, xCoord, yCoord)
					> stratMapHelpers::getDistanceInTiles(b.first, b.second, xCoord, yCoord);
			};
			if (const auto settlement = tile->getSettlement(); settlement)
			{
				if (tileEx->turns > 0 && tileEx->turns <= turns)
					settlements.emplace_back(settlement, tileEx, settlement->faction->factionID == thisChar->getFaction()->factionID);
				else
				{
					std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comp)> neighbours(comp);
					auto nTiles = stratMapHelpers::getNeighbourTiles(settlement->xCoord, settlement->yCoord);
					while(!nTiles.empty())
					{
						neighbours.push(nTiles.front());
						nTiles.pop();
					}
					while(!neighbours.empty())
					{
						const auto [nX, nY] = neighbours.top();
						neighbours.pop();
						if (const auto neighbourTileEx = moveExtents->getTile(nX, nY); (nX == xCoord && nY == yCoord) || (neighbourTileEx && neighbourTileEx->turns > 0 && neighbourTileEx->turns <= turns))
						{
							settlements.emplace_back(settlement, tileEx, settlement->faction->factionID == thisChar->getFaction()->factionID);
							settlements.back().turns = neighbourTileEx ? neighbourTileEx->turns : 1;
							settlements.back().moveCost = neighbourTileEx ? neighbourTileEx->movePoints : 0;
							break;
						}
					}
				}
			}
			if (const auto fort = tile->getFort(); fort)
			{
				if (tileEx->turns > 0 && tileEx->turns <= turns)
					forts.emplace_back(fort, tileEx, fort->faction->factionID == thisChar->getFaction()->factionID);
				else
				{
					std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comp)> neighbours(comp);
					auto nTiles = stratMapHelpers::getNeighbourTiles(fort->xCoord, fort->yCoord);
					while(!nTiles.empty())
					{
						neighbours.push(nTiles.front());
						nTiles.pop();
					}
					while(!neighbours.empty())
					{
						const auto [nX, nY] = neighbours.top();
						neighbours.pop();
						if (const auto neighbourTileEx = moveExtents->getTile(nX, nY);neighbourTileEx && neighbourTileEx->turns > 0 && neighbourTileEx->turns <= turns)
						{
							forts.emplace_back(fort, tileEx, fort->faction->factionID == thisChar->getFaction()->factionID);
							forts.back().turns = neighbourTileEx->turns;
							forts.back().moveCost = neighbourTileEx->movePoints;
							break;
						}
					}
				}
			}
			if (const auto resource = tile->getResource(); resource && tileEx->turns > 0 && tileEx->turns <= turns)
			{
				resources.emplace_back(resource, tileEx, tile->factionId == thisChar->getFaction()->factionID);
			}
			if (const int characterNum = tile->getTileCharacterCount(); characterNum > 0)
			{
				if (tileEx->turns > 0 && tileEx->turns <= turns)
				{
					for (int i = 0; i < characterNum; i++)
					{
						const auto tileChar = tile->getTileCharacterAtIndex(i);
						characters.emplace_back(tileChar, tileEx, tileChar->getFaction()->factionID == thisChar->getFaction()->factionID);
						if (tileChar->army)
							armies.emplace_back(tileChar->army, tileEx, tileChar->army->faction->factionID == thisChar->getFaction()->factionID);
					}
				}
				else
				{
					std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comp)> neighbours(comp);
					auto nTiles = stratMapHelpers::getNeighbourTiles(tile->getTileX(), tile->getTileY());
					while(!nTiles.empty())
					{
						neighbours.push(nTiles.front());
						nTiles.pop();
					}
					while(!neighbours.empty())
					{
						const auto [nX, nY] = neighbours.top();
						neighbours.pop();
						if (const auto neighbourTileEx = moveExtents->getTile(nX, nY);neighbourTileEx && neighbourTileEx->turns > 0 && neighbourTileEx->turns <= turns)
						{
							for (int i = 0; i < characterNum; i++)
							{
								const auto tileChar = tile->getTileCharacterAtIndex(i);
								characters.emplace_back(tileChar, tileEx, charPtr->getFaction()->factionID == thisChar->getFaction()->factionID);
								characters.back().turns = neighbourTileEx->turns;
								characters.back().moveCost = neighbourTileEx->movePoints;
								if (tileChar->army)
								{
									armies.emplace_back(tileChar->army, tileEx, tileChar->army->faction->factionID == thisChar->getFaction()->factionID);
									armies.back().turns = neighbourTileEx->turns;
									armies.back().moveCost = neighbourTileEx->movePoints;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
	sortSettlementsDistance();
	sortFortsDistance();
	sortResourcesDistance();
	sortCharactersDistance();
	sortArmiesDistance();
	sortTilesDistance();
}

void characterMoveData::sortArmiesStrength()
{
	std::sort(armies.begin(), armies.end(), [](const moveDataArmy& a, const moveDataArmy& b) { return a.army->totalStrength > b.army->totalStrength; });
}

void characterMoveData::sortSettlementsStrength()
{
	std::sort(settlements.begin(), settlements.end(), [](const moveDataSettlement& a, const moveDataSettlement& b)
	{
		int aStrength = 0;
		int bStrength = 0;
		if (a.settlement->army)
			aStrength = a.settlement->army->totalStrength;
		if (b.settlement->army)
			bStrength = b.settlement->army->totalStrength;
		return aStrength > bStrength;
	});
}

void characterMoveData::sortFortsStrength()
{
	std::sort(forts.begin(), forts.end(), [](const moveDataFort& a, const moveDataFort& b)
	{
		int aStrength = 0;
		int bStrength = 0;
		if (a.fort->army)
			aStrength = a.fort->army->totalStrength;
		if (b.fort->army)
			bStrength = b.fort->army->totalStrength;
		return aStrength > bStrength;
	});
}

void resourceStruct::setModel(UINT32 modelId)
{
	stratModelsChange::setModel(xCoord, yCoord, modelId, modelId);
}

oneTile* movementExtentTile::getTile()
{
	const auto stratMap = stratMapHelpers::getStratMap();
	if (!stratMap)
		return nullptr;
	return &stratMap->tilesArr[tileIndex];
}

characterMovementExtents* moveExtentsManager::createMoveExtents(character* searchChar, const int searchType, const int numTurns)
{
	return GAME_FUNC(characterMovementExtents*(__thiscall*)(moveExtentsManager*, int, character*, characterAction, int),
		createMoveExtents)(this, searchType, searchChar, characterAction::movingNormal, numTurns);
}

characterRecord* campaign::getCharacterByLabel(const std::string& label)
{
	for (int i = 0; i < factionCount; i++)
	{
		const auto fac = getFactionByOrder(i);
		if (const auto rec = fac->getCharacterByLabel(label); rec)
			return rec;
	}
	return nullptr;
}

characterRecord* campaign::worldwideAncillaryExists(const std::string& ancName)
{
	for (int i = 0; i < factionCount; i++)
	{
		if (const auto fac = getFactionByOrder(i); fac)
		{
			if (const auto rec = fac->ancillaryExists(ancName); rec)
				return rec;
		}
	}
	return nullptr;
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
		gameHelpers::scriptCommand("console_command", (command + "war").c_str());
	else if (dipType == peace)
		gameHelpers::scriptCommand("console_command", (command + "neutral").c_str());
	else if (dipType == alliance)
		gameHelpers::scriptCommand("console_command", (command + "allied").c_str());
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
	auto set = diplomaticStandings[facIdOne][facIdTwo].hasTradeRights == 0 ? 1 : 0;
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
	return gameStringHelpers::uniStringToStr(currentDescrFile);
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
	if (!plugData::data.luaAll.hashLoaded || plugData::data.luaAll.factions.empty())
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

scriptEvent::scriptEvent(const std::string& name, const std::string& eventType, const int xCoord, const int yCoord,
	const int scale, const std::string& movie)
{
	positionArray = techFuncs::allocateGameClass<coordPair>(sizeof(coordPair) * 8);
	regionArray = techFuncs::allocateGameClass<int>(sizeof(int) * 8);
	positionArray[0].xCoord = xCoord;
	positionArray[0].yCoord = yCoord;
	positionArrayNum = 1;
	gameStringHelpers::setHashedString(&eventTypeName, eventType.c_str());
	gameStringHelpers::setHashedString(&eventName, name.c_str());
	if (!movie.empty())
		gameStringHelpers::setHashedString(&this->movie, movie.c_str());
	this->scale = scale;
	eventController = GAME_FUNC(DWORD*(__thiscall*)(DWORD, scriptEvent*)
		, getEventController)(dataOffsets::offsets.eventManager, this);
}

void crusade::start(settlementStruct* target, const int timeToJoin, character* caller)
{
	callClassFunc<crusade*, void, settlementStruct*, int, character*>(this, 0x8, target, timeToJoin, caller);
}

void crusade::stop(const int result)
{
	callClassFunc<crusade*, void, int>(this, 0xC, result);
}

void jihad::start(settlementStruct* target, const int timeToJoin, character* caller)
{
	callClassFunc<jihad*, void, settlementStruct*, int, character*>(this, 0x8, target, timeToJoin, caller);
}

void jihad::stop(const int result)
{
	callClassFunc<jihad*, void, int>(this, 0xC, result);
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
	
	void execScriptEvent(const std::string& name, const std::string& eventType, const int xCoord, const int yCoord,
		const int scale, const std::string& movie)
	{
		const auto event = new scriptEvent(name, eventType, xCoord, yCoord, scale, movie);
		if (event->eventController)
			callClassFunc<DWORD*, void, scriptEvent*>(event->eventController, 0x0, event);
		GAME_FUNC(void(__thiscall*)(scriptEvent*), deleteScriptEvent)(event);
	}
	
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
			gameStringHelpers::setHashedString(&stratMod->tga, newS.c_str());
	}
	
	int getPoolIndex(const gameList<mercPoolUnit> *unitPtr)
	{
		if (const auto nextUnitsPtr = unitPtr->next; nextUnitsPtr && nextUnitsPtr->currentCount )
			return unitPtr->currentCount + getPoolIndex(nextUnitsPtr);
		return unitPtr->currentCount;
	}

	campaignDifficulty1* getCampaignDifficulty1()
	{
		return reinterpret_cast<campaignDifficulty1*>(dataOffsets::offsets.campaignDiff1);
	}

	campaignDifficulty2* getCampaignDifficulty2()
	{
		return reinterpret_cast<campaignDifficulty2*>(dataOffsets::offsets.campaignDiff2);
	}

	void setPerfectSpy(const bool set)
	{
		*reinterpret_cast<bool*>(dataOffsets::offsets.perfectSpy) = set;
	}

	int getLocalFactionID()
	{
		return *gameHelpers::getGameDataAll()->localFactionID;
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

	selectionInfo* getSelectionInfo()
	{
		return gameHelpers::getGameDataAll()->selectInfo;
	}

	int modifyWithSettMechanics(const std::string& mechanic, const float raw, const bool isCastle)
	{
		const auto hashed = reinterpret_cast<stringWithHash*>(gameStringHelpers::createHashedString(mechanic.c_str()));
		return GAME_FUNC(int(__cdecl*)(const char*, int, float, bool), modifyWithSettMechanics)(hashed->name, hashed->hash, raw, isCastle);
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
        return gameHelpers::getGameDataAll()->campaignStruct;
    }

	stratPathFinding* getStratPathFinding()
	{
		return gameHelpers::getGameDataAll()->stratPathFinding;
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
        	sol::usertype<campaignDifficulty1> campaignDifficulty1;
        	sol::usertype<campaignDifficulty2> campaignDifficulty2;
        	sol::usertype<selectionInfo> selectionInfo;
        	sol::usertype<characterMovementExtents> characterMovementExtents;
        	sol::usertype<movementExtentTile> movementExtentTile;
        	sol::usertype<characterMoveData> characterMoveData;
        	sol::usertype<moveDataTile> moveDataTile;
        	sol::usertype<moveDataFort> moveDataFort;
        	sol::usertype<moveDataResource> moveDataResource;
        	sol::usertype<moveDataArmy> moveDataArmy;
        	sol::usertype<moveDataCharacter> moveDataCharacter;
        	sol::usertype<moveDataSettlement> moveDataSettlement;
        }typeAll;
    	
		///Campaign
		//@section Campaign

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
		@tfield getFactionDiplomacy getFactionDiplomacy
		@tfield historicEvent historicEvent
		@tfield addCharacterCas addCharacterCas
		@tfield getCharacterByLabel getCharacterByLabel
		@tfield worldwideAncillaryExists worldwideAncillaryExists
		@tfield execScriptEvent execScriptEvent

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
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
		local unitSize = campaign.getUnitSize();
		*/
		typeAll.campaignTable.set_function("getUnitSize", &gameHelpers::getUnitSize);
		/***
		Get fort by index.
		@function campaignStruct:getFort
		@tparam int index
		@treturn fortStruct fort
		@usage
    	local campaign = M2TW.campaign
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
    	local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
		local sett = campaign:getSettlementByName("London")
		*/
		typeAll.campaignTable.set_function("getSettlementByName", &campaign::getSettlementByName);
		/***
		Get a faction by it's internal name.
		@function campaignStruct:getFaction
		@tparam string name
		@treturn factionStruct faction
		@usage
		local CAMPAIGN = M2TW.campaign
		local fac = CAMPAIGN:getFaction("england")
		*/
		typeAll.campaignTable.set_function("getFaction", &campaign::getFactionHashed);
		/***
		Get path to the current descr\_strat file used.
		@function campaignStruct:getCampaignPath
		@treturn string path
		@usage
		local campaign = M2TW.campaign
		local path = campaign:getCampaignPath();
		*/
		typeAll.campaignTable.set_function("getCampaignPath", &campaign::getCampaignPath);
		/***
		Get a mercenary pool by index.
		@function campaignStruct:getMercPool
		@tparam int index
		@treturn mercPool pool
		@usage
    	local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
		local pool = campaign:getMercPoolByName("mercpool1");
		*/
		typeAll.campaignTable.set_function("getMercPoolByName", &campaign::getMercPoolByName);
		/***
		Get a road by index.
		@function campaignStruct:getRoad
		@tparam int index
		@treturn roadStruct road
		@usage
    	local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
		local fac = campaign:getPlayerFaction(0);
		*/
		typeAll.campaignTable.set_function("getPlayerFaction", &campaign::getPlayerFaction);
		/***
		Get a faction by faction ID.
		@function campaignStruct:getFactionByID
		@tparam int factionID
		@treturn factionStruct fac
		@usage
		local campaign = M2TW.campaign
		local fac = campaign:getFactionByID(0);
		*/
		typeAll.campaignTable.set_function("getFactionByID", &campaign::getFactionById);
		/***
		Get a faction by turn order (best way to iterate over factions on the campaign map).
		@function campaignStruct:getFactionByOrder
		@tparam int index
		@treturn factionStruct fac
		@usage
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
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
		local campaign = M2TW.campaign
		local marriages = campaign:getNumberOfMarriages(4, 8)
		*/
		typeAll.campaignTable.set_function("getNumberOfMarriages", &campaign::getNumberOfMarriages);
		
		/***
		Get faction diplomacy data.
		@function campaignStruct:getFactionDiplomacy
		@tparam int factionIDOne
		@tparam int factionIDTwo
		@treturn factionDiplomacy diplomacy
		@usage
		local campaign = M2TW.campaign
		local diplomacy = campaign:getFactionDiplomacy(4, 8)
		*/
		typeAll.campaignTable.set_function("getFactionDiplomacy", &campaign::getFactionDiplomacy);
		
		/***
		Get settlement by index.
		@function campaignStruct:getSettlement
		@tparam int index
		@treturn settlementStruct sett
		@usage
    	local campaign = M2TW.campaign
		for i = 0, campaign.settlementNum - 1 do
			local sett = campaign:getSettlement(i);
		end
		*/
		typeAll.campaignTable.set_function("getSettlement", &campaign::getSettlement);
		/***
		Add a new .cas character strategy model to the game with a unique name. Only add it after loading to campaign map!
		@function campaignStruct.addCharacterCas
		@tparam string skeleton name of skeleton used.
		@tparam string caspath Relative path from the mods folder (starting with "mods/").
		@tparam string shadowcaspath Relative path from the mods folder (starting with "mods/").
		@tparam string typename Name of the new model used to assign.
		@tparam string texturepath Relative path from the mods folder (starting with "mods/").
		@tparam float scale
		@usage
		M2TW.campaign.addCharacterCas(
			"strat_named_with_army",
			"mods/Bare_Geomod/data/models_strat/islamic_general2.cas",
			"mods/Bare_Geomod/data/models_strat/shadow_sword2.cas",
			"islamic_general2",
			"mods/Bare_Geomod/data/models_strat/textures/islamic_general_turks.tga",
			0.7
		);
		*/
		typeAll.campaignTable.set_function("addCharacterCas", &stratModelsChange::addCharacterCas);
		/***
		Fire a game event message. Picture needs to be provided in the ui folders as default.
		@function campaignStruct.historicEvent
		@tparam string eventName
		@tparam string title
		@tparam string body
		@tparam bool isChoice optional
		@tparam int xCoord optional, -1 to disable
		@tparam int yCoord optional, -1 to disable
		@tparam table factions optional
		@usage
		    M2TW.campaign.historicEvent("my_event", "my title", "my description")
		*/
		typeAll.campaignTable.set_function("historicEvent", sol::overload(
			sol::resolve<void(const std::string&, const std::string&, const std::string&)>(gameHelpers::historicEvent),
			sol::resolve<void(const std::string&, const std::string&, const std::string&, bool)>(gameHelpers::historicEvent),
			sol::resolve<void(const std::string&, const std::string&, const std::string&, bool, int, int)>(gameHelpers::historicEvent),
			sol::resolve<void(const std::string&, const std::string&, const std::string&, bool, int, int, const sol::table&)>(gameHelpers::historicEvent)
		));
		/***
		Get a character by script label.
		@function campaignStruct:getCharacterByLabel
		@tparam string label
		@treturn characterRecord charRecord
		@usage
		    M2TW.campaign:getCharacterByLabel("rufus_1")
		*/
		typeAll.campaignTable.set_function("getCharacterByLabel", &campaign::getCharacterByLabel);
        /***
		Get the first character it finds holding an ancillary, or nil if it doesn't exist in the campaign.
		@function campaignStruct:worldwideAncillaryExists
		@tparam string ancName
		@treturn characterRecord characterWithAnc
		@usage
		    local ringHolder = M2TW.campaign:worldwideAncillaryExists("one_ring")
		*/
		typeAll.campaignTable.set_function("worldwideAncillaryExists", &campaign::worldwideAncillaryExists);
        /***
		Fire an event such as a disaster.
		@function campaignStruct.execScriptEvent
		@tparam string name Needs entry in historic_events.txt!
		@tparam string eventType earthquake, flood, horde, storm, volcano, dustbowl, locusts, famine, plague, riot, fire, historic
		@tparam int xCoord
		@tparam int yCoord
		@tparam int scale default is 1
		@tparam string movie Empty string if not applicable
		@usage
		    M2TW.campaign.execScriptEvent("my_event", "volcano", 214, 122, 1, "")
		*/
		typeAll.campaignTable.set_function("execScriptEvent", &execScriptEvent);

		///Faction Diplomacy
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
    	
    	
		///College of Cardinals
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

		///Crusades
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
		@tfield start start
		@tfield stop stop

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
		Start a new crusade.
		@function crusadeStruct:start
		@tparam settlementStruct target
		@tparam int timeToJoin (Game uses 10).
		@tparam character caller
		@usage
				crusade:start(someSett, 10, someChar);
		*/
		typeAll.crusadeStruct.set_function("start", &crusade::start);

		/***
		Stop an ongoing crusade.
		@function crusadeStruct:stop
		@tparam int result none = 0, success = 1, failure = 2, cancelled = 3
		@usage
				crusade:stop(3);
		*/
		typeAll.crusadeStruct.set_function("stop", &crusade::stop);
    	
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
		@tfield start start
		@tfield stop stop

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
		Start a new jihad.
		@function jihadStruct:start
		@tparam settlementStruct target
		@tparam int timeToJoin (Game uses 10).
		@tparam character caller
		@usage
			    jihad:start(someSett, 10, someChar);
		*/
    	typeAll.jihadStruct.set_function("start", &jihad::start);

    	/***
		Stop an ongoing jihad.
		@function jihadStruct:stop
		@tparam int result none = 0, success = 1, failure = 2, cancelled = 3
		@usage
			    jihad:stop(3);
		*/
    	typeAll.jihadStruct.set_function("stop", &jihad::stop);
    	
    	/***
		Basic crusadeReward table.

		@tfield int money
		@tfield int experience

		@table crusadeReward
		*/
		typeAll.crusadeReward = luaState.new_usertype<crusadeReward>("crusadeReward");
    	typeAll.crusadeReward.set("money", &crusadeReward::money);
    	typeAll.crusadeReward.set("experience", &crusadeReward::experience);
    	
		///Mercenaries
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
		local map = M2TW.stratMap;
		local region = map.getRegion(2);
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
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local mercpool = region.mercPool
		local mercUnit = mercpool:addMercUnit(1907, 0, 570, 0.15, 0.35, 3, 3, 0, 0, 0)
		*/
		typeAll.mercPool.set_function("addMercUnit", &mercPool::addMercUnit);

		/***
		Get a mercenary unit from a pool by index.
		@function mercPool:getMercUnit
		@tparam int idx
		@treturn mercPoolUnit mercUnit
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local mercpool = region.mercPool
		local mercNum = mercPool:getMercUnitNum()
		for i = 0, mercNum-1 do
			local mercUnit = mercpool:getMercUnit(i)
		end
		*/
		typeAll.mercPool.set_function("getMercUnit", &mercPool::getMercUnit);
		
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
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local mercpool = region.mercPool
		local mercNum = mercPool:getMercUnitNum()
		for i = 0, mercNum-1 do
			local mercUnit = mercPool:getMercUnit(i)
			mercUnit:setMercReligion(3, true)
		end
		*/
		typeAll.mercPoolUnit.set_function("setMercReligion", &mercPoolUnit::setMercReligion);
	
		///Watchtower
		//@section Watchtower

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

			
		///Port
		//@section Port

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

		///Trade Resource
		//@section Trade Resource

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
		@tparam int modelId Added with M2TWEOP.addModelToGame
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
		
		///Campaign Difficulty Modifiers
		//@section campaignDifficulty1

		/***
		Basic campaignDifficulty1 table.

		@tfield int orderFromGrowth
		@tfield int considerWarWithPlayer
		@tfield float brigandChanceAi
		@tfield float brigandChancePlayer
		@tfield int forceAttackDelay
		@tfield float taxIncomeModifierPlayer
		@tfield float farmingIncomeModifierPlayer
		@tfield float incomeModifierAi
		@tfield float playerRegionValueModifier

		@table campaignDifficulty1
		*/

		typeAll.campaignDifficulty1 = luaState.new_usertype<campaignDifficulty1>("campaignDifficulty1");
		typeAll.campaignDifficulty1.set("orderFromGrowth", &campaignDifficulty1::orderFromGrowth);
		typeAll.campaignDifficulty1.set("considerWarWithPlayer", &campaignDifficulty1::considerWarWithPlayer);
		typeAll.campaignDifficulty1.set("brigandChanceAi", &campaignDifficulty1::brigandChanceAi);
		typeAll.campaignDifficulty1.set("brigandChancePlayer", &campaignDifficulty1::brigandChancePlayer);
		typeAll.campaignDifficulty1.set("forceAttackDelay", &campaignDifficulty1::forceAttackDelay);
		typeAll.campaignDifficulty1.set("taxIncomeModifierPlayer", &campaignDifficulty1::taxIncomeModifierPlayer);
		typeAll.campaignDifficulty1.set("farmingIncomeModifierPlayer", &campaignDifficulty1::farmingIncomeModifierPlayer);
		typeAll.campaignDifficulty1.set("incomeModifierAi", &campaignDifficulty1::incomeModifierAi);
		typeAll.campaignDifficulty1.set("playerRegionValueModifier", &campaignDifficulty1::playerRegionValueModifier);

		/***
		Basic campaignDifficulty2 table.

		@tfield int popGrowthBonusAi
		@tfield int publicOrderBonusAi
		@tfield int experienceBonusAi
		@tfield int incomeBonusAi
		@tfield int wantsTargetPlayer
		@tfield int wantsTargetPlayerNaval
		@tfield int autoAttackPlayerIfCrusadeTarget

		@table campaignDifficulty2
		*/

		typeAll.campaignDifficulty2 = luaState.new_usertype<campaignDifficulty2>("campaignDifficulty2");
		typeAll.campaignDifficulty2.set("popGrowthBonusAi", &campaignDifficulty2::popGrowthBonusAi);
		typeAll.campaignDifficulty2.set("publicOrderBonusAi", &campaignDifficulty2::publicOrderBonusAi);
		typeAll.campaignDifficulty2.set("experienceBonusAi", &campaignDifficulty2::experienceBonusAi);
		typeAll.campaignDifficulty2.set("incomeBonusAi", &campaignDifficulty2::incomeBonusAi);
		typeAll.campaignDifficulty2.set("wantsTargetPlayer", &campaignDifficulty2::wantsTargetPlayer);
		typeAll.campaignDifficulty2.set("wantsTargetPlayerNaval", &campaignDifficulty2::wantsTargetPlayerNaval);
		typeAll.campaignDifficulty2.set("autoAttackPlayerIfCrusadeTarget", &campaignDifficulty2::autoAttackPlayerIfCrusadeTarget);
		
		///Selection Info
		//@section Selection Info

		/***
		Basic selectionInfo table
	
		@tfield character selectedCharacter (Get only)
		@tfield character hoveredCharacter (Get only)
		@tfield character selectedEnemyCharacter You can only select non-player characters with zoom to location button (Get only)
		@tfield settlementStruct selectedSettlement (Get only)
		@tfield settlementStruct hoveredSettlement (Get only)
		@tfield settlementStruct selectedEnemySettlement You can only select non-player settlements with zoom to location button (Get only)
		@tfield fortStruct selectedFort (Get only)
		@tfield fortStruct hoveredFort (Get only)
		@tfield fortStruct selectedEnemyFort You can only select non-player forts with zoom to location button (Get only)
	
		@table selectionInfo
		*/
		typeAll.selectionInfo = luaState.new_usertype<selectionInfo>("selectionInfo");
		typeAll.selectionInfo.set("selectedCharacter", sol::property(&selectionInfo::getSelectedCharacter));
		typeAll.selectionInfo.set("hoveredCharacter", sol::property(&selectionInfo::getHoveredCharacter));
		typeAll.selectionInfo.set("selectedEnemyCharacter", sol::property(&selectionInfo::getSelectedEnemyCharacter));
		typeAll.selectionInfo.set("selectedSettlement", sol::property(&selectionInfo::getSelectedSettlement));
		typeAll.selectionInfo.set("hoveredSettlement", sol::property(&selectionInfo::getHoveredSettlement));
		typeAll.selectionInfo.set("selectedEnemySettlement", sol::property(&selectionInfo::getSelectedEnemySettlement));
		typeAll.selectionInfo.set("selectedFort", sol::property(&selectionInfo::getSelectedFort));
		typeAll.selectionInfo.set("hoveredFort", sol::property(&selectionInfo::getHoveredFort));
		typeAll.selectionInfo.set("selectedEnemyFort", sol::property(&selectionInfo::getSelectedEnemyFort));

		///Character Movement
		//@Character Movement

		/***
		Basic characterMovementExtents table
	
		@tfield int searchType (only get)
		@tfield int totalTiles
		@tfield int xCoordMin
		@tfield int yCoordMin
		@tfield int xCoordMax
		@tfield int yCoordMax
		@tfield int turns
		@tfield character character
		@tfield getTile getTile
	
		@table characterMovementExtents
		*/
		typeAll.characterMovementExtents = luaState.new_usertype<characterMovementExtents>("characterMovementExtents");
		typeAll.characterMovementExtents.set("searchType", &characterMovementExtents::searchType);
		typeAll.characterMovementExtents.set("totalTiles", &characterMovementExtents::totalTiles);
		typeAll.characterMovementExtents.set("xCoordMin", &characterMovementExtents::xCoordMin);
		typeAll.characterMovementExtents.set("yCoordMin", &characterMovementExtents::yCoordMin);
		typeAll.characterMovementExtents.set("xCoordMax", &characterMovementExtents::xCoordMax);
		typeAll.characterMovementExtents.set("yCoordMax", &characterMovementExtents::yCoordMax);
		typeAll.characterMovementExtents.set("turns", &characterMovementExtents::turns);
		typeAll.characterMovementExtents.set("character", &characterMovementExtents::character);
		
		/***
		Get a movement extent tile.
		@function characterMovementExtents:getTile
		@tparam int xCoord
		@tparam int yCoord
		@treturn movementExtentTile extentTile
		@usage
		     local extentTile = extents:getTile(234, 245)
		*/
		typeAll.characterMovementExtents.set_function("getTile", &characterMovementExtents::getTile);

		/***
		Basic movementExtentTile table
	
		@tfield tileStruct tile
		@tfield float moveCost
		@tfield int turns
	
		@table movementExtentTile
		*/
		typeAll.movementExtentTile = luaState.new_usertype<movementExtentTile>("movementExtentTile");
		typeAll.movementExtentTile.set("tile", sol::property(&movementExtentTile::getTile));
		typeAll.movementExtentTile.set("moveCost", &movementExtentTile::movePoints);
		typeAll.movementExtentTile.set("turns", &movementExtentTile::turns);

		/***
		Basic characterMoveData table
	
		@tfield character character
		@tfield int xCoord
		@tfield int yCoord
		@tfield int searchType
		@tfield int turns
		@tfield int settlementCount
		@tfield int fortCount
		@tfield int tileCount
		@tfield int armyCount
		@tfield int resourceCount
		@tfield int characterCount
		@tfield characterMovementExtents moveExtents
		@tfield getSettlement getSettlement
		@tfield getCharacter getCharacter
		@tfield getArmy getArmy
		@tfield getResource getResource
		@tfield getFort getFort
		@tfield getTile getTile
		@tfield sortSettlementsDistance sortSettlementsDistance
		@tfield sortSettlementsStrength sortSettlementsStrength
		@tfield sortArmiesDistance sortArmiesDistance
		@tfield sortArmiesStrength sortArmiesStrength
		@tfield sortFortsDistance sortFortsDistance
		@tfield sortFortsStrength sortFortsStrength
	
		@table characterMoveData
		*/
		typeAll.characterMoveData = luaState.new_usertype<characterMoveData>("characterMoveData");
		typeAll.characterMoveData.set("character", &characterMoveData::thisChar);
		typeAll.characterMoveData.set("xCoord", &characterMoveData::xCoord);
		typeAll.characterMoveData.set("yCoord", &characterMoveData::yCoord);
		typeAll.characterMoveData.set("searchType", &characterMoveData::searchType);
		typeAll.characterMoveData.set("turns", &characterMoveData::turns);
		typeAll.characterMoveData.set("moveExtents", &characterMoveData::moveExtents);
		typeAll.characterMoveData.set("settlementCount", sol::property(&characterMoveData::getSettlementCount));
		typeAll.characterMoveData.set("fortCount", sol::property(&characterMoveData::getFortCount));
		typeAll.characterMoveData.set("tileCount", sol::property(&characterMoveData::getTileCount));
		typeAll.characterMoveData.set("armyCount", sol::property(&characterMoveData::getArmyCount));
		typeAll.characterMoveData.set("resourceCount", sol::property(&characterMoveData::getResourceCount));
		typeAll.characterMoveData.set("characterCount", sol::property(&characterMoveData::getCharacterCount));
		
		/***
		Get a settlement.
		@function characterMoveData:getSettlement
		@tparam int index
		@treturn moveDataSettlement settlement
		@usage
			 local settlement = moveData:getSettlement(0)
		*/
		typeAll.characterMoveData.set_function("getSettlement", &characterMoveData::getSettlement);
		
		/***
		Get a character.
		@function characterMoveData:getCharacter
		@tparam int index
		@treturn moveDataCharacter foundChar
		@usage
			 local foundChar = moveData:getCharacter(0)
		*/
		typeAll.characterMoveData.set_function("getCharacter", &characterMoveData::getCharacter);
		
		/***
		Get an army.
		@function characterMoveData:getArmy
		@tparam int index
		@treturn moveDataArmy army
		@usage
			 local army = moveData:getArmy(0)
		*/
		typeAll.characterMoveData.set_function("getArmy", &characterMoveData::getArmy);
		
		/***
		Get a resource.
		@function characterMoveData:getResource
		@tparam int index
		@treturn moveDataResource resource
		@usage
			 local resource = moveData:getResource(0)
		*/
		typeAll.characterMoveData.set_function("getResource", &characterMoveData::getResource);
		
		/***
		Get a fort.
		@function characterMoveData:getFort
		@tparam int index
		@treturn moveDataFort fort
		@usage
			 local fort = moveData:getFort(0)
		*/
		typeAll.characterMoveData.set_function("getFort", &characterMoveData::getFort);
		
		/***
		Get a tile.
		@function characterMoveData:getTile
		@tparam int index
		@treturn moveDataTile tile
		@usage
			 local tile = moveData:getTile(0)
		*/
		typeAll.characterMoveData.set_function("getTile", &characterMoveData::getTile);
		
		/***
		Sort settlements by distance (already sorted by default).
		@function characterMoveData:sortSettlementsDistance
		@usage
			 moveData:sortSettlementsDistance()
		*/
		typeAll.characterMoveData.set_function("sortSettlementsDistance", &characterMoveData::sortSettlementsDistance);
		
		/***
		Sort settlements by strength.
		@function characterMoveData:sortSettlementsStrength
		@usage
			 moveData:sortSettlementsStrength()
		*/
		typeAll.characterMoveData.set_function("sortSettlementsStrength", &characterMoveData::sortSettlementsStrength);
		
		/***
		Sort armies by distance (already sorted by default).
		@function characterMoveData:sortArmiesDistance
		@usage
			 moveData:sortArmiesDistance()
		*/
		typeAll.characterMoveData.set_function("sortArmiesDistance", &characterMoveData::sortArmiesDistance);
		
		/***
		Sort armies by strength.
		@function characterMoveData:sortArmiesStrength
		@usage
			 moveData:sortArmiesStrength()
		*/
		typeAll.characterMoveData.set_function("sortArmiesStrength", &characterMoveData::sortArmiesStrength);
		
		/***
		Sort forts by distance (already sorted by default).
		@function characterMoveData:sortFortsDistance
		@usage
			 moveData:sortFortsDistance()
		*/
		typeAll.characterMoveData.set_function("sortFortsDistance", &characterMoveData::sortFortsDistance);
		
		/***
		Sort forts by strength.
		@function characterMoveData:sortFortsStrength
		@usage
			 moveData:sortFortsStrength()
		*/
		typeAll.characterMoveData.set_function("sortFortsStrength", &characterMoveData::sortFortsStrength);

		/***
		Basic moveDataSettlement table
	
		@tfield settlementStruct settlement
		@tfield float moveCost
		@tfield int turns
		@tfield bool ownFaction
	
		@table moveDataSettlement
		*/
		typeAll.moveDataSettlement = luaState.new_usertype<moveDataSettlement>("moveDataSettlement");
		typeAll.moveDataSettlement.set("settlement", &moveDataSettlement::settlement);
		typeAll.moveDataSettlement.set("moveCost", &moveDataSettlement::moveCost);
		typeAll.moveDataSettlement.set("turns", &moveDataSettlement::turns);
		typeAll.moveDataSettlement.set("ownFaction", &moveDataSettlement::ownFaction);

		/***
		Basic moveDataCharacter table
	
		@tfield character character
		@tfield float moveCost
		@tfield int turns
		@tfield bool ownFaction
	
		@table moveDataCharacter
		*/
		typeAll.moveDataCharacter = luaState.new_usertype<moveDataCharacter>("moveDataCharacter");
		typeAll.moveDataCharacter.set("character", &moveDataCharacter::thisChar);
		typeAll.moveDataCharacter.set("moveCost", &moveDataCharacter::moveCost);
		typeAll.moveDataCharacter.set("turns", &moveDataCharacter::turns);
		typeAll.moveDataCharacter.set("ownFaction", &moveDataCharacter::ownFaction);

		/***
		Basic moveDataArmy table
	
		@tfield armyStruct army
		@tfield float moveCost
		@tfield int turns
		@tfield bool ownFaction
	
		@table moveDataArmy
		*/
		typeAll.moveDataArmy = luaState.new_usertype<moveDataArmy>("moveDataArmy");
		typeAll.moveDataArmy.set("army", &moveDataArmy::army);
		typeAll.moveDataArmy.set("moveCost", &moveDataArmy::moveCost);
		typeAll.moveDataArmy.set("turns", &moveDataArmy::turns);
		typeAll.moveDataArmy.set("ownFaction", &moveDataArmy::ownFaction);

		/***
		Basic moveDataResource table
	
		@tfield tradeResource resource
		@tfield float moveCost
		@tfield int turns
		@tfield bool ownFaction
	
		@table moveDataResource
		*/
		typeAll.moveDataResource = luaState.new_usertype<moveDataResource>("moveDataResource");
		typeAll.moveDataResource.set("resource", &moveDataResource::resource);
		typeAll.moveDataResource.set("moveCost", &moveDataResource::moveCost);
		typeAll.moveDataResource.set("turns", &moveDataResource::turns);
		typeAll.moveDataResource.set("ownFaction", &moveDataResource::ownFaction);

		/***
		Basic moveDataFort table
	
		@tfield fortStruct fort
		@tfield float moveCost
		@tfield int turns
		@tfield bool ownFaction
	
		@table moveDataFort
		*/
		typeAll.moveDataFort = luaState.new_usertype<moveDataFort>("moveDataFort");
		typeAll.moveDataFort.set("fort", &moveDataFort::fort);
		typeAll.moveDataFort.set("moveCost", &moveDataFort::moveCost);
		typeAll.moveDataFort.set("turns", &moveDataFort::turns);
		typeAll.moveDataFort.set("ownFaction", &moveDataFort::ownFaction);

		/***
		Basic moveDataTile table
	
		@tfield tileStruct tile
		@tfield float moveCost
		@tfield int turns
		@tfield int xCoord
		@tfield int yCoord
	
		@table moveDataTile
		*/
		typeAll.moveDataTile = luaState.new_usertype<moveDataTile>("moveDataTile");
		typeAll.moveDataTile.set("tile", &moveDataTile::tile);
		typeAll.moveDataTile.set("moveCost", &moveDataTile::moveCost);
		typeAll.moveDataTile.set("turns", &moveDataTile::turns);
		typeAll.moveDataTile.set("xCoord", &moveDataTile::xCoord);
		typeAll.moveDataTile.set("yCoord", &moveDataTile::yCoord);
		
    }
};
