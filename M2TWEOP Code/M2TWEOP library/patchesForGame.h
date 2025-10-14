#pragma once
#include "stratModelsChange.h"
#include "graphicsD3D.h"
#include "cultures.h"
#include "faction.h"
#include "gameHelpers.h"
#include "unit.h"

struct aiPersonalityValues;
struct aiLongTermGoalDirector;
struct unitRQ;
struct bannerData;
struct aiRegionData;
struct aiRegionController;
struct settlementStats;
struct settlementStatsManager;
struct aiCampaignController;
struct aiGlobalStrategyDirector;
struct buildingsQueue;
struct buildingInQueue;
struct portBuildingStruct;
struct aiProductionController;
struct regionStruct;
struct aiFaction;
struct buildingLevel;
struct edbEntry;
struct settlementStruct;

struct trackedArmy
{
	DWORD vfTable;
	armyStruct* army;
};

struct generalAssault
{
	DWORD vtbl;
	int8_t finished : 1;
	int8_t started : 1;
	int8_t init : 1;
	int8_t stop : 1;
	character* character;
	int xCoordStart;
	int yCoordStart;
	int xCoordTarget;
	int yCoordTarget;
	settlementStruct* settlement;
	bool nightBattle;
	char pad[3];
};

class patchesForGame
{
public:
	//select worldpkgdesc from db
	static worldRecord* __fastcall selectWorldpkgdesc(char* database, worldRecord* selectedRecord);


