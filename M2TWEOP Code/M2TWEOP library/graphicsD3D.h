#pragma once
#include"headersSTD.h"
#include "headersMEM.h"
#include "headersGraphics.h"
#include "globals.h"
#include "techFuncs.h"

#include "exportHeader.h"
#include "imgui_notify.h"

typedef void (*EOPDrawCallback)();
class graphicsD3D
{
public:
	static struct dataT
	{
		IDirect3DDevice9* pDevice = nullptr;

		//state of fresh created device
		IDirect3DStateBlock9* clearStateBlock = NULL;

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

		vector<EOPDrawCallback> stratmapDrawCallbacks;
		vector<EOPDrawCallback> imguiDrawCallbacks;
	}dataS;
	static NOINLINE void APIENTRY Draw(LPDIRECT3DDEVICE9 pDevice);

	static NOINLINE void APIENTRY onDrawPartsOfStratObjects();
	static NOINLINE void APIENTRY onDrawAllGameStuff();



	static NOINLINE LRESULT APIENTRY hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static NOINLINE LRESULT APIENTRY hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static NOINLINE void APIENTRY initImgGui(IDirect3DDevice9* pDevice);
	static bool init();
	static DWORD WINAPI InitS();


};


namespace graphicsExport
{
	struct D3dState
	{
		IDirect3DStateBlock9* d3d9_state_block;
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
	D3dState GetD3dState();

	void AddStratmapDrawCallback(EOPDrawCallback callFunk);
	void AddImGuiDrawCallback(EOPDrawCallback callFunk);

	void SetClearD3dState();
	void SetD3dState(D3dState& state);
	void ReleaseD3dState(D3dState& state);


	const D3DXMATRIXA16* GetMatView();
	const D3DXMATRIXA16* GetMatProj();

	IDirect3DDevice9* GetDevice();
	LPDIRECT3DTEXTURE9 loadTexture(const char* path, int* x, int* y);
	void unloadTexture(LPDIRECT3DTEXTURE9 texture);


	void onCreateDevice(IDirect3DDevice9* pDevice);
	void onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
};