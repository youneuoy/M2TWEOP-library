#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include <vector>

#include "lua/sol.hpp"
namespace gameHelpers
{
	UINT32 getFactionsCount();
	std::string callConsole(std::string cmdName, sol::variadic_args va);
	factionStruct* getFaction(int index);
	guild* getGuild(unsigned char index);


	general* createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y);
	stackStruct* createArmy(general* character);
	stackStruct* createArmyInSettlement(settlementStruct* sett);

	std::tuple<bool, int> getScriptCounter(const char* type);
	void setScriptCounter(const char* type, int value);
	regionStruct* getRegion(int index);
	oneTile* getTile(int x, int y);
	stackStruct* getStack(const regionStruct* region, int index);
	fortStruct* getFort(const regionStruct* region, int index);
	watchTowerStruct* getWatchtower(const regionStruct* region, int index);
	resStrat* getResource(const regionStruct* region, int index);
	regionStruct* getNeighbour(regionStruct* region, int index);
	bool getHiddenResource(regionStruct* region, int index);
	void setHiddenResource(regionStruct* region, int index, bool enable);
	void changeRegionName(regionStruct* region, const char* newName);
	std::string getRegionName(regionStruct* region);
	void changeRebelsName(regionStruct* region, const char* newName);

	std::string getRebelsName(regionStruct* region);

	int getMercUnitNum(mercPool* mercPool);
	mercPoolUnit* getMercUnit(mercPool* pool, int index);
	void setMercReligion(mercPoolUnit* unit, int religion, bool set);
	mercPoolUnit* addMercUnit(mercPool* mercPool, int idx, int exp, int cost, float repmin, float repmax, int maxunits, float startpool, float startyear, float endyear, int crusading);
	void saveGame(const char* path);
	void historicEvent(const char* name, const char* title, const char* description);
	void scriptCommand(const char* command, const char* args);
};

