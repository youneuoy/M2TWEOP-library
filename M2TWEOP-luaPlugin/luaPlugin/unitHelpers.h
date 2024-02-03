#pragma once
#include <string>
#include <unordered_map>

#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace unitHelpers
{

	enum unitBattleProperties :int
	{
		guardMode = 1,
		fireAtWill = 2,
		skirmish = 4,
	};

	int getExp(unit* un);
	void setExp(unit* un,int exp);
	int getarmourLVL(unit* un);
	void setarmourLVL(unit* un,int lvl);
	int getweaponLVL(unit* un);
	void setweaponLVL(unit* un,int lvl);
	bool hasBattleProperty(const unit* un, unitBattleProperties prop);
	void setBattleProperty(unit* un, unitBattleProperties prop, bool value);
	int getMaxSoldiersCount(unit* un);
	const char* getActionStatus(unit* un);
	void setMovepoints(unit* un,float movepoints);
	void setSoldiersCount(unit* un,int number);
	void killUnit(unit* un);
	void setUnitParams(unit* un, int exp, int armor, int weap);
	float getMovepoints(const unit* un);
	bool isMovingFastSet(unit* un);
	void setMovingFastSet(unit* un, bool set);
	bool isOnWalls(unit* un);
	bool isEngaged(unit* un);
	bool isUnderFire(unit* un);
	unit* getUnitUnderFireFrom(unitPositionData* posData, int index);
	unit* getEngagedUnit(unitPositionData* posData, int index);
	unit* getTargetUnit(unitPositionData* posData);
	int getsoldierCountStratMap(const unit* un);
	int getMountClass(unit* un);
};

