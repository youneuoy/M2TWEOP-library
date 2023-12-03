#include "gameHelpers.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include <vector>

namespace gameHelpers
{
	using namespace std;

	UINT32 gameHelpers::getFactionsCount()
	{
		return (*(*plugData::data.funcs.getFactionsCount))();
	}

	std::string gameHelpers::callConsole(const std::string cmdName, sol::variadic_args va)
	{
		char buffer[100]{};
		buffer[0] = '\0';
		bool isOk = false;
		if (va.size() == 0)
		{
			isOk = (*(*plugData::data.funcs.callGameConsoleCommand))(cmdName.c_str(), "", buffer);
		}
		else
		{
			isOk = (*(*plugData::data.funcs.callGameConsoleCommand))(cmdName.c_str(), va.begin()->as<std::string>().c_str(), buffer);
		}

		if (isOk == false && buffer[0] == '\0')
		{
			sprintf_s(buffer, "error");
		}
		return buffer;;
	}

	factionStruct* gameHelpers::getFaction(const int index)
	{
		return (*(*plugData::data.funcs.getFactionsList))()[index];
	}

	guild* gameHelpers::getGuild(const unsigned char index)
	{
		return (*(*plugData::data.funcs.getGuild))(index);
	}

	general* gameHelpers::createCharacter(const char* type, factionStruct* fac, const int age, const char* name, const char* name2, const int subFaction, const char* portrait, const int x, const int y)
	{
		if (portrait != nullptr && strlen(portrait) == 0)
		{
			portrait = nullptr;
		}
		if (name != nullptr && strlen(name) == 0)
		{
			name = nullptr;
		}
		if (name2 != nullptr && strlen(name2) == 0)
		{
			name2 = nullptr;
		}
		return (*(*plugData::data.funcs.createCharacter))(type, fac, age, name, name2, subFaction, portrait, x, y);
	}

	stackStruct* gameHelpers::createArmy(general* character)
	{
		return (*(*plugData::data.funcs.createArmy))(character);
	}

	stackStruct* gameHelpers::createArmyInSettlement(settlementStruct* sett)
	{
		return (*(*plugData::data.funcs.createArmyInSettlement))(sett);
	}


	std::tuple<bool, int> gameHelpers::getScriptCounter(const char* type)
	{
		bool isExist = false;
		int counterValue = (*(*plugData::data.funcs.getScriptCounter))(type, isExist);

		return std::make_tuple(isExist, counterValue);
	}

