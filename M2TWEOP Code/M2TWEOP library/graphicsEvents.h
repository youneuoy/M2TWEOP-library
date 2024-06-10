#pragma once
#include "pluginM2TWEOP.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "imgui/imgui.h"


extern "C" PLUGINM2TWEOP_API void drawOnEndScene(LPDIRECT3DDEVICE9 pDevice);


extern "C" PLUGINM2TWEOP_API void onReset(LPDIRECT3DDEVICE9 pDevice);
extern "C" PLUGINM2TWEOP_API void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice);


extern "C" PLUGINM2TWEOP_API void onChangeImGuiContext(ImGuiContext* imCtx, ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void* userData);

extern "C" PLUGINM2TWEOP_API void onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

