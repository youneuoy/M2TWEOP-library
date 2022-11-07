#include "console.h"
#include "imgui_stdlib.h"
#include "plugData.h"
#include "pluginM2TWEOP.h"

#include "gameDataAllHelper.h"
namespace console
{
	struct
	{
		std::string input;
		
		bool isDraw = false;
	}consoleData;

	void applyCommand()
	{
		if (consoleData.input.size() > 0)
		{
			plugData::data.luaAll.logS.push_back("Command: "+consoleData.input+'\n');
		}
		else
		{
			return;
		}


		plugData::data.luaAll.runScriptS(&consoleData.input);

		consoleData.input.clear();
	}

	void draw()
	{
			if ((ImGui::GetIO().KeysDownDuration[VK_CONTROL] > 0.f && ImGui::GetIO().KeysDownDuration['1'] > 0.f && ImGui::GetIO().KeysDownDuration['9'] > 0.f)
				&& (ImGui::GetIO().KeysDownDurationPrev[VK_CONTROL] == 0.f || ImGui::GetIO().KeysDownDurationPrev['1'] == 0.f || ImGui::GetIO().KeysDownDuration['9'] == 0.f)
				)
			{
				gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
				campaign* campaign = gameDataAll->campaignData;
				//if (campaign->isAdminPasswordExist == false || (campaign->isAdminPasswordExist == true && campaign->isHotseatLogon == true))
				//{
					initLua();
					return;
				//}
			}

			if ((ImGui::GetIO().KeysDownDuration[VK_CONTROL] > 0.f && ImGui::GetIO().KeysDownDuration['1'] > 0.f)
				&& (ImGui::GetIO().KeysDownDurationPrev[VK_CONTROL] == 0.f || ImGui::GetIO().KeysDownDurationPrev['1'] == 0.f)
				)
			{
				gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
				campaign* campaign = gameDataAll->campaignData;
			//	if (campaign->isAdminPasswordExist == false || (campaign->isAdminPasswordExist == true && campaign->isHotseatLogon == true))
			//	{
					consoleData.isDraw = !consoleData.isDraw;

					if (plugData::data.luaAll.checkVar("enableConsole", 1) == false)
					{
						consoleData.isDraw = false;
					}
				//}
			}

		if (consoleData.isDraw == false)
		{
			return;
		}



		ImGuiWindowFlags iwf = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0.5f, 0.5f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));

		ImGui::Begin("##consoleInWindow", NULL, iwf);
		if (ImGui::Button("Run"))
		{
			applyCommand();
		}

		ImGui::InputTextMultiline("##console", &consoleData.input, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_AllowTabInput);
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x,0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
		ImGui::Begin("##consoleWindow", NULL, iwf);

		std::string outputs;

		for (auto& str : plugData::data.luaAll.logS)
		{
			outputs += str;
			outputs += "\n";
		}

		ImGui::InputTextMultiline("##consoleLog", &outputs, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);

		ImGui::End();
	}

}