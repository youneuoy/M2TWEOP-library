#include "DeveloperMode.h"

#include "campaign.h"
#include "globals.h"
#include "m2tweopMapManager.h"
	
void toggleDeveloperMode()
{
	auto& developerMode = globals::dataS.Modules.developerMode;
	developerMode.toggleDeveloperModeBase();
}

void DeveloperMode::Update()
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
		if (m2tweopMapManager::draw() == false)
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

void DeveloperMode::toggleDeveloperModeBase()
{
	isWork = !isWork;
}

string DeveloperMode::SelectWorldpkgdesc(const std::string& selectedRec, const std::string& selectedGroup)
{
	return subs.battleMapOverrider.SelectWorldpkgdesc(selectedRec, selectedGroup);
}
