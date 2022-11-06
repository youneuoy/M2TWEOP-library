#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"

namespace siegeHelpers
{
	settlementStruct* getSiegedSettlement(siegeS* siege);
	fortStruct* getSiegedFort(siegeS* siege);
};

