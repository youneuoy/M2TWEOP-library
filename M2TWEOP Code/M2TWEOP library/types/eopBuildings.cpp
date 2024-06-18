#include "eopBuildings.h"

#include "fastFunctsHelpers.h"
#include "smallFuncs.h"
#include "settlement.h"

#include <cstdio>

#include "dataOffsets.h"
#include "functionsOffsets.h"
#include "techFuncs.h"

std::vector<eopBuildEntry> buildEntryDB::eopEdb = {};

edbEntry* buildEntryDB::addEopBuildEntry(edbEntry* oldEntry, const int newIndex)
{
	if (getEopBuildEntry(newIndex)) //dont overwrite existing entries
		return nullptr;

	const eopBuildEntry newEntry(oldEntry, newIndex);
	eopEdb.push_back(newEntry);
	edbEntry* entry = getEopBuildEntry(newIndex);
	const int32_t levelNum = entry->buildingLevelCount; //because levels can be unlocked to 57 needs to be dynamic not 9
	buildingLevel* eopLevel = techFuncs::allocateGameClass<buildingLevel>(sizeof(buildingLevel) * levelNum);
	//making a complete copy of the building
	for (int i = 0; i < levelNum; i++)
	{
		eopLevel[i] = oldEntry->buildingLevel[i];
		addCaps(eopLevel, oldEntry->buildingLevel);
		addPools(eopLevel, oldEntry->buildingLevel);
	}
	entry->buildingLevel = eopLevel;
	return entry;
}

//get data from an existing eop build entry, in edbentry format
edbEntry* buildEntryDB::getEopBuildEntry(const int idx)
{
	for (eopBuildEntry& entry : eopEdb)
	{
		if (entry.eopBuildID == idx)
		{
			return &entry.baseEntry;
		}
	}
	return eopBuildings::getBuildingByID(idx);
}

void buildEntryDB::addCaps(buildingLevel* eopLevel, const buildingLevel* oldLevel)
{
	if (!oldLevel->capabilities)
		return;
	auto eopCap = techFuncs::allocateGameClass<BuildingLvlCapability>(sizeof(BuildingLvlCapability));
	eopLevel->capabilities = eopCap;
	const BuildingLvlCapability* oldCap = oldLevel->capabilities;
	*eopCap = *oldCap;
	oldCap = oldCap->nextCapability;
	while (oldCap != nullptr)
	{
		eopCap->nextCapability = techFuncs::allocateGameClass<BuildingLvlCapability>(sizeof(BuildingLvlCapability));
		eopCap = eopCap->nextCapability;
		*eopCap = *oldCap;
		oldCap = oldCap->nextCapability;
	}
}

void buildEntryDB::addPools(buildingLevel* eopLevel, const buildingLevel* oldLevel)
{
	if (!oldLevel->recruitPools)
		return;
	auto eopPool = techFuncs::allocateGameClass<recruitPool>(sizeof(recruitPool));
	eopLevel->recruitPools = eopPool;
	const recruitPool* oldPool = oldLevel->recruitPools;
	*eopPool = *oldPool;
	oldPool = oldPool->nextPool;
	while (oldPool != nullptr)
	{
		eopPool->nextPool = techFuncs::allocateGameClass<recruitPool>(sizeof(recruitPool));
		eopPool = eopPool->nextPool;
		*eopPool = *oldPool;
		oldPool = oldPool->nextPool;
	}
}

namespace eopBuildings
{

	//unique per culture!
	void setBuildingPic(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		fastFunctsHelpers::setCryptedString(&eoplevel->buildingPic[cultureID].buildingPicPath, newPic);
	}

	//unique per culture!
	void setBuildingPicConstructed(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];

