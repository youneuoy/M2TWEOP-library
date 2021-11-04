#include "pch.h"
#include "plugins.h"
#include "tests.h"

#include "smallFuncs.h"
#include "techFuncs.h"

#include "eduThings.h"
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
void __fastcall plugins::onEvent(DWORD** vTab)
{
	DWORD adr = (*vTab)[43];
	char* event = nullptr;
	_asm
	{
		mov eax, adr
		call eax
		mov event, eax
	}

	if (event == nullptr)return;

	/*if (strcmp(event, "CharacterSelected") == 0)
	{
		
		generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);

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
		if (compareEvent(event, &pl->onFactionTurnStart.stringAdr, pl->onFactionTurnStart.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFactionTurnStart))(fac);
		}
		else if (compareEvent(event, &pl->onPreFactionTurnStart.stringAdr, pl->onPreFactionTurnStart.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onPreFactionTurnStart))(fac);
		}
		else if (compareEvent(event, &pl->onFactionTurnEnd.stringAdr, pl->onFactionTurnEnd.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFactionTurnEnd))(fac);
		}
		else if (compareEvent(event, &pl->onFactionNewCapital.stringAdr, pl->onFactionNewCapital.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFactionNewCapital))(fac);
		}
		else if (compareEvent(event, &pl->onFactionWarDeclared.stringAdr, pl->onFactionWarDeclared.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onFactionWarDeclared))(fac, fac2);
		}
		else if (compareEvent(event, &pl->onFactionAllianceDeclared.stringAdr, pl->onFactionAllianceDeclared.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onFactionAllianceDeclared))(fac, fac2);
		}
		else if (compareEvent(event, &pl->onFactionTradeAgreementMade.stringAdr, pl->onFactionTradeAgreementMade.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onFactionTradeAgreementMade))(fac, fac2);
		}
		else if (compareEvent(event, &pl->onFactionBreakAlliance.stringAdr, pl->onFactionBreakAlliance.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onFactionBreakAlliance))(fac, fac2);
		}
		else if (compareEvent(event, &pl->onGiveMoney.stringAdr, pl->onGiveMoney.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			float money;
			techFuncs::Read((DWORD)&vTab[3], &money, 4);
			(*(*pl->onGiveMoney))(fac, fac2, money);
		}
		else if (compareEvent(event, &pl->onUpdateAttitude.stringAdr, pl->onUpdateAttitude.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onUpdateAttitude))(fac, fac2);
		}
		else if (compareEvent(event, &pl->onDemeanour.stringAdr, pl->onDemeanour.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			float amount;
			techFuncs::Read((DWORD)&vTab[3], &amount, 4);
			(*(*pl->onDemeanour))(fac, fac2, amount);
		}
		else if (compareEvent(event, &pl->onCharacterSelected.stringAdr, pl->onCharacterSelected.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);

			(*(*pl->onCharacterSelected))(prs);


		}
		else if (compareEvent(event, &pl->onGeneralAssaultsGeneral.stringAdr, pl->onGeneralAssaultsGeneral.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			generalCharacterictics* prs2 = reinterpret_cast<generalCharacterictics*>(vTab[2]);


			(*(*pl->onGeneralAssaultsGeneral))(prs, prs2);
		}
		else if (compareEvent(event, &pl->onGeneralAssaultsResidence.stringAdr, pl->onGeneralAssaultsResidence.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[2]);
			fortStruct* frt = fastFuncts::findFort(setl->xCoord, setl->yCoord);

			if (frt != nullptr)setl = nullptr;

			(*(*pl->onGeneralAssaultsResidence))(prs, setl, frt);
		}
		else if (compareEvent(event, &pl->onGeneralCaptureSettlement.stringAdr, pl->onGeneralCaptureSettlement.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onGeneralCaptureSettlement))(prs, setl);
		}
		else if (compareEvent(event, &pl->onGeneralCaptureResidence.stringAdr, pl->onGeneralCaptureResidence.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			(*(*pl->onGeneralCaptureResidence))(prs);
		}
		else if (compareEvent(event, &pl->onSiegeEquipmentCompleted.stringAdr, pl->onSiegeEquipmentCompleted.strCmp))
		{
			settlementStruct* setl = reinterpret_cast<settlementStruct*>(vTab[1]);
			fortStruct* frt = fastFuncts::findFort(setl->xCoord, setl->yCoord);

			if (frt != nullptr)setl = nullptr;
			(*(*pl->onSiegeEquipmentCompleted))(setl, frt);
		}
		else if (compareEvent(event, &pl->onPostBattle.stringAdr, pl->onPostBattle.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			(*(*pl->onPostBattle))(prs);
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
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onOccupySettlement))(prs, fac);
		}
		else if (compareEvent(event, &pl->onExterminatePopulation.stringAdr, pl->onExterminatePopulation.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onExterminatePopulation))(prs, fac);
		}
		else if (compareEvent(event, &pl->onSackSettlement.stringAdr, pl->onSackSettlement.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
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
		else if (compareEvent(event, &pl->onEventCounter.stringAdr, pl->onEventCounter.strCmp))
		{
			char* str = reinterpret_cast<char*>(vTab[1]);
			(*(*pl->onEventCounter))(str);
		}
		else if (compareEvent(event, &pl->onPopeAcceptsCrusadeTarget.stringAdr, pl->onPopeAcceptsCrusadeTarget.strCmp))
		{
			void* crusade = reinterpret_cast<void*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onPopeAcceptsCrusadeTarget))(crusade, sett);
		}
		else if (compareEvent(event, &pl->onCrusadeCalled.stringAdr, pl->onCrusadeCalled.strCmp))
		{
			void* crusade = reinterpret_cast<void*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onCrusadeCalled))(crusade, sett);
		}
		else if (compareEvent(event, &pl->onCrusadeEnds.stringAdr, pl->onCrusadeEnds.strCmp))
		{
			void* crusade = reinterpret_cast<void*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onCrusadeEnds))(crusade, sett);
		}
		else if (compareEvent(event, &pl->onPopeRejectsCrusadeTarget.stringAdr, pl->onPopeRejectsCrusadeTarget.strCmp))
		{
			void* crusade = reinterpret_cast<void*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			(*(*pl->onPopeRejectsCrusadeTarget))(crusade, sett);
		}
		else if (compareEvent(event, &pl->onArmyTakesCrusadeTarget.stringAdr, pl->onArmyTakesCrusadeTarget.strCmp))
		{
			void* crusade = reinterpret_cast<void*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			stackStruct* army = reinterpret_cast<stackStruct*>(vTab[3]);
			(*(*pl->onArmyTakesCrusadeTarget))(crusade, sett, army);
		}
		else if (compareEvent(event, &pl->onFactionExcommunicated.stringAdr, pl->onFactionExcommunicated.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onFactionExcommunicated))(fac);
		}
		else if (compareEvent(event, &pl->onDisaster.stringAdr, pl->onDisaster.strCmp))
		{
			int eType = (int)vTab[1];
			(*(*pl->onDisaster))(eType);
		}
		else if (compareEvent(event, &pl->onHordeFormed.stringAdr, pl->onHordeFormed.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onHordeFormed))(fac);
		}
		else if (compareEvent(event, &pl->onInquisitorAppointed.stringAdr, pl->onInquisitorAppointed.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[3]);
			(*(*pl->onInquisitorAppointed))(fac, sett);
		}
		else if (compareEvent(event, &pl->onUnitsDesertCrusade.stringAdr, pl->onUnitsDesertCrusade.strCmp))
		{
			void* crusade = reinterpret_cast<void*>(vTab[1]);
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[2]);
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[3]);
			(*(*pl->onUnitsDesertCrusade))(crusade, sett, fac);
		}
		else if (compareEvent(event, &pl->onPopeElected.stringAdr, pl->onPopeElected.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			(*(*pl->onPopeElected))(fac, fac2);
		}
		else if (compareEvent(event, &pl->onVotedForPope.stringAdr, pl->onVotedForPope.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			(*(*pl->onVotedForPope))(fac);
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
		else if (compareEvent(event, &pl->onUnitDisbanded.stringAdr, pl->onUnitDisbanded.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			unit* un = reinterpret_cast<unit*>(vTab[2]);
			(*(*pl->onUnitDisbanded))(fac, un);
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
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			int prsType = (int)vTab[2];
			settlementStruct* sett = reinterpret_cast<settlementStruct*>(vTab[3]);
			(*(*pl->onAgentCreated))(prs, prsType, sett);
		}
		else if (compareEvent(event, &pl->onObjSeen.stringAdr, pl->onObjSeen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[2]);
			int xC = (int)vTab[3];
			int yC = (int)vTab[4];

			(*(*pl->onObjSeen))(fac, fac2, xC, yC);
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
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
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
			EduEntry* edEnt = reinterpret_cast<EduEntry*>(vTab[2]);
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
		else if (compareEvent(event, &pl->onTileSeen.stringAdr, pl->onTileSeen.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			int x = (int)vTab[2];
			int y = (int)vTab[3];
			(*(*pl->onTileSeen))(fac, x, y);
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
		else if (compareEvent(event, &pl->onUngarrisonedFort.stringAdr, pl->onUngarrisonedFort.strCmp))
		{
			fortStruct* str = reinterpret_cast<fortStruct*>(vTab[1]);
			(*(*pl->onUngarrisonedFort))(str);
		}
		else if (compareEvent(event, &pl->onMultiTurnMove.stringAdr, pl->onMultiTurnMove.strCmp))
		{
			generalCharacterictics* prs = reinterpret_cast<generalCharacterictics*>(vTab[1]);
			/*if (prs->gen->armyLeaded)
			{
				string str = "Mailed Knights";

				unit* un = FastFuncts::createUnitN((char*)str.c_str());

				FastFuncts::addUnitToArmy(un, prs->gen->armyLeaded);
			}*/
			(*(*pl->onMultiTurnMove))(prs);
		}
		else if (compareEvent(event, &pl->onGameReloaded.stringAdr, pl->onGameReloaded.strCmp))
		{
			int some = (int)vTab[1];
			(*(*pl->onGameReloaded))(some);
		}
		else if (compareEvent(event, &pl->onCollegeOfCardinalsPanelOpen.stringAdr, pl->onCollegeOfCardinalsPanelOpen.strCmp))
		{
			void* colleg = reinterpret_cast<void*>(vTab[1]);
			(*(*pl->onCollegeOfCardinalsPanelOpen))(colleg);
		}
		else if (compareEvent(event, &pl->onTransgression.stringAdr, pl->onTransgression.strCmp))
		{
			factionStruct* fac = reinterpret_cast<factionStruct*>(vTab[1]);
			char* str = reinterpret_cast<char*>(*vTab[2]);
			factionStruct* fac2 = reinterpret_cast<factionStruct*>(vTab[4]);
			(*(*pl->onTransgression))(fac, str, fac2);
		}

	}
}



