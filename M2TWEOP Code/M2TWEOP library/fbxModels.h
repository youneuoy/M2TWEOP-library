#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "globals.h"
#include "techFuncs.h"
#include "realGameTypes.h"


using namespace std;
namespace fbxModels
{
	void set3dDevice(LPDIRECT3DDEVICE9 device);

	//1-stratmap
	//2-tactmap
	void draw(int drawType,int gameVer);

	bool addFbxModel(const char* path, const char* texture,int modelID);
	bool deleteFbxModel(int modelID);
	void* getFbxModel(int modelID);





	void* addFbxObject(int modelID,int objectID);
	bool deleteFbxObject(void* object);
	void* getFbxObject(int objectID);


	void setFbxObjectCoords(void*object,float* coords);
	float* getFbxObjectCoords(void* object);

	void setFbxObjectSize(void* object, float size);
	float getFbxObjectSize(void* object);

	void setFbxObjectDrawState(void* object, bool isDrawNeed);
	bool getFbxObjectDrawState(void* object);

	void setFbxObjectAnimState(void* object, bool isAnimate);
	bool getFbxObjectAnimState(void* object);

	int getFbxObjectDrawType(void* object);
	void setFbxObjectDrawType(void* object,int drawType);
};

