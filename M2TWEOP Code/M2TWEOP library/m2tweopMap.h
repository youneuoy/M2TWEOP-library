#pragma once
#include <vector>
#include "imgui/imgui.h"

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

	int mapWidth = 0;
	int mapHeight = 0;

};

