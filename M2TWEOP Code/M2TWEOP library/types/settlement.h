#pragma once
#include <filesystem>
#include <string>

#include "eopBuildings.h"
#include "gameStringHelpers.h"
#include "functionsOffsets.h"
#include "realGameTypes.h"
#include "strategyMap.h"
#include "lua/sol.hpp"



enum class productionBias
{
	frontier,
	conflict,
	frontline,
	abandon,
	increaseGarrison,
	trade,
	tax,
	spies,
	assassins,
	merchants,
	priests,
	diplomats,
	ships,
	qualityUnits,
	buildStrengthLimit
};

struct settlementCapability
{
public:
	int16_t value; //0x0000
	int16_t bonus; //0x0002
}; //Size: 0x0004

struct recruitCapRome
{
public:
	char pad[8];
}; //Size: 0x0008

struct recruitmentCapability
{
public:
	int32_t eduIndex; //0x0000
	int32_t xp; //0x0004
	float initialSize; //0x0008
	float replenishRate; //0x000C
	float maxSize; //0x0010
}; //Size: 0x0014

struct settlementRecruitmentPool
{
public:
	int32_t eduIndex; //0x0000
	float availablePool; //0x0004
}; //Size: 0x0008


struct recruitRome
{
	char pad[8];
};

struct capabilityStruct
{
public:
	struct settlementCapability settlementCapabilities[57]; //0x0000
	struct settlementCapability settlementCapabilitiesAgent[6]; //0x00E4
	struct settlementCapability settlementCapabilitiesAgentLimit[6]; //0x00FC
	struct settlementCapability settlementCapabilitiesAncillaryROME[4]; //0x0114
	struct recruitRome settlementCapabilitiesRecruitROME[64]; //0x0124
	int32_t recruitCapabilityCount; //0x0324
	struct recruitmentCapability recruitmentCapabilities[64]; //0x0328
	int32_t recruitmentCapabilityCount; //0x0828
}; //Size: 0x082C


struct unitRQ {
public:
	struct eduEntry* entry{}; //0x0000
	int32_t factionType{}; //0x0004
	struct settlementStruct* settlement{}; //0x0008
	int8_t frozen{}; //0x000C
	int8_t recruitType{}; //0x000D
	int8_t experience{}; //0x000E
	int8_t armourUpg{}; //0x000F
	int8_t weaponUpgrade{}; //0x0010
	int8_t weaponUpgradeSec{}; //0x0011
	int8_t turnsTrainedAlready{}; //0x0012
	int8_t percentFinished{}; //0x0013
	int8_t turnsToTrain{}; //0x0014
	int8_t byte_15{}; //0x0015
	int16_t cost{}; //0x0016
	int16_t soldierCount{}; //0x0018
	int16_t turnNumber{}; //0x001A
	int8_t isValid{}; //0x001C
	int8_t purchased{}; //0x001D
	int16_t availability{}; //0x001E
	int8_t isMercenary{}; //0x0020
	char pad_0021[3]{}; //0x0021

public:
	unitRQ();
	
	eduEntry* getUnitEntry();
	
	unit* getUnit()
	{
		if (recruitType >= 3)
			return reinterpret_cast<unit*>(this->entry);
		return nullptr;
	}

	int getAgentType()
	{
		if (recruitType == 2)
			return reinterpret_cast<int>(entry);
		return -1;
	}

	void setUnitEntry(eduEntry* newEntry)
	{
		if (recruitType < 2)
			entry = newEntry;
	}

	void setAgentType(const int type)
	{
		if (recruitType == 2)
			entry = reinterpret_cast<eduEntry*>(type);
	}
	
	void setUnit(unit* unit)
	{
		if (recruitType >= 3)
			entry = reinterpret_cast<eduEntry*>(unit);
	}
}; //Size: 0x0024

//building
struct building { /* building structure */
	spyingInfo spyInfo;
	DWORD buildingVer1;
	DWORD buildingVer2;
	DWORD buildingVer3;
	struct edbEntry* edbEntry; /* many info (type, name, etc), exportDescrBuildingEntry */
	int8_t level;
	int8_t plugins[10];
	int8_t plugins2[10];
	int factionID;
	int hp; /* health points of the building */
	struct settlementStruct* settlement;
	int constructType;
	int constructionPercent;
	void setBuildingHealth(const int newHealth)
	{
		const int newHp = newHealth - hp ;
		GAME_FUNC(void(__thiscall*)(building*, int), repairBuilding)(this, newHp);
	}
	int getBuildingHealth()
	{
		return hp;
	}
};


