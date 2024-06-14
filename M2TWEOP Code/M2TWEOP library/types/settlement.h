#pragma once
#include <string>

#include "fastFunctsHelpers.h"
#include "functionsOffsets.h"
#include "realGameTypes.h"
#include "smallFuncs.h"
#include "stackStructHelpers.h"

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
	
	eduEntry* getUnitEntry()
	{
		if (recruitType != 2)
			return this->entry;
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
		if (recruitType != 2)
			entry = newEntry;
	}

	void setAgentType(int type)
	{
		if (recruitType == 2)
			entry = reinterpret_cast<eduEntry*>(type);
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
	void setBuildingHealth(int health)
	{
		GAME_FUNC(void(__thiscall*)(building*, int), repairBuilding)(this, health);
	}
	int getBuildingHealth()
	{
		return hp;
	}
};

struct recruitmentOptions
{
	int field0;
	struct unitRQ *units;
	int lastUnit;
	int unitsEnd;
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
	struct buildingInQueue items[6];
	int firstIndex;
	int lastIndex;
	int buildingsInQueue;
	int currentBuildingIndex;
};

struct availableBuildings
{
	struct buildingInQueue *buildingsList;
	struct availableBuildings *next;
	struct availableBuildings *previous;
	int listSize;
	int buildingCount;
};

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
	std::vector<int8_t> excludedGuilds; //0x001C
	std::vector<std::pair<stringWithHash,int>> scores;
}; //Size: 0x004C

struct settlementBuildingOptions
{
	std::vector<std::shared_ptr<buildingInQueue>> constructionOptions;
	int count;
	int totalCost;
	int totalTime;
	int turn;
	int hash;
};

struct settlementRecruitmentOptions
{
	std::vector<std::shared_ptr<unitRQ>> recruitOptions;
	int count;
	int totalCost;
	int totalTime;
	int turn;
	int hash;
};

struct settlementPolicies
{
	int32_t regionID;
	int32_t autoManageBuildPolicy;
	int32_t secondaryPolicy;
	int8_t autoManagedConstruction;
	int8_t autoManagedRecruitment;
	char pad_000E[2];
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
	int controllerIndex;
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
	void setRecruitmentValueSett(int type, int value)
	{
		recruitBias[type] = value;
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

struct buildingDamage
{
	int perimeter;
	bool breachable;
	int	type;
	int8_t health;
	char pad[3];
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
	struct stackStruct* army;
	struct oneSiege sieges[8];
	int8_t siegeNum; //0x0088
	char pad_0089[3]; //0x0089
	int32_t maxHoldoutTurns;
	int32_t siegeDuration;
	int32_t siegeCasualties;
	int32_t preSiegePopulation;
	int32_t factionTradedFrom;
	int32_t plagueDuration;
	int8_t plagued;
	int8_t salliedOut; //0x00A5
	int8_t gatesAreOpened;
	int8_t readyToSurrender;
	crusade* takenByCrusade;
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
	struct settMod* model;
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
	UINT32 regionID; /* number of region */
	int32_t factionID; //0x01AC
	int32_t yearFounded; //0x01B0
	int32_t triumph; //0x01B4
	int32_t moneySpentConstruction;//0x01B8
	int32_t moneySpentTraining;//0x01BC
	int32_t moneySpentRecruitment;//0x01C0
	char agentRecruitnotification[16];
	struct unitRQ unitQueue[9];
	int startIndexRQ;
	int endIndexRQ;
	int countRQ;
	int32_t recruitmentPoints;
	struct settlementStruct *thisBeforeBuildings;
	struct factionStruct **pointerToSelfFaction2;
	char smthingBuildingIdInQueue[128][2];
	int hasReligiousBuildingBitfield;
	struct buildingsQueue buildingsQueueArray;
	struct settlementStruct *thisBeforeBuildings2;
	struct factionStruct **pointerToSelfFaction3;
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
	uchar isProvokedRebellion;//0x0E05
	int8_t rebelAgainstRome;
	int8_t pad3[1];
	int timeSinceLastRebellion;
	int entertainmentType;
	int8_t isCapital; //0x0E10
	int8_t isTradeBlocked;
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
	int32_t loyalty; //0x0E38
	int32_t lastPopulation; //0x0E3C
	int32_t harvestSuccess; //0x0E40
	struct resStrat** resources;
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
	std::string packagePath;
	float foundingConversionRate;
	float ownerConversionRate;
	factionStruct* factionMechanics;
	settlementStruct* settlementMechanics;
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
	struct settlementStats settlementStatsLastTurn;
	struct settlementStats settlementStats;
	int32_t doingTempCalc; //0x1910
public:
	siegeS* getSiege(int index)
	{
		return sieges[index].siege;
	}
	resStrat* getResource(int index)
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
		return settlementStats.population;
	}
	void setPopulation(int newPop)
	{
		settlementStats.population = newPop;
	}
	settlementRecruitmentPool* getSettlementRecruitmentPool(int index)
	{
		return &recruitmentPools[index];
	}
	int getGuildStanding(int index)
	{
		return guildStandings[index];
	}
	unitRQ* getUnitInQueue(int index)
	{
		return &unitQueue[startIndexRQ + index];
	}
	void setGuildStanding(int index, int amount)
	{
		guildStandings[index] = amount;
	}
};

namespace settlementHelpers
{
	void setSettlementOwner(settlementStruct* sett, factionStruct* newOwner, bool convertGarrison);
	void changeOwner(settlementStruct* sett, factionStruct* newOwner);
	void upgradeSettlement(settlementStruct* sett);
	std::string getSettlementName(settlementStruct* sett);
	void changeSettlementName(settlementStruct* sett, const char* newName);
	float getReligion(settlementStruct* sett, int index);
	void setReligion(settlementStruct* sett, int index, float value);
	void addToLua(sol::state& luaState);
	void createBuilding(settlementStruct* sett, const char* building_level_id);
	void destroyBuilding(settlementStruct* sett, const char* typeName, bool isReturnMoney);
	
	bool addBuildingToQueue(buildingInQueue* building);
	int getAvailableBuildingsCount(const availableBuildings* list);
	buildingInQueue* getBuildingInQueue(buildingsQueue* queue, int position);
	buildingInQueue* getBuildingOption(const availableBuildings* list, const int index);
	availableBuildings* getAvailableBuildingsMem();
	availableBuildings* getAvailableBuildings(settlementStruct* sett);
	int makeBuildOptionsHash(const settlementStruct* sett);
	settlementBuildingOptions* getBuildingOptions(settlementStruct* sett);
	buildingInQueue* getBuildingOptionFromDb(const settlementBuildingOptions* list, const int index);
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
			fastFunctsHelpers::setCryptedString(&sett->name, newS.c_str());
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
			fastFunctsHelpers::setCryptedString(&guild->name, newS.c_str());
	}
};
