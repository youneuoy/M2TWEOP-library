#include "discordManager.h"

namespace discordManager
{
    void OnFactionTurnStart(factionStruct* fac)
    {
        if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true) {
			setJSONInFile(
				"\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json",
				"factionName",
				techFuncs::uniToANSI(fac->localizedName);
			)
		}
    }
}