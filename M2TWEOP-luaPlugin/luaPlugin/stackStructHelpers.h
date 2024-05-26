#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace stackStructHelpers
{
	unit* getUnit(const stackStruct* army, int index);
	general* getCharacter(const stackStruct* army, int index);
	int attackArmy(stackStruct* army, stackStruct* defender);
	void siegeSettlement(stackStruct* army, settlementStruct* sett, bool isAttack);
	void siegeFort(stackStruct* army, fortStruct* fort, bool isAttack);
	void sortStack(stackStruct* stack, const int sortType, const int sortType2, int sortType3);
	settlementStruct* findInSettlement(const stackStruct* army);
	fortStruct* findInFort(const stackStruct* army);
	unitGroup* getGroup(const stackStruct* army, int index);

	unit* createUnit(stackStruct* army, const char* type, int exp, int arm, int weap);
	unit* createUnitByIDX(stackStruct* army,int typeIDX, int exp, int arm, int weap);
	unit* createEOPUnit(stackStruct* army,int typeIDX, int exp, int arm, int weap);
	void mergeArmies(stackStruct* army, stackStruct* targetArmy);
	stackStruct* spawnArmy(
		factionStruct* faction,
		const char* name,
		const char* name2,
		int characterType,
		const char* label,
		const char* portrait,
		int x,
		int y,
		int age,
		bool family,
		int subFaction,
		int unitIndex,
		int exp,
		int wpn,
		int armour
		);
};

