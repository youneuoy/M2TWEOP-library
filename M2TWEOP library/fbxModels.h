#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "realGameTypes.h"


using namespace std;
namespace fbxModels
{
	void set3dDevice(LPDIRECT3DDEVICE9 device);

	//1-stratmap
	//2-tactmap
	void draw(int drawType,int gameVer);

	NOINLINE EOP_EXPORT bool addFbxModel(const char* path, const char* texture,int modelID);
	NOINLINE EOP_EXPORT bool deleteFbxModel(int modelID);
	NOINLINE EOP_EXPORT void* getFbxModel(int modelID);





	NOINLINE EOP_EXPORT void* addFbxObject(int modelID,int objectID);
	NOINLINE EOP_EXPORT bool deleteFbxObject(void* object);
	NOINLINE EOP_EXPORT void* getFbxObject(int objectID);


	NOINLINE EOP_EXPORT void setFbxObjectCoords(void*object,float* coords);
	NOINLINE EOP_EXPORT float* getFbxObjectCoords(void* object);

	NOINLINE EOP_EXPORT void setFbxObjectSize(void* object, float size);
	NOINLINE EOP_EXPORT float getFbxObjectSize(void* object);

	NOINLINE EOP_EXPORT void setFbxObjectDrawState(void* object, bool isDrawNeed);
	NOINLINE EOP_EXPORT bool getFbxObjectDrawState(void* object);

	NOINLINE EOP_EXPORT void setFbxObjectAnimState(void* object, bool isAnimate);
	NOINLINE EOP_EXPORT bool getFbxObjectAnimState(void* object);
};

