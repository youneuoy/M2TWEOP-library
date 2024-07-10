#include "pch.h"
#include "patchesForGame.h"
#include "tilesChange.h"
#include "graphicsD3D.h"


#include "events.h"
#include "fort.h"
#include "battle.h"
#include "cultures.h"
#include "eopBuildings.h"
#include "onlineThings.h"
#include "character.h"
#include "faction.h"
#include "characterRecord.h"
#include "strategyMap.h"
#include "campaignAi.h"

#include "plannedRetreatRoute.h"
#include "discordManager.h"
#include "eopdu.h"
#include "unit.h"
#include "army.h"
#include "campaign.h"
#include "gameUi.h"
#include "rebelFactions.h"


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
			selectedWorld = globals::dataS.Modules.developerMode.selectWorldpkgdesc(selectRecordS, selectRecordG);
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

aiProductionController* __fastcall patchesForGame::onCreateProductionController(aiProductionController* controller, settlementStruct* sett)
{
	controller->settlement = sett;
	return controller;
}

void __fastcall patchesForGame::onDecideNeighbours(factionStruct* faction)
{
	faction->updateNeighbours();
}

portBuildingStruct* __fastcall patchesForGame::onTransferSettlementPort(const settlementStruct* settlement)
{
	if (settlement->isMinorSettlement)
		return nullptr;
	return settlement->port;
}

void __fastcall patchesForGame::onInitGsd(aiGlobalStrategyDirector* director)
{
	director->initialize();
}

void __fastcall patchesForGame::onInitGsd2(aiGlobalStrategyDirector* director)
{
	director->initialize2();
}

void __fastcall patchesForGame::onTransferSettlement(const settlementStruct* settlement, const int reason, factionStruct* faction)
{
	if (settlement->isMinorSettlement)
		return;
	const int watchTowerNum = stratMapHelpers::getRegion(settlement->regionID)->watchtowersNum;
	for (int i = 0; i < watchTowerNum; i++)
	{
		if (const auto watchTower = stratMapHelpers::getRegion(settlement->regionID)->watchtowers[i];
			!watchTower->blockingArmy)
		{
			GAME_FUNC(void(__thiscall*)(watchTowerStruct*, bool, factionStruct*, int)
				, changeWatchTowerFaction)(watchTower, reason != 1, faction, 0);
		}
	}
	GAME_FUNC(void(__thiscall*)(stratMap*, int, factionStruct*),
		setRegionFaction)(stratMapHelpers::getStratMap(), settlement->regionID, faction);
}

int __fastcall patchesForGame::onCheckConstructionItem(const buildingsQueue* queue, const buildingInQueue* constructionItem)
{
	const auto entry = constructionItem->edbEntry;
	if (queue->settlement->isMinorSettlement)
	{
		if (entry->isFarm || entry->isPort  || entry->isTemple || (entry->isHinterland && !entry->isCoreBuilding))
			return 1; 
	}
	return queue->conversion > 0 ? 1 : 0;
}

DWORD* __fastcall patchesForGame::onCreateTakeResidenceObjective(aiCampaignController* campaignController, DWORD* oldResidence)
{
	if (*reinterpret_cast<DWORD*>(campaignController) != dataOffsets::offsets.campaignControllerVtbl)
		return oldResidence;
	if (*oldResidence == dataOffsets::offsets.settlementVtbl)
		return reinterpret_cast<DWORD*>(campaignController->regionData->getSettlement());
	return oldResidence;
}

