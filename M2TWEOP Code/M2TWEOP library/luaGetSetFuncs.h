#pragma once
#define generalStruct_abilityID 1


#define EduEntryStruct_Type 1
#define EduEntryStruct_Soldier 2
#define EduEntryStruct_UnitCardTga 3
#define EduEntryStruct_InfoCardTga 4

#define ancillaryStruct_name 1
#define ancillaryStruct_imagePath 2
#include <string>
#include "realGameTypes.h"
#include "character.h"
namespace luaGetSetFuncs
{
	int getTraitLevel(const traitContainer* currTrait);

	std::string getTraitName(const traitContainer* currTrait);
	traitContainer* getNextTrait(const traitContainer* currTrait);
	traitContainer* getPrevTrait(const traitContainer* currTrait);
	traitEffect* getTraitEffect(const traitLevel* level, int index);




	void setGameString(char* targetP, const char* newS);



#pragma region ancillary
	//ancillary
	template <char fieldIndex>
	std::string getStringPropertyAnc(const ancillary* anc)
	{
		char* retS = nullptr;
		if (fieldIndex == ancillaryStruct_name)
		{
			retS = anc->ancName;
		}
		else if (fieldIndex == ancillaryStruct_imagePath)
		{
			retS = anc->patchToAncImage;
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
	void setStringPropertyAnc(ancillary* anc, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == ancillaryStruct_name)
		{

			arg = reinterpret_cast<char*>(&anc->ancName);
		}
		else if (fieldIndex == ancillaryStruct_imagePath)
		{

			arg = reinterpret_cast<char*>(&anc->patchToAncImage);
		}
		setGameString(arg, newS.c_str());
	}
#pragma endregion


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
		char* arg = nullptr;
		if (fieldIndex == EduEntryStruct_Type)
		{

			arg = reinterpret_cast<char*>(&eduEn->Type);
		}
		else if (fieldIndex == EduEntryStruct_Soldier)
		{

			arg = reinterpret_cast<char*>(&eduEn->Soldier);
		}
		else if (fieldIndex == EduEntryStruct_UnitCardTga)
		{

			arg = reinterpret_cast<char*>(&eduEn->UnitCardTga);
		}
		else if (fieldIndex == EduEntryStruct_InfoCardTga)
		{

			arg = reinterpret_cast<char*>(&eduEn->InfoCardTga);
		}
		setGameString(arg, newS.c_str());
	}
#pragma endregion
};

