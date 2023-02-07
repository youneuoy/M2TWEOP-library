#pragma once
#include "imgui.h"
class ContextMenuStrat
{
public:
	void Draw();
private:
	ImVec2 ctxPos = { 0,0 };
	int posAtMap[2]{};
	bool isWork = false;
};

