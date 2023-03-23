#include "PlannedRetreatRoute.h"
#include "graphicsD3D.h"
#include <mutex>
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "MapTextDrawer.h"

#include "PathMap.h"

namespace PlannedRetreatRoute
{

	struct coordsVText
	{
		coordsVText() = delete;
		coordsVText(int x, int y, MapTextDrawer::Text3DDrawable* pointText)
			:PointText(pointText), X(x), Y(y)
		{

		}
		~coordsVText()
		{
			MapTextDrawer::Delete3dText(PointText);
		}
		MapTextDrawer::Text3DDrawable* PointText = nullptr;
		int X = 0;
		int Y = 0;
	};
	struct stateS
	{
		vector<coordsVText>possibleCoords;
		int maxPathLenInTiles = 9;
	}state;

	static void Draw()
	{
		for (auto& txt : state.possibleCoords)
		{
			MapTextDrawer::DrawingTextOnce(txt.PointText);
		}
	}
	static void MakeTexts(stateS& st)
	{
		void* font = MapTextDrawer::MakeTextFont("Times New Roman");
		if (font == nullptr)
		{
			MessageBoxA(NULL, "Cannot create text font for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
			std::terminate();
		}

		for (auto& txt : state.possibleCoords)
		{
			txt.PointText = MapTextDrawer::MakeText(font, "+");
			if (txt.PointText == nullptr)
			{
				MessageBoxA(NULL, "Cannot create text for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
				std::terminate();
			}
			txt.PointText->xCoord = txt.X;
			txt.PointText->yCoord = txt.Y;
			txt.PointText->zCoord = 0.2f;
		}

		MapTextDrawer::DeleteTextFont(font);
	}
	static void TryInit()
	{
		graphicsExport::AddStratmapDrawCallback(Draw);
	}

	void StartWork(int x, int y)
	{
		static std::once_flag initFLAG;
		std::call_once(initFLAG, TryInit);

		auto* army = fastFuncts::findArmy(x, y);
		if (army == nullptr)
		{
			return;
		}

		state.possibleCoords.clear();
		state.possibleCoords.reserve(state.maxPathLenInTiles * state.maxPathLenInTiles);

		float possibleMP = smallFuncs::GetMinimumPossibleMovepointsForArmy(army);


		int coordsMod = 1;

		void* cashe = PathFinder::CreateCasheForDistances(x, y, 50);

		state.possibleCoords.emplace_back(x, y, nullptr);

		do
		{
			size_t posSize = state.possibleCoords.size();
			int xMin = x - coordsMod;
			int xMax = x + coordsMod;
			int yMin = y - coordsMod;
			int yMax = y + coordsMod;


			for (int yDest = yMin, xDest = xMin; yDest < yMax; ++yDest)
			{
				float distance = PathFinder::GetMovepointsForReachTileFromCashe(cashe, x, y, xDest, yDest);
				if (distance >= 0 && distance < possibleMP)
				{
					state.possibleCoords.emplace_back(xDest, yDest, nullptr);
				}
			}

			for (int yDest = yMin, xDest = xMax; yDest <= yMax; ++yDest)
			{
				float distance = PathFinder::GetMovepointsForReachTileFromCashe(cashe, x, y, xDest, yDest);
				if (distance >= 0 && distance < possibleMP)
				{
					state.possibleCoords.emplace_back(xDest, yDest, nullptr);
				}
			}


			for (int xDest = xMin + 1, yDest = yMin; xDest < xMax; ++xDest)
			{
				float distance = PathFinder::GetMovepointsForReachTileFromCashe(cashe, x, y, xDest, yDest);
				if (distance >= 0 && distance < possibleMP)
				{
					state.possibleCoords.emplace_back(xDest, yDest, nullptr);
				}
			}

			for (int xDest = xMin, yDest = yMax; xDest < xMax; ++xDest)
			{
				float distance = PathFinder::GetMovepointsForReachTileFromCashe(cashe, x, y, xDest, yDest);
				if (distance >= 0 && distance < possibleMP)
				{
					state.possibleCoords.emplace_back(xDest, yDest, nullptr);
				}
			}


			if (state.possibleCoords.size() == posSize)
			{
				break;
			}
			++coordsMod;
		} while (true);

		PathFinder::DeleteCasheForDistances(cashe);
		MakeTexts(state);
	}
}