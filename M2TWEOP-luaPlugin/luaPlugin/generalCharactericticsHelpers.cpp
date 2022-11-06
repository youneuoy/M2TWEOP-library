#include "generalCharactericticsHelpers.h"
#include "plugData.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(generalCharacterictics* character)
	{
		return character->traits;
	}
	anchillary* getAnchillary(generalCharacterictics* character, int index)
	{
		return character->anchillaries[index]->dataAnch;

	}
	int addAnchillary(generalCharacterictics* character, std::string anchName)
	{
		anchillary* anch=(*(*plugData::data.funcs.findAnchillary))((char*)anchName.c_str());
		if (anch == nullptr)return 0;

		return (*(*plugData::data.funcs.addAnchillary))(character, anch);
	}
	void removeAnchillary(generalCharacterictics* character, anchillary* anch)
	{
		(*(*plugData::data.funcs.removeAnchillary))(character, anch);
	}
	void addTrait(generalCharacterictics* character, const char* traitName, int traitLevel)
	{
		(*(*plugData::data.funcs.addTrait))(character, traitName, traitLevel);
	}
	void removeTrait(generalCharacterictics* character, const char* traitName)
	{
		(*(*plugData::data.funcs.removeTrait))(character, traitName);
	}
	int getAge(generalCharacterictics* character)
	{
		return (character->age >> 3) & 0x7f;
	}
	void setAge(generalCharacterictics* character, int age)
	{
		character->age = character->age ^ (age * 8 ^ character->age) & 0x3f8;
	}
	bool isAlive(generalCharacterictics* character)
	{
		return character->age&1;
	}
	bool getIsMale(generalCharacterictics* character) {
		return character->age & 2;
	}
	void setIsMale(generalCharacterictics* character, bool isMale) {
		character->age = character->age ^ ((int) isMale << 1 ^ character->age) & 2;
	}
	void setAsHeir(generalCharacterictics* character, bool isJustSet)
	{
		(*(*plugData::data.funcs.setHeir))(character, isJustSet);
	}
}
