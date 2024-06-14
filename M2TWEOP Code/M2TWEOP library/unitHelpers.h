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
	groupLabels* getGroupLabels();
	siegeEngine* getSiegeEngine(const unit* un, const int index);
	std::string getLocalizedUnitName(const eduEntry* entry);
	std::string getLocalizedUnitDescr(const eduEntry* entry);
	std::string getLocalizedUnitDescrShort(const eduEntry* entry);
	void setUnitName(eduEntry* entry, const std::string& name);
	void setUnitDescr(eduEntry* entry, const std::string& descr);
	void setUnitDescrShort(eduEntry* entry, const std::string& descr);
	float getEngineAngle(const siegeEngine* engine);
	void setEngineAngle(siegeEngine* engine, float angle);
	unitGroup* getEmptyGroup(const stackStruct* army);
	const char* getGroupLabel(const unitGroup* group);
	void releaseUnit(unit* un);
	int16_t angleFloatToShort(float angle);
	float angleShortToFloat(int16_t angle);
	
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
    int getSiegeEngineNum(unit* un);
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