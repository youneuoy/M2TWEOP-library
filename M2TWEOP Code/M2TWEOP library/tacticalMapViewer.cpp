#include "pch.h"
#include "tacticalMapViewer.h"
#include "imgui/imgui.h"
#include "onlineThings.h"
#include "strategyMap.h"
#include "techFuncs.h"

#include "texturesManager.h"
void tacticalMapViewer::init(const int gameVer)
{
	state.isBattleGeneratorWorking = battleCreator::getIsGenerationNeeded();
	if (gameVer == 2)//steam
	{
		functions.startTransitionToMapView = reinterpret_cast<startTransitionToMapViewFunc>(0x00ac18c0);
		functions.initPathCache = reinterpret_cast<initPathCacheFunc>(0x00610730);
		offsets.pathCacheOffset = reinterpret_cast<int*>(0x016a7338);
	}
	else
	{
		functions.startTransitionToMapView = reinterpret_cast<startTransitionToMapViewFunc>(0x00ac0890);
		functions.initPathCache = reinterpret_cast<initPathCacheFunc>(0x00610380);
		offsets.pathCacheOffset = reinterpret_cast<int*>(0x016f0510);
	}
	state.backupData.clear();
	state.backupData.reserve(500);
	state.backupData.resize(500);
	techFuncs::read(reinterpret_cast<DWORD>(functions.startTransitionToMapView), state.backupData.data(),500);
}

void tacticalMapViewer::view(const int x, const int y)
{
	if (!isTacticalMapViewerNeeded)
		return;
	state.isBattleGeneratorWorking = battleCreator::getIsGenerationNeeded();
	battleCreator::setCreationNeeded(false);
	if (const auto tile = stratMapHelpers::getTile(x, y); !tile->getSettlement() && !tile->getFort())
	{
		const DWORD startAdr = reinterpret_cast<DWORD>(functions.startTransitionToMapView);
		techFuncs::nopBytes(startAdr + 143,16);
		techFuncs::nopBytes(startAdr + 222,16);
		techFuncs::nopBytes(startAdr + 250,3);
	}
	functions.startTransitionToMapView(x, y);
	functions.initPathCache();
}

void tacticalMapViewer::unView()
{
	battleCreator::setCreationNeeded(state.isBattleGeneratorWorking);
	techFuncs::writeData(state.backupData.data(), reinterpret_cast<DWORD>(functions.startTransitionToMapView),500);
}

void tacticalMapViewer::draw()
{
	ImGui::Begin("TacticalMapViewer", nullptr, ImGuiWindowFlags_NoDecoration);

	ImGui::InputInt2("Coords", state.coords);

	if(ImGui::Button("Go"))
	{
		view(state.coords[0], state.coords[1]);
	}

	ImGui::End();
}

int __fastcall tacticalMapViewer::getPathCache(const tacticalMapViewer* we)
{
	if (we->offsets.pathCacheOffset == nullptr || *(we->offsets.pathCacheOffset) == 0)
	{
		we->functions.initPathCache();
	}
	assert(we);
	assert(we->offsets.pathCacheOffset);

	return *(we->offsets.pathCacheOffset);
}
