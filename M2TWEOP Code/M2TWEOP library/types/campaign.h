#pragma once
#include "character.h"
#include "characterRecord.h"
#include "realGameTypes.h"
#include "lua/sol.hpp"
#include "faction.h"

struct regionStruct;
namespace campaignEnums
{
	enum dipRelEnum :int
	{
		war = 1,
		peace = 2,
		alliance = 3,
		suzerain = 4,
		trade = 6
	};
	enum dipStateInternalEnum :int
	{
		peaceState = 200,
		warState = 600,
		allianceState = 0
	};
}

struct rebelUnitName
{
public:
	char* unitName; //0x0000
	char pad_0004[20]; //0x0004
}; //Size: 0x0018

enum class dipStance
{
	alliance = 0,
	peace = 200,
	war = 600
};

inline bool operator == (int a, dipStance b)
{
	return a == static_cast<int>(b);
}

inline bool operator != (int a, dipStance b)
{
	return a != static_cast<int>(b);
}

inline bool operator > (int a, dipStance b)
{
	return a > static_cast<int>(b);
}

inline bool operator < (int a, dipStance b)
{
	return a < static_cast<int>(b);
}

inline bool operator >= (int a, dipStance b)
{
	return a >= static_cast<int>(b);
}

inline bool operator <= (int a, dipStance b)
{
	return a <= static_cast<int>(b);
}

struct descrRebelEntry
{
public:
	int32_t index; //0x0000
	char* name; //0x0004
	int32_t nameHash; //0x0008
	int32_t category; //0x000C
	struct N00022A30* localizedName; //0x0010
	int32_t chance; //0x0014
	struct rebelUnitName unitNames[121]; //0x0018
	int32_t unitCount; //0x0B70
}; //Size: 0x0B74

struct tradingResource {
	char* name;
	int nameCrypt;
	int null1;
	struct model_Rigid* model;
	char* item;
	int itemCrypt;
	int null3;
	int trade_value;
	char* icon;
	int iconCrypt;
	int null4;//
};

struct tradingResources {
	struct tradingResource resources[27];
	char* mine;
	int mineCrypt;
};

struct aiResourcePrivate
{
	DWORD *vfTable /*VFT*/;
	int resourceType;
	void* trackedCharVtbl;
	character* character;
	void* trackedArmyVtbl;
	struct armyStruct* army;
	int priority;
	void *aiController;
	int regionID;
	int lastOwner;
	bool valid;
	bool assigned;
	bool used;
	bool local;
	bool canMerge;
	char pad_2d[3];
	void *newController;
	int newPriority;
	void *newController2;
	int newPriority2;
};

struct 	mercPoolUnit
{
public:
    struct eduEntry* eduEntry; //0x0000
    int32_t experience; //0x0004
    int32_t cost; //0x0008
    float replenishMin; //0x000C
    float replenishMax; //0x0010
    int32_t maxUnits; //0x0014
    float currentPool; //0x0018
    float startYear; //0x001C
    float endYear; //0x0020
    gameStdVector<int> religions{}; //0x0024
    int32_t crusading; //0x0034
    gameStdVector<stringWithHash> events{}; //0x0024
    int16_t poolIndex; //0x0048
    int16_t mercPoolUnitIndex; //0x004A
    struct mercPool* mercPool; //0x004C
public:
	void setMercReligion(const int religion, const bool set)
	{
		auto religionVector = reinterpret_cast<std::vector<int>&>(religions);
		if (std::any_of(religionVector.begin(), religionVector.end(), [religion](const int r) { return r == religion; }) == set)
			return;
		if (set)
			religionVector.push_back(religion);
		else
			religionVector.erase(std::remove(religionVector.begin(), religionVector.end(), religion), religionVector.end());
	}
}; //Size: 0x0050


struct crusadeReward
{
	int money;
	int experience;
};

struct crusade
{
public:
	char pad_0000[20]; //0x0000
	void* N0000121D; //0x0014
	char pad_0018[28]; //0x0018
	int32_t startTurn; //0x0034
	int32_t endTurn; //0x0038
	struct settlementStruct* targetSettlement; //0x003C
	int32_t length; //0x0040
	int32_t outcome; //0x0044
	crusadeReward rewards[31];
	char targetViewers[12];
	uint32_t joinedFactions;
	uint32_t joinedFactionsLastTurn;
	uint32_t foughtFactions;
	uint32_t factionsInTarget;
	uint32_t unitCostTurn;
	uint32_t orthodoxId;
	uint32_t catholicId;
	uint32_t hereticId;
	stringWithHash targetName;
public:
	bool factionJoined(int factionId) const { return (joinedFactions & (1 << factionId)) != 0; }
	bool factionJoinedLastTurn(int factionId) const { return (joinedFactionsLastTurn & (1 << factionId)) != 0; }
	bool factionFought(int factionId) const { return (foughtFactions & (1 << factionId)) != 0; }
	bool factionInTarget(int factionId) const { return (factionsInTarget & (1 << factionId)) != 0; }
	crusadeReward* getReward(int factionId)
	{
		if (factionId < 0 || factionId >= 31)
			return nullptr;
		return &rewards[factionId];
	}
}; //Size: 0x0174

