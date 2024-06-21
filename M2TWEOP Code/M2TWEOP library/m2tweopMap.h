#pragma once
#include "m2tweopTile.h"

class m2tweopMap
{
public:
	void buildMap();
	void drawMap(const ImVec2& size);
	void drawInteract();

private:
	std::shared_ptr<m2tweopTile>selectedTile = nullptr;
	std::vector<ImVec2>borders;
	bool needDrawBorders = true;
	std::vector<std::shared_ptr<m2tweopTile>>tiles;
	ImVec2 tileSize = { 4.f,4.f };
	int mapWidth = 0;
	int mapHeight = 0;
};

