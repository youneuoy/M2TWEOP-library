﻿#pragma once
#include <string>

#include "exportHeader.h"
#include "headersMEM.h"
#include "realGameTypes.h"

namespace unitActions
{
	NOINLINE EOP_EXPORT void unitMovetoPosition(const unit* unit, float xCoord, float yCoord, bool run);
    NOINLINE EOP_EXPORT int getUnitFormation(const unit* unit);
    NOINLINE EOP_EXPORT void placeUnit(unit* unit, float xCoord, float yCoord, int16_t angle, int width);
    NOINLINE EOP_EXPORT unit* getUnitByLabel(const char* label);
    NOINLINE EOP_EXPORT void unitAttackClosest(const unit* un, int16_t angle, bool run);
    NOINLINE EOP_EXPORT void attackUnit(const unit* un, const unit* targetUnit, bool run);
    NOINLINE EOP_EXPORT void deployStakes(const unit* un);
    NOINLINE EOP_EXPORT void changeUnitFormation(const unit* un, int formationType);
    NOINLINE EOP_EXPORT void haltUnit(const unit* un);
    NOINLINE EOP_EXPORT float getBattleMapHeight(float xCoord, float yCoord);
    NOINLINE EOP_EXPORT void moveToOrientation(const unit* un, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run);
    NOINLINE EOP_EXPORT void moveRelative(const unit* un, float xCoord, float yCoord, bool run);
    NOINLINE EOP_EXPORT void moveToMissileRange(const unit* un, const unit* targetUnit, bool run);
    NOINLINE EOP_EXPORT void unitTurn(const unit* un, int16_t angle, bool isRelative);
    NOINLINE EOP_EXPORT void taunt(const unit* un);
    NOINLINE EOP_EXPORT void useSpecialAbility(const unit* un);
    NOINLINE EOP_EXPORT int getSiegeEngineType(const unit* un);
    NOINLINE EOP_EXPORT void attackBuilding(const unit* un, buildingBattle* building);
    NOINLINE EOP_EXPORT void logStringGame(const std::string& msg);

	NOINLINE EOP_EXPORT void changeGroupUnitFormation(const unitGroup* group, int formationType);
    NOINLINE EOP_EXPORT void moveToRangeOfGroup(const unitGroup* group, const unitGroup* targetGroup, bool run);
    NOINLINE EOP_EXPORT void moveGroupToRangeOfUnit(const unitGroup* group, const unit* targetUnit, bool run);
    NOINLINE EOP_EXPORT void groupAttackGroup(const unitGroup* group, const unitGroup* targetGroup, bool run);
    NOINLINE EOP_EXPORT void groupHalt(const unitGroup* group);
	NOINLINE EOP_EXPORT void groupMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run);
    NOINLINE EOP_EXPORT void groupMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run);
    NOINLINE EOP_EXPORT void groupMoveFormedRelative(const unitGroup* group, float xCoord, float yCoord, bool run);
    NOINLINE EOP_EXPORT void groupMoveUnformedRelative(const unitGroup* group, float xCoord, float yCoord, bool run);
    NOINLINE EOP_EXPORT void groupTurn(const unitGroup* group, int16_t angle, bool isRelative);
	


}