#pragma once

#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"

namespace gameSTDUIHelpers
{
	uiElement* getUiElement(const char* elementName);
	void useUiElement(uiElement* element);
	std::string getUIElementName(const uiElement * element);
	uiElement* getSubElement(const uiElement * element,int index);
};

