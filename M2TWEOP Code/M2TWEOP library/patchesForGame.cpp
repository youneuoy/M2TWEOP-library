#include "pch.h"
#include "patchesForGame.h"
#include "tilesChange.h"
#include "graphicsD3D.h"
#include <functional>


#include "events.h"
#include "battle.h"
#include "cultures.h"
#include "eopBuildings.h"
#include "onlineThings.h"
#include "character.h"
#include "faction.h"
#include "characterRecord.h"
#include "strategyMap.h"

#include "plannedRetreatRoute.h"
#include "discordManager.h"
#include "eopdu.h"
#include "unit.h"
#include "army.h"
#include "campaign.h"


worldRecord* __fastcall patchesForGame::selectWorldpkgdesc(char* database, worldRecord* selectedRecord)
{
	auto& battlemapWorker = globals::dataS.Modules.battleMapWorker;


	battlemapWorker.TryCreateRecodsList(reinterpret_cast<battlemapWorker::dataBaseWorlds*>(database));

	string selectRecordS = battlemapWorker.getRecordName(selectedRecord);
	string selectRecordG = battlemapWorker.getRecordGroup(selectedRecord);
	
	std::string retVal;
	std::string* tmpVal = gameEvents::onSelectWorldpkgdesc(selectRecordS.c_str(), selectRecordG.c_str());
	string selectedWorld = *tmpVal;
	delete tmpVal;
	if (selectedWorld.empty() || selectedWorld == selectRecordS)
	{
		selectedWorld = battleCreator::selectWorldpkgdesc(selectRecordS, selectRecordG);
		if (selectedWorld.empty() || selectedWorld == selectRecordS)
		{
			selectedWorld = globals::dataS.Modules.developerMode.SelectWorldpkgdesc(selectRecordS, selectRecordG);
		}
	}

	if (selectedWorld.empty() || selectedWorld == selectRecordS)
	{
		return nullptr;
	}



	string selectedWorldCPP = selectedWorld;
	struct dataBaseS
	{
		int something;
		int something2;
		worldRecord* records;
		worldRecord* recordsEnd;
	};

	const dataBaseS* db = reinterpret_cast<dataBaseS*>(database);

	worldRecord* currRecord = db->records;
	do
	{
		string recName = battlemapWorker.getRecordName(currRecord);
		string recGroup = battlemapWorker.getRecordGroup(selectedRecord);
		if (recName == selectedWorldCPP && recGroup == selectRecordG)
		{
			return currRecord;
		}


		currRecord++;
	} while (currRecord != db->recordsEnd);

	return nullptr;
}
void __fastcall patchesForGame::onLoadSettlementWorldpkgdesc(worldRecord* selectedRecord)
{
	auto& battleMapWorker = globals::dataS.Modules.battleMapWorker;

	const string selectRecordS = battleMapWorker.getRecordName(selectedRecord);
	const string selectRecordG = battleMapWorker.getRecordGroup(selectedRecord);
	battleCreator::OnLoadSettlementWorldpkgdesc(selectRecordS, selectRecordG);
}

float __fastcall patchesForGame::onCalculateUnitValue(eduEntry* entry, const DWORD value)
{
	float floatValue;
	std::memcpy(&floatValue, &value, sizeof(float));
	return gameEvents::onCalculateUnitValue(entry, floatValue);
}

struct nameIndexCombo
{
	nameIndexCombo(char* n, int i) : name(n), index(i) {}
	char* name{};
	int index{};
	nameIndexCombo() = default;
};

std::shared_ptr<nameIndexCombo> GLOBAL_NAME_INDEX_COMBO = std::make_shared<nameIndexCombo>();

DWORD __fastcall patchesForGame::onSearchUnitType(char* typeName)
{
	if (const auto eopUnit = eopDu::getEopEduEntryByName(typeName); eopUnit != nullptr)
	{
		gameHelpers::logStringGame("Unit found in M2TWEOP: " + std::string(typeName) + " index: " + std::to_string(eopUnit->index));
		GLOBAL_NAME_INDEX_COMBO = std::make_shared<nameIndexCombo>(typeName, eopUnit->index);
		return reinterpret_cast<DWORD>(GLOBAL_NAME_INDEX_COMBO.get());
	}
	return 0;
}

int patchesForGame::onEvaluateUnit(const int eduIndex)
{
	return eopDu::getEduEntry(eduIndex)->unitProductionClass;
}

DWORD __fastcall patchesForGame::onCustomBattleUnitCards(DWORD cardArrayThing, int factionID)
{
	const int eopUnitNum = eopDu::getEopEntryNum();
	for (int i = 0; i < eopUnitNum; i++)
	{
		const auto eopUnit = eopDu::getEopEduEntryInternalIterating(i);
		if (!eopUnit->isFileAdded)
			continue;
		const auto entry = &eopUnit->data.edu;
		if (entry->category == unitCategory::nonCombatant || entry->category == unitCategory::ship)
			continue;
		if (GAME_FUNC(bool(__thiscall*)(eduEntry*, int, int), checkOwnershipCustom)(entry, factionID, 3))
		{
			stringWithHash card{};
			GAME_FUNC(void(__thiscall*)(eduEntry*, int, stringWithHash*), getUnitCard)(entry, factionID, &card);
			GAME_FUNC(void(__thiscall*)(DWORD, stringWithHash*), addToCardArray)(cardArrayThing, &card);
		}
	}
	return cardArrayThing;
}

int __fastcall patchesForGame::onCustomBattleUnits(eduEntry** unitArray, int currentCount, int factionID)
{
	const int eopUnitNum = eopDu::getEopEntryNum();
	for (int i = 0; i < eopUnitNum; i++)
	{
		if (currentCount >= 200)
			break;
		const auto eopUnit = eopDu::getEopEduEntryInternalIterating(i);
		if (!eopUnit->isFileAdded)
			continue;
		const auto entry = &eopUnit->data.edu;
		if (entry->category == unitCategory::nonCombatant
			|| entry->category == unitCategory::ship
			|| entry->noCustom != 0)
			continue;
		if (const int era = *reinterpret_cast<int*>(dataOffsets::offsets.selectedEra);
			GAME_FUNC(bool(__thiscall*)(eduEntry*, int, int), checkOwnershipCustom)(entry, factionID, era))
		{
			*unitArray = entry;
			unitArray++;
			currentCount++;
		}
	}
	return currentCount;
}

