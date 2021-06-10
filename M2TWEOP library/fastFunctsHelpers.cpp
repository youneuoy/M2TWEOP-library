#include "fastFunctsHelpers.h"
namespace fastFunctsHelpers
{
	NOINLINE EOP_EXPORT void setCryptedString(char* targetS, const char* newS)
	{
		UINT32 functionOffset = codes::offsets.stringCryptFunc;
		_asm {
			push newS
			mov ecx, targetS
			mov eax, functionOffset
			call eax
		}
	}
	int getEDUIndex(const char* type)
	{
		eduEntryes* EDB = reinterpret_cast<eduEntryes*>(dataOffsets::offsets.unitTypesStart - 4);

		int unitsNum = EDB->numberOfTupes;
		for (int i = 0; i < unitsNum; i++)
		{
			if (strcmp(EDB->unitTupes[i].Type, type) == 0)
			{
				return i;
			}
		}
		return -1;
	}
	char** makeCryptedString(const char* string)
	{
		char** cryptS = (char**)malloc(8);
		if (cryptS == nullptr)
		{
			MessageBoxA(NULL, "Cannot allocate memory for m2tweop", "ERROR!", NULL);
			exit(0);
			return nullptr;
		}
		memset(cryptS, 0, 8);
		//cryptS[0] = _strdup(string);
		setCryptedString((char*)cryptS, string);
		return cryptS;
	}
	std::string wstrToANSIStr(const std::wstring& wstr)
	{		if (wstr.empty()) return std::string();

		std::string strTo;
		char* szTo = new char[wstr.length() + 1];
		szTo[wstr.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);

		strTo = szTo;
		delete[] szTo;

		return strTo;

	}
	std::wstring ANSIStrToWstr(const std::string& str)
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}
}

