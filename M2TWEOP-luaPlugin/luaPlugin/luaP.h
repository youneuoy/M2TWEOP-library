#pragma once
#ifndef luaH_DEF
#define luaH_DEF
#pragma comment(lib,"lua/lua5.1.lib")
#pragma comment(lib,"lua/luajit.lib")


#define SOL_SAFE_FUNCTIONS 1
#define SOL_ALL_SAFETIES_ON  1
#define SOL_LUAJIT 1
#include "lua/sol.hpp"


#include <string>
#include "realGameTypes.h"
#include <windows.h>
#include "luaGetSetFuncs.h"
#include "factionHelpers.h"
#include "settlementHelpers.h"
#include "resourcesHelpers.h"
#include "objectsHelpers.h"
#include "gameHelpers.h"
#include "stackStructHelpers.h"
#include "buildingStructHelpers.h"
#include "generalHelpers.h"
#include "unitHelpers.h"
#include "cameraHelpers.h"
#include "m2tweopHelpers.h"
#include "generalCharactericticsHelpers.h"
#include "technicalHelpers.h"
#include "eopEduHelpers.h"
#include "guildHelpers.h"
#include "eopFbxHelpers.h"
#include "siegeHelpers.h"
#include "gameSTDUIHelpers.h"

#define tryLua(luaFunc)  \
auto funcResult = luaFunc;\
if (!funcResult.valid())\
{\
	sol::error luaError = funcResult;\
	MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);\
	if (plugData::data.luaAll.checkVar("terminateAtLuaException", 1) == true)\
	{\
		terminate();\
	}\
}\


#define tryLuaGetRes(luaFunc,result)  \
auto funcResult = luaFunc;\
if (!funcResult.valid())\
{\
	sol::error luaError = funcResult;\
	MessageBoxA(NULL, luaError.what(), "Lua exception!", NULL);\
	if (plugData::data.luaAll.checkVar("terminateAtLuaException", 1) == true)\
	{\
		terminate();\
	}\
}\
else\
{\
result=funcResult;	\
}\



class luaP
{
public:
	static std::vector<std::string> logS;
	static std::vector<std::string> logCommands;
	std::string luaPath;
	bool checkVar(const char* gName, int variable);

	//create lua instance, init some basic tables
	sol::state* init(std::string& luaFilePath, std::string& modPath);
	//init part 2 of tables
	void initP2();
	void initCampaign();
	void initTech();
	void initEopEdu();
	void initEopFbx();
	void initEop3dObjects();
	void initEopSounds();
	void runScriptS(std::string* script);

	//lua functions and events controllers
	void onPluginLoadF();
	void onChangeTurnNum(int num);
	void onCharacterSelected(generalCharacterictics* gen);
	void onPreFactionTurnStart(factionStruct* fac);
	void onFactionTurnStart(factionStruct* fac);
	void onGeneralDevastatesTile(generalCharacterictics* gen);
	void onFactionTurnEnd(factionStruct* fac);
	void onFactionNewCapital(factionStruct* fac);
	void onFactionWarDeclared(factionStruct* fac, factionStruct* fac2);
	void onFactionAllianceDeclared(factionStruct* fac, factionStruct* fac2);
	void onFactionTradeAgreementMade(factionStruct* fac, factionStruct* fac2);
	void onFactionBreakAlliance(factionStruct* fac, factionStruct* fac2);

	void onGiveMoney(factionStruct* fac, factionStruct* fac2, float amount);
	void onUpdateAttitude(factionStruct* fac, factionStruct* fac2);
	void onDemeanour(factionStruct* fac, factionStruct* fac2, float amount);

	void onGeneralAssaultsGeneral(generalCharacterictics* gen, generalCharacterictics* gen2);
	void onGeneralAssaultsResidence(generalCharacterictics* gen, settlementStruct* sett, fortStruct* frt);
	void onGeneralCaptureSettlement(generalCharacterictics* gen, settlementStruct* sett);

	void onGeneralCaptureResidence(generalCharacterictics* gen);
	void onSiegeEquipmentCompleted(settlementStruct* sett, fortStruct* frt);
	void onPostBattle(generalCharacterictics* gen);



	void onMultiTurnMove(generalCharacterictics* gen);
	void onSettlementSelected(settlementStruct* sett);
	void onSettlementUpgraded(settlementStruct* sett);
	void onSettlementConverted(settlementStruct* sett);

