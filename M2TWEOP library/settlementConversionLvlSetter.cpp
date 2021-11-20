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
}