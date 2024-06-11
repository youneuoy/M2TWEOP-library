#pragma once
#include <string>
#include <filesystem>
#include "realGameTypes.h"
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
	eduEntry* addEopEduEntryFromFile(const char*fileName, int newIdx);
	eduEntry* addEopEduEntry(int baseIdx , int newIdx);
	eduEntry* getEopEduEntry(int idx);
	eduEntry* getEduEntry(int idx);
	int getEopEntryNum();
	eduEntry* getEopEduEntryByName(const char* entryName);

	eopEduEntry* getEopEduEntryInternalIterating(int idx);
	 char* getEopNameOfEduEntry(eduEntry*entryAdress);
	//used for creation of new units, etc
	int getDataEopEdu(int idx);
	int* tryFindDataEopEdu(char* entryName);

	int* tryFindDataEopEduIndex(char* entryName);


	void setEntryUnitCardTga(int entryIdx,const char*newCard);
	void setEntryInfoCardTga(int entryIdx,const char*newCard);
	void setEntrySoldierModel(int entryIdx,const char*newModel);
	void setEntryLocalizedName(int entryIdx,const char*newName);
	void setEntryLocalizedDescr(int entryIdx,const char*newDesr);
	void setEntryLocalizedShortDescr(int entryIdx,const char*newDecrShort);
	int getEduIndexByType(const char* type);
	eduEntry* getEduEntryByType(const char* type);
};

