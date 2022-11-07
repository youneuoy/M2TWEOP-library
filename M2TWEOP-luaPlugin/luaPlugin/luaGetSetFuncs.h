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


#define anchillaryStruct_name 1
#define anchillaryStruct_imagePath 2
#include <string>
#include "realGameTypes.h"
namespace luaGetSetFuncs
{
	int getTraitLevel(const traitContainer*currTrait);

	std::string getTraitName(const traitContainer* currTrait);
	traitContainer* getNextTrait(const traitContainer* currTrait);
	traitContainer* getPrevTrait(const traitContainer* currTrait);




	void setGameString(char* targetP, const char* newS);



#pragma region anchillary
	//anchillary
	template <char fieldIndex>
	std::string getStringPropertyAnc(const anchillary* anch)
	{
		char* retS = nullptr;
		if (fieldIndex == anchillaryStruct_name)
		{
			retS = anch->anchName;
		}
		else if (fieldIndex == anchillaryStruct_imagePath)
		{
			retS = anch->patchToAnchImage;
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
	void setStringPropertyAnc(anchillary* anch, std::string newS)
	{
		char* arg = nullptr;
		if (fieldIndex == anchillaryStruct_name)
		{

			arg = reinterpret_cast<char*>(&anch->anchName);
		}
		else if (fieldIndex == anchillaryStruct_imagePath)
		{

			arg = reinterpret_cast<char*>(&anch->patchToAnchImage);
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

#pragma region generalCharacterictics


	//generalCharacterictics
	template <char fieldIndex>
	std::string getStringPropertyGenChar(const generalCharacterictics* genChar)
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
	void setStringPropertyGenChar(generalCharacterictics* genChar, std::string newS)
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
	std::string getStringPropertyEDU(const EduEntry* eduEn)
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
	void setStringPropertyEDU(EduEntry* eduEn, std::string newS)
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
		setGameString(arg, newS.c_str());
	}
#pragma endregion
};

