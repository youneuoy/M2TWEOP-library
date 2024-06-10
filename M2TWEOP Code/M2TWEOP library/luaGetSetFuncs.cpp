#include "luaGetSetFuncs.h"

#include "fastFunctsHelpers.h"
#include "plugData.h"
int luaGetSetFuncs::getTraitLevel(const traitContainer* currTrait)
{
	return currTrait->trait->level->level;
}
std::string luaGetSetFuncs::getTraitName(const traitContainer* currTrait)
{
	return currTrait->trait->traitEntry->name;
}
traitContainer* luaGetSetFuncs::getNextTrait(const traitContainer* currTrait)
{
	return currTrait->next;
}
traitContainer* luaGetSetFuncs::getPrevTrait(const traitContainer* currTrait)
{
	return currTrait->prev;
}
traitEffect* luaGetSetFuncs::getTraitEffect(const traitLevel* level, int index)
{
	return &level->effects[index];
}
void luaGetSetFuncs::setGameString(char* targetP, const char* newS)
{
	fastFunctsHelpers::setCryptedString(reinterpret_cast<char**>(targetP), newS);
}
