#include "gameHelpers.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include <vector>

#include "eduThings.h"
#include "FastFuncts.h"
#include "smallFuncs.h"
#include "technicalHelpers.h"
#include "character.h"

namespace gameHelpers
{
	using namespace std;

	UINT32 gameHelpers::getFactionsCount()
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		const campaign* campaign = gameData->campaignData;
		if (!campaign)
			return 0;
		return campaign->numberOfFactionsWithSlave;
	}

	std::string gameHelpers::callConsole(const std::string& cmdName, sol::variadic_args va)
	{
		char buffer[100]{};
		buffer[0] = '\0';
		bool isOk = false;
		if (va.size() == 0)
		{
			fastFuncts::callGameConsoleCommand(cmdName.c_str(), "", buffer);
		}
		else
		{
			fastFuncts::callGameConsoleCommand(cmdName.c_str(), va.begin()->as<std::string>().c_str(), buffer);
		}

		if (isOk == false && buffer[0] == '\0')
		{
			sprintf_s(buffer, "error");
		}
		return buffer;;
	}

	factionStruct* gameHelpers::getFaction(const int index)
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		const campaign* campaign = gameData->campaignData;
		if (!campaign || index < 0 || index >= campaign->numberOfFactionsWithSlave)
			return nullptr;
		return campaign->factionsSortedByDescrStrat[index];
	}


	std::tuple<bool, int> gameHelpers::getScriptCounter(const char* type)
	{
		bool isExist = false;
		int counterValue = smallFuncs::getScriptCounter(type, isExist);

		return std::make_tuple(isExist, counterValue);
	}

	regionStruct* gameHelpers::getRegion(const int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		return &map->regions[index];
	}

	oneTile* gameHelpers::getTile(const int x, const int y)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		return &map->tilesArr[gameDataAll->stratMap->mapWidth * y + x];
	}

	stackStruct* gameHelpers::getStack(const regionStruct* region, const int index)
	{
		return region->armies[index];
	}

	fortStruct* gameHelpers::getFort(const regionStruct* region, const int index)
	{
		return region->forts[index];
	}

	campaignDb* getCampaignDb()
	{
		DWORD offset = 0x0161E7E4;
		if (smallFuncs::getGameVersion() == 1)
			offset = 0x16666BC;
		return reinterpret_cast<campaignDb*>(offset);
	}

	campaignDbExtra* getCampaignDbExtra()
	{
		DWORD offset = 0x0186170C;
		if (smallFuncs::getGameVersion() == 1)
			offset = 0x18AA87C;
		return reinterpret_cast<campaignDbExtra*>(offset);
	}

	watchTowerStruct* gameHelpers::getWatchtower(const regionStruct* region, const int index)
	{
		return region->watchtowers[index];
	}

	resStrat* gameHelpers::getResource(const regionStruct* region, const int index)
	{
		return region->resources[index];
	}

	neighbourRegion* gameHelpers::getNeighbour(regionStruct* region, const int index)
	{
		return &region->neighbourRegions[index];
	}

	//1 never impelemnted

	unit* getEventAttackingUnit(eventTrigger* eventData)
	{
		return CallVFunc<2, unit*>(eventData);
	}

	unit* getEventDefendingUnit(eventTrigger* eventData)
	{
		return CallVFunc<3, unit*>(eventData);
	}

	character* getEventCharacter(eventTrigger* eventData)
	{
		return CallVFunc<4, character*>(eventData);
	}

	characterRecord* getEventNamedCharacter(eventTrigger* eventData)
	{
		return CallVFunc<5, characterRecord*>(eventData);
	}

	characterRecord* getEventNamedCharacter2(eventTrigger* eventData)
	{
		return CallVFunc<6, characterRecord*>(eventData);
	}

	//7 status thing

	characterRecord* getEventTargetNamedCharacter(eventTrigger* eventData)
	{
		return CallVFunc<8, characterRecord*>(eventData);
	}

	settlementStruct* getEventSettlement(eventTrigger* eventData)
	{
		return CallVFunc<9, settlementStruct*>(eventData);
	}

	settlementStruct* getEventTargetSettlement(eventTrigger* eventData)
	{
		return CallVFunc<10, settlementStruct*>(eventData);
	}

	fortStruct* getEventFort(eventTrigger* eventData)
	{
		return CallVFunc<11, fortStruct*>(eventData);
	}

	factionStruct* getEventFaction(eventTrigger* eventData)
	{
		return CallVFunc<12, factionStruct*>(eventData);
	}

	factionStruct* getEventTargetFaction(eventTrigger* eventData)
	{
		return CallVFunc<13, factionStruct*>(eventData);
	}

	stackStruct* getEventArmy(eventTrigger* eventData)
	{
		return CallVFunc<14, stackStruct*>(eventData);
	}

	int getEventRegion(eventTrigger* eventData)
	{
		return CallVFunc<15, int>(eventData);
	}

	int getEventTargetRegion(eventTrigger* eventData)
	{
		return CallVFunc<16, int>(eventData);
	}

	unit* getEventPlayerUnit(eventTrigger* eventData)
	{
		return CallVFunc<17, unit*>(eventData);
	}

	unit* getEventEnemyUnit(eventTrigger* eventData)
	{
		return CallVFunc<18, unit*>(eventData);
	}

	//19 building_battle
	buildingBattle* getBuildingBattle(eventTrigger* eventData)
	{
		return CallVFunc<19, buildingBattle*>(eventData);
	}

	//20 prior_build
	buildingInQueue* getPriorBuild(eventTrigger* eventData)
	{
		return CallVFunc<20, buildingInQueue*>(eventData);
	}

	//21 advised_build

	//22 best_finance_option

	//23 resource_description
	const char* getResourceDescription(eventTrigger* eventData)
	{
		return *CallVFunc<23, const char**>(eventData);
	}

	eduEntry* getEduEntry(eventTrigger* eventData)
	{
		return CallVFunc<23, eduEntry*>(eventData);
	}

	int getEventCharacterType(eventTrigger* eventData)
	{
		return CallVFunc<24, int>(eventData);
	}

	int getEventTargetCharacterType(eventTrigger* eventData)
	{
		return CallVFunc<25, int>(eventData);
	}

	std::unordered_map<int, const char*> disasterTypes = {
		{0,"earthquake"},
		{1,"flood"},
		{2,"horde"},
		{3,"storm"},
		{4,"volcano"},
		{5,"dustbowl"},
		{6,"locusts"},
		{7,"famine"},
		{8,"plague"},
		{9,"riot"},
		{10,"fire"}
	};
	std::unordered_map<int, const char*> ransomTypes = {
		{0,"ransom"},
		{1,"execute"},
		{2,"release"},
		{3,"cannot_pay_ransom"}
	};
	std::unordered_map<int, const char*> missionSuccessLvl = {
		{0,"not_successful"},
		{1,"slightly_successful"},
		{2,"partly_successful"},
		{3,"highly_successful"}
	};

	const char* getEventType(eventTrigger* eventData)
	{
		auto eventType = "unknown";
		auto eventNumber = CallVFunc<26, int>(eventData);
		if (disasterTypes.find(eventNumber) != disasterTypes.end()) {
			eventType = disasterTypes.at(eventNumber);
		}
		return eventType;
	}

	const char* getMissionSuccessLevel(eventTrigger* eventData)
	{
		auto level = "unknown";
		auto levelNumber = CallVFunc<27, int>(eventData);
		if (disasterTypes.find(levelNumber) != disasterTypes.end()) {
			level = disasterTypes.at(levelNumber);
		}
		return level;
	}

	int getMissionProbability(eventTrigger* eventData)
	{
		return CallVFunc<28, int>(eventData);;
	}

	//29 mission_details
	const char* getMissionDetails(eventTrigger* eventData)
	{
		return *CallVFunc<29, const char**>(eventData);
	}

	//30 character_action_advice

	//31 event
	int getEventID(eventTrigger* eventData)
	{
		return CallVFunc<31, int>(eventData);;
	}

	//32 unk

	//33 unk2

	//34 guild handler
	int getEventGuild(eventTrigger* eventData)
	{
		return CallVFunc<34, int>(eventData);
	}

	//35 event_counter
	const char* getEventCounter(eventTrigger* eventData)
	{
		return *CallVFunc<35, const char**>(eventData);
	}

	//36 sm_position
	coordPair* getPosition(eventTrigger* eventData)
	{
		return CallVFunc<36, coordPair*>(eventData);
	}

	//37 get religion
	int getReligion(eventTrigger* eventData)
	{
		return CallVFunc<37, int>(eventData);;
	}

	//38 get target religion
	int getTargetReligion(eventTrigger* eventData)
	{
		return CallVFunc<38, int>(eventData);;
	}

	//39 amount
	float getAmount(eventTrigger* eventData)
	{
		return CallVFunc<39, float>(eventData);;
	}

	//40 crusade
	crusade* getCrusade(eventTrigger* eventData)
	{
		return CallVFunc<40, crusade*>(eventData);;
	}

	//41 captured faction info
	capturedFactionInfo* getCapturedFactionInfo(eventTrigger* eventData)
	{
		return CallVFunc<41, capturedFactionInfo*>(eventData);;
	}

	//42 ransom type
	const char* getRansomType(eventTrigger* eventData)
	{
		auto ransomType = "unknown";
		auto ransom = CallVFunc<42, int>(eventData);
		if (ransomTypes.find(ransom) != ransomTypes.end()) {
			ransomType = ransomTypes.at(ransom);
		}
		return ransomType;
	}

	//43 unk

	//44 unk

	//45 unit
	unit* getUnit(eventTrigger* eventData)
	{
		return CallVFunc<45, unit*>(eventData);;
	}
	
	bool condition(std::string condition, const eventTrigger* eventData)
	{
		const char* conditionC = condition.c_str();
		return smallFuncs::condition(conditionC, eventData);
	}


	bool gameHelpers::getHiddenResource(regionStruct* region, int index)
	{
		int resources;
		int set = 1 << index;
		if (index < 32)
		{
			resources = region->hiddenResources1;
		}
		else if (index < 64) {
			index = index - 32;
			set = 1 << index;
			resources = region->hiddenResources2;
		}
		else {
			return false;
		}
		return resources & set;
	}

	void gameHelpers::setHiddenResource(regionStruct* region, int index, const bool enable)
	{

		if (index < 32)
		{
			int set = 1 << index;
			if (enable == true)
			{
				region->hiddenResources1 |= (set);
			}
			else
			{
				region->hiddenResources1 &= ~set;
			}
		}
		else if (index < 64) {
			index = index - 32;
			int set = 1 << index;
			if (enable == true)
			{
				region->hiddenResources2 |= (set);
			}
			else
			{
				region->hiddenResources2 &= ~set;
			}
		}
	}

	std::string gameHelpers::getRegionName(regionStruct* region)
	{
		return technicalHelpers::uniStringToStr(region->localizedRegionName);
	}

	std::string gameHelpers::getRebelsName(regionStruct* region)
	{
		return technicalHelpers::uniStringToStr(region->localizedRebelsName);
	}

	std::string gameHelpers::getCampaignPath(campaign* campaign)
	{
		return technicalHelpers::uniStringToStr(campaign->currentDescrFile);
	}

	int gameHelpers::getMercUnitNum(mercPool* mercPool)
	{
		mercPoolUnitsPtr* unitsPtr = &mercPool->firstUnits;
		int units = 0;
		while (unitsPtr != nullptr)
		{
			units += unitsPtr->currentPool;
			unitsPtr = unitsPtr->nextUnitsPtr;
		}
		return units;
	}

	roadStruct* getRoad(const campaign* campaign, const int index)
	{
		return campaign->roads[index];
	}

	int getPoolIndex(mercPoolUnitsPtr *unitPtr)
	{
		auto nextUnitsPtr = unitPtr->nextUnitsPtr;
		if ( nextUnitsPtr && nextUnitsPtr->currentPool )
			return unitPtr->currentPool + getPoolIndex(nextUnitsPtr);
		return unitPtr->currentPool;
	}

	mercPool* getMercPool(const campaign* campaign, const std::string& name)
	{
		auto mercPools = &campaign->allMercPools;
		if (!mercPools || !mercPools->currentCount)
			return nullptr;
		while (mercPools)
		{
			for (int i = 0; i < mercPools->currentCount; i++)
			{
				if (strcmp(mercPools->mercPools[i].name, name.c_str()) == 0)
					return &mercPools->mercPools[i];
			}
			mercPools = mercPools->nextMercPools;
		}
		return nullptr;
	}

	mercPoolUnit* getNewMercUnit(mercPoolUnitsPtr* unitPtr)
	{
		const auto currentPool = unitPtr->currentPool;
		if (currentPool >= unitPtr->Maxpool)
		{
			if (!unitPtr->nextUnitsPtr)
			{
				const auto newPtr = reinterpret_cast<mercPoolUnitsPtr*>(fastFuncts::allocateGameMem(0x14));
				newPtr->Maxpool = unitPtr->Maxpool * 2;
				newPtr->currentPool = 0;
				newPtr->mercPoolUnits = reinterpret_cast<mercPoolUnit*>(fastFuncts::allocateGameMem(0x50 * newPtr->Maxpool));
				unitPtr->nextUnitsPtr = newPtr;
				newPtr->prevPoolUnits = unitPtr->mercPoolUnits;
			}
			return getNewMercUnit(unitPtr->nextUnitsPtr);
		}
		const auto unit = &unitPtr->mercPoolUnits[currentPool];
		unitPtr->currentPool++;
		unit->eduEntry = nullptr;
		unit->experience = 0;
		unit->replenishMin = 0;
		unit->replenishMax = 0;
		unit->maxUnits = 0;
		unit->currentPool = 0;
		unit->startYear = 0;
		unit->endYear = 0;
		unit->religionsList = nullptr;
		unit->religionsListEnd = nullptr;
		unit->religionsListEnd2 = nullptr;
		unit->crusading = 0;
		unit->eventsList = nullptr;
		unit->eventsListEnd = nullptr;
		unit->eventsListEnd2 = nullptr;
		unit->poolIndex = 0;
		unit->mercPoolUnitIndex = 0;
		unit->mercPool = nullptr;
		return unit;
	}

	mercPoolUnit* gameHelpers::addMercUnit(mercPool* mercPool, const int idx, const int exp, const int cost, const float repmin, const float repmax, const int maxunits, const float startpool, const float startyear, const float endyear, const int crusading)
	{
		int16_t mercPoolUnitIndex = mercPool->firstUnits.currentPool;
		int16_t poolIndex = 0;
		const auto unit = &mercPool->firstUnits.mercPoolUnits[0];
		if (!unit)
		{
			const auto campaign = gameDataAllHelper::get()->campaignData;
			auto mercPools = &campaign->allMercPools;
			while (mercPools)
			{
				for (int i = 0; i < mercPools->currentCount; i++)
				{
					if (strcmp(mercPools->mercPools[i].name , mercPool->name) == 0)
						break;
					poolIndex++;
				}
				mercPools = mercPools->nextMercPools;
			}
		}
		else
			poolIndex = unit->poolIndex;
		const auto nextUnitsPtr = mercPool->firstUnits.nextUnitsPtr;
		if ( nextUnitsPtr && nextUnitsPtr->currentPool )
			mercPoolUnitIndex = mercPool->firstUnits.currentPool + getPoolIndex(nextUnitsPtr);
		const auto newMerc = getNewMercUnit(&mercPool->firstUnits);
		newMerc->mercPoolUnitIndex = mercPoolUnitIndex;
		newMerc->poolIndex = poolIndex;
		newMerc->mercPool = mercPool;
		eduEntry* entry = eduThings::getEduEntry(idx);
		newMerc->eduEntry = entry;
		newMerc->experience = exp;
		newMerc->cost = cost;
		newMerc->replenishMin = repmin;
		newMerc->replenishMax = repmax;
		newMerc->maxUnits = maxunits;
		newMerc->currentPool = startpool;
		newMerc->startYear = startyear;
		newMerc->endYear = endyear;
		newMerc->crusading = crusading;
		return newMerc;
	}

	mercPoolUnit* gameHelpers::getMercUnit(const mercPool* pool, const int index)
	{
		const mercPoolUnitsPtr* poolUnitsPtr = &pool->firstUnits;
		while (poolUnitsPtr != nullptr)
		{
			const int unitCount = poolUnitsPtr->currentPool;
			for (int i = 0; i < unitCount; i++)
			{
				if (&poolUnitsPtr->mercPoolUnits[i] != nullptr && poolUnitsPtr->mercPoolUnits[i].mercPoolUnitIndex == index)
				{
					return &poolUnitsPtr->mercPoolUnits[i];
				}
			}
			poolUnitsPtr = poolUnitsPtr->nextUnitsPtr;
		}
		return nullptr;
	}


	void gameHelpers::setMercReligion(mercPoolUnit* unit, const int religion, const bool set)
	{
		vector<int>religions;
		bool relFound = false;
		int mercRelNum = (unit->religionsListEnd - unit->religionsList);
		for (int i = 0; i < mercRelNum; i++)
		{
			if (unit->religionsList[i] == religion)
			{
				if (set)
					return;
				relFound = true;
			}
			else
				religions.push_back(unit->religionsList[i]);
		}
		if (relFound == false && set == false)
			return;
		if (set)
		{
			religions.push_back(religion);
			mercRelNum++;
		}
		else
			mercRelNum--;
		const auto newList = reinterpret_cast<int*>(fastFuncts::allocateGameMem(sizeof(int) * mercRelNum));
		unit->religionsList = newList;
		for (int i = 0; i < mercRelNum; i++)
			unit->religionsList[i] = religions[i];
		unit->religionsListEnd = &unit->religionsList[mercRelNum];
		unit->religionsListEnd2 = &unit->religionsList[mercRelNum];
	}

	void gameHelpers::scriptCommand(std::string command, sol::variadic_args va)
	{
		const char* commandC = command.c_str();
		if (va.size() == 0)
		{
			smallFuncs::scriptCommand(commandC, "");
		}
		else
		{
			smallFuncs::scriptCommand(commandC, va.begin()->as<std::string>().c_str());
		}
	}

	character* gameHelpers::getCardinal(const collegeOfCardinals* college, const int index)
	{
		return college->cardinalsArray->cardinals[index].trackedCharacter->character;
	}

	fortStruct* gameHelpers::getFortAll(const campaign* campaign, const int index)
	{
		return campaign->fortsArray[index];
	}

	portBuildingStruct* gameHelpers::getPortAll(const campaign* campaign, const int index)
	{
		return campaign->portsBuildings[index];
	}

	watchTowerStruct* gameHelpers::getWatchTowerAll(const campaign* campaign, const int index)
	{
		return campaign->watchtowers[index];
	}

	unit* gameHelpers::getSelectedUnitCard(const uiCardManager* cardManager, const int index)
	{
		return cardManager->selectedUnitCards[index]->unit;
	}

	unit* gameHelpers::getUnitCard(const uiCardManager* cardManager, const int index)
	{
		return cardManager->unitCards[index]->unit;
	}

	coordPair* getVolcanoCoords(const stratMap* map, const int index)
	{
		return &map->volcanoes[index];
	}

	coordPair* getLandConnection(const stratMap* map, const int index)
	{
		return &map->landMassConnections[index];
	}

	landMass* getLandMass(stratMap* map, const int index)
	{
		return &map->landMass[index];
	}

	int getRegionID(const landMass* landMass, const int index)
	{
		return landMass->regions[index];
	}

	coordPair* getRoadCoord(const roadStruct* road, const int index)
	{
		return &road->coords[index];
	}

	int getTileFactionID(const oneTile* tile)
	{
		return tile->factionId & 0b00011111;
	}

	bool tileHasRiver(const oneTile* tile)
	{
		return tile->factionId & 0b01100000;
	}

	bool tileHasRiverSource(const oneTile* tile)
	{
		return tile->factionId & 0b00100000;
	}

	bool tileHasCrossing(const oneTile* tile)
	{
		return tile->factionId & 0b10000000;
	}

	bool tileHasCliff(const oneTile* tile)
	{
		return tile->factionId & ((1 << 8) | (1 << 10));
	}

	bool isLandConnection(const oneTile* tile)
	{
		return tile->factionId & (1 << 12);
	}

	bool isSeaCrossing(const oneTile* tile)
	{
		return tile->factionId & (1 << 14);
	}

	int tileRoadLevel(const oneTile* tile)
	{
		if (tile->factionId & (1 << 17))
			return 3;
		if (tile->factionId & (1 << 16))
			return 2;
		if (tile->factionId & (1 << 15))
			return 1;
		return 0;
	}

	bool isCoastalWater(const oneTile* tile)
	{
		return tile->factionId & (1 << 9);
	}

	int tileBorderType(const oneTile* tile)
	{
		return tile->border;
	}

	bool tileHasCharacter(const oneTile* tile)
	{
		return tile->objectTypes & (1 << 6);
	}

	bool tileHasShip(const oneTile* tile)
	{
		return tile->objectTypes & (1 << 5);
	}

	bool tileHasWatchtower(const oneTile* tile)
	{
		return tile->objectTypes & (1 << 4);
	}

	bool tileHasPort(const oneTile* tile)
	{
		return tile->objectTypes & (1 << 3);
	}

	bool tileHasFort(const oneTile* tile)
	{
		return tile->objectTypes & (1 << 1);
	}

	bool tileHasSettlement(const oneTile* tile)
	{
		return tile->objectTypes & (1 << 0);
	}

	bool factionHasArmyNearTile(const oneTile* tile, int factionID)
	{
		return tile->armiesNearTile & (1 << factionID);
	}

	bool factionHasCharacterOnTile(const oneTile* tile, int factionID)
	{
		return tile->charactersOnTile & (1 << factionID);
	}

	coordPair* convertTileCoords(const DWORD arrayIndex)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		const int index = arrayIndex;
		for (int y = 0;y < map->mapHeight;y++)
		{
			const int x = index - y * map->mapWidth;
			if (x >= 0 && x < map->mapWidth)
			{
				const auto coords = new coordPair({x ,y});
				coords->xCoord = x;
				coords->yCoord = y;
				return coords;
			}
		}
		return nullptr;
	}

	int getTileX(const oneTile* tile)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return -1;
		const auto coords = convertTileCoords(tile - map->tilesArr);
		const int x = coords->xCoord;
		delete coords;
		return x;
	}

	int getTileY(const oneTile* tile)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return -1;
		const auto coords = convertTileCoords(tile - map->tilesArr);
		const int y = coords->yCoord;
		delete coords;
		return y;
	}

	settlementStruct* getSettlementByName(campaign* campaign, const char* name)
	{
		const settlementList* settlementList = &campaign->settlementList;
		while (settlementList != nullptr)
		{
			for (int i = 0; i < settlementList->settlementsNum; i++)
			{
				if (strcmp(settlementList->settlements[i]->name, name) == 0)
					return settlementList->settlements[i];
			}
			settlementList = settlementList->nextSettlements;
		}
		return nullptr;
	}

	const char* getReligionName2(const int index)
	{
		const auto* religionDb = *reinterpret_cast <religionDatabase**>(0x016A0B90);
		if (smallFuncs::getGameVersion() == 1)
		{
			religionDb = *reinterpret_cast <religionDatabase**>(0x016E9DC0);
		}
		const wchar_t* name = religionDb->religionNames[index]->string;
		// Determine the size of the required buffer
		const int size = WideCharToMultiByte(CP_UTF8, 0, name, -1, nullptr, 0, nullptr, nullptr);
		if (size == 0) {
			return nullptr;
		}
		// Allocate a buffer for the converted string
		const auto buffer = new char[size];
		if (buffer == nullptr) {
			// Allocation failed
			return nullptr;
		}
		// Convert the string
		WideCharToMultiByte(CP_UTF8, 0, name, -1, buffer, size, nullptr, nullptr);
		// Return the converted string
		return buffer;
	}

	const char* getClimateName2(const int index)
	{
	    const gameDataAllStruct* gameData = gameDataAllHelper::get();
	    const auto stratMap = gameData->stratMap;
		const wchar_t* name = stratMap->climates->climateArray[index].rcString->string;
		// Determine the size of the required buffer
		const int size = WideCharToMultiByte(CP_UTF8, 0, name, -1, nullptr, 0, nullptr, nullptr);
		if (size == 0) {
			return nullptr;
		}
		// Allocate a buffer for the converted string
		const auto buffer = new char[size];
		if (buffer == nullptr) {
			// Allocation failed
			return nullptr;
		}
		// Convert the string
		WideCharToMultiByte(CP_UTF8, 0, name, -1, buffer, size, nullptr, nullptr);
		// Return the converted string
		return buffer;
	}

	const char* getReligionName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto religionName = plugData::data.luaAll.religionNames.find(index);
		if (religionName == plugData::data.luaAll.religionNames.end()) 
			return nullptr;
		return religionName->second;
	}

	const char* getClimateName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto name = plugData::data.luaAll.climateNames.find(index);
		if (name == plugData::data.luaAll.climateNames.end()) 
			return nullptr;
		return name->second;
	}

	const char* getCultureName(const int index)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto name = plugData::data.luaAll.cultureNames.find(index);
		if (name == plugData::data.luaAll.cultureNames.end()) 
			return nullptr;
		return name->second;
	}

	int getReligionCount()
	{
		const auto* religionDb = *reinterpret_cast <religionDatabase**>(0x016A0B90);
		if (smallFuncs::getGameVersion() == 1)
		{
			religionDb = *reinterpret_cast <religionDatabase**>(0x016E9DC0);
		}
		return religionDb->religionCount;
	}

	int getReligionN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto religionIndex = plugData::data.luaAll.religionIndex.find(name);
		if (religionIndex == plugData::data.luaAll.religionIndex.end()) 
			return -1;
		return religionIndex->second;
	}

	int getClimateN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto index = plugData::data.luaAll.climateIndex.find(name);
		if (index == plugData::data.luaAll.climateIndex.end()) 
			return -1;
		return index->second;
	}

	int getCultureN(const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto index = plugData::data.luaAll.cultureIndex.find(name);
		if (index == plugData::data.luaAll.cultureIndex.end()) 
			return -1;
		return index->second;
	}

	seaConnectedRegion* getSeaConnectedRegion(const regionStruct* region, const int index)
	{
		return &region->seaConnectedRegions[index];
	}

	seaConnectedRegion* getSeaImportRegion(const regionStruct* region, const int index)
	{
		return region->seaImportRegions[index];
	}

	oneTile* getRegionSeaEdge(const regionStruct* region, const int index)
	{
		if (index >= region->regionSeaEdgesCount)
			return nullptr;
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;

		return &map->tilesArr[region->regionSeaEdges[index]];
	}

	oneTile* getDevastatedTile(const regionStruct* region, const int index)
	{
		if (index >= region->devastatedTilesCount)
			return nullptr;
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;

		return &map->tilesArr[region->devastatedTiles[index]];
	}

	int getHostileArmiesStrength(const regionStruct* region, const int factionID)
	{
		return region->armiesHostileToArrayIndexIDStrength[factionID];
	}

	bool isDevastated(const oneTile* tile)
	{
		return tile->hasRoad & 1;
	}

	oneTileDouble* tileToDoubleTile(const oneTile* tile)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		const int mapWidth = map->mapWidth * 2 + 1;
		const int x = getTileX(tile) * 2;
		const int y = getTileY(tile) * 2;
		if (x < 0 || y < 0 || x >= mapWidth || y >= mapWidth)
			return nullptr;
		return &map->climateTileArray[y  * mapWidth + x];
	}

	float getTileHeight(const oneTile* tile)
	{
		const auto doubleTile = tileToDoubleTile(tile);
		if (!doubleTile)
			return 0.0f;
		return doubleTile->height;
	}

	int getTileClimate(const oneTile* tile)
	{
		const auto doubleTile = tileToDoubleTile(tile);
		if (!doubleTile)
			return 0;
		return tileToDoubleTile(tile)->climate;
	}

	void setTileHeight(const oneTile* tile, float height)
	{
		const auto doubleTile = tileToDoubleTile(tile);
		if (!doubleTile)
			return;
		doubleTile->height = height;
	}

	void setTileClimate(const oneTile* tile, int climate)
	{
		const auto doubleTile = tileToDoubleTile(tile);
		if (!doubleTile)
			return;
		doubleTile->climate = climate;
	}

	void setTileGroundType(oneTile* tile, int ground)
	{
		const auto doubleTile = tileToDoubleTile(tile);
		tile->groundType = ground;
		if (ground >= 14 || ground == 4 || ground == 5 || ground == 7 || ground >= 10 && ground < 13)
		{
			tile->nonPassable = -1;
			if (ground >= 10 && ground < 13)
				tile->isLand = 0;
		}
		if (!doubleTile)
			return;
		doubleTile->groundType = ground;
	}

	int getTileGroundType(oneTile* tile)
	{
		return tile->groundType;
	}

	int getTileHeatValue(const oneTile* tile)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return 0;
		const auto climate = getTileClimate(tile);
		return map->climates->climateArray[climate].heatValue;
	}

	float getReligionHistory(const regionStruct* region, const int religionId, const int turnsAgo)
	{
		if (turnsAgo > 19)
			return 0.0f;
		if (turnsAgo == 0)
			return region->religionsARR[religionId];
		return region->religionHistory[turnsAgo][religionId];
	}

	oneTile* getTileBorderingEdgeOfMap(const regionStruct* region, const int index)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		return &map->tilesArr[region->tilesBorderingEdgeOfMap[index]];
	}

	oneTile* getTileRegion(const regionStruct* region, const int index)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		return &map->tilesArr[region->tiles[index]];
	}

	oneTile* getFertileTile(const regionStruct* region, const int index)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		return &map->tilesArr[region->fertileTiles[index]];
	}

	oneTile* getBorderTile(const neighbourRegion* region, const int index)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		return &map->tilesArr[region->bordertiles[index]];
	}

	oneTile* getReachableTile(const seaConnectedRegion* region, const int index)
	{
		const gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		const stratMap* map = gameDataAll->stratMap;
		if (!map)
			return nullptr;
		if (!&region->tilesReachable[index])
			return nullptr;
		const auto tile = region->tilesReachable[index].tileId;
		return &map->tilesArr[tile];
	}

	coordPair* getTradeLaneCoord(const seaConnectedRegion* region, const int index)
	{
		return &region->seaTradeLanePath[index];
	}

	bool hasResourceType(const regionStruct* region, const int resourceType)
	{
		return region->resourceTypesBitMap & 1 << resourceType;
	}

	/*
		strategy map object types
		0 -> 27 = resource ID
		28 = character
		29 = settlement
		30 = fort
		31 = port
		32 = watchTower
		33 = sundry
		34 = rally point
		35 = floatingGeneral
		36 = battleSiteMarker
	*/

	DWORD* getTileObject(const oneTile* tile, const int type)
	{
		DWORD* object = tile->object;

		while (object)
		{
			const int objectType = CallVFunc<4, int>(object);
			if (objectType == type || (type == 0 && objectType < 28))
				return object;
			object = reinterpret_cast<DWORD*>(*(object + 1));  // NOLINT(performance-no-int-to-ptr)
		}

		return nullptr;
	}

	resStrat* getTileResource(const oneTile* tile)
	{
		return reinterpret_cast<resStrat*>(getTileObject(tile, 0));
	}

	character* getTileCharacter(const oneTile* tile)
	{
		return reinterpret_cast<character*>(getTileObject(tile, 28));
	}

	int getTileCharacterCount(const oneTile* tile)
	{
		int count = 0;
		const auto character = getTileCharacter(tile);
		if (character)
			count += 1;
		else
			return 0;
		auto object = static_cast<DWORD*>(character->obj);
		while (object)
		{
			const int objectType = CallVFunc<4, int>(object);
			if (objectType == 28)
				count += 1;
			else
				break;
			object = reinterpret_cast<DWORD*>(*(object + 1));  // NOLINT(performance-no-int-to-ptr)
		}
		return count;
	}

	character* getTileCharacterAtIndex(const oneTile* tile, int index)
	{
		int count = 0;
		const auto thisChar = getTileCharacter(tile);
		if (index == 0)
			return thisChar;
		if (!thisChar)
			return nullptr;
		auto object = static_cast<DWORD*>(thisChar->obj);
		while (object)
		{
			const int objectType = CallVFunc<4, int>(object);
			if (objectType == 28)
			{
				count += 1;
				if (count == index)
					return reinterpret_cast<character*>(object);
			}
			else
				return nullptr;
			object = reinterpret_cast<DWORD*>(*(object + 1));  // NOLINT(performance-no-int-to-ptr)
		}
		return nullptr;
	}

	settlementStruct* getTileSettlement(const oneTile* tile)
	{
		return reinterpret_cast<settlementStruct*>(getTileObject(tile, 29));
	}

	fortStruct* getTileFort(const oneTile* tile)
	{
		return reinterpret_cast<fortStruct*>(getTileObject(tile, 30));
	}

	portBuildingStruct* getTilePort(const oneTile* tile)
	{
		return reinterpret_cast<portBuildingStruct*>(getTileObject(tile, 31));
	}

	watchTowerStruct* getTileWatchtower(const oneTile* tile)
	{
		return reinterpret_cast<watchTowerStruct*>(getTileObject(tile, 32));
	}
	
	factionStruct* getFactionHashed(const campaign* campaign, const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto factionId = plugData::data.luaAll.factions.find(name);
		if (factionId == plugData::data.luaAll.factions.end()) 
			return nullptr;
		return campaign->factionsSortedByID[factionId->second];
	}

	settlementStruct* getSettlement(const stratMap* map, const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto regionId = plugData::data.luaAll.settlements.find(name);
		if (regionId == plugData::data.luaAll.settlements.end()) 
			return nullptr;
		return map->regions[regionId->second].settlement;
	}

	regionStruct* getRegionByName(stratMap* map, const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto regionId = plugData::data.luaAll.regions.find(name);
		if (regionId == plugData::data.luaAll.regions.end()) 
			return nullptr;
		return &map->regions[regionId->second];
	}

}
