#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"

namespace discordManager
{
	void battleLoaded();
	void onCampaignMapLoaded();
	void OnFactionTurnStart(factionStruct*fac);
	void OnChangeTurnNum(int num);
}