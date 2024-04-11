#include "pch.h"
#include "plugins.h"
#include "tests.h"

#include "smallFuncs.h"
#include "techFuncs.h"
#include "discordManager.h"

#include "eduThings.h"
#include "onlineThings.h"

#include "PlannedRetreatRoute.h"
plugins::configT plugins::pluginsCfg;

void plugins::init()
{
	pluginsCfg.pluginsPath = globals::dataS.modPatch;

	pluginsCfg.pluginsPath = pluginsCfg.pluginsPath + "\\youneuoy_Data\\plugins";
	vector<string>plList = getAllPlPachs(&pluginsCfg.pluginsPath);
	if (plList.size() < 1)return;
	
	for (string s : plList)
	{
		if (s == "M2TWEOPLibrary.dll"
			|| s == "lua.dll")continue;
		plugin* pl = new plugin();
		if (!pl->init(&s))
		{
			delete pl;
			continue;
		}
		pluginsCfg.plugins.push_back(pl);
	}
}
int ReplaceByte(int index, int value, char replaceByte)
{
	return (value & ~(0xFF << (index * 8))) | (replaceByte << (index * 8));
}
void __fastcall plugins::onEvent(DWORD** vTab, DWORD arg2)
{
	DWORD adr = (*vTab)[43];
	char* event = nullptr;
	_asm
	{
		mov eax, adr
		call eax
		mov event, eax
	}

	DWORD* eventAddr = vTab[0];

	if (event == nullptr)return;

	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onEventWrapper))(eventAddr, vTab, arg2);
	}

	if (strcmp(event, "ScrollOpened") == 0)
	{
		char* str = reinterpret_cast<char*>(vTab[1]);
		if (strcmp(str, "prebattle_scroll") == 0)
		{
			battleCreator::onBattleStratScreen();
		}
		else if (strcmp(str, "post_battle_scroll") == 0)
		{
			battleCreator::onPostBattleStratScreen();
		}
		else if (strcmp(str, "hotseat_scroll") == 0)
		{
			battleCreator::onHotseatScreen();
		}
	}
	else if (strcmp(event, "FactionTurnStart") == 0)
	{
		factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
		discordManager::OnFactionTurnStart(fac);
		PlannedRetreatRoute::OnFactionTurnStart(fac);
	}
}



std::string plugins::onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup)
{
	std::string retVal;

	for (plugin* pl : pluginsCfg.plugins)
	{
		std::string* tmpVal = (*(*pl->onSelectWorldpkgdesc))(selectedRec, selectedGroup);

		retVal = *tmpVal;

		delete tmpVal;
	}

	return retVal;
}

int plugins::onfortificationlevelS(settlementStruct* settlement, bool* isCastle)
{
	int retVal = -2;//magic value, mean not change anything


	for (plugin* pl : pluginsCfg.plugins)
	{
		bool isChanged = false;
		int tmpVal = (*(*pl->onfortificationlevelS))(settlement, isCastle, &isChanged);
		if (isChanged == true)
		{
			retVal = tmpVal;
		}
	}

	return retVal;
}

float plugins::OnCalculateUnitValue(eduEntry* entry, float value)
{
	for (const plugin* pl : pluginsCfg.plugins)
	{
		value = (*(*pl->onCalculateUnitValue))(entry, value);
	}
	return value;
}

void plugins::onClickAtTile(int x, int y)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onClickAtTile))(x, y);
	}

	PlannedRetreatRoute::OnClickAtTile(x, y);
}

void plugins::onCampaignMapLoaded()
{
	discordManager::onCampaignMapLoaded();

	globals::dataS.Modules.tacticalMapVeiwer.UnView();
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onCampaignMapLoaded))();
	}
}

void plugins::onEndSiege(settlementStruct* sett)//settlement or fort!
{
	int x = sett->xCoord;
	int y = sett->yCoord;

	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onEndSiege))(x, y);
	}
}

void plugins::onStartSiege(settlementStruct* sett)
{
	int x = sett->xCoord;
	int y = sett->yCoord;

	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onStartSiege))(x, y);
	}
}

void plugins::onNewGameStart()
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onNewGameStart))();
	}
}

void plugins::onChangeTurnNum()
{
	int num = fastFuncts::getPassedTurnsNum();
	discordManager::OnChangeTurnNum(num);

	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onChangeTurnNum))(num);
	}
}

void plugins::onLoadGame(UNICODE_STRING**& savePath)
{
	vector<string>files = techFuncs::loadGameLoadArchive(savePath);
	if (files.size() == 0)
	{
		return;
	}

	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onLoadGamePl))(&files);
	}
	PlannedRetreatRoute::OnGameLoad(files);

	techFuncs::deleteFiles(files);

}

