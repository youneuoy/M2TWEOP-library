//do not include in any file except onlineThings.cpp
//you really don`t need it
#pragma once
#include <string>

#include <filesystem>
#include "realGameTypes.h"
struct armyStruct;
using namespace std;
namespace battleCreator
{
	void initStructsForResults();
	void fillStructsForResults();

	void createResultsFile();
	void writeCharactersJson(const std::string& filePath);
	void writeSettlementJson(const std::string& filePath, const std::string& lastSettlementWorldRec);

	bool transferResults(const std::string&resultsPath,int selectedWinner, bool noCasualities);
	void transferResults2();

	//create new characters here
	void onLoadCharacter(armyStruct* army, const std::filesystem::path& relativePath);
	//create settlement here
	string onLoadSettlementWord(const std::filesystem::path& relativePath);
	void clearStructs();
};

