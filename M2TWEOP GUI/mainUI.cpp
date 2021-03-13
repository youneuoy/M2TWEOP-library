#include "mainUI.h"
#include "headersSTD.h"
#include "helpers.h"
#include "dataG.h"

#include "aboutUI.h"
#include "gameSTDUI.h"
namespace mainUI
{
	struct
	{
		bool isAboutOpen = false;
		bool isGameSTDMenuOpen = false;
	}childs;
	void draw(bool* isOpen)
	{
		if (childs.isAboutOpen == true)
		{
			aboutUI::drawAboutUi(&childs.isAboutOpen);
			return;
		}
		if (childs.isGameSTDMenuOpen==true)
		{
			gameSTDUI::drawSTDUI(&childs.isGameSTDMenuOpen);
			return;
		}

		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(helpers::getScreen().screenSize.x * 0.5f, helpers::getScreen().screenSize.y * 0.8f), ImGuiCond_Once);
		ImGui::Begin("##Main menu", isOpen, ImGuiWindowFlags_NoCollapse);

		ImGui::NewLine();
		if (ImGui::Button("Run vanilla or dlc(no M2TWEOP capabilities)", helpers::getScreen().centerXButton))
		{
			childs.isGameSTDMenuOpen = true;
		}
		if (ImGui::Button("About", helpers::getScreen().centerXButton))
		{
			childs.isAboutOpen = true;
		}
		ImGui::End();

	}
};