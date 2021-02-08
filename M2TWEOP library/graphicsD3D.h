#pragma once
#include"headersSTD.h"
#include "headersMEM.h"
#include "globals.h"
class graphicsD3D
{
public:
	static struct dataT
	{
		void* d3d9Device[119];
		HWND Window = nullptr;


		struct 
		{
			WNDPROC oWndProc = NULL;
			// Originals
			uint64_t onewGameWndProc = NULL;
			uint64_t oEndScene = NULL;
			uint64_t oReset = NULL;
		}hookD;
	}dataS;

	static NOINLINE LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE LRESULT APIENTRY hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pDevice);
	static NOINLINE HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);


	static bool init();
	static DWORD WINAPI InitS();
	static bool GetD3D9Device(void** pTable, size_t Size);
};