void __fastcall patchesForGame::initGlobalStrategyDirector(aiGlobalStrategyDirector* gsd)
{
	
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
	return eopDu::getEduEntry(eduIndex);
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

uint32_t __fastcall patchesForGame::onSetExtentsTexture(bool isEnemy)
{
	if (isEnemy)
		return extentColors::getEnemyColorHex();
	return extentColors::getOwnColorHex();
}

uint8_t __fastcall patchesForGame::onSetExtentsBorder(uint8_t isBorder, DWORD extentsDisplay)
{
	if (isBorder == 0)
		return 0;
	const DWORD extents = extentsDisplay - 0x24;
	const bool isEnemy = *reinterpret_cast<bool*>(extents + 0x80034);
	const bool border = isBorder == static_cast<uint8_t>(0xFF);
	if (isEnemy)
	{
		return border ? extentColors::getEnemyColor().borderAlpha : extentColors::getEnemyColor().alpha;
	}
	return border ? extentColors::getOwnColor().borderAlpha : extentColors::getOwnColor().alpha;
}

uint32_t __fastcall patchesForGame::onSetExtentsZoc()
{
	return extentColors::getZocColorHex();
}

uint8_t __fastcall patchesForGame::onSetExtentsZocAlpha(uint8_t oldAlpha)
{
	const float alphaDiff = oldAlpha / 200.0f;
	return extentColors::getZocColor().borderAlpha * alphaDiff;
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
		if (!eopDu::getEopEduEntryByName(*entryName))
			return -1;
		return 0;
	}
	return eduIndex;
}

void patchesForGame::onAttachRegionSettlement(settlementStruct* sett, int regionId)
{
	sett->regionID = regionId;
	const auto map = stratMapHelpers::getStratMap();
	const auto region = &map->regions[regionId];
	if ((region->settlement && region->settlement != sett) || sett->isMinorSettlement || sett->minorSettlementIndex > 0)
		return;
	region->settlement = sett;
	GAME_FUNC(void(__thiscall*)(stratMap*, int, factionStruct*),
		setRegionFaction)(map, sett->regionID, sett->faction);
	for (int i = 0; i < region->resourcesNum; i++)
	{
		GAME_FUNC(void(__thiscall*)(resourceStruct*, settlementStruct*),
			attachResourceSett)(region->resources[i], sett);
	}
}

void patchesForGame::onCalculateSettlement(settlementStruct* sett)
{
	if(!sett->isMinorSettlement)
		return;
	balanceMinorSettStats(&sett->stats.settlementStats, sett);
}

//Sally out fix, consider armies around settlement not defender
int* patchesForGame::onGetSupportingArmies(armyStruct* defender, armyStruct* attacker)
{
	if (!defender->siege || !attacker->settlement)
		return reinterpret_cast<int*>(defender->getCoordPair());
	
	const auto attSett = attacker->findInSettlement();
	if (const auto sett = defender->siege->getSiegedSettlement(); attSett && sett)
	{
		if (sett->xCoord == attSett->xCoord && sett->yCoord == attSett->yCoord)
			return reinterpret_cast<int*>(&sett->xCoord);
	}
	const auto attFort= attacker->findInFort();
	if (const auto fort = defender->siege->getSiegedFort(); attFort && fort)
	{
		if (fort->xCoord == attFort->xCoord && fort->yCoord == attFort->yCoord)
			return reinterpret_cast<int*>(&fort->xCoord);
	}
	
	return reinterpret_cast<int*>(defender->getCoordPair());
}

uint32_t makeColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

uint32_t patchesForGame::onDrawBanner(const trackedArmy* army)
{
	const auto stack = army->army;
	if (stack->bannerSet)
	{
		return makeColor(stack->bannerRed, stack->bannerGreen, stack->bannerBlue, 0xFF);
	}
	if (stack->descrRebel && stack->faction->factionID == campaignHelpers::getCampaignData()->slaveFactionID)
	{
		if (const auto rebelFac = eopRebelFactionDb::getRebelFaction(stack->descrRebel->getName()); rebelFac && rebelFac->bannerSet)
		{
			return makeColor(rebelFac->bannerRed, rebelFac->bannerGreen, rebelFac->bannerBlue, 0xFF);
		}
	}
	const auto factionRec = stack->faction->factionRecord;
	return makeColor(factionRec->primary_colour_red, factionRec->primary_colour_green, factionRec->primary_colour_blue, 0xFF);
}