	void gameHelpers::setScriptCounter(const char* type, const int value)
	{
		(*(*plugData::data.funcs.setScriptCounter))(type, value);
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

	general* getEventCharacter(eventTrigger* eventData)
	{
		return CallVFunc<4, general*>(eventData);
	}

	namedCharacter* getEventNamedCharacter(eventTrigger* eventData)
	{
		return CallVFunc<5, namedCharacter*>(eventData);
	}

	namedCharacter* getEventNamedCharacter2(eventTrigger* eventData)
	{
		return CallVFunc<6, namedCharacter*>(eventData);
	}

	//7 status thing

	namedCharacter* getEventTargetNamedCharacter(eventTrigger* eventData)
	{
		return CallVFunc<8, namedCharacter*>(eventData);
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
	guild* getEventGuild(eventTrigger* eventData)
	{
		return CallVFunc<34, guild*>(eventData);
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
		return (*(*plugData::data.funcs.condition))(conditionC, eventData);
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
			resources = region->hiddenResources2;
		}
		else {
			return false;
		}
		return ((resources & set) != 0);
	}

	void gameHelpers::setHiddenResource(regionStruct* region, int index, const bool enable)
	{
		int set = 1 << index;
		if (index < 32)
		{
			if (enable == true)
			{
				region->hiddenResources1 = region->hiddenResources1 | set;
			}
			else
			{
				region->hiddenResources1 = region->hiddenResources1 & (0b1111111111111111 - set);
			}
		}
		else if (index < 64) {
			index = index - 32;
			if (enable == true)
			{
				region->hiddenResources2 = region->hiddenResources2 | set;
			}
			else
			{
				region->hiddenResources2 = region->hiddenResources2 & (0b1111111111111111 - set);
			}
		}
	}

	void gameHelpers::changeRegionName(regionStruct* region, const char* newName)
	{
		(*(*plugData::data.funcs.changeRegionName))(region, newName);
	}

	std::string gameHelpers::getRegionName(regionStruct* region)
	{
		return technicalHelpers::uniStringToStr(region->localizedRegionName);
	}

	void gameHelpers::changeRebelsName(regionStruct* region, const char* newName)
	{
		(*(*plugData::data.funcs.changeRebelsName))(region, newName);
	}

	std::string gameHelpers::getRebelsName(regionStruct* region)
	{
		return technicalHelpers::uniStringToStr(region->localizedRebelsName);
	}

	int gameHelpers::getMercUnitNum(mercPool* mercPool)
	{
		mercPoolUnitsPtr* unitptr = &mercPool->firstUnits;
		int units = 0;
		int addunits = 0;
		while (unitptr != nullptr)
		{
			addunits = unitptr->currentPool;
			units = units + addunits;
			unitptr = unitptr->nextUnitsPtr;
		}
		return units;
	}

	mercPoolUnit* gameHelpers::addMercUnit(mercPool* mercPool, const int idx, const int exp, const int cost, const float repmin, const float repmax, const int maxunits, const float startpool, const float startyear, const float endyear, const int crusading)
	{
		auto* newMerc = reinterpret_cast<mercPoolUnit*>(technicalHelpers::allocateGameMem(sizeof(mercPoolUnit)));
		*newMerc = mercPool->firstUnits.mercPoolUnits[0];
		int mercUnitNum = gameHelpers::getMercUnitNum(mercPool);
		eduEntry* entry = eopEduHelpers::getEduEntry(idx);
		newMerc->eduEntry = entry;
		newMerc->experience = exp;
		newMerc->cost = cost;
		newMerc->replenishMin = repmin;
		newMerc->replenishMax = repmax;
		newMerc->maxUnits = maxunits;
		newMerc->currentPool = startpool;
		newMerc->startYear = startyear;
		newMerc->endYear = endyear;
		newMerc->religionsList = nullptr;
		newMerc->religionsListEnd = nullptr;
		newMerc->religionsListEnd2 = nullptr;
		newMerc->eventsList = nullptr;
		newMerc->eventsListEnd = nullptr;
		newMerc->eventsListEnd2 = nullptr;
		newMerc->crusading = crusading;
		newMerc->mercPoolUnitIndex = static_cast<int16_t>(mercUnitNum);
		newMerc->mercPool = mercPool;
		mercPoolUnitsPtr* unitptr = &mercPool->firstUnits;
		int currunits = unitptr->currentPool;
		int maxunitsP = unitptr->Maxpool;
		while ((maxunitsP - currunits) == 0 && unitptr->nextUnitsPtr != nullptr)
		{
			unitptr = unitptr->nextUnitsPtr;
			currunits = unitptr->currentPool;
			maxunitsP = unitptr->Maxpool;
		}
		if ((maxunitsP - currunits) == 0)
		{
			const auto newPtr = reinterpret_cast<mercPoolUnitsPtr*>(technicalHelpers::allocateGameMem(sizeof(mercPoolUnitsPtr)));
			newPtr->Maxpool = unitptr->Maxpool * 2;
			newPtr->currentPool = 0;
			newPtr->nextUnitsPtr = nullptr;
			newPtr->prevPoolUnits = unitptr->mercPoolUnits;
			unitptr->nextUnitsPtr = newPtr;
			unitptr = unitptr->nextUnitsPtr;
			currunits = 0;
		}
		const auto newArray = reinterpret_cast<mercPoolUnit*>(technicalHelpers::allocateGameMem(sizeof(mercPoolUnitsPtr) * unitptr->Maxpool));
		for (int i = 0; i < currunits; i++) {
			newArray[i] = unitptr->mercPoolUnits[i];
		}
		unitptr->mercPoolUnits = newArray;
		unitptr->mercPoolUnits[currunits] = *newMerc;
		unitptr->currentPool++;
		return &unitptr->mercPoolUnits[currunits];
	}

	mercPoolUnit* gameHelpers::getMercUnit(mercPool* pool, const int index)
	{
		mercPoolUnitsPtr* unitptr = &pool->firstUnits;
		int currunits = 0;
		while (unitptr != nullptr)
		{
			currunits = unitptr->currentPool;
			for (int i = 0; i < currunits; i++)
			{
				if (&unitptr->mercPoolUnits[i] != nullptr && unitptr->mercPoolUnits[i].mercPoolUnitIndex == index)
				{
					return &unitptr->mercPoolUnits[i];
				}
			}
			unitptr = unitptr->nextUnitsPtr;
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
				{
					return;
				}
				else
				{
					relFound = true;
				}
			}
			else
			{
				religions.push_back(unit->religionsList[i]);
			}
		}
		if (relFound == false && set == false) { return; }
		if (set)
		{
			religions.push_back(religion);
			mercRelNum++;
		}
		else
		{
			mercRelNum--;
		}
		int* newList = new int[mercRelNum];
		unit->religionsList = newList;
		for (int i = 0; i < mercRelNum; i++)
		{
			unit->religionsList[i] = religions[i];
		}
		unit->religionsListEnd = &unit->religionsList[mercRelNum];
		unit->religionsListEnd2 = &unit->religionsList[mercRelNum];
	}

	void gameHelpers::saveGame(const char* path)
	{
		(*(*plugData::data.funcs.saveGame))(path);
	}