struct jihad
{
public:
	char pad_0000[20]; //0x0000
	void* N0000121D; //0x0014
	char pad_0018[28]; //0x0018
	int32_t startTurn; //0x0034
	int32_t endTurn; //0x0038
	struct settlementStruct* targetSettlement; //0x003C
	int32_t length; //0x0040
	int32_t outcome; //0x0044
	crusadeReward rewards[31];
	char targetViewers[12]; 
	uint32_t joinedFactions;
	uint32_t joinedFactionsLastTurn;
	uint32_t foughtFactions;
	uint32_t factionsInTarget;
	uint32_t unitCostTurn;
	uint32_t muslimId;
	stringWithHash targetName;
public:
	bool factionJoined(int factionId) const { return (joinedFactions & (1 << factionId)) != 0; }
	bool factionJoinedLastTurn(int factionId) const { return (joinedFactionsLastTurn & (1 << factionId)) != 0; }
	bool factionFought(int factionId) const { return (foughtFactions & (1 << factionId)) != 0; }
	bool factionInTarget(int factionId) const { return (factionsInTarget & (1 << factionId)) != 0; }
	crusadeReward* getReward(int factionId)
	{
		if (factionId < 0 || factionId >= 31)
			return nullptr;
		return &rewards[factionId];
	}
}; //Size: 0x016C

struct mercPool
{
public:
    char* name; //0x0000
    int nameHash; //0x0004
    struct gameList<regionStruct> regions; //0x0008
    gameList<mercPoolUnit> mercenaryUnits; //0x001C
    settlementRecruitmentPool* disbandedUnits;
    int disbandedUnitsSize;
    int disbandedUnitsNum;
public:
	mercPoolUnit* addMercUnit(int idx, int exp, int cost, float repMin, float repMax, int maxUnits, float startPool,
		float startYear, float endYear, int crusading);
	
	int getMercUnitNum()
	{
		return mercenaryUnits.size();
	}
	mercPoolUnit* getMercUnit(const int index)
	{
		if (index < 0 || index >= mercenaryUnits.size())
			return nullptr;
		return mercenaryUnits.get(index);
	}
}; //Size: 0x0180

struct factionDiplomacy
{
public:
    int prevState;
    int32_t state;
    __int32 hasTradeRights : 1;
    __int32 hasMilitaryAccess : 1;
    __int8 isKnown : 1;
    __int8 lastDiplomaticResult : 1;
    __int8 isTrustWorthy : 1;
    __int8 isProtectorate : 1;
    float factionStanding;
    int32_t protectoratePayment;
    int32_t numTurnsAllied;
    int32_t numTurnsWar;
    int32_t numTurnsState;
    int32_t numTurnsTrade;
    int32_t numTurnsAccess;
    int numTurnsProtectorate;
    int numTreaties;
    int numAssists;
    int numAssistFailures;
    int numEmptyThreats;
    int numValidThreats;
    int32_t leaveTerritoryTimer;
    int winsInBattle;
    int tributeAmount;
    int tributeDuration;
    int preWarRegions[8];
    int turnsSinceLastMeeting;
    int32_t diplomaticSuccess;
    int diplomaticBalance;
    int32_t lastMapInfoTrade;
    int numTurnsCeasefire;
public:
	bool getHasTradeRights() const { return hasTradeRights; }
	bool getHasMilitaryAccess() const { return hasMilitaryAccess; }
	bool getIsTrustWorthy() const { return isTrustWorthy; }
	bool getIsProtectorate() const { return isProtectorate; }
	bool getIsKnown() const { return isKnown; }
	bool getLastDiplomaticResult() const { return lastDiplomaticResult; }
	void setHasMilitaryAccess(bool value) { hasMilitaryAccess = value; }
	void setIsTrustWorthy(bool value) { isTrustWorthy = value; }
	void setIsKnown(bool value) { isKnown = value; }
	void setLastDiplomaticResult(bool value) { lastDiplomaticResult = value; }
}; //Size: 0x0088


struct cardinal
{
public:
    struct trackedCharacter trackedCharacter; //0x0000
    int factionID; //0x0004
    int voteFactionID;
    bool isPreferati;
    char pad[3];
    UNICODE_STRING** cardinalRank;
    UNICODE_STRING** cardinalName;
    char* cardPath;
    int cardPathHash;
	character* getCharacter()
	{
		return trackedCharacter.character;
	}
}; //Size: 0x0008

struct cardinalSP
{
	cardinal* cardinal;
	void* sharedPointer;
};

