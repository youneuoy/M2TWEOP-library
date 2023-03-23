#pragma once
#include <unordered_map>
#include "micropather.h"
#include "exportHeader.h"
#include "headersMEM.h"


using namespace micropather;

namespace PathFinder
{
	class PathNode {
	public:
		int X, Y;
		float W;
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
		virtual ~PathMap() {
			delete Pather;
		}

		float CalculateDistance(int x, int y, int destX, int destY);
	private:
		MicroPather* Pather;
		size_t Diameter = 0;
		void* GetState(int x, int y);


		std::unordered_map<int, PathNode, Hasher, EqualFn>StateMap;


		float LeastCostEstimate(void* stateStart, void* stateEnd) override;
		void AdjacentCost(void* state, MP_VECTOR< micropather::StateCost >* adjacent) override;
		void PrintStateInfo(void* state) override;


		bool IsCoordsValid(int x, int y);
		bool IsSameTypeOfGround(int x, int y, int destX, int destY);
	};


	NOINLINE EOP_EXPORT void* CreateCasheForDistances(int x, int y, int radius);
	NOINLINE EOP_EXPORT void DeleteCasheForDistances(void* cashe);
	NOINLINE EOP_EXPORT float GetMovepointsForReachTileFromCashe(void* cashe, int x, int y, int destX, int destY);
};