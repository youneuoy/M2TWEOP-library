#include "gameSTDUIHelpers.h"
#include "gameDataAllHelper.h"
#include "plugData.h"

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


namespace gameSTDUIHelpers
{
	bool useButton(const char* buttonName)
	{
		DWORD findedButton = 0;
		char** cryptS = fastFunctsHelpers::makeCryptedString(buttonName);
		DWORD adrF = codes::offsets.getUiElementFunc;
		_asm
			{
			push cryptS
			mov eax, adrF
			call eax
			mov findedButton, eax
			add esp, 0x4
			}
		if (findedButton == 0)
		{
			return false;
		}

		adrF = codes::offsets.useButtonFunc;
		_asm
			{
			mov ecx, findedButton
			mov eax, adrF
			call eax
			}
		return true;
	}
	uiElement* getUiElement(const char* elementName)
	{
		uiElement* resElement = nullptr;
		char** cryptS = fastFunctsHelpers::makeCryptedString(elementName);
		DWORD adrF = codes::offsets.getUiElementFunc;
		_asm
			{
			push cryptS
			mov eax, adrF
			call eax
			mov resElement, eax
			add esp, 0x4
			}

		return resElement;
	}
	void useUiElement(uiElement* element)
	{
		DWORD adrF = codes::offsets.useButtonFunc;
		_asm
			{
			mov ecx, element
			mov eax, adrF
			call eax
			}
	}
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

