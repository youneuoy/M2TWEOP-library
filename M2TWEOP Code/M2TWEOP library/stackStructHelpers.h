#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace stackStructHelpers
{
	unit* getUnit(const stackStruct* army, int index);
	character* getCharacter(const stackStruct* army, int index);
	int attackArmy(stackStruct* army, stackStruct* defender);
	void sortStack(stackStruct* stack, const int sortType, const int sortType2, int sortType3);
	settlementStruct* findInSettlement(const stackStruct* army);
	fortStruct* findInFort(const stackStruct* army);
	unitGroup* getGroup(const stackStruct* army, int index);
	unit* getDeadUnit(const stackStruct* army, int index);
	void setAiActiveSet(const stackStruct* army, const int set);
	void releaseUnits(const stackStruct* army);
	

	unit* createUnit(stackStruct* army, const char* type, int exp, int arm, int weap);
	unit* createUnitByIDX(stackStruct* army,int typeIDX, int exp, int arm, int weap);
	unit* createEOPUnit(stackStruct* army,int typeIDX, int exp, int arm, int weap);
	void mergeArmies(stackStruct* army, stackStruct* targetArmy, bool force = true);
	void mergeArmies(stackStruct* army, stackStruct* targetArmy);
};

