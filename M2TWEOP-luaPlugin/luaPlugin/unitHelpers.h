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
	int getUnitFormation(const unit* unit);
	unit* getUnitByLabel(const char* label);
	void unitAttackClosest(const unit* un, int16_t angle, bool run);
	void unitAttackUnit(const unit* un, const unit* targetUnit, bool run);
	void unitMovetoPosition(const unit* unit, float xCoord, float yCoord, bool run);
	void moveToOrientation(const unit* unit, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run);
	void placeUnit(unit* unit, float xCoord, float yCoord, int16_t angle, int width);
	void deployStakes(const unit* unit);
	void haltUnit(const unit* unit);
	void changeUnitFormation(const unit* unit, int formationType);
	float getBattleMapHeight(float xCoord, float yCoord);
	void moveRelative(const unit* unit, float xCoord, float yCoord, bool run);
	void moveToMissileRange(const unit* un, const unit* targetUnit, bool run);
	void unitTurn(const unit* un, int16_t angle, bool isRelative);
	void taunt(const unit* un);
	void useSpecialAbility(const unit* un);
	siegeEngine* getSiegeEngine(const unit* un, const int index);
	void attackBuilding(const unit* un, buildingBattle* building);
	void collectEngine(const unit* un, siegeEngine* engine);

	
	unitGroup* getEmptyGroup(const stackStruct* army);
	const char* getGroupLabel(const unitGroup* group);
	
	unitGroup* defineUnitGroup(const stackStruct* army, const char* label, unit* un);
	void addUnitToGroup(unitGroup* group, unit* un);
	unitGroup* getGroupByLabel(const char* label);
	void undefineUnitGroup(const unitGroup* group);
	void removeUnitFromGroup(unitGroup* group, unit* un);
	unit* getUnitInFormation(const unitGroup* group, int index);
	unit* getUnitNotInFormation(const unitGroup* group, int index);
	void automateGroup(const unitGroup* group, bool automate);
	void automateAttack(unitGroup* group, unit* targetUnit);
	void automateDefense(unitGroup* group, float xCoord, float yCoord, float radius);
	void placeGroup(unitGroup* group, float xCoord, float yCoord, float angle);
};

