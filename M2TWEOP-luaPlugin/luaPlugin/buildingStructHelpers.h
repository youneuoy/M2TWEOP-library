#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#include "lua/sol.hpp"

#define building_dataStruct_type 1
#define buildingDrawInfoStruct_name 1

namespace buildingStructHelpers
{
	std::string getType(building* build);
	std::string getName(building* build);
	std::string getQueueType(buildingInQueue* build);
	std::string getQueueName(buildingInQueue* build);
	buildingInQueue* getBuildingInQueue(buildingsQueue* queue, int position);

	std::string getQueueType(buildingInQueue* build);
	std::string getQueueName(buildingInQueue* build);

#pragma region buildingDrawInfo

	//building_data
	template <char fieldIndex>
	std::string getStringPropertyBDI(const buildingDrawInfo* bInfo)
	{
		char* retS = nullptr;
		if (fieldIndex == buildingDrawInfoStruct_name)
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
	void setStringPropertyBDI(buildingDrawInfo* bInfo, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == buildingDrawInfoStruct_name)
		{

			arg = reinterpret_cast<char*>(&bInfo->name);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}

#pragma endregion
#pragma region building_data

	//building_data
	template <char fieldIndex>
	std::string getStringPropertyBD(const building_data* bData)
	{
		char* retS = nullptr;
		if (fieldIndex == building_dataStruct_type)
		{
			retS = bData->type;
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
	void setStringPropertyBD(building_data* bData, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == building_dataStruct_type)
		{

			arg = reinterpret_cast<char*>(&bData->type);
		}
		luaGetSetFuncs::setGameString(arg, newS.c_str());
	}

#pragma endregion

};

