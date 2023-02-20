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
#include "fastFuncts.h"
using namespace std;


namespace eopBuildings
{
    NOINLINE EOP_EXPORT edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex);
    NOINLINE EOP_EXPORT edbEntry* getEopBuildEntry(int idx);
    NOINLINE EOP_EXPORT void setBuildingPic(edbEntry* entry, const char* newPic, int level, int cultureID);
    NOINLINE EOP_EXPORT void setBuildingPicConstructed(edbEntry* entry, const char* newPic, int level, int cultureID);
    NOINLINE EOP_EXPORT void setBuildingPicConstruction(edbEntry* entry, const char* newPic, int level, int cultureID);
    NOINLINE EOP_EXPORT void setBuildingLocalizedName(edbEntry* entry, const char* newName, int level, int facnum);
    NOINLINE EOP_EXPORT void setBuildingLocalizedDescr(edbEntry* entry, const char* newName, int level, int facnum);
    NOINLINE EOP_EXPORT void setBuildingLocalizedDescrShort(edbEntry* entry, const char* newName, int level, int facnum);
    NOINLINE EOP_EXPORT void addBuildingCapability(edbEntry* entry, int level, int capability, int16_t value, bool bonus);
    NOINLINE EOP_EXPORT void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp);
    NOINLINE EOP_EXPORT void removeBuildingCapability(edbEntry* entry, int level, int index);
    NOINLINE EOP_EXPORT void removeBuildingPool(edbEntry* entry, int level, int index);
    NOINLINE EOP_EXPORT BuildingLvlCapability* getBuildingCapability(edbEntry* entry, int level, int index);
    NOINLINE EOP_EXPORT recruitPool* getBuildingPool(edbEntry* entry, int level, int index);
    NOINLINE EOP_EXPORT int getBuildingCapabilityNum(edbEntry* entry, int level);
    NOINLINE EOP_EXPORT int getBuildingPoolNum(edbEntry* entry, int level);
    NOINLINE EOP_EXPORT void createEOPBuilding(settlementStruct* sett, int edbIdx, int level);
    NOINLINE EOP_EXPORT void addCaps(buildingLevel* oldlevel, buildingLevel* eoplevel, int lvlidx);
    NOINLINE EOP_EXPORT void addPools(buildingLevel* oldlevel, buildingLevel* eoplevel, int lvlidx);
    NOINLINE EOP_EXPORT edbEntry* getBuildingByName(const char* name);
};