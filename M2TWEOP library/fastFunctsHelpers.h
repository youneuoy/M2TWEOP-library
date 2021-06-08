#pragma once
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include"headersSTD.h"
#include "headersMEM.h"
#include "realGameTypes.h"
#include "globals.h"
#include "exportHeader.h"
namespace fastFunctsHelpers
{
	//set new crypted string
	//alloc memory, copy string to it
	//copy crypt to next 4 bytes after pointer
	NOINLINE EOP_EXPORT void setCryptedString(char* targetS, const char* newS);

	int getEDUIndex(const char* type);

	char** makeCryptedString(const char* string);

	std::string wstrToANSIStr(const std::wstring& wstr);
	std::wstring ANSIStrToWstr(const std::string& str);
};

