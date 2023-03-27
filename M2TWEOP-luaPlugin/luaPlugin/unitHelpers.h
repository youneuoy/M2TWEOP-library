#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace unitHelpers
{
	int getExp(unit* un);
	void setExp(unit* un,int exp);
	int getarmourLVL(unit* un);
	void setarmourLVL(unit* un,int lvl);
	int getweaponLVL(unit* un);
	void setweaponLVL(unit* un,int lvl);

	int getMaxSoldiersCount(unit* un);

	void setMovepoints(unit* un,float movepoints);
	void setSoldiersCount(unit* un,int number);
	void killUnit(unit* un);
	void setUnitParams(unit* un, int exp, int armor, int weap);
	float getMovepoints(const unit* un);

	int getsoldierCountStratMap(const unit* un);
};

