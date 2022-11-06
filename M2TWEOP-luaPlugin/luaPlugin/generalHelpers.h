#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalHelpers
{
	void moveToTile(general* gen, int x, int y);
	void reposition(general* gen, int x, int y);


	void killGeneral(general* gen);
	void setBodyguard(general* gen,unit*un);
	void setMovepoints(general* gen, float movepoints);
	float getMovepoints(const general* gen);
	int getTypeID(const general* gen);
	void setTypeID(general* gen,int newType);
};

