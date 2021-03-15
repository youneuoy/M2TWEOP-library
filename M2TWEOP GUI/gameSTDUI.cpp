#include "gameSTDUI.h"

#include "headersSTD.h"
#include "dataG.h"
#include "helpers.h"
#include "gameStarter.h"
namespace gameSTDUI
{
	struct
	{
		int campaign = 0;
	}gameSTDUIData;


	void drawSTDUI(bool* isOpen)
	{
		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(helpers::getScreen().screenSize.x * 0.4f, -1.f));
		ImGui::Begin("Select campaign", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


		if (ImGui::Button("Grand campaign", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 1;
		}
		ImGui::NewLine();
		if (ImGui::Button("Americas", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 2;
		}
		if (ImGui::Button("British isles", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 3;
		}
		if (ImGui::Button("Crusades", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 4;
		}
		if (ImGui::Button("Teutonic", helpers::getScreen().centerXButton))
		{
			gameSTDUIData.campaign = 5;
		}


		ImGui::End();


		if (gameSTDUIData.campaign!=0)
		{
			dataG::data.gameData.gameMode = gameSTDUIData.campaign;

			gameSTDUIData.campaign = 0;


			gameStarter::startGame();
		}
	}
};