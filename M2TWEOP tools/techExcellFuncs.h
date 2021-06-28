#pragma once
#include "headersSTD.h"

#if _DEBUG
#pragma comment(lib,"../3rd/xlsx/OpenXLSX-staticd.lib")
#else
#pragma comment(lib,"../3rd/xlsx/OpenXLSX-static.lib")
#endif
#include "../3rd/xlsx/OpenXLSX.hpp"
using namespace OpenXLSX;
namespace techExcellFuncs
{
	void makeNextCell(std::string* cell, std::string* col, int row);
    string cp1251_to_utf8(const char* str);
    
};

