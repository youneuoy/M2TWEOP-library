#include "generalCharactericticsHelpers.h"
#include "plugData.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(namedCharacter* character)
	{
		return character->traits;
	}
	anchillary* getAnchillary(namedCharacter* character, int index)
	{
		return character->anchillaries[index]->dataAnch;

	}
	int addAnchillary(namedCharacter* character, std::string anchName)
	{
		anchillary* anch = (*(*plugData::data.funcs.findAnchillary))((char*)anchName.c_str());
		if (anch == nullptr)return 0;

		return (*(*plugData::data.funcs.addAnchillary))(character, anch);
	}
	void removeAnchillary(namedCharacter* character, anchillary* anch)
	{
		(*(*plugData::data.funcs.removeAnchillary))(character, anch);
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
	bool getIsMale(namedCharacter* character) {
		return character->age & 2;
	}
	void setIsMale(namedCharacter* character, bool isMale) {
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
