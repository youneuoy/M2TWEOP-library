#pragma once
#define namedChar_localizedFullName 1
#define namedChar_localizedNameForSave 2
#define namedChar_localizedNextNameForSave 3
#define namedChar_localizedNicknameForSave 4


#include <string>
#include "realGameTypes.h"


namespace technicalHelpers
{
	std::string unitUniStringToStr(unit* unit);
	void setUnitUniStr(unit* unit, const char* str);

	std::string uniStringToStr(UNICODE_STRING**& uniString);


	template <char fieldIndex>
	std::string namedCharUniStringToStr(namedCharacter* genChar)
	{
		if (fieldIndex == namedChar_localizedFullName)
		{
			return uniStringToStr(genChar->localizedFullName);
		}
		else if (fieldIndex == namedChar_localizedNameForSave)
		{
			return uniStringToStr(genChar->localizedNameForSave);

		}
		else if (fieldIndex == namedChar_localizedNextNameForSave)
		{
			return uniStringToStr(genChar->localizedNextNameForSave);
		}
		else if (fieldIndex == namedChar_localizedNicknameForSave)
		{
			return uniStringToStr(genChar->localizedNicknameForSave);
		}
		else
		{
			return "";
		}
	}


	void namedCharSetLocalizedFullName(namedCharacter* genChar, const char* str);
	void namedCharSetLocalizedNameForSave(namedCharacter* genChar, const char* str);
	void namedCharSetLocalizedNextNameForSave(namedCharacter* genChar, const char* str);
	void namedCharSetLocalizedNicknameForSave(namedCharacter* genChar, const char* str);
};

