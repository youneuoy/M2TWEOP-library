#pragma once
#include "fastFuncts.h"
#include "faction.h"
class SCoord
{
public:
	int X;
	int Y;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SCoord, X, Y)
};

class RetreatRoute
{
public:
	SCoord RouteStart;
	SCoord RouteEnd;
	int FactionID;
	int TurnNum;
public:
	RetreatRoute() = default;
	RetreatRoute(int x, int y, int endX, int endY)
	{
		RouteStart.X = x;
		RouteStart.Y = y;

		RouteEnd.X = endX;
		RouteEnd.Y = endY;

		auto* army = fastFuncts::findArmy(x, y);
		if (army == nullptr)
		{
			FactionID = -1;
			TurnNum = -1;
			return;
		}
		FactionID = army->faction->factionID;

		if (fastFuncts::GetCurrentFaction()->factionID != FactionID)
		{
			throw std::exception("Cannot add retreat point for other player faction");
		}

		TurnNum = fastFuncts::getPassedTurnsNum();

		fastFuncts::NuullifyMovepoints(army);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RetreatRoute, RouteStart, RouteEnd, FactionID, TurnNum)
};