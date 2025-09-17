///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "events.h"
#include "gameHelpers.h"
#include "character.h"
#include "settlement.h"
#include "luaPlugin.h"
#include "faction.h"
#include "fort.h"
#include "army.h"
#include "characterRecord.h"
#include "unit.h"
#include "battle.h"
#include "strategyMap.h"
#include "campaignAi.h"
using namespace std;

namespace gameEvents
{
	void onEventWrapper(const DWORD eventAddr, DWORD** vTab)
	{
		if (const auto currentEvent = gameEventManager::getEvent(eventAddr); currentEvent != nullptr)
			currentEvent->callEvent(vTab);
	}

	EventBase* gameEventManager::getEvent(DWORD key)
	{
		if (const auto it = m_Events.find(key); it != m_Events.end())
			return it->second.get();
		return nullptr;
	}

	std::unordered_map<DWORD, std::unique_ptr<EventBase>> gameEventManager::m_Events = {};

	template<EventType EvType> 
	int Event<EvType>::callEvent(DWORD** vTab)
	{
		if (EvType == EventType::standardEvent)
		{
			auto eventData = reinterpret_cast<eventTrigger*>(vTab);
			if (funk != nullptr) {
				tryLuaBasicEventFunk((*funk)(eventData))
			}
			return 1;
		}
		if (EvType == EventType::attackResidenceEvent)
		{
			settlementStruct* settlement = nullptr;
			fortStruct* fort = nullptr;
			auto eventData = reinterpret_cast<eventTrigger*>(vTab);
			const auto character = eventData->getEventCharacterRecord();
			if (const auto army = character->gen->army; army && army->siege)
			{
				settlement = army->siege->getSiegedSettlement();
				fort = army->siege->getSiegedFort();
			}
			if (funk != nullptr) {
				tryLuaBasicEventFunk((*funk)(eventData, settlement, fort))
			}
			return 2;
		}
		return 0;
	}
	
