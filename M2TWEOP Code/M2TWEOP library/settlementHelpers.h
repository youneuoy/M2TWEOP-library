#pragma once
#define settlementStruct_name 1
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace settlementHelpers
{

	siegeS* getSiege(const settlementStruct* sett, int index);
	void changeOwner(settlementStruct* sett, factionStruct* newOwner);
	void setBuildingHealth(building* building, int health);
	bool addBuildingToQueue(buildingInQueue* building);
	bool addUnitToQueue(unitRQ* unit);
	recruitmentOptions* getAvailableRetrainingUnits(settlementStruct* sett);
	float getReligion(const settlementStruct* sett, int index);
	void setReligion(const settlementStruct* sett, int index, float value);
	exportDescrBuildings* getEdb();
	void upgradeSettlement(settlementStruct* sett);
	int getAvailableBuildingsCount(const availableBuildings* list);
	buildingInQueue* getBuildingOption(const availableBuildings* list, const int index);
	availableBuildings* getAvailableBuildingsMem();
	availableBuildings* getAvailableBuildings(settlementStruct* sett);
	int makeBuildOptionsHash(const settlementStruct* sett);
	settlementBuildingOptions* getBuildingOptions(settlementStruct* sett);
	buildingInQueue* getBuildingOptionFromDb(const settlementBuildingOptions* list, const int index);
	int getBuildingHealth(building* build);
	settlementRecruitmentOptions* getRecruitOptions(settlementStruct* sett);
	unitRQ* getRecruitOptionFromDb(const settlementRecruitmentOptions* list, const int index);
	int makeRecruitOptionsHash(const settlementStruct* sett);
	recruitmentOptions* getAvailableUnitsMem();
	recruitmentOptions* getAvailableUnits(settlementStruct* sett);
	eduEntry* getUnitEntry(const unitRQ* unitOption);
	void setUnitEntry(unitRQ* unitOption, eduEntry* entry);
	void setAgentType(unitRQ* unitOption, int type);
	int getAgentType(const unitRQ* unitOption);
	
	resStrat* getResource(const settlementStruct* sett, int index);
	building* getBuilding(const settlementStruct* sett, int index);
	int getGuildStanding(settlementStruct* sett, int index);
	void setGuildStanding(settlementStruct* sett, int index, int amount);
	settlementCapability* getSettlementCapability(settlementStruct* sett, int capabilityType);
	settlementCapability* getAgentCapability(settlementStruct* sett, int agentType);
	settlementCapability* getAgentLimitCapability(settlementStruct* sett, int agentType);
	recruitmentCapability* getRecruitmentCapability(settlementStruct* sett, int index);
	settlementRecruitmentPool* getSettlementRecruitmentPool(settlementStruct* sett, int index);
	int getRecruitmentCapabilityNum(settlementStruct* sett);
	int getPopulation(settlementStruct* sett);

	std::string getSettlementName(settlementStruct* sett);


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
		char* arg = nullptr;
		if (fieldIndex == settlementStruct_name)
		{

			arg = reinterpret_cast<char*>(&sett->name);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}
};

