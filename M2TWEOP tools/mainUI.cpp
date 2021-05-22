#include "mainUI.h"
#include "headersSTD.h"
#include "helpers.h"

#include "modelsFilesUI.h"
/*#include "dataG.h"

#include "aboutUI.h"
#include "gameSTDUI.h"
#include "modSettingsUI.h"
#include "gameStarter.h"
#include "gameRunnerUI.h"*/

#include <shellapi.h>

namespace mainUI
{
	struct
	{
		bool isModelsFilesOpen = false;
	}childs;


	int draw(bool* isOpen)
	{

		if (childs.isModelsFilesOpen == true)
		{
			modelsFilesUI::draw(&childs.isModelsFilesOpen);
			return 0;
		}

		ImVec2 windowSize = ImGui::CalcTextSize("Adapt m2tweop v1.x models change files");
		windowSize.x *= 1.5;

		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, -1), ImGuiCond_Once);
		ImGui::Begin("M2TWEOP tools", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


		if (ImGui::Button("Convert m2tweop v1.x stratmodels files", helpers::getScreen().centerXButton))
		{
			childs.isModelsFilesOpen = true;
		}

		ImGui::End();

		if (*isOpen == 0)
		{
			return 1;
		}

		return 0;
	}
};