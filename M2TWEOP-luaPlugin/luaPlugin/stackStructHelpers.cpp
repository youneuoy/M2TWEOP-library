#include "stackStructHelpers.h"
#include "plugData.h"
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
			for (int sortType : sortTypes)
			{
				switch (sortType)
				{
					case 1: // By EDU Type
						if (std::string(unitA->eduEntry->Type) != std::string(unitB->eduEntry->Type))
							return std::string(unitA->eduEntry->Type) > std::string(unitB->eduEntry->Type);
						break;
					case 2: // By Category
						if (unitA->eduEntry->Category != unitB->eduEntry->Category)
							return unitA->eduEntry->Category > unitB->eduEntry->Category;
						break;
					case 3: // By Class
						if (unitA->eduEntry->Class != unitB->eduEntry->Class)
							return unitA->eduEntry->Class > unitB->eduEntry->Class;
						break;
					case 4: // Soldier Count
						if (unitA->SoldierCountStrat != unitB->SoldierCountStrat)
							return unitA->SoldierCountStrat > unitB->SoldierCountStrat;
						break;
					case 5: // Experience
						if (unitA->expScreen != unitB->expScreen)
							return unitA->expScreen > unitB->expScreen;
						break;
					case 6: // categorClass
						if (unitA->eduEntry->categoryClassCombinationForAI != unitB->eduEntry->categoryClassCombinationForAI)
							return unitA->eduEntry->categoryClassCombinationForAI > unitB->eduEntry->categoryClassCombinationForAI;
						break;
					default: 
						break;
				}
			}
			// Ensuring Generals stay at the front/start of the stack
			return unitA->general && !unitB->general;
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
		general *atkGen = army->gen;
		general *defGen = defender->gen;
		if (atkGen == nullptr || defGen == nullptr)
			return 0;

		(*(*plugData::data.funcs.attackCharacter))(atkGen, defGen);

		return 1;
	}
	void siegeSettlement(stackStruct *army, settlementStruct *sett)
	{
		general *atkGen = army->gen;

		if (atkGen == nullptr || sett == nullptr)
			return;

		(*(*plugData::data.funcs.siegeSettlement))(atkGen, sett);
	}
	settlementStruct *findInSettlement(const stackStruct *army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		for (int i = 0; i < army->faction->settlementsNum; i++)
		{
			settlementStruct *sett = army->faction->settlements[i];
			if (sett->army == army)
				return sett;
		}

		return nullptr;
	}
	fortStruct *findInFort(const stackStruct *army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		for (int i = 0; i < army->faction->fortsNum; i++)
		{
			fortStruct *frt = army->faction->forts[i];
			if (frt->army == army)
				return frt;
		}

		return nullptr;
	}

	unit *createUnit(stackStruct *army, const char *type, int exp, int arm, int weap)
	{
		unit *newUnit = (*(*plugData::data.funcs.createUnitN))(type, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	unit *createUnitByIDX(stackStruct *army, int typeIDX, int exp, int arm, int weap)
	{
		unit *newUnit = (*(*plugData::data.funcs.createUnitIdx))(typeIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	unit *createEOPUnit(stackStruct *army, int typeIDX, int exp, int arm, int weap)
	{
		int eopIDX = (*(*plugData::data.funcsEopEdu.getDataEopEdu))(typeIDX);
		if (eopIDX == 0)
		{
			return nullptr;
		}

		unit *newUnit = (*(*plugData::data.funcs.createUnitEDB))(eopIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	void mergeArmies(stackStruct *army, stackStruct *targetArmy)
	{
		(*(*plugData::data.funcs.mergeArmies))(army, targetArmy);
	}
}