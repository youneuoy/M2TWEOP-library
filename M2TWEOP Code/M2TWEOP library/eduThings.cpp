#include "eduThings.h"

#include "fastFunctsHelpers.h"

#include <cstdio>
namespace eduThings
{
	struct eopEduEntry
	{
		eopEduEntry(int baseIdx, int newIdx)
		{
			EduEntry*oldEn=fastFunctsHelpers::getEDUEntryById(baseIdx);
			if (oldEn == nullptr)
			{
				string errs = "Can`t create eop`s unit entry:\n";
				errs += to_string(newIdx);
				MessageBoxA(NULL, errs.c_str(), "ERROR!", NULL);
				exit(0);
			}
			data.edu = *oldEn;
			data.edu.Index = newIdx;

			eopTypeName = "EOPT";
			eopTypeName.append(data.edu.Type);
		}
		eopEduEntry(const char*fileName, int newIdx)
		{
			int isOk=eduFastFuncts::readEduFile(fileName, &data.edu);
			if (isOk == 0)
			{
				std::string errS = "Can`t read edu file: ";
				errS += fileName;
				MessageBoxA(NULL, errS.c_str(), "ERROR!", NULL);
				throw "not good";
			}
			data.edu.Index = newIdx;

			eopTypeName = "EOPT";
			eopTypeName.append(data.edu.Type);
		}


		std::string eopTypeName;
		std::string eopSoldierString;
		struct
		{
			int fakeVtable = 0;
			EduEntry edu;
		}data;
	};
	struct
	{
		vector<eopEduEntry>eopEdu;
	}data;
	NOINLINE EOP_EXPORT EduEntry* addEopEduEntryFromFile(const char* fileName, int newIdx)
	{
		if (getEopEduEntry(newIdx))
		{
			return nullptr;
		}

		try 
		{
			eopEduEntry newEntry(fileName, newIdx);

			data.eopEdu.push_back(newEntry);
		}
		catch (...)
		{
			return nullptr;
		}


		eduEntryes* EDB = reinterpret_cast<eduEntryes*>(dataOffsets::offsets.unitTypesStart - 4);

		--EDB->numberOfTupes;

		return getEopEduEntry(newIdx);
	}
	NOINLINE EOP_EXPORT EduEntry* addEopEduEntry(int baseIdx , int newIdx)
	{
		if (getEopEduEntry(newIdx))
		{
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
	NOINLINE EOP_EXPORT EduEntry* getEopEduEntry(int idx)
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
	NOINLINE EOP_EXPORT char* getEopNameOfEduEntry(EduEntry* entryAdress)
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
	NOINLINE EOP_EXPORT int getDataEopEdu(int idx)
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
	NOINLINE EOP_EXPORT int* tryFindDataEopEdu(char* entryName)
	{
		for (eopEduEntry& entry : data.eopEdu)
		{
			if (strcmp(entry.eopTypeName.c_str(), entryName)==0)
			{
				return (int*)&entry.data;
			}
		}
		return nullptr;
	}
	NOINLINE EOP_EXPORT void setEntryUnitCardTga(int entryIdx, const char* newCard)
	{
		EduEntry* entry=getEopEduEntry(entryIdx);

		fastFunctsHelpers::setCryptedString(&entry->UnitCardTga, newCard);
	}
	NOINLINE EOP_EXPORT void setEntryInfoCardTga(int entryIdx, const char* newCard)
	{
		EduEntry* entry = getEopEduEntry(entryIdx);

		fastFunctsHelpers::setCryptedString(&entry->InfoCardTga, newCard);
	}

	NOINLINE EOP_EXPORT void setEntrySoldierModel(int entryIdx, const char* newModel)
	{
		EduEntry* entry = getEopEduEntry(entryIdx);

		eopEduEntry* entryInternal = getEopEduEntryInternal(entryIdx);
		entryInternal->eopSoldierString = newModel;

		entry->ModelDBEntry = fastFuncts::findBattleModel(newModel);
		entry->Soldier = (char*)entryInternal->eopSoldierString.c_str();
	}

	NOINLINE EOP_EXPORT void setEntryLocalizedName(int entryIdx, const char* newName)
	{
		EduEntry* entry = getEopEduEntry(entryIdx);

		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
		entry->localizedName =nameMem;


		smallFuncs::createUniString(*entry->localizedName, newName);
	}

	NOINLINE EOP_EXPORT void setEntryLocalizedDescr(int entryIdx, const char* newDesr)
	{
		EduEntry* entry = getEopEduEntry(entryIdx);

		UNICODE_STRING*** descrMem = new UNICODE_STRING * *[4];
		entry->localizedDescr = descrMem;
		smallFuncs::createUniString(*entry->localizedDescr, newDesr);
	}

	NOINLINE EOP_EXPORT void setEntryLocalizedShortDescr(int entryIdx, const char* newDecrShort)
	{
		EduEntry* entry = getEopEduEntry(entryIdx);

		UNICODE_STRING*** shDescrMem = new UNICODE_STRING * *[4];
		entry->localizedDescrShort = shDescrMem;
		smallFuncs::createUniString(*entry->localizedDescrShort, newDecrShort);
	}

};
