#include "plannedRetreatRoute.h"
#include "graphicsD3D.h"
#include <iomanip>
#include <mutex>
#include "mapTextDrawer.h"
#include "character.h"
#include "faction.h"

#include "pathMap.h"
#include "imgui_notify.h"

#include <filesystem>

#include "gameHelpers.h"
#include "retreater.h"
#include "strategyMap.h"

using namespace mapTextDrawer;
namespace plannedRetreatRoute
{
	struct
	{
		vector<retreatRoute>data;
	}routes;
	
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
		stratMapHelpers::getGameTileCoordsWithCursor(cursorX, cursorY);
		for (auto& txt : state.possibleCoords)
		{
			drawingTextOnce(txt.PointText);
		}

		state.selectedCoord.PointText->xCoord = cursorX;
		state.selectedCoord.PointText->yCoord = cursorY;
		state.selectedCoord.PointText->zCoord = 0.2f;
		drawingTextOnce(state.selectedCoord.PointText);
	}
	static void makeTexts(stateS& st)
	{
		void* font = mapTextDrawer::makeTextFont("Times New Roman", 400, 1);
		if (font == nullptr)
		{
			MessageBoxA(NULL, "Cannot create text font for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
			std::terminate();
		}

		for (auto& txt : state.possibleCoords)
		{
			txt.PointText = mapTextDrawer::makeText(font, "+");
			if (txt.PointText == nullptr)
			{
				MessageBoxA(NULL, "Cannot create text for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
				std::terminate();
			}
			txt.PointText->xCoord = txt.X;
			txt.PointText->yCoord = txt.Y;
			txt.PointText->zCoord = 0.2f;
		}

		mapTextDrawer::deleteTextFont(font);
	}
	static void tryInit()
	{
		graphicsExport::AddImGuiDrawCallback(Draw);


		void* font = mapTextDrawer::makeTextFont("Times New Roman", 400, 1);
		if (font == nullptr)
		{
			MessageBoxA(NULL, "Cannot create text font for PlannedRetreatRoute selectedCoord", "ATTENTION! Exit now!", NULL);
			std::terminate();
		}

		state.selectedCoord.PointText = mapTextDrawer::makeText(font, "+");
		mapTextDrawer::ChangeTextColor(state.selectedCoord.PointText, 255, 255, 0, 0);
		mapTextDrawer::deleteTextFont(font);
	}

	void startWork(int x, int y)
	{
		static std::once_flag initFLAG;
		std::call_once(initFLAG, tryInit);

		const auto tile = stratMapHelpers::getTile(x, y);
		auto* army = tile->getArmy();
		if (army == nullptr)
			return;
		state.StartX = x;
		state.StartY = y;

		state.possibleCoords.clear();
		state.possibleCoords.reserve(state.maxPathLenInTiles * state.maxPathLenInTiles);

		float possibleMP = armyHelpers::getMinimumMovePointsForArmy(army) * 0.9;


		int coordsMod = 1;

		//void* cashe = PathFinder::CreateCasheForDistances(x, y, 50);
		void* cashe = pathFinder::createCacheForArmy(army, 50);


		std::unordered_set<std::pair<int, int>, pathFinder::pathPairHash> testCoords;
		pathFinder::getPossibleTilesForArmyFromCache(cashe, x, y, testCoords);
		pathFinder::deleteCacheForDistances(cashe);

		for (auto& coord : testCoords)
		{
			state.possibleCoords.emplace_back(coord.first, coord.second, nullptr);
		}
		makeTexts(state);

		state.workingNow = true;
		return;
	}
	void onNewGameStart()
	{
		state.possibleCoords.clear();
		routes.data.clear();
	}
	void onFactionTurnStart(factionStruct* fac)
	{
		if (fac == nullptr)
		{
			return;
		}

		routes.data.erase(std::remove_if(routes.data.begin(), routes.data.end(), [&](retreatRoute& route)
			{
				if (route.factionID == fac->factionID)
				{
					return true;
				}
				//just in case, remove too old ones
				if (route.turnNum > campaignHelpers::getCampaignData()->turnNumber + 2)
				{
					return true;
				}

				return false;
			}), routes.data.end());
	}
	std::string onGameSave()
	{
		std::string fPath = gameHelpers::getModPath();
		
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
	void onGameLoad(const std::vector<std::string>& filePathes)
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
					routes.data = json2.get<vector<retreatRoute>>();
				}
				catch (jsn::json::exception& e)
				{
					MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}
				return;
			}
		}
	}

	void removeRoutesWithCoords(int x, int y)
	{
		routes.data.erase(std::remove_if(routes.data.begin(), routes.data.end(), [&](retreatRoute& route)
			{
				if (route.routeStart.x == x && route.routeStart.y == y)
				{
					return true;
				}

				return false;
			}), routes.data.end());
	}

	retreatRoute* getRouteWithCoords(int x, int y)
	{
		auto res = std::find_if(routes.data.begin(), routes.data.end(), [&](retreatRoute& route)
			{
				if (route.routeStart.x == x && route.routeStart.y == y)
				{
					return true;
				}

				return false;
			});

		if (res == routes.data.end())
		{
			return nullptr;
		}
		return &(*res);
	}
	void onClickAtTile(int x, int y)
	{
		if (state.workingNow == false)
		{
			return;
		}
		auto routeSelected = std::find_if(begin(state.possibleCoords), end(state.possibleCoords), [&](coordsVText& txt)
			{
				if (txt.X == x && txt.Y == y)
				{
					return true;
				}
				return false;
			});
		if (routeSelected != std::end(state.possibleCoords))
		{
			try
			{
				retreatRoute route(state.StartX, state.StartY, x, y);

				removeRoutesWithCoords(state.StartX, state.StartY);

				routes.data.emplace_back(route);
				ImGuiToast bMsg(ImGuiToastType_Success, 25000);

				bMsg.set_title("Retreat route");
				bMsg.set_content("Added route %d, %d", x, y);
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

	bool tryRetreatArmyWithRoute(const battleArmy& army, std::pair<int, int>& resCoords)
	{
		auto* route = getRouteWithCoords(army.character->xCoord, army.character->yCoord);
		if (route == nullptr)
			return false;
		const int x = route->routeStart.x;
		const int y = route->routeStart.y;
		const int destX = route->routeEnd.x;
		const int destY = route->routeEnd.y;
		const float dist = stratMapHelpers::getDistanceInTiles(x, y, destX, destY);
		constexpr int distance = 3;
		void* cache = pathFinder::createCacheForArmy(army.army, dist + distance);
		const auto [xCoord, yCoord] = pathFinder::getNearestTileForArmyFromCache(cache, x, y, destX, destY);
		characterHelpers::teleportCharacter(army.character, xCoord, yCoord);
		pathFinder::deleteCacheForDistances(cache);

		resCoords = { route->routeEnd.x ,route->routeEnd.y };
		removeRoutesWithCoords(route->routeStart.x, route->routeStart.y);
		return true;
	}
	
	void onRetreat()
	{
		battleDataS* battle = battleHelpers::getBattleData();
		if (battle == nullptr)
			return;
		retreater retreater(battle);
		retreater.retreat();
	}
}