#pragma once
#include"headersSTD.h"
#include "realGameTypes.h"
namespace fastFunctsHelpers
{
	//set new crypted string
	//alloc memory, copy string to it
	//copy crypt to next 4 bytes after pointer
	void setCryptedString(char** targetS, const char* newS);

	int getEduIndex(const char* type);
	eduEntry* getEduEntryByName(const char* type);
	eduEntry* getEDUEntryById(int id);

	char** makeCryptedString(const char* string);

	std::string wstrToANSIStr(const std::wstring& wstr);
	std::wstring ANSIStrToWstr(const std::string& str);

	jsn::json loadJsonFromFile(const std::string& fpath);
	jsn::json loadJsonFromFile(const std::wstring& fpath);
	void writeJsonToFile(const std::string& fpath,const jsn::json& json);
};

