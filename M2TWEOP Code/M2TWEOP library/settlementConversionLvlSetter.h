#include "headersMEM.h"
#include "exportHeader.h"
namespace settlementConversionLvlSetter
{


	int _fastcall getConversionLvlFromCastle(int castleLvl);
	int _fastcall getConversionLvlFromCity(int cityLvl);

	void setConversionLvlFromCastle(int castleLvl, int convertToLvl);
	void setConversionLvlFromCity(int cityLvl, int convertToLvl);
}