#include "TacticalMapVeiwer.h"
#include "imgui.h"
#include "onlineThings.h"

#include "fastFuncts.h"
#include "techFuncs.h"

#include "TexturesManager.h"
void TacticalMapViewer::Init(int gameVer)
{
	state.IsBattleGeneratorWorking = battleCreator::GetIsGenerationNeeded();
	if (gameVer == 2)//steam
	{
		functions.startTransitionToMapView = reinterpret_cast<StartTransitionToMapView>(0x00ac18c0);
		functions.initPathCashe = reinterpret_cast<InitPathCashe>(0x00610730);

		offsets.PathCasheOffset = (int*)0x016a7338;
	}
	else
	{
		functions.startTransitionToMapView = reinterpret_cast<StartTransitionToMapView>(0x00ac0890);
		functions.initPathCashe = reinterpret_cast<InitPathCashe>(0x00610380);

		offsets.PathCasheOffset = (int*)0x016f0510;
	}

	state.backupedData.clear();
	state.backupedData.reserve(500);
	state.backupedData.resize(500);
	techFuncs::Read((DWORD)functions.startTransitionToMapView, state.backupedData.data(),500);
}

void TacticalMapViewer::View(int x, int y)
{
	if (isTacticalMapViewerNeeded == false)
	{
		return;
	}
	state.IsBattleGeneratorWorking = battleCreator::GetIsGenerationNeeded();
	battleCreator::setCreationNeeded(false);


	if (fastFuncts::findSettlement(x, y) == nullptr
		&& fastFuncts::findFort(x, y) == nullptr
		)
	{
		DWORD startAdr = (DWORD)functions.startTransitionToMapView;
		techFuncs::NopBytes(startAdr+143,16);
		techFuncs::NopBytes(startAdr+222,16);
		techFuncs::NopBytes(startAdr+250,3);
	}

	functions.startTransitionToMapView(x, y);

	functions.initPathCashe();
}

void TacticalMapViewer::UnView()
{
	battleCreator::setCreationNeeded(state.IsBattleGeneratorWorking);
	techFuncs::WriteData(state.backupedData.data(), (DWORD)functions.startTransitionToMapView,500);
}

void TacticalMapViewer::Draw()
{
	ImGui::Begin("TacticalMapViewer", nullptr, ImGuiWindowFlags_NoDecoration);

	ImGui::InputInt2("Coords", state.cords);

	if(ImGui::Button("Go"))
	{
		View(state.cords[0], state.cords[1]);
	}

	ImGui::End();
}

int __fastcall TacticalMapViewer::GetPathCashe(TacticalMapViewer* we)
{
	if (we->offsets.PathCasheOffset == nullptr || *(we->offsets.PathCasheOffset) == 0)
	{
		we->functions.initPathCashe();
	}
	assert(we);
	assert(we->offsets.PathCasheOffset);

	return *(we->offsets.PathCasheOffset);
}
