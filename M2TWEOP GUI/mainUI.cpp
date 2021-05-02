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
		ImVec2 windowSize = ImGui::CalcTextSize("Run vanilla or dlc(no M2TWEOP capabilities)");
		windowSize.x *= 1.5;

		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, -1), ImGuiCond_Once);
		ImGui::Begin("M2TWEOP", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		ImGui::NewLine();
		if (ImGui::Button("Run vanilla or dlc(no M2TWEOP capabilities)", helpers::getScreen().centerXButton))
		{
			childs.isGameSTDMenuOpen = true;
		}
		if (ImGui::Button("Run mod", helpers::getScreen().centerXButton))
		{
			dataG::data.gameData.gameMode = 7;
			gameStarter::startGame();
			childs.isGameRunnerUIOpen = true;
			dataG::data.gameData.gameMode = 0;
		}
		if (ImGui::Button("Mod settings", helpers::getScreen().centerXButton))
		{
			childs.isModSettingsUIOpen = true;
		}
		ImGui::NewLine();
		if (ImGui::Button("About M2TWEOP", helpers::getScreen().centerXButton))
		{
			childs.isAboutOpen = true;
		}		
		if (ImGui::Button("Open M2TWEOP documentation", helpers::getScreen().centerXButton))
		{
			HINSTANCE res=ShellExecuteA(NULL, "open", "eopData\\helpPages\\index.html", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::End();

		if (*isOpen == 0)
		{
			return 1;
		}

		return 0;
	}
};