bannerData* patchesForGame::onGetRebelSymbol(const trackedArmy* army, bannerData* oldData)
{
	const auto stack = army->army;
	if (!stack->descrRebel)
		return oldData;
	const auto rebelFac = eopRebelFactionDb::getRebelFaction(stack->descrRebel->getName());
	if (!rebelFac)
		return oldData;
	if (rebelFac->bannerSymbolSet)
		return rebelFac->banner.get();
	return oldData;
}

void patchesForGame::balanceMinorSettStats(settlementStats* stats, const settlementStruct* sett)
{
	if (const auto region = stratMapHelpers::getRegion(sett->regionID); region->factionOwner->factionID != sett->faction->factionID)
	{
		const auto facDip = campaignHelpers::getCampaignData()->getFactionDiplomacy(region->factionOwner->factionID, sett->faction->factionID);
		float modifierIncome = 0.0f;
		float modifierGrowth = 0.25f;
		if (facDip->state == dipStance::alliance)
		{
			modifierIncome += 0.30f;
			modifierGrowth += 0.40f;
		}
		else if (facDip->state < dipStance::war)
		{
			modifierIncome += 0.15f;
			modifierGrowth += 0.20f;
		}
		if (facDip->hasTradeRights)
		{
			modifierIncome += 0.30f;
			modifierGrowth += 0.15f;
		}
		stats->TradeIncome = static_cast<int>(round(stats->TradeIncome * modifierIncome));
		stats->FarmsIncome = static_cast<int>(round(stats->FarmsIncome * modifierIncome));
		stats->MiningIncome = static_cast<int>(round(stats->MiningIncome * modifierIncome));
		stats->PopGrowthFarms = static_cast<int>(round(stats->PopGrowthFarms * modifierGrowth));
		stats->PopGrowthBaseFarm = static_cast<int>(round(stats->PopGrowthBaseFarm * modifierGrowth));
		stats->PopGrowthTrade = static_cast<int>(round(stats->PopGrowthTrade * modifierGrowth));
	}
	else
	{
		stats->TradeIncome = static_cast<int>(round(stats->TradeIncome * 0.75f));
		stats->FarmsIncome = static_cast<int>(round(stats->FarmsIncome * 0.75f));
		stats->MiningIncome = static_cast<int>(round(stats->MiningIncome * 0.75f));
		stats->PopGrowthFarms = static_cast<int>(round(stats->PopGrowthFarms * 0.75f));
		stats->PopGrowthBaseFarm = static_cast<int>(round(stats->PopGrowthBaseFarm * 0.75f));
		stats->PopGrowthTrade = static_cast<int>(round(stats->PopGrowthTrade * 0.75f));
	}
	stats->TotalIncomeWithoutAdmin = stats->TradeIncome + stats->FarmsIncome + stats->MiningIncome + stats->TaxesIncome;
}

void patchesForGame::onPredictedStats(settlementStats* statsManager)
{
	if(!statsManager->settlement->isMinorSettlement)
		return;
	balanceMinorSettStats(statsManager, statsManager->settlement);
}

int patchesForGame::onEvalAttObjective(const aiCampaignController* controller)
{
	int factionId = controller->regionData->getTargetFaction()->factionID;
	if (factionId != controller->regionData->factionID || !controller->attacking)
		return factionId;
	factionId = controller->targetFactionId;
	if (factionId == controller->regionData->factionID && controller->attacking)
		factionId = campaignHelpers::getCampaignData()->slaveFactionID;
	return factionId;
}

void patchesForGame::onUpdateControllerAlloc(aiCampaignController* controller)
{
	controller->updateAllocation();
}

int patchesForGame::onScoreBestCapital(const settlementStruct* sett)
{
	if (sett->isMinorSettlement)
		return 0;
	return 1;
}

int patchesForGame::onAssessRequiredStrength(const aiRegionController* controller)
{
	if (!controller)
		return 0;
	return 1;
}

