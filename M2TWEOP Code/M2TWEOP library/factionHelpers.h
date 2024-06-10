#pragma once
#define factionStruct_ai_label 1
#define factionStruct_name 2
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include "lua/sol.hpp"
namespace factionHelpers
{
	std::string getFactionName(const factionStruct* fac);
	namedCharacter* getCharacterFromFullList(const factionStruct* fac, int index);
	general* getCharacterFromGeneralsList(const factionStruct* fac, int index);
	stackStruct* getStack(const factionStruct* fac, int index);
	settlementStruct* getSettlement(const factionStruct* fac, int index);
	fortStruct* getFort(const factionStruct* fac, int index);
	portBuildingStruct* getPort(const factionStruct* fac, int index);
	watchTowerStruct* getWatchtower(const factionStruct* fac, int index);
	bool hasMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2);
	float getFactionStanding(const factionStruct* fac1, const factionStruct* fac2);
	void setFactionStanding(const factionStruct* fac1, const factionStruct* fac2, float standing);
	void setMilitaryAccess(const factionStruct* fac1, const factionStruct* fac2, bool set);
	std::string getLocalizedFactionName(factionStruct* fac);
	const char* getRegionToHoldName(const holdRegionsWinCondition* condition, int index);
	int getRegionToHoldLength(const holdRegionsWinCondition* condition, int index);
	factionRanking* getFactionRanking(const factionStruct* fac, int turnNum);
	factionEconomy* getFactionEconomy(factionStruct* fac, int turnsAgo);
	bool isNeighbourFaction(const factionStruct* fac1, const factionStruct* fac2);
	watchTowerStruct* spawnWatchtower(const factionStruct* fac, int x, int y);
	int getNeighbourRegionID(const factionStruct* fac, int index);
	decisionValuesLTGD* getlongTermGoalValues(aiLongTermGoalDirector* LTGD, int targetFaction);
	void setConstructionValue(aiPersonalityValues* personality, int type, int value);
	void setRecruitmentValue(aiPersonalityValues* personality, int type, int value);
	int getConstructionValue(aiPersonalityValues* personality, int type);
	int getRecruitmentValue(aiPersonalityValues* personality, int type);
	int getRecruitmentValueSett(aiProductionController* controller, int type);
	int getConstructionValueSett(aiProductionController* controller, int type);
	void setRecruitmentValueSett(aiProductionController* controller, int type, int value);
	void setConstructionValueSett(aiProductionController* controller, int type, int value);
	aiProductionController* getProductionController(aiPersonalityValues* personality, int index);
	battleFactionCounter* getBattleVsFactionStats(factionStruct* fac, int targetFactionID);
	ltgdFactionValues* getAiFactionValues(factionStruct* fac);
	interFactionLTGD* getInterFactionLTGD(factionStruct* fac, factionStruct* targetFac);
	void setCharacterNameFaction(factionStruct* fac, int characterType, int factionID);
	stackStruct* splitArmy(factionStruct *faction, const sol::table& units, int x, int y);
	//faction
	template <char fieldIndex>
	std::string getStringProperty(const factionStruct* fac)
	{
		char* retS = nullptr;
		if (fieldIndex == factionStruct_ai_label)
		{
			retS = fac->ai_label;
		}
		if (fieldIndex == factionStruct_name)
		{
			retS = fac->factSmDescr->facName;
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
	void setStringProperty(factionStruct* fac, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == factionStruct_ai_label)
		{

			arg = reinterpret_cast<char*>(&fac->ai_label);
		}
		if (fieldIndex == factionStruct_name)
		{
			arg = reinterpret_cast<char*>(&fac->factSmDescr->facName);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}
};


