#pragma once
#include <vector>
#include <memory>
#include "imgui/imgui.h"
#include "realGameTypes.h"

enum class tileContent:uint8_t
{
	nothing,
	settlement,
	army,
	fort,
	port
};
class m2tweopTile
{
public:
	void buildTile(int x,int y);
	bool drawTile(const ImVec2&tileSize, const ImVec2& coordsStart, std::vector<ImVec2>*borders,bool isSelectedNow);

	bool buildBorder(
		const std::shared_ptr<m2tweopTile>& leftTile,
		const std::shared_ptr<m2tweopTile>& rightTile,
		const std::shared_ptr<m2tweopTile>& upTile,
		const std::shared_ptr<m2tweopTile>& lowTile);
private:
	bool haveLBorder = false;
	bool haveRBorder = false;
	bool haveUBorder = false;
	bool haveDBorder = false;

	bool IsSameCoords(std::shared_ptr<m2tweopTile> anotherTile);
	bool isNeighbor(std::shared_ptr<m2tweopTile>anotherTile);

	int xTile = 0;
	int yTile = 0;
	void buildAsSettlementTile();
	void buildAsFortTile();
	void buildAsPortTile();
	void buildAsArmyTile(armyStruct* army);

	int tileRegionID = 0;
	int ownerDipNum = 0;

	ImVec4 tileColor{ 41/255.f,140/255.f, 233/255.f, 1.0f };
	ImVec4 tileSecColor{ 41/255.f,140/255.f, 233/255.f,  1.0f };
	tileContent tileCont= tileContent::nothing;

	friend class m2tweopMap;
	friend class m2tweopRegion;
};

