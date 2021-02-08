#include "pch.h"
#include "graphicsD3D.h"
#include "plugins.h"
graphicsD3D::dataT graphicsD3D::dataS;


template<typename T>
T FnCast(uint32_t fnToCast, T pFnCastTo) {
	(void)pFnCastTo;
	return (T)fnToCast;
}
NOINLINE LRESULT APIENTRY graphicsD3D::hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	plugins::onWindowProc(hWnd, uMsg, wParam, lParam);

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
	if (dataS.ifMouseOrKeyBoardAtImgui)
	{
		return true;
	}


	return CallWindowProc(dataS.hookD.onewGameWndProc,hWnd, uMsg, wParam, lParam);
}
NOINLINE void graphicsD3D::Draw(LPDIRECT3DDEVICE9 pDevice)
{
	plugins::onEndScene(pDevice);


	return;
}
NOINLINE LRESULT APIENTRY graphicsD3D::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	return CallWindowProc(dataS.hookD.oWndProc, hWnd, uMsg, wParam, lParam);
}


NOINLINE HRESULT APIENTRY graphicsD3D::hkEndScene(IDirect3DDevice9* pDevice)
{
	if (!dataS.ImInitialized)
	{
		initImgGui(pDevice);
	}


	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Draw(pDevice);



	dataS.ifMouseOrKeyBoardAtImgui = ImGui::GetIO().WantCaptureMouse;
	dataS.ifMouseOrKeyBoardAtImgui |= ImGui::GetIO().WantCaptureKeyboard;

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return FnCast(dataS.hookD.oEndScene, &hkEndScene)(pDevice);
}

NOINLINE HRESULT APIENTRY graphicsD3D::hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (dataS.ImInitCount > 0)
	{
		D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		dataS.ImInitialized = false;
	}
	return FnCast(dataS.hookD.oReset,&hkReset)(pDevice, pPresentationParameters);
}

NOINLINE void graphicsD3D::initImgGui(IDirect3DDevice9* pDevice)
{
	dataS.ImInitCount++;

	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;


	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.MouseDrawCursor = false;
	string f = globals::dataS.modPatch;
	if (f.size() == 0)
	{
		MessageBoxA(NULL, "graphics init error", "Error", NULL);
		exit(0);
	}
	f = f + "\\youneuoy_Data\\inGame.ttf";


	io.Fonts->AddFontFromFileTTF(f.c_str(), 24, &font_config, io.Fonts->GetGlyphRangesCyrillic());

	ImGui_ImplWin32_Init(dataS.Window);
	ImGui_ImplDX9_Init(pDevice);

	dataS.ImInitialized = true;
	return;
}

bool graphicsD3D::init()
{
	IMGUI_CHECKVERSION();
	ImGuiContext* imCtx = ImGui::CreateContext();
	plugins::onChangeImGuiCtx(imCtx);


	while (dataS.Window == nullptr)
	{
		dataS.Window = FindWindowA(0, ("Medieval 2"));
		if (dataS.Window) break;
		Sleep(20);
	}

	dataS.hookD.oWndProc = (WNDPROC)SetWindowLongA(dataS.Window, GWL_WNDPROC, (LONG)hkWndProc);

	if (dataS.hookD.oWndProc == nullptr) return false;

	return true;
}

DWORD __stdcall graphicsD3D::InitS()
{
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	init();


	if (GetD3D9Device(dataS.d3d9Device, sizeof(dataS.d3d9Device)))
	{
		if (MH_CreateHook((char*)dataS.d3d9Device[42], &hkEndScene,
			reinterpret_cast<LPVOID*>(&dataS.hookD.oEndScene)) != MH_OK)
		{
			return 1;
		}
		if (MH_EnableHook((char*)dataS.d3d9Device[42]) != MH_OK)
		{
			return 1;
		}


		if (MH_CreateHook((char*)dataS.d3d9Device[16], &hkReset,
			reinterpret_cast<LPVOID*>(&dataS.hookD.oReset)) != MH_OK)
		{
			return 1;
		}
		if (MH_EnableHook((char*)dataS.d3d9Device[16]) != MH_OK)
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
	DWORD adr = 0;
	if (globals::dataS.gamever == 2)//steam
	{
		adr = 0x01178a40;
	}
	else
	{
		adr = 0x011780d0;
	}

	if (MH_CreateHook((char*)adr, &hkWndProc2,
		reinterpret_cast<LPVOID*>(&dataS.hookD.onewGameWndProc)) != MH_OK)
	{
		return 1;
	}
	if (MH_EnableHook((char*)adr) != MH_OK)
	{
		return 1;
	}

	return 1;
}

bool graphicsD3D::GetD3D9Device(void** pTable, size_t Size)
{
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = NULL;


	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = dataS.Window;

	HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDeviceCreated != S_OK)
	{

		d3dpp.Windowed = !d3dpp.Windowed;

		dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDeviceCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

	pDummyDevice->Release();
	pD3D->Release();
	return true;
}