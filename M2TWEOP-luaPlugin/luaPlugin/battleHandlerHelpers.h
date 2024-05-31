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
	battlefieldEngines* getBattlefieldEngines();
	siegeEngine* getSiegeEngine(const battlefieldEngines* engineDb, const int index);
	battleDataS* getBattleData();
	bool inBattle();
	battlePos* getCoordPair(const deploymentAreaS* deployArea, const int index);
	int getEngineType(const siegeEngine* engine);
	AIBattleObjectiveBase* getObjective(const battleAI* battleAi, int index);
	int getObjectiveType(AIBattleObjectiveBase* objective);
	unit* getUnit(const AIBattleObjectiveBase* objective, int index);
	buildingBattle* getBattleBuilding(const battleBuildings* buildings, int index);
};