eduEntry* patchesForGame::onEvaluateUnit2(int eduIndex)
{
	if (const auto eopUnit = eopDu::getEopEduEntry(eduIndex); eopUnit == nullptr)
		return eopDu::getEduEntry(eduIndex);
	else
		return eopUnit;
}

int __fastcall patchesForGame::onFortificationLevelS(settlementStruct* settlement, bool* isCastle)
{
	int selectedLevel = -2;//magic value, mean not change anything
	bool isChanged = false;
	int tmpVal = gameEvents::onFortificationLevelS(settlement, isCastle, &isChanged);
	if (isChanged == true)
	{
		selectedLevel = tmpVal;
	}
	if (selectedLevel == -2)
	{
		*isCastle = settlement->isCastle;
	}
	return selectedLevel;//use old thing
}
char* __fastcall patchesForGame::onSaveEDUStringS(eduEntry* eduEntry)
{
	char* retName = eopDu::getEopNameOfEduEntry(eduEntry);
	if (retName == nullptr)
		return eduEntry->eduType;
	return retName;
}

int __fastcall patchesForGame::onCreateUnit(char** entryName, int* eduIndex)
{
	if (eduIndex == nullptr)
	{
		const auto newEdu = eopDu::getEopEduEntryByName(*entryName);
		if (newEdu == nullptr)
			return -1;
		return newEdu->index;
	}
	return *eduIndex;
}

int __fastcall patchesForGame::onFindUnit(char* entry, int* eduIndex)
{
	if (eduIndex == nullptr)
	{
		if (entry == nullptr)
			return  -1;
		const auto newEdu = eopDu::getEopEduEntryByName(entry);
		if (newEdu == nullptr)
			return  -1;
		return newEdu->index;
	}
	return *eduIndex;
}

int __fastcall patchesForGame::onReligionCombatBonus(int religionID, characterRecord* namedChar)
{
	if (religionID > 9)
		return 0;

	return namedChar->combatVsReligion[religionID];
}

std::string FILE_PATH;
std::string GAME_PATH;

std::string getBuildingPicPath(const char* cultureName, const char* levelName)
{
	FILE_PATH = "";
	FILE_PATH.append("/data/ui/");
	FILE_PATH.append(cultureName);
	FILE_PATH.append("/buildings/#");
	FILE_PATH.append(cultureName);
	FILE_PATH.append("_");
	FILE_PATH.append(levelName);
	FILE_PATH.append(".tga");
	return FILE_PATH;
}

std::string getBuildingPicConstructionPath(const char* cultureName, const char* levelName)
{
	FILE_PATH = "";
	FILE_PATH.append("/data/ui/");
	FILE_PATH.append(cultureName);
	FILE_PATH.append("/buildings/construction/#");
	FILE_PATH.append(cultureName);
	FILE_PATH.append("_");
	FILE_PATH.append(levelName);
	FILE_PATH.append(".tga");
	return FILE_PATH;
}

std::string getBuildingPicConstructedPath(const char* cultureName, const char* levelName)
{
	FILE_PATH = "";
	FILE_PATH.append("/data/ui/");
	FILE_PATH.append(cultureName);
	FILE_PATH.append("/buildings/#");
	FILE_PATH.append(cultureName);
	FILE_PATH.append("_");
	FILE_PATH.append(levelName);
	FILE_PATH.append("_constructed.tga");
	return FILE_PATH;
}

std::string getGamePath(const std::string& modPath) {
	size_t pos = modPath.length();
	int dirCount = 0;
	if (modPath.back() == '/' || modPath.back() == '\\') {
		pos--;
	}
	for (; pos > 0; pos--) {
		if (modPath[pos] == '/' || modPath[pos] == '\\') {
			dirCount++;
			if (dirCount == 2) {
				break;
			}
		}
	}
	return modPath.substr(0, pos);
}


char* __fastcall patchesForGame::getBrowserPicConstructed(int cultureID, edbEntry* entry, int buildingLevel)
{
	const auto level = entry->levels[buildingLevel];
	const auto levelName = level.name;
	if (levelName == nullptr)
		return nullptr;
	const auto cultureName = cultures::getCultureName(cultureID);
	if (cultureName.empty())
		return nullptr;
	const auto picPath = getBuildingPicConstructedPath(cultureName.c_str(), levelName);
	const auto modPath = gameHelpers::getModPath();
	FILE_PATH = modPath + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	if (GAME_PATH.empty())
		GAME_PATH = getGamePath(modPath);
	FILE_PATH = GAME_PATH + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	const int variantNum = edb->lookupVariants.size();
	for (int i = 0; i < variantNum; i++)
	{
		const auto variant = edb->lookupVariants.get(i);
		const auto variantNamesSize = variant->names.size();
		for (int j = 0; j < variantNamesSize; j++)
		{
			const auto variantName = variant->names.get(j);
			const auto variantString = variantName->name;
			FILE_PATH = modPath + getBuildingPicConstructedPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
			FILE_PATH = GAME_PATH + getBuildingPicConstructedPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
		}
	}
	for (auto& i : level.buildingPicConstructed)
	{
		if (i.buildingPicPath != nullptr && i.picHash != 0)
		{
			FILE_PATH = i.buildingPicPath;
			return FILE_PATH.data();
		}
	}
	gameHelpers::logStringGame("getBrowserPicConstructed error: " + std::string(modPath + picPath));
	FILE_PATH = modPath + "/data/ui/generic/generic_constructed_building.tga";
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	FILE_PATH = GAME_PATH + "/data/ui/generic/generic_constructed_building.tga";
	return FILE_PATH.data();
}

