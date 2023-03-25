#include "PlannedRetreatRoute.h"
#include "graphicsD3D.h"
#include <iomanip>
#include <mutex>
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "MapTextDrawer.h"

#include "PathMap.h"
#include "imgui_notify.h"

#include <filesystem>
#include "RetreatRoutes.h"
namespace PlannedRetreatRoute
{
	struct
	{
		vector<RetreatRoute>data;
	}routes;


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
		bool workingNow = false;

		vector<coordsVText>possibleCoords;
		int maxPathLenInTiles = 15;

		coordsVText selectedCoord{ 0,0,nullptr };

		int StartX = 0;
		int StartY = 0;
	}state;

	static void Draw()
	{
		if (state.workingNow == false)
		{
			return;
		}
		int cursorX = 0;
		int cursorY = 0;
		fastFuncts::GetGameTileCoordsWithCursor(cursorX, cursorY);
		for (auto& txt : state.possibleCoords)
		{
			MapTextDrawer::DrawingTextOnce(txt.PointText);
		}

		state.selectedCoord.PointText->xCoord = cursorX;
		state.selectedCoord.PointText->yCoord = cursorY;
		state.selectedCoord.PointText->zCoord = 0.2f;
		MapTextDrawer::DrawingTextOnce(state.selectedCoord.PointText);


		if ((ImGui::GetIO().MouseDownDuration[0] > 0.f)
			&& (ImGui::GetIO().MouseDownDurationPrev[0] == 0.f)
			)
		{
			auto routeSelected = std::find_if(begin(state.possibleCoords), end(state.possibleCoords), [&](coordsVText& txt)
				{
					if (txt.X == cursorX && txt.Y == cursorY)
					{
						return true;
					}
					return false;
				});
			if (routeSelected != std::end(state.possibleCoords))
			{
				try
				{
					RetreatRoute route(state.StartX, state.StartY, cursorX, cursorY);

					routes.data.emplace_back(route);
					ImGuiToast bMsg(ImGuiToastType_Success, 25000);

					bMsg.set_title("Retreat route");
					bMsg.set_content("Added route %d, %d", cursorX, cursorY);
					ImGui::InsertNotification(bMsg);
				}
				catch (std::exception& ex)
				{
					ImGuiToast bMsg(ImGuiToastType_Warning, 25000);

					bMsg.set_title("Retreat route");
					bMsg.set_content("Stop plan route, reason: %s", ex.what());
					ImGui::InsertNotification(bMsg);
				}
			}
			else
			{
				ImGuiToast bMsg(ImGuiToastType_Warning, 25000);

				bMsg.set_title("Retreat route");
				bMsg.set_content("Stop plan route");
				ImGui::InsertNotification(bMsg);
			}

			state.possibleCoords.clear();
			state.workingNow = false;
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
		graphicsExport::AddImGuiDrawCallback(Draw);


		void* font = MapTextDrawer::MakeTextFont("Times New Roman");
		if (font == nullptr)
		{
			MessageBoxA(NULL, "Cannot create text font for PlannedRetreatRoute selectedCoord", "ATTENTION! Exit now!", NULL);
			std::terminate();
		}

		state.selectedCoord.PointText = MapTextDrawer::MakeText(font, "+");
		MapTextDrawer::ChangeTextColor(state.selectedCoord.PointText, 255, 255, 0, 0);
		MapTextDrawer::DeleteTextFont(font);
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
		state.StartX = x;
		state.StartY = y;

		state.possibleCoords.clear();
		state.possibleCoords.reserve(state.maxPathLenInTiles * state.maxPathLenInTiles);

		float possibleMP = smallFuncs::GetMinimumPossibleMovepointsForArmy(army)*0.9;


		int coordsMod = 1;

		void* cashe = PathFinder::CreateCasheForDistances(x, y, 50);

		if (army->siege != nullptr)
		{
			if (army->siege->goal != nullptr)
			{
				state.possibleCoords.emplace_back(x, y, nullptr);
			}
		}

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
			if (state.maxPathLenInTiles < coordsMod)
			{
				break;
			}
			++coordsMod;
		} while (true);

		PathFinder::DeleteCasheForDistances(cashe);
		MakeTexts(state);

		state.workingNow = true;
	}
	void OnNewGameStart()
	{
		state.possibleCoords.clear();
		routes.data.clear();
	}
	void OnFactionTurnStart(factionStruct* fac)
	{
		if (fac == nullptr)
		{
			return;
		}

		routes.data.erase(std::remove_if(routes.data.begin(), routes.data.end(), [&](RetreatRoute& route)
			{
				if (route.FactionID == fac->dipNum)
				{
					return true;
				}
				//just in case, remove too old ones
				if (route.TurnNum > fastFuncts::getPassedTurnsNum() + 2)
				{
					return true;
				}

				return false;
			}), routes.data.end());
	}
	std::string OnGameSave()
	{
		std::string fPath = fastFuncts::GetModPath();


		fPath += "\\eopData\\Temp_PlannedRetreatRoute";
		filesystem::remove_all(fPath);
		filesystem::create_directory(fPath);

		std::string outFile = fPath;
		outFile += "\\RetreatRoutes.json";


		ofstream f1(outFile);
		jsn::json json = routes.data;
		f1 << setw(4) << json;
		f1.close();


		return outFile;
	}
	void OnGameLoad(const std::vector<std::string>& filePathes)
	{
		for (auto& path : filePathes)
		{
			if (path.find("RetreatRoutes.json") != string::npos)
			{
				jsn::json json2;
				try
				{

					std::ifstream f2(path);

					if (f2.is_open() == false)
					{
					}
					f2 >> json2;

					f2.close();
				}
				catch (jsn::json::parse_error& e)
				{
					MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}


				try
				{
					routes.data = json2.get<vector<RetreatRoute>>();
				}
				catch (jsn::json::exception& e)
				{
					MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}



				return;
			}
		}
	}
}