#pragma once

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "imgui/imgui.h"


void drawOnEndScene(LPDIRECT3DDEVICE9 pDevice);


void onReset(LPDIRECT3DDEVICE9 pDevice);
void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice);


void onChangeImGuiContext(ImGuiContext* imCtx, ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void* userData);

void onWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