char* __fastcall patchesForGame::getBrowserPicConstruction(int cultureID, edbEntry* entry, int buildingLevel)
{
	const auto level = entry->levels[buildingLevel];
	const auto levelName = level.name;
	if (levelName == nullptr)
		return nullptr;
	const auto cultureName = cultures::getCultureName(cultureID);
	if (cultureName.empty())
		return nullptr;
	const auto picPath = getBuildingPicConstructionPath(cultureName.c_str(), levelName);
	const auto modPath = gameHelpers::getModPath();
	FILE_PATH = modPath + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	if (GAME_PATH.empty())
		GAME_PATH = getGamePath(modPath);
	FILE_PATH = GAME_PATH + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	const int variantNum = edb->lookupVariants.size();
	for (int i = 0; i < variantNum; i++)
	{
		const auto variant = edb->lookupVariants.get(i);
		const auto variantNamesSize = variant->names.size();
		for (int j = 0; j < variantNamesSize; j++)
		{
			const auto variantName = variant->names.get(j);
			const auto variantString = variantName->name;
			FILE_PATH = modPath + getBuildingPicConstructionPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
			FILE_PATH = GAME_PATH + getBuildingPicConstructionPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
		}
	}
	for (auto& i : level.buildingPicConstruction)
	{
		if (i.buildingPicPath != nullptr && i.picHash != 0)
		{
			FILE_PATH = i.buildingPicPath;
			return FILE_PATH.data();
		}
	}
	gameHelpers::logStringGame("getBrowserPicConstruction error: " + std::string(modPath + picPath));
	FILE_PATH = modPath + "/data/ui/generic/generic_preconstructed_building.tga";
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	FILE_PATH = GAME_PATH + "/data/ui/generic/generic_preconstructed_building.tga";
	return FILE_PATH.data();
}

char* __fastcall patchesForGame::getBuildingPic(buildingLevel* level, int cultureID)
{
	const auto levelName = level->name;
	if (levelName == nullptr)
		return nullptr;
	const auto cultureName = cultures::getCultureName(cultureID);
	if (cultureName.empty())
		return nullptr;
	const auto picPath = getBuildingPicPath(cultureName.c_str(), levelName);
	const auto modPath = gameHelpers::getModPath();
	FILE_PATH = modPath + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	if (GAME_PATH.empty())
		GAME_PATH = getGamePath(modPath);
	FILE_PATH = GAME_PATH + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	const int variantNum = edb->lookupVariants.size();
	for (int i = 0; i < variantNum; i++)
	{
		const auto variant = edb->lookupVariants.get(i);
		const auto variantNamesSize = variant->names.size();
		for (int j = 0; j < variantNamesSize; j++)
		{
			const auto variantName = variant->names.get(j);
			const auto variantString = variantName->name;
			FILE_PATH = modPath + getBuildingPicPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
			FILE_PATH = GAME_PATH + getBuildingPicPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
		}
	}
	for (auto& i : level->buildingPic)
	{
		if (i.buildingPicPath != nullptr && i.picHash != 0)
		{
			FILE_PATH = i.buildingPicPath;
			return FILE_PATH.data();
		}
	}
	gameHelpers::logStringGame("getBuildingPic error: " + std::string(modPath + picPath));
	FILE_PATH = modPath + "/data/ui/generic/generic_building.tga";
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	FILE_PATH = GAME_PATH + "/data/ui/generic/generic_building.tga";
	return FILE_PATH.data();
}

char* __fastcall patchesForGame::getBuildingPicConstructed(buildingLevel* level, int cultureID)
{
	const auto levelName = level->name;
	if (levelName == nullptr)
		return nullptr;
	const auto cultureName = cultures::getCultureName(cultureID);
	if (cultureName.empty())
		return nullptr;
	const auto picPath = getBuildingPicConstructedPath(cultureName.c_str(), levelName);
	const auto modPath = gameHelpers::getModPath();
	FILE_PATH = modPath + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	if (GAME_PATH.empty())
		GAME_PATH = getGamePath(modPath);
	FILE_PATH = GAME_PATH + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	const int variantNum = edb->lookupVariants.size();
	for (int i = 0; i < variantNum; i++)
	{
		const auto variant = edb->lookupVariants.get(i);
		const auto variantNamesSize = variant->names.size();
		for (int j = 0; j < variantNamesSize; j++)
		{
			const auto variantName = variant->names.get(j);
			const auto variantString = variantName->name;
			FILE_PATH = modPath + getBuildingPicConstructedPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
			FILE_PATH = GAME_PATH + getBuildingPicConstructedPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
		}
	}
	for (auto& i : level->buildingPicConstructed)
	{
		if (i.buildingPicPath != nullptr && i.picHash != 0)
		{
			FILE_PATH = i.buildingPicPath;
			return FILE_PATH.data();
		}
	}
	gameHelpers::logStringGame("getBuildingPicConstructed error: " + std::string(modPath + picPath));
	FILE_PATH = modPath + "/data/ui/generic/generic_constructed_building.tga";
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	FILE_PATH = GAME_PATH + "/data/ui/generic/generic_constructed_building.tga";
	return FILE_PATH.data();
}

char* __fastcall patchesForGame::getBuildingPicConstruction(buildingLevel* level, int cultureID)
{
	const auto levelName = level->name;
	if (levelName == nullptr)
		return nullptr;
	const auto cultureName = cultures::getCultureName(cultureID);
	if (cultureName.empty())
		return nullptr;
	const auto picPath = getBuildingPicConstructionPath(cultureName.c_str(), levelName);
	const auto modPath = gameHelpers::getModPath();
	FILE_PATH = modPath + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	if (GAME_PATH.empty())
		GAME_PATH = getGamePath(modPath);
	FILE_PATH = GAME_PATH + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	const int variantNum = edb->lookupVariants.size();
	for (int i = 0; i < variantNum; i++)
	{
		const auto variant = edb->lookupVariants.get(i);
		const auto variantNamesSize = variant->names.size();
		for (int j = 0; j < variantNamesSize; j++)
		{
			const auto variantName = variant->names.get(j);
			const auto variantString = variantName->name;
			FILE_PATH = modPath + getBuildingPicConstructionPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
			FILE_PATH = GAME_PATH + getBuildingPicConstructionPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
		}
	}
	for (auto& i : level->buildingPicConstruction)
	{
		if (i.buildingPicPath != nullptr && i.picHash != 0)
		{
			FILE_PATH = i.buildingPicPath;
			return FILE_PATH.data();
		}
	}
	gameHelpers::logStringGame("getBuildingPicConstruction error: " + std::string(modPath + picPath));
	FILE_PATH = modPath + "/data/ui/generic/generic_preconstructed_building.tga";
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	FILE_PATH = GAME_PATH + "/data/ui/generic/generic_preconstructed_building.tga";
	return FILE_PATH.data();
}

