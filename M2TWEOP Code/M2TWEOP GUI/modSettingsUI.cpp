#include "modSettingsUI.h"
#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"
#include "managerG.h"
#include "../M2TWEOP Common/m2tweopConstData.h"

namespace modSettingsUI
{
	enum selectedSettingsPage_
	{
		selectedSettingsPage_general,
		selectedSettingsPage_launcher,
		selectedSettingsPage_hs,
		selectedSettingsPage_rules,
		selectedSettingsPage_game,
	};


	struct settingsPage
	{
		std::string pageName;
		selectedSettingsPage_ pageId;
	};

	struct
	{
		std::vector<settingsPage>settingsPages;
		selectedSettingsPage_ selectedPage = selectedSettingsPage_general;

		//size of one selectable
		ImVec2 selectableSize{ 100,50 };
		//size of selectables child
		ImVec2 selectablesSize{};
	}settingsUIData;




	void initSettingsUI()
	{
		settingsUIData.settingsPages.clear();


		//main page
		{
			settingsPage generalPage;
			generalPage.pageId = selectedSettingsPage_general;
			generalPage.pageName = "General";

			settingsUIData.settingsPages.push_back(std::move(generalPage));
		}
		//launcher page
		{
			settingsPage launcherPage;
			launcherPage.pageId = selectedSettingsPage_launcher;
			launcherPage.pageName = "Launcher";

			settingsUIData.settingsPages.push_back(std::move(launcherPage));
		}
		//game page
		{
			settingsPage gamePage;
			gamePage.pageId = selectedSettingsPage_game;
			gamePage.pageName = "Game";

			settingsUIData.settingsPages.push_back(std::move(gamePage));
		}
		//hs page
		{
			settingsPage hsPage;
			hsPage.pageId = selectedSettingsPage_hs;
			hsPage.pageName = "Hotseat";

			settingsUIData.settingsPages.push_back(std::move(hsPage));
		}

		/*
		//rules page
		{
			settingsPage rulesPage;
			rulesPage.pageId = selectedSettingsPage_rules;
			rulesPage.pageName = "Rules";

			settingsUIData.settingsPages.push_back(std::move(rulesPage));
		}*/

		settingsUIData.selectablesSize.x = settingsUIData.selectableSize.x;
		settingsUIData.selectablesSize.y = settingsUIData.selectableSize.y * settingsUIData.settingsPages.size() + ImGui::GetStyle().ItemSpacing.y * settingsUIData.settingsPages.size();
	}

	void drawGeneralSettings()
	{
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

	}

	void drawLauncherSettigs()
	{
		if (ImGui::Checkbox("Play background music", &dataG::data.audio.bkgMusic.isMusicNeeded))
		{
			if (dataG::data.audio.bkgMusic.isMusicNeeded == false)
			{
				dataG::data.audio.bkgMusic.music->stop();
			}
			else
			{
				dataG::data.audio.bkgMusic.music->play();
			}
		}
		if (ImGui::SliderInt("Music volume", &dataG::data.audio.bkgMusic.musicVolume, 0, 100))
		{
			dataG::data.audio.bkgMusic.music->setVolume(dataG::data.audio.bkgMusic.musicVolume);
		}
	}

	void drawRulesSettings()
	{

	}

	void drawHsSettings()
	{
		ImGui::Checkbox("Autogeneration of historical battles", &dataG::data.battlesData.isGenerationNeeded);
		ImGui::Checkbox("Autogeneration of battle results files", &dataG::data.battlesData.isResultTransferNeeded);
	}

	void drawGameSettings()
	{
		ImGui::Checkbox("Block modification launch without EOP", &dataG::data.gameData.isBlockLaunchWithoutEop);
	}
	void drawModSettingsUI(bool* isOpen)
	{
		ImGuiIO& io = ImGui::GetIO();


		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize({800,-1}, ImGuiCond_Once);
		ImGui::Begin(eopConstData::eopVersionName, isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);



		//selectable child
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.143f, 0.143f, 0.143f, 1.000f));
			ImGui::BeginChild("PreferencesSelectable", settingsUIData.selectablesSize);
			static int selected = 0;

			for (size_t n = 0; n < settingsUIData.settingsPages.size(); n++)
			{
				ImVec2 cp = ImGui::GetCursorScreenPos();
				ImVec2 cp2 = ImVec2(cp.x + settingsUIData.selectableSize.x, cp.y + settingsUIData.selectableSize.y);


				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));

				if (ImGui::Selectable(settingsUIData.settingsPages[n].pageName.c_str(), selected == n, 0, settingsUIData.selectableSize))
				{
					selected = n;
					settingsUIData.selectedPage = settingsUIData.settingsPages[n].pageId;
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();

				if (selected == n)
				{

					ImU32 col_a = ImGui::GetColorU32(ImGuiCol_Button);


					ImDrawList* draw_list = ImGui::GetWindowDrawList();
					ImU32 col_b = ImGui::GetColorU32(ImGuiCol_ChildBg);
					draw_list->AddRectFilledMultiColor(cp, cp2, col_a, col_b, col_b, col_a);
				}

			}

			ImGui::EndChild();
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();


		//settings content child
		{
			ImGui::BeginChild("PreferencesContent", { -1,settingsUIData.selectablesSize.y});

			if (settingsUIData.selectedPage == selectedSettingsPage_general)
			{
				drawGeneralSettings();
			}
			else if (settingsUIData.selectedPage == selectedSettingsPage_launcher)
			{
				drawLauncherSettigs();
			}
			else if (settingsUIData.selectedPage == selectedSettingsPage_rules)
			{
				drawRulesSettings();
			}
			else if (settingsUIData.selectedPage == selectedSettingsPage_hs)
			{
				drawHsSettings();
			}
			else if (settingsUIData.selectedPage == selectedSettingsPage_game)
			{
				drawGameSettings();
			}

			ImGui::EndChild();
		}
		if (ImGui::Button("Save", helpers::getScreen().centerXButton))
		{

			managerG::saveJsonSettings();
			*isOpen = false;
		}

		ImGui::End();
	}
};