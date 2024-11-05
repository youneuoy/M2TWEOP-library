#pragma once
#include <string>

#include "dataOffsets.h"
#include "gameStringHelpers.h"
#include "realGameTypes.h"
#include "techFuncs.h"
#include "lua/sol.hpp"

enum
{
	edbEntryStruct_type = 1,
	buildingLevelStruct_name = 1
};

struct settlementStruct;
struct buildingLevelCapability
{
public:
	int32_t capabilityType; //0x0000 0 = capability 3 = agent 4 = agentLimit 5 = recruitPool
	int16_t capabilityLvl; //0x0004
	int16_t bonus; //0x0006
	int32_t capabilityID; //0x0008
	float initial;
	float replenishment;
	float max;
	void* buildingLevelCondition; //0x0018
	struct buildingLevelCapability* nextCapability; //0x001C
	int getCapabilityID()
	{
		if (capabilityType == 0)
			return capabilityID;
		return -1;
	}
	int getAgentType()
	{
		if (capabilityType == 3 || capabilityType == 4)
			return capabilityID;
		return -1;
	}
}; //Size: 0x0020


struct recruitPool
{
public:
	int32_t capabilityType; //0x0000 0 = capability 3 = agent 4 = agentLimit 5 = recruitPool
	int16_t experienceOrLevel; //
	int16_t bonus; //0x0006
	int32_t unitID; //0x0006
	float initialSize;
	float gainPerTurn;
	float maxSize;
	void* buildingLevelCondition; //0x0018
	struct recruitPool* nextPool; //0x001C
	int getUnitID()
	{
		if (capabilityType == 5)
			return unitID;
		return -1;
	}
	void setUnitID(int newID)
	{
		unitID = newID;
	}
	int getUnitExperience()
	{
		if (capabilityType == 5)
			return experienceOrLevel;
		return -1;
	}
	void setUnitExperience(int16_t newExp)
	{
		experienceOrLevel = newExp;
	}
	int getAgentAmount()
	{
		if (capabilityType == 5)
			return experienceOrLevel;
		return -1;
	}
	void setAgentAmount(int16_t newAmount)
	{
		experienceOrLevel = newAmount;
	}
	int getAgentLimit()
	{
		if (capabilityType == 4)
			return experienceOrLevel;
		return -1;
	}
	void setAgentLimit(int16_t newLimit)
	{
		experienceOrLevel = newLimit;
	}
	int getAgentType()
	{
		if (capabilityType == 3 || capabilityType == 4)
			return unitID;
		return -1;
	}
	void setAgentType(int newType)
	{
		unitID = newType;
	}
}; //Size: 0x0020

struct buildingPicEntry
{
public:
	char* buildingPicPath; //0x0000
	int picHash;
}; //Size: 0x0008

