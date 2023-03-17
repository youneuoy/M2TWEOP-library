#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <list>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
using namespace std;



#include "json.hpp"
//json namespace

namespace jsn = nlohmann;


//macro for fast getting values from json. Need jsn::json obect, named json!
#define getJson(field,jsonName)\
field= json.at(jsonName);

//macro for fast setting values to json. Need jsn::json obect, named json!
#define setJson(jsonName,field)\
json[jsonName]=field;