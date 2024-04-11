#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h"
#include "eventsCodes.h"
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "imgui.h"
#include <d3d9.h>
#include <sstream>
#include "realGameTypes.h"
using namespace std;
template <class T>
class ProcLoader
{
public:
	T			 m_pProc;
	string			m_strNameProcedure;

	//strings for compare
	char* stringAdr = nullptr;
	const char* strCmp; 

	ProcLoader() {};

	BOOL Load(string* pczNameLibrary, string* pczNameProcedure, char* sCmp = nullptr)
	{
		m_strNameProcedure = *pczNameProcedure;
		m_pProc = NULL;
		strCmp = sCmp;
		HANDLE hModule;

		if (!(hModule = ::LoadLibraryA(pczNameLibrary->c_str())))
		{
			std::stringstream error;
			error << "Could not load plugin located at:\n" << pczNameProcedure->c_str() << "\n" << "Error Code: " << GetLastError();
			MessageBoxA(NULL, error.str().c_str(), "Error", NULL);
			return FALSE;
		}
		if (!(m_pProc = (T)::GetProcAddress((HMODULE)hModule, m_strNameProcedure.c_str())))
		{
			std::stringstream error2;
			error2 << "Can't load function: " << m_strNameProcedure.c_str();
			m_pProc = (T)emptyProc;
			MessageBoxA(NULL, error2.str().c_str(), "Attention", NULL);
			return FALSE;
		}
		return TRUE;
	};

	T operator *() const
	{
		return m_pProc;
	}

	static void emptyProc()
	{

	}
private:
	ProcLoader& operator = (ProcLoader&)
	{
		return *this;
	};
};


class plugin
{
public:

	ProcLoader<std::string* (__cdecl*)(const char*, const char*)> onSelectWorldpkgdesc;
	ProcLoader<int(__cdecl*)(settlementStruct*, bool*, bool*)> onfortificationlevelS;
	ProcLoader<float(__cdecl*)(eduEntry*, float)> onCalculateUnitValue;

	ProcLoader<void(__cdecl*)(int, int)> onClickAtTile;

	ProcLoader<void(__cdecl*)()> onCampaignMapLoaded;

	ProcLoader<void(__cdecl*)()> onNewGameStart;
	ProcLoader<vector<string>* (__cdecl*)(UNICODE_STRING**& savePath)> onSaveGamePl;
	ProcLoader<void(__cdecl*)(vector<string>*)> onLoadGamePl;


	ProcLoader<void(__cdecl*)(int)> onChangeTurnNum;

	ProcLoader<void(__cdecl*)(DWORD*, DWORD**, DWORD)> onEventWrapper;

	ProcLoader<void(__cdecl*)(LPDIRECT3DDEVICE9 pDevice)> drawOnEndScene;
	ProcLoader<void(__cdecl*)(LPDIRECT3DDEVICE9 pDevice)> onReset;
	ProcLoader<void(__cdecl*)(LPDIRECT3DDEVICE9 pDevice)> onLoadingFonts;


	ProcLoader<void(__cdecl*)(ImGuiContext*, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)> onChangeImGuiContext;

	ProcLoader<LRESULT(__cdecl*)(HWND, UINT, WPARAM, LPARAM)> onWndProc;
	ProcLoader<void(__cdecl*)()> onReadGameDbsAtStart;
	ProcLoader<void(__cdecl*)()> onGameInit;
	ProcLoader<void(__cdecl*)()> onUnloadCampaign;
	ProcLoader<void(__cdecl*)(aiFaction*)> onAiTurn;

	ProcLoader<void(__cdecl*)(int, int)> onEndSiege;
	ProcLoader<void(__cdecl*)(int, int)> onStartSiege;
	int init(string* nameP);
private:
	string name;
};


class plugins
{
public:
	static struct  configT {
		string pluginsPath;
		vector<plugin*>plugins;
	}
	pluginsCfg;


	static void init();

	static void __fastcall onEvent(DWORD** vTab, DWORD arg2);

	//from here calls all plugins

	static std::string onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
	static int onfortificationlevelS(settlementStruct* settlement, bool* isCastle);
	static float OnCalculateUnitValue(eduEntry* entry, float value);
	static void onClickAtTile(int x, int y);
	static void onCampaignMapLoaded();

	static void onEndSiege(settlementStruct* sett);//settlement or fort!
	static void onStartSiege(settlementStruct* sett);//settlement or fort!

	static void onNewGameStart();
	static void onChangeTurnNum();
	static void onLoadGame(UNICODE_STRING**& savePath);
	static void onSaveGame(UNICODE_STRING**& savePath);

	static void onReadGameDbsAtStart();
	static void onGameInit();
	static void onUnloadCampaign();
	static void onAiTurn(aiFaction* aifaction);

	static void onWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void onEndScene(LPDIRECT3DDEVICE9 pDevice);
	static void onReset(LPDIRECT3DDEVICE9 pDevice);
	static void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice);

	static void onChangeImGuiCtx(ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);
private:
	static int compareEvent(const char* event, char** adr1, const char* adr2);

	static vector<string> getAllPlPachs(string* s);

};

