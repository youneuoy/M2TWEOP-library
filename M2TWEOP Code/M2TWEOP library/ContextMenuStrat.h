#pragma once
#include "imgui.h"
#include <map>
#include <string>

//return true - menu closed
typedef bool (*ContextMenuItemWork)();
//return true - menu state changes to active
typedef bool (*ContextMenuItemNeedWork)(int posAtMap[2]);
class ContextMenuItem
{
public:
	bool Active = false;

	ContextMenuItem(ContextMenuItemWork work, ContextMenuItemNeedWork needWork);
	ContextMenuItemWork Work;
	ContextMenuItemNeedWork NeedWork;
};

class ContextMenuStrat
{
public:
	ContextMenuStrat();
	bool isContextMenuNeeded = true;
	void Draw();
private:
	std::map<std::string,ContextMenuItem>ctxItems;


	ImVec2 ctxPos = { 0,0 };
	ImVec2 ctxSize = { 0,0 };
	bool isWork = false;

};