	void initEvents()
	{
		const int gv = gameHelpers::getGameVersion() - 1;
		DWORD newCode[] = { 0x0136BFE4, 0x01326FBC };
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterTurnStart");

		newCode[0] = 0x013370DC; newCode[1] = 0x012F20BC;
		addEvent<EventType::standardEvent>(newCode[gv], "onCapturedCharacterRansomed");

		newCode[0] = 0x01337194; newCode[1] = 0x012F2174;
		addEvent<EventType::standardEvent>(newCode[gv], "onCapturedCharacterReleased");

		newCode[0] = 0x0133DF5C; newCode[1] = 0x012F8F3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onFatherDiesNatural");

		newCode[0] = 0x01366D54; newCode[1] = 0x01321D2C;
		addEvent<EventType::standardEvent>(newCode[gv], "onPreBattleWithdrawal");

		newCode[0] = 0x01367ADC; newCode[1] = 0x01322AB4;
		addEvent<EventType::standardEvent>(newCode[gv], "onPostBattle");

		newCode[0] = 0x01369E3C; newCode[1] = 0x01324E14;
		addEvent<EventType::standardEvent>(newCode[gv], "onHireMercenaries");

		newCode[0] = 0x01369F0C; newCode[1] = 0x01324EE4;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralCaptureResidence");

		newCode[0] = 0x0136A0B4; newCode[1] = 0x0132508C;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderDestroyedFaction");

		newCode[0] = 0x0136A584; newCode[1] = 0x0132555C;
		addEvent<EventType::standardEvent>(newCode[gv], "onOfferedForAdoption");

		newCode[0] = 0x0136A65C; newCode[1] = 0x01325634;
		addEvent<EventType::standardEvent>(newCode[gv], "onLesserGeneralOfferedForAdoption");

		newCode[0] = 0x0136A72C; newCode[1] = 0x01325704;
		addEvent<EventType::standardEvent>(newCode[gv], "onOfferedForMarriage");

		newCode[0] = 0x0136A7F4; newCode[1] = 0x013257CC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBrotherAdopted");

		newCode[0] = 0x0136A8C4; newCode[1] = 0x0132589C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBecomesFactionLeader");

		newCode[0] = 0x0136A994; newCode[1] = 0x0132596C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBecomesFactionHeir");

		newCode[0] = 0x0136AA64; newCode[1] = 0x01325A3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCeasedFactionLeader");

		newCode[0] = 0x0136AB34; newCode[1] = 0x01325B0C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCeasedFactionHeir");

		newCode[0] = 0x0136B54C; newCode[1] = 0x01326524;
		addEvent<EventType::standardEvent>(newCode[gv], "onBirth");

		newCode[0] = 0x0136B61C; newCode[1] = 0x013265F4;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterComesOfAge");

		newCode[0] = 0x0136B6EC; newCode[1] = 0x013266C4;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterMarries");

		newCode[0] = 0x0136B7C4; newCode[1] = 0x0132679C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterMarriesPrincess");

		newCode[0] = 0x0136B89C; newCode[1] = 0x01326874;
		addEvent<EventType::standardEvent>(newCode[gv], "onMarriageAlliancePossible");

		newCode[0] = 0x0136B96C; newCode[1] = 0x01326944;
		addEvent<EventType::standardEvent>(newCode[gv], "onMarriageAllianceOffered");

		newCode[0] = 0x0136BA3C; newCode[1] = 0x01326A14;
		addEvent<EventType::standardEvent>(newCode[gv], "onPriestBecomesHeretic");

		newCode[0] = 0x0136BB0C; newCode[1] = 0x01326AE4;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterNearHeretic");

		newCode[0] = 0x0136BBDC; newCode[1] = 0x01326BB4;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterNearWitch");

		newCode[0] = 0x0136BCAC; newCode[1] = 0x01326C84;
		addEvent<EventType::standardEvent>(newCode[gv], "onCardinalPromoted");

		newCode[0] = 0x0136BF14; newCode[1] = 0x01326EEC;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterBecomesAFather");

		newCode[0] = 0x0136C25C; newCode[1] = 0x01327234;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralDevastatesTile");

		newCode[0] = 0x0136CD64; newCode[1] = 0x01327D3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onExecutesASpyOnAMission");

		newCode[0] = 0x0136CFE4; newCode[1] = 0x01327FBC;
		addEvent<EventType::standardEvent>(newCode[gv], "onExecutesAnAssassinOnAMission");

		newCode[0] = 0x0136D26C; newCode[1] = 0x01328244;
		addEvent<EventType::standardEvent>(newCode[gv], "onSufferAssassinationAttempt");

		newCode[0] = 0x0136D414; newCode[1] = 0x013283EC;
		addEvent<EventType::standardEvent>(newCode[gv], "onSufferAcquisitionAttempt");

		newCode[0] = 0x0136D5AC; newCode[1] = 0x01328584;
		addEvent<EventType::standardEvent>(newCode[gv], "onSufferMarriageAttempt");

		newCode[0] = 0x0136D754; newCode[1] = 0x0132872C;
		addEvent<EventType::standardEvent>(newCode[gv], "onSufferDenouncementAttempt");

		newCode[0] = 0x0136D8EC; newCode[1] = 0x013288C4;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderOrderedSabotage");

		newCode[0] = 0x0136DA84; newCode[1] = 0x01328A5C;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderOrderedBribery");

		newCode[0] = 0x0136DB4C; newCode[1] = 0x01328B24;
		addEvent<EventType::standardEvent>(newCode[gv], "onAcceptBribe");

		newCode[0] = 0x0136DC14; newCode[1] = 0x01328BEC;
		addEvent<EventType::standardEvent>(newCode[gv], "onRefuseBribe");

		newCode[0] = 0x0136DCDC; newCode[1] = 0x01328CB4;
		addEvent<EventType::standardEvent>(newCode[gv], "onInsurrection");

		newCode[0] = 0x0136DE7C; newCode[1] = 0x01328E54;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderOrderedDiplomacy");

		newCode[0] = 0x0136E394; newCode[1] = 0x0132936C;
		addEvent<EventType::standardEvent>(newCode[gv], "onNewAdmiralCreated");

		newCode[0] = 0x0136EC8C; newCode[1] = 0x01329C64;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorBuildingDestroyed");

		newCode[0] = 0x0136F154; newCode[1] = 0x0132A12C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorThrowGames");

		newCode[0] = 0x0136F224; newCode[1] = 0x0132A1FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorThrowRaces");

		newCode[0] = 0x013703A4; newCode[1] = 0x0132B37C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterSelected");

		newCode[0] = 0x01370474; newCode[1] = 0x0132B44C;
		addEvent<EventType::standardEvent>(newCode[gv], "onEnemyCharacterSelected");

		newCode[0] = 0x0137060C; newCode[1] = 0x0132B5E4;
		addEvent<EventType::standardEvent>(newCode[gv], "onMultiTurnMove");

		newCode[0] = 0x013714F4; newCode[1] = 0x0132C4CC;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterPanelOpen");

		//standardEvent
		newCode[0] = 0x0136C8EC; newCode[1] = 0x013278C4;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralPrisonersRansomedCaptor");

		newCode[0] = 0x0136C9C4; newCode[1] = 0x0132799C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralPrisonersRansomedCaptive");

		newCode[0] = 0x0136CAA4; newCode[1] = 0x01327A7C;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionLeaderPrisonersRansomedCaptor");

		newCode[0] = 0x0136CB84; newCode[1] = 0x01327B5C;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionLeaderPrisonersRansomedCaptive");

		//spyMissionEvent
		newCode[0] = 0x0136CC94; newCode[1] = 0x01327C6C;
		addEvent<EventType::standardEvent>(newCode[gv], "onSpyMission");

		//missionEvent
		newCode[0] = 0x0136E0E4; newCode[1] = 0x013290BC;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderMissionSuccess");

		newCode[0] = 0x0136DF64; newCode[1] = 0x01328F3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderMissionFailed");

		//standardEvent
		newCode[0] = 0x0136D9B4; newCode[1] = 0x0132898C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBriberyMission");

		newCode[0] = 0x0136DDAC; newCode[1] = 0x01328D84;
		addEvent<EventType::standardEvent>(newCode[gv], "onDiplomacyMission");

		//standardEvent
		newCode[0] = 0x0136CF0C; newCode[1] = 0x01327EE4;
		addEvent<EventType::standardEvent>(newCode[gv], "onAssassinationMission");

		newCode[0] = 0x0136D33C; newCode[1] = 0x01328314;
		addEvent<EventType::standardEvent>(newCode[gv], "onAcquisitionMission");

		newCode[0] = 0x0136D4DC; newCode[1] = 0x013284B4;
		addEvent<EventType::standardEvent>(newCode[gv], "onMarriageMission");

		newCode[0] = 0x0136D67C; newCode[1] = 0x01328654;
		addEvent<EventType::standardEvent>(newCode[gv], "onDenouncementMission");

		newCode[0] = 0x0136D81C; newCode[1] = 0x013287F4;
		addEvent<EventType::standardEvent>(newCode[gv], "onSabotageMission");

		//standardEvent
		newCode[0] = 0x0136CE3C; newCode[1] = 0x01327E14;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderOrderedSpyingMission");

		//standardEvent
		newCode[0] = 0x0136D194; newCode[1] = 0x0132816C;
		addEvent<EventType::standardEvent>(newCode[gv], "onLeaderOrderedAssassination");

		//standardEvent
		newCode[0] = 0x0136C3E4; newCode[1] = 0x013273BC;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralJoinCrusade");

		newCode[0] = 0x0136C4B4; newCode[1] = 0x0132748C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralAbandonCrusade");

		newCode[0] = 0x0136C594; newCode[1] = 0x0132756C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralArrivesCrusadeTargetRegion");

		newCode[0] = 0x0136C66C; newCode[1] = 0x01327644;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralTakesCrusadeTarget");

		//standardEvent
		newCode[0] = 0x0136C0B4; newCode[1] = 0x0132708C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterTurnEnd");

		newCode[0] = 0x0136C18C; newCode[1] = 0x01327164;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterTurnEndInSettlement");

		//standardEvents
		newCode[0] = 0x01369FE4; newCode[1] = 0x01324FBC;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralCaptureSettlement");

		newCode[0] = 0x0136A3E4; newCode[1] = 0x013253BC;
		addEvent<EventType::attackResidenceEvent>(newCode[gv], "onGeneralAssaultsResidence");

		//standardEvent
		newCode[0] = 0x0136F3C4; newCode[1] = 0x0132A39C;
		addEvent<EventType::standardEvent>(newCode[gv], "onOccupySettlement");
		newCode[0] = 0x0136F48C; newCode[1] = 0x0132A464;
		addEvent<EventType::standardEvent>(newCode[gv], "onSackSettlement");
		newCode[0] = 0x0136F55C; newCode[1] = 0x0132A534;
		addEvent<EventType::standardEvent>(newCode[gv], "onExterminatePopulation");

		//standardEvent
		newCode[0] = 0x0136EEEC; newCode[1] = 0x01329EC4;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorCityRiots");
		newCode[0] = 0x0136F084; newCode[1] = 0x0132A05C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorCityRebels");

		//standardEvents
		newCode[0] = 0x0136A4B4; newCode[1] = 0x0132548C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGeneralAssaultsGeneral");
		newCode[0] = 0x0136C75C; newCode[1] = 0x01327734;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterAttacksCrusadingGeneral");

		//standardEvents
		newCode[0] = 0x0136A254; newCode[1] = 0x0132522C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCharacterDamagedByDisaster");

		//standardEvents
		newCode[0] = 0x0136E2B4; newCode[1] = 0x0132928C;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementTurnStart");
		newCode[0] = 0x0136F2F4; newCode[1] = 0x0132A2CC;
		addEvent<EventType::standardEvent>(newCode[gv], "onUngarrisonedSettlement");
		newCode[0] = 0x0136F6F4; newCode[1] = 0x0132A6CC;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementUpgraded");
		newCode[0] = 0x0136F7C4; newCode[1] = 0x0132A79C;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementConverted");
		newCode[0] = 0x0136FA24; newCode[1] = 0x0132A9FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onSiegeEquipmentCompleted");
		newCode[0] = 0x0136FADC; newCode[1] = 0x0132AAB4;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementTurnEnd");
		newCode[0] = 0x01370544; newCode[1] = 0x0132B51C;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementSelected");
		newCode[0] = 0x013706EC; newCode[1] = 0x0132B6C4;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementPanelOpen");
		newCode[0] = 0x01370C04; newCode[1] = 0x0132BBDC;
		addEvent<EventType::standardEvent>(newCode[gv], "onRecruitmentPanelOpen");
		newCode[0] = 0x01370CD4; newCode[1] = 0x0132BCAC;
		addEvent<EventType::standardEvent>(newCode[gv], "onConstructionPanelOpen");
		newCode[0] = 0x01370D9C; newCode[1] = 0x0132BD74;
		addEvent<EventType::standardEvent>(newCode[gv], "onTradePanelOpen");
		newCode[0] = 0x01371264; newCode[1] = 0x0132C23C;
		addEvent<EventType::standardEvent>(newCode[gv], "onSettlementScrollAdviceRequested");

		//standardEvents
		newCode[0] = 0x0136F88C; newCode[1] = 0x0132A864;
		addEvent<EventType::standardEvent>(newCode[gv], "onGuildUpgraded");
		newCode[0] = 0x0136F954; newCode[1] = 0x0132A92C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGuildDestroyed");

		//standardEvents
		newCode[0] = 0x0136ED54; newCode[1] = 0x01329D2C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCityRiots");
		newCode[0] = 0x0136EE1C; newCode[1] = 0x01329DF4;
		addEvent<EventType::standardEvent>(newCode[gv], "onGiveSettlement");
		newCode[0] = 0x0136EFB4; newCode[1] = 0x01329F8C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCityRebels");
		newCode[0] = 0x0136F624; newCode[1] = 0x0132A5FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onCitySacked");

		//gameEvents
		newCode[0] = 0x0135E30C; newCode[1] = 0x013192E4;
		addEvent<EventType::standardEvent>(newCode[gv], "onAbandonShowMe");

		newCode[0] = 0x013319E4; newCode[1] = 0x012EC9C4;
		addEvent<EventType::standardEvent>(newCode[gv], "onGameReloaded");

		newCode[0] = 0x01367F5C; newCode[1] = 0x01322F34;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleWinningPlaza");

		newCode[0] = 0x01368044; newCode[1] = 0x0132301C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleStopsWinningPlaza");

		newCode[0] = 0x01368124; newCode[1] = 0x013230FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleDominatingPlaza");

		newCode[0] = 0x013681F4; newCode[1] = 0x013231CC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattngineUnmanned");

		newCode[0] = 0x013682E4; newCode[1] = 0x013232BC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattlePlayerArmyHalfDestroyed");

		newCode[0] = 0x013683BC; newCode[1] = 0x01323394;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattnemyArmyHalfDestroyed");

		newCode[0] = 0x01368584; newCode[1] = 0x0132355C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleTideofBattle");

		newCode[0] = 0x01368DA4; newCode[1] = 0x01323D7C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleFinished");

		newCode[0] = 0x01368E5C; newCode[1] = 0x01323E34;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleArmyHalfDestroyed");

		newCode[0] = 0x0139622C; newCode[1] = 0x013511FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onEscPressed");

		newCode[0] = 0x0139552C; newCode[1] = 0x013504FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onScriptedAdvice");

		newCode[0] = 0x013DB05C; newCode[1] = 0x01395DF4;
		addEvent<EventType::standardEvent>(newCode[gv], "onNavalPreBattleScrollAdviceRequested");

		newCode[0] = 0x013DB114; newCode[1] = 0x01395EAC;
		addEvent<EventType::standardEvent>(newCode[gv], "onPreBattleScrollAdviceRequested");

		newCode[0] = 0x013D9004; newCode[1] = 0x01393D9C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCollegeOfCardinalsPanelOpen");

		newCode[0] = 0x013D2074; newCode[1] = 0x0138CE0C;
		addEvent<EventType::standardEvent>(newCode[gv], "onDiplomaticStandingPanelOpen");

		newCode[0] = 0x01368FCC; newCode[1] = 0x0132355C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleWinningCombat");

		newCode[0] = 0x01368F14; newCode[1] = 0x01323EEC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleArmyTired");

		newCode[0] = 0x01367E34; newCode[1] = 0x01322E0C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleSpySuccess");

		//standardEvents
		newCode[0] = 0x013321D4; newCode[1] = 0x012ED1B4;
		addEvent<EventType::standardEvent>(newCode[gv], "onCrusadeCalled");

		newCode[0] = 0x0133778C; newCode[1] = 0x012F276C;
		addEvent<EventType::standardEvent>(newCode[gv], "onPopeAcceptsCrusadeTarget");

		newCode[0] = 0x01337844; newCode[1] = 0x012F2824;
		addEvent<EventType::standardEvent>(newCode[gv], "onPopeRejectsCrusadeTarget");

		newCode[0] = 0x0133228C; newCode[1] = 0x012ED26C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCrusadeEnds");

		//standardEvents
		newCode[0] = 0x0136AEAC; newCode[1] = 0x01325E84;
		addEvent<EventType::standardEvent>(newCode[gv], "onUnitsDesertCrusade");

		//standardEvents
		newCode[0] = 0x0136AF7C; newCode[1] = 0x01325F54;
		addEvent<EventType::standardEvent>(newCode[gv], "onArmyTakesCrusadeTarget");

		//standardEvents
		newCode[0] = 0x0136B044; newCode[1] = 0x0132601C;
		addEvent<EventType::standardEvent>(newCode[gv], "onTransgression");

		newCode[0] = 0x0136B1DC; newCode[1] = 0x013261B4;
		addEvent<EventType::standardEvent>(newCode[gv], "onForgiveness");

		//standardEvents
		newCode[0] = 0x01366E24; newCode[1] = 0x01321DFC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleAiCommenced");

		newCode[0] = 0x01366EF8; newCode[1] = 0x01321ED4;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleDelayPhaseCommenced");

		newCode[0] = 0x01366FD4; newCode[1] = 0x01321FAC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleDeploymentPhaseCommenced");

		newCode[0] = 0x013670AC; newCode[1] = 0x01322084;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleConflictPhaseCommenced");

		newCode[0] = 0x0136931C; newCode[1] = 0x013242F4;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionTurnStart");

		newCode[0] = 0x013694BC; newCode[1] = 0x01324494;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionNewCapital");

		newCode[0] = 0x013693EC; newCode[1] = 0x013243C4;
		addEvent<EventType::standardEvent>(newCode[gv], "onPreFactionTurnStart");

		newCode[0] = 0x01369A5C; newCode[1] = 0x01324A34;
		addEvent<EventType::standardEvent>(newCode[gv], "onVotedForPope");

		newCode[0] = 0x01369B14; newCode[1] = 0x01324AEC;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionExcommunicated");

		newCode[0] = 0x01369CAC; newCode[1] = 0x01324C84;
		addEvent<EventType::standardEvent>(newCode[gv], "onHordeFormed");

		newCode[0] = 0x01369D74; newCode[1] = 0x01324D4C;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionTurnEnd");

		newCode[0] = 0x0136BD74; newCode[1] = 0x01326D4C;
		addEvent<EventType::standardEvent>(newCode[gv], "onCardinalRemoved");

		newCode[0] = 0x0136BE44; newCode[1] = 0x01326E1C;
		addEvent<EventType::standardEvent>(newCode[gv], "onInquisitorAppointed");

		newCode[0] = 0x0136D0BC; newCode[1] = 0x01328094;
		addEvent<EventType::standardEvent>(newCode[gv], "onAssassinCaughtAttackingPope");

		newCode[0] = 0x013707BC; newCode[1] = 0x0132B794;
		addEvent<EventType::standardEvent>(newCode[gv], "onFinancesPanelOpen");

		newCode[0] = 0x0137088C; newCode[1] = 0x0132B864;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionSummaryPanelOpen");

		newCode[0] = 0x0137095C; newCode[1] = 0x0132B934;
		addEvent<EventType::standardEvent>(newCode[gv], "onFamilyTreePanelOpen");

		newCode[0] = 0x01370A64; newCode[1] = 0x0132BA3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onDiplomacyPanelOpen");

		newCode[0] = 0x01370B34; newCode[1] = 0x0132BB0C;
		addEvent<EventType::standardEvent>(newCode[gv], "onPreBattlePanelOpen");

		newCode[0] = 0x01371364; newCode[1] = 0x0132C33C;
		addEvent<EventType::standardEvent>(newCode[gv], "onDeclineAutomatedSettlementManagement");

		//standardEvents
		newCode[0] = 0x0136958C; newCode[1] = 0x01324564;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionWarDeclared");

		newCode[0] = 0x0136965C; newCode[1] = 0x01324634;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionAllianceDeclared");

		newCode[0] = 0x0136972C; newCode[1] = 0x01324704;
		addEvent<EventType::standardEvent>(newCode[gv], "onInterFactionMarriage");

		newCode[0] = 0x01369804; newCode[1] = 0x013247DC;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionTradeAgreementMade");

		newCode[0] = 0x01369994; newCode[1] = 0x0132496C;
		addEvent<EventType::standardEvent>(newCode[gv], "onPopeElected");

		newCode[0] = 0x01369BE4; newCode[1] = 0x01324BBC;
		addEvent<EventType::standardEvent>(newCode[gv], "onFactionBreakAlliance");

		newCode[0] = 0x0136B2A4; newCode[1] = 0x0132627C;
		addEvent<EventType::standardEvent>(newCode[gv], "onUpdateAttitude");

		//standardEvents
		newCode[0] = 0x013698CC; newCode[1] = 0x013248A4;
		addEvent<EventType::standardEvent>(newCode[gv], "onGiveMoney");

		newCode[0] = 0x0136B36C; newCode[1] = 0x01326344;
		addEvent<EventType::standardEvent>(newCode[gv], "onDemeanour");

		//standardEvent
		newCode[0] = 0x013686FC; newCode[1] = 0x013236D4;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleArmyRouted");

		newCode[0] = 0x0136886C; newCode[1] = 0x01323844;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleReinforcementsArrive");

		newCode[0] = 0x0137171C; newCode[1] = 0x0132C6F4;
		addEvent<EventType::standardEvent>(newCode[gv], "onRequestMercenariesAdvice");

		//standardEvent
		newCode[0] = 0x013687B4; newCode[1] = 0x0132378C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleGeneralRouted");

		newCode[0] = 0x0136914C; newCode[1] = 0x01324124;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleGeneralKilled");

		//standardEvent
		newCode[0] = 0x0136B434; newCode[1] = 0x0132640C;
		addEvent<EventType::standardEvent>(newCode[gv], "onUnitDisbanded");

		//standardEvent
		newCode[0] = 0x0136E45C; newCode[1] = 0x01329434;
		addEvent<EventType::standardEvent>(newCode[gv], "onUnitTrained");

		//standardEvent
		newCode[0] = 0x0136E52C; newCode[1] = 0x01329504;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorUnitTrained");

		//standardEvent
		newCode[0] = 0x0136E614; newCode[1] = 0x013295EC;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorBuildingCompleted");

		//standardEvent
		newCode[0] = 0x0136EA14; newCode[1] = 0x013299EC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBuildingDestroyed");

		newCode[0] = 0x0136EAE4; newCode[1] = 0x01329ABC;
		addEvent<EventType::standardEvent>(newCode[gv], "onAddedToBuildingQueue");

		newCode[0] = 0x0136EBB4; newCode[1] = 0x01329B8C;
		addEvent<EventType::standardEvent>(newCode[gv], "onAddedToTrainingQueue");

		newCode[0] = 0x013715AC; newCode[1] = 0x0132C584;
		addEvent<EventType::standardEvent>(newCode[gv], "onRequestBuildingAdvice");

		newCode[0] = 0x01371664; newCode[1] = 0x0132C63C;
		addEvent<EventType::standardEvent>(newCode[gv], "onRequestTrainingAdvice");

		
		//standardEvents
		newCode[0] = 0x0136FBB4; newCode[1] = 0x0132AB8C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBuildingCompleted");

		//standardEvent
		newCode[0] = 0x0136E874; newCode[1] = 0x0132984C;
		addEvent<EventType::standardEvent>(newCode[gv], "onAgentCreated");

		newCode[0] = 0x0136E944; newCode[1] = 0x0132991C;
		addEvent<EventType::standardEvent>(newCode[gv], "onGovernorAgentCreated");

		//standardEvent
		newCode[0] = 0x0136863C; newCode[1] = 0x01323614;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleUnitGoesBerserk");

		newCode[0] = 0x01368924; newCode[1] = 0x013238FC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleSiegeEngineDestroyed");

		newCode[0] = 0x013689E4; newCode[1] = 0x013239BC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleSiegeEngineDocksWall");

		newCode[0] = 0x01368AA4; newCode[1] = 0x01323A7C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleGatesAttackedByEngine");

		newCode[0] = 0x01368B64; newCode[1] = 0x01323B3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleGatesDestroyedByEngine");

		newCode[0] = 0x01368C24; newCode[1] = 0x01323BFC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleWallsBreachedByEngine");

		newCode[0] = 0x0136908C; newCode[1] = 0x01324064;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleUnitRouts");

		//standardEvent
		newCode[0] = 0x01368CE4; newCode[1] = 0x01323CBC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleWallsCaptured");

		//standardEvent
		newCode[0] = 0x013684A4; newCode[1] = 0x0132347C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattlePlayerUnderAttackIdle");

		//standardEvents
		newCode[0] = 0x0136718C; newCode[1] = 0x01322164;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattlePlayerUnitAttacksEnemyUnit");

		newCode[0] = 0x013675D4; newCode[1] = 0x013225AC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattlePlayerUnitGoesBerserk");

		newCode[0] = 0x0136778C; newCode[1] = 0x01322764;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattlePlayerUnitRouts");

		newCode[0] = 0x0136793C; newCode[1] = 0x01322914;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattlePlayerSiegeEngineDestroyed");

		newCode[0] = 0x01367C64; newCode[1] = 0x01322C3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleGatesAttackedByPlayerEngine");

		//standardEvents
		newCode[0] = 0x0136726C; newCode[1] = 0x01322244;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleEnemyUnitAttacksPlayerUnit");

		newCode[0] = 0x013676AC; newCode[1] = 0x01322684;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleEnemyUnitGoesBerserk");

		newCode[0] = 0x0136785C; newCode[1] = 0x01322834;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattnemyUnitRouts");

		newCode[0] = 0x01367A14; newCode[1] = 0x013229EC;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattnemySiegeEngineDestroyed");

		newCode[0] = 0x01367D44; newCode[1] = 0x01322D1C;
		addEvent<EventType::standardEvent>(newCode[gv], "onBattleGatesAttackedByEnemyEngine");

		//standardEvents
		newCode[0] = 0x0136A17C; newCode[1] = 0x01325154;
		addEvent<EventType::standardEvent>(newCode[gv], "onDisaster");

		//standardEvents
		newCode[0] = 0x0136A30C; newCode[1] = 0x013252E4;
		addEvent<EventType::standardEvent>(newCode[gv], "onEventCounter");

		//standardEvents
		newCode[0] = 0x0136AC04; newCode[1] = 0x01325BDC;
		addEvent<EventType::standardEvent>(newCode[gv], "onUngarrisonedFort");

		//standardEvents
		newCode[0] = 0x013717D4; newCode[1] = 0x0132C7AC;
		addEvent<EventType::standardEvent>(newCode[gv], "onButtonPressed");
		newCode[0] = 0x0137188C; newCode[1] = 0x0132C864;
		addEvent<EventType::standardEvent>(newCode[gv], "onShortcutTriggered");
		newCode[0] = 0x01371944; newCode[1] = 0x0132C91C;
		addEvent<EventType::standardEvent>(newCode[gv], "onUIElementVisible");
		newCode[0] = 0x013719FC; newCode[1] = 0x0132C9D4;
		addEvent<EventType::standardEvent>(newCode[gv], "onScrollOpened");
		newCode[0] = 0x01371AB4; newCode[1] = 0x0132CA8C;
		addEvent<EventType::standardEvent>(newCode[gv], "onScrollClosed");
		newCode[0] = 0x01371B6C; newCode[1] = 0x0132CB44;
		addEvent<EventType::standardEvent>(newCode[gv], "onScrollAdviceRequested");
		newCode[0] = 0x01371144; newCode[1] = 0x0132C11C;
		addEvent<EventType::standardEvent>(newCode[gv], "onAdviceSupressed");

		//standardEvents
		newCode[0] = 0x0137102C; newCode[1] = 0x0132C004;
		addEvent<EventType::standardEvent>(newCode[gv], "onMessageClosed");

		//standardEvents
		newCode[0] = 0x01370E9C; newCode[1] = 0x0132BE74;
		addEvent<EventType::standardEvent>(newCode[gv], "onIncomingMessage");
		newCode[0] = 0x01370F64; newCode[1] = 0x0132BF3C;
		addEvent<EventType::standardEvent>(newCode[gv], "onMessageOpen");

		//standardEvents
		newCode[0] = 0x0136ACCC; newCode[1] = 0x01325CA4;
		addEvent<EventType::standardEvent>(newCode[gv], "onTileSeen");

		//standardEvents
		newCode[0] = 0x0136AD8C; newCode[1] = 0x01325D64;
		addEvent<EventType::standardEvent>(newCode[gv], "onObjSeen");
	}

	template<EventType EvType>
	void gameEventManager::addEvent(const DWORD key, const char* name)
	{
		m_Events[key] = std::make_unique<Event<EvType>>(name, plugData::data.luaAll.luaState);
	}
	
	template<EventType EvType>
	void addEvent(const DWORD key, const char* name)
	{
		gameEventManager::addEvent<EvType>(key, name);
	}

