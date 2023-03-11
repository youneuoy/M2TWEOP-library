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

	std::string gameHelpers::callConsole(std::string cmdName, sol::variadic_args va)
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

	factionStruct* gameHelpers::getFaction(int index)
	{
		return (*(*plugData::data.funcs.getFactionsList))()[index];
	}

	guild* gameHelpers::getGuild(unsigned char index)
	{
		return (*(*plugData::data.funcs.getGuild))(index);
	}

	general* gameHelpers::createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y)
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

	void gameHelpers::setScriptCounter(const char* type, int value)
	{
		(*(*plugData::data.funcs.setScriptCounter))(type, value);
	}

	regionStruct* gameHelpers::getRegion(int index)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		return &map->regions[index];
	}

	oneTile* gameHelpers::getTile(int x, int y)
	{
		gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
		stratMap* map = gameDataAll->stratMap;
		return &map->tilesArr[gameDataAll->stratMap->mapWidth * y + x];
	}

	stackStruct* gameHelpers::getStack(const regionStruct* region, int index)
	{
		return region->armies[index];
	}

	fortStruct* gameHelpers::getFort(const regionStruct* region, int index)
	{
		return region->forts[index];
	}

	watchTowerStruct* gameHelpers::getWatchtower(const regionStruct* region, int index)
	{
		return region->watchtowers[index];
	}

	resStrat* gameHelpers::getResource(const regionStruct* region, int index)
	{
		return region->resources[index];
	}

	regionStruct* gameHelpers::getNeighbour(regionStruct* region, int index)
	{
		neighbourRegion* nregion = &region->neighbourRegions[index];
		return nregion->region;
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

	void gameHelpers::setHiddenResource(regionStruct* region, int index, bool enable)
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

	mercPoolUnit* gameHelpers::addMercUnit(mercPool* mercPool, int idx, int exp, int cost, float repmin, float repmax, int maxunits, float startpool, float startyear, float endyear, int crusading)
	{
		mercPoolUnit* newMerc = new mercPoolUnit;
		*newMerc = mercPool->firstUnits.mercPoolUnits[0];
		int mercUnitNum = gameHelpers::getMercUnitNum(mercPool);
		EduEntry* entry = eopEduHelpers::getEduEntry(idx);
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
		newMerc->mercPoolUnitIndex = (int16_t)mercUnitNum;
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
			mercPoolUnitsPtr* newPtr = new mercPoolUnitsPtr;
			newPtr->Maxpool = (unitptr->Maxpool) * 2;
			newPtr->currentPool = 0;
			newPtr->nextUnitsPtr = nullptr;
			newPtr->prevPoolUnits = unitptr->mercPoolUnits;
			unitptr->nextUnitsPtr = newPtr;
			unitptr = unitptr->nextUnitsPtr;
			currunits = 0;
		}
		mercPoolUnit* newArray = new mercPoolUnit[currunits + 1];
		for (int i = 0; i < currunits; i++) {
			newArray[i] = unitptr->mercPoolUnits[i];
		}
		unitptr->mercPoolUnits = newArray;
		unitptr->mercPoolUnits[currunits] = *newMerc;
		unitptr->currentPool++;
		return &unitptr->mercPoolUnits[currunits];
	}

	mercPoolUnit* gameHelpers::getMercUnit(mercPool* pool, int index)
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


	void gameHelpers::setMercReligion(mercPoolUnit* unit, int religion, bool set)
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
	};

	void historicEvent(const char* name, const char* title, const char* description)
	{
		(*(*plugData::data.funcs.historicEvent))(name, title, description);
	}
}
