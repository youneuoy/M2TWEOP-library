#include "pch.h"
#include "eopFbxHelpers.h"

#include "fbxModels.h"
#include "luaPlugin.h"

void eopFbxHelpers::setFbxObjectCoords(void* object, float x, float y, float z)
{
	float coords[3];
	coords[0] = x;
	coords[1] = z;
	coords[2] = y;
	fbxModels::setFbxObjectCoords(object, coords);
}

std::tuple<float, float, float> eopFbxHelpers::getFbxObjectCoords(void* object)
{
	float* coords = fbxModels::getFbxObjectCoords(object);
	return std::make_tuple(coords[0], coords[2], coords[1]);
}
