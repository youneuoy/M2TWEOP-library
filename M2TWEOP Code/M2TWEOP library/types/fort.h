#pragma once
#include "json.hpp"
#include "realGameTypes.h"
#include "lua/sol.hpp"

struct fortStruct
{
public:
	//fields
    DWORD* fortVtable;
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
    int8_t plagued;
    int8_t salliedOut; //0x00A5
    int8_t gatesAreOpened;
    int8_t readyToSurrender;
    struct crusade* takenByCrusade;
    struct descrRebelEntry* descrRebel; //0x00AC
    int32_t subFactionID; //0x00B0
	spyingInfo spyInfo;
    struct cultureFort* stratModel;
    int regionID;
    struct factionStruct* faction;
    int32_t creatorFactionID; //0x00EC
    int32_t creatorCultureID; //0x00F0
    UNICODE_STRING** name; //0x00F4
    char* fortType;
    int32_t fortTypeHash;
	int32_t fortFortificationLevel; //0x0100

public :
	//methods
	siegeS* getSiege(const int index)
	{
		return sieges[index].siege;
	}
	bool isAllyToFaction(const factionStruct* otherFac);
	bool isEnemyToFaction(const factionStruct* otherFac);
	int characterCount();
	character* getCharacter(int index);
	character* getGovernor()
	{
		if (characterCount() > 0)
			return getCharacter(0);
		return nullptr;
	}
};

class eopFortData
{
public:
	eopFortData() = default;
	eopFortData(const int x, const int y, const int modelId, const int modelIdWalls)
		: xCoord(x), yCoord(y), modelId(modelId), modelIdWalls(modelIdWalls) {}
	int xCoord{};
	int yCoord{};
	int modelId = -1;
	int modelIdWalls = -1;
	nlohmann::json serialize()
	{
		nlohmann::json json;
		json["xCoord"] = xCoord;
		json["yCoord"] = yCoord;
		json["modelId"] = modelId;
		json["modelIdWalls"] = modelIdWalls;
		return json;
	}
	void deserialize(const nlohmann::json& json)
	{
		xCoord = json["xCoord"];
		yCoord = json["yCoord"];
		modelId = json["modelId"];
		modelIdWalls = json["modelIdWalls"];
	}
private:
};

class eopFortDataDb
{
public:
	static eopFortDataDb* get() { return m_Instance; }
	eopFortData* getFortData(int x, int y);
	void addFortData(int x, int y, int modelId, int modelIdWalls);
	void removeFortData(int x, int y);
	nlohmann::json serialize()
	{
		nlohmann::json json;
		for (auto& [label, data] : m_FortData)
		{
			json[label] = data->serialize();
		}
		return json;
	}
	void deserialize(const nlohmann::json& json)
	{
		m_FortData = std::unordered_map<std::string, std::shared_ptr<eopFortData>>();
		for (auto it = json.begin(); it != json.end(); ++it)
		{
			const auto data = std::make_shared<eopFortData>();
			data->deserialize(it.value());
			m_FortData[it.key()] = data;
		}
	}
	std::string onGameSave();
	void onGameLoad(const std::vector<std::string>& filePaths);
	void onGameLoaded();

private:
	static eopFortDataDb* m_Instance;
	bool m_Initialized = false;
	std::unordered_map<std::string, std::shared_ptr<eopFortData>> m_FortData{};
};

namespace fortHelpers
{
	void addToLua(sol::state& luaState);
	void deleteFort(const factionStruct* fac, fortStruct* fort);
	fortStruct* createFortXY(factionStruct* fac, int x, int y);
	fortStruct* createFortXYCulture(factionStruct* fac, int x, int y, int cultureId);
	void changeOwner(fortStruct* fort, factionStruct* newFaction, bool convertGarrison);
};