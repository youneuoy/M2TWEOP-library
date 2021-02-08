#include "pch.h"
#include "graphicsD3D.h"
graphicsD3D::dataT graphicsD3D::dataS;


template<typename T>
T FnCast(uint64_t fnToCast, T pFnCastTo) {
	(void)pFnCastTo;
	return (T)fnToCast;
}
NOINLINE LRESULT graphicsD3D::hkWndProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return FnCast(dataS.hookD.onewGameWndProc,&hkWndProc2)(hWnd, uMsg, wParam, lParam);
}

NOINLINE LRESULT graphicsD3D::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(dataS.hookD.oWndProc, hWnd, uMsg, wParam, lParam);
}


NOINLINE HRESULT graphicsD3D::hkEndScene(IDirect3DDevice9* pDevice)
{
	return FnCast(dataS.hookD.oEndScene, &hkEndScene)(pDevice);
}

NOINLINE HRESULT graphicsD3D::hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return FnCast(dataS.hookD.oReset,&hkReset)(pDevice, pPresentationParameters);
}

bool graphicsD3D::init()
{
/*	IMGUI_CHECKVERSION();
	ImGuiContext* imCtx = ImGui::CreateContext();*/
	//	plugins::onChangeImGuiCtx(imCtx);

		//menuStopTime = GetTickCount() + 7000;
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
/*	DWORD adr = 0;
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
	}*/

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