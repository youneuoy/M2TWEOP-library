#include "stackStructHelpers.h"
#include "plugData.h"
namespace stackStructHelpers
{
	void sortStack(stackStruct *stack, int sortType)
	{
		int N = stack->numOfUnits;

		// Lambda function to compare unit objects based on name
		auto compareByEduTypeName = [sortType](const unit *unitA, const unit *unitB)
		{	
			// Ensuring Generals stay at the front/start of the stack
			if (unitA->general && !unitB->general) {
				return true; 
			}
			if (!unitA->general && unitB->general) {
				return false;
			}

			// Sorting Options
			// By EDU Type
			if (sortType == 1)
			{
				return std::string(unitA->eduEntry->Type) > std::string(unitB->eduEntry->Type);
			}
			// By Category
			else if (sortType == 2)
			{
				return unitA->eduEntry->Category > unitB->eduEntry->Category;
			}
			// By Class
			else if (sortType == 3)
			{
				return unitA->eduEntry->Class > unitB->eduEntry->Class;
			}
			// Soldier Count
			else if (sortType == 4)
			{
				return unitA->number > unitB->number;
			}
			// Experience
			else if (sortType == 5)
			{
				return unitA->expScreen > unitB->expScreen;
			}
		};
		// Sort the units (?)
		std::sort(stack->units, stack->units + N, compareByEduTypeName);
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