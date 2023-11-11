#include "modSettingsUI.h"
#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"
#include "managerG.h"
#include "themeManager.h"
#include "../M2TWEOP Common/m2tweopConstData.h"

namespace modSettingsUI
{
	enum selectedSettingsPage_
	{
		selectedSettingsPage_general,
		selectedSettingsPage_launcher,
		selectedSettingsPage_hs,
		selectedSettingsPage_rules,
		selectedSettingsPage_game
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

		//ImGui::InputText("Config file name", &dataG::data.modData.configName);

		std::vector<std::string> cfgFiles = helpers::getCfgFilesInFolder();
		std::vector<const char*> items;
		for (const auto& file : cfgFiles) {
			items.push_back(file.c_str());
		}
		string path;
		helpers::getCurrentPath(path);
		static int selectedItem = -1;
		if (items.size() == 0)
		{
			dataG::data.modData.useVanillaConfig = true;
		}
		else
		{
			if (dataG::data.modData.configName == "")
			{
				dataG::data.modData.configName = items[0];
				dataG::data.modData.useVanillaConfig = false;
				selectedItem = 0;
			}
			else
			{
				for (const auto& file : items) {
					if (file == dataG::data.modData.configName)
					{
						selectedItem = std::distance(items.begin(), std::find(items.begin(), items.end(), file));
						dataG::data.modData.configName = items[selectedItem];
						dataG::data.modData.useVanillaConfig = false;
						break;
					}
				}
			}
		}
		if (dataG::data.modData.useVanillaConfig == true)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImVec2 textSize = ImGui::CalcTextSize("Readme Teutonic");
		ImGui::PushItemWidth(textSize.x);
		ImGui::Combo("Config Files", &selectedItem, &items[0], items.size());
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("The mod config file you want EOP to read when it launches the game");}

		if (selectedItem != -1)
		{
			dataG::data.modData.configName = items[selectedItem];
			dataG::data.modData.useVanillaConfig = false;
		}

		if (dataG::data.modData.useVanillaConfig == true)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Checkbox("Use standard config", &dataG::data.modData.useVanillaConfig);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Don't use any mod specific config when launching the game");}

		ImGui::NewLine();

		if (!helpers::verifyModPath(helpers::checkCfgFileForMod(path + "\\" + dataG::data.modData.configName)))
		{
			// Warn the user
			if (dataG::data.modData.useVanillaConfig == false)
			{
				ImGui::Text("Warning: Mod path in %s does not match the current directory.", dataG::data.modData.configName.c_str());
			}
		}
		ImGui::Checkbox("Use M2TWEOP", &dataG::data.modData.useM2TWEOP);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable EOP when launching the mod");}

		ImGui::Checkbox("Hide launcher on startup", &dataG::data.modData.hideLauncherAtStart);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Hide the launcher when starting up. Can be shown again in eopData/uiCfg.json");}
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enabled music in the EOP Launcher");}

		ImGui::Checkbox("Discord Rich Presence", &dataG::data.gameData.isDiscordRichPresenceEnabled);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Update your Discord status/presence with details about what mod you are playing");}

		if (ImGui::SliderInt("Music volume", &dataG::data.audio.bkgMusic.musicVolume, 0, 100))
		{
			dataG::data.audio.bkgMusic.music->setVolume(dataG::data.audio.bkgMusic.musicVolume);
		}

		// Get all the TOML files in the eopData/themes folder
		std::vector<std::string> tomlFiles = helpers::getTomlFilesInFolder();
		
		// Check for the selected item
		static int selectedItem = -1;

		// Calculate text size for theme toml file
		ImVec2 textSize = ImGui::CalcTextSize("Launcher Theme: ");
		ImGui::PushItemWidth(textSize.x);

		// Display the combo box and tooltip
		if (ImGui::BeginCombo("Launcher Theme: ", dataG::data.modData.themeName.c_str()))
		{
			for (int i = 0; i < tomlFiles.size(); i++)
			{
				bool isSelected = (selectedItem == i);
				if (ImGui::Selectable(tomlFiles[i].c_str(), isSelected))
				{
					selectedItem = i;
					dataG::data.modData.themeName = tomlFiles[selectedItem];
				    setStyle(dataG::data.modData.themeName);

				}

				// Set the initial focus on the selected item
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		// Display the combo box and tooltip
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Allows you to select a custom EOP theme for the launcher"); }
	}

	void drawRulesSettings()
	{

	}

	void drawHsSettings()
	{
		ImGui::Checkbox("Automatically generate hotseat/historical battles", &dataG::data.battlesData.isGenerationNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable if you want to generate a historical battle each time you start a battle");}

		ImGui::Checkbox("Automatically generate battle result files", &dataG::data.battlesData.isGenerationNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable if you want to generate a results file from an online battle");}

		ImGui::Checkbox("Planned retreat route", &dataG::data.battlesData.isPlannedRetreatRoute);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Experimental: Allows you to set the location where armies will retreat to when attacked	");}
	}

	void drawGameSettings()
	{
		ImGui::Checkbox("Stratmap context menu", &dataG::data.modulesData.isContextMenuNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable the strategy/campaign map while pressing MMB");}

		ImGui::Checkbox("Tactical map viewer", &dataG::data.modulesData.isTacticalMapViewerNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable the tactical map viewer which allows you to view battlemaps before a battle");}

		ImGui::Checkbox("M2TWEOP developer mode", &dataG::data.modulesData.isDeveloperModeNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable to quickly debug battle maps and .worldpackage files");}
		
		ImGui::Checkbox("Block modification launch without EOP", &dataG::data.gameData.isBlockLaunchWithoutEop);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Prevent your mod from starting without EOP enabled. See FAQ for more details.");}
		
		ImGui::Checkbox("Override battle camera and controls", &dataG::data.gameData.IsOverrideBattleCamera);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("For hotseat online play");}
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