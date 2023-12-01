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
		int keypressamount = 0;
		int commandNum = 0;
	}consoleData;

	void applyCommand()
	{
		if (consoleData.input.size() > 0)
		{
			plugData::data.luaAll.logS.push_back("\n== Command == \n" + consoleData.input);
			plugData::data.luaAll.logCommands.push_back(consoleData.input);
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
			if ((ImGui::GetIO().KeysDownDuration[VK_CONTROL] > 0.f && ImGui::GetIO().KeysDownDuration['1'] > 0.f && ImGui::GetIO().KeysDownDuration['8'] > 0.f)
				&& (ImGui::GetIO().KeysDownDurationPrev[VK_CONTROL] == 0.f || ImGui::GetIO().KeysDownDurationPrev['1'] == 0.f || ImGui::GetIO().KeysDownDuration['8'] == 0.f)
				)
			{
				gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
				campaign* campaign = gameDataAll->campaignData;
				reloadLua();
				consoleData.keypressamount = 0;
				return;
			}

			if ((ImGui::GetIO().KeysDownDuration[VK_CONTROL] > 0.f && ImGui::GetIO().KeysDownDuration['1'] > 0.f && ImGui::GetIO().KeysDownDuration['9'] > 0.f)
				&& (ImGui::GetIO().KeysDownDurationPrev[VK_CONTROL] == 0.f || ImGui::GetIO().KeysDownDurationPrev['1'] == 0.f || ImGui::GetIO().KeysDownDuration['9'] == 0.f)
				)
			{
				gameDataAllStruct* gameDataAll = gameDataAllHelper::get();
				campaign* campaign = gameDataAll->campaignData;
				initLua();
				consoleData.keypressamount = 0;
				return;
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
					consoleData.keypressamount = 0;

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
		consoleData.commandNum = plugData::data.luaAll.logCommands.size();

		ImGui::Begin("##consoleInWindow", NULL, iwf);
		if (ImGui::Button("Run script"))
		{
			applyCommand();
			consoleData.keypressamount = 0;
		}


		if (ImGui::GetIO().KeysDownDuration[VK_UP] > 0.f && ImGui::GetIO().KeysDownDurationPrev[VK_UP] == 0.f && ((consoleData.commandNum - 1) - consoleData.keypressamount >= 0) )
		{
			consoleData.input = plugData::data.luaAll.logCommands[consoleData.commandNum - 1 - consoleData.keypressamount];
			consoleData.keypressamount++;
		}


		if (ImGui::GetIO().KeysDownDuration[VK_DOWN] > 0.f && ImGui::GetIO().KeysDownDurationPrev[VK_DOWN] == 0.f && consoleData.keypressamount > 0)
		{
			consoleData.keypressamount--;
			consoleData.input = plugData::data.luaAll.logCommands[consoleData.commandNum - 1 - consoleData.keypressamount];
		}

		ImGui::InputTextMultiline("##console", &consoleData.input, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_AllowTabInput);
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x,0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
		ImGui::Begin("##consoleWindow", NULL, iwf);

		ImGui::Button("Output");

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