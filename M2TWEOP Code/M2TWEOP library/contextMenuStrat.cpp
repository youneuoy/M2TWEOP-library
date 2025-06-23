#include "pch.h"
#include "contextMenuStrat.h"
#include "texturesManager.h"
#include "plannedRetreatRoute.h"
#include "strategyMap.h"
#include "onlineThings.h"

ContextMenuStrat::ContextMenuStrat()
{
		ctxItems.insert(std::make_pair("TacticMapView", ContextMenuItem(
			[]()
			{
				if (ImGui::Button("View battle map"))
				{
					int posAtMap[2]{};

					stratMapHelpers::getGameTileCoordsWithCursor(posAtMap[0], posAtMap[1]);
					stratMapHelpers::viewTacticalMap(posAtMap[0], posAtMap[1]);

					return true;
				}

				return false;
			},
			[](int posAtMap[2])
			{
				return true;
			}
			)));
		if (battleCreator::isRetreatsOverhaulEnabled()) {
			ctxItems.insert(std::make_pair("RetreatPlanner", ContextMenuItem(
			[]()
			{
				if (ImGui::Button("Set planned retreat route"))
				{
					int posAtMap[2]{};

					stratMapHelpers::getGameTileCoordsWithCursor(posAtMap[0], posAtMap[1]);
					plannedRetreatRoute::startWork(posAtMap[0], posAtMap[1]);

					return true;
				}

				return false;
			},
			[](int posAtMap[2])
			{
				if (const auto tile = stratMapHelpers::getTile(posAtMap[0], posAtMap[1]); tile->getArmy(false))
					return true;

				return false;
			})));
		}
}

void ContextMenuStrat::Draw()
{
	if (isContextMenuNeeded == false)
	{
		return;
	}
	if (stratMapHelpers::isStratMap() == false)
	{
		return;
	}
	if ((ImGui::GetIO().MouseDownDuration[2] > 0.f)
		&& (ImGui::GetIO().MouseDownDurationPrev[2] == 0.f)
		)
	{
		isWork = !isWork;

		ctxPos = ImGui::GetMousePos();


		if (isWork == true)
		{
			int posAtMap[2]{};
			stratMapHelpers::getGameTileCoordsWithCursor(posAtMap[0], posAtMap[1]);
			for (auto& item : ctxItems)
			{
				auto& ctx = item.second;
				if (ctx.NeedWork(posAtMap) == true)
				{
					ctx.Active = true;
				}
				else
				{
					ctx.Active = false;
				}
			}
		}
	}

	if (isWork == false)
	{
		return;
	}

	ImGui::SetNextWindowPos(ctxPos, ImGuiCond_Always);
	ImGui::Begin("##ContextMenuTactical", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_AlwaysAutoResize);



	for (auto& item : ctxItems)
	{
		auto& ctx = item.second;
		if (ctx.Active == false)
		{
			continue;
		}
		if (ctx.Work()==true)
		{
			isWork = false;
		}
	}


	auto image = TexturesManager::GetImage(99);

	auto currPos = ImGui::GetCursorPos();
	if (image == nullptr)
	{
		//TexturesManager::LoadTexture(99, "/eopData/resources/images/SMALL_SCROLL_BACKGROUND.png");
	}
	else
	{
		currPos = ImGui::GetCursorPos();


		ImGui::Image(image, ctxSize);

		ImGui::SetCursorPos(currPos);
	}

	ctxSize = ImGui::GetWindowSize();
	ImGui::End();

}


ContextMenuItem::ContextMenuItem(ContextMenuItemWork work, ContextMenuItemNeedWork needWork)
{
	Work = work;
	NeedWork = needWork;
}
