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

	enum class unitActionType
	{
		unitChangeFormation = 0,
		unitChangeFormationClose = 1,
		unitChangeFormationWidth = 2,
		unitChangeSpeed = 3,
		unitChangeMeleeState = 4,
		unitSetupEngine = 5,
		unitCollectEngine = 6,
		unitDropEngine = 7,
		unitHide = 8,
		unitMove = 9,
		UnitMoveIntoPlayableArea = 10,
		unitTurn = 11,
		unitAttackUnit = 12,
		unitAttackBuilding = 13,
		unitAttackEngine = 14,
		unitWithdraw = 15,
		unitFeignRout = 16,
		unitUseAbility = 17,
		unitHalt = 18,
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
	float getUnitAngle(const unit* un);
	void setUnitAngle(unit* un, float angle);
	void setUnitParams(unit* un, int exp, int armor, int weap);
	float getMovepoints(const unit* un);
	int getMoraleLevel(const unit* un);
	void setMoraleLevel(unit* un, int level);
	unit* getNearbyUnit(const unit* unit, const int index);
	unit* getNearbyEnemyUnit(const unit* unit, const int index);
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
	void unitAttackClosest(unit* un, int16_t angle, bool run);
	void unitAttackUnit(unit* un, const unit* targetUnit, bool run);
	void unitMovetoPosition(unit* unit, float xCoord, float yCoord, bool run);
	void moveToOrientation(unit* unit, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run);
	void placeUnit(unit* unit, float xCoord, float yCoord, int16_t angle, int width);
	void deployStakes(const unit* unit);
	void haltUnit(const unit* unit);
	void changeUnitFormation(const unit* unit, int formationType);
	float getBattleMapHeight(float xCoord, float yCoord);
	void moveRelative(unit* unit, float xCoord, float yCoord, bool run);
	void moveToMissileRange(unit* un, const unit* targetUnit, bool run);
	void unitTurn(unit* un, int16_t angle, bool isRelative);
	void taunt(const unit* un);
	void useSpecialAbility(const unit* un);
	siegeEngine* getSiegeEngine(const unit* un, const int index);
	void attackBuilding(unit* un, buildingBattle* building);
	void collectEngine(unit* un, siegeEngine* engine);
	std::string getLocalizedUnitName(const eduEntry* entry);
	std::string getLocalizedUnitDescr(const eduEntry* entry);
	std::string getLocalizedUnitDescrShort(const eduEntry* entry);
	void setUnitName(const eduEntry* entry, const std::string& name);
	void setUnitDescr(const eduEntry* entry, const std::string& descr);
	void setUnitDescrShort(const eduEntry* entry, const std::string& descr);
	float getEngineAngle(const siegeEngine* engine);
	void setEngineAngle(siegeEngine* engine, float angle);
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
	void changeGroupUnitFormation(const unitGroup* group, int formationType);
	void moveToRangeOfGroup(const unitGroup* group, const unitGroup* targetGroup, bool run);
	void moveGroupToRangeOfUnit(const unitGroup* group, const unit* targetUnit, bool run);
	void groupAttackGroup(const unitGroup* group, const unitGroup* targetGroup, bool run);
	void groupHalt(const unitGroup* group);
	void groupMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run);
	void groupMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run);
	void groupMoveFormedRelative(const unitGroup* group, float xCoord, float yCoord, bool run);
	void groupMoveUnformedRelative(const unitGroup* group, float xCoord, float yCoord, bool run);
	void groupTurn(const unitGroup* group, int16_t angle, bool isRelative);
	
};

/*
* @tfield moveToMissileRangeOfUnit moveToMissileRangeOfUnit
	@tfield attackGroup attackGroup
	@tfield halt halt
	@tfield moveFormed moveFormed
	@tfield moveUnformed moveUnformed
	@tfield moveRelativeFormed moveRelativeFormed
	@tfield moveRelativeUnformed moveRelativeUnformed
	@tfield turn turn
 */