#include "ContextMenuStrat.h"
#include "fastFuncts.h"
#include "TexturesManager.h"

void ContextMenuStrat::Draw()
{
	if (isContextMenuNeeded == false)
	{
		return;
	}
	if (fastFuncts::IsStratMap() == false)
	{
		return;
	}
	if ((ImGui::GetIO().MouseDownDuration[2] > 0.f)
		&& (ImGui::GetIO().MouseDownDurationPrev[2] == 0.f)
		)
	{
		isWork = !isWork;

		ctxPos = ImGui::GetMousePos();

		fastFuncts::GetGameTileCoordsWithCursor(posAtMap[0], posAtMap[1]);
	}

	if (isWork == false)
	{
		return;
	}

	ImGui::SetNextWindowPos(ctxPos, ImGuiCond_Always);
	ImGui::Begin("##ContextMenuTactical", nullptr, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoScrollWithMouse);


	auto image = TexturesManager::GetImage(99);
	if (image == nullptr)
	{
		TexturesManager::LoadTexture(99, "/eopData/images/SMALL_SCROLL_BACKGROUND.png");
	}
	else
	{
		auto currPos = ImGui::GetCursorPos();


		ImGui::Image(image, ctxSize);

		ImGui::SetCursorPos(currPos);
	}


	if (ImGui::Button("View tactical map"))
	{
		fastFuncts::ViewTacticalMap(posAtMap[0], posAtMap[1]);

		isWork = false;
	}
	ctxSize = ImGui::GetItemRectSize();
	ImGui::End();

}
