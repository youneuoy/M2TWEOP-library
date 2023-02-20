#include "eopBuildings.h"

#include "fastFunctsHelpers.h"
#include "fastFuncts.h"

#include <cstdio>
namespace eopBuildings
{
    struct eopBuildEntry
    {
		struct dataS
		{
			int fakeVtable = 0;
			edbEntry edb;
		}data;

        eopBuildEntry(edbEntry* oldEntry, int newIndex)
        {
			data.edb = *oldEntry;
			eopBuildID = newIndex;
            
        }
		int eopBuildID;
    };
	struct buildEntryDB
	{
		vector<eopBuildEntry>eopEdb;
	}data;

	NOINLINE EOP_EXPORT edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex)
	{
		if (getEopBuildEntry(newIndex)) //dont overwrite existing entries
		{
			return nullptr;
		}
		eopBuildEntry newEntry(oldEntry, newIndex);

		data.eopEdb.push_back(newEntry);
		edbEntry* eopentry = getEopBuildEntry(newIndex);
		int32_t levelsnum = eopentry->buildingLevelCount; //because levels can be unlocked to 57 needs to be dynamic not 9
		buildingLevel* eoplevel = new buildingLevel[levelsnum];
		//making a complete copy of the building
        for (int i = 0; i < levelsnum; i++)
        {
			eoplevel[i] = oldEntry->buildingLevel[i];
            addCaps(eoplevel, oldEntry->buildingLevel, i);
            addPools(eoplevel, oldEntry->buildingLevel, i);
        }
		eopentry->buildingLevel = eoplevel;

		return eopentry;
	}

    NOINLINE EOP_EXPORT void addCaps(buildingLevel* eoplevel, buildingLevel* oldlevel, int lvlidx)
    {
		if (oldlevel->capabilities == nullptr)
		{
			return;
		}
        BuildingLvlCapability* eopcap = new BuildingLvlCapability;
		eoplevel->capabilities = eopcap;
        BuildingLvlCapability* oldcap = oldlevel->capabilities;
        *eopcap = *oldcap;
        oldcap = oldcap->nextCapability;

        while (oldcap != nullptr)
        { 
            eopcap->nextCapability = new BuildingLvlCapability;
            eopcap = eopcap->nextCapability;
            *eopcap = *oldcap;
            oldcap = oldcap->nextCapability;
        }

    }

    NOINLINE EOP_EXPORT void addPools(buildingLevel* eoplevel, buildingLevel* oldlevel, int lvlidx)
    {
		if (oldlevel->recruitPools == nullptr)
		{
			return;
		}
        recruitPool* eoppool = new recruitPool;
		eoplevel->recruitPools = eoppool;
        recruitPool* oldpool = oldlevel->recruitPools;
        *eoppool = *oldpool;
        oldpool = oldpool->nextPool;

        while (oldpool != nullptr)
        { 
            eoppool->nextPool = new recruitPool;
            eoppool = eoppool->nextPool;
            *eoppool = *oldpool;
            oldpool = oldpool->nextPool;
        }

    }

	//get data from an existing eop build entry, in edbentry format
	NOINLINE EOP_EXPORT edbEntry* getEopBuildEntry(int idx)
	{
		for (eopBuildEntry& entry : data.eopEdb)
		{
			if (entry.eopBuildID == idx)
			{
				return &entry.data.edb;
			}
		}
		return nullptr;
	}

	//unique per culture!
	NOINLINE EOP_EXPORT void setBuildingPic(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
        buildingLevel* eoplevel = &entry->buildingLevel[level];

		fastFunctsHelpers::setCryptedString(&eoplevel->buildingPic[cultureID].buildingPicPath, newPic);
	}

	//unique per culture!
	NOINLINE EOP_EXPORT void setBuildingPicConstructed(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];

		fastFunctsHelpers::setCryptedString(&eoplevel->buildingPicConstructed[cultureID].buildingPicPath, newPic);
	}

	//unique per culture!
	NOINLINE EOP_EXPORT void setBuildingPicConstruction(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];

		fastFunctsHelpers::setCryptedString(&eoplevel->buildingPicConstruction[cultureID].buildingPicPath, newPic);
	}

	//unique per faction!
	NOINLINE EOP_EXPORT void setBuildingLocalizedName(edbEntry* entry, const char* newName, int level, int facnum)
	{
		UNICODE_STRING*** nameMem = new UNICODE_STRING**;

        buildingLevel* eoplevel = &entry->buildingLevel[level];
		eoplevel->buildingName[facnum] = nameMem;

		smallFuncs::createUniString(*eoplevel->buildingName[facnum], newName);
	}

	//unique per faction!
	NOINLINE EOP_EXPORT void setBuildingLocalizedDescr(edbEntry* entry, const char* newName, int level, int facnum)
	{
		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
        buildingLevel* eoplevel = &entry->buildingLevel[level];
		eoplevel->buildingDescr[facnum] = nameMem;


		smallFuncs::createUniString(*eoplevel->buildingDescr[facnum], newName);
	}

	//unique per faction!
	NOINLINE EOP_EXPORT void setBuildingLocalizedDescrShort(edbEntry* entry, const char* newName, int level, int facnum)
	{
		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
        buildingLevel* eoplevel = &entry->buildingLevel[level];
		eoplevel->buildingDescrShort[facnum] = nameMem;


		smallFuncs::createUniString(*eoplevel->buildingDescrShort[facnum], newName);
	}
	
	//add new building capability, bonus refers to bonus keyboard in edb
	NOINLINE EOP_EXPORT void addBuildingCapability(edbEntry* entry, int level, int capability, int16_t value, bool bonus)
	{
		DWORD funcPointer = (DWORD)0x008A955B; //I dont think this does anything but not 100% sure, havent checked disk version as I do not think this is needed
		DWORD EDBpointer = dataOffsets::offsets.edbDataStart; //for some reason this is included, not sure if needed
		BuildingLvlCapability* cap = new BuildingLvlCapability; //allocating memory
        buildingLevel* eoplevel = &entry->buildingLevel[level];
		cap->capabilityType = 0; //always 0 for normal capabilities
		if (bonus)
		{
			cap->bonus = value;
			cap->capabilityLvl = -1;
		}
		else
		{
			cap->bonus = 0;
			cap->capabilityLvl = value;
		}
		cap->capabilityID = capability;
		cap->funcPointer = funcPointer;
		cap->buildingLevel = eoplevel;
		cap->EDBpointer = EDBpointer;
		cap->buildingLevelCondition = nullptr;
		cap->nextCapability = nullptr;
		if (eoplevel->capabilities != nullptr)
		{
			cap->nextCapability = eoplevel->capabilities; //always inserting at start of capability linked list
		}
		eoplevel->capabilities = cap;

	}
	
	NOINLINE EOP_EXPORT void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp)
	{
		recruitPool* pool = new recruitPool;
        buildingLevel* eoplevel = &entry->buildingLevel[level];
		pool->capabilityType = 5; //5 means normal unit, there are some other for agents I havent added yet
		pool->capabilityLvlorExp = exp; //for units this always is xp, for agents this can be agent
		pool->unitID = eduIndex;
		pool->initialSize = initialSize;
		pool->gainPerTurn = gainPerTurn;
		pool->maxSize = maxSize;
		pool->buildingLevelCondition = nullptr;
		pool->nextPool = nullptr;
		if (eoplevel->recruitPools != nullptr)
		{
			pool->nextPool = eoplevel->recruitPools; //always insert at start of pools
		}
		eoplevel->recruitPools = pool;
	}


	NOINLINE EOP_EXPORT void removeBuildingCapability(edbEntry* entry, int level, int index)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		BuildingLvlCapability* cap = eoplevel->capabilities;
		int capNum = getBuildingCapabilityNum(entry, level);
		if (index >= capNum || cap == nullptr)
		{
			return;
		}
		if (index == 0)
		{	
			eoplevel->capabilities = nullptr;
			if (cap->nextCapability != nullptr)
			{
				eoplevel->capabilities = cap->nextCapability;
			}
			return;
		}
		int i = 0;
		BuildingLvlCapability* prevcap = cap;
        while (cap != nullptr)
		{
			i++;
			prevcap = cap;
			cap = cap->nextCapability;
			if(index = i)
			{
				if (cap->nextCapability != nullptr)
				{
					prevcap->nextCapability = cap->nextCapability;
				}
				else
				{
					prevcap->nextCapability = nullptr;
				}
				cap = nullptr;
				return;
			}
		}
	}

	NOINLINE EOP_EXPORT void removeBuildingPool(edbEntry* entry, int level, int index)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		recruitPool* pool = eoplevel->recruitPools;
		int poolNum = getBuildingPoolNum(entry, level);
		if (index >= poolNum || pool == nullptr)
		{
			return;
		}
		if (index == 0)
		{
			eoplevel->recruitPools = nullptr;
			if (pool->nextPool != nullptr)
			{
				eoplevel->recruitPools = pool->nextPool;
			}
			return;
		}
		int i = 0;
		recruitPool* prevpool = pool;
        while (pool != nullptr)
		{
			i++;
			prevpool = pool;
			pool = pool->nextPool;
			if(index = i)
			{
				if (pool->nextPool != nullptr)
				{
					prevpool->nextPool = pool->nextPool;
				}
				else
				{
					prevpool->nextPool = nullptr;
				}
				pool = nullptr;
				return;	
			}
		}
	}
	
	//get a capability to change some of its attributes or check them, like to remove it
	NOINLINE EOP_EXPORT BuildingLvlCapability* getBuildingCapability(edbEntry* entry, int level, int index)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		BuildingLvlCapability* cap = eoplevel->capabilities;
		int capNum = getBuildingCapabilityNum(entry, level);
		if (index >= capNum || cap == nullptr)
		{
			return nullptr;
		}
		if (index == 0) //just return first one
		{
			return cap;
		}
		int i = 0;
        while (cap != nullptr)
		{
				cap = cap->nextCapability;
				i++;
				if(index = i)
				{
					return cap;
				}
		}
		return nullptr;
	}
	
	NOINLINE EOP_EXPORT recruitPool* getBuildingPool(edbEntry* entry, int level, int index)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		recruitPool* pool = eoplevel->recruitPools;
		int poolNum = getBuildingPoolNum(entry, level);
		if (index >= poolNum || pool == nullptr)
		{
			return nullptr;
		}
		if (index == 0)
		{
			return pool;
		}
		int i = 0;
        while (pool != nullptr)
		{
				pool = pool->nextPool;
				i++;
				if(index = i)
				{
					return pool;
				}
		}
		return nullptr;
	}
	
	//get amount of capabilities, useful for iteration
	NOINLINE EOP_EXPORT int getBuildingCapabilityNum(edbEntry* entry, int level)
	{
		if (entry == nullptr)
		{
			return 0;
		}
        buildingLevel* eoplevel = &entry->buildingLevel[level];
		BuildingLvlCapability* cap = eoplevel->capabilities;
		if (cap == nullptr)
		{
			return 0;
		}
		BuildingLvlCapability* nextcap = cap->nextCapability;
		int capNum = 1;
        while (nextcap != nullptr)
        { 
			nextcap = nextcap->nextCapability;
			capNum++;
		}
		return capNum;
	}

	//get amount of pools, useful for iteration
	NOINLINE EOP_EXPORT int getBuildingPoolNum(edbEntry* entry, int level)
	{
		if (entry == nullptr)
		{
			return 0;
		}
        buildingLevel* eoplevel = &entry->buildingLevel[level];
		recruitPool* pool = eoplevel->recruitPools;
		if (pool == nullptr)
		{
			return 0;
		}
		recruitPool* nextpool = pool->nextPool;
		int poolNum = 1;
        while (nextpool != nullptr)
        { 
			nextpool = nextpool->nextPool;
			poolNum++;
		}
		return poolNum;
	}
	
	//not sure how useful this function is, basically just combines creating a building with switching its entry to eop building
	NOINLINE EOP_EXPORT void createEOPBuilding(settlementStruct* sett, int edbIdx, int level)
	{
		edbEntry* entry = getEopBuildEntry(edbIdx);
        buildingLevel* eoplevel = &entry->buildingLevel[level];
        const char* building_level_id = (const char*)(eoplevel->name);
        fastFuncts::createBuilding(sett, building_level_id);
        sett->buildings[sett->buildingsNum-1]->edbEntry = entry;
	}
	
	//get a building entry by its name, very useful to change some of the edb building attributes dynamically, does not save!
	NOINLINE EOP_EXPORT edbEntry* getBuildingByName(const char* name)
	{
		exportDescrBuildings* edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
		buildingListPointer* buildingsptr = &edb->buildingsList;
		int buildingNum = 0;
		while (buildingsptr != nullptr)
		{
			buildingNum = buildingsptr->arrayCount;
			for (int i = 0; i < buildingNum; i++)
			{
				if (strcmp(buildingsptr->buildingsArray->buildings[i].type, name) == 0)
				{
					return &buildingsptr->buildingsArray->buildings[i];
				}
			}
			buildingsptr = buildingsptr->nextBuildingsListPointer;
		}
		return nullptr;
	}

};