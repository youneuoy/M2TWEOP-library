#pragma once
#include "plugData.h"
#include <string>

namespace gameEvents
{
	void initEvents();
}

int initLuaPlugin(std::string * modPath);
void initLua();
void reloadLua();