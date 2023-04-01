#pragma once
#include "realGameTypes.h"
class Retreater
{
private:
	battleDataS* Battle;

	void RetreatArmy(armyAndCharacter& army);
	void RetreatSide(battleSide& bside);
public:
	Retreater(battleDataS* battle);
	~Retreater();

	void Retreat();
};
