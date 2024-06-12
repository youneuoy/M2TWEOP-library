#include "settlementHelpers.h"

#include "fastFuncts.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "technicalHelpers.h"
#include "unitActions.h"

siegeS* settlementHelpers::getSiege(const settlementStruct* sett, int index)
{
	return sett->sieges[index].siege;
}


void settlementHelpers::changeOwner(settlementStruct* sett, factionStruct* newOwner)
{
	fastFuncts::setSettlementOwner(sett, newOwner, false);
}


void settlementHelpers::setBuildingHealth(building* build, int health)
{
	DWORD repairBuilding = 0x005F8DC0;
	if (smallFuncs::getGameVersion() == 1)
		repairBuilding = 0x005F89D0;
	GAME_FUNC_RAW(void(__thiscall*)(building*, int), repairBuilding)(build, health);
}
int settlementHelpers::getBuildingHealth(building* build)
{
	return build->hp;
}

bool settlementHelpers::addBuildingToQueue(buildingInQueue* building)
{
	auto sett = building->settlement;
	if (!sett)
		return false;
	DWORD funcAddr = 0x005E4600;
	if (smallFuncs::getGameVersion() == 1)
		funcAddr = 0x5E4190;
	int result = 0;
	_asm
	{
		push building
		mov ecx, sett
		mov eax, funcAddr
		call eax
		mov result, eax
	}
	return result != 0;
}

bool settlementHelpers::addUnitToQueue(unitRQ* unit)
{
	auto sett = unit->settlement;
	if (!sett)
		return false;
	DWORD funcAddr = 0x005E4510;
	if (smallFuncs::getGameVersion() == 1)
		funcAddr = 0x5E40A0;
	int result = 0;
	_asm
	{
		push unit
		mov ecx, sett
		mov eax, funcAddr
		call eax
		mov result, eax
	}
	return result != 0;
}

void settlementHelpers::upgradeSettlement(settlementStruct* sett)
{
	DWORD createBuildInSett = 0x008A3CE0;
	if (smallFuncs::getGameVersion() == 1)
		createBuildInSett = 0x008A32F0;
	DWORD upgradeBuildInSett = 0x005F80A0;
	if (smallFuncs::getGameVersion() == 1)
		upgradeBuildInSett = 0x005F7CB0;
	DWORD upgradeSett = 0x005DBA20;
	if (smallFuncs::getGameVersion() == 1)
		upgradeSett = 0x005DB560;
	int upgraded = 0;
	_asm
	{
		mov ecx, sett
		mov eax, upgradeSett
		call eax
		mov upgraded, eax
	}
	if (!upgraded)
	{
		unitActions::logStringGame("settlementStruct.upgrade: could not upgrade settlement.");
		return;
	}
	edbEntry* coreEntry;
	auto edb = getEdb();
	
	if (sett->isCastle)
		coreEntry = edb->coreCastleBuilding;
	else
		coreEntry = edb->coreCityBuilding;

	building* existing = nullptr;
	for (int i = 0; i < sett->buildingsNum; i++)
	{
		if (sett->buildings[i]->edbEntry->buildingID == coreEntry->buildingID)
		{
			existing = sett->buildings[i];
			int factionId = sett->faction->dipNum;
			int newLevel = existing->level + 1;
			_asm
			{
				push 1
				push factionId
				push newLevel
				mov ecx, existing
				mov eax, upgradeBuildInSett
				call eax
			}
			existing->field_64 = 0;
			existing->settlementLevel = 5;
			if (existing->hp != 100)
				setBuildingHealth(existing, 100);
			return;
		}
	}
	_asm
    {
	    push 1
	    push coreEntry
	    push sett
	    mov eax, createBuildInSett
	    call eax
    }
		
}

int settlementHelpers::getAvailableBuildingsCount(const availableBuildings* list)
{
	int count = 0;
	while (list)
	{
		count += list->buildingCount;
		list = list->next;
	}
	return count;
}

buildingInQueue* settlementHelpers::getBuildingOption(const availableBuildings* list, const int index)
{
	int passed = 0;
	while (list)
	{
		if (index < passed + list->buildingCount)
		{
			return &list->buildingsList[index - passed];
		}
		passed += list->buildingCount;
		list = list->next;
	}
	return nullptr;
}

availableBuildings* settlementHelpers::getAvailableBuildingsMem()
{
	DWORD offset = 0x016ABB14;
	if (smallFuncs::getGameVersion() == 1)
		offset = 0x016F4CAC;
	return reinterpret_cast<availableBuildings*>(offset);
}

recruitmentOptions* settlementHelpers::getAvailableUnitsMem()
{
	DWORD offset = 0x0016ABC68;
	if (smallFuncs::getGameVersion() == 1)
		offset = 0x16F4E00;
	return reinterpret_cast<recruitmentOptions*>(offset);
}

