#include "modSettingsUI.h"
#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"
#include "managerG.h"
#include "themeManagerGUI.h"
#include "../M2TWEOP Common/m2tweopConstData.h"
#include <cstdlib>  
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

#include <shellapi.h>

namespace modSettingsUI
{
	typedef void (*DrawSettingsFunc)();


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
		DrawSettingsFunc drawFunc;
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

	void addSettingsPage(const std::string& name, selectedSettingsPage_ pageId, DrawSettingsFunc drawFunc) {
		settingsPage page;
		page.pageName = name;
		page.pageId = pageId;
		page.drawFunc = drawFunc;

		settingsUIData.settingsPages.push_back(std::move(page));
	}


	void drawGeneralSettings()
	{
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
		ImVec2 textSize = ImGui::CalcTextSize(dataG::data.modData.configName.c_str());
		ImGui::PushItemWidth(textSize.x + 50);
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

		ImGui::NewLine();

		if (!helpers::verifyModPath(helpers::checkCfgFileForMod(path + "\\" + dataG::data.modData.configName)))
		{
			// Warn the user
			if (dataG::data.modData.useVanillaConfig == false)
			{
				ImGui::Text("Warning: Mod path in %s does not match the current directory.", dataG::data.modData.configName.c_str());
			}
		}
	
		ImGui::Text("Mod Version");
		ImGui::InputText("", &dataG::data.gameData.modVersion);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Mod version to display in the top left corner beneath EOP version. Set it in eopData/config/uiCfg.json");}

		ImGui::Checkbox("Use M2TWEOP", &dataG::data.modData.useM2TWEOP);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable EOP when launching the mod");}

		ImGui::Checkbox("Hide launcher on startup", &dataG::data.modData.hideLauncherAtStart);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Hide the launcher when starting up. Can be shown again in eopData/config/uiCfg.json");}
	}

