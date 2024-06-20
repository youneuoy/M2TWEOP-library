#include "PathMap.h"
#include "realGameTypes.h"
#include "fort.h"
#include "dataOffsets.h"
#include "smallFuncs.h"
#include <map>
#include "headersMEM.h"
#include "character.h"
#include "faction.h"
#include "characterRecord.h"
#include "army.h"
#include "strategyMap.h"

#include <mutex>
#include "ring_buffer.h"
namespace pathFinder
{
	PathMap::PathMap()
	{
		Pather = nullptr;
	}

	PathMap::PathMap(int xCenter, int yCenter, int radius) :PathMap()
	{
		Diameter = radius * 2;
		Pather = new MicroPather(this, (Diameter * Diameter), Diameter);

		if (IsCoordsValid(xCenter, yCenter) == false)
		{
			return;
		}
		XCenter = xCenter - radius;
		YCenter = yCenter - radius;

		StateMap.resize(Diameter * Diameter + Diameter + 1);
		for (int x = 0; x <= Diameter; ++x)
		{
			for (int y = 0; y <= Diameter; ++y)
			{
				if (IsCoordsValid(x + XCenter, y + YCenter))
				{
					StateMap[x * Diameter + y] = PathNode(x + XCenter, y + YCenter, 1, true);
				}
			}
		}
	}
	PathMap::PathMap(armyStruct* army, int radius) :PathMap()
	{
		Diameter = radius * 2;
		Pather = new MicroPather(this, (Diameter * Diameter), Diameter);


		if (army == nullptr)
		{
			return;
		}
		int xCenter = -1;
		int yCenter = -1;
		if (army->gen != nullptr)
		{
			xCenter = army->gen->xCoord;
			yCenter = army->gen->yCoord;
		}
		if (army->settlement != nullptr)
		{
			xCenter = army->settlement->xCoord;
			yCenter = army->settlement->yCoord;
		}

		if (IsCoordsValid(xCenter, yCenter) == false)
		{
			return;
		}

		bool isAtLand = stratMapHelpers::getTile(xCenter, yCenter)->isLand;


		std::map<strategyGroundType, bool> pathableGround = {
			{strategyGroundType::lowFertility, isAtLand == true},
			{strategyGroundType::mediumFertility, isAtLand == true},
			{strategyGroundType::highFertility, isAtLand == true},
			{strategyGroundType::wilderness, isAtLand == true},
			{strategyGroundType::highMountains, false},
			{strategyGroundType::lowMountains, false},
			{strategyGroundType::hills, isAtLand == true},
			{strategyGroundType::denseForest, false},
			{strategyGroundType::woodland,  isAtLand == true},
			{strategyGroundType::swamp,  isAtLand == true},
			{strategyGroundType::ocean,  isAtLand == false},
			{strategyGroundType::deepSea,  isAtLand == false},
			{strategyGroundType::shallowSea,  isAtLand == false},
			{strategyGroundType::coast,  isAtLand == false},
			{strategyGroundType::impassableLand, false},
			{strategyGroundType::impassableSea, false}
		};



		auto isAcceptableTile = [&](int xDest, int yDest)
		{
			if (xDest == xCenter && yDest == yCenter)
			{
				return true;
			}
			auto prohibitNeigbourTiles = [&]()
			{
				auto prohibitTile = [&](int x, int y)
				{
					int idx = (int)GetState(x, y);
					if (idx == -1)
					{
						return;
					}

					StateMap[idx] = PathNode(0, 0, 0, true);
				};
				prohibitTile(xDest - 1, yDest - 1);
				prohibitTile(xDest, yDest - 1);
				prohibitTile(xDest + 1, yDest - 1);

				prohibitTile(xDest - 1, yDest + 1);
				prohibitTile(xDest, yDest + 1);
				prohibitTile(xDest + 1, yDest + 1);

				prohibitTile(xDest - 1, yDest);
				prohibitTile(xDest + 1, yDest);
			};

			auto* destDile = stratMapHelpers::getTile(xDest, yDest);

			if (destDile->isLand != isAtLand)
			{
				return false;
			}
			//river
			if (destDile->factionId > 0 && destDile->factionId & 0x60)
			{
				return false;
			}
			strategyGroundType currGround = strategyGroundType(destDile->groundType);


			if (destDile->object != nullptr)
			{

				void* endObj = stratMapHelpers::getMainStratObject(destDile->object);

				strategyObject objT = callVFunc<4, strategyObject>(endObj);
				switch (objT)
				{
				case strategyObject::floatingGeneral:
				{
					prohibitNeigbourTiles();
					return false;
					break;
				}
				case strategyObject::character:
				{
					character* gen = (character*)endObj;
					if (gen->characterRecord->faction->factionID == army->faction->factionID)
					{
						return true;
					}
					else
					{
						if (gen->genType->type == 6 || gen->genType->type == 7 || gen->genType->type == 3)
						{
							//prohibitNeigbourTiles();
							return false;
						}
						else
						{
							return true;
						}
					}
					break;
				}
				case strategyObject::settlement:
				{
					settlementStruct* set = (settlementStruct*)endObj;

					if (set->faction->factionID == army->faction->factionID)
					{
						return true;
					}
					else
					{
						prohibitNeigbourTiles();
						return false;
					}
					break;
				}
				case strategyObject::fort:
				{
					fortStruct* fort = (fortStruct*)endObj;

					if (fort->faction->factionID == army->faction->factionID)
					{
						return true;
					}
					else
					{
						prohibitNeigbourTiles();
						return false;
					}
					break;
				}
				//for port and also for port dock
				//need check ground type etc
				case strategyObject::port:
					break;
				default:
					break;
				}
			}
			if (pathableGround[currGround] == true)
			{
				return true;
			}

			return false;
		};
		XCenter = xCenter - radius;
		YCenter = yCenter - radius;

		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);

