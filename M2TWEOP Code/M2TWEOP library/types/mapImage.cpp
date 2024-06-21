///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "mapImage.h"
#include "campaign.h"
#include "CImg.h"
#include "dataOffsets.h"
#include "graphicsD3D.h"
#include "m2tweopMap.h"
#include "strategyMap.h"

#define GETBLUE(color) ((color) & 0xFF)
#define GETGREEN(color) (((color) >> 8) & 0xFF)
#define GETRED(color) (((color) >> 16) & 0xFF)
#define GETALPHA(color) (((color) >> 24) & 0xFF)
#define SETBLUE(color, red) ((color) = ((color) & 0xFFFFFF00) | (red))
#define SETGREEN(color, green) ((color) = ((color) & 0xFFFF00FF) | ((green) << 8))
#define SETRED(color, blue) ((color) = ((color) & 0xFF00FFFF) | ((blue) << 16))
#define SETALPHA(color, alpha) ((color) = ((color) & 0x00FFFFFF) | ((alpha) << 24))
#define MAKECOLOR(r, g, b, a) ((b) | ((g) << 8) | ((r) << 16) | ((a) << 24))

namespace mapImageManager
{
	
	std::shared_ptr<mapImage> makeMapImage()
	{
		return std::make_shared<mapImage>(mapImage());
	}

	void clearMapImage(mapImage* img)
	{
		img->tiles.clear();
	}
	
