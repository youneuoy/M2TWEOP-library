#include "factionHelpers.h"
#include "plugData.h"

std::string factionHelpers::getFactionName(const factionStruct* fac)
{
    return std::string(fac->factSmDescr->facName);
}

generalCharacterictics* factionHelpers::getCharacterFromFullList(const factionStruct* fac, int index)
{
    return fac->charactersAll[index];
}

general* factionHelpers::getCharacterFromGeneralsList(const factionStruct* fac, int index)
{
    return fac->characters[index];
}

stackStruct* factionHelpers::getStack(const factionStruct* fac, int index)
{
    return fac->stacks[index];
}

settlementStruct* factionHelpers::getSettlement(const factionStruct* fac, int index)
{
    return fac->settlements[index];
}

fortStruct* factionHelpers::getFort(const factionStruct* fac, int index)
{
    return fac->forts[index];
}

portBuildingStruct* factionHelpers::getPort(const factionStruct* fac, int index)
{
    return fac->portBuildings[index];
}

watchTowerStruct* factionHelpers::getWatchtower(const factionStruct* fac, int index)
{
    return fac->watchTowers[index];
}