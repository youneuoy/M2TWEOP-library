#pragma once
#include <lua/sol.hpp>
#include "realGameTypes.h"
#include "settlement.h"


struct aiCampaignController;
struct aiResourcePrivate;
struct aiRegionController;

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

struct smFactionsDb {
	struct factionRecord* facDescrs;
	int capacity;
	int size;
};

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

struct factionHordeInfo
{
	factionStruct *faction;
	int someFactionID;
	int xCoord;
	int yCoord;
	int hordeUnitCount;
	char field_14;
	char field_15;
	char field_16;
	char field_17;
	int amountTimesHorded;
	int array1c;
	int array1cSize;
	int array1cNum;
	char isHorde;
	char field_29;
	char field_2a;
	char field_2b;
	int hordeMinUnits2;
};

struct tileRevealer
{
	int xCoord;
	int yCoord;
	int lineOfSight;
	float height;
};

enum class tileVisbility : int8_t
{
	notVisibleStart = 0,
	notVisible = 1,
	wasVisible = 2,
	visible = 3
};

inline bool operator==(const int8_t val, tileVisbility vis)
{
	return val == static_cast<int8_t>(vis);
}

inline bool operator!=(const int8_t val, tileVisbility vis)
{
	return val != static_cast<int8_t>(vis);
}

inline bool operator >(const int8_t val, tileVisbility vis)
{
	return val > static_cast<int8_t>(vis);
}

inline bool operator <(const int8_t val, tileVisbility vis)
{
	return val < static_cast<int8_t>(vis);
}

inline bool operator >=(const int8_t val, tileVisbility vis)
{
	return val >= static_cast<int8_t>(vis);
}

inline bool operator <=(const int8_t val, tileVisbility vis)
{
	return val <= static_cast<int8_t>(vis);
}


struct factionTileStruct {
	int8_t* tilesVisiblity;
	int tilesXBound;
	int tilesYBound;
	undefined field3_0xc[28];
	tileRevealer* tileRevealers;
	int tileRevealersSize;
	int tileRevealersNum;
};

struct battleFactionCounter
{
public:
	int32_t battlesWon; //0x0000
	int32_t battlesLost; //0x0004
	int32_t lastResult; //0x0008
}; //Size: 0x000C

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
};

struct holdRegionsWinCondition
{
public:
	char pad_0000[4]; //0x0000
	uint32_t stratTableStringIndex; //0x0004
	struct stringWithHash* regionsToHold; //0x0008
	int32_t regionsToHoldSize; //0x000C
	int32_t regionsToHoldCount; //0x0010
	uint32_t factionsToOutliveBitfield; //0x0014
	int32_t numberOfRegions; //0x0018
	char pad_001C[4]; //0x001C
	int32_t* holdRegionLengths; //0x0020
	int32_t holdRegionLengthsSize; //0x0024
	int32_t holdRegionLengthsCount; //0x0028
	char pad_002C[12]; //0x002C
public:
	const char* getRegionToHoldName(int index)
	{
		return regionsToHold[index].name;
	}
	int getRegionToHoldLength(int index)
	{
		if (index < holdRegionLengthsCount)
			return holdRegionLengths[index];
		return 0;
	}
}; //Size: 0x0080

struct decisionValuesLTGD
{
public:
	int32_t defendType; //0x0000
	int32_t defendPriority; //0x0004
	int32_t invasionType; //0x0008
	int32_t invadePriority; //0x000C
	int8_t atWar; //0x0010
	int8_t wantPeace; //0x0011
	int8_t wantAlly; //0x0012
	int8_t wantBeProtect; //0x0013
	int8_t wantOfferProtect; //0x0014
	int8_t forceInvade;
	int8_t pad16;
	int8_t pad17;
	int32_t allianceAgainst; //0x0018
	int32_t ptsDesire; //0x001C
	int32_t ptsAlliance; //0x0020
	int32_t pointsInvasion; //0x0024
	int32_t pointsDefense; //0x0028
	int8_t canForceInvade; //0x002C
	char pad_002D[3]; //0x002D
}; //Size: 0x0030

struct factionEconomy
{
public:
	int32_t farmingIncome; //0x0000
	int32_t taxesIncome; //0x0004
	int32_t miningIncome; //0x0008
	int32_t tradeIncome; //0x000C
	int32_t merchantIncome; //0x0010
	int32_t constructionIncome; //0x0014
	int32_t lootingIncome; //0x0018
	int32_t missionIncome; //0x001C
	int32_t diplomacyIncome; //0x0020
	int32_t tributesIncome; //0x0024
	int32_t adminIncome; //0x0028
	int32_t kingsPurseIncome; //0x002C
	int32_t wagesExpense; //0x0030
	int32_t upkeepExpense; //0x0034
	int32_t constructionExpenseBuildings; //0x0038
	int32_t constructionExpenseField; //0x003C
	int32_t recruitmentExpenseBuildings; //0x0040
	int32_t recruitmentExpenseMercs; //0x0044
	int32_t corruptionExpense; //0x0048
	int32_t diplomacyExpense; //0x004C
	int32_t tributesExpense; //0x0050
	int32_t disasterExpense; //0x0054
	int32_t entertainmentExpense; //0x0058
	int32_t devastationExpense; //0x005C
}; //Size: 0x0060