enum class constructionType
{
	upgrade,
	construction,
	repair,
	demolition,
	convert,
	none
};

struct buildingInQueue { /* building in settlement queue */
	int constructType{};
	struct edbEntry* edbEntry{};
	struct building* existsBuilding{}; /* 0 if building dont exist yet */
	struct settlementStruct* settlement{};
	int currentLevel{};
	int pastLevel{};
	int buildCost{};
	int repairPerTurn{};
	int turnsToBuild{};
	int buildTurnsPassed{};
	int percentBuild{};
	int16_t turnNumber{};							
	bool isValid;																
	bool isFrozen;
	int rawCost{};
	int rawTurns{};
	bool bought;
	char pad[3];
};

struct buildingsQueue {
	uint16_t summaryFlags[128];
	int16_t somethingTemple;
	int16_t conversion;
	gameCircularBuffer<buildingInQueue, 6> buildingQueue;
	int constructionPoints;
	struct settlementStruct *settlement;
	struct factionStruct **faction;
	int getBuildingQueueSize()
	{
		return buildingQueue.num;
	}
};

using availableBuildings = gameList<buildingInQueue>;
using recruitmentOptions = gameStdVector<unitRQ>;

struct guild
{
	char* name; //0x0000
	DWORD nameHash; //0x0004
	uint8_t id; //0x0008
	char pad_0009[3]; //0x0009
	edbEntry* building; //0x000C
	int32_t level1Threshold; //0x0010
	int32_t level2Threshold; //0x0014
	int32_t level3Threshold; //0x0018
	gameStdVector<int8_t> excludedGuilds; //0x001C
	gameStdVector<std::pair<stringWithHash,int>> scores;
	gameStdVector<const char*> unresolvedExclusions;
}; //Size: 0x004C

struct settlementBuildingOptions
{
	std::vector<std::shared_ptr<buildingInQueue>> constructionOptions;
	int count;
	int totalCost;
	int totalTime;
	int turn;
	int hash;
	int settIndex;
};

struct settlementRecruitmentOptions
{
	std::vector<std::shared_ptr<unitRQ>> recruitOptions;
	int count;
	int totalCost;
	int totalTime;
	int turn;
	int hash;
	int settIndex{};
};

struct settlementPolicies
{
	int32_t regionID;
	int32_t autoManageBuildPolicy;
	int32_t secondaryPolicy;
	int8_t autoManagedConstruction;
	int8_t autoManagedRecruitment;
	uint8_t settlementIndex;
	uint8_t pad_000E;
};

struct aiProductionItem
{
	settlementStruct* settlement;
	unitRQ recruitItem;
	buildingInQueue buildItem;
	int priority;
	int cost;
	bool isBuilding;
	bool isValid;
	char pad[2];
};

struct aiProductionController
{
	void* vftable /*VFT*/;
	struct aiFaction* aiFaction;
	int regionID;
	struct settlementStruct* settlement;
	int32_t autoManagePolicy;
	int secondaryPolicy;
	int extraBias[15];
	int extraX;
	int extraY;
	int priority;
	int notControlledDuration;
	bool isAutoManaged;
	bool isAutoManagedRecruitment;
	bool isAutoManagedConstruction;
	bool isAutoManagedTaxes;
	int8_t hasGrowthBuildPolicy;
	char pad_x[3];
	int religionBias[10];
	aiProductionItem bestBuilding;
	int buildingBias[57];
	int ancillaryBias[4];
	int agentBias[6];
	int unitBias[11];
	int coreBias;
	bool bestBuildingBuilt;
	bool buildingFinished;
	bool bestTroopBuilt;
	int8_t pad22;
	aiProductionItem bestTroop;
	int spyBias;
	int assassinBias;
	int diplomatBias;
	int admiralBias;
	int priestBias;
	int merchantBias;
	int recruitBias[11];
	void setConstructionValueSett(int type, int value)
	{
		buildingBias[type] = value;
	}
	void incConstructionValueEnum(buildingCapabilities type, int value)
	{
		buildingBias[static_cast<int>(type)] += value;
	}
	void setConstructionValueEnum(buildingCapabilities type, int value)
	{
		buildingBias[static_cast<int>(type)] = value;
	}
	void setRecruitmentValueSett(int type, int value)
	{
		recruitBias[type] = value;
	}
	void incRecruitmentValue(int type, int value)
	{
		recruitBias[type] += value;
	}
	void incConstructionUnitValue(int type, int value)
	{
		unitBias[type] += value;
	}
	void setExtraBias(int type, int value)
	{
		extraBias[type] = value;
	}
	int getExtraBias(int type)
	{
		return extraBias[type];
	}
	int getConstructionValueSett(int type)
	{
		return buildingBias[type];
	}
	int getRecruitmentValueSett(int type)
	{
		return recruitBias[type];
	}
	void setBuildPoliciesAndTaxLevel(int policy, int recruitPolicy);
	void setSettlementTaxLevel();
	void setPriorities();
	void underControlCheck(const factionStruct* faction);
	void resetExtraBias()
	{
		for (int& bias : extraBias)
			bias = 0;
		extraX = 0;
		extraY = 0;
	}
};


