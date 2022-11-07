#include "eopFbxHelpers.h"
#include "plugData.h"

void eopFbxHelpers::addFbxModel(const char* modelPath, const char* texturePath, int modelID)
{
	(*(*plugData::data.functsEopFbx.addFbxModel))(modelPath, texturePath, modelID);
}

void eopFbxHelpers::deleteFbxModel(int modelID)
{
	(*(*plugData::data.functsEopFbx.deleteFbxModel))(modelID);
}

void* eopFbxHelpers::getFbxModel(int modelID)
{
	return (*(*plugData::data.functsEopFbx.getFbxModel))(modelID);
}

void* eopFbxHelpers::addFbxObject(int modelID, int objectID)
{
	return (*(*plugData::data.functsEopFbx.addFbxObject))(modelID, objectID);
}

void eopFbxHelpers::deleteFbxObject(void* object)
{
	(*(*plugData::data.functsEopFbx.deleteFbxObject))(object);
}

void* eopFbxHelpers::getFbxObject(int objectID)
{
	return (*(*plugData::data.functsEopFbx.getFbxObject))(objectID);
}

void eopFbxHelpers::setFbxObjectCoords(void* object, float x, float y, float z)
{
	float coords[3];
	coords[0] = x;
	coords[1] = z;
	coords[2] = y;
	(*(*plugData::data.functsEopFbx.setFbxObjectCoords))(object, coords);
}

std::tuple<float, float, float> eopFbxHelpers::getFbxObjectCoords(void* object)
{
	float*coords=(*(*plugData::data.functsEopFbx.getFbxObjectCoords))(object);
	return std::make_tuple(coords[0], coords[2], coords[1]);
}

void eopFbxHelpers::setFbxObjectSize(void* object, float size)
{
	(*(*plugData::data.functsEopFbx.setFbxObjectSize))(object, size);
}

float eopFbxHelpers::getFbxObjectSize(void* object)
{
	return (*(*plugData::data.functsEopFbx.getFbxObjectSize))(object);
}

void eopFbxHelpers::setFbxObjectDrawState(void* object, bool drawState)
{
	(*(*plugData::data.functsEopFbx.setFbxObjectDrawState))(object, drawState);
}

bool eopFbxHelpers::getFbxObjectDrawState(void* object)
{
	return (*(*plugData::data.functsEopFbx.getFbxObjectDrawState))(object);
}

void eopFbxHelpers::setFbxObjectAnimState(void* object, bool animState)
{
	(*(*plugData::data.functsEopFbx.setFbxObjectAnimState))(object, animState);
}

bool eopFbxHelpers::getFbxObjectAnimState(void* object)
{
	return (*(*plugData::data.functsEopFbx.getFbxObjectAnimState))(object);
}

void eopFbxHelpers::setFbxObjectDrawType(void* object, int drawType)
{
	(*(plugData::data.functsEopFbx.setFbxObjectDrawType))(object, drawType);
}

int eopFbxHelpers::getFbxObjectDrawType(void* object)
{
	return (*(plugData::data.functsEopFbx.getFbxObjectDrawType))(object);
}
