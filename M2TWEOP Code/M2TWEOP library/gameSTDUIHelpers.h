#pragma once

#include <string>
#include "realGameTypes.h"

namespace gameSTDUIHelpers
{
	std::string getUIElementName(const uiElement * element);
	uiElement* getSubElement(const uiElement * element,int index);
	buildingInfoScroll* getBuildingInfoScroll();
	unitInfoScroll* getUnitInfoScroll();
};

