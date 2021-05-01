#include "pch.h"
#include "graphicsD3D.h"
#include "plugins.h"
#include <DxErr.h>
#pragma comment(lib, "DXERR.lib")




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


	return CallWindowProc(dataS.hookD.onewGameWndProc, hWnd, uMsg, wParam, lParam);
}

struct
{
	float drawInfoEndTime = 0;
	bool drawEOPStartInfo=false;

	ImVec2 beginCoords{ 0.f,0.f };
}drawParams;
NOINLINE void graphicsD3D::Draw(LPDIRECT3DDEVICE9 pDevice)
{

	plugins::onEndScene(pDevice);

	if (drawParams.drawEOPStartInfo == true)
	{
		float currTime = (float)ImGui::GetTime();

		if (currTime < drawParams.drawInfoEndTime)
		{
			static ImGuiWindowFlags transparentF = ImGuiWindowFlags_NoBackground| ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoMove;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::Begin("eopInitTitle", nullptr, transparentF);

			ImGui::Text("M2TWEOP 2.0 TEST");

			ImGui::End();
		}
		else
		{
			drawParams.drawEOPStartInfo = false;
		}
	}

	/*ImGui::Begin("test");

	if (ImGui::Button("fight"))
	{
		_asm
		{
			push 1
			mov ecx, 0x02BF8550
			mov eax,0x0044edb0
			call eax
			add esp, 4
		}
	}
	ImGui::End();*/
	return;
}
NOINLINE LRESULT APIENTRY graphicsD3D::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(dataS.hookD.oWndProc, hWnd, uMsg, wParam, lParam);
}


NOINLINE void graphicsD3D::initImgGui(IDirect3DDevice9* pDevice)
{

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

	ImGui::StyleGrey();
	io.Fonts->AddFontFromFileTTF(f.c_str(), 24, &font_config, io.Fonts->GetGlyphRangesCyrillic());

	ImGui_ImplWin32_Init(dataS.Window);
	ImGui_ImplDX9_Init(pDevice);

	dataS.ImInitialized = true;

	drawParams.drawEOPStartInfo = true;
	drawParams.drawInfoEndTime = (float)ImGui::GetTime()+20.0f;
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


NOINLINE EOP_EXPORT LPDIRECT3DTEXTURE9 graphicsExport::loadTexture(const char* path, int* x, int* y)
{
	LPDIRECT3DTEXTURE9 imageRet = nullptr;
	HRESULT res = D3DXCreateTextureFromFileA(graphicsD3D::dataS.pDevice, path, &imageRet);
	if (res != D3D_OK || imageRet == nullptr)
	{
		MessageBoxA(NULL, DXGetErrorStringA(res), "Loading texture err!", MB_OK | MB_ICONASTERISK);
		return nullptr;
	}

	D3DSURFACE_DESC my_image_desc;

	imageRet->GetLevelDesc(0, &my_image_desc);

	*x = (int)my_image_desc.Width;
	*y = (int)my_image_desc.Height;

	return imageRet;
}

NOINLINE EOP_EXPORT void graphicsExport::unloadTexture(LPDIRECT3DTEXTURE9 texture)
{
	texture->Release();
	texture = nullptr;
}

NOINLINE EOP_EXPORT void graphicsExport::onCreateDevice(IDirect3DDevice9* pDevice)
{
	graphicsD3D::dataS.pDevice = pDevice;

	graphicsD3D::InitS();
	graphicsD3D::initImgGui(pDevice);
}

NOINLINE EOP_EXPORT void graphicsExport::onEndScene(IDirect3DDevice9* pDevice)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	graphicsD3D::Draw(pDevice);


	graphicsD3D::dataS.ifMouseOrKeyBoardAtImgui = ImGui::GetIO().WantCaptureMouse;
	graphicsD3D::dataS.ifMouseOrKeyBoardAtImgui |= ImGui::GetIO().WantCaptureKeyboard;


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

NOINLINE EOP_EXPORT void graphicsExport::onReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

}

NOINLINE EOP_EXPORT void graphicsExport::afterReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_CreateDeviceObjects();
}
