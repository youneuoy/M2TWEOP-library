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
	static std::vector<std::string> splitString(const std::string& phrase, const std::string& delimiter);
	static std::string doEOPPipe(int waitSeconds, bool cleanup);
	static jsn::json loadJsonFromFile(const std::string& fpath);
	static std::string helpers::getModPathFromSharedMemory();
	static std::string helpers::getModFolderFromPipe(const std::string &msg);
};