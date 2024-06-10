#include "unitHelpers.h"

#include "battleHandlerHelpers.h"
#include "gameDataAllHelper.h"
#include "plugData.h"
#include "smallFuncs.h"

namespace unitHelpers
{
	std::unordered_map<int, const char*> actionTypes = {
		{0,"idling"},
		{1,"hiding"},
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
		{23,"infighting"},
		{16,"go_berserk"},
		{17,"rallying"},
		{18,"dead"},
		{19,"leaving_battle"},
		{20,"entering_battle"},
		{21,"left_battle"},
		{2,"taunting"},
		{24,"bracing"},
	};
	int getExp(unit* un)
	{
		return un->expScreen;
	}
	void setExp(unit* un, int exp)
	{
		un->expScreen = exp;
	}
	int getarmourLVL(unit* un)
	{
		return un->stats >> 0xd & 0x1f;
	}
	void setarmourLVL(unit* un, int lvl)
	{
		(*(*plugData::data.funcs.setUnitParams))(un, un->SoldierCountStrat, un->expScreen, lvl, getweaponLVL(un));
	}
	int getweaponLVL(unit* un)
	{
		return un->stats >> 0x8 & 0x1f;
	}
	void setweaponLVL(unit* un, int lvl)
	{
		(*(*plugData::data.funcs.setUnitParams))(un, un->SoldierCountStrat, un->expScreen, getarmourLVL(un), lvl);
	}
	
	int getMaxSoldiersCount(unit* un)
	{

		int sizeMul=m2tweopHelpers::GetUnitSize();

		float mul = 2.5f;
		switch (sizeMul)
		{
		case 0:
		{
			mul = 1.f;
			break;
		}
		case 1:
		{			
			mul = 1.25f;
			break;
		}
		case 2:
		{
			mul = 1.875f;
			break;
		}
		default:
			break;
		}
		return static_cast<int>(un->eduEntry->SoldierCount) * mul;
	}
	void setMovepoints(unit* un, float movepoints)
	{
		(*(*plugData::data.funcs.setUnitMovepoints))(un, movepoints);

	}
	void setSoldiersCount(unit* un, int number)
	{
		(*(*plugData::data.funcs.setSoldiersCount))(un, number);
	}
	void killUnit(unit* un)
	{
		(*(*plugData::data.funcs.killUnit))(un);
	}
	void setUnitParams(unit* un, int exp, int armor, int weap)
	{
		(*(*plugData::data.funcs.setUnitParams))(un, un->SoldierCountStrat, exp, armor, weap);
	}
	float getMovepoints(const unit* un)
	{
		return un->movePoints;
	}
	int getMoraleLevel(const unit* un)
	{
		return un->unitMorale.moraleLevel;
	}
	void setMoraleLevel(unit* un, int level)
	{
		un->unitMorale.moraleLevel = level;
	}

	int16_t angleFloatToShort(float angle)
	{
		return static_cast<int16_t>(angle * 182.044432904376f);
	}

	float angleShortToFloat(int16_t angle)
	{
		return angle * 0.0054931640625f;
	}

	float getUnitAngle(const unit* un)
	{
		if (!un->unitPositionData)
			return 0;
		return angleShortToFloat(un->unitPositionData->angle);
	}
	
	void setUnitAngle(unit* un, float angle)
	{
		un->unitPositionData->angle = angleFloatToShort(angle);
	}

	float getEngineAngle(const siegeEngine* engine)
	{
		return angleShortToFloat(engine->angle);
	}
	
	void setEngineAngle(siegeEngine* engine, float angle)
	{
		engine->angle = angleFloatToShort(angle);
	}
	
	int getsoldierCountStratMap(const unit* un)
	{
		return un->SoldierCountStrat;
	}
	