std::string plugins::onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup)
{
	std::string retVal;

	for (plugin* pl : pluginsCfg.plugins)
	{
		std::string *tmpVal=(*(*pl->onSelectWorldpkgdesc))(selectedRec, selectedGroup);

		retVal = *tmpVal;

		delete tmpVal;
	}

	return retVal;
}

void plugins::onClickAtTile(int x, int y)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onClickAtTile))(x,y);
	}
}

void plugins::onCampaignMapLoaded()
{
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
		(*(*pl->onEndSiege))(x,y);
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
	vector<string>files=techFuncs::loadGameLoadArchive(savePath);
	

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
		vector<string>*plugFiles= (*(*pl->onSaveGamePl))(savePath);

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

void plugins::onChangeImGuiCtx(ImGuiContext* imCtx)
{
	for (plugin* pl : pluginsCfg.plugins)
	{
		(*(*pl->onChangeImGuiContext))(imCtx);
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
		"UngarrisonedFort"
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


	//onPreFactionTurnStart
	fName = "onPreFactionTurnStart";
	onPreFactionTurnStart.Load(&plPath, &fName);
	onPreFactionTurnStart.strCmp = (*plugins::eventNames)[PreFactionTurnStartNumCode];

	//onFactionTurnStart
	fName = "onFactionTurnStart";
	onFactionTurnStart.Load(&plPath, &fName);
	onFactionTurnStart.strCmp = (*plugins::eventNames)[FactionTurnStartCode];

	//onFactionTurnEnd
	fName = "onFactionTurnEnd";
	onFactionTurnEnd.Load(&plPath, &fName);
	onFactionTurnEnd.strCmp = (*plugins::eventNames)[FactionTurnEndCode];

	//onFactionNewCapital
	fName = "onFactionNewCapital";
	onFactionNewCapital.Load(&plPath, &fName);
	onFactionNewCapital.strCmp = (*plugins::eventNames)[FactionNewCapitalCode];

	//onFactionWarDeclared
	fName = "onFactionWarDeclared";
	onFactionWarDeclared.Load(&plPath, &fName);
	onFactionWarDeclared.strCmp = (*plugins::eventNames)[FactionWarDeclaredCode];

	//onFactionAllianceDeclared
	fName = "onFactionAllianceDeclared";
	onFactionAllianceDeclared.Load(&plPath, &fName);
	onFactionAllianceDeclared.strCmp = (*plugins::eventNames)[FactionAllianceDeclaredCode];

	//onFactionTradeAgreementMade
	fName = "onFactionTradeAgreementMade";
	onFactionTradeAgreementMade.Load(&plPath, &fName);
	onFactionTradeAgreementMade.strCmp = (*plugins::eventNames)[FactionTradeAgreementMadeCode];

	//onFactionBreakAlliance
	fName = "onFactionBreakAlliance";
	onFactionBreakAlliance.Load(&plPath, &fName);
	onFactionBreakAlliance.strCmp = (*plugins::eventNames)[FactionBreakAllianceCode];


	//onGiveMoney
	fName = "onGiveMoney";
	onGiveMoney.Load(&plPath, &fName);
	onGiveMoney.strCmp = (*plugins::eventNames)[GiveMoneyCode];


	//onUpdateAttitude
	fName = "onUpdateAttitude";
	onUpdateAttitude.Load(&plPath, &fName);
	onUpdateAttitude.strCmp = (*plugins::eventNames)[UpdateAttitudeCode];

	//onDemeanour
	fName = "onDemeanour";
	onDemeanour.Load(&plPath, &fName);
	onDemeanour.strCmp = (*plugins::eventNames)[DemeanourCode];


	//onCharacterSelected
	fName = "onCharacterSelected";
	onCharacterSelected.Load(&plPath, &fName);
	onCharacterSelected.strCmp = (*plugins::eventNames)[CharacterSelectedCode];


	//onGeneralAssaultsGeneral
	fName = "onGeneralAssaultsGeneral";
	onGeneralAssaultsGeneral.Load(&plPath, &fName);
	onGeneralAssaultsGeneral.strCmp = (*plugins::eventNames)[GeneralAssaultsGeneralCode];

	//onGeneralAssaultsResidence
	fName = "onGeneralAssaultsResidence";
	onGeneralAssaultsResidence.Load(&plPath, &fName);
	onGeneralAssaultsResidence.strCmp = (*plugins::eventNames)[GeneralAssaultsResidenceCode];

	//onGeneralCaptureSettlement
	fName = "onGeneralCaptureSettlement";
	onGeneralCaptureSettlement.Load(&plPath, &fName);
	onGeneralCaptureSettlement.strCmp = (*plugins::eventNames)[GeneralCaptureSettlementCode];

	//onGeneralCaptureResidence
	fName = "onGeneralCaptureResidence";
	onGeneralCaptureResidence.Load(&plPath, &fName);
	onGeneralCaptureResidence.strCmp = (*plugins::eventNames)[GeneralCaptureResidenceCode];


	//onSiegeEquipmentCompleted
	fName = "onSiegeEquipmentCompleted";
	onSiegeEquipmentCompleted.Load(&plPath, &fName);
	onSiegeEquipmentCompleted.strCmp = (*plugins::eventNames)[SiegeEquipmentCompletedCode];


	//onPostBattle
	fName = "onPostBattle";
	onPostBattle.Load(&plPath, &fName);
	onPostBattle.strCmp = (*plugins::eventNames)[PostBattleCode];

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


	//onEventCounter
	fName = "onEventCounter";
	onEventCounter.Load(&plPath, &fName);
	onEventCounter.strCmp = (*plugins::eventNames)[EventCounterCode];

	//onPopeAcceptsCrusadeTarget
	fName = "onPopeAcceptsCrusadeTarget";
	onPopeAcceptsCrusadeTarget.Load(&plPath, &fName);
	onPopeAcceptsCrusadeTarget.strCmp = (*plugins::eventNames)[PopeAcceptsCrusadeTargetCode];

	//onCrusadeCalled
	fName = "onCrusadeCalled";
	onCrusadeCalled.Load(&plPath, &fName);
	onCrusadeCalled.strCmp = (*plugins::eventNames)[CrusadeCalledCode];

	//onCrusadeEnds
	fName = "onCrusadeEnds";
	onCrusadeEnds.Load(&plPath, &fName);
	onCrusadeEnds.strCmp = (*plugins::eventNames)[CrusadeEndsCode];

	//onPopeRejectsCrusadeTarget
	fName = "onPopeRejectsCrusadeTarget";
	onPopeRejectsCrusadeTarget.Load(&plPath, &fName);
	onPopeRejectsCrusadeTarget.strCmp = (*plugins::eventNames)[PopeRejectsCrusadeTargetCode];

	//onArmyTakesCrusadeTarget
	fName = "onArmyTakesCrusadeTarget";
	onArmyTakesCrusadeTarget.Load(&plPath, &fName);
	onArmyTakesCrusadeTarget.strCmp = (*plugins::eventNames)[ArmyTakesCrusadeTargetCode];

	//onFactionExcommunicated
	fName = "onFactionExcommunicated";
	onFactionExcommunicated.Load(&plPath, &fName);
	onFactionExcommunicated.strCmp = (*plugins::eventNames)[FactionExcommunicatedCode];

	//onDisaster
	fName = "onDisaster";
	onDisaster.Load(&plPath, &fName);
	onDisaster.strCmp = (*plugins::eventNames)[DisasterCode];

	//onHordeFormed
	fName = "onHordeFormed";
	onHordeFormed.Load(&plPath, &fName);
	onHordeFormed.strCmp = (*plugins::eventNames)[HordeFormedCode];

	//onInquisitorAppointed
	fName = "onInquisitorAppointed";
	onInquisitorAppointed.Load(&plPath, &fName);
	onInquisitorAppointed.strCmp = (*plugins::eventNames)[InquisitorAppointedCode];

	//onUnitsDesertCrusade
	fName = "onUnitsDesertCrusade";
	onUnitsDesertCrusade.Load(&plPath, &fName);
	onUnitsDesertCrusade.strCmp = (*plugins::eventNames)[UnitsDesertCrusadeCode];


	//onPopeElected
	fName = "onPopeElected";
	onPopeElected.Load(&plPath, &fName);
	onPopeElected.strCmp = (*plugins::eventNames)[PopeElectedCode];

	//onVotedForPope
	fName = "onVotedForPope";
	onVotedForPope.Load(&plPath, &fName);
	onVotedForPope.strCmp = (*plugins::eventNames)[VotedForPopeCode];

	//onAssassinCaughtAttackingPope
	fName = "onAssassinCaughtAttackingPope";
	onAssassinCaughtAttackingPope.Load(&plPath, &fName);
	onAssassinCaughtAttackingPope.strCmp = (*plugins::eventNames)[AssassinCaughtAttackingPopeCode];

	//onAddedToTrainingQueue
	fName = "onAddedToTrainingQueue";
	onAddedToTrainingQueue.Load(&plPath, &fName);
	onAddedToTrainingQueue.strCmp = (*plugins::eventNames)[AddedToTrainingQueueCode];


	//onUnitDisbanded
	fName = "onUnitDisbanded";
	onUnitDisbanded.Load(&plPath, &fName);
	onUnitDisbanded.strCmp = (*plugins::eventNames)[UnitDisbandedCode];

	//onUnitTrained
	fName = "onUnitTrained";
	onUnitTrained.Load(&plPath, &fName);
	onUnitTrained.strCmp = (*plugins::eventNames)[UnitTrainedCode];


	//onAgentCreated
	fName = "onAgentCreated";
	onAgentCreated.Load(&plPath, &fName);
	onAgentCreated.strCmp = (*plugins::eventNames)[AgentCreatedCode];

	//onObjSeen
	fName = "onObjSeen";
	onObjSeen.Load(&plPath, &fName);
	onObjSeen.strCmp = (*plugins::eventNames)[ObjSeenCode];

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

	//onTileSeen
	fName = "onTileSeen";
	onTileSeen.Load(&plPath, &fName);
	onTileSeen.strCmp = (*plugins::eventNames)[TileSeenCode];

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

	//onUngarrisonedFort
	fName = "onUngarrisonedFort";
	onUngarrisonedFort.Load(&plPath, &fName);
	onUngarrisonedFort.strCmp = (*plugins::eventNames)[UngarrisonedFortCode];

	//onMultiTurnMove
	fName = "onMultiTurnMove";
	onMultiTurnMove.Load(&plPath, &fName);
	onMultiTurnMove.strCmp = (*plugins::eventNames)[MultiTurnMoveCode];

	//onGameReloaded
	fName = "onGameReloaded";
	onGameReloaded.Load(&plPath, &fName);
	onGameReloaded.strCmp = (*plugins::eventNames)[GameReloadedCode];

	//onCollegeOfCardinalsPanelOpen
	fName = "onCollegeOfCardinalsPanelOpen";
	onCollegeOfCardinalsPanelOpen.Load(&plPath, &fName);
	onCollegeOfCardinalsPanelOpen.strCmp = (*plugins::eventNames)[GameReloadedCode];

	//onTransgression
	fName = "onTransgression";
	onTransgression.Load(&plPath, &fName);
	onTransgression.strCmp = (*plugins::eventNames)[TransgressionCode];





	//drawOnEndScene
	fName = "drawOnEndScene";
	drawOnEndScene.Load(&plPath, &fName);

	//onReset
	fName = "onReset";
	onReset.Load(&plPath, &fName);


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
