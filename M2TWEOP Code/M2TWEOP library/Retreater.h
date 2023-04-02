#pragma once
#include "realGameTypes.h"
#include <vector>
class Retreater
{
private:
	battleDataS* Battle;

	void RetreatArmy(armyAndCharacter& army);
	void RetreatSide(battleSide& bside);

	void StartPostWork();
	bool isPlayerInvolved = false;
	std::vector<std::tuple<int, int, int, int>> retreats;
public:
	Retreater(battleDataS* battle);
	~Retreater();

	void Retreat();
};
