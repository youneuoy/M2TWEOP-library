#pragma once
#include <string>

#include "fastFunctsHelpers.h"
#include "realGameTypes.h"
#include "settlement.h"
#include "lua/sol.hpp"

#define edbEntryStruct_type 1
#define buildingLevelStruct_name 1

class eopBuildEntry
{
public:
    edbEntry baseEntry;
    eopBuildEntry(edbEntry* oldEntry, int newIndex)
    {
        baseEntry = *oldEntry;
        eopBuildID = newIndex;
    }
    int eopBuildID;
};

class buildEntryDB
{
public:
    static edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex);
    static edbEntry* getEopBuildEntry(int idx);
    static void addCaps(buildingLevel* eopLevel, const buildingLevel* oldLevel);
    static void addPools(buildingLevel* eopLevel, const buildingLevel* oldLevel);
public:
    static std::vector<eopBuildEntry> eopEdb;
};



namespace eopBuildings
{
    void setBuildingPic(edbEntry* entry, const char* newPic, int level, int cultureID);
    void setBuildingPicConstructed(edbEntry* entry, const char* newPic, int level, int cultureID);
    void setBuildingPicConstruction(edbEntry* entry, const char* newPic, int level, int cultureID);
    void setBuildingLocalizedName(edbEntry* entry, const char* newName, int level, int facnum);
    void setBuildingLocalizedDescr(edbEntry* entry, const char* newName, int level, int facnum);
    void setBuildingLocalizedDescrShort(edbEntry* entry, const char* newName, int level, int facnum);
    void addBuildingCapability(edbEntry* entry, int level, int capability, int16_t value, bool bonus);
    void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp, const char* condition);
    void removeBuildingCapability(edbEntry* entry, int level, int index);
    void removeBuildingPool(edbEntry* entry, int level, int index);
    BuildingLvlCapability* getBuildingCapability(edbEntry* entry, int level, int index);
    recruitPool* getBuildingPool(edbEntry* entry, int level, int index);
    int getBuildingCapabilityNum(edbEntry* entry, int level);
    int getBuildingPoolNum(edbEntry* entry, int level);
    void createEOPBuilding(settlementStruct* sett, int edbIdx, int level);
    void addCaps(buildingLevel* oldlevel, buildingLevel* eoplevel, int lvlidx);
    void addPools(buildingLevel* oldlevel, buildingLevel* eoplevel, int lvlidx);
    edbEntry* getBuildingByName(const char* name);
    edbEntry* getBuildingByID(int id);
    exportDescrBuildings* getEdb();
    guild* getGuild(unsigned char index);
    
#pragma region buildingLevel

    //edbEntry
    template <char fieldIndex>
    std::string getStringPropertyBDI(const buildingLevel* bInfo)
    {
        char* retS = nullptr;
        if (fieldIndex == buildingLevelStruct_name)
        {
            retS = bInfo->name;
        }

        if (retS != nullptr)
        {
            return std::string(retS);
        }
        else
        {
            return std::string("");
        }
    }

    template <char fieldIndex>
    void setStringPropertyBDI(buildingLevel* bInfo, std::string newS)
    {
        if (fieldIndex == buildingLevelStruct_name)
            fastFunctsHelpers::setCryptedString(&bInfo->name, newS.c_str());
    }

#pragma endregion
#pragma region edbEntry

    //edbEntry
    template <char fieldIndex>
    std::string getStringPropertyBD(const edbEntry* edbEntry)
    {
        char* retS = nullptr;
        if (fieldIndex == edbEntryStruct_type)
        {
            retS = edbEntry->type;
        }

        if (retS != nullptr)
        {
            return std::string(retS);
        }
        else
        {
            return std::string("");
        }
    }

    template <char fieldIndex>
    void setStringPropertyBD(edbEntry* edbEntry, std::string newS)
    {
        if (fieldIndex == edbEntryStruct_type)
            fastFunctsHelpers::setCryptedString(&edbEntry->type, newS.c_str());
    }

#pragma endregion
};

namespace buildingHelpers
{
    void addToLua(sol::state& luaState);
}