recruitmentOptions* settlementHelpers::getAvailableUnits(settlementStruct* sett)
{
	const auto mem = getAvailableUnitsMem();
	DWORD getUnits = 0x005E7EA0;
	if (smallFuncs::getGameVersion() == 1)
		getUnits = 0x005E7A40;
	GAME_FUNC_RAW(void(__thiscall*)(settlementStruct*, recruitmentOptions*), getUnits)(sett, mem);
	return mem;
}

recruitmentOptions* settlementHelpers::getAvailableRetrainingUnits(settlementStruct* sett)
{
	const auto mem = getAvailableUnitsMem();
	DWORD getUnits = 0x005E65A0;
	if (smallFuncs::getGameVersion() == 1)
		getUnits = 0x005E6140;
	GAME_FUNC_RAW(void(__thiscall*)(settlementStruct*, recruitmentOptions*), getUnits)(sett, mem);
	return mem;
}

struct buildingOptionsDb
{
	std::array<std::shared_ptr<settlementBuildingOptions>, 200> settOptions{};
};
struct recruitOptionsDb
{
	std::array<std::shared_ptr<settlementRecruitmentOptions>, 200> settRecruitOptions{};
};

std::shared_ptr<buildingOptionsDb> buildingOptionsDbPtr = std::make_shared<buildingOptionsDb>();
std::shared_ptr<recruitOptionsDb> recruitOptionsDbPtr = std::make_shared<recruitOptionsDb>();

availableBuildings* settlementHelpers::getAvailableBuildings(settlementStruct* sett)
{
	const auto mem = getAvailableBuildingsMem();
	DWORD funcAddrReset = 0x5FA710;
	if (smallFuncs::getGameVersion() == 1)
		funcAddrReset = 0x005FA320;
	GAME_FUNC_RAW(void(__thiscall*)(availableBuildings*), funcAddrReset)(mem);
	DWORD getAvailableBuildings = 0x005E1C00;
	if (smallFuncs::getGameVersion() == 1)
		getAvailableBuildings = 0x005E1730;
	GAME_FUNC_RAW(void(__thiscall*)(settlementStruct*, availableBuildings*, int, int, int),
		getAvailableBuildings)(sett, mem, 1, 1, 0);
	return mem;
}

int settlementHelpers::makeBuildOptionsHash(const settlementStruct* sett)
{
	int hash = 0;
	for (int i = 0; i < sett->buildingsNum; i++)
		hash += sett->buildings[i]->edbEntry->buildingID;
	for (int i = sett->buildingsQueueArray.firstIndex; i <= sett->buildingsQueueArray.currentBuildingIndex; i++)
		if (const auto inQueue = &sett->buildingsQueueArray.items[i]; inQueue && inQueue->edbEntry)
			hash += inQueue->edbEntry->buildingID * 2;
	return hash;
}

int settlementHelpers::makeRecruitOptionsHash(const settlementStruct* sett)
{
	int hash = 0;
	for (int i = 0; i < sett->recruitmentPoolCount; i++)
	{
		hash += sett->recruitmentPools[i].eduIndex;
		hash += static_cast<int>(sett->recruitmentPools[i].availablePool * 100);
	}
	return hash;
}

settlementBuildingOptions* settlementHelpers::getBuildingOptions(settlementStruct* sett)
{
	const int index = sett->regionID;
	const int turnNum = gameDataAllHelper::get()->campaignData->TurnNumber;
	const int hash = makeBuildOptionsHash(sett);
	auto options = buildingOptionsDbPtr->settOptions[index];
	if (options && options->hash == hash && options->turn == turnNum)
		return options.get();
	
	buildingOptionsDbPtr->settOptions[index] = std::make_shared<settlementBuildingOptions>();
	options = buildingOptionsDbPtr->settOptions[index];
	options->hash = hash;
	options->turn = turnNum;
	const auto available = getAvailableBuildings(sett);
	options->count = getAvailableBuildingsCount(available);
	for (int i = 0; i < options->count; i++)
	{
		options->constructionOptions.push_back(std::make_shared<buildingInQueue>());
		*options->constructionOptions[i] = *getBuildingOption(available, i);
		options->totalCost += options->constructionOptions[i]->buildCost;
		options->totalTime += options->constructionOptions[i]->turnsToBuild;
	}
	return options.get();
}

settlementRecruitmentOptions* settlementHelpers::getRecruitOptions(settlementStruct* sett)
{
	const int index = sett->regionID;
	const int turnNum = gameDataAllHelper::get()->campaignData->TurnNumber;
	const int hash = makeRecruitOptionsHash(sett);
	auto options = recruitOptionsDbPtr->settRecruitOptions[index];
	if (options && options->hash == hash && options->turn == turnNum)
		return options.get();
	
	recruitOptionsDbPtr->settRecruitOptions[index] = std::make_shared<settlementRecruitmentOptions>();
	options = recruitOptionsDbPtr->settRecruitOptions[index];
	options->hash = hash;
	options->turn = turnNum;
	auto available = getAvailableUnits(sett);
	const int trainCount = (available->lastUnit - reinterpret_cast<DWORD>(available->units)) / sizeof(unitRQ);
	options->count = trainCount;
	for (int i = 0; i < options->count; i++)
	{
		options->recruitOptions.push_back(std::make_shared<unitRQ>());
		*options->recruitOptions[i] = available->units[i];
		options->totalCost += options->recruitOptions[i]->cost;
		options->totalTime += options->recruitOptions[i]->turnsToTrain;
	}
	available = getAvailableRetrainingUnits(sett);
	const int retrainCount = (available->lastUnit - reinterpret_cast<DWORD>(available->units)) / sizeof(unitRQ);
	options->count += retrainCount;
	for (int i = trainCount; i < options->count; i++)
	{
		options->recruitOptions.push_back(std::make_shared<unitRQ>());
		*options->recruitOptions[i] = available->units[i];
		options->totalCost += options->recruitOptions[i]->cost;
		options->totalTime += options->recruitOptions[i]->turnsToTrain;
	}
	return options.get();
}

buildingInQueue* settlementHelpers::getBuildingOptionFromDb(const settlementBuildingOptions* list, const int index)
{
	return list->constructionOptions[index].get();
}

unitRQ* settlementHelpers::getRecruitOptionFromDb(const settlementRecruitmentOptions* list, const int index)
{
	return list->recruitOptions[index].get();
}

eduEntry* settlementHelpers::getUnitEntry(const unitRQ* unitOption)
{
	if (unitOption->recruitType != 2)
		return unitOption->eduEntry;
	return nullptr;
}

int settlementHelpers::getAgentType(const unitRQ* unitOption)
{
	if (unitOption->recruitType == 2)
		return reinterpret_cast<int>(unitOption->eduEntry);
	return -1;
}

void settlementHelpers::setUnitEntry(unitRQ* unitOption, eduEntry* entry)
{
	if (unitOption->recruitType != 2)
		unitOption->eduEntry = entry;
}

void settlementHelpers::setAgentType(unitRQ* unitOption, int type)
{
	if (unitOption->recruitType == 2)
		unitOption->eduEntry = reinterpret_cast<eduEntry*>(type);
}

exportDescrBuildings* settlementHelpers::getEdb()
{
	DWORD offset = 0x01861438;
	if (smallFuncs::getGameVersion() == 1)
		offset = 0x018AA5A8;
	return reinterpret_cast<exportDescrBuildings*>(offset);
}

float settlementHelpers::getReligion(const settlementStruct* sett, int index)
{
	const regionStruct* currRegion = &smallFuncs::getStratMap()->regions[sett->regionID];
	if (currRegion == nullptr)
	{
		return 0.0f;
	}
	return currRegion->religionsARR[index];
}
void settlementHelpers::setReligion(const settlementStruct* sett, int index, float value)
{
	const regionStruct* currRegion = &smallFuncs::getStratMap()->regions[sett->regionID];
	currRegion->religionsARR[index] = value;
}
resStrat* settlementHelpers::getResource(const settlementStruct* sett, int index)
{
	return sett->resources[index];
}

building* settlementHelpers::getBuilding(const settlementStruct* sett, int index)
{
	return sett->buildings[index];
}

std::string settlementHelpers::getSettlementName(settlementStruct* sett)
{
	return technicalHelpers::uniStringToStr(sett->localizedName);
}
int settlementHelpers::getGuildStanding(settlementStruct* sett, int index)
{
	return *(int*)((sett->guildStandings) + 0x4 * index);
}
void settlementHelpers::setGuildStanding(settlementStruct* sett, int index, int amount)
{
	*(int*)((sett->guildStandings) + 0x4 * index) = amount;
}
settlementCapability* settlementHelpers::getSettlementCapability(settlementStruct* sett, int capabilityType)
{
	return &sett->capabilities.settlementCapabilities[capabilityType];
}
settlementCapability* settlementHelpers::getAgentCapability(settlementStruct* sett, int agentType)
{
	return &sett->capabilities.settlementCapabilitiesAgent[agentType];
}
settlementCapability* settlementHelpers::getAgentLimitCapability(settlementStruct* sett, int agentType)
{
	return &sett->capabilities.settlementCapabilitiesAgentLimit[agentType];
}
recruitmentCapability* settlementHelpers::getRecruitmentCapability(settlementStruct* sett, int index)
{
	return &sett->capabilities.recruitmentCapabilities[index];
}
int settlementHelpers::getRecruitmentCapabilityNum(settlementStruct* sett)
{
	return sett->capabilities.recruitmentCapabilityCount;
}
int settlementHelpers::getPopulation(settlementStruct* sett)
{
	return sett->settlementStats.population;
}
settlementRecruitmentPool* settlementHelpers::getSettlementRecruitmentPool(settlementStruct* sett, int index)
{
	return &sett->recruitmentPools[index];
}


