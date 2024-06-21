#include "discordManager.h"

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
				"\\eopData\\config\\discordRichPresenceCfg.json",
				"status",
				"Fighting a battle ⚔️");
		}
	}

	void onCampaignMapLoaded()
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJsonInFile(
				"\\eopData\\config\\discordRichPresenceCfg.json",
				"status",
				"On the campaign map 🗺️");
		}
	}

	void onChangeTurnNum(const int num)
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJsonInFile(
				"\\eopData\\config\\discordRichPresenceCfg.json",
				"turnNum",
				std::to_string(num));
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
				"\\eopData\\config\\discordRichPresenceCfg.json",
				"factionName",
				name);
		}
	}
}