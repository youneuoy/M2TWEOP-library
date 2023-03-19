#include "pch.h"
#include "plugins.h"
#include "tests.h"

#include "smallFuncs.h"
#include "techFuncs.h"

#include "eduThings.h"
#include "onlineThings.h"
plugins::configT plugins::pluginsCfg;
vector<const char*>* plugins::eventNames;

void plugins::init()
{
	pluginsCfg.pluginsPath = globals::dataS.modPatch;

	pluginsCfg.pluginsPath = pluginsCfg.pluginsPath + "\\youneuoy_Data\\plugins";
	vector<string>plList = getAllPlPachs(&pluginsCfg.pluginsPath);
	if (plList.size() < 1)return;

	initEvNames();
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


	/*if (strcmp(event, "CharacterSelected") == 0)
	{

		namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);

		stackStruct* army = prs->gen->armyLeaded;
		if (army)
		{
			eduThings::addEopEduEntry(army->units[0]->eduEntry->Index,1000);
			int edb=eduThings::getDataEopEdu(1000);
			eduThings::setEntryUnitCardTga(1000,"#akavir_swordsmen.tga");
			eduThings::setEntryInfoCardTga(1000,"akavir_swordsmen_info.tga");
			eduThings::setEntrySoldierModel(1000,"Sword_and_Buckler_Men");
			eduThings::setEntryLocalizedName(1000,u8"Тест");
			eduThings::setEntryLocalizedDescr(1000, u8"Описание");
			eduThings::setEntryLocalizedShortDescr(1000, u8"Короткое описание");

			unit* res = nullptr;

			DWORD adr = codes::offsets.createUnitFunc;
			_asm {
					mov ecx, edb

					push 1
					push 1
					push -1
					push 1
					push 0
					push 0
					push 0

					mov eax, [adr]
					call eax
					mov res, eax
			}
			fastFuncts::addUnitToArmy(army, res);
		}
	}*/

	for (plugin* pl : pluginsCfg.plugins)
	{
		if (compareEvent(event, &pl->onCharacterSelected.stringAdr, pl->onCharacterSelected.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);

			(*(*pl->onCharacterSelected))(prs);


		}
		else if (compareEvent(event, &pl->onSiegeEquipmentCompleted.stringAdr, pl->onSiegeEquipmentCompleted.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			fortStruct* frt = fastFuncts::findFort(setl->xCoord, setl->yCoord);

			if (frt != nullptr)setl = nullptr;
			(*(*pl->onSiegeEquipmentCompleted))(setl, frt);
		}
		else if (compareEvent(event, &pl->onSettlementSelected.stringAdr, pl->onSettlementSelected.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);

			(*(*pl->onSettlementSelected))(setl);
		}
		else if (compareEvent(event, &pl->onSettlementUpgraded.stringAdr, pl->onSettlementUpgraded.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onSettlementUpgraded))(setl);
		}
		else if (compareEvent(event, &pl->onSettlementConverted.stringAdr, pl->onSettlementConverted.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onSettlementConverted))(setl);
		}
		else if (compareEvent(event, &pl->onCityRiots.stringAdr, pl->onCityRiots.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onCityRiots))(setl);
		}
		else if (compareEvent(event, &pl->onUngarrisonedSettlement.stringAdr, pl->onUngarrisonedSettlement.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onUngarrisonedSettlement))(setl);
		}
		else if (compareEvent(event, &pl->onGiveSettlement.stringAdr, pl->onGiveSettlement.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[2]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[3]);
			(*(*pl->onGiveSettlement))(setl, fac, fac2);
		}
		else if (compareEvent(event, &pl->onOccupySettlement.stringAdr, pl->onOccupySettlement.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onOccupySettlement))(prs, fac);
		}
		else if (compareEvent(event, &pl->onExterminatePopulation.stringAdr, pl->onExterminatePopulation.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onExterminatePopulation))(prs, fac);
		}
		else if (compareEvent(event, &pl->onSackSettlement.stringAdr, pl->onSackSettlement.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onSackSettlement))(prs, fac);
		}
		else if (compareEvent(event, &pl->onAddedToBuildingQueue.stringAdr, pl->onAddedToBuildingQueue.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			char* str = reinterpret_cast<char*>(vTab[2]);
			(*(*pl->onAddedToBuildingQueue))(sett, str);
		}
		else if (compareEvent(event, &pl->onBuildingDestroyed.stringAdr, pl->onBuildingDestroyed.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			char* str = reinterpret_cast<char*>(vTab[2]);
			(*(*pl->onBuildingDestroyed))(sett, str);
		}
		else if (compareEvent(event, &pl->onBuildingCompleted.stringAdr, pl->onBuildingCompleted.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onBuildingCompleted))(fac, sett);
		}
		else if (compareEvent(event, &pl->onAssassinCaughtAttackingPope.stringAdr, pl->onAssassinCaughtAttackingPope.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onAssassinCaughtAttackingPope))(fac);
		}
		else if (compareEvent(event, &pl->onAddedToTrainingQueue.stringAdr, pl->onAddedToTrainingQueue.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			char* str = reinterpret_cast<char*>(vTab[2]);
			if (*str == 0)str = nullptr;
			(*(*pl->onAddedToTrainingQueue))(sett, str);
		}
		else if (compareEvent(event, &pl->onUnitTrained.stringAdr, pl->onUnitTrained.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			unit* un = reinterpret_cast<unit*>(vTab[3]);
			(*(*pl->onUnitTrained))(fac, sett, un);
		}
		else if (compareEvent(event, &pl->onAgentCreated.stringAdr, pl->onAgentCreated.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			int prsType = (int)vTab[2];
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[3]);
			(*(*pl->onAgentCreated))(prs, prsType, sett);
		}
		else if (compareEvent(event, &pl->onSettlementPanelOpen.stringAdr, pl->onSettlementPanelOpen.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onSettlementPanelOpen))(sett);
		}
		else if (compareEvent(event, &pl->onFinancesPanelOpen.stringAdr, pl->onFinancesPanelOpen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFinancesPanelOpen))(fac);
		}
		else if (compareEvent(event, &pl->onFactionSummaryPanelOpen.stringAdr, pl->onFactionSummaryPanelOpen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFactionSummaryPanelOpen))(fac);
		}
		else if (compareEvent(event, &pl->onFamilyTreePanelOpen.stringAdr, pl->onFamilyTreePanelOpen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFamilyTreePanelOpen))(fac);
		}
		else if (compareEvent(event, &pl->onDiplomaticStandingPanelOpen.stringAdr, pl->onDiplomaticStandingPanelOpen.strCmp))
		{
			(*(*pl->onDiplomaticStandingPanelOpen))();
		}
		else if (compareEvent(event, &pl->onDiplomacyPanelOpen.stringAdr, pl->onDiplomacyPanelOpen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onDiplomacyPanelOpen))(fac);
		}
		else if (compareEvent(event, &pl->onPreBattlePanelOpen.stringAdr, pl->onPreBattlePanelOpen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onPreBattlePanelOpen))(fac);
		}
		else if (compareEvent(event, &pl->onNavalAutoResolvePanelOpen.stringAdr, pl->onNavalAutoResolvePanelOpen.strCmp))
		{
			(*(*pl->onNavalAutoResolvePanelOpen))();
		}
		else if (compareEvent(event, &pl->onCharacterPanelOpen.stringAdr, pl->onCharacterPanelOpen.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			(*(*pl->onCharacterPanelOpen))(prs);
		}
		else if (compareEvent(event, &pl->onTradePanelOpen.stringAdr, pl->onTradePanelOpen.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onTradePanelOpen))(sett);
		}
		else if (compareEvent(event, &pl->onRequestBuildingAdvice.stringAdr, pl->onRequestBuildingAdvice.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			(*(*pl->onRequestBuildingAdvice))(sett);
		}
		else if (compareEvent(event, &pl->onRequestTrainingAdvice.stringAdr, pl->onRequestTrainingAdvice.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			eduEntry* edEnt = reinterpret_cast<eduEntry*>(vTab[2]);
			(*(*pl->onRequestTrainingAdvice))(sett, edEnt);
		}
		else if (compareEvent(event, &pl->onMessageOpen.stringAdr, pl->onMessageOpen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			int some = (int)vTab[2];
			(*(*pl->onMessageOpen))(fac, some);
		}
		else if (compareEvent(event, &pl->onIncomingMessage.stringAdr, pl->onIncomingMessage.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			int some = (int)vTab[2];
			(*(*pl->onIncomingMessage))(fac, some);
		}
		else if (compareEvent(event, &pl->onMessageClosed.stringAdr, pl->onMessageClosed.strCmp))
		{
			int some = (int)vTab[1];
			(*(*pl->onMessageClosed))(some);
		}
		else if (compareEvent(event, &pl->onButtonPressed.stringAdr, pl->onButtonPressed.strCmp))
		{
			char* str = reinterpret_cast<char*>(vTab[1]);
			(*(*pl->onButtonPressed))(str);
		}
		else if (compareEvent(event, &pl->onScrollClosed.stringAdr, pl->onScrollClosed.strCmp))
		{
			char* str = reinterpret_cast<char*>(vTab[1]);
			(*(*pl->onScrollClosed))(str);
		}
		else if (compareEvent(event, &pl->onScrollOpened.stringAdr, pl->onScrollOpened.strCmp))
		{
			char* str = reinterpret_cast<char*>(vTab[1]);
			(*(*pl->onScrollOpened))(str);
		}
		else if (compareEvent(event, &pl->onUIElementVisible.stringAdr, pl->onUIElementVisible.strCmp))
		{
			char* str = reinterpret_cast<char*>(vTab[1]);
			(*(*pl->onUIElementVisible))(str);
		}
		else if (compareEvent(event, &pl->onSettlementScrollAdviceRequested.stringAdr, pl->onSettlementScrollAdviceRequested.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			char* str = reinterpret_cast<char*>(vTab[2]);
			(*(*pl->onSettlementScrollAdviceRequested))(sett, str);
		}
		else if (compareEvent(event, &pl->onPreBattleScrollAdviceRequested.stringAdr, pl->onPreBattleScrollAdviceRequested.strCmp))
		{
			(*(*pl->onPreBattleScrollAdviceRequested))();
		}
		else if (compareEvent(event, &pl->onNavalPreBattleScrollAdviceRequested.stringAdr, pl->onNavalPreBattleScrollAdviceRequested.strCmp))
		{
			(*(*pl->onNavalPreBattleScrollAdviceRequested))();
		}
		else if (compareEvent(event, &pl->onScrollAdviceRequested.stringAdr, pl->onScrollAdviceRequested.strCmp))
		{
			char* str = reinterpret_cast<char*>(*vTab[1]);
			(*(*pl->onScrollAdviceRequested))(str);
		}
		else if (compareEvent(event, &pl->onMultiTurnMove.stringAdr, pl->onMultiTurnMove.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			/*if (prs->gen->armyLeaded)
			{
				string str = "Mailed Knights";

				unit* un = FastFuncts::createUnitN((char*)str.c_str());

				FastFuncts::addUnitToArmy(un, prs->gen->armyLeaded);
			}*/
			(*(*pl->onMultiTurnMove))(prs);
		}
		else if (compareEvent(event, &pl->onCollegeOfCardinalsPanelOpen.stringAdr, pl->onCollegeOfCardinalsPanelOpen.strCmp))
		{
			void* colleg = reinterpret_cast<void*>(vTab[1]);
			(*(*pl->onCollegeOfCardinalsPanelOpen))(colleg);
		}
		else if (compareEvent(event, &pl->onGuildUpgraded.stringAdr, pl->onGuildUpgraded.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			char* str = reinterpret_cast<char*>(vTab[2]);
			(*(*pl->onGuildUpgraded))(sett, str);
		}
		else if (compareEvent(event, &pl->onGuildDestroyed.stringAdr, pl->onGuildDestroyed.strCmp))
		{
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[1]);
			unsigned char guildID = (((BYTE*)vTab)[8]);
			(*(*pl->onGuildDestroyed))(sett, guildID);
		}
		else if (compareEvent(event, &pl->onNewAdmiralCreated.stringAdr, pl->onNewAdmiralCreated.strCmp))
		{
			namedCharacter* prs = reinterpret_cast<namedCharacter*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onNewAdmiralCreated))(prs, sett);
		}
		else if (compareEvent(event, &pl->onShortcutTriggered.stringAdr, pl->onShortcutTriggered.strCmp))
		{
			char* str = reinterpret_cast<char*>(vTab[1]);
			(*(*pl->onShortcutTriggered))(str);
		}
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

void plugins::onClickAtTile(int x, int y)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onClickAtTile))(x, y);
	}
}

