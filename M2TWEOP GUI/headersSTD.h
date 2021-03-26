#pragma once
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "../3rd/glfw/lib-vc2010-32/glfw3.lib")

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "imgui_stdlib.h"
#include <imgui_internal.h>
#include <GLFW/glfw3.h>
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
struct GLImage
{
	string path;
	GLuint image = 0;
	int xSize = 0;
	int ySize = 0;
};

struct screenS
{
	ImVec2 screenSize{ 0,0 };

	ImVec2 screenHalfSize{ 0,0 };
	ImVec2 centerXButton{ -1.0f,0.0f };


	GLFWimage programIcon[1];
};

#include "dataG.h"