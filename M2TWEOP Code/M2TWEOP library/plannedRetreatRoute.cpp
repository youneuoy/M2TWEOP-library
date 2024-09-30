#include "pch.h"
#include "plannedRetreatRoute.h"
#include "graphicsD3D.h"
#include "mapTextDrawer.h"
#include "character.h"
#include "faction.h"

#include "pathMap.h"
#include "imgui_notify.h"

#include "gameHelpers.h"
#include "retreater.h"
#include "strategyMap.h"

using namespace mapTextDrawer;
namespace plannedRetreatRoute
{
	struct
	{
		vector<retreatRoute>data;
	}ROUTES;
	
	struct stateS
	{
		bool workingNow = false;

		vector<coordsVText>possibleCoords;
		int maxPathLenInTiles = 15;

		coordsVText selectedCoord{ 0,0,nullptr };

		int startX = 0;
		int startY = 0;
	}STATE;

	namespace
	{
		void draw()
		{
			if (STATE.workingNow == false)
				return;
			int cursorX = 0;
			int cursorY = 0;
			stratMapHelpers::getGameTileCoordsWithCursor(cursorX, cursorY);
			for (const auto& txt : STATE.possibleCoords)
			{
				drawingTextOnce(txt.PointText);
			}
			STATE.selectedCoord.PointText->xCoord = static_cast<float>(cursorX);
			STATE.selectedCoord.PointText->yCoord = static_cast<float>(cursorY);
			STATE.selectedCoord.PointText->zCoord = 0.2f;
			drawingTextOnce(STATE.selectedCoord.PointText);
		}
		
		void makeTexts(stateS& st)
		{
			void* font = mapTextDrawer::makeTextFont("Times New Roman", 400, 1);
			if (font == nullptr)
			{
				MessageBoxA(nullptr, "Cannot create text font for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
				std::terminate();
			}
			for (auto& txt : STATE.possibleCoords)
			{
				txt.PointText = makeText(font, "+");
				if (txt.PointText == nullptr)
				{
					MessageBoxA(nullptr, "Cannot create text for PlannedRetreatRoute", "ATTENTION! Exit now!", NULL);
					std::terminate();
				}
				txt.PointText->xCoord = static_cast<float>(txt.X);
				txt.PointText->yCoord = static_cast<float>(txt.Y);
				txt.PointText->zCoord = 0.2f;
			}
			mapTextDrawer::deleteTextFont(font);
		}
		
		void tryInit()
		{
			graphicsExport::AddImGuiDrawCallback(draw);
			void* font = mapTextDrawer::makeTextFont("Times New Roman", 400, 1);
			if (font == nullptr)
			{
				MessageBoxA(nullptr, "Cannot create text font for PlannedRetreatRoute selectedCoord", "ATTENTION! Exit now!", NULL);
				std::terminate();
			}
			STATE.selectedCoord.PointText = mapTextDrawer::makeText(font, "+");
			mapTextDrawer::changeTextColor(STATE.selectedCoord.PointText, 255, 255, 0, 0);
			mapTextDrawer::deleteTextFont(font);
		}
	}

	void startWork(const int x, const int y)
	{
		static std::once_flag initFlag;
		std::call_once(initFlag, tryInit);

		const auto tile = stratMapHelpers::getTile(x, y);
		const auto* army = tile->getArmy(false);
		if (army == nullptr)
			return;
		STATE.startX = x;
		STATE.startY = y;

		STATE.possibleCoords.clear();
		STATE.possibleCoords.reserve(STATE.maxPathLenInTiles * STATE.maxPathLenInTiles);
		
		void* cache = pathFinder::createCacheForArmy(army, 50);
		std::unordered_set<std::pair<int, int>, pathFinder::pathPairHash> testCoords;
		pathFinder::getPossibleTilesForArmyFromCache(cache, x, y, testCoords);
		pathFinder::deleteCacheForDistances(cache);

		for (const auto& [xCoord, yCoord] : testCoords)
		{
			STATE.possibleCoords.emplace_back(xCoord, yCoord, nullptr);
		}
		makeTexts(STATE);

		STATE.workingNow = true;
	}
	
	void onNewGameStart()
	{
		STATE.possibleCoords.clear();
		ROUTES.data.clear();
	}
	
	void onFactionTurnStart(const factionStruct* fac)
	{
		if (fac == nullptr)
			return;
		ROUTES.data.erase(std::remove_if(ROUTES.data.begin(), ROUTES.data.end(), [&](const retreatRoute& route)
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
			}), ROUTES.data.end());
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
		const jsn::json json = ROUTES.data;
		f1 << setw(4) << json;
		f1.close();
		return outFile;
	}
	
	void onGameLoad(const std::vector<std::string>& filePaths)
	{
		for (auto& path : filePaths)
		{
			if (path.find("RetreatRoutes.json") != string::npos)
			{
				jsn::json json2;
				try
				{
					std::ifstream f2(path);
					f2 >> json2;
					f2.close();
				}
				catch (jsn::json::parse_error& e)
				{
					MessageBoxA(nullptr, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}
				try
				{
					ROUTES.data = json2.get<vector<retreatRoute>>();
				}
				catch (jsn::json::exception& e)
				{
					MessageBoxA(nullptr, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}
				return;
			}
		}
	}

	void removeRoutesWithCoords(const int x, const int y)
	{
		ROUTES.data.erase(std::remove_if(ROUTES.data.begin(), ROUTES.data.end(), [&](const retreatRoute& route)
			{
				if (route.routeStart.x == x && route.routeStart.y == y)
				{
					return true;
				}
				return false;
			}), ROUTES.data.end());
	}

	retreatRoute* getRouteWithCoords(const int x, const int y)
	{
		const auto res = std::find_if(ROUTES.data.begin(), ROUTES.data.end(), [&](const retreatRoute& route)
		{
			if (route.routeStart.x == x && route.routeStart.y == y)
			{
				return true;
			}

			return false;
		});

		if (res == ROUTES.data.end())
		{
			return nullptr;
		}
		return &(*res);
	}
	void onClickAtTile(int x, int y)
	{
		if (STATE.workingNow == false)
		{
			return;
		}
		if (const auto routeSelected = std::find_if(begin(STATE.possibleCoords),
			end(STATE.possibleCoords), [&](const coordsVText& txt)
		{
			if (txt.X == x && txt.Y == y)
			{
				return true;
			}
			return false;
		}); routeSelected != std::end(STATE.possibleCoords))
		{
			try
			{
				retreatRoute route(STATE.startX, STATE.startY, x, y);

				removeRoutesWithCoords(STATE.startX, STATE.startY);

				ROUTES.data.emplace_back(route);
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

		STATE.possibleCoords.clear();
		STATE.workingNow = false;
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
		constexpr float distance = 3.f;
		void* cache = pathFinder::createCacheForArmy(army.army, static_cast<int>(dist + distance));
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