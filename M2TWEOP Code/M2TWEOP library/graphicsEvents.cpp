#include "pch.h"
#include "graphicsEvents.h"
#include "console.h"
#include "luaPlugin.h"
#include "globals.h"

struct
{
	float drawInfoEndTime = 0;
	bool drawEopStartInfo = false;

	ImVec2 beginCoords{ 0.f,0.f };
}drawParams;
void drawOnEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (plugData::data.luaAll.drawLuaFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.drawLuaFunc)(pDevice))
	}

	console::draw();

	if (drawParams.drawEopStartInfo == true)
	{
		if (const auto currentTime = static_cast<float>(ImGui::GetTime()); currentTime < drawParams.drawInfoEndTime)
		{
			static ImGuiWindowFlags transparentF = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

			if ((globals::dataS.gameCfg.modVersion).length() > 0){
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
				ImGui::Begin("eopInitTitle", nullptr, transparentF);
					ImGui::Text(globals::dataS.gameCfg.modVersion.c_str());
				ImGui::End();
			}
		}
		else
		{
			drawParams.drawEopStartInfo = false;
		}
	}

	if(consoleData.shouldReloadScript){
		reloadLua();
		consoleData.shouldReloadScript = false;
	}

	if(consoleData.shouldRestartLua){
		initLuaPlugin(true);
		consoleData.shouldRestartLua = false;
	}
}

void onReset(LPDIRECT3DDEVICE9 pDevice)
{
	if (plugData::data.luaAll.resetDXFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.resetDXFunc)(pDevice))
	}
}
void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice)
{
	if (plugData::data.luaAll.onLoadingFonts != nullptr)
	{
		tryLua((*plugData::data.luaAll.onLoadingFonts)(pDevice))
	}
}

void onChangeImGuiContext(ImGuiContext* imCtx, const ImGuiMemAllocFunc allocFunc, const ImGuiMemFreeFunc freeFunc, void* userData)
{
	if (plugData::data.luaAll.initDXFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.initDXFunc)())
	}

	//do not delete this line!!!!!!!!!
	ImGui::SetCurrentContext(imCtx);
	ImGui::SetAllocatorFunctions(allocFunc, freeFunc, userData);
	drawParams.drawEopStartInfo = true;
	drawParams.drawInfoEndTime = static_cast<float>(ImGui::GetTime()) + 20.0f;
}

void onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


}
