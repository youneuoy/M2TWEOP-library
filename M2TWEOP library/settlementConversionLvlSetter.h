#include "headersMEM.h"
#include "exportHeader.h"
namespace settlementConversionLvlSetter
{


	int _fastcall getConversionLvlFromCastle(int castleLvl);
	int _fastcall getConversionLvlFromCity(int cityLvl);

	NOINLINE EOP_EXPORT void setConversionLvlFromCastle(int castleLvl, int convertToLvl);
	NOINLINE EOP_EXPORT void setConversionLvlFromCity(int cityLvl, int convertToLvl);
}