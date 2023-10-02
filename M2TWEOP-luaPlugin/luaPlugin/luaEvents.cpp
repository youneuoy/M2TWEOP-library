///
//![Lua logo](_static/Lua.png)
//M2TWEOP structures and functions. There are not many examples and descriptions here. Also note that the examples do not include many of the checks that would be required when creating modifications.
//@module LuaPlugin
//@author youneuoy
//@license GPL-3.0
#include "luaP.h"
#include "plugData.h"
void luaP::onPluginLoadF()
{
	///EOP Events
	//@section eventsList

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
	@tfield onReadGameDbsAtStart onReadGameDbsAtStart
	@tfield onClickAtTile onClickAtTile
	@tfield onCampaignMapLoaded onCampaignMapLoaded
	@tfield onCreateSaveFile onCreateSaveFile
	@tfield onLoadSaveFile onLoadSaveFile
	@tfield onChangeTurnNum onChangeTurnNum
	@tfield onSelectWorldpkgdesc onSelectWorldpkgdesc
	@tfield onfortificationlevelS onfortificationlevelS
	@tfield onEndSiege onEndSiege
	@tfield onStartSiege onStartSiege
	@tfield onPluginLoad onPluginLoad




	@table EventsFunctionsList
	*/


	/***
	Called at a character's turn start.

	@function onCharacterTurnStart
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterTurnStart(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	A captured character has been successfully ransomed back from the enemy.

	@function onCapturedCharacterRansomed
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCapturedCharacterRansomed(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	A captured character has been released by the enemy.

	@function onCapturedCharacterReleased
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCapturedCharacterReleased(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	A character father died of natural causes.

	@function onFatherDiesNatural
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onFatherDiesNatural(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	When a battle is about to start but one of the armies withdraws.

	@function onPreBattleWithdrawal
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onPreBattleWithdrawal(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	When a battle has finished.

	@function onPostBattle
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onPostBattle(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	A General has hired some mercenaries.

	@function onHireMercenaries
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onHireMercenaries(namedChar, faction, region, characterType, religion)
	--something here
	end
	*/

	/***
	A General has captured a residence such as a fort or watchtower.

	@function onGeneralCaptureResidence
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGeneralCaptureResidence(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A faction has been destroyed.

	@function onLeaderDestroyedFaction
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderDestroyedFaction(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	An adoption has been proposed.

	@function onOfferedForAdoption
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onOfferedForAdoption(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A lesser general adoption has been proposed (man of the hour event).

	@function onLesserGeneralOfferedForAdoption
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLesserGeneralOfferedForAdoption(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A marriage offer has been proposed.

	@function onOfferedForMarriage
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onOfferedForMarriage(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A brother has been adopted.

	@function onBrotherAdopted
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onBrotherAdopted(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A child is born to the faction leader.

	@function onBirth
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onBirth(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character has come of age.

	@function onCharacterComesOfAge
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterComesOfAge(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character has married.

	@function onCharacterMarries
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterMarries(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character has married a princess.

	@function onCharacterMarriesPrincess
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterMarriesPrincess(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A marriage alliance is possible.

	@function onMarriageAlliancePossible
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onMarriageAlliancePossible(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A marriage alliance has been offered.

	@function onMarriageAllianceOffered
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onMarriageAllianceOffered(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A priest has gone mad.

	@function onPriestBecomesHeretic
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onPriestBecomesHeretic(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character is adjacent to a heretic.

	@function onCharacterNearHeretic
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterNearHeretic(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character is adjacent to a witch.

	@function onCharacterNearWitch
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterNearWitch(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character has been promoted to a cardinal.

	@function onCardinalPromoted
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCardinalPromoted(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A character has become a father.

	@function onCharacterBecomesAFather
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterBecomesAFather(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A General and his army has devastated an enemy's fertile land.

	@function onGeneralDevastatesTile
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGeneralDevastatesTile(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A spying mission has failed and the spy is executed by the target.

	@function onExecutesASpyOnAMission
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onExecutesASpyOnAMission(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	An assassination mission has failed and the assassin is executed by the target.

	@function onExecutesAnAssassinOnAMission
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onExecutesAnAssassinOnAMission(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Someone has had an attempt on their life.

	@function onSufferAssassinationAttempt
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onSufferAssassinationAttempt(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Someone has had an attempt on their assets.

	@function onSufferAcquisitionAttempt
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onSufferAcquisitionAttempt(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Someone has had an attempt on their bachelorhood.

	@function onSufferMarriageAttempt
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onSufferMarriageAttempt(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Someone has had a denouncement attempt.

	@function onSufferDenouncementAttempt
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onSufferDenouncementAttempt(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A Faction leader has ordered a sabotage mission.

	@function onLeaderOrderedSabotage
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderOrderedSabotage(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Someone has been bribed.

	@function onAcceptBribe
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onAcceptBribe(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Someone has refused a bribe.

	@function onRefuseBribe
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onRefuseBribe(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Insurgence has been provoked.

	@function onInsurrection
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onInsurrection(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A Faction leader has ordered a diplomacy mission.

	@function onLeaderOrderedDiplomacy
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderOrderedDiplomacy(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A new admiral has been created for a new ship.

	@function onNewAdmiralCreated
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onNewAdmiralCreated(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A building has been destroyed.

	@function onGovernorBuildingDestroyed
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGovernorBuildingDestroyed(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Games have been thrown.

	@function onGovernorThrowGames
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGovernorThrowGames(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	Races have been thrown.

	@function onGovernorThrowRaces
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGovernorThrowRaces(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	The player has selected a character.

	@function onCharacterSelected
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterSelected(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	The player has selected an enemy character.

	@function onEnemyCharacterSelected
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onEnemyCharacterSelected(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	The player has selected a position beyond the character's extents.

	@function onMultiTurnMove
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onMultiTurnMove(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	The player has opened the panel for the selected character.

	@function onCharacterPanelOpen
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterPanelOpen(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A mission has been completed.

	@function onLeaderMissionSuccess
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam string missionName
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderMissionSuccess(namedChar, faction, missionName, region, characterType, religion)
	--something
	end
	*/

	/***
	A mission has failed.

	@function onLeaderMissionFailed
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam string missionName
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderMissionFailed(namedChar, faction, missionName, region, characterType, religion)
	--something
	end
	*/

	/***
	A General has been sent on Crusade/Jihad.

	@function onGeneralJoinCrusade
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralJoinCrusade(namedChar, faction, targetSettlement, targetFaction, targetRegion, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A General has left a Crusade/Jihad.

	@function onGeneralAbandonCrusade
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralAbandonCrusade(namedChar, faction, targetSettlement, targetFaction, targetRegion, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A General has arrived in the Crusade/Jihad target region.

	@function onGeneralArrivesCrusadeTargetRegion
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralArrivesCrusadeTargetRegion(namedChar, faction, targetSettlement, targetFaction, targetRegion, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A General has taken the Crusade/Jihad target settlement.

	@function onGeneralTakesCrusadeTarget
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralTakesCrusadeTarget(namedChar, faction, targetSettlement, targetFaction, targetRegion, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A Character has finished its turn.

	@function onCharacterTurnEnd
	@tparam namedCharacter namedChar
	@tparam settlementStruct|nil settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterTurnEnd(namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A Character has finished its turn in a settlement.

	@function onCharacterTurnEndInSettlement
	@tparam namedCharacter namedChar
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterTurnEndInSettlement(namedChar, settlement, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	The character has been made the faction leader.

	@function onBecomesFactionLeader
	@tparam namedCharacter leader
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onBecomesFactionLeader(leader, faction, region, characterType, religion)
		print("Function: onBecomesFactionLeader()\n\tLeader: "..leader.fullName)
	end
	*/

	/***
	The character is no longer faction leader.

	@function onCeasedFactionLeader
	@tparam namedCharacter leader
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCeasedFactionLeader(leader, faction, region, characterType, religion)
		--something
	end
	*/

	/***
	The character has been made a faction heir.

	@function onBecomesFactionHeir
	@tparam namedCharacter heir
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onBecomesFactionHeir(heir, faction, region, characterType, religion)
		print("Function: onBecomesFactionHeir()\n\tHeir: "..heir.fullName)
	end
	*/

	/***
	The character is no longer faction heir.

	@function onCeasedFactionHeir
	@tparam namedCharacter heir
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCeasedFactionHeir(heir, faction, region, characterType, religion)
		print("Function: onCeasedFactionHeir()\n\tHeir: "..heir.fullName)
	end
	*/

	/***
	A character has been injured by a disaster.

	@function onCharacterDamagedByDisaster
	@tparam string eventType (earthquake, flood, horde, storm, volcano, dustbowl, locusts, famine, plague, riot, fire)
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onCharacterDamagedByDisaster(eventType, namedChar, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A General has captured a settlement.

	@function onGeneralCaptureSettlement
	@tparam namedCharacter namedChar
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralCaptureSettlement(namedChar, settlement, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	An assault has taken place.

	@function onGeneralAssaultsResidence
	@tparam namedCharacter namedChar
	@tparam settlementStruct|fortStruct residence
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion
	@tparam bool isFort

	@usage
	function onGeneralAssaultsResidence(namedChar, residence, faction, targetFaction, region, characterType, religion, targetReligion, isFort)
	--something
	end
	*/

	/***
	An assault has taken place.

	@function onGeneralAssaultsGeneral
	@tparam namedCharacter namedChar
	@tparam namedCharacter targetNamedChar
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string targetCharacterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralAssaultsGeneral(namedChar, targetNamedChar, faction, targetFaction, region, characterType, targetCharacterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A general on crusade/jihad has been attacked by other character (it includes crusading generals attacked in a residence or on navy and generals attacked by spotted and killed assassin).

	@function onCharacterAttacksCrusadingGeneral
	@tparam namedCharacter namedChar
	@tparam namedCharacter targetNamedChar
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string targetCharacterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onCharacterAttacksCrusadingGeneral(namedChar, targetNamedChar, faction, targetFaction, region, characterType, targetCharacterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A General of a captor faction has made a ransom decision.

	@function onGeneralPrisonersRansomedCaptor
	@tparam namedCharacter namedChar
	@tparam capturedFactionInfo capInfo
	@tparam string ransomType
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralPrisonersRansomedCaptor(character, capInfo, ransomType, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A General of a captive faction has made a ransom decision.

	@function onGeneralPrisonersRansomedCaptive
	@tparam namedCharacter namedChar
	@tparam capturedFactionInfo capInfo
	@tparam string ransomType
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onGeneralPrisonersRansomedCaptive(character, capInfo, ransomType, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A captor faction has made a ransom decision.

	@function onFactionLeaderPrisonersRansomedCaptor
	@tparam namedCharacter namedChar
	@tparam capturedFactionInfo capInfo
	@tparam string ransomType
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onFactionLeaderPrisonersRansomedCaptor(character, capInfo, ransomType, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A captive faction has made a ransom decision.

	@function onFactionLeaderPrisonersRansomedCaptive
	@tparam namedCharacter namedChar
	@tparam capturedFactionInfo capInfo
	@tparam string ransomType
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onFactionLeaderPrisonersRansomedCaptive(character, capInfo, ransomType, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A spy mission has completed. May also export fort or settlement if target was a garrison residence.

	@function onSpyMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam int missionProbability
	@tparam settlementStruct|fortStruct|nil targetResidence
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onSpyMission(namedChar, missionSuccess, missionProbability, targetResidence, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	An assassination mission has completed.

	@function onAssassinationMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam int missionProbability
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onAssassinationMission(namedChar, missionSuccess, missionProbability, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	An acquisition mission has completed.

	@function onAcquisitionMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam int missionProbability
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onAcquisitionMission(namedChar, missionSuccess, missionProbability, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A marriage mission has completed.

	@function onMarriageMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam int missionProbability
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onMarriageMission(namedChar, missionSuccess, missionProbability, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A denouncement mission has completed.

	@function onDenouncementMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam int missionProbability
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onDenouncementMission(namedChar, missionSuccess, missionProbability, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A sabotage mission has completed.

	@function onSabotageMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam int missionProbability
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onSabotageMission(namedChar, missionSuccess, missionProbability, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A bribery mission has completed.

	@function onBriberyMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onBriberyMission(namedChar, missionSuccess, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A bribery mission has completed.

	@function onDiplomacyMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onDiplomacyMission(namedChar, missionSuccess, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A Faction leader has ordered a spying mission.

	@function onLeaderOrderedSpyingMission
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam settlementStruct|fortStruct|nil targetResidence
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onLeaderOrderedSpyingMission(namedChar, missionSuccess, targetResidence, faction, targetFaction, region, characterType, religion, targetReligion)
	--something
	end
	*/

	/***
	A Faction leader has ordered an assassination mission.

	@function onLeaderOrderedAssassination
	@tparam namedCharacter namedChar
	@tparam string missionSuccess
	@tparam namedCharacter targetCharacter
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderOrderedAssassination(namedChar, missionSuccess, targetCharacter, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A Faction leader has ordered a bribery mission.

	@function onLeaderOrderedBribery
	@tparam namedCharacter namedChar
	@tparam namedCharacter targetCharacter
	@tparam factionStruct faction
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onLeaderOrderedBribery(namedChar, missionSuccess, targetCharacter, faction, region, characterType, religion)
	--something
	end
	*/

	/***
	A settlement is being processed for the start of its faction's turn.

	@function onSettlementTurnStart
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementTurnStart(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	A settlement is no longer garrisoned.

	@function onUngarrisonedSettlement
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onUngarrisonedSettlement(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	A settlement has been upgraded.

	@function onSettlementUpgraded
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementUpgraded(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	A settlement has been converted.

	@function onSettlementConverted
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementConverted(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	Siege equipment has been completed by one of the besieging armies.

	@function onSiegeEquipmentCompleted
	@tparam settlementStruct|fortStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSiegeEquipmentCompleted(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	A Settlement is being processed for the end of its faction's turn.

	@function onSettlementTurnEnd
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementTurnEnd(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	The player has selected a settlement.

	@function onSettlementSelected
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementSelected(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	The player has opened the panel for the selected settlement.

	@function onSettlementPanelOpen
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementPanelOpen(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	The player has opened a recruitment panel.

	@function onRecruitmentPanelOpen
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onRecruitmentPanelOpen(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	The player has opened a construction panel.

	@function onConstructionPanelOpen
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onConstructionPanelOpen(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	The player has opened a trade panel.

	@function onTradePanelOpen
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onTradePanelOpen(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	The player has requested advice on the settlement scroll.

	@function onSettlementScrollAdviceRequested
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onSettlementScrollAdviceRequested(settlement, faction, region, religion)
	--something
	end
	*/

	/***
	A guild has been created/upgraded.

	@function onGuildUpgraded
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam guild guild

	@usage
	function onGuildUpgraded(settlement, faction, region, religion, guild)
	--something
	end
	*/

	/***
	A guild has been destroyed.

	@function onGuildDestroyed
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam guild guild

	@usage
	function onGuildDestroyed(settlement, faction, region, religion, guild)
	--something
	end
	*/

	/***
	A settlement has been captured and occupied.

	@function onOccupySettlement
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onOccupySettlement(namedChar, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has been captured and sacked.

	@function onSackSettlement
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onSackSettlement(namedChar, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has been captured and some of its population has been decimated.

	@function onExterminatePopulation
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onExterminatePopulation(namedChar, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has rioted.

	@function onCityRiots
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onCityRiots(settlement, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has been given to another faction.

	@function onGiveSettlement
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onGiveSettlement(settlement, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has rebelled.

	@function onCityRebels
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onCityRebels(settlement, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has been razed.

	@function onCitySacked
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onCitySacked(settlement, faction, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has rioted.

	@function onGovernorCityRiots
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onGovernorCityRiots(namedChar, faction, settlement, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A settlement has rebelled.

	@function onGovernorCityRebels
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam regionStruct region
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onGovernorCityRebels(namedChar, faction, settlement, region, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	The player has abandoned a show me scipt.

	@function onAbandonShowMe

	@usage
	function onAbandonShowMe()
	--something here
	end
	*/

	/***
	A strat map game has been reloaded.

	@function onGameReloaded

	@usage
	function onGameReloaded()
	--something here
	end
	*/

	/***
	The plaza is being captured.

	@function onBattleWinningPlaza

	@usage
	function onBattleWinningPlaza()
	--something here
	end
	*/

	/***
	The plaza capture has been stopped.

	@function onBattleStopsWinningPlaza

	@usage
	function onBattleStopsWinningPlaza()
	--something here
	end
	*/

	/***
	The enemy will have captured the plaza in 30s.

	@function onBattleDominatingPlaza

	@usage
	function onBattleDominatingPlaza()
	--something here
	end
	*/

	/***
	A siege engine is now unmanned.

	@function onBattngineUnmanned

	@usage
	function onBattngineUnmanned()
	--something here
	end
	*/

	/***
	Half of the player's army has been destroyed.

	@function onBattlePlayerArmyHalfDestroyed

	@usage
	function onBattlePlayerArmyHalfDestroyed()
	--something here
	end
	*/

	/***
	Half of the enemy's army has been destroyed.

	@function onBattnemyArmyHalfDestroyed

	@usage
	function onBattnemyArmyHalfDestroyed()
	--something here
	end
	*/

	/***
	The battle has finished.

	@function onBattleFinished

	@usage
	function onBattleFinished()
	--something here
	end
	*/

	/***
	Half of an army has been destroyed.

	@function onBattleArmyHalfDestroyed

	@usage
	function onBattleArmyHalfDestroyed()
	--something here
	end
	*/

	/***
	The escape key has been pressed. This trigger will only fire if the command StealEscKey has been used.

	@function onEscPressed

	@usage
	function onEscPressed()
	--something here
	end
	*/

	/***
	The escape key has been pressed. This trigger will only fire if the command StealEscKey has been used.

	@function onScriptedAdvice

	@usage
	function onScriptedAdvice()
	--something here
	end
	*/

	/***
	The player has requested advice on the naval prebattle scroll.

	@function onNavalPreBattleScrollAdviceRequested

	@usage
	function onNavalPreBattleScrollAdviceRequested()
	--something here
	end
	*/

	/***
	The player has requested advice on the prebattle scroll.

	@function onPreBattleScrollAdviceRequested

	@usage
	function onPreBattleScrollAdviceRequested()
	--something here
	end
	*/

	/***
	The player has opened the college of cardinals panel.

	@function onCollegeOfCardinalsPanelOpen

	@usage
	function onCollegeOfCardinalsPanelOpen()
	--something here
	end
	*/

	/***
	The player has opened the diplomatic standing panel.

	@function onDiplomaticStandingPanelOpen

	@usage
	function onDiplomaticStandingPanelOpen()
	--something here
	end
	*/

	/***
	An idle unit is under missile fire.

	@function onBattlePlayerUnderAttackIdle
	@tparam stackStruct army
	@tparam unit playerUnit

	@usage
	function onBattlePlayerUnderAttackIdle(army, playerUnit)
	--something here
	end
	*/

	/***
	A team has gained the advantage in combat.

	@function onBattleWinningCombat
	@tparam stackStruct army

	@usage
	function onBattleWinningCombat(army)
	--something here
	end
	*/

	/***
	The whole army is tired.

	@function onBattleArmyTired
	@tparam stackStruct army

	@usage
	function onBattleArmyTired(army)
	--something here
	end
	*/

	/***
	A different team is now the strongest.

	@function onBattleTideofBattle

	@usage
	function onBattleTideofBattle()
	--something here
	end
	*/

	/***
	A unit has gone berserk.

	@function onBattleUnitGoesBerserk
	@tparam unit unit

	@usage
	function onBattleUnitGoesBerserk(unit)
	--something here
	end
	*/

	/***
	A siege engine has been destroyed.

	@function onBattleSiegeEngineDestroyed
	@tparam unit unit

	@usage
	function onBattleSiegeEngineDestroyed(unit)
	--something here
	end
	*/

	/***
	A siege engine has docked with a wall.

	@function onBattleSiegeEngineDocksWall
	@tparam unit unit

	@usage
	function onBattleSiegeEngineDocksWall(unit)
	--something here
	end
	*/

	/***
	An engine has started attacking a gate.

	@function onBattleGatesAttackedByEngine
	@tparam unit unit

	@usage
	function onBattleGatesAttackedByEngine(unit)
	--something here
	end
	*/

	/***
	An engine has destroyed a gate.

	@function onBattleGatesDestroyedByEngine
	@tparam unit unit

	@usage
	function onBattleGatesDestroyedByEngine(unit)
	--something here
	end
	*/

	/***
	A siege engine has knocked down a wall.

	@function onBattleWallsBreachedByEngine
	@tparam unit unit

	@usage
	function onBattleWallsBreachedByEngine(unit)
	--something here
	end
	*/

	/***
	A wall has been captured.

	@function onBattleWallsCaptured
	@tparam unit unit

	@usage
	function onBattleWallsCaptured(unit)
	--something here
	end
	*/

	/***
	A unit has routed.

	@function onBattleUnitRouts
	@tparam unit unit

	@usage
	function onBattleUnitRouts(unit)
	--something here
	end
	*/

	/***
	A unit has been disbanded.

	@function onUnitDisbanded
	@tparam factionStruct faction
	@tparam unit unit
	@tparam eduEntry entry
	@tparam string religion

	@usage
	function onUnitDisbanded(faction, unit, entry, religion)
	--something here
	end
	*/

	/***
	A unit has been trained.

	@function onUnitTrained
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam unit unit
	@tparam eduEntry entry
	@tparam string religion

	@usage
	function onUnitTrained(faction, settlement, unit, entry, religion)
	--something here
	end
	*/

	/***
	A unit has been trained.

	@function onGovernorUnitTrained
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam unit unit
	@tparam eduEntry entry
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGovernorUnitTrained(namedChar, faction, settlement, unit, entry, region, characterType, religion)
	--something here
	end
	*/

	/***
	A building has been completed.

	@function onGovernorBuildingCompleted
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam building building
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGovernorBuildingCompleted(namedChar, faction, settlement, building, region, characterType, religion)
	--something here
	end
	*/

	/***
	An agent has been trained.

	@function onAgentCreated
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onAgentCreated(namedChar, faction, settlement, region, characterType, religion)
	--something here
	end
	*/

	/***
	An agent has been trained.

	@function onGovernorAgentCreated
	@tparam namedCharacter namedChar
	@tparam factionStruct faction
	@tparam settlementStruct settlement
	@tparam regionStruct|nil region
	@tparam string characterType
	@tparam string religion

	@usage
	function onGovernorAgentCreated(namedChar, faction, settlement, building, region, characterType, religion)
	--something here
	end
	*/

	/***
	A building has been destroyed.

	@function onBuildingDestroyed
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam string buildingName
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onBuildingDestroyed(settlement, faction, buildingName, region, religion)
	--something here
	end
	*/

	/***
	A building has been added to the construction queue.

	@function onAddedToBuildingQueue
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam string buildingName
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onAddedToBuildingQueue(settlement, faction, buildingName, region, religion)
	--something here
	end
	*/

	/***
	A building has been completed.

	@function onBuildingCompleted
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam building building
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onBuildingCompleted(settlement, faction, buildingName, region, religion)
	--something here
	end
	*/

	/***
	The player has requested building advice.

	@function onRequestBuildingAdvice
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam string buildingName
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onRequestBuildingAdvice(settlement, faction, buildingName, region, religion)
	--something here
	end
	*/

	/***
	The player has requested training advice.

	@function onRequestTrainingAdvice
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam string resource
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onRequestTrainingAdvice(settlement, faction, resource, region, religion)
	--something here
	end
	*/

	/***
	A unit has been added to the training queue.

	@function onAddedToTrainingQueue
	@tparam settlementStruct settlement
	@tparam factionStruct faction
	@tparam string unitName
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onAddedToTrainingQueue(settlement, faction, unitName, region, religion)
	--something here
	end
	*/

	/***
	An army has been entirely routed.

	@function onBattleArmyRouted
	@tparam stackStruct army

	@usage
	function onBattleArmyRouted(army)
	--something here
	end
	*/

	/***
	A reinforcing army has arrived on the battlefield.

	@function onBattleReinforcementsArrive
	@tparam stackStruct army

	@usage
	function onBattleReinforcementsArrive(army)
	--something here
	end
	*/

	/***
	The player has requested mercenaries advice.

	@function onRequestMercenariesAdvice
	@tparam stackStruct army

	@usage
	function onRequestMercenariesAdvice(army)
	--something here
	end
	*/

	/***
	The player has clicked on a button.

	@function onButtonPressed
	@tparam string name

	@usage
	function onButtonPressed(name)
	--something here
	end
	*/

	/***
	The player triggered a keyboard shortcut.

	@function onShortcutTriggered
	@tparam string name

	@usage
	function onShortcutTriggered(name)
	--something here
	end
	*/

	/***
	A special UI Element is visible.

	@function onUIElementVisible
	@tparam string name

	@usage
	function onUIElementVisible(name)
	--something here
	end
	*/

	/***
	The player opened a scroll.

	@function onScrollOpened
	@tparam string name

	@usage
	function onScrollOpened(name)
	--something here
	end
	*/

	/***
	The player closed a scroll.

	@function onScrollClosed
	@tparam string name

	@usage
	function onScrollClosed(name)
	--something here
	end
	*/

	/***
	The player has requested advice on a scroll.

	@function onScrollAdviceRequested
	@tparam string name

	@usage
	function onScrollAdviceRequested(name)
	--something here
	end
	*/

	/***
	The player has suppressed a piece of advice.

	@function onAdviceSupressed
	@tparam string name

	@usage
	function onAdviceSupressed(name)
	--something here
	end
	*/

	/***
	A general has been routed.

	@function onBattleGeneralRouted
	@tparam character general
	@tparam namedCharacter namedChar

	@usage
	function onBattleGeneralRouted(general, namedChar)
	--something here
	end
	*/

	/***
	A general has been killed.

	@function onBattleGeneralKilled
	@tparam character general
	@tparam namedCharacter namedChar

	@usage
	function onBattleGeneralKilled(general, namedChar)
	--something here
	end
	*/

	/***
	A Crusade/Jihad has been called.

	@function onCrusadeCalled
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string targetReligion

	@usage
	function onCrusadeCalled(targetSettlement, targetFaction, targetRegion, targetReligion)
	--something here
	end
	*/

	/***
	The Pope has accepted the player's crusade target.

	@function onPopeAcceptsCrusadeTarget
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string targetReligion

	@usage
	function onPopeAcceptsCrusadeTarget(targetSettlement, targetFaction, targetRegion, targetReligion)
	--something here
	end
	*/

	/***
	The Pope has rejected the player's crusade target.

	@function onPopeRejectsCrusadeTarget
	@tparam settlementStruct targetSettlement
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string targetReligion

	@usage
	function onPopeRejectsCrusadeTarget(targetSettlement, targetFaction, targetRegion, targetReligion)
	--something here
	end
	*/

	/***
	A Crusade/Jihad has ended.

	@function onCrusadeEnds
	@tparam settlementStruct targetSettlement
	@tparam regionStruct targetRegion

	@usage
	function onCrusadeEnds(targetSettlement, targetRegion)
	--something here
	end
	*/

	/***
	Called before the faction's turn starts.

	@function onPreFactionTurnStart
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onPreFactionTurnStart(faction, religion)
	--something here
	end
	*/

	/***
	Called at a faction's turn start.

	@function onFactionTurnStart
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFactionTurnStart(faction, religion)
	--something here
	end
	*/

	/***
	Called after faction changes to a new capital.

	@function onFactionNewCapital
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFactionNewCapital(faction, religion)
	newCapital=fac.capital();
	end
	*/

	/***
	Called at a faction's turn end.

	@function onFactionTurnEnd
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFactionTurnEnd(faction, religion)
	--something here
	end
	*/

	/***
	A Faction has voted for the new pope.

	@function onVotedForPope
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onVotedForPope(faction, religion)
	--something here
	end
	*/

	/***
	A Faction has been excommunicated.

	@function onFactionExcommunicated
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFactionExcommunicated(faction, religion)
	--something here
	end
	*/

	/***
	A Faction has formed a horde.

	@function onHordeFormed
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onHordeFormed(faction, religion)
	--something here
	end
	*/

	/***
	A cardinal has been removed from the game.

	@function onCardinalRemoved
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onCardinalRemoved(faction, religion)
	--something here
	end
	*/

	/***
	An inquisitor has been dispatched to a region.

	@function onInquisitorAppointed
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onInquisitorAppointed(faction, religion)
	--something here
	end
	*/

	/***
	An assassination mission against the pope has failed and the assassin is executed.

	@function onAssassinCaughtAttackingPope
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onAssassinCaughtAttackingPope(faction, religion)
	--something here
	end
	*/

	/***
	The player has opened his finances panel.

	@function onFinancesPanelOpen
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFinancesPanelOpen(faction, religion)
	--something here
	end
	*/

	/***
	The player has opened the faction summary panel.

	@function onFactionSummaryPanelOpen
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFactionSummaryPanelOpen(faction, religion)
	--something here
	end
	*/

	/***
	The player has opened the family tree panel.

	@function onFamilyTreePanelOpen
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onFamilyTreePanelOpen(faction, religion)
	--something here
	end
	*/

	/***
	The player has opened the diplomatic standing panel.

	@function onDiplomacyPanelOpen
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onDiplomacyPanelOpen(faction, religion)
	--something here
	end
	*/

	/***
	The player has attacked an army or settlement. The decision panel is now open.

	@function onPreBattlePanelOpen
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onPreBattlePanelOpen(faction, religion)
	--something here
	end
	*/

	/***
	A message has arrived for the player.

	@function onIncomingMessage
	@tparam factionStruct faction
	@tparam string religion
	@tparam int eventid

	@usage
	function onIncomingMessage(faction, religion, eventid)
	--something here
	end
	*/

	/***
	The player has opened a message.

	@function onMessageOpen
	@tparam factionStruct faction
	@tparam string religion
	@tparam int eventid

	@usage
	function onMessageOpen(faction, religion, eventid)
	--something here
	end
	*/

	/***
	The player has closed a message.

	@function onMessageClosed
	@tparam int eventid

	@usage
	function onMessageClosed(eventid)
	--something here
	end
	*/

	/***
	The player has declined automated settlement management.

	@function onDeclineAutomatedSettlementManagement
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onDeclineAutomatedSettlementManagement(faction, religion)
	--something here
	end
	*/

	/***
	The battle AI has begun processing.

	@function onBattleAiCommenced
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onBattleAiCommenced(faction, religion)
	--something here
	end
	*/

	/***
	The delay phase has begun.

	@function onBattleDelayPhaseCommenced
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onBattleDelayPhaseCommenced(faction, religion)
	--something here
	end
	*/

	/***
	The deployment phase has begun.

	@function onBattleDeploymentPhaseCommenced
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onBattleDeploymentPhaseCommenced(faction, religion)
	--something here
	end
	*/

	/***
	The conflict phase has begun.

	@function onBattleConflictPhaseCommenced
	@tparam factionStruct faction
	@tparam string religion

	@usage
	function onBattleConflictPhaseCommenced(faction, religion)
	--something here
	end
	*/

	/***
	Called after a faction declares a war.

	@function onFactionWarDeclared
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onFactionWarDeclared(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	Called after a faction declares an alliance.

	@function onFactionAllianceDeclared
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onFactionAllianceDeclared(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A marriage has occured between two factions. gets fired for both factions involved.

	@function onInterFactionMarriage
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onInterFactionMarriage(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	Called after a faction makes a trade agreement.

	@function onFactionTradeAgreementMade
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onFactionTradeAgreementMade(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A new pope has been elected.

	@function onPopeElected
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onPopeElected(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	Called after a faction breaks an alliance.

	@function onFactionBreakAlliance
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onFactionBreakAlliance(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A faction to faction attitude update has occurred (once every round).

	@function onUpdateAttitude
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion

	@usage
	function onUpdateAttitude(faction, religion, targetFaction, targetReligion)
	--something
	end
	*/

	/***
	A demeanour response has occured in diplomacy talks.

	@function onDemeanour
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion
	@tparam float amountChange

	@usage
	function onDemeanour(faction, religion, targetFaction, targetReligion, amountChange)
	--something
	end
	*/

	/***
	Called after a faction gives money to another faction.

	@function onGiveMoney
	@tparam factionStruct faction
	@tparam string religion
	@tparam factionStruct targetFaction
	@tparam string targetReligion
	@tparam int amount

	@usage
	function onGiveMoney(faction, religion, targetFaction, targetReligion, amount)
	--something
	end
	*/

	/***
	A player unit has attacked one of the enemies units.

	@function onBattlePlayerUnitAttacksEnemyUnit
	@tparam unit attackingUnit
	@tparam unit defendingUnit
	@tparam unit playerUnit
	@tparam unit enemyUnit

	@usage
	function onBattlePlayerUnitAttacksEnemyUnit(attackingUnit, defendingUnit, playerUnit, enemyUnit)
	--something here
	end
	*/

	/***
	An enemy unit has attacked one of the players units.

	@function onBattleEnemyUnitAttacksPlayerUnit
	@tparam unit attackingUnit
	@tparam unit defendingUnit
	@tparam unit playerUnit
	@tparam unit enemyUnit

	@usage
	function onBattleEnemyUnitAttacksPlayerUnit(attackingUnit, defendingUnit, playerUnit, enemyUnit)
	--something here
	end
	*/

	/***
	One of the player's units has gone berserk.

	@function onBattlePlayerUnitGoesBerserk
	@tparam unit playerUnit

	@usage
	function onBattlePlayerUnitGoesBerserk(playerUnit)
	--something here
	end
	*/

	/***
	One of the player's units has routed.

	@function onBattlePlayerUnitRouts
	@tparam unit playerUnit

	@usage
	function onBattlePlayerUnitRouts(playerUnit)
	--something here
	end
	*/

	/***
	A siege engine belonging to the player has been destroyed.

	@function onBattlePlayerSiegeEngineDestroyed
	@tparam unit playerUnit

	@usage
	function onBattlePlayerSiegeEngineDestroyed(playerUnit)
	--something here
	end
	*/

	/***
	A player's engine has started attacking a gate.

	@function onBattleGatesAttackedByPlayerEngine
	@tparam unit playerUnit

	@usage
	function onBattleGatesAttackedByPlayerEngine(playerUnit)
	--something here
	end
	*/

	/***
	One of the enemy's units has gone berserk.

	@function onBattleEnemyUnitGoesBerserk
	@tparam unit enemyUnit

	@usage
	function onBattleEnemyUnitGoesBerserk(enemyUnit)
	--something here
	end
	*/

	/***
	One of the enemy's units has routed.

	@function onBattnemyUnitRouts
	@tparam unit enemyUnit

	@usage
	function onBattnemyUnitRouts(enemyUnit)
	--something here
	end
	*/

	/***
	A siege engine belonging to the enemy has been destroyed.

	@function onBattnemySiegeEngineDestroyed
	@tparam unit enemyUnit

	@usage
	function onBattnemySiegeEngineDestroyed(enemyUnit)
	--something here
	end
	*/

	/***
	An enemy's engine has started attacking a gate.

	@function onBattleGatesAttackedByEnemyEngine
	@tparam unit enemyUnit

	@usage
	function onBattleGatesAttackedByEnemyEngine(enemyUnit)
	--something here
	end
	*/

	/***
	When a particular disaster has just happened.

	@function onDisaster
	@tparam string eventType (earthquake, flood, storm, volcano, plague)

	@usage
	function onDisaster(eventType)
		if eventType == "earthquake" then
			--something here
		end
	end
	*/

	/***
	An event counter has changed it's value.

	@function onEventCounter
	@tparam string counter

	@usage
	function onEventCounter(counter)
		if counter == "my_event_counter" then
			--something here
		end
	end
	*/

	/***
	The last unit has been removed from a fort, agents do not count.

	@function onUngarrisonedFort
	@tparam fortStruct fort
	@tparam factionStruct faction
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onUngarrisonedFort(fort, faction, region, religion)
		--something here
	end
	*/

	/***
	An object of the target faction has been seen by the faction.

	@function onObjSeen
	@tparam factionStruct fac Faction that saw the object.
	@tparam factionStruct targetFac Faction that owns the object.
	@tparam int xCoord
	@tparam int yCoord

	@usage
	function onObjSeen(faction, targetFaction, x, y, region, religion)
		--something here
	end
	*/

	/***
	The tile has been seen by the faction.

	@function onTileSeen
	@tparam factionStruct faction
	@tparam int xCoord
	@tparam int yCoord
	@tparam regionStruct region
	@tparam string religion

	@usage
	function onTileSeen(faction, x, y, region, religion)
		--something here
	end
	*/

	/***
	A faction to faction transgression has occurred.

	@function onTransgression
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam string transgression (TC_THREATEN_WAR, TC_DECLARED_WAR, TC_MINOR_ASSASSINATION_ATTEMPT, TC_BROKE_TREATY_TERMS, TC_BROKE_ALLIANCE, TC_INVASION)
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onTransgression(faction, targetFaction, transgression, religion, targetReligion)
		--something here
	end
	*/

	/***
	A faction to faction forgiveness has occurred.

	@function onForgiveness
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam string forgiveness (not sure what this could be)
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onForgiveness(faction, targetFaction, forgiveness, religion, targetReligion)
		--something here
	end
	*/

	/***
	An army has taken a crusade or jihad target settlement.

	@function onArmyTakesCrusadeTarget
	@tparam settlementStruct targetSettlement
	@tparam stackStruct army
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onArmyTakesCrusadeTarget(targetSettlement, army, faction, targetFaction, targetRegion, religion, targetReligion)
		--something here
	end
	*/

	/***
	Units have deserted a crusade or jihad.

	@function onUnitsDesertCrusade
	@tparam settlementStruct targetSettlement
	@tparam factionStruct faction
	@tparam factionStruct targetFaction
	@tparam regionStruct targetRegion
	@tparam string religion
	@tparam string targetReligion

	@usage
	function onUnitsDesertCrusade(targetSettlement, faction, targetFaction, targetRegion, religion, targetReligion)
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
	Called when a new campaign is started from the menu.

	@function onNewGameStart

	@usage
	function onNewGameStart()
	--something here
	end
	*/



	onNewGameStart = new sol::function(luaState["onNewGameStart"]);
	checkLuaFunc(&onNewGameStart);

	/***
	Called after the game loads various db`s (edu, etc) at startup.

	@function onReadGameDbsAtStart

	@usage
	function onReadGameDbsAtStart()
	--something here
	end
	*/



	onReadGameDbsAtStart = new sol::function(luaState["onReadGameDbsAtStart"]);
	checkLuaFunc(&onReadGameDbsAtStart);

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
	Called after loading the campaign map

	@function onCampaignMapLoaded

	@usage
	function onCampaignMapLoaded()
	--something here
	end
	*/



	onCampaignMapLoaded = new sol::function(luaState["onCampaignMapLoaded"]);
	checkLuaFunc(&onCampaignMapLoaded);

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

	@function onfortificationlevelS
	@tparam settlementStruct siegedSettlement
	@treturn int overridedFortificationlevel
	@treturn bool isCastle override settlement type (siege equipment is slightly different between cities and castles of the same level)

	@usage
	function onfortificationlevelS(settlement)
		if settlement.xCoord == 10 and settlement.yCoord == 25 then
			return 3, false --override settlement under siege at these coordinates to level 3 city
		end
		return nil --else, do not override
	end
	*/
	onfortificationlevelS = new sol::function(luaState["onfortificationlevelS"]);
	checkLuaFunc(&onfortificationlevelS);

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


	if (onPluginLoad != nullptr)
	{
		tryLua((*onPluginLoad)());
	}
}