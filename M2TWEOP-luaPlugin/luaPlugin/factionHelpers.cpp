#include "factionHelpers.h"
#include "plugData.h"
#include <windows.h>

#include "gameDataAllHelper.h"

std::string factionHelpers::getFactionName(const factionStruct* fac)
{
	return std::string(fac->factSmDescr->facName);
}

namedCharacter* factionHelpers::getCharacterFromFullList(const factionStruct* fac, int index)
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

int factionHelpers::getNeighbourRegionID(const factionStruct* fac, int index)
{
	return fac->neighBourRegions[index];
}

watchTowerStruct* factionHelpers::getWatchtower(const factionStruct* fac, int index)
{
	return fac->watchTowers[index];
}

void factionHelpers::deleteFort(const factionStruct* fac, fortStruct* fort)
{
	(*(*plugData::data.funcs.deleteFort))(fac, fort);
}

battleFactionCounter* factionHelpers::getBattleVsFactionStats(factionStruct* fac, int targetFactionID)
{
	return fac->battlesWonVsFaction[targetFactionID];
}

bool factionHelpers::hasMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2)
{
	const auto gameData = gameDataAllHelper::get();
	const auto campaign = gameData->campaignData;
	const auto agreements = campaign->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade;
	return agreements & static_cast<int8_t>(0x2);
}

void factionHelpers::setMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2, bool set)
{
	const auto gameData = gameDataAllHelper::get();
	const auto campaign = gameData->campaignData;
	const auto agreements = campaign->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade;
	if (set)
	{
		campaign->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade = agreements | static_cast<int8_t>(0x2);
	}
	else
	{
		campaign->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade = agreements & static_cast<int8_t>(~0x2);
	}
}

float factionHelpers::getFactionStanding(const factionStruct* fac1, const factionStruct* fac2)
{
	const auto gameData = gameDataAllHelper::get();
	const auto campaign = gameData->campaignData;
	return campaign->diplomaticStandings[fac1->dipNum][fac2->dipNum].factionStanding;
}

bool factionHelpers::isNeighbourFaction(const factionStruct* fac1, const factionStruct* fac2)
{
	return fac1->neighBourFactionsBitmap & (1 << fac2->dipNum);
}

void factionHelpers::setFactionStanding(const factionStruct* fac1, const factionStruct* fac2, float standing)
{
	const auto gameData = gameDataAllHelper::get();
	const auto campaign = gameData->campaignData;
	campaign->diplomaticStandings[fac1->dipNum][fac2->dipNum].factionStanding = standing;
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

factionRanking* factionHelpers::getFactionRanking(const factionStruct* fac, int turnNum)
{
	return &fac->factionRankings[turnNum];
}

factionEconomy* factionHelpers::getFactionEconomy(factionStruct* fac, int turnsAgo)
{
	int turnIndex = fac->counterEconomy - turnsAgo;
	if (turnIndex < 0)
	{
		turnIndex += fac->maxTurnsTillReset;
	}
	return &fac->factionEconomy[turnIndex];
}

const char* factionHelpers::getRegionToHoldName(const holdRegionsWinCondition* condition, int index)
{
	return condition->regionsToHold[index].name;
}

int factionHelpers::getRegionToHoldLength(const holdRegionsWinCondition* condition, int index)
{
	if (index < condition->holdRegionLengthsCount)
	{
		return condition->holdRegionLengths[index];
	}
	return 0;
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