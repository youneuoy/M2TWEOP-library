#include "console.h"
#include "imgui_stdlib.h"
#include "plugData.h"
#include "pluginM2TWEOP.h"

#include "imgui/imgui.h"

namespace console
{
	struct
	{
		std::string input;
		
		bool isDraw = false;
		int pressAmount = 0;
		int commandNum = 0;
		bool shouldRestartLua   = false;
		bool shouldReloadScript = false;
	}consoleData;

	void applyCommand()
	{
		if (!consoleData.input.empty())
		{
			luaP::logS.push_back("\n== Command == \n" + consoleData.input);
			luaP::logCommands.push_back(consoleData.input);
		}
		else
		{
			return;
		}

		plugData::data.luaAll.runScriptS(&consoleData.input);

		consoleData.input.clear();
	}

	void toggleConsole()
	{
		consoleData.isDraw = !consoleData.isDraw;
		consoleData.pressAmount = 0;

		if (plugData::data.luaAll.checkVar("enableConsole", 1) == false)
		{
			consoleData.isDraw = false;
		}
	}

	void reloadScript()
	{
		plugData::data.luaAll.hashLoaded = false;
		consoleData.shouldReloadScript = true;
		consoleData.pressAmount = 0;
	}

	void restartLua()
	{
		plugData::data.luaAll.hashLoaded = false;
		consoleData.shouldRestartLua = true;
		consoleData.pressAmount = 0;
	}

	void draw()
	{
		if (consoleData.isDraw == false)
		{
			return;
		}

		constexpr ImGuiWindowFlags iwf = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0.5f, 0.5f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
		consoleData.commandNum = luaP::logCommands.size();

		ImGui::Begin("##consoleInWindow", nullptr, iwf);
		if (ImGui::Button("Run script"))
		{
			applyCommand();
			consoleData.pressAmount = 0;
		}


		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)
			&& ((consoleData.commandNum - 1) - consoleData.pressAmount >= 0) )
		{
			consoleData.input = luaP::logCommands[consoleData.commandNum - 1 - consoleData.pressAmount];
			consoleData.pressAmount++;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)
			&& consoleData.pressAmount > 0)
		{
			consoleData.pressAmount--;
			consoleData.input = luaP::logCommands[consoleData.commandNum - 1 - consoleData.pressAmount];
		}

		ImGui::InputTextMultiline("##console", &consoleData.input, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_AllowTabInput);
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x,0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
		ImGui::Begin("##consoleWindow", nullptr, iwf);

		ImGui::Button("Output");

		std::string outputs;

		for (const auto& str : luaP::logS)
		{
			outputs += str;
			outputs += "\n";
		}

		ImGui::InputTextMultiline("##consoleLog", &outputs, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);

		ImGui::End();

		if(consoleData.shouldReloadScript){
			reloadLua();
			consoleData.shouldReloadScript = false;
		}

		if(consoleData.shouldRestartLua){
			initLua();
			consoleData.shouldRestartLua = false;
		}
	}

}
