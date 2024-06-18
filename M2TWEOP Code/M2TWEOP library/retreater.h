#pragma once
#include "realGameTypes.h"
#include <vector>
class retreater
{
private:
	battleDataS* Battle;

	void retreatArmy(battleArmy& battleArmy);
	void retreatSide(battleSide& bside);

	void startPostWork();
	bool isPlayerInvolved = false;
	std::vector<std::tuple<int, int, int, int>> retreats;
public:
	retreater(battleDataS* battle);
	~retreater();

	void retreat();
};
