#include "siegeHelpers.h"

#include "gameHelpers.h"

settlementStruct* siegeHelpers::getSiegedSettlement(siegeS* siege)
{
	auto settlement = siege->goal;
	auto tile = gameHelpers::getTile(settlement->xCoord, settlement->yCoord);
	return gameHelpers::getTileSettlement(tile);
}

fortStruct* siegeHelpers::getSiegedFort(siegeS* siege)
{
	auto settlement = siege->goal;
	auto tile = gameHelpers::getTile(settlement->xCoord, settlement->yCoord);
	return gameHelpers::getTileFort(tile);
}
