#pragma once
#include "realGameTypes.h"
namespace gameDataAllHelper
{
	gameDataAllStruct* get();

	character* getSelectedCharacter(selectionInfo* selection);
	character* getHoveredCharacter(selectionInfo* selection);
	character* getSelectedEnemyCharacter(selectionInfo* selection);
	
	settlementStruct* getSelectedSettlement(selectionInfo* selection);
	settlementStruct* getHoveredSettlement(selectionInfo* selection);
	settlementStruct* getSelectedEnemySettlement(selectionInfo* selection);
	
	fortStruct* getSelectedFort(selectionInfo* selection);
	fortStruct* getHoveredFort(selectionInfo* selection);
	fortStruct* getSelectedEnemyFort(selectionInfo* selection);
	
};

