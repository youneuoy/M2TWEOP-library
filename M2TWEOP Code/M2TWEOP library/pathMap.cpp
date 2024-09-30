#include "pch.h"
#include "pathMap.h"
#include "realGameTypes.h"
#include "fort.h"
#include "dataOffsets.h"
#include "character.h"
#include "faction.h"
#include "characterRecord.h"
#include "army.h"
#include "strategyMap.h"
#include "ring_buffer.h"
namespace pathFinder
{
	pathMap::pathMap()
	{
		Pather = nullptr;
	}

	pathMap::pathMap(const int xCenter, const int yCenter, const int radius) :pathMap()
	{
		Diameter = radius * 2;
		Pather = new MicroPather(this, (Diameter * Diameter), Diameter);

		if (isCoordsValid(xCenter, yCenter) == false)
			return;
		
		XCenter = xCenter - radius;
		YCenter = yCenter - radius;
		StateMap.resize(Diameter * Diameter + Diameter + 1);
		for (size_t x = 0; x <= Diameter; ++x)
		{
			for (size_t y = 0; y <= Diameter; ++y)
			{
				if (isCoordsValid(x + XCenter, y + YCenter))
				{
					StateMap[x * Diameter + y] = PathNode(x + XCenter, y + YCenter, 1, true);
				}
			}
		}
	}
	
