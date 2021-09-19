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

	//not threadsafe, but looks stable
	void startCreationThread();
};

