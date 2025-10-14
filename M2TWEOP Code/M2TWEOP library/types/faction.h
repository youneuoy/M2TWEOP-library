#pragma once
#include <lua/sol.hpp>
#include "realGameTypes.h"
#include "settlement.h"


struct smFactionsDb {
	struct factionRecord* facDescrs;
	int capacity;
	int size;
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
public:
	void updateFromObject(void* object);
	void update(factionStruct* fromFaction);
};

struct battleFactionCounter
{
public:
	int32_t battlesWon; //0x0000
	int32_t battlesLost; //0x0004
	int32_t lastResult; //0x0008
}; //Size: 0x000C
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
	characterRecord* getCharacterByLabel(const std::string& label);
	characterRecord* getFamilyHead();
	characterRecord* newRecord();
	void setLeader(characterRecord* newLeader, bool onlyLeader);
	void updateNeighbours();
	void setCapital(settlementStruct* newCapital);
	void setFactionBanner(const std::string& newFac);
	characterRecord* ancillaryExists(const std::string& ancName);
	int getAliveCharacterNumOfType(characterTypeStrat charType);
	int getCharacterCountOfType(int characterType)
	{
		return getAliveCharacterNumOfType(static_cast<characterTypeStrat>(characterType));
	}
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
	void getMapInfo(factionStruct* fromFaction)
	{
		tilesFac->update(fromFaction);
	}
	void hideRevealedTile(int x, int y);
	int8_t getTileVisibility(const int x, const int y)
	{
		return tilesFac->tilesVisiblity[tilesFac->tilesXBound * y + x];
	}
	void setTileVisibility(const int x, const int y, const int8_t vis)
	{
		tilesFac->tilesVisiblity[tilesFac->tilesXBound * y + x] = vis;
	}
	bool canSeeCharacter(character* candidate);
	
	void setColor(uint8_t r, uint8_t g, uint8_t b);
	void setSecondaryColor(uint8_t r, uint8_t g, uint8_t b);
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
	struct culture* facCulture;
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
	int getCultureId();
	void setCulture(int Id);
	void setFactionStratModel(const std::string& model, int characterType, int level);
	void setFactionBattleModel(const std::string& model, int characterType);
	std::string getLocalizedName();
};

struct facStratModel
{
	std::string model;
	int characterType;
	int level;
	facStratModel(std::string model, int charType, int lvl)
		: model(std::move(model)), characterType(charType), level(lvl) {}
};

struct facBattleModel
{
	std::string model;
	int characterType;
	facBattleModel(std::string model, int charType)
		: model(std::move(model)), characterType(charType) {}
};

class eopFactionData
{
public:
	uint8_t primaryColorR{};
	uint8_t primaryColorG{};
	uint8_t primaryColorB{};
	uint8_t secondaryColorR{};
	uint8_t secondaryColorG{};
	uint8_t secondaryColorB{};
	int cultureID{};
	std::vector<facStratModel> changedStratModels{};
	std::vector<facBattleModel> changedBattleModels{};
	nlohmann::json serialize() const
	{
		nlohmann::json json;
		json["primaryColorR"] = primaryColorR;
		json["primaryColorG"] = primaryColorG;
		json["primaryColorB"] = primaryColorB;
		json["secondaryColorR"] = secondaryColorR;
		json["secondaryColorG"] = secondaryColorG;
		json["secondaryColorB"] = secondaryColorB;
		json["cultureID"] = cultureID;

		for (const auto& model : changedStratModels)
		{
			nlohmann::json modelJson;
			modelJson["model"] = model.model;
			modelJson["characterType"] = model.characterType;
			modelJson["level"] = model.level;
			json["changedStratModels"].push_back(modelJson);
		}

		for (const auto& model : changedBattleModels)
		{
			nlohmann::json modelJson;
			modelJson["model"] = model.model;
			modelJson["characterType"] = model.characterType;
			json["changedBattleModels"].push_back(modelJson);
		}

		return json;
	}

	void deserialize(const nlohmann::json& json)
	{
		primaryColorR = json["primaryColorR"];
		primaryColorG = json["primaryColorG"];
		primaryColorB = json["primaryColorB"];
		secondaryColorR = json["secondaryColorR"];
		secondaryColorG = json["secondaryColorG"];
		secondaryColorB = json["secondaryColorB"];
		cultureID = json["cultureID"];

		for (const auto& modelJson : json["changedStratModels"])
		{
			facStratModel model(modelJson["model"], modelJson["characterType"], modelJson["level"]);
			changedStratModels.push_back(model);
		}

		for (const auto& modelJson : json["changedBattleModels"])
		{
			facBattleModel model(modelJson["model"], modelJson["characterType"]);
			changedBattleModels.push_back(model);
		}
	}
};

class eopFactionDataDb
{
public:
	void getOriginalData();
	void setNewStratModel(const int factionId, const std::string& model, int charType, int level);
	void setNewBattleModel(const int factionId, const std::string& model, int charType);
	static eopFactionDataDb* get() { return m_Instance.get(); }
	eopFactionData* getFactionData(const int factionId)
	{
		if (factionId < 0 || factionId >= static_cast<int>(m_FactionData.size()))
			return nullptr;
		return &m_FactionData[factionId];
	}
	eopFactionData* getOriginalFactionData(const int factionId)
	{
		if (factionId < 0 || factionId >= static_cast<int>(m_OriginalData.size()))
			return nullptr;
		return &m_OriginalData[factionId];
	}
	void checkInitialized();
	void restoreOriginalData();
	bool isRestoring() const
	{
		return m_Restoring;
	}
	nlohmann::json serialize()
	{
		nlohmann::json json;
		for (const auto& data : m_FactionData)
		{
			json.push_back(data.serialize());
		}
		return json;
	}
	void deserialize(const nlohmann::json& json)
	{
		for (size_t i = 0; i < json.size() && i < m_FactionData.size(); ++i)
		{
			m_FactionData[i].deserialize(json[i]);
		}
	}
	void onGameLoad(const std::vector<std::string>& filePaths);
	void onGameLoaded();
	std::string onGameSave();
private:
	static unique_ptr<eopFactionDataDb> m_Instance;
	std::array<eopFactionData, 31> m_FactionData{};
	std::array<eopFactionData, 31> m_OriginalData{};
	bool m_Initialized = false;
	bool m_Restoring = false;
};

namespace factionHelpers
{
	factionRecord* getFactionRecord(int id);
	int getFactionRecordNum();
	std::string getFactionName(const factionStruct* fac);
	void changeFactionName(factionStruct* fac, const char* newName);
	std::string getFactionNameLocalStr(const std::string& name);
	bool hasMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2);
	void setMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2, bool set);
	float getFactionStanding(const factionStruct* fac1, const factionStruct* fac2);
	watchTowerStruct* spawnWatchtower(const factionStruct* fac, int x, int y);
	void setFactionStanding(const factionStruct* fac1, const factionStruct* fac2, float standing);
	std::string getLocalizedFactionName(const factionStruct* fac);
	void* disembark(armyStruct* army, int x, int y);
	armyStruct* splitArmy(factionStruct *faction, const sol::table& units, int x, int y);
	bool getTileVisibility(factionStruct* faction, int x, int y);
    void addToLua(sol::state& luaState);
}