struct collegeOfCardinals
{
public:
    DWORD vTable; //0x0000
    struct characterRecord* pope; //0x0004
    int papalFactionID; //0x0008
    struct cardinalSP* cardinalsArray; //0x000C
    int32_t cardinalsArraySize; //0x0010
    int32_t cardinalNum; //0x0014
    bool popeOld;
    bool inElection;
    bool electionInTurn;
    int8_t byte_1b;
    struct cardinalSP* electionResults;
    int32_t electionResultsSize;
    int32_t electionResultsCount;
    struct UNICODE_STRING **lastPopeName;
    int32_t lastPopeFactionID;
    int32_t popePreviousFaction;
    int32_t maxCardinals;
    int32_t maxPreferati;
	cardinal* getCardinal(int index)
	{
		if (index < 0 || index >= cardinalNum)
			return nullptr;
		return cardinalsArray[index].cardinal;
	}
	cardinal* getElectionResult(int index)
	{
		if (index < 0 || index >= cardinalNum)
			return nullptr;
		return electionResults[index].cardinal;
	}
}; //Size: 0x004C



/* 1746 */
struct otherPlayerId
{
	bool inBattle;
	bool turnEnded;
	bool isAutoresolving;
	bool autoResolve;
	int factionID;
};

/* 2505 */
struct weatherValues
{
	float sunDirectionX;
	float sunDirectionZ;
	float sunDirectionY;
	float mainColorR;
	float mainColorG;
	float mainColorB;
	float mainColorA;
	float enviromentShadeR;
	float enviromentShadeG;
	float enviromentShadeB;
	float enviromentShadeA;
	float entityGlowR;
	float entityGlowG;
	float entityGlowB;
	float entityGlowA;
};

/* 2509 */
struct weatherData
{
	struct weatherValues weatherValues;
	float envMapIntensity;
	int8_t fogR;
	int8_t fogG;
	int8_t fogB;
	int8_t fogA;
	float fogMin;
	float fogMax;
	float cloudFadeMin;
	float cloudFadeMax;
	float windStrength;
	int32_t rainType;
	int32_t rainLevel;
	float lightningFrequencyMin;
	float lightningFrequencyMax;
	float lightningDistributionX;
	float lightningDistributionY;
	int32_t lightningDistributionRadius;
	uint8_t bloomRed;
	uint8_t bloomGreen;
	uint8_t bloomBlue;
	uint8_t bloomSelect;
	uint8_t bloomIntensity;
	char pad_007D[3];
};

