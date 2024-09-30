#include "pch.h"
#include "discordManager.h"

#include "campaign.h"
#include "globals.h"
#include "jsonManager.h"
#include "techFuncs.h"
#include "faction.h"

namespace discordManager
{
	template <typename... Args>
	std::string string_format(const std::string &format, Args... args)
	{
		const int sizeS = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
		if (sizeS <= 0)
		{
			throw std::runtime_error("Error during formatting.");
		}
		const auto size = static_cast<size_t>(sizeS);
		const std::unique_ptr<char[]> buf(new char[size]);
		std::snprintf(buf.get(), size, format.c_str(), args...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

	void battleLoaded()
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"status",
				"Fighting a battle ⚔️");
		}
	}

	void menuLoaded()
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"status",
				"At the main menu 🏰");
			
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"factionName",
				"");
			
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"turnNum",
				"0");
		}
	}

	void onCampaignMapLoaded()
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"status",
				"On the campaign map 🗺️");
			if (const auto campaignData = campaignHelpers::getCampaignData())
			{
				jsonManager::setJsonInFile(
					R"(\eopData\config\discordRichPresenceCfg.json)",
					"turnNum",
					std::to_string(campaignData->turnNumber + 1));
			}
		}
	}

	void onChangeTurnNum(const int num)
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"turnNum",
				std::to_string(num + 1));
		}
	}

	void onFactionTurnStart(factionStruct *fac)
	{
		if (fac == nullptr)
			return;
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true && fac->isPlayerControlled == 1)
		{
			const auto name = factionHelpers::getLocalizedFactionName(fac);
			jsonManager::setJsonInFile(
				R"(\eopData\config\discordRichPresenceCfg.json)",
				"factionName",
				name);
		}
	}
}