struct settlementStats
{
	struct settlementStruct* settlement;
	int32_t PopGrowthBaseFarm;
	int32_t PopGrowthFarms;
	int32_t PopGrowthHealth;
	int32_t PopGrowthBuildings;
	int32_t PopGrowthTaxBonus;
	int32_t PopGrowthEntertainment;
	int32_t PopGrowthTrade;
	int32_t PopGrowthGovernorInfluence;
	int32_t PopGrowthSqualor;
	int32_t PopGrowthPlague;
	int32_t PopGrowthTaxPenalty;
	int32_t population;
	int32_t PublicOrderGarrison;
	int32_t PublicOrderLaw;
	int32_t PublicOrderBuildingsEntertainment;
	int32_t PublicOrderGovernorInfluence;
	int32_t PublicOrderTaxBonus;
	int32_t PublicOrderTriumph;
	int32_t PublicOrderPopulationBoom;
	int32_t PublicOrderEntertainment;
	int32_t PublicOrderHealth;
	int32_t PublicOrderReligiousOrder;
	int32_t PublicOrderFear;
	int32_t PublicOrderGlory;
	int32_t PublicOrderSqualor;
	int32_t PublicOrderDistanceToCapital;
	int32_t PublicOrderNoGovernance;
	int32_t PublicOrderTaxPenalty;
	int32_t PublicOrderUnrest;
	int32_t PublicOrderBesieged;
	int32_t PublicOrderBlockaded;
	int32_t PublicOrderCulturalUnrest;
	int32_t PublicOrderExcommunication;
	int32_t PublicOrder;
	int32_t FarmsIncome;
	int32_t TaxesIncome;
	int32_t MiningIncome;
	int32_t TradeIncome;
	int32_t DiplomaticIncome;
	int32_t DemolitionIncome;
	int32_t LootingIncome;
	int32_t BuildingsIncome;
	int32_t AdminIncome;
	int32_t wagesExpense;
	int32_t upkeepExpense;
	int32_t ConstructionExpense;
	int32_t RecruitmentExpense;
	int32_t DiplomaticExpense;
	int32_t CorruptionExpense;
	int32_t EntertainmentExpense;
	int32_t DevastationExpense;
	int32_t TotalIncomeWithoutAdmin;
	int32_t* religionPercentages;
	int32_t religionPercentagesSize;
	int32_t religionPercentagesNum;
	int32_t majorityReligionID;
};

struct settlementStatsManager
{
	factionStruct* faction;
	settlementStruct* settlement;
	int32_t baseFertilityValue; //0x16F8
	int32_t rebelFactionChance; //0x16FC
	int32_t turmoil; //0x1700
	int32_t popGainedDisbands; //0x1704
	int32_t popGainedSlaves; //0x1708
	int32_t popLostRecruits; //0x170C
	int32_t popLostDisaster; //0x1710
	int32_t popLostRiots; //0x1714
	int32_t popLostExtermination; //0x1718
	int32_t popLostEnslaved; //0x171C
	int32_t demolitionIncome; //0x1720
	int32_t diplomacyIncome; //0x1724
	int32_t lootingIncome; //0x1728
	int32_t wages; //0x172C
	int32_t upkeep; //0x1730
	int constructionExpense; //0x1734
	int recruitmentExpense; //0x1738
	int diplomacyExpense;
	int disasterExpense;
	int entertainmentExpense;
	settlementStats settlementStatsLastTurn;
	settlementStats settlementStats;
	bool doingTempCalc; //0x1910
	char pad[3]{}; //0x1911
	void recalculate(bool recalculateFacEconomy);
	void setPopulation(int newPop);
};

