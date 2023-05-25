#pragma once
#include <wtypes.h>
#include <d3dx9mesh.h>
struct Text3DDrawable
{
	LPD3DXMESH textMesh;
	DWORD color;
	float xCoord;
	float yCoord;
	float zCoord;
	float xSize = 1;
	float ySize = 1;
	float zSize = 1;
	float xRoll = 1;
	float yRoll = 90;
	float zRoll = 1;
	bool isDrawOnce;
	bool isDeleteNeeded;
};