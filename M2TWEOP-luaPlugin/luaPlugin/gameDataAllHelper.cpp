#include "gameDataAllHelper.h"

#include "plugData.h"
namespace gameDataAllHelper
{
	gameDataAllStruct* get()
	{
			return (*(*plugData::data.funcs.getGameDataAll))();
	}
}
