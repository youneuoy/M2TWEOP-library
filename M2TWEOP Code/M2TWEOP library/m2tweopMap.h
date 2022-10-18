#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "eventsCodes.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "imgui.h"
#include <d3d9.h>
#include <sstream>
#include "realGameTypes.h"

#include "m2tweopTile.h"
using namespace std;


class m2tweopMap
{
public:
	void buildMap();
	void drawMap(const ImVec2& tileSize);
	void drawInteract();

private:
	shared_ptr<m2tweopTile>selectedTile = nullptr;

	vector<ImVec2>borders;
	bool needDrawBorders = true;

	vector<shared_ptr<m2tweopTile>>tiles;
	ImVec2 tileSize = { 4.f,4.f };

	int xBound = 0;
	int yBound = 0;

};

