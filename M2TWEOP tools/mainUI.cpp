#include "mainUI.h"
#include "headersSTD.h"
#include "helpers.h"
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
		bool isAboutOpen = false;
	}childs;


	int draw(bool* isOpen)
	{
		if (childs.isAboutOpen == true)
		{
		//	aboutUI::drawAboutUi(&childs.isAboutOpen);
			return 0;
		}

		ImVec2 windowSize = ImGui::CalcTextSize("Run vanilla or dlc(no M2TWEOP capabilities)");
		windowSize.x *= 1.5;

		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(windowSize.x, -1), ImGuiCond_Once);
		ImGui::Begin("M2TWEOP tools", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


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