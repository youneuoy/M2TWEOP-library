#pragma once
#include <unordered_set>
#include "micropather.h"
#include "exportHeader.h"
#include "headersMEM.h"


using namespace micropather;

namespace PathFinder
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

		PathNode() = default;
		PathNode(int x, int y, float w) : X(x), Y(y), W(w) {}
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

	class PathMap : public Graph
	{
	public:
		PathMap(int xCenter, int yCenter, int radius);
		//build map with army specific
		//for example for sea one we add only sea tiles, etc
		PathMap(int xCenter, int yCenter, int radius, stackStruct* army);
		virtual ~PathMap() {
			delete Pather;
		}

		float CalculateDistance(int x, int y, int destX, int destY);
		float CalculateDistance(int x, int y, int destX, int destY, std::vector<std::pair<int, int>>& path);
		void GetPossibleTilesForArmy(int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords);
	private:
		MicroPather* Pather;
		size_t Diameter = 0;
		int XCenter = 0;
		int YCenter = 0;
		void* GetState(int x, int y);


		std::vector<PathNode>StateMap;


		float LeastCostEstimate(void* stateStart, void* stateEnd) override;
		void AdjacentCost(void* state, MP_VECTOR< micropather::StateCost >* adjacent) override;
		void AdjOne(int x, int y, int currX, int currY, MP_VECTOR<micropather::StateCost>*& adjacent);
		void PrintStateInfo(void* state) override;


		bool IsCoordsValid(int x, int y);
		bool IsSameTypeOfGround(int x, int y, int destX, int destY);
	};
	void GetPossibleTilesForArmyFromCashe(void* cashe, int x, int y, std::unordered_set<std::pair<int, int>, pathPairHash>& possibleCoords);

	NOINLINE EOP_EXPORT void* CreateCasheForDistances(int x, int y, int radius);
	NOINLINE EOP_EXPORT void DeleteCasheForDistances(void* cashe); 
	NOINLINE EOP_EXPORT float GetMovepointsForReachTileFromCashe(void* cashe, int x, int y, int destX, int destY);
};