struct campaign {
	char padStart[32];
	bool firstOpen;
	bool firstOpen2;
	int8_t field_0022[2];
	int32_t type;
	bool isOpen;
	int8_t field_0029[3];
	uint32_t playableFactionsBitmap; //0x002C
    struct otherPlayerId playerIds[31];
	int humanPlayers; /* number of player-controlled factions */
	int playerFacBitMap; /* pow(2,factionId) */
	int32_t campaignDifficultyFaction[31];
	int32_t battleDifficultyFaction[31];
	struct factionStruct* factionsSortedByTurnOrder[31];
	struct factionStruct* factionsSortedByID[31];
	int factionCount;
	uint32_t existingFactionsBitMap; //0x0324
    int32_t worldLeaders[4];
	struct collegeOfCardinals* collegeOfCardinals; //0x033C
	struct factionStruct* papalFaction; //0x0340
	struct factionStruct* currentFactionTurn;
	int factionTurnID; /* FactionTurn? */
	UNICODE_STRING** currentDescrFile; /* path to descr_battle.txt or to descr_strat.txt */
	bool fogOfWar; //0x0350
	int8_t byte_34d[3];
	gameStdVector<void*> aiTurnEvents;
	bool pendingAllianceDecision;
	bool speedUp;
	bool ignoreSpeedUp;
	bool isPaused;
	bool followMovement;
	int8_t field_0365[3];
	int tickCount; /* TickCount? */
	int millisecondCount;
	float secondCount;
	int turnNumber;
	int field25_0x37c; /* PauseForBattleDialog? */
	float timeScale;
	undefined field27_0x384[4];
	struct settlementStruct* rome;
	undefined field29_0x38c[4];
	struct settlementStruct* constantinople;
	bool isScriptedBattle;
	bool battleDayTypeFixed;
	int8_t byte_0392[2];
	char *weatherPatternFixed;
	int weatherPatternFixedHash;
	void *battleVolcanos;
	int32_t battleVolcanosSize;
	int32_t battleVolcanosCount;
	void *battleFords;
	int32_t battleFordsSize;
	int32_t battleFordsCount;
	void* script; //0x03B4
	void* scriptEnd; //0x03B8
	char pad_03BC[8]; //0x03BC
	void* pointerToScript; //0x03C4
	void* pointerToScriptEnd; //0x03C8
	int8_t isPrologueMap; //0x03CC
	int8_t isPrologue; //0x03CD
	int8_t filteredUiCommand;
	int8_t pad3cf;
	int8_t uiSaveLoaded;
	int8_t byte_03d1[3];
	int uiSaveMode;
	int uiSaveLastMode;
	bool movieMode;
	bool movieView;
	bool uiSaveHidden;
	bool uiSaveTooltips;
	bool marianReformsActive; //0x03E0
	bool marianReformsDisabled; //0x03E1
	bool rebellingCharactersActive; //0x03E2
	bool gladiatorUprisingDisabled; //0x03E3
	bool nightBattlesEnabled; //0x03E4
    int8_t byte3e5[3];
	float brigandSpawnValue;
	float pirateSpawnValue;
	int8_t romanoBritishEmerged; //0x03F4
	int8_t showDateAsTurns; //0x03F5
	int8_t restrictAutoResolve; //0x03F6
	char pad_03F7[1]; //0x03F7
	int freeUpkeepForts;
	int32_t slaveFactionID;
	void *harvestEvents;
	int32_t harvestEventsSize;
	int32_t harvestEventsCount;
	int lastRandomSeed;
	struct crusade crusade; //0x0410
	struct jihad jihad; //0x0584
	struct UNICODE_STRING** introFmv; //0x06F0
	int8_t savePrefs; //0x06F4
	int8_t saveEnabled; //0x06F5
	bool isUpdatingFinanceScroll;
	int8_t byte_6f3;
	int adminPwHash;
	bool hasAdminPw;
	bool adminLoggedOn;
	bool validatedSave;
	bool validatedData;
	int8_t majorGameVersion;
	int8_t minorGameVersion;
	int8_t field_06Fe[2];
	int campaignWinnerID;
	bool reactivateMessages;
	int8_t pad705[3];
	float currentDate;
	int32_t season;
	float startDate;
	int32_t startSeason;
	float endDate;
	int32_t endSeason;
	int32_t daysAtStart;
	float timeAtStartBattle;
	int daysInBattle;
	float currentTimeInBattle; /* 24 max, so calc as daysInBattle*24+currentTimeInBattle */
	int32_t daysAtEnd;
	float timeAtEndBattle;
	int battleTimer;
	int dayTime;
	bool seasonChanged;
	char padSeason[3];
	int weatherSeason;
	int weatherClimate;
	float timeOfDay;
	void *weatherPattern;
	void *weatherPeriod;
	void *globalLighting;
	void *globalLightingSharedPointer;
	weatherData weatherData;
	float timeSinceBattleStart;
	float floatColorR;
	float floatColorG;
	float floatColorB;
	float fogData1;
	float fogData2;
	float fogData3;
	float fogData4;
	int randomSeedWeather;
	float lastWindSpeed;
	float windSpeed;
	float lastWindDirX;
	float lastWindDirZ;
	float lastWindDirY;
	float windDirX;
	float windDirZ;
	float windDirY;
	float lastSomethingWeather;
	float somethingWeather;
	float currentWindSpeed;
	float currentWindDirX;
	float currentWindDirZ;
	float currentWindDirY;
	float currentWindVelocityX;
	float currentWindVelocityZ;
	float currentWindVelocityY;
	float visibility;
	float visibility2;
	float somethingLightning;
	float nextStrike;
	int32_t interFactionMarriage[31][31]; //0x0858
	struct factionDiplomacy diplomaticStandings[31][31]; //0x175C
	gameList<settlementStruct*> settlements;
	gameList<settlementStruct*> settlementsEmpty;
	struct fortStruct** fortsArray;
	int32_t fortsSize; //0x21610
	int32_t fortsNum; //0x21614
	struct portBuildingStruct** portsBuildings;
	int32_t portsBuildingsSize; //0x2161C
	int32_t portsBuildingsNum; //0x21620
	struct watchTowerStruct** watchtowers;
	int32_t watchtowersSize; //0x21628
	int32_t watchtowersNum; //0x2162C
	struct roadStruct **roads;
	int32_t roadsSize;
	int32_t roadsNum;
	bool updateRoads;
	int8_t pad_21639[3];
	gameList<mercPool> mercPools;
	struct UNICODE_STRING **campaignString;
	struct UNICODE_STRING **campaignDirString;
public:
	factionStruct* getPlayerFaction(int index)
	{
		if (index < 0 || index >= humanPlayers)
			return nullptr;
		const auto playerData = &playerIds[index];
		return factionsSortedByID[playerData->factionID];
	}
	factionStruct* getFactionById(int index)
	{
		if (index < 0 || index >= 31)
			return nullptr;
		return factionsSortedByID[index];
	}
	factionStruct* getFactionByOrder(int index)
	{
		if (index < 0 || index >= factionCount)
			return nullptr;
		return factionsSortedByTurnOrder[index];
	}
	bool isPlayableFaction(int index)
	{
		if (index < 0 || index >= 31)
			return false;
		return (playableFactionsBitmap & (1 << index)) != 0;
	}
	bool isPlayerFaction(int index)
	{
		if (index < 0 || index >= 31)
			return false;
		return (playerFacBitMap & (1 << index)) != 0;
	}
	bool isExistingFaction(int index)
	{
		if (index < 0 || index >= 31)
			return false;
		return (existingFactionsBitMap & (1 << index)) != 0;
	}
	int getNumberOfMarriages(int faction1, int faction2)
	{
		if (faction1 < 0 || faction1 >= 31 || faction2 < 0 || faction2 >= 31)
			return 0;
		return interFactionMarriage[faction1][faction2];
	}
	factionDiplomacy* getFactionDiplomacy(int faction1, int faction2)
	{
		if (faction1 < 0 || faction1 >= 31 || faction2 < 0 || faction2 >= 31)
			return nullptr;
		return &diplomaticStandings[faction1][faction2];
	}
	int getSettlementNum()
	{
		return settlements.size();
	}
	settlementStruct* getSettlement(int index)
	{
		if (index < 0 || index >= settlements.size())
			return nullptr;
		return *settlements.get(index);
	}
	fortStruct* getFort(int index)
	{
		if (index < 0 || index >= fortsNum)
			return nullptr;
		return fortsArray[index];
	}
	portBuildingStruct* getPort(int index)
	{
		if (index < 0 || index >= portsBuildingsNum)
			return nullptr;
		return portsBuildings[index];
	}
	watchTowerStruct* getWatchTower(int index)
	{
		if (index < 0 || index >= watchtowersNum)
			return nullptr;
		return watchtowers[index];
	}
	int getMercPoolNum()
	{
		return mercPools.size();
	}
	mercPool* getMercPool(int index)
	{
		if (index < 0 || index >= mercPools.size())
			return nullptr;
		return mercPools.get(index);
	}
	mercPool* getMercPoolByName(const std::string& name)
	{
		const int num =  mercPools.size();
		for (int i = 0; i < num; i++)
		{
			if (strcmp(mercPools.get(i)->name, name.c_str()) == 0)
				return mercPools.get(i);
		}
		return nullptr;
	}
	roadStruct* getRoad(int index)
	{
		if (index < 0 || index >= roadsNum)
			return nullptr;
		return roads[index];
	}
	factionStruct* getSlaveFaction()
	{
		return getFactionById(slaveFactionID);
	}
	void setDipStance(campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2);
	bool checkDipStance(const campaignEnums::dipRelEnum dipType, const factionStruct* fac1, const factionStruct* fac2);
	void setFactionTrade(factionStruct* factionOne, factionStruct* factionTwo);
	void setFactionProtectorate(factionStruct* factionOne, factionStruct* factionTwo);
	settlementStruct* getSettlementByName(const char* name);
	factionStruct* getFactionHashed(const std::string& name);
	std::string getCampaignPath();
};

