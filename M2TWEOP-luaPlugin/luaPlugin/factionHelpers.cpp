#include "factionHelpers.h"
#include "plugData.h"
#include <windows.h>

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

void factionHelpers::deleteFort(const factionStruct* fac, fortStruct* fort)
{
	(*(*plugData::data.funcs.deleteFort))(fac, fort);
}

void factionHelpers::createFortXY(const factionStruct* fac, int x, int y)
{
	(*(*plugData::data.funcs.createFortXY))(fac, x, y);
}

void factionHelpers::createFort(const general* gen)
{
	(*(*plugData::data.funcs.createFort))(gen);
}

void factionHelpers::changeFactionName(factionStruct* fac, const char* newName)
{
	(*(*plugData::data.funcs.changeFactionName))(fac, newName);
}

std::string factionHelpers::getLocalizedFactionName(factionStruct* fac)
{
	UNICODE_STRING** localizedname = fac->localizedName;
	UNICODE_STRING* name = *localizedname;
	if (name->Length == 0)
	{
		localizedname = (*(*plugData::data.funcs.getFactionName))(fac);
	}
	return technicalHelpers::uniStringToStr(localizedname);
}