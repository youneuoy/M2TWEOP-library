#pragma once
#define factionStruct_ai_label 1
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include "lua/sol.hpp"
namespace factionHelpers
{
	std::string getFactionName(const factionStruct* fac);
	generalCharacterictics* getCharacterFromFullList(const factionStruct* fac, int index);
	general* getCharacterFromGeneralsList(const factionStruct* fac, int index);
	stackStruct* getStack(const factionStruct* fac, int index);
	settlementStruct* getSettlement(const factionStruct* fac, int index);
	fortStruct* getFort(const factionStruct* fac, int index);
	portBuildingStruct* getPort(const factionStruct* fac, int index);
	watchTowerStruct* getWatchtower(const factionStruct* fac, int index);
	void deleteFort(const factionStruct* fac, fortStruct* fort);
	void createFortXY(const factionStruct* fac, int x, int y);
	void createFort(const general* gen);
	void changeFactionName(factionStruct* fac, const char* newName);

	//faction
	template <char fieldIndex>
	std::string getStringProperty(const factionStruct* fac)
	{
		char* retS = nullptr;
		if (fieldIndex == factionStruct_ai_label)
		{
			retS = fac->ai_label;
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
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}
};