struct buildingDamage
{
	int perimeter;
	bool breachable;
	int	type;
	int8_t health;
	char pad[3];
};

struct settlementBuildings
{
	struct building* buildingsByIndex[128];
	struct building* buildings[128];
	int buildingsNum; /* number of the buildings in the settlement */
	int8_t hasReligiousBuilding;
	struct building *guildList;
};

struct stackCapabilities
{
	struct settlementStruct* settlement;
	struct factionStruct* faction;
	int factionId;
	int factionId2;
	int factionId3;
	struct capabilityStruct* capabilities;
};

//settlement
struct settlementStruct {
	DWORD* Vtable;
	void* nextObject;
	void* prevObject;
	UINT32 xCoord;
	UINT32 yCoord;
	int fade;
	bool render;
	char pad0[3];
	float opacity;
	bool highlighted;
	char pad1[3];
	trackedObject trackObject;
	void* crusadeFinishedVtbl;
	float stratPosX;
	float stratPosZ;
	float stratPosY;
	void* trackedPointerArmyVtable;
	struct armyStruct* army;
	struct oneSiege sieges[8];
	int8_t siegeNum; //0x0088
	char pad_0089[3]; //0x0089
	int32_t maxHoldoutTurns;
	int32_t siegeDuration;
	int32_t siegeCasualties;
	int32_t preSiegePopulation;
	int32_t factionTradedFrom;
	int32_t plagueDuration;
	bool plagued;
	bool salliedOut; //0x00A5
	bool gatesAreOpened;
	bool readyToSurrender;
	struct crusade* takenByCrusade;
	struct descrRebelEntry* descrRebel; //0x00AC
	int32_t subFactionID; //0x00B0
	spyingInfo spyInfo;
	char settlementLostNotification[0x10];
	char settlementTakenNotification[0x10];
	char settlementLootedNotification[0x10];
	char settlementSackedNotification[0x10];
	char settlementExterminatedNotification[0x10];
	char settlementSurrenderedNotification[0x10];
	DWORD settVerification;
	DWORD settVerificationPos;
	DWORD settVerificationPos2;
	struct cultureSettlement* model;
	struct settMod* model2; //0000014C
	int cultureID;
	float smthingPosX;
	float smthingPosY;
	struct portBuildingStruct* port;
	char* name; /* internal name of settlement */
	int nameCrypt;
	UNICODE_STRING** localizedName;
	struct factionStruct* faction; /* faction of the owner */
	int previousOwner;
	int8_t factionPercentages[32];
	int level; /* level of the settlement/castle */
	int fac_creatorModNum;
	int lastHordeFaction;
	BYTE isCastle; /* castle or settlement */
	undefined field21_0x1a5[1];
	int16_t turnsOwned; /* start at 10 for settlements owned at game start without specification in descr_strat */
	int regionID; /* number of region */
	int32_t factionID; //0x01AC
	int32_t yearFounded; //0x01B0
	int32_t minorSettlementIndex; //0x01B4
	int32_t moneySpentConstruction;//0x01B8
	int32_t moneySpentTraining;//0x01BC
	int32_t moneySpentRecruitment;//0x01C0
	char agentRecruitnotification[16];
	gameCircularBuffer<unitRQ, 9> unitQueue;
	int32_t recruitmentPoints;
	struct settlementStruct *thisBeforeBuildings;
	struct factionStruct **pointerToSelfFaction2;
	struct buildingsQueue buildingsQueueArray;
	struct building* buildingsByIndex[128];
	struct building* buildings[128];
	int buildingsNum; /* number of the buildings in the settlement */
	int8_t hasReligiousBuilding;
	struct building *guildList;
	struct settlementStruct *thisBeforeLongUnkMem;
	factionStruct **pointerToSelfFaction4;
	buildingDamage *buildingDamages;
	int32_t buildingDamagesSize;
	int32_t buildingDamagesNum;
	int32_t *buildingDamageCodes;
	int32_t buildingDamageCodesSize;
	int32_t buildingDamageCodesNum;
	bool partialConstructionsCreated;
	char pad2[3];
	struct building* partialBuildingsIndex[128];
	struct building* partialBuildings[128];
	int32_t partialBuildingsNum;
	int8_t hasReligiousBuildingPartial;
	int8_t byte_de9[3];
	struct building *partialGuild;
	int32_t rallyPointX; //0x0DF0
	int32_t rallyPointY; //0x0DF4
	void* rallyPointPointer; //0x0DF8
	struct aiProductionController* aiProductionController; //0x0DFC
	int32_t plagueDeaths; //0x0E00
	int8_t scriptRebel; //0x0E04
	bool isProvokedRebellion;//0x0E05
	bool isMinorSettlement;
	int8_t pad3[1];
	int timeSinceLastRebellion;
	int entertainmentType;
	bool isCapital; //0x0E10
	bool isTradeBlocked;
	int8_t gamesMessage;
	int8_t racesMessage; 
	int8_t governorLeft; 
	int8_t pad4[3];
	int governorDuration;
	void* governorTrackedVtbl;
	struct character* governor;//0x0E20
	void* governorTrackedVtbl2;
		struct character* lastGovernor;//0x0E20
	void* governorTrackedVtbl3;
	struct character* portAdmiral;//0x0E20
	int32_t publicHealth; //0x0E34
	int32_t publicOrderLastTurn; //0x0E38
	int32_t lastPopulation; //0x0E3C
	int32_t harvestSuccess; //0x0E40
	struct resourceStruct** resources;
	int32_t resourceSize; //0x0E48
	int resourcesNum;
	int32_t settlementTaxLevel; //0x0E50
	struct capabilityStruct capabilities; //0x0E54
	int8_t inEndOfRound; //0x1680
	int8_t expandMessage; //0x1681
	char pad_1682[2]; //0x1682
	struct settlementStruct* thisSett; //0x1684
	struct settlementRecruitmentPool* recruitmentPools; //0x1688
	int32_t recruitmentPoolsSize; //0x168C
	int32_t recruitmentPoolCount; //0x1690
	struct settlementRecruitmentPool* disbandPools;
	int32_t disbandPoolsSize; 
	int32_t disbandPoolsCount;
	int8_t freezeRecruitmentPool; //0x16A0
	char pad_16A1[3]; //0x16A1
	int32_t spiesInRecruitmentQueue; //0x16A4
	int32_t assassinsInRecruitmentQueue; //0x16A8
	int32_t diplomatsInRecruitmentQueue; //0x16AC
	int32_t admiralsInRecruitmentQueue; //0x16B0
	int32_t merchantsInRecruitmentQueue; //0x16B4
	int32_t priestsInRecruitmentQueue; //0x16B8
	int32_t generalsInRecruitmentQueue; //0x16BC
	char pad_16C0[4]; //0x16BC
	int* guildStandings;
	int capturedSettlementOption;
	basicStringGame packagePath;
	float foundingConversionRate;
	float ownerConversionRate;
	settlementStatsManager stats;
public:
	siegeS* getSiege(int index)
	{
		return sieges[index].siege;
	}
	resourceStruct* getResource(int index)
	{
		return resources[index];
	}
	building* getBuilding(int index)
	{
		return buildings[index];
	}
	settlementCapability* getSettlementCapability(int capabilityType)
	{
		return &capabilities.settlementCapabilities[capabilityType];
	}
	settlementCapability* getSettlementCapabilityEnum(buildingCapabilities capabilityType)
	{
		return &capabilities.settlementCapabilities[static_cast<int>(capabilityType)];
	}
	settlementCapability* getAgentCapability(int agentType)
	{
		return &capabilities.settlementCapabilitiesAgent[agentType];
	}
	settlementCapability* getAgentLimitCapability(int agentType)
	{
		return &capabilities.settlementCapabilitiesAgentLimit[agentType];
	}
	recruitmentCapability* getRecruitmentCapability(int index)
	{
		return &capabilities.recruitmentCapabilities[index];
	}
	int getRecruitmentCapabilityNum()
	{
		return capabilities.recruitmentCapabilityCount;
	}
	int getPopulation()
	{
		return stats.settlementStats.population;
	}
	fortStruct* isFort();
	void setPopulation(const int newPop)
	{
		const auto pop = newPop;
		stats.setPopulation(pop);
	}
	int getFortificationLevel();
	settlementRecruitmentPool* getSettlementRecruitmentPool(int index)
	{
		return &recruitmentPools[index];
	}
	int getTurmoil()
	{
		return stats.turmoil;
	}
	void setTurmoil(const int turmoil)
	{
		stats.turmoil = turmoil;
	}
	int getBaseFertility()
	{
		return stats.baseFertilityValue;
	}
	void setBaseFertility(const int fertility)
	{
		stats.baseFertilityValue = fertility;
	}
	int getRebelFactionChance()
	{
		return stats.rebelFactionChance;
	}
	void setRebelFactionChance(const int chance)
	{
		stats.rebelFactionChance = chance;
	}
	settlementStats* getSettlementStats()
	{
		return &stats.settlementStats;
	}
	settlementStats* getSettlementStatsLastTurn()
	{
		return &stats.settlementStatsLastTurn;
	}
	int getGuildStanding(int index)
	{
		return guildStandings[index];
	}
	int getSettlementValue();
	int getMinorSettlementIndex()
	{
		return minorSettlementIndex;
	}
	unitRQ* getUnitInQueue(int index)
	{
		return &unitQueue[index];
	}
	int getUnitQueueSize()
	{
		return unitQueue.num;
	}
	void setGuildStanding(const int index, const int amount)
	{
		guildStandings[index] = amount;
	}
	void recalculate(const bool recalculateFacEconomy)
	{
		stats.recalculate(recalculateFacEconomy);
	}
	bool canConstructBuilding(const edbEntry* entry, int lvl);
	bool isPlayerControlled();
	bool isEnemyToFaction(factionStruct* otherFac);
	bool isAllyToFaction(factionStruct* otherFac);
	int characterCount();
	character* getCharacter(int index);
	building* buildingPresent(const std::string& buildingName);
	bool buildingPresentMinLevel(const std::string& levelName, bool exact);
};

