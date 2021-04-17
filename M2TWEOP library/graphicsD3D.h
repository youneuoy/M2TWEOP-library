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
		IDirect3DDevice9* pDevice = nullptr;

		HWND Window = nullptr;



		//if imgui initialized
		bool ImInitialized = false;
		bool ifMouseOrKeyBoardAtImgui = false;
		struct 
		{
			WNDPROC oWndProc = NULL;
			// Originals
			WNDPROC onewGameWndProc = NULL;
		}hookD;
	}dataS;
	static NOINLINE void APIENTRY Draw(LPDIRECT3DDEVICE9 pDevice);



	static NOINLINE LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE LRESULT APIENTRY hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static NOINLINE void APIENTRY initImgGui(IDirect3DDevice9* pDevice);
	static bool init();
	static DWORD WINAPI InitS();


};


namespace graphicsExport
{
	NOINLINE EOP_EXPORT LPDIRECT3DTEXTURE9 loadTexture(const char*path, int* x, int* y);
	NOINLINE EOP_EXPORT void unloadTexture(LPDIRECT3DTEXTURE9 texture);


	NOINLINE EOP_EXPORT void onCreateDevice(IDirect3DDevice9* pDevice);
	NOINLINE EOP_EXPORT void onEndScene(IDirect3DDevice9* pDevice);
	NOINLINE EOP_EXPORT void onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	NOINLINE EOP_EXPORT void afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
};