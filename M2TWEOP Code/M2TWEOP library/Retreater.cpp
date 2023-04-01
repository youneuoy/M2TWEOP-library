#include "Retreater.h"
#include "PlannedRetreatRoute.h"
#include "PathMap.h"


void Retreater::RetreatArmy(armyAndCharacter& army)
{
	if (army.character == nullptr)
	{
		return;
	}

	//if route planned
	if (PlannedRetreatRoute::TryRetreatArmyWithRoute(army)==true)
	{
		return;
	}

	void* cashe = PathFinder::CreateCasheForArmy(army.army, 15);
	auto coord = PathFinder::GetSafestTileForArmyFromCashe(cashe, army.army);
	PathFinder::DeleteCasheForDistances(cashe);

	fastFuncts::teleportCharacter(army.character, coord.first, coord.second);
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
