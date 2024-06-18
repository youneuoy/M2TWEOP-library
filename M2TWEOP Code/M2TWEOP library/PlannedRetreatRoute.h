#pragma once
#include<string>
#include<vector>
#include "realGameTypes.h"
#include "RetreatRoutes.h"
namespace plannedRetreatRoute
{
	void startWork(int x, int y);

	//reset routes
	void onNewGameStart();
	
	void onFactionTurnStart(factionStruct*fac);
	std::string onGameSave();
	void onGameLoad(const std::vector<std::string>& filePathes);


	void onClickAtTile(int x, int y);

	bool tryRetreatArmyWithRoute(battleArmy& army, std::pair<int, int>&resCoords);
	void onRetreat();
};

