#pragma once
#include "headersSTD.h"
#include "realGameTypes.h"
#include "fastFuncts.h"
#include "exportHeader.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
namespace actionsStrat
{
	DWORD getReadyIdleAction(DWORD allocatedMem);

	//Siege helpers
	DWORD getReadySiegeActionSett(DWORD allocatedMem, settlementStruct* sett);
	void getReadySiegeActionChar(DWORD allocatedMem, general* gen);
	NOINLINE EOP_EXPORT void sendOffMap(general* gen);
	NOINLINE EOP_EXPORT bool blockadePort(stackStruct* fleet, portBuildingStruct* port);

	//moving helpers
	DWORD getReadyForMoving(DWORD allocatedMem,general* gen,int x, int y);
	void getGeneralReadyForMoving(general* gen);
	NOINLINE EOP_EXPORT void switchCharacterFaction(general* gen, factionStruct* fac, bool keepArmy, bool keepBg);
	
	//helpers
	void finalizeAction(DWORD allocatedMem, general* gen);

	NOINLINE EOP_EXPORT void diplomacyFort(general* gen, fortStruct* targetFort);
	NOINLINE EOP_EXPORT void bribeFort(general* gen, fortStruct* targetFort);
	NOINLINE EOP_EXPORT void spyFort(general* gen, fortStruct* targetFort);
	NOINLINE EOP_EXPORT void bribeSettlement(general* gen, settlementStruct* targetSettlement);
	NOINLINE EOP_EXPORT void spySettlement(general* gen, settlementStruct* targetSettlement);
	NOINLINE EOP_EXPORT void sabotageSettlement(general* gen, settlementStruct* targetSettlement);
	
	NOINLINE EOP_EXPORT void Idle(general* gen);
	NOINLINE EOP_EXPORT void siegeSettlement(general* gen, settlementStruct* sett, bool isAttack);
	NOINLINE EOP_EXPORT void siegeFort(general* gen, fortStruct* fort, bool isAttack);
	NOINLINE EOP_EXPORT void moveNormal(general* gen, int x,int y);
	NOINLINE EOP_EXPORT void attackCharacter(general* attacker, general* defender);
	NOINLINE EOP_EXPORT void diplomacyCharacter(general* gen, general* targetCharacter);
	NOINLINE EOP_EXPORT void diplomacySettlement(general* gen, settlementStruct* targetSettlement);
	NOINLINE EOP_EXPORT void targetCharacterAction(general* gen, general* targetCharacter, characterAction type);
	NOINLINE EOP_EXPORT void targetFortAction(general* gen, fortStruct* targetFort, characterAction type);
	NOINLINE EOP_EXPORT void targetSettlementAction(general* gen, settlementStruct* targetSettlement, characterAction type);
	NOINLINE EOP_EXPORT void assassinate(general* gen, general* targetCharacter);
	NOINLINE EOP_EXPORT void marry(general* gen, general* targetCharacter);
	NOINLINE EOP_EXPORT void spyCharacter(general* gen, general* targetCharacter);
	NOINLINE EOP_EXPORT void denounce(general* gen, general* targetCharacter);
	NOINLINE EOP_EXPORT void bribe(general* gen, general* targetCharacter);
	NOINLINE EOP_EXPORT void acquire(general* gen, general* targetCharacter);
};