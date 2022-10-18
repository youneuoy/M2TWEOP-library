#include "m2tweopTile.h"

void m2tweopTile::buildTile(int x, int y)
{
	xTile = x;
	yTile = y;
	tileRegionID = fastFuncts::getTileRegionID(xTile, yTile);

	settlementStruct* sett= fastFuncts::findSettlement(xTile, yTile);
	if (sett != nullptr)
	{
		buildAsSettlementTile();
	}
	else
	{
		factionStruct* owner=fastFuncts::getRegionOwner(tileRegionID);

		if (owner != nullptr)
		{
			ownerDipNum = owner->dipNum;

			tileColor.x= owner->factSmDescr->primary_colour_red / 255.0F;
			tileColor.y= owner->factSmDescr->primary_colour_green / 255.0F;
			tileColor.z= owner->factSmDescr->primary_colour_blue / 255.0F;


			tileSecColor.x= owner->factSmDescr->secondary_colour_red / 255.0F;
			tileSecColor.y= owner->factSmDescr->secondary_colour_green / 255.0F;
			tileSecColor.z= owner->factSmDescr->secondary_colour_blue / 255.0F;
		}
	}
}

bool m2tweopTile::drawTile(const ImVec2& tileSize, const ImVec2& coordsStart, vector<ImVec2>* borders, bool isSelectedNow)
{
	bool isSelected = false;

	ImGui::PushID(this);
	switch (tileCont)
	{
	case tileContent::nothing:
		break;
	case tileContent::settlement:
		break;
	case tileContent::army:
		break;
	}

	ImVec2 butCoords= ImGui::GetCursorPos();

	if (isSelectedNow)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f-tileColor.x, 1.f - tileColor.y, 1.f - tileColor.z, tileColor.w));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, tileColor);
	}

	isSelected = ImGui::Button("", tileSize);
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("x:%d, y:%d, region id:%d", xTile, yTile, tileRegionID);
	}
	ImGui::PopStyleColor();
	ImGui::PopID();


	if (borders != nullptr)
	{
		if (haveLBorder)
		{
			ImVec2 p1 = butCoords;
			p1.x += coordsStart.x;
			p1.y += coordsStart.y;
			ImVec2 p2 = p1;
			p2.y += tileSize.y;

			borders->push_back(p1);
			borders->push_back(p2);
			//drawList->AddLine(p1, p2, IM_COL32(200, 200, 200, 255), 0.2f * tileSize.y);
		}
		if (haveUBorder)
		{
			ImVec2 p1 = butCoords;
			p1.x += coordsStart.x;
			p1.y += coordsStart.y;
			ImVec2 p2 = p1;
			p2.x += tileSize.x;

			borders->push_back(p1);
			borders->push_back(p2);
			//drawList->AddLine(p1, p2, IM_COL32(200, 200, 200, 255), 0.2f * tileSize.y);
		}
	}

	return isSelected;
}

bool m2tweopTile::buildBorder(const shared_ptr<m2tweopTile>& leftTile, const shared_ptr<m2tweopTile>& rightTile, const shared_ptr<m2tweopTile>& upTile, const shared_ptr<m2tweopTile>& lowTile)
{

	if (isNeighbor(leftTile))
	{
		haveLBorder = true;
	}
	if (isNeighbor(rightTile))
	{
		haveRBorder = true;
	}
	if (isNeighbor(upTile))
	{
		haveUBorder = true;
	}
	if (isNeighbor(lowTile))
	{
		haveDBorder = true;
	}



	return false;
}

bool m2tweopTile::IsSameCoords(shared_ptr<m2tweopTile> anotherTile)
{
	if (anotherTile->tileRegionID != tileRegionID)
	{
		return false;
	}
	int anX = anotherTile->xTile;
	int anY = anotherTile->yTile;
	return (anX == xTile && anY == yTile);
}

bool m2tweopTile::isNeighbor(shared_ptr<m2tweopTile> anotherTile)
{
	if (anotherTile->tileRegionID == tileRegionID)
	{
		return false;
	}
	int anX=anotherTile->xTile;
	int anY=anotherTile->yTile;

	int coordsDif = abs(xTile - anX) + abs(yTile - anY);

	if (coordsDif == 1)
	{
		return true;
	}
	return false;
}

void m2tweopTile::buildAsSettlementTile()
{
	tileColor.x = 1.0F;
	tileColor.y =1.0F;
	tileColor.z = 1.0F;

	tileCont = tileContent::settlement;


}
