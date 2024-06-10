#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_internal.h"

#include <string>
#include <fstream>
#include <vector>

namespace console
{
	void draw();
	void toggleConsole();
	void reloadScript();
	void restartLua();
};

