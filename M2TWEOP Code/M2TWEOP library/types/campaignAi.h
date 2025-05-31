#pragma once
#include <lua/sol.hpp>

#include "army.h"
#include "realGameTypes.h"

struct settlementResource;
struct aiCampaignController;
struct aiResourcePrivate;
struct aiRegionController;

enum class unitCategoryClass
{
	nonCombatant = 0,
	lightInfantry = 1,
	heavyInfantry = 2,
	spearmenInfantry = 3,
	missileInfantry = 4,
	lightCavalry = 5,
	heavyCavalry = 6,
	missileCavalry = 7,
	siegeWeapon = 8,
	animalHandler = 9,
	battleShip = 10
};

enum class defendTypes
{
	minimal,
	normal,
	raid,
	frontline,
	fortified,
	deep,
	count
};

inline bool operator==(const int val, defendTypes vis)
{
	return val == static_cast<int>(vis);
}

inline bool operator!=(const int val, defendTypes vis)
{
	return val != static_cast<int>(vis);
}

inline bool operator >(const int val, defendTypes vis)
{
	return val > static_cast<int>(vis);
}

inline bool operator <(const int val, defendTypes vis)
{
	return val < static_cast<int>(vis);
}

inline bool operator >=(const int val, defendTypes vis)
{
	return val >= static_cast<int>(vis);
}

inline bool operator <=(const int val, defendTypes vis)
{
	return val <= static_cast<int>(vis);
}

enum class invasionTypes
{
	buildup,
	immediate,
	raids,
	opportunistic,
	start,
	none,
	count
};

inline bool operator==(const int val, invasionTypes vis)
{
	return val == static_cast<int>(vis);
}

inline bool operator!=(const int val, invasionTypes vis)
{
	return val != static_cast<int>(vis);
}

inline bool operator >(const int val, invasionTypes vis)
{
	return val > static_cast<int>(vis);
}

inline bool operator <(const int val, invasionTypes vis)
{
	return val < static_cast<int>(vis);
}

inline bool operator >=(const int val, invasionTypes vis)
{
	return val >= static_cast<int>(vis);
}

inline bool operator <=(const int val, invasionTypes vis)
{
	return val <= static_cast<int>(vis);
}
enum class aiCampaignStrategy
{
    dormant = 0,
    gathering = 1,
    attackNormal = 2,
    attackBlitz = 3,
    attackGrind = 4,
    attackRaid = 5,
    defendBorder = 6,
    defendFortified = 7,
    defendHidden = 8,
    defendDeep = 9,
    attackAid = 10,
    count = 11,
};

inline bool operator==(const int val, aiCampaignStrategy strat)
{
    return val == static_cast<int>(strat);
}
inline bool operator<(const int val, aiCampaignStrategy strat)
{
    return val < static_cast<int>(strat);
}
inline bool operator>(const int val, aiCampaignStrategy strat)
{
    return val > static_cast<int>(strat);
}
inline bool operator<=(const int val, aiCampaignStrategy strat)
{
    return val <= static_cast<int>(strat);
}
inline bool operator>=(const int val, aiCampaignStrategy strat)
{
    return val >= static_cast<int>(strat);
}
inline bool operator!=(const int val, aiCampaignStrategy strat)
{
    return val != static_cast<int>(strat);
}

enum class aiCampaignStrength
{
    veryLow = 0,
    low = 1,
    medium = 2,
    high = 3,
    veryHigh = 4,
    count = 5,
};

inline bool operator==(const int val, aiCampaignStrength strat)
{
    return val == static_cast<int>(strat);
}

inline bool operator!=(const int val, aiCampaignStrength strat)
{
    return val != static_cast<int>(strat);
}

enum class regionRisk
{
    safe,
    lowRisk,
    mediumRisk,
    highRisk,
    extremeRisk
};

inline bool operator==(const int val, regionRisk vis)
{
    return val == static_cast<int>(vis);
}

inline bool operator!=(const int val, regionRisk vis)
{
    return val != static_cast<int>(vis);
}

inline bool operator >(const int val, regionRisk vis)
{
    return val > static_cast<int>(vis);
}

inline bool operator <(const int val, regionRisk vis)
{
    return val < static_cast<int>(vis);
}

