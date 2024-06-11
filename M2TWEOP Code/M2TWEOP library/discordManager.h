#pragma once
#include "realGameTypes.h"

namespace discordManager
{
	void battleLoaded();
	void onCampaignMapLoaded();
	void OnFactionTurnStart(factionStruct*fac);
	void OnChangeTurnNum(int num);
}