///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "unit.h"

#include "character.h"
#include "dataOffsets.h"
#include "eopdu.h"
#include "luaPlugin.h"
#include "gameStringHelpers.h"
#include "functionsOffsets.h"
#include "faction.h"
#include "army.h"
#include "battle.h"

#include "gameHelpers.h"
#include "strategyMap.h"

std::unique_ptr<eopMountedEngineDb> eopMountedEngineDb::m_MountedEngineDb = std::make_unique<eopMountedEngineDb>();

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
	adrFunc = codes::offsets.setUnitWeaponFunc;
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

int unit::getActionStatusInt()
{
	return actionStatus;
}

unitStatus unit::getActionStatusEnum()
{
	return static_cast<unitStatus>(actionStatus);
}

bool unit::isFiring()
{
	const auto status = getActionStatusEnum();
	return status == unitStatus::missilesFiring || status == unitStatus::missilesReloading;
}

bool unit::isIdle()
{
	const auto status = getActionStatusInt();
	return status < 5;
}

void unit::setArmour(const uint8_t armour)
{
	const auto arm = armour;
	GAME_FUNC(void(__thiscall*)(unit*, uint8_t), setUnitArmorFunc)(this, arm);
}

void unit::setExperience(const uint8_t exp)
{
	expScreen = exp;
	for (int i = 0; i < SoldierCountStrat; i++)
	{
		soldiersArr[i].experience = exp;
	}
}

void unit::setWeapon(uint8_t wpn)
{
	const auto weapon = wpn;
	GAME_FUNC(void(__thiscall*)(unit*, uint8_t), setUnitWeaponFunc)(this, weapon);
}

bool unitDb::parse(descrParser* parser)
{
	while (parser->walker < parser->end)
	{
		eduEntry* newEntry;
		bool isEop = false;
		if (this->numberOfEntries < 500)
		{
			newEntry = &unitEntries[this->numberOfEntries];
			this->numberOfEntries++;
		}
		else
		{
			newEntry = techFuncs::createGameClass<eduEntry>();
			GAME_FUNC(void(__thiscall*)(eduEntry*), createEduEntry)(newEntry);
			isEop = true;
		}
		if (!GAME_FUNC(bool(__thiscall*)(eduEntry*, descrParser*), readEDUEntryFunc)(newEntry, parser))
		{
			gameHelpers::logStringGame("DATABASE_TABLE error found : error reading record from file  " + std::string(parser->getFileName()));
			return false;
		}
		if (!isEop)
		{
			char** name = &newEntry->eduType; 
			if (GAME_FUNC(int(__thiscall*)(int*, char**), dbHashTableGet)(&this->maxEntryNum, name))
			{
				gameHelpers::logStringGame("DATABASE_TABLE error found : ids must be unique, non-unique entry " + std::string(*name) + " found in file " + parser->getFileName());
				return false;
			}
			GAME_FUNC(void(__thiscall*)(int*, char**, unsigned long*), dbHashTableSet)(&this->maxEntryNum, name, &newEntry->index);
		}
		else
		{
			eopDu::addEopEduEntryFromEdu(newEntry);
		}
		
	}
	return true;
}

bool mountedEngineDb::parse(descrParser* parser)
{
	while (parser->walker < parser->end)
	{
		mountedEngine* newEngine;
		if (this->mountedEngineNum < 4)
		{
			newEngine = &this->mountedEngines[this->mountedEngineNum];
			this->mountedEngineNum++;
		}
		else
		{
			newEngine = techFuncs::createGameClass<mountedEngine>();
			GAME_FUNC(void(__thiscall*)(mountedEngine*), createMountedEngineRecord)(newEngine);
		}
		if (!GAME_FUNC(bool(__thiscall*)(mountedEngine*, descrParser*), ParseDescrMountedEngineEntry)(newEngine, parser))
		{
			gameHelpers::logStringGame("DATABASE_TABLE error found: error reading record from file " + std::string(parser->getFileName()));
			return false;
		}
		
		if (eopMountedEngineDb::get()->getMountedEngine(newEngine->record.name.getString()))
			gameHelpers::logStringGame("mountedEngineDb::parse: Added duplicate engine, overwriting");
		
		eopMountedEngineDb::get()->addMountedEngine(newEngine->record.name.getString(), newEngine);
	}
	return true;
}

namespace unitActions
{
	
    unit* getUnitByLabel(const char* label)
    {
    	if (!label)
    		return nullptr;
        const auto labelCrypt = gameStringHelpers::createHashedStringGame(label);
        const auto un = GAME_FUNC(unit*(__cdecl*)(stringWithHash*), getUnitByLabel)(labelCrypt);
    	gameStringHelpers::freeHashString(labelCrypt);
    	if (!un)
    	{
    		gameHelpers::logStringGame("unitActions::getUnitByLabel: unit not found with label: " + std::string(label));
    	}
	    return un;
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<unitImmediatePlace>(un, xCoord, yCoord, angle, width);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitImmediatePlace);
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
        if (!battleHelpers::inBattle())
            return;
        unit->aiActiveSet = 2;
        const auto order = std::make_shared<unitOrderMove>(unit, xCoord, yCoord, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitOrderMove);
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
        if (!battleHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitAttackClosestUnit>(un, angle, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitAttackClosestUnit);
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
        if (!battleHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitAttackUnit>(un, targetUnit, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitAttackUnit);
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
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitDeployStakes);
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
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitChangeFormation);
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
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitHalt);
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
        if (!battleHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitMoveToOrientation>(un, xCoord, yCoord, widthInMen, angle, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitMoveToOrientation);
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
        if (!battleHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitMoveRelative>(un, xCoord, yCoord, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.moveRelative);
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
        if (!battleHelpers::inBattle())
            return;
        un->aiActiveSet = 2;
        const auto order = std::make_shared<unitMoveToMissileRange>(un, targetUnit, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.moveToMissileRange);
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
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitTurn);
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
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.unitTaunt);
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
    
    void useSpecialAbility(const unit* un, bool heroOnly)
    {
        if (un == nullptr)
            return;
    	const heroAbility* ability = nullptr;
    	if (un->generalInfo)
    		ability = un->generalInfo->ability;
    	if (!ability && heroOnly)
			return;
    	if (ability)
    	{
		    if (ability->usesLeft > 0 && ability->abilityTimer > 0)
				return;
    		if (heroOnly && ability->usesLeft == 0)
				return;
    	}
        const auto order = std::make_shared<unitUseSpecialAbility>(un);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.useSpecialAbility);
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
        unit->unitPositionData->hasTargets = true;
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupChangeUnitFormation>(group, formationType);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupUnitChangeFormation);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupMoveToRangeOfGroup>(group, targetGroup, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveToRangeOfGroup);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupMoveToRangeOfUnit>(group, targetUnit, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveToRangeOfUnit);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderAttackGroup>(group, targetGroup, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupAttackGroup);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderHalt>(group);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupHalt);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderMoveFormed>(group, xCoord, yCoord, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveFormed);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderMoveUnformed>(group, xCoord, yCoord, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupMoveUnformed);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderRelativeMoveFormed>(group, xCoord, yCoord, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupRelativeMoveFormed);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderRelativeMoveUnformed>(group, xCoord, yCoord, run);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupRelativeMoveUnformed);
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
        if (!battleHelpers::inBattle())
            return;
        const auto order = std::make_shared<groupOrderTurn>(group, angle, isRelative);
        gameHelpers::fireGameScriptFunc(order.get(), codes::offsets.groupTurn);
    }
    
    void collectEngine(unit* un, siegeEngine* engine)
    {
        if (!battleHelpers::inBattle())
            return;
        if (!un || !engine || un->eduEntry->category != 0)
            return;
        haltUnit(un);
        un->aiActiveSet = 2;
        un->unitPositionData->targetArray[0].siegeEngine = engine;
        un->unitPositionData->targetArray[0].actionType = static_cast<int>(unitActionType::unitCollectEngine);
        un->unitPositionData->isHalted = false;
        un->unitPositionData->hasTargets = true;
        un->unitPositionData->targetArray.num = 1;
    }
}

