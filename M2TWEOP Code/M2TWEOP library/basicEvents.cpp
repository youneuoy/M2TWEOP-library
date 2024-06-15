#include "basicEvents.h"
#include <unordered_map>

#include "fastFuncts.h"
#include "gameHelpers.h"
#include "smallFuncs.h"
#include "character.h"
#include "settlement.h"
#include "faction.h"
#include "characterRecord.h"
#include "m2tweopHelpers.h"
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
		else if (EvType == EventType::attackResidenceEvent)
		{
			auto eventData = reinterpret_cast<eventTrigger*>(vTab);
			auto character = gameHelpers::getEventNamedCharacter(eventData);
			auto settlement = character->gen->besiegingSettlement;
			fortStruct* fort = nullptr;
			if (settlement)
			{
				fort = gameHelpers::getTileFort(gameHelpers::getTile(settlement->xCoord, settlement->yCoord));
				if (fort)
					settlement = nullptr;
			}
			if (funk != nullptr) {
				tryLuaBasicEventFunk((*funk)(eventData, settlement, fort))
			}
			return 2;
		}
		else
		{
			return 0;
		}
		
	}
	
	void initEvents()
	{
		const int gv = (smallFuncs::getGameVersion()) - 1;
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
		return gameHelpers::getRegion(fastFuncts::getTileRegionID(character->gen->xCoord, character->gen->yCoord));
	}


void onReadGameDbsAtStart()
{
	if (plugData::data.luaAll.onReadGameDbsAtStart != nullptr)
	{
		tryLua((*plugData::data.luaAll.onReadGameDbsAtStart)());
	}
}

void onGameInit()
{
	if (plugData::data.luaAll.onGameInit != nullptr)
	{
		tryLua((*plugData::data.luaAll.onGameInit)());
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

int onfortificationlevelS(settlementStruct* settlement, bool* isCastle, bool* isChanged)
{
	if (plugData::data.luaAll.onfortificationlevelS != nullptr)
	{

		auto funcResult = (*plugData::data.luaAll.onfortificationlevelS)(settlement);
		if (!funcResult.valid())
		{

			sol::error luaError = funcResult;
			MessageBoxA(NULL, luaError.what(), "Lua exception in onfortificationlevelS() call!", NULL);
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
	plugData::data.luaAll.onChangeTurnNum(num);
}

void onNewGameStart()
{
	//initReligionNames();
	if (plugData::data.luaAll.onNewGameStart != nullptr)
	{
		tryLua((*plugData::data.luaAll.onNewGameStart)());
	}
}


void onCampaignMapLoaded()
{
	if (!plugData::data.luaAll.hashLoaded)
	{
		plugData::data.luaAll.fillHashMaps();
		m2tweopHelpers::logStringGame("Hashmaps filled");
	}
	if (plugData::data.luaAll.onCampaignMapLoaded != nullptr)
	{
		tryLua((*plugData::data.luaAll.onCampaignMapLoaded)());
	}
}

void onLoadGamePl(std::vector<std::string>* saveFiles)
{
	if (plugData::data.luaAll.onLoadSaveFile != nullptr)
	{
		sol::as_table_t<std::vector<std::string>> wrapped_vec = *saveFiles;
		tryLua((*plugData::data.luaAll.onLoadSaveFile)(wrapped_vec));
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


