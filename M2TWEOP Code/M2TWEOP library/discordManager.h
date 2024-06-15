#pragma once
#include "realGameTypes.h"

namespace discordManager
{
	void battleLoaded();
	void onCampaignMapLoaded();
	void onFactionTurnStart(factionStruct*fac);
	void onChangeTurnNum(int num);
}