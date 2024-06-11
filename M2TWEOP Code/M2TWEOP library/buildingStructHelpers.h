#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include "lua/sol.hpp"

#define edbEntryStruct_type 1
#define buildingLevelStruct_name 1

namespace buildingStructHelpers
{
	std::string getType(building* build);
	std::string getName(building* build);
	std::string getQueueType(buildingInQueue* build);
	std::string getQueueName(buildingInQueue* build);
	buildingInQueue* getBuildingInQueue(buildingsQueue* queue, int position);
	void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp, std::string condition);

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

