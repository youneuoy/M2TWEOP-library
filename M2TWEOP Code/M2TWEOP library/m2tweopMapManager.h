#pragma once
#include <windows.h>
#include <d3d9.h>
#include "realGameTypes.h"

namespace m2tweopMapManager
{
	void createImage(mapImage* image);
	DWORD interpolateColors(DWORD color1, DWORD color2, float weight);
	IDirect3DTexture9* updateRegionColors(mapImage* img, IDirect3DTexture9* regionDataTexture, int width, int height);
	bool draw();
};

