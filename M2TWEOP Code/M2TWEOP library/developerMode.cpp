#include "pch.h"
#include "developerMode.h"

#include "campaign.h"
#include "globals.h"
#include "m2tweopMap.h"
#include "mapImage.h"
	
void developerMode::toggleDeveloperMode()
{
	auto& developerMode = globals::dataS.Modules.developerMode;
	developerMode.toggleDeveloperModeBase();
}

void developerMode::update()
{
	if (isDeveloperModeNeeded == false)
	{
		return;
	}
	if (const campaign* campaign = campaignHelpers::getCampaignData(); campaign->humanPlayers > 1)
		return;

	if (isWork == false)
	{
		return;
	}
	
	if (subsState.isMapManager==true)
	{
		if (draw() == false)
		{
			subsState.isMapManager = false;
		}
		return;
	}
	

	if (subsState.isBattleMapOverrider ==true)
	{
		if (subs.battleMapOverrider.Draw() == false)
		{
			subsState.isBattleMapOverrider = false;
		}
		return;
	}

	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);

	ImGui::Begin("Developer mode", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if (ImGui::Button("Interactive map"))
	{
		subsState.isMapManager = true;
	}
	if (ImGui::Button("Override battle map objects"))
	{
		subsState.isBattleMapOverrider = true;
	}


	ImGui::End();
}

void developerMode::toggleDeveloperModeBase()
{
	isWork = !isWork;
}

string developerMode::selectWorldpkgdesc(const std::string& selectedRec, const std::string& selectedGroup)
{
	return subs.battleMapOverrider.SelectWorldpkgdesc(selectedRec, selectedGroup);
}

struct
{
	m2tweopMap currentMap;
	ImVec2 tileSize = {7.f,7.f};
	bool isShowMap = false;
	bool isShowBattleMap = false;
	bool showBattleStats = false;
}managerData;
	
bool developerMode::draw()
{
	bool isWork = true;

	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
	if (managerData.isShowMap == true)
	{
		ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize, ImGuiCond_Always);
	}
	else
	{
		ImGui::SetNextWindowSize({ 0,0 }, ImGuiCond_Always);
	}
	ImGui::Begin("Map manager",nullptr, ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoMove);


	if (ImGui::Button("Close"))
	{
		isWork = false;
	}
	if (managerData.isShowMap == false)
	{
		if (ImGui::Button("Open map manager"))
		{
			managerData.currentMap.buildMap();
			managerData.isShowMap = true;
		}
	}
	else
	{
		if (ImGui::Button("Hide map manager"))
		{
			managerData.isShowMap = false;
		}
	}


	if (managerData.isShowMap == true)
	{
		ImGui::SetNextItemWidth(200.f);
		if (ImGui::SliderFloat("Map scale", &managerData.tileSize.x, 2.0f, 100.0f, "%.0f"))
		{
		}
		managerData.tileSize.y = managerData.tileSize.x;

		ImVec2 mapImageSize = ImGui::GetMainViewport()->WorkSize;
		mapImageSize.x -= mapImageSize.x / 5;
		mapImageSize.y -= mapImageSize.y / 8;
		ImGui::BeginChild("mapImage", mapImageSize,true, ImGuiWindowFlags_HorizontalScrollbar);
		managerData.currentMap.drawMap(managerData.tileSize);
		ImGui::EndChild();


		ImGui::SameLine();
		ImVec2 mapInteractSize;
		mapInteractSize.x = ImGui::GetMainViewport()->WorkSize.x - mapImageSize.x;
		mapInteractSize.y = mapImageSize.y;

		ImGui::BeginChild("mapInteract", mapInteractSize, true, ImGuiWindowFlags_HorizontalScrollbar);
		managerData.currentMap.drawInteract();
		ImGui::EndChild();
	}

	ImGui::End();

	return isWork;
}
