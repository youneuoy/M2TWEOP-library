#include "PathMap.h"
#include "realGameTypes.h"
#include "dataOffsets.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include <map>
namespace PathFinder
{
	PathMap::PathMap(int xCenter, int yCenter, int radius)
	{
		Diameter = radius * 2;
		Pather = new MicroPather(this, (Diameter * Diameter), Diameter);

		if (IsCoordsValid(xCenter, yCenter) == false)
		{
			return;
		}
		XCenter = xCenter - radius;
		YCenter = yCenter - radius;

		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);

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
	PathMap::PathMap(stackStruct* army, int radius)
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
		enum class GroundType
		{
			low_fertility = 0,
			medium_fertility = 1,
			high_fertility = 2,
			wilderness = 3,
			high_moutains = 4,
			low_moutains = 5,
			hills = 6,
			dense_forest = 7,
			woodland = 8,
			swamp = 9,
			ocean = 10,
			deep_sea = 11,
			shallow_sea = 12,
			coast = 13,
			impassable_land = 14,
			impassable_sea = 15,
		};
		bool isAtLand = fastFuncts::getTileStruct(xCenter, yCenter)->isLand;


		std::map<GroundType, bool> pathableGround = {
			{GroundType::low_fertility, isAtLand == true},
			{GroundType::medium_fertility, isAtLand == true},
			{GroundType::high_fertility, isAtLand == true},
			{GroundType::wilderness, isAtLand == true},
			{GroundType::high_moutains, false},
			{GroundType::low_moutains, false},
			{GroundType::hills, isAtLand == true},
			{GroundType::dense_forest, false},
			{GroundType::woodland,  isAtLand == true},
			{GroundType::swamp,  isAtLand == true},
			{GroundType::ocean,  isAtLand == false},
			{GroundType::deep_sea,  isAtLand == false},
			{GroundType::shallow_sea,  isAtLand == false},
			{GroundType::coast,  isAtLand == false},
			{GroundType::impassable_land, false},
			{GroundType::impassable_sea, false}
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

			auto* destDile = fastFuncts::getTileStruct(xDest, yDest);

			if (destDile->isLand != isAtLand)
			{
				return false;
			}
			//river
			if (destDile->factionId > 0 && destDile->factionId & 0x60)
			{
				return false;
			}
			GroundType currGround = GroundType(destDile->groundType);


			if (destDile->object != nullptr)
			{

				void* endObj = smallFuncs::GetMainStratObject(destDile->object);

				StartMapObjectType objT = CallVFunc<4, StartMapObjectType>(endObj);
				switch (objT)
				{
				case StartMapObjectType::FloatingGeneral:
				{
					prohibitNeigbourTiles();
					return false;
					break;
				}
				case StartMapObjectType::Character:
				{
					general* gen = (general*)endObj;
					if (gen->genChar->faction->dipNum == army->faction->dipNum)
					{
						return true;
					}
					else
					{
						if (gen->genType->type == 6 || gen->genType->type == 7 || gen->genType->type == 3)
						{
							prohibitNeigbourTiles();
							return false;
						}
						else
						{
							return true;
						}
					}
					break;
				}
				case StartMapObjectType::Settlement:
				{
					settlementStruct* set = (settlementStruct*)endObj;

					if (set->ownerFac->dipNum == army->faction->dipNum)
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
				case StartMapObjectType::Fort:
				{
					fortStruct* fort = (fortStruct*)endObj;

					if (fort->faction->dipNum == army->faction->dipNum)
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
				case StartMapObjectType::Port:
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

		StateMap[XCenter * Diameter + YCenter] = PathNode(xCenter, yCenter, 1, true);
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
		if (destX == 203 && destY == 276)
		{
			int i = 0;
		}
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
		auto* army = fastFuncts::findArmy(x, y);
		if (army == nullptr)
		{
			return;
		}


		float possibleMP = smallFuncs::GetMinimumPossibleMovepointsForArmy(army);

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

		int dx = statenodeF.X - statenodeE.X;
		int dy = statenodeF.Y - statenodeF.Y;
		return (float)sqrt((double)(dx * dx) + (double)(dy * dy));
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

			float distance = fastFuncts::GetMovepointsForReachNearTile(x, y, currX, currY);
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
		auto* fTile = fastFuncts::getTileStruct(x, y);
		auto* nTile = fastFuncts::getTileStruct(destX, destY);
		return fTile->isLand == nTile->isLand;
	}
	void GetPossibleTilesForArmyFromCashe(void* cashe, int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);

		pathMap->GetPossibleTilesForArmy(x, y, possibleCoords);
	}
	NOINLINE EOP_EXPORT void* CreateCasheForArmy(stackStruct* army, int radius)
	{
		PathMap* pathMap = new PathMap(army, radius);

		return pathMap;
	}
	NOINLINE EOP_EXPORT void* CreateCasheForDistances(int x, int y, int radius)
	{
		PathMap* pathMap = new PathMap(x, y, radius);

		return pathMap;
	}
	NOINLINE EOP_EXPORT void DeleteCasheForDistances(void* cashe)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);
		delete pathMap;
	}
	NOINLINE EOP_EXPORT float GetMovepointsForReachTileFromCashe(void* cashe, int x, int y, int destX, int destY)
	{
		PathMap* pathMap = reinterpret_cast<PathMap*>(cashe);
		return pathMap->CalculateDistance(x, y, destX, destY);
	}
};