	void drawLauncherSettings()
	{	
		// Discord Rich Presence settings
		ImGui::Checkbox("Discord Rich Presence", &dataG::data.gameData.isDiscordRichPresenceEnabled);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Update your Discord status/presence with details about what mod you are playing");}

		// Misc. Settings
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable music in the EOP Launcher");}

		if (ImGui::SliderInt("Music volume", &dataG::data.audio.bkgMusic.musicVolume, 0, 100))
		{
			dataG::data.audio.bkgMusic.music->setVolume(dataG::data.audio.bkgMusic.musicVolume);
		}

		// Get all the TOML files in the eopData/resources/themes folder
		std::vector<std::string> tomlFiles = helpers::getTomlFilesInFolder();
		
		// Check for the selected item
		static int selectedItem = -1;

		// Calculate text size for theme toml file
		ImVec2 textSize = ImGui::CalcTextSize("Launcher Theme: ");
		ImGui::PushItemWidth(textSize.x);

		// Display the combo box and tooltip
		if (ImGui::BeginCombo("Launcher Theme", dataG::data.modData.themeName.c_str()))
		{
			for (int i = 0; i < tomlFiles.size(); i++)
			{
				bool isSelected = (selectedItem == i);
				tomlFiles[i] = helpers::remove_extension(tomlFiles[i]);
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

		if (ImGui::Button("Open Theme Editor", helpers::getScreen().centerXButton))
		{
			std::wstring folderPath = L".\\eopData\\resources\\themes\\ImTheme";
			std::wstring exePath = L".\\eopData\\resources\\themes\\ImTheme\\ImThemes-0.2.6-amd64.exe";
			LPSTR lpwstr = helpers::ConvertWideStringToLPSTR(exePath);
			LPSTR lpwstr_folder = helpers::ConvertWideStringToLPSTR(folderPath);
			helpers::openProcess(lpwstr, lpwstr_folder);
		}
		if (ImGui::Button("Refresh theme", helpers::getScreen().centerXButton))
		{
			setStyle(dataG::data.modData.themeName);
		}
	}

	void drawRulesSettings()
	{

	}

	void drawHsSettings()
	{
		ImGui::Checkbox("Automatically generate hotseat/historical battles", &dataG::data.battlesData.isGenerationNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable if you want to generate a historical battle each time you start a battle");}

		ImGui::Checkbox("Automatically generate battle result files", &dataG::data.battlesData.isResultTransferNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable if you want to generate a results file from an online battle");}

		ImGui::Checkbox("Planned retreat route", &dataG::data.battlesData.isPlannedRetreatRoute);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Experimental: Allows you to set the location where armies will retreat to when attacked	");}
	}

	void drawGameSettings()
	{
		ImGui::Checkbox("DXVK Rendering Mode", &dataG::data.modulesData.isDXVKEnabled);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Experimental: Forces Medieval 2 to use DXVK instead of DirectX for rendering. Can massively improve performance on some hardware. \nNote: The first time you use DXVK Rendering, you may experience worse performance due to compilation of shaders. \nThe second time you launch the game, assuming the shaders have compiled, performance should be much better (even better than Vanilla DirectX Rendering)");}

		ImGui::Checkbox("Campaign Context Menu", &dataG::data.modulesData.isContextMenuNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable the context menu on the campaign map while pressing MMB");}

		ImGui::Checkbox("Battle Map Viewer", &dataG::data.modulesData.isTacticalMapViewerNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable the tactical map viewer which allows you to view the battlemap of any tile");}

		ImGui::Checkbox("M2TWEOP Developer Mode", &dataG::data.modulesData.isDeveloperModeNeeded);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Enable to quickly debug battle maps and .worldpackage files");}
		
		ImGui::Checkbox("Block mod launch without EOP", &dataG::data.gameData.isBlockLaunchWithoutEop);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Prevent your mod from starting without EOP enabled. See FAQ for more details.");}
		
		ImGui::Checkbox("Create backup saves", &dataG::data.gameData.isSaveBackupEnabled);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Automatically creates backups of Quick and Auto saves to prevent potential corruption");}

		// OpenTelelemtry Collector Settings
		ImGui::Checkbox("OpenTelemetry Collector Integration", &dataG::data.gameData.otelCollectorIntegration);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Automatically start and close the OpenTelemetry Collector application when the game is launched");}
		
		// Freecam Settings
		ImGui::Checkbox("Freecam Integration", &dataG::data.gameData.freecamIntegration);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) { ImGui::SetTooltip("Automatically start and close the Freecam application when the game is launched");}
		
		std::string configPath = dataG::data.gameData.modFolder + "\\eopData\\resources\\tools\\freecam\\config.txt";
		if (ImGui::Button("Open Freecam config", helpers::getScreen().centerXButton))
		{
			ShellExecuteA(NULL, "open", configPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
	}
	
	void drawModSettingsUI(bool* isOpen)
	{
		ImGuiIO& io = ImGui::GetIO();


		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize({800,-1}, ImGuiCond_Once);
		ImGui::Begin(eopConstData::eopVersionName, isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);



		//selectable child
		{
			ImGui::BeginChild("PreferencesSelectable", settingsUIData.selectablesSize);
			static int selected = 0;

			for (size_t n = 0; n < settingsUIData.settingsPages.size(); n++)
			{
				ImVec2 cp = ImGui::GetCursorScreenPos();
				ImVec2 cp2 = ImVec2(cp.x + settingsUIData.selectableSize.x, cp.y + settingsUIData.selectableSize.y);



				if (ImGui::Selectable(settingsUIData.settingsPages[n].pageName.c_str(), selected == n, 0, settingsUIData.selectableSize))
				{
					selected = n;
					settingsUIData.selectedPage = settingsUIData.settingsPages[n].pageId;
				}

			}

			ImGui::EndChild();
		}

		ImGui::SameLine();


		//settings content child
		{
			ImGui::BeginChild("PreferencesContent", { -1,settingsUIData.selectablesSize.y});

			for (const auto& page : settingsUIData.settingsPages) {
				if (settingsUIData.selectedPage == page.pageId) {
					page.drawFunc(); 
				}
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

	void initSettingsUI()
	{
		settingsUIData.settingsPages.clear();

		addSettingsPage("General", selectedSettingsPage_general, drawGeneralSettings);
		addSettingsPage("Launcher", selectedSettingsPage_launcher, drawLauncherSettings);
		addSettingsPage("Game", selectedSettingsPage_game, drawGameSettings);
		addSettingsPage("Hotseat", selectedSettingsPage_hs, drawHsSettings);


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
};