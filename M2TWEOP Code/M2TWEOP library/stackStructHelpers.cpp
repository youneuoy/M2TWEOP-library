#include "stackStructHelpers.h"
#include "plugData.h"
namespace stackStructHelpers
{
	void sortStack(stackStruct* stack, const int sortType, const int sortType2, int sortType3)
	{
		int N = stack->numOfUnits;

		std::vector<int> sortTypes = { sortType };
		if (sortType2 != 0)
			sortTypes.push_back(sortType2);
		if (sortType3 != 0)
			sortTypes.push_back(sortType3);

		// Lambda function to compare unit objects based on multiple criteria
		auto compareUnits = [&sortTypes](const unit* unitA, const unit* unitB)
		{
			// Ensuring Generals stay at the front/start of the stack
			if (unitA->general && !unitB->general) {
				return true;
			}
			if (!unitA->general && unitB->general) {
				return false;
			}
			for (int sortType : sortTypes)
			{
				switch (sortType)
				{
					case 1: // By EDU Type
						if (std::string(unitA->eduEntry->Type) != std::string(unitB->eduEntry->Type))
							return std::string(unitA->eduEntry->Type) < std::string(unitB->eduEntry->Type);
						break;
					case 2: // By Category
						if (unitA->eduEntry->Category != unitB->eduEntry->Category)
							return unitA->eduEntry->Category < unitB->eduEntry->Category;
						break;
					case 3: // By Class
						if (unitA->eduEntry->Class != unitB->eduEntry->Class)
							return unitA->eduEntry->Class < unitB->eduEntry->Class;
						break;
					case 4: // Soldier Count
						if (unitA->SoldierCountStrat != unitB->SoldierCountStrat)
							return unitA->SoldierCountStrat > unitB->SoldierCountStrat;
						break;
					case 5: // Experience
						if (unitA->expScreen != unitB->expScreen)
							return unitA->expScreen > unitB->expScreen;
						break;
					case 6: // categoryClass
						if (unitA->eduEntry->categoryClassCombinationForAI != unitB->eduEntry->categoryClassCombinationForAI)
							return unitA->eduEntry->categoryClassCombinationForAI < unitB->eduEntry->categoryClassCombinationForAI;
						break;
					case 7: // aiUnitValue
						if (unitA->eduEntry->aiUnitValuePerSoldier != unitB->eduEntry->aiUnitValuePerSoldier)
							return unitA->eduEntry->aiUnitValuePerSoldier < unitB->eduEntry->aiUnitValuePerSoldier;
						break;
					default:
						if (std::string(unitA->eduEntry->Type) != std::string(unitB->eduEntry->Type))
							return std::string(unitA->eduEntry->Type) < std::string(unitB->eduEntry->Type);
						break;
				}
			}
			return false;
		};

		// Stable sort the units
		std::stable_sort(stack->units, stack->units + N, compareUnits);
	}

	unit *getUnit(const stackStruct *army, int index)
	{
		return army->units[index];
	}
	general *getCharacter(const stackStruct *army, int index)
	{
		return army->characters[index];
	}
	int attackArmy(stackStruct *army, stackStruct *defender)
	{
		if (!army || !defender || defender->settlement)
			return 0;
		DWORD funcAddr = 0x007195A0;
		if (m2tweopHelpers::getGameVersion() == 1)
			funcAddr = 0x718E80;
		_asm
		{
			push defender
			mov ecx, army
			mov eax, funcAddr
			call eax
		}

		return 1;
	}
	
	void siegeSettlement(stackStruct *army, settlementStruct *sett, bool isAttack)
	{
		general *atkGen = army->gen;

		if (atkGen == nullptr || sett == nullptr)
			return;

		(*(*plugData::data.funcs.siegeSettlement))(atkGen, sett, isAttack);
	}
	
	void siegeFort(stackStruct *army, fortStruct *fort, bool isAttack)
	{
		general *atkGen = army->gen;

		if (atkGen == nullptr || fort == nullptr)
			return;

		(*(*plugData::data.funcs.siegeFort))(atkGen, fort, isAttack);
	}
	