inline bool operator >=(const int val, regionRisk vis)
{
    return val >= static_cast<int>(vis);
}

inline bool operator <=(const int val, regionRisk vis)
{
    return val <= static_cast<int>(vis);
}

enum class settlementPolicy
{
    balanced,
    financial,
    military,
    growth,
    cultural,
    none
};

inline bool operator==(const int val, settlementPolicy vis)
{
    return val == static_cast<int>(vis);
}

inline bool operator!=(const int val, settlementPolicy vis)
{
    return val != static_cast<int>(vis);
}

enum class settlementTroopPolicy
{
    infantry,
    cavalry,
    missile,
    spearmen,
    siege,
    none
};

inline bool operator==(const int val, settlementTroopPolicy vis)
{
    return val == static_cast<int>(vis);
}

inline bool operator!=(const int val, settlementTroopPolicy vis)
{
    return val != static_cast<int>(vis);
}

struct militaryValuesLTGD
{
    int32_t totalPopulation; //0x0000
    int32_t tileCount; //0x0004
    int32_t averagePopulation; //0x0008
    int32_t productionValue; //0x000C
    int32_t nonAlliedBorderLength; //0x0010
    int32_t alliedBorderLength; //0x0014
    int32_t fleetCount; //0x0018
    int32_t navalPowerPerFleet; //0x001C
    int32_t navalStrength; //0x0020
    int32_t armyCount; //0x0024
    int32_t strengthPerArmy; //0x0028
    int32_t totalStrength; //0x002C
    int32_t freeStrength; //0x0030
    int32_t enemyNum; //0x0034
    int32_t immediateEnemyStrength; //0x0038
    int32_t protectorateOf; //0x003C
};

struct strategyValuesLTGD
{
    int32_t borderTiles; //0x0000
    int32_t frontLineBalance; //0x0004
    bool hasAllianceAgainst; //0x0008
    bool isWeakestNeighbour; //0x0009
    bool isStrongestNeighbour; //0x000A
    char pad_000B[1]; //0x000B
};

struct interFactionLTGD
{
public:
    int32_t borderTiles; //0x0000
    int32_t frontLineBalance; //0x0004
    int8_t hasAllianceAgainst; //0x0008
    int8_t isStrongestNeighbour; //0x0009
    int8_t isWeakestNeighbour; //0x000A
    char pad_000B[1]; //0x000B
}; //Size: 0x000C

struct ltgdGlobals
{
public:
    struct factionStruct* currentFaction; //0x0000
    char pad_0004[8]; //0x0004
    uint32_t N00024EDA; //0x000C
    char pad_0010[4]; //0x0010
    struct militaryValuesLTGD ltgdFactionValues[31]; //0x0014
    struct interFactionLTGD interFactionLTGD[31][31]; //0x07D4
};

struct factionDataStrategy
{
    int32_t threat;
    int32_t threatSize;
    int size;
    int potential;
    int desire;
    float campaignStr;
    float battleStr;
    bool atWar;
    bool activeWar;
    bool plannedWar;
    int8_t pad;
};

struct regionStrengths
{
    int ownStrength;
    int ownCount;
    int enemyStrength;
    int enemyCount;
    int neutralStrength;
    int neutralCount;
};

struct aiRegionData
{
    trackedObject obj;
    int factionID;
    int regionID;
    bool navalTarget;
    bool isLastResort;
    char pad2[2];
    int regionValue;
    int priority;
    int neighbourEnemyNum;
    int neighbourOtherNum;
    int settlementIndex;
    regionStrengths strength;
    regionStrengths neighboursStrength;
    int regionRisk;
    aiRegionController* regionController;
public:
    void setRisk(enum regionRisk risk)
    {
        regionRisk = static_cast<int>(risk);
    }
    settlementStruct* getSettlement();
    factionStruct* getTargetFaction();
    void calculateRegionStrengths();
    bool targetsSameSettlement(const aiRegionData* other)
    {
        return other->regionID == regionID && other->settlementIndex == settlementIndex;
    }
};

struct aiMilitaryControllerWorldWide
{
    DWORD vfTable;
    aiFaction* aiFaction;
    aiResourcePrivate** resources;
    int resourcesSize;
    int resourcesNum;
    int targetId;
};


