#include "unitHelpers.h"
#include "plugData.h"
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
		(*(*plugData::data.funcs.setUnitParams))(un, un->SoldierCountStrat, exp, getarmourLVL(un), getweaponLVL(un));
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
		return un->eduEntry->SoldierCount* mul;
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
		return posData->towersUnderFireFromCount + posData->unitsUnderFireFromCount + posData->underFireSomething2Count + posData->underFireSomething3Count;
	}
	unit* getUnitUnderFireFrom(unitPositionData* posData, int index)
	{
		if (index < posData->unitsUnderFireFromCount)
		{
			return posData->unitsUnderFireFrom[index];
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
	unit* getTargetUnit(unitPositionData* posData)
	{
		if (!posData->hasTargets || posData->isHalted)
		{
			return nullptr;
		}
		if (posData->targetArray[posData->targetsToGo].targetUnit)
		{
			return posData->targetArray[posData->targetsToGo].targetUnit->target;
		}
		return nullptr;
	}

}