	std::tuple<int, int, void*> loadMapTexture(mapImage* mapImage, const std::string& path)
	{
		int x = 0;
		int y = 0;
		LPDIRECT3DTEXTURE9 retTexture = graphicsExport::loadTexture(path.c_str(), &x, &y);
		retTexture = updateRegionColors(mapImage, retTexture, x, y);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			x, y
		});

		return std::make_tuple(x, y, reinterpret_cast<void*>(retTexture));
	}
	
	void fillRegionColor(mapImage* img, const int id, int r, int g, int b, int a)
	{
		const auto sMap = stratMapHelpers::getStratMap();
		if (!sMap) 
			return;
		if (id >= sMap->regionsNum) 
			return;
		r = max(0, min(255, r));
		g = max(0, min(255, g));
		b = max(0, min(255, b));
		a = max(0, min(255, a));
		const auto region = &sMap->regions[id];
		for (int i = 0; i < region->tileCount; i++)
		{
			const int tileIndex = region->tiles[i];
			const uint32_t color = MAKECOLOR(r, g, b, a);
			auto [xCoord, yCoord] = stratMapHelpers::convertTileCoords(tileIndex);
			if (tileIndex >= img->tiles.size())
				img->tiles.resize(tileIndex + 1);
			img->tiles[tileIndex] = tileColor(color, xCoord, yCoord);
		}
	}

	void addRegionColor(mapImage* img, const int id, const int r, const int g, const int b, const int a)
	{
		const auto sMap = stratMapHelpers::getStratMap();
		if (!sMap) 
			return;
		if (id >= sMap->regionsNum) 
			return;
		const auto region = &sMap->regions[id];
		for (int i = 0; i < region->tileCount; i++)
		{
			const int tileIndex = region->tiles[i];
			const auto [xCoord, yCoord] = stratMapHelpers::convertTileCoords(tileIndex);
			if (tileIndex >= img->tiles.size())
				img->tiles.resize(tileIndex + 1);
			const auto newR = max(0, min(255, GETRED(img->tiles[tileIndex].color) + r));
			const auto newG = max(0, min(255, GETGREEN(img->tiles[tileIndex].color) + g));
			const auto newB = max(0, min(255, GETBLUE(img->tiles[tileIndex].color) + b));
			const auto newA = max(0, min(255, GETALPHA(img->tiles[tileIndex].color) + a));
			const uint32_t color = MAKECOLOR(newR, newG, newB, newA);
			img->tiles[tileIndex] = tileColor(color, xCoord, yCoord);
		}
	}

	void fillTileColor(mapImage* img, const int x, const int y, int r, int g, int b, int a)
	{
		const auto sMap = stratMapHelpers::getStratMap();
		if (!sMap) 
			return;
		if (x >= sMap->mapWidth || y >= sMap->mapHeight) 
			return;
		if (x < 0 || y < 0) 
			return;
		r = max(0, min(255, r));
		g = max(0, min(255, g));
		b = max(0, min(255, b));
		a = max(0, min(255, a));
		const int tileIndex = sMap->mapWidth * y + x;
		if (tileIndex >= img->tiles.size())
			img->tiles.resize(tileIndex + 1);
		img->tiles[tileIndex] = tileColor(MAKECOLOR(r, g, b, a), x, y);
	}
	
	void addTileColor(mapImage* img, const int x, const int y, int r, int g, int b, int a)
	{
		const auto sMap = stratMapHelpers::getStratMap();
		if (!sMap) 
			return;
		if (x >= sMap->mapWidth || y >= sMap->mapHeight) 
			return;
		if (x < 0 || y < 0) 
			return;
		const int tileIndex = sMap->mapWidth * y + x;
		if (tileIndex >= img->tiles.size())
			img->tiles.resize(tileIndex + 1);
		r = max(0, min(255, GETRED(img->tiles[tileIndex].color) + r));
		g = max(0, min(255, GETGREEN(img->tiles[tileIndex].color) + g));
		b = max(0, min(255, GETBLUE(img->tiles[tileIndex].color) + b));
		a = max(0, min(255, GETALPHA(img->tiles[tileIndex].color) + a));
		img->tiles[tileIndex] = tileColor(MAKECOLOR(r, g, b, a), x, y);
	}

	// Helper function to extract individual color components from a pixel
	inline int getRed(DWORD pixel) { return (pixel >> 16) & 0xFF; }
	inline int getGreen(DWORD pixel) { return (pixel >> 8) & 0xFF; }
	inline int getBlue(DWORD pixel) { return pixel & 0xFF; }

	float calculateLocalContrast(const std::vector<DWORD>& pixels, int x, int y, int width, int height) {
		int maxR = 0, maxG = 0, maxB = 0;
		int minR = 255, minG = 255, minB = 255;

		for (int ky = -1; ky <= 1; ky++) {
			for (int kx = -1; kx <= 1; kx++) {
				int px = x + kx;
				int py = y + ky;
				if (px >= 0 && px < width && py >= 0 && py < height) {
					DWORD pixel = pixels[py * width + px];
					int r = getRed(pixel);
					int g = getGreen(pixel);
					int b = getBlue(pixel);

					maxR = max(maxR, r);
					minR = min(minR, r);
					maxG = max(maxG, g);
					minG = min(minG, g);
					maxB = max(maxB, b);
					minB = min(minB, b);
				}
			}
		}
		// Calculate contrast for each channel and use the maximum as the overall contrast
		int contrastR = maxR - minR;
		int contrastG = maxG - minG;
		int contrastB = maxB - minB;

		return max(contrastR, max(contrastG, contrastB));
	}

	void applyAdaptiveGaussianBlur(DWORD* pixels, int width, int height, float strength) {
		std::vector<DWORD> temp(pixels, pixels + width * height);

		for (int y = 1; y < height - 1; y++) {
			for (int x = 1; x < width - 1; x++) {
				const float contrast = calculateLocalContrast(temp, x, y, width, height);
				float weight = contrast / 255.0f;
				weight = max(0.05f, weight);  // Ensure some blurring happens
				weight *= strength;  // Scale weight by the strength parameter

				float kernel[3][3];
				float kernelSum = 0;
				for (int ky = -1; ky <= 1; ky++) {
					for (int kx = -1; kx <= 1; kx++) {
						kernel[ky + 1][kx + 1] = exp(-0.5f * (kx * kx + ky * ky) / (weight * weight)) * weight;
						kernelSum += kernel[ky + 1][kx + 1];
					}
				}

				// Normalize the kernel
				for (int ky = 0; ky < 3; ky++) {
					for (int kx = 0; kx < 3; kx++) {
						kernel[ky][kx] /= kernelSum;
					}
				}

				float sumR = 0, sumG = 0, sumB = 0;
				for (int ky = -1; ky <= 1; ky++) {
					for (int kx = -1; kx <= 1; kx++) {
						int px = x + kx;
						int py = y + ky;
						const DWORD pixel = temp[py * width + px];

						sumR += ((pixel >> 16) & 0xFF) * kernel[ky + 1][kx + 1];
						sumG += ((pixel >> 8) & 0xFF) * kernel[ky + 1][kx + 1];
						sumB += (pixel & 0xFF) * kernel[ky + 1][kx + 1];
					}
				}

				int newR = min(max(int(sumR), 0), 255);
				int newG = min(max(int(sumG), 0), 255);
				int newB = min(max(int(sumB), 0), 255);
				pixels[y * width + x] = (0xFF << 24) | (newR << 16) | (newG << 8) | newB;
			}
		}
	}

	void generateGaussianKernel(float kernel[3][3], float sigma) {
		float sum = 0.0f;
		int size = 3;  // Kernel size
		int kCenter = size / 2;
		float twoSigmaSquare = 2.0f * sigma * sigma;

		for (int y = -kCenter; y <= kCenter; y++) {
			for (int x = -kCenter; x <= kCenter; x++) {
				float exponent = -(x * x + y * y) / twoSigmaSquare;
				kernel[y + kCenter][x + kCenter] = exp(exponent) / (cimg_library::cimg::PI * twoSigmaSquare);
				sum += kernel[y + kCenter][x + kCenter];
			}
		}

		// Normalize the kernel
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				kernel[y][x] /= sum;
			}
		}
	}

	void applyGaussianBlur(DWORD* pixels, int width, int height, float strength) {
		std::vector<DWORD> temp(pixels, pixels + width * height);  // Copy original pixels to temp

		float kernel[3][3];
		generateGaussianKernel(kernel, strength);

		for (int y = 1; y < height - 1; y++) {
			for (int x = 1; x < width - 1; x++) {
				float sumR = 0, sumG = 0, sumB = 0;

				for (int ky = -1; ky <= 1; ky++) {
					for (int kx = -1; kx <= 1; kx++) {
						int px = x + kx;
						int py = y + ky;
						DWORD pixel = temp[py * width + px];

						sumR += ((pixel >> 16) & 0xFF) * kernel[ky + 1][kx + 1];
						sumG += ((pixel >> 8) & 0xFF) * kernel[ky + 1][kx + 1];
						sumB += (pixel & 0xFF) * kernel[ky + 1][kx + 1];
					}
				}

				int newR = min(max(int(sumR), 0), 255);
				int newG = min(max(int(sumG), 0), 255);
				int newB = min(max(int(sumB), 0), 255);
				pixels[y * width + x] = (0xFF << 24) | (newR << 16) | (newG << 8) | newB;
			}
		}
	}
	
	IDirect3DTexture9* updateRegionColors(mapImage* img, IDirect3DTexture9* regionDataTexture, int width, int height) {
		ofstream f1("logs\\lol.youneuoylog");
		D3DLOCKED_RECT lockedRect;
		auto success = regionDataTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
		if (FAILED(success)) {
			regionDataTexture->Release();
			return nullptr;
		}
		auto* pTexels = static_cast<DWORD*>(lockedRect.pBits);
		const gameDataAllStruct* gameDataAll = dataOffsets::offsets.gameDataAllOffset;
		const stratMap* sMap = gameDataAll->stratMap;
		const int xScale = width / sMap->mapWidth;
		const int yScale = height / sMap->mapHeight;

		for (auto tileCol : img->tiles)
		{
			if (tileCol.color == 0)
				continue;
			const float weight = GETALPHA(tileCol.color) / 255.0f;
			SETALPHA(tileCol.color, 255);
			const int gameX = tileCol.coords.xCoord;
			const int gameY = tileCol.coords.yCoord;
			for (int i = 0, imageX = gameX * xScale; i < xScale; i++, imageX++)
				for (int j = 0, imageY = height - (gameY * yScale) - yScale; j < yScale; j++, imageY++)
				{
					imageX = max(0, min(width - 1, imageX));
					imageY = max(0, min(height - 1, imageY));
					//auto formattedString = "imageX:" + std::to_string(imageX) + " imageY:" + std::to_string(imageY) + " gameX:" + std::to_string(gameX) + " gameY:" + std::to_string(gameY);
					//f1 << formattedString << '\n';
					DWORD* pPixel = pTexels + imageY * lockedRect.Pitch / sizeof(DWORD) + imageX;
					*pPixel = interpolateColors(*pPixel, tileCol.color, weight);
				}
		}

		if (img->useBlur)
		{
			if (img->adaptiveBlur)
				applyAdaptiveGaussianBlur(pTexels, width, height, img->blurStrength);
			else
				applyGaussianBlur(pTexels, width, height, img->blurStrength);
		}
		
		
		success = regionDataTexture->UnlockRect(0);
		if (FAILED(success)) {
			regionDataTexture->Release();
		}
		return regionDataTexture;
	}

	DWORD interpolateColors(DWORD color1, DWORD color2, float weight) {
		const int r1 = (color1 >> 16) & 0xFF;
		const int g1 = (color1 >> 8) & 0xFF;
		const int b1 = color1 & 0xFF;

		const int r2 = (color2 >> 16) & 0xFF;
		const int g2 = (color2 >> 8) & 0xFF;
		const int b2 = color2 & 0xFF;

		int r = static_cast<int>(r1 * (1 - weight) + r2 * weight);
		int g = static_cast<int>(g1 * (1 - weight) + g2 * weight);
		int b = static_cast<int>(b1 * (1 - weight) + b2 * weight);

		r = std::clamp(r, 0, 255);
		g = std::clamp(g, 0, 255);
		b = std::clamp(b, 0, 255);
		
		return D3DCOLOR_ARGB(255, r, g, b);
	}
	
	void mapImageManager::createImage(mapImage* image)
	{
	}
	
	void addToLua(sol::state& luaState)
	{
		struct
		{
			sol::usertype<mapImage> mapImageStruct;
		}typeAll;
		
		///mapImage
		//@section mapImageStruct

		/***
		@tfield float blurStrength
		@tfield bool useBlur
		@tfield bool adaptiveBlur Can be slow on large or frequently updated images! needs use blur also true.
		@tfield makeMapImage makeMapImage
		@tfield clearMapImage clearMapImage
		@tfield loadMapTexture loadMapTexture
		@tfield fillRegionColor fillRegionColor
		@tfield addRegionColor addRegionColor
		@tfield fillTileColor fillTileColor
		@tfield addTileColor addTileColor
		@table mapImageStruct
		*/
		typeAll.mapImageStruct = luaState.new_usertype<mapImage>("mapImageStruct");
		typeAll.mapImageStruct.set("useBlur", &mapImage::useBlur);
		typeAll.mapImageStruct.set("adaptiveBlur", &mapImage::adaptiveBlur);
		typeAll.mapImageStruct.set("blurStrength", &mapImage::blurStrength);
		
		/***
		Create a new image you want to determine region colors.
		@function mapImageStruct.makeMapImage
		@treturn mapImageStruct mapImage
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		*/
		typeAll.mapImageStruct.set_function("makeMapImage", &makeMapImage);
		
		/***
		Reset image state.
		@function mapImageStruct:clearMapImage
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		mapImage:clearMapImage();
		*/
		typeAll.mapImageStruct.set_function("clearMapImage", &clearMapImage);
		
		/***
		Create a new map texture. Use an uncompressed(!) dds image that is some multiple of your map_regions size. The background must line up with map_regions for whatever scale you chose for it to display correctly.
		@function mapImageStruct:loadMapTexture
		@tparam string path full path to texture
		@treturn int x size of the image
		@treturn int y size of the image
		@treturn int id of the image
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		local x, y, id = mapImage:loadMapTexture(M2TWEOP.getModPath() .. "map_regions.dds");
		ImGui.Image(id, x, y);
		*/
		typeAll.mapImageStruct.set_function("loadMapTexture", &loadMapTexture);
		
		/***
		Fill a region with a color.
		@function mapImageStruct:fillRegionColor
		@tparam int id region ID
		@tparam int r red
		@tparam int g green
		@tparam int b blue
		@tparam int a alpha
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		mapImage:fillRegionColor(50, 0, 255, 0, 255);
		*/
		typeAll.mapImageStruct.set_function("fillRegionColor", &fillRegionColor);
		
		/***
		Add a color to already filled region.
		@function mapImageStruct:addRegionColor
		@tparam int id region ID
		@tparam int r red
		@tparam int g green
		@tparam int b blue
		@tparam int a alpha
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		mapImage:fillRegionColor(50, 0, 255, 0, 255);
		mapImage:addRegionColor(50, 10, -10, 10, 0);
		*/
		typeAll.mapImageStruct.set_function("addRegionColor", &addRegionColor);
		
		/***
		Fill a tile with a color.
		@function mapImageStruct:fillTileColor
		@tparam int x x coordinate
		@tparam int y y coordinate
		@tparam int r red
		@tparam int g green
		@tparam int b blue
		@tparam int a alpha
		@treturn mapImageStruct mapImage
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		mapImage:fillTileColor(153, 210, 0, 255, 0, 255);
		*/
		typeAll.mapImageStruct.set_function("fillTileColor", &fillTileColor);
		/***
		Add a color to an already set tile.
		@function mapImageStruct:addTileColor
		@tparam int x x coordinate
		@tparam int y y coordinate
		@tparam int r red
		@tparam int g green
		@tparam int b blue
		@tparam int a alpha
		@treturn mapImageStruct mapImage
		@usage
		local mapImage = mapImageStruct.makeMapImage();
		mapImage:fillTileColor(153, 210, 0, 255, 0, 255);
		mapImage:addTileColor(153, 210, 20, -10, 20, 0);
		*/
		typeAll.mapImageStruct.set_function("addTileColor", &addTileColor);
	}
};