struct aiRegionController
{
    DWORD vfTable;
    aiFaction* aiFaction;
    int regionID;
    struct regionStruct *region;
    struct aiRegionData *gsdData;
    struct settlementStruct *settlement;
    int unitNeed;
    struct aiProductionController *productionController;
    struct aiResourcePrivate *garrison;
    int garrisonType;
    int requiredGarrisonStrength;
    void initialize();
    void update();
};

struct decisionValuesLTGD
{
public:
    int32_t defendType; //0x0000
    int32_t defendPriority; //0x0004
    int32_t invasionType; //0x0008
    int32_t invadePriority; //0x000C
    bool atWar; //0x0010
    bool wantPeace; //0x0011
    bool wantAlly; //0x0012
    bool wantBeProtect; //0x0013
    bool wantOfferProtect; //0x0014
    bool forceInvade;
    int8_t pad16;
    int8_t pad17;
    int32_t allianceAgainst; //0x0018
    int32_t ptsDesire; //0x001C
    int32_t ptsAlliance; //0x0020
    int32_t pointsInvasion; //0x0024
    int32_t pointsDefense; //0x0028
    bool canForceInvade; //0x002C
    char pad_002D[3]; //0x002D
}; //Size: 0x0030

struct aiSubterfugeController
{
    DWORD vfTable;
    aiFaction* aiFac;
    bool missionsExist;
    char pad1[3];
    void* missions;
    bool missionsActiveExist;
    char pad2[3];
    void* missionsActive;
    gameLinkedList<aiResourcePrivate*> resources;
    gameLinkedList<void*> missionsList;
    void destroyFaction(factionStruct* fac, int value);
    void reduceUnitProduction(factionStruct* fac, int category, int value);
    void reduceEconomicProduction(factionStruct* fac, int value);
};

struct aiCampaignObjective
{
    DWORD vfTable;
    aiFaction* aiFaction;
    int type;
    int priority;
    aiCampaignObjective* next;
};

struct aiCampaignMission
{
    DWORD vfTable;
    aiFaction* aiFaction;
    aiResourcePrivate* resource;
    int priority;
};

struct aiMilitaryController
{
    DWORD vfTable;
    aiFaction* aiFaction;
    struct factionStruct *faction;
    aiCampaignObjective *objectives;
    aiCampaignMission* currentMission;
    aiResourcePrivate *resources;
    int resourcesSize;
    int resourcesNum;
    bool merge;
    char pad_0x21[3];
    int minStrengthReq;
    int highestEnemyStrength;
    int lowestEnemyStrength;
};

struct aiCampaignController : aiMilitaryController
{
    int targetRegionID;
    aiRegionData* regionData;
    int targetFactionId;
    bool attacking;
    bool canRequestMore;
    bool canAttack;
    bool fieldx3F;
    int strategyType;
    int lastStrategyType;
    int nextStrategyType;
    int strengthRequiredType;
    int totalStrengthRequired;
    int strengthThreshold;
    int totalAllocated;
    int totalFree;
    int maxEnemyPower;
    int minEnemyPower;
    int *neighboursForDefense;
    int neighboursForDefenseSize;
    int neighboursForDefenseNum;
    bool isDefensive();
    void initialize();
    void update();
    void logData();
    void updateAllocation();
    void setStrategy(aiCampaignStrategy strategy);
	aiCampaignStrategy getCurrentStrategy() { return static_cast<aiCampaignStrategy>(strategyType);}
	aiCampaignStrategy getNextStrategy() { return static_cast<aiCampaignStrategy>(nextStrategyType);}
};

struct aiResourceArray
{
    aiResourcePrivate* resources;
    int size;
    int num;
};

struct aiResourceManager
{
    struct aiFaction *aiFaction;
    struct factionStruct *faction;
    int field_C;
    int32_t field_10;
    int field_14;
    int field_18;
    aiResourceArray resourceArrays[10];
    void* aiControllers;
    int32_t aiControllersSize;
    int32_t aiControllersNum;
    bool update;
    char pad[3]{};
    void releaseResource(aiResourcePrivate* res);
};

struct aiBrigandController : aiMilitaryController
{
    regionStruct* region;
    int strongestArmy;
    int weakestArmy;
};

