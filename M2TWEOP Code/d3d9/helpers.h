#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include "json.hpp"

// Json namespace
namespace jsn = nlohmann;

//macro for fast getting values from json. Need jsn::json obect, named json!
#define getJson(field,jsonName)\
field= json.at(jsonName);

//macro for fast setting values to json. Need jsn::json obect, named json!
#define setJson(jsonName,field)\
json[jsonName]=field;


class helpers
{
public:
	static std::vector<std::wstring> splitString(const std::wstring& phrase, const std::wstring& delimiter);
	static std::wstring doEOPPipe(int waitSeconds, bool cleanup);
	static jsn::json loadJsonFromFile(const std::wstring& fpath);
	static std::wstring helpers::getModPathFromSharedMemory();
	static std::wstring helpers::getModFolderFromPipe(const std::wstring &msg);
};