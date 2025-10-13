///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "faction.h"
#include "eopBuildings.h"
#include "settlement.h"
#include "dataOffsets.h"
#include "functionsOffsets.h"
#include "gameHelpers.h"
#include "luaPlugin.h"
#include "techFuncs.h"

std::vector<std::shared_ptr<eopBuildEntry>> buildEntryDB::eopEdb = {};
std::array<std::vector<int>, 200> eopHiddenResources::m_HiddenResources = {};
unordered_map<std::string, int> eopHiddenResources::m_HiddenResourcesLookup = {};
unordered_map<std::string, std::vector<int>> eopHiddenResources::m_NamesToIndexes = {};
bool eopHiddenResources::m_Initialized = false;

edbEntry* buildEntryDB::addEopBuildEntry(edbEntry* oldEntry, const int newIndex)
{
	if (getEopBuildEntry(newIndex)) //dont overwrite existing entries
		return nullptr;

	const auto newEntry = std::make_shared<eopBuildEntry>(oldEntry, newIndex);
	eopEdb.push_back(newEntry);
	edbEntry* entry = newEntry->baseEntry;
	const int32_t levelNum = entry->buildingLevelCount; //because levels can be unlocked to 57 needs to be dynamic not 9
	entry->levels = techFuncs::allocateGameClass<buildingLevel>(sizeof(buildingLevel) * levelNum);
	//making a complete copy of the building
	for (int i = 0; i < levelNum; i++)
	{
		memset(&entry->levels[i], 0, sizeof(buildingLevel));
		if (i < oldEntry->buildingLevelCount)
		{
			entry->levels[i] = oldEntry->levels[i];
			addCaps(&entry->levels[i], &oldEntry->levels[i]);
			addPools(&entry->levels[i], &oldEntry->levels[i]);
		}
	}
	return entry;
}

//get data from an existing eop build entry, in edb entry format
edbEntry* buildEntryDB::getEopBuildEntry(const int idx)
{
	for (const auto& entry : eopEdb)
	{
		if (entry->eopBuildId == idx)
			return entry->baseEntry;
	}
	return nullptr;
}