struct aiFaction
{
public:
	DWORD* vtable; //0x0000
	struct factionStruct* faction; //0x0004
	int32_t factionID; //0x0008
	void* personality; //0x000C
	bool aiTurnStarted; //0x0010
	bool aiFinished; //0x0011
	bool battleResolve; //0x0012
	char pad_0013[1]; //0x0013
	factionStruct* factionFleeingFrom; //0x0014
	int hordeEndTargets[2];
	struct aiLongTermGoalDirector* ltgd; //0x0020
	struct aiDiplomacyManager* aiDiplomacyManager; //0x0024
	struct aiActionRequestController* aiActionRequestController; //0x0028
	struct aiResourceManager* aiResourceManager; //0x002C
	struct AiFinanceManager* AiFinanceManager; //0x0030
	struct aiPersonalityValues* aiProductionControllers; //0x0034
	struct aiGlobalStrategyDirector* aiGlobalStrategyDirector; //0x0038
	aiSubterfugeController* subterfugeController; //0x003C
	struct aiNamedCharacterController* aiNamedCharacterController; //0x0040
	struct aiPriestController* aiPriestController; //0x0044
	struct aiMerchantController* aiMerchantController; //0x0048
}; //Size: 0x0058

struct trackedAiRegionData
{
	void* vtbl; //0x0000
	aiRegionData* regionData;
};

struct regionGroupElement
{
	int regionID;
	regionStruct *region;
	trackedAiRegionData aiGsdRegion;
};

struct regionGroup
{
	regionGroupElement *regions;
	int32_t regionsSize;
	int32_t regionsNum;
	struct factionStruct *faction;
	int32_t factionID;
	uint32_t neighbourFactionBitfield;
	int population;
	int area;
	int income;
	int frontage;
	int allArmiesStrength;
	regionStrengths strengths;
	int *neighbourSeaRegionIds;
	int neighbourSeaRegionIdsSize;
	int neighbourSeaRegionIdsNum;
};

struct aiLongTermGoalDirector
{
public:
	DWORD* vtable; //0x0000
	struct aiFaction* aiFaction; //0x0004
	struct factionStruct* faction; //0x0008
	uint32_t trustedAllyEnemies; //0x000C --bitfield
	int32_t freeStrengthEnemy; //0x0010
	int32_t freeStrengthEnemyBalance; //0x0014
	bool consideringNavalInvasion; //0x0018
	char pad_0019[3]; //0x0019
	int32_t navalTargetRegionID;
	int32_t stagingRegionID;
	int32_t regionGroupNum;
	struct decisionValuesLTGD longTermGoalValues[31]; //0x0028
	int32_t longTermPolicy; //0x05F8
	int32_t longTermTroopStatus; //0x05FC
public:
	decisionValuesLTGD* getlongTermGoalValues(int targetFaction)
	{
		return &longTermGoalValues[targetFaction];
	}
	bool isTrustedAllyEnemy(const factionStruct* fac)
	{
		return trustedAllyEnemies & (1 << fac->factionID);
	}
	int getInvasionTargetNum(int regionId);
	int getInvasionTargetPriority(int regionId);
	bool regionsBordersOnlyTrusted(int regionId);
	bool isTrustedAlly(int targetFactionId);
	void update();
	void setNavalTarget();
	int getNavalTargetScore(const seaConnectedRegion* seaRegion, int fromRegionId);
}; //Size: 0x0604

