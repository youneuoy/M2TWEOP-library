//do not include in any file except onlineThings.cpp
//you really don`t need it
#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "eventsCodes.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "imgui.h"
#include <d3d9.h>
#include <sstream>
#include <thread>
#include <set>
#include <map>
#include <filesystem>
#include "realGameTypes.h"
using namespace std;
namespace battleCreator
{
	void initStructsForResults();

	void createResultsFile();
	void writeCharactersJson(const std::string& filePath);
	void writeSettlementJson(const std::string& filePath, const std::string& lastSettlementWorldRec);

	bool transferResults(const std::string&resultsPath,int selectedWinner, bool noCasualities);
	void transferResults2();

	//create new characters here
	void onLoadCharacter(stackStruct* army, const std::filesystem::path& relativePath);


	//create settlement here
	string onLoadSettlementWord(const std::filesystem::path& relativePath);

	void clearStructs();
};

