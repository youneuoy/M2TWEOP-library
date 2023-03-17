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
	bool isDrawOnce;
	bool isDeleteNeeded;
};