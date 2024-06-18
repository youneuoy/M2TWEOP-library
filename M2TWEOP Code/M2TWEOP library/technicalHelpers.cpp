#include "technicalHelpers.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "unit.h"

namespace technicalHelpers
{
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

};

