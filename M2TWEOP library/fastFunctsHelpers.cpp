#include "fastFunctsHelpers.h"
namespace fastFunctsHelpers
{
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
}

