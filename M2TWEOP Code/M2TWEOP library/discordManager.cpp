#include "discordManager.h"
#include "jsonManager.h"
#include "techFuncs.h"

namespace discordManager
{

    void OnFactionTurnStart(factionStruct* fac)
    {
		if (fac == nullptr)
		{
			return;
		}

		std::string factionName = techFuncs::uniToANSI(fac->localizedName);

        if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true && fac->isPlayerControlled == true && factionName.length() > 0) {
			jsonManager::setJSONInFile(
				"\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json",
				"factionName",
				factionName
			);
		}
    }
}