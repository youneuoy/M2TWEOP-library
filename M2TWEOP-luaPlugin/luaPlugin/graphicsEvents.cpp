#include "graphicsEvents.h"
#include "console.h"

struct
{
	float drawInfoEndTime = 0;
	bool drawEOPStartInfo = false;

	ImVec2 beginCoords{ 0.f,0.f };
}drawParams;
void drawOnEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (plugData::data.luaAll.drawLuaFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.drawLuaFunc)(pDevice));
	}

	console::draw();

	if (drawParams.drawEOPStartInfo == true)
	{
		float currTime = (float)ImGui::GetTime();

		if (currTime < drawParams.drawInfoEndTime)
		{
			static ImGuiWindowFlags transparentF = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::Begin("eopInitTitle", nullptr, transparentF);

			ImGui::Text("M2TWEOP LUA PLUGIN");

			ImGui::End();
		}
		else
		{
			drawParams.drawEOPStartInfo = false;
		}
	}
}

void onReset(LPDIRECT3DDEVICE9 pDevice)
{
	if (plugData::data.luaAll.resetDXFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.resetDXFunc)(pDevice));
	}
}
void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice)
{
	if (plugData::data.luaAll.onLoadingFonts != nullptr)
	{
		tryLua((*plugData::data.luaAll.onLoadingFonts)(pDevice));
	}
}



void onChangeImGuiContext(ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)
{
	if (plugData::data.luaAll.initDXFunc != nullptr)
	{
		tryLua((*plugData::data.luaAll.initDXFunc)());
	}

	//do not delete this line!!!!!!!!!
	ImGui::SetCurrentContext(imCtx);
	ImGui::SetAllocatorFunctions(alloc_func, free_func, user_data);
	drawParams.drawEOPStartInfo = true;
	drawParams.drawInfoEndTime = (float)ImGui::GetTime() + 20.0f;
}

void onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


}
