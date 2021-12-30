#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "eventsCodes.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <d3d9.h>
#include <sstream>
#include <thread>
#include <set>
#include <map>
#include <filesystem>
#include "realGameTypes.h"
using namespace std;
namespace battleCreator
{
	void setCreationNeeded(bool isNeeded = false);
	void readParams();
	//for creation of battles and battles results
	//not threadsafe, but looks stable
	void startCreationThread();


	void onBattleResultsScreen();

	void onBattleStratScreen();
	void onHotseatScreen();
	void onPostBattleStratScreen();
	void draw(LPDIRECT3DDEVICE9 pDevice);


	void addCharactersToCustomBattleArmy(stackStruct* army, const std::string& relativePath);
};


