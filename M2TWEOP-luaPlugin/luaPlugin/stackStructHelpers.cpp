#include "stackStructHelpers.h"
#include "plugData.h"
namespace stackStructHelpers
{
	unit* getUnit(const stackStruct* army, int index)
	{
		return army->units[index];
	}
	general* getCharacter(const stackStruct* army, int index)
	{
		return army->characters[index];
	}
	int attackArmy(stackStruct* army, stackStruct* defender)
	{
		general* atkGen = army->gen;
		general* defGen = defender->gen;
		if (atkGen == nullptr
			|| defGen == nullptr
			)return 0;

		(*(*plugData::data.funcs.attackCharacter))(atkGen, defGen);

		return 1;
	}
	void siegeSettlement(stackStruct* army, settlementStruct* sett)
	{
		general* atkGen = army->gen;

		if (atkGen == nullptr
			|| sett == nullptr
			)return;

		(*(*plugData::data.funcs.siegeSettlement))(atkGen, sett);


	}
	settlementStruct* findInSettlement(const stackStruct* army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		for (int i = 0; i < army->faction->settlementsNum; i++)
		{
			settlementStruct* sett = army->faction->settlements[i];
			if (sett->army == army)return sett;
		}

		return nullptr;
	}
	fortStruct* findInFort(const stackStruct* army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		for (int i = 0; i < army->faction->fortsNum; i++)
		{
			fortStruct* frt = army->faction->forts[i];
			if (frt->army == army)return frt;
		}

		return nullptr;
	}

	unit* createUnit(stackStruct* army, const char* type, int exp, int arm, int weap)
	{
		unit*newUnit= (*(*plugData::data.funcs.createUnitN))(type, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	unit* createUnitByIDX(stackStruct* army, int typeIDX, int exp, int arm, int weap)
	{
		unit* newUnit = (*(*plugData::data.funcs.createUnitIdx))(typeIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	unit* createEOPUnit(stackStruct* army, int typeIDX, int exp, int arm, int weap)
	{
		int eopIDX=(*(*plugData::data.funcsEopEdu.getDataEopEdu))(typeIDX);
		if (eopIDX == 0)
		{
			return nullptr;
		}

		unit* newUnit = (*(*plugData::data.funcs.createUnitEDB))(eopIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	void mergeArmies(stackStruct* army, stackStruct* targetArmy)
	{
		(*(*plugData::data.funcs.mergeArmies))(army, targetArmy);
	}
}