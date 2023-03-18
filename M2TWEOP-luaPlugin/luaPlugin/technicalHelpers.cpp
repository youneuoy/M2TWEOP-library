#include "technicalHelpers.h"

#include "plugData.h"

namespace technicalHelpers
{
	std::string unitUniStringToStr(unit* unit)
	{
		return uniStringToStr(unit->alias);
	}
	void setUnitUniStr(unit* unit, const char* str)
	{
		(*(*plugData::data.funcsTech.createUniString))(unit->alias, str);
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
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szTo, (int)uniS->Length, NULL, NULL);

		strTo = szTo;
		delete[] szTo;

		return strTo;
	}

	void namedCharSetLocalizedFullName(generalCharacterictics* genChar, const char* str)
	{
		(*(*plugData::data.funcsTech.createUniString))(genChar->localizedFullName, str);
	}

	void namedCharSetLocalizedNameForSave(generalCharacterictics* genChar, const char* str)
	{
		(*(*plugData::data.funcsTech.createUniString))(genChar->localizedNameForSave, str);
	}

	void namedCharSetLocalizedNextNameForSave(generalCharacterictics* genChar, const char* str)
	{
		(*(*plugData::data.funcsTech.createUniString))(genChar->localizedNextNameForSave, str);
	}

	void namedCharSetLocalizedNicknameForSave(generalCharacterictics* genChar, const char* str)
	{
		(*(*plugData::data.funcsTech.createUniString))(genChar->localizedNicknameForSave, str);
	}
	void createUniString(UNICODE_STRING** &newUniStringPointer, const char* nonUniStr)
	{
		(*(*plugData::data.funcsTech.createUniString))(newUniStringPointer, nonUniStr);
	}

};