		fastFunctsHelpers::setCryptedString(&eoplevel->buildingPicConstructed[cultureID].buildingPicPath, newPic);
	}

	//unique per culture!
	void setBuildingPicConstruction(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
		buildingLevel* eoplevel = &entry->buildingLevel[level];

		fastFunctsHelpers::setCryptedString(&eoplevel->buildingPicConstruction[cultureID].buildingPicPath, newPic);
	}

	//unique per faction!
	void setBuildingLocalizedName(edbEntry* entry, const char* newName, int level, int facnum)
	{
		UNICODE_STRING*** nameMem = new UNICODE_STRING**;

		buildingLevel* eoplevel = &entry->buildingLevel[level];
		eoplevel->buildingName[facnum] = nameMem;

		smallFuncs::createUniString(*eoplevel->buildingName[facnum], newName);
	}

	//unique per faction!
	void setBuildingLocalizedDescr(edbEntry* entry, const char* newName, int level, int facnum)
	{
		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		eoplevel->buildingDescr[facnum] = nameMem;


		smallFuncs::createUniString(*eoplevel->buildingDescr[facnum], newName);
	}

	//unique per faction!
	void setBuildingLocalizedDescrShort(edbEntry* entry, const char* newName, int level, int facnum)
	{
		UNICODE_STRING*** nameMem = new UNICODE_STRING**;
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		eoplevel->buildingDescrShort[facnum] = nameMem;


		smallFuncs::createUniString(*eoplevel->buildingDescrShort[facnum], newName);
	}

	//add new building capability, bonus refers to bonus keyboard in edb
	void addBuildingCapability(edbEntry* entry, int level, int capability, int16_t value, bool bonus)
	{
		DWORD funcPointer = (DWORD)0x008A955B; //I dont think this does anything but not 100% sure, havent checked disk version as I do not think this is needed
		DWORD EDBpointer = dataOffsets::offsets.edbDataStart; //for some reason this is included, not sure if needed
		BuildingLvlCapability* cap = reinterpret_cast<BuildingLvlCapability*>(techFuncs::allocateGameMem(0x20));
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

	void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp, const char* condition)
	{
		recruitPool* pool = reinterpret_cast<recruitPool*>(techFuncs::allocateGameMem(0x20));
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		pool->capabilityType = 5; //5 means normal unit, there are some other for agents I havent added yet
		pool->capabilityLvlorExp = exp; //for units this always is xp, for agents this can be agent
		pool->unitID = eduIndex;
		pool->initialSize = initialSize;
		pool->gainPerTurn = gainPerTurn;
		pool->maxSize = maxSize;
		pool->buildingLevelCondition = nullptr;
		pool->nextPool = nullptr;
		auto fakeText = make_shared<fakeTextInput>(condition, 0);
		const auto makeConditionFunc = codes::offsets.makeBuildingCondition;
		auto conditionPtr = &pool->buildingLevelCondition;
		_asm
		{
			push 3
			push conditionPtr
			push fakeText
			mov eax, makeConditionFunc
			call eax
		}
		if (eoplevel->recruitPools != nullptr)
		{
			pool->nextPool = eoplevel->recruitPools; //always insert at start of pools
		}
		eoplevel->recruitPools = pool;
	}


	void removeBuildingCapability(edbEntry* entry, int level, int index)
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
		while (cap != nullptr)
		{
			i++;
			BuildingLvlCapability* prevCap = cap;
			cap = cap->nextCapability;
			if(index == i)
			{
				if (cap->nextCapability != nullptr)
				{
					prevCap->nextCapability = cap->nextCapability;
				}
				else
				{
					prevCap->nextCapability = nullptr;
				}
				cap = nullptr;
				return;
			}
		}
	}

	void removeBuildingPool(edbEntry* entry, int level, int index)
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
		while (pool != nullptr)
		{
			i++;
			recruitPool* prevPool = pool;
			pool = pool->nextPool;
			if (index == i)
			{
				if (pool->nextPool != nullptr)
				{
					prevPool->nextPool = pool->nextPool;
				}
				else
				{
					prevPool->nextPool = nullptr;
				}
				pool = nullptr;
				return;
			}
		}
	}

	//get a capability to change some of its attributes or check them, like to remove it
	BuildingLvlCapability* getBuildingCapability(edbEntry* entry, int level, int index)
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
			if (index == i)
			{
				return cap;
			}
		}
		return nullptr;
	}

	exportDescrBuildings* getEdb()
	{
		return reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	}
	recruitPool* getBuildingPool(edbEntry* entry, int level, int index)
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
			if (index == i)
			{
				return pool;
			}
		}
		return nullptr;
	}

	//get amount of capabilities, useful for iteration
	int getBuildingCapabilityNum(edbEntry* entry, int level)
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
	int getBuildingPoolNum(edbEntry* entry, int level)
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
	void createEOPBuilding(settlementStruct* sett, int edbIdx, int level)
	{
		edbEntry* entry = buildEntryDB::getEopBuildEntry(edbIdx);
		buildingLevel* eoplevel = &entry->buildingLevel[level];
		const char* building_level_id = (const char*)(eoplevel->name);
		settlementHelpers::createBuilding(sett, building_level_id);
		sett->buildings[sett->buildingsNum - 1]->edbEntry = entry;
	}

	//get a building entry by its name, very useful to change some of the edb building attributes dynamically, does not save!
	edbEntry* getBuildingByName(const char* name)
	{
		const auto edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
		const buildingListPointer* buildingsPtr = &edb->buildingsList;
		while (buildingsPtr != nullptr)
		{
			const int buildingNum = buildingsPtr->arrayCount;
			for (int i = 0; i < buildingNum; i++)
			{
				if (strcmp(buildingsPtr->buildingsArray->buildings[i].type, name) == 0)
				{
					return &buildingsPtr->buildingsArray->buildings[i];
				}
			}
			buildingsPtr = buildingsPtr->nextBuildingsListPointer;
		}
		return nullptr;
	}

	edbEntry* getBuildingByID(int id)
	{
		const auto* edb = reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
		const buildingListPointer* buildingsPointer = &edb->buildingsList;
		while (buildingsPointer != nullptr)
		{
			const int buildingNum = buildingsPointer->arrayCount;
			for (int i = 0; i < buildingNum; i++)
			{
				if (buildingsPointer->buildingsArray->buildings[i].buildingID == id)
				{
					return &buildingsPointer->buildingsArray->buildings[i];
				}
			}
			buildingsPointer = buildingsPointer->nextBuildingsListPointer;
		}
		return nullptr;
	}
	
	guild* getGuild(unsigned char index)
	{
		uintptr_t currentOffsett = dataOffsets::offsets.guildDataStart;
		int count = 0;
		UINT32 maxCount = 0;
		UINT8 currID = 0;

		do {
			techFuncs::read(currentOffsett + 0x10, &count);
			techFuncs::read(currentOffsett + 0xC, &maxCount);
			uintptr_t guilds = *(uintptr_t*)currentOffsett;

			if (guilds == 0)
			{
				return nullptr;
			}

			for (int i = 0; i < count; ++i)
			{
				if (index == currID)
				{
					return  (guild*)(guilds + 0x4C * i);
				}
				++currID;
			}

			if (count < (int)maxCount || *(uintptr_t*)(currentOffsett + 0x4) == 0)
			{
				return nullptr;
			}
			currentOffsett = *(uintptr_t*)(currentOffsett + 0x4);

		} while (*(uintptr_t*)currentOffsett != 0);

		return nullptr;
	}
};