struct factionRanking
{
public:
	float totalRanking; //0x0000
	float militaryRanking; //0x0004
	float productionRanking; //0x0008
	float territoryRanking; //0x000C
	float financialRanking; //0x0010
	float populationRanking; //0x0014
}; //Size: 

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

//faction
struct factionStruct {
	DWORD* vfTable;
	undefined field_0x4[176];
	int factionID; /* number of faction in diplomatic array */
	int cultureID;
	char* aiLabel; /* ai_label of faction */
	int32_t aiLabelHash; //0x00C0
	struct settlementStruct* capital; /* capital of the faction */
	struct characterRecord* leader; /* faction leader */
	struct characterRecord* heir; /* faction heir */
	struct factionRecord* factionRecord;
	int isPlayerControlled; /* is faction a controlled by player */
	struct aiFaction* aiFaction; //0x00D8
	int32_t aiPersonalityType; //0x00DC
	int32_t aiPersonalityName; //0x00E0
	void* factionGoals;
	int factionGoalsSize;
	int factionGoalsNum;
	struct holdRegionsWinCondition* WinConditions; //0x00F0
	int32_t regionsOwnedStart; //0x00F4
	struct characterRecord** characterRecords; /* all characters, died, alive, etc */
	int32_t characterRecordsSize; //0x00FC
	int characterRecordNum; /* all characters, died, alive, etc */
	struct character** characters; /* characters on stratmap */
	int32_t charactersSize; //0x0108
	int numOfCharacters; /* characters on stratmap */
	struct armyStruct** armies;
	int32_t armiesSize; //0x0114
	int stackNum;
	int* regionsID;
	int32_t regionsSize; //0x0120
	int regionsNum;
	struct settlementStruct** settlements;
	int32_t settlementsSize; //0x012C
	int settlementsNum;
	struct fortStruct** forts;
	int32_t fortsSize; //0x0138
	int fortsNum;
	struct watchTowerStruct** watchTowers; /* watchtowers */
	int32_t watchtowersSize; //0x0144
	int watchtowersNum;
	struct portBuildingStruct** portBuildings; /* port buildings */
	int32_t portBuildingsSize; //0x0150
	int portBuildingsNum;
	uint32_t neighBourFactionsBitmap; //0x0158
	int* neighBourRegions; //0x015C
	int32_t neighBourRegionsSize; //0x0160
	int32_t neighBourRegionsNum; //0x0164
	bool hasSeaAccess;
	bool groupPresent;
	bool inCivilWar;
	bool inEndGame;
	factionStruct* groupLeader;
	factionStruct** groupMembers;
	int32_t groupMembersSize;
	int32_t groupMembersNum;
	factionStruct** groupRebels;
	int32_t groupRebelsSize;
	int32_t groupRebelsNum;
	int unitsToMove;
	float timeStartTurn;
	int worldInfluence;
	int32_t deadStatus; //0x0194 3 means until resurrected 4 means until emerged
	int8_t reEmergent; //0x0198
	int8_t isUndiscovered; //0x0199
	char pad_019A[2]; //0x019A
	factionTileStruct* tilesFac;
	struct mission** missions; //0x01A0
	int32_t missionsSize; //0x01A4
	int32_t missionCount; //0x01A8
	struct mission** pendingMissions;
	int32_t pendingMissionsSize;
	int32_t pendingMissionsCount;
	uint32_t triumphPoints;
	uint32_t triumphPoints2;
	uint32_t triumphPoints3;
	character** hostages;
	int32_t hostagesSize;
	int32_t hostagesCount;
	trackedCharacter* spottedCharacters;
	int32_t spottedCharactersSize;
	int32_t spottedCharactersCount;
	struct factionRanking* factionRankings; //0x01DC
	int32_t factionRankingsSize; //0x01E0
	int32_t factionRankingsCount; //0x01E4
	void* scoreRanks;
	int32_t scoreRanksSize;
	int32_t scoreRanksCount;
	int32_t triumphValue; //0x01F4
	bool autoManageAll;
	bool autoManageRecruitment;
	bool autoManageBuildings;
	bool autoManageTaxes;
	float spendingLimit;
	int8_t freezeFactionAI; //0x0200
	char pad_0201[3]; //0x0201
	struct capabilityStruct factionCapabilities;
	int8_t hasSettlementsProcessed; //0x0A30
	char pad_0A31[3];
	int32_t treasuryTurnStart;
	bool isHorde;
	char pad_0A39[3];
	factionHordeInfo *factionHordeInfo;
	UINT32 religion; /* number of religion */
	bool shadowSwitch;
	bool shadowWon;
	char pad3333[2];
    struct factionStruct *guildSection;
	void* guildDeclined;
	void* guildUpgrade;
	bool isExcommunicated; //0x0A54
	char pad_0A55[3]; //0x0A55
	int32_t turnsSinceAmericaDiscovered; //0x0A34
	gameStdVector<void*> diplomaticProposals;
	gameStdVector<float> previousStandings;
	gameStdVector<float> resourceModifiers;
	int revoltReason;
	int playerPasswordHash;
	bool hasPassWord;
	char pad_0A7D[3];
	UNICODE_STRING** email;
	UNICODE_STRING** localizedName;
	int32_t factionBannerIndex; //0x0AA0
	int32_t agentNameFactionId[12]; //0x0AA4
	bool showDefeatScreen;
	char pad_0AD4[3];
	gameStdVector<void*> hotSeatStuff;
	factionStruct* factionEconomyOffset;
	int money; /* money of the faction */
	int kingsPurse; /* money of the faction */
	int32_t incomeDoubled; //0x0AF4
	struct factionEconomy factionEconomyTable[10]; //0x0AF8
	int32_t nextCounterEconomy; //0x0EB8
	int32_t counterEconomy; //0x0EBC
	int32_t maxTurnsTillReset; //0x0EC0
	int32_t upkeepModifier; //0x0EC4
	factionStruct* factionFamilyOffset; //0x0EC8
	void* marriageOption;
	void* adoptionOption;
	struct battleFactionCounter(*battlesWonVsFaction)[31]; //0x0ED4
	int32_t factionCountStart; //0x0ED8
	int32_t otherFactionCount; //0x0EDC
	int32_t battlesWon; //0x0EE0
	int32_t battlesLost; //0x0EE4
	int32_t settlementsCaptured; //0x0EE8
	int32_t settlementsLost; //0x0EEC
	int32_t unitsRecruited; //0x0EE8
	int32_t unitsLost; //0x0EEC
	int32_t lastOpponentId; //0x0EEC
public:
	void updateNeighbours();
	int getAliveCharacterNumOfType(characterTypeStrat charType);
	characterRecord* getCharacterRecord(int index)
	{
		return characterRecords[index];
	}
	character* getCharacter(int index)
	{
		return characters[index];
	}
	character* getSpottedCharacter(int index)
	{
		return spottedCharacters[index].character;
	}
	armyStruct* getArmy(int index)
	{
		return armies[index];
	}
	settlementStruct* getSettlement(int index)
	{
		return settlements[index];
	}
	fortStruct* getFort(int index)
	{
		return forts[index];
	}
	portBuildingStruct* getPort(int index)
	{
		return portBuildings[index];
	}
	int getNeighbourRegionID(int index)
	{
		return neighBourRegions[index];
	}
	watchTowerStruct* getWatchtower(int index)
	{
		return watchTowers[index];
	}
	battleFactionCounter* getBattleVsFactionStats(int targetFactionID)
	{
		return battlesWonVsFaction[targetFactionID];
	}
	void setCharacterNameFaction(int characterType, int facId)
	{
		agentNameFactionId[characterType] = facId;
	}
	bool isNeighbourFaction(const factionStruct* fac2)
	{
		return neighBourFactionsBitmap & (1 << fac2->factionID);
	}
	factionRanking* getFactionRanking(int turnNum)
	{
		return &factionRankings[turnNum];
	}
	factionEconomy* getFactionEconomy(int turnsAgo)
	{
		int turnIndex = counterEconomy - turnsAgo;
		if (turnIndex < 0)
			turnIndex += maxTurnsTillReset;
		return &factionEconomyTable[turnIndex];
	}
	bool isInNeighbourArray(const int regionId)
	{
		for (int i = 0; i < neighBourRegionsNum; ++i)
		{
			if (neighBourRegions[i] == regionId)
				return true;
		}
		return false;
	}
	void revealTile(int x, int y);
	void hideRevealedTile(int x, int y);
	int8_t getTileVisibility(const int x, const int y)
	{
		return tilesFac->tilesVisiblity[tilesFac->tilesXBound * y + x];
	}
	void setTileVisibility(const int x, const int y, const int8_t vis)
	{
		tilesFac->tilesVisiblity[tilesFac->tilesXBound * y + x] = vis;
	}
	void setColor(uint8_t r, uint8_t g, uint8_t b);
	void setSecondaryColor(uint8_t r, uint8_t g, uint8_t b);
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
	void setStrategy(aiCampaignStrategy strategy);
};

