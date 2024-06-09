#include "gameSTDUIHelpers.h"

#include "gameDataAllHelper.h"
#include "plugData.h"

uiElement* gameSTDUIHelpers::getUiElement(const char* elementName)
{
	return (*(*plugData::data.functsGameUI.getUiElement))(elementName);
}

void gameSTDUIHelpers::useUiElement(uiElement* element)
{
	(*(*plugData::data.functsGameUI.useUiElement))(element);
}

std::string gameSTDUIHelpers::getUIElementName(const uiElement* element)
{
	if (element->elementName)
	{
		return std::string(element->elementName);
	}

	return std::string("");
}

uiElement* gameSTDUIHelpers::getSubElement(const uiElement* element, int index)
{
	if (element->subElementsNum > index)
	{
		return element->subElements[index];
	}
	return nullptr;
}

buildingInfoScroll* gameSTDUIHelpers::getBuildingInfoScroll()
{
	const auto gameData = gameDataAllHelper::get();
	if (!gameData->uiManager) 
		return nullptr;
	if (!gameData->uiManager->stratUI) 
		return nullptr;
	return gameData->uiManager->stratUI->buildingInfoScroll;
}

unitInfoScroll* gameSTDUIHelpers::getUnitInfoScroll()
{
	const auto gameData = gameDataAllHelper::get();
	if (!gameData->uiManager) 
		return nullptr;
	if (!gameData->uiManager->stratUI) 
		return nullptr;
	return gameData->uiManager->stratUI->unitInfoScroll;
}

