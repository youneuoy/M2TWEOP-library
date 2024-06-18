#include "m2tweopMapManager.h"

#include <numbers>

#include "campaign.h"
#include "imgui/imgui.h"

#include "CImg.h"
#include "dataOffsets.h"
#include "graphicsD3D.h"
#include "m2tweopMap.h"
#include "strategyMap.h"


namespace m2tweopMapManager
{
	struct
	{
		m2tweopMap currentMap;
		ImVec2 tileSize = {7.f,7.f};
		bool isShowMap = false;
		bool isShowBattleMap = false;
		bool showBattleStats = false;
	}managerData;

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
	
	void m2tweopMapManager::createImage(mapImage* image)
	{
	}
	
	bool draw()
	{
		bool isWork = true;

		ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
		if (managerData.isShowMap == true)
		{
			ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize, ImGuiCond_Always);
		}
		else
		{
			ImGui::SetNextWindowSize({ 0,0 }, ImGuiCond_Always);
		}
		ImGui::Begin("Map manager",nullptr, ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoMove);


		if (ImGui::Button("Close"))
		{
			isWork = false;
		}
		if (managerData.isShowMap == false)
		{
			if (ImGui::Button("Open map manager"))
			{
				managerData.currentMap.buildMap();
				managerData.isShowMap = true;
			}
		}
		else
		{
			if (ImGui::Button("Hide map manager"))
			{
				managerData.isShowMap = false;
			}
		}


		if (managerData.isShowMap == true)
		{
			ImGui::SetNextItemWidth(200.f);
			if (ImGui::SliderFloat("Map scale", &managerData.tileSize.x, 2.0f, 100.0f, "%.0f"))
			{
			}
			managerData.tileSize.y = managerData.tileSize.x;

			ImVec2 mapImageSize = ImGui::GetMainViewport()->WorkSize;
			mapImageSize.x -= mapImageSize.x / 5;
			mapImageSize.y -= mapImageSize.y / 8;
			ImGui::BeginChild("mapImage", mapImageSize,true, ImGuiWindowFlags_HorizontalScrollbar);
			managerData.currentMap.drawMap(managerData.tileSize);
			ImGui::EndChild();


			ImGui::SameLine();
			ImVec2 mapInteractSize;
			mapInteractSize.x = ImGui::GetMainViewport()->WorkSize.x - mapImageSize.x;
			mapInteractSize.y = mapImageSize.y;

			ImGui::BeginChild("mapInteract", mapInteractSize, true, ImGuiWindowFlags_HorizontalScrollbar);
			managerData.currentMap.drawInteract();
			ImGui::EndChild();
		}

		ImGui::End();

		return isWork;
	}
};