void plugins::onCampaignMapLoaded()
{
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

void plugins::initEvNames()
{
	eventNames = new vector<const char*>{
		"PreFactionTurnStart",
		"FactionTurnStart",
		"FactionTurnEnd",
		"FactionNewCapital",
		"FactionWarDeclared",
		"FactionAllianceDeclared",
		"FactionTradeAgreementMade",
		"FactionBreakAlliance",
		"InterFactionMarriage",
		"GiveMoney",
		"HordeFormed",
		"Transgression",
		"DishonestTransgression",
		"UpdateAttitude",
		"Demeanour",
		"CharacterSelected",
		"GeneralAssaultsResidence",
		"GeneralAssaultsGeneral",
		"GeneralCaptureResidence",
		"GeneralCaptureSettlement",
		"SiegeEquipmentCompleted",
		"PostBattle",
		"SettlementSelected",
		"SettlementUpgraded",
		"SettlementConverted",
		"Insurrection",
		"CityRiots",
		"GiveSettlement",
		"CityRebels",
		"UngarrisonedSettlement",
		"OccupySettlement",
		"SackSettlement",
		"ExterminatePopulation",
		"CitySacked",
		"BuildingCompleted",
		"BuildingDestroyed",
		"AddedToBuildingQueue",
		"GuildUpgraded",
		"GuildDestroyed",
		"AgentCreated",
		"UnitTrained",
		"UnitDisbanded",
		"AddedToTrainingQueue",
		"EventCounter",
		"TileSeen",
		"ObjSeen",
		"CharacterPanelOpen",
		"SettlementPanelOpen",
		"FinancesPanelOpen",
		"FactionSummaryPanelOpen",
		"FamilyTreePanelOpen",
		"DiplomaticStandingPanelOpen",
		"DiplomacyPanelOpen",
		"PreBattlePanelOpen",
		"RecruitmentPanelOpen",
		"ConstructionPanelOpen",
		"TradePanelOpen",
		"HireMercenariesPanelOpen",
		"NavalAutoResolvePanelOpen",
		"IncomingMessage",
		"MessageOpen",
		"MessageClosed",
		"RequestBuildingAdvice",
		"RequestTrainingAdvice",
		"RequestMercenariesAdvice",
		"ButtonPressed",
		"UIElementVisible",
		"ScrollOpened",
		"ScrollClosed",
		"AdviceSupressed",
		"ScrollAdviceRequested",
		"PreBattleScrollAdviceRequested",
		"NavalPreBattleScrollAdviceRequested",
		"SettlementScrollAdviceRequested",
		"AbandonShowMe",
		"ScriptedAdvice",
		"GameReloaded",
		"DeclineAuatedSettlementManagement",
		"Disaster",
		"PopeElected",
		"VotedForPope",
		"FactionExcommunicated",
		"CrusadeCalled",
		"CrusadeEnds",
		"PopeAcceptsCrusadeTarget",
		"PopeRejectsCrusadeTarget",
		"UnitsDesertCrusade",
		"ArmyTakesCrusadeTarget",
		"Forgiveness",
		"InquisitorAppointed",
		"AssassinCaughtAttackingPope",
		"CollegeOfCardinalsPanelOpen",
		"MultiTurnMove",
		"UngarrisonedFort",
		"BrotherAdopted",
		"Birth",
		"CharacterComesOfAge",
		"CharacterMarries",
		"CharacterBecomesAFather",
		"NewAdmiralCreated",
		"ShortcutTriggered",
		"CharacterMarriesPrincess",
		"BecomesFactionLeader",
		"BecomesFactionHeir",
		"GeneralDevastatesTile"
	};

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


	//onCharacterSelected
	fName = "onCharacterSelected";
	onCharacterSelected.Load(&plPath, &fName);
	onCharacterSelected.strCmp = (*plugins::eventNames)[CharacterSelectedCode];


	//onSiegeEquipmentCompleted
	fName = "onSiegeEquipmentCompleted";
	onSiegeEquipmentCompleted.Load(&plPath, &fName);
	onSiegeEquipmentCompleted.strCmp = (*plugins::eventNames)[SiegeEquipmentCompletedCode];


	//onSettlementSelected
	fName = "onSettlementSelected";
	onSettlementSelected.Load(&plPath, &fName);
	onSettlementSelected.strCmp = (*plugins::eventNames)[SettlementSelectedCode];


	//onSettlementUpgraded
	fName = "onSettlementUpgraded";
	onSettlementUpgraded.Load(&plPath, &fName);
	onSettlementUpgraded.strCmp = (*plugins::eventNames)[SettlementUpgradedCode];


	//onSettlementConverted
	fName = "onSettlementConverted";
	onSettlementConverted.Load(&plPath, &fName);
	onSettlementConverted.strCmp = (*plugins::eventNames)[SettlementConvertedCode];


	//onCityRiots
	fName = "onCityRiots";
	onCityRiots.Load(&plPath, &fName);
	onCityRiots.strCmp = (*plugins::eventNames)[CityRiotsCode];

	//onUngarrisonedSettlement
	fName = "onUngarrisonedSettlement";
	onUngarrisonedSettlement.Load(&plPath, &fName);
	onUngarrisonedSettlement.strCmp = (*plugins::eventNames)[UngarrisonedSettlementCode];

	//onGiveSettlement
	fName = "onGiveSettlement";
	onGiveSettlement.Load(&plPath, &fName);
	onGiveSettlement.strCmp = (*plugins::eventNames)[GiveSettlementCode];


	//onOccupySettlement
	fName = "onOccupySettlement";
	onOccupySettlement.Load(&plPath, &fName);
	onOccupySettlement.strCmp = (*plugins::eventNames)[OccupySettlementCode];

	//onExterminatePopulation
	fName = "onExterminatePopulation";
	onExterminatePopulation.Load(&plPath, &fName);
	onExterminatePopulation.strCmp = (*plugins::eventNames)[ExterminatePopulationCode];


	//onSackSettlement
	fName = "onSackSettlement";
	onSackSettlement.Load(&plPath, &fName);
	onSackSettlement.strCmp = (*plugins::eventNames)[SackSettlementCode];

	//onAddedToBuildingQueue
	fName = "onAddedToBuildingQueue";
	onAddedToBuildingQueue.Load(&plPath, &fName);
	onAddedToBuildingQueue.strCmp = (*plugins::eventNames)[AddedToBuildingQueueCode];

	//onBuildingDestroyed
	fName = "onBuildingDestroyed";
	onBuildingDestroyed.Load(&plPath, &fName);
	onBuildingDestroyed.strCmp = (*plugins::eventNames)[BuildingDestroyedCode];

	//onBuildingCompleted
	fName = "onBuildingCompleted";
	onBuildingCompleted.Load(&plPath, &fName);
	onBuildingCompleted.strCmp = (*plugins::eventNames)[BuildingCompletedCode];

	//onAssassinCaughtAttackingPope
	fName = "onAssassinCaughtAttackingPope";
	onAssassinCaughtAttackingPope.Load(&plPath, &fName);
	onAssassinCaughtAttackingPope.strCmp = (*plugins::eventNames)[AssassinCaughtAttackingPopeCode];

	//onAddedToTrainingQueue
	fName = "onAddedToTrainingQueue";
	onAddedToTrainingQueue.Load(&plPath, &fName);
	onAddedToTrainingQueue.strCmp = (*plugins::eventNames)[AddedToTrainingQueueCode];

	//onUnitTrained
	fName = "onUnitTrained";
	onUnitTrained.Load(&plPath, &fName);
	onUnitTrained.strCmp = (*plugins::eventNames)[UnitTrainedCode];


	//onAgentCreated
	fName = "onAgentCreated";
	onAgentCreated.Load(&plPath, &fName);
	onAgentCreated.strCmp = (*plugins::eventNames)[AgentCreatedCode];

	//onSettlementPanelOpen
	fName = "onSettlementPanelOpen";
	onSettlementPanelOpen.Load(&plPath, &fName);
	onSettlementPanelOpen.strCmp = (*plugins::eventNames)[SettlementPanelOpenCode];

	//onFinancesPanelOpen
	fName = "onFinancesPanelOpen";
	onFinancesPanelOpen.Load(&plPath, &fName);
	onFinancesPanelOpen.strCmp = (*plugins::eventNames)[FinancesPanelOpenCode];

	//onFactionSummaryPanelOpen
	fName = "onFactionSummaryPanelOpen";
	onFactionSummaryPanelOpen.Load(&plPath, &fName);
	onFactionSummaryPanelOpen.strCmp = (*plugins::eventNames)[FactionSummaryPanelOpenCode];

	//onFamilyTreePanelOpen
	fName = "onFamilyTreePanelOpen";
	onFamilyTreePanelOpen.Load(&plPath, &fName);
	onFamilyTreePanelOpen.strCmp = (*plugins::eventNames)[FamilyTreePanelOpenCode];

	//onDiplomaticStandingPanelOpen
	fName = "onDiplomaticStandingPanelOpen";
	onDiplomaticStandingPanelOpen.Load(&plPath, &fName);
	onDiplomaticStandingPanelOpen.strCmp = (*plugins::eventNames)[DiplomaticStandingPanelOpenCode];

	//onDiplomacyPanelOpen
	fName = "onDiplomacyPanelOpen";
	onDiplomacyPanelOpen.Load(&plPath, &fName);
	onDiplomacyPanelOpen.strCmp = (*plugins::eventNames)[DiplomacyPanelOpenCode];

	//onPreBattlePanelOpen
	fName = "onPreBattlePanelOpen";
	onPreBattlePanelOpen.Load(&plPath, &fName);
	onPreBattlePanelOpen.strCmp = (*plugins::eventNames)[PreBattlePanelOpenCode];

	//onNavalAutoResolvePanelOpen
	fName = "onNavalAutoResolvePanelOpen";
	onNavalAutoResolvePanelOpen.Load(&plPath, &fName);
	onNavalAutoResolvePanelOpen.strCmp = (*plugins::eventNames)[NavalAutoResolvePanelOpenCode];

	//onCharacterPanelOpen
	fName = "onCharacterPanelOpen";
	onCharacterPanelOpen.Load(&plPath, &fName);
	onCharacterPanelOpen.strCmp = (*plugins::eventNames)[CharacterPanelOpenCode];

	//onTradePanelOpen
	fName = "onTradePanelOpen";
	onTradePanelOpen.Load(&plPath, &fName);
	onTradePanelOpen.strCmp = (*plugins::eventNames)[TradePanelOpenCode];

	//onRequestBuildingAdvice
	fName = "onRequestBuildingAdvice";
	onRequestBuildingAdvice.Load(&plPath, &fName);
	onRequestBuildingAdvice.strCmp = (*plugins::eventNames)[RequestBuildingAdviceCode];

	//onRequestTrainingAdvice
	fName = "onRequestTrainingAdvice";
	onRequestTrainingAdvice.Load(&plPath, &fName);
	onRequestTrainingAdvice.strCmp = (*plugins::eventNames)[RequestTrainingAdviceCode];

	//onMessageOpen
	fName = "onMessageOpen";
	onMessageOpen.Load(&plPath, &fName);
	onMessageOpen.strCmp = (*plugins::eventNames)[MessageOpenCode];

	//onIncomingMessage
	fName = "onIncomingMessage";
	onIncomingMessage.Load(&plPath, &fName);
	onIncomingMessage.strCmp = (*plugins::eventNames)[IncomingMessageCode];

	//onMessageClosed
	fName = "onMessageClosed";
	onMessageClosed.Load(&plPath, &fName);
	onMessageClosed.strCmp = (*plugins::eventNames)[MessageClosedCode];

	//onButtonPressed
	fName = "onButtonPressed";
	onButtonPressed.Load(&plPath, &fName);
	onButtonPressed.strCmp = (*plugins::eventNames)[ButtonPressedCode];

	//onScrollClosed
	fName = "onScrollClosed";
	onScrollClosed.Load(&plPath, &fName);
	onScrollClosed.strCmp = (*plugins::eventNames)[ScrollClosedCode];

	//onScrollOpened
	fName = "onScrollOpened";
	onScrollOpened.Load(&plPath, &fName);
	onScrollOpened.strCmp = (*plugins::eventNames)[ScrollOpenedCode];

	//onUIElementVisible
	fName = "onUIElementVisible";
	onUIElementVisible.Load(&plPath, &fName);
	onUIElementVisible.strCmp = (*plugins::eventNames)[UIElementVisibleCode];

	//onSettlementScrollAdviceRequested
	fName = "onSettlementScrollAdviceRequested";
	onSettlementScrollAdviceRequested.Load(&plPath, &fName);
	onSettlementScrollAdviceRequested.strCmp = (*plugins::eventNames)[SettlementScrollAdviceRequestedCode];

	//onPreBattleScrollAdviceRequested
	fName = "onPreBattleScrollAdviceRequested";
	onPreBattleScrollAdviceRequested.Load(&plPath, &fName);
	onPreBattleScrollAdviceRequested.strCmp = (*plugins::eventNames)[PreBattleScrollAdviceRequestedCode];

	//onNavalPreBattleScrollAdviceRequested
	fName = "onNavalPreBattleScrollAdviceRequested";
	onNavalPreBattleScrollAdviceRequested.Load(&plPath, &fName);
	onNavalPreBattleScrollAdviceRequested.strCmp = (*plugins::eventNames)[NavalPreBattleScrollAdviceRequestedCode];

	//onScrollAdviceRequested
	fName = "onScrollAdviceRequested";
	onScrollAdviceRequested.Load(&plPath, &fName);
	onScrollAdviceRequested.strCmp = (*plugins::eventNames)[ScrollAdviceRequestedCode];

	//onMultiTurnMove
	fName = "onMultiTurnMove";
	onMultiTurnMove.Load(&plPath, &fName);
	onMultiTurnMove.strCmp = (*plugins::eventNames)[MultiTurnMoveCode];

	//onCollegeOfCardinalsPanelOpen
	fName = "onCollegeOfCardinalsPanelOpen";
	onCollegeOfCardinalsPanelOpen.Load(&plPath, &fName);
	onCollegeOfCardinalsPanelOpen.strCmp = (*plugins::eventNames)[CollegeOfCardinalsPanelOpenCode];

	//onGuildUpgraded
	fName = "onGuildUpgraded";
	onGuildUpgraded.Load(&plPath, &fName);
	onGuildUpgraded.strCmp = (*plugins::eventNames)[GuildUpgradedCode];

	//onGuildDestroyed
	fName = "onGuildDestroyed";
	onGuildDestroyed.Load(&plPath, &fName);
	onGuildDestroyed.strCmp = (*plugins::eventNames)[GuildDestroyedCode];

	//onNewAdmiralCreated
	fName = "onNewAdmiralCreated";
	onNewAdmiralCreated.Load(&plPath, &fName);
	onNewAdmiralCreated.strCmp = (*plugins::eventNames)[NewAdmiralCreatedCode];

	//onShortcutTriggered
	fName = "onShortcutTriggered";
	onShortcutTriggered.Load(&plPath, &fName);
	onShortcutTriggered.strCmp = (*plugins::eventNames)[ShortcutTriggeredCode];

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
