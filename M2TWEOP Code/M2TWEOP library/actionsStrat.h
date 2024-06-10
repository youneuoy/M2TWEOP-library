#pragma once
#include "realGameTypes.h"

namespace actionsStrat
{
	DWORD getReadyIdleAction(DWORD allocatedMem);

	//Siege helpers
	DWORD getReadySiegeActionSett(DWORD allocatedMem, settlementStruct* sett);
	void getReadySiegeActionChar(DWORD allocatedMem, general* gen);
	void sendOffMap(general* gen);
	bool blockadePort(stackStruct* fleet, portBuildingStruct* port);

	//moving helpers
	DWORD getReadyForMoving(DWORD allocatedMem,general* gen,int x, int y);
	void getGeneralReadyForMoving(general* gen);
	void switchCharacterFaction(general* gen, factionStruct* fac, bool keepArmy, bool keepBg);
	
	//helpers
	void finalizeAction(DWORD allocatedMem, general* gen);

	void diplomacyFort(general* gen, fortStruct* targetFort);
	void bribeFort(general* gen, fortStruct* targetFort);
	void spyFort(general* gen, fortStruct* targetFort);
	void bribeSettlement(general* gen, settlementStruct* targetSettlement);
	void spySettlement(general* gen, settlementStruct* targetSettlement);
	void sabotageSettlement(general* gen, settlementStruct* targetSettlement);
	void buildWatchTower(stackStruct* army);
	
	void idle(general* gen);
	void siegeSettlement(general* gen, settlementStruct* sett, bool isAttack);
	void siegeFort(general* gen, fortStruct* fort, bool isAttack);
	void moveNormal(general* gen, int x,int y);
	void attackCharacter(general* attacker, general* defender);
	void diplomacyCharacter(general* gen, general* targetCharacter);
	void diplomacySettlement(general* gen, settlementStruct* targetSettlement);
	void targetCharacterAction(general* gen, general* targetCharacter, characterAction type);
	void targetFortAction(general* gen, fortStruct* targetFort, characterAction type);
	void targetSettlementAction(general* gen, settlementStruct* targetSettlement, characterAction type);
	void assassinate(general* gen, general* targetCharacter);
	void marry(general* gen, general* targetCharacter);
	void spyCharacter(general* gen, general* targetCharacter);
	void denounce(general* gen, general* targetCharacter);
	void bribe(general* gen, general* targetCharacter);
	void acquire(general* gen, general* targetCharacter);
};