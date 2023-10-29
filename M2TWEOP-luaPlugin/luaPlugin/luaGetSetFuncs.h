#pragma once
#define generalStruct_abilityID 1


#define generalCharactericticsStruct_shortName 1
#define generalCharactericticsStruct_fullName 2
#define generalCharactericticsStruct_label 3
#define generalCharactericticsStruct_modelName 4
#define generalCharactericticsStruct_portrait 5
#define generalCharactericticsStruct_portrait2 6
#define generalCharactericticsStruct_portrait_custom 7

#define EduEntryStruct_Type 1
#define EduEntryStruct_Soldier 2
#define EduEntryStruct_UnitCardTga 3
#define EduEntryStruct_InfoCardTga 4


#define ancillaryStruct_name 1
#define ancillaryStruct_imagePath 2
#include <string>
#include "realGameTypes.h"
namespace luaGetSetFuncs
{
	int getTraitLevel(const traitContainer* currTrait);

	std::string getTraitName(const traitContainer* currTrait);
	traitContainer* getNextTrait(const traitContainer* currTrait);
	traitContainer* getPrevTrait(const traitContainer* currTrait);




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


#pragma region general


	//general
	template <char fieldIndex>
	std::string getStringPropertyGen(const general* gen)
	{
		char* retS = nullptr;
		if (fieldIndex == generalStruct_abilityID)
		{
			retS = gen->ability;
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
	void setStringPropertyGen(general* gen, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == generalStruct_abilityID)
		{

			arg = reinterpret_cast<char*>(&gen->ability);
		}
		setGameString(arg, newS.c_str());
	}
#pragma endregion

#pragma region namedCharacter


	//namedCharacter
	template <char fieldIndex>
	std::string getStringPropertyGenChar(const namedCharacter* genChar)
	{
		char* retS = nullptr;
		if (fieldIndex == generalCharactericticsStruct_shortName)
		{
			retS = genChar->shortName;
		}
		else if (fieldIndex == generalCharactericticsStruct_fullName)
		{
			retS = genChar->fullName;
		}
		else if (fieldIndex == generalCharactericticsStruct_label)
		{
			retS = genChar->label;
		}
		else if (fieldIndex == generalCharactericticsStruct_modelName)
		{
			retS = genChar->modelName;
		}
		else if (fieldIndex == generalCharactericticsStruct_portrait)
		{
			retS = genChar->portrait;
		}
		else if (fieldIndex == generalCharactericticsStruct_portrait2)
		{
			retS = genChar->portrait2;
		}
		else if (fieldIndex == generalCharactericticsStruct_portrait_custom)
		{
			retS = genChar->portrait_custom;
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
	void setStringPropertyGenChar(namedCharacter* genChar, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == generalCharactericticsStruct_shortName)
		{

			arg = reinterpret_cast<char*>(&genChar->shortName);
		}
		else if (fieldIndex == generalCharactericticsStruct_fullName)
		{

			arg = reinterpret_cast<char*>(&genChar->fullName);
		}
		else if (fieldIndex == generalCharactericticsStruct_label)
		{

			arg = reinterpret_cast<char*>(&genChar->label);
		}
		else if (fieldIndex == generalCharactericticsStruct_modelName)
		{

			arg = reinterpret_cast<char*>(&genChar->modelName);
		}
		else if (fieldIndex == generalCharactericticsStruct_portrait)
		{

			arg = reinterpret_cast<char*>(&genChar->portrait);
		}
		else if (fieldIndex == generalCharactericticsStruct_portrait2)
		{

			arg = reinterpret_cast<char*>(&genChar->portrait2);
		}
		else if (fieldIndex == generalCharactericticsStruct_portrait_custom)
		{

			arg = reinterpret_cast<char*>(&genChar->portrait_custom);
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

