#include "battleHandlerHelpers.h"

#include "gameDataAllHelper.h"
#include "plugData.h"
namespace battleHandlerHelpers
{
	std::string getWinConditionS(DWORD condition)
	{
		int condCode = (*(*plugData::data.funcsBattle.getBattleCondCode))(condition);

		switch (condCode)
		{
		case 0: return "destroy_or_rout_enemy"; break;
		case 1: return "balance_of_strength_percent"; break;
		case 2: return "destroy_enemy_strength_percent"; break;
		case 3: return "capture_location"; break;
		case 4: return "destroy_character"; break;
		case 5: return "capture_major_settlement"; break;
		case 6: return "capture_army_settlement"; break;
		default: return "unknown_condition"; break;
		}
	}

	armyAndCharacter* getBattleArmy(const battleSide* side, int index)
	{
		return &side->forces[index];
	}

	battleUnit* getBattleUnit(const armyAndCharacter* battleArmy, int index)
	{
		return &battleArmy->units[index];
	}

	stackStruct* getPlayerArmy(const battleDataS* battleData)
	{
		return battleData->playerArmies[0].army;
	}

	factionStruct* getFaction(const battleSide* side, int index)
	{
		return side->factions[index];
	}

	AIBattleObjectiveBase* getObjective(const battleAI* battleAi, int index)
	{
		if (!battleAi->addedObjectives) return nullptr;
		if ( (index < 0) || (index >= battleAi->addedObjectivesCount)) return nullptr;
		return battleAi->addedObjectives[index];
	}

	unit* getUnit(const AIBattleObjectiveBase* objective, int index)
	{
		if (!objective->aiUnits) return nullptr;
		if ( (index < 0) || (index >= objective->aiUnitsCount)) return nullptr;
		return objective->aiUnits[index];
	}

	int getObjectiveType(AIBattleObjectiveBase* objective)
	{
		if (!objective) return -1;
		return CallVFunc<7, int>(objective);
	}

	battleResidence* getBattleResidence()
	{
		const auto gameData = gameDataAllHelper::get();
		const auto battleSettlement = gameData->battleSettlement;
		if (battleSettlement)
		{
			return battleSettlement->battleResidence;
		}
		return nullptr;
	}
};
