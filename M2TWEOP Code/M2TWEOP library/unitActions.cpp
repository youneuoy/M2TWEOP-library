// ReSharper disable CppTooWideScopeInitStatement
#include "unitActions.h"

#include "fastFunctsHelpers.h"
#include "functionsOffsets.h"

namespace unitActions
{
    enum class formation
    {
        horde = 0,
        column = 1,
        square = 2,
        wedge = 3,
        phalanx = 5,
        schiltrom = 6,
        shieldWall = 7,
        wall = 8,
        movingThrough = 9,
    };
    
    enum class engineType
    {
        catapult = 0,
        trebuchet = 1,
        ballista = 2,
        bombard = 3,
        grandBombard = 4,
        hugeBombard = 5,
        culverin = 6,
        basilisk = 7,
        cannon = 8,
        mortar = 9,
        scorpion = 10,
        serpentine = 11,
        rocketLauncher = 12,
        ribault = 13,
        monsterRibault = 14,
        mangonel = 15,
        tower = 17,
        ram = 18,
        ladder = 19,
        holy_cart = 20,
    };

    int getUnitFormation(const unit* unit)
    {
        return CallVFunc<0x1, int>(unit->formationsArray);
    }

    float getBattleMapHeight(float xCoord, float yCoord)
    {
        const DWORD* offset = reinterpret_cast<DWORD*>(dataOffsets::offsets.battleMapData);
        return GAME_FUNC(float(__thiscall*)(DWORD, float, float, float), getMapHeight)(*offset, xCoord, yCoord, 0.0f);
    }

    unit* getUnitByLabel(const char* label)
    {
        const auto labelCrypt = fastFunctsHelpers::makeCryptedString(label);
        return GAME_FUNC(unit*(__cdecl*)(char**), getUnitByLabel)(labelCrypt);
    }

    struct scriptCommand
    {
        DWORD vftable /*VFT*/;
        int field_4 = 0;
        int lineNumber = 0;
        const char *filePath = "luaCommand";
        int filePathHash = 35062250;
        const char *className = nullptr;
        scriptCommand(const char* name) : className(name)
        {
            this->vftable = dataOffsets::offsets.scriptCommandVFT;
        }
    };

    void fireGameScriptFunc(void* scriptStruct, DWORD offset)
    {
        void* scriptStructPtr = scriptStruct;
        DWORD func = offset;
        _asm
        {
            mov ecx, scriptStructPtr
            mov eax, func
            call eax
        }
    }

    enum aiGtaObjective : int
    {
        gtaObjective_INVALID = 0,
        gtaObjective_MOVE_TO_POINT = 1,
        gtaObjective_ATTACK_ENEMY_BATTLEGROUP = 2,
        gtaObjective_DEFEND_TERRAIN_HILL = 3,
        gtaObjective_DEFEND_TERRAIN_FOREST = 4,
        gtaObjective_DEFEND_TERRAIN_AREA = 5,
        gtaObjective_DEFEND_CROSSING = 6,
        gtaObjective_ASSAULT_CROSSING = 7,
        gtaObjective_DEFEND_LINE = 8,
        gtaObjective_SCOUT = 9,
        gtaObjective_WITHDRAW = 10,
        gtaObjective_DEFEND_SETTLEMENT = 11,
        gtaObjective_SUPPORT_DEFEND_SETTLEMENT = 12,
        gtaObjective_ATTACK_SETTLEMENT = 13,
        gtaObjective_SKIRMISH = 14,
        gtaObjective_BOMBARD = 15,
        gtaObjective_ATTACK_MODEL = 16,
        gtaObjective_SALLY_OUT = 17,
        gtaObjective_AMBUSH = 18,
        gtaObjective_ERROR = 19,
    };

    
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
        const auto order = std::make_shared<unitImmediatePlace>(un, xCoord, yCoord, angle, width);
        fireGameScriptFunc(order.get(), codes::offsets.unitImmediatePlace);
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
    
    void unitMovetoPosition(const unit* un, float xCoord, float yCoord, bool run)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitOrderMove>(un, xCoord, yCoord, run);
        fireGameScriptFunc(order.get(), codes::offsets.unitOrderMove);
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
    
    void unitAttackClosest(const unit* un, int16_t angle, bool run)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitAttackClosestUnit>(un, angle, run);
        fireGameScriptFunc(order.get(), codes::offsets.unitAttackClosestUnit);
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
    
    void attackUnit(const unit* un, const unit* targetUnit, bool run)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitAttackUnit>(un, targetUnit, run);
        fireGameScriptFunc(order.get(), codes::offsets.unitAttackUnit);
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
        fireGameScriptFunc(order.get(), codes::offsets.unitDeployStakes);
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
        fireGameScriptFunc(order.get(), codes::offsets.unitChangeFormation);
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
        fireGameScriptFunc(order.get(), codes::offsets.unitHalt);
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
    
    void moveToOrientation(const unit* un, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitMoveToOrientation>(un, xCoord, yCoord, widthInMen, angle, run);
        fireGameScriptFunc(order.get(), codes::offsets.unitMoveToOrientation);
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
    
    void moveRelative(const unit* un, float xCoord, float yCoord, bool run)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitMoveRelative>(un, xCoord, yCoord, run);
        fireGameScriptFunc(order.get(), codes::offsets.moveRelative);
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
    
    void moveToMissileRange(const unit* un, const unit* targetUnit, bool run)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitMoveToMissileRange>(un, targetUnit, run);
        fireGameScriptFunc(order.get(), codes::offsets.moveToMissileRange);
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
    
    void unitTurn(const unit* un, int16_t angle, bool isRelative)
    {
        if (un == nullptr)
            return;
        const auto order = std::make_shared<unitOrderTurn>(un, angle, isRelative);
        fireGameScriptFunc(order.get(), codes::offsets.unitTurn);
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
        fireGameScriptFunc(order.get(), codes::offsets.unitTaunt);
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
        fireGameScriptFunc(order.get(), codes::offsets.useSpecialAbility);
    }

    /*----------------------------------------------------------------------------------------------------------------*\
                                                  Attack Gate
    \*----------------------------------------------------------------------------------------------------------------*/

    int getSiegeEngineType(const unit* un)
    {
        if (un == nullptr)
            return -1;
        if (un->engineRec == nullptr)
            return -1;
        return un->engineRec->classID;
    }

    void attackBuilding(const unit* un, buildingBattle* building)
    {
        if (un == nullptr || building == nullptr)
            return;
        if (getSiegeEngineType(un) == -1)
            return;
        auto battleResidence = building->battleResidence;
        if (battleResidence == nullptr)
            return;
        auto worldData = battleResidence->worldData;
        if (worldData == nullptr)
            return;
        auto unit = un;
        int buildingAngle = GAME_FUNC(int(__thiscall*)(buildingBattle*), getBuildingAngle)(building);
        float* coords = &building->posX;
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
}
