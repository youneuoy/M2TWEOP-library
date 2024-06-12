#include "generalCharactericticsHelpers.h"

#include "FastFuncts.h"
#include "plugData.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(characterRecord* character)
	{
		return character->traits;
	}
	ancillary* getAncillary(characterRecord* character, int index)
	{
		return character->ancillaries[index]->dataAnc;

	}
	int addAncillary(characterRecord* character, const std::string& ancName)
	{
		ancillary* anc = fastFuncts::findAncillary(ancName.c_str());
		if (anc == nullptr)return 0;

		return fastFuncts::addAncillary(character, anc);
	}
	int getAge(characterRecord* character)
	{
		return (character->age >> 3) & 0x7f;
	}
	void setAge(characterRecord* character, int age)
	{
		character->age = character->age ^ (age * 8 ^ character->age) & 0x3f8;
	}
	bool isAlive(characterRecord* character)
	{
		return character->age & 1;
	}
	bool isFamily(characterRecord* character)
	{
		return character->age & (1 << 13);
	}
	bool isOffMap(characterRecord* character)
	{
		return character->status & 8;
	}
	bool isChild(characterRecord* character)
	{
		return character->age & 4;
	}
	bool setAsFamily(characterRecord* character, bool set)
	{
		return character->age = set ? character->age | (1 << 13) : character->age & ~(1 << 13);
	}
	bool getIsMale(characterRecord* character)
	{
		return character->age & 2;
	}
	void setIsMale(characterRecord* character, bool isMale)
	{
		character->age = character->age ^ ((int)isMale << 1 ^ character->age) & 2;
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