struct aiPersonalityValues
{
	struct aiFaction* aiFaction;
	struct aiProductionController** aiProductionControllers;
	int32_t aiProductionControllersSize;
	int32_t aiProductionControllersNum;
	int32_t AIPersonalityType;
	int32_t AIPersonalityName;
	int32_t buildingValues[57];
	int32_t ancillaryValues[4];
	int32_t agentBuildingValues[6];
	int32_t unitBuildingValues[11];
	int32_t populationBias;
	int32_t agentRecruitValues[6];
	int32_t unitRecruitmentValues[11];
	int32_t balancedPolicyNum;
	int32_t financialPolicyNum;
	int32_t militaryPolicyNum;
	int32_t growthPolicyNum;
	int32_t culturalPolicyNum;
	struct settlementPolicies* settlementPolicies;
	int32_t settlementPoliciesSize;
	int32_t settlementPoliciesCount;
	int8_t autoManagedRecruitment;
	int8_t autoManagedConstruction;
	int8_t autoManagedTaxes;
	char pad_01BB[1];
public:
	void setConstructionValue(int type, int value)
	{
		buildingValues[type] = value;
	}
	void setConstructionAgentValue(int type, int value)
	{
		agentBuildingValues[type] = value;
	}
	void setConstructionUnitValue(int type, int value)
	{
		unitBuildingValues[type] = value;
	}
	void setRecruitmentValue(int type, int value)
	{
		unitRecruitmentValues[type] = value;
	}
	void setAgentValue(int type, int value)
	{
		agentRecruitValues[type] = value;
	}
	int getConstructionValue(int type)
	{
		return buildingValues[type];
	}
	int getConstructionAgentValue(int type)
	{
		return agentBuildingValues[type];
	}
	int getConstructionUnitValue(int type)
	{
		return unitBuildingValues[type];
	}
	int getConstructionUnitValueEnum(unitCategoryClass type)
	{
		return unitBuildingValues[static_cast<int>(type)];
	}
	int getRecruitmentValue(int type)
	{
		return unitRecruitmentValues[type];
	}
	int getRecruitmentValueEnum(unitCategoryClass type)
	{
		return unitBuildingValues[static_cast<int>(type)];
	}
	int getAgentValue(int type)
	{
		return agentRecruitValues[type];
	}
	aiProductionController* getProductionController(int index)
	{
		return aiProductionControllers[index];
	}
	void evaluatePolicies(int regionId, int settlementIndex);
	settlementPolicy decideSettlementPolicy(const settlementStruct* settlement);
	settlementTroopPolicy decideSettlementTroopPolicy(const settlementStruct* settlement);
	float getPolicyPriority(settlementPolicy policyType);
};

struct aiNavalRegion
{
	aiRegionData regionData;
	int stagingId;
	int duration;
	bool active;
	bool highPriority;
	char pad[2]{};
};

struct aiNavalTransportRegion
{
	int regionId;
	int powerBalance;
	int capacityAllocated;
	int powerAllocated;
	int* armies;
	int armiesSize;
	int armiesNum;
	gameStdVector<character* > transportCharacters;
	regionStruct* targetRegion;
};

struct campaignControllerArray
{
	aiCampaignController** campaigns;
	int size;
	int count;
};

struct aiMilitaryDirector
{
	struct aiFaction* aiFaction;
	struct factionStruct* faction;
	campaignControllerArray defensiveCampaigns;
	campaignControllerArray offensiveCampaigns;
	campaignControllerArray aidingCampaigns;
	aiBrigandController** aiBrigandControllers;
	int32_t aiBrigandControllersSize;
	int32_t aiBrigandControllersNum;
	aiCampaignController* lastResortController;
	aiMilitaryControllerWorldWide worldwideController;
	void initialize();
	void logData();
	static void removeCampaign(campaignControllerArray* array, int index);
	static void addCampaign(campaignControllerArray* array, aiCampaignController* controller);
	void checkDefensiveCampaigns(const aiGlobalStrategyDirector* director);
	void checkOffensiveCampaigns(const aiGlobalStrategyDirector* director);
	void checkAidingCampaigns(const aiGlobalStrategyDirector* director);
	void decideStrategies();
	void decideDefensiveStrategy(aiCampaignController* controller);
	void decideOffensiveStrategy(aiCampaignController* controller);
	aiCampaignController* createCampaignController(aiRegionData* regionData);
};

struct ltgdConfig
{
	float trustedFsThreshold;
	float trustedTargetFsThreshold;
	float trustedGsThreshold;
	float trustedTargetGsThreshold;
	int minInvadePriority;
	int maxInvadePriority;
	float invPriorityFsModifier;
	bool useCheatOverride;
	char pad[3];
	int assistanceOffset;
	float scalePriority(int priority)
	{
		return priorityScale(priority) * 100.f;
	}
	float priorityScale(int priority)
	{
		return ((priority - minInvadePriority) / static_cast<float>(maxInvadePriority - minInvadePriority));
	}
	float getPriorityScaled(const factionStruct* fac1, const factionStruct* fac2);
	float getPriorityMod(const factionStruct* fac1, const factionStruct* fac2);
};


