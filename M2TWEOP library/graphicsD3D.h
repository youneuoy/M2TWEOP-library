#pragma once
#include"headersSTD.h"
#include "headersMEM.h"
#include "headersGraphics.h"
#include "globals.h"


#include "exportHeader.h"

class graphicsD3D
{
public:
	static struct dataT
	{
		void* d3d9Device[119];
		IDirect3DDevice9* pDevice = nullptr;

		HWND Window = nullptr;



		//if imgui initialized
		bool ImInitialized = false;
		int ImInitCount = 0;
		bool ifMouseOrKeyBoardAtImgui = false;
		struct 
		{
			WNDPROC oWndProc = NULL;
			// Originals
			WNDPROC onewGameWndProc = NULL;
			uint32_t oEndScene = NULL;
			uint32_t oReset = NULL;
			uint32_t oPresent = NULL;
			uint32_t oBeginScene = NULL;
			uint32_t oGetRenderTargetData = NULL;

		}hookD;
	}dataS;
	static NOINLINE void APIENTRY Draw(LPDIRECT3DDEVICE9 pDevice);



	static NOINLINE LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE LRESULT APIENTRY hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pDevice);

	static NOINLINE HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	static NOINLINE void APIENTRY initImgGui(IDirect3DDevice9* pDevice);
	static bool init();
	static DWORD WINAPI InitS();
	static bool GetD3D9Device(void** pTable, size_t Size);


};


namespace graphicsExport
{
	NOINLINE EOP_EXPORT LPDIRECT3DTEXTURE9* loadTexture(const char*path, int* x, int* y);
};