		StateMap.resize(Diameter * Diameter + Diameter + 1);
		for (int x = 0; x <= Diameter; ++x)
		{
			for (int y = 0; y <= Diameter; ++y)
			{
				if (x + XCenter == 170 && y + YCenter == 266)
				{
					int i = 0;
				}
				if (IsCoordsValid(x + XCenter, y + YCenter) == false)
				{
					continue;
				}
				if (StateMap[x * Diameter + y].Inited == true)
				{
					continue;
				}

				if (isAcceptableTile(x + XCenter, y + YCenter))
				{
					StateMap[x * Diameter + y] = PathNode(x + XCenter, y + YCenter, 1, true);
				}
			}
		}


		int idx = (int)GetState(xCenter, yCenter);
		if (idx == -1)
		{
			return;
		}
		StateMap[idx] = PathNode(xCenter, yCenter, 1, true);
	}
	float PathMap::CalculateDistance(int x, int y, int destX, int destY)
	{
		MPVector<void*> path;

		float totalCost;
		int succ = Pather->Solve(GetState(x, y), GetState(destX, destY), &path, &totalCost);
		if (succ != 0)
		{
			totalCost = -1.f;
		}
		else
		{

		}
		return totalCost;
	}
	float PathMap::CalculateDistance(int x, int y, int destX, int destY, std::vector<std::pair<int, int>>& path)
	{
		MPVector<void*> gpath;

		float totalCost;
		int succ = Pather->Solve(GetState(x, y), GetState(destX, destY), &gpath, &totalCost);
		if (succ != 0)
		{
			totalCost = -1.f;
		}
		else
		{
			for (int i = 0; i < gpath.size(); ++i)
			{
				int idxInMap = (int)gpath[i];

				path.emplace_back(StateMap[idxInMap].X, StateMap[idxInMap].Y);
			}
		}
		return totalCost;
	}
	void PathMap::GetPossibleTilesForArmy(int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords)
	{
		const auto tile = stratMapHelpers::getTile(x, y);
		auto* army = tile->getArmy();
		if (army == nullptr)
			return;


		float possibleMP = smallFuncs::getMinimumMovePointsForArmy(army);

		//bitset wrapper, 1 bit per value
		std::vector<bool>visited;
		visited.resize(StateMap.size(), false);
		auto checkDone = [&](int xDest, int yDest)
		{
			//enabling this give very very little performance win
			//but give wrong result - we skipped some tiles with high cost
			return false;
			if (xDest == 406 && yDest == 245)
			{
				int i = 0;
			}
			int idx = (int)GetState(xDest, yDest);
			if (idx == -1)
			{
				return false;
			}

			if (visited[idx] == true)
			{
				return true;
			}
			return false;
		};

		int coordsMod = Diameter / 2;
		do
		{
			int currCoordsMod = coordsMod;
			do
			{
				bool isAdded = false;

				int xMin = x - currCoordsMod;
				int xMax = x + currCoordsMod;
				int yMin = y - currCoordsMod;
				int yMax = y + currCoordsMod;

				for (int yDest = yMin, xDest = xMin; yDest < yMax; ++yDest)
				{
					if (checkDone(xDest, yDest) == true)
					{
						continue;
					}

					std::vector<std::pair<int, int>> currPath;
					float distance = CalculateDistance(x, y, xDest, yDest, currPath);
					if (distance >= 0 && distance < possibleMP)
					{
						isAdded = true;
						visited[(int)GetState(xDest, yDest)] = true;
						for (auto& pathP : currPath)
						{
							visited[(int)GetState(pathP.first, pathP.second)] = true;
							possibleCoords.emplace(pathP.first, pathP.second);
						}
					}
				}

				for (int yDest = yMin, xDest = xMax; yDest <= yMax; ++yDest)
				{
					if (checkDone(xDest, yDest) == true)
					{
						continue;
					}

					std::vector<std::pair<int, int>> currPath;
					float distance = CalculateDistance(x, y, xDest, yDest, currPath);
					if (distance >= 0 && distance < possibleMP)
					{
						isAdded = true;
						visited[(int)GetState(xDest, yDest)] = true;
						for (auto& pathP : currPath)
						{
							visited[(int)GetState(pathP.first, pathP.second)] = true;
							possibleCoords.emplace(pathP.first, pathP.second);
						}
					}
				}


				for (int xDest = xMin + 1, yDest = yMin; xDest < xMax; ++xDest)
				{
					if (checkDone(xDest, yDest) == true)
					{
						continue;
					}

					std::vector<std::pair<int, int>> currPath;
					float distance = CalculateDistance(x, y, xDest, yDest, currPath);
					if (distance >= 0 && distance < possibleMP)
					{
						isAdded = true;
						visited[(int)GetState(xDest, yDest)] = true;
						for (auto& pathP : currPath)
						{
							visited[(int)GetState(pathP.first, pathP.second)] = true;
							possibleCoords.emplace(pathP.first, pathP.second);
						}
					}
				}

				for (int xDest = xMin, yDest = yMax; xDest < xMax; ++xDest)
				{
					if (checkDone(xDest, yDest) == true)
					{
						continue;
					}

					std::vector<std::pair<int, int>> currPath;
					float distance = CalculateDistance(x, y, xDest, yDest, currPath);
					if (distance >= 0 && distance < possibleMP)
					{
						isAdded = true;
						visited[(int)GetState(xDest, yDest)] = true;
						for (auto& pathP : currPath)
						{
							visited[(int)GetState(pathP.first, pathP.second)] = true;
							possibleCoords.emplace(pathP.first, pathP.second);
						}
					}
				}
				if (isAdded == false)
				{
					break;
				}
				++currCoordsMod;
			} while (currCoordsMod <= coordsMod * 2);


			coordsMod = coordsMod / 2;
		} while (coordsMod != 0);
	}

	std::pair<int, int> PathMap::GetNearestTileForArmy(int x, int y, int destx, int desty)
	{
		std::unordered_set<std::pair<int, int>, pathFinder::pathPairHash> possibleCoords;
		GetPossibleTilesForArmy(x, y, possibleCoords);

		std::pair<int, int> res = std::make_pair(x, y);

		float minResDistance = FLT_MAX;
		for (auto& coord : possibleCoords)
		{
			float dist = smallFuncs::getDistanceInTiles(destx, desty, coord.first, coord.second);
			if (dist < minResDistance)
			{
				res = coord;
				minResDistance = dist;
			}
		}
		return res;
	}

	std::pair<int, int> PathMap::GetSafestTileForArmy(armyStruct* army)
	{

		int x = 0;
		int y = 0;
		if (army->gen != nullptr)
		{
			x = army->gen->xCoord;
			y = army->gen->yCoord;
		}
		else if (army->settlement != nullptr)
		{
			x = army->settlement->xCoord;
			y = army->settlement->yCoord;
		}
		else
		{
			MessageBoxA(NULL, "GetSafestTileForArmy error", "ERROR", NULL);
		}
		auto evaluateSafety = [&](int evX, int evY)
		{
			int safetyCost = 0;
			auto* destDile = stratMapHelpers::getTile(evX, evY);

			strategyGroundType currGround = strategyGroundType(destDile->groundType);

			if (destDile->object != nullptr)
			{
				void* endObj = stratMapHelpers::getMainStratObject(destDile->object);

				strategyObject objT = callVFunc<4, strategyObject>(endObj);

				switch (objT)
				{
				case strategyObject::floatingGeneral:
					break;
				case strategyObject::settlement:
				{
					settlementStruct* set = (settlementStruct*)endObj;

					if (set->faction->factionID == army->faction->factionID)
					{
						if (set->army == nullptr)
						{
							safetyCost += 999;
						}
						else
						{
							if (set->army->numOfUnits + army->numOfUnits <= 20)
							{
								safetyCost += 599;
							}
						}
					}
					break;
				}
				case strategyObject::fort:
				{
					fortStruct* fort = (fortStruct*)endObj;

					if (fort->faction->factionID == army->faction->factionID)
					{
						if (fort->army == nullptr)
						{
							safetyCost += 899;
						}
						else
						{
							if (fort->army->numOfUnits + army->numOfUnits <= 20)
							{
								safetyCost += 499;
							}
						}
					}
					break;
				}
				case strategyObject::port:
					break;
				case strategyObject::character:
					break;
				case strategyObject::rallyPointSundry:
					break;
				default:
					break;
				}
			}
			switch (currGround)
			{
			case strategyGroundType::lowFertility:
				break;
			case strategyGroundType::mediumFertility:
				break;
			case strategyGroundType::highFertility:
				break;
			case strategyGroundType::wilderness:
				break;
			case strategyGroundType::highMountains:
				break;
			case strategyGroundType::lowMountains:
				break;
			case strategyGroundType::hills:
				break;
			case strategyGroundType::denseForest:
				break;
			case strategyGroundType::woodland:
			{
				//we can hide army here
				safetyCost += 199;
				break;
			}

			case strategyGroundType::swamp:
				break;
			case strategyGroundType::ocean:
				break;
			case strategyGroundType::deepSea:
				break;
			case strategyGroundType::shallowSea:
				break;
			case strategyGroundType::coast:
				break;
			case strategyGroundType::impassableLand:
				break;
			case strategyGroundType::impassableSea:
				break;
			default:
				break;
			}

			//prefer long distances
			safetyCost += smallFuncs::getDistanceInTiles(x, y, evX, evY);
			return safetyCost;
		};


		std::unordered_set<std::pair<int, int>, pathFinder::pathPairHash> possibleCoords;
		GetPossibleTilesForArmy(x, y, possibleCoords);
		std::pair<int, int> res = std::make_pair(x, y);
		int currSafety = -1;

		ring_buffer<std::pair<int, int>> bestOnes(5);
		for (auto& coord : possibleCoords)
		{
			int safety = evaluateSafety(coord.first, coord.second);
			if (safety > currSafety)
			{
				bestOnes.push_back(coord);
				currSafety = safety;
			}
		}

		if (bestOnes.size() > 0)
		{
			int rnd = std::rand() % bestOnes.size();
			res = bestOnes.at(rnd);
		}

		return res;
	}

	void* PathMap::GetState(int x, int y)
	{
		if (x < 0 || y < 0)
		{
			return (void*)-1;
		}
		if (x - XCenter < 0 || y - YCenter < 0)
		{
			return (void*)-1;
		}
		int idx = (x - XCenter) * Diameter + y - YCenter;
		if (idx >= StateMap.size())
		{
			return (void*)-1;
		}

		return (void*)idx;
		//return (void*)(y * Diameter + x);
		int res;
		//auto f = StateMap.find(idx);
		//if (f == StateMap.end())
		//{
		//	res = -1;
		//}
		//else
		//{
		//	res = f->first;
		//}

		//return (void*)res;
	}

	float PathMap::LeastCostEstimate(void* stateStart, void* stateEnd)
	{
		if ((int)stateStart == -1)
		{
			return 9999999.f;
		}
		if ((int)stateEnd == -1)
		{
			return 9999999.f;
		}

		auto& statenodeF = StateMap[(int)stateStart];
		auto& statenodeE = StateMap[(int)stateEnd];
		return smallFuncs::getDistanceInTiles(statenodeF.X, statenodeF.Y, statenodeE.X, statenodeE.Y);
	}

	void PathMap::AdjOne(int x, int y, int currX, int currY, MP_VECTOR<micropather::StateCost>*& adjacent)
	{
		if (IsCoordsValid(currX, currY) && IsSameTypeOfGround(x, y, currX, currY))
		{
			void* statEn = GetState(currX, currY);
			if ((int)statEn == -1)
			{
				return;
			}
			if (StateMap[(int)statEn].W == 0)
			{
				return;
			}

			float distance = stratMapHelpers::getTileMoveCost(x, y, currX, currY);
			if (distance >= 0)
			{
				StateCost nodeCost = { statEn, distance };
				adjacent->push_back(nodeCost);
			}
		}
	}
	void PathMap::AdjacentCost(void* state, MP_VECTOR<micropather::StateCost>* adjacent)
	{

		if ((int)state == -1)
		{
			return;
		}
		auto& statenode = StateMap[(int)state];
		int x = statenode.X;
		int y = statenode.Y;

		AdjOne(x, y, x - 1, y - 1, adjacent);
		AdjOne(x, y, x, y - 1, adjacent);
		AdjOne(x, y, x + 1, y - 1, adjacent);

		AdjOne(x, y, x - 1, y + 1, adjacent);
		AdjOne(x, y, x, y + 1, adjacent);
		AdjOne(x, y, x + 1, y + 1, adjacent);

		AdjOne(x, y, x - 1, y, adjacent);
		AdjOne(x, y, x + 1, y, adjacent);
	}
	void PathMap::PrintStateInfo(void* state)
	{
		//empty
	}
	bool PathMap::IsCoordsValid(int x, int y)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		if (x >= gameDataAll->stratMap->mapWidth)
		{
			return false;
		}
		if (y >= gameDataAll->stratMap->mapHeight)
		{
			return false;
		}
		if (x < 0)
		{
			return false;
		}
		if (y < 0)
		{
			return false;
		}

		return true;
	}
	bool PathMap::IsSameTypeOfGround(int x, int y, int destX, int destY)
	{
		auto* fTile = stratMapHelpers::getTile(x, y);
		auto* nTile = stratMapHelpers::getTile(destX, destY);
		return fTile->isLand == nTile->isLand;
	}
	void getPossibleTilesForArmyFromCache(void* cashe, int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);

		pathMap->GetPossibleTilesForArmy(x, y, possibleCoords);
	}
	std::pair<int, int> GetNearestTileForArmyFromCashe(void* cashe, int x, int y, int destx, int desty)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);

		return pathMap->GetNearestTileForArmy(x, y, destx, desty);
	}
	std::pair<int, int> getSafestTileForArmyFromCache(void* cashe, armyStruct* army)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);

		return pathMap->GetSafestTileForArmy(army);
	}
	void* createCacheForArmy(armyStruct* army, int radius)
	{
		PathMap* pathMap = new PathMap(army, radius);

		return pathMap;
	}
	void* CreateCasheForDistances(int x, int y, int radius)
	{
		PathMap* pathMap = new PathMap(x, y, radius);

		return pathMap;
	}
	void deleteCacheForDistances(void* cashe)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);
		delete pathMap;
	}
	float GetMovepointsForReachTileFromCashe(void* cashe, int x, int y, int destX, int destY)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);
		return pathMap->CalculateDistance(x, y, destX, destY);
	}
};