	regionStruct* getCharacterRegion(const characterRecord* character)
	{
		return stratMapHelpers::getRegion(stratMapHelpers::getTile(character->gen->xCoord, character->gen->yCoord)->regionId);
	}

void onReadGameDbsAtStart()
{
	if (plugData::data.luaAll.onReadGameDbsAtStart != nullptr)
	{
		tryLua((*plugData::data.luaAll.onReadGameDbsAtStart)());
	}
	if (plugData::data.luaAll.onEduParsed != nullptr)
	{
		tryLua((*plugData::data.luaAll.onEduParsed)());
	}
}

void onGameInit()
{
	if (plugData::data.luaAll.onGameInit != nullptr)
	{
		tryLua((*plugData::data.luaAll.onGameInit)())
	}
}

void onNewGameLoaded()
{
	if (plugData::data.luaAll.onNewGameLoaded != nullptr)
	{
		tryLua((*plugData::data.luaAll.onNewGameLoaded)())
	}
}

void onUnloadCampaign()
{
	plugData::data.luaAll.hashLoaded = false;
	if (plugData::data.luaAll.onUnloadCampaign != nullptr)
	{
		tryLua((*plugData::data.luaAll.onUnloadCampaign)());
	}
}

void onAiTurn(aiFaction* aifaction)
{
	if (plugData::data.luaAll.onAiTurn != nullptr)
	{
		tryLua((*plugData::data.luaAll.onAiTurn)(aifaction));
	}
}

void onEndSiege(int x, int y)
{
	if (plugData::data.luaAll.onEndSiege != nullptr)
	{
		tryLua((*plugData::data.luaAll.onEndSiege)(x, y));
	}
}

void onStartSiege(int x, int y)
{
	if (plugData::data.luaAll.onStartSiege != nullptr)
	{
		tryLua((*plugData::data.luaAll.onStartSiege)(x, y));
	}
}

void onClickAtTile(int x, int y)
{
	if (plugData::data.luaAll.onClickAtTile != nullptr)
	{
		tryLua((*plugData::data.luaAll.onClickAtTile)(x, y));
	}
}

void onCharacterClicked(character* enemy)
{
	if (plugData::data.luaAll.onCharacterClicked != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCharacterClicked)(enemy));
	}
}

void onCampaignTick()
{
	if (plugData::data.luaAll.onCampaignTick != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCampaignTick)());
	}
}


void onBattleTick()
{
	if (plugData::data.luaAll.onBattleTick != nullptr)
	{
		tryLua((*plugData::data.luaAll.onBattleTick)());
	}
}

void onCalculateLTGD(aiLongTermGoalDirector* ltgd)
{
	if (plugData::data.luaAll.onCalculateLTGD != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCalculateLTGD)(ltgd))
	}
}

void onSetProductionControllers(aiPersonalityValues* personality)
{
	if (plugData::data.luaAll.onSetProductionControllers != nullptr)
	{
		tryLua((*plugData::data.luaAll.onSetProductionControllers)(personality))
	}
}

std::string* onSelectWorldpkgdesc(const char* selectedRec, const char* selectedGroup)
{
	std::string tmpS;
	if (plugData::data.luaAll.onSelectWorldpkgdesc != nullptr)
	{

		auto funcResult = (*plugData::data.luaAll.onSelectWorldpkgdesc)(selectedRec, selectedGroup);
		if (!funcResult.valid())
		{

			sol::error luaError = funcResult;
			MessageBoxA(NULL, luaError.what(), "Lua exception in onSelectWorldpkgdesc() call!", NULL);
		}
		else
		{
			sol::optional<std::string>s = funcResult;
			if (s)
			{
				tmpS = s.value();
			}
		}

	}

	std::string* retS = new std::string(tmpS);

	return retS;
}

int onFortificationLevelS(settlementStruct* settlement, bool* isCastle, bool* isChanged)
{
	if (plugData::data.luaAll.onFortificationLevelS != nullptr)
	{

		auto funcResult = (*plugData::data.luaAll.onFortificationLevelS)(settlement);
		if (!funcResult.valid())
		{

			sol::error luaError = funcResult;
			MessageBoxA(NULL, luaError.what(), "Lua exception in onFortificationLevelS() call!", NULL);
		}
		else
		{
			sol::optional<std::tuple<int, bool>>retVal = funcResult;
			if (retVal)
			{
				*isChanged = true;

				std::tuple<int, bool>retValN = retVal.value();

				*isCastle = std::get<1>(retValN);
				return std::get<0>(retValN);
			}
		}

	}

	return 0;
}
float onCalculateUnitValue(eduEntry* entry, float value)
{
	if (plugData::data.luaAll.onCalculateUnitValue != nullptr)
	{
		const auto funcResult = (*plugData::data.luaAll.onCalculateUnitValue)(entry, value);
		if (!funcResult.valid())
		{
			const sol::error luaError = funcResult;
			MessageBoxA(nullptr, luaError.what(), "Lua exception in onCalculateUnitValue() call!", NULL);
		}
		else
		{
			sol::optional<float>res = funcResult;
			if (res)
				return res.value();
		}
	}

	return value;
}

void onChangeTurnNum(const int num)
{
	if (plugData::data.luaAll.onChangeTurnNumFunc)
	{
		tryLua((*plugData::data.luaAll.onChangeTurnNumFunc)(num))
	}
}

void onNewGameStart()
{
	//initReligionNames();
	if (plugData::data.luaAll.onNewGameStart != nullptr)
	{
		tryLua((*plugData::data.luaAll.onNewGameStart)())
	}
}

void onRemoveFromUnitQueue(const unitRQ* item)
{
	//initReligionNames();
	if (plugData::data.luaAll.onRemoveFromUnitQueue != nullptr)
	{
		tryLua((*plugData::data.luaAll.onRemoveFromUnitQueue)(item))
	}
}
	
void onCampaignMapLoaded()
{
	if (!plugData::data.luaAll.hashLoaded)
	{
		plugData::data.luaAll.fillHashMaps();
	}
	if (plugData::data.luaAll.onCampaignMapLoaded != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCampaignMapLoaded)())
	}
}

void onFactionSymbolSelect(int factionID)
{
	if (plugData::data.luaAll.onFactionSymbolSelect != nullptr)
	{
		tryLua((*plugData::data.luaAll.onFactionSymbolSelect)(factionID))
	}
}

void onLoadGamePl(const std::vector<std::string>* saveFiles)
{
	if (plugData::data.luaAll.onLoadSaveFile != nullptr)
	{
		sol::as_table_t<std::vector<std::string>> wrapped_vec = *saveFiles;
		tryLua((*plugData::data.luaAll.onLoadSaveFile)(wrapped_vec))
	}
}

std::vector<std::string>* onSaveGamePl(UNICODE_STRING**& savePath)
{
	sol::as_table_t<std::vector<std::string>> wrapped_vec;
	if (plugData::data.luaAll.onCreateSaveFile != nullptr)
	{
		auto funcResult = (*plugData::data.luaAll.onCreateSaveFile)();
		if (!funcResult.valid())
		{
			sol::error luaError = funcResult;
			MessageBoxA(NULL, luaError.what(), "Lua exception in onSaveGamePl()!", NULL); \
		}
		else
		{
			wrapped_vec = funcResult;
		}
	}
	std::vector<std::string>* retVec = new std::vector<std::string>();
	*retVec = wrapped_vec.value();

	return retVec;
}
}

void checkLuaFunc(sol::function** lRef)
{
	if ((*lRef)->valid() == false)
	{
		*lRef = nullptr;
	}
};

std::unordered_map<int, const char*> DISASTER_TYPES = {
	{0,"earthquake"},
	{1,"flood"},
	{2,"horde"},
	{3,"storm"},
	{4,"volcano"},
	{5,"dustbowl"},
	{6,"locusts"},
	{7,"famine"},
	{8,"plague"},
	{9,"riot"},
	{10,"fire"}
};

std::unordered_map<int, const char*> RANSOM_TYPES = {
	{0,"ransom"},
	{1,"execute"},
	{2,"release"},
	{3,"cannot_pay_ransom"}
};

std::unordered_map<int, const char*> MISSION_SUCCESS_LVL = {
	{0,"not_successful"},
	{1,"slightly_successful"},
	{2,"partly_successful"},
	{3,"highly_successful"}
};

const char* eventTrigger::getEventType()
{
	auto eventType = "unknown";
	if (const auto eventNumber = callVFunc<26, int>(this);
		DISASTER_TYPES.find(eventNumber) != DISASTER_TYPES.end()) {
		eventType = DISASTER_TYPES.at(eventNumber);
	}
	return eventType;
}

const char* eventTrigger::getMissionSuccessLevel()
{
	auto level = "unknown";
	if (const auto levelNumber = callVFunc<27, int>(this);
		MISSION_SUCCESS_LVL.find(levelNumber) != MISSION_SUCCESS_LVL.end()) {
		level = MISSION_SUCCESS_LVL.at(levelNumber);
	}
	return level;
}

//42 ransom type
const char* eventTrigger::getRansomType()
{
	auto ransomType = "unknown";
	if (const auto ransom = callVFunc<42, int>(this);
		RANSOM_TYPES.find(ransom) != RANSOM_TYPES.end()) {
		ransomType = RANSOM_TYPES.at(ransom);
	}
	return ransomType;
}
	