struct stratPortModel {
	struct model_Rigid* model_rigid;
	undefined field_0x4[4];
	char* modelPath;
	int modelPathCrypt;
};

//port
struct portBuildingStruct {
	DWORD* vtbl;
	void* nextObject;
	void* prevObject;
	int xCoord;
	int yCoord;
	int fade;
	bool render;
	char pad0[3];
	float opacity;
	bool highlighted;
	char pad1[3];
	trackedObject object;
	struct settlementStruct* settlement;
	int regionID;
	int16_t angle;
	int16_t anglePad;
	int portLevel;
	int subFactionCultureID;
	void *trackedPointerArmyVtbl;
	struct armyStruct* occupantsArmy; /* if port block by army then it here */
	int32_t numTurnsBlocked; //0x004C
	int dockX;
	int dockY;
	int32_t rallyCoordX; //0x0058
	int32_t rallyCoordY; //0x005C
	void* rallyPoint; //0x0060
	struct factionStruct* fac; /* blockading faction becomes owner */
	undefined field_0x68[4];
	struct armyStruct* army;
	int partType;
	struct stratPortModel* portStratModel;
	struct portDockStrat* portDock;
};

//trading resource on stratmap
struct resourceStruct {
	DWORD* vtbl;
	void* nextObject;
	void* prevObject;
	int xCoord;
	int yCoord;
	int fade;
	bool render;
	char pad0[3];
	float opacity;
	bool highlighted;
	char pad1[3];
	struct stratResMod* stratMod;
	struct settlementStruct* settlement;
	int regionID;
	int resourceID;
	int duration;
public:
	void setModel(UINT32 modelId);
	int getResourceCode()
	{
		return stratMod->resource_num;
	}
	int getResourceCost()
	{
		return stratMod->resource_cost;
	}
	bool hasMineBuilt()
	{
		return settlement->getSettlementCapabilityEnum(buildingCapabilities::mine_resource) && stratMod->hasMine; 
	}
	int getResourceHasMine()
	{
		return stratMod->hasMine;
	}
	std::string getImage()
	{
		return stratMod->tga;
	}
};

