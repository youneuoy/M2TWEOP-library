#include "Retreater.h"
#include "PlannedRetreatRoute.h"
#include "PathMap.h"
#include "smallFuncs.h"
#include "MasterDefines.h"


void Retreater::RetreatArmy(armyAndCharacter& army)
{
	fastFuncts::NuullifyMovepoints(army.army);
	if (army.character == nullptr)
	{
		return;
	}

	//if route planned
	if (PlannedRetreatRoute::TryRetreatArmyWithRoute(army) == true)
	{
		return;
	}
	auto currCoords = fastFuncts::getArmyCoords(army.army);
	void* cashe = PathFinder::CreateCasheForArmy(army.army, 15);
	auto coord = PathFinder::GetSafestTileForArmyFromCashe(cashe, army.army);
	PathFinder::DeleteCasheForDistances(cashe);

	if (currCoords == coord)
	{
		return;
	}

	int x = coord.first;
	int y = coord.second;
	auto* destDile = fastFuncts::getTileStruct(x, y);

	if (destDile->object == nullptr)
	{
		fastFuncts::teleportCharacter(army.character, x, y);
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
	if (bside.wonBattle != 0)//0 - lost
	{
		return;
	}

	for (int i = 0; i < bside.armiesNum; ++i)
	{
		if (bside.forces[i].army->settlement != nullptr)
		{
			continue;
		}

		RetreatArmy(bside.forces[i]);
	}
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
}