struct aiBrigandController : aiMilitaryController
{
	regionStruct* region;
	int strongestArmy;
	int weakestArmy;
};

struct factionRecord { /* see descr_sm_factions.txt */
	int id;
	char* facName;
	DWORD facNameHash;
	int shadowFactionId;
	char* shadowFaction;
	int shadowFactionHash;
	int32_t shadowFactionOwnerId;
	char* shadowFactionOwner;
	int shadowFactionOwnerHash;
	int32_t spawnsOnRevoltID;
	char* spawnsOnRevolt;
	int spawnsOnRevoltHash;
	int32_t spawnsOnRevoltOwnerID;
	char* spawnsOnRevoltOwner;
	int spawnsOnRevoltOwnerHash;
	bool spawnsOnEvent;
	char pad_0x1D[3];
	struct culture* culture;
	int religionID;
	struct model_Rigid* symbol;
	char* symbolPath;
	DWORD symbolPathHash;
	struct model_Rigid* rebel_symbol;
	char* rebel_symbolPath;
	DWORD rebel_symbolPathHash;
	char* card;
	DWORD cardHash;
	char* loading_logo;
	DWORD loading_logoHash;
	uchar primary_colour_blue;
	uchar primary_colour_green;
	uchar primary_colour_red;
	uchar secondary_colour_blue;
	uchar secondary_colour_green;
	uchar secondary_colour_red;
	undefined field_0x76[2];
	int triumph_value;
	int standard_index;
	int logo_index;
	int small_logo_index;
	bool roman;
	bool barbarian;
	bool eastern;
	bool slave;
	struct stringWithHash* hordeUnitNames;
	int32_t hordeUnitNamesSize;
	int32_t hordeUnitNamesCount;
	int32_t hordeMinUnits;
	int32_t hordeMaxUnits; //0x009C
	int32_t reductionPerHorde;
	int32_t hordeUnitPerSettlementPop;
	int32_t hordeMinNamedCharacters;
	int32_t hordeMaxPercentArmyStack;
	int* disbandPercentOnSettCapture;
	int32_t disbandPercentOnSettCaptureSize;
	int32_t disbandPercentOnSettCaptureNum;
	bool customBattleAvailability; //0x00BC
	char pad_00BD[3]; //0x00BD
	uint32_t periodsUnavailableInCustomBattle; //0x00C0
	bool canHorde; //0x00C4
	bool canSap; //0x00C5
	char pad_00C6[2]; //0x00C6
	int32_t prefersNavalInvasions; //0x00C8
	bool canHavePrincess; //0x00CC
	char pad_00CD[3]; //0x00CD
	char* specialFactionType; //0x00D0
	int32_t specialFactionTypeHash;
	bool hasFamilyTree; //0x00D8
	bool teutonic; //0x00D9
	bool disbandToPools; //0x00DA
	bool canBuildSiegeTowers; //0x00DB
	bool canTransmitPlague; //0x00DC
	char pad_00DD[3]; //0x00DD
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
}; //Size: 0x0604

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
	int getRecruitmentValue(int type)
	{
		return unitRecruitmentValues[type];
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
	static settlementTroopPolicy decideSettlementTroopPolicy(const settlementStruct* settlement);
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
};

namespace factionHelpers
{
	factionRecord* getFactionRecord(int id);
	std::string getFactionName(const factionStruct* fac);
	void changeFactionName(factionStruct* fac, const char* newName);
	UNICODE_STRING** getFactionNameLocal(factionStruct* fac);
	bool hasMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2);
	void setMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2, bool set);
	float getFactionStanding(const factionStruct* fac1, const factionStruct* fac2);
	watchTowerStruct* spawnWatchtower(const factionStruct* fac, int x, int y);
	void setFactionStanding(const factionStruct* fac1, const factionStruct* fac2, float standing);
	std::string getLocalizedFactionName(factionStruct* fac);
	void* disembark(armyStruct* army, int x, int y);
	armyStruct* splitArmy(factionStruct *faction, const sol::table& units, int x, int y);
	bool getTileVisibility(factionStruct* faction, int x, int y);

	ltgdGlobals* getLtgdGlobals();
	militaryValuesLTGD* getAiFactionValues(factionStruct* fac);
	interFactionLTGD* getInterFactionLTGD(factionStruct* fac, factionStruct* targetFac);
    void addToLua(sol::state& luaState);
}

