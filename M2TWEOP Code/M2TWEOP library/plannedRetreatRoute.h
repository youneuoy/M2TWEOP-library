#pragma once
#include<string>
#include<vector>
#include "realGameTypes.h"
#include "retreatRoutes.h"
namespace plannedRetreatRoute
{
	void startWork(int x, int y);

	//reset routes
	void onNewGameStart();
	
	void onFactionTurnStart(const factionStruct*fac);
	std::string onGameSave();
	void onGameLoad(const std::vector<std::string>& filePaths);


	void onClickAtTile(int x, int y);

	bool tryRetreatArmyWithRoute(const battleArmy& army, std::pair<int, int>&resCoords);
	void onRetreat();
};