void luaPlugin::onPluginLoadF()
{

	struct
	{
		sol::usertype<eventTrigger> eventTrigger;
		sol::usertype<missionDetails> missionDetailsStruct;
	}typeAll;
	
	///Event Data
	//@section Event Data

	/***
	This is data that comes with game events. You need to check the event documentation to see what data is available under "Exports". The rest of the fields not stated inside "Exports" will return nil! 
	
	@tfield unit attackingUnit
	@tfield unit defendingUnit
	@tfield character stratCharacter
	@tfield characterRecord character - Note it is characterRecord, not character
	@tfield characterRecord targetCharacter - Note it is characterRecord, not character
	@tfield settlementStruct settlement
	@tfield settlementStruct targetSettlement
	@tfield fortStruct fort
	@tfield factionStruct faction
	@tfield factionStruct targetFaction
	@tfield armyStruct army
	@tfield int regionID
	@tfield int targetRegionID
	@tfield unit playerUnit
	@tfield unit enemyUnit
	@tfield buildingBattle battleBuilding
	@tfield buildingInQueue priorBuild
	@tfield string resourceDescription
	@tfield eduEntry eduEntry
	@tfield int characterType
	@tfield int targetCharacterType
	@tfield string disasterType
	@tfield string missionSuccessLevel
	@tfield int missionProbability
	@tfield missionDetailsStruct missionDetails
	@tfield int eventID
	@tfield int guildID
	@tfield string eventCounter
	@tfield coordPair coords
	@tfield int religion
	@tfield int targetReligion
	@tfield float amount
	@tfield crusadeStruct crusade
	@tfield capturedFactionInfo captureInfo
	@tfield string ransomType
	@tfield unit unit
	
	
	@table eventTrigger
	*/
	typeAll.eventTrigger = luaState.new_usertype<eventTrigger>("eventTrigger");
	typeAll.eventTrigger.set("attackingUnit", sol::property(&eventTrigger::getEventAttackingUnit));
	typeAll.eventTrigger.set("defendingUnit", sol::property(&eventTrigger::getEventDefendingUnit));
	typeAll.eventTrigger.set("stratCharacter", sol::property(&eventTrigger::getEventCharacter));
	typeAll.eventTrigger.set("character", sol::property(&eventTrigger::getEventCharacterRecord));
	typeAll.eventTrigger.set("targetCharacter", sol::property(&eventTrigger::getEventTargetNamedCharacter));
	typeAll.eventTrigger.set("settlement", sol::property(&eventTrigger::getEventSettlement));
	typeAll.eventTrigger.set("targetSettlement", sol::property(&eventTrigger::getEventTargetSettlement));
	typeAll.eventTrigger.set("fort", sol::property(&eventTrigger::getEventFort));
	typeAll.eventTrigger.set("faction", sol::property(&eventTrigger::getEventFaction));
	typeAll.eventTrigger.set("targetFaction", sol::property(&eventTrigger::getEventTargetFaction));
	typeAll.eventTrigger.set("army", sol::property(&eventTrigger::getEventArmy));
	typeAll.eventTrigger.set("regionID", sol::property(&eventTrigger::getEventRegion));
	typeAll.eventTrigger.set("targetRegionID", sol::property(&eventTrigger::getEventTargetRegion));
	typeAll.eventTrigger.set("playerUnit", sol::property(&eventTrigger::getEventPlayerUnit));
	typeAll.eventTrigger.set("enemyUnit", sol::property(&eventTrigger::getEventEnemyUnit));
	typeAll.eventTrigger.set("battleBuilding", sol::property(&eventTrigger::getBuildingBattle));
	typeAll.eventTrigger.set("priorBuild", sol::property(&eventTrigger::getPriorBuild));
	typeAll.eventTrigger.set("resourceDescription", sol::property(&eventTrigger::getResourceDescription));
	typeAll.eventTrigger.set("eduEntry", sol::property(&eventTrigger::getEduEntry));
	typeAll.eventTrigger.set("characterType", sol::property(&eventTrigger::getEventCharacterType));
	typeAll.eventTrigger.set("targetCharacterType", sol::property(&eventTrigger::getEventTargetCharacterType));
	typeAll.eventTrigger.set("disasterType", sol::property(&eventTrigger::getEventType));
	typeAll.eventTrigger.set("missionSuccessLevel", sol::property(&eventTrigger::getMissionSuccessLevel));
	typeAll.eventTrigger.set("missionProbability", sol::property(&eventTrigger::getMissionProbability));
	typeAll.eventTrigger.set("missionDetails", sol::property(&eventTrigger::getMissionDetails));
	typeAll.eventTrigger.set("eventID", sol::property(&eventTrigger::getEventID));
	typeAll.eventTrigger.set("guildID", sol::property(&eventTrigger::getEventGuild));
	typeAll.eventTrigger.set("eventCounter", sol::property(&eventTrigger::getEventCounter));
	typeAll.eventTrigger.set("coords", sol::property(&eventTrigger::getPosition));
	typeAll.eventTrigger.set("religion", sol::property(&eventTrigger::getReligion));
	typeAll.eventTrigger.set("targetReligion", sol::property(&eventTrigger::getTargetReligion));
	typeAll.eventTrigger.set("amount", sol::property(&eventTrigger::getAmount));
	typeAll.eventTrigger.set("crusade", sol::property(&eventTrigger::getCrusade));
	typeAll.eventTrigger.set("captureInfo", sol::property(&eventTrigger::getCapturedFactionInfo));
	typeAll.eventTrigger.set("ransomType", sol::property(&eventTrigger::getRansomType));
	typeAll.eventTrigger.set("unit", sol::property(&eventTrigger::getUnit));
	
	/***
	 
	@tfield string missionName
	@tfield string paybackName
	
	@table missionDetailsStruct
	*/
	typeAll.missionDetailsStruct = luaState.new_usertype<missionDetails>("missionDetailsStruct");
	typeAll.missionDetailsStruct.set("missionName", sol::property(&missionDetails::getMissionName));
	typeAll.missionDetailsStruct.set("paybackName", sol::property(&missionDetails::getPaybackName));
	
	///EOP Events
	//@section EOP Events

	/***
	Events functions list.
	Just list, use it without EventsFunctionsList.!!!

	@tfield onCharacterTurnStart onCharacterTurnStart
	@tfield onCapturedCharacterRansomed onCapturedCharacterRansomed
	@tfield onCapturedCharacterReleased onCapturedCharacterReleased
	@tfield onFatherDiesNatural onFatherDiesNatural
	@tfield onPreBattleWithdrawal onPreBattleWithdrawal
	@tfield onPostBattle onPostBattle
	@tfield onHireMercenaries onHireMercenaries
	@tfield onGeneralCaptureResidence onGeneralCaptureResidence
	@tfield onLeaderDestroyedFaction onLeaderDestroyedFaction
	@tfield onOfferedForAdoption onOfferedForAdoption
	@tfield onLesserGeneralOfferedForAdoption onLesserGeneralOfferedForAdoption
	@tfield onOfferedForMarriage onOfferedForMarriage
	@tfield onBrotherAdopted onBrotherAdopted
	@tfield onBirth onBirth
	@tfield onCharacterComesOfAge onCharacterComesOfAge
	@tfield onCharacterMarries onCharacterMarries
	@tfield onCharacterMarriesPrincess onCharacterMarriesPrincess
	@tfield onMarriageAlliancePossible onMarriageAlliancePossible
	@tfield onMarriageAllianceOffered onMarriageAllianceOffered
	@tfield onPriestBecomesHeretic onPriestBecomesHeretic
	@tfield onCharacterNearHeretic onCharacterNearHeretic
	@tfield onCharacterNearWitch onCharacterNearWitch
	@tfield onCardinalPromoted onCardinalPromoted
	@tfield onCharacterBecomesAFather onCharacterBecomesAFather
	@tfield onGeneralDevastatesTile onGeneralDevastatesTile
	@tfield onExecutesASpyOnAMission onExecutesASpyOnAMission
	@tfield onExecutesAnAssassinOnAMission onExecutesAnAssassinOnAMission
	@tfield onSufferAssassinationAttempt onSufferAssassinationAttempt
	@tfield onSufferAcquisitionAttempt onSufferAcquisitionAttempt
	@tfield onSufferMarriageAttempt onSufferMarriageAttempt
	@tfield onSufferDenouncementAttempt onSufferDenouncementAttempt
	@tfield onLeaderOrderedSabotage onLeaderOrderedSabotage
	@tfield onAcceptBribe onAcceptBribe
	@tfield onRefuseBribe onRefuseBribe
	@tfield onInsurrection onInsurrection
	@tfield onLeaderOrderedDiplomacy onLeaderOrderedDiplomacy
	@tfield onNewAdmiralCreated onNewAdmiralCreated
	@tfield onGovernorBuildingDestroyed onGovernorBuildingDestroyed
	@tfield onGovernorThrowGames onGovernorThrowGames
	@tfield onGovernorThrowRaces onGovernorThrowRaces
	@tfield onCharacterSelected onCharacterSelected
	@tfield onEnemyCharacterSelected onEnemyCharacterSelected
	@tfield onMultiTurnMove onMultiTurnMove
	@tfield onCharacterPanelOpen onCharacterPanelOpen
	@tfield onLeaderMissionSuccess onLeaderMissionSuccess
	@tfield onLeaderMissionFailed onLeaderMissionFailed
	@tfield onGeneralJoinCrusade onGeneralJoinCrusade
	@tfield onGeneralAbandonCrusade onGeneralAbandonCrusade
	@tfield onGeneralArrivesCrusadeTargetRegion onGeneralArrivesCrusadeTargetRegion
	@tfield onGeneralTakesCrusadeTarget onGeneralTakesCrusadeTarget
	@tfield onCharacterTurnEnd onCharacterTurnEnd
	@tfield onCharacterTurnEndInSettlement onCharacterTurnEndInSettlement
	@tfield onBecomesFactionLeader onBecomesFactionLeader
	@tfield onCeasedFactionLeader onCeasedFactionLeader
	@tfield onBecomesFactionHeir onBecomesFactionHeir
	@tfield onCeasedFactionHeir onCeasedFactionHeir
	@tfield onCharacterDamagedByDisaster onCharacterDamagedByDisaster
	@tfield onGeneralCaptureSettlement onGeneralCaptureSettlement
	@tfield onGeneralAssaultsResidence onGeneralAssaultsResidence
	@tfield onGeneralAssaultsGeneral onGeneralAssaultsGeneral
	@tfield onCharacterAttacksCrusadingGeneral onCharacterAttacksCrusadingGeneral
	@tfield onGeneralPrisonersRansomedCaptor onGeneralPrisonersRansomedCaptor
	@tfield onGeneralPrisonersRansomedCaptive onGeneralPrisonersRansomedCaptive
	@tfield onFactionLeaderPrisonersRansomedCaptor onFactionLeaderPrisonersRansomedCaptor
	@tfield onFactionLeaderPrisonersRansomedCaptive onFactionLeaderPrisonersRansomedCaptive
	@tfield onSpyMission onSpyMission
	@tfield onAssassinationMission onAssassinationMission
	@tfield onAcquisitionMission onAcquisitionMission
	@tfield onMarriageMission onMarriageMission
	@tfield onDenouncementMission onDenouncementMission
	@tfield onSabotageMission onSabotageMission
	@tfield onBriberyMission onBriberyMission
	@tfield onDiplomacyMission onDiplomacyMission
	@tfield onLeaderOrderedSpyingMission onLeaderOrderedSpyingMission
	@tfield onLeaderOrderedAssassination onLeaderOrderedAssassination
	@tfield onLeaderOrderedBribery onLeaderOrderedBribery
	@tfield onSettlementTurnStart onSettlementTurnStart
	@tfield onUngarrisonedSettlement onUngarrisonedSettlement
	@tfield onSettlementUpgraded onSettlementUpgraded
	@tfield onSettlementConverted onSettlementConverted
	@tfield onSiegeEquipmentCompleted onSiegeEquipmentCompleted
	@tfield onSettlementTurnEnd onSettlementTurnEnd
	@tfield onSettlementSelected onSettlementSelected
	@tfield onSettlementPanelOpen onSettlementPanelOpen
	@tfield onRecruitmentPanelOpen onRecruitmentPanelOpen
	@tfield onConstructionPanelOpen onConstructionPanelOpen
	@tfield onTradePanelOpen onTradePanelOpen
	@tfield onSettlementScrollAdviceRequested onSettlementScrollAdviceRequested
	@tfield onGuildUpgraded onGuildUpgraded
	@tfield onGuildDestroyed onGuildDestroyed
	@tfield onOccupySettlement onOccupySettlement
	@tfield onSackSettlement onSackSettlement
	@tfield onExterminatePopulation onExterminatePopulation
	@tfield onCityRiots onCityRiots
	@tfield onGiveSettlement onGiveSettlement
	@tfield onCityRebels onCityRebels
	@tfield onCitySacked onCitySacked
	@tfield onGovernorCityRiots onGovernorCityRiots
	@tfield onGovernorCityRebels onGovernorCityRebels
	@tfield onAbandonShowMe onAbandonShowMe
	@tfield onGameReloaded onGameReloaded
	@tfield onBattleWinningPlaza onBattleWinningPlaza
	@tfield onBattleStopsWinningPlaza onBattleStopsWinningPlaza
	@tfield onBattleDominatingPlaza onBattleDominatingPlaza
	@tfield onBattngineUnmanned onBattngineUnmanned
	@tfield onBattlePlayerArmyHalfDestroyed onBattlePlayerArmyHalfDestroyed
	@tfield onBattnemyArmyHalfDestroyed onBattnemyArmyHalfDestroyed
	@tfield onBattleFinished onBattleFinished
	@tfield onBattleArmyHalfDestroyed onBattleArmyHalfDestroyed
	@tfield onEscPressed onEscPressed
	@tfield onScriptedAdvice onScriptedAdvice
	@tfield onNavalPreBattleScrollAdviceRequested onNavalPreBattleScrollAdviceRequested
	@tfield onPreBattleScrollAdviceRequested onPreBattleScrollAdviceRequested
	@tfield onCollegeOfCardinalsPanelOpen onCollegeOfCardinalsPanelOpen
	@tfield onDiplomaticStandingPanelOpen onDiplomaticStandingPanelOpen
	@tfield onBattlePlayerUnderAttackIdle onBattlePlayerUnderAttackIdle
	@tfield onBattleWinningCombat onBattleWinningCombat
	@tfield onBattleArmyTired onBattleArmyTired
	@tfield onBattleSpySuccess onBattleSpySuccess
	@tfield onBattleTideofBattle onBattleTideofBattle
	@tfield onBattleUnitGoesBerserk onBattleUnitGoesBerserk
	@tfield onBattleSiegeEngineDestroyed onBattleSiegeEngineDestroyed
	@tfield onBattleSiegeEngineDocksWall onBattleSiegeEngineDocksWall
	@tfield onBattleGatesAttackedByEngine onBattleGatesAttackedByEngine
	@tfield onBattleGatesDestroyedByEngine onBattleGatesDestroyedByEngine
	@tfield onBattleWallsBreachedByEngine onBattleWallsBreachedByEngine
	@tfield onBattleWallsCaptured onBattleWallsCaptured
	@tfield onBattleUnitRouts onBattleUnitRouts
	@tfield onUnitDisbanded onUnitDisbanded
	@tfield onUnitTrained onUnitTrained
	@tfield onGovernorUnitTrained onGovernorUnitTrained
	@tfield onGovernorBuildingCompleted onGovernorBuildingCompleted
	@tfield onAgentCreated onAgentCreated
	@tfield onGovernorAgentCreated onGovernorAgentCreated
	@tfield onBuildingDestroyed onBuildingDestroyed
	@tfield onAddedToBuildingQueue onAddedToBuildingQueue
	@tfield onBuildingCompleted onBuildingCompleted
	@tfield onRequestBuildingAdvice onRequestBuildingAdvice
	@tfield onRequestTrainingAdvice onRequestTrainingAdvice
	@tfield onAddedToTrainingQueue onAddedToTrainingQueue
	@tfield onBattleArmyRouted onBattleArmyRouted
	@tfield onBattleReinforcementsArrive onBattleReinforcementsArrive
	@tfield onRequestMercenariesAdvice onRequestMercenariesAdvice
	@tfield onButtonPressed onButtonPressed
	@tfield onShortcutTriggered onShortcutTriggered
	@tfield onUIElementVisible onUIElementVisible
	@tfield onScrollOpened onScrollOpened
	@tfield onScrollClosed onScrollClosed
	@tfield onScrollAdviceRequested onScrollAdviceRequested
	@tfield onAdviceSupressed onAdviceSupressed
	@tfield onBattleGeneralRouted onBattleGeneralRouted
	@tfield onBattleGeneralKilled onBattleGeneralKilled
	@tfield onCrusadeCalled onCrusadeCalled
	@tfield onPopeAcceptsCrusadeTarget onPopeAcceptsCrusadeTarget
	@tfield onPopeRejectsCrusadeTarget onPopeRejectsCrusadeTarget
	@tfield onCrusadeEnds onCrusadeEnds
	@tfield onPreFactionTurnStart onPreFactionTurnStart
	@tfield onFactionTurnStart onFactionTurnStart
	@tfield onFactionNewCapital onFactionNewCapital
	@tfield onFactionTurnEnd onFactionTurnEnd
	@tfield onVotedForPope onVotedForPope
	@tfield onFactionExcommunicated onFactionExcommunicated
	@tfield onHordeFormed onHordeFormed
	@tfield onCardinalRemoved onCardinalRemoved
	@tfield onInquisitorAppointed onInquisitorAppointed
	@tfield onAssassinCaughtAttackingPope onAssassinCaughtAttackingPope
	@tfield onFinancesPanelOpen onFinancesPanelOpen
	@tfield onFactionSummaryPanelOpen onFactionSummaryPanelOpen
	@tfield onFamilyTreePanelOpen onFamilyTreePanelOpen
	@tfield onDiplomacyPanelOpen onDiplomacyPanelOpen
	@tfield onPreBattlePanelOpen onPreBattlePanelOpen
	@tfield onIncomingMessage onIncomingMessage
	@tfield onMessageOpen onMessageOpen
	@tfield onMessageClosed onMessageClosed
	@tfield onDeclineAutomatedSettlementManagement onDeclineAutomatedSettlementManagement
	@tfield onBattleAiCommenced onBattleAiCommenced
	@tfield onBattleDelayPhaseCommenced onBattleDelayPhaseCommenced
	@tfield onBattleDeploymentPhaseCommenced onBattleDeploymentPhaseCommenced
	@tfield onBattleConflictPhaseCommenced onBattleConflictPhaseCommenced
	@tfield onFactionWarDeclared onFactionWarDeclared
	@tfield onFactionAllianceDeclared onFactionAllianceDeclared
	@tfield onInterFactionMarriage onInterFactionMarriage
	@tfield onFactionTradeAgreementMade onFactionTradeAgreementMade
	@tfield onPopeElected onPopeElected
	@tfield onFactionBreakAlliance onFactionBreakAlliance
	@tfield onUpdateAttitude onUpdateAttitude
	@tfield onDemeanour onDemeanour
	@tfield onGiveMoney onGiveMoney
	@tfield onBattlePlayerUnitAttacksEnemyUnit onBattlePlayerUnitAttacksEnemyUnit
	@tfield onBattleEnemyUnitAttacksPlayerUnit onBattleEnemyUnitAttacksPlayerUnit
	@tfield onBattlePlayerUnitGoesBerserk onBattlePlayerUnitGoesBerserk
	@tfield onBattlePlayerUnitRouts onBattlePlayerUnitRouts
	@tfield onBattlePlayerSiegeEngineDestroyed onBattlePlayerSiegeEngineDestroyed
	@tfield onBattleGatesAttackedByPlayerEngine onBattleGatesAttackedByPlayerEngine
	@tfield onBattleEnemyUnitGoesBerserk onBattleEnemyUnitGoesBerserk
	@tfield onBattnemyUnitRouts onBattnemyUnitRouts
	@tfield onBattnemySiegeEngineDestroyed onBattnemySiegeEngineDestroyed
	@tfield onBattleGatesAttackedByEnemyEngine onBattleGatesAttackedByEnemyEngine
	@tfield onDisaster onDisaster
	@tfield onEventCounter onEventCounter
	@tfield onUngarrisonedFort onUngarrisonedFort
	@tfield onObjSeen onObjSeen
	@tfield onTileSeen onTileSeen
	@tfield onTransgression onTransgression
	@tfield onForgiveness onForgiveness
	@tfield onArmyTakesCrusadeTarget onArmyTakesCrusadeTarget
	@tfield onUnitsDesertCrusade onUnitsDesertCrusade
	@tfield draw draw
	@tfield onLoadingFonts onLoadingFonts
	@tfield onNewGameStart onNewGameStart
	@tfield onEduParsed onEduParsed
	@tfield onGameInit onGameInit
	@tfield onUnloadCampaign onUnloadCampaign
	@tfield onAiTurn onAiTurn
	@tfield onCalculateLTGD onCalculateLTGD
	@tfield onSetProductionControllers onSetProductionControllers
	@tfield onClickAtTile onClickAtTile
	@tfield onCharacterClicked onCharacterClicked
	@tfield onCampaignTick onCampaignTick
	@tfield onBattleTick onBattleTick
	@tfield onCampaignMapLoaded onCampaignMapLoaded
	@tfield onCreateSaveFile onCreateSaveFile
	@tfield onLoadSaveFile onLoadSaveFile
	@tfield onChangeTurnNum onChangeTurnNum
	@tfield onSelectWorldpkgdesc onSelectWorldpkgdesc
	@tfield onFortificationLevelS onFortificationLevelS
	@tfield onCalculateUnitValue onCalculateUnitValue
	@tfield onEndSiege onEndSiege
	@tfield onStartSiege onStartSiege
	@tfield onPluginLoad onPluginLoad
	@tfield onNewGameLoaded onNewGameLoaded
	@tfield onRemoveFromUnitQueue onRemoveFromUnitQueue
	@tfield onFactionSymbolSelect onFactionSymbolSelect

	@table EventsFunctionsList
	*/


	/***
	Called at a character's turn start.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterTurnStart
	@tparam eventTrigger eventData

	@usage
	function onCharacterTurnStart(eventData)
	--something here
	end
	*/

	/***
	A captured character has been successfully ransomed back from the enemy.
	Exports: character, faction, regionID, characterType, religion

	@function onCapturedCharacterRansomed
	@tparam eventTrigger eventData

	@usage
	function onCapturedCharacterRansomed(eventData)
	--something here
	end
	*/

	/***
	A captured character has been released by the enemy.
	Exports: character, faction, regionID, characterType, religion

	@function onCapturedCharacterReleased
	@tparam eventTrigger eventData

	@usage
	function onCapturedCharacterReleased(eventData)
	--something here
	end
	*/

	/***
	A character father died of natural causes.
	Exports: character, faction, regionID, characterType, religion

	@function onFatherDiesNatural
	@tparam eventTrigger eventData

	@usage
	function onFatherDiesNatural(eventData)
	--something here
	end
	*/

	/***
	When a battle is about to start but one of the armies withdraws.
	Exports: character, faction, regionID, characterType, religion

	@function onPreBattleWithdrawal
	@tparam eventTrigger eventData

	@usage
	function onPreBattleWithdrawal(eventData)
	--something here
	end
	*/

	/***
	When a battle has finished.
	Exports: character, faction, regionID, characterType, religion

	@function onPostBattle
	@tparam eventTrigger eventData

	@usage
	function onPostBattle(eventData)
	--something here
	end
	*/

	/***
	A General has hired some mercenaries.
	Exports: character, faction, regionID, characterType, religion

	@function onHireMercenaries
	@tparam eventTrigger eventData

	@usage
	function onHireMercenaries(eventData)
	--something here
	end
	*/

	/***
	A General has captured a residence such as a fort or watchtower.
	Exports: character, faction, regionID, characterType, religion

	@function onGeneralCaptureResidence
	@tparam eventTrigger eventData

	@usage
	function onGeneralCaptureResidence(eventData)
	--something
	end
	*/

	/***
	A faction has been destroyed.
	Exports: character, faction, regionID, characterType, religion

	@function onLeaderDestroyedFaction
	@tparam eventTrigger eventData

	@usage
	function onLeaderDestroyedFaction(eventData)
	--something
	end
	*/

	/***
	An adoption has been proposed.
	Exports: character, faction, regionID, characterType, religion

	@function onOfferedForAdoption
	@tparam eventTrigger eventData

	@usage
	function onOfferedForAdoption(eventData)
	--something
	end
	*/

	/***
	A lesser general adoption has been proposed (man of the hour event).
	Exports: character, faction, regionID, characterType, religion

	@function onLesserGeneralOfferedForAdoption
	@tparam eventTrigger eventData

	@usage
	function onLesserGeneralOfferedForAdoption(eventData)
	--something
	end
	*/

	/***
	A marriage offer has been proposed.
	Exports: character, faction, regionID, characterType, religion

	@function onOfferedForMarriage
	@tparam eventTrigger eventData

	@usage
	function onOfferedForMarriage(eventData)
	--something
	end
	*/

	/***
	A brother has been adopted.
	Exports: character, faction, regionID, characterType, religion

	@function onBrotherAdopted
	@tparam eventTrigger eventData

	@usage
	function onBrotherAdopted(eventData)
	--something
	end
	*/

	/***
	A child is born to the faction leader.
	Exports: character, faction, regionID, characterType, religion

	@function onBirth
	@tparam eventTrigger eventData

	@usage
	function onBirth(eventData)
	--something
	end
	*/

	/***
	A character has come of age.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterComesOfAge
	@tparam eventTrigger eventData

	@usage
	function onCharacterComesOfAge(eventData)
	--something
	end
	*/

	/***
	A character has married.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterMarries
	@tparam eventTrigger eventData

	@usage
	function onCharacterMarries(eventData)
	--something
	end
	*/

	/***
	A character has married a princess.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterMarriesPrincess
	@tparam eventTrigger eventData

	@usage
	function onCharacterMarriesPrincess(eventData)
	--something
	end
	*/

	/***
	A marriage alliance is possible.
	Exports: character, faction, regionID, characterType, religion

	@function onMarriageAlliancePossible
	@tparam eventTrigger eventData

	@usage
	function onMarriageAlliancePossible(eventData)
	--something
	end
	*/

	/***
	A marriage alliance has been offered.
	Exports: character, faction, regionID, characterType, religion

	@function onMarriageAllianceOffered
	@tparam eventTrigger eventData

	@usage
	function onMarriageAllianceOffered(eventData)
	--something
	end
	*/

	/***
	A priest has gone mad.
	Exports: character, faction, regionID, characterType, religion

	@function onPriestBecomesHeretic
	@tparam eventTrigger eventData

	@usage
	function onPriestBecomesHeretic(eventData)
	--something
	end
	*/

	/***
	A character is adjacent to a heretic.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterNearHeretic
	@tparam eventTrigger eventData

	@usage
	function onCharacterNearHeretic(eventData)
	--something
	end
	*/

	/***
	A character is adjacent to a witch.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterNearWitch
	@tparam eventTrigger eventData

	@usage
	function onCharacterNearWitch(eventData)
	--something
	end
	*/

	/***
	A character has been promoted to a cardinal.
	Exports: character, faction, regionID, characterType, religion

	@function onCardinalPromoted
	@tparam eventTrigger eventData

	@usage
	function onCardinalPromoted(eventData)
	--something
	end
	*/

	/***
	A character has become a father.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterBecomesAFather
	@tparam eventTrigger eventData

	@usage
	function onCharacterBecomesAFather(eventData)
	--something
	end
	*/

	/***
	A General and his army has devastated an enemy's fertile land.
	Exports: character, faction, regionID, characterType, religion

	@function onGeneralDevastatesTile
	@tparam eventTrigger eventData

	@usage
	function onGeneralDevastatesTile(eventData)
	--something
	end
	*/

	/***
	A spying mission has failed and the spy is executed by the target.
	Exports: character, faction, regionID, characterType, religion

	@function onExecutesASpyOnAMission
	@tparam eventTrigger eventData

	@usage
	function onExecutesASpyOnAMission(eventData)
	--something
	end
	*/

	/***
	An assassination mission has failed and the assassin is executed by the target.
	Exports: character, faction, regionID, characterType, religion

	@function onExecutesAnAssassinOnAMission
	@tparam eventTrigger eventData

	@usage
	function onExecutesAnAssassinOnAMission(eventData)
	--something
	end
	*/

	/***
	Someone has had an attempt on their life.
	Exports: character, faction, regionID, characterType, religion

	@function onSufferAssassinationAttempt
	@tparam eventTrigger eventData

	@usage
	function onSufferAssassinationAttempt(eventData)
	--something
	end
	*/

	/***
	Someone has had an attempt on their assets.
	Exports: character, faction, regionID, characterType, religion

	@function onSufferAcquisitionAttempt
	@tparam eventTrigger eventData

	@usage
	function onSufferAcquisitionAttempt(eventData)
	--something
	end
	*/

	/***
	Someone has had an attempt on their bachelorhood.
	Exports: character, faction, regionID, characterType, religion

	@function onSufferMarriageAttempt
	@tparam eventTrigger eventData

	@usage
	function onSufferMarriageAttempt(eventData)
	--something
	end
	*/

	/***
	Someone has had a denouncement attempt.
	Exports: character, faction, regionID, characterType, religion

	@function onSufferDenouncementAttempt
	@tparam eventTrigger eventData

	@usage
	function onSufferDenouncementAttempt(eventData)
	--something
	end
	*/

	/***
	A Faction leader has ordered a sabotage mission.
	Exports: character, faction, regionID, characterType, religion

	@function onLeaderOrderedSabotage
	@tparam eventTrigger eventData

	@usage
	function onLeaderOrderedSabotage(eventData)
	--something
	end
	*/

	/***
	Someone has been bribed.
	Exports: character, faction, regionID, characterType, religion

	@function onAcceptBribe
	@tparam eventTrigger eventData

	@usage
	function onAcceptBribe(eventData)
	--something
	end
	*/

	/***
	Someone has refused a bribe.
	Exports: character, faction, regionID, characterType, religion

	@function onRefuseBribe
	@tparam eventTrigger eventData

	@usage
	function onRefuseBribe(eventData)
	--something
	end
	*/

	/***
	Insurgence has been provoked.
	Exports: character, faction, regionID, characterType, religion

	@function onInsurrection
	@tparam eventTrigger eventData

	@usage
	function onInsurrection(eventData)
	--something
	end
	*/

	/***
	A Faction leader has ordered a diplomacy mission.
	Exports: character, faction, regionID, characterType, religion

	@function onLeaderOrderedDiplomacy
	@tparam eventTrigger eventData

	@usage
	function onLeaderOrderedDiplomacy(eventData)
	--something
	end
	*/

	/***
	A new admiral has been created for a new ship.
	Exports: stratCharacter, character, faction, regionID, characterType, religion

	@function onNewAdmiralCreated
	@tparam eventTrigger eventData

	@usage
	function onNewAdmiralCreated(eventData)
	--something
	end
	*/

	/***
	A building has been destroyed.
	Exports: character, faction, regionID, characterType, religion, settlement

	@function onGovernorBuildingDestroyed
	@tparam eventTrigger eventData

	@usage
	function onGovernorBuildingDestroyed(eventData)
	--something
	end
	*/

	/***
	Games have been thrown.
	Exports: character, faction, regionID, characterType, religion

	@function onGovernorThrowGames
	@tparam eventTrigger eventData

	@usage
	function onGovernorThrowGames(eventData)
	--something
	end
	*/

	/***
	Races have been thrown.
	Exports: character, faction, regionID, characterType, religion

	@function onGovernorThrowRaces
	@tparam eventTrigger eventData

	@usage
	function onGovernorThrowRaces(eventData)
	--something
	end
	*/

	/***
	The player has selected a character.
	Exports: character, targetSettlement, faction, regionID, characterType, religion

	@function onCharacterSelected
	@tparam eventTrigger eventData

	@usage
	function onCharacterSelected(eventData)
	--something
	end
	*/

	/***
	The player has selected an enemy character.
	Exports: character, faction, regionID, characterType, religion

	@function onEnemyCharacterSelected
	@tparam eventTrigger eventData

	@usage
	function onEnemyCharacterSelected(eventData)
	--something
	end
	*/

	/***
	The player has selected a position beyond the character's extents.
	Exports: character, faction, regionID, characterType, religion

	@function onMultiTurnMove
	@tparam eventTrigger eventData

	@usage
	function onMultiTurnMove(eventData)
	--something
	end
	*/

	/***
	The player has opened the panel for the selected character.
	Exports: character, faction, regionID, characterType, religion

	@function onCharacterPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onCharacterPanelOpen(eventData)
	--something
	end
	*/

	/***
	A mission has been completed.
	Exports: character, faction, regionID, characterType, missionDetails, religion

	@function onLeaderMissionSuccess
	@tparam eventTrigger eventData

	@usage
	function onLeaderMissionSuccess(eventData)
	--something
	end
	*/

	/***
	A mission has failed.
	Exports: character, faction, regionID, characterType, missionDetails, religion

	@function onLeaderMissionFailed
	@tparam eventTrigger eventData

	@usage
	function onLeaderMissionFailed(eventData)
	--something
	end
	*/

	/***
	A General has been sent on Crusade/Jihad.
	Exports: character, targetSettlement, faction, targetFaction, regionID, targetRegionID, characterType, religion, targetReligion, crusade

	@function onGeneralJoinCrusade
	@tparam eventTrigger eventData

	@usage
	function onGeneralJoinCrusade(eventData)
	--something
	end
	*/

	/***
	A General has left a Crusade/Jihad.
	Exports: character, targetSettlement, faction, targetFaction, regionID, targetRegionID, characterType, religion, targetReligion, crusade

	@function onGeneralAbandonCrusade
	@tparam eventTrigger eventData

	@usage
	function onGeneralAbandonCrusade(eventData)
	--something
	end
	*/

	/***
	A General has arrived in the Crusade/Jihad target region.
	Exports: character, targetSettlement, faction, targetFaction, army, regionID, targetRegionID, characterType, religion, targetReligion, crusade

	@function onGeneralArrivesCrusadeTargetRegion
	@tparam eventTrigger eventData

	@usage
	function onGeneralArrivesCrusadeTargetRegion(eventData)
	--something
	end
	*/

	/***
	A General has taken the Crusade/Jihad target settlement.
	Exports: character, targetSettlement, faction, targetFaction, regionID, targetRegionID, characterType, religion, targetReligion, crusade

	@function onGeneralTakesCrusadeTarget
	@tparam eventTrigger eventData

	@usage
	function onGeneralTakesCrusadeTarget(eventData)
	--something
	end
	*/

	/***
	A Character has finished its turn.
	Exports: character, settlement, faction, regionID, characterType, religion

	@function onCharacterTurnEnd
	@tparam eventTrigger eventData

	@usage
	function onCharacterTurnEnd(eventData)
	--something
	end
	*/

	/***
	A Character has finished its turn in a settlement.
	Exports: character, settlement, faction, regionID, characterType, religion

	@function onCharacterTurnEndInSettlement
	@tparam eventTrigger eventData

	@usage
	function onCharacterTurnEndInSettlement(eventData)
	--something
	end
	*/

	/***
	The character has been made the faction leader.
	Exports: character, faction, regionID, characterType, religion

	@function onBecomesFactionLeader
	@tparam eventTrigger eventData

	@usage
	function onBecomesFactionLeader(eventData)
		print("Function: onBecomesFactionLeader()\n\tLeader: ".. eventData.character.fullName)
	end
	*/

	/***
	The character is no longer faction leader.
	Exports: character, faction, regionID, characterType, religion

	@function onCeasedFactionLeader
	@tparam eventTrigger eventData

	@usage
	function onCeasedFactionLeader(eventData)
		--something
	end
	*/

	/***
	The character has been made a faction heir.
	Exports: character, faction, regionID, characterType, religion

	@function onBecomesFactionHeir
	@tparam eventTrigger eventData

	@usage
	function onBecomesFactionHeir(eventData)
		print("Function: onBecomesFactionHeir()\n\tHeir: ".. eventData.character.fullName)
	end
	*/

	/***
	The character is no longer faction heir.
	Exports: character, faction, regionID, characterType, religion

	@function onCeasedFactionHeir
	@tparam eventTrigger eventData

	@usage
	function onCeasedFactionHeir(eventData)
		print("Function: onCeasedFactionHeir()\n\tHeir: "..  eventData.character.fullName)
	end
	*/

	/***
	A character has been injured by a disaster.
	Exports: character, faction, regionID, disasterType, characterType, religion
	disasterTypes: earthquake, flood, horde, storm, volcano, dustbowl, locusts, famine, plague, riot, fire

	@function onCharacterDamagedByDisaster
	@tparam eventTrigger eventData

	@usage
	function onCharacterDamagedByDisaster(eventData)
	--something
	end
	*/

	/***
	A General has captured a settlement.
	Exports: character, settlement, targetSettlement, faction, targetFaction, regionID, characterType, religion

	@function onGeneralCaptureSettlement
	@tparam eventTrigger eventData

	@usage
	function onGeneralCaptureSettlement(eventData)
	--something
	end
	*/

	/***
	An assault has taken place. NOTE: settlement and fort are not in eventData! They are separate arguments!.
	Exports: character, faction, targetFaction, regionID, targetRegionID, characterType, religion, targetReligion

	@function onGeneralAssaultsResidence
	@tparam eventTrigger eventData
	@tparam settlementStruct|nil settlement
	@tparam fortStruct|nil fort

	@usage
	function onGeneralAssaultsResidence(eventData, settlement, fort)
	--something
	end
	*/

	/***
	An assault has taken place.
	Exports: character, targetCharacter, faction, targetFaction, regionID, characterType, targetCharacterType, religion, targetReligion

	@function onGeneralAssaultsGeneral
	@tparam eventTrigger eventData

	@usage
	function onGeneralAssaultsGeneral(eventData)
	--something
	end
	*/

	/***
	A general on crusade/jihad has been attacked by other character (it includes crusading generals attacked in a residence or on navy and generals attacked by spotted and killed assassin).
	Exports: character, targetCharacter, faction, targetFaction, regionID, characterType, targetCharacterType, religion, targetReligion

	@function onCharacterAttacksCrusadingGeneral
	@tparam eventTrigger eventData

	@usage
	function onCharacterAttacksCrusadingGeneral(eventData)
	--something
	end
	*/

	/***
	A General of a captor faction has made a ransom decision.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion, captureInfo, ransomType
	ransomType: ransom, execute, release, cannot_pay_ransom

	@function onGeneralPrisonersRansomedCaptor
	@tparam eventTrigger eventData

	@usage
	function onGeneralPrisonersRansomedCaptor(eventData)
	--something
	end
	*/

	/***
	A General of a captive faction has made a ransom decision.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion, captureInfo, ransomType

	@function onGeneralPrisonersRansomedCaptive
	@tparam eventTrigger eventData

	@usage
	function onGeneralPrisonersRansomedCaptive(eventData)
	--something
	end
	*/

	/***
	A captor faction has made a ransom decision.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion, captureInfo, ransomType

	@function onFactionLeaderPrisonersRansomedCaptor
	@tparam eventTrigger eventData

	@usage
	function onFactionLeaderPrisonersRansomedCaptor(eventData)
	--something
	end
	*/

	/***
	A captive faction has made a ransom decision.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion, captureInfo, ransomType

	@function onFactionLeaderPrisonersRansomedCaptive
	@tparam eventTrigger eventData

	@usage
	function onFactionLeaderPrisonersRansomedCaptive(eventData)
	--something
	end
	*/

	/***
	A spy mission has completed. May also export fort or settlement if target was a garrison residence.
	Exports: character, settlement, fort, faction, targetFaction, regionID, characterType, missionSuccessLevel, missionProbability, religion, targetReligion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onSpyMission
	@tparam eventTrigger eventData

	@usage
	function onSpyMission(eventData)
	--something
	end
	*/

	/***
	An assassination mission has completed.
	Exports: character, faction, regionID, characterType, missionSuccessLevel, missionProbability, religion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onAssassinationMission
	@tparam eventTrigger eventData

	@usage
	function onAssassinationMission(eventData)
	--something
	end
	*/

	/***
	An acquisition mission has completed.
	Exports: character, faction, regionID, characterType, missionSuccessLevel, missionProbability, religion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onAcquisitionMission
	@tparam eventTrigger eventData

	@usage
	function onAcquisitionMission(eventData)
	--something
	end
	*/

	/***
	A marriage mission has completed.
	Exports: character, faction, regionID, characterType, missionSuccessLevel, missionProbability, religion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onMarriageMission
	@tparam eventTrigger eventData

	@usage
	function onMarriageMission(eventData)
	--something
	end
	*/

	/***
	A denouncement mission has completed.
	Exports: character, faction, regionID, characterType, missionSuccessLevel, missionProbability, religion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onDenouncementMission
	@tparam eventTrigger eventData

	@usage
	function onDenouncementMission(eventData)
	--something
	end
	*/

	/***
	A sabotage mission has completed.
	Exports: character, faction, regionID, characterType, missionSuccessLevel, missionProbability, religion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onSabotageMission
	@tparam eventTrigger eventData

	@usage
	function onSabotageMission(eventData)
	--something
	end
	*/

	/***
	A bribery mission has completed.
	Exports: character, faction, targetFaction, regionID, characterType, missionSuccessLevel, religion, targetReligion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful
	
	@function onBriberyMission
	@tparam eventTrigger eventData

	@usage
	function onBriberyMission(eventData)
	--something
	end
	*/

	/***
	A diplomacy mission has completed.
	Exports: character, faction, targetFaction, regionID, characterType, missionSuccessLevel, religion, targetReligion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful
	
	@function onDiplomacyMission
	@tparam eventTrigger eventData

	@usage
	function onDiplomacyMission(eventData)
	--something
	end
	*/

	/***
	A Faction leader has ordered a spying mission.
	Exports: character, settlement, fort, faction, targetFaction, regionID, characterType, missionSuccessLevel, religion, targetReligion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful
	
	@function onLeaderOrderedSpyingMission
	@tparam eventTrigger eventData

	@usage
	function onLeaderOrderedSpyingMission(eventData)
	--something
	end
	*/

	/***
	A Faction leader has ordered an assassination mission.
	Exports: character, targetCharacter, faction, regionID, characterType, missionSuccessLevel, religion
	missionSuccessLevel: not_successful, slightly_successful, partly_successful, highly_successful

	@function onLeaderOrderedAssassination
	@tparam eventTrigger eventData

	@usage
	function onLeaderOrderedAssassination(eventData)
	--something
	end
	*/

	/***
	A Faction leader has ordered a bribery mission.
	Exports: character, faction, regionID, characterType, religion

	@function onLeaderOrderedBribery
	@tparam eventTrigger eventData

	@usage
	function onLeaderOrderedBribery(eventData)
	--something
	end
	*/

	/***
	A settlement is being processed for the start of its faction's turn.
	Exports: settlement, faction, regionID, religion

	@function onSettlementTurnStart
	@tparam eventTrigger eventData

	@usage
	function onSettlementTurnStart(eventData)
	--something
	end
	*/

	/***
	A settlement is no longer garrisoned.
	Exports: settlement, faction, regionID, religion

	@function onUngarrisonedSettlement
	@tparam eventTrigger eventData

	@usage
	function onUngarrisonedSettlement(eventData)
	--something
	end
	*/

	/***
	A settlement has been upgraded.
	Exports: settlement, faction, regionID, religion

	@function onSettlementUpgraded
	@tparam eventTrigger eventData

	@usage
	function onSettlementUpgraded(eventData)
	--something
	end
	*/

	/***
	A settlement has been converted.
	Exports: settlement, faction, regionID, religion

	@function onSettlementConverted
	@tparam eventTrigger eventData

	@usage
	function onSettlementConverted(eventData)
	--something
	end
	*/

	/***
	Siege equipment has been completed by one of the besieging armies.
	Exports: settlement, faction, regionID, religion

	@function onSiegeEquipmentCompleted
	@tparam eventTrigger eventData

	@usage
	function onSiegeEquipmentCompleted(eventData)
	--something
	end
	*/

	/***
	A Settlement is being processed for the end of its faction's turn.
	Exports: settlement, faction, regionID, religion

	@function onSettlementTurnEnd
	@tparam eventTrigger eventData

	@usage
	function onSettlementTurnEnd(eventData)
	--something
	end
	*/

	/***
	The player has selected a settlement.
	Exports: settlement, faction, regionID, religion

	@function onSettlementSelected
	@tparam eventTrigger eventData

	@usage
	function onSettlementSelected(eventData)
	--something
	end
	*/

	/***
	The player has opened the panel for the selected settlement.
	Exports: settlement, faction, regionID, religion

	@function onSettlementPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onSettlementPanelOpen(eventData)
	--something
	end
	*/

	/***
	The player has opened a recruitment panel.
	Exports: settlement, faction, regionID, religion

	@function onRecruitmentPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onRecruitmentPanelOpen(eventData)
	--something
	end
	*/

	/***
	The player has opened a construction panel.
	Exports: settlement, faction, regionID, religion

	@function onConstructionPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onConstructionPanelOpen(eventData)
	--something
	end
	*/

	/***
	The player has opened a trade panel.
	Exports: settlement, faction, regionID, religion

	@function onTradePanelOpen
	@tparam eventTrigger eventData

	@usage
	function onTradePanelOpen(eventData)
	--something
	end
	*/

	/***
	The player has requested advice on the settlement scroll.
	Exports: settlement, faction, regionID, resourceDescription, religion

	@function onSettlementScrollAdviceRequested
	@tparam eventTrigger eventData

	@usage
	function onSettlementScrollAdviceRequested(eventData)
	--something
	end
	*/

	/***
	A guild has been created/upgraded.
	Exports: settlement, faction, regionID, resourceDescription, guildId, religion

	@function onGuildUpgraded
	@tparam eventTrigger eventData

	@usage
	function onGuildUpgraded(eventData)
	--something
	end
	*/

	/***
	A guild has been destroyed.
	Exports: settlement, faction, regionID, guildId, religion

	@function onGuildDestroyed
	@tparam eventTrigger eventData

	@usage
	function onGuildDestroyed(eventData)
	--something
	end
	*/

	/***
	A settlement has been captured and occupied.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion

	@function onOccupySettlement
	@tparam eventTrigger eventData

	@usage
	function onOccupySettlement(eventData)
	--something
	end
	*/

	/***
	A settlement has been captured and sacked.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion

	@function onSackSettlement
	@tparam eventTrigger eventData

	@usage
	function onSackSettlement(eventData)
	--something
	end
	*/

	/***
	A settlement has been captured and some of its population has been decimated.
	Exports: character, faction, targetFaction, regionID, characterType, religion, targetReligion

	@function onExterminatePopulation
	@tparam eventTrigger eventData
	
	@usage
	function onExterminatePopulation(eventData)
	--something
	end
	*/

	/***
	A settlement has rioted.
	Exports: settlement, faction, targetFaction, regionID, religion, targetReligion

	@function onCityRiots
	@tparam eventTrigger eventData

	@usage
	function onCityRiots(eventData)
	--something
	end
	*/

	/***
	A settlement has been given to another faction.
	Exports: settlement, faction, targetFaction, regionID, religion, targetReligion

	@function onGiveSettlement
	@tparam eventTrigger eventData

	@usage
	function onGiveSettlement(eventData)
	--something
	end
	*/

	/***
	A settlement has rebelled.
	Exports: settlement, faction, targetFaction, regionID, religion, targetReligion

	@function onCityRebels
	@tparam eventTrigger eventData

	@usage
	function onCityRebels(eventData)
	--something
	end
	*/

	/***
	A settlement has been razed.
	Exports: settlement, faction, targetFaction, regionID, religion, targetReligion

	@function onCitySacked
	@tparam eventTrigger eventData

	@usage
	function onCitySacked(eventData)
	--something
	end
	*/

	/***
	A settlement has rioted.
	Exports: character, settlement, faction, targetFaction, regionID, characterType, religion, targetReligion

	@function onGovernorCityRiots
	@tparam eventTrigger eventData

	@usage
	function onGovernorCityRiots(eventData)
	--something
	end
	*/

	/***
	A settlement has rebelled.
	Exports: character, settlement, faction, targetFaction, regionID, characterType, religion, targetReligion

	@function onGovernorCityRebels
	@tparam eventTrigger eventData

	@usage
	function onGovernorCityRebels(eventData)
	--something
	end
	*/

	/***
	The player has abandoned a show me scipt.

	@function onAbandonShowMe
	@tparam eventTrigger eventData

	@usage
	function onAbandonShowMe(eventData)
	--something here
	end
	*/

	/***
	A strat map game has been reloaded.

	@function onGameReloaded
	@tparam eventTrigger eventData

	@usage
	function onGameReloaded(eventData)
	--something here
	end
	*/

	/***
	The plaza is being captured.

	@function onBattleWinningPlaza
	@tparam eventTrigger eventData

	@usage
	function onBattleWinningPlaza(eventData)
	--something here
	end
	*/

	/***
	The plaza capture has been stopped.

	@function onBattleStopsWinningPlaza
	@tparam eventTrigger eventData

	@usage
	function onBattleStopsWinningPlaza(eventData)
	--something here
	end
	*/

	/***
	The enemy will have captured the plaza in 30s.

	@function onBattleDominatingPlaza
	@tparam eventTrigger eventData

	@usage
	function onBattleDominatingPlaza(eventData)
	--something here
	end
	*/

	/***
	A siege engine is now unmanned.

	@function onBattngineUnmanned
	@tparam eventTrigger eventData

	@usage
	function onBattngineUnmanned(eventData)
	--something here
	end
	*/

	/***
	Half of the player's army has been destroyed.

	@function onBattlePlayerArmyHalfDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBattlePlayerArmyHalfDestroyed(eventData)
	--something here
	end
	*/

	/***
	Half of the enemy's army has been destroyed.

	@function onBattnemyArmyHalfDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBattnemyArmyHalfDestroyed(eventData)
	--something here
	end
	*/

	/***
	The battle has finished.

	@function onBattleFinished
	@tparam eventTrigger eventData

	@usage
	function onBattleFinished(eventData)
	--something here
	end
	*/

	/***
	Half of an army has been destroyed.

	@function onBattleArmyHalfDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBattleArmyHalfDestroyed(eventData)
	--something here
	end
	*/

	/***
	The escape key has been pressed. This trigger will only fire if the command StealEscKey has been used.

	@function onEscPressed
	@tparam eventTrigger eventData

	@usage
	function onEscPressed(eventData)
	--something here
	end
	*/

	/***
	The player has been issued with advice by a script.

	@function onScriptedAdvice
	@tparam eventTrigger eventData

	@usage
	function onScriptedAdvice(eventData)
	--something here
	end
	*/

	/***
	The player has requested advice on the naval prebattle scroll.

	@function onNavalPreBattleScrollAdviceRequested
	@tparam eventTrigger eventData

	@usage
	function onNavalPreBattleScrollAdviceRequested(eventData)
	--something here
	end
	*/

	/***
	The player has requested advice on the prebattle scroll.

	@function onPreBattleScrollAdviceRequested
	@tparam eventTrigger eventData

	@usage
	function onPreBattleScrollAdviceRequested(eventData)
	--something here
	end
	*/

	/***
	The player has opened the college of cardinals panel.

	@function onCollegeOfCardinalsPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onCollegeOfCardinalsPanelOpen(eventData)
	--something here
	end
	*/

	/***
	The player has opened the diplomatic standing panel.

	@function onDiplomaticStandingPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onDiplomaticStandingPanelOpen(eventData)
	--something here
	end
	*/

	/***
	An idle unit is under missile fire.

	@function onBattlePlayerUnderAttackIdle
	@tparam eventTrigger eventData

	@usage
	function onBattlePlayerUnderAttackIdle(eventData)
	--something here
	end
	*/

	/***
	A team has gained the advantage in combat.

	@function onBattleWinningCombat
	@tparam eventTrigger eventData

	@usage
	function onBattleWinningCombat(eventData)
	--something here
	end
	*/

	/***
	The whole army is tired.

	@function onBattleArmyTired
	@tparam eventTrigger eventData

	@usage
	function onBattleArmyTired(eventData)
	--something here
	end
	*/

	/***
	A spy has successfully opened the gates.

	@function onBattleSpySuccess
	@tparam eventTrigger eventData

	@usage
	function onBattleSpySuccess(eventData)
	--something here
	end
	*/

	/***
	A different team is now the strongest.

	@function onBattleTideofBattle
	@tparam eventTrigger eventData

	@usage
	function onBattleTideofBattle(eventData)
	--something here
	end
	*/

	/***
	A unit has gone berserk.
	Exports: unit

	@function onBattleUnitGoesBerserk
	@tparam eventTrigger eventData

	@usage
	function onBattleUnitGoesBerserk(eventData)
	--something here
	end
	*/

	/***
	A siege engine has been destroyed.
	Exports: unit

	@function onBattleSiegeEngineDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBattleSiegeEngineDestroyed(eventData)
	--something here
	end
	*/

	/***
	A siege engine has docked with a wall.
	Exports: unit

	@function onBattleSiegeEngineDocksWall
	@tparam eventTrigger eventData

	@usage
	function onBattleSiegeEngineDocksWall(eventData)
	--something here
	end
	*/

	/***
	An engine has started attacking a gate.
	Exports: unit

	@function onBattleGatesAttackedByEngine
	@tparam eventTrigger eventData

	@usage
	function onBattleGatesAttackedByEngine(eventData)
	--something here
	end
	*/

	/***
	An engine has destroyed a gate.
	Exports: unit

	@function onBattleGatesDestroyedByEngine
	@tparam eventTrigger eventData

	@usage
	function onBattleGatesDestroyedByEngine(eventData)
	--something here
	end
	*/

	/***
	A siege engine has knocked down a wall.
	Exports: unit

	@function onBattleWallsBreachedByEngine
	@tparam eventTrigger eventData

	@usage
	function onBattleWallsBreachedByEngine(eventData)
	--something here
	end
	*/

	/***
	A wall has been captured.
	Exports: unit

	@function onBattleWallsCaptured
	@tparam eventTrigger eventData

	@usage
	function onBattleWallsCaptured(eventData)
	--something here
	end
	*/

	/***
	A unit has routed.
	Exports: unit

	@function onBattleUnitRouts
	@tparam eventTrigger eventData

	@usage
	function onBattleUnitRouts(eventData)
	--something here
	end
	*/

	/***
	A unit has been disbanded.
	Exports: faction, playerUnit, eduEntry, religion

	@function onUnitDisbanded
	@tparam eventTrigger eventData

	@usage
	function onUnitDisbanded(eventData)
	--something here
	end
	*/

	/***
	A unit has been trained.
	Exports: settlement, faction, playerUnit, eduEntry, religion

	@function onUnitTrained
	@tparam eventTrigger eventData

	@usage
	function onUnitTrained(eventData)
	--something here
	end
	*/

	/***
	A unit has been trained.
	Exports: character, settlement, faction, regionID, playerUnit, eduEntry, characterType, religion

	@function onGovernorUnitTrained
	@tparam eventTrigger eventData

	@usage
	function onGovernorUnitTrained(eventData)
	--something here
	end
	*/

	/***
	A building has been completed.
	Exports: character, settlement, faction, regionID, priorBuild, characterType, religion

	@function onGovernorBuildingCompleted
	@tparam eventTrigger eventData

	@usage
	function onGovernorBuildingCompleted(eventData)
	--something here
	end
	*/

	/***
	An agent has been trained.
	Exports: character, settlement, faction, regionID, characterType, religion

	@function onAgentCreated
	@tparam eventTrigger eventData

	@usage
	function onAgentCreated(eventData)
	--something here
	end
	*/

	/***
	An agent has been trained.
	Exports: character, settlement, faction, regionID, characterType, religion

	@function onGovernorAgentCreated
	@tparam eventTrigger eventData

	@usage
	function onGovernorAgentCreated(eventData)
	--something here
	end
	*/

	/***
	A building has been destroyed.
	Exports: settlement, faction, regionID, resourceDescription, religion

	@function onBuildingDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBuildingDestroyed(eventData)
	--something here
	end
	*/

	/***
	A building has been added to the construction queue.
	Exports: settlement, faction, regionID, resourceDescription, religion

	@function onAddedToBuildingQueue
	@tparam eventTrigger eventData

	@usage
	function onAddedToBuildingQueue(eventData)
	--something here
	end
	*/

	/***
	A building has been completed.
	Exports: settlement, faction, priorBuild, religion

	@function onBuildingCompleted
	@tparam eventTrigger eventData

	@usage
	function onBuildingCompleted(eventData)
	--something here
	end
	*/

	/***
	The player has requested building advice.
	Exports: settlement, faction, regionID, religion

	@function onRequestBuildingAdvice
	@tparam eventTrigger eventData

	@usage
	function onRequestBuildingAdvice(eventData)
	--something here
	end
	*/

	/***
	The player has requested training advice.
	Exports: settlement, faction, regionID, religion

	@function onRequestTrainingAdvice
	@tparam eventTrigger eventData

	@usage
	function onRequestTrainingAdvice(eventData)
	--something here
	end
	*/

	/***
	A unit has been added to the training queue.
	Exports: settlement, faction, regionID, resourceDescription, religion

	@function onAddedToTrainingQueue
	@tparam eventTrigger eventData

	@usage
	function onAddedToTrainingQueue(eventData)
	--something here
	end
	*/

	/***
	An army has been entirely routed.
	Exports: army

	@function onBattleArmyRouted
	@tparam eventTrigger eventData

	@usage
	function onBattleArmyRouted(eventData)
	--something here
	end
	*/

	/***
	A reinforcing army has arrived on the battlefield.
	Exports: army

	@function onBattleReinforcementsArrive
	@tparam eventTrigger eventData

	@usage
	function onBattleReinforcementsArrive(eventData)
	--something here
	end
	*/

	/***
	The player has requested mercenaries advice.
	Exports: army

	@function onRequestMercenariesAdvice
	@tparam eventTrigger eventData

	@usage
	function onRequestMercenariesAdvice(eventData)
	--something here
	end
	*/

	/***
	The player has clicked on a button.
	Exports: resourceDescription

	@function onButtonPressed
	@tparam eventTrigger eventData

	@usage
	function onButtonPressed(eventData)
	--something here
	end
	*/

	/***
	The player triggered a keyboard shortcut.
	Exports: resourceDescription

	@function onShortcutTriggered
	@tparam eventTrigger eventData

	@usage
	function onShortcutTriggered(eventData)
	--something here
	end
	*/

	/***
	A special UI Element is visible.
	Exports: resourceDescription

	@function onUIElementVisible
	@tparam eventTrigger eventData

	@usage
	function onUIElementVisible(eventData)
	--something here
	end
	*/

	/***
	The player opened a scroll.
	Exports: resourceDescription

	@function onScrollOpened
	@tparam string eventData

	@usage
	function onScrollOpened(eventData)
	--something here
	end
	*/

	/***
	The player closed a scroll.
	Exports: resourceDescription

	@function onScrollClosed
	@tparam eventTrigger eventData

	@usage
	function onScrollClosed(eventData)
	--something here
	end
	*/

	/***
	The player has requested advice on a scroll.
	Exports: resourceDescription

	@function onScrollAdviceRequested
	@tparam eventTrigger eventData

	@usage
	function onScrollAdviceRequested(eventData)
	--something here
	end
	*/

	/***
	The player has suppressed a piece of advice.
	Exports: resourceDescription

	@function onAdviceSupressed
	@tparam eventTrigger eventData

	@usage
	function onAdviceSupressed(eventData)
	--something here
	end
	*/

	/***
	A general has been routed.
	Exports: stratCharacter, character

	@function onBattleGeneralRouted
	@tparam eventTrigger eventData

	@usage
	function onBattleGeneralRouted(eventData)
	--something here
	end
	*/

	/***
	A general has been killed.
	Exports: stratCharacter

	@function onBattleGeneralKilled
	@tparam eventTrigger eventData

	@usage
	function onBattleGeneralKilled(eventData)
	--something here
	end
	*/

	/***
	A Crusade/Jihad has been called.
	Exports: targetSettlement, targetFaction, targetRegionID, targetReligion, crusade

	@function onCrusadeCalled
	@tparam eventTrigger eventData

	@usage
	function onCrusadeCalled(eventData)
	--something here
	end
	*/

	/***
	The Pope has accepted the player's crusade target.
	Exports: targetSettlement, targetFaction, targetRegionID, targetReligion, crusade

	@function onPopeAcceptsCrusadeTarget
	@tparam eventTrigger eventData

	@usage
	function onPopeAcceptsCrusadeTarget(eventData)
	--something here
	end
	*/

	/***
	The Pope has rejected the player's crusade target.
	Exports: targetSettlement, targetFaction, targetRegionID, targetReligion, crusade

	@function onPopeRejectsCrusadeTarget
	@tparam eventTrigger eventData

	@usage
	function onPopeRejectsCrusadeTarget(eventData)
	--something here
	end
	*/

	/***
	A Crusade/Jihad has ended.
	Exports: targetSettlement, targetRegionID, crusade

	@function onCrusadeEnds
	@tparam eventTrigger eventData

	@usage
	function onCrusadeEnds(eventData)
	--something here
	end
	*/

	/***
	Called before the faction's turn starts.
	Exports: faction, religion

	@function onPreFactionTurnStart
	@tparam eventTrigger eventData

	@usage
	function onPreFactionTurnStart(eventData)
	--something here
	end
	*/

	/***
	Called at a faction's turn start.
	Exports: faction, religion

	@function onFactionTurnStart
	@tparam eventTrigger eventData

	@usage
	function onFactionTurnStart(eventData)
	--something here
	end
	*/

	/***
	Called after faction changes to a new capital.
	Exports: faction, religion

	@function onFactionNewCapital
	@tparam eventTrigger eventData

	@usage
	function onFactionNewCapital(eventData)
	newCapital=fac.capital();
	end
	*/

	/***
	Called at a faction's turn end.
	Exports: faction, religion

	@function onFactionTurnEnd
	@tparam eventTrigger eventData

	@usage
	function onFactionTurnEnd(eventData)
	--something here
	end
	*/

	/***
	A Faction has voted for the new pope.
	Exports: faction, religion

	@function onVotedForPope
	@tparam eventTrigger eventData

	@usage
	function onVotedForPope(eventData)
	--something here
	end
	*/

	/***
	A Faction has been excommunicated.
	Exports: faction, religion

	@function onFactionExcommunicated
	@tparam eventTrigger eventData

	@usage
	function onFactionExcommunicated(eventData)
	--something here
	end
	*/

	/***
	A Faction has formed a horde.
	Exports: faction, religion

	@function onHordeFormed
	@tparam eventTrigger eventData

	@usage
	function onHordeFormed(eventData)
	--something here
	end
	*/

	/***
	A cardinal has been removed from the game.
	Exports: faction, religion

	@function onCardinalRemoved
	@tparam eventTrigger eventData

	@usage
	function onCardinalRemoved(eventData)
	--something here
	end
	*/

	/***
	An inquisitor has been dispatched to a region.
	Exports: faction, religion

	@function onInquisitorAppointed
	@tparam eventTrigger eventData

	@usage
	function onInquisitorAppointed(eventData)
	--something here
	end
	*/

	/***
	An assassination mission against the pope has failed and the assassin is executed.
	Exports: faction, religion

	@function onAssassinCaughtAttackingPope
	@tparam eventTrigger eventData

	@usage
	function onAssassinCaughtAttackingPope(eventData)
	--something here
	end
	*/

	/***
	The player has opened his finances panel.
	Exports: faction, religion

	@function onFinancesPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onFinancesPanelOpen(eventData)
	--something here
	end
	*/

	/***
	The player has opened the faction summary panel.
	Exports: faction, religion

	@function onFactionSummaryPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onFactionSummaryPanelOpen(eventData)
	--something here
	end
	*/

	/***
	The player has opened the family tree panel.
	Exports: faction, religion

	@function onFamilyTreePanelOpen
	@tparam eventTrigger eventData

	@usage
	function onFamilyTreePanelOpen(eventData)
	--something here
	end
	*/

	/***
	The player has opened the diplomatic standing panel.
	Exports: faction, religion

	@function onDiplomacyPanelOpen
	@tparam eventTrigger eventData

	@usage
	function onDiplomacyPanelOpen(eventData)
	--something here
	end
	*/

	/***
	The player has attacked an army or settlement. The decision panel is now open.
	Exports: faction, religion

	@function onPreBattlePanelOpen
	@tparam eventTrigger eventData

	@usage
	function onPreBattlePanelOpen(eventData)
	--something here
	end
	*/

	/***
	A message has arrived for the player.
	Exports: faction, eventID, religion

	@function onIncomingMessage
	@tparam eventTrigger eventData

	@usage
	function onIncomingMessage(eventData)
	--something here
	end
	*/

	/***
	The player has opened a message.
	Exports: faction, eventID, religion

	@function onMessageOpen
	@tparam eventTrigger eventData

	@usage
	function onMessageOpen(eventData)
	--something here
	end
	*/

	/***
	The player has closed a message.
	Exports: eventID

	@function onMessageClosed
	@tparam eventTrigger eventData

	@usage
	function onMessageClosed(eventData)
	--something here
	end
	*/

	/***
	The player has declined automated settlement management.
	Exports: faction, religion

	@function onDeclineAutomatedSettlementManagement
	@tparam eventTrigger eventData

	@usage
	function onDeclineAutomatedSettlementManagement(eventData)
	--something here
	end
	*/

	/***
	The battle AI has begun processing.
	Exports: faction, religion

	@function onBattleAiCommenced
	@tparam eventTrigger eventData

	@usage
	function onBattleAiCommenced(eventData)
	--something here
	end
	*/

	/***
	The delay phase has begun.
	Exports: faction, religion

	@function onBattleDelayPhaseCommenced
	@tparam eventTrigger eventData

	@usage
	function onBattleDelayPhaseCommenced(eventData)
	--something here
	end
	*/

	/***
	The deployment phase has begun.
	Exports: faction, religion

	@function onBattleDeploymentPhaseCommenced
	@tparam eventTrigger eventData

	@usage
	function onBattleDeploymentPhaseCommenced(eventData)
	--something here
	end
	*/

	/***
	The conflict phase has begun.
	Exports: faction, religion

	@function onBattleConflictPhaseCommenced
	@tparam eventTrigger eventData

	@usage
	function onBattleConflictPhaseCommenced(eventData)
	--something here
	end
	*/

	/***
	Called after a faction declares a war.
	Exports: faction, targetFaction, religion, targetReligion

	@function onFactionWarDeclared
	@tparam eventTrigger eventData

	@usage
	function onFactionWarDeclared(eventData)
	--something
	end
	*/

	/***
	Called after a faction declares an alliance.
	Exports: faction, targetFaction, religion, targetReligion

	@function onFactionAllianceDeclared
	@tparam eventTrigger eventData

	@usage
	function onFactionAllianceDeclared(eventData)
	--something
	end
	*/

	/***
	A marriage has occured between two factions. gets fired for both factions involved.
	Exports: faction, targetFaction, religion, targetReligion

	@function onInterFactionMarriage
	@tparam eventTrigger eventData

	@usage
	function onInterFactionMarriage(eventData)
	--something
	end
	*/

	/***
	Called after a faction makes a trade agreement.
	Exports: faction, targetFaction, religion, targetReligion

	@function onFactionTradeAgreementMade
	@tparam eventTrigger eventData

	@usage
	function onFactionTradeAgreementMade(eventData)
	--something
	end
	*/

	/***
	A new pope has been elected.
	Exports: faction, targetFaction, religion, targetReligion

	@function onPopeElected
	@tparam eventTrigger eventData

	@usage
	function onPopeElected(eventData)
	--something
	end
	*/

	/***
	Called after a faction breaks an alliance.
	Exports: faction, targetFaction, religion, targetReligion

	@function onFactionBreakAlliance
	@tparam eventTrigger eventData

	@usage
	function onFactionBreakAlliance(eventData)
	--something
	end
	*/

	/***
	A faction to faction attitude update has occurred (once every round).
	Exports: faction, targetFaction, religion

	@function onUpdateAttitude
	@tparam eventTrigger eventData

	@usage
	function onUpdateAttitude(eventData)
	--something
	end
	*/

	/***
	A demeanour response has occured in diplomacy talks.
	Exports: faction, targetFaction, religion, targetReligion, amount

	@function onDemeanour
	@tparam eventTrigger eventData

	@usage
	function onDemeanour(eventData)
	--something
	end
	*/

	/***
	Called after a faction gives money to another faction.
	Exports: faction, targetFaction, religion, targetReligion, amount

	@function onGiveMoney
	@tparam eventTrigger eventData

	@usage
	function onGiveMoney(eventData)
	--something
	end
	*/

	/***
	A player unit has attacked one of the enemies units.
	Exports: attackingUnit, defendingUnit, playerUnit, enemyUnit

	@function onBattlePlayerUnitAttacksEnemyUnit
	@tparam eventTrigger eventData

	@usage
	function onBattlePlayerUnitAttacksEnemyUnit(eventData)
	--something here
	end
	*/

	/***
	An enemy unit has attacked one of the players units.
	Exports: attackingUnit, defendingUnit, playerUnit, enemyUnit

	@function onBattleEnemyUnitAttacksPlayerUnit
	@tparam eventTrigger eventData

	@usage
	function onBattleEnemyUnitAttacksPlayerUnit(eventData)
	--something here
	end
	*/

	/***
	One of the player's units has gone berserk.
	Exports: playerUnit

	@function onBattlePlayerUnitGoesBerserk
	@tparam eventTrigger eventData

	@usage
	function onBattlePlayerUnitGoesBerserk(eventData)
	--something here
	end
	*/

	/***
	One of the player's units has routed.
	Exports: playerUnit

	@function onBattlePlayerUnitRouts
	@tparam eventTrigger eventData

	@usage
	function onBattlePlayerUnitRouts(eventData)
	--something here
	end
	*/

	/***
	A siege engine belonging to the player has been destroyed.
	Exports: playerUnit

	@function onBattlePlayerSiegeEngineDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBattlePlayerSiegeEngineDestroyed(eventData)
	--something here
	end
	*/

	/***
	A player's engine has started attacking a gate.
	Exports: playerUnit

	@function onBattleGatesAttackedByPlayerEngine
	@tparam eventTrigger eventData

	@usage
	function onBattleGatesAttackedByPlayerEngine(eventData)
	--something here
	end
	*/

	/***
	One of the enemy's units has gone berserk.
	Exports: enemyUnit

	@function onBattleEnemyUnitGoesBerserk
	@tparam eventTrigger eventData

	@usage
	function onBattleEnemyUnitGoesBerserk(eventData)
	--something here
	end
	*/

	/***
	One of the enemy's units has routed.
	Exports: enemyUnit

	@function onBattnemyUnitRouts
	@tparam eventTrigger eventData

	@usage
	function onBattnemyUnitRouts(eventData)
	--something here
	end
	*/

	/***
	A siege engine belonging to the enemy has been destroyed.
	Exports: enemyUnit

	@function onBattnemySiegeEngineDestroyed
	@tparam eventTrigger eventData

	@usage
	function onBattnemySiegeEngineDestroyed(eventData)
	--something here
	end
	*/

	/***
	An enemy's engine has started attacking a gate.
	Exports: enemyUnit

	@function onBattleGatesAttackedByEnemyEngine
	@tparam eventTrigger eventData

	@usage
	function onBattleGatesAttackedByEnemyEngine(eventData)
	--something here
	end
	*/

	/***
	When a particular disaster has just happened.
	Exports: resourceDescription

	@function onDisaster
	@tparam eventTrigger eventData

	@usage
	function onDisaster(eventData)
		if eventData.resourceDescription == "earthquake" then
			--something here
		end
	end
	*/

	/***
	An event counter has changed it's value.
	Exports: eventCounter

	@function onEventCounter
	@tparam eventTrigger eventData

	@usage
	function onEventCounter(eventData)
		if eventData.eventCounter == "my_event_counter" then
			--something here
		end
	end
	*/

	/***
	The last unit has been removed from a fort, agents do not count.
	Exports: fort, faction, regionID, religion

	@function onUngarrisonedFort
	@tparam eventTrigger eventData

	@usage
	function onUngarrisonedFort(eventData)
		--something here
	end
	*/

	/***
	An object of the target faction has been seen by the faction.
	Exports: faction, targetFaction, regionID, coords, religion

	@function onObjSeen
	@tparam eventTrigger eventData

	@usage
	function onObjSeen(eventData)
		--something here
	end
	*/

	/***
	The tile has been seen by the faction.
	Exports: faction, regionID, coords, religion

	@function onTileSeen
	@tparam eventTrigger eventData

	@usage
	function onTileSeen(eventData)
		--something here
	end
	*/

	/***
	A faction to faction transgression has occurred.
	Exports: faction, targetFaction, resourceDescription, religion, targetReligion
	transgressions: TC_THREATEN_WAR, TC_DECLARED_WAR, TC_MINOR_ASSASSINATION_ATTEMPT, TC_BROKE_TREATY_TERMS, TC_BROKE_ALLIANCE, TC_INVASION

	@function onTransgression
	@tparam eventTrigger eventData

	@usage
	function onTransgression(eventData)
		--something here
	end
	*/

	/***
	A faction to faction forgiveness has occurred.
	Exports: faction, targetFaction, resourceDescription, religion
	forgiveness: FC_MILITARY_ASSISTANCE, FC_OBVIOUS_BRIBE

	@function onForgiveness
	@tparam eventTrigger eventData

	@usage
	function onForgiveness(eventData)
		--something here
	end
	*/

	/***
	An army has taken a crusade or jihad target settlement.
	Exports: targetSettlement, faction, targetFaction, army, regionID, targetRegionID, coords, religion, targetReligion, crusade

	@function onArmyTakesCrusadeTarget
	@tparam eventTrigger eventData

	@usage
	function onArmyTakesCrusadeTarget(eventData)
		--something here
	end
	*/

	/***
	Units have deserted a crusade or jihad.
	Exports: targetSettlement, faction, targetFaction, targetRegionID, religion, targetReligion, crusade

	@function onUnitsDesertCrusade
	@tparam eventTrigger eventData

	@usage
	function onUnitsDesertCrusade(eventData)
		--something here
	end
	*/




	/***
	Called every time an image is rendered for display

	@function draw
	@tparam LPDIRECT3DDEVICE9 pDevice

	@usage
	function draw(device)
		ImGui.Begin("test");

		ImGui.Text("Some test text")

		ImGui.End();
	end
	*/
	drawLuaFunc = new sol::function(luaState["draw"]);
	checkLuaFunc(&drawLuaFunc);

	/***
	Called when ImGui backend reload fonts

	@function onLoadingFonts

	@usage
	function onLoadingFonts()
	--something here
	end
	*/
	onLoadingFonts = new sol::function(luaState["onLoadingFonts"]);
	checkLuaFunc(&onLoadingFonts);

	resetDXFunc = new sol::function(luaState["deinitDX"]);
	checkLuaFunc(&resetDXFunc);

	initDXFunc = new sol::function(luaState["initDX"]);
	checkLuaFunc(&initDXFunc);

	/***
	Called when a new campaign is started from the menu (when you click the start campaign button, nothing loaded yet).

	@function onNewGameStart

	@usage
	function onNewGameStart()
	--something here
	end
	*/
	onNewGameStart = new sol::function(luaState["onNewGameStart"]);
	checkLuaFunc(&onNewGameStart);
	
	onReadGameDbsAtStart = new sol::function(luaState["onReadGameDbsAtStart"]);
	checkLuaFunc(&onReadGameDbsAtStart);

	/***
	Called after the game reads the EDU at startup (add units here). Previously called onReadGameDbsAtStart.

	@function onEduParsed

	@usage
	function onEduParsed()
	--something here
	end
	*/
	onEduParsed = new sol::function(luaState["onEduParsed"]);
	checkLuaFunc(&onEduParsed);

	/***
	Called after the game has loaded to the main menu.

	@function onGameInit

	@usage
	function onGameInit()
	--something here
	end
	*/
	onGameInit = new sol::function(luaState["onGameInit"]);
	checkLuaFunc(&onGameInit);

	/***
	Called after a new campaign's data has been loaded first time. (M2TW.stratMap and M2TW.campaign)
	@function onNewGameLoaded
	@usage
	function onNewGameLoaded()
	--something here
	end
	*/
	onNewGameLoaded = new sol::function(luaState["onNewGameLoaded"]);
	checkLuaFunc(&onNewGameLoaded);

	/***
	Called after the campaignStruct gets unloaded (exit to menu, load save etc).

	@function onUnloadCampaign

	@usage
	function onUnloadCampaign()
	--something here
	end
	*/
	onUnloadCampaign = new sol::function(luaState["onUnloadCampaign"]);
	checkLuaFunc(&onUnloadCampaign);

	/***
	Called on ai initialized on turn start.

	@function onAiTurn
	@tparam aiFaction aiFaction

	@usage
	function onAiTurn(aiFaction)
	--something here
	end
	*/

	onAiTurn = new sol::function(luaState["onAiTurn"]);
	checkLuaFunc(&onAiTurn);

	/***
	Called just after LTGD reads xml every turn.

	@function onCalculateLTGD
	@tparam aiLongTermGoalDirector ltgd

	@usage
	function onCalculateLTGD(ltgd)
	--something here
	end
	*/

	onCalculateLTGD = new sol::function(luaState["onCalculateLTGD"]);
	checkLuaFunc(&onCalculateLTGD);

	/***
	Called just before the personality production values are propogated to the production controllers.

	@function onSetProductionControllers
	@tparam aiPersonality personality

	@usage
	function onSetProductionControllers(personality)
	--something here
	end
	*/

	onSetProductionControllers = new sol::function(luaState["onSetProductionControllers"]);
	checkLuaFunc(&onSetProductionControllers);

	/***
	Called on clicking the stratmap.

	@function onClickAtTile
	@tparam int x
	@tparam int y
	@usage
	clickAtMap={["isClicked"]=false,["xTile"]=0,["yTile"]=0}
	function draw(device)
		if(clickAtMap.isClicked==true)
		then
			print(tostring(ImGui.IsMouseDown(ImGuiMouseButton.ImGuiMouseButton_Right)).."\n");
			print(tostring(ImGui.IsMouseDown(ImGuiMouseButton.ImGuiMouseButton_Left)).."\n");
			print(tostring(clickAtMap.xTile).." "..tostring(clickAtMap.yTile));

			clickAtMap.isClicked=false;
		end
	end

	function onClickAtTile(x,y)
		clickAtMap.isClicked=true;
		clickAtMap.xTile=x;
		clickAtMap.yTile=y;
	end
	*/



	onClickAtTile = new sol::function(luaState["onClickAtTile"]);
	checkLuaFunc(&onClickAtTile);

	/***
	Called when clicking on a character.

	@function onCharacterClicked
	@tparam character clickedCharacter
	@usage
	
	function onCharacterClicked(clickedCharacter)
	--something here
	end
	*/
	onCharacterClicked = new sol::function(luaState["onCharacterClicked"]);
	checkLuaFunc(&onCharacterClicked);

	/***
	Called on campaign tick.

	@function onCampaignTick
	@usage
	
	function onCampaignTick()
	--something here
	end
	*/
	onCampaignTick = new sol::function(luaState["onCampaignTick"]);
	checkLuaFunc(&onCampaignTick);

	/***
	Called on battle tick.

	@function onBattleTick
	@usage
	
	function onBattleTick()
	--something here
	end
	*/
	onBattleTick = new sol::function(luaState["onBattleTick"]);
	checkLuaFunc(&onBattleTick);

	/***
	Called after loading the campaign map, including cas models and textures. Best used for model related stuff. This fires every time including post-battle and save loading.

	@function onCampaignMapLoaded

	@usage
	function onCampaignMapLoaded()
	--something here
	end
	*/
	
	onCampaignMapLoaded = new sol::function(luaState["onCampaignMapLoaded"]);
	checkLuaFunc(&onCampaignMapLoaded);

	/***
	Called when a unit is removed from the unit queue.

	@function onRemoveFromUnitQueue
	@tparam unitInQueue item

	@usage
	function onRemoveFromUnitQueue(item)
	--something here
	end
	*/
	
	onRemoveFromUnitQueue = new sol::function(luaState["onRemoveFromUnitQueue"]);
	checkLuaFunc(&onRemoveFromUnitQueue);

	/***
	Called on plugin load (at game start).

	@function onPluginLoad

	@usage
	--An example of using this event to perform actions at the beginning of a campaign once:

	isCampaignLoadedFirstTime=false;
	function onNewGameStart()
	  isCampaignLoadedFirstTime=true;
	end

	function onCampaignMapLoaded()
	  if(isCampaignLoadedFirstTime==false)
		then
		  do return end
		end


	  --here we do our stuff, what works only one time for each campaign
	  isCampaignLoadedFirstTime=false;
	end
	*/



	onPluginLoad = new sol::function(luaState["onPluginLoad"]);
	checkLuaFunc(&onPluginLoad);


	/***
	Called on creating a new save file.

	@function onCreateSaveFile

	@usage
		function onCreateSaveFile()
			local savefiles, currentPath = {}, M2TWEOP.getPluginPath()
			savefiles[1] = currentPath.."\\testSaveFile1.ourSave"
			savefiles[2] = currentPath.."\\testSaveFile2.ourSave"

			file1 = io.open(savefiles[1], "w"); file1:write("This is save file 1!!!"); file1:close()
			file2 = io.open(savefiles[2], "w"); file2:write("This is save file 2!!!"); file2:close()

			return savefiles
		end
	*/
	onCreateSaveFile = new sol::function(luaState["onCreateSaveFile"]);
	checkLuaFunc(&onCreateSaveFile);

	/***
	Called on loading a save file.

	@function onLoadSaveFile

	@usage
	function onLoadSaveFile(paths)
		lastSaveFiles={};
		for key, value in pairs(paths) do

		print(key, " -- ", value);

		lastSaveFiles[key]={};
		lastSaveFiles[key][1]=value;
		lastSaveFiles[key][2]=readFile(value);

		end
	end


	--read file and return it`s content
	local function readFile(path)
		local file = io.open(path, "r") ;
		if (not file) then return nil end
		local content = file:read "*a";
		file:close();
		return content;
	end
	*/
	onLoadSaveFile = new sol::function(luaState["onLoadSaveFile"]);
	checkLuaFunc(&onLoadSaveFile);


	/***
	Called at the start of a new turn.

	@function onChangeTurnNum
	@tparam int turnNumber

	@usage
	function onChangeTurnNum(turnNum)
	--something here
	end
	*/
	onChangeTurnNumFunc = new sol::function(luaState["onChangeTurnNum"]);
	checkLuaFunc(&onChangeTurnNumFunc);

	/***
	Called on select worldpkgdesc for battlemap. See https://wiki.twcenter.net/index.php?title=.worldpkgdesc_-_M2TW. M2TWEOP will ignore the return value if its group does not match the required group!

	@function onSelectWorldpkgdesc
	@tparam string selectedRecordName
	@tparam string selectedRecordGroup
	@treturn string newSelectedRecordName

	@usage
	function onSelectWorldpkgdesc(selectedRecord,selectedRecordGroup)
	--something here

		return selectedRecord;
	end
	*/
	onSelectWorldpkgdesc = new sol::function(luaState["onSelectWorldpkgdesc"]);
	checkLuaFunc(&onSelectWorldpkgdesc);
	/***
	Called on specified fortificationlevel in a siege of a settlement.

	@function onFortificationLevelS
	@tparam settlementStruct siegedSettlement
	@treturn int overridedFortificationlevel
	@treturn bool isCastle override settlement type (siege equipment is slightly different between cities and castles of the same level)

	@usage
	function onFortificationLevelS(settlement)
		if settlement.xCoord == 10 and settlement.yCoord == 25 then
			return 3, false --override settlement under siege at these coordinates to level 3 city
		end
		return nil --else, do not override
	end
	*/
	onFortificationLevelS = new sol::function(luaState["onFortificationLevelS"]);
	checkLuaFunc(&onFortificationLevelS);
	
	/***
	Called when the game calculates the value of a unit. For example, in battle, when it says 'Victory seems certain' when units are engaging each other it uses this, it uses this to decide which units to recruit, to evaluate army strength for attack decisions, for auto resolve balance and results, it is just the value that decides how strong it thinks a unit is. The long term goal director also uses this for the values you have in campaign_ai_db like military balance and free strength balance.
  
	@function onCalculateUnitValue
	@tparam eduEntry entry
	@tparam float value
	@treturn float newValue

	@usage
	function onCalculateUnitValue(entry, value)
		if entry.eduType = "my_unit" then
			return value * 2
		end
		return value
	end
	*/
	onCalculateUnitValue = new sol::function(luaState["onCalculateUnitValue"]);
	checkLuaFunc(&onCalculateUnitValue);

	/***
	Called on the completion of the siege (in any way, with any outcome).

	@function onEndSiege
	@tparam int xCoord x coordinate of siege(settlement or fort)
	@tparam int yCoord y coordinate of siege(settlement or fort)

	@usage
	function onEndSiege(x,y)
	--something here
	end
	*/
	onEndSiege = new sol::function(luaState["onEndSiege"]);
	checkLuaFunc(&onEndSiege);
	/***
	Called on the starting of the siege (in any way, with any outcome).

	@function onStartSiege
	@tparam int xCoord x coordinate of siege(settlement or fort)
	@tparam int yCoord y coordinate of siege(settlement or fort)

	@usage
	function onStartSiege(x,y)
	--something here
	end
	*/
	onStartSiege = new sol::function(luaState["onStartSiege"]);
	checkLuaFunc(&onStartSiege);


	/***
	Called when clicking on a faction symbol in the faction selection menu.

	@function onFactionSymbolSelect(factionID)
	@tparam int factionID

	@usage
	function onFactionSymbolSelect(factionID)
	--something here
	end
	*/
	onFactionSymbolSelect = new sol::function(luaState["onFactionSymbolSelect"]);
	checkLuaFunc(&onFactionSymbolSelect);


	if (onPluginLoad != nullptr)
	{
		tryLua((*onPluginLoad)())
	}
}


