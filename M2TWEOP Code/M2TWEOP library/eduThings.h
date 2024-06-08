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
#include <sstream>
#include <unordered_set>
#include <filesystem>
#include "realGameTypes.h"
#include "eduFastFuncts.h"
using namespace std;

namespace eduThings
{
	struct eopEduEntry
	{
		eopEduEntry(int baseIdx, int newIdx);
		eopEduEntry(const char* fileName, int newIdx);
		std::string eopTypeName;
		std::string originalTypeName;
		std::string eopUnitLabel;
		std::string eopSoldierString;
		bool isFileAdded = false;
		struct dataS
		{
			int fakeVtable = 0;
			eduEntry edu;
		}data;
	};
	NOINLINE EOP_EXPORT eduEntry* addEopEduEntryFromFile(const char*fileName, int newIdx);
	NOINLINE EOP_EXPORT eduEntry* addEopEduEntry(int baseIdx , int newIdx);
	NOINLINE EOP_EXPORT eduEntry* getEopEduEntry(int idx);
	NOINLINE EOP_EXPORT eduEntry* getEduEntry(int idx);
	NOINLINE EOP_EXPORT int getEopEntryNum();

	eopEduEntry* getEopEduEntryInternalIterating(int idx);
	NOINLINE EOP_EXPORT  char* getEopNameOfEduEntry(eduEntry*entryAdress);
	//used for creation of new units, etc
	NOINLINE EOP_EXPORT int getDataEopEdu(int idx);
	NOINLINE EOP_EXPORT int* tryFindDataEopEdu(char* entryName);

	NOINLINE EOP_EXPORT int* tryFindDataEopEduIndex(char* entryName);


	NOINLINE EOP_EXPORT void setEntryUnitCardTga(int entryIdx,const char*newCard);
	NOINLINE EOP_EXPORT void setEntryInfoCardTga(int entryIdx,const char*newCard);
	NOINLINE EOP_EXPORT void setEntrySoldierModel(int entryIdx,const char*newModel);
	NOINLINE EOP_EXPORT void setEntryLocalizedName(int entryIdx,const char*newName);
	NOINLINE EOP_EXPORT void setEntryLocalizedDescr(int entryIdx,const char*newDesr);
	NOINLINE EOP_EXPORT void setEntryLocalizedShortDescr(int entryIdx,const char*newDecrShort);
	NOINLINE EOP_EXPORT int getEduIndexByType(const char* type);
	NOINLINE EOP_EXPORT eduEntry* getEduEntryByType(const char* type);
};