void buildEntryDB::addCaps(buildingLevel* eopLevel, const buildingLevel* oldLevel)
{
	if (!oldLevel->capabilities)
		return;
	auto eopCap = techFuncs::allocateGameClass<buildingLevelCapability>(sizeof(buildingLevelCapability));
	eopLevel->capabilities = eopCap;
	const buildingLevelCapability* oldCap = oldLevel->capabilities;
	*eopCap = *oldCap;
	oldCap = oldCap->nextCapability;
	while (oldCap != nullptr)
	{
		eopCap->nextCapability = techFuncs::allocateGameClass<buildingLevelCapability>(sizeof(buildingLevelCapability));
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

edbEntry* exportDescrBuildings::getBuildingByName(const std::string& name)
{
	if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.buildings.empty())
		plugData::data.luaAll.fillHashMapsNonCampaign();
	if (const auto build = plugData::data.luaAll.buildings.find(name); build != plugData::data.luaAll.buildings.end()) 
		return buildings.get(build->second);
	const int buildingNum = buildings.size();
	for (int i = 0; i < buildingNum; i++)
	{
		if (const auto entry = buildings.get(i); strcmp(entry->type, name.c_str()) == 0)
			return entry;
	}
	return nullptr;
}

int eopHiddenResources::getHiddenResourceIndex(const std::string& name)
{
	if (const auto res = m_HiddenResourcesLookup.find(name);
		res != m_HiddenResourcesLookup.end())
		return res->second;
	const auto edb = eopBuildings::getEdb();
	if (!edb)
		return -1;
	return edb->getHiddenResourceIndex(name.c_str());
}

void eopHiddenResources::addHiddenResource(const std::string& name)
{
	m_HiddenResourcesLookup[name] = m_HiddenResourcesLookup.size();
}

void eopHiddenResources::addHiddenResourceWithId(const std::string& name, const int id)
{
	m_HiddenResourcesLookup[name] = id;
}

void eopHiddenResources::addHiddenResourceToRegion(const int regionId, const std::string& name)
{
	const auto index = getHiddenResourceIndex(name);
	if (std::find(m_HiddenResources[regionId].begin(), m_HiddenResources[regionId].end(), index) != m_HiddenResources[regionId].end())
		return; //already added
	m_HiddenResources[regionId].push_back(getHiddenResourceIndex(name));
}

void eopHiddenResources::removeHiddenResourceFromRegion(int regionId, const std::string& name)
{
	const auto res = getHiddenResourceIndex(name);
	if (res == -1)
		return;
	auto& hiddenRes = m_HiddenResources[regionId];
	if (const auto it = std::find(hiddenRes.begin(), hiddenRes.end(), res); it != hiddenRes.end())
		hiddenRes.erase(it);
}

void eopHiddenResources::addHiddenResourceToRegionIndex(const std::string& name, const int id)
{
	if (m_NamesToIndexes.find(name) == m_NamesToIndexes.end())
		m_NamesToIndexes[name] = {};
	m_NamesToIndexes[name].push_back(id);
}

bool eopHiddenResources::hasHiddenResource(const int regionId, const int id)
{
	if (!isInitialized())
		initialize();
	if (regionId >= static_cast<int>(m_HiddenResources.size()))
		return false;
	const auto& res = m_HiddenResources[regionId];
	return std::find(res.begin(), res.end(), id) != res.end();
}

void eopHiddenResources::initialize()
{
	const auto mapData = stratMapHelpers::getStratMap();
	const auto n = mapData->regionsNum;
	for (int i = 0; i < n; i++)
	{
		if (const auto region = &mapData->regions[i]; m_NamesToIndexes.find(region->regionName) != m_NamesToIndexes.end())
		{
			const auto& res = m_NamesToIndexes[region->regionName];
			for (const auto id : res)
				m_HiddenResources[i].push_back(id);
		}
	}
	m_Initialized = true;
}


std::string buildingLevel::getLocalizedName(const int factionID)
{
	if (const int facNum = factionHelpers::getFactionRecordNum(); factionID >= facNum)
		return "";
	if (buildingName[factionID] != nullptr && *buildingName[factionID] != nullptr)
		return gameStringHelpers::uniStringToStr(*buildingName[factionID]);
	return "";
}


std::string buildingLevel::getLocalizedDescr(const int factionID)
{
	if (const int facNum = factionHelpers::getFactionRecordNum(); factionID >= facNum)
		return "";
	if (buildingDescr[factionID] != nullptr && *buildingDescr[factionID] != nullptr)
		return gameStringHelpers::uniStringToStr(*buildingDescr[factionID]);
	return "";
}


std::string buildingLevel::getLocalizedDescrShort(const int factionID)
{
	if (const int facNum = factionHelpers::getFactionRecordNum(); factionID >= facNum)
		return "";
	if (buildingDescrShort[factionID] != nullptr)
		return gameStringHelpers::uniStringToStr(*buildingDescrShort[factionID]);
	return "";
}


//add new building capability, bonus refers to bonus keyboard in edb
void buildingLevel::addCapability(int capability, int16_t value, bool bonus, const std::string& condition)
{
	const int capabilityNum = getCapabilityNum();
	buildingLevelCapability* cap;
	if (capabilityNum == 0)
	{
		capabilities = techFuncs::createGameClass<buildingLevelCapability>();
		cap = capabilities;
	}
	else
	{
		const auto lastCap = getCapability(capabilityNum - 1);
		lastCap->nextCapability = techFuncs::createGameClass<buildingLevelCapability>();
		cap = lastCap->nextCapability;
	}
	memset(cap, 0, sizeof(buildingLevelCapability));
	cap->capabilityType = 0; //always 0 for normal capabilities
	cap->capabilityLvl = -1;
	cap->bonus = 0;
	if (bonus)
		cap->bonus = value;
	else
		cap->capabilityLvl = value;
	cap->capabilityID = capability;
	cap->initial = 0;
	cap->replenishment = 0;
	cap->max = 0;
	cap->buildingLevelCondition = nullptr;
	if (condition.empty())
		return;
	const auto fakeText = make_shared<fakeTextInput>(condition.c_str(), 0);
	GAME_FUNC(void(__stdcall*)(fakeTextInput*, void**, int), makeBuildingCondition)(fakeText.get(), &cap->buildingLevelCondition, 2);
}

//add new building capability, bonus refers to bonus keyboard in edb
void buildingLevel::addFactionCapability(int capability, int16_t value, bool bonus, const std::string& condition)
{
	const int capabilityNum = getFactionCapabilityNum();
	buildingLevelCapability* cap;
	if (capabilityNum == 0)
	{
		factionCapabilities = techFuncs::createGameClass<buildingLevelCapability>();
		cap = factionCapabilities;
	}
	else
	{
		const auto lastCap = getFactionCapability(capabilityNum - 1);
		lastCap->nextCapability = techFuncs::createGameClass<buildingLevelCapability>();
		cap = lastCap->nextCapability;
	}
	memset(cap, 0, sizeof(buildingLevelCapability));
	cap->capabilityType = 0; //always 0 for normal capabilities
	cap->capabilityLvl = -1;
	cap->bonus = 0;
	if (bonus)
		cap->bonus = value;
	else
		cap->capabilityLvl = value;
	cap->capabilityID = capability;
	cap->initial = 0;
	cap->replenishment = 0;
	cap->max = 0;
	cap->buildingLevelCondition = nullptr;
	if (condition.empty())
		return;
	const auto fakeText = make_shared<fakeTextInput>(condition.c_str(), 0);
	GAME_FUNC(void(__stdcall*)(fakeTextInput*, void**, int), makeBuildingCondition)(fakeText.get(), &cap->buildingLevelCondition, 2);
}

void buildingLevel::addRecruitPool(int eduIndex, float initialSize, float gainPerTurn, float maxSize, int16_t exp, const std::string& condition)
{
	const int capabilityNum = getPoolNum();
	recruitPool* pool;
	if (capabilityNum == 0)
	{
		recruitPools = techFuncs::createGameClass<recruitPool>();
		pool = recruitPools;
	}
	else
	{
		const auto lastCap = getPool(capabilityNum - 1);
		lastCap->nextPool = techFuncs::createGameClass<recruitPool>();
		pool = lastCap->nextPool;
	}
	memset(pool, 0, sizeof(recruitPool));
	pool->capabilityType = 5; //5 means normal unit, there are some other for agents I haven't added yet
	pool->experienceOrLevel = exp; //for units this always is xp, for agents this can be agent
	pool->unitID = eduIndex;
	pool->initialSize = initialSize;
	pool->gainPerTurn = gainPerTurn;
	pool->maxSize = maxSize;
	pool->buildingLevelCondition = nullptr;
	if (condition.empty())
		return;
	const auto fakeText = make_shared<fakeTextInput>(condition.c_str(), 0);
	GAME_FUNC(void(__stdcall*)(fakeTextInput*, void**, int), makeBuildingCondition)(fakeText.get(), &pool->buildingLevelCondition, 1);
}


void buildingLevel::removeBuildingCapability(int index)
{
	buildingLevelCapability* cap = capabilities;
	if (const int capNum = getCapabilityNum(); index >= capNum || cap == nullptr)
		return;
	if (index == 0)
	{
		capabilities = nullptr;
		if (cap->nextCapability != nullptr)
			capabilities = cap->nextCapability;
		return;
	}
	int i = 0;
	while (cap != nullptr)
	{
		i++;
		buildingLevelCapability* prevCap = cap;
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

void buildingLevel::removeBuildingFactionCapability(const int index)
{
	buildingLevelCapability* cap = factionCapabilities;
	if (const int capNum = getCapabilityNum(); index >= capNum || cap == nullptr)
		return;
	if (index == 0)
	{
		factionCapabilities = nullptr;
		if (cap->nextCapability != nullptr)
			factionCapabilities = cap->nextCapability;
		return;
	}
	int i = 0;
	while (cap != nullptr)
	{
		i++;
		buildingLevelCapability* prevCap = cap;
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

void buildingLevel::removeBuildingPool(int index)
{
	recruitPool* pool = recruitPools;
	if (const int poolNum = getPoolNum(); index >= poolNum || pool == nullptr)
		return;
	if (index == 0)
	{
		recruitPools = nullptr;
		if (pool->nextPool != nullptr)
			recruitPools = pool->nextPool;
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

namespace eopBuildings
{
    ////////////////////// Legacy compatibility functions //////////////////////
	
	//unique per culture!
	void setBuildingPic(const edbEntry* entry, const char* newPic, const int level, const int cultureID)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->setBuildingPicPath(cultureID, newPic);
	}
	//unique per culture!
	void setBuildingPicConstructed(const edbEntry* entry, const char* newPic, const int level, const int cultureID)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->setBuildingPicConstructedPath(cultureID, newPic);
	}
	//unique per culture!
	void setBuildingPicConstruction(const edbEntry* entry, const char* newPic, const int level, const int cultureID)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->setBuildingPicConstructionPath(cultureID, newPic);
	}
	//unique per faction!
	void setBuildingLocalizedName(const edbEntry* entry, const char* newName, const int level, const int factionID)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->setLocalizedName(factionID, newName);
	}
	//unique per faction!
	void setBuildingLocalizedDescr(const edbEntry* entry, const char* newName, const int level, const int factionID)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->setLocalizedDescr(factionID, newName);
	}
	//unique per faction!
	void setBuildingLocalizedDescrShort(const edbEntry* entry, const char* newName, const int level, const int factionID)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->setLocalizedDescrShort(factionID, newName);
	}
	void removeBuildingCapability(const edbEntry* entry, const int level, const int index)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->removeBuildingCapability(index);
	}
	void removeBuildingPool(const edbEntry* entry, const int level, const int index)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->removeBuildingPool(index);
	}
	//get a capability to change some of its attributes or check them, like to remove it
	buildingLevelCapability* getBuildingCapability(const edbEntry* entry, const int level, const int index)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->getCapability(index);
	}
	exportDescrBuildings* getEdb()
	{
		return reinterpret_cast<exportDescrBuildings*>(dataOffsets::offsets.edbDataStart);
	}
	
	recruitPool* getBuildingPool(const edbEntry* entry, const int level, const int index)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->getPool(index);
	}
	void addBuildingCapability(const edbEntry* entry, const int level, const int capability, const int16_t value, const bool bonus)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->addCapability(capability, value, bonus, "");
	}
	
	void addBuildingPool(const edbEntry* entry, const int level, const int eduIndex, const float initialSize, const float gainPerTurn,
	                     const float maxSize, const int32_t exp, const char* condition)
	{
		buildingLevel* eopLevel = &entry->levels[level];
		eopLevel->addRecruitPool(eduIndex, initialSize, gainPerTurn, maxSize, exp, condition);
	}
	
	//get amount of capabilities, useful for iteration
	int getBuildingCapabilityNum(const edbEntry* entry, const int level)
	{
		if (entry == nullptr)
			return 0;
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->getCapabilityNum();
	}

	//get amount of pools, useful for iteration
	int getBuildingPoolNum(const edbEntry* entry, const int level)
	{
		if (entry == nullptr)
			return 0;
		buildingLevel* eopLevel = &entry->levels[level];
		return eopLevel->getPoolNum();
	}

	//get a building entry by its name, very useful to change some of the edb building attributes dynamically, does not save!


	gameList<guild>* getGuilds()
	{
		return reinterpret_cast<gameList<guild>*>(dataOffsets::offsets.guildDataStart);
	}
	
	guild* getGuild(const int index)
	{
		return getGuilds()->get(index);
	}

	int getGuildNum()
	{
		return getGuilds()->size();
	}
	
	guild* getGuildByName(const std::string& name)
	{
		const auto guilds = getGuilds();
		const int guildNum = guilds->size();
		for (int i = 0; i < guildNum; i++)
		{
			if (const auto guild = guilds->get(i); strcmp(guild->name, name.c_str()) == 0)
				return guild;
		}
		return nullptr;
	}
};