struct eopSettlementData
{
	int settlementID = -1;
	int regionID = -1;
	std::array<int, 128> eopBuildingEntries{};
	int modelId = -1;
	std::string regionName;
	std::string settlementLabel;
	std::string regionRebelsName;
	std::vector<std::string> addedHiddenResources;
	std::vector<std::string> removedHiddenResources;
	nlohmann::json serialize()
	{
		nlohmann::json json;
		json["eopBuildingEntries"] = eopBuildingEntries;
		json["modelId"] = modelId;
		json["regionName"] = regionName;
		json["settlementLabel"] = settlementLabel;
		json["regionRebelsName"] = regionRebelsName;
		json["settlementID"] = settlementID;
		json["regionID"] = regionID;
		for (const auto& resource : addedHiddenResources)
		{
			json["addedHiddenResources"].push_back(resource);
		}
		for (const auto& resource : removedHiddenResources)
		{
			json["removedHiddenResources"].push_back(resource);
		}
		return json;
	}
	void deserialize(const nlohmann::json& json)
	{
		eopBuildingEntries = json["eopBuildingEntries"].get<std::array<int, 128>>();
		modelId = json["modelId"];
		regionName = json["regionName"];
		settlementLabel = json["settlementLabel"];
		regionRebelsName = json["regionRebelsName"];
		settlementID = json["settlementID"];
		regionID = json["regionID"];
		addedHiddenResources.clear();
		removedHiddenResources.clear();
		for (const auto& resource : json["addedHiddenResources"])
		{
			addedHiddenResources.push_back(resource.get<std::string>());
		}
		for (const auto& resource : json["removedHiddenResources"])
		{
			removedHiddenResources.push_back(resource.get<std::string>());
		}
	}
};

