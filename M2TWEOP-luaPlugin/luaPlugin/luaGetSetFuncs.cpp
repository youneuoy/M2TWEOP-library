#include "luaGetSetFuncs.h"
#include "plugData.h"
int luaGetSetFuncs::getTraitLevel(const traitContainer* currTrait)
{
	return *currTrait->trait->level;
}
std::string luaGetSetFuncs::getTraitName(const traitContainer* currTrait)
{
	return currTrait->trait->nameS->name;
}
traitContainer* luaGetSetFuncs::getNextTrait(const traitContainer* currTrait)
{
	return currTrait->next;
}
traitContainer* luaGetSetFuncs::getPrevTrait(const traitContainer* currTrait)
{
	return currTrait->prev;
}
void luaGetSetFuncs::setGameString(char* targetP, const char* newS)
{
	(*(*plugData::data.funcs.setCryptedString))(targetP, newS);

}
