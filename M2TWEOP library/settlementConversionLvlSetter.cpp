#include "settlementConversionLvlSetter.h"


namespace settlementConversionLvlSetter 
{
	int lvlFromCastle[6] = { 0,1,2,3,3,6 };
	int lvlFromCity[6] = { 0,1,2,6,6,6 };

	int _fastcall getConversionLvlFromCastle(int castleLvl)
	{
		return lvlFromCastle[castleLvl];
	}

	int _fastcall getConversionLvlFromCity(int cityLvl)
	{
		return lvlFromCity[cityLvl];
	}

	NOINLINE EOP_EXPORT void setConversionLvlFromCastle(int castleLvl, int convertToLvl)
	{
		if (castleLvl < 0 || castleLvl > 6 || convertToLvl < 0 || convertToLvl > 6)
		{
			return;
		}
		else
		{
			lvlFromCastle[castleLvl] = convertToLvl;
		}
	}

	NOINLINE EOP_EXPORT void setConversionLvlFromCity(int cityLvl, int convertToLvl)
	{
		if (cityLvl < 0 || cityLvl > 6 || convertToLvl < 0 || convertToLvl > 6)
		{
			return;
		}
		else
		{
			lvlFromCity[cityLvl] = convertToLvl;
		}
	}
}