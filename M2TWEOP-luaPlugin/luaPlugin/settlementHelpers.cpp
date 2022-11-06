#include "settlementHelpers.h"

#include "plugData.h"

siegeS* settlementHelpers::getSiege(const settlementStruct* sett, int index)
{
	return sett->sieges[index].siege;
}

void settlementHelpers::changeOwner(settlementStruct* sett, factionStruct* newOwner)
{
	(*(*plugData::data.funcs.setSettlementOwner))(sett, newOwner);
}

float settlementHelpers::getReligion(const settlementStruct* sett, int index)
{
	regionStruct*currRegion= (*(*plugData::data.funcs.getRegionByID))(sett->regionNumber);
	if (currRegion == nullptr)
	{
		return 0.0f;
	}
	return currRegion->religionsARR[index];
}
void settlementHelpers::setReligion(const settlementStruct* sett, int index, float value)
{
	regionStruct* currRegion = (*(*plugData::data.funcs.getRegionByID))(sett->regionNumber);
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

int settlementHelpers::getGuildStanding(settlementStruct* sett, int index)
{
	return *(int*)((sett->guildStandings) + 0x4 * index);
}
void settlementHelpers::setGuildStanding(settlementStruct* sett, int index, int amount)
{
	*(int*)((sett->guildStandings) + 0x4 * index) = amount;
}
