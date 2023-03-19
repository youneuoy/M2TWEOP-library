#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(namedCharacter* character);
	anchillary* getAnchillary(namedCharacter* character, int index);
	int addAnchillary(namedCharacter* character, std::string anchName);

	void removeAnchillary(namedCharacter* character, anchillary* anch);


	void addTrait(namedCharacter* character, const char* traitName, int traitLevel);
	void removeTrait(namedCharacter* character, const char* traitName);


	int getAge(namedCharacter* character);
	void setAge(namedCharacter* character, int age);

	bool getIsMale(namedCharacter* character);
	void setIsMale(namedCharacter* character, bool isMale);

	bool isAlive(namedCharacter* character);
	void setAsHeir(namedCharacter* character, bool isJustSet);
	int getCapturedUnitNum(capturedFactionInfo* capInfo);
	int getCapturedCharacterNum(capturedFactionInfo* capInfo);
	capturedUnit* getCapturedUnit(const capturedFactionInfo* capInfo, const int index);
	capturedCharacter* getCapturedCharacter(const capturedFactionInfo* capInfo, const int index);
};