void plugins::onSaveGame(UNICODE_STRING**& savePath)
{

	vector<string>files;

	for (plugin* pl : pluginsCfg.plugins)
	{
		vector<string>* plugFiles = (*(*pl->onSaveGamePl))(savePath);

		for (string& path : *plugFiles)
		{
			files.push_back(path);
		}

		delete plugFiles;
	}
	std::string retreatsFile = PlannedRetreatRoute::OnGameSave();
	if (!retreatsFile.empty())
	{
		files.push_back(retreatsFile);
	}
	techFuncs::saveGameMakeArchive(savePath, files);


	techFuncs::deleteFiles(files);

}

void plugins::onReadGameDbsAtStart()
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onReadGameDbsAtStart))();
	}
}

void plugins::onGameInit()
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onGameInit))();
	}
}

void plugins::onUnloadCampaign()
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onUnloadCampaign))();
	}
}

void plugins::onAiTurn(aiFaction* aifaction)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onAiTurn))(aifaction);
	}
}

void plugins::onWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onWndProc))(hWnd, uMsg, wParam, lParam);
	}
}

void plugins::onEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	for (plugin* pl : pluginsCfg.plugins)
	{

		(*(*pl->drawOnEndScene))(pDevice);
	}
}

void plugins::onReset(LPDIRECT3DDEVICE9 pDevice)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onReset))(pDevice);
	}
}

void plugins::onLoadingFonts(LPDIRECT3DDEVICE9 pDevice)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onLoadingFonts))(pDevice);
	}
}

void plugins::onChangeImGuiCtx(ImGuiContext* imCtx, ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onChangeImGuiContext))(imCtx, alloc_func, free_func, user_data);
	}
}

int plugins::compareEvent(const char* event, char** adr1, const char* adr2)
{
	if (event == *adr1)return 1;

	if (strcmp(event, adr2) == 0)
	{
		*adr1 = (char*)event;
		return 1;
	}
	return 0;

}

vector<string> plugins::getAllPlPachs(string* s)
{
	vector<string> names;
	string search_path = *s + "/*.dll";
	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

int plugin::init(string* nameP)
{
	//set the name of plugin(filename)
	name = *nameP;

	//load the init function and call it
	string plPath = plugins::pluginsCfg.pluginsPath + "\\";
	plPath = plPath + name;

	string fName = "initPlugin";
	ProcLoader<int(__cdecl*)(string*)> initPlugin;
	if (!initPlugin.Load(&plPath, &fName))
		return 0;
	int res = (*initPlugin)(&globals::dataS.modPatch);
	if (!res)return 0;

	//onChangeTurnNum function
	fName = "onChangeTurnNum";
	onChangeTurnNum.Load(&plPath, &fName);

	//onEvent
	fName = "onEventWrapper";
	onEventWrapper.Load(&plPath, &fName);

	//drawOnEndScene
	fName = "drawOnEndScene";
	drawOnEndScene.Load(&plPath, &fName);

	//onReset
	fName = "onReset";
	onReset.Load(&plPath, &fName);

	//onLoadingFonts
	fName = "onLoadingFonts";
	onLoadingFonts.Load(&plPath, &fName);


	//onChangeImGuiContext
	fName = "onChangeImGuiContext";
	onChangeImGuiContext.Load(&plPath, &fName);

	//onWndProc
	fName = "onWndProc";
	onWndProc.Load(&plPath, &fName);

	//onSaveGamePl
	fName = "onSaveGamePl";
	onSaveGamePl.Load(&plPath, &fName);

	//onLoadGamePl
	fName = "onLoadGamePl";
	onLoadGamePl.Load(&plPath, &fName);


	//onNewGameStart
	fName = "onNewGameStart";
	onNewGameStart.Load(&plPath, &fName);

	//onCampaignMapLoaded
	fName = "onCampaignMapLoaded";
	onCampaignMapLoaded.Load(&plPath, &fName);


	//onClickAtTile
	fName = "onClickAtTile";
	onClickAtTile.Load(&plPath, &fName);

	//onSelectWorldpkgdesc
	fName = "onSelectWorldpkgdesc";
	onSelectWorldpkgdesc.Load(&plPath, &fName);

	//onfortificationlevelS
	fName = "onfortificationlevelS";
	onfortificationlevelS.Load(&plPath, &fName);

	//onCalculateUnitValue
	fName = "onCalculateUnitValue";
	onCalculateUnitValue.Load(&plPath, &fName);

	//onGameInit
	fName = "onGameInit";
	onGameInit.Load(&plPath, &fName);

	//onUnloadCampaign
	fName = "onUnloadCampaign";
	onUnloadCampaign.Load(&plPath, &fName);

	//onAiTurn
	fName = "onAiTurn";
	onAiTurn.Load(&plPath, &fName);

	//onReadGameDbsAtStart
	fName = "onReadGameDbsAtStart";
	onReadGameDbsAtStart.Load(&plPath, &fName);

	//onEndSiege
	fName = "onEndSiege";
	onEndSiege.Load(&plPath, &fName);

	//onStartSiege
	fName = "onStartSiege";
	onStartSiege.Load(&plPath, &fName);
	return 1;
}