portraitDbEntry* patchesForGame::getPortraitDbEntry(int cultureID)
{
	if (cultureID < 7)
	{
		const auto portraitDatabase = reinterpret_cast<portraitDb*>(dataOffsets::offsets.portraitDatabase);
		return &portraitDatabase->cultures[cultureID];
	}
	return cultures::eopPortraitDb::getEntry(cultureID);
}

char* patchesForGame::onGetGuildOfferPic(DWORD level, int cultureID)
{
	const DWORD lvlAddr = level - 8;
	const auto lvl = reinterpret_cast<buildingLevel*>(lvlAddr);
	const auto levelName = lvl->name;
	if (levelName == nullptr)
		return nullptr;
	const auto cultureName = cultures::getCultureName(cultureID);
	if (cultureName.empty())
		return nullptr;
	const auto picPath = getBuildingPicConstructedPath(cultureName.c_str(), levelName);
	const auto modPath = gameHelpers::getModPath();
	FILE_PATH = modPath + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	if (GAME_PATH.empty())
		GAME_PATH = getGamePath(modPath);
	FILE_PATH = GAME_PATH + picPath;
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	const int variantNum = edb->lookupVariants.size();
	for (int i = 0; i < variantNum; i++)
	{
		const auto variant = edb->lookupVariants.get(i);
		const auto variantNamesSize = variant->names.size();
		for (int j = 0; j < variantNamesSize; j++)
		{
			const auto variantName = variant->names.get(j);
			const auto variantString = variantName->name;
			FILE_PATH = modPath + getBuildingPicConstructedPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
			FILE_PATH = GAME_PATH + getBuildingPicConstructedPath(variantString, levelName);
			if (std::filesystem::exists(FILE_PATH))
				return FILE_PATH.data();
		}
	}
	for (auto& i : lvl->buildingPicConstructed)
	{
		if (i.buildingPicPath != nullptr && i.picHash != 0)
		{
			FILE_PATH = i.buildingPicPath;
			return FILE_PATH.data();
		}
	}
	gameHelpers::logStringGame("onGetGuildOfferPic error: " + std::string(modPath + picPath));
	FILE_PATH = modPath + "/data/ui/generic/generic_preconstructed_building.tga";
	if (std::filesystem::exists(FILE_PATH))
		return FILE_PATH.data();
	FILE_PATH = GAME_PATH + "/data/ui/generic/generic_constructed_building.tga";
	return FILE_PATH.data();
}

unit** __fastcall patchesForGame::onGetUnitByLabel(DWORD unitLabels, char* label)
{
	if (const DWORD value = **reinterpret_cast<DWORD**>(label); value >= 0x01308DE4 && value <= 0x0135106C)
		return reinterpret_cast<unit**>(label);
	
	return GAME_FUNC(unit**(__thiscall*)(DWORD, char*), getUnitByLabel2)(unitLabels, label);
}

//12FFA84
unitGroup** __fastcall patchesForGame::onGetGroupByLabel(DWORD groupLabels, char* label)
{
	if (const DWORD value = **reinterpret_cast<DWORD**>(label); value == 0x12FFA84 || value == 0x01344AA4)
		return reinterpret_cast<unitGroup**>(label);
	
	return GAME_FUNC(unitGroup**(__thiscall*)(DWORD, char*), getGroupByLabel)(groupLabels, label);
}

std::string sound;

char* patchesForGame::onGetCultureEndTurnSound(int cultureID)
{
	if (cultureID == 0)
		sound = "END_TURN";
	else
		sound = "END_TURN_CULTURE_" + to_string(cultureID);
	return sound.data();
}

int __fastcall patchesForGame::onCreateMercUnitCheck(char** entryName, int eduIndex)
{
	if (eduIndex == -1)
	{
		if (!eopDu::getEopEduEntry(eduIndex))
			return -1;
		return 0;
	}
	return eduIndex;
}

int __fastcall patchesForGame::onAttackGate(unit* un, void* tactic)
{
	const auto eduEntry = un->eduEntry;
	if (eduEntry == nullptr)
		return 0;
	if (eduEntry->mountedEngine)
	{
		const auto building = *reinterpret_cast<buildingBattle**>(reinterpret_cast<DWORD>(tactic) + 0x2AD0);
		if (building == nullptr)
			return 0;
		unitActions::attackBuilding(un, building);
		return 1;
	}
	return 0;
}

eduEntry* __fastcall patchesForGame::onCreateMercUnit(char** entryName, eduEntry* entry)
{
	const DWORD entryAddr = reinterpret_cast<DWORD>(entry);
	if (const DWORD mercEopValue = codes::offsets.mercEOPValue; entryAddr == mercEopValue)
	{
		eduEntry* eopEntry = eopDu::getEopEduEntryByName(*entryName);
		return eopEntry;
	}
	return entry;
}

eduEntry* __fastcall patchesForGame::onCreateUnitWrapper(int eduIndexBase, int removeValue)
{
	const int eduIndex = eduIndexBase - (removeValue * 8);
	eduEntry* entry = eopDu::getEduEntry(eduIndex);
	return entry;
}

