#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(namedCharacter* character);
	ancillary* getAncillary(namedCharacter* character, int index);
	int addAncillary(namedCharacter* character, const std::string& ancName);


	int getAge(namedCharacter* character);
	void setAge(namedCharacter* character, int age);
	bool isFamily(namedCharacter* character);
	bool setAsFamily(namedCharacter* character, bool set);
	bool isOffMap(namedCharacter* character);
	bool isChild(namedCharacter* character);

	bool getIsMale(namedCharacter* character);
	void setIsMale(namedCharacter* character, bool isMale);

	bool isAlive(namedCharacter* character);
	int getCapturedUnitNum(capturedFactionInfo* capInfo);
	int getCapturedCharacterNum(capturedFactionInfo* capInfo);
	capturedUnit* getCapturedUnit(const capturedFactionInfo* capInfo, const int index);
	capturedCharacter* getCapturedCharacter(const capturedFactionInfo* capInfo, const int index);
};

