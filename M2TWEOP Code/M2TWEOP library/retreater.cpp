#include "retreater.h"
#include "PlannedRetreatRoute.h"
#include "PathMap.h"
#include "smallFuncs.h"
#include "MapTextDrawer.h"
#include "character.h"
#include "faction.h"
#include <mutex>

#include "army.h"
#include "strategyMap.h"


void retreater::retreatArmy(battleArmy& battleArmy)
{
	battleArmy.army->nullifyMovePoints();
	if (battleArmy.character == nullptr)
		return;

	std::pair<int, int>retreatCoords;

	auto currCoords = battleArmy.army->getCoords();
	//if route planned
	if (plannedRetreatRoute::tryRetreatArmyWithRoute(battleArmy, retreatCoords) == true)
	{
		retreats.emplace_back(currCoords.first, currCoords.second, retreatCoords.first, retreatCoords.second);
		return;
	}
	void* cache = pathFinder::createCacheForArmy(battleArmy.army, 15);
	retreatCoords = pathFinder::getSafestTileForArmyFromCache(cache, battleArmy.army);
	pathFinder::deleteCacheForDistances(cache);

	if (currCoords == retreatCoords)
		return;
	
	retreats.emplace_back(currCoords.first, currCoords.second, retreatCoords.first, retreatCoords.second);

	const int x = retreatCoords.first;
	const int y = retreatCoords.second;
	const auto* destDile = stratMapHelpers::getTile(x, y);

	if (destDile->object == nullptr)
	{
		characterHelpers::teleportCharacter(battleArmy.character, x, y);
		battleArmy.character->actionType = static_cast<int>(characterAction::ambush);
		return;
	}

	switch (void* endObj = stratMapHelpers::getMainStratObject(destDile->object);
		callVFunc<4, strategyObject>(endObj))
	{
	case strategyObject::settlement:
	{
		armyHelpers::stopSiege(battleArmy.army);
		armyHelpers::stopBlockPort(battleArmy.army);
		const auto set = static_cast<settlementStruct*>(endObj);
		armyHelpers::addToSettlement(battleArmy.army, set);
		break;
	}
	case strategyObject::fort:
	{
		armyHelpers::stopSiege(battleArmy.army);
		armyHelpers::stopBlockPort(battleArmy.army);
		const auto fort = static_cast<fortStruct*>(endObj);
		armyHelpers::addToFort(battleArmy.army, fort);	
		break;
	}
	case strategyObject::floatingGeneral:
	case strategyObject::port:
	case strategyObject::character:
	case strategyObject::rallyPointSundry:
    case strategyObject::battleSiteMarker:
    case strategyObject::watchtower:
    case strategyObject::sundry:
		break;
	}
}

void retreater::retreatSide(battleSide& bside)
{

	for (int i = 0; i < bside.armiesNum; ++i)
	{
		if (bside.forces[i].army->faction->isPlayerControlled == true)
		{
			isPlayerInvolved = true;
		}
		if (bside.wonBattle != 0)//0 - lost
		{
			continue;
		}
		if (bside.forces[i].army->settlement != nullptr)
		{
			continue;
		}

		retreatArmy(bside.forces[i]);
	}
}

void retreater::startPostWork()
{
	if (isPlayerInvolved == false)
	{
		return;
	}


	void* font = mapTextDrawer::makeTextFont("Times New Roman", 400, 1);
	if (font == nullptr)
	{
		MessageBoxA(NULL, "Cannot create text font for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
		std::terminate();
	}

	for (auto& txt : retreats)
	{
		auto* text = mapTextDrawer::makeText(font, "o");
		if (text == nullptr)
		{
			MessageBoxA(NULL, "Cannot create text for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
			std::terminate();
		}
		int stX = std::get<0>(txt);
		int stY = std::get<1>(txt);

		int enX = std::get<2>(txt);
		int enY = std::get<3>(txt);

		text->xCoord = enX;
		text->yCoord = enY;
		text->zCoord = 0.2f;
		text->xSize = 4;
		text->ySize = 0.6f;
		text->zSize = 4;

		mapTextDrawer::coordsVText* cvt = new mapTextDrawer::coordsVText(stX, stY, text);

		cvt->SetTileToLive(30.f);
	}

	mapTextDrawer::deleteTextFont(font);
}

retreater::retreater(battleDataS* battle)
{
	Battle = battle;
}

retreater::~retreater()
{
}

void retreater::retreat()
{
	for (int i = 0; i < Battle->sidesNum; ++i)
	{
		retreatSide(Battle->sides[i]);
	}

	startPostWork();
}
