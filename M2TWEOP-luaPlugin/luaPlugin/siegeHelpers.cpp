#include "siegeHelpers.h"
#include "plugData.h"

settlementStruct* siegeHelpers::getSiegedSettlement(siegeS* siege)
{
	return (*(*plugData::data.funcs.findSettlement))(siege->goal->xCoord, siege->goal->yCoord);
}

fortStruct* siegeHelpers::getSiegedFort(siegeS* siege)
{
	return (*(*plugData::data.funcs.findFort))(siege->goal->xCoord, siege->goal->yCoord);
}