int patchesForGame::onCalcGarrisonStr(const aiRegionData* regData, const factionStruct* fac,
	const settlementStruct* sett)
{
	return campaignAi::assessGarrisonStrength(regData, sett, fac);
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
	return unitHelpers::getEduEntryByName(*entryName);
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
		if (const auto unit = group1->unitsInFormation[i]; unit && distance(unit->positionX, unit->positionY, aiTactic->advanceX, aiTactic->advanceY) > 500)
			unitActions::placeUnit(unit, aiTactic->advanceX, aiTactic->advanceY, aiTactic->angle, 0);
	}
	GAME_FUNC_RAW(bool(__thiscall*)(aiUnitGroup*, float*, int16_t, int, bool, bool), funcAddr)(
		group2,
		&aiTactic->advanceX,
		aiTactic->angle,
		static_cast<int>(groupMoveType::formed),
		true,
		true);
}

std::shared_ptr<std::vector<int>> USED_GATES = std::make_shared<std::vector<int>>();
aiDetachment* LAST_DETACHMENT = nullptr;
int GATES_IGNORED = 0;

bool __fastcall patchesForGame::onDecideRamAttacks(buildingBattle* gate, aiDetachment* detachment, int numRamsLeft)
{
	if (detachment != LAST_DETACHMENT)
	{
		GATES_IGNORED = 0;
		LAST_DETACHMENT = detachment;
	}
	if (numRamsLeft + GATES_IGNORED <= 0)
		return false;
	for (auto usedGate : *USED_GATES)
	{
		if (gate->buildingIndex == usedGate)
		{
			GATES_IGNORED++;
			gameHelpers::logStringGame("Gate already used: " + std::to_string(usedGate));
			return false;
		}
	}
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
	auto battleSideArmy = battleData->getSideArmy(detachment->aiDetachUnits[0].unit->army);
	auto deployArea = &battleSideArmy->deploymentAreas.get(0)->area;
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
	{
		gameHelpers::logStringGame("Using gate 0: " + std::to_string(gate->buildingIndex) + " with detachment " + std::to_string(reinterpret_cast<DWORD>(detachment)));
		USED_GATES->push_back(gate->buildingIndex);
		return true;
	}
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
		if (distance(deployArea->coordsPairs[i].xCoord, deployArea->coordsPairs[i].yCoord, gate->xCoord, gate->yCoord) < 250)
			isCloseEnough = true;
		if (!gates.empty() && distance(gates[0]->xCoord, gates[0]->yCoord, gate->xCoord, gate->yCoord) < 200)
			isCloseEnough = true;
		if (isCloseEnough)
			break;
	}
	if (!isCloseEnough)
		return false;
	if (std::any_of(gates.begin(), gates.end(), [gate](const buildingBattle* gt){return gate == gt;}))
	{
		gameHelpers::logStringGame("Using gate: " + std::to_string(gate->buildingIndex) + " with detachment " + std::to_string(reinterpret_cast<DWORD>(detachment)));
		USED_GATES->push_back(gate->buildingIndex);
		return true;
	}
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
		gameHelpers::logStringGame("Error in quickSave " + std::string(e.what()));
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
		gameHelpers::logStringGame("Error in autoSave " + std::string(e.what()));
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
			const auto ourFort = reinterpret_cast<fortStruct*>(army->settlement);
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
	*reinterpret_cast<bool*>(dataOffsets::offsets.bugReport) = true;
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
	discordManager::menuLoaded();
	gameEvents::onGameInit();
}

void __stdcall patchesForGame::onUnloadCampaign()
{
	minorSettlementDb::clear();
	eopRebelFactionDb::clear();
	globalEopAiConfig::clearFactionData();
	discordManager::menuLoaded();
	gameEvents::onUnloadCampaign();
}
void __stdcall patchesForGame::onNewGameLoaded()
{
	minorSettlementDb::load();
	eopSettlementDataDb::get()->newGameLoaded();
	eopRebelFactionDb::loadData();
	plugData::data.luaAll.fillHashMaps();
	gameEvents::onNewGameLoaded();
}

bool AI_ACTIVE = false;
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

