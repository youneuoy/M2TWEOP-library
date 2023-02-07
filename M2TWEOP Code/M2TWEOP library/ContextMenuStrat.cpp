#include "ContextMenuStrat.h"
#include "fastFuncts.h"

void ContextMenuStrat::Draw()
{
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
	}

	if (isWork == false)
	{
		return;
	}

	ImGui::SetNextWindowPos(ctxPos, ImGuiCond_Always);
	ImGui::Begin("##ContextMenuTactical", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Button("View tactical map"))
	{
		int x = 0;
		int y = 0;
		fastFuncts::GetGameTileCoordsWithCursor(x,y);
		fastFuncts::ViewTacticalMap(x, y);

		isWork = false;
	}

	ImGui::End();
}