struct aiGlobalStrategyDirector
{
	void* vftable /*VFT*/;
	struct aiFaction* aiFaction;
	struct factionStruct* faction;
	struct factionDataStrategy stratFacData[31];
	struct aiRegionData* ownRegions;
	int32_t ownRegionsSize;
	int32_t ownRegionsCount;
	struct aiRegionData* allNeighbourRegions;
	int32_t allNeighbourRegionsSize;
	int32_t allNeighbourRegionsCount;
	struct aiRegionData* targetRegions;
	int32_t targetRegionsSize;
	int32_t targetRegionsCount;
	struct aiRegionData targetRegionCrusade;
	aiNavalRegion* navalInvasionTargets;
	int32_t navalInvasionTargetsSize;
	int32_t navalInvasionTargetsNum;
	aiNavalTransportRegion* navalTransportTargets;
	int32_t navalTransportTargetsSize;
	int32_t navalTransportTargetsNum;
	aiRegionController** regionControllers;
	int32_t regionControllersSize;
	int32_t regionControllersNum;
	struct aiMilitaryDirector militaryDirector;
	void** aiNavalControllers;
	int32_t aiNavalControllersSize;
	int32_t aiNavalControllersNum;
	void** regionGroupControllers;
	int32_t regionGroupControllersSize;
	int32_t regionGroupControllersNum;
	int hordeTargetRegionID;
	void* hordeController;
	int hordeTargetFactions[2];
	void* crusadeController;
	void initialize();
	void initialize2();
	void initOwnRegions();
	void initNeighbourRegions();
	void initNavalRegions();
	void initTargetRegions();
	void updateRegionControllers();
	void logData();
};

struct armyResource;

struct armySupportResource
{
	std::shared_ptr<armyResource> resource{};
	float moveCost{};
	int8_t turns = 1;
	armySupportResource(armyResource* army) : resource(army) {}
	armySupportResource(const std::shared_ptr<armyResource>& army) : resource(army) {}
};

struct nearSettResource
{
	std::shared_ptr<settlementResource> resource{};
	float moveCost{};
	int8_t turns = 1;
	nearSettResource(settlementResource* sett) : resource(sett) {}
	nearSettResource(const std::shared_ptr<settlementResource>& sett) : resource(sett) {}
};

struct armyResource
{
	armyStruct* army{};
	std::vector<std::shared_ptr<armySupportResource>> nearResources{};
	std::vector<std::shared_ptr<nearSettResource>> nearSetts{};
	int positionPower{};
	float value = 0.f;
	int totalThreatReceiving{};
	int totalThreatGiving{};
	int totalSupportReceiving{};
	int totalSupportGiving{};
	int unitCount{};
	bool used = false;
	bool searched = false;
	bool own = true;
	bool enemy = false;
	int targets = 0;
	bool validate();
	armyResource(armyStruct* army) : army(army), unitCount(army->numOfUnits) {}
	void calculatePositionPower();
};

struct settlementResource
{
	settlementStruct* settlement{};
	std::vector<std::shared_ptr<armySupportResource>> nearResources{};
	int positionPower{};
	float value = 0.f;
	int totalThreatReceiving{};
	int totalSupportReceiving{};
	bool used = false;
	bool searched = false;
	bool own = true;
	bool enemy = false;
	settlementResource(settlementStruct* sett) : settlement(sett) {}
	void calculatePositionPower();
	int calculatePriority(bool isOwn);
};

struct aiOrder
{
	float priority{};
	std::vector<std::shared_ptr<armySupportResource>> assignedArmies{};
	std::vector<std::pair<int, int>> validTiles{};
	void setTiles(int x, int y);
	void sortArmies();
	bool playerInvolved = false;
	bool executed = false;
	void removeUsedResources();
	std::vector<int> tileNums{};
	virtual bool evaluate() = 0;
	virtual bool execute() = 0;
	virtual std::string toString() = 0;
};