	static void __fastcall onLoadSettlementWorldpkgdesc(worldRecord* selectedRecord);
	static int __fastcall onFortificationLevelS(settlementStruct* settlement, bool* isCastle);
	static char* __fastcall onSaveEDUStringS(const eduEntry* eduEntry);
	static void __fastcall initGlobalStrategyDirector(aiGlobalStrategyDirector* gsd);
	static int __fastcall onCreateUnit(char** entryName, const int* eduIndex);
	static void __fastcall onSetArmyGeneralsUnit(armyStruct* army);
	static int __fastcall onCalculateMiningIncome(int value, const settlementStruct* settlement);
	static int __fastcall onCreateMercUnitCheck(char** entryName, int eduIndex);
	static void __fastcall onWriteSoldiersToStrat(unit* unit);
	static int __fastcall onSetRegionHiddenResources(int hiddenResourceId, const mapRegion* region);
	static int __fastcall onReadHiddenResources(int hiddenResourceId, void* textBuffer);
	static int __fastcall onGetHiddenResource(const stringWithHash* name);
	static int __fastcall onCheckHiddenResource(int id, int region);
	static int __cdecl onGetWatchTowerRange();
	static void __fastcall onSetSettlementModel(settlementStruct* settlement);
	static void __fastcall onGeneralAssaultAction(generalAssault* assault);
	static mountedEngine* __fastcall onGetMountedEngine(const stringWithHash* name);
	static bool __fastcall onParseMountedEngines(mountedEngineDb* db, descrParser* parser);
	static bool __fastcall onParseEdu(unitDb* db, descrParser* parser);
	static int __fastcall onAddSettlementToDiplomacyScroll(const settlementStruct* settlement);
	static settlementStruct* __fastcall onDecideMissionTarget(settlementStruct* settlement, factionStruct* faction);
	static int __fastcall onCanWithdrawPreBattle(const settlementStruct* settlement);
	static int __fastcall onCalculateCommand(const characterRecord* general);
	static void __fastcall onRemoveFromUnitQueue(const unitRQ* queue, int index);
	static void __fastcall onLoadModelRigid(model_Rigid* model);
	static void __fastcall onUnloadModels();
	static building* __fastcall onGetBuildingById(const settlementBuildings* buildings, int index);
	static int __fastcall onCheckSettHasBuilding(const settlementBuildings* buildings, int index);
	static void __fastcall getPossibleConstructions(exportDescrBuildings* edb, settlementStruct* sett, void* data, void* caps, void* bonus, bool checkQueue, bool forceTemple);
	static int __fastcall onConflictTest(const buildingsQueue* queue, int index);
	static int __fastcall onOffMapModelThing(int culture);
	static DWORD* __fastcall onSetKhakiText(DWORD* text);
	static int __fastcall onAddBuildingCapsAfterConstruction(const settlementStruct* sett, int index);
	static building* __fastcall onCheckBuildUpgrade(const settlementStruct* sett, int buildingId);
	static void __fastcall onAttachRegionSettlement(settlementStruct* sett, int regionId);
	static int __fastcall onGetTrueBuildingCapabilities(int counter, const stackCapabilities* cap);
	static void __fastcall onCalculateSettlement(settlementStruct* sett);
	static eduEntry* __fastcall onCustomBattleCost(int eduIndexOffset);
	static int __fastcall onMarriageOption(const factionRecord* facRecord);
	static int __fastcall onCalcUnitStatsWpn(int weapon);
	static int* __fastcall onGetSupportingArmies(armyStruct* defender, armyStruct* attacker);
	static uint32_t __fastcall onDrawBanner(const trackedArmy* army);
	static bannerData* __fastcall onGetRebelSymbol(const trackedArmy* army, bannerData* oldData);
	static void __fastcall onPredictedStats(settlementStats* statsManager);
	static int __fastcall onEvalAttObjective(const aiCampaignController* controller);
	static void __fastcall onCalculateLTGD(aiLongTermGoalDirector* ltgd);
	static void __fastcall onStartProductionTurn(aiPersonalityValues* personality);
	static factionStruct* __fastcall onCheckGarrison(const aiRegionController* controller);
	static int __fastcall onValidateGarrison(const aiRegionController* controller, const armyStruct* army);
	static void __fastcall onUpdateControllerAlloc(aiCampaignController* controller);
	static int __fastcall onScoreBestCapital(const settlementStruct* sett);
	static int __fastcall onGetUnitCard(const eduEntry* entry, int factionId, stringWithHash* newPath);
	static int __fastcall onGetUnitInfoCard(const eduEntry* entry, int factionId, stringWithHash* newPath);
	static int __fastcall onAssessRequiredStrength(const aiRegionController* controller);
	static int __fastcall onCalcGarrisonStr(const aiRegionData* regData, const factionStruct* fac, const settlementStruct* sett);
	static DWORD* __fastcall onCreateTakeResidenceObjective(aiCampaignController* campaignController, DWORD* oldResidence);
	static void __fastcall onPreBattlePlacement(aiTacticAssault* aiTactic);
	static bool __thiscall onPreBattlePlacement2(aiUnitGroup* group, DWORD formationTemplate, bool forceOrder);
	static eduEntry* __fastcall onCreateMercUnit(char** entryName, eduEntry* entry);
	static eduEntry* __fastcall onCreateUnitWrapper(int eduIndexBase, int removeValue);
	static bool __fastcall onDecideRamAttacks(buildingBattle* gate, aiDetachment* detachment, int numRamsLeft);
	static const char* __fastcall onQuickSave();
	static const char* __fastcall onAutoSave();
	static void balanceMinorSettStats(settlementStats* stats, const settlementStruct* sett);
	static eduEntry* __fastcall onGetRecruitPoolUnitEntry(int eduIndex);
	static int __fastcall onFindUnit(char* entry, int* eduIndex);
	static uint32_t __fastcall onSetExtentsTexture(bool isEnemy);
	static uint8_t __fastcall onSetExtentsBorder(uint8_t isBorder, DWORD extentsDisplay);
	static uint32_t __fastcall onSetExtentsZoc();
	static uint8_t __fastcall onSetExtentsZocAlpha(uint8_t oldAlpha);
	static void __fastcall onDecideNeighbours(factionStruct* faction);
	static void __fastcall onInitGsd(aiGlobalStrategyDirector* director);
	static void __fastcall onInitGsd2(aiGlobalStrategyDirector* director);
	static void __fastcall onSetBuildPolicies(aiProductionController* controller, int policy, int secondaryPolicy);
	static void __fastcall onUpdateProdControllers(aiPersonalityValues* personality);
	static void __fastcall onSetProdPriorities(aiProductionController* controller);
	static DWORD __fastcall onUnitInfo(DWORD entryAddress);
	static void __fastcall onTransferSettlement(const settlementStruct* settlement, int reason, factionStruct* faction);
	static portBuildingStruct* __fastcall onTransferSettlementPort(const settlementStruct* settlement);
	static int __fastcall onCheckConstructionItem(const buildingsQueue* queue, const buildingInQueue* constructionItem);
	static float __fastcall onCalculateUnitValue(eduEntry* entry, DWORD value);
	static int __fastcall onEvaluateUnit(int eduIndex);
	static DWORD __fastcall onCustomBattleUnitCards(DWORD cardArrayThing, int factionID);
	static int __fastcall onCustomBattleUnits(eduEntry** unitArray, int currentCount, int factionID);
	static int __fastcall onAttackGate(unit* unit, void* tactic);
	static eduEntry* __fastcall onEvaluateUnit2(int eduIndex);
	static eduEntry* __fastcall onReadDescrRebel(DWORD value);
	static DWORD __fastcall onSearchUnitType(char* typeName);
	static int __fastcall onReligionCombatBonus(int religionID, characterRecord* namedChar);
	static char* __fastcall getBrowserPicConstructed(int cultureID, edbEntry* entry, int buildingLevel);
	static char* __fastcall getBrowserPicConstruction(int cultureID, edbEntry* entry, int buildingLevel);
	static char* __fastcall getBuildingPic(buildingLevel* level, int cultureID);
	static char* __fastcall getBuildingPicConstructed(buildingLevel* level, int cultureID);
	static char* __fastcall getBuildingPicConstruction(buildingLevel* level, int cultureID);
	static portraitDbEntry* __fastcall getPortraitDbEntry(int cultureID);
	static char* __fastcall onGetGuildOfferPic(DWORD level, int cultureID);
	static unit** __fastcall onGetUnitByLabel(DWORD unitLabels, char* label);
	static unitGroup** __fastcall onGetGroupByLabel(DWORD groupLabels, char* label);
	static char* __fastcall onGetCultureEndTurnSound(int cultureID);
	static void __fastcall onCharacterClicked(character* enemy);
	static DWORD __fastcall onCampaignTick();
	static void __fastcall onBattleTick();