namespace buildingHelpers
{
	edbEntry* luaGetBuildingByName(const std::string& name)
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return nullptr;
		return edb->getBuildingByName(name);
	}
	
	edbEntry* luaGetBuildingByID(const int id)
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return nullptr;
		return edb->getBuildingByID(id);
	}
	
	int getHiddenResourceCount()
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return 0;
		return edb->hiddenResourceCount;
	}
	
	int getBuildingCount()
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return 0;
		return edb->getBuildingNum();
	}
	
	edbEntry* getEdbPort()
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return nullptr;
		return edb->port;
	}
	
	edbEntry* getEdbCastlePort()
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return nullptr;
		return edb->castlePort;
	}
	
	edbEntry* getEdbCoreCityBuilding()
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return nullptr;
		return edb->coreCityBuilding;
	}
	
	edbEntry* getEdbCoreCastleBuilding()
	{
		const auto edb = eopBuildings::getEdb();
		if (!edb)
			return nullptr;
		return edb->coreCastleBuilding;
	}
	
	int getBuildingId(const std::string& name)
	{
		if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.buildings.empty())
			plugData::data.luaAll.fillHashMapsNonCampaign();
		const auto id = plugData::data.luaAll.buildings.find(name);
		if (id == plugData::data.luaAll.buildings.end())
		{
			gameHelpers::logStringGame("eopBuildings::getBuildingId: building not found: " + name);
			return -1;
		}
		return id->second;
	}

	int getBuildingLevelId(const std::string& name)
	{
		if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.buildingLevelLines.empty())
			plugData::data.luaAll.fillHashMapsNonCampaign();
		const auto id = plugData::data.luaAll.buildingLevelLines.find(name);
		if (id == plugData::data.luaAll.buildingLevelLines.end())
		{
			gameHelpers::logStringGame("eopBuildings::getBuildingLevelId: building level not found: " + name);
			return -1;
		}
		return id->second;
	}

	int getBuildingLevelPos(const std::string& name)
	{
		if (!plugData::data.luaAll.hashNonCampaignLoaded || plugData::data.luaAll.buildingLevels.empty())
			plugData::data.luaAll.fillHashMapsNonCampaign();
		const auto id = plugData::data.luaAll.buildingLevels.find(name);
		if (id == plugData::data.luaAll.buildingLevels.end())
		{
			gameHelpers::logStringGame("eopBuildings::getBuildingLevelPos: building level not found: " + name);
			return -1;
		}
		return id->second;
	}

	void addToLua(sol::state& luaState)
	{
		struct
		{
			sol::usertype<edbEntry>edbEntry;
			sol::usertype<buildingLevelCapability>capability;
			sol::usertype<recruitPool>recruitPool;
			sol::usertype<buildingLevel>buildingLevel;
			sol::usertype<exportDescrBuildings>EDB;
		}types;
		
		///Buildings
		//@section Buildings

		/***
		Basic edbEntry table.

		@tfield int buildingID
		@tfield string localizedName
		@tfield string name
		@tfield int classification
		@tfield int isCoreBuilding
		@tfield int isPort
		@tfield int isWallBuilding
		@tfield int isTemple
		@tfield int religionID
		@tfield int isHinterland
		@tfield int isFarm
		@tfield int buildingLevelCount
		@tfield getBuildingLevel getBuildingLevel

		@table edbEntry
		*/

		types.edbEntry = luaState.new_usertype<edbEntry>("edbEntry");
		types.edbEntry.set("buildingID", &edbEntry::buildingID);
		types.edbEntry.set("classification", &edbEntry::classification);
		types.edbEntry.set("isCoreBuilding", &edbEntry::isCoreBuilding);
		types.edbEntry.set("isPort", &edbEntry::isPort);
		types.edbEntry.set("isWallBuilding", &edbEntry::isWallBuilding);
		types.edbEntry.set("isTemple", &edbEntry::isTemple);
		types.edbEntry.set("religionID", &edbEntry::religionID);
		types.edbEntry.set("isHinterland", &edbEntry::isHinterland);
		types.edbEntry.set("isFarm", &edbEntry::isFarm);
		types.edbEntry.set("name", sol::property(&edbEntry::getName, &edbEntry::setName));
		types.edbEntry.set("localizedName", sol::property(&edbEntry::getLocalizedName, &edbEntry::setLocalizedName));
		types.edbEntry.set("buildingLevelCount", &edbEntry::buildingLevelCount);
		
		/***
		Get a building level.
		@function edbEntry:getBuildingLevel
		@tparam int index
		@treturn buildingLevel level
		@usage
			local level = edbEntry:getBuildingLevel(0)
		*/
		types.edbEntry.set_function("getBuildingLevel", &edbEntry::getBuildingLevel);

		/***
		Basic buildingLevel table.

		@tfield string name
		@tfield string genericBuildingPic
		@tfield string genericBuildingPicConstructed
		@tfield string genericBuildingPicConstruction
		@tfield int buildCost
		@tfield int buildTime
		@tfield int capabilityNum
		@tfield int recruitPoolNum
		@tfield int factionCapabilityNum
		@tfield int settlementMinLvl
		@tfield bool availableCastle
		@tfield bool availableCity
		@tfield setBuildingPic setBuildingPic
		@tfield getBuildingPic getBuildingPic
		@tfield setBuildingPicConstructed setBuildingPicConstructed
		@tfield getBuildingPicConstructed getBuildingPicConstructed
		@tfield setBuildingPicConstruction setBuildingPicConstruction
		@tfield getBuildingPicConstruction getBuildingPicConstruction
		@tfield setLocalizedName setLocalizedName
		@tfield getLocalizedName getLocalizedName
		@tfield setLocalizedDescr setLocalizedDescr
		@tfield getLocalizedDescr getLocalizedDescr
		@tfield setLocalizedDescrShort setLocalizedDescrShort
		@tfield getLocalizedDescrShort getLocalizedDescrShort
		@tfield getCapability getCapability
		@tfield getFactionCapability getFactionCapability
		@tfield getRecruitPool getRecruitPool
		@tfield addCapability addCapability
		@tfield addFactionCapability addFactionCapability
		@tfield addRecruitPool addRecruitPool
		@tfield removeCapability removeCapability
		@tfield removeFactionCapability removeFactionCapability
		@tfield removeRecruitPool removeRecruitPool

		@table buildingLevel
		*/
		types.buildingLevel = luaState.new_usertype<buildingLevel>("buildingLevel");
		types.buildingLevel.set("name", sol::property(&buildingLevel::getName, &buildingLevel::setName));
		types.buildingLevel.set("genericBuildingPic", sol::property(&buildingLevel::getGenericBuildingPic, &buildingLevel::setGenericBuildingPic));
		types.buildingLevel.set("genericBuildingPicConstructed", sol::property(&buildingLevel::getGenericBuildingPicConstructed, &buildingLevel::setGenericBuildingPicConstructed));
		types.buildingLevel.set("genericBuildingPicConstruction", sol::property(&buildingLevel::getGenericBuildingPicConstruction, &buildingLevel::setGenericBuildingPicConstruction));
		types.buildingLevel.set("buildCost", &buildingLevel::buildCost);
		types.buildingLevel.set("buildTime", &buildingLevel::buildTime);
		types.buildingLevel.set("settlementMinLvl", &buildingLevel::settlementMinLvl);
		types.buildingLevel.set("availableCastle", sol::property(&buildingLevel::availableCastle));
		types.buildingLevel.set("availableCity", sol::property(&buildingLevel::availableCity));
		types.buildingLevel.set("capabilityNum", sol::property(&buildingLevel::getCapabilityNum));
		types.buildingLevel.set("recruitPoolNum", sol::property(&buildingLevel::getPoolNum));
		types.buildingLevel.set("factionCapabilityNum", sol::property(&buildingLevel::getFactionCapabilityNum));
		
		/***
		Set picture of building.
		@function buildingLevel:setBuildingPic
		@tparam int cultureID of the culture to set the pic for. Only cultures 0-6 are valid.
		@tparam string path Path to the picture.
		@usage
			buildingLevel:setBuildingPic(0, "some path to pic")
		*/
		types.buildingLevel.set_function("setBuildingPic", &buildingLevel::setBuildingPicPath);
		
		/***
		Get picture of building.
		@function buildingLevel:getBuildingPic
		@tparam int cultureID of the culture to set the pic for. Only cultures 0-6 are valid.
		@treturn string path Path to the picture.
		@usage
			local path = buildingLevel:getBuildingPic(0)
		*/
		types.buildingLevel.set_function("getBuildingPic", &buildingLevel::getBuildingPicPath);
		
		/***
		Set constructed picture of building.
		@function buildingLevel:setBuildingPicConstructed
		@tparam int cultureID of the culture to set the pic for. Only cultures 0-6 are valid.
		@tparam string path Path to the picture.
		@usage
			buildingLevel:setBuildingPicConstructed(0, "some path to pic")
		*/
		types.buildingLevel.set_function("setBuildingPicConstructed", &buildingLevel::setBuildingPicConstructedPath);
		
		/***
		Get constructed picture of building.
		@function buildingLevel:getBuildingPicConstructed
		@tparam int cultureID of the culture to set the pic for. Only cultures 0-6 are valid.
		@treturn string path Path to the picture.
		@usage
			local path = buildingLevel:getBuildingPicConstructed(0)
		*/
		types.buildingLevel.set_function("getBuildingPicConstructed", &buildingLevel::getBuildingPicConstructedPath);
		
		/***
		Set construction picture of building.
		@function buildingLevel:setBuildingPicConstruction
		@tparam int cultureID of the culture to set the pic for. Only cultures 0-6 are valid.
		@tparam string path Path to the picture.
		@usage
			buildingLevel:setBuildingPicConstruction(0, "some path to pic")
		*/
		types.buildingLevel.set_function("setBuildingPicConstruction", &buildingLevel::setBuildingPicConstructionPath);
		
		/***
		Get construction picture of building.
		@function buildingLevel:getBuildingPicConstruction
		@tparam int cultureID of the culture to set the pic for. Only cultures 0-6 are valid.
		@tparam string path Path to the picture.
		@usage
			local path = buildingLevel:getBuildingPicConstruction(0)
		*/
		types.buildingLevel.set_function("getBuildingPicConstruction", &buildingLevel::getBuildingPicConstructionPath);
		
		/***
		Set the name of the building level.
		@function buildingLevel:setLocalizedName
		@tparam int factionID of the faction to set the pic for.
		@tparam string name New name.
		@usage
			buildingLevel:setLocalizedName(0, "some name")
		*/
		types.buildingLevel.set_function("setLocalizedName", &buildingLevel::setLocalizedName);
		
		/***
		Get the name of the building level.
		@function buildingLevel:getLocalizedName
		@tparam int factionID
		@treturn string descr 
		@usage
			local name = buildingLevel:getLocalizedName(0)
		*/
		types.buildingLevel.set_function("getLocalizedName", &buildingLevel::getLocalizedName);
		
		/***
		Set the description of the building level.
		@function buildingLevel:setLocalizedDescr
		@tparam int factionID
		@tparam string descr
		@usage
			buildingLevel:setLocalizedDescr(0, "some description")
		*/
		types.buildingLevel.set_function("setLocalizedDescr", &buildingLevel::setLocalizedDescr);
		
		/***
		Get the description of the building level.
		@function buildingLevel:getLocalizedDescr
		@tparam int factionID
		@treturn string descr 
		@usage
			local descr = buildingLevel:getLocalizedDescr(0)
		*/
		types.buildingLevel.set_function("getLocalizedDescr", &buildingLevel::getLocalizedDescr);
		
		/***
		Set the description of the building level.
		@function buildingLevel:setLocalizedDescrShort
		@tparam int factionID
		@tparam string descr
		@usage
			buildingLevel:setLocalizedDescrShort(0, "some description")
		*/
		types.buildingLevel.set_function("setLocalizedDescrShort", &buildingLevel::setLocalizedDescrShort);
		
		/***
		Get the description of the building level.
		@function buildingLevel:getLocalizedDescrShort
		@tparam int factionID
		@treturn string descr 
		@usage
			local descr = buildingLevel:getLocalizedDescrShort(0)
		*/
		types.buildingLevel.set_function("getLocalizedDescrShort", &buildingLevel::getLocalizedDescrShort);
		
		/***
		Get a capability.
		@function buildingLevel:getCapability
		@tparam int index
		@treturn capability cap 
		@usage
			local cap = buildingLevel:getCapability(0)
		*/
		types.buildingLevel.set_function("getCapability", &buildingLevel::getCapability);
		
		/***
		Get a faction capability.
		@function buildingLevel:getFactionCapability
		@tparam int index
		@treturn capability cap 
		@usage
			local cap = buildingLevel:getFactionCapability(0)
		*/
		types.buildingLevel.set_function("getFactionCapability", &buildingLevel::getFactionCapability);
		
		/***
		Get a recruit pool.
		@function buildingLevel:getRecruitPool
		@tparam int index
		@treturn recruitPool pool 
		@usage
			local pool = buildingLevel:getRecruitPool(0)
		*/
		types.buildingLevel.set_function("getRecruitPool", &buildingLevel::getPool);
		
		/***
		Add a capability.
		@function buildingLevel:addCapability
		@tparam int capability use capabilities enum
		@tparam int value
		@tparam bool bonus
		@tparam string condition
		@usage
			   buildingLevel:addCapability(buildingCapability.income_bonus, 500, true, "factions { england, france, }")
		*/
		types.buildingLevel.set_function("addCapability", &buildingLevel::addCapability);
		
		/***
		Add a faction capability.
		@function buildingLevel:addFactionCapability
		@tparam int capability use capabilities enum
		@tparam int value
		@tparam bool bonus
		@tparam string condition
		@usage
			   buildingLevel:addFactionCapability(buildingCapability.income_bonus, 500, true, "factions { england, france, }")
		*/
		types.buildingLevel.set_function("addFactionCapability", &buildingLevel::addFactionCapability);
		
		/***
		Add a recruit pool.
		@function buildingLevel:addRecruitPool
		@tparam int eduIndex
		@tparam float initialSize
		@tparam float gainPerTurn
		@tparam float maxSize
		@tparam int exp
		@tparam string condition
		@usage
			   buildingLevel:addRecruitPool(M2TWEOPDU.getEduIndexByType("Peasants"), 1, 0.1, 2, 1, "factions { england, france, }")
		*/
		types.buildingLevel.set_function("addRecruitPool", &buildingLevel::addRecruitPool);
		
		/***
		Remove a capability.
		@function buildingLevel:removeCapability
		@tparam int index
		@usage
			   buildingLevel:removeCapability(0)
		*/
		types.buildingLevel.set_function("removeCapability", &buildingLevel::removeBuildingCapability);
		
		/***
		Remove a faction capability.
		@function buildingLevel:removeFactionCapability
		@tparam int index
		@usage
			   buildingLevel:removeFactionCapability(0)
		*/
		types.buildingLevel.set_function("removeFactionCapability", &buildingLevel::removeBuildingFactionCapability);
		
		/***
		Remove a recruit pool.
		@function buildingLevel:removeRecruitPool
		@tparam int index
		@usage
			   buildingLevel:removeRecruitPool(0)
		*/
		types.buildingLevel.set_function("removeRecruitPool", &buildingLevel::removeBuildingPool);
		
		/***
		Basic capability table.

		@tfield int capabilityType
		@tfield int capabilityLvl
		@tfield int bonus
		@tfield int capabilityID

		@table capability
		*/
		types.capability = luaState.new_usertype<buildingLevelCapability>("capability");
		types.capability.set("capabilityType", &buildingLevelCapability::capabilityType);
		types.capability.set("capabilityLvl", &buildingLevelCapability::capabilityLvl);
		types.capability.set("bonus", &buildingLevelCapability::bonus);
		types.capability.set("capabilityID", &buildingLevelCapability::capabilityID);
		
		/***
		Basic recruitPool table.

		@tfield int capabilityType
		@tfield int experience
		@tfield int unitID
		@tfield int agentAmount
		@tfield int agentLimit
		@tfield int agentType
		@tfield float initialSize
		@tfield float gainPerTurn
		@tfield float maxSize

		@table recruitPool
		*/
		types.recruitPool = luaState.new_usertype<recruitPool>("recruitPool");
		types.recruitPool.set("capabilityType", &recruitPool::capabilityType);
		types.recruitPool.set("experience", sol::property(&recruitPool::getUnitExperience, &recruitPool::setUnitExperience));
		types.recruitPool.set("unitID", sol::property(&recruitPool::getUnitID, &recruitPool::setUnitID));
		types.recruitPool.set("agentAmount", sol::property(&recruitPool::getAgentAmount, &recruitPool::setAgentAmount));
		types.recruitPool.set("agentLimit", sol::property(&recruitPool::getAgentLimit, &recruitPool::setAgentLimit));
		types.recruitPool.set("agentType", sol::property(&recruitPool::getAgentType, &recruitPool::setAgentType));
		types.recruitPool.set("initialSize", &recruitPool::initialSize);
		types.recruitPool.set("gainPerTurn", &recruitPool::gainPerTurn);
		types.recruitPool.set("maxSize", &recruitPool::maxSize);

		///EDB
		//@section EDB 

		/***
		Basic EDB table.

		@tfield addEopBuildEntry addEopBuildEntry
		@tfield int hiddenResourceCount
		@tfield edbEntry port
		@tfield edbEntry castlePort
		@tfield edbEntry coreCityBuilding
		@tfield edbEntry coreCastleBuilding
		@tfield getBuildingByName getBuildingByName
		@tfield getBuildingByID getBuildingByID
		@tfield getBuildingNum getBuildingNum
		@tfield getGuildNum getGuildNum
		@tfield getGuild getGuild
		@tfield getGuildByName getGuildByName

		@table EDB
		*/
		types.EDB = luaState.new_usertype<exportDescrBuildings>("EDB");
		types.EDB.set("hiddenResourceCount", sol::property(&getHiddenResourceCount));
		types.EDB.set("port", sol::property(&getEdbPort));
		types.EDB.set("castlePort", sol::property(&getEdbCastlePort));
		types.EDB.set("coreCityBuilding", sol::property(&getEdbCoreCityBuilding));
		types.EDB.set("coreCastleBuilding", sol::property(&getEdbCoreCastleBuilding));

		/***
		Create new EOP Building entry
		@function EDB.addEopBuildEntry
		@tparam edbEntry edbEntry Old entry.
		@tparam int newIndex New index of new entry. Use index > 127!
		@treturn edbEntry eopentry.
		@usage
		*/
		types.EDB.set_function("addEopBuildEntry", &buildEntryDB::addEopBuildEntry);
		
		/***
		Get a building edb entry by name.
		@function EDB.getBuildingByName
		@tparam string buildingname
		@treturn edbEntry entry
		@usage
		local building = EDB.getBuildingByName("market")
		*/
		types.EDB.set_function("getBuildingByName", &luaGetBuildingByName);
		
		/***
		Get a building edb entry by index (EOP buildings too).
		@function EDB.getBuildingByID
		@tparam int index
		@treturn edbEntry entry
		@usage
		local building = EDB.getBuildingByID(22)
		*/
		types.EDB.set_function("getBuildingByID", &luaGetBuildingByID);
		
		/***
		Get number of vanilla buildings.
		@function EDB.getBuildingNum
		@treturn int num
		@usage
		local num = EDB.getBuildingNum()
		*/
		types.EDB.set_function("getBuildingNum", &getBuildingCount);
		
		/***
		Get a guild by index.
		@function EDB.getGuild
		@tparam int index
		@treturn guild guild
		@usage
		local guild = EDB.getGuild(0)
		*/
		types.EDB.set_function("getGuild", &eopBuildings::getGuild);
		
		/***
		Get number of guilds.
		@function EDB.getGuildNum
		@treturn int num
		@usage
		local num = EDB.getGuildNum()
		*/
		types.EDB.set_function("getGuildNum", &eopBuildings::getGuildNum);
		
		/***
		Get a guild by name.
		@function EDB.getGuildByName
		@tparam string name
		@treturn guild guild
		@usage
		local guild = EDB.getGuildByName("assassins_guild")
		*/
		types.EDB.set_function("getGuildByName", &eopBuildings::getGuildByName);


		
		///Legacy functions///
		types.EDB.set_function("getEopBuildEntry", &buildEntryDB::getEopBuildEntry);
		types.EDB.set_function("setBuildingPic", &eopBuildings::setBuildingPic);
		types.EDB.set_function("setBuildingPicConstructed", &eopBuildings::setBuildingPicConstructed);
		types.EDB.set_function("setBuildingPicConstruction", &eopBuildings::setBuildingPicConstruction);
		types.EDB.set_function("setBuildingLocalizedName", &eopBuildings::setBuildingLocalizedName);
		types.EDB.set_function("setBuildingLocalizedDescr", &eopBuildings::setBuildingLocalizedDescr);
		types.EDB.set_function("setBuildingLocalizedDescrShort", &eopBuildings::setBuildingLocalizedDescrShort);
		types.EDB.set_function("addBuildingCapability", &eopBuildings::addBuildingCapability);
		types.EDB.set_function("removeBuildingCapability", &eopBuildings::removeBuildingCapability);
		types.EDB.set_function("getBuildingCapability", &eopBuildings::getBuildingCapability);
		types.EDB.set_function("getBuildingCapabilityNum", &eopBuildings::getBuildingCapabilityNum);
		types.EDB.set_function("addBuildingPool", &eopBuildings::addBuildingPool);
		types.EDB.set_function("removeBuildingPool", &eopBuildings::removeBuildingPool);
		types.EDB.set_function("getBuildingPool", &eopBuildings::getBuildingPool);
		types.EDB.set_function("getBuildingPoolNum", &eopBuildings::getBuildingPoolNum);
	}
}
