#pragma once
#include "faction.h"
#include "realGameTypes.h"

namespace discordManager
{
	void battleLoaded();
	void onCampaignMapLoaded();
	void onFactionTurnStart(factionStruct* fac);
	void onChangeTurnNum(int num);
}