struct attackSettlementOrder : aiOrder
{
	std::shared_ptr<settlementResource> targetSettlement{};
	int attackX = -1;
	int attackY = -1;
	attackSettlementOrder(std::shared_ptr<settlementResource> sett) : targetSettlement(std::move(sett)) {}
	bool evaluate() override;
	bool evaluateAttack();
	void sortAssigned();
	bool execute() override;
	std::string toString() override;
};

struct attackArmyOrder : aiOrder
{
	std::shared_ptr<armyResource> targetArmy{};
	attackArmyOrder(std::shared_ptr<armyResource> army) : targetArmy(std::move(army)) {}
	bool evaluate() override;
	bool evaluateAttack();
	bool execute() override;
	std::string toString() override;
};

struct defendSettlementOrder : aiOrder
{
	std::shared_ptr<settlementResource> targetSettlement{};
	defendSettlementOrder(std::shared_ptr<settlementResource> sett) : targetSettlement(std::move(sett)) {}
	bool evaluate() override;
	bool execute() override;
	std::string toString() override;
};

struct defendArmyOrder : aiOrder
{
	std::shared_ptr<armyResource> targetArmy{};
	defendArmyOrder(std::shared_ptr<armyResource> army) : targetArmy(std::move(army)) {}
	bool evaluate() override;
	bool execute() override;
	std::string toString() override;
};

struct mergeArmiesOrder : aiOrder
{
	armyStruct* targetArmy{};
	std::shared_ptr<armyResource> targetRes{};
	mergeArmiesOrder(armyStruct* army) : targetArmy(army) {}
	bool evaluate() override;
	bool execute() override;
	std::string toString() override;
};

struct aiFactionData
{
	aiFactionData() {targetFactionFactors.fill(1.f); aidFactionFactors.fill(1.f);}
	int noActionTurns{};
	std::array<float, 31> targetFactionFactors{};
	std::array<float, 31> aidFactionFactors{};
	std::vector<std::pair<std::pair<int, int>, float>> settlementFactors{};
	std::unordered_map<int, float> targetReligionFactors{};
	std::unordered_map<int, float> aidReligionFactors{};
	float getTargetFactionFactor(const factionStruct* faction);
	float getAidFactionFactor(const factionStruct* faction);
	float getSettlementFactor(const settlementStruct* settlement);
	float getTargetReligionFactor(int religion);
	float getAidReligionFactor(int religion);
	void setTargetFactionFactor(const factionStruct* faction, float factor);
	void setAidFactionFactor(const factionStruct* faction, float factor);
	void setSettlementFactor(const settlementStruct* settlement, float factor);
	void setTargetReligionFactor(int religion, float factor);
	void setAidReligionFactor(int religion, float factor);
};

enum priorityType
{
	priType_own,
	priType_target,
	priType_aid
};

