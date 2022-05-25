#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>


#if _DEBUG
#pragma comment(lib,"SDL2_D.lib")
#pragma comment(lib,"SDL2main_d.lib")
#else
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#endif
#pragma comment(lib, "opengl32.lib")
#include "imgui.h"
#include "eopImGui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "imgui_stdlib.h"
#include <imgui_internal.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include <SFML/Audio.hpp>
using namespace std;



#include "json.hpp"
//json namespace

namespace jsn = nlohmann;


//macro for fast getting values from json. Need jsn::json obect, named json!
#define getJson(field,jsonName)\
field= json.at(jsonName);

//macro for fast setting values to json. Need jsn::json obect, named json!
#define setJson(jsonName,field)\
json[jsonName]=field;


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
	ImVec2 screenUpperPos{ 0,0 };
	ImVec2 centerXButton{ -1.0f,0.0f };


	//GLFWimage programIcon[1];
};

#include "dataG.h"

