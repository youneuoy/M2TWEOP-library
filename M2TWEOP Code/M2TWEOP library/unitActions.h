#pragma once
#include <string>

#include "exportHeader.h"
#include "headersMEM.h"
#include "realGameTypes.h"

namespace unitActions
{
	void unitMovetoPosition(unit* unit, float xCoord, float yCoord, bool run);
	int getUnitFormation(const unit* unit);
	void placeUnit(unit* unit, float xCoord, float yCoord, int16_t angle, int width);
	unit* getUnitByLabel(const char* label);
	void unitAttackClosest(unit* un, int16_t angle, bool run);
	void attackUnit(unit* un, const unit* targetUnit, bool run);
	void deployStakes(const unit* un);
	void changeUnitFormation(const unit* un, int formationType);
	void haltUnit(const unit* un);
	float getBattleMapHeight(float xCoord, float yCoord);
	void moveToOrientation(unit* un, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run);
	void moveRelative(unit* un, float xCoord, float yCoord, bool run);
	void moveToMissileRange(unit* un, const unit* targetUnit, bool run);
	void unitTurn(unit* un, int16_t angle, bool isRelative);
	void taunt(const unit* un);
	void useSpecialAbility(const unit* un);
	int getSiegeEngineType(const unit* un);
	void attackBuilding(unit* un, buildingBattle* building);
	void logStringGame(const std::string& msg);
	void collectEngine(unit* un, siegeEngine* engine);

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
	


}
