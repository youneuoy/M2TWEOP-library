#include "discordManager.h"
#include "jsonManager.h"
#include "techFuncs.h"

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

	void OnFactionTurnStart(factionStruct *fac)
	{
		if (fac == nullptr)
		{
			return;
		}

		if (globals::dataS.gameCfg.isDiscordRichPresenceEnabled == true && fac->isPlayerControlled == true && factionName.length() > 0)
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