	sol::state luaState;

	/*
	struct
	{
		sol::table M2TWEOPTable;
		sol::table M2TWEOPEDUTable;
		sol::table M2TWEOPFBXTable;


		sol::table stratmapTable;
		//this inside stratmap table
		sol::table objectsTable;
		sol::table cameraTable;
		sol::table gameTable;


	}tables;
	struct
	{
		//global game table
		sol::usertype<gameDataAllStruct> gameDataAllTable;
		//this inside gameDataAll table
		sol::usertype<battleDataS> battleTable;
		sol::usertype<battleSide> battleSideTable;
		sol::usertype<trackedPointerArmy> trackedPointerArmyTable;
		sol::usertype<deploymentAreaS> deploymentAreaTable;

		sol::usertype<campaign> campaignTable;
	}typeAll;
	struct
	{
		sol::table uniString;
	}techTypes;
	struct
	{
		sol::usertype<unit>unit;
		sol::usertype<general>character;
		sol::usertype<generalCharacterictics>namedCharacter;
		sol::usertype<anchillary>ancillary;
		sol::usertype<traitContainer>traitContainerT;
		sol::usertype<EduEntry>EduEntry;
		sol::usertype<factionStruct>factionStruct;
		sol::usertype<fortStruct>fortStruct;
		sol::usertype<portBuildingStruct>portBuildingStruct;
		sol::usertype<settlementStruct>settlementStruct;
		sol::usertype<guild>guild;
		sol::usertype<resStrat>resStrat;
		sol::usertype<stackStruct>stackStruct;
		sol::usertype<building>building;
		sol::usertype<siegeS>siege;
		sol::usertype<edbEntry>edbEntry;
		sol::usertype<buildingLevel>buildingLevel;

	}types;
	*/


	sol::function* onCampaignMapLoaded = nullptr;

	sol::function* onReadGameDbsAtStart = nullptr;
	sol::function* onEndSiege = nullptr;
	sol::function* onStartSiege = nullptr;
	sol::function* onSelectWorldpkgdesc = nullptr;
	sol::function* onfortificationlevelS = nullptr;
	sol::function* onClickAtTile = nullptr;
	sol::function* onNewGameStart = nullptr;
	sol::function* onPluginLoad = nullptr;
	sol::function* onCreateSaveFile = nullptr;
	sol::function* onLoadSaveFile = nullptr;

	sol::function* drawLuaFunc = nullptr;
	sol::function* resetDXFunc = nullptr;
	sol::function* onLoadingFonts = nullptr;
	sol::function* initDXFunc = nullptr;


	sol::function* onChangeTurnNumFunc = nullptr;
	sol::function* onCharacterSelectedFunc = nullptr;
	sol::function* onPreFactionTurnStartFunc = nullptr;
	sol::function* onGeneralDevastatesTileFunc = nullptr;
	sol::function* onFactionTurnStartFunc = nullptr;
	sol::function* onFactionTurnEndFunc = nullptr;
	sol::function* onFactionNewCapitalFunc = nullptr;
	sol::function* onFactionWarDeclaredFunc = nullptr;
	sol::function* onFactionAllianceDeclaredFunc = nullptr;
	sol::function* onFactionTradeAgreementMadeFunc = nullptr;
	sol::function* onFactionBreakAllianceFunc = nullptr;

	sol::function* onGiveMoneyFunc = nullptr;
	sol::function* onUpdateAttitudeFunc = nullptr;
	sol::function* onDemeanourFunc = nullptr;

	sol::function* onGeneralAssaultsGeneralFunc = nullptr;
	sol::function* onGeneralAssaultsResidenceFunc = nullptr;
	sol::function* onGeneralCaptureSettlementFunc = nullptr;

	sol::function* onGeneralCaptureResidenceFunc = nullptr;
	sol::function* onSiegeEquipmentCompletedFunc = nullptr;
	sol::function* onPostBattleFunc = nullptr;


	sol::function* onMultiTurnMoveFunc = nullptr;
	sol::function* onSettlementSelectedFunc = nullptr;
	sol::function* onSettlementUpgradedFunc = nullptr;
	sol::function* onSettlementConvertedFunc = nullptr;
	sol::function* onCityRiotsFunc = nullptr;
	sol::function* onUngarrisonedSettlementFunc = nullptr;