	void gameHelpers::scriptCommand(std::string command, const char* args)
	{
		const char* commandC = command.c_str();
		(*(*plugData::data.funcs.scriptCommand))(commandC, args);
	}

	void gameHelpers::historicEvent(const char* name, const char* title, const char* description)
	{
		(*(*plugData::data.funcs.historicEvent))(name, title, description);
	}

	general* gameHelpers::getCardinal(const collegeOfCardinals* college, const int index)
	{
		return college->cardinalsArray[index].trackedCharacter->character;
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
		return tile->factionId & 0b00111111;
	}

	bool tileHasRiver(const oneTile* tile)
	{
		return tile->factionId & 0b01000000;
	}

	bool tileHasCrossing(const oneTile* tile)
	{
		return tile->factionId & 0b10000000;
	}

	int tileBorderType(const oneTile* tile)
	{
		return tile->border / 4;
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

	coordPair* convertTileCoords(DWORD arrayIndex)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		bool found = false;
		int x = 0;
		int y = 0;
		int index = arrayIndex;
		for (y = 0;y < map->mapHeight;y++)
		{
			x = index - y * map->mapWidth;
			if (x >= 0 && x < map->mapWidth)
			{
				auto coords = new coordPair;
				coords->xCoord = x;
				coords->yCoord = y;
				return coords;
			}
		}
		return nullptr;
	}

	coordPair* getTileCoords(const oneTile* tile)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		return convertTileCoords(tile - map->tilesArr);
	}

	settlementStruct* getSettlementByName(campaign* campaign, const char* name)
	{
		settlementList* settlementList = &campaign->settlementList;
		int settNum = 0;
		while (settlementList != nullptr)
		{
			settNum = settlementList->settlementsNum;
			for (int i = 0; i < settNum; i++)
			{
				if (strcmp(settlementList->settlements[i]->name, name) == 0)
				{
					return settlementList->settlements[i];
				}
			}
			settlementList = settlementList->nextSettlements;
		}
		return nullptr;
	}

	const char* getReligionName(const int index)
	{

		const auto* religionDb = *reinterpret_cast <religionDatabase**>(0x016A0B90);
		if (m2tweopHelpers::getGameVersion() == 1)
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
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		int mapWidth = map->mapWidth * 2 + 1;
		auto coords = getTileCoords(tile);
		return &map->climateTileArray[(coords->yCoord * 2) * mapWidth + (coords->xCoord * 2)];
	}

	float getTileHeight(const oneTile* tile)
	{
		return tileToDoubleTile(tile)->height;
	}

	int getTileClimate(const oneTile* tile)
	{
		return tileToDoubleTile(tile)->climate;
	}

	int getTileHeatValue(const oneTile* tile)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		auto climate = tileToDoubleTile(tile)->climate;
		return map->climates->climateArray[climate].heatValue;
	}

	float getReligionHistory(const regionStruct* region, const int religionID, int turnsAgo)
	{
		if (turnsAgo > 19)
		{
			return 0.0f;
		}
		if (turnsAgo == 0)
		{
			return region->religionsARR[religionID];
		}
		return region->religionHistory[turnsAgo][religionID];
	}

	oneTile* getTileBorderingEdgeOfMap(const regionStruct* region, const int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;

		return &map->tilesArr[region->tilesBorderingEdgeOfMap[index]];
	}

	oneTile* getTileRegion(const regionStruct* region, const int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;

		return &map->tilesArr[region->tiles[index]];
	}

	oneTile* getFertileTile(const regionStruct* region, const int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;

		return &map->tilesArr[region->fertileTiles[index]];
	}

	oneTile* getBorderTile(const neighbourRegion* region, const int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;

		return &map->tilesArr[region->bordertiles[index]];
	}

	oneTile* getReachableTile(const seaConnectedRegion* region, int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		if (!&region->tilesReachable[index])
		{
			return nullptr;
		}
		auto tile = region->tilesReachable[index].tileId;
		return &map->tilesArr[tile];
	}

	coordPair* getTradeLaneCoord(const seaConnectedRegion* region, int index)
	{
		return &region->seaTradeLanePath[index];
	}

	bool hasResourceType(const regionStruct* region, const int resourceType)
	{
		return region->resourceTypesBitMap & (1 << resourceType);
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

	DWORD* getTileObject(const oneTile* tile, int type)
	{
		DWORD* object = tile->object;

		while (object)
		{
			int objectType = CallVFunc<4, int>(object);
			if (objectType == type || (type == 0 && objectType < 28))
				return object;
			object = reinterpret_cast<DWORD*>(*(object + 1));
		}

		return nullptr;
	}

	resStrat* getTileResource(const oneTile* tile)
	{
		return reinterpret_cast<resStrat*>(getTileObject(tile, 0));
	}

	general* getTileCharacter(const oneTile* tile)
	{
		return reinterpret_cast<general*>(getTileObject(tile, 28));
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






}
