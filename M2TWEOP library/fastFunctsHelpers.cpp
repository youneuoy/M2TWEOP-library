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
			return nullptr;
		}
		cryptS[0] = _strdup(string);
		setCryptedString((char*)cryptS, string);
		return cryptS;
	}
}

