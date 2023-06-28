#include "discordManager.h"
#include "jsonManager.h"
#include "techFuncs.h"
#include "smallFuncs.h"

namespace discordManager
{
	std::string uniStringToStr(UNICODE_STRING **&uniString)
	{
		UNICODE_STRING *uniS = *uniString;
		wchar_t *wstr = (wchar_t *)&uniS->Buffer;

		std::string strTo;
		int wchars_num = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		if (wchars_num <= 0)
		{
			return strTo;
		}
		char *szTo = new char[wchars_num];
		szTo[wchars_num - 1] = '\0';
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szTo, wchars_num, NULL, NULL);

		strTo = szTo;
		delete[] szTo;

		return strTo;
	}

	template <typename... Args>
	std::string string_format(const std::string &format, Args... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
		if (size_s <= 0)
		{
			throw std::runtime_error("Error during formatting.");
		}
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<char[]> buf(new char[size]);
		std::snprintf(buf.get(), size, format.c_str(), args...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

	void battleLoaded()
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJSONInFile(
				"\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json",
				"status",
				"Fighting a battle ⚔️");
		}
	}

	void onCampaignMapLoaded()
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJSONInFile(
				"\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json",
				"status",
				"On the campaign map 🗺️");
		}
	}

	void OnChangeTurnNum(int num)
	{
		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true)
		{
			jsonManager::setJSONInFile(
				"\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json",
				"turnNum",
				std::to_string(num));
		}
	}

	void OnFactionTurnStart(factionStruct *fac)
	{
		if (fac == nullptr)
		{
			return;
		}

		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true && fac->isPlayerControlled == true)
		{
			UNICODE_STRING **factionName = fac->localizedName;
			UNICODE_STRING *name = *factionName;
			if (name->Length == 0)
			{
				factionName = (*(*smallFuncs::getFactionName))(fac);
			}

			jsonManager::setJSONInFile(
				"\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json",
				"factionName",
				uniStringToStr(factionName));
		}
	}
}