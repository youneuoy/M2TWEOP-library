#include "unit.h"

#include "battleHandlerHelpers.h"
#include "character.h"
#include "dataOffsets.h"
#include "eopEduHelpers.h"
#include "fastFunctsHelpers.h"
#include "functionsOffsets.h"
#include "m2tweopHelpers.h"
#include "smallFuncs.h"
#include "faction.h"
#include "fastFuncts.h"
#include "technicalHelpers.h"

std::unordered_map<int, const char*> actionTypes = {
	{0,"idling"},
	{1,"hiding"},
	{2,"taunting"},
	{3,"celebrating"},
	{4,"ready"},
	{5,"reforming"},
	{6,"moving"},
	{7,"withdrawing"},
	{8,"missiles_firing"},
	{9,"missiles_reloading"},
	{10,"charging"},
	{11,"fighting"},
	{12,"pursuing"},
	{13,"routing"},
	{14,"fighting_backs_to_the_walls"},
	{15,"running_amok"},
	{16,"go_berserk"},
	{17,"rallying"},
	{18,"dead"},
	{19,"leaving_battle"},
	{20,"entering_battle"},
	{21,"left_battle"},
	{22,"bracing"},
	{23,"infighting"},
};

void unit::setUnitParams(int count, int exp, int armor, int weap)
{
	unitHelpers::setSoldiersCountAndExp(this, count, exp);
	DWORD adrFunc = codes::offsets.setUnitArmorFunc;
	_asm
	{
		push armor
		mov ecx, this
		mov eax, adrFunc
		call eax
	}
	adrFunc = codes::offsets.setUnitWeapFunc;
	_asm
	{
		push weap
		mov ecx, this
		mov eax, adrFunc
		call eax
	}
	return;
}

float unit::getUnitAngle()
{
	if (!unitPositionData)
		return 0;
	return unitHelpers::angleShortToFloat(unitPositionData->angle);
}
	
void unit::setUnitAngle(float angle)
{
	unitPositionData->angle = unitHelpers::angleFloatToShort(angle);
}

const char* unit::getActionStatus()
{
	auto it = actionTypes.find(actionStatus);
	if (it != actionTypes.end())
	{
		return it->second;
	}
	return "unknown";
}

namespace unitActions
{
	
    unit* getUnitByLabel(const char* label)
    {
        const auto labelCrypt = fastFunctsHelpers::makeCryptedString(label);
        return GAME_FUNC(unit*(__cdecl*)(char**), getUnitByLabel)(labelCrypt);
    }
    
    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Place unit
    \*----------------------------------------------------------------------------------------------------------------*/
    struct unitImmediatePlace
    {
        scriptCommand command = scriptCommand("unitImmediatePlace");
        unit* unitToMove = nullptr;
        int unitLabelHash = 0;
        char *positionLabel = nullptr;
        int positionLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        float angleInDegrees = 0;
        int width = 0;
        unitImmediatePlace(unit* _un, float xCoord, float yCoord, int16_t angle, int width)
            : unitToMove(_un), xCoord(xCoord), yCoord(yCoord), width(width)
        {
            this->angleInDegrees = (angle * 0.017453292f) * 10430.378f;
        }
    };