//dock (water tile)
struct portDockStrat {
	DWORD* vtbl;
	void* nextObject;
	void* prevObject;
	int xCoord;
	int yCoord;
	int fade;
	bool render;
	char pad0[3];
	float opacity;
	bool highlighted;
	char pad1[3];
	trackedObject object;
	struct settlementStruct* settlement;
	int regionID;
	int16_t angle;
	int16_t anglePad;
	int portLevel;
	int subFactionCultureID;
	void *trackedPointerArmyVtbl;
	struct armyStruct* occupantsArmy; /* if port block by army then it here */
	int32_t numTurnsBlocked; //0x004C
	int dockX;
	int dockY;
	int32_t rallyCoordX; //0x0058
	int32_t rallyCoordY; //0x005C
	void* rallyPoint; //0x0060
	struct factionStruct* fac; /* blockading faction becomes owner */
	undefined field_0x68[4];
	struct armyStruct* army;
	int partType;
	struct stratPortModel* dockStratModel;
	struct portBuildingStruct* port;
	void* trackedPointerArmyVtbl3;
	struct armyStruct* dockedArmy;
};

struct watchTowerModel {
	struct model_Rigid* modelP;
	undefined field_0x4[26];
};

//watchtower
struct watchTowerStruct {
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
	struct watchTowerModel* model;
	int32_t regionID; //0x0028
	struct factionStruct* faction; //0x002C
	struct settlementStruct* settlement; //0x0030
	void* trackedPointerArmyVtable; //0x0034
	struct armyStruct* blockingArmy; //0x0038
	int32_t factionID; //0x003C
};

struct campaignDifficulty1
{
	int orderFromGrowth;
	int considerWarWithPlayer;
	float difficultyFloat1_unused;
	float brigandChanceAi;
	float brigandChancePlayer;
	int forceAttackDelay;
	float taxIncomeModifierPlayer;
	float farmingIncomeModifierPlayer;
	float incomeModifierAi;
	float playerRegionValueModifier;
};

struct campaignDifficulty2
{
public:
	int32_t popGrowthBonusAi; //0x0000
	int32_t publicOrderBonusAi; //0x0004
	int32_t experienceBonusAi; //0x0008
	int32_t difficultyInt4_unused; //0x000C
	int32_t incomeBonusAi; //0x0010
	int8_t wantsTargetPlayer; //0x0014
	int8_t wantsTargetPlayerNaval; //0x0015
	char pad_0016[6]; //0x0016
	int32_t autoAttackPlayerIfCrusadeTarget; //0x001C
	char pad_0020[32]; //0x0020
}; //Size: 0x0040

struct movePoint
{
	float float1;
	int32_t field4;
	float float2;
	int32_t tileIndex;
	int32_t xCoord;
	int32_t yCoord;
	struct movePoint *nextCoords;
};

struct movementExtentTile
{
	int tileIndex;
	float movePoints;
	int8_t turns;
	int8_t inOpenList;
	char pad[2];
	oneTile* getTile();
};

struct characterMovementExtents
{
	void * vtbl;
	int totalTiles;
	int searchType;
	int tileIndex;
	int xCoordMin;
	int yCoordMin;
	int xCoordMax;
	int yCoordMax;
	gameStdVector<movementExtentTile> movementExtentTiles;
	struct character *character;
	int turns;
	movementExtentTile* getTile(const int xCoord, const int yCoord)
	{
		if (xCoord < xCoordMin || xCoord > xCoordMax || yCoord < yCoordMin || yCoord > yCoordMax)
			return nullptr;
		return &movementExtentTiles[(yCoord - yCoordMin) * (xCoordMax - xCoordMin + 1) + (xCoord - xCoordMin)];
	}
};


enum class searchType
{
	avoidZoc,
	ignoreZoc,
	direct,
	militaryAccess,
	landingPoints,
	region,
	strategic
};

inline bool operator == (const searchType& a, const int& b)
{
	return static_cast<int>(a) == b;
}

inline bool operator != (const searchType& a, const int& b)
{
	return static_cast<int>(a) != b;
}

struct moveDataSettlement
{
	settlementStruct* settlement;
	float moveCost;
	int8_t turns;
	bool ownFaction;
	moveDataSettlement(settlementStruct* foundObj, const movementExtentTile* tileEx, const bool ownFaction)
		: settlement(foundObj), moveCost(tileEx->movePoints), turns(tileEx->turns), ownFaction(ownFaction)
	{}
};

struct moveDataCharacter
{
	character* thisChar;
	float moveCost;
	int8_t turns;
	bool ownFaction;
	moveDataCharacter(character* foundObj, const movementExtentTile* tileEx, const bool ownFaction)
		: thisChar(foundObj), moveCost(tileEx->movePoints), turns(tileEx->turns), ownFaction(ownFaction)
	{}
};

struct moveDataArmy
{
	armyStruct* army;
	float moveCost;
	int8_t turns;
	bool ownFaction;
	moveDataArmy(armyStruct* foundObj, const movementExtentTile* tileEx, const bool ownFaction)
		: army(foundObj), moveCost(tileEx->movePoints), turns(tileEx->turns), ownFaction(ownFaction)
	{}
};

struct moveDataResource
{
	resourceStruct* resource;
	float moveCost;
	int8_t turns;
	bool ownFaction;
	moveDataResource(resourceStruct* foundObj, const movementExtentTile* tileEx, const bool ownFaction)
		: resource(foundObj), moveCost(tileEx->movePoints), turns(tileEx->turns), ownFaction(ownFaction)
	{}
};

