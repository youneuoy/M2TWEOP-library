#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(characterRecord* character);
	ancillary* getAncillary(characterRecord* character, int index);
	int addAncillary(characterRecord* character, const std::string& ancName);


	int getAge(characterRecord* character);
	void setAge(characterRecord* character, int age);
	bool isFamily(characterRecord* character);
	bool setAsFamily(characterRecord* character, bool set);
	bool isOffMap(characterRecord* character);
	bool isChild(characterRecord* character);

	bool getIsMale(characterRecord* character);
	void setIsMale(characterRecord* character, bool isMale);

	bool isAlive(characterRecord* character);
	int getCapturedUnitNum(capturedFactionInfo* capInfo);
	int getCapturedCharacterNum(capturedFactionInfo* capInfo);
	capturedUnit* getCapturedUnit(const capturedFactionInfo* capInfo, const int index);
	capturedCharacter* getCapturedCharacter(const capturedFactionInfo* capInfo, const int index);
};

