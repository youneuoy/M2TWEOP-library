#include "pch.h"
#include "console.h"
#include "luaPlugin.h"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

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
			luaPlugin::logS.push_back("\n== Command == \n" + consoleData.input);
			luaPlugin::logCommands.push_back(consoleData.input);
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
		consoleData.commandNum = luaPlugin::logCommands.size();

		ImGui::Begin("##consoleInWindow", nullptr, iwf);
		if (ImGui::Button("Run script"))
		{
			applyCommand();
			consoleData.pressAmount = 0;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Enter)
			and ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		{
			applyCommand();
			consoleData.pressAmount = 0;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)
			&& ((consoleData.commandNum - 1) - consoleData.pressAmount >= 0) )
		{
			consoleData.input = luaPlugin::logCommands[consoleData.commandNum - 1 - consoleData.pressAmount];
			consoleData.pressAmount++;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)
			&& consoleData.pressAmount > 0)
		{
			consoleData.pressAmount--;
			consoleData.input = luaPlugin::logCommands[consoleData.commandNum - 1 - consoleData.pressAmount];
		}

		if (ImGui::InputTextMultiline("##console", &consoleData.input, ImVec2(-FLT_MIN, -FLT_MIN),
			ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
			{
				applyCommand();
				consoleData.pressAmount = 0;
			}
		}
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x,0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
		ImGui::Begin("##consoleWindow", nullptr, iwf);

		ImGui::Button("Output");

		std::string outputs;

		for (const auto& str : luaPlugin::logS)
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
			initLuaPlugin();
			consoleData.shouldRestartLua = false;
		}
	}

}
