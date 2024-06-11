#include "eduThings.h"

#include "fastFunctsHelpers.h"

#include <cstdio>

#include "eduFastFuncts.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "unitActions.h"

namespace eduThings
{
	eopEduEntry::eopEduEntry(int baseIdx, int newIdx)
	{
		eduEntry* oldEn = fastFunctsHelpers::getEDUEntryById(baseIdx);
		if (oldEn == nullptr)
		{
			string errs = "Can`t create eop`s unit entry:\n";
			errs += to_string(newIdx);
			MessageBoxA(NULL, errs.c_str(), "ERROR!", NULL);
			exit(0);
		}

		data.edu = *oldEn;
		data.edu.Index = newIdx;
		originalTypeName = data.edu.Type;
		isFileAdded = false;
		eopTypeName.append(data.edu.Type);
		eopTypeName.append("_");
		eopTypeName.append(to_string(newIdx));//added to make typename unique
		fastFunctsHelpers::setCryptedString(&data.edu.Type, eopTypeName.c_str());
	}
	eopEduEntry::eopEduEntry(const char* fileName, int newIdx)
	{
		int isOk = eduFastFuncts::readEduFile(fileName, &data.edu);
		if (isOk == 0)
		{
			std::string errS = "Can`t read edu file: ";
			errS += fileName;
			MessageBoxA(NULL, errS.c_str(), "ERROR!", NULL);
			throw "not good";
		}
		data.edu.Index = newIdx;
		originalTypeName = data.edu.Type;
		isFileAdded = true;
		eopTypeName.append(data.edu.Type);
		eopTypeName.append("_");
		eopTypeName.append(to_string(newIdx));//added to make typename unique
		fastFunctsHelpers::setCryptedString(&data.edu.Type, eopTypeName.c_str());
	}
	struct eduThingsG
	{
		vector<eopEduEntry>eopEdu;
	}data;
	eduEntry* addEopEduEntryFromFile(const char* fileName, int newIdx)
	{
		if (getEopEduEntry(newIdx))
		{
			unitActions::logStringGame("Duplicate EOP index " + to_string(newIdx) + " in addEopEduEntryFromFile");
			return nullptr;
		}

		try
		{
			const eopEduEntry newEntry(fileName, newIdx);
			if (getEduEntryByType(newEntry.originalTypeName.c_str()))
			{
				unitActions::logStringGame("Duplicate unit name " + newEntry.originalTypeName + " in addEopEduEntryFromFile");
				std::string errS = "Can`t add: " + newEntry.originalTypeName + " Duplicate type name";
				MessageBoxA(NULL, errS.c_str(), "ERROR!", NULL);
				exit(0);
			}
			data.eopEdu.push_back(newEntry);
		}
		catch (...)
		{
			return nullptr;
		}
		return getEopEduEntry(newIdx);
	}
	eduEntry* addEopEduEntry(int baseIdx, int newIdx)
	{
		if (getEopEduEntry(newIdx))
		{
			unitActions::logStringGame("Duplicate EOP index " + to_string(newIdx) + " in addEopEduEntry");
			return nullptr;
		}
		eopEduEntry newEntry(baseIdx, newIdx);

		data.eopEdu.push_back(newEntry);

		return getEopEduEntry(newIdx);
	}

	eopEduEntry* getEopEduEntryInternal(int idx)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (entry.data.edu.Index == idx)
			{
				return &entry;
			}
		}
		return nullptr;
	}

	eopEduEntry* getEopEduEntryInternalIterating(int idx)
	{
		return &data.eopEdu[idx];
	}
	
	eduEntry* getEopEduEntry(int idx)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (entry.data.edu.Index == idx)
			{
				return &entry.data.edu;
			}
		}
		return nullptr;
	}
	int getEopEntryNum()
	{
		return data.eopEdu.size();
	}
	char* getEopNameOfEduEntry(eduEntry* entryAdress)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (&entry.data.edu == entryAdress)
			{
				return (char*)entry.eopTypeName.c_str();
			}
		}
		return nullptr;
	}
	int getDataEopEdu(int idx)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (entry.data.edu.Index == idx)
			{
				return (int)&entry.data;
			}
		}
		return 0;
	}
	int* tryFindDataEopEdu(char* entryName)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (strcmp(entry.eopTypeName.c_str(), entryName) == 0
				|| (entry.isFileAdded && strcmp(entry.originalTypeName.c_str(), entryName) == 0))
			{
				return (int*)&entry.data;
			}
		}
		return nullptr;
	}
	eduEntry* getEopEduEntryByName(const char* entryName)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (strcmp(entry.eopTypeName.c_str(), entryName) == 0
				|| (entry.isFileAdded && strcmp(entry.originalTypeName.c_str(), entryName) == 0))
			{
				return &entry.data.edu;
			}
		}
		return nullptr;
	}
	int* tryFindDataEopEduIndex(char* entryName)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (strcmp(entry.eopTypeName.c_str(), entryName) == 0
				|| (entry.isFileAdded && strcmp(entry.originalTypeName.c_str(), entryName) == 0))
				return reinterpret_cast<int*>(&entry.data.edu.Index);
		}
		return nullptr;
	}
	void setEntryUnitCardTga(int entryIdx, const char* newCard)
	{
		eduEntry* entry = getEopEduEntry(entryIdx);

		fastFunctsHelpers::setCryptedString(&entry->UnitCardTga, newCard);
	}
	eduEntry* getEduEntry(int Idx)
	{
		eduEntry* entry = getEopEduEntry(Idx);
		if (entry)
			return entry;
		entry = fastFunctsHelpers::getEDUEntryById(Idx);
		return entry;
	}
	void setEntryInfoCardTga(int entryIdx, const char* newCard)
	{
		eduEntry* entry = getEopEduEntry(entryIdx);

		fastFunctsHelpers::setCryptedString(&entry->InfoCardTga, newCard);
	}

	void setEntrySoldierModel(int entryIdx, const char* newModel)
	{
		eduEntry* entry = getEopEduEntry(entryIdx);

		eopEduEntry* entryInternal = getEopEduEntryInternal(entryIdx);
		entryInternal->eopSoldierString = newModel;

		entry->ModelDBEntry = fastFuncts::findBattleModel(newModel);
		entry->Soldier = (char*)entryInternal->eopSoldierString.c_str();
	}

	void setEntryLocalizedName(int entryIdx, const char* newName)
	{
		eduEntry* entry = getEopEduEntry(entryIdx);

		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
		entry->localizedName = nameMem;


		smallFuncs::createUniString(*entry->localizedName, newName);
	}

	void setEntryLocalizedDescr(int entryIdx, const char* newDesr)
	{
		eduEntry* entry = getEopEduEntry(entryIdx);

		UNICODE_STRING*** descrMem = new UNICODE_STRING * *[4];
		entry->localizedDescr = descrMem;
		smallFuncs::createUniString(*entry->localizedDescr, newDesr);
	}

	void setEntryLocalizedShortDescr(int entryIdx, const char* newDecrShort)
	{
		eduEntry* entry = getEopEduEntry(entryIdx);

		UNICODE_STRING*** shDescrMem = new UNICODE_STRING * *[4];
		entry->localizedDescrShort = shDescrMem;
		smallFuncs::createUniString(*entry->localizedDescrShort, newDecrShort);
	}
	int getEduIndexByType(const char* type)
	{
		return fastFunctsHelpers::getEduIndex(type);
	}
	eduEntry* getEduEntryByType(const char* type)
	{
		return fastFunctsHelpers::getEduEntryByName(type);
	}
};