class globalEopAiConfig
{
public:
	globalEopAiConfig();
	bool enabled = true;
	bool enableLogging = false;
	float aggressionFactor = 1.f;
	float defenseFactor = 0.75f;
	float religionFactor = 0.5f;
	float nonBorderFactor = 0.20f;
	float residenceFactor = 1.f;
	float aidFactor = 0.5f;
	float moveCostFactor = 1.20f;
	float powerFactor = 1.f;
	float invadePriorityFactor = 1.f;
	int maxTurnSearchCount = 4;
	static std::shared_ptr<aiFactionData> getFactionData(const factionStruct* fac) {return getInstance()->m_FactionData[fac->factionID];}
	static aiFactionData* getFactionDataLua(const factionStruct* fac);
	static bool getEnabled() { return getInstance()->enabled; }
	static bool getLoggingEnabled() { return getInstance()->enableLogging; }
	static float getAggressionFactor() { return getInstance()->aggressionFactor; }
	static float getDefenseFactor() { return getInstance()->defenseFactor; }
	static float getResidenceFactor() { return getInstance()->residenceFactor; }
	static float getAidFactor() { return getInstance()->aidFactor; }
	static float getMoveCostFactor() { return getInstance()->moveCostFactor; }
	static float getPowerFactor() { return getInstance()->powerFactor; }
	static float getInvadePriorityFactor() { return getInstance()->invadePriorityFactor; }
	static float getNonBorderFactor() { return getInstance()->nonBorderFactor; }
	static int getMaxTurnSearchCount() { return getInstance()->maxTurnSearchCount; }
	static void setEnabled(bool set) { getInstance()->enabled = set; }
	static void setLoggingEnabled(bool set) { getInstance()->enableLogging = set; }
	static void setAggressionFactor(float set) { getInstance()->aggressionFactor = set; }
	static void setDefenseFactor(float set) { getInstance()->defenseFactor = set; }
	static void setResidenceFactor(float set) { getInstance()->residenceFactor = set; }
	static void setAidFactor(float set) { getInstance()->aidFactor = set; }
	static void setMoveCostFactor(float set) { getInstance()->moveCostFactor = set; }
	static void setPowerFactor(float set) { getInstance()->powerFactor = set; }
	static void setNonBorderFactor(float set) { getInstance()->nonBorderFactor = set; }
	static void setInvadePriorityFactor(float set) { getInstance()->invadePriorityFactor = set; }
	static void setMaxTurnSearchCount(int set) { getInstance()->maxTurnSearchCount = set; }
	static factionStruct* getCurrentFaction() { return getInstance()->m_Faction; }
	static std::shared_ptr<aiFactionData> getCurrentFactionData() { return getInstance()->m_CurrentFacData; }
	static void increaseActionCount() { getInstance()->m_CurrentFacData->noActionTurns++; }
	static void setLogging(bool log) { getInstance()->enableLogging = log; }
	static void toggleAi(bool set) { getInstance()->enabled = set; }
	static std::shared_ptr<globalEopAiConfig> getInstance() { return m_Instance; }
	static globalEopAiConfig* getInstanceLua() { return m_Instance.get(); }
	void turnStartMove(factionStruct* fac, bool isEnd = false);
	void assignOrders(factionStruct* fac);
	void turnEndMerge(factionStruct* fac);
	std::shared_ptr<armyResource> findArmyResource(armyStruct* army);
	std::shared_ptr<settlementResource> findSettResource(settlementStruct* sett);
	static void clearFactionData() { m_Instance = std::make_shared<globalEopAiConfig>(); }
	bool isEndTurn = false;
	void characterTurnStart(character* currentChar);
protected:
	static std::shared_ptr<globalEopAiConfig> m_Instance;
	factionStruct* m_Faction{};
	std::shared_ptr<aiFactionData> m_CurrentFacData{};
	std::array<std::shared_ptr<aiFactionData>, 31> m_FactionData{};
	float calculateSettPriority(const std::shared_ptr<settlementResource>& settRes, priorityType priType);
	float calculateArmyPriority(const std::shared_ptr<armyResource>& armyRes, priorityType priType);
	void checkRegion(int regionId);
	void getData(const factionStruct* fac);
	void clearData()
	{
		m_Armies.clear();
		m_AllyArmies.clear();
		m_TargetArmies.clear();
		m_Settlements.clear();
		m_AllySettlements.clear();
		m_TargetSettlements.clear();
		m_Orders.clear();
		m_CheckedRegions.clear();
		m_SearchedRegions.clear();
	}
	std::vector<int> m_CheckedRegions;
	std::vector<int> m_SearchedRegions;
	std::vector<std::shared_ptr<armyResource>> m_Armies{};
	std::vector<std::shared_ptr<armyResource>> m_AllyArmies{};
	std::vector<std::shared_ptr<armyResource>> m_TargetArmies{};
	std::vector<std::shared_ptr<settlementResource>> m_Settlements{};
	std::vector<std::shared_ptr<settlementResource>> m_AllySettlements{};
	std::vector<std::shared_ptr<settlementResource>> m_TargetSettlements{};
	std::vector<std::shared_ptr<aiOrder>> m_Orders;
};

struct eopAiFactionConfig
{
	float aggressionFactor{};
	float mergeFactor{};
	float aidFactor{};
};

namespace campaignAi
{
	int assessGarrisonStrength(const aiRegionData* gsdData, const settlementStruct* settlement, const factionStruct* faction);
	ltgdGlobals* getLtgdGlobals();
	ltgdConfig* getLtgdConfig();
	militaryValuesLTGD* getAiFactionValues(const factionStruct* fac);
	interFactionLTGD* getInterFactionLTGD(const factionStruct* fac, const factionStruct* targetFac);
	void addToLua(sol::state& luaState);
}
