#include "gameDataAllHelper.h"

#include "plugData.h"
namespace gameDataAllHelper
{
	gameDataAllStruct* get()
	{
			return (*(*plugData::data.funcs.getGameDataAll))();
	}

	general* getSelectedCharacter(selectionInfo* selection)
	{
		if (!selection || !selection->selectedCharacter)
			return nullptr;
		return selection->selectedCharacter->selectedCharacter;
	}

	general* getHoveredCharacter(selectionInfo* selection)
	{
		if (!selection || !selection->hoveredCharacter)
			return nullptr;
		return selection->hoveredCharacter->selectedCharacter;
	}

	general* getSelectedEnemyCharacter(selectionInfo* selection)
	{
		if (!selection || !selection->selectedEnemyCharacter)
			return nullptr;
		return selection->selectedEnemyCharacter->selectedCharacter;
	}

	settlementStruct* getSelectedSettlement(selectionInfo* selection)
	{
		if (!selection || !selection->selectedSettlement)
			return nullptr;
		return selection->selectedSettlement->selectedSettlement;
	}

	settlementStruct* getHoveredSettlement(selectionInfo* selection)
	{
		if (!selection || !selection->hoveredSettlement)
			return nullptr;
		return selection->hoveredSettlement->selectedSettlement;
	}

	settlementStruct* getSelectedEnemySettlement(selectionInfo* selection)
	{
		if (!selection || !selection->selectedEnemySettlement)
			return nullptr;
		return selection->selectedEnemySettlement->selectedSettlement;
	}

	fortStruct* getSelectedFort(selectionInfo* selection)
	{
		if (!selection || !selection->selectedSettlement)
			return nullptr;
		return selection->selectedSettlement->selectedFort;
	}

	fortStruct* getHoveredFort(selectionInfo* selection)
	{
		if (!selection || !selection->hoveredSettlement)
			return nullptr;
		return selection->hoveredSettlement->selectedFort;
	}

	fortStruct* getSelectedEnemyFort(selectionInfo* selection)
	{
		if (!selection || !selection->selectedEnemySettlement)
			return nullptr;
		return selection->selectedEnemySettlement->selectedFort;
	}
}
