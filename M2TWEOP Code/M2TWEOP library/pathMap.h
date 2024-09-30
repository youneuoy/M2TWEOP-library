#pragma once
#include <unordered_set>
#include "micropather.h"

#include "realGameTypes.h"

#include <random>
struct armyStruct;
using namespace micropather;

namespace pathFinder
{
	struct pathPairHash {
		inline std::size_t operator()(const std::pair<int, int>& v) const {
			return v.first * 31 + v.second;
		}
	};
	
	class PathNode {
	public:
		int X = 0;
		int Y = 0;
		float W = 0;
		bool Inited = false;

		PathNode() = default;
		PathNode(int x, int y, float w, bool inited) : X(x), Y(y), W(w), Inited(inited) {}
	};
	class Hasher
	{
	public:
		size_t operator() (int const& key) const
		{
			return key;
		}
	};
	class EqualFn
	{
	public:
		bool operator() (int const& t1, int const& t2) const
		{
			//return std::tie(t1.W, t1.X, t1.Y) == std::tie(t2.W, t2.X, t2.Y);
			return t1 == t2;
		}

	};

	class pathMap : public Graph
	{
	public:
		pathMap(int xCenter, int yCenter, int radius);
		//build map with army specific
		//for example for sea one we add only sea tiles, etc
		pathMap(const armyStruct* army, int radius);
		virtual ~pathMap() {
			delete Pather;
		}

		float calculateDistance(int x, int y, int destX, int destY);
		float calculateDistance(int x, int y, int destX, int destY, std::vector<std::pair<int, int>>& path);
		void getPossibleTilesForArmy(int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords);
		std::pair<int, int> getNearestTileForArmy(int x, int y, int destX, int destY);
		std::pair<int, int> getSafestTileForArmy(const armyStruct* army);
	private:
		pathMap();
		MicroPather* Pather;
		size_t Diameter = 0;
		int XCenter = 0;
		int YCenter = 0;
		void* getState(int x, int y);

		std::vector<PathNode>StateMap;


		float leastCostEstimate(void* stateStart, void* stateEnd) override;
		void adjacentCost(void* state, MP_VECTOR< micropather::StateCost >* adjacent) override;
		void adjOne(int x, int y, int currX, int currY, MP_VECTOR<micropather::StateCost>*& adjacent);
		void printStateInfo(void* state) override;


		static bool isCoordsValid(int x, int y);
		static bool isSameTypeOfGround(int x, int y, int destX, int destY);
	};

	void getPossibleTilesForArmyFromCache(void* cache, int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords);
	std::pair<int, int> getNearestTileForArmyFromCache(void* cache, int x, int y, int destX, int destY);

	//for retreats, etc
	std::pair<int, int> getSafestTileForArmyFromCache(void* cache, const armyStruct* army);

	void* createCacheForArmy(const armyStruct* army, int radius);
	void* createCacheForDistances(int x, int y, int radius);
	void deleteCacheForDistances(void* cache); 
	float getMovePointsForReachTileFromCache(void* cache, int x, int y, int destX, int destY);
};