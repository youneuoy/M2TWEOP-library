#pragma once
#include "realGameTypes.h"
namespace eopFbxHelpers
{
	void setFbxObjectCoords(void* object,float x,float y,float z);
	std::tuple <float,float,float> getFbxObjectCoords(void* object);
};

