#include "technicalHelpers.h"

#include "FastFuncts.h"
#include "plugData.h"
#include "smallFuncs.h"

namespace technicalHelpers
{
	std::string unitUniStringToStr(unit* unit)
	{
		return uniStringToStr(unit->alias);
	}
	void setUnitUniStr(unit* unit, const char* str)
	{
		smallFuncs::createUniString(unit->alias, str);
	}
	std::string uniStringToStr(UNICODE_STRING**& uniString)
	{
		UNICODE_STRING* uniS = *uniString;
		wchar_t* wstr = (wchar_t*)&uniS->Buffer;

		std::string strTo;
		int wchars_num = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		if (wchars_num <= 0)
		{
			return strTo;
		}
		char* szTo = new char[wchars_num];
		szTo[wchars_num-1] = '\0';
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szTo, wchars_num, NULL, NULL);

		strTo = szTo;
		delete[] szTo;

		return strTo;
	}

	void namedCharSetLocalizedFullName(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedFullName, str);
	}

	void namedCharSetLocalizedNameForSave(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedNameForSave, str);
	}

	void namedCharSetLocalizedNextNameForSave(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedNextNameForSave, str);
	}

	void namedCharSetLocalizedNicknameForSave(characterRecord* genChar, const char* str)
	{
		smallFuncs::createUniString(genChar->localizedNicknameForSave, str);
	}

};

