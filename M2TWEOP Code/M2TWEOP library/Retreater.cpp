#include "Retreater.h"
#include "PlannedRetreatRoute.h"
#include "PathMap.h"
#include "smallFuncs.h"
#include "MasterDefines.h"
#include "MapTextDrawer.h"
#include "character.h"
#include <mutex>


void Retreater::RetreatArmy(armyAndCharacter& army)
{
	fastFuncts::NuullifyMovepoints(army.army);
	if (army.character == nullptr)
	{
		return;
	}

	std::pair<int, int>retreatCoords;

	auto currCoords = fastFuncts::getArmyCoords(army.army);
	//if route planned
	if (PlannedRetreatRoute::TryRetreatArmyWithRoute(army, retreatCoords) == true)
	{
		retreats.emplace_back(currCoords.first, currCoords.second, retreatCoords.first, retreatCoords.second);
		return;
	}
	void* cashe = PathFinder::CreateCasheForArmy(army.army, 15);
	retreatCoords = PathFinder::GetSafestTileForArmyFromCashe(cashe, army.army);
	PathFinder::DeleteCasheForDistances(cashe);

	if (currCoords == retreatCoords)
	{
		return;
	}
	retreats.emplace_back(currCoords.first, currCoords.second, retreatCoords.first, retreatCoords.second);

	int x = retreatCoords.first;
	int y = retreatCoords.second;
	auto* destDile = fastFuncts::getTileStruct(x, y);

	if (destDile->object == nullptr)
	{
		characterHelpers::teleportCharacter(army.character, x, y);
		army.character->ambushState = 10;
		return;
	}

	void* endObj = smallFuncs::GetMainStratObject(destDile->object);

	StartMapObjectType objT = CallVFunc<4, StartMapObjectType>(endObj);

	switch (objT)
	{
	case StartMapObjectType::FloatingGeneral:
		break;
	case StartMapObjectType::Settlement:
	{
		fastFuncts::StopSiege(army.army);
		fastFuncts::StopBlockPort(army.army);
		settlementStruct* set = (settlementStruct*)endObj;
		fastFuncts::AddToSettlement(army.army,set);
		break;
	}
	case StartMapObjectType::Fort:
	{
		fastFuncts::StopSiege(army.army);
		fastFuncts::StopBlockPort(army.army);
		fortStruct* fort = (fortStruct*)endObj;
		fastFuncts::AddToFort(army.army, fort);	
		break;
	}
	case StartMapObjectType::Port:
		break;
	case StartMapObjectType::Character:
		break;
	case StartMapObjectType::RallyPointSundry:
		break;
	default:
		break;
	}
}

void Retreater::RetreatSide(battleSide& bside)
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

		RetreatArmy(bside.forces[i]);
	}
}

void Retreater::StartPostWork()
{
	if (isPlayerInvolved == false)
	{
		return;
	}


	void* font = MapTextDrawer::MakeTextFont("Times New Roman", 400, 1);
	if (font == nullptr)
	{
		MessageBoxA(NULL, "Cannot create text font for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
		std::terminate();
	}

	for (auto& txt : retreats)
	{
		auto* text = MapTextDrawer::MakeText(font, "o");
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

		MapTextDrawer::coordsVText* cvt = new MapTextDrawer::coordsVText(stX, stY, text);

		cvt->SetTileToLive(30.f);
	}

	MapTextDrawer::DeleteTextFont(font);
}

Retreater::Retreater(battleDataS* battle)
{
	Battle = battle;
}

Retreater::~Retreater()
{
}

void Retreater::Retreat()
{
	for (int i = 0; i < Battle->sidesNum; ++i)
	{
		RetreatSide(Battle->sides[i]);
	}

	StartPostWork();
}
