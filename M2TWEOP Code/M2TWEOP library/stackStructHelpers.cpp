#include "stackStructHelpers.h"

#include "eduThings.h"
#include "factionHelpers.h"
#include "fastFuncts.h"
#include "gameHelpers.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "unitHelpers.h"

namespace stackStructHelpers
{
	void sortStack(stackStruct* stack, const int sortType, const int sortType2, int sortType3)
	{
		int N = stack->numOfUnits;

		std::vector<int> sortTypes = { sortType };
		if (sortType2 != 0)
			sortTypes.push_back(sortType2);
		if (sortType3 != 0)
			sortTypes.push_back(sortType3);

		// Lambda function to compare unit objects based on multiple criteria
		auto compareUnits = [&sortTypes](const unit* unitA, const unit* unitB)
		{
			// Ensuring Generals stay at the front/start of the stack
			if (unitA->general && !unitB->general) {
				return true;
			}
			if (!unitA->general && unitB->general) {
				return false;
			}
			for (int sortType : sortTypes)
			{
				switch (sortType)
				{
					case 1: // By EDU Type
						if (std::string(unitA->eduEntry->Type) != std::string(unitB->eduEntry->Type))
							return std::string(unitA->eduEntry->Type) < std::string(unitB->eduEntry->Type);
						break;
					case 2: // By Category
						if (unitA->eduEntry->Category != unitB->eduEntry->Category)
							return unitA->eduEntry->Category < unitB->eduEntry->Category;
						break;
					case 3: // By Class
						if (unitA->eduEntry->Class != unitB->eduEntry->Class)
							return unitA->eduEntry->Class < unitB->eduEntry->Class;
						break;
					case 4: // Soldier Count
						if (unitA->SoldierCountStrat != unitB->SoldierCountStrat)
							return unitA->SoldierCountStrat > unitB->SoldierCountStrat;
						break;
					case 5: // Experience
						if (unitA->expScreen != unitB->expScreen)
							return unitA->expScreen > unitB->expScreen;
						break;
					case 6: // categoryClass
						if (unitA->eduEntry->categoryClassCombinationForAI != unitB->eduEntry->categoryClassCombinationForAI)
							return unitA->eduEntry->categoryClassCombinationForAI < unitB->eduEntry->categoryClassCombinationForAI;
						break;
					case 7: // aiUnitValue
						if (unitA->eduEntry->aiUnitValuePerSoldier != unitB->eduEntry->aiUnitValuePerSoldier)
							return unitA->eduEntry->aiUnitValuePerSoldier < unitB->eduEntry->aiUnitValuePerSoldier;
						break;
					default:
						if (std::string(unitA->eduEntry->Type) != std::string(unitB->eduEntry->Type))
							return std::string(unitA->eduEntry->Type) < std::string(unitB->eduEntry->Type);
						break;
				}
			}
			return false;
		};

		// Stable sort the units
		std::stable_sort(stack->units, stack->units + N, compareUnits);
	}

	unit *getUnit(const stackStruct *army, int index)
	{
		return army->units[index];
	}
	general *getCharacter(const stackStruct *army, int index)
	{
		return army->characters[index];
	}
	int attackArmy(stackStruct *army, stackStruct *defender)
	{
		if (!army || !defender || defender->settlement)
			return 0;
		DWORD funcAddr = 0x007195A0;
		if (smallFuncs::getGameVersion() == 1)
			funcAddr = 0x718E80;
		_asm
		{
			push defender
			mov ecx, army
			mov eax, funcAddr
			call eax
		}

		return 1;
	}
	
	settlementStruct *findInSettlement(const stackStruct *army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		int x = army->settlement->xCoord;
		int y = army->settlement->yCoord;
		auto tile = gameHelpers::getTile(x, y);
		return gameHelpers::getTileSettlement(tile);
	}
	fortStruct *findInFort(const stackStruct *army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		int x = army->settlement->xCoord;
		int y = army->settlement->yCoord;
		auto tile = gameHelpers::getTile(x, y);
		return gameHelpers::getTileFort(tile);
	}

	unitGroup* getGroup(const stackStruct* army, int index)
	{
		if (!army || !army->unitGroups || index < 0 || index >= army->maxUnitGroups)
			return nullptr;
		return &army->unitGroups[index];
	}

	unit* getDeadUnit(const stackStruct* army, int index)
	{
		if (!army || !army->deadUnits || index < 0 || index >= army->deadUnitsNum)
			return nullptr;
		return army->deadUnits[index];
	}

	void setAiActiveSet(const stackStruct* army, const int set)
	{
		if (!army)
			return;
		for (int i = 0; i < army->numOfUnits; i++)
			army->units[i]->aiActiveSet = set;
	}

	void releaseUnits(const stackStruct* army)
	{
		if (!army)
			return;
		for (int i = 0; i < army->numOfUnits; i++)
			unitHelpers::releaseUnit(army->units[i]);
	}

	unit *createUnit(stackStruct *army, const char *type, int exp, int arm, int weap)
	{
		unit *newUnit = fastFuncts::createUnitN(type, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
			return newUnit;
		fastFuncts::addUnitToArmy(army, newUnit);
		return newUnit;
	}
	unit *createUnitByIDX(stackStruct *army, int typeIDX, int exp, int arm, int weap)
	{
		unit *newUnit = fastFuncts::createUnitIdx(typeIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
			return newUnit;
		fastFuncts::addUnitToArmy(army, newUnit);
		return newUnit;
	}
	unit *createEOPUnit(stackStruct *army, int typeIDX, int exp, int arm, int weap)
	{
		const int eopIdx = eduThings::getDataEopEdu(typeIDX);
		if (eopIdx == 0)
			return nullptr;

		unit *newUnit = fastFuncts::createUnitEDB(eopIdx, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
			return newUnit;
		fastFuncts::addUnitToArmy(army, newUnit);
		return newUnit;
	}
	void mergeArmies(stackStruct *army, stackStruct *targetArmy, bool force)
	{
		if (force)
			fastFuncts::mergeArmies(army, targetArmy);
		else
		{
			int targetX, targetY;
			if (targetArmy->gen)
			{
				targetX = targetArmy->gen->xCoord;
				targetY = targetArmy->gen->yCoord;
			}
			else if (targetArmy->settlement)
			{
				targetX = targetArmy->settlement->xCoord;
				targetY = targetArmy->settlement->yCoord;
			}
			else
				return;
			
			sol::table units = sol::state_view(plugData::data.luaAll.luaState).create_table();
			for (int i = 0; i < army->numOfUnits; i++)
			{
				units.add(army->units[i]);
			}
			const auto faction = army->faction;
			factionHelpers::splitArmy(faction, units, targetX, targetY);
		}
	}

	void mergeArmies(stackStruct* army, stackStruct* targetArmy)
	{
		mergeArmies(army, targetArmy, true);
	}
	

	
}
