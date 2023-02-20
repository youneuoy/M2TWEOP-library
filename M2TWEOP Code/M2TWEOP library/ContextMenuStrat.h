#pragma once
#include "imgui.h"
class ContextMenuStrat
{
public:
	bool isContextMenuNeeded = true;
	void Draw();
private:
	ImVec2 ctxPos = { 0,0 };
	int posAtMap[2]{};
	bool isWork = false;
};