struct moveDataFort
{
	fortStruct* fort;
	float moveCost;
	int8_t turns;
	bool ownFaction;
	moveDataFort(fortStruct* foundFort, const movementExtentTile* tileEx, const bool ownFaction)
		: fort(foundFort), moveCost(tileEx->movePoints), turns(tileEx->turns), ownFaction(ownFaction)
	{}
};

struct moveDataTile
{
	oneTile* tile;
	int xCoord;
	int yCoord;
	float moveCost;
	int8_t turns;
	moveDataTile(movementExtentTile* tileEx)
	{
		tile = tileEx->getTile();
		auto [x, y] = tile->getTileCoords();
		xCoord = x;
		yCoord = y;
		moveCost = tileEx->movePoints;
		turns = tileEx->turns;
	}
};

struct characterMoveData
{
	character* thisChar{};
	int xCoord{};
	int yCoord{};
	float movePoints{};
	int searchType{};
	int turns{};
	characterMovementExtents* moveExtents{};
	std::vector<moveDataSettlement> settlements{};
	std::vector<moveDataCharacter> characters{};
	std::vector<moveDataArmy> armies{};
	std::vector<moveDataResource> resources{};
	std::vector<moveDataFort> forts{};
	std::vector<moveDataTile> tiles{};
	characterMoveData(character* charPtr, int searchType, int turns);
	int getSettlementCount() const { return settlements.size(); }
	int getCharacterCount() const { return characters.size(); }
	int getArmyCount() const { return armies.size(); }
	int getResourceCount() const { return resources.size(); }
	int getFortCount() const { return forts.size(); }
	int getTileCount() const { return tiles.size(); }
	moveDataSettlement* getSettlement(const int index)
	{
		if (index < 0 || index >= static_cast<int>(settlements.size()))
			return nullptr;
		return &settlements[index];
	}
	moveDataCharacter* getCharacter(const int index)
	{
		if (index < 0 || index >= static_cast<int>(characters.size()))
			return nullptr;
		return &characters[index];
	}
	moveDataArmy* getArmy(const int index)
	{
		if (index < 0 || index >= static_cast<int>(armies.size()))
			return nullptr;
		return &armies[index];
	}
	moveDataResource* getResource(const int index)
	{
		if (index < 0 || index >= static_cast<int>(resources.size()))
			return nullptr;
		return &resources[index];
	}
	moveDataFort* getFort(const int index)
	{
		if (index < 0 || index >= static_cast<int>(forts.size()))
			return nullptr;
		return &forts[index];
	}
	moveDataTile* getTile(const int index)
	{
		if (index < 0 || index >= static_cast<int>(tiles.size()))
			return nullptr;
		return &tiles[index];
	}
	void sortSettlementsDistance()
	{
		std::sort(settlements.begin(), settlements.end(), [](const moveDataSettlement& a, const moveDataSettlement& b)
			{
				return a.moveCost < b.moveCost;
			});
	}
	void sortSettlementsStrength();
	void sortCharactersDistance()
	{
		std::sort(characters.begin(), characters.end(), [](const moveDataCharacter& a, const moveDataCharacter& b)
			{
				return a.moveCost < b.moveCost;
			});
	}
	void sortArmiesDistance()
	{
		std::sort(armies.begin(), armies.end(), [](const moveDataArmy& a, const moveDataArmy& b)
			{
				return a.moveCost < b.moveCost;
			});
	}
	void sortArmiesStrength();
	void sortResourcesDistance()
	{
		std::sort(resources.begin(), resources.end(), [](const moveDataResource& a, const moveDataResource& b)
			{
				return a.moveCost < b.moveCost;
			});
	}
	void sortFortsDistance()
	{
		std::sort(forts.begin(), forts.end(), [](const moveDataFort& a, const moveDataFort& b)
			{
				return a.moveCost < b.moveCost;
			});
	}
	void sortFortsStrength();
	void sortTilesDistance()
	{
		std::sort(tiles.begin(), tiles.end(), [](const moveDataTile& a, const moveDataTile& b)
			{
				return a.moveCost < b.moveCost;
			});
	}
};

struct moveExtentsManager
{
	gameStdVector<characterMovementExtents> moveExtents;
	gameStdVector<characterMovementExtents> smpExtents;
	characterMovementExtents* createMoveExtents(character* searchChar, int searchType, int numTurns);
};

