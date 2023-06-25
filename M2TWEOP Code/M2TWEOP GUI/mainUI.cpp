#include "mainUI.h"
#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"

#include "aboutUI.h"
#include "gameSTDUI.h"
#include "modSettingsUI.h"
#include "gameStarter.h"
#include "gameRunnerUI.h"

#include <shellapi.h>

#include "discord.h"
discord::Core* core{};

namespace mainUI
{
	struct
	{
		bool isAboutOpen = false;
		bool isGameSTDMenuOpen = false;

		bool isModSettingsUIOpen = false;

		bool isGameRunnerUIOpen = false;
	}childs;
	void tryJustStartMod()
	{
		if (dataG::data.modData.hideLauncherAtStart == false)
		{
			return;
		}

		dataG::data.gameData.gameMode = 7;
		gameStarter::startGame();
		childs.isGameRunnerUIOpen = true;
		dataG::data.gameData.gameMode = 0;
	}


	int draw(bool* isOpen)
	{
		::core->RunCallbacks();

		if (childs.isAboutOpen == true)
		{
			aboutUI::drawAboutUi(&childs.isAboutOpen);
			return 0;
		}
		else if (childs.isGameSTDMenuOpen == true)
		{
			gameSTDUI::drawSTDUI(&childs.isGameSTDMenuOpen);
			return 0;
		}
		else if (childs.isModSettingsUIOpen == true)
		{
			modSettingsUI::drawModSettingsUI(&childs.isModSettingsUIOpen);
			return 0;
		}
		else if (childs.isGameRunnerUIOpen == true)
		{
			gameRunnerUI::drawUI(&childs.isGameRunnerUIOpen);
			return 0;
		}
		ImVec2 windowSize = ImGui::CalcTextSize("Run Vanilla or DLC without M2TWEOP");
		windowSize.x *= 1.5;

		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, -1), ImGuiCond_Once);

		// Set the title of the window and the main highlighted button to
		// the name of the mod as defined in uiCfg.json
		string windowTitle;
		string runTitle;
		string modTitle = dataG::data.gameData.modTitle;

		if(modTitle == "") {
			windowTitle = "M2TWEOP";
			runTitle = "Run mod";
		}
		else {
			windowTitle = modTitle;
			runTitle =  "Run " + modTitle;
		}

		ImGui::Begin(windowTitle.c_str(), isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		const char logoName[] = "eopData/images/logoAbout.png";
		GLImage* imgFinded = helpers::findImage("eopData/images/logoAbout.png", sizeof logoName-1);

		ImVec2 const csz = ImGui::GetContentRegionAvail();
		ImVec2 logoSize = ImVec2((float)imgFinded->xSize, (float)imgFinded->ySize);
		if (logoSize.x > csz.x) {
			const float r = logoSize.y / logoSize.x;
			logoSize.x = csz.x;
			logoSize.y = csz.x * r;
		}
		ImGui::SetCursorPosX((windowSize.x - logoSize.x) * 0.5f);
		ImGui::Image((void*)(intptr_t)imgFinded->image, logoSize);

		if(dataG::data.gameData.buttonColorString == "") {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.584, 0.270, 0.250, 1.0f));
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(
				dataG::data.gameData.buttonColor.r,
				dataG::data.gameData.buttonColor.g,
				dataG::data.gameData.buttonColor.b,
				dataG::data.gameData.buttonColor.a,
			));
		}

		if(dataG::data.gameData.buttonHoverColorString == "") {
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.564, 0.250, 0.230, 1.0f));
		} else {
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(
				dataG::data.gameData.buttonHoverColor.r,
				dataG::data.gameData.buttonHoverColor.g,
				dataG::data.gameData.buttonHoverColor.b,
				dataG::data.gameData.buttonHoverColor.a,
			));
		}

		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.804, 0.490, 0.470, 1.0f));
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		const ImVec2 label_size = ImGui::CalcTextSize(runTitle.c_str(), NULL, true);

		if (ImGui::Button(runTitle.c_str(), ImVec2(-1.0f,(label_size.y + style.FramePadding.y * 2.0f)*2)))
		{
			dataG::data.gameData.gameMode = 7;
			gameStarter::startGame();
			childs.isGameRunnerUIOpen = true;
			dataG::data.gameData.gameMode = 0;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		if (ImGui::Button("Settings", helpers::getScreen().centerXButton))
		{
			childs.isModSettingsUIOpen = true;
		}
		ImGui::NewLine();
		if (ImGui::Button("Run Vanilla or DLC without M2TWEOP", helpers::getScreen().centerXButton))
		{
			childs.isGameSTDMenuOpen = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("M2TWEOP Documentation", helpers::getScreen().centerXButton))
		{
			ShellExecuteA(NULL, "open", "eopData\\helpPages\\index.html", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::Button("M2TWEOP Website", helpers::getScreen().centerXButton))
		{
			ShellExecuteA(NULL, "open", "https://youneuoy.github.io/M2TWEOP-library/ ", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::Button("About M2TWEOP", helpers::getScreen().centerXButton))
		{
			childs.isAboutOpen = true;
		}
		ImGui::End();

		if (*isOpen == 0)
		{
			return 1;
		}

		return 0;
	}
};