    void placeUnit(unit* un, float xCoord, float yCoord, int16_t angle, int width)
    {
        if (un == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<unitImmediatePlace>(un, xCoord, yCoord, angle, width);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitImmediatePlace);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Move to position
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitOrderMove
    {
        scriptCommand command = scriptCommand("unitOrderMove");
        const unit* unitToMove = nullptr;
        int unitLabelHash = 0;
        char *positionLabel = nullptr;
        int positionLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        bool run = true;
        char pad_31[3]{};
        unitOrderMove(const unit* _un, float xCoord, float yCoord, bool run)
            : unitToMove(_un), xCoord(xCoord), yCoord(yCoord), run(run) {}
    };
    
    void unitMovetoPosition(unit* unit, float xCoord, float yCoord, bool run)
    {
        if (unit == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        unit->aiActiveSet = 2;
        const auto order = std::make_shared<unitOrderMove>(unit, xCoord, yCoord, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitOrderMove);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Attack closest unit
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitAttackClosestUnit
    {
        scriptCommand command = scriptCommand("unitAttackClosestUnit");;
        const unit* unitToMove = nullptr;
        int unitLabelHash = 0;
        int16_t angle = 0;
        bool run = true;
        char pad_23 = 0;
        unitAttackClosestUnit(const unit* _un, int16_t angle, bool run)
            : unitToMove(_un), run(run)
        {
            this->angle = static_cast<int16_t>((angle * 0.017453292f) * 10430.378f);
        }
    };
    
    void unitAttackClosest(unit* un, int16_t angle, bool run)
    {
        if (un == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitAttackClosestUnit>(un, angle, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitAttackClosestUnit);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Attack unit
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitAttackUnit
    {
        scriptCommand command = scriptCommand("unitAttackUnit");
        const unit* unitToMove = nullptr;
        int unitLabelHash = 0;
        const unit* targetUnit = nullptr;
        int targetUnitHash = 0;
        bool run = true;
        char pad_23[3]{};
        unitAttackUnit(const unit* _un, const unit* targetUn, bool run)
            : unitToMove(_un), targetUnit(targetUn), run(run) {}
    };
    
    void attackUnit(unit* un, const unit* targetUnit, bool run)
    {
        if (un == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitAttackUnit>(un, targetUnit, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitAttackUnit);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Place Stakes
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitDeployStakes
    {
        scriptCommand command = scriptCommand("unitDeployStakes");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        unitDeployStakes(const unit* unit) : thisUnit(unit) {}
    };
    
    void deployStakes(const unit* un)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitDeployStakes>(un);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitDeployStakes);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Change Formation
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitChangeFormation
    {
        scriptCommand command = scriptCommand("unitChangeFormation");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        int formationType = 0;
        unitChangeFormation(const unit* unit, int formation) : thisUnit(unit), formationType(formation) {}
    };
    
    void changeUnitFormation(const unit* un, int formationType)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitChangeFormation>(un, formationType);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitChangeFormation);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Halt unit
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitHalt
    {
        scriptCommand command = scriptCommand("unitHalt");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        unitHalt(const unit* unit) : thisUnit(unit) {}
    };
    
    void haltUnit(const unit* un)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitHalt>(un);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitHalt);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Move To Orientation
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitMoveToOrientation
    {
        scriptCommand command = scriptCommand("unitMoveToOrientation");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        char *positionLabel = nullptr;
        int positionLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        int widthInMen = 0;
        int16_t angleInDegrees = 0;
        bool run = true;
        char pad_37 = 0;
        unitMoveToOrientation(const unit* unit, float x, float y, int width, int16_t angle, bool run)
            : thisUnit(unit), xCoord(x), yCoord(y), widthInMen(width), run(run) {
            this->angleInDegrees = static_cast<int16_t>((angle * 0.017453292f) * 10430.378f);
        }
    };
    
    void moveToOrientation(unit* un, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run)
    {
        if (un == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitMoveToOrientation>(un, xCoord, yCoord, widthInMen, angle, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitMoveToOrientation);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Move Relative
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitMoveRelative
    {
        scriptCommand command = scriptCommand("unitMoveRelative");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        bool run = true;
        char field_29[3]{};
        unitMoveRelative(const unit* unit, float x, float y, bool run)
            : thisUnit(unit), xCoord(x), yCoord(y), run(run) {}
    };
    
    void moveRelative(unit* un, float xCoord, float yCoord, bool run)
    {
        if (un == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitMoveRelative>(un, xCoord, yCoord, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.moveRelative);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Move To Missile Range
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitMoveToMissileRange
    {
        scriptCommand command = scriptCommand("unitMoveToMissileRange");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        const unit* targetUnit = nullptr;
        int targetUnitHash = 0;
        bool run = true;
        char field_29[3]{};
        unitMoveToMissileRange(const unit* un, const unit* targetUnit, bool run)
            : thisUnit(un), targetUnit(targetUnit), run(run) {}
    };
    
    void moveToMissileRange(unit* un, const unit* targetUnit, bool run)
    {
        if (un == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitMoveToMissileRange>(un, targetUnit, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.moveToMissileRange);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Move To Missile Range
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitOrderTurn
    {
        scriptCommand command = scriptCommand("unitOrderTurn");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        int16_t angle = 0;
        bool isRelative = true;
        char pad_23 = 0;
        unitOrderTurn(const unit* un, int16_t angle, bool isRelative)
            : thisUnit(un), isRelative(isRelative)
        {
            this->angle = static_cast<int16_t>((angle * 0.017453292f) * 10430.378f);
        }
    };
    
    void unitTurn(unit* un, int16_t angle, bool isRelative)
    {
        if (un == nullptr)
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitOrderTurn>(un, angle, isRelative);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitTurn);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                 Taunt
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitTaunt
    {
        scriptCommand command = scriptCommand("unitTaunt");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        unitTaunt(const unit* un) : thisUnit(un) {}
    };
    
    void taunt(const unit* un)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitTaunt>(un);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitTaunt);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                            Use Special Ability
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct unitUseSpecialAbility
    {
        scriptCommand command = scriptCommand("unitUseSpecialAbility");
        const unit* thisUnit = nullptr;
        int unitLabelHash = 0;
        unitUseSpecialAbility(const unit* un) : thisUnit(un) {}
    };
    
    void useSpecialAbility(const unit* un)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitUseSpecialAbility>(un);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.useSpecialAbility);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                  Attack Building
    \*----------------------------------------------------------------------------------------------------------------*/

    int getSiegeEngineType(const unit* un)
    {
        if (un == nullptr)
            return -1;
        if (un->engineRec == nullptr)
            return -1;
        return un->engineRec->classID;
    }

    void attackBuilding(unit* un, buildingBattle* building)
    {
        if (un == nullptr || building == nullptr)
            return;
        //if (getSiegeEngineType(un) == -1)
        //    return;
        un->aiActiveSet = 2;
        auto battleResidence = building->battleResidence;
        if (battleResidence == nullptr)
            return;
        auto worldData = battleResidence->worldData;
        if (worldData == nullptr)
            return;
        auto unit = un;
        int buildingAngle = GAME_FUNC(int(__thiscall*)(buildingBattle*), getBuildingAngle)(building);
        float* coords = &building->xCoord;
        DWORD funcAddr = codes::offsets.attackBuilding;
        _asm
        {
            push buildingAngle
            push 1
            push coords
            push worldData
            push 0
            push 6
            mov ecx, unit
            mov eax, funcAddr
            call eax
        }
        unit->unitPositionData->hasTargets = 1;
    }

    /////////////////////////////////////// GROUP COMMANDS ////////////////////////////////////////////////////////////

    /*----------------------------------------------------------------------------------------------------------------*\
                                          Group change unit formation
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupChangeUnitFormation
    {
        scriptCommand command = scriptCommand("groupChangeUnitFormation");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        int formationId = 0;
        groupChangeUnitFormation(const unitGroup* group, int formationType)
            : group(group), formationId(formationType){}
    };
    
    void changeGroupUnitFormation(const unitGroup* group, int formationType)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupChangeUnitFormation>(group, formationType);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupUnitChangeFormation);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                          Group Move To Missile Range of Group
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupMoveToRangeOfGroup
    {
        scriptCommand command = scriptCommand("groupChangeUnitFormation");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        const unitGroup* targetGroup = nullptr;
        int targetGroupLabelHash = 0;
        bool run = true;
        char pad[3]{};
        groupMoveToRangeOfGroup(const unitGroup* group, const unitGroup* targetGroup, bool run)
            : group(group), targetGroup(targetGroup), run(run) {}
    };
    
    void moveToRangeOfGroup(const unitGroup* group, const unitGroup* targetGroup, bool run)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupMoveToRangeOfGroup>(group, targetGroup, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveToRangeOfGroup);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                          Group Move To Missile Range of Unit
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupMoveToRangeOfUnit
    {
        scriptCommand command = scriptCommand("groupMoveToRangeOfUnit");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        const unit* targetUnit = nullptr;
        int unitLabelHash = 0;
        bool run = true;
        char pad[3]{};
        groupMoveToRangeOfUnit(const unitGroup* group, const unit* targetUnit, bool run)
            : group(group), targetUnit(targetUnit), run(run) {}
    };
    
    void moveGroupToRangeOfUnit(const unitGroup* group, const unit* targetUnit, bool run)
    {
        if (group == nullptr || targetUnit == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupMoveToRangeOfUnit>(group, targetUnit, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveToRangeOfUnit);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                          Group Attack Group
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderAttackGroup
    {
        scriptCommand command = scriptCommand("groupOrderAttackGroup");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        const unitGroup* targetGroup = nullptr;
        int targetGroupLabelHash = 0;
        bool run = true;
        char pad[3]{};
        groupOrderAttackGroup(const unitGroup* group, const unitGroup* targetGroup, bool run)
            : group(group), targetGroup(targetGroup), run(run) {}
    };
    
    void groupAttackGroup(const unitGroup* group, const unitGroup* targetGroup, bool run)
    {
        if (group == nullptr || targetGroup == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderAttackGroup>(group, targetGroup, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupAttackGroup);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                   Group Halt
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderHalt
    {
        scriptCommand command = scriptCommand("groupOrderHalt");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        groupOrderHalt(const unitGroup* group) : group(group){}
    };
    
    void groupHalt(const unitGroup* group)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderHalt>(group);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupHalt);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                   Group Move Formed
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderMoveFormed
    {
        scriptCommand command = scriptCommand("groupOrderMoveFormed");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        char *positionLabel = nullptr;
        int positionLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        bool run = true;
        char pad[3]{};
        groupOrderMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run)
        : group(group), xCoord(xCoord), yCoord(yCoord), run(run) {}
    };
    
    void groupMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderMoveFormed>(group, xCoord, yCoord, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveFormed);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                   Group Move Unformed
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderMoveUnformed
    {
        scriptCommand command = scriptCommand("groupOrderMoveUnformed");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        char *positionLabel = nullptr;
        int positionLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        bool run = true;
        char pad[3]{};
        groupOrderMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run)
        : group(group), xCoord(xCoord), yCoord(yCoord), run(run) {}
    };
    
    void groupMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderMoveUnformed>(group, xCoord, yCoord, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveUnformed);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                   Group Move Formed Relative
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderRelativeMoveFormed
    {
        scriptCommand command = scriptCommand("groupOrderRelativeMoveFormed");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        bool run = true;
        char pad[3]{};
        groupOrderRelativeMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run)
        : group(group), xCoord(xCoord), yCoord(yCoord), run(run) {}
    };
    
    void groupMoveFormedRelative(const unitGroup* group, float xCoord, float yCoord, bool run)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderRelativeMoveFormed>(group, xCoord, yCoord, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupRelativeMoveFormed);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                   Group Move Unformed Relative
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderRelativeMoveUnformed
    {
        scriptCommand command = scriptCommand("groupOrderRelativeMoveUnformed");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        float xCoord = 0;
        float yCoord = 0;
        bool run = true;
        char pad[3]{};
        groupOrderRelativeMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run)
        : group(group), xCoord(xCoord), yCoord(yCoord), run(run) {}
    };
    
    void groupMoveUnformedRelative(const unitGroup* group, float xCoord, float yCoord, bool run)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderRelativeMoveUnformed>(group, xCoord, yCoord, run);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupRelativeMoveUnformed);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                   Group Turn
    \*----------------------------------------------------------------------------------------------------------------*/
    
    struct groupOrderTurn
    {
        scriptCommand command = scriptCommand("groupOrderTurn");
        const unitGroup* group = nullptr;
        int groupLabelHash = 0;
        int16_t angle = 0;
        bool isRelative = true;
        char pad2[1]{};
        groupOrderTurn(const unitGroup* group, int16_t angle, bool isRelative)
        : group(group), isRelative(isRelative)
        {
            this->angle = static_cast<int16_t>((angle * 0.017453292f) * 10430.378f);
        }
    };
    
    void groupTurn(const unitGroup* group, int16_t angle, bool isRelative)
    {
        if (group == nullptr)
            return;
        if (!battleHandlerHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderTurn>(group, angle, isRelative);
        m2tweopHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupTurn);
    }
    
    void collectEngine(unit* un, siegeEngine* engine)
    {
        if (!battleHandlerHelpers::inBattle())
            return;
        if (!un || !engine || un->eduEntry->Category != 0)
            return;
        haltUnit(un);
        un->aiActiveSet = 2;
        un->unitPositionData->targetArray[0].siegeEngine = engine;
        un->unitPositionData->targetArray[0].actionType = static_cast<int>(unitActionType::unitCollectEngine);
        un->unitPositionData->isHalted = 0;
        un->unitPositionData->hasTargets = 1;
        un->unitPositionData->currentIndex = 1;
    }
}

namespace unitHelpers
{
	

	unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap)
	{
		int unitIndex = fastFunctsHelpers::getEduIndex(type);
		
		return createUnitIdx(unitIndex, regionID, facNum, exp, arm, weap);
	}

	unit* createUnitIdx(int index, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (index == -1)return nullptr;

		unit* res = nullptr;

		DWORD EDB = dataOffsets::offsets.unitTypesStart - 4;
		DWORD adr = codes::offsets.createUnitFunc;

		regionStruct* region = fastFuncts::getRegionByID(regionID);
		_asm
		{
			mov ecx, EDB;
			push weap
			push arm
			push - 1
			push exp
			push facNum
			push index
			push region

			mov eax, [adr]
			call eax
			mov res, eax
		}

		return res;
	}

	unit* createUnitEDB(int edb, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (edb == 0)
			return nullptr;
		
		unit* res = nullptr;
		DWORD adr = codes::offsets.createUnitFunc;
		auto* entry = reinterpret_cast<eduEntry*>(edb);
		DWORD eduIndex = entry->UnitCreatedCounter;
		regionStruct* region = fastFuncts::getRegionByID(regionID);
		_asm
		{
			//mov ecx, edb;
			push weap
			push arm
			push - 1
			push exp
			push facNum
			push eduIndex
			push region

			mov eax, [adr]
			call eax
			mov res, eax
		}

		return res;
	}
	
	std::string unitUniStringToStr(unit* unit)
	{
		return technicalHelpers::uniStringToStr(unit->alias);
	}
	void setUnitUniStr(unit* unit, const char* str)
	{
		smallFuncs::createUniString(unit->alias, str);
	}
	
	void setUnitMovePoints(unit* un, float movePoints)
	{
		un->movePoints = movePoints;
		if (un->general == nullptr)
		{
			if (character* gen = un->army->gen; gen)
			{
				DWORD adrFunc = codes::offsets.setUnitMovePoints;
				_asm
				{
					mov ecx, gen

					mov eax, adrFunc
					call eax
				}
			}
			return;
		}
		un->general->movePointsCharacter = movePoints;
		un->general->movePointsArmy = movePoints;
		
		character* gen = un->general;
		
		DWORD adrFunc = codes::offsets.setUnitMovePoints;
		_asm
		{
			mov ecx, gen

			mov eax, adrFunc
			call eax
		}
	}
	
	void setSoldiersCountAndExp(unit* un, int count, int exp)
	{
		un->expScreen = exp;
		setSoldiersCount(un, count);
	}

	void setSoldiersCount(unit* un, int count)
	{
		if (count == 0)
		{
			killUnit(un);
			return;
		}
		
		int diff = count - un->SoldierCountStrat;
		if (diff == 0)
		{
			return;
		}

		int exp = un->expScreen;
		if (diff > 0)
		{
			DWORD adrFunc = codes::offsets.replenishUnitFunc;
			_asm
			{
				push exp
				push diff
				mov ecx, un
				mov eax, adrFunc
				call eax
			}
		}
		else
		{
			un->SoldierCountStrat = count;
		}
		return;
	}

	void killUnit(unit* un)
	{
		if (un->general)
		{
			characterHelpers::killCharacter(un->general);
			return;
		}
		DWORD adr = codes::offsets.killUnitStratMapFunc;

		_asm {
			push 0x1
			mov ecx, [un]
			mov eax, [adr]
			call eax
		}
	}
	
	groupLabels* getGroupLabels()
	{
		return reinterpret_cast<groupLabels*>(dataOffsets::offsets.groupLabels);
	}
	
	int getMaxSoldiersCount(unit* un)
	{

		int sizeMul = smallFuncs::GetUnitSize();

		float mul = 2.5f;
		switch (sizeMul)
		{
		case 0:
			mul = 1.f;
			break;
		case 1:	
			mul = 1.25f;
			break;
		case 2:
			mul = 1.875f;
			break;
		default:
			break;
		}
		return static_cast<int>(un->eduEntry->SoldierCount) * mul;
	}
	int16_t angleFloatToShort(float angle)
	{
		return static_cast<int16_t>(angle * 182.044432904376f);
	}

	float angleShortToFloat(int16_t angle)
	{
		return angle * 0.0054931640625f;
	}

	float getEngineAngle(const siegeEngine* engine)
	{
		return angleShortToFloat(engine->angle);
	}
	
	void setEngineAngle(siegeEngine* engine, float angle)
	{
		engine->angle = angleFloatToShort(angle);
	}

	unitGroup* getEmptyGroup(const stackStruct* army)
	{
		if (!army || !army->unitGroups)
			return nullptr;
		for (int i = 0; i < army->maxUnitGroups; i++)
		{
			const auto group = &army->unitGroups[i];
			if (group->unitsInFormationNum + group->unitsNotInFormationNum == 0)
				return group;
		}
		return nullptr;
	}

	unitGroup* getGroupByLabel(const char* label)
	{
		DWORD funcOffset = codes::offsets.getGroupByLabel;
		
		auto groupLabels = getGroupLabels();
		
		stringWithHash* labelHash = new stringWithHash();
		fastFunctsHelpers::setCryptedString(&labelHash->name, label);
		unitGroup** group = nullptr;
		__asm
		{
			push labelHash
			mov ecx, groupLabels
			mov eax, funcOffset
			call eax
			mov group, eax
		}
		delete labelHash;
		return *group;
	}
	
	const char* getGroupLabel(const unitGroup* group)
	{
		if (!group)
			return "";

		const auto allLabels = getGroupLabels();
		
		for (int i = 0; i < allLabels->count; i++)
		{
			if (allLabels->labels[i].group == group)
				return allLabels->labels[i].name;
		}
		return "";
	}

	void removeUnitFromGroup(unitGroup* group, unit* un)
	{
		if (!group || !un || !un->unitGroup || un->unitGroup != group)
			return;
		un->releaseUnit();
		CallVFunc<2, void*>(group, un, 1);
		if (group->unitsInFormationNum + group->unitsNotInFormationNum == 0)
			undefineUnitGroup(group);
	}

	void automateGroup(const unitGroup* group, bool automate)
	{
		if (!group)
			return;
		DWORD funcAddr = codes::offsets.automateGroup;
		_asm
		{
			push automate
			mov ecx, group
			mov eax, funcAddr
			call eax
		}
	}

	void automateAttack(unitGroup* group, unit* targetUnit)
	{
		if (!group || !targetUnit)
			return;
		group->newAutomationType = 0;
		group->newDefendXCoord = 0;
		group->newDefendYCoord = 0;
		group->newField_targetWorldData = nullptr;
		group->newTargetUnit = nullptr;
		group->newDefendRadius = 0;
		group->newField_targetX = 0;
		group->newField_targetZ = 0;
		group->newField_targetY = 0;
		group->newAutomationType = 2;
		group->newTargetUnit = targetUnit;
	}

	void automateDefense(unitGroup* group, float xCoord, float yCoord, float radius)
	{
		if (!group)
			return;
		group->newAutomationType = 0;
		group->newDefendXCoord = 0;
		group->newDefendYCoord = 0;
		group->newField_targetWorldData = nullptr;
		group->newTargetUnit = nullptr;
		group->newDefendRadius = 0;
		group->newField_targetX = 0;
		group->newField_targetZ = 0;
		group->newField_targetY = 0;
		group->newAutomationType = 1;
		group->newDefendXCoord = xCoord;
		group->newDefendYCoord = yCoord;
		group->newDefendRadius = radius;
	}

	void placeGroup(unitGroup* group, float xCoord, float yCoord, float angle)
	{
		if (!group)
			return;
		const auto coords = new float[2];
		coords[0] = xCoord;
		coords[1] = yCoord;
		DWORD funcAddr = codes::offsets.placeGroup;
		int16_t pushAngle = angleFloatToShort(angle);
		_asm
		{
			push 4
			push 1
			push pushAngle
			push coords
			mov ecx, group
			mov eax, funcAddr
			call eax
		}
		delete[] coords;
	}

	void undefineUnitGroup(const unitGroup* group)
	{
		if (!group)
			return;
		DWORD clearUnitGroup = codes::offsets.clearUnitGroup;
		_asm
		{
			mov ecx, group
			mov eax, clearUnitGroup
			call eax
		}
		if (auto label = getGroupLabel(group); strcmp(label, "") != 0)
		{
			DWORD removeUnusedLabel = codes::offsets.removeUnusedLabel;
			auto labels = getGroupLabels();
			auto labelHash = new stringWithHash();
		    fastFunctsHelpers::setCryptedString(&labelHash->name, label);
			_asm
			{
				push labelHash
				mov ecx, labels
				mov eax, removeUnusedLabel
				call eax
			}
			delete labelHash;
		}
	}

	void addUnitToGroup(unitGroup* group, unit* un)
	{
		if (!group || !un || un->unitGroup)
			return;
		un->aiActiveSet = 2;
		CallVFunc<1, void*>(group, un);
	}

	unitGroup* defineUnitGroup(const stackStruct* army, const char* label, unit* un)
	{
		if (!army || !army->unitGroups || !un || un->unitGroup)
			return nullptr;
		auto group = getEmptyGroup(army);
		if (!group)
			return nullptr;
		
		DWORD funcOffset = codes::offsets.defineUnitGroup;
		auto labels = getGroupLabels();
		stringWithHash* labelHash = new stringWithHash();
		fastFunctsHelpers::setCryptedString(&labelHash->name, label);
		unitGroup** groupPtr = &group;
		__asm
		{
			push groupPtr
			push labelHash
			mov ecx, labels
			mov eax, funcOffset
			call eax
		}
		addUnitToGroup(group, un);
		delete labelHash;
		return group;
	}

	std::string getLocalizedUnitName(const eduEntry* entry)
	{
		return technicalHelpers::uniStringToStr(*entry->localizedName);
	}

	std::string getLocalizedUnitDescr(const eduEntry* entry)
	{
		return technicalHelpers::uniStringToStr(*entry->localizedDescr);
	}

	std::string getLocalizedUnitDescrShort(const eduEntry* entry)
	{
		return technicalHelpers::uniStringToStr(*entry->localizedDescrShort);
	}

	void setUnitName(eduEntry* entry, const std::string& name)
	{

		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
		entry->localizedName = nameMem;
		smallFuncs::createUniString(*entry->localizedName, name.c_str());
	}

	void setUnitDescr(eduEntry* entry, const std::string& descr)
	{
		UNICODE_STRING*** descrMem = new UNICODE_STRING * *[4];
		entry->localizedDescr = descrMem;
		smallFuncs::createUniString(*entry->localizedDescr, descr.c_str());
	}

	void setUnitDescrShort(eduEntry* entry, const std::string& descr)
	{
		UNICODE_STRING*** shDescrMem = new UNICODE_STRING * *[4];
		entry->localizedDescrShort = shDescrMem;
		smallFuncs::createUniString(*entry->localizedDescrShort, descr.c_str());
	}
	
    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::usertype<unit>unit;
            sol::usertype<unitPositionData>unitPositionData;
            sol::usertype<unitGroup> unitGroup;
            sol::usertype<siegeEngine> siegeEngineStruct;
        }types;
    	
		///Unit
		//@section unitTable

		/***
		Unit table

		@tfield eduEntry eduEntry
		@tfield float movePoints
		@tfield int aiActiveSet  0 means inactive, 1 means active, 2 means labelled unit (for Battle)
		@tfield int soldierCountStratMap soldiers count. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
		@tfield int exp soldiers expierence. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
		@tfield int armourLVL soldiers armour. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
		@tfield int weaponLVL soldiers weapon. You can change it on stratmap and soldiers updated. Use @{setParams} if you need change several parameters at once.
		@tfield int soldierCountStratMapMax Read only
		@tfield int soldierCountBattleMap Read only
		@tfield int moraleLevel use moraleStatus enum
		@tfield int isCloseFormation
		@tfield int ID
		@tfield int flankRightState
		@tfield int flankLeftState
		@tfield int flankRearState
		@tfield int soldiersFled
		@tfield bool isInfighting
		@tfield bool inPlayableArea
		@tfield bool isHidden
		@tfield bool canBeSeen
		@tfield bool hasBeenSeen
		@tfield bool inFormation
		@tfield bool isMercenary
		@tfield bool stakesUsed
		@tfield bool specialAbilityActive
		@tfield bool holyInspired
		@tfield bool dead
		@tfield bool unableToFire
		@tfield unit flankRightThreat
		@tfield unit flankLeftThreat
		@tfield unit flankRearThreat
		@tfield int nearbyFriendlyUnitsNum
		@tfield int nearbyEnemyUnitsNum
		@tfield settlementStruct retrainingSettlement
		@tfield unitGroup unitGroup
		@tfield int fatigue (battle)
		@tfield float angle (battle)
		@tfield int maxAmmo (battle)
		@tfield int currentAmmo (battle)
		@tfield float battlePosZ (battle)
		@tfield float battlePosX (battle)
		@tfield float battlePosY (battle)
		@tfield character character
		@tfield unitPositionData unitPositionData
		@tfield stackStruct army
		@tfield int siegeEngineNum (only infantry and artillery units!)
		@tfield kill kill
		@tfield setParams setParams change soldierCountStratMap, exp, armourLVL, weaponLVL at one time.
		@tfield hasAttribute hasAttribute Check if unit has edu attribute.
		@tfield string alias
		@tfield hasBattleProperty hasBattleProperty
		@tfield setBattleProperty setBattleProperty
		@tfield getActionStatus getActionStatus
		@tfield isMovingFastSet isMovingFastSet
		@tfield setMovingFastSet setMovingFastSet
		@tfield isOnWalls isOnWalls
		@tfield int supplies
		@tfield isEngaged isEngaged
		@tfield isUnderFire isUnderFire
		@tfield getMountClass getMountClass
		@tfield moveToPosition moveToPosition
		@tfield immediatePlace immediatePlace
		@tfield getFormation getFormation
		@tfield attackClosestUnit attackClosestUnit
		@tfield attackUnit attackUnit
		@tfield deployStakes deployStakes
		@tfield changeFormation changeFormation
		@tfield halt halt
		@tfield moveToOrientation moveToOrientation
		@tfield moveRelative moveRelative
		@tfield moveToMissileRange moveToMissileRange
		@tfield turn turn
		@tfield taunt taunt
		@tfield useSpecialAbility useSpecialAbility
		@tfield getSiegeEngine getSiegeEngine
		@tfield attackBuilding attackBuilding
		@tfield collectEngine collectEngine
		@tfield getNearbyFriendlyUnit getNearbyFriendlyUnit
		@tfield getNearbyEnemyUnit getNearbyEnemyUnit
		@tfield releaseUnit releaseUnit

		@table unit
		*/
		types.unit = luaState.new_usertype<unit>("unit");
		types.unit.set("eduEntry", &unit::eduEntry);
		types.unit.set("aiActiveSet", &unit::aiActiveSet);
		types.unit.set("movePoints", sol::property(&unit::getMovepoints, &setUnitMovePoints));
		types.unit.set("soldierCountStratMap", sol::property(&unit::getSoldierCountStratMap, &setSoldiersCount));
		types.unit.set("exp", sol::property(&unit::getExp, &unit::setExp));
		types.unit.set("armourLVL", sol::property(&unit::getArmourLevel, &unit::setarmourLVL));
		types.unit.set("weaponLVL", sol::property(&unit::getWeaponLevel, &unit::setweaponLVL));
		types.unit.set("soldierCountStratMapMax", sol::property(&getMaxSoldiersCount));
		types.unit.set("soldierCountBattleMap", &unit::SoldierCountBattlemap);
		types.unit.set("unitPositionData", &unit::unitPositionData);
		types.unit.set("character", &unit::general);
		types.unit.set("flankRightState", &unit::flankRightState);
		types.unit.set("flankLeftState", &unit::flankLeftState);
		types.unit.set("flankRearState", &unit::flankRearState);
		types.unit.set("flankRightThreat", &unit::flankRightThreat);
		types.unit.set("flankLeftThreat", &unit::flankLeftThreat);
		types.unit.set("flankRearThreat", &unit::flankRearThreat);
		types.unit.set("soldiersFled", &unit::soldiersFled);
		types.unit.set("isInfighting", &unit::isInfighting);
		types.unit.set("isCloseFormation", &unit::isCloseFormation);
		types.unit.set("moraleLevel", sol::property(&unit::getMoraleLevel, &unit::setMoraleLevel));
		types.unit.set("inPlayableArea", sol::property(&unit::getInPlayableArea, &unit::setInPlayableArea));
		types.unit.set("isHidden", sol::property(&unit::getIsHidden, &unit::setIsHidden));
		types.unit.set("canBeSeen", sol::property(&unit::getCanBeSeen, &unit::setCanBeSeen));
		types.unit.set("hasBeenSeen", sol::property(&unit::getHasBeenSeen, &unit::setHasBeenSeen));
		types.unit.set("inFormation", sol::property(&unit::getInFormation, &unit::setInFormation));
		types.unit.set("isMercenary", sol::property(&unit::getIsMercenary, &unit::setIsMercenary));
		types.unit.set("stakesUsed", sol::property(&unit::getStakesUsed, &unit::setStakesUsed));
		types.unit.set("specialAbilityActive", sol::property(&unit::getSpecialAbilityActive, &unit::setSpecialAbilityActive));
		types.unit.set("holyInspired", sol::property(&unit::getHolyInspired, &unit::setHolyInspired));
		types.unit.set("dead", sol::property(&unit::getDead, &unit::setDead));
		types.unit.set("unableToFire", sol::property(&unit::getUnableToFire, &unit::setUnableToFire));
		types.unit.set("battlePosX", &unit::positionX);
		types.unit.set("battlePosZ", &unit::positionZ);
		types.unit.set("battlePosY", &unit::positionY);
		types.unit.set("angle", sol::property(&unit::getUnitAngle, &unit::setUnitAngle));
		types.unit.set("unitGroup", &unit::unitGroup);
		types.unit.set("fatigue", &unit::fatigue);
		types.unit.set("maxAmmo", &unit::maxAmmo);
		types.unit.set("ID", &unit::ID);
		types.unit.set("currentAmmo", &unit::currentAmmo);
		types.unit.set("siegeEngineNum", sol::property(&unit::getSiegeEngineNum));
		types.unit.set("army", &unit::army);
		types.unit.set("retrainingSettlement", &unit::retrainingSettlement);
		types.unit.set("supplies", &unit::foodRequirement);
		types.unit.set("nearbyFriendlyUnitsNum", &unit::nearbyFriendlyUnitsNum);
		types.unit.set("nearbyEnemyUnitsNum", &unit::nearbyEnemyUnitsNum);
		/***
		Kill this unit
		@function unit:kill
		@usage
		unit:kill();
		*/
		types.unit.set_function("kill", &killUnit);
		/***
		Set unit basic parameters
		@function unit:setParams
		@tparam int exp Experience. Maximum: 9
		@tparam int armor Armour level.
		@tparam int weapon Weapon Upgrade. Maximum: 1
		@usage
		unit:setParams(0,0,0);
		*/
		types.unit.set_function("setParams", &unit::setUnitParamsLua);
		types.unit.set("alias", sol::property(&unitUniStringToStr, &setUnitUniStr));
		/***
		Check if unit has edu attribute.
		@function unit:hasAttribute
		@usage
		local hasAttr = unit:hasAttribute("sea_faring");
		*/
		types.unit.set_function("hasAttribute", &eopEduHelpers::hasAttribute);
		/***
		Check if unit has guard mode, skirmish or fire at will on.
		@function unit:hasBattleProperty
		@tparam int property use enum: unitBattleProperties.guardMode, unitBattleProperties.skirmish, unitBattleProperties.fireAtWill
		@treturn bool hasProperty
		@usage
		local hasProp = unit:hasBattleProperty(unitBattleProperties.guardMode);
		*/
		types.unit.set_function("hasBattleProperty", &unit::hasBattleProperty);
		/***
		Set a unit battle property (guard mode, skirmish or fire at will).
		@function unit:setBattleProperty
		@tparam int property use enum: unitBattleProperties.guardMode, unitBattleProperties.skirmish, unitBattleProperties.fireAtWill
		@tparam bool value
		@usage
		unit:setBattleProperty(unitBattleProperties.skirmish, true);
		*/
		types.unit.set_function("setBattleProperty", &unit::setBattleProperty);
		/***
		Get unit action status in battle ( idling, hiding, ready, reforming, moving, withdrawing, missiles\_firing, missiles\_reloading, charging, fighting, pursuing, routing, fighting\_backs\_to\_the\_walls, running\_amok, rallying, dead, leaving\_battle, entering\_battle, left\_battle, go\_berserk, taunting, bracing, infighting).
		@function unit:getActionStatus
		@treturn string actionStatus
		@usage
		local status = unit:getActionStatus();
		*/
		types.unit.set_function("getActionStatus", &unit::getActionStatus);
		/***
		Is unit set to run?
		@function unit:isMovingFastSet
		@treturn bool movingFastSet
		@usage
		local isRunning = unit:isMovingFastSet();
		*/
		types.unit.set_function("isMovingFastSet", &unit::isMovingFastSet);
		/***
		Toggle unit running.
		@function unit:setMovingFastSet
		@tparam bool movingFastSet
		@usage
		unit:setMovingFastSet(true);
		*/
		types.unit.set_function("setMovingFastSet", &unit::setMovingFastSet);
		/***
		Is unit on walls?
		@function unit:isOnWalls
		@treturn bool isOnWalls
		@usage
		local isOnWalls = unit:isOnWalls();
		*/
		types.unit.set_function("isOnWalls", &unit::isOnWalls);
		/***
		Is unit engaged in melee?
		@function unit:isEngaged
		@treturn bool isEngaged
		@usage
		local isEngaged = unit:isEngaged();
		*/
		types.unit.set_function("isEngaged", &unit::isEngaged);
		/***
		Is unit under fire?
		@function unit:isUnderFire
		@treturn bool isUnderFire
		@usage
		local isUnderFire = unit:isUnderFire();
		*/
		types.unit.set_function("isUnderFire", &unit::isUnderFire);
		/***
		Get unit mount class.
		@function unit:getMountClass
		@treturn int mountClass
		@usage
		local mountClass = unit:getMountClass();
		*/
		types.unit.set_function("getMountClass", &unit::getMountClass);
		/***
		Get unit formation type.
		@function unit:getFormation
		@treturn int formationType
		@usage
		local formationType = unit:getFormation();
		*/
		types.unit.set_function("getFormation", &unit::getUnitFormation);
		/***
		Orders the unit to move to the specified position.
		@function unit:moveToPosition
		@tparam float xCoord
		@tparam float yCoord
		@tparam bool run
		@usage
			unit:moveToPosition(150, -300, true);
		*/
		types.unit.set_function("moveToPosition", &unitActions::unitMovetoPosition);
		/***
		Immediately positions the unit at the given location.
		@function unit:immediatePlace
		@tparam float xCoord
		@tparam float yCoord
		@tparam int angle
		@tparam int width
		@usage
			unit:immediatePlace(150, -300, 60, 0);
		*/
		types.unit.set_function("immediatePlace", &unitActions::placeUnit);
		/***
		Searches an arc in front of the unit, and attacks the closest enemy found.
		@function unit:attackClosestUnit
		@tparam int angle
		@tparam bool run
		@usage
			unit:attackClosestUnit(0, true);
		*/
		types.unit.set_function("attackClosestUnit", &unitActions::unitAttackClosest);
		/***
		Orders the unit to attack another unit.
		@function unit:attackUnit
		@tparam unit targetUnit
		@tparam bool run
		@usage
			unit:attackUnit(target, true);
		*/
		types.unit.set_function("attackUnit", &unitActions::attackUnit);
		/***
		Deploys stakes.
		@function unit:deployStakes
		@usage
			unit:deployStakes();
		*/
		types.unit.set_function("deployStakes", &unitActions::deployStakes);
		/***
		Changes the unit's formation.
		@function unit:changeFormation
		@tparam int formationType use formationType enum
		@usage
			unit:changeFormation();
		*/
		types.unit.set_function("changeFormation", &unitActions::changeUnitFormation);
		/***
		Orders the unit to stop it's orders.
		@function unit:halt
		@usage
			unit:halt();
		*/
		types.unit.set_function("halt", &unitActions::haltUnit);
		/***
		Orders the unit to move to the specified position with a specified rotation and orientation.
		@function unit:moveToOrientation
		@tparam float xCoord
		@tparam float yCoord
		@tparam int widthInMen
		@tparam int angle
		@tparam bool run
		@usage
			unit:moveToOrientation(150, 105, 100, 60, true);
		*/
		types.unit.set_function("moveToOrientation", &unitActions::moveToOrientation);
		/***
		Orders the unit to move to the specified position.
		@function unit:moveRelative
		@tparam float xCoord
		@tparam float yCoord
		@tparam bool run
		@usage
			unit:moveRelative(150, 105, true);
		*/
		types.unit.set_function("moveRelative", &unitActions::moveRelative);
		/***
		Orders the attacker to move into missile range of the target.
		@function unit:moveToMissileRange
		@tparam unit targetUnit
		@tparam bool run
		@usage
			unit:moveToMissileRange(targetUnit, true);
		*/
		types.unit.set_function("moveToMissileRange", &unitActions::moveToMissileRange);
		/***
		Turn to an absolute angle or by an angle relative to it's current rotation.
		@function unit:turn
		@tparam int angle
		@tparam bool isRelative
		@usage
			unit:turn(90, true);
		*/
		types.unit.set_function("turn", &unitActions::unitTurn);
		/***
		Makes the unit taunt.
		@function unit:taunt
		@usage
			unit:taunt();
		*/
		types.unit.set_function("taunt", &unitActions::taunt);
		/***
		Makes the unit perform their special ability.
		@function unit:useSpecialAbility
		@usage
			unit:useSpecialAbility();
		*/
		types.unit.set_function("useSpecialAbility", &unitActions::useSpecialAbility);
		/***
		Get Siege engine.
		@function unit:getSiegeEngine
		@tparam int index
		@treturn siegeEngineStruct engine
		@usage
			local engine = unit:getSiegeEngine(0);
		*/
		types.unit.set_function("getSiegeEngine", &unit::getSiegeEngine);
		/***
		Order a unit with appropriate siege equipment to attack an appropriate building (including docking towers/ladders to walls).
		@function unit:attackBuilding
		@tparam buildingBattle building
		@usage
			unit:attackBuilding(building);
		*/
		types.unit.set_function("attackBuilding", &unitActions::attackBuilding);
		/***
		Order an infantry unit to collect an engine (ram/ladder/tower).
		@function unit:collectEngine
		@tparam siegeEngineStruct engine
		@usage
			unit:collectEngine(engine);
		*/
		types.unit.set_function("collectEngine", &unitActions::collectEngine);
		/***
		Get Nearby friendly unit.
		@function unit:getNearbyFriendlyUnit
		@tparam int index
		@treturn unit nearUnit
		@usage
			local nearUnit = unit:getNearbyFriendlyUnit(0);
		*/
		types.unit.set_function("getNearbyFriendlyUnit", &unit::getNearbyUnit);
		/***
		Get Nearby Enemy unit.
		@function unit:getNearbyEnemyUnit
		@tparam int index
		@treturn unit nearUnit
		@usage
			local nearUnit = unit:getNearbyEnemyUnit(0);
		*/
		types.unit.set_function("getNearbyEnemyUnit", &unit::getNearbyEnemyUnit);
		/***
		Set ai active set to on or off depending on if the unit is player controlled.
		@function unit:releaseUnit
		@usage
			unit:releaseUnit();
		*/
		types.unit.set_function("releaseUnit", &unit::releaseUnit);
		
		///Unit Position Data
		//@section unitPositionData

		/***

		@tfield int engagedUnitsNum
		@tfield unit unit
		@tfield int isOnWallsCount (amount of walls
		@tfield int isInTowerCount (amount of towers)
		@tfield int isInGateHouseCount (amount of gatehouses)
		@tfield bool isHalted
		@tfield bool hasTargets
		@tfield int combatStatus use enum: combatStatus
		@tfield float lastTargetCoordX dont set
		@tfield float lastTargetCoordY dont set
		@tfield int towersUnderFireFromCount
		@tfield int unitsUnderFireFromCount
		@tfield getUnitUnderFireFrom getUnitUnderFireFrom
		@tfield getEngagedUnit getEngagedUnit
		@tfield getTargetUnit getTargetUnit

		@table unitPositionData
		*/
		types.unitPositionData = luaState.new_usertype<unitPositionData>("unitPositionData");
		types.unitPositionData.set("engagedUnitsNum", &unitPositionData::engagedUnitsNum);
		types.unitPositionData.set("unit", &unitPositionData::thisUnit);
		types.unitPositionData.set("isOnWallsCount", &unitPositionData::isOnWallsCount);
		types.unitPositionData.set("isInTowerCount", &unitPositionData::isInTowerCount);
		types.unitPositionData.set("isInGateHouseCount", &unitPositionData::isInGateHouseCount);
		types.unitPositionData.set("hasTargets", &unitPositionData::hasTargets);
		types.unitPositionData.set("combatStatus", &unitPositionData::combatStatus);
		types.unitPositionData.set("isHalted", &unitPositionData::isHalted);
		types.unitPositionData.set("lastTargetCoordX", &unitPositionData::lastTargetCoordX);
		types.unitPositionData.set("lastTargetCoordY", &unitPositionData::lastTargetCoordY);
		types.unitPositionData.set("towersUnderFireFromCount", &unitPositionData::towersUnderFireFromCount);
		types.unitPositionData.set("unitsUnderFireFromCount", &unitPositionData::unitsUnderFireFromCount);

		/***
		Get unit that is firing at this unit.
		@function unitPositionData:getUnitUnderFireFrom
		@tparam int index
		@treturn unit unit
		@usage
		local enemyUnit = unit.unitPositionData:getUnitUnderFireFrom(0);
		*/
		types.unitPositionData.set_function("getUnitUnderFireFrom", &unitPositionData::getUnitUnderFireFrom);

		/***
		Get unit that is fighting this unit in melee.
		@function unitPositionData:getEngagedUnit
		@tparam int index
		@treturn unit unit
		@usage
		local enemyUnit = unit.unitPositionData:getEngagedUnit(0);
		*/
		types.unitPositionData.set_function("getEngagedUnit", &unitPositionData::getEngagedUnit);

		/***
		Get the unit this unit is currently targeting.
		@function unitPositionData:getTargetUnit
		@treturn unit unit
		@usage
		local enemyUnit = unit.unitPositionData:getTargetUnit();
		*/
		types.unitPositionData.set_function("getTargetUnit", &unitPositionData::getTargetUnit);
    	

    	///Siege Engine
    	//@section Siege Engine

    	/***
		Basic siegeEngineStruct table
	
		@tfield float xCoord
		@tfield float zCoord
		@tfield float yCoord
		@tfield float mass
		@tfield int angle
		@tfield int engineID
		@tfield unit currentUnit
		@tfield unit lastUnit
		@tfield getType getType
		@tfield int fireLevel
		@tfield int isBurning
	
		@table siegeEngineStruct
		*/
    	types.siegeEngineStruct = luaState.new_usertype<siegeEngine>("siegeEngineStruct");
    	types.siegeEngineStruct.set("xCoord", &siegeEngine::posX);
    	types.siegeEngineStruct.set("zCoord", &siegeEngine::posZ);
    	types.siegeEngineStruct.set("yCoord", &siegeEngine::posY);
    	types.siegeEngineStruct.set("mass", &siegeEngine::mass);
    	types.siegeEngineStruct.set("angle", sol::property(&getEngineAngle, &setEngineAngle));
    	types.siegeEngineStruct.set("currentUnit", &siegeEngine::currentUnit);
    	types.siegeEngineStruct.set("lastUnit", &siegeEngine::lastUnit);
    	types.siegeEngineStruct.set("engineID", &siegeEngine::engineID);
    	types.siegeEngineStruct.set("fireLevel", &siegeEngine::fireLevel);
    	types.siegeEngineStruct.set("isBurning", &siegeEngine::isBurning);

    	/***
		Get the type of the engine (use the enum).
		@function siegeEngineStruct:getType
		@treturn int type
		@usage
			local type = siegeEngineStruct:getType()
		*/
    	types.siegeEngineStruct.set_function("getType", &battleHandlerHelpers::getEngineType);
    	
		///unitGroup
		//@section Unit Group

		/***
		Basic unitGroup table

		@tfield int unitsInFormationNum
		@tfield int unitsNotInFormationNum
		@tfield int unitNumTotal
		@tfield float xCoord
		@tfield float yCoord
		@tfield int angle
		@tfield int automationType 1 = defend, 2 = attack
		@tfield float defendXCoord if automated defend (1)
		@tfield float defendYCoord if automated defend (1)
		@tfield float defendRadius if automated defend (1)
		@tfield unit targetUnit if automated attack (2)
		@tfield int newAutomationType 1 = defend, 2 = attack (setting this passes info from new fields to active fields)
		@tfield float newDefendXCoord if automated defend (1)
		@tfield float newDefendYCoord if automated defend (1)
		@tfield float newDefendRadius if automated defend (1)
		@tfield unit newTargetUnit if automated attack (2)
		@tfield getUnitInFormation getUnitInFormation
		@tfield getUnitNotInFormation getUnitNotInFormation
		@tfield addUnit addUnit
		@tfield removeUnit removeUnit
		@tfield undefine undefine
		@tfield automate automate
		@tfield automateAttack automateAttack
		@tfield automateDefense automateDefense
		@tfield changeUnitFormation changeUnitFormation
		@tfield moveToMissileRangeOfGroup moveToMissileRangeOfGroup
		@tfield moveToMissileRangeOfUnit moveToMissileRangeOfUnit
		@tfield attackGroup attackGroup
		@tfield halt halt
		@tfield place place
		@tfield moveFormed moveFormed
		@tfield moveUnformed moveUnformed
		@tfield moveRelativeFormed moveRelativeFormed
		@tfield moveRelativeUnformed moveRelativeUnformed
		@tfield turn turn
		

		@table unitGroup
		*/
		types.unitGroup = luaState.new_usertype<unitGroup>("unitGroup");
		types.unitGroup.set("unitsInFormationNum", &unitGroup::unitsInFormationNum);
		types.unitGroup.set("unitsNotInFormationNum", &unitGroup::unitsNotInFormationNum);
		types.unitGroup.set("unitNumTotal", &unitGroup::unitsNumTotal);
		types.unitGroup.set("xCoord", &unitGroup::xCoord);
		types.unitGroup.set("yCoord", &unitGroup::yCoord);
		types.unitGroup.set("angle", &unitGroup::angle);
		types.unitGroup.set("automationType", &unitGroup::automationType);
		types.unitGroup.set("defendXCoord", &unitGroup::defendXCoord);
		types.unitGroup.set("defendYCoord", &unitGroup::defendYCoord);
		types.unitGroup.set("defendRadius", &unitGroup::defendRadius);
		types.unitGroup.set("targetUnit", &unitGroup::targetUnit);
		types.unitGroup.set("newAutomationType", &unitGroup::newAutomationType);
		types.unitGroup.set("newDefendXCoord", &unitGroup::newDefendXCoord);
		types.unitGroup.set("newDefendYCoord", &unitGroup::newDefendYCoord);
		types.unitGroup.set("newDefendRadius", &unitGroup::newDefendRadius);
		types.unitGroup.set("newTargetUnit", &unitGroup::newTargetUnit);
		
		/***
		Add a unit to the group (won't add if unit already in group).
		@function unitGroup:addUnit
		@tparam unit unit
		@usage
				unitGroup:addUnit(unit);
		*/
		types.unitGroup.set_function("addUnit", &addUnitToGroup);
		
		/***
		Remove a unit from a group.
		@function unitGroup:removeUnit
		@tparam unit unit
		@usage
				unitGroup:removeUnit(unit);
		*/
		types.unitGroup.set_function("removeUnit", &removeUnitFromGroup);
		
		/***
		Undefine a unit group.
		@function unitGroup:undefine
		@usage
				unitGroup:undefine();
		*/
		types.unitGroup.set_function("undefine", &undefineUnitGroup);
		
		/***
		Toggle group automation.
		@function unitGroup:automate
		@tparam bool automate
		@usage
				unitGroup:automate(true);
		*/
		types.unitGroup.set_function("automate", &automateGroup);
		
		/***
		Automate group attack.
		@function unitGroup:automateAttack
		@tparam unit targetUnit
		@usage
				unitGroup:automateAttack(targetUnit);
		*/
		types.unitGroup.set_function("automateAttack", &automateAttack);
		
		/***
		Automate group defense.
		@function unitGroup:automateDefense
		@tparam float xCoord
		@tparam float yCoord
		@tparam float radius
		@usage
				unitGroup:automateDefense(180, 283, 50);
		*/
		types.unitGroup.set_function("automateDefense", &automateDefense);
		
		/***
		Get a unit in the group's formation (not given individual commands). Once you give a unit a command once it seems they will always be in the other array.
		@function unitGroup:getUnitInFormation
		@tparam int index
		@treturn unit unit
		@usage
				local un = unitGroup:getUnitInFormation(0);
		*/
		types.unitGroup.set_function("getUnitInFormation", &unitGroup::getUnitInFormation);
		
		/***
		Get a unit not in the group's formation (not given individual commands). Once you give a unit a command once it seems they will always be in the other array.
		@function unitGroup:getUnitNotInFormation
		@tparam int index
		@treturn unit unit
		@usage
				local un = unitGroup:getUnitNotInFormation(0);
		*/
		types.unitGroup.set_function("getUnitNotInFormation", &unitGroup::getUnitNotInFormation);
		
		/***
		Place a group at a location.
		@function unitGroup:place
		@tparam float xCoord
		@tparam float yCoord
		@tparam float angle
		@usage
				unitGroup:place(150, 127, 0);
		*/
		types.unitGroup.set_function("place", &placeGroup);
		
		/***
		Change the group's units formations.
		@function unitGroup:changeUnitFormation
		@tparam int formationType use enum
		@usage
				unitGroup:changeUnitFormation(formationType.phalanx);
		*/
		types.unitGroup.set_function("changeUnitFormation", &unitActions::changeGroupUnitFormation);
		
		/***
		Move to put an enemy group inside your missile range.
		@function unitGroup:moveToMissileRangeOfGroup
		@tparam unitGroup targetGroup
		@tparam bool run
		@usage
				unitGroup:moveToMissileRangeOfGroup(otherGroup, true);
		*/
		types.unitGroup.set_function("moveToMissileRangeOfGroup", &unitActions::moveToRangeOfGroup);
		
		/***
		Move to put an enemy unit inside your missile range.
		@function unitGroup:moveToMissileRangeOfUnit
		@tparam unit targetUnit
		@tparam bool run
		@usage
				unitGroup:moveToMissileRangeOfUnit(targetUnit, true);
		*/
		types.unitGroup.set_function("moveToMissileRangeOfUnit", &unitActions::moveGroupToRangeOfUnit);
		
		/***
		Attack another group.
		@function unitGroup:attackGroup
		@tparam unitGroup targetGroup
		@tparam bool run
		@usage
				unitGroup:attackGroup(targetGroup, true);
		*/
		types.unitGroup.set_function("attackGroup", &unitActions::groupAttackGroup);
		
		/***
		Halt the group's orders.
		@function unitGroup:halt
		@usage
				unitGroup:halt();
		*/
		types.unitGroup.set_function("halt", &unitActions::groupHalt);
		
		/***
		Move to the specified location in formation.
		@function unitGroup:moveFormed
		@tparam float xCoord
		@tparam float yCoord
		@tparam bool run
		@usage
				unitGroup:moveFormed(182, 333, true);
		*/
		types.unitGroup.set_function("moveFormed", &unitActions::groupMoveFormed);
		
		/***
		Move to the specified location not in formation.
		@function unitGroup:moveUnformed
		@tparam float xCoord
		@tparam float yCoord
		@tparam bool run
		@usage
				unitGroup:moveUnformed(182, 333, true);
		*/
		types.unitGroup.set_function("moveUnformed", &unitActions::groupMoveUnformed);
		
		/***
		Move to the specified location in formation.
		@function unitGroup:moveRelativeFormed
		@tparam float xCoord
		@tparam float yCoord
		@tparam bool run
		@usage
				unitGroup:moveRelativeFormed(182, 333, true);
		*/
		types.unitGroup.set_function("moveRelativeFormed", &unitActions::groupMoveFormedRelative);
		
		/***
		Move to the specified location not in formation.
		@function unitGroup:moveRelativeUnformed
		@tparam float xCoord
		@tparam float yCoord
		@tparam bool run
		@usage
				unitGroup:moveRelativeUnformed(182, 333, true);
		*/
		types.unitGroup.set_function("moveRelativeUnformed", &unitActions::groupMoveUnformedRelative);
		
		/***
		Turn the group either relative or absolute.
		@function unitGroup:turn
		@tparam int angle
		@tparam bool isRelative
		@usage
				unitGroup:turn(90, true);
		*/
		types.unitGroup.set_function("turn", &unitActions::groupTurn);
	
    	

    }
}