struct settlementConditionContext
{
	settlementStruct *sett;
	struct capabilityStruct *caps;
	int settFactionId;
	int buildingFactionId;
	int forcedFactionId;
	capabilityStruct fallbackCapabilities;
};

settlementConditionContext* createSettlementConditionContext(settlementStruct* sett);

namespace settlementHelpers
{
	void setSettlementOwner(settlementStruct* sett, factionStruct* newOwner, bool convertGarrison);
	void changeOwner(settlementStruct* sett, factionStruct* newOwner);
	settlementStruct* createSettlement(factionStruct* faction, int xCoord, int yCoord, const std::string& name,
		int level, bool castle);
	void upgradeSettlement(settlementStruct* sett);
	std::string getSettlementName(settlementStruct* sett);
	settlementStruct* getSettlementByRegionID(int index);
	void changeSettlementName(settlementStruct* sett, const char* newName);
	float getReligion(settlementStruct* sett, int index);
	void setReligion(settlementStruct* sett, int index, float value);
	void addToLua(sol::state& luaState);
	building* createBuilding(settlementStruct* sett, const std::string& buildingLevelId);
	void destroyBuilding(settlementStruct* sett, const char* typeName, bool isReturnMoney);
	
	bool addBuildingToQueue(buildingInQueue* building);
	buildingInQueue* getBuildingInQueue(buildingsQueue* queue, int position);
	availableBuildings* getAvailableBuildingsMem();
	availableBuildings* getAvailableBuildings(settlementStruct* sett);
	int makeBuildOptionsHash(const settlementStruct* sett);
	settlementBuildingOptions* getBuildingOptions(settlementStruct* sett);
	buildingInQueue* getBuildingOptionFromDb(const settlementBuildingOptions* list, int index);
	void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp, std::string condition);
	bool addUnitToQueue(unitRQ* unit);
	recruitmentOptions* getAvailableUnitsMem();
	recruitmentOptions* getAvailableUnits(settlementStruct* sett);
	recruitmentOptions* getAvailableRetrainingUnits(settlementStruct* sett);
	int makeRecruitOptionsHash(const settlementStruct* sett);
	settlementRecruitmentOptions* getRecruitOptions(settlementStruct* sett);
	unitRQ* getRecruitOptionFromDb(const settlementRecruitmentOptions* list, const int index);