	static character* __fastcall mercenaryMovePointsGetGeneral(armyStruct* army);

	//click at tile
	static void __fastcall clickAtTile(coordPair* xy);

	//after loading of campaign map
	static void WINAPI afterCampaignMapLoaded();

	//after new game start
	static void WINAPI onNewGameStart();
	//after reading EDU
	static void WINAPI onEduParsed();
	static void WINAPI onGameInit();
	static void WINAPI onUnloadCampaign();
	static void WINAPI onNewGameLoaded();
	static void __fastcall onAiTurn(aiFaction* aiFac);
	
	//before start of a first faction turn
	static void WINAPI onChangeTurnNum();

	//on console command give_trait, after searching for character
	static void WINAPI onGiveTrait();

	//after loading campaign or battle data
	static void WINAPI afterLoadGameData();

	//after comparison in I_CompareCounter command
	static void WINAPI afterI_CompareCounter();

	//check fort for deleting
	static int WINAPI checkFort();

	//add suzerain dip state
	static int WINAPI checkDipString();
	static int WINAPI checkFacDip();

	//spawn_army command patch
	static int WINAPI spawnArmyCoords();
	static void WINAPI spawnEndArmyCoords();

	//spawn_character command patch
	static int WINAPI spawnCharacterCoords();

	//check and replace characters battle models before battle
	static void WINAPI onBattleStratScreen();


	//parse events in this functions
	static void __fastcall onEvent(DWORD** vTab, DWORD arg2);

	//load game
	static void __fastcall onLoadSaveFile(UNICODE_STRING**& savePath);
	//save game
	static void __fastcall onSaveGame(UNICODE_STRING**& savePath);

	//check of custom tile before battle, replace it here
	static void __fastcall onTileCheck(int* coords);
	//read custom tiles file and make right strings, structures, etc
	static void WINAPI readTilesFile();


	//read and load all startmap models
	static void WINAPI unicalStratModsRead();
	static void WINAPI loadCharStratModels();
	//moment before game start select stratmap models for drawing
	//replace models for the settlements, forts, ports, etc here without craches
	static void WINAPI checkAndChangeModels();
	
	static void WINAPI battleLoaded();
	static void WINAPI toReadGameDbsAtStart();
	
	static void WINAPI onDrawPartsOfStratObjects();
	static void WINAPI onDrawAll();
	static void WINAPI onStartOfDrawFunction();
	static void WINAPI onRetreat();


	static void __fastcall onStopCharacter(character* character);
	static void WINAPI onMoveRecruitQueue();
	static eduEntry* __fastcall recruitEopUnit(int eduIndex);
	static void __fastcall recruitEopUnit2(int eduIndex);
	static void __fastcall recruitEopMercUnit(DWORD pad, DWORD pad2, regionStruct* region, int eduIndex, int factionID, int exp);


	static void __fastcall onEndSiege(settlementStruct* sett);
	static void __fastcall onStartSiege(settlementStruct* sett);
	static void __fastcall onLoadDescrBattleCharacter(armyStruct* army, character* goalGen);


	//called not in all cases!
	static void WINAPI onBattleStateChange();

	static void __thiscall onFactionSymbolSelect();
};

class consolePatches
{
public:
	static void WINAPI onGameConsoleCommandFromConsole();
	static void WINAPI onGameConsoleCommandFromScript();

	static int __fastcall onReadLogonOrLogoff(int isLogonNow);
};