DWORD __fastcall patchesForGame::onUnitInfo(DWORD entryAddress)
{
	DWORD eduBaseAddress = dataOffsets::offsets.unitTypesStart;
	int index = (entryAddress - eduBaseAddress) / 996; // 996 is the size of the eduEntry struct
	if (index < 500)
	{
		return entryAddress;
	}
	auto entry = eopDu::getEopEduEntry(index);
	return reinterpret_cast<DWORD>(entry);
}

eduEntry* __fastcall patchesForGame::onGetRecruitPoolUnitEntry(int eduIndex)
{
	return eopDu::getEduEntry(eduIndex);
}

DWORD getVFunc(DWORD* addr, DWORD offset)
{
	DWORD vtbl = reinterpret_cast<DWORD>(addr);
	return *reinterpret_cast<DWORD*>(vtbl + offset);
}

enum class groupMoveType
{
	formedStatic,
	formed,						
	unformedUnitsFormed,		
	unformedUnitsUnformed,	
};


float distance(const float x, const float y, const float x2, const float y2)
{
	return static_cast<float>(sqrt(pow(x - x2, 2) + pow(y - y2, 2)));
}

void __fastcall patchesForGame::onPreBattlePlacement(aiTacticAssault* aiTactic)
{
	if (const auto battle = battleHelpers::getBattleData(); !battle || battle->battleState >= 5)
		return;
	const DWORD vFunc = getVFunc(aiTactic->vftable, 0x4C);
	if (const int tacticType = GAME_FUNC_RAW(int(__thiscall*)(aiTacticAssault*), vFunc)(aiTactic); tacticType != 19)
		return;
	const auto group1 = &aiTactic->aiUnitGroup;
	const auto group2 = &aiTactic->siegeUnitGroup;
	const DWORD funcAddr = codes::offsets.issueMoveOrder;
	GAME_FUNC_RAW(bool(__thiscall*)(aiUnitGroup*, float*, int16_t, int, bool, bool), funcAddr)(
		group1,
		&aiTactic->advanceX,
		aiTactic->angle,
		static_cast<int>(groupMoveType::formed),
		true,
		true);
	for (int i = 0; i < group1->unitsInFormationNum; i++)
	{
		if (const auto unit = group1->unitsInFormation[i];
			distance(unit->positionX, unit->positionY, aiTactic->advanceX, aiTactic->advanceY) > 150)
			unitActions::placeUnit(unit, aiTactic->advanceX, aiTactic->advanceY, aiTactic->angle, 0);
	}
	GAME_FUNC_RAW(bool(__thiscall*)(aiUnitGroup*, float*, int16_t, int, bool, bool), funcAddr)(
		group2,
		&aiTactic->advanceX,
		aiTactic->angle,
		static_cast<int>(groupMoveType::formed),
		true,
		true);
	for (int i = 0; i < group2->unitsInFormationNum; i++)
	{
		if (const auto unit = group2->unitsInFormation[i];
			distance(unit->positionX, unit->positionY, aiTactic->advanceX, aiTactic->advanceY) > 150)
			unitActions::placeUnit(unit, aiTactic->advanceX, aiTactic->advanceY, aiTactic->angle, 0);
	}
}

bool __fastcall patchesForGame::onDecideRamAttacks(buildingBattle* gate, aiDetachment* detachment, int numRamsLeft)
{
	if (numRamsLeft <= 0)
		return false;
	int numLaddersTowers = 0;
	int numRams = 0;
	int infCavNum = 0;
	for (int i = 0; i < detachment->aiDetachUnitsCount; i++)
	{
		auto unit = detachment->aiDetachUnits[i].unit;
		if (unit->eduEntry->category != unitCategory::siege
			&& unit->eduEntry->unitClass != unitClass::missile
			&& unit->eduEntry->unitClass != unitClass::skirmish)
			infCavNum++;
		if (unit->eduEntry->category != unitCategory::infantry)
			continue;
		if (unit->siegeEnNum == 0)
			continue;
		auto engine = unit->siegeEngines[0];
		if (engine == nullptr)
			continue;
		if (engine->engineRec->classID == engineType::ram)
			numRams++;
		if (engine->engineRec->classID == engineType::ladder || engine->engineRec->classID == engineType::tower)
			numLaddersTowers++;
	}
	auto battleData = battleHelpers::getBattleData();
	battleSide* attacker = nullptr;
	battleSide* defender = nullptr;
	for (int i = 0; i < battleData->sidesNum; i++)
	{
		auto side = battleData->sides[i];
		if (side.isDefender)
			defender = &side;
		else
			attacker = &side;
	}
	auto defenderArmy = defender->armies[0].stack;
	int infCavNumDef = 0;
	if (defenderArmy)
	{
		for (int i = 0; i < defenderArmy->numOfUnits; i++)
		{
			if (auto unit = defenderArmy->units[i];
				unit->eduEntry->category != unitCategory::siege
			&& unit->eduEntry->unitClass != unitClass::missile
			&& unit->eduEntry->unitClass != unitClass::skirmish)
				infCavNumDef++;
		}
	}
	auto battleSideArmy = attacker->armies[0];
	auto deployArea = &battleSideArmy.deploymentAreas.get(0)->area;
	auto areaX = deployArea->centreX;
	auto areaY = deployArea->centreY;
	int bonus = std::clamp(infCavNum - infCavNumDef, -3, 3);
	int ramsNeeded = (infCavNum - numLaddersTowers) / (4 - bonus);
	std::vector<buildingBattle*> gates{};
	auto buildings = gate->battleResidence->battleBuildings;
	for (int i = 0; i < buildings->allBuildingsNum; i++)
	{
		if (auto building = buildings->allBuildings[i]; building->type == 3)
			gates.push_back(building);
	}
	std::sort(gates.begin(), gates.end(), [areaX, areaY](buildingBattle* a, buildingBattle* b)
		{
			return distance(a->xCoord, a->yCoord, areaX, areaY) < distance(b->xCoord, b->yCoord, areaX, areaY);
		});
	if (gate == gates[0])
		return true;
	int gateNum = static_cast<int>(gates.size());
	ramsNeeded = std::clamp(ramsNeeded, 1, gateNum);
	if (int ramsUsed = numRams - numRamsLeft; ramsUsed >= ramsNeeded)
		return false;
	if (ramsNeeded < gateNum)
	{
		for (int i = 0; i < gateNum - ramsNeeded; i++)
			gates.pop_back();
	}
	bool isCloseEnough = false;
	for (int i = 0; i < deployArea->coordsNum; i++)
	{
		if (distance(deployArea->coordsPairs[i].xCoord, deployArea->coordsPairs[i].yCoord, gate->xCoord, gate->yCoord) < 200)
			isCloseEnough = true;
		if (!gates.empty() && distance(gates[0]->xCoord, gates[0]->yCoord, gate->xCoord, gate->yCoord) < 125)
			isCloseEnough = true;
		if (isCloseEnough)
			break;
	}
	if (!isCloseEnough)
		return false;
	if (std::any_of(gates.begin(), gates.end(), [gate](const buildingBattle* gt){return gate == gt;}))
		return true;
	return false;
}