//building draw info(pics, etc)
struct buildingLevel { /* (name, tga's, models, etc) */
	char* name;
	int32_t nameHash; //0x0004
	UNICODE_STRING*** buildingName[31]; //0x0008
	UNICODE_STRING*** buildingDescr[31]; //0x0084
	UNICODE_STRING*** buildingDescrShort[31]; //0x0100
	struct buildingPicEntry buildingPic[7]; //0x017C
	char* genericBuildingPic; //0x01B4
	int32_t genericBuildingPicHash; //0x01B8
	struct buildingPicEntry buildingPicConstructed[7]; //0x01BC
	char* genericBuildingPicConstructed; //0x01F4
	int32_t genericBuildingPicConstructedHash; //0x01F8
	struct buildingPicEntry buildingPicConstruction[7]; //0x01FC
	char* genericBuildingPicConstruction; //0x0234
	int32_t genericBuildingPicConstructionHash; //0x0238
	void* buildingLevelCondition; //0x023C
	int16_t buildCost; //0x0240
	uint8_t buildTime; //0x0242
	char pad_0243[1]; //0x0243
	uint32_t settlementMinLvl; //0x0244
	int8_t cityOneCastleMinusOne; //0x0248
	char pad_0249[3]; //0x0249
	gameStdVector<int8_t> converts; //0x024C
	struct buildingLevelCapability* capabilities; //0x025C
	struct recruitPool* recruitPools; //0x0260
	struct buildingLevelCapability* factionCapabilities; //0x0264
	void* upgrades; //0x0268
public:
	bool availableCastle()
	{
		return cityOneCastleMinusOne <= 0;
	}
	bool availableCity()
	{
		return cityOneCastleMinusOne >= 0;
	}
	std::string getName()
	{
		return std::string(name);
	}
	void setName(const std::string& newName)
	{
		gameStringHelpers::setHashedString(&name, newName.c_str());
	}
	std::string getLocalizedName(const int factionID);
	void setLocalizedName(const int factionID, const std::string& newName)
	{
		const auto nameMem = techFuncs::createGameClass<UNICODE_STRING**>();
		buildingName[factionID] = nameMem;
		gameStringHelpers::createUniString(*buildingName[factionID], newName.c_str());
	}
	std::string getLocalizedDescr(const int factionID);
	void setLocalizedDescr(const int factionID, const std::string& newName)
	{
		const auto nameMem  = techFuncs::createGameClass<UNICODE_STRING**>();
		buildingDescr[factionID] = nameMem;
		gameStringHelpers::createUniString(*buildingDescr[factionID], newName.c_str());
	}
	std::string getLocalizedDescrShort(const int factionID);
	void setLocalizedDescrShort(const int factionID, const std::string& newName)
	{
		const auto nameMem  = techFuncs::createGameClass<UNICODE_STRING**>();
		buildingDescrShort[factionID] = nameMem;
		gameStringHelpers::createUniString(*buildingDescrShort[factionID], newName.c_str());
	}
	std::string getBuildingPicPath(const int index)
	{
		if (index < 7)
			return buildingPic[index].buildingPicPath;
		return "";
	}
	void setBuildingPicPath(const int index, const std::string& newPath)
	{
		if (index < 7)
			gameStringHelpers::setHashedString(&buildingPic[index].buildingPicPath, newPath.c_str());
	}
	std::string getGenericBuildingPic()
	{
		return genericBuildingPic;
	}
	void setGenericBuildingPic(const std::string& newPath)
	{
		gameStringHelpers::setHashedString(&genericBuildingPic, newPath.c_str());
	}
	std::string getGenericBuildingPicConstructed()
	{
		return genericBuildingPicConstructed;
	}
	void setGenericBuildingPicConstructed(const std::string& newPath)
	{
		gameStringHelpers::setHashedString(&genericBuildingPicConstructed, newPath.c_str());
	}
	std::string getGenericBuildingPicConstruction()
	{
		return genericBuildingPicConstruction;
	}
	void setGenericBuildingPicConstruction(const std::string& newPath)
	{
		gameStringHelpers::setHashedString(&genericBuildingPicConstruction, newPath.c_str());
	}
	std::string getBuildingPicConstructedPath(const int index)
	{
		if (index < 7)
			return buildingPicConstructed[index].buildingPicPath;
		return "";
	}
	void setBuildingPicConstructedPath(const int index, const std::string& newPath)
	{
		if (index < 7)
			gameStringHelpers::setHashedString(&buildingPicConstructed[index].buildingPicPath, newPath.c_str());
	}
	std::string getBuildingPicConstructionPath(const int index)
	{
		if (index < 7)
			return buildingPicConstruction[index].buildingPicPath;
		return "";
	}
	void setBuildingPicConstructionPath(const int index, const std::string& newPath)
	{
		if (index < 7)
			gameStringHelpers::setHashedString(&buildingPicConstruction[index].buildingPicPath, newPath.c_str());
	}
	int getCapabilityNum()
	{
		int count = 0;
		const buildingLevelCapability* current = capabilities;
		while (current != nullptr)
		{
			count++;
			current = current->nextCapability;
		}
		return count;
	}
	int getPoolNum()
	{
		int count = 0;
		const recruitPool* current = recruitPools;
		while (current != nullptr)
		{
			count++;
			current = current->nextPool;
		}
		return count;
	}
	buildingLevelCapability* getCapability(const int index)
	{
		if (index < getCapabilityNum())
		{
			buildingLevelCapability* current = capabilities;
			for (int i = 0; i < index; i++)
			{
				current = current->nextCapability;
			}
			return current;
		}
		return nullptr;
	}
	recruitPool* getPool(const int index)
	{
		if (index < getPoolNum())
		{
			recruitPool* current = recruitPools;
			for (int i = 0; i < index; i++)
			{
				current = current->nextPool;
			}
			return current;
		}
		return nullptr;
	}
	int getFactionCapabilityNum()
	{
		int count = 0;
		const buildingLevelCapability* current = factionCapabilities;
		while (current != nullptr)
		{
			count++;
			current = current->nextCapability;
		}
		return count;
	}
	buildingLevelCapability* getFactionCapability(const int index)
	{
		if (index < getFactionCapabilityNum())
		{
			buildingLevelCapability* current = factionCapabilities;
			for (int i = 0; i < index; i++)
			{
				current = current->nextCapability;
			}
			return current;
		}
		return nullptr;
	}
	void addCapability(int capability, int16_t value, bool bonus,  const std::string& condition);
	void addRecruitPool(int eduIndex, float initialSize, float gainPerTurn, float maxSize, int16_t exp,  const std::string& condition);
	void removeBuildingPool(int index);
	void removeBuildingCapability(int index);
	void removeBuildingFactionCapability(int index);
	void addFactionCapability(int capability, int16_t value, bool bonus,  const std::string& condition);
};

