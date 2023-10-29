#pragma once
#include <string>
#include "realGameTypes.h"
namespace battleHandlerHelpers
{
	std::string getWinConditionS(DWORD condition);
	armyAndCharacter* getBattleArmy(const battleSide* side, int index);
	battleUnit* getBattleUnit(const armyAndCharacter* battleArmy, int index);
	stackStruct* getPlayerArmy(const battleDataS* battleData);
	factionStruct* getFaction(const battleSide* side, int index);
	battleResidence* getBattleResidence();
};

