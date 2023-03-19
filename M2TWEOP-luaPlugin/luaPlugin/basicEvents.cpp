#include "basicEvents.h"
#include <unordered_map>
using namespace std;


void onEventWrapper(const DWORD eventAddr, DWORD** vTab, DWORD arg2)
{
	const auto currentEvent = gameEvents::events.find(eventAddr);
	if (currentEvent == gameEvents::events.end()) {
		return;
	}
	currentEvent->second->callEvent(vTab);
}

void initReligionNames()
{
	const auto* religionDb = *reinterpret_cast <religionDatabase**>(0x016A0B90);
	for (int i = 0; i < religionDb->religionCount; i++)
	{
		religionNames[i] = gameHelpers::getReligionName(i);
	}
}

namespace gameEvents
{

	void initEvents()
	{
		initReligionNames();
		const int gv = (m2tweopHelpers::getGameVersion()) - 1;
		//namedCharacterEvents
		DWORD newCode[] = { 0x0136BFE4, 0x01326FBC };
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterTurnStart");

		newCode[0] = 0x013370DC; newCode[1] = 0x012F20BC;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCapturedCharacterRansomed");

		newCode[0] = 0x01337194; newCode[1] = 0x012F2174;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCapturedCharacterReleased");

		newCode[0] = 0x0133DF5C; newCode[1] = 0x012F8F3C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onFatherDiesNatural");

		newCode[0] = 0x01366D54; newCode[1] = 0x01321D2C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onPreBattleWithdrawal");

		newCode[0] = 0x01367ADC; newCode[1] = 0x01322AB4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onPostBattle");

		newCode[0] = 0x01369E3C; newCode[1] = 0x01324E14;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onHireMercenaries");

		newCode[0] = 0x01369F0C; newCode[1] = 0x01324EE4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onGeneralCaptureResidence");

		newCode[0] = 0x0136A0B4; newCode[1] = 0x0132508C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onLeaderDestroyedFaction");

		newCode[0] = 0x0136A584; newCode[1] = 0x0132555C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onOfferedForAdoption");

		newCode[0] = 0x0136A65C; newCode[1] = 0x01325634;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onLesserGeneralOfferedForAdoption");

		newCode[0] = 0x0136A72C; newCode[1] = 0x01325704;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onOfferedForMarriage");

		newCode[0] = 0x0136A7F4; newCode[1] = 0x013257CC;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onBrotherAdopted");

		newCode[0] = 0x0136A8C4; newCode[1] = 0x0132589C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onBecomesFactionLeader");

		newCode[0] = 0x0136A994; newCode[1] = 0x0132596C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onBecomesFactionHeir");

		newCode[0] = 0x0136AA64; newCode[1] = 0x01325A3C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCeasedFactionLeader");

		newCode[0] = 0x0136AB34; newCode[1] = 0x01325B0C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCeasedFactionHeir");

		newCode[0] = 0x0136B54C; newCode[1] = 0x01326524;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onBirth");

		newCode[0] = 0x0136B61C; newCode[1] = 0x013265F4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterComesOfAge");

		newCode[0] = 0x0136B6EC; newCode[1] = 0x013266C4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterMarries");

		newCode[0] = 0x0136B7C4; newCode[1] = 0x0132679C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterMarriesPrincess");

		newCode[0] = 0x0136B89C; newCode[1] = 0x01326874;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onMarriageAlliancePossible");

		newCode[0] = 0x0136B96C; newCode[1] = 0x01326944;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onMarriageAllianceOffered");

		newCode[0] = 0x0136BA3C; newCode[1] = 0x01326A14;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onPriestBecomesHeretic");

		newCode[0] = 0x0136BB0C; newCode[1] = 0x01326AE4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterNearHeretic");

		newCode[0] = 0x0136BBDC; newCode[1] = 0x01326BB4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterNearWitch");

		newCode[0] = 0x0136BCAC; newCode[1] = 0x01326C84;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCardinalPromoted");

		newCode[0] = 0x0136BF14; newCode[1] = 0x01326EEC;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onCharacterBecomesAFather");

		newCode[0] = 0x0136C25C; newCode[1] = 0x01327234;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onGeneralDevastatesTile");

		newCode[0] = 0x0136CD64; newCode[1] = 0x01327D3C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onExecutesASpyOnAMission");

		newCode[0] = 0x0136CFE4; newCode[1] = 0x01327FBC;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onExecutesAnAssassinOnAMission");

		newCode[0] = 0x0136D26C; newCode[1] = 0x01328244;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onSufferAssassinationAttempt");

		newCode[0] = 0x0136D414; newCode[1] = 0x013283EC;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onSufferAcquisitionAttempt");

		newCode[0] = 0x0136D5AC; newCode[1] = 0x01328584;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onSufferMarriageAttempt");

		newCode[0] = 0x0136D754; newCode[1] = 0x0132872C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onSufferDenouncementAttempt");

		newCode[0] = 0x0136D8EC; newCode[1] = 0x013288C4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onLeaderOrderedSabotage");

		newCode[0] = 0x0136DA84; newCode[1] = 0x01328A5C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onLeaderOrderedBribery");

		newCode[0] = 0x0136DB4C; newCode[1] = 0x01328B24;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onAcceptBribe");

		newCode[0] = 0x0136DC14; newCode[1] = 0x01328BEC;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onRefuseBribe");

		newCode[0] = 0x0136DCDC; newCode[1] = 0x01328CB4;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onInsurrection");

		newCode[0] = 0x0136DE7C; newCode[1] = 0x01328E54;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onLeaderOrderedDiplomacy");

		newCode[0] = 0x0136E394; newCode[1] = 0x0132936C;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onNewAdmiralCreated");

		newCode[0] = 0x0136EC8C; newCode[1] = 0x01329C64;
		addEvent<EventType::namedCharacterEvent>(newCode[gv], "onGovernorBuildingDestroyed");

		//ransomEvent
		newCode[0] = 0x0136C8EC; newCode[1] = 0x013278C4;
		addEvent<EventType::ransomEvent>(newCode[gv], "onGeneralPrisonersRansomedCaptor");

		newCode[0] = 0x0136C9C4; newCode[1] = 0x0132799C;
		addEvent<EventType::ransomEvent>(newCode[gv], "onGeneralPrisonersRansomedCaptive");

		newCode[0] = 0x0136CAA4; newCode[1] = 0x01327A7C;
		addEvent<EventType::ransomEvent>(newCode[gv], "onFactionLeaderPrisonersRansomedCaptor");

		newCode[0] = 0x0136CB84; newCode[1] = 0x01327B5C;
		addEvent<EventType::ransomEvent>(newCode[gv], "onFactionLeaderPrisonersRansomedCaptive");

		//spyMissionEvent
		newCode[0] = 0x0136CC94; newCode[1] = 0x01327C6C;
		addEvent<EventType::spyMissionEvent>(newCode[gv], "onSpyMission");

		//missionEvent
		newCode[0] = 0x0136E0E4; newCode[1] = 0x013290BC;
		addEvent<EventType::missionEvent>(newCode[gv], "onLeaderMissionSuccess");

		newCode[0] = 0x0136DF64; newCode[1] = 0x01328F3C;
		addEvent<EventType::missionEvent>(newCode[gv], "onLeaderMissionFailed");

		//diplomacyMissionEvent
		newCode[0] = 0x0136D9B4; newCode[1] = 0x0132898C;
		addEvent<EventType::diplomacyMissionEvent>(newCode[gv], "onBriberyMission");

		newCode[0] = 0x0136DDAC; newCode[1] = 0x01328D84;
		addEvent<EventType::diplomacyMissionEvent>(newCode[gv], "onDiplomacyMission");

		//agentMissionEvent
		newCode[0] = 0x0136CF0C; newCode[1] = 0x01327EE4;
		addEvent<EventType::agentMissionEvent>(newCode[gv], "onAssassinationMission");

		newCode[0] = 0x0136D33C; newCode[1] = 0x01328314;
		addEvent<EventType::agentMissionEvent>(newCode[gv], "onAcquisitionMission");

		newCode[0] = 0x0136D4DC; newCode[1] = 0x013284B4;
		addEvent<EventType::agentMissionEvent>(newCode[gv], "onMarriageMission");

		newCode[0] = 0x0136D67C; newCode[1] = 0x01328654;
		addEvent<EventType::agentMissionEvent>(newCode[gv], "onDenouncementMission");

		newCode[0] = 0x0136D81C; newCode[1] = 0x013287F4;
		addEvent<EventType::agentMissionEvent>(newCode[gv], "onSabotageMission");

		//OrderSpyMissionEvent
		newCode[0] = 0x0136CE3C; newCode[1] = 0x01327E14;
		addEvent<EventType::orderSpyMissionEvent>(newCode[gv], "onLeaderOrderedSpyingMission");

		//OrderKillMissionEvent
		newCode[0] = 0x0136D194; newCode[1] = 0x0132816C;
		addEvent<EventType::orderKillMissionEvent>(newCode[gv], "onLeaderOrderedAssassination");

		//namedCharacterCrusadeEvent
		newCode[0] = 0x0136C3E4; newCode[1] = 0x013273BC;
		addEvent<EventType::namedCharacterCrusadeEvent>(newCode[gv], "onGeneralJoinCrusade");

		newCode[0] = 0x0136C4B4; newCode[1] = 0x0132748C;
		addEvent<EventType::namedCharacterCrusadeEvent>(newCode[gv], "onGeneralAbandonCrusade");

		newCode[0] = 0x0136C594; newCode[1] = 0x0132756C;
		addEvent<EventType::namedCharacterCrusadeEvent>(newCode[gv], "onGeneralArrivesCrusadeTargetRegion");

		newCode[0] = 0x0136C66C; newCode[1] = 0x01327644;
		addEvent<EventType::namedCharacterCrusadeEvent>(newCode[gv], "onGeneralTakesCrusadeTarget");

		//namedCharacterSettEvent
		newCode[0] = 0x0136C0B4; newCode[1] = 0x0132708C;
		addEvent<EventType::namedCharacterSettEvent>(newCode[gv], "onCharacterTurnEnd");

		newCode[0] = 0x0136C18C; newCode[1] = 0x01327164;
		addEvent<EventType::namedCharacterSettEvent>(newCode[gv], "onCharacterTurnEndInSettlement");

		//namedCharacterTargetSettEvents
		newCode[0] = 0x01369FE4; newCode[1] = 0x01324FBC;
		addEvent<EventType::namedCharacterTargetSettEvent>(newCode[gv], "onGeneralCaptureSettlement");

		newCode[0] = 0x0136A3E4; newCode[1] = 0x013253BC;
		addEvent<EventType::namedCharacterTargetSettEvent>(newCode[gv], "onGeneralAssaultsResidence");

		//namedCharacterTargetCharEvents
		newCode[0] = 0x0136A4B4; newCode[1] = 0x0132548C;
		addEvent<EventType::namedCharacterTargetCharEvent>(newCode[gv], "onGeneralAssaultsGeneral");

		newCode[0] = 0x0136C75C; newCode[1] = 0x01327734;
		addEvent<EventType::namedCharacterTargetCharEvent>(newCode[gv], "onCharacterAttacksCrusadingGeneral");

		//namedCharacterDisasterEvents
		newCode[0] = 0x0136A254; newCode[1] = 0x0132522C;
		addEvent<EventType::namedCharacterDisasterEvent>(newCode[gv], "onCharacterDamagedByDisaster");

		//settlementEvents
		newCode[0] = 0x0136E2B4; newCode[1] = 0x0132928C;
		addEvent<EventType::settlementEvent>(newCode[gv], "onSettlementTurnStart");

		//gameEvents
		newCode[0] = 0x0135E30C; newCode[1] = 0x013192E4;
		addEvent<EventType::gameEvent>(newCode[gv], "onAbandonShowMe");

		newCode[0] = 0x013319E4; newCode[1] = 0x012EC9C4;
		addEvent<EventType::gameEvent>(newCode[gv], "onGameReloaded");

		newCode[0] = 0x01367F5C; newCode[1] = 0x01322F34;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattleWinningPlaza");

		newCode[0] = 0x01368044; newCode[1] = 0x0132301C;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattleStopsWinningPlaza");

		newCode[0] = 0x01368124; newCode[1] = 0x013230FC;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattleDominatingPlaza");

		newCode[0] = 0x013681F4; newCode[1] = 0x013231CC;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattngineUnmanned");

		newCode[0] = 0x013682E4; newCode[1] = 0x013232BC;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattlePlayerArmyHalfDestroyed");

		newCode[0] = 0x013683BC; newCode[1] = 0x01323394;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattnemyArmyHalfDestroyed");

		newCode[0] = 0x01368584; newCode[1] = 0x0132355C;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattleTideofBattle");

		newCode[0] = 0x01368DA4; newCode[1] = 0x01323D7C;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattleFinished");

		newCode[0] = 0x01368E5C; newCode[1] = 0x01323E34;
		addEvent<EventType::gameEvent>(newCode[gv], "onBattleArmyHalfDestroyed");

		//crusadeEvents
		newCode[0] = 0x013321D4; newCode[1] = 0x012ED1B4;
		addEvent<EventType::crusadeEvent>(newCode[gv], "onCrusadeCalled");

		newCode[0] = 0x0133778C; newCode[1] = 0x012F276C;
		addEvent<EventType::crusadeEvent>(newCode[gv], "onPopeAcceptsCrusadeTarget");

		newCode[0] = 0x01337844; newCode[1] = 0x012F2824;
		addEvent<EventType::crusadeEvent>(newCode[gv], "onPopeRejectsCrusadeTarget");

		newCode[0] = 0x0133228C; newCode[1] = 0x012ED26C;
		addEvent<EventType::crusadeEventEnds>(newCode[gv], "onCrusadeEnds");

		//factionCrusadeEvents
		newCode[0] = 0x0136AEAC; newCode[1] = 0x01325E84;
		addEvent<EventType::factionCrusadeEvent>(newCode[gv], "onUnitsDesertCrusade");

		//crusadeTargetEvents
		newCode[0] = 0x0136AF7C; newCode[1] = 0x01325F54;
		addEvent<EventType::crusadeTargetEvent>(newCode[gv], "onArmyTakesCrusadeTarget");

		//transgressionEvents
		newCode[0] = 0x0136B044; newCode[1] = 0x0132601C;
		addEvent<EventType::transgressionEvent>(newCode[gv], "onTransgression");

		newCode[0] = 0x0136B1DC; newCode[1] = 0x013261B4;
		addEvent<EventType::transgressionEvent>(newCode[gv], "onForgiveness");

		//factionEvents
		newCode[0] = 0x01366E24; newCode[1] = 0x01321DFC;
		addEvent<EventType::factionEvent>(newCode[gv], "onBattleAiCommenced");

		newCode[0] = 0x01366EF8; newCode[1] = 0x01321ED4;
		addEvent<EventType::factionEvent>(newCode[gv], "onBattleDelayPhaseCommenced");

		newCode[0] = 0x01366FD4; newCode[1] = 0x01321FAC;
		addEvent<EventType::factionEvent>(newCode[gv], "onBattleDeploymentPhaseCommenced");

		newCode[0] = 0x013670AC; newCode[1] = 0x01322084;
		addEvent<EventType::factionEvent>(newCode[gv], "onBattleConflictPhaseCommenced");

		newCode[0] = 0x0136931C; newCode[1] = 0x013242F4;
		addEvent<EventType::factionEvent>(newCode[gv], "onFactionTurnStart");

		newCode[0] = 0x013694BC; newCode[1] = 0x01324494;
		addEvent<EventType::factionEvent>(newCode[gv], "onFactionNewCapital");

		newCode[0] = 0x013693EC; newCode[1] = 0x013243C4;
		addEvent<EventType::factionEvent>(newCode[gv], "onPreFactionTurnStart");

		newCode[0] = 0x01369A5C; newCode[1] = 0x01324A34;
		addEvent<EventType::factionEvent>(newCode[gv], "onVotedForPope");

		newCode[0] = 0x01369B14; newCode[1] = 0x01324AEC;
		addEvent<EventType::factionEvent>(newCode[gv], "onFactionExcommunicated");

		newCode[0] = 0x01369CAC; newCode[1] = 0x01324C84;
		addEvent<EventType::factionEvent>(newCode[gv], "onHordeFormed");

		newCode[0] = 0x01369D74; newCode[1] = 0x01324D4C;
		addEvent<EventType::factionEvent>(newCode[gv], "onFactionTurnEnd");

		newCode[0] = 0x0136BD74; newCode[1] = 0x01326D4C;
		addEvent<EventType::factionEvent>(newCode[gv], "onCardinalRemoved");

		newCode[0] = 0x0136BE44; newCode[1] = 0x01326E1C;
		addEvent<EventType::factionEvent>(newCode[gv], "onInquisitorAppointed");

		newCode[0] = 0x0136D0BC; newCode[1] = 0x01328094;
		addEvent<EventType::factionEvent>(newCode[gv], "onAssassinCaughtAttackingPope");

		//diplomacyEvents
		newCode[0] = 0x0136958C; newCode[1] = 0x01324564;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onFactionWarDeclared");

		newCode[0] = 0x0136965C; newCode[1] = 0x01324634;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onFactionAllianceDeclared");

		newCode[0] = 0x0136972C; newCode[1] = 0x01324704;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onInterFactionMarriage");

		newCode[0] = 0x01369804; newCode[1] = 0x013247DC;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onFactionTradeAgreementMade");

		newCode[0] = 0x01369994; newCode[1] = 0x0132496C;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onPopeElected");

		newCode[0] = 0x01369BE4; newCode[1] = 0x01324BBC;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onFactionBreakAlliance");

		newCode[0] = 0x0136B2A4; newCode[1] = 0x0132627C;
		addEvent<EventType::diplomacyEvent>(newCode[gv], "onUpdateAttitude");

		//diplomacyMoneyEvents
		newCode[0] = 0x013698CC; newCode[1] = 0x013248A4;
		addEvent<EventType::diplomacyMoneyEvent>(newCode[gv], "onGiveMoney");

		newCode[0] = 0x0136B36C; newCode[1] = 0x01326344;
		addEvent<EventType::diplomacyMoneyEvent>(newCode[gv], "onDemeanour");

		//armyEvent
		newCode[0] = 0x013686FC; newCode[1] = 0x013236D4;
		addEvent<EventType::armyEvent>(newCode[gv], "onBattleArmyRouted");

		newCode[0] = 0x0136886C; newCode[1] = 0x01323844;
		addEvent<EventType::armyEvent>(newCode[gv], "onBattleReinforcementsArrive");

		//characterEvent
		newCode[0] = 0x013687B4; newCode[1] = 0x0132378C;
		addEvent<EventType::characterEvent>(newCode[gv], "onBattleGeneralRouted");

		newCode[0] = 0x0136914C; newCode[1] = 0x01324124;
		addEvent<EventType::characterEvent>(newCode[gv], "onBattleGeneralKilled");

		//stratUnitEvent
		newCode[0] = 0x0136B434; newCode[1] = 0x0132640C;
		addEvent<EventType::stratUnitEvent>(newCode[gv], "onUnitDisbanded");

		//stratUnitSettEvent
		newCode[0] = 0x0136E45C; newCode[1] = 0x01329434;
		addEvent<EventType::stratUnitSettEvent>(newCode[gv], "onUnitTrained");

		//stratUnitSettEvent
		newCode[0] = 0x0136E52C; newCode[1] = 0x01329504;
		addEvent<EventType::characterUnitSettEvent>(newCode[gv], "onGovernorUnitTrained");

		//characterBuildingEvent
		newCode[0] = 0x0136E614; newCode[1] = 0x013295EC;
		addEvent<EventType::characterBuildingEvent>(newCode[gv], "onGovernorBuildingCompleted");

		//buildingEvent
		newCode[0] = 0x0136EA14; newCode[1] = 0x013299EC;
		addEvent<EventType::buildingEvent>(newCode[gv], "onBuildingDestroyed");

		newCode[0] = 0x0136EAE4; newCode[1] = 0x01329ABC;
		addEvent<EventType::buildingEvent>(newCode[gv], "onAddedToBuildingQueue");

		newCode[0] = 0x0136EBB4; newCode[1] = 0x01329B8C;
		addEvent<EventType::buildingEvent>(newCode[gv], "onAddedToTrainingQueue");

		//agentCreatedEvent
		newCode[0] = 0x0136E874; newCode[1] = 0x0132984C;
		addEvent<EventType::agentCreatedEvent>(newCode[gv], "onAgentCreated");

		newCode[0] = 0x0136E944; newCode[1] = 0x0132991C;
		addEvent<EventType::agentCreatedEvent>(newCode[gv], "onGovernorAgentCreated");

		//battleUnitEvent
		newCode[0] = 0x0136863C; newCode[1] = 0x01323614;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleUnitGoesBerserk");

		newCode[0] = 0x01368924; newCode[1] = 0x013238FC;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleSiegeEngineDestroyed");

		newCode[0] = 0x013689E4; newCode[1] = 0x013239BC;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleSiegeEngineDocksWall");

		newCode[0] = 0x01368AA4; newCode[1] = 0x01323A7C;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleGatesAttackedByEngine");

		newCode[0] = 0x01368B64; newCode[1] = 0x01323B3C;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleGatesDestroyedByEngine");

		newCode[0] = 0x01368C24; newCode[1] = 0x01323BFC;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleWallsBreachedByEngine");

		newCode[0] = 0x0136908C; newCode[1] = 0x01324064;
		addEvent<EventType::battleUnitEvent>(newCode[gv], "onBattleUnitRouts");

		//battleUnitEvent2
		newCode[0] = 0x01368CE4; newCode[1] = 0x01323CBC;
		addEvent<EventType::battleUnitEvent2>(newCode[gv], "onBattleWallsCaptured");

		//battlePlayerArmyUnitEvent
		newCode[0] = 0x013684A4; newCode[1] = 0x0132347C;
		addEvent<EventType::battlePlayerArmyUnitEvent>(newCode[gv], "onBattlePlayerUnderAttackIdle");

		//battleArmyFromUnitEvent
		newCode[0] = 0x01368FCC; newCode[1] = 0x0132355C;
		addEvent<EventType::battleArmyFromUnitEvent>(newCode[gv], "onBattleWinningCombat");

		//battleArmyFromUnitEvent2
		newCode[0] = 0x01368F14; newCode[1] = 0x01323EEC;
		addEvent<EventType::battleArmyFromUnitEvent2>(newCode[gv], "onBattleArmyTired");

		//battlePlayerUnitEvents
		newCode[0] = 0x0136718C; newCode[1] = 0x01322164;
		addEvent<EventType::battlePlayerUnitAttackEvent>(newCode[gv], "onBattlePlayerUnitAttacksEnemyUnit");

		newCode[0] = 0x013675D4; newCode[1] = 0x013225AC;
		addEvent<EventType::battlePlayerUnitEvent>(newCode[gv], "onBattlePlayerUnitGoesBerserk");

		newCode[0] = 0x0136778C; newCode[1] = 0x01322764;
		addEvent<EventType::battlePlayerUnitEvent>(newCode[gv], "onBattlePlayerUnitRouts");

		newCode[0] = 0x0136793C; newCode[1] = 0x01322914;
		addEvent<EventType::battlePlayerUnitEvent>(newCode[gv], "onBattlePlayerSiegeEngineDestroyed");

		newCode[0] = 0x01367C64; newCode[1] = 0x01322C3C;
		addEvent<EventType::battlePlayerUnitEvent>(newCode[gv], "onBattleGatesAttackedByPlayerEngine");

		//battleAIUnitEvents
		newCode[0] = 0x0136726C; newCode[1] = 0x01322244;
		addEvent<EventType::battleAiUnitAttackEvent>(newCode[gv], "onBattleEnemyUnitAttacksPlayerUnit");

		newCode[0] = 0x013676AC; newCode[1] = 0x01322684;
		addEvent<EventType::battleAiUnitEvent>(newCode[gv], "onBattleEnemyUnitGoesBerserk");

		newCode[0] = 0x0136785C; newCode[1] = 0x01322834;
		addEvent<EventType::battleAiUnitEvent>(newCode[gv], "onBattnemyUnitRouts");

		newCode[0] = 0x01367A14; newCode[1] = 0x013229EC;
		addEvent<EventType::battleAiUnitEvent>(newCode[gv], "onBattnemySiegeEngineDestroyed");

		newCode[0] = 0x01367D44; newCode[1] = 0x01322D1C;
		addEvent<EventType::battleAiUnitEvent>(newCode[gv], "onBattleGatesAttackedByEnemyEngine");

		//disasterEvents
		newCode[0] = 0x0136A17C; newCode[1] = 0x01325154;
		addEvent<EventType::disasterEvent>(newCode[gv], "onDisaster");

		//eventCounterEvents
		newCode[0] = 0x0136A30C; newCode[1] = 0x013252E4;
		addEvent<EventType::eventCounterEvent>(newCode[gv], "onEventCounter");

		//fortEvents
		newCode[0] = 0x0136AC04; newCode[1] = 0x01325BDC;
		addEvent<EventType::fortEvent>(newCode[gv], "onUngarrisonedFort");

		//tileEvents
		newCode[0] = 0x0136ACCC; newCode[1] = 0x01325CA4;
		addEvent<EventType::tileEvent>(newCode[gv], "onTileSeen");

		//objEvents
		newCode[0] = 0x0136AD8C; newCode[1] = 0x01325D64;
		addEvent<EventType::objEvent>(newCode[gv], "onObjSeen");
	}

	template<EventType EvType> void addEvent(const DWORD key, const char* name)
	{
		events[key] = std::make_unique<Event<EvType>>(name, plugData::data.luaAll.luaState);
	}

	regionStruct* getCharacterRegion(const namedCharacter* character)
	{
		return gameHelpers::getRegion(m2tweopHelpers::getTileRegionID(character->gen->xCoord, character->gen->yCoord));
	}

}



void checkLuaFunc(sol::function** lRef)
{
	if ((*lRef)->valid() == false)
	{
		*lRef = nullptr;
	}
};

void onCharacterSelected(namedCharacter* gen)
{
	plugData::data.luaAll.onCharacterSelected(gen);
}

void onSiegeEquipmentCompleted(settlementStruct* sett, fortStruct* frt)
{
	plugData::data.luaAll.onSiegeEquipmentCompleted(sett, frt);
}
void onReadGameDbsAtStart()
{
	gameEvents::initEvents();
	if (plugData::data.luaAll.onReadGameDbsAtStart != nullptr)
	{
		tryLua((*plugData::data.luaAll.onReadGameDbsAtStart)());
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
			MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);
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
			MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);
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
			MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL); \
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