//building data
struct edbEntry { /* SETTLEMENT_CONDITION_SETTLEMENT_FACTION */
	int vfTable;
	int factionBitField;
	int factionsRaw;
	int cultures;
	bool negate;
	bool allFactions;
	char field_12[2];
	int32_t buildingID; //0x0014
	int32_t classification; //0x0018
	int material;
	DWORD buildingPic[13];
	DWORD eopBuildingID; //just using this mem to store if this is eop building entry
	int genericPic;
	int genericPicHash;
	int8_t isWallBuilding; //0x0060
	int8_t isPort; //0x0061
	int8_t isCoreBuilding; //0x0062
	int8_t isTemple; //0x0063
	int32_t religionID; //0x0064
	int8_t isHinterland; //0x0068
	int8_t isFarm; //0x0069
	char pad_006A[2]; //0x006A
	UNICODE_STRING** localizedName;
	gameStdVector<stringWithHash> converts;
	char* type; /* type of building (core_building,barracks)  */
	int typeHash;
	struct buildingLevel* levels; /* name of building(stone_wall), tga's, etc */
	uint32_t *levelFactions;
	int32_t buildingLevelCount; //0x0090
	void *plugins;
	int pluginCount;
public:
	buildingLevel* getBuildingLevel(const int index)
	{
		if (index < 0 || index >= buildingLevelCount)
			return nullptr;
		return &levels[index];
	}
};

struct hiddenResource
{
public:
	char* hiddenResName; //0x0000
	int32_t hiddenResNameHash; //0x0004
}; //Size: 0x0008

struct lookupVariant
{
public:
	gameList<stringWithHash> names;
	char *name; //0x0014
	int32_t nameHash; //0x0018
}; //Size: 0x001C

class eopBuildEntry
{
public:
	edbEntry* baseEntry;
	eopBuildEntry(const edbEntry* oldEntry, const int newIndex)
	{
		baseEntry = techFuncs::createGameClass<edbEntry>();
		*baseEntry = *oldEntry;
		eopBuildId = newIndex;
		baseEntry->eopBuildingID = newIndex;
	}
	int eopBuildId;
};

class buildEntryDB
{
public:
	static edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex);
	static edbEntry* getEopBuildEntry(int idx);
	static void addCaps(buildingLevel* eopLevel, const buildingLevel* oldLevel);
	static void addPools(buildingLevel* eopLevel, const buildingLevel* oldLevel);
public:
	static std::vector<std::shared_ptr<eopBuildEntry>>  eopEdb;
};