	bool hasBattleProperty(const unit* un, unitBattleProperties prop)
	{
		return (un->unitBattleProperties & prop) != 0;
	}
	void setBattleProperty(unit* un, unitBattleProperties prop, bool value)
	{
		if (value)
		{
			un->unitBattleProperties |= prop;
		}
		else
		{
			un->unitBattleProperties &= ~prop;
		}
	}
	const char* getActionStatus(unit* un)
	{
		auto it = actionTypes.find(un->actionStatus);
		if (it != actionTypes.end())
		{
			return it->second;
		}
		return "unknown";
	}
	bool isMovingFastSet(unit* un)
	{
		return un->statusField2 & 0x1;
	}
	void setMovingFastSet(unit* un, bool set)
	{
		if (un->eduEntry->Category == 0 && un->siegeEnNum > 0
			&& un->siegeEngine[0]->engineRecord->classID != engineType::ladder)
		{
			un->statusField2 &= ~0x1;
			return;
		}
		un->statusField2 = set ? un->statusField2 | 0x1 : un->statusField2 & ~0x1;
	}
	bool isOnWalls(unit* un)
	{
		return un->unitPositionData->isInGateHouseCount + un->unitPositionData->isInTowerCount + un->unitPositionData->isOnWallsCount;
	}
	bool isEngaged(unit* un)
	{
		return un->unitPositionData->engagedUnitsNum;
	}
	bool isUnderFire(unit* un)
	{
		auto posData = un->unitPositionData;
		return posData->towersUnderFireFromCount + posData->unitsUnderFireFromCount + posData->underFireSomething2Count + posData->artilleryUnderFireFromNum;
	}
	unit* getUnitUnderFireFrom(unitPositionData* posData, int index)
	{
		if (index < posData->unitsUnderFireFromCount)
		{
			return posData->unitsUnderFireFrom[index].unit;
		}
		return nullptr;
	}
	unit* getEngagedUnit(unitPositionData* posData, int index)
	{
		if (index < posData->engagedUnitsNum)
		{
			return posData->engagedUnits[index].unit;
		}
		return nullptr;
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
		DWORD funcOffset = 0x00A931C0;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcOffset = 0x00A945C0;
		
		DWORD groupLabels = 0x01B60580;
		if (m2tweopHelpers::getGameVersion() == 1)
			groupLabels = 0x001BA96A0;
		
		stringWithHash* labelHash = new stringWithHash();
		luaGetSetFuncs::setGameString(reinterpret_cast<char*>(labelHash), label);
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
		
		auto allLabels = reinterpret_cast<groupLabels*>(0x01B60580);
		if (m2tweopHelpers::getGameVersion() == 1)
			allLabels = reinterpret_cast<groupLabels*>(0x001BA96A0);
		
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
		if (un->army && un->army->faction)
		{
			if (un->army->faction->isPlayerControlled)
				un->aiActiveSet = 0;
			else
				un->aiActiveSet = 1;
		}
		CallVFunc<2, void*>(group, un, 1);
		if (group->unitsInFormationNum + group->unitsNotInFormationNum == 0)
			undefineUnitGroup(group);
	}

	unit* getUnitInFormation(const unitGroup* group, const int index)
	{
		if (!group || index < 0 || index >= group->unitsInFormationNum)
			return nullptr;
		return group->unitsInFormation[index];
	}

	unit* getUnitNotInFormation(const unitGroup* group, const int index)
	{
		if (!group || index < 0 || index >= group->unitsNotInFormationNum)
			return nullptr;
		return group->unitsNotInFormation[index];
	}

	void automateGroup(const unitGroup* group, bool automate)
	{
		if (!group)
			return;
		DWORD funcAddr = 0x00721B50;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcAddr = 0x00721410;
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
		group->field_1338 = 0;
		group->newTargetUnit = nullptr;
		group->newDefendRadius = 0;
		group->field_133C = 0;
		group->field_1340 = 0;
		group->field_1344 = 0;
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
		group->field_1338 = 0;
		group->newTargetUnit = nullptr;
		group->newDefendRadius = 0;
		group->field_133C = 0;
		group->field_1340 = 0;
		group->field_1344 = 0;
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
		DWORD funcAddr = 0x0071E620;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcAddr = 0x0071DEE0;
		int16_t pushAngle = (angle * 0.017453292) * 10430.378;
		_asm
		{
			push 1
			push 1
			push pushAngle
			push coords
			mov ecx, group
			mov eax, funcAddr
			call eax
		}
		delete[] coords;
	}

	void changeGroupUnitFormation(const unitGroup* group, int formationType)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.changeGroupUnitFormation))(group, formationType);
	}

	void moveToRangeOfGroup(const unitGroup* group, const unitGroup* targetGroup, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.moveToRangeOfGroup))(group, targetGroup, run);
	}

	void moveGroupToRangeOfUnit(const unitGroup* group, const unit* targetUnit, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.moveGroupToRangeOfUnit))(group, targetUnit, run);
	}

	void groupAttackGroup(const unitGroup* group, const unitGroup* targetGroup, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupAttackGroup))(group, targetGroup, run);
	}

	void groupHalt(const unitGroup* group)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupHalt))(group);
	}

	void groupMoveFormed(const unitGroup* group, float xCoord, float yCoord, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupMoveFormed))(group, xCoord, yCoord, run);
	}

	void groupMoveUnformed(const unitGroup* group, float xCoord, float yCoord, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupMoveUnformed))(group, xCoord, yCoord, run);
	}

	void groupMoveFormedRelative(const unitGroup* group, float xCoord, float yCoord, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupMoveFormedRelative))(group, xCoord, yCoord, run);
	}

	void groupMoveUnformedRelative(const unitGroup* group, float xCoord, float yCoord, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupMoveUnformedRelative))(group, xCoord, yCoord, run);
	}

	void groupTurn(const unitGroup* group, int16_t angle, bool isRelative)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.groupTurn))(group, angle, isRelative);
	}

	void undefineUnitGroup(const unitGroup* group)
	{
		if (!group)
			return;
		DWORD clearUnitGroup = 0x0071ECD0;
		if (m2tweopHelpers::getGameVersion() == 1)
			clearUnitGroup = 0x0071E590;
		_asm
		{
			mov ecx, group
			mov eax, clearUnitGroup
			call eax
		}
		auto label = getGroupLabel(group);
		if (label != "")
		{
			DWORD removeUnusedLabel = 0x00A93380;
			if (m2tweopHelpers::getGameVersion() == 1)
				removeUnusedLabel = 0x00A92320;
			
			DWORD groupLabels = 0x01B60580;
			if (m2tweopHelpers::getGameVersion() == 1)
				groupLabels = 0x001BA96A0;
			
			stringWithHash* labelHash = new stringWithHash();
			luaGetSetFuncs::setGameString(reinterpret_cast<char*>(labelHash), label);
			_asm
			{
				push labelHash
				mov ecx, groupLabels
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
		
		DWORD funcOffset = 0x00A92C30;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcOffset = 0x00A91BD0;
		
		DWORD groupLabels = 0x01B60580;
		if (m2tweopHelpers::getGameVersion() == 1)
			groupLabels = 0x001BA96A0;
		
		stringWithHash* labelHash = new stringWithHash();
		luaGetSetFuncs::setGameString(reinterpret_cast<char*>(labelHash), label);
		unitGroup** groupPtr = &group;
		__asm
		{
			push groupPtr
			push labelHash
			mov ecx, groupLabels
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

	/*
	* 

	void namedCharSetLocalizedNameForSave(namedCharacter* genChar, const char* str)
	{
		(*(*plugData::data.funcsTech.createUniString))(genChar->localizedNameForSave, str);
	}

	 */
	
	unit* getTargetUnit(unitPositionData* posData)
	{
		if (!battleHandlerHelpers::inBattle())
			return nullptr;
		if (!posData->hasTargets || posData->isHalted)
		{
			return nullptr;
		}
		if (posData->targetArray[posData->currIndex].targetVerification.tablePos)
		{
			return posData->targetArray[posData->currIndex].targetVerification.tablePos->unit;
		}
		return nullptr;
	}
	
	int getMountClass(unit* un)
	{
		if (!un) return -1;
		return CallVFunc<186, int>(un);
	}

	int getUnitFormation(const unit* unit)
	{
		return (*(*plugData::data.funcs.getUnitFormation))(unit);
	}

	unit* getUnitByLabel(const char* label)
	{
		return (*(*plugData::data.funcs.getUnitByLabel))(label);
	}

	void unitAttackClosest(unit* un, int16_t angle, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		un->aiActiveSet = 2;
		(*(*plugData::data.funcs.unitAttackClosest))(un, angle, run);
	}

	void unitAttackUnit(unit* un, const unit* targetUnit, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		un->aiActiveSet = 2;
		(*(*plugData::data.funcs.attackUnit))(un, targetUnit, run);
	}

	void unitMovetoPosition(unit* unit, float xCoord, float yCoord, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		unit->aiActiveSet = 2;
		(*(*plugData::data.funcs.unitMovetoPosition))(unit, xCoord, yCoord, run);
	}

	void moveToOrientation(unit* unit, float xCoord, float yCoord, int widthInMen, int16_t angle, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		unit->aiActiveSet = 2;
		(*(*plugData::data.funcs.moveToOrientation))(unit, xCoord, yCoord, widthInMen, angle, run);
	}

	void placeUnit(unit* unit, float xCoord, float yCoord, int16_t angle, int width)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.placeUnit))(unit, xCoord, yCoord, angle, width);
	}

	void deployStakes(const unit* unit)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.deployStakes))(unit);
	}

	void haltUnit(const unit* unit)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.haltUnit))(unit);
	}

	void changeUnitFormation(const unit* unit, int formationType)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.changeUnitFormation))(unit, formationType);
	}

	float getBattleMapHeight(float xCoord, float yCoord)
	{
		return (*(*plugData::data.funcs.getBattleMapHeight))(xCoord, yCoord);
	}

	void moveRelative(unit* unit, float xCoord, float yCoord, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		unit->aiActiveSet = 2;
		(*(*plugData::data.funcs.moveRelative))(unit, xCoord, yCoord, run);
	}

	void moveToMissileRange(unit* un, const unit* targetUnit, bool run)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		un->aiActiveSet = 2;
		(*(*plugData::data.funcs.moveToMissileRange))(un, targetUnit, run);
	}

	void unitTurn(unit* un, int16_t angle, bool isRelative)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		un->aiActiveSet = 2;
		(*(*plugData::data.funcs.unitTurn))(un, angle, isRelative);
	}

	void taunt(const unit* un)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.taunt))(un);
	}

	void useSpecialAbility(const unit* un)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		(*(*plugData::data.funcs.useSpecialAbility))(un);
	}

	unit* getNearbyUnit(const unit* unit, const int index)
	{
		if (index < 0 || index >= unit->nearbyFriendlyUnitsNum)
			return nullptr;
		return unit->nearbyFriendlyUnits[index];
	}

	unit* getNearbyEnemyUnit(const unit* unit, const int index)
	{
		if (index < 0 || index >= unit->nearbyEnemyUnitsNum)
			return nullptr;
		return unit->nearbyEnemyUnits[index].unit;
	}
	
	siegeEngine* getSiegeEngine(const unit* un, const int index)
	{
		const auto category = un->eduEntry->Category;
		if (category != 0 && category != 2)
			return nullptr;
		if (!un->siegeEngine)
			return nullptr;
		if (index < 0 || index >= un->siegeEnNum)
			return nullptr;
		return un->siegeEngine[index];
	}

	void attackBuilding(unit* un, buildingBattle* building)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		un->aiActiveSet = 2;
		(*(*plugData::data.funcs.attackBuilding))(un, building);
	}

	void releaseUnit(unit* un)
	{
		if (!battleHandlerHelpers::inBattle())
			return;
		const auto stack = un->army;
		const auto battleData = battleHandlerHelpers::getBattleData();
		for (int i = 0; i < battleData->playerArmyNum; i++)
		{
			const auto playerArmy = battleData->playerArmies[i].army;
			if (playerArmy == stack)
			{
				un->aiActiveSet = 0;
				return;
			}
		}
		un->aiActiveSet = 1;
	}

	int getSiegeEngineNum(unit* un)
	{
		if (un->eduEntry->Category != 0 && un->eduEntry->Category != 2)
			return 0;
		return un->siegeEnNum;
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
		un->unitPositionData->targetsToGo = 1;
	}
}
