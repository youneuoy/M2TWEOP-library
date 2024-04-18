#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "eventsCodes.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "imgui.h"
#include <d3d9.h>
#include <sstream>
#include "realGameTypes.h"

#include "m2tweopTile.h"
#include "m2tweopMap.h"
using namespace std;
namespace m2tweopMapManager
{
	void createImage(mapImage* image);
	DWORD interpolateColors(DWORD color1, DWORD color2, float weight);
	NOINLINE EOP_EXPORT IDirect3DTexture9* updateRegionColors(mapImage* img, IDirect3DTexture9* regionDataTexture, int width, int height);
	bool draw();
};

