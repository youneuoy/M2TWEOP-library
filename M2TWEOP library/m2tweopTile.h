#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "eventsCodes.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "imgui.h"
#include <d3d9.h>
#include <sstream>
#include "realGameTypes.h"
using namespace std;

enum class tileContent:uint8_t
{
	nothing,
	settlement,
	army
};
class m2tweopTile
{
public:
	void buildTile(int x,int y);
	bool drawTile(const ImVec2&tileSize, const ImVec2& coordsStart, vector<ImVec2>*borders);

	bool buildBorder(
		const shared_ptr<m2tweopTile>& leftTile,
		const shared_ptr<m2tweopTile>& rightTile,
		const shared_ptr<m2tweopTile>& upTile,
		const shared_ptr<m2tweopTile>& lowTile);
private:
	bool haveLBorder = false;
	bool haveRBorder = false;
	bool haveUBorder = false;
	bool haveDBorder = false;


	bool isNeighbor(shared_ptr<m2tweopTile>anotherTile);

	int xTile = 0;
	int yTile = 0;
	void buildAsSettlementTile();
	int tileRegionID = 0;
	int ownerDipNum = 0;

	ImVec4 tileColor{ 41/255.f,140/255.f, 233/255.f, 1.0f };
	ImVec4 tileSecColor{ 41/255.f,140/255.f, 233/255.f,  1.0f };
	tileContent tileCont= tileContent::nothing;

	friend class m2tweopMap;
	friend class m2tweopRegion;
};

