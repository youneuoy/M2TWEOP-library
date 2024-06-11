#pragma once
#include "realGameTypes.h"

namespace eopBuildings
{
    edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex);
    edbEntry* getEopBuildEntry(int idx);
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
};