bool __thiscall patchesForGame::onPreBattlePlacement2(aiUnitGroup* group, DWORD formationTemplate, bool forceOrder)
{
	DWORD orderChangeFormation = codes::offsets.issueFormationOrder;
	auto retBool = GAME_FUNC_RAW(bool(__thiscall*)(aiUnitGroup*, DWORD, bool), orderChangeFormation)(group, formationTemplate, forceOrder);
	auto battle = battleHelpers::getBattleData();
	if (battle->battleType != 3)
		return retBool;
	if (battle->battleState >= 5)
		return retBool;
	auto tactic = group->detachmentTactic;
	if (!tactic)
		return retBool;
	DWORD vFunc = getVFunc(tactic->vftable, 0x4C);
	int tacticType = GAME_FUNC_RAW(int(__thiscall*)(aiDetachmentTactic*), vFunc)(tactic);
	if (tacticType != 19)
		return retBool;
	auto aiTactic = reinterpret_cast<aiTacticAssault*>(tactic);
	float posX = aiTactic->building->xCoord;
	float posY = aiTactic->building->yCoord;
	aiTactic->advanceX = posX;
	aiTactic->advanceY = posY;
	auto group1 = aiTactic->aiUnitGroup;
	auto angle = unitHelpers::angleShortToFloat(aiTactic->angle);
	for (int i = 0; i < group1.unitsInFormationNum; i++)
	{
		auto unit = group1.unitsInFormation[i];
		unitActions::placeUnit(unit, posX, posY, aiTactic->angle, 0);
		gameHelpers::logStringGame("Unit placed at2: " + to_string(unit->positionX) + " " + to_string(unit->positionY) + " " + to_string(angle));
		group1.xCoord = unit->positionX;
		group1.yCoord = unit->positionY;
		group1.angle = aiTactic->angle;
	}
	auto group2 = aiTactic->siegeUnitGroup;
	for (int i = 0; i < group2.unitsInFormationNum; i++)
	{
		auto unit = group2.unitsInFormation[i];
		unitActions::placeUnit(unit, posX, posY, aiTactic->angle, 0);
		group2.xCoord = unit->positionX;
		group2.yCoord = unit->positionY;
		group2.angle = aiTactic->angle;
		gameHelpers::logStringGame("Siege unit placed at2: " + to_string(unit->positionX) + " " + to_string(unit->positionY) + " " + to_string(angle));
	}
	return retBool;
}

const char* __fastcall patchesForGame::onQuickSave()
{
	static std::vector<std::string> saveNames = { u8"%S-1.sav" ,u8"%S-2.sav", u8"%S-3.sav" };
	jsn::json json;

	std::string fPath = globals::dataS.modPath;
	fPath += "\\saves\\quickSavesM2TWEOP.json";

	int currSaveID = 0;
	int maxSaveID = 2;
	try
	{
		std::ifstream f1(fPath);
		if (f1.is_open())
		{
			f1 >> json;

			f1.close();

			json.at("saveID").get_to(currSaveID);

			if (currSaveID + 1 > maxSaveID)
			{
				json["saveID"] = 0;
			}
			else
			{
				json["saveID"] = currSaveID + 1;
			}
			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
		else
		{
			currSaveID = 0;
			json["saveID"] = 0;

			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
	}
	catch (jsn::json::type_error& e)
	{
		currSaveID = 0;
		json["saveID"] = 0;

		std::ofstream f2(fPath);

		f2 << json;

		f2.close();
	}

	return saveNames[currSaveID].c_str();
}
const char* __fastcall patchesForGame::onAutoSave()
{
	static std::vector<std::string> saveNames = { u8"%s%S%s%S-1.sav" ,u8"%s%S%s%S-2.sav", u8"%s%S%s%S-3.sav",
		 u8"%s%S%s%S-4.sav" ,u8"%s%S%s%S-5.sav", u8"%s%S%s%S-6.sav"
	};
	jsn::json json;

	std::string fPath = globals::dataS.modPath;
	fPath += "\\saves\\autoSavesM2TWEOP.json";

	int currSaveID = 0;
	int maxSaveID = 5;
	try
	{
		std::ifstream f1(fPath);
		if (f1.is_open())
		{
			f1 >> json;

			f1.close();

			json.at("saveID").get_to(currSaveID);

			if (currSaveID + 1 > maxSaveID)
			{
				json["saveID"] = 0;
			}
			else
			{
				json["saveID"] = currSaveID + 1;
			}
			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
		else
		{
			currSaveID = 0;
			json["saveID"] = 0;

			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
	}
	catch (jsn::json::type_error& e)
	{
		currSaveID = 0;
		json["saveID"] = 0;

		std::ofstream f2(fPath);

		f2 << json;

		f2.close();
	}

	return saveNames[currSaveID].c_str();
}
character* __fastcall patchesForGame::mercenaryMovePointsGetGeneral(armyStruct* army)
{
	character* gen = army->gen;
	if (gen == nullptr)
	{
		if (army->settlement != nullptr)
		{
			fortStruct* ourFort = reinterpret_cast<fortStruct*>(army->settlement);
			const auto nextObject = ourFort->nextObject;
			if (nextObject == nullptr)
				return nullptr;
			if (const int objectType = callVFunc<4, int>(nextObject); objectType == 4)
			{
				gen = static_cast<character*>(nextObject);
			}
		}

	}
	return gen;
}
void __fastcall patchesForGame::clickAtTile(coordPair* xy)
{
	gameEvents::onClickAtTile(xy->xCoord, xy->yCoord);
	plannedRetreatRoute::onClickAtTile(xy->xCoord, xy->yCoord);
}
void __stdcall patchesForGame::afterCampaignMapLoaded()
{
	discordManager::onCampaignMapLoaded();
	globals::dataS.Modules.tacticalMapViewer.unView();
	gameEvents::onCampaignMapLoaded();
}

void __stdcall patchesForGame::onNewGameStart()
{
	eopSettlementDataDb::get()->clearData();
	gameEvents::onNewGameStart();
	plannedRetreatRoute::onNewGameStart();
}

//#define TESTPATCHES
void __stdcall patchesForGame::onEduParsed()
{
	unitHelpers::initBaseUnitsLookup();
	gameEvents::onReadGameDbsAtStart();
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterEDUread" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::onGameInit()
{
	cultures::eopPortraitDb::createEopPortraitDb();
	gameEvents::onGameInit();
}

void __stdcall patchesForGame::onUnloadCampaign()
{
	gameEvents::onUnloadCampaign();
}

void __stdcall patchesForGame::onNewGameLoaded()
{
	plugData::data.luaAll.fillHashMaps();
	gameEvents::onNewGameLoaded();
}

void __fastcall patchesForGame::onAiTurn(aiFaction* aiFac)
{
	gameEvents::onAiTurn(aiFac);
}

void __stdcall patchesForGame::onChangeTurnNum()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onChangeTurnNum" << endl;
	f1.close();
#endif
	const int num = campaignHelpers::getCampaignData()->turnNumber;
	discordManager::onChangeTurnNum(num);
	gameEvents::onChangeTurnNum(num);
}

void __stdcall patchesForGame::onGiveTrait()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onGiveTrait" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::afterLoadGameData()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterLoadGameData" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::afterI_CompareCounter()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterI_CompareCounter" << endl;
	f1.close();
#endif
}

int __stdcall patchesForGame::checkFort()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkFort" << endl;
	f1.close();
#endif
	return 0;
}

int __stdcall patchesForGame::checkDipString()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkDipString" << endl;
	f1.close();
#endif
	return 0;
}

int __stdcall patchesForGame::checkFacDip()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkFacDip" << endl;
	f1.close();
#endif
	return 0;
}

int __stdcall patchesForGame::spawnArmyCoords()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "spawnArmyCoords" << endl;
	f1.close();
#endif
	return 0;
}

void __stdcall patchesForGame::spawnEndArmyCoords()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "spawnEndArmyCoords" << endl;
	f1.close();
#endif
}

int __stdcall patchesForGame::spawnCharacterCoords()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "spawnCharacterCoords" << endl;
	f1.close();
#endif
	return 0;
}

void __stdcall patchesForGame::onBattleStratScreen()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onBattleStratScreen" << endl;
	f1.close();
#endif
	//battleCreator::onBattleStratScreen();

}

void __fastcall patchesForGame::onEvent(DWORD** vTab, DWORD arg2)
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onEvent" << endl;
	f1.close();
#endif

	const DWORD eventCode = reinterpret_cast<DWORD>(vTab[0]);
	gameEvents::onEventWrapper(eventCode, vTab);

	const int gameVersion = gameHelpers::getGameVersion();
	const DWORD scrollOpenedCode = gameVersion == 1 ? 0x013719FC : 0x0132C9D4;
	const DWORD factionTurnStartCode = gameVersion == 1 ? 0x0136931C : 0x013242F4;
	const DWORD gameReloaded = gameVersion == 1 ? 0x013319E4 : 0x012EC9C4;
	if (eventCode == scrollOpenedCode)
	{
		char* str = reinterpret_cast<char*>(vTab[1]);
		if (strcmp(str, "prebattle_scroll") == 0)
		{
			battleCreator::onBattleStratScreen();
		}
		else if (strcmp(str, "post_battle_scroll") == 0)
		{
			battleCreator::onPostBattleStratScreen();
		}
		else if (strcmp(str, "hotseat_scroll") == 0)
		{
			battleCreator::onHotseatScreen();
		}
	}
	else if (eventCode == factionTurnStartCode)
	{
		factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
		discordManager::onFactionTurnStart(fac);
		plannedRetreatRoute::onFactionTurnStart(fac);
	}
	else if (eventCode == gameReloaded)
	{
		eopSettlementDataDb::get()->onGameLoaded();
	}
}

void __fastcall patchesForGame::onLoadSaveFile(UNICODE_STRING**& savePath)
{
	const string relativePath = techFuncs::uniToANSI(savePath);
	vector<string> files = techFuncs::getEopArchiveFiles(relativePath);
	if (files.empty())
	{
		onSaveGame(savePath);
		vector<string> newFiles = techFuncs::getEopArchiveFiles(relativePath);
		for (string& path : newFiles)
		{
			files.push_back(path);
		}
	}
	files = techFuncs::loadGameLoadArchive(files, savePath);
	gameEvents::onLoadGamePl(&files);
	plannedRetreatRoute::onGameLoad(files);
	eopSettlementDataDb::get()->onGameLoad(files);

	techFuncs::deleteFiles(files);
}

void __fastcall patchesForGame::onSaveGame(UNICODE_STRING**& savePath)
{
	vector<string>files;
	vector<string>* plugFiles = gameEvents::onSaveGamePl(savePath);
	for (string& path : *plugFiles)
	{
		files.push_back(path);
	}

	delete plugFiles;
	if (const std::string retreatsFile = plannedRetreatRoute::onGameSave(); !retreatsFile.empty())
		files.push_back(retreatsFile);
	if (const std::string settlementData = eopSettlementDataDb::get()->onGameSave(); !settlementData.empty())
		files.push_back(settlementData);
	techFuncs::saveGameMakeArchive(savePath, files);
	techFuncs::deleteFiles(files);
}

void __fastcall patchesForGame::onTileCheck(int* coords)
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onTileCheck" << endl;
	f1.close();
#endif
	if (coords == nullptr)
	{
		MessageBoxA(NULL, "Error in onTileCheck", "Attention", NULL);
		return;
	}

	tilesChange::onTilesCheck(coords);
}

void __stdcall patchesForGame::readTilesFile()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "readTilesFile" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::unicalStratModsRead()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "unicalStratModsRead" << endl;
	f1.close();
#endif
	stratModelsChange::loadModels();
}

void __stdcall patchesForGame::loadCharStratModels()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "loadCharStratModels" << endl;
	f1.close();
#endif
	stratModelsChange::loadCharModels();
}


void __stdcall patchesForGame::checkAndChangeModels()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkAndChangeModels" << endl;
	f1.close();
#endif
	stratModelsChange::checkAndChangeStratModels();
}

void __stdcall patchesForGame::battleLoaded()
{
	discordManager::battleLoaded();
	battleCreator::startCreationThread();
}

void __stdcall patchesForGame::toReadGameDbsAtStart()
{
	//	plugins::onReadGameDbsAtStart();
}

void __stdcall patchesForGame::onDrawPartsOfStratObjects()
{
	graphicsD3D::onDrawPartsOfStratObjects();
}

void __stdcall patchesForGame::onDrawAll()
{
	graphicsD3D::onDrawAllGameStuff();
}

void __stdcall patchesForGame::onStartOfDrawFunction()
{
	stratModelsChange::update();
}

void __stdcall patchesForGame::onRetreat()
{
	plannedRetreatRoute::onRetreat();
}

void __fastcall patchesForGame::onStopCharacter(character* character)
{
	if (const auto campaign = campaignHelpers::getCampaignData(); campaign->humanPlayers < 2)
		character->isStopCharacterNeeded = 1;
}

eduEntry* __fastcall patchesForGame::recruitEopUnit(int eduIndex)
{
	eduEntry* entry = eopDu::getEduEntry(eduIndex);
	if (entry == nullptr)
	{
		entry = eopDu::getEopEduEntry(eduIndex);
	}
	return entry;
}

void __fastcall patchesForGame::recruitEopUnit2(int eduIndex)
{
	eduEntry* entry = eopDu::getEduEntry(eduIndex);
	if (entry == nullptr)
	{
		entry = eopDu::getEopEduEntry(eduIndex);
	}
	if (entry)
	{
		entry->unitCreatedCounter++;
	}
}

eduEntry* __fastcall patchesForGame::onReadDescrRebel(DWORD value)
{
	const DWORD eduIndex = (value * 4) / 996;
	return eopDu::getEduEntry(eduIndex);
}

void __fastcall patchesForGame::recruitEopMercUnit(DWORD pad, DWORD pad2, regionStruct* region, int eduIndex, int factionID, int exp)
{
	int regionID = region->regionID;
	unitHelpers::createUnitIdx(eduIndex, regionID, factionID, exp, 0, 0);
}

void WINAPI patchesForGame::onMoveRecruitQueue()
{
	if (const auto campaign = campaignHelpers::getCampaignData(); campaign->humanPlayers < 2)
		return;
	MessageBoxA(nullptr, "Moving queue disabled in hotseat mode. EXIT NOW!!!!!", "ATTENTION!", NULL);
	std::terminate();
}

void __fastcall patchesForGame::onEndSiege(settlementStruct* sett)
{
	const int x = sett->xCoord;
	const int y = sett->yCoord;
	gameEvents::onEndSiege(x, y);
}

void __fastcall patchesForGame::onStartSiege(settlementStruct* sett)
{
	if (sett->siegeNum == 0)
	{
		return;
	}
	const int x = sett->xCoord;
	const int y = sett->yCoord;
	gameEvents::onStartSiege(x, y);
}

void __fastcall patchesForGame::onLoadDescrBattleCharacter(armyStruct* army, character* goalGen)
{
	characterHelpers::setBodyguard(goalGen, army->units[0]);//we replace game function what set army leader character.

	std::string relativePath = techFuncs::uniToANSI(campaignHelpers::getCampaignData()->currentDescrFile);

	if (relativePath.find("battle") != std::string::npos)
	{
		battleCreator::addCharactersToCustomBattleArmy(army, relativePath);
	}
	//general*newGen=fastFuncts::createCharacter("named character",army->faction,25,"testGen", "testGen",0,nullptr, 0,0);
}

void __stdcall patchesForGame::onBattleStateChange()
{
	//results screen
	if (battleHelpers::getBattleData()->battleState == 9)
	{
		battleCreator::onBattleResultsScreen();
	}
}
/*
int* __fastcall patchesForGame::ontryFindTypeIdInListRecruitPoolEDB(char* unitTypeString)
{
	return  eduThings::tryFindDataEopEduIndex(unitTypeString);
}

EduEntry* __fastcall patchesForGame::onrecruitPoolFillFromFile(int eduIndex)
{
	EduEntry*retEntry =	fastFunctsHelpers::getEDUEntryById(eduIndex);
	if (retEntry != nullptr)
	{
		return retEntry;
	}

	return eduThings::getEopEduEntry(eduIndex);
}
*/

struct
{
	bool isComingFromConsole = false;
}logonConsoleD;
void __stdcall consolePatches::onGameConsoleCommandFromConsole()
{
	logonConsoleD.isComingFromConsole = true;
}

void __stdcall consolePatches::onGameConsoleCommandFromScript()
{
	logonConsoleD.isComingFromConsole = false;
}

int __fastcall consolePatches::onReadLogonOrLogoff(int isLogonNow)
{
	if (logonConsoleD.isComingFromConsole == true)
	{
		return isLogonNow;
	}
	return true;
}
