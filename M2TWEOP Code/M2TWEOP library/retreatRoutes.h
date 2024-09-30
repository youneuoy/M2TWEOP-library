#pragma once
#include "army.h"
#include "faction.h"
#include "strategyMap.h"

class sCoord
{
public:
	int x;
	int y;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(sCoord, x, y)
};

class retreatRoute
{
public:
	sCoord routeStart;
	sCoord routeEnd;
	int factionID;
	int turnNum;
public:
	retreatRoute() = default;
	retreatRoute(int x, int y, int endX, int endY)
	{
		routeStart.x = x;
		routeStart.y = y;

		routeEnd.x = endX;
		routeEnd.y = endY;
		const auto campaignData = campaignHelpers::getCampaignData();
		const auto tile = stratMapHelpers::getTile(x, y);
		auto* army = tile->getArmy(false);
		if (army == nullptr)
		{
			factionID = -1;
			turnNum = -1;
			return;
		}
		factionID = army->faction->factionID;

		if (campaignData->currentFactionTurn->factionID != factionID)
		{
			throw std::exception("Cannot add retreat point for other player faction");
		}
		turnNum = campaignData->turnNumber;
		army->nullifyMovePoints();
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(retreatRoute, routeStart, routeEnd, factionID, turnNum)
};