struct stratPathFinding
{
	float minMpCost;
	struct trackedCharacter trackedPointerCharacter;
	int characterType;
	struct factionStruct *faction;
	int invalidOrReached;
	bool followCharacters;
	bool displayStandards;
	int8_t bytex1A[2];
	int cameraMode;
	bool exchangeScroll;
	bool waitingForScroll;
	bool waitingForScrollInTurnEnd;
	int8_t bytex23;
	void *floatingGeneral1;
	struct character *someCharacter;
	void *charVerification;
	void *charVerificationPos;
	int charVerThing;
	int battleResolution;
	int endTurnX;
	int endTurnY;
	int stratToBattleX;
	int stratToBattleY;
	struct character *lastSelectedCharacter;
	struct settlementStruct *lastSelectedSettlement;
	struct fortStruct *lastSelectedFort;
	int conflictType;
	int8_t isNightBattle;
	int8_t fieldx5D[3];
	armyStruct *attackingArmy;
	armyStruct *defendingArmy;
	int campaignBattlePhase;
	bool hideDisplay;
	char padHide[3];
	struct character **displacedCharacters;
	int displacedCharactersSize;
	int displacedCharactersNum;
	struct character **nextTurnCharacters; 
	int nextTurnCharactersSize;
	int nextTurnCharactersNum;
	moveExtentsManager extentsManager;
	struct coordPair *coords;
	int coordsSize;
	int coordsNum;
};


struct sundryStruct
{
	void* vtable; //0x0000
	DWORD* nextObject; //0x0004
	DWORD* previousObject; //0x0008
	int xCoord;
	int yCoord;
	int somethingIdle;
	bool inSettOrNotArmyLead;
	char pad19[3];
	float floatCheckedForLessThen1ForMove;
	bool fieldx20;
	char pad_0021[3]; //0x0021
	int selectIndicators;
	int field_28;
	float xCoordFloat;
	int field_30;
	float yCoordFloat;
	int8_t gap38[4];
	struct character *selectedCharacter;
	struct settlementStruct *selectedSettlement;
	struct fortStruct *selectedFort;
	int field_48;
	float field_4C_rapidChange;
	int field_50_init1_changeToMinusOne;
	int selectionType;
};

struct rallyPointStruct : sundryStruct
{
	int field_58_initMinus1;
};

struct selectionInfo
{
	float floats[119];
	struct sundryStruct *selectedLandUnitCard;
	struct sundryStruct *selectedShipUnitCard;
	struct sundryStruct *selectedCharacter;
	struct sundryStruct *hoveredCharacter;
	struct sundryStruct *selectedSettlement;
	struct sundryStruct *hoveredSettlement;
	struct sundryStruct *selectedRallyPoint;
	struct sundryStruct *hoveredRallyPoint;
	struct sundryStruct *selectedEnemyCharacter;
	struct sundryStruct *selectedEnemySettlement;
	struct rallyPointStruct **rallyPoints;
	int32_t rallyPointsSize;
	int32_t rallyPointsNum;
	int32_t N0002DC9B;
	void *array22;
	int32_t array22Size;
	int32_t array22Count;
public:
	character* getSelectedCharacter()
	{
		if (!selectedCharacter)
			return nullptr;
		return selectedCharacter->selectedCharacter;
	}
	character* getHoveredCharacter()
	{
		if (!hoveredCharacter)
			return nullptr;
		return hoveredCharacter->selectedCharacter;
	}
	character* getSelectedEnemyCharacter()
	{
		if (!selectedEnemyCharacter)
			return nullptr;
		return selectedEnemyCharacter->selectedCharacter;
	}
	settlementStruct* getSelectedSettlement()
	{
		if (!selectedSettlement)
			return nullptr;
		return selectedSettlement->selectedSettlement;
	}
	settlementStruct* getHoveredSettlement()
	{
		if (!hoveredSettlement)
			return nullptr;
		return hoveredSettlement->selectedSettlement;
	}
	settlementStruct* getSelectedEnemySettlement()
	{
		if (!selectedEnemySettlement)
			return nullptr;
		return selectedEnemySettlement->selectedSettlement;
	}
	fortStruct* getSelectedFort()
	{
		if (!selectedSettlement)
			return nullptr;
		return selectedSettlement->selectedFort;
	}
	fortStruct* getHoveredFort()
	{
		if (!hoveredSettlement)
			return nullptr;
		return hoveredSettlement->selectedFort;
	}
	fortStruct* getSelectedEnemyFort()
	{
		if (!selectedEnemySettlement)
			return nullptr;
		return selectedEnemySettlement->selectedFort;
	}
};

namespace campaignHelpers
{
    campaign* getCampaignData();
	UINT32 getFactionsCount();
	stratPathFinding* getStratPathFinding();
	selectionInfo* getSelectionInfo();
	campaignDifficulty1* getCampaignDifficulty1();
	campaignDifficulty2* getCampaignDifficulty2();
	void setPerfectSpy(bool set);
	int getLocalFactionID();
	factionStruct* getFaction(int index);
	int getPoolIndex(const gameList<mercPoolUnit> *unitPtr);
	mercPoolUnit* getNewMercUnit(gameList<mercPoolUnit>* unitPtr);
	bool checkDipStance(const campaign* campaignStruct, campaignEnums::dipRelEnum dipType, const factionStruct* fac1, const factionStruct* fac2);
	void setFactionTrade(factionStruct* factionOne, factionStruct* factionTwo);
	void setFactionProtectorate(factionStruct* factionOne, factionStruct* factionTwo);
    void addToLua(sol::state& luaState);
};
