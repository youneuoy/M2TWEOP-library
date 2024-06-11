#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalHelpers
{
	void sabotageBuilding(general* gen, building* build);

	const char* getTypeName(const general* gen);
	void setMovepoints(general* gen, float movepoints);
	float getMovepoints(const general* gen);
	int getTypeID(const general* gen);
}