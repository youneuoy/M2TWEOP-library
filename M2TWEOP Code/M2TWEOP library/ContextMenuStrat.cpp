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

		fastFuncts::GetGameTileCoordsWithCursor(posAtMap[0], posAtMap[1]);
	}

	if (isWork == false)
	{
		return;
	}

	ImGui::SetNextWindowPos(ctxPos, ImGuiCond_Always);
	ImGui::Begin("##ContextMenuTactical", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Button("View tactical map"))
	{
		fastFuncts::ViewTacticalMap(posAtMap[0], posAtMap[1]);

		isWork = false;
	}

	ImGui::End();
}
