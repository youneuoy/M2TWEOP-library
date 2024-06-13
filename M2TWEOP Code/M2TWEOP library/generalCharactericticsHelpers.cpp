#include "generalCharactericticsHelpers.h"

#include "FastFuncts.h"
#include "plugData.h"
namespace generalCharactericticsHelpers
{
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
