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

void factionHelpers::setCharacterNameFaction(factionStruct* fac, int characterType, int factionID)
{
	fac->agentNameFactionId[characterType] = factionID;
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

watchTowerStruct* factionHelpers::spawnWatchtower(const factionStruct* fac, int x, int y)
{
	auto tile = gameHelpers::getTile(x, y);
	if (!tile)
		return nullptr;
	auto region = gameHelpers::getRegion(tile->regionId);
	if (!region || !region->settlement || region->factionOwner != fac)
		return nullptr;
	int gameVer = m2tweopHelpers::getGameVersion();
	auto settlement = region->settlement;
	auto tower = nullptr;
	DWORD makeTowerAddr = 0x008B97B0;
	if (gameVer == 1)
		makeTowerAddr = 0x008B8DC0;
	_asm
	{
		push settlement
		mov eax, makeTowerAddr
		call eax
		mov tower, eax
		add esp, 4
	}
	DWORD spawnCreatedObject = 0x004CD800;
	if (gameVer == 1)
		spawnCreatedObject = 0x004CD240;
	coords* spawnCoords = new coords();
	spawnCoords->xCoord = x;
	spawnCoords->yCoord = y;
	_asm
	{
		push spawnCoords
		push tower
		mov eax, spawnCreatedObject
		call eax
	}
	auto watchtowers = &gameDataAllHelper::get()->campaignData->watchtowers;
	DWORD addToWatchtowerList = 0x004DD940;
	if (gameVer == 1)
		addToWatchtowerList = 0x004DD390;
	_asm
	{
		push tower
		mov ecx, watchtowers
		mov eax, addToWatchtowerList
		call eax
	}
	DWORD blockadeStuff = 0x004DD2F0;
	if (gameVer == 1)
		blockadeStuff = 0x004DCD40;
	_asm
	{
		mov ecx, tower
		mov eax, blockadeStuff
		call eax
	}
	delete spawnCoords;
	return tower;
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

float distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void* disembark(stackStruct* army, int x, int y)
{
	DWORD cadClass = technicalHelpers::allocateGameMem(0x301Cu);
	DWORD createCadDisembark = 0x005B06D0;
	if (m2tweopHelpers::getGameVersion() == 1)
		createCadDisembark = 0x5B01F0;
	cadClass = GAME_FUNC_RAW(DWORD(__thiscall*)(DWORD, int), createCadDisembark)(cadClass, 0);
	DWORD setCadClass = 0xAAC080;
	if (m2tweopHelpers::getGameVersion() == 1)
		setCadClass = 0xAAB050;
	auto charPtr = &army->gen;
	coordPair coords{x, y};
	auto coordsPtr = &coords;
	GAME_FUNC_RAW(void(__thiscall*)(general**, coordPair*, DWORD), setCadClass)(charPtr, coordsPtr, cadClass);
	DWORD globalCadClass = 0x0162C740;
	if (m2tweopHelpers::getGameVersion() == 1)
		globalCadClass = 0x1674570;
	DWORD cadClass2 = *reinterpret_cast<DWORD*>(globalCadClass);
	DWORD finalize = 0x0059ec70;
	if (m2tweopHelpers::getGameVersion() == 1)
		finalize = 0x0059e790;
	auto character = army->gen;
	_asm
	{
		push cadClass2
		mov ecx, character
		mov eax, finalize
		call eax
	}
}

stackStruct* factionHelpers::splitArmy(factionStruct *faction, const sol::table& units, int x, int y)
{
	unit* unitList[20]{};
	coordPair targetCoords{x, y};
	const int unitCount = units.size();
	if (unitCount > 20 || unitCount < 1)
	{
		m2tweopHelpers::logStringGame("factionStruct.splitArmy: unit count must be between 1 and 20.");
		return nullptr;
	}
	auto tile = gameHelpers::getTile(x, y);
	auto tileChar = gameHelpers::getTileCharacter(tile);
	stackStruct* targetMerge = nullptr;
	if (tileChar)
	{
		if (tileChar->armyLeaded)
		{
			if (tileChar->armyLeaded->faction != faction)
			{
				m2tweopHelpers::logStringGame("factionStruct.splitArmy: can not split army, tile is occupied by enemy.");
				return nullptr;
			}
		}
	}
	auto tileSett = gameHelpers::getTileSettlement(tile);
	if (tileSett)
	{
		if (tileSett->faction != faction)
		{
			m2tweopHelpers::logStringGame("factionStruct.splitArmy: can not split army, tile is occupied by enemy settlement.");
			return nullptr;
		}
	}
	auto tileFort = gameHelpers::getTileFort(tile);
	if (tileFort)
	{
		if (tileFort->faction != faction)
		{
			m2tweopHelpers::logStringGame("factionStruct.splitArmy: can not split army, tile is occupied by enemy fort.");
			return nullptr;
		}
	}
	stackStruct* stack = nullptr;
	for (int i{ 1 }; i <= unitCount; i++)
	{
		const auto& un = units.get<sol::optional<unit*>>(i);
		if (!un)
			break;
		unit* unit = un.value_or(nullptr);
		if (!unit)
			break;
		if (!stack)
			stack = unit->army;
		if (unit && unit->army == stack)
		    unitList[i - 1] = unit;
	}
	if (!stack)
		return nullptr;
	if (stack->shipArmy)
	{
		if (distance(stack->shipArmy->gen->xCoord, stack->shipArmy->gen->yCoord, x, y) > 1.5)
		{
			disembark(stack->shipArmy, x, y);
		}
	}
	DWORD canArmySplit = 0x4D61F0;
	if (m2tweopHelpers::getGameVersion() == 1)
		canArmySplit = 0x4D5C30;
	auto stratPathFind = gameDataAllHelper::get()->stratPathFinding; 
	if (!GAME_FUNC_RAW(bool(__thiscall*)(stratPathFinding*, unit**, int, coordPair*), canArmySplit)
		(stratPathFind, &unitList[0], unitCount, &targetCoords))
	{
		m2tweopHelpers::logStringGame("factionStruct.splitArmy: can not split army.");
		return nullptr;
	}
	DWORD splitArmy = 0x4F9C90;
	if (m2tweopHelpers::getGameVersion() == 1)
		splitArmy = 0x4F9740;
	auto coordsPtr = &targetCoords;
	auto listPtr = &unitList[0];
	_asm
	{
		push coordsPtr
		push unitCount
		push listPtr
		mov ecx, faction
		mov eax, splitArmy
		call eax
	}
	return unitList[0]->army;
}

decisionValuesLTGD* factionHelpers::getlongTermGoalValues(aiLongTermGoalDirector* LTGD, int targetFaction)
{
	return &LTGD->longTermGoalValues[targetFaction];
}

void factionHelpers::setConstructionValue(aiPersonalityValues* personality, int type, int value)
{
	personality->buildingValues[type] = value;
}

void factionHelpers::setRecruitmentValue(aiPersonalityValues* personality, int type, int value)
{
	personality->recruitmentValues[type] = value;
}

int factionHelpers::getConstructionValue(aiPersonalityValues* personality, int type)
{
	return personality->buildingValues[type];
}

int factionHelpers::getRecruitmentValue(aiPersonalityValues* personality, int type)
{
	return personality->recruitmentValues[type];
}

void factionHelpers::setConstructionValueSett(aiProductionController* controller, int type, int value)
{
	controller->buildingBias[type] = value;
}

void factionHelpers::setRecruitmentValueSett(aiProductionController* controller, int type, int value)
{
	controller->recruitBias[type] = value;
}

int factionHelpers::getConstructionValueSett(aiProductionController* controller, int type)
{
	return controller->buildingBias[type];
}

int factionHelpers::getRecruitmentValueSett(aiProductionController* controller, int type)
{
	return controller->recruitBias[type];
}

ltgdFactionValues* factionHelpers::getAiFactionValues(factionStruct* fac)
{
	int gameVer = m2tweopHelpers::getGameVersion();
	ltgdGlobals* ltgd = nullptr;
	if (gameVer == 2)
	{
		ltgd = reinterpret_cast<ltgdGlobals*>(0x016A7Fc4);
	}
	else if (gameVer == 1)
	{
		ltgd = reinterpret_cast<ltgdGlobals*>(0x016F115C);
	}
	if (!ltgd)
		return nullptr;
	return &ltgd->ltgdFactionValues[fac->dipNum];
}

interFactionLTGD* factionHelpers::getInterFactionLTGD(factionStruct* fac, factionStruct* targetFac)
{
	int gameVer = m2tweopHelpers::getGameVersion();
	ltgdGlobals* ltgd = nullptr;
	if (gameVer == 2)
	{
		ltgd = reinterpret_cast<ltgdGlobals*>(0x016A7Fc4);
	}
	else if (gameVer == 1)
	{
		ltgd = reinterpret_cast<ltgdGlobals*>(0x016F115C);
	}
	if (!ltgd)
		return nullptr;
	return &ltgd->interFactionLTGD[fac->dipNum][targetFac->dipNum];
}

aiProductionController* factionHelpers::getProductionController(aiPersonalityValues* personality, int index)
{
	return personality->aiProductionControllers[index];
}