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
			m_pProc = (T)emptyProc;
			MessageBoxA(NULL, "Can`t load function", "Attention", NULL);
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

	ProcLoader<void(__cdecl*)(int, int)> onClickAtTile;

	ProcLoader<void(__cdecl*)()> onCampaignMapLoaded;

	ProcLoader<void(__cdecl*)()> onNewGameStart;
	ProcLoader<vector<string>* (__cdecl*)(UNICODE_STRING**& savePath)> onSaveGamePl;
	ProcLoader<void(__cdecl*)(vector<string>*)> onLoadGamePl;


	ProcLoader<void(__cdecl*)(int)> onChangeTurnNum;


	ProcLoader<void(__cdecl*)(factionStruct*)> onPreFactionTurnStart;
	ProcLoader<void(__cdecl*)(factionStruct*)> onFactionTurnStart;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onGeneralDevastatesTile;
	ProcLoader<void(__cdecl*)(factionStruct*)> onFactionTurnEnd;
	ProcLoader<void(__cdecl*)(factionStruct*)> onFactionNewCapital;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*)> onFactionWarDeclared;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*)> onFactionAllianceDeclared;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*)> onFactionTradeAgreementMade;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*)> onFactionBreakAlliance;


	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*, float)> onGiveMoney;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*)> onUpdateAttitude;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*, float)> onDemeanour;

	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onCharacterSelected;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onMultiTurnMove;
	ProcLoader<void(__cdecl*)(generalCharacterictics*, generalCharacterictics*)> onGeneralAssaultsGeneral;

	ProcLoader<void(__cdecl*)(generalCharacterictics*, settlementStruct*, fortStruct*)> onGeneralAssaultsResidence;
	ProcLoader<void(__cdecl*)(generalCharacterictics*, settlementStruct*)> onGeneralCaptureSettlement;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onGeneralCaptureResidence;

	ProcLoader<void(__cdecl*)(settlementStruct*, fortStruct*)> onSiegeEquipmentCompleted;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onPostBattle;

	ProcLoader<void(__cdecl*)(settlementStruct*)> onSettlementSelected;
	ProcLoader<void(__cdecl*)(settlementStruct*)> onSettlementUpgraded;
	ProcLoader<void(__cdecl*)(settlementStruct*)> onSettlementConverted;

	ProcLoader<void(__cdecl*)(settlementStruct*)> onCityRiots;

	ProcLoader<void(__cdecl*)(settlementStruct*)> onUngarrisonedSettlement;
	ProcLoader<void(__cdecl*)(fortStruct*)> onUngarrisonedFort;

	ProcLoader<void(__cdecl*)(settlementStruct*, factionStruct*, factionStruct*)> onGiveSettlement;

	ProcLoader<void(__cdecl*)(generalCharacterictics*, factionStruct*)> onOccupySettlement;
	ProcLoader<void(__cdecl*)(generalCharacterictics*, factionStruct*)> onExterminatePopulation;
	ProcLoader<void(__cdecl*)(generalCharacterictics*, factionStruct*)> onSackSettlement;


	ProcLoader<void(__cdecl*)(settlementStruct*, const char*)> onAddedToBuildingQueue;
	ProcLoader<void(__cdecl*)(settlementStruct*, const char*)> onBuildingDestroyed;
	ProcLoader<void(__cdecl*)(factionStruct*, settlementStruct*)> onBuildingCompleted;

	ProcLoader<void(__cdecl*)(const char*)> onEventCounter;
	ProcLoader<void(__cdecl*)(void*, settlementStruct*)> onPopeAcceptsCrusadeTarget;
	ProcLoader<void(__cdecl*)(void*, settlementStruct*)> onCrusadeCalled;
	ProcLoader<void(__cdecl*)(void*, settlementStruct*)> onCrusadeEnds;
	ProcLoader<void(__cdecl*)(void*, settlementStruct*)> onPopeRejectsCrusadeTarget;
	ProcLoader<void(__cdecl*)(void*, settlementStruct*, stackStruct*)> onArmyTakesCrusadeTarget;
	ProcLoader<void(__cdecl*)(void*, settlementStruct*, factionStruct*)> onUnitsDesertCrusade;

	ProcLoader<void(__cdecl*)(factionStruct*)> onFactionExcommunicated;

	ProcLoader<void(__cdecl*)(int)> onDisaster;

	ProcLoader<void(__cdecl*)(factionStruct*)> onHordeFormed;
	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*)> onPopeElected;
	ProcLoader<void(__cdecl*)(factionStruct*)> onVotedForPope;
	ProcLoader<void(__cdecl*)(factionStruct*)> onAssassinCaughtAttackingPope;
	ProcLoader<void(__cdecl*)(factionStruct*, settlementStruct*)> onInquisitorAppointed;


	ProcLoader<void(__cdecl*)(settlementStruct*, const char*)> onAddedToTrainingQueue;
	ProcLoader<void(__cdecl*)(factionStruct*, unit*)> onUnitDisbanded;
	ProcLoader<void(__cdecl*)(factionStruct*, settlementStruct*, unit*)> onUnitTrained;
	ProcLoader<void(__cdecl*)(generalCharacterictics*, int, settlementStruct*)> onAgentCreated;

	ProcLoader<void(__cdecl*)(factionStruct*, factionStruct*, int, int)> onObjSeen;
	ProcLoader<void(__cdecl*)(factionStruct*, int, int)> onTileSeen;


	ProcLoader<void(__cdecl*)(settlementStruct*)> onSettlementPanelOpen;
	ProcLoader<void(__cdecl*)(factionStruct*)> onFinancesPanelOpen;
	ProcLoader<void(__cdecl*)(factionStruct*)> onFactionSummaryPanelOpen;
	ProcLoader<void(__cdecl*)(factionStruct*)> onFamilyTreePanelOpen;
	ProcLoader<void(__cdecl*)()> onDiplomaticStandingPanelOpen;
	ProcLoader<void(__cdecl*)(factionStruct*)> onDiplomacyPanelOpen;
	ProcLoader<void(__cdecl*)(factionStruct*)> onPreBattlePanelOpen;
	ProcLoader<void(__cdecl*)()> onNavalAutoResolvePanelOpen;

	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onCharacterPanelOpen;
	ProcLoader<void(__cdecl*)(settlementStruct*)> onTradePanelOpen;
	ProcLoader<void(__cdecl*)(settlementStruct*)> onRequestBuildingAdvice;
	ProcLoader<void(__cdecl*)(settlementStruct*, EduEntry*)> onRequestTrainingAdvice;

	ProcLoader<void(__cdecl*)(factionStruct*, int)> onMessageOpen;
	ProcLoader<void(__cdecl*)(factionStruct*, int)> onIncomingMessage;
	ProcLoader<void(__cdecl*)(int)> onMessageClosed;

	ProcLoader<void(__cdecl*)(const char*)> onButtonPressed;
	ProcLoader<void(__cdecl*)(const char*)> onScrollClosed;
	ProcLoader<void(__cdecl*)(const char*)> onScrollOpened;
	ProcLoader<void(__cdecl*)(const char*)> onUIElementVisible;
	ProcLoader<void(__cdecl*)(const char*)> onScrollAdviceRequested;

	ProcLoader<void(__cdecl*)(settlementStruct*, const char*)> onSettlementScrollAdviceRequested;

	ProcLoader<void(__cdecl*)()> onPreBattleScrollAdviceRequested;
	ProcLoader<void(__cdecl*)()> onNavalPreBattleScrollAdviceRequested;
	ProcLoader<void(__cdecl*)(void*)> onCollegeOfCardinalsPanelOpen;


	ProcLoader<void(__cdecl*)(int)> onGameReloaded;

	ProcLoader<void(__cdecl*)(factionStruct*, const char*, factionStruct*)> onTransgression;

	ProcLoader<void(__cdecl*)(settlementStruct*, const char*)> onGuildUpgraded;
	ProcLoader<void(__cdecl*)(settlementStruct*, unsigned char)> onGuildDestroyed;

	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onBrotherAdopted;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onBirth;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onCharacterComesOfAge;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onCharacterMarries;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onCharacterBecomesAFather;

	ProcLoader<void(__cdecl*)(generalCharacterictics*, settlementStruct*)> onNewAdmiralCreated;

	ProcLoader<void(__cdecl*)(const char*)> onShortcutTriggered;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onCharacterMarriesPrincess;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onBecomesFactionLeader;
	ProcLoader<void(__cdecl*)(generalCharacterictics*)> onBecomesFactionHeir;

	ProcLoader<void(__cdecl*)(LPDIRECT3DDEVICE9 pDevice)> drawOnEndScene;
	ProcLoader<void(__cdecl*)(LPDIRECT3DDEVICE9 pDevice)> onReset;
	ProcLoader<void(__cdecl*)(LPDIRECT3DDEVICE9 pDevice)> onLoadingFonts;


	ProcLoader<void(__cdecl*)(ImGuiContext*, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)> onChangeImGuiContext;

	ProcLoader<LRESULT(__cdecl*)(HWND, UINT, WPARAM, LPARAM)> onWndProc;
	ProcLoader<void(__cdecl*)()> onReadGameDbsAtStart;

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

	static void __fastcall onEvent(DWORD** vTab);

	//from here calls all plugins

	static std::string onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup);
	static int onfortificationlevelS(settlementStruct* settlement, bool* isCastle);
	static void onClickAtTile(int x, int y);
	static void onCampaignMapLoaded();

	static void onEndSiege(settlementStruct* sett);//settlement or fort!
	static void onStartSiege(settlementStruct* sett);//settlement or fort!

	static void onNewGameStart();
	static void onChangeTurnNum();
	static void onLoadGame(UNICODE_STRING**& savePath);
	static void onSaveGame(UNICODE_STRING**& savePath);

	static void onReadGameDbsAtStart();

	static void onWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void onEndScene(LPDIRECT3DDEVICE9 pDevice);
	static void onReset(LPDIRECT3DDEVICE9 pDevice);
	static void onLoadingFonts(LPDIRECT3DDEVICE9 pDevice);

	static void onChangeImGuiCtx(ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data);

	static vector<const char*>* eventNames;
private:
	static int compareEvent(const char* event, char** adr1, const char* adr2);

	static vector<string> getAllPlPachs(string* s);

	static void initEvNames();

};

