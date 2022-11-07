#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalCharactericticsHelpers
{
	traitContainer* getTraits(generalCharacterictics* character);
	anchillary* getAnchillary(generalCharacterictics* character, int index);
	int addAnchillary(generalCharacterictics* character, std::string anchName);

	void removeAnchillary(generalCharacterictics* character, anchillary* anch);


	void addTrait(generalCharacterictics* character, const char* traitName,int traitLevel);
	void removeTrait(generalCharacterictics* character, const char* traitName);


	int getAge(generalCharacterictics* character);
	void setAge(generalCharacterictics* character,int age);

	bool getIsMale(generalCharacterictics* character);
	void setIsMale(generalCharacterictics* character, bool isMale);

	bool isAlive(generalCharacterictics* character);
	void setAsHeir(generalCharacterictics* character,bool isJustSet);
};

