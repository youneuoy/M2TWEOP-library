#pragma once
#include<string>
#include<vector>
#include "realGameTypes.h"
#include "RetreatRoutes.h"
namespace PlannedRetreatRoute
{
	void StartWork(int x, int y);

	//reset routes
	void OnNewGameStart();
	
	void OnFactionTurnStart(factionStruct*fac);
	std::string OnGameSave();
	void OnGameLoad(const std::vector<std::string>& filePathes);


	void OnClickAtTile(int x, int y);

	bool TryRetreatArmyWithRoute(armyAndCharacter& army);
	void OnRetreat();
};