namespace buildingHelpers
{
	void addToLua(sol::state& luaState)
	{
		
		struct
		{
			sol::table EDB;
		}tables;
		
		struct
		{
			sol::usertype<edbEntry>edbEntry;
			sol::usertype<BuildingLvlCapability>capability;
			sol::usertype<recruitPool>recruitpool;
		}types;
		
		///EdbEntry
		//@section edbEntry

		/***
		Basic edbEntry table.

		@tfield int buildingID
		@tfield int classification
		@tfield int isCoreBuilding
		@tfield int isPort
		@tfield int isCoreBuilding2
		@tfield int hasReligion
		@tfield int religionID
		@tfield int isHinterland
		@tfield int isFarm
		@tfield int buildingLevelCount

		@table edbEntry
		*/

		types.edbEntry = luaState.new_usertype<edbEntry>("edbEntry");
		types.edbEntry.set("buildingID", &edbEntry::buildingID);
		types.edbEntry.set("classification", &edbEntry::classification);
		types.edbEntry.set("isCoreBuilding", &edbEntry::isCoreBuilding);
		types.edbEntry.set("isPort", &edbEntry::isPort);
		types.edbEntry.set("isCoreBuilding2", &edbEntry::isCoreBuilding2);
		types.edbEntry.set("hasReligion", &edbEntry::hasReligion);
		types.edbEntry.set("religionID", &edbEntry::religionID);
		types.edbEntry.set("isHinterland", &edbEntry::isHinterland);
		types.edbEntry.set("isFarm", &edbEntry::isFarm);
		types.edbEntry.set("buildingLevelCount", &edbEntry::buildingLevelCount);

		///Capability
		//@section capability 

		/***
		Basic capability table.

		@tfield int capabilityType
		@tfield int capabilityLvl
		@tfield int bonus
		@tfield int capabilityID

		@table capability
		*/
		types.capability = luaState.new_usertype<BuildingLvlCapability>("capability");
		types.capability.set("capabilityType", &BuildingLvlCapability::capabilityType);
		types.capability.set("capabilityLvl", &BuildingLvlCapability::capabilityLvl);
		types.capability.set("bonus", &BuildingLvlCapability::bonus);
		types.capability.set("capabilityID", &BuildingLvlCapability::capabilityID);

		///RecruitPool
		//@section recruitpool 

		/***
		Basic recruitpool table.

		@tfield int capabilityType
		@tfield int capabilityLvlorExp Difference is for agents
		@tfield int unitID
		@tfield float initialSize
		@tfield float gainPerTurn
		@tfield float maxSize

		@table recruitpool
		*/
		types.recruitpool = luaState.new_usertype<recruitPool>("recruitpool");
		types.recruitpool.set("capabilityType", &recruitPool::capabilityType);
		types.recruitpool.set("capabilityLvlorExp", &recruitPool::capabilityLvlorExp);
		types.recruitpool.set("unitID", &recruitPool::unitID);
		types.recruitpool.set("initialSize", &recruitPool::initialSize);
		types.recruitpool.set("gainPerTurn", &recruitPool::gainPerTurn);
		types.recruitpool.set("maxSize", &recruitPool::maxSize);

		///EDB
		//@section EDB 

		/***
		Basic EDB table.

		@tfield addEopBuildEntry addEopBuildEntry
		@tfield getEopBuildEntry getEopBuildEntry
		@tfield setBuildingPic setBuildingPic
		@tfield setBuildingPicConstructed setBuildingPicConstructed
		@tfield setBuildingPicConstruction setBuildingPicConstruction
		@tfield setBuildingLocalizedName setBuildingLocalizedName
		@tfield setBuildingLocalizedDescr setBuildingLocalizedDescr
		@tfield setBuildingLocalizedDescrShort setBuildingLocalizedDescrShort
		@tfield addBuildingCapability addBuildingCapability
		@tfield removeBuildingCapability removeBuildingCapability
		@tfield getBuildingCapability getBuildingCapability
		@tfield getBuildingCapabilityNum getBuildingCapabilityNum
		@tfield addBuildingPool addBuildingPool
		@tfield removeBuildingPool removeBuildingPool
		@tfield getBuildingPool getBuildingPool
		@tfield getBuildingPoolNum getBuildingPoolNum
		@tfield getBuildingByName getBuildingByName

		@table EDB
		*/
		tables.EDB = luaState.create_table("EDB");

		/***
		Create new EOP Building entry
		@function EDB.addEopBuildEntry
		@tparam edbEntry edbEntry Old entry.
		@tparam int newIndex New index of new entry. Use index > 127!
		@treturn edbEntry eopentry.
		@usage
		-- Basic Example
		oldBuilding = EDB.getBuildingByName("market")
		newBuilding = EDB.addEopBuildEntry(oldBuilding,150);

		-- Full example
		local oldBuilding = EDB.getBuildingByName("market");
		local eopBuilding = EDB.addEopBuildEntry(oldBuilding, 150);

		-- Set pictures, names and descriptions by culture and faction
		for c = 0, 6 do --every culture
			EDB.setBuildingPic(eopBuilding, 'some path to pic', 0, c)
			EDB.setBuildingPicConstructed(eopBuilding,'some path to pic', 0, c)
		end
		for f = 0, 30 do --every faction
			EDB.setBuildingLocalizedName(eopBuilding, 'some name', 0, f)
			EDB.setBuildingLocalizedDescr(eopBuilding, 'some description', 0, f)
		end

		-- Add in an income bonus of 500
		EDB.addBuildingCapability(eopBuilding, 0, buildingCapability.income_bonus, 500, true)

		-- Add a recruit pool
		EDB.addBuildingPool(eopBuilding, 0, M2TWEOPDU.getEduIndexByType("Peasants"), 1, 0.1, 2, 0, "");

		-- Create a dummy building and get it
		sett:createBuilding("market");; --just assuming you have got a sett with some loop or function

		-- Set the existing building in the settlement to be the EOP building we just created
		local dummyBuilding = sett:getBuilding(5)
		dummyBuilding.edbEntry = eopBuilding
		*/
		tables.EDB.set_function("addEopBuildEntry", &buildEntryDB::addEopBuildEntry);

		/***
		Get EOP Building entry. Returns vanilla build entry if you use a vanilla building index (< 128).
		@function EDB.getEopBuildEntry
		@tparam int index Index of eop entry.
		@treturn edbEntry eopentry.
		@usage
		building = EDB.getEopBuildEntry(150);
		*/
		tables.EDB.set_function("getEopBuildEntry", &buildEntryDB::getEopBuildEntry);

		/***
		Set picture of building.
		@function EDB.setBuildingPic
		@tparam edbEntry edbEntry Entry to set.
		@tparam string newPic Path to new pic.
		@tparam int level Building level to set pic for.
		@tparam int culture ID of the culture to set the pic for.
		@usage
		building = EDB.getBuildingByName("market")
		EDB.setBuildingPic(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
		*/
		tables.EDB.set_function("setBuildingPic", &eopBuildings::setBuildingPic);

		/***
		Set constructed picture of building.
		@function EDB.setBuildingPicConstructed
		@tparam edbEntry edbEntry Entry to set.
		@tparam string newPic Path to new pic.
		@tparam int level Building level to set pic for.
		@tparam int culture ID of the culture to set the pic for.
		@usage
		building = EDB.getBuildingByName("market")
		EDB.setBuildingPicConstructed(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
		*/
		tables.EDB.set_function("setBuildingPicConstructed", &eopBuildings::setBuildingPicConstructed);

		/***
		Set construction picture of building.
		@function EDB.setBuildingPicConstruction
		@tparam edbEntry edbEntry Entry to set.
		@tparam string newPic Path to new pic.
		@tparam int level Building level to set pic for.
		@tparam int culture ID of the culture to set the pic for.
		@usage
		building = EDB.getBuildingByName("market")
		EDB.setBuildingPicConstruction(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
		*/
		tables.EDB.set_function("setBuildingPicConstruction", &eopBuildings::setBuildingPicConstruction);

		/***
		Set name of a building.
		@function EDB.setBuildingLocalizedName
		@tparam edbEntry edbEntry Entry to set.
		@tparam string newName New name.
		@tparam int level Building level.
		@tparam int facnum Faction ID of the faction to set it for (factionID).
		@usage
		building = EDB.getBuildingByName("market")
		EDB.setBuildingLocalizedName(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
		*/
		tables.EDB.set_function("setBuildingLocalizedName", &eopBuildings::setBuildingLocalizedName);

		/***
		Set description of a building.
		@function EDB.setBuildingLocalizedDescr
		@tparam edbEntry edbEntry Entry to set.
		@tparam string newName New description.
		@tparam int level Building level.
		@tparam int facnum Faction ID of the faction to set it for (factionID).
		@usage
		building = EDB.getBuildingByName("market")
		EDB.setBuildingLocalizedDescr(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
		*/
		tables.EDB.set_function("setBuildingLocalizedDescr", &eopBuildings::setBuildingLocalizedDescr);

		/***
		Set short description of a building.
		@function EDB.setBuildingLocalizedDescrShort
		@tparam edbEntry edbEntry Entry to set.
		@tparam string newName New short description.
		@tparam int level Building level.
		@tparam int facnum Faction ID of the faction to set it for (factionID).
		@usage
		building = EDB.getBuildingByName("market")
		EDB.setBuildingLocalizedDescrShort(building, modPath .. mp_path_mods .. "data/ui/northern_european/buildings/#northern_european_vintner.tga", 0, 4);
		*/
		tables.EDB.set_function("setBuildingLocalizedDescrShort", &eopBuildings::setBuildingLocalizedDescrShort);

		/***
		Add a capability to a building.
		@function EDB.addBuildingCapability
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@tparam int capability ID of capability to set. Use buildingCapability enum.
		@tparam int value Value to set.
		@tparam bool bonus Is it bonus or not.
		@usage
		
		building = EDB.getBuildingByName("market")

		-- Add a population growth bonus to the market building
		EDB.addBuildingCapability(building, 0, 0, 5, true);

		-- Add a 500 income bonus to the market building
		EDB.addBuildingCapability(building, 0, buildingCapability.income_bonus, 500, true)
		*/
		tables.EDB.set_function("addBuildingCapability", &eopBuildings::addBuildingCapability);

		/***
		Remove a capability from a building.
		@function EDB.removeBuildingCapability
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@tparam int index Which capability to remove (In order of iterating).
		@usage
		building = EDB.getBuildingByName("market")
		EDB.removeBuildingCapability(building, 0, 3);
		*/
		tables.EDB.set_function("removeBuildingCapability", &eopBuildings::removeBuildingCapability);

		/***

		Get capability from a building at an index.
		@function EDB.getBuildingCapability
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@tparam int index
		@treturn capability capability.
		@usage
		building = EDB.getBuildingByName("market")
		cap = EDB.getBuildingCapability(building, 0, 3);
		*/
		tables.EDB.set_function("getBuildingCapability", &eopBuildings::getBuildingCapability);


		/***

		Get capability amount from a building.
		@function EDB.getBuildingCapabilityNum
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@treturn int capabilityNum.
		@usage
		building = EDB.getBuildingByName("market")
		EDB.getBuildingCapabilityNum(building, 0);
		*/
		tables.EDB.set_function("getBuildingCapabilityNum", &eopBuildings::getBuildingCapabilityNum);

		/***
		Add a recruitment pool to a building.
		@function EDB.addBuildingPool
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@tparam int eduIndex edu index of unit to add.
		@tparam float initialSize Initial pool.
		@tparam float gainPerTurn Replenishment per turn.
		@tparam float maxSize Maximum size.
		@tparam int exp Initial experience.
		@tparam string condition Like in export_descr_buildings but without "requires".
		@usage
		building = EDB.getBuildingByName("market")
		EDB.addBuildingPool(building, 0, 55, 1, 0.1, 2, 0, "region_religion catholic 34");
		*/
		tables.EDB.set_function("addBuildingPool", &settlementHelpers::addBuildingPool);


		/***
		Remove a recruitment pool from a building.
		@function EDB.removeBuildingPool
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@tparam int index Which pool to remove (In order of iterating).
		@usage
		building = EDB.getBuildingByName("market")
		EDB.removeBuildingPool(building, 0, 3);
		*/
		tables.EDB.set_function("removeBuildingPool", &eopBuildings::removeBuildingPool);


		/***
		Get a recruitment pool from a building by index.
		@function EDB.getBuildingPool
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@tparam int index Which pool to get (In order of iterating).
		@treturn recruitpool pool.
		@usage
		building = EDB.getBuildingByName("market")
		EDB.getBuildingPool(building, 0, 3);
		*/
		tables.EDB.set_function("getBuildingPool", &eopBuildings::getBuildingPool);


		/***
		Get a recruitment pool count.
		@function EDB.getBuildingPoolNum
		@tparam edbEntry edbEntry Entry to set.
		@tparam int level Building level.
		@treturn int poolNum.
		@usage
		building = EDB.getBuildingByName("market")
		EDB.getBuildingPoolNum(building, 0);
		*/
		tables.EDB.set_function("getBuildingPoolNum", &eopBuildings::getBuildingPoolNum);


		//tables.EDB.set_function("createEOPBuilding", &buildingStructHelpers::createEOPBuilding);


		/***
		Get a building edb entry by name.
		@function EDB.getBuildingByName
		@tparam string buildingname
		@treturn edbEntry entry
		@usage
		building = EDB.getBuildingByName("market")
		*/
		tables.EDB.set_function("getBuildingByName", &eopBuildings::getBuildingByName);
	}
}