	pathMap::pathMap(const armyStruct* army, const int radius) :pathMap()
	{
		if (army == nullptr)
			return;
		
		Diameter = radius * 2;
		Pather = new MicroPather(this, (Diameter * Diameter), Diameter);
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

		if (isCoordsValid(xCenter, yCenter) == false)
			return;

		const bool isAtLand = stratMapHelpers::getTile(xCenter, yCenter)->isLand;
		std::map<strategyGroundType, bool> validGroundTypes = {
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
		
		auto isAcceptableTile = [&](const int xDest, const int yDest)
		{
			if (xDest == xCenter && yDest == yCenter)
			{
				return true;
			}
			auto prohibitNeighbourTiles = [&]()
			{
				auto prohibitTile = [&](const int x, const int y)
				{
					const int idx = reinterpret_cast<int>(getState(x, y));
					if (idx == -1)
						return;
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

			auto* destinationTile = stratMapHelpers::getTile(xDest, yDest);

			if (destinationTile->isLand != static_cast<int8_t>(isAtLand))
				return false;
			if (destinationTile->isRiver())
				return false;
			
			const auto currGround = static_cast<strategyGroundType>(destinationTile->groundType);
			if (destinationTile->object != nullptr)
			{
				switch (void* endObj = stratMapHelpers::getMainStratObject(destinationTile->object); callVFunc<4, strategyObject>(endObj))
				{
				case strategyObject::floatingGeneral:
				{
					prohibitNeighbourTiles();
					return false;
				}
				case strategyObject::character:
				{
					if (const auto gen = static_cast<character*>(endObj);
						gen->characterRecord->faction->factionID == army->faction->factionID)
					{
						return true;
					}
					else
					{
						if (gen->genType->type == characterTypeStrat::general
							|| gen->genType->type == characterTypeStrat::namedCharacter
							|| gen->genType->type == characterTypeStrat::admiral)
						{
							return false;
						}
						return true;
					}
				}
				case strategyObject::settlement:
				{
					if (const auto sett = static_cast<settlementStruct*>(endObj);
						sett->faction->factionID == army->faction->factionID)
					{
						return true;
					}
					prohibitNeighbourTiles();
					return false;
				}
				case strategyObject::fort:
				{
					if (const auto fort = static_cast<fortStruct*>(endObj);
						fort->faction->factionID == army->faction->factionID)
					{
						return true;
					}
					prohibitNeighbourTiles();
					return false;
				}
				//for port and also for port dock
				//need check ground type etc
				case strategyObject::port:
				case strategyObject::watchtower:
				case strategyObject::sundry:
				case strategyObject::rallyPointSundry:
				case strategyObject::battleSiteMarker:
					break;
				}
			}
			if (validGroundTypes[currGround] == true)
			{
				return true;
			}

			return false;
		};
		XCenter = xCenter - radius;
		YCenter = yCenter - radius;

		StateMap.resize(Diameter * Diameter + Diameter + 1);
		for (size_t x = 0; x <= Diameter; ++x)
		{
			for (size_t y = 0; y <= Diameter; ++y)
			{
				if (isCoordsValid(x + XCenter, y + YCenter) == false)
					continue;
				if (StateMap[x * Diameter + y].Inited == true)
					continue;
				if (isAcceptableTile(x + XCenter, y + YCenter))
					StateMap[x * Diameter + y] = PathNode(x + XCenter, y + YCenter, 1, true);
			}
		}
		const int idx = reinterpret_cast<int>(getState(xCenter, yCenter));
		if (idx == -1)
			return;
		StateMap[idx] = PathNode(xCenter, yCenter, 1, true);
	}
	
	float pathMap::calculateDistance(const int x, const int y, const int destX, const int destY)
	{
		MPVector<void*> path;
		float totalCost;
		if (Pather->Solve(getState(x, y), getState(destX, destY), &path, &totalCost) != 0)
			totalCost = -1.f;
		return totalCost;
	}
	
	float pathMap::calculateDistance(const int x, const int y, const int destX, const int destY, std::vector<std::pair<int, int>>& path)
	{
		MPVector<void*> gpath;
		float totalCost;
		if (Pather->Solve(getState(x, y), getState(destX, destY), &gpath, &totalCost) != 0)
			totalCost = -1.f;
		else
		{
			for (uint32_t i = 0; i < gpath.size(); ++i)
			{
				const int idxInMap = reinterpret_cast<int>(gpath[i]);
				path.emplace_back(StateMap[idxInMap].X, StateMap[idxInMap].Y);
			}
		}
		return totalCost;
	}
	
	void pathMap::getPossibleTilesForArmy(const int x, const int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords)
	{
		const auto tile = stratMapHelpers::getTile(x, y);
		const auto* army = tile->getArmy(false);
		if (army == nullptr)
			return;
		const float possibleMp = armyHelpers::getMinimumMovePointsForArmy(army);
		//bitset wrapper, 1 bit per value
		std::vector<bool>visited;
		visited.resize(StateMap.size(), false);
		int coordsMod = Diameter / 2;
		do
		{
			int currCoordsMod = coordsMod;
			do
			{
				bool isAdded = false;

				const int xMin = x - currCoordsMod;
				const int xMax = x + currCoordsMod;
				const int yMin = y - currCoordsMod;
				const int yMax = y + currCoordsMod;
				for (int yDest = yMin, xDest = xMin; yDest < yMax; ++yDest)
				{
					std::vector<std::pair<int, int>> currPath;
					if (const float distance = calculateDistance(x, y, xDest, yDest, currPath); distance >= 0 && distance < possibleMp)
					{
						isAdded = true;
						visited[reinterpret_cast<int>(getState(xDest, yDest))] = true;
						for (auto& [fst, snd] : currPath)
						{
							visited[reinterpret_cast<int>(getState(fst, snd))] = true;
							possibleCoords.emplace(fst, snd);
						}
					}
				}

				for (int yDest = yMin, xDest = xMax; yDest <= yMax; ++yDest)
				{
					std::vector<std::pair<int, int>> currPath;
					if (const float distance = calculateDistance(x, y, xDest, yDest, currPath); distance >= 0 && distance < possibleMp)
					{
						isAdded = true;
						visited[reinterpret_cast<int>(getState(xDest, yDest))] = true;
						for (auto& [fst, snd] : currPath)
						{
							visited[reinterpret_cast<int>(getState(fst, snd))] = true;
							possibleCoords.emplace(fst, snd);
						}
					}
				}


				for (int xDest = xMin + 1, yDest = yMin; xDest < xMax; ++xDest)
				{
					std::vector<std::pair<int, int>> currPath;
					if (const float distance = calculateDistance(x, y, xDest, yDest, currPath); distance >= 0 && distance < possibleMp)
					{
						isAdded = true;
						visited[reinterpret_cast<int>(getState(xDest, yDest))] = true;
						for (auto& [fst, snd] : currPath)
						{
							visited[reinterpret_cast<int>(getState(fst, snd))] = true;
							possibleCoords.emplace(fst, snd);
						}
					}
				}

				for (int xDest = xMin, yDest = yMax; xDest < xMax; ++xDest)
				{
					std::vector<std::pair<int, int>> currPath;
					if (const float distance = calculateDistance(x, y, xDest, yDest, currPath); distance >= 0 && distance < possibleMp)
					{
						isAdded = true;
						visited[reinterpret_cast<int>(getState(xDest, yDest))] = true;
						for (auto& [fst, snd] : currPath)
						{
							visited[reinterpret_cast<int>(getState(fst, snd))] = true;
							possibleCoords.emplace(fst, snd);
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

	std::pair<int, int> pathMap::getNearestTileForArmy(int x, int y, const int destX, const int destY)
	{
		std::unordered_set<std::pair<int, int>, pathFinder::pathPairHash> possibleCoords;
		getPossibleTilesForArmy(x, y, possibleCoords);

		std::pair<int, int> res = std::make_pair(x, y);

		float minResDistance = FLT_MAX;
		for (auto& coord : possibleCoords)
		{
			if (const float dist = stratMapHelpers::getDistanceInTiles(destX, destY, coord.first, coord.second);
				dist < minResDistance)
			{
				res = coord;
				minResDistance = dist;
			}
		}
		return res;
	}

	std::pair<int, int> pathMap::getSafestTileForArmy(const armyStruct* army)
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
			MessageBoxA(nullptr, "GetSafestTileForArmy error", "ERROR", NULL);
		}
		auto evaluateSafety = [&](const int evX, const int evY)
		{
			int safetyCost = 0;
			auto* destDile = stratMapHelpers::getTile(evX, evY);

			const auto currGround = static_cast<strategyGroundType>(destDile->groundType);

			if (destDile->object != nullptr)
			{
				switch (void* endObj = stratMapHelpers::getMainStratObject(destDile->object); callVFunc<4, strategyObject>(endObj))
				{
				case strategyObject::floatingGeneral:
					break;
				case strategyObject::settlement:
				{
					if (const auto sett = static_cast<settlementStruct*>(endObj); sett->faction->factionID == army->faction->factionID)
					{
						if (!sett->army)
							safetyCost += 999;
						else if (sett->army->numOfUnits + army->numOfUnits <= 20)
							safetyCost += 599;
					}
					break;
				}
				case strategyObject::fort:
				{
					if (const auto fort = static_cast<fortStruct*>(endObj); fort->faction->factionID == army->faction->factionID)
					{
						if (!fort->army)
							safetyCost += 899;
						else if (fort->army->numOfUnits + army->numOfUnits <= 20)
							safetyCost += 499;
					}
					break;
				}
				case strategyObject::port:
				case strategyObject::character:
				case strategyObject::rallyPointSundry:
				case strategyObject::watchtower:
				case strategyObject::sundry:
				case strategyObject::battleSiteMarker:
					break;
				}
			}
			switch (currGround)
			{
			case strategyGroundType::woodland:
			{
				//we can hide army here
				safetyCost += 199;
				break;
			}
			case strategyGroundType::lowFertility:
			case strategyGroundType::mediumFertility:
			case strategyGroundType::highFertility:
			case strategyGroundType::wilderness:
			case strategyGroundType::highMountains:
			case strategyGroundType::lowMountains:
			case strategyGroundType::hills:
			case strategyGroundType::denseForest:
			case strategyGroundType::swamp:
			case strategyGroundType::ocean:
			case strategyGroundType::deepSea:
			case strategyGroundType::shallowSea:
			case strategyGroundType::coast:
			case strategyGroundType::impassableLand:
			case strategyGroundType::impassableSea:
				break;
			}

			//prefer long distances
			safetyCost += static_cast<int>(stratMapHelpers::getDistanceInTiles(x, y, evX, evY));
			return safetyCost;
		};


		std::unordered_set<std::pair<int, int>, pathPairHash> possibleCoords;
		getPossibleTilesForArmy(x, y, possibleCoords);
		std::pair<int, int> res = std::make_pair(x, y);
		int currSafety = -1;

		ring_buffer<std::pair<int, int>> bestOnes(5);
		for (auto& coord : possibleCoords)
		{
			if (const int safety = evaluateSafety(coord.first, coord.second); safety > currSafety)
			{
				bestOnes.push_back(coord);
				currSafety = safety;
			}
		}

		if (bestOnes.size() > 0)
		{
			const int rnd = std::rand() % bestOnes.size();
			res = bestOnes.at(rnd);
		}

		return res;
	}

	void* pathMap::getState(const int x, const int y)
	{
		if (x < 0 || y < 0)
			return reinterpret_cast<void*>(-1);
		if (x - XCenter < 0 || y - YCenter < 0)
			return reinterpret_cast<void*>(-1);
		const uint32_t idx = (x - XCenter) * Diameter + y - YCenter;
		if (idx >= StateMap.size())
		{
			return reinterpret_cast<void*>(-1);
		}
		return reinterpret_cast<void*>(idx);
	}

	float pathMap::leastCostEstimate(void* stateStart, void* stateEnd)
	{
		if (reinterpret_cast<int>(stateStart) == -1)
			return 9999999.f;
		if (reinterpret_cast<int>(stateEnd) == -1)
			return 9999999.f;
		const auto& stateNodeF = StateMap[reinterpret_cast<int>(stateStart)];
		const auto& stateNodeE = StateMap[reinterpret_cast<int>(stateEnd)];
		return stratMapHelpers::getDistanceInTiles(stateNodeF.X, stateNodeF.Y, stateNodeE.X, stateNodeE.Y);
	}

	void pathMap::adjOne(const int x, const int y, const int currX, const int currY, MP_VECTOR<micropather::StateCost>*& adjacent)
	{
		if (isCoordsValid(currX, currY) && isSameTypeOfGround(x, y, currX, currY))
		{
			void* statEn = getState(currX, currY);
			
			if (reinterpret_cast<int>(statEn) == -1)
				return;
			if (StateMap[reinterpret_cast<int>(statEn)].W <= 0.0001f)
				return;
			
			if (const float distance = stratMapHelpers::getTileMoveCost(x, y, currX, currY); distance >= 0)
			{
				const StateCost nodeCost = { statEn, distance };
				adjacent->push_back(nodeCost);
			}
		}
	}
	
	void pathMap::adjacentCost(void* state, MP_VECTOR<micropather::StateCost>* adjacent)
	{
		if (reinterpret_cast<int>(state) == -1)
			return;
		
		const auto& stateNode = StateMap[reinterpret_cast<int>(state)];
		const int x = stateNode.X;
		const int y = stateNode.Y;
		adjOne(x, y, x - 1, y - 1, adjacent);
		adjOne(x, y, x, y - 1, adjacent);
		adjOne(x, y, x + 1, y - 1, adjacent);

		adjOne(x, y, x - 1, y + 1, adjacent);
		adjOne(x, y, x, y + 1, adjacent);
		adjOne(x, y, x + 1, y + 1, adjacent);

		adjOne(x, y, x - 1, y, adjacent);
		adjOne(x, y, x + 1, y, adjacent);
	}
	
	void pathMap::printStateInfo(void* state)
	{
		//empty
	}
	
	bool pathMap::isCoordsValid(const int x, const int y)
	{
		const auto sMap = stratMapHelpers::getStratMap();
		if (x >= sMap->mapWidth)
			return false;
		if (y >= sMap->mapHeight)
			return false;
		if (x < 0)
			return false;
		if (y < 0)
			return false;
		return true;
	}
	
	bool pathMap::isSameTypeOfGround(const int x, const int y, const int destX, const int destY)
	{
		const auto* fTile = stratMapHelpers::getTile(x, y);
		const auto* nTile = stratMapHelpers::getTile(destX, destY);
		return fTile->isLand == nTile->isLand;
	}
	
	void getPossibleTilesForArmyFromCache(void* cache, const int x, const int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords)
	{
		const auto map = static_cast<pathMap*>(cache);
		map->getPossibleTilesForArmy(x, y, possibleCoords);
	}
	
	std::pair<int, int> getNearestTileForArmyFromCache(void* cache, const int x, const int y, const int destX, const int destY)
	{
		const auto map = static_cast<pathMap*>(cache);
		return map->getNearestTileForArmy(x, y, destX, destY);
	}
	
	std::pair<int, int> getSafestTileForArmyFromCache(void* cache, const armyStruct* army)
	{
		const auto map = static_cast<pathMap*>(cache);
		return map->getSafestTileForArmy(army);
	}
	
	void* createCacheForArmy(const armyStruct* army, const int radius)
	{
		const auto map = new pathMap(army, radius);
		return map;
	}
	
	void* createCacheForDistances(const int x, const int y, const int radius)
	{
		const auto map = new pathMap(x, y, radius);
		return map;
	}
	
	void deleteCacheForDistances(void* cache)
	{
		const auto map = static_cast<pathMap*>(cache);
		delete map;
	}
	float getMovePointsForReachTileFromCache(void* cache, const int x, const int y, const int destX, const int destY)
	{
		const auto map = static_cast<pathMap*>(cache);
		return map->calculateDistance(x, y, destX, destY);
	}
};