#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace stackStructHelpers
{
	unit* getUnit(const stackStruct* army, int index);
	general* getCharacter(const stackStruct* army, int index);
	int attackArmy(stackStruct* army, stackStruct* defender);
	void siegeSettlement(stackStruct* army, settlementStruct* sett);
	settlementStruct* findInSettlement(const stackStruct* army);
	fortStruct* findInFort(const stackStruct* army);

	unit* createUnit(stackStruct* army, const char* type, int exp, int arm, int weap);
	unit* createUnitByIDX(stackStruct* army,int typeIDX, int exp, int arm, int weap);
	unit* createEOPUnit(stackStruct* army,int typeIDX, int exp, int arm, int weap);
	void mergeArmies(stackStruct* army, stackStruct* targetArmy);
};

