// pluginM2TWEOP.cpp : Определяет экспортируемые функции для DLL.
//

#include "luaPlugin.h"

#include "AsmJit.h"
#include "lua/sol.hpp"

#include "imgui/sol_ImGui.h"

int initLuaPlugin()
{
	std::string luaFile = plugData::data.modFolder + R"(\youneuoy_Data\plugins\lua\luaPluginScript.lua)";

	sol::state* luaState = plugData::data.luaAll.init(luaFile, plugData::data.modFolder);
	plugData::data.luaAll.initCampaign();
	plugData::data.luaAll.initP2();
	plugData::data.luaAll.initCampaignDb();
	plugData::data.luaAll.initUnits();
	plugData::data.luaAll.initEopFbx();
	plugData::data.luaAll.initEop3dObjects();
	plugData::data.luaAll.initEopSounds();
	plugData::data.luaAll.initTech();
	sol_ImGui::Init(*luaState);

	gameEvents::initEvents();
	if (luaState == nullptr)
	{
		MessageBoxA(nullptr, "LUA loading error!", "Error!", NULL);
		exit(0);
	}
	plugData::data.luaAll.onPluginLoadF();
	return 1;
}

void reloadLua()
{
	const std::string luaFile = plugData::data.modFolder + R"(\youneuoy_Data\plugins\lua\luaPluginScript.lua)";
	auto script = plugData::data.luaAll.luaState.load_file(luaFile);
	script();
}

