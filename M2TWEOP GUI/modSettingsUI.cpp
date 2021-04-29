#include "modSettingsUI.h"
#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"
#include "managerG.h"

namespace modSettingsUI
{
	void drawModSettingsUI(bool* isOpen)
	{
		ImVec2 windowSize = ImGui::CalcTextSize("Run vanilla or dlc(no M2TWEOP capabilities)");
		windowSize.x *= 1.5;

		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, -1), ImGuiCond_Once);
		ImGui::Begin("M2TWEOP mod settings", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		if (dataG::data.modData.useVanillaConfig == true)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		ImVec2 textSize = ImGui::CalcTextSize("Readme Teutonic");
		ImGui::PushItemWidth(textSize.x);
		ImGui::InputText("Config file name", &dataG::data.modData.configName);

		if (dataG::data.modData.useVanillaConfig == true)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Checkbox("Use standard config", &dataG::data.modData.useVanillaConfig);
		ImGui::NewLine();
		ImGui::Checkbox("Use M2TWEOP", &dataG::data.modData.useM2TWEOP);

		ImGui::Checkbox("Hide launcher on startup", &dataG::data.modData.hideLauncherAtStart);
		
		ImGui::NewLine();
		if (ImGui::Button("Save", helpers::getScreen().centerXButton))
		{
			managerG::saveSettings();
		}
		ImGui::End();
	}
};