#define settlementStruct_name 1
#define guild_name 1

	//settlement
	template <char fieldIndex>
	std::string getStringProperty(const settlementStruct* sett)
	{
		char* retS = nullptr;
		if (fieldIndex == settlementStruct_name)
		{
			retS = sett->name;
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
	void setStringProperty(settlementStruct* sett, std::string newS)
	{
		if (fieldIndex == settlementStruct_name)
			gameStringHelpers::setHashedString(&sett->name, newS.c_str());
	}

	
	template <char fieldIndex>
	std::string getGuildStringProperty(const guild* guild)
	{
		char* retS = nullptr;
		if (fieldIndex == guild_name)
		{
			retS = guild->name;
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
	void setGuildStringProperty(guild* guild, std::string newS)
	{
		if (fieldIndex == guild_name)
			gameStringHelpers::setHashedString(&guild->name, newS.c_str());
	}
};


class eopSettlementDataDb
{
public:
	eopSettlementDataDb()
	{
		eopSettData = std::make_shared<std::vector<std::shared_ptr<eopSettlementData>>>();
	}
	void newGameLoaded();
	std::shared_ptr<eopSettlementData> getSettlementData(int regionId, int settlementId);
	
	static eopSettlementDataDb* get()
	{
		return instance.get();
	}
	std::shared_ptr<std::vector<std::shared_ptr<eopSettlementData>>> eopSettData{};
	static std::shared_ptr<eopSettlementDataDb> instance;
	nlohmann::json serialize()
	{
		nlohmann::json json;
		for (auto& data : *eopSettData)
		{
			json.push_back(data.get()->serialize());
		}
		return json;
	}
	void deserialize(const nlohmann::json& json)
	{
		eopSettData = std::make_shared<std::vector<std::shared_ptr<eopSettlementData>>>();
		for (const auto& item : json)
		{
			std::shared_ptr<eopSettlementData> data = std::make_shared<eopSettlementData>();
			data->deserialize(item);
			eopSettData->push_back(data);
		}
	}
	void clearData()
	{
		eopSettData = std::make_shared<std::vector<std::shared_ptr<eopSettlementData>>>();
		m_Loaded = false;
	}
	std::string onGameSave();
	void onGameLoad(const std::vector<std::string>& filePaths);
	void onGameLoaded();
private:
	bool m_Loaded = false;
};
