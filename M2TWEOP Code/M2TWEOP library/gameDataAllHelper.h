#pragma once
#include "realGameTypes.h"
namespace gameDataAllHelper
{
	gameDataAllStruct* get();

	general* getSelectedCharacter(selectionInfo* selection);
	general* getHoveredCharacter(selectionInfo* selection);
	general* getSelectedEnemyCharacter(selectionInfo* selection);
	
	settlementStruct* getSelectedSettlement(selectionInfo* selection);
	settlementStruct* getHoveredSettlement(selectionInfo* selection);
	settlementStruct* getSelectedEnemySettlement(selectionInfo* selection);
	
	fortStruct* getSelectedFort(selectionInfo* selection);
	fortStruct* getHoveredFort(selectionInfo* selection);
	fortStruct* getSelectedEnemyFort(selectionInfo* selection);
	
};

