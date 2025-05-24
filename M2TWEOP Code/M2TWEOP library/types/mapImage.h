#pragma once
#include <windows.h>
#include <d3d9.h>
#include "realGameTypes.h"
#include "lua/sol.hpp"

struct color
{
	int8_t r = 0;
	int8_t g = 0;
	int8_t b = 0;
};

struct tileColor
{
	tileColor() : color(0) {}
	tileColor(const uint32_t colorA, const int x, const int y) : color(colorA), coords(x, y) {}
	uint32_t color{};
	coordPair coords{};
};

struct mapImage
{
	std::vector<tileColor> tiles = {};
	bool useBlur = true;
	float blurStrength = 1.0f;
	bool adaptiveBlur = false;
};

namespace mapImageManager
{
	std::shared_ptr<mapImage> makeMapImage();
	void addToLua(sol::state& luaState);
	void clearMapImage(mapImage* img);
	std::tuple<int, int, void*> loadMapTexture(const mapImage* mapImage, const std::string& path);
	void fillRegionColor(mapImage* img, int id, int r, int g, int b, int a);
	void addRegionColor(mapImage* img, int id, int r, int g, int b, int a);
	void fillTileColor(mapImage* img, int x, int y, int r, int g, int b, int a);
	void addTileColor(mapImage* img, int x, int y, int r, int g, int b, int a);
	DWORD interpolateColors(DWORD color1, DWORD color2, float weight);
	IDirect3DTexture9* updateRegionColors(const mapImage* img, IDirect3DTexture9* regionDataTexture, int width, int height);
};

