#pragma once
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
using namespace std;

struct fontS
{
	ImFont* font;
	string path;
	string name;
};
struct d3dImage
{
	string path;
	LPDIRECT3DTEXTURE9 image = nullptr;
	int xSize = 0;
	int ySize = 0;
};

struct screenS
{
	ImVec2 screenSize{ 0,0 };

	ImVec2 screenHalfSize{ 0,0 };
	ImVec2 centerXButton{ -1.0f,0.0f };
};

#include "dataG.h"