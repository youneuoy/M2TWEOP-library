#pragma once
#include"headersSTD.h"
#include "realGameTypes.h"
namespace gameStringHelpers
{
	//set new crypted string
	//alloc memory, copy string to it
	//copy crypt to next 4 bytes after pointer
	void setHashedString(char** targetS, const char* newS);
	void setHashedString(const char** targetS, const char* newS);
	void freeHashString(stringWithHash* str);
	void setHashedStringGame(stringWithHash* str, const char* newS);
	stringWithHash* createHashedStringGame(const char* string);
	char** createHashedString(const char* string);
	std::string wstrToAnsiStr(const std::wstring& wstr);
	std::wstring ansiStrToWstr(const std::string& str);
	void createUniString(UNICODE_STRING**& newUniStringPointer, const char* nonUniStr);
	std::string uniStringToStr(UNICODE_STRING**& uniString);
};

