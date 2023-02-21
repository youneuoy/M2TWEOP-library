#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include "lua/sol.hpp"

#define edbEntryStruct_type 1
#define buildingLevelStruct_name 1

namespace buildingStructHelpers
{
    //enum buildingCap:int
    //{
    //    population_growth_bonus = 0,
    //    population_loyalty_bonus = 1,
    //    population_health_bonus = 2,
    //    trade_base_income_bonus = 3,
    //    trade_level_bonus = 4,
    //    trade_fleet = 5,
    //    taxable_income_bonus = 6,
    //    mine_resource = 7,
    //    farming_level = 8,
    //    road_level = 9,
    //    gate_strength = 10,
    //    gate_defences = 11,
    //    wall_level = 12,
    //    tower_level = 13,
    //    armour = 14,
    //    stage_games = 15,
    //    stage_races = 16,
    //    fire_risk = 17,
    //    weapon_melee_simple = 18,
    //    weapon_melee_blade = 19,
    //    weapon_missile_mechanical = 20,
    //    weapon_missile_gunpowder = 21,
    //    weapon_artillery_mechanical = 22,
    //    weapon_artillery_gunpowder = 23,
    //    weapon_naval_gunpowder = 24,
    //    upgrade_bodyguard = 25,
    //    recruits_morale_bonus = 26,
    //    recruits_exp_bonus = 27,
    //    happiness_bonus = 28,
    //    law_bonus = 29,
    //    construction_cost_bonus_military = 30,
    //    construction_cost_bonus_religious = 31,
    //    construction_cost_bonus_defensive = 32,
    //    construction_cost_bonus_other = 33,
    //    construction_time_bonus_military = 34,
    //    construction_time_bonus_religious = 35,
    //    construction_time_bonus_defensive = 36,
    //    construction_time_bonus_other = 37,
    //    construction_cost_bonus_wooden = 38,
    //    construction_cost_bonus_stone = 39,
    //    construction_time_bonus_wooden = 40,
    //    construction_time_bonus_stone = 41,
    //    free_upkeep = 42,
    //    pope_approval = 43,
    //    pope_disapproval = 44,
    //    religion_level = 45,
    //    amplify_religion_level = 46,
    //    archer_bonus = 47,
    //    cavalry_bonus = 48,
    //    heavy_cavalry_bonus = 49,
    //    gun_bonus = 50,
    //    navy_bonus = 51,
    //    recruitment_cost_bonus_naval = 52,
    //    retrain_cost_bonus = 53,
    //    weapon_projectile = 54,
    //    income_bonus = 55,
    //    recruitment_slots = 56
    //};
	std::string getType(building* build);
	std::string getName(building* build);
	std::string getQueueType(buildingInQueue* build);
	std::string getQueueName(buildingInQueue* build);
	buildingInQueue* getBuildingInQueue(buildingsQueue* queue, int position);
	void addCapability(building* building, int capability, int16_t value, bool bonus);
	edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex);
	edbEntry* getEopBuildEntry(int idx);
	void setBuildingPic(edbEntry* entry, const char* newPic, int level, int cultureID);
	void setBuildingPicConstructed(edbEntry* entry, const char* newPic, int level, int cultureID);
	void setBuildingPicConstruction(edbEntry* entry, const char* newPic, int level, int cultureID);
	void setBuildingLocalizedName(edbEntry* entry, const char* newName, int level, int facnum);
	void setBuildingLocalizedDescr(edbEntry* entry, const char* newName, int level, int facnum);
	void setBuildingLocalizedDescrShort(edbEntry* entry, const char* newName, int level, int facnum);
	void addBuildingCapability(edbEntry* entry, int level, int capability, int16_t value, bool bonus);
	void removeBuildingCapability(edbEntry* entry, int level, int index);
	BuildingLvlCapability* getBuildingCapability(edbEntry* entry, int level, int index);
	int getBuildingCapabilityNum(edbEntry* entry, int level);
	void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp);
	void removeBuildingPool(edbEntry* entry, int level, int index);
	recruitPool* getBuildingPool(edbEntry* entry, int level, int index);
	int getBuildingPoolNum(edbEntry* entry, int level);
	void createEOPBuilding(settlementStruct* sett, int edbIdx, int level);
	edbEntry* getBuildingByName(const char* name);

	std::string getQueueType(buildingInQueue* build);
	std::string getQueueName(buildingInQueue* build);

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
		char* arg = nullptr;
		if (fieldIndex == buildingLevelStruct_name)
		{

			arg = reinterpret_cast<char*>(&bInfo->name);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
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
		char* arg = nullptr;
		if (fieldIndex == edbEntryStruct_type)
		{

			arg = reinterpret_cast<char*>(&edbEntry->type);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}

#pragma endregion

};

