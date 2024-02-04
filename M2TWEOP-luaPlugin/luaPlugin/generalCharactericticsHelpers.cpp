#include "generalCharactericticsHelpers.h"
#include "plugData.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(namedCharacter* character)
	{
		return character->traits;
	}
	ancillary* getAncillary(namedCharacter* character, int index)
	{
		return character->ancillaries[index]->dataAnc;

	}
	int addAncillary(namedCharacter* character, std::string ancName)
	{
		ancillary* anc = (*(*plugData::data.funcs.findAncillary))((char*)ancName.c_str());
		if (anc == nullptr)return 0;

		return (*(*plugData::data.funcs.addAncillary))(character, anc);
	}
	void removeAncillary(namedCharacter* character, ancillary* anc)
	{
		(*(*plugData::data.funcs.removeAncillary))(character, anc);
	}
	void addTrait(namedCharacter* character, const char* traitName, int traitLevel)
	{
		(*(*plugData::data.funcs.addTrait))(character, traitName, traitLevel);
	}
	void removeTrait(namedCharacter* character, const char* traitName)
	{
		(*(*plugData::data.funcs.removeTrait))(character, traitName);
	}
	int getAge(namedCharacter* character)
	{
		return (character->age >> 3) & 0x7f;
	}
	void setAge(namedCharacter* character, int age)
	{
		character->age = character->age ^ (age * 8 ^ character->age) & 0x3f8;
	}
	bool isAlive(namedCharacter* character)
	{
		return character->age & 1;
	}
	bool isFamily(namedCharacter* character)
	{
		return character->age & (1 << 13);
	}
	bool isOffMap(namedCharacter* character)
	{
		return character->status & 8;
	}
	bool isAdult(namedCharacter* character)
	{
		return character->age & 4;
	}
	bool setAsFamily(namedCharacter* character, bool set)
	{
		return character->age = set ? character->age | (1 << 13) : character->age & ~(1 << 13);
	}
	bool getIsMale(namedCharacter* character)
	{
		return character->age & 2;
	}
	void setIsMale(namedCharacter* character, bool isMale)
	{
		character->age = character->age ^ ((int)isMale << 1 ^ character->age) & 2;
	}
	void setAsHeir(namedCharacter* character, bool isJustSet)
	{
		(*(*plugData::data.funcs.setHeir))(character, isJustSet);
	}
	int getCapturedUnitNum(capturedFactionInfo* capInfo)
	{
		return (capInfo->capturedUnitsEnd - (DWORD)capInfo->capturedUnits) / 12;
	}
	int getCapturedCharacterNum(capturedFactionInfo* capInfo)
	{
		return (capInfo->capturedCharactersEnd - (DWORD)capInfo->capturedCharacters) / 8;
	}
	capturedUnit* getCapturedUnit(const capturedFactionInfo* capInfo, const int index)
	{
		return &capInfo->capturedUnits[index];
	}
	capturedCharacter* getCapturedCharacter(const capturedFactionInfo* capInfo, const int index)
	{
		return &capInfo->capturedCharacters[index];
	}
}