struct exportDescrBuildings
{
public:
	struct hiddenResource hiddenResources[64]; //0x0000
	int32_t hiddenResourceCount; //0x0200
	int32_t field_204; //0x0204
	void *stringLookupTable; //0x0208
	int32_t field_20c; //0x020C
	int32_t field_210; //0x0210
	int32_t field_214; //0x0214
	void *array_218; //0x0218
	int32_t array_218Next; //0x021C
	int32_t array_218NextPRev; //0x0220
	int32_t array_218Size; //0x0224
	int32_t array_218SizeNum; //0x0228
	gameList<lookupVariant> lookupVariants; //0x022C
	void *stringTable1; //0x0240
	void *stringTable2; //0x0244
	struct edbEntry *port; //0x0248
	struct edbEntry *castlePort; //0x024C
	struct edbEntry *coreCityBuilding; //0x0250
	struct edbEntry *coreCastleBuilding; //0x0254
	gameList<void> battleBuildingList; //0x0258
	void *array_26c; //0x026C
	void*array_26cNext; //0x0270
	void*array_26cPrev; //0x0274
	int32_t array_26cSize; //0x0278
	int32_t array_26cNum; //0x027C
	gameList<edbEntry> buildings; //0x0280
	void *array_294; //0x0294
	void *array_294NExt; //0x0298
	void*array_294Prev; //0x029C
	int32_t array_294Size; //0x02A0
	int32_t array_294Num; //0x02A4
	gameList<void> buildingBattleWalls; //0x02A8
	void *uniRepairString1; //0x02BC
	void*uniRepairString2; //0x02C0
	void *uniRepairString3; //0x02C4
	void*uniRepairString4; //0x02C8
	int getHiddenResourceIndex(const char* name)
	{
		for (int i = 0; i < hiddenResourceCount; i++)
		{
			if (strcmp(hiddenResources[i].hiddenResName, name) == 0)
				return i;
		}
		return -1;
	}
	int getBuildingNum()
	{
		return buildings.size();
	}
	edbEntry* getBuildingByID(const int id)
	{
		if (const auto entry = buildEntryDB::getEopBuildEntry(id); entry)
			return entry;
		const int buildingNum = buildings.size();
		for (int i = 0; i < buildingNum; i++)
		{
			if (const auto entry = buildings.get(i); entry->buildingID == id)
				return entry;
		}
		return nullptr;
	}
	edbEntry* getBuildingByName(const std::string& name);

}; //Size: 0x0350

class eopHiddenResources
{
public:
	static int getHiddenResourceIndex(const std::string& name);
	static void addHiddenResource(const std::string& name);
	static void addHiddenResourceWithId(const std::string& name, int id);
	static void addHiddenResourceToRegion(int regionId, const std::string& name);
	static void removeHiddenResourceFromRegion(int regionId, const std::string& name);
	static void addHiddenResourceToRegionIndex(const std::string& name, int id);
	static bool hasHiddenResource(int regionId, int id);
	static bool isInitialized() { return m_Initialized; }
	static void setInitialized(const bool value) { m_Initialized = value; }
	static void initialize();
private:
	static std::array<std::vector<int>, 200> m_HiddenResources;
	static unordered_map<std::string, int> m_HiddenResourcesLookup;
	static unordered_map<std::string, std::vector<int>> m_NamesToIndexes;
	static bool m_Initialized;
};

namespace eopBuildings
{
    void setBuildingPic(const edbEntry* entry, const char* newPic, int level, int cultureID);
    void setBuildingPicConstructed(const edbEntry* entry, const char* newPic, int level, int cultureID);
    void setBuildingPicConstruction(const edbEntry* entry, const char* newPic, int level, int cultureID);
    void setBuildingLocalizedName(const edbEntry* entry, const char* newName, int level, int factionID);
    void setBuildingLocalizedDescr(const edbEntry* entry, const char* newName, int level, int factionID);
    void setBuildingLocalizedDescrShort(const edbEntry* entry, const char* newName, int level, int factionID);
    void addBuildingCapability(const edbEntry* entry, int level, int capability, int16_t value, bool bonus);
    void addBuildingPool(const edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp, const char* condition);
    void removeBuildingCapability(const edbEntry* entry, int level, int index);
    void removeBuildingPool(const edbEntry* entry, int level, int index);
    buildingLevelCapability* getBuildingCapability(const edbEntry* entry, int level, int index);
    recruitPool* getBuildingPool(const edbEntry* entry, int level, int index);
    int getBuildingCapabilityNum(const edbEntry* entry, int level);
    int getBuildingPoolNum(const edbEntry* entry, int level);
    void addCaps(buildingLevel* oldLevel, buildingLevel* eopLevel, int lvlIdx);
    void addPools(buildingLevel* oldLevel, buildingLevel* eopLevel, int lvlIdx);
    exportDescrBuildings* getEdb();
    
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
            gameStringHelpers::setHashedString(&bInfo->name, newS.c_str());
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
            gameStringHelpers::setHashedString(&edbEntry->type, newS.c_str());
    }

#pragma endregion
};



namespace buildingHelpers
{
	void addToLua(sol::state& luaState);
	int getBuildingId(const std::string& name);
	int getBuildingLevelId(const std::string& name);
	int getBuildingLevelPos(const std::string& name);
}