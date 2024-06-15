#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
#define resource_dataStruct_type 1
namespace resourcesHelpers
{
	void setModel(const resStrat* resource, UINT32 modelId);
	int getResourceCode(const resStrat* resource);
	int getResourceCost(const resStrat* resource);
	int getResourceHasMine(const resStrat* resource);
	std::string getImage(const resStrat* resource);

#pragma region stratResMod
	//stratResMod
	template <char fieldIndex>
	std::string getStringPropertyBD(const stratResMod* stratMod)
	{
		char* retS = nullptr;
		if (fieldIndex == resource_dataStruct_type)
		{
			retS = stratMod->tga;
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
	void setStringPropertyBD(stratResMod* stratMod, std::string newS)
	{
		if (fieldIndex == resource_dataStruct_type)
			fastFunctsHelpers::setCryptedString(&stratMod->tga, newS.c_str());
	}
#pragma endregion
};