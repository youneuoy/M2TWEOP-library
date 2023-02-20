#pragma once
#include "pluginM2TWEOP.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"


extern "C" PLUGINM2TWEOP_API void drawOnEndScene(LPDIRECT3DDEVICE9 pDevice);


extern "C" PLUGINM2TWEOP_API void onReset(LPDIRECT3DDEVICE9 pDevice);
extern "C" PLUGINM2TWEOP_API void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice);


extern "C" PLUGINM2TWEOP_API void onChangeImGuiContext(ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);

extern "C" PLUGINM2TWEOP_API void onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

