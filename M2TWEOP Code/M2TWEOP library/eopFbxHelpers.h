#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace eopFbxHelpers
{
	void addFbxModel(const char* modelPath, const char* texturePath,int modelID);
	void deleteFbxModel(int modelID);
	void* getFbxModel(int modelID);

	void* addFbxObject(int modelID,int objectID);
	void deleteFbxObject(void* object);
	void* getFbxObject(int objectID);


	void setFbxObjectCoords(void* object,float x,float y,float z);

	std::tuple <float,float,float> getFbxObjectCoords(void* object);

	void setFbxObjectSize(void* object,float size);
	float getFbxObjectSize(void* object);


	void setFbxObjectDrawState(void* object,bool drawState);
	bool getFbxObjectDrawState(void* object);

	void setFbxObjectAnimState(void* object,bool animState);
	bool getFbxObjectAnimState(void* object);

	void setFbxObjectDrawType(void* object, int drawType);
	int getFbxObjectDrawType(void* object);
};

