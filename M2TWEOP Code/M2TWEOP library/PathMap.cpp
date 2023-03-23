#include "PathMap.h"
#include "realGameTypes.h"
#include "dataOffsets.h"
#include "fastFuncts.h"

namespace PathFinder
{
	PathMap::PathMap(int xCenter, int yCenter, int radius)
	{

		Pather = new MicroPather(this, radius);
		Diameter = radius * 2;
		if (IsCoordsValid(xCenter, yCenter) == false)
		{
			return;
		}
		StateMap[(yCenter) * Diameter + xCenter] = PathNode(xCenter, yCenter, 1);
		int coordsMod = 1;
		do
		{
			int xMin = xCenter - coordsMod;
			int xMax = xCenter + coordsMod;
			int yMin = yCenter - coordsMod;
			int yMax = yCenter + coordsMod;


			for (int yDest = yMin, xDest = xMin; yDest < yMax; ++yDest)
			{
				if (IsCoordsValid(xDest, yDest))
				{
					StateMap[yDest * Diameter + xDest] = PathNode(xDest, yDest, 1);
				}
			}

			for (int yDest = yMin, xDest = xMax; yDest <= yMax; ++yDest)
			{
				if (IsCoordsValid(xDest, yDest))
				{
					StateMap[yDest * Diameter + xDest] = PathNode(xDest, yDest, 1);
				}
			}


			for (int xDest = xMin+1, yDest = yMin; xDest < xMax; ++xDest)
			{
				if (IsCoordsValid(xDest, yDest))
				{
					StateMap[yDest * Diameter + xDest] = PathNode(xDest, yDest, 1);
				}
			}

			for (int xDest = xMin, yDest = yMax; xDest < xMax; ++xDest)
			{
				if (IsCoordsValid(xDest, yDest))
				{
					StateMap[yDest * Diameter + xDest] = PathNode(xDest, yDest, 1);
				}
			}


			++coordsMod;
		} while (coordsMod <= radius);
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

		return totalCost;
	}
	void* PathMap::GetState(int x, int y)
	{
		return (void*)(y * Diameter + x);
		/*int res;
		auto f = StateMap.find(y * Diameter + x);
		if (f == StateMap.end())
		{
			res = -1;
		}
		else
		{
			res = f->first;
		}

		return (void*)res;*/
	}

	float PathMap::LeastCostEstimate(void* stateStart, void* stateEnd)
	{
		//do it later
		return 99999.f;
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

		int coordsMod = 1;

		int xMin = x - coordsMod;
		int xMax = x + coordsMod;
		int yMin = y - coordsMod;
		int yMax = y + coordsMod;


		for (int yDest = yMin, xDest = xMin; yDest < yMax; ++yDest)
		{
			if (IsCoordsValid(xDest, yDest) && IsSameTypeOfGround(x, y, xDest, yDest))
			{
				float distance = fastFuncts::GetMovepointsForReachNearTile(x, y, xDest, yDest);
				StateCost nodeCost = { GetState(xDest, yDest), distance };
				adjacent->push_back(nodeCost);
			}
		}

		for (int yDest = yMin, xDest = xMax; yDest <= yMax; ++yDest)
		{
			if (IsCoordsValid(xDest, yDest) && IsSameTypeOfGround(x, y, xDest, yDest))
			{
				float distance = fastFuncts::GetMovepointsForReachNearTile(x, y, xDest, yDest);
				StateCost nodeCost = { GetState(xDest, yDest), distance };
				adjacent->push_back(nodeCost);
			}
		}


		for (int xDest = xMin + 1, yDest = yMin; xDest < xMax; ++xDest)
		{
			if (IsCoordsValid(xDest, yDest) && IsSameTypeOfGround(x, y, xDest, yDest))
			{
				float distance = fastFuncts::GetMovepointsForReachNearTile(x, y, xDest, yDest);
				StateCost nodeCost = { GetState(xDest, yDest), distance };
				adjacent->push_back(nodeCost);
			}
		}

		for (int xDest = xMin, yDest = yMax; xDest < xMax; ++xDest)
		{
			if (IsCoordsValid(xDest, yDest) && IsSameTypeOfGround(x, y, xDest, yDest))
			{
				float distance = fastFuncts::GetMovepointsForReachNearTile(x, y, xDest, yDest);
				StateCost nodeCost = { GetState(xDest, yDest), distance };
				adjacent->push_back(nodeCost);
			}
		}
	}
	void PathMap::PrintStateInfo(void* state)
	{
		//empty
	}
	bool PathMap::IsCoordsValid(int x, int y)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		if (x > gameDataAll->stratMap->mapWidth)
		{
			return false;
		}
		if (y > gameDataAll->stratMap->mapHeight)
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