namespace unitHelpers
{
	

	unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (!type)
		{
			gameHelpers::logStringGame("Can not create unit, type is null");
			return nullptr;
		}
		const int unitIndex = getEduIndex(type);
		if (unitIndex == -1)
		{
			gameHelpers::logStringGame("Can not create unit, type not found: " + std::string(type));
			return nullptr;
		}
		return createUnitIdx2(unitIndex, regionID, facNum, exp, arm, weap, -1);
	}
	
	unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap, int soldierCount)
	{
		if (!type)
		{
			gameHelpers::logStringGame("Can not create unit, type is null");
			return nullptr;
		}
		const int unitIndex = getEduIndex(type);
		if (unitIndex == -1)
		{
			gameHelpers::logStringGame("Can not create unit, type not found: " + std::string(type));
			return nullptr;
		}
		return createUnitIdx2(unitIndex, regionID, facNum, exp, arm, weap, soldierCount);
	}

	unit* createUnitIdx(int index, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (index == -1)
			return nullptr;

		unit* res = nullptr;

		DWORD EDB = dataOffsets::offsets.unitTypesStart - 4;
		DWORD adr = codes::offsets.createUnitFunc;

		regionStruct* region = stratMapHelpers::getRegion(regionID);
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

	unit* createUnitIdx2(const int index, const int regionId, const int facNum, const int exp, const uint8_t arm, const uint8_t weapon, const int soldiers)
	{
		if (!eopDu::getEduEntry(index))
		{
			gameHelpers::logStringGame("Can not create unit, index not found: " + std::to_string(index));
			return nullptr;
		}
		
		regionStruct* region = stratMapHelpers::getRegion(regionId);
		const auto un = GAME_FUNC(unit*(__stdcall*)(regionStruct*, int, int, int, int), createUnitFunc2)(region, index, facNum, exp, soldiers);
		if (un)
		{
			un->setArmour(arm);
			un->setWeapon(weapon);
			un->setExperience(exp);
		}
		return un;
	}

	

	unit* createUnitEDB(int edb, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (edb == 0)
			return nullptr;
		unit* res = nullptr;
		DWORD adr = codes::offsets.createUnitFunc;
		auto* entry = reinterpret_cast<eduEntry*>(edb);
		DWORD eduIndex = entry->unitCreatedCounter;
		regionStruct* region = stratMapHelpers::getRegion(regionID);
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
		return gameStringHelpers::uniStringToStr(unit->alias);
	}
	void setUnitUniStr(unit* unit, const char* str)
	{
		gameStringHelpers::createUniString(unit->alias, str);
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
		un->setExperience(exp);
		setSoldiersCount(un, count);
	}

	void setSoldiersCount(unit* un, int count)
	{
		if (count <= 0)
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
	
	modelDbEntry* findBattleModel(const char* modelName)
	{
		DWORD funcAdr = codes::offsets.findBattleModel;
		DWORD paramAdr = *reinterpret_cast<int*>(dataOffsets::offsets.modelsDb);
		paramAdr += 0x84;
		modelDbEntry* res = nullptr;
		_asm
		{
			mov ecx, paramAdr
			push modelName
			mov eax, [funcAdr]
			call eax
			mov res, eax
		}
		return res;
	}
	
	groupLabels* getGroupLabels()
	{
		return reinterpret_cast<groupLabels*>(dataOffsets::offsets.groupLabels);
	}
	
	int getMaxSoldiersCount(const unit* un)
	{
		const int sizeMul = gameHelpers::getUnitSize();
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
		return static_cast<int>(un->eduEntry->soldierCount * mul);
	}
	
	int16_t angleFloatToShort(const float angle)
	{
		return static_cast<int16_t>(angle * 182.044432904376f);
	}

	float angleShortToFloat(const int16_t angle)
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

	unitGroup* getEmptyGroup(const armyStruct* army)
	{
		if (!army || !army->unitGroups)
			return nullptr;
		for (int i = 0; i < army->maxUnitGroups; i++)
		{
			if (const auto group = &army->unitGroups[i];
				group->unitsInFormationNum + group->unitsNotInFormationNum == 0)
				return group;
		}
		return nullptr;
	}

	unitGroup* getGroupByLabel(const char* label)
	{
		DWORD funcOffset = codes::offsets.getGroupByLabel;
		
		auto groupLabels = getGroupLabels();

		auto labelHash = new stringWithHash();
		gameStringHelpers::setHashedString(const_cast<char**>(&labelHash->name), label);
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
		if (!group)
		{
			gameHelpers::logStringGame("Can not find group with label: " + std::string(label));
			return nullptr;
		}
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
		gameHelpers::logStringGame("getGroupLabel: Can not find group label");
		return "";
	}

	void removeUnitFromGroup(unitGroup* group, unit* un)
	{
		if (!group || !un || !un->unitGroup || un->unitGroup != group)
			return;
		un->releaseUnit();
		callVFunc<2, void*>(group, un, 1);
		if (group->unitsInFormationNum + group->unitsNotInFormationNum == 0)
			undefineUnitGroup(group);
	}

	void automateGroup(const unitGroup* group, bool automate)
	{
		if (!group)
			return;
		DWORD funcAddr = codes::offsets.automateGroup;
		int isAuto = automate ? 1 : 0;
		_asm
		{
			push isAuto
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
		const int16_t pushAngle = angleFloatToShort(angle);
		GAME_FUNC(bool(__thiscall*)(unitGroup*, float*, int16_t, int, int), placeGroup)(group, coords, pushAngle, 1, 4);
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
		    gameStringHelpers::setHashedString(const_cast<char**>(&labelHash->name), label);
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
		callVFunc<1, void*>(group, un);
	}

	unitGroup* defineUnitGroup(const armyStruct* army, const char* label, unit* un)
	{
		if (!army || !army->unitGroups || !un || un->unitGroup)
			return nullptr;
		auto group = getEmptyGroup(army);
		if (!group)
			return nullptr;
		
		DWORD funcOffset = codes::offsets.defineUnitGroup;
		auto labels = getGroupLabels();
		stringWithHash* labelHash = new stringWithHash();
		gameStringHelpers::setHashedString(const_cast<char**>(&labelHash->name), label);
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
#define EduEntryStruct_Type 1
#define EduEntryStruct_Soldier 2
#define EduEntryStruct_UnitCardTga 3
#define EduEntryStruct_InfoCardTga 4
#define EduEntryStruct_InfoPicDir 5
#define EduEntryStruct_CardPicDir 6
	
	template <char fieldIndex>
	std::string getStringPropertyEDU(const eduEntry* eduEn)
	{
		if (*reinterpret_cast<DWORD*>(&eduEn) <= 2)
		{
			return std::string("");
		}

		char* retS = nullptr;
		if (fieldIndex == EduEntryStruct_Type)
		{
			retS = eduEn->eduType;
		}
		else if (fieldIndex == EduEntryStruct_Soldier)
		{
			retS = eduEn->soldier;
		}
		else if (fieldIndex == EduEntryStruct_UnitCardTga)
		{
			retS = eduEn->unitCardTga;
		}
		else if (fieldIndex == EduEntryStruct_InfoCardTga)
		{
			retS = eduEn->infoCardTga;
		}
		else if (fieldIndex == EduEntryStruct_InfoPicDir)
		{
			retS = eduEn->infoPicDir;
		}
		else if (fieldIndex == EduEntryStruct_CardPicDir)
		{
			retS = eduEn->cardPicDir;
		}

		if (retS != nullptr)
		{
			return std::string(retS);
		}
		else
		{
			return std::string("");
		}
	}

	template <char fieldIndex>
	void setStringPropertyEDU(eduEntry* eduEn, std::string newS)
	{
		if (fieldIndex == EduEntryStruct_Type)
			gameStringHelpers::setHashedString(&eduEn->eduType, newS.c_str());
		else if (fieldIndex == EduEntryStruct_Soldier)
			gameStringHelpers::setHashedString(&eduEn->soldier, newS.c_str());
		else if (fieldIndex == EduEntryStruct_UnitCardTga)
			gameStringHelpers::setHashedString(&eduEn->unitCardTga, newS.c_str());
		else if (fieldIndex == EduEntryStruct_InfoCardTga)
			gameStringHelpers::setHashedString(&eduEn->infoCardTga, newS.c_str());
		else if (fieldIndex == EduEntryStruct_InfoPicDir)
			gameStringHelpers::setHashedString(&eduEn->infoPicDir, newS.c_str());
		else if (fieldIndex == EduEntryStruct_CardPicDir)
			gameStringHelpers::setHashedString(&eduEn->cardPicDir, newS.c_str());
	}

	std::string getLocalizedUnitName(const eduEntry* entry)
	{
		return gameStringHelpers::uniStringToStr(*entry->localizedName);
	}

	std::string getLocalizedUnitDescr(const eduEntry* entry)
	{
		return gameStringHelpers::uniStringToStr(*entry->localizedDescr);
	}

	std::string getLocalizedUnitDescrShort(const eduEntry* entry)
	{
		return gameStringHelpers::uniStringToStr(*entry->localizedDescrShort);
	}

	void setUnitName(eduEntry* entry, const std::string& name)
	{
		const auto nameMem = techFuncs::createGameClass<UNICODE_STRING**>();
		entry->localizedName = nameMem;
		gameStringHelpers::createUniString(*entry->localizedName, name.c_str());
	}

	void setUnitDescr(eduEntry* entry, const std::string& descr)
	{
		const auto nameMem = techFuncs::createGameClass<UNICODE_STRING**>();
		entry->localizedDescr = nameMem;
		gameStringHelpers::createUniString(*entry->localizedDescr, descr.c_str());
	}

	void setUnitDescrShort(eduEntry* entry, const std::string& descr)
	{
		const auto nameMem = techFuncs::createGameClass<UNICODE_STRING**>();
		entry->localizedDescrShort = nameMem;
		gameStringHelpers::createUniString(*entry->localizedDescrShort, descr.c_str());
	}
	
	int getEduIndex(const char* type)
	{
		if (!type)
			return -1;
		
		if (const auto data = eopDu::getEopEduEntryByName(type))
			return data->index;
		
		unitDb* EDB = reinterpret_cast<unitDb*>(dataOffsets::offsets.unitTypesStart - 4);

		int unitsNum = EDB->numberOfEntries;
		for (int i = 0; i < unitsNum; i++)
		{
			if (strcmp(EDB->unitEntries[i].eduType, type) == 0)
			{
				return i;
			}
		}

		gameHelpers::logStringGame("Edu index not found for type: " + std::string(type));
		return -1;
	}

	std::unordered_map<std::string, int> BASE_UNITS_LOOKUP = {};

	void initBaseUnitsLookup()
	{
		const auto edu = getEdu();
		const int unitsNum = edu->numberOfEntries;
		for (int i = 0; i < unitsNum; i++)
		{
			if (edu->unitEntries[i].eduType)
				BASE_UNITS_LOOKUP[edu->unitEntries[i].eduType] = edu->unitEntries[i].index;
			else
				gameHelpers::logStringGame("initBaseUnitsLookup: No name for unit at index: " + std::to_string(i));
		}
	}
	
	eduEntry* getEduEntryByName(const char* type)
	{
		if (BASE_UNITS_LOOKUP.find(type) != BASE_UNITS_LOOKUP.end())
			return getEDUEntryById(BASE_UNITS_LOOKUP[type]);
		if (const auto data = eopDu::getEopEduEntryByName(type))
			return data;
		const auto edu = getEdu();
		const int unitsNum = edu->numberOfEntries;
		for (int i = 0; i < unitsNum; i++)
		{
			if (edu->unitEntries[i].eduType)
			{
				if (strcmp(edu->unitEntries[i].eduType, type) == 0)
				{
					return &edu->unitEntries[i];
				}
			}
			else
			{
				gameHelpers::logStringGame("getEduEntryByName: No name for unit at index: " + std::to_string(i));
			}
		}
		gameHelpers::logStringGame("getEduEntryByName: No unit found with name: " + std::string(type));
		return nullptr;
	}

	unitDb* getEdu()
	{
		return reinterpret_cast<unitDb*>(dataOffsets::offsets.unitTypesStart - 4);
	}
	
	eduEntry* getEDUEntryById(int id)
	{
		const auto edu = getEdu();
		const int unitsNum = edu->numberOfEntries;
		if (id < 0 || id >= unitsNum)
			return nullptr;
		if (edu->unitEntries[id].index == id)
			return &edu->unitEntries[id];
		for (int i = 0; i < unitsNum; i++)
		{
			if (edu->unitEntries[i].index == id)
			{
				return &edu->unitEntries[i];
			}
		}
		return nullptr;
	}
	
}

//void addToLua(sol::state& luaState)
void luaPlugin::initUnits()
{
	eopDuHelpers::addToLua(luaState);
	struct
	{
		sol::usertype<unit>unit;
		sol::usertype<unitPositionData>unitPositionData;
		sol::usertype<unitGroup> unitGroup;
		sol::usertype<siegeEngine> siegeEngineStruct;
		sol::usertype<eduEntry>eduEntry;
		sol::usertype<descrMountEntry> mountStruct;
		sol::usertype<projectile> projectileStruct;
		sol::usertype<statPri> attackStats;
		sol::usertype<statArmour> defenseStats;
		sol::usertype<unitStats> unitStats;
		sol::usertype<unitAiGroupData> unitAiGroupData;
		sol::usertype<vector3> vector3;
	}types;
	
	///Unit
	//@section Unit

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
	@tfield unitAiGroupData aiGroupData
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
	@tfield armyStruct army
	@tfield int siegeEngineNum (only infantry and artillery units!)
	@tfield kill kill
	@tfield setParams setParams change soldierCountStratMap, exp, armourLVL, weaponLVL at one time.
	@tfield hasAttribute hasAttribute Check if unit has edu attribute.
	@tfield string alias
	@tfield int crusadeState >0 means crusading, <0 means disbaning, 0 means no crusade
	@tfield crusadeStruct crusade
	@tfield hasBattleProperty hasBattleProperty
	@tfield setBattleProperty setBattleProperty
	@tfield getActionStatus getActionStatus
	@tfield getActionStatusInt getActionStatusInt
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
	@tfield isFiring isFiring
	@tfield isIdle isIdle
	@tfield unitStats unitStats

	@table unit
	*/
	types.unit = luaState.new_usertype<unit>("unit");
	types.unit.set("eduEntry", &unit::eduEntry);
	types.unit.set("aiActiveSet", &unit::aiActiveSet);
	types.unit.set("movePoints", sol::property(&unit::getMovepoints, &unitHelpers::setUnitMovePoints));
	types.unit.set("soldierCountStratMap", sol::property(&unit::getSoldierCountStratMap, &unitHelpers::setSoldiersCount));
	types.unit.set("exp", sol::property(&unit::getExp, &unit::setExp));
	types.unit.set("armourLVL", sol::property(&unit::getArmourLevel, &unit::setarmourLVL));
	types.unit.set("weaponLVL", sol::property(&unit::getWeaponLevel, &unit::setweaponLVL));
	types.unit.set("soldierCountStratMapMax", sol::property(&unitHelpers::getMaxSoldiersCount));
	types.unit.set("soldierCountBattleMap", &unit::SoldierCountBattlemap);
	types.unit.set("unitPositionData", &unit::unitPositionData);
	types.unit.set("character", &unit::general);
	types.unit.set("flankRightState", &unit::flankRightState);
	types.unit.set("flankLeftState", &unit::flankLeftState);
	types.unit.set("flankRearState", &unit::flankRearState);
	types.unit.set("flankRightThreat", &unit::flankRightThreat);
	types.unit.set("flankLeftThreat", &unit::flankLeftThreat);
	types.unit.set("flankRearThreat", &unit::flankRearThreat);
	types.unit.set("crusadeState", &unit::crusadeState);
	types.unit.set("crusade", &unit::crusade);
	types.unit.set("soldiersFled", &unit::soldiersFled);
	types.unit.set("aiGroupData", &unit::unitAiGroupData);
	types.unit.set("isInfighting", &unit::isInfighting);
	types.unit.set("unitStats", &unit::battleStats);
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
	types.unit.set_function("kill", &unitHelpers::killUnit);
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
	types.unit.set("alias", sol::property(&unitHelpers::unitUniStringToStr, &unitHelpers::setUnitUniStr));
	/***
	Check if unit has edu attribute.
	@function unit:hasAttribute
	@tparam string attributeName
	@usage
	local hasAttr = unit:hasAttribute("sea_faring");
	*/
	types.unit.set_function("hasAttribute", &unit::hasAttribute);
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
	Get unit action status in battle (use enum, faster than string).
	@function unit:getActionStatusInt
	@treturn int actionStatus
	@usage
	local status = unit:getActionStatusInt();
	*/
	types.unit.set_function("getActionStatusInt", &unit::getActionStatusInt);
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
	@tparam bool heroOnly
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
	/***
	Check if action status is firing or reloading.
	@function unit:isFiring
	@treturn bool isFiring
	@usage
		local isFiring = unit:isFiring()
	*/
	types.unit.set_function("isFiring", &unit::isFiring);
	/***
	Check if action status is idle or hiding or taunting or celebrating or ready.
	@function unit:isIdle
	@treturn bool isIdle
	@usage
		local isIdle = unit:isIdle()
	*/
	types.unit.set_function("isIdle", &unit::isIdle);

	/***

	@tfield unitGroup group
	@tfield battleObjective battleObjective
	@tfield bool inMelee
	@tfield bool isAvailable

	@table unitAiGroupData
    */
	 		
	types.unitAiGroupData = luaState.new_usertype<unitAiGroupData>("unitAiGroupData");
	types.unitAiGroupData.set("group", &unitAiGroupData::unitGroup);
	types.unitAiGroupData.set("battleObjective", &unitAiGroupData::battleObjective);
	types.unitAiGroupData.set("inMelee", &unitAiGroupData::inMelee);
	types.unitAiGroupData.set("isAvailable", &unitAiGroupData::isAvailable);
	
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
	@tfield getActionType getActionType

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

	/***
	Get unit's current action type (use enum).
	@function unitPositionData:getActionType
	@treturn int actionType
	@usage
	local action = unit.unitPositionData:getActionType();
	*/
	types.unitPositionData.set_function("getActionType", &unitPositionData::getActionType);
    	

	///Siege Engine
	//@section Siege Engine

	/***
	Basic siegeEngineStruct table

	@tfield float xCoord
	@tfield float zCoord
	@tfield float yCoord
	@tfield float mass
	@tfield int angle
	@tfield int health
	@tfield int engineID Unique ID not class!
	@tfield unit currentUnit
	@tfield unit lastUnit
	@tfield getType getType
	@tfield int fireLevel
	@tfield int isBurning
	@tfield unitStats stats

	@table siegeEngineStruct
	*/
	types.siegeEngineStruct = luaState.new_usertype<siegeEngine>("siegeEngineStruct");
	types.siegeEngineStruct.set("xCoord", &siegeEngine::posX);
	types.siegeEngineStruct.set("zCoord", &siegeEngine::posZ);
	types.siegeEngineStruct.set("yCoord", &siegeEngine::posY);
	types.siegeEngineStruct.set("mass", &siegeEngine::mass);
	types.siegeEngineStruct.set("health", &siegeEngine::health);
	types.siegeEngineStruct.set("angle", sol::property(&unitHelpers::getEngineAngle, &unitHelpers::setEngineAngle));
	types.siegeEngineStruct.set("currentUnit", &siegeEngine::currentUnit);
	types.siegeEngineStruct.set("lastUnit", &siegeEngine::lastUnit);
	types.siegeEngineStruct.set("engineID", &siegeEngine::ID);
	types.siegeEngineStruct.set("fireLevel", &siegeEngine::fireLevel);
	types.siegeEngineStruct.set("isBurning", &siegeEngine::isBurning);
	types.siegeEngineStruct.set("stats", &siegeEngine::stats);

	/***
	Get the type of the engine (use the enum).
	@function siegeEngineStruct:getType
	@treturn int type
	@usage
		local type = siegeEngineStruct:getType()
	*/
	types.siegeEngineStruct.set_function("getType", &siegeEngine::getEngineType);
    	
	///Unit Group
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
	types.unitGroup.set_function("addUnit", &unitHelpers::addUnitToGroup);
		
	/***
	Remove a unit from a group.
	@function unitGroup:removeUnit
	@tparam unit unit
	@usage
			unitGroup:removeUnit(unit);
	*/
	types.unitGroup.set_function("removeUnit", &unitHelpers::removeUnitFromGroup);
		
	/***
	Undefine a unit group.
	@function unitGroup:undefine
	@usage
			unitGroup:undefine();
	*/
	types.unitGroup.set_function("undefine", &unitHelpers::undefineUnitGroup);
		
	/***
	Toggle group automation.
	@function unitGroup:automate
	@tparam bool automate
	@usage
			unitGroup:automate(true);
	*/
	types.unitGroup.set_function("automate", &unitHelpers::automateGroup);
		
	/***
	Automate group attack.
	@function unitGroup:automateAttack
	@tparam unit targetUnit
	@usage
			unitGroup:automateAttack(targetUnit);
	*/
	types.unitGroup.set_function("automateAttack", &unitHelpers::automateAttack);
		
	/***
	Automate group defense.
	@function unitGroup:automateDefense
	@tparam float xCoord
	@tparam float yCoord
	@tparam float radius
	@usage
			unitGroup:automateDefense(180, 283, 50);
	*/
	types.unitGroup.set_function("automateDefense", &unitHelpers::automateDefense);
		
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
	types.unitGroup.set_function("place", &unitHelpers::placeGroup);
		
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
		
	///Projectile
	//@section Projectile

	/***

	@tfield string name
	@tfield float accuracy
	@tfield float accuracyVsBuildings
	@tfield float accuracyVsTowers
	@tfield int damage
	@tfield int damageToTroops
	@tfield int isBodyPiercing
	@tfield int fiery
	@tfield int affectedByRain

	@table projectileStruct
	*/
	types.projectileStruct = luaState.new_usertype<projectile>("projectileStruct");
	types.projectileStruct.set("name", &projectile::name);
	types.projectileStruct.set("accuracy", &projectile::accuracyVsUnits);
	types.projectileStruct.set("accuracyVsBuildings", &projectile::accuracyVsBuildings);
	types.projectileStruct.set("accuracyVsTowers", &projectile::accuracyVsTowers);
	types.projectileStruct.set("damage", &projectile::damage);
	types.projectileStruct.set("damageToTroops", &projectile::damageToTroops);
	types.projectileStruct.set("affectedByRain", &projectile::affectedByRain);
	types.projectileStruct.set("isBodyPiercing", &projectile::accuracyVsUnits);
	types.projectileStruct.set("fiery", &projectile::fiery);
		
	///Mount
	//@section mountStruct

	/***

	@tfield string name
	@tfield string modelName
	@tfield int mountClass
	@tfield float radius
	@tfield float xRadius
	@tfield float yRadius
	@tfield float yOffset
	@tfield float height
	@tfield float mass
	@tfield float elephantDeadRadius
	@tfield float elephantTuskRadius
	@tfield float elephantTuskZ
	@tfield float elephantRootNodeHeight
	@tfield int elephantNumberOfRiders
	@tfield float riderOffSetX
	@tfield float riderOffSetY
	@tfield float riderOffsetZ
	@tfield float rootNodeHeight
	@tfield getElephantRiderOffset getElephantRiderOffset

	@table mountStruct
	*/
	types.mountStruct = luaState.new_usertype<descrMountEntry>("mountStruct");
	types.mountStruct.set("name", &descrMountEntry::name);
	types.mountStruct.set("modelName", &descrMountEntry::modelName);
	types.mountStruct.set("mountClass", &descrMountEntry::mountClass);
	types.mountStruct.set("radius", &descrMountEntry::radius);
	types.mountStruct.set("xRadius", &descrMountEntry::xRadius);
	types.mountStruct.set("yRadius", &descrMountEntry::yRadius);
	types.mountStruct.set("yOffset", &descrMountEntry::yOffset);
	types.mountStruct.set("height", &descrMountEntry::height);
	types.mountStruct.set("mass", &descrMountEntry::radius);
	types.mountStruct.set("elephantDeadRadius", &descrMountEntry::elephantDeadRadius);
	types.mountStruct.set("elephantTuskRadius", &descrMountEntry::elephantTuskRadius);
	types.mountStruct.set("elephantTuskZ", &descrMountEntry::elephantTuskZ);
	types.mountStruct.set("elephantRootNodeHeight", &descrMountEntry::elephantRootNodeHeight);
	types.mountStruct.set("elephantNumberOfRiders", &descrMountEntry::elephantNumberOfRiders);
	types.mountStruct.set("riderOffSetX", &descrMountEntry::riderOffSetX);
	types.mountStruct.set("riderOffSetY", &descrMountEntry::riderOffSetY);
	types.mountStruct.set("riderOffsetZ", &descrMountEntry::riderOffsetZ);
	types.mountStruct.set("rootNodeHeight", &descrMountEntry::rootNodeHeight);
	
	/***
	Get elephant rider offset.
	@function mountStruct:getElephantRiderOffset
	@tparam int index
	@treturn vector3 offsets
	@usage
	local offsets = mount:getElephantRiderOffset(0);
	*/
	types.mountStruct.set_function("getElephantRiderOffset", &descrMountEntry::getElephantRiderOffset);
	
	/***

	@tfield float xCoord
	@tfield float yCoord
	@tfield float zCoord

	@table vector3
	*/
	types.vector3 = luaState.new_usertype<vector3>("vector3");
	types.vector3.set("xCoord", &vector3::x);
	types.vector3.set("yCoord", &vector3::y);
	types.vector3.set("zCoord", &vector3::z);
	
	///Edu Entry
	//@section Edu Entry

	/***
	Basic eduEntry table

	@tfield string eduType - Get only
	@tfield string soldier - Get only
	@tfield string unitCardTga - Get only
	@tfield string infoCardTga - Get only
	@tfield string infoPicDir
	@tfield string cardPicDir
	@tfield string localizedName
	@tfield string localizedDescription
	@tfield string localizedDescriptionShort
	@tfield int index - Get only
	@tfield int unitCreatedCounter - Get only
	@tfield int category - Get only
	@tfield int class - Get only
	@tfield int categoryClassCombo - Get only
	@tfield int recruitPriorityOffset times 4
	@tfield float crusadingUpkeepModifier
	@tfield float aiUnitValuePerSoldier
	@tfield float aiUnitValue
	@tfield int soldierCount
	@tfield float mass
	@tfield float width
	@tfield float height
	@tfield bool legio
	@tfield bool hideForest
	@tfield bool hideImprovedForest
	@tfield bool hideLongGrass
	@tfield bool hideAnywhere
	@tfield bool powerCharge
	@tfield bool seaFaring
	@tfield bool command
	@tfield bool heavyUnit
	@tfield bool hardy
	@tfield bool veryHardy
	@tfield bool slave
	@tfield bool frightenFoot
	@tfield bool frightenMounted
	@tfield bool freeUpkeepUnit
	@tfield bool gunpowderUnit
	@tfield bool gunpowderArtillery
	@tfield bool fireByRank
	@tfield bool startNotSkirmishing
	@tfield bool mercenaryUnit
	@tfield bool noCustom
	@tfield bool isPeasant
	@tfield bool generalUnit
	@tfield bool generalUnitUpgrade
	@tfield bool cantabrianCircle
	@tfield bool druid
	@tfield bool screechingWomen
	@tfield bool warCry
	@tfield bool stakes
	@tfield bool canSkirmish
	@tfield float moveSpeedMod
	@tfield float unitSpacingFrontToBackClose
	@tfield float unitSpacingSideToSideClose
	@tfield float unitSpacingFrontToBackLoose
	@tfield float unitSpacingSideToSideLoose
	@tfield int statHealth
	@tfield int statHealthAnimal
	@tfield int statHeat
	@tfield int statScrub
	@tfield int statSand
	@tfield int statForest
	@tfield int statSnow
	@tfield int recruitTime
	@tfield int recruitCost
	@tfield int upkeepCost
	@tfield int weaponCost
	@tfield int armourCost
	@tfield int customBattleCost
	@tfield int customBattleIncrease
	@tfield int customBattleLimit
	@tfield int training
	@tfield int discipline
	@tfield int canPhalanx
	@tfield int morale
	@tfield int moraleLocked
	@tfield int statFood1
	@tfield int statFood2
	@tfield attackStats primaryStats
	@tfield attackStats secondaryStats
	@tfield attackStats tertiaryStats
	@tfield attackStats engineStats
	@tfield attackStats mountStats
	@tfield attackStats animalStats
	@tfield defenseStats primaryDefenseStats
	@tfield defenseStats secondaryDefenseStats
	@tfield defenseStats mountDefenseStats
	@tfield defenseStats animalDefenseStats
	@tfield mountStruct mount
	@tfield string primaryAnim
	@tfield string secondaryAnim
	@tfield hasOwnership hasOwnership
	@tfield hasAttribute hasAttribute
	@tfield setOwnership setOwnership
	@tfield getArmourLevelNum getArmourLevelNum
	@tfield getArmourLevel getArmourLevel
	@tfield setArmourLevel setArmourLevel
	@tfield getArmourModelNum getArmourModelNum
	@tfield getArmourModel getArmourModel
	@tfield setArmourModel setArmourModel

	@table eduEntry
	*/
	types.eduEntry = luaState.new_usertype<eduEntry>("eduEntry");
	types.eduEntry.set("eduType", sol::property(
		                   &unitHelpers::getStringPropertyEDU<EduEntryStruct_Type>, &unitHelpers::setStringPropertyEDU<EduEntryStruct_Type>
	                   ));
	types.eduEntry.set("soldier", sol::property(
		                   &unitHelpers::getStringPropertyEDU<EduEntryStruct_Soldier>, &unitHelpers::setStringPropertyEDU<EduEntryStruct_Soldier>
	                   ));
	types.eduEntry.set("unitCardTga", sol::property(
		                   &unitHelpers::getStringPropertyEDU<EduEntryStruct_UnitCardTga>, &unitHelpers::setStringPropertyEDU<EduEntryStruct_UnitCardTga>
	                   ));
	types.eduEntry.set("infoCardTga", sol::property(
		                   &unitHelpers::getStringPropertyEDU<EduEntryStruct_InfoCardTga>, &unitHelpers::setStringPropertyEDU<EduEntryStruct_InfoCardTga>
	                   ));
	types.eduEntry.set("infoPicDir", sol::property(
		                   &unitHelpers::getStringPropertyEDU<EduEntryStruct_InfoPicDir>, &unitHelpers::setStringPropertyEDU<EduEntryStruct_InfoPicDir>
	                   ));
	types.eduEntry.set("cardPicDir", sol::property(
		                   &unitHelpers::getStringPropertyEDU<EduEntryStruct_CardPicDir>, &unitHelpers::setStringPropertyEDU<EduEntryStruct_CardPicDir>
	                   ));
	types.eduEntry.set("localizedName", sol::property(&unitHelpers::getLocalizedUnitName, &unitHelpers::setUnitName));
	types.eduEntry.set("localizedDescription", sol::property(&unitHelpers::getLocalizedUnitDescr, &unitHelpers::setUnitDescr));
	types.eduEntry.set("localizedDescriptionShort", sol::property(&unitHelpers::getLocalizedUnitDescrShort, &unitHelpers::setUnitDescrShort));
	types.eduEntry.set("index", &eduEntry::index);
	types.eduEntry.set("unitCreatedCounter", &eduEntry::unitCreatedCounter);
	types.eduEntry.set("soldierCount", &eduEntry::soldierCount);
	types.eduEntry.set("mass", &eduEntry::mass);
	types.eduEntry.set("width", &eduEntry::width);
	types.eduEntry.set("primaryStats", &eduEntry::priStats);
	types.eduEntry.set("secondaryStats", &eduEntry::secStats);
	types.eduEntry.set("tertiaryStats", &eduEntry::terStats);
	types.eduEntry.set("engineStats", &eduEntry::engineStats);
	types.eduEntry.set("mountStats", &eduEntry::mountStats);
	types.eduEntry.set("animalStats", &eduEntry::animalStats);
	types.eduEntry.set("primaryDefenseStats", &eduEntry::statPriArmour);
	types.eduEntry.set("secondaryDefenseStats", &eduEntry::statSecArmour);
	types.eduEntry.set("mountDefenseStats", &eduEntry::statArmourMount);
	types.eduEntry.set("animalDefenseStats", &eduEntry::statArmourAnimal);
	types.eduEntry.set("height", &eduEntry::height);
	types.eduEntry.set("training", &eduEntry::trained);
	types.eduEntry.set("discipline", &eduEntry::discipline);
	types.eduEntry.set("mount", &eduEntry::mount);
	types.eduEntry.set("canPhalanx", &eduEntry::formationPhalanx);
	types.eduEntry.set("moveSpeedMod", &eduEntry::moveSpeedMod);
	types.eduEntry.set("unitSpacingFrontToBackClose", &eduEntry::unitSpacingFrontToBackClose);
	types.eduEntry.set("unitSpacingSideToSideClose", &eduEntry::unitSpacingSideToSideClose);
	types.eduEntry.set("unitSpacingFrontToBackLoose", &eduEntry::unitSpacingFrontToBackLoose);
	types.eduEntry.set("unitSpacingSideToSideLoose", &eduEntry::unitSpacingSideToSideLoose);
	types.eduEntry.set("statHealth", &eduEntry::statHealth);
	types.eduEntry.set("statHealthAnimal", &eduEntry::statHealthAnimal);
	types.eduEntry.set("statHeat", &eduEntry::statHeat);
	types.eduEntry.set("statScrub", &eduEntry::statScrub);
	types.eduEntry.set("statSand", &eduEntry::statSand);
	types.eduEntry.set("statForest", &eduEntry::statForest);
	types.eduEntry.set("statSnow", &eduEntry::statSnow);
	types.eduEntry.set("recruitTime", &eduEntry::recruitTime);
	types.eduEntry.set("recruitCost", &eduEntry::recruitCost);
	types.eduEntry.set("upkeepCost", &eduEntry::upkeepCost);
	types.eduEntry.set("weaponCost", &eduEntry::weaponCost);
	types.eduEntry.set("armourCost", &eduEntry::armourCost);
	types.eduEntry.set("customBattleCost", &eduEntry::customBattleCost);
	types.eduEntry.set("customBattleIncrease", &eduEntry::customBattleIncrease);
	types.eduEntry.set("customBattleLimit", &eduEntry::customBattleLimit);
	types.eduEntry.set("morale", &eduEntry::morale);
	types.eduEntry.set("moraleLocked", &eduEntry::moraleLocked);
	types.eduEntry.set("statFood1", &eduEntry::statFood1);
	types.eduEntry.set("statFood2", &eduEntry::statFood2);
	types.eduEntry.set("category", &eduEntry::category);
	types.eduEntry.set("primaryAnim", sol::property(&eduEntry::getPrimaryAnim));
	types.eduEntry.set("secondaryAnim", sol::property(&eduEntry::getSecondaryAnim));
	types.eduEntry.set("class", &eduEntry::unitClass);
	types.eduEntry.set("categoryClassCombo", &eduEntry::unitProductionClass);
	types.eduEntry.set("recruitPriorityOffset", &eduEntry::recruitPriorityOffset);
	types.eduEntry.set("crusadingUpkeepModifier", &eduEntry::crusadingUpkeepModifier);
	types.eduEntry.set("aiUnitValuePerSoldier", &eduEntry::aiUnitValuePerSoldier);
	types.eduEntry.set("aiUnitValue", &eduEntry::aiUnitValue);
	types.eduEntry.set("haveAttributeLegio", sol::property(&eduEntry::getIsLegio, &eduEntry::setIsLegio));
	types.eduEntry.set("legio", sol::property(&eduEntry::getIsLegio, &eduEntry::setIsLegio));
	types.eduEntry.set("hideForest", sol::property(&eduEntry::getHideForest, &eduEntry::setHideForest));
	types.eduEntry.set("hideImprovedForest", sol::property(&eduEntry::getHideImprovedForest, &eduEntry::setHideImprovedForest));
	types.eduEntry.set("hideLongGrass", sol::property(&eduEntry::getHideLongGrass, &eduEntry::setHideLongGrass));
	types.eduEntry.set("hideAnywhere", sol::property(&eduEntry::getHideAnywhere, &eduEntry::setHideAnywhere));
	types.eduEntry.set("powerCharge", sol::property(&eduEntry::getPowerCharge, &eduEntry::setPowerCharge));
	types.eduEntry.set("seaFaring", sol::property(&eduEntry::getSeaFaring, &eduEntry::setSeaFaring));
	types.eduEntry.set("command", sol::property(&eduEntry::getCommand, &eduEntry::setCommand));
	types.eduEntry.set("heavyUnit", sol::property(&eduEntry::getHeavyUnit, &eduEntry::setHeavyUnit));
	types.eduEntry.set("hardy", sol::property(&eduEntry::getHardy, &eduEntry::setHardy));
	types.eduEntry.set("veryHardy", sol::property(&eduEntry::getVeryHardy, &eduEntry::setVeryHardy));
	types.eduEntry.set("slave", sol::property(&eduEntry::getSlave, &eduEntry::setSlave));
	types.eduEntry.set("frightenFoot", sol::property(&eduEntry::getFrightenFoot, &eduEntry::setFrightenFoot));
	types.eduEntry.set("frightenMounted", sol::property(&eduEntry::getFrightenMounted, &eduEntry::setFrightenMounted));
	types.eduEntry.set("freeUpkeepUnit", sol::property(&eduEntry::getFreeUpkeepUnit, &eduEntry::setFreeUpkeepUnit));
	types.eduEntry.set("gunpowderUnit", sol::property(&eduEntry::getGunpowderUnit, &eduEntry::setGunpowderUnit));
	types.eduEntry.set("gunpowderArtillery", sol::property(&eduEntry::getGunpowderArtillery, &eduEntry::setGunpowderArtillery));
	types.eduEntry.set("fireByRank", sol::property(&eduEntry::getFireByRank, &eduEntry::setFireByRank));
	types.eduEntry.set("startNotSkirmishing", sol::property(&eduEntry::getStartNotSkirmishing, &eduEntry::setStartNotSkirmishing));
	types.eduEntry.set("mercenaryUnit", sol::property(&eduEntry::getMercenaryUnit, &eduEntry::setMercenaryUnit));
	types.eduEntry.set("noCustom", sol::property(&eduEntry::getNoCustom, &eduEntry::setNoCustom));
	types.eduEntry.set("isPeasant", sol::property(&eduEntry::getIsPeasant, &eduEntry::setIsPeasant));
	types.eduEntry.set("generalUnit", sol::property(&eduEntry::getGeneralUnit, &eduEntry::setGeneralUnit));
	types.eduEntry.set("generalUnitUpgrade", sol::property(&eduEntry::getGeneralUnitUpgrade, &eduEntry::setGeneralUnitUpgrade));
	types.eduEntry.set("cantabrianCircle", sol::property(&eduEntry::getCantabrianCircle, &eduEntry::setCantabrianCircle));
	types.eduEntry.set("druid", sol::property(&eduEntry::getDruid, &eduEntry::setDruid));
	types.eduEntry.set("screechingWomen", sol::property(&eduEntry::getScreechingWomen, &eduEntry::setScreechingWomen));
	types.eduEntry.set("warCry", sol::property(&eduEntry::getWarCry, &eduEntry::setWarCry));
	types.eduEntry.set("stakes", sol::property(&eduEntry::getStakes, &eduEntry::setStakes));
	types.eduEntry.set("canSkirmish", sol::property(&eduEntry::getCanSkirmish, &eduEntry::setCanSkirmish));
		
		
	/***
	Check if a faction has ownership of this entry.
	@function eduEntry:hasOwnership
	@tparam int factionID
	@treturn bool hasOwnership
	@usage
	local hasOwnership = unit.eduEntry:hasOwnership(2);
	*/
	types.eduEntry.set_function("hasOwnership", &eduEntry::hasOwnership);

	/***
	Set if a faction has ownership of this entry.
	@function eduEntry:setOwnership
	@tparam int factionID
	@tparam bool setOwnership
	@usage
		unit.eduEntry:setOwnership(2, true);
	*/
	types.eduEntry.set_function("setOwnership", &eduEntry::setOwnership);
		
	/***
	Check if the entry has an attribute.
	@function eduEntry:hasAttribute
	@tparam string attributeName
	@treturn bool hasAttribute
	@usage
	local hasAttribute = entry:hasAttribute("sea_faring");
	*/
	types.eduEntry.set_function("hasAttribute", &eduEntry::hasAttribute);
		
	/***
	Get number of armour levels.
	@function eduEntry:getArmourLevelNum
	@treturn int levelNum
	@usage
	local num = entry:getArmourLevelNum();
	*/
	types.eduEntry.set_function("getArmourLevelNum", &eduEntry::getArmourLevelNum);
		
	/***
	Get number of armour models.
	@function eduEntry:getArmourModelNum
	@treturn int levelNum
	@usage
	local num = entry:getArmourModelNum();
	*/
	types.eduEntry.set_function("getArmourModelNum", &eduEntry::getArmourModelNum);
		
	/***
	Get armour level at index.
	@function eduEntry:getArmourLevel
	@tparam int index
	@treturn int level
	@usage
	local level = entry:getArmourLevel(0);
	*/
	types.eduEntry.set_function("getArmourLevel", &eduEntry::getArmourLevel);
		
	/***
	Get armour model at index.
	@function eduEntry:getArmourModel
	@tparam int index
	@treturn string model
	@usage
	local model = entry:getArmourModel(0);
	*/
	types.eduEntry.set_function("getArmourModel", &eduEntry::getArmourModel);
		
	/***
	Set armour level at index.
	@function eduEntry:setArmourLevel
	@tparam int index
	@tparam int level
	@usage
		entry:setArmourLevel(0, 2);
	*/
	types.eduEntry.set_function("setArmourLevel", &eduEntry::setArmourLevel);
		
	/***
	Set armour model at index.
	@function eduEntry:setArmourModel
	@tparam int index
	@tparam string model
	@usage
		entry:setArmourModel(0, "peasants");
	*/
	types.eduEntry.set_function("setArmourModel", &eduEntry::setArmourModel);
		
	/***
	@tfield bool isValid
	@tfield bool isMissile
	@tfield bool isSpear
	@tfield bool isLightSpear
	@tfield bool isPrec
	@tfield bool isAP
	@tfield bool isBP
	@tfield bool isArea
	@tfield bool isFire
	@tfield bool isBodyLaunching
	@tfield bool isThrown
	@tfield bool isShortPike
	@tfield bool isLongPike
	@tfield bool isSpearBonus12
	@tfield bool isSpearBonus10
	@tfield bool isSpearBonus8
	@tfield bool isSpearBonus6
	@tfield bool isSpearBonus4
	@tfield int attack
	@tfield int charge
	@tfield int ammo
	@tfield int range
	@tfield float rangeSquared
	@tfield projectileStruct projectile
	@tfield int weaponType
	@tfield int techType
	@tfield int damageType
	@tfield int soundType
	@tfield int minAttackDelayPri
	@tfield string weaponFX

	@table attackStats
	*/
	types.attackStats = luaState.new_usertype<statPri>("attackStats");
	types.attackStats.set("isValid", sol::property(&statPri::getIsValid, &statPri::setIsValid));
	types.attackStats.set("isMissile", sol::property(&statPri::getIsMissile, &statPri::setIsMissile));
	types.attackStats.set("isSpear", sol::property(&statPri::getIsSpear, &statPri::setIsSpear));
	types.attackStats.set("isLightSpear", sol::property(&statPri::getIsLightSpear, &statPri::setIsLightSpear));
	types.attackStats.set("isPrec", sol::property(&statPri::getIsPrec, &statPri::setIsPrec));
	types.attackStats.set("isAP", sol::property(&statPri::getIsAP, &statPri::setIsAP));
	types.attackStats.set("isBP", sol::property(&statPri::getIsBP, &statPri::setIsBP));
	types.attackStats.set("isArea", sol::property(&statPri::getIsArea, &statPri::setIsArea));
	types.attackStats.set("isFire", sol::property(&statPri::getIsFire, &statPri::setIsFire));
	types.attackStats.set("isBodyLaunching", sol::property(&statPri::getIsBodyLaunching, &statPri::setIsBodyLaunching));
	types.attackStats.set("isThrown", sol::property(&statPri::getIsThrown, &statPri::setIsThrown));
	types.attackStats.set("isShortPike", sol::property(&statPri::getIsShortPike, &statPri::setIsShortPike));
	types.attackStats.set("isLongPike", sol::property(&statPri::getIsLongPike, &statPri::setIsLongPike));
	types.attackStats.set("isSpearBonus12", sol::property(&statPri::getIsSpearBonus12, &statPri::setIsSpearBonus12));
	types.attackStats.set("isSpearBonus10", sol::property(&statPri::getIsSpearBonus10, &statPri::setIsSpearBonus10));
	types.attackStats.set("isSpearBonus8", sol::property(&statPri::getIsSpearBonus8, &statPri::setIsSpearBonus8));
	types.attackStats.set("isSpearBonus6", sol::property(&statPri::getIsSpearBonus6, &statPri::setIsSpearBonus6));
	types.attackStats.set("isSpearBonus4", sol::property(&statPri::getIsSpearBonus4, &statPri::setIsSpearBonus4));
	types.attackStats.set("attack", sol::property(&statPri::getAttack, &statPri::setAttack));
	types.attackStats.set("charge", sol::property(&statPri::getCharge, &statPri::setCharge));
	types.attackStats.set("ammo", sol::property(&statPri::getAmmo, &statPri::setAmmo));
	types.attackStats.set("range", &statPri::missileRange);
	types.attackStats.set("rangeSquared", &statPri::missileRangeSquared);
	types.attackStats.set("projectile", &statPri::missile);
	types.attackStats.set("weaponType", &statPri::weaponType);
	types.attackStats.set("techType", &statPri::weaponTecType);
	types.attackStats.set("damageType", &statPri::damageType);
	types.attackStats.set("soundType", &statPri::soundType);
	types.attackStats.set("minAttackDelayPri", &statPri::minAttackDelayPri);
	types.attackStats.set("weaponFX", &statPri::weaponFX);
		
	/***
	@tfield bool isValid
	@tfield int armour
	@tfield int defense
	@tfield int shield
	@tfield int armourMaterial
	
	@table defenseStats
	*/
	types.defenseStats = luaState.new_usertype<statArmour>("defenseStats");
	types.defenseStats.set("isValid", sol::property(&statArmour::getIsValid, &statArmour::setIsValid));
	types.defenseStats.set("armour", sol::property(&statArmour::getArmour, &statArmour::setArmour));
	types.defenseStats.set("defense", sol::property(&statArmour::getDefense, &statArmour::setDefense));
	types.defenseStats.set("shield", sol::property(&statArmour::getShield, &statArmour::setShield));
	types.defenseStats.set("armourMaterial", &statArmour::armourMaterial);
		
	/***
	@tfield attackStats attackStats
	@tfield defenseStats defenseStats
	@tfield int attackInBattle
	@tfield int armourInBattle
	@tfield int formationDefBonus
	@tfield int formationAttBonus
	@tfield int generalsBonus
	@tfield int chargeBonus
	@tfield int chargeDecay
	
	@table unitStats
	*/
	types.unitStats = luaState.new_usertype<unitStats>("unitStats");
	types.unitStats.set("chargeBonus", sol::property(&unitStats::getChargeBonus, &unitStats::setChargeBonus));
	types.unitStats.set("chargeDecay", sol::property(&unitStats::getChargeDecay, &unitStats::setChargeDecay));
	types.unitStats.set("attackInBattle", &unitStats::attackInBattle);
	types.unitStats.set("armourInBattle", &unitStats::armourInBattle);
	types.unitStats.set("formationDefBonus", &unitStats::formationDefBonus);
	types.unitStats.set("formationAttBonus", &unitStats::formationAttBonus);
	types.unitStats.set("generalsBonus", &unitStats::generalsBonus);
}
