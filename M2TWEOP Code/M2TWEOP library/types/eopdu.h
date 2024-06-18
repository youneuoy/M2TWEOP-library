#pragma once
#include "unit.h"


class eopEduEntry
{
public:
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
    }data{};
    void setEntrySoldierModel(const char* newModel) const;
};

class eopDu
{
public:
    static std::vector<eopEduEntry> eopUnitDb;
    static std::unordered_map<std::string, eopEduEntry*> eopUnitLookup;
    static std::unordered_map<int, eopEduEntry*> eopUnitIndexLookup;
    static eduEntry* addEopEduEntryFromFile(const char* fileName, int newIdx);
    static eduEntry* getEopEduEntry(int idx);
    static int readEduFile(const std::string& fileName, eduEntry* entryForFilling);
    static eduEntry* addEopEduEntry(int baseIdx, int newIdx);
    static eopEduEntry* getEopEduEntryInternal(int idx);
    static eopEduEntry* getEopEduEntryInternalIterating(int idx);
    static int getEopEntryNum();
    static char* getEopNameOfEduEntry(const eduEntry* entryAddress);
    static int getDataEopEdu(int idx);
    static int* tryFindDataEopEdu(const char* entryName);
    static eduEntry* getEopEduEntryByName(const char* entryName);
    static int* tryFindDataEopEduIndex(char* entryName);
    static eduEntry* getEduEntry(int idx);
    static void setEntrySoldierModelLua(int idx, const char* newModel);
};

namespace eopDuHelpers
{
	void addToLua(sol::state& luaState);
};