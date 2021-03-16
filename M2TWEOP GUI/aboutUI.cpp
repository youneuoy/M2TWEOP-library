#include "headersSTD.h"
#include "aboutUI.h"
#include "aboutUIHelpers.h"
#include "dataG.h"
#include "helpers.h"

namespace aboutUI
{
	void drawAboutUi(bool* isOpen)
	{
		ImGui::SetNextWindowPos(helpers::getScreen().screenHalfSize, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(helpers::getScreen().screenSize.x * 0.8f, -1));
		ImGui::Begin("About M2TWEOP", isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		MarkdownExample();
		ImGui::End();
	}
};