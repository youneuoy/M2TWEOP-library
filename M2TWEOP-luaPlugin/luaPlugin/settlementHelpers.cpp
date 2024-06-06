#include "settlementHelpers.h"

#include "plugData.h"

siegeS* settlementHelpers::getSiege(const settlementStruct* sett, int index)
{
	return sett->sieges[index].siege;
}

siegeS* settlementHelpers::getSiegeFort(const fortStruct* fort, int index)
{
	return fort->sieges[index].siege;
}

void settlementHelpers::changeOwner(settlementStruct* sett, factionStruct* newOwner)
{
	(*(*plugData::data.funcs.setSettlementOwner))(sett, newOwner, false);
}

void settlementHelpers::changeOwner(settlementStruct* sett, factionStruct* newOwner, bool convertGarrison)
{
	(*(*plugData::data.funcs.setSettlementOwner))(sett, newOwner, convertGarrison);
}

void settlementHelpers::changeFortOwner(fortStruct* fort, factionStruct* newFaction)
{
	(*(*plugData::data.funcs.changeFortOwner))(fort, newFaction);
}

float settlementHelpers::getReligion(const settlementStruct* sett, int index)
{
	regionStruct* currRegion = (*(*plugData::data.funcs.getRegionByID))(sett->regionID);
	if (currRegion == nullptr)
	{
		return 0.0f;
	}
	return currRegion->religionsARR[index];
}
void settlementHelpers::setReligion(const settlementStruct* sett, int index, float value)
{
	regionStruct* currRegion = (*(*plugData::data.funcs.getRegionByID))(sett->regionID);
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

void settlementHelpers::destroyBuilding(settlementStruct* sett, const char* typeName, bool isReturnMoney)
{
	(*(*plugData::data.funcs.destroyBuilding))(sett, typeName, isReturnMoney);
}

void settlementHelpers::createBuilding(settlementStruct* sett, const char* building_level_id)
{
	(*(*plugData::data.funcs.createBuilding))(sett, building_level_id);
}
void settlementHelpers::changeSettlementName(settlementStruct* sett, const char* newName)
{
	(*(*plugData::data.funcs.changeSettlementName))(sett, newName);
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


