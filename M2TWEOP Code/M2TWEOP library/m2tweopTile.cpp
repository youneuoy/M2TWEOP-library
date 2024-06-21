#include "pch.h"
#include "m2tweopTile.h"
#include "globals.h"
#include "faction.h"
#include "army.h"
#include "character.h"
#include "strategyMap.h"

#include "techFuncs.h"

void m2tweopTile::buildTile(int x, int y)
{
	xTile = x;
	yTile = y;
	const auto tile = stratMapHelpers::getTile(x, y);
	tileRegionID = tile->regionId;
	const factionStruct* owner = stratMapHelpers::getRegion(tileRegionID)->factionOwner;
	if (owner != nullptr)
	{
		ownerDipNum = owner->factionID;
	}
	const settlementStruct* sett = tile->getSettlement();
	if (sett != nullptr)
	{
		buildAsSettlementTile();
		return;
	}
	auto* fort = tile->getFort();
	if (fort != nullptr)
	{
		buildAsFortTile();
		return;
	}
	auto* port = tile->getPort();
	if (port != nullptr)
	{
		buildAsPortTile();
		return;
	}
	const auto* character = tile->getCharacter();
	if (character && character->armyLeaded)
	{
		buildAsArmyTile(character->armyLeaded);
		return;
	}
	if (owner != nullptr)
	{

		tileColor.x = owner->factionRecord->primary_colour_red / 255.0F;
		tileColor.y = owner->factionRecord->primary_colour_green / 255.0F;
		tileColor.z = owner->factionRecord->primary_colour_blue / 255.0F;


		tileSecColor.x = owner->factionRecord->secondary_colour_red / 255.0F;
		tileSecColor.y = owner->factionRecord->secondary_colour_green / 255.0F;
		tileSecColor.z = owner->factionRecord->secondary_colour_blue / 255.0F;
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
	case tileContent::port:
		break;
	case tileContent::fort:
		break;
	}

	ImVec2 butCoords = ImGui::GetCursorPos();

	if (isSelectedNow)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f - tileColor.x, 1.f - tileColor.y, 1.f - tileColor.z, tileColor.w));
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
	int anX = anotherTile->xTile;
	int anY = anotherTile->yTile;

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
	tileColor.y = 1.0F;
	tileColor.z = 1.0F;

	tileCont = tileContent::settlement;


}

void m2tweopTile::buildAsFortTile()
{
	tileColor.x = 0.9F;
	tileColor.y = 0.9F;
	tileColor.z = 0.9F;

	tileCont = tileContent::fort;
}
void m2tweopTile::buildAsPortTile()
{
	tileColor.x = 0.8F;
	tileColor.y = 0.8F;
	tileColor.z = 0.8F;

	tileCont = tileContent::port;
}

void m2tweopTile::buildAsArmyTile(armyStruct* army)
{
	factionStruct* fac = army->faction;

	if (fac != nullptr)
	{
		tileColor.x = fac->factionRecord->secondary_colour_red / 255.0F;
		tileColor.y = fac->factionRecord->secondary_colour_green / 255.0F;
		tileColor.z = fac->factionRecord->secondary_colour_blue / 255.0F;
	}

	tileCont = tileContent::army;
}