	settlementStruct *findInSettlement(const stackStruct *army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		int x = army->settlement->xCoord;
		int y = army->settlement->yCoord;
		auto tile = gameHelpers::getTile(x, y);
		return gameHelpers::getTileSettlement(tile);
	}
	fortStruct *findInFort(const stackStruct *army)
	{
		if (army->settlement == nullptr)
		{
			return nullptr;
		}

		int x = army->settlement->xCoord;
		int y = army->settlement->yCoord;
		auto tile = gameHelpers::getTile(x, y);
		return gameHelpers::getTileFort(tile);
	}

	unitGroup* getGroup(const stackStruct* army, int index)
	{
		if (!army || !army->unitGroups || index < 0 || index >= army->maxUnitGroups)
			return nullptr;
		return &army->unitGroups[index];
	}

	unit* getDeadUnit(const stackStruct* army, int index)
	{
		if (!army || !army->deadUnits || index < 0 || index >= army->deadUnitsNum)
			return nullptr;
		return army->deadUnits[index];
	}

	void setAiActiveSet(const stackStruct* army, const int set)
	{
		if (!army)
			return;
		for (int i = 0; i < army->numOfUnits; i++)
			army->units[i]->aiActiveSet = set;
	}

	void releaseUnits(const stackStruct* army)
	{
		if (!army)
			return;
		for (int i = 0; i < army->numOfUnits; i++)
			unitHelpers::releaseUnit(army->units[i]);
	}

	unit *createUnit(stackStruct *army, const char *type, int exp, int arm, int weap)
	{
		unit *newUnit = (*(*plugData::data.funcs.createUnitN))(type, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	unit *createUnitByIDX(stackStruct *army, int typeIDX, int exp, int arm, int weap)
	{
		unit *newUnit = (*(*plugData::data.funcs.createUnitIdx))(typeIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	unit *createEOPUnit(stackStruct *army, int typeIDX, int exp, int arm, int weap)
	{
		int eopIDX = (*(*plugData::data.funcsEopEdu.getDataEopEdu))(typeIDX);
		if (eopIDX == 0)
		{
			return nullptr;
		}

		unit *newUnit = (*(*plugData::data.funcs.createUnitEDB))(eopIDX, army->regionID, army->faction->dipNum, exp, arm, weap);
		if (newUnit == nullptr)
		{
			return newUnit;
		}
		(*(*plugData::data.funcs.addUnitToArmy))(army, newUnit);
		return newUnit;
	}
	void mergeArmies(stackStruct *army, stackStruct *targetArmy, bool force)
	{
		if (force)
			(*(*plugData::data.funcs.mergeArmies))(army, targetArmy);
		else
		{
			int targetX, targetY;
			if (targetArmy->gen)
			{
				targetX = targetArmy->gen->xCoord;
				targetY = targetArmy->gen->yCoord;
			}
			else if (targetArmy->settlement)
			{
				targetX = targetArmy->settlement->xCoord;
				targetY = targetArmy->settlement->yCoord;
			}
			else
				return;
			
			sol::table units = sol::state_view(plugData::data.luaAll.luaState).create_table();
			for (int i = 0; i < army->numOfUnits; i++)
			{
				units.add(army->units[i]);
			}
			const auto faction = army->faction;
			factionHelpers::splitArmy(faction, units, targetX, targetY);
		}
	}
	void mergeArmies(stackStruct *army, stackStruct *targetArmy)
	{
		(*(*plugData::data.funcs.mergeArmies))(army, targetArmy);
	}
	
	stackStruct* spawnArmy(
		factionStruct* faction,
		const char* name,
		const char* name2,
		int characterType,
		const char* label,
		const char* portrait,
		int x,
		int y,
		int age,
		bool family,
		int subFaction,
		int unitIndex,
		int exp,
		int wpn,
		int armour
		)
	{
		if (portrait != nullptr && strlen(portrait) == 0)
			portrait = nullptr;
		if (label != nullptr && strlen(label) == 0)
			label = nullptr;
		const auto army = (*(*plugData::data.funcs.spawnArmy))(faction, name, name2, characterType, label, portrait, x, y, age, family, subFaction, unitIndex, exp, wpn, armour);
		if (army && label && label != "")
			luaGetSetFuncs::setStringPropertyGenChar<generalCharactericticsStruct_label>(army->gen->genChar, std::string(label));
		return army;
	}
}