void deployGateAttackers(const aiTacticAssault* aiTactic)
{
	if (const auto unit = aiTactic->attackUnit; unit && distance(unit->positionX, unit->positionY, aiTactic->advanceX, aiTactic->advanceY) > 200)
		unitActions::placeUnit(unit, aiTactic->advanceX, aiTactic->advanceY, aiTactic->angle, 0);
}

bool FIRST_END = true;

void __fastcall patchesForGame::onEvent(DWORD** vTab, DWORD arg2)
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onEvent" << endl;
	f1.close();
#endif

	const DWORD eventCode = reinterpret_cast<DWORD>(vTab[0]);

	const int gameVersion = gameHelpers::getGameVersion();
	const DWORD scrollOpenedCode = gameVersion == 1 ? 0x013719FC : 0x0132C9D4;
	const DWORD factionTurnStartCode = gameVersion == 1 ? 0x0136931C : 0x013242F4;
	const DWORD gameReloaded = gameVersion == 1 ? 0x013319E4 : 0x012EC9C4;
	const DWORD settlementTurnStart = gameVersion == 1 ? 0x0136E2B4 : 0x0132928C;
	const DWORD settlementTurnEnd = gameVersion == 1 ? 0x0136FADC : 0x0132AAB4;
	const DWORD conflictPhaseCommenced = gameVersion == 1 ? 0x013670AC : 0x01322084 ;
	const DWORD factionTurnEnd = gameVersion == 1 ? 0x01369D74 : 0x01324D4C;
	const DWORD characterTurnEnd = gameVersion == 1 ? 0x0136C0B4 : 0x0132708C;
	const DWORD characterTurnStart = gameVersion == 1 ? 0x0136BFE4 : 0x01326FBC;
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
		else if (strcmp(str, "hotseat_scroll") == 0)
		{
			battleCreator::onHotseatScreen();
		}
		/*
		else if (m2tweopOptions::getHideUnknownUnitTooltips() && !battleHelpers::inBattle())
		{
			if (strcmp(str, "enemy_army_info_scroll") == 0)
			{
				gameHelpers::logStringGame("enemy_army_info_scroll");
				gameUiHelpers::removeToolTipsArmy();
			}
			else if (strcmp(str, "enemy_character_info_scroll") == 0)
			{
				gameHelpers::logStringGame("enemy_character_info_scroll");
				gameUiHelpers::removeToolTipsArmy();
			}
			else if (strcmp(str, "enemy_settlement_info_scroll") == 0)
			{
				gameHelpers::logStringGame("enemy_settlement_info_scroll");
				gameUiHelpers::removeToolTipsSett();
			}
		}*/
	}
	else if (eventCode == factionTurnStartCode)
	{
		factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
		gameHelpers::logStringGame("Faction turn start: " + string(fac->factionRecord->facName));
		globalEopAiConfig::getInstance()->turnStartMove(fac, false);
		discordManager::onFactionTurnStart(fac);
		plannedRetreatRoute::onFactionTurnStart(fac);
		AI_ACTIVE = true;
	}
	else if (eventCode == gameReloaded)
	{
		eopSettlementDataDb::get()->onGameLoaded();
		eopCharacterDataDb::get()->onGameLoaded();
	}
	else if (eventCode == conflictPhaseCommenced)
	{
		const auto battle = battleHelpers::getBattleData();
		USED_GATES->clear();
		if (battle && battle->battleType == battleType::siege)
		{
			if (const auto aiSide = battle->getAiSide(); aiSide && !aiSide->isDefender)
			{
				if (const auto battleAi = aiSide->battleAIPlan; battleAi->objectivesCount > 0)
				{
					for (int i = 0; i < battleAi->objectivesCount; i++)
					{
						const auto objective = battleAi->objectives[i];
						if (const auto objType = objective->getType(); objType == gtaObjective_ATTACK_SETTLEMENT)
						{
							for (int j = 0; j < objective->aiDetachmentsCount; j++)
							{
								const auto detachment = objective->aiDetachments[j];
								if (detachment->aiDetachUnitsCount == 0)
									continue;
								for (int t = 0; t < detachment->tacticsCount; t++)
								{
									const auto tactic = detachment->tactics[t];
									if (const auto tacticType = callClassFunc<aiDetachmentTactic*, int>(tactic, 0x4C); tacticType == 19)
									{
										const auto assaultTactic = reinterpret_cast<aiTacticAssault*>(tactic);
										deployGateAttackers(assaultTactic);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (eventCode == characterTurnEnd)
	{
		if (const auto record = reinterpret_cast<characterRecord*>(vTab[1]);
			record->gen
			&& record->faction->isPlayerControlled == 0
			&& record->gen->isGeneral()
			&& record->gen->army
			)
		{
			globalEopAiConfig::getInstance()->characterTurnStart(record->gen);
		}
	}
	else if (eventCode == characterTurnStart)
	{
		if (const auto record = reinterpret_cast<characterRecord*>(vTab[1]);
			record->gen
			&& record->faction->factionID == campaignHelpers::getCampaignData()->slaveFactionID
			&& record->gen->getTypeID() == characterTypeStrat::general
			&& record->gen->army
			&& (!record->labelCrypt || !eopCharacterDataDb::get()->getCharacterData(record->label))
			)
		{
			const auto rebelFac = eopRebelFactionDb::getRebelFaction(record->gen->army->descrRebel->name);
			if (rebelFac && !rebelFac->characterModels.empty())
			{
				std::random_device rd;
				std::mt19937 g(rd());
				std::shuffle(rebelFac->characterModels.begin(), rebelFac->characterModels.end(), g);
				const auto randomModel = rebelFac->characterModels.front();
				stratModelsChange::setCharacterModel(record->gen, randomModel.c_str());
			}
		}
	}
	else if (eventCode == factionTurnEnd)
	{
		const auto fac = reinterpret_cast<factionStruct*>(vTab[1]);
		
		if (globalEopAiConfig::getInstance()->enableLogging && !fac->isPlayerControlled)
			fac->aiFaction->aiGlobalStrategyDirector->militaryDirector.logData();
		//globalEopAiConfig::getInstance()->turnStartMove(fac, true);
		
		FIRST_END = true;
	}
	//else if (eventCode == settlementTurnStart)
	//{
	//	const auto sett = reinterpret_cast<settlementStruct*>(vTab[1]);
	//	if (sett->minorSettlementIndex > 3 || sett->minorSettlementIndex < 0)
	//	{
	//		const std::string str = "minorSettlementIndex failed to verify " + string(sett->name);
	//		gameHelpers::logStringGame(str);
	//		MessageBoxA(nullptr, "minorSettlementIndex failed to verify", "Attention", NULL);
	//	}
	//	if (static_cast<int8_t>(sett->isMinorSettlement) < 0 || static_cast<int8_t>(sett->isMinorSettlement) > 1)
	//	{
	//		const std::string str = "isMinorSettlement failed to verify " + string(sett->name);
	//		gameHelpers::logStringGame(str);
	//		MessageBoxA(nullptr, "isMinorSettlement failed to verify", "Attention", NULL);
	//	}
	//}
	
	gameEvents::onEventWrapper(eventCode, vTab);
}

void __fastcall patchesForGame::onLoadSaveFile(UNICODE_STRING**& savePath)
{
	const string relativePath = techFuncs::uniToAnsi(savePath);
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
	eopSettlementDataDb::get()->onGameLoad(files);
	eopCharacterDataDb::get()->onGameLoad(files);
	gameEvents::onLoadGamePl(&files);
	plannedRetreatRoute::onGameLoad(files);
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
	if (const std::string characterData = eopCharacterDataDb::get()->onGameSave(); !characterData.empty())
		files.push_back(characterData);
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
	return eopDu::getEduEntry(eduIndex);
}

void __fastcall patchesForGame::recruitEopUnit2(int eduIndex)
{
	if (eduEntry* entry = eopDu::getEduEntry(eduIndex))
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

	std::string relativePath = techFuncs::uniToAnsi(campaignHelpers::getCampaignData()->currentDescrFile);

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