	sol::function* onUngarrisonedFortFunc = nullptr;
	sol::function* onGiveSettlementFunc = nullptr;
	sol::function* onOccupySettlementFunc = nullptr;
	sol::function* onExterminatePopulationFunc = nullptr;

	sol::function* onSackSettlementFunc = nullptr;
	sol::function* onAddedToBuildingQueueFunc = nullptr;
	sol::function* onBuildingDestroyedFunc = nullptr;
	sol::function* onBuildingCompletedFunc = nullptr;
	sol::function* onEventCounterFunc = nullptr;

	sol::function* onFactionExcommunicatedFunc = nullptr;
	sol::function* onDisasterFunc = nullptr;
	sol::function* onHordeFormedFunc = nullptr;
	sol::function* onAddedToTrainingQueueFunc = nullptr;

	sol::function* onUnitDisbandedFunc = nullptr;
	sol::function* onUnitTrainedFunc = nullptr;
	sol::function* onAgentCreatedFunc = nullptr;

	sol::function* onObjSeenFunc = nullptr;
	sol::function* onTileSeenFunc = nullptr;
	sol::function* onGameReloadedFunc = nullptr;
	sol::function* onTransgressionFunc = nullptr;


	sol::function* onPopeAcceptsCrusadeTargetFunc = nullptr;
	sol::function* onCrusadeCalledFunc = nullptr;
	sol::function* onCrusadeEndsFunc = nullptr;
	sol::function* onPopeRejectsCrusadeTargetFunc = nullptr;
	sol::function* onArmyTakesCrusadeTargetFunc = nullptr;

	sol::function* onUnitsDesertCrusadeFunc = nullptr;
	sol::function* onPopeElectedFunc = nullptr;
	sol::function* onVotedForPopeFunc = nullptr;
	sol::function* onAssassinCaughtAttackingPopeFunc = nullptr;
	sol::function* onInquisitorAppointedFunc = nullptr;



	sol::function* onSettlementPanelOpenFunc = nullptr;
	sol::function* onFinancesPanelOpenFunc = nullptr;
	sol::function* onFactionSummaryPanelOpenFunc = nullptr;
	sol::function* onFamilyTreePanelOpenFunc = nullptr;
	sol::function* onDiplomaticStandingPanelOpenFunc = nullptr;
	sol::function* onDiplomacyPanelOpenFunc = nullptr;
	sol::function* onPreBattlePanelOpenFunc = nullptr;
	sol::function* onNavalAutoResolvePanelOpenFunc = nullptr;

	sol::function* onCharacterPanelOpenFunc = nullptr;
	sol::function* onTradePanelOpenFunc = nullptr;
	sol::function* onRequestBuildingAdviceFunc = nullptr;
	sol::function* onRequestTrainingAdviceFunc = nullptr;
	sol::function* onMessageOpenFunc = nullptr;
	sol::function* onIncomingMessageFunc = nullptr;
	sol::function* onMessageClosedFunc = nullptr;

	sol::function* onButtonPressedFunc = nullptr;
	sol::function* onScrollClosedFunc = nullptr;
	sol::function* onScrollOpenedFunc = nullptr;
	sol::function* onUIElementVisibleFunc = nullptr;
	sol::function* onScrollAdviceRequestedFunc = nullptr;
	sol::function* onSettlementScrollAdviceRequestedFunc = nullptr;
	sol::function* onPreBattleScrollAdviceRequestedFunc = nullptr;
	sol::function* onNavalPreBattleScrollAdviceRequestedFunc = nullptr;
	sol::function* onCollegeOfCardinalsPanelOpenFunc = nullptr;

	sol::function* onGuildUpgradedFunc = nullptr;
	sol::function* onGuildDestroyedFunc = nullptr;

	sol::function* onBrotherAdoptedFunc = nullptr;
	sol::function* onBirthFunc = nullptr;
	sol::function* onCharacterComesOfAgeFunc = nullptr;
	sol::function* onCharacterMarriesFunc = nullptr;
	sol::function* onCharacterMarriesPrincessFunc = nullptr;
	sol::function* onCharacterBecomesAFatherFunc = nullptr;

	sol::function* onNewAdmiralCreatedFunc = nullptr;

	sol::function* onShortcutTriggeredFunc = nullptr;

	sol::function* onBecomesFactionLeaderFunc = nullptr;
	sol::function* onBecomesFactionHeirFunc = nullptr;



private:
	void checkLuaFunc(sol::function** lRef);

};



#endif
