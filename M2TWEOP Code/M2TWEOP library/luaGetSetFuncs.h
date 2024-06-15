#pragma once

#define EduEntryStruct_Type 1
#define EduEntryStruct_Soldier 2
#define EduEntryStruct_UnitCardTga 3
#define EduEntryStruct_InfoCardTga 4

#include <string>

#include "fastFunctsHelpers.h"
#include "realGameTypes.h"
namespace luaGetSetFuncs
{
#pragma region EduEntry
	//general
	template <char fieldIndex>
	std::string getStringPropertyEDU(const eduEntry* eduEn)
	{
		char* retS = nullptr;
		if (fieldIndex == EduEntryStruct_Type)
		{
			retS = eduEn->Type;
		}
		else if (fieldIndex == EduEntryStruct_Soldier)
		{
			retS = eduEn->Soldier;
		}
		else if (fieldIndex == EduEntryStruct_UnitCardTga)
		{
			retS = eduEn->UnitCardTga;
		}
		else if (fieldIndex == EduEntryStruct_InfoCardTga)
		{
			retS = eduEn->InfoCardTga;
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
	void setStringPropertyEDU(eduEntry* eduEn, std::string newS)
	{
		if (fieldIndex == EduEntryStruct_Type)
			fastFunctsHelpers::setCryptedString(&eduEn->Type, newS.c_str());
		else if (fieldIndex == EduEntryStruct_Soldier)
			fastFunctsHelpers::setCryptedString(&eduEn->Soldier, newS.c_str());
		else if (fieldIndex == EduEntryStruct_UnitCardTga)
			fastFunctsHelpers::setCryptedString(&eduEn->UnitCardTga, newS.c_str());
		else if (fieldIndex == EduEntryStruct_InfoCardTga)
			fastFunctsHelpers::setCryptedString(&eduEn->InfoCardTga, newS.c_str());
	}
#pragma endregion
};

