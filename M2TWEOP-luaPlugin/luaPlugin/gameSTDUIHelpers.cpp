#include "gameSTDUIHelpers.h"
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

