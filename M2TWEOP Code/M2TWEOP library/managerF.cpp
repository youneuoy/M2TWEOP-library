#include "pch.h"
#include "managerF.h"

#include "onlineThings.h"
#include "settlementConversionLvlSetter.h"
#include "gameStringHelpers.h"

#include "jsonManager.h"

#include "casModelsDrawer.h"
#include "globals.h"
#include "Injects.h"
#include "luaPlugin.h"
#include "memWork.h"
#include "patchesForGame.h"
#include "dbghelp.h"

void managerF::debug()
{
	//MessageBoxA(NULL, "TEST", "TEST", NULL);
}
static void noFunc(){}

//apply code injects
void managerF::execPatches()
{
	ofstream f1("logs\\applyingPatches.youneuoylog");
	f1 << "Log:" << '\n';
	MemWork* mem = new MemWork();

	f1 << "Start applying berserkers patch" << '\n';

	berserkersP* brs = new berserkersP(mem, globals::dataS.gameVersion);
	brs->SetlBersCode();
	brs->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying history battles soldier patch" << '\n';
	{
		unsigned char disSold[2] = { 0x90, 0x90 };
		DWORD nPtr;
		DWORD nbPtr;


		if (globals::dataS.gameVersion == 2)
		{

			nPtr = 0x008eebbd;
			nbPtr = 0x008EEBB5;

		}
		else
		{
			nPtr = 0x008ee13d;
			nbPtr = 0x008ee135;

		}

		MemWork::WriteData(disSold, nPtr, 2);
		MemWork::WriteData(disSold, nbPtr, 2);
	}
	f1 << "Done" << '\n';

	f1 << "Start applying stop_character patch" << '\n';

	OnStopCharacter* toStopCharacter = new OnStopCharacter(mem, (LPVOID)patchesForGame::onStopCharacter, globals::dataS.gameVersion);
	toStopCharacter->SetNewCode();
	toStopCharacter->Enable();


	OnMoveRecruitQueue* toMoveRecruitQueue = new OnMoveRecruitQueue(mem, (LPVOID)patchesForGame::onMoveRecruitQueue, globals::dataS.gameVersion);
	toMoveRecruitQueue->SetNewCode();
	toMoveRecruitQueue->Enable();

	//old code, what just disable backspace
	//{
	//	unsigned char ret[1] = { 0xC3};
	//	DWORD nPtr;


	//	if (globals::dataS.gamever == 2)
	//	{

	//		nPtr = 0x00ab5170;

	//	}
	//	else
	//	{
	//		nPtr = 0x00ab4140;

	//	}

	//	MemWork::WriteData(ret, nPtr,1);
	//}
	f1 << "Done" << '\n';

	//f1 << "Start applying toSizeOfBattleField patch" << endl;
	//sizeOfBattleField* toSizeOfBattleField = new sizeOfBattleField(mem, globals::dataS.gamever, 500, 500);
	//toSizeOfBattleField->SetlSizeCode();
	//toSizeOfBattleField->Enable();
	//f1 << "Done" << endl;

	f1 << "Start applying recruit eop unit 1 patch" << '\n';
	recruitEOPunit* toRecruitEOPunit = new recruitEOPunit(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toRecruitEOPunit->SetNewCode();
	toRecruitEOPunit->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying recruit eop unit 2 patch" << '\n';
	recruitEOPunit2* toRecruitEOPunit2 = new recruitEOPunit2(mem, (LPVOID)patchesForGame::recruitEopUnit2, globals::dataS.gameVersion);
	toRecruitEOPunit2->SetNewCode();
	toRecruitEOPunit2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying recruit merc eop unit patch" << '\n';
	recruitEOPMercunit* torecruitEOPMercunit = new recruitEOPMercunit(mem, (LPVOID)patchesForGame::recruitEopMercUnit, globals::dataS.gameVersion);
	torecruitEOPMercunit->SetNewCode();
	torecruitEOPMercunit->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toSearchUnitType patch" << '\n';
	onSearchUnitType* toSearchUnitType = new onSearchUnitType(mem, (LPVOID)patchesForGame::onSearchUnitType, globals::dataS.gameVersion);
	toSearchUnitType->SetNewCode();
	toSearchUnitType->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying toLoadDescrBattleCharacter patch #1" << '\n';

	toLoadDescrBattleCharacter* toDescrChar = new toLoadDescrBattleCharacter(mem, (LPVOID)patchesForGame::onLoadDescrBattleCharacter, globals::dataS.gameVersion);
	toDescrChar->SetlBattleCode();
	toDescrChar->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toBattleStateResults patch #1" << '\n';

	toBattleStateResults* toBattleStats = new toBattleStateResults(mem, (LPVOID)patchesForGame::onBattleStateChange, globals::dataS.gameVersion);
	toBattleStats->SetlBattleCode();
	toBattleStats->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying unit types patch #1" << '\n';

	toReadEDU* ed = new toReadEDU(mem, (LPVOID)patchesForGame::onEduParsed, globals::dataS.gameVersion);
	ed->SetlEDUCode();
	ed->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying OnChangePassedTurnsCount patch" << '\n';

	toChangePassedTurnsCount* tcptc = new toChangePassedTurnsCount(mem, (LPVOID)patchesForGame::onChangeTurnNum, globals::dataS.gameVersion);
	tcptc->SetlTurnsCountCode();
	tcptc->Enable();
	f1 << "Done" << '\n';

	/*
	f1 << "Start applying spawn_army coords patch" << endl;
	toSpawnArmy* spwnArm = new toSpawnArmy(mem, (LPVOID)patchesForGame::spawnArmyCoords, globals::dataS.gameVersion);
	spwnArm->SetlSpawnCode();
	spwnArm->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying spawn_army coords patch end" << endl;
	toEndOfSpawnArmy* endSpwnArm = new toEndOfSpawnArmy(mem, (LPVOID)patchesForGame::spawnEndArmyCoords, globals::dataS.gameVersion);
	endSpwnArm->SetlEndSpawnCode();
	endSpwnArm->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying spawn_character coords patch" << endl;
	toSpawnCharacter* spwnChar = new toSpawnCharacter(mem, (LPVOID)patchesForGame::spawnCharacterCoords, globals::dataS.gameVersion);
	spwnChar->SetlSpawnCode();
	spwnChar->Enable();
	*/
	f1 << "Done" << '\n';


	f1 << "Start applying battle_start patch" << '\n';
	toBattleStratScreen* bstart = new toBattleStratScreen(mem, (LPVOID)patchesForGame::onBattleStratScreen, globals::dataS.gameVersion);
	bstart->SetlBSCode();
	bstart->Enable();

	f1 << "Done" << '\n';
	
	//f1 << "Start applying models load patch #1" << '\n';
	//toStartReadModels* readModelsStart = new toStartReadModels(mem, (LPVOID)stratModelsChange::disableChecker, globals::dataS.gameVersion);
	//readModelsStart->SetlStReadModelsCode();
	//readModelsStart->Enable();
//
	//f1 << "Done" << '\n';


	f1 << "Start applying unical strat models patch" << '\n';
	toResourcesReadModels* readResModels = new toResourcesReadModels(mem, (LPVOID)patchesForGame::unicalStratModsRead, globals::dataS.gameVersion);
	readResModels->SetlResModelsCode();
	readResModels->Enable();

	f1 << "Done" << '\n';


	f1 << "Start applying char strat models patch" << '\n';
	toCharReadModels* CharReadModels = new toCharReadModels(mem, (LPVOID)patchesForGame::loadCharStratModels, globals::dataS.gameVersion);
	CharReadModels->SetNewCode();
	CharReadModels->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying onEvents patch" << '\n';
	toEvents* evnts = new toEvents(mem, (LPVOID)patchesForGame::onEvent, globals::dataS.gameVersion);
	evnts->SetlEventsCode();
	evnts->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying custom_tiles patch" << '\n';
	toCustomTileSelection* ctiles = new toCustomTileSelection(mem, (LPVOID)patchesForGame::onTileCheck, globals::dataS.gameVersion);
	ctiles->SetlTilesCode();
	ctiles->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying custom_tiles file patch" << '\n';
	toCustomTileFileRead* ctilesF = new toCustomTileFileRead(mem, (LPVOID)patchesForGame::readTilesFile, globals::dataS.gameVersion);
	ctilesF->SetlTilesCode();
	ctilesF->Enable();

	f1 << "Done" << '\n';


	f1 << "Start applying start fort models patch" << '\n';
	toStratModelsSelect* stratModSel = new toStratModelsSelect(mem, (LPVOID)patchesForGame::checkAndChangeModels, globals::dataS.gameVersion);
	stratModSel->SetlStratModelsCode();
	stratModSel->Enable();

	f1 << "Done" << '\n';


	f1 << "Start applying load save patch" << '\n';
	toLoadSaveFile* loadSave = new toLoadSaveFile(mem, (LPVOID)patchesForGame::onLoadSaveFile, globals::dataS.gameVersion);
	loadSave->SetlLoadCode();
	loadSave->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying save game patch" << '\n';
	toSaveGame* saveGame = new toSaveGame(mem, (LPVOID)patchesForGame::onSaveGame, globals::dataS.gameVersion);
	saveGame->SetlSaveCode();
	saveGame->Enable();

	f1 << "Done" << '\n';



	f1 << "Start applying new game patch" << '\n';
	toStartNewGame* newGame = new toStartNewGame(mem, (LPVOID)patchesForGame::onNewGameStart, globals::dataS.gameVersion);
	newGame->SetlStartCode();
	newGame->Enable();

	f1 << "Done" << '\n';


	f1 << "Start applying strategical map loading patch" << '\n';
	toEndReadModels* stratMapLoaded = new toEndReadModels(mem, (LPVOID)patchesForGame::afterCampaignMapLoaded, globals::dataS.gameVersion);
	stratMapLoaded->SetlEndReadModelsCode();
	stratMapLoaded->Enable();

	f1 << "Done" << '\n';


	f1 << "Start applying click on tile patch" << '\n';
	toClickAtCoords* clickAtTile = new toClickAtCoords(mem, (LPVOID)patchesForGame::clickAtTile, globals::dataS.gameVersion);
	clickAtTile->SetlClickCode();
	clickAtTile->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying select worldpkgdesc from db patch" << '\n';
	toSelectWorldFromDB* selWorld = new toSelectWorldFromDB(mem, (LPVOID)patchesForGame::selectWorldpkgdesc, globals::dataS.gameVersion);
	selWorld->SetlSelectCode();
	selWorld->Enable();

	f1 << "Done" << '\n';



	f1 << "Start applying battle loaded patch" << '\n';
	toBattleLoaded* battleLoad = new toBattleLoaded(mem, (LPVOID)patchesForGame::battleLoaded, globals::dataS.gameVersion);
	battleLoad->SetlLoadCode();
	battleLoad->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying toDrawPartsOfStratObjects patch" << '\n';
	toDrawPartsOfStratObjects* toDrawPartsOfStratO = new toDrawPartsOfStratObjects(mem, (LPVOID)patchesForGame::onDrawPartsOfStratObjects, globals::dataS.gameVersion);
	toDrawPartsOfStratO->SetlDrawCode();
	toDrawPartsOfStratO->Enable();

	f1 << "Start applying toGameFormDrawImage patch" << '\n';
	toGameFormDrawImage* toDrawAllSt = new toGameFormDrawImage(mem, (LPVOID)patchesForGame::onDrawAll, globals::dataS.gameVersion);
	toDrawAllSt->SetlDrawCode();
	toDrawAllSt->Enable();


	f1 << "Start applying toEndSettlementSiege patch" << '\n';
	toEndSettlementSiege* toEndSiegSett = new toEndSettlementSiege(mem, (LPVOID)patchesForGame::onEndSiege, globals::dataS.gameVersion);
	toEndSiegSett->SetlSiegeCode();
	toEndSiegSett->Enable();

	f1 << "Start applying toStartSettlementSiege patch" << '\n';
	toStartSettlementSiege* toStartSiegSett = new toStartSettlementSiege(mem, (LPVOID)patchesForGame::onStartSiege, globals::dataS.gameVersion);
	toStartSiegSett->SetlSiegeCode();
	toStartSiegSett->Enable();

	f1 << "Done" << '\n';

	f1 << "Start applying CastleConversionLvl patch" << '\n';
	CastleConversionLvlSetter* castleConversionLvlSetter = new CastleConversionLvlSetter(mem, (LPVOID)settlementConversionLvlSetter::getConversionLvlFromCastle, globals::dataS.gameVersion);
	castleConversionLvlSetter->SetNewCode();
	castleConversionLvlSetter->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying CityConversionLvl patch" << '\n';
	CityConversionLvlSetter* cityConversionLvlSetter = new CityConversionLvlSetter(mem, (LPVOID)settlementConversionLvlSetter::getConversionLvlFromCity, globals::dataS.gameVersion);
	cityConversionLvlSetter->SetNewCode();
	cityConversionLvlSetter->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toGameInitialize patch" << '\n';
	onGameInitialized* toGameInitialize = new onGameInitialized(mem, (LPVOID)patchesForGame::onGameInit, globals::dataS.gameVersion);
	toGameInitialize->SetNewCode();
	toGameInitialize->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toUnloadCampaign patch" << '\n';
	onUnloadCampaign* toUnloadCampaign = new onUnloadCampaign(mem, (LPVOID)patchesForGame::onUnloadCampaign, globals::dataS.gameVersion);
	toUnloadCampaign->SetNewCode();
	toUnloadCampaign->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onBuildingUnitAttributes patch" << '\n';
	auto toBuildingUnitAttributes = new onBuildingUnitAttributes(mem, (LPVOID)patchesForGame::onEvaluateUnit2, globals::dataS.gameVersion);
	toBuildingUnitAttributes->SetNewCode();
	toBuildingUnitAttributes->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onBuildingUnitAttributes2 patch" << '\n';
	auto toBuildingUnitAttributes2 = new onBuildingUnitAttributes2(mem, (LPVOID)patchesForGame::onEvaluateUnit2, globals::dataS.gameVersion);
	toBuildingUnitAttributes2->SetNewCode();
	toBuildingUnitAttributes2->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying mercenaryMovepointsGetGeneral patch" << '\n';
	mercenaryMovepointsGetGeneral* mercP = new mercenaryMovepointsGetGeneral(mem, (LPVOID)patchesForGame::mercenaryMovePointsGetGeneral, globals::dataS.gameVersion);
	mercP->SetNewCode();
	mercP->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onGameConsoleCommandFromConsole patch" << '\n';
	onGameConsoleCommandFromConsole* cons1 = new onGameConsoleCommandFromConsole(mem, (LPVOID)consolePatches::onGameConsoleCommandFromConsole, globals::dataS.gameVersion);
	cons1->SetNewCode();
	cons1->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onGameConsoleCommandFromScript patch" << '\n';
	onGameConsoleCommandFromScript* cons2 = new onGameConsoleCommandFromScript(mem, (LPVOID)consolePatches::onGameConsoleCommandFromScript, globals::dataS.gameVersion);
	cons2->SetNewCode();
	cons2->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onGameConsoleCommandFromScript patch" << '\n';
	OnReadLogonOrLogoff* cons3 = new OnReadLogonOrLogoff(mem, (LPVOID)consolePatches::onReadLogonOrLogoff, globals::dataS.gameVersion);
	cons3->SetNewCode();
	cons3->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying OnLoadSettlementWorldpkgdesc patch" << '\n';
	OnLoadSettlementWorldpkgdesc* loadWordSet = new OnLoadSettlementWorldpkgdesc(mem, (LPVOID)patchesForGame::onLoadSettlementWorldpkgdesc, globals::dataS.gameVersion);
	loadWordSet->SetNewCode();
	loadWordSet->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying fortificationLevelS patch" << '\n';
	fortificationlevelS* fortificationLevel = new fortificationlevelS(mem, (LPVOID)patchesForGame::onFortificationLevelS, globals::dataS.gameVersion);
	fortificationLevel->SetNewCode();
	fortificationLevel->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying LimitRecruitmentQueueToSlots patch" << '\n';
	LimitRecruitmentQueueToSlotsInj* limitRecr = new LimitRecruitmentQueueToSlotsInj(mem, &globals::dataS.eopGameplaySettings.bLimitRecruitmentToSlotsAndByMoney, globals::dataS.gameVersion);
	limitRecr->SetNewCode();
	limitRecr->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying OnSaveEDUStringS patch" << '\n';
	OnSaveEDUStringS* eduStringsSaveOn = new OnSaveEDUStringS(mem, (LPVOID)patchesForGame::onSaveEDUStringS, globals::dataS.gameVersion);
	eduStringsSaveOn->SetNewCode();
	eduStringsSaveOn->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying OnSaveEDUStringS2 patch" << '\n';
	OnSaveEDUStringS2* eduStringsSaveOn2 = new OnSaveEDUStringS2(mem, (LPVOID)patchesForGame::onSaveEDUStringS, globals::dataS.gameVersion);
	eduStringsSaveOn2->SetNewCode();
	eduStringsSaveOn2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying OnCreateUnit2 patch" << '\n';
	OnCreateUnit2* onCreateUnit2 = new OnCreateUnit2(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	onCreateUnit2->SetNewCode();
	onCreateUnit2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toFindUnit patch" << '\n';
	OnFindUnit*toFindUnit = new OnFindUnit(mem, (LPVOID)patchesForGame::onFindUnit, globals::dataS.gameVersion);
	toFindUnit->SetNewCode();
	toFindUnit->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onReadDescrStrat patch" << '\n';
	onReadDescrStrat* toReadDescrStrat = new onReadDescrStrat(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toReadDescrStrat->SetNewCode();
	toReadDescrStrat->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onSpawnBrigands patch" << '\n';
	onSpawnBrigands* toSpawnBrigands = new onSpawnBrigands(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnBrigands->SetNewCode();
	toSpawnBrigands->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onSpawnBrigands2 patch" << '\n';
	onSpawnBrigands2* toSpawnBrigands2 = new onSpawnBrigands2(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnBrigands2->SetNewCode();
	toSpawnBrigands2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onCustomBattleCost patch" << '\n';
	onCustomBattleCost* toCustomBattleCost = new onCustomBattleCost(mem, (LPVOID)patchesForGame::onCustomBattleCost, globals::dataS.gameVersion);
	toCustomBattleCost->SetNewCode();
	toCustomBattleCost->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying spawnUnitsSettGift patch" << '\n';
	spawnUnitsSettGift* toSpawnUnitsSettGift = new spawnUnitsSettGift(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift->SetNewCode();
	toSpawnUnitsSettGift->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying spawnUnitsSettGift2 patch" << '\n';
	spawnUnitsSettGift2* toSpawnUnitsSettGift2 = new spawnUnitsSettGift2(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift2->SetNewCode();
	toSpawnUnitsSettGift2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying spawnUnitsSettGift3 patch" << '\n';
	spawnUnitsSettGift3* toSpawnUnitsSettGift3 = new spawnUnitsSettGift3(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift3->SetNewCode();
	toSpawnUnitsSettGift3->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onReadDescrRebel patch" << '\n';
	onReadDescrRebel* toReadDescrRebel = new onReadDescrRebel(mem, (LPVOID)patchesForGame::onReadDescrRebel, globals::dataS.gameVersion);
	toReadDescrRebel->SetNewCode();
	toReadDescrRebel->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onCustomBattleUnitCards patch" << '\n';
	onCustomBattleUnitCards* toCustomBattleUnitCards = new onCustomBattleUnitCards(mem, (LPVOID)patchesForGame::onCustomBattleUnitCards, globals::dataS.gameVersion);
	toCustomBattleUnitCards->SetNewCode();
	toCustomBattleUnitCards->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onCustomBattleUnits patch" << '\n';
	onCustomBattleUnits* toCustomBattleUnits = new onCustomBattleUnits(mem, (LPVOID)patchesForGame::onCustomBattleUnits, globals::dataS.gameVersion);
	toCustomBattleUnits->SetNewCode();
	toCustomBattleUnits->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onPopActionMem patch" << '\n';
	onPopActionMem* toPopActionMem = new onPopActionMem(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toPopActionMem->SetNewCode();
	toPopActionMem->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onSetSettlementModel patch" << '\n';
	onSetSettlementModel* toSetSettlementModel = new onSetSettlementModel(mem, (LPVOID)patchesForGame::onSetSettlementModel, globals::dataS.gameVersion);
	toSetSettlementModel->SetNewCode();
	toSetSettlementModel->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onCalculateCommand patch" << '\n';
	onCalculateCommand* toCalculateCommand = new onCalculateCommand(mem, (LPVOID)patchesForGame::onCalculateCommand, globals::dataS.gameVersion);
	toCalculateCommand->SetNewCode();
	toCalculateCommand->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onUnitMerge patch" << '\n';
	onUnitMerge* toUnitMerge = new onUnitMerge(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toUnitMerge->SetNewCode();
	toUnitMerge->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onArmyDecimate patch" << '\n';
	onArmyDecimate* toArmyDecimate = new onArmyDecimate(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toArmyDecimate->SetNewCode();
	toArmyDecimate->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onWriteSoldiersToStrat patch" << '\n';
	onWriteSoldiersToStrat* toWriteSoldiersToStrat = new onWriteSoldiersToStrat(mem, (LPVOID)patchesForGame::onWriteSoldiersToStrat, globals::dataS.gameVersion);
	toWriteSoldiersToStrat->SetNewCode();
	toWriteSoldiersToStrat->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onGetUnitInfoCard patch" << '\n';
	onGetUnitInfoCard* toGetUnitInfoCard = new onGetUnitInfoCard(mem, (LPVOID)patchesForGame::onGetUnitInfoCard, globals::dataS.gameVersion);
	toGetUnitInfoCard->SetNewCode();
	toGetUnitInfoCard->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onGetUnitCard patch" << '\n';
	onGetUnitCard* toGetUnitCard = new onGetUnitCard(mem, (LPVOID)patchesForGame::onGetUnitCard, globals::dataS.gameVersion);
	toGetUnitCard->SetNewCode();
	toGetUnitCard->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying onCalculateMiningIncome patch" << '\n';
	onCalculateMiningIncome* toCalculateMiningIncome = new onCalculateMiningIncome(mem, (LPVOID)patchesForGame::onCalculateMiningIncome, globals::dataS.gameVersion);
	toCalculateMiningIncome->SetNewCode();
	toCalculateMiningIncome->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onNewGameLoaded patch" << '\n';
	onNewGameLoaded* toNewGameLoaded = new onNewGameLoaded(mem, (LPVOID)patchesForGame::onNewGameLoaded, globals::dataS.gameVersion);
	toNewGameLoaded->SetNewCode();
	toNewGameLoaded->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onBattleRams patch" << '\n';
	onBattleRams* toBattleRams = new onBattleRams(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toBattleRams->SetNewCode();
	toBattleRams->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onPreBattlePlacement patch" << '\n';
	onPreBattlePlacement* toPreBattlePlacement = new onPreBattlePlacement(mem, (LPVOID)patchesForGame::onPreBattlePlacement, globals::dataS.gameVersion);
	toPreBattlePlacement->SetNewCode();
	toPreBattlePlacement->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDecideRamAttacks patch" << '\n';
	onDecideRamAttacks* toDecideRamAttacks = new onDecideRamAttacks(mem, (LPVOID)patchesForGame::onDecideRamAttacks, globals::dataS.gameVersion);
	toDecideRamAttacks->SetNewCode();
	toDecideRamAttacks->Enable();
	f1 << "Done" << '\n';
	

	f1 << "Start applying spawnUnitsSettGift4 patch" << '\n';
	spawnUnitsSettGift4* toSpawnUnitsSettGift4 = new spawnUnitsSettGift4(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift4->SetNewCode();
	toSpawnUnitsSettGift4->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toCreateMercUnit patch" << '\n';
	OnCreateMercUnit* toCreateMercUnit = new OnCreateMercUnit(mem, (LPVOID)patchesForGame::onCreateMercUnit, globals::dataS.gameVersion);
	toCreateMercUnit->SetNewCode();
	toCreateMercUnit->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onEvaluateUnit patch" << '\n';
	onEvaluateUnit* toEvaluateUnit = new onEvaluateUnit(mem, (LPVOID)patchesForGame::onEvaluateUnit, globals::dataS.gameVersion);
	toEvaluateUnit->SetNewCode();
	toEvaluateUnit->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onEvaluateUnit2 patch" << '\n';
	onEvaluateUnit2* toEvaluateUnit2 = new onEvaluateUnit2(mem, (LPVOID)patchesForGame::onEvaluateUnit2, globals::dataS.gameVersion);
	toEvaluateUnit2->SetNewCode();
	toEvaluateUnit2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toAddCultureToArray patch" << '\n';
	onAddCultureToArray* toAddCultureToArray = new onAddCultureToArray(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAddCultureToArray->SetNewCode();
	toAddCultureToArray->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toParseBuildingPics patch" << '\n';
	onParseBuildingPics* toParseBuildingPics = new onParseBuildingPics(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toParseBuildingPics->SetNewCode();
	toParseBuildingPics->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onParseBuildingLevelPics patch" << '\n';
	onParseBuildingLevelPics* toParseBuildingLevelPics = new onParseBuildingLevelPics(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toParseBuildingLevelPics->SetNewCode();
	toParseBuildingLevelPics->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toParseBuildingPics patch" << '\n';
	onParseEventBodies* toParseEventBodies = new onParseEventBodies(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toParseEventBodies->SetNewCode();
	toParseEventBodies->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCreatePortraitDb patch" << '\n';
	onCreatePortraitDb* toCreatePortraitDb = new onCreatePortraitDb(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toCreatePortraitDb->SetNewCode();
	toCreatePortraitDb->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying toAssignGateAttacker patch" << '\n';
	onAssignGateAttacker* toAssignGateAttacker = new onAssignGateAttacker(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAssignGateAttacker->SetNewCode();
	toAssignGateAttacker->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAttackGate patch" << '\n';
	onAttackGate2* toAttackGate2 = new onAttackGate2(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAttackGate2->SetNewCode();
	toAttackGate2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAttackGate patch" << '\n';
	onAttackGate* toAttackGate = new onAttackGate(mem, (LPVOID)patchesForGame::onAttackGate, globals::dataS.gameVersion);
	toAttackGate->SetNewCode();
	toAttackGate->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetExtentsTexture patch" << '\n';
	onSetExtentsTexture* toSetExtentsTexture = new onSetExtentsTexture(mem, (LPVOID)patchesForGame::onSetExtentsTexture, globals::dataS.gameVersion);
	toSetExtentsTexture->SetNewCode();
	toSetExtentsTexture->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetExtentsTextureBorder patch" << '\n';
	onSetExtentsTextureBorder* toSetExtentsTextureBorder = new onSetExtentsTextureBorder(mem, (LPVOID)patchesForGame::onSetExtentsBorder, globals::dataS.gameVersion);
	toSetExtentsTextureBorder->SetNewCode();
	toSetExtentsTextureBorder->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetExtentsTextureZOCAlpha patch" << '\n';
	onSetExtentsTextureZOCAlpha* toSetExtentsTextureZOCAlpha = new onSetExtentsTextureZOCAlpha(mem, (LPVOID)patchesForGame::onSetExtentsZocAlpha, globals::dataS.gameVersion);
	toSetExtentsTextureZOCAlpha->SetNewCode();
	toSetExtentsTextureZOCAlpha->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetExtentsTextureZOC patch" << '\n';
	onSetExtentsTextureZOC* toSetExtentsTextureZOC = new onSetExtentsTextureZOC(mem, (LPVOID)patchesForGame::onSetExtentsZoc, globals::dataS.gameVersion);
	toSetExtentsTextureZOC->SetNewCode();
	toSetExtentsTextureZOC->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCreateProductionController patch" << '\n';
	onCreateProductionController* toCreateProductionController = new onCreateProductionController(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toCreateProductionController->SetNewCode();
	toCreateProductionController->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetBuildPolicies patch" << '\n';
	onSetBuildPolicies* toSetBuildPolicies = new onSetBuildPolicies(mem, (LPVOID)patchesForGame::onSetBuildPolicies, globals::dataS.gameVersion);
	toSetBuildPolicies->SetNewCode();
	toSetBuildPolicies->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onUpdateProdControllers patch" << '\n';
	onUpdateProdControllers* toUpdateProdControllers = new onUpdateProdControllers(mem, (LPVOID)patchesForGame::onUpdateProdControllers, globals::dataS.gameVersion);
	toUpdateProdControllers->SetNewCode();
	toUpdateProdControllers->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetProdPriorities patch" << '\n';
	onSetProdPriorities* toSetProdPriorities = new onSetProdPriorities(mem, (LPVOID)patchesForGame::onSetProdPriorities, globals::dataS.gameVersion);
	toSetProdPriorities->SetNewCode();
	toSetProdPriorities->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetTrueBuildingCapabilities patch" << '\n';
	onGetTrueBuildingCapabilities* toGetTrueBuildingCapabilities = new onGetTrueBuildingCapabilities(mem, (LPVOID)patchesForGame::onGetTrueBuildingCapabilities, globals::dataS.gameVersion);
	toGetTrueBuildingCapabilities->SetNewCode();
	toGetTrueBuildingCapabilities->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCheckMountedEngineValid patch" << '\n';
	onCheckMountedEngineValid* toCheckMountedEngineValid = new onCheckMountedEngineValid(mem, (LPVOID)patchesForGame::onGetMountedEngine, globals::dataS.gameVersion);
	toCheckMountedEngineValid->SetNewCode();
	toCheckMountedEngineValid->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetMountedEngine patch" << '\n';
	onGetMountedEngine* toGetMountedEngine = new onGetMountedEngine(mem, (LPVOID)patchesForGame::onGetMountedEngine, globals::dataS.gameVersion);
	toGetMountedEngine->SetNewCode();
	toGetMountedEngine->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onParseMountedEngines patch" << '\n';
	onParseMountedEngines* toParseMountedEngines = new onParseMountedEngines(mem, (LPVOID)patchesForGame::onParseMountedEngines, globals::dataS.gameVersion);
	toParseMountedEngines->SetNewCode();
	toParseMountedEngines->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onParseEdu patch" << '\n';
	onParseEdu* toParseEdu = new onParseEdu(mem, (LPVOID)patchesForGame::onParseEdu, globals::dataS.gameVersion);
	toParseEdu->SetNewCode();
	toParseEdu->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onApplyKillChance patch" << '\n';
	onApplyKillChance* toApplyKillChance = new onApplyKillChance(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toApplyKillChance->SetNewCode();
	toApplyKillChance->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onRemoveKillChance patch" << '\n';
	onRemoveKillChance* toRemoveKillChance = new onRemoveKillChance(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toRemoveKillChance->SetNewCode();
	toRemoveKillChance->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onInitUnitBattle patch" << '\n';
	onInitUnitBattle* toInitUnitBattle = new onInitUnitBattle(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toInitUnitBattle->SetNewCode();
	toInitUnitBattle->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCreateUnitKillChance patch" << '\n';
	onCreateUnitKillChance* toCreateUnitKillChance = new onCreateUnitKillChance(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toCreateUnitKillChance->SetNewCode();
	toCreateUnitKillChance->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDecideNeighbours patch" << '\n';
	onDecideNeighbours* toDecideNeighbours = new onDecideNeighbours(mem, (LPVOID)patchesForGame::onDecideNeighbours, globals::dataS.gameVersion);
	toDecideNeighbours->SetNewCode();
	toDecideNeighbours->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onTransferSettlement patch" << '\n';
	onTransferSettlement* toTransferSettlement = new onTransferSettlement(mem, (LPVOID)patchesForGame::onTransferSettlement, globals::dataS.gameVersion);
	toTransferSettlement->SetNewCode();
	toTransferSettlement->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onRemoveFromUnitQueue patch" << '\n';
	onRemoveFromUnitQueue* toRemoveFromUnitQueue = new onRemoveFromUnitQueue(mem, (LPVOID)patchesForGame::onRemoveFromUnitQueue, globals::dataS.gameVersion);
	toRemoveFromUnitQueue->SetNewCode();
	toRemoveFromUnitQueue->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddSettlementToDiplomacyScroll patch" << '\n';
	onAddSettlementToDiplomacyScroll* toAddSettlementToDiplomacyScroll = new onAddSettlementToDiplomacyScroll(mem, (LPVOID)patchesForGame::onAddSettlementToDiplomacyScroll, globals::dataS.gameVersion);
	toAddSettlementToDiplomacyScroll->SetNewCode();
	toAddSettlementToDiplomacyScroll->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onPartialConstructionPush patch" << '\n';
	onPartialConstructionPush* toPartialConstructionPush = new onPartialConstructionPush(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toPartialConstructionPush->SetNewCode();
	toPartialConstructionPush->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onLoadBuilding patch" << '\n';
	onLoadBuilding* toLoadBuilding = new onLoadBuilding(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toLoadBuilding->SetNewCode();
	toLoadBuilding->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onLoadModelRigid patch" << '\n';
	onLoadModelRigid* toLoadModelRigid = new onLoadModelRigid(mem, (LPVOID)patchesForGame::onLoadModelRigid, globals::dataS.gameVersion);
	toLoadModelRigid->SetNewCode();
	toLoadModelRigid->Enable();
	f1 << "Done" << '\n';
	
	
	f1 << "Start applying onUnloadModels patch" << '\n';
	onUnloadModels* toUnloadModels = new onUnloadModels(mem, (LPVOID)patchesForGame::onUnloadModels, globals::dataS.gameVersion);
	toUnloadModels->SetNewCode();
	toUnloadModels->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onOffMapModelThing patch" << '\n';
	onOffMapModelThing* toOffMapModelThing = new onOffMapModelThing(mem, (LPVOID)patchesForGame::onOffMapModelThing, globals::dataS.gameVersion);
	toOffMapModelThing->SetNewCode();
	toOffMapModelThing->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onOffMapModelThing2 patch" << '\n';
	onOffMapModelThing2* toOffMapModelThing2 = new onOffMapModelThing2(mem, (LPVOID)patchesForGame::onOffMapModelThing, globals::dataS.gameVersion);
	toOffMapModelThing2->SetNewCode();
	toOffMapModelThing2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onRemoveBuilding patch" << '\n';
	onRemoveBuilding* toRemoveBuilding = new onRemoveBuilding(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toRemoveBuilding->SetNewCode();
	toRemoveBuilding->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddBuilding patch" << '\n';
	onAddBuilding* toAddBuilding = new onAddBuilding(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAddBuilding->SetNewCode();
	toAddBuilding->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBuildingByIdConst patch" << '\n';
	onGetBuildingByIdConst* toGetBuildingByIdConst = new onGetBuildingByIdConst(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toGetBuildingByIdConst->SetNewCode();
	toGetBuildingByIdConst->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBuildingById patch" << '\n';
	onGetBuildingById* toGetBuildingById = new onGetBuildingById(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toGetBuildingById->SetNewCode();
	toGetBuildingById->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDamageCoreBuilding patch" << '\n';
	onDamageCoreBuilding* toDamageCoreBuilding = new onDamageCoreBuilding(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toDamageCoreBuilding->SetNewCode();
	toDamageCoreBuilding->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDamageCoreBuilding2 patch" << '\n';
	onDamageCoreBuilding2* toDamageCoreBuilding2 = new onDamageCoreBuilding2(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toDamageCoreBuilding2->SetNewCode();
	toDamageCoreBuilding2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDamageCoreBuilding3 patch" << '\n';
	onDamageCoreBuilding3* toDamageCoreBuilding3 = new onDamageCoreBuilding3(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toDamageCoreBuilding3->SetNewCode();
	toDamageCoreBuilding3->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetWatchTowerRange patch" << '\n';
	onGetWatchTowerRange* toGetWatchTowerRange = new onGetWatchTowerRange(mem, (LPVOID)patchesForGame::onGetWatchTowerRange, globals::dataS.gameVersion);
	toGetWatchTowerRange->SetNewCode();
	toGetWatchTowerRange->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetDefensesBuilding patch" << '\n';
	onGetDefensesBuilding* toGetDefensesBuilding = new onGetDefensesBuilding(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toGetDefensesBuilding->SetNewCode();
	toGetDefensesBuilding->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onIsBuildingTypePresentAtMinLevel patch" << '\n';
	onIsBuildingTypePresentAtMinLevel* toIsBuildingTypePresentAtMinLevel = new onIsBuildingTypePresentAtMinLevel(mem, (LPVOID)patchesForGame::onGetBuildingById, globals::dataS.gameVersion);
	toIsBuildingTypePresentAtMinLevel->SetNewCode();
	toIsBuildingTypePresentAtMinLevel->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onConflictTest2 patch" << '\n';
	onConflictTest2* toConflictTest2 = new onConflictTest2(mem, (LPVOID)patchesForGame::onConflictTest, globals::dataS.gameVersion);
	toConflictTest2->SetNewCode();
	toConflictTest2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onConflictTest patch" << '\n';
	onConflictTest* toConflictTest = new onConflictTest(mem, (LPVOID)patchesForGame::onConflictTest, globals::dataS.gameVersion);
	toConflictTest->SetNewCode();
	toConflictTest->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCheckBuildUpgrade patch" << '\n';
	onCheckBuildUpgrade* toCheckBuildUpgrade = new onCheckBuildUpgrade(mem, (LPVOID)patchesForGame::onCheckBuildUpgrade, globals::dataS.gameVersion);
	toCheckBuildUpgrade->SetNewCode();
	toCheckBuildUpgrade->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCheckBuildUpgrade2 patch" << '\n';
	onCheckBuildUpgrade2* toCheckBuildUpgrade2 = new onCheckBuildUpgrade2(mem, (LPVOID)patchesForGame::onCheckBuildUpgrade, globals::dataS.gameVersion);
	toCheckBuildUpgrade2->SetNewCode();
	toCheckBuildUpgrade2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCheckSettHasBuilding patch" << '\n';
	onCheckSettHasBuilding* toCheckSettHasBuilding = new onCheckSettHasBuilding(mem, (LPVOID)patchesForGame::onCheckSettHasBuilding, globals::dataS.gameVersion);
	toCheckSettHasBuilding->SetNewCode();
	toCheckSettHasBuilding->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying getPossibleConstructions patch" << '\n';
	getPossibleConstructions* toGetPossibleConstructions = new getPossibleConstructions(mem, (LPVOID)patchesForGame::getPossibleConstructions, globals::dataS.gameVersion);
	toGetPossibleConstructions->SetNewCode();
	toGetPossibleConstructions->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetCqFlags patch" << '\n';
	onSetCqFlags* toSetCqFlags = new onSetCqFlags(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toSetCqFlags->SetNewCode();
	toSetCqFlags->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetCqFlags2 patch" << '\n';
	onSetCqFlags2* toSetCqFlags2 = new onSetCqFlags2(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toSetCqFlags2->SetNewCode();
	toSetCqFlags2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onRemoveCqFlags patch" << '\n';
	onRemoveCqFlags* toRemoveCqFlags = new onRemoveCqFlags(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toRemoveCqFlags->SetNewCode();
	toRemoveCqFlags->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onRemoveCqFlags2 patch" << '\n';
	onRemoveCqFlags2* toRemoveCqFlags2 = new onRemoveCqFlags2(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toRemoveCqFlags2->SetNewCode();
	toRemoveCqFlags2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddBuildingCapsAfterConstruction patch" << '\n';
	onAddBuildingCapsAfterConstruction* toAddBuildingCapsAfterConstruction = new onAddBuildingCapsAfterConstruction(mem, (LPVOID)patchesForGame::onAddBuildingCapsAfterConstruction, globals::dataS.gameVersion);
	toAddBuildingCapsAfterConstruction->SetNewCode();
	toAddBuildingCapsAfterConstruction->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCheckGarrison patch" << '\n';
	onCheckGarrison* toCheckGarrison = new onCheckGarrison(mem, (LPVOID)patchesForGame::onCheckGarrison, globals::dataS.gameVersion);
	toCheckGarrison->SetNewCode();
	toCheckGarrison->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onValidateGarrison patch" << '\n';
	onValidateGarrison* toValidateGarrison = new onValidateGarrison(mem, (LPVOID)patchesForGame::onValidateGarrison, globals::dataS.gameVersion);
	toValidateGarrison->SetNewCode();
	toValidateGarrison->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGeneralAssaultAction patch" << '\n';
	onGeneralAssaultAction* toGeneralAssaultAction = new onGeneralAssaultAction(mem, (LPVOID)patchesForGame::onGeneralAssaultAction, globals::dataS.gameVersion);
	toGeneralAssaultAction->SetNewCode();
	toGeneralAssaultAction->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddBuildingCapsAfterConstruction2 patch" << '\n';
	onAddBuildingCapsAfterConstruction2* toAddBuildingCapsAfterConstruction2 = new onAddBuildingCapsAfterConstruction2(mem, (LPVOID)patchesForGame::onAddBuildingCapsAfterConstruction, globals::dataS.gameVersion);
	toAddBuildingCapsAfterConstruction2->SetNewCode();
	toAddBuildingCapsAfterConstruction2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddBuildingCapsAfterConstruction3 patch" << '\n';
	onAddBuildingCapsAfterConstruction3* toAddBuildingCapsAfterConstruction3 = new onAddBuildingCapsAfterConstruction3(mem, (LPVOID)patchesForGame::onAddBuildingCapsAfterConstruction, globals::dataS.gameVersion);
	toAddBuildingCapsAfterConstruction3->SetNewCode();
	toAddBuildingCapsAfterConstruction3->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddBuildingCapsAfterConstruction4 patch" << '\n';
	onAddBuildingCapsAfterConstruction4* toAddBuildingCapsAfterConstruction4 = new onAddBuildingCapsAfterConstruction4(mem, (LPVOID)patchesForGame::onAddBuildingCapsAfterConstruction, globals::dataS.gameVersion);
	toAddBuildingCapsAfterConstruction4->SetNewCode();
	toAddBuildingCapsAfterConstruction4->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAddBuildingCapsAfterConstruction5 patch" << '\n';
	onAddBuildingCapsAfterConstruction5* toAddBuildingCapsAfterConstruction5 = new onAddBuildingCapsAfterConstruction5(mem, (LPVOID)patchesForGame::onAddBuildingCapsAfterConstruction, globals::dataS.gameVersion);
	toAddBuildingCapsAfterConstruction5->SetNewCode();
	toAddBuildingCapsAfterConstruction5->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCanWithdrawPreBattle patch" << '\n';
	onCanWithdrawPreBattle* toCanWithdrawPreBattle = new onCanWithdrawPreBattle(mem, (LPVOID)patchesForGame::onCanWithdrawPreBattle, globals::dataS.gameVersion);
	toCanWithdrawPreBattle->SetNewCode();
	toCanWithdrawPreBattle->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCalculateLTGD patch" << '\n';
	onCalculateLTGD* toCalculateLTGD = new onCalculateLTGD(mem, (LPVOID)patchesForGame::onCalculateLTGD, globals::dataS.gameVersion);
	toCalculateLTGD->SetNewCode();
	toCalculateLTGD->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onStartProductionTurn patch" << '\n';
	onStartProductionTurn* toStartProductionTurn = new onStartProductionTurn(mem, (LPVOID)patchesForGame::onStartProductionTurn, globals::dataS.gameVersion);
	toStartProductionTurn->SetNewCode();
	toStartProductionTurn->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDecideMissionTarget patch" << '\n';
	onDecideMissionTarget* toDecideMissionTarget = new onDecideMissionTarget(mem, (LPVOID)patchesForGame::onDecideMissionTarget, globals::dataS.gameVersion);
	toDecideMissionTarget->SetNewCode();
	toDecideMissionTarget->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetKhakiText patch" << '\n';
	onSetKhakiText* toSetKhakiText = new onSetKhakiText(mem, (LPVOID)patchesForGame::onSetKhakiText, globals::dataS.gameVersion);
	toSetKhakiText->SetNewCode();
	toSetKhakiText->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetKhakiText2 patch" << '\n';
	onSetKhakiText2* toSetKhakiText2 = new onSetKhakiText2(mem, (LPVOID)patchesForGame::onSetKhakiText, globals::dataS.gameVersion);
	toSetKhakiText2->SetNewCode();
	toSetKhakiText2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetKhakiText3 patch" << '\n';
	onSetKhakiText3* toSetKhakiText3 = new onSetKhakiText3(mem, (LPVOID)patchesForGame::onSetKhakiText, globals::dataS.gameVersion);
	toSetKhakiText3->SetNewCode();
	toSetKhakiText3->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetKhakiText4 patch" << '\n';
	onSetKhakiText4* toSetKhakiText4 = new onSetKhakiText4(mem, (LPVOID)patchesForGame::onSetKhakiText, globals::dataS.gameVersion);
	toSetKhakiText4->SetNewCode();
	toSetKhakiText4->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetKhakiText5 patch" << '\n';
	onSetKhakiText5* toSetKhakiText5 = new onSetKhakiText5(mem, (LPVOID)patchesForGame::onSetKhakiText, globals::dataS.gameVersion);
	toSetKhakiText5->SetNewCode();
	toSetKhakiText5->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onReadHiddenResources patch" << '\n';
	onReadHiddenResources* toReadHiddenResources = new onReadHiddenResources(mem, (LPVOID)patchesForGame::onReadHiddenResources, globals::dataS.gameVersion);
	toReadHiddenResources->SetNewCode();
	toReadHiddenResources->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetRegionHiddenResources patch" << '\n';
	onSetRegionHiddenResources* toSetRegionHiddenResources = new onSetRegionHiddenResources(mem, (LPVOID)patchesForGame::onSetRegionHiddenResources, globals::dataS.gameVersion);
	toSetRegionHiddenResources->SetNewCode();
	toSetRegionHiddenResources->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetRegionHiddenResources2 patch" << '\n';
	onSetRegionHiddenResources2* toSetRegionHiddenResources2 = new onSetRegionHiddenResources2(mem, (LPVOID)patchesForGame::onGetHiddenResource, globals::dataS.gameVersion);
	toSetRegionHiddenResources2->SetNewCode();
	toSetRegionHiddenResources2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying toCreateMarriageOption patch" << '\n';
	onCreateMarriageOption* toCreateMarriageOption = new onCreateMarriageOption(mem, (LPVOID)patchesForGame::onMarriageOption, globals::dataS.gameVersion);
	toCreateMarriageOption->SetNewCode();
	toCreateMarriageOption->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying toCreateMarriageOption2 patch" << '\n';
	onCreateMarriageOption2* toCreateMarriageOption2 = new onCreateMarriageOption2(mem, (LPVOID)patchesForGame::onMarriageOption, globals::dataS.gameVersion);
	toCreateMarriageOption2->SetNewCode();
	toCreateMarriageOption2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCampaignTick patch" << '\n';
	onCampaignTick* toCampaignTick = new onCampaignTick(mem, (LPVOID)patchesForGame::onCampaignTick, globals::dataS.gameVersion);
	toCampaignTick->SetNewCode();
	toCampaignTick->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onBattleTick patch" << '\n';
	onBattleTick* toBattleTick = new onBattleTick(mem, (LPVOID)patchesForGame::onBattleTick, globals::dataS.gameVersion);
	toBattleTick->SetNewCode();
	toBattleTick->Enable();
	f1 << "Done" << '\n';
	
	//f1 << "Start applying onEnemyClicked patch" << '\n';
	//onEnemyClicked* toEnemyClicked = new onEnemyClicked(mem, (LPVOID)patchesForGame::onEnemyClicked, globals::dataS.gameVersion);
	//toEnemyClicked->SetNewCode();
	//toEnemyClicked->Enable();
	//f1 << "Done" << '\n';
	
	f1 << "Start applying onCreateBirthOption patch" << '\n';
	onCreateBirthOption* toCreateBirthOption = new onCreateBirthOption(mem, (LPVOID)patchesForGame::onMarriageOption, globals::dataS.gameVersion);
	toCreateBirthOption->SetNewCode();
	toCreateBirthOption->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCreateBirthOption2 patch" << '\n';
	onCreateBirthOption2* toCreateBirthOption2 = new onCreateBirthOption2(mem, (LPVOID)patchesForGame::onMarriageOption, globals::dataS.gameVersion);
	toCreateBirthOption2->SetNewCode();
	toCreateBirthOption2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onHiddenResourceConditionParse patch" << '\n';
	onHiddenResourceConditionParse* toHiddenResourceConditionParse = new onHiddenResourceConditionParse(mem, (LPVOID)patchesForGame::onGetHiddenResource, globals::dataS.gameVersion);
	toHiddenResourceConditionParse->SetNewCode();
	toHiddenResourceConditionParse->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying toHiddenResourceCheck patch" << '\n';
	onHiddenResourceCheck* toHiddenResourceCheck = new onHiddenResourceCheck(mem, (LPVOID)patchesForGame::onCheckHiddenResource, globals::dataS.gameVersion);
	toHiddenResourceCheck->SetNewCode();
	toHiddenResourceCheck->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying toHiddenResourceCheck2 patch" << '\n';
	onHiddenResourceCheck2* toHiddenResourceCheck2 = new onHiddenResourceCheck2(mem, (LPVOID)patchesForGame::onCheckHiddenResource, globals::dataS.gameVersion);
	toHiddenResourceCheck2->SetNewCode();
	toHiddenResourceCheck2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetArmyGeneralsUnit patch" << '\n';
	onSetArmyGeneralsUnit* toSetArmyGeneralsUnit = new onSetArmyGeneralsUnit(mem, (LPVOID)patchesForGame::onSetArmyGeneralsUnit, globals::dataS.gameVersion);
	toSetArmyGeneralsUnit->SetNewCode();
	toSetArmyGeneralsUnit->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onTransferSettlementPort patch" << '\n';
	onTransferSettlementPort* toTransferSettlementPort = new onTransferSettlementPort(mem, (LPVOID)patchesForGame::onTransferSettlementPort, globals::dataS.gameVersion);
	toTransferSettlementPort->SetNewCode();
	toTransferSettlementPort->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCheckConstructionItem patch" << '\n';
	onCheckConstructionItem* toCheckConstructionItem = new onCheckConstructionItem(mem, (LPVOID)patchesForGame::onCheckConstructionItem, globals::dataS.gameVersion);
	toCheckConstructionItem->SetNewCode();
	toCheckConstructionItem->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCreateTakeResidenceObjective patch" << '\n';
	onCreateTakeResidenceObjective* toCreateTakeResidenceObjective = new onCreateTakeResidenceObjective(mem, (LPVOID)patchesForGame::onCreateTakeResidenceObjective, globals::dataS.gameVersion);
	toCreateTakeResidenceObjective->SetNewCode();
	toCreateTakeResidenceObjective->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onInitGsd patch" << '\n';
	onInitGsd* toInitGsd = new onInitGsd(mem, (LPVOID)patchesForGame::onInitGsd, globals::dataS.gameVersion);
	toInitGsd->SetNewCode();
	toInitGsd->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onInitGsd2 patch" << '\n';
	onInitGsd2* toInitGsd2 = new onInitGsd2(mem, (LPVOID)patchesForGame::onInitGsd2, globals::dataS.gameVersion);
	toInitGsd2->SetNewCode();
	toInitGsd2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAttachRegionSettlement patch" << '\n';
	onAttachRegionSettlement* toAttachRegionSettlement = new onAttachRegionSettlement(mem, (LPVOID)patchesForGame::onAttachRegionSettlement, globals::dataS.gameVersion);
	toAttachRegionSettlement->SetNewCode();
	toAttachRegionSettlement->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onCalculateSettlement patch" << '\n';
	onCalculateSettlement* toCalculateSettlement = new onCalculateSettlement(mem, (LPVOID)patchesForGame::onCalculateSettlement, globals::dataS.gameVersion);
	toCalculateSettlement->SetNewCode();
	toCalculateSettlement->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onScoreBestCapital patch" << '\n';
	onScoreBestCapital* toScoreBestCapital = new onScoreBestCapital(mem, (LPVOID)patchesForGame::onScoreBestCapital, globals::dataS.gameVersion);
	toScoreBestCapital->SetNewCode();
	toScoreBestCapital->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onPredictedStats patch" << '\n';
	onPredictedStats* toPredictedStats = new onPredictedStats(mem, (LPVOID)patchesForGame::onPredictedStats, globals::dataS.gameVersion);
	toPredictedStats->SetNewCode();
	toPredictedStats->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onEvalAttObjective patch" << '\n';
	onEvalAttObjective* toEvalAttObjective = new onEvalAttObjective(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective->SetNewCode();
	toEvalAttObjective->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onEvalAttObjective2 patch" << '\n';
	onEvalAttObjective2* toEvalAttObjective2 = new onEvalAttObjective2(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective2->SetNewCode();
	toEvalAttObjective2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onEvalAttObjective patch" << '\n';
	onEvalAttObjective3* toEvalAttObjective3 = new onEvalAttObjective3(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective3->SetNewCode();
	toEvalAttObjective3->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onEvalAttObjective patch" << '\n';
	onEvalAttObjective4* toEvalAttObjective4 = new onEvalAttObjective4(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective4->SetNewCode();
	toEvalAttObjective4->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onEvalAttObjective patch" << '\n';
	onEvalAttObjective5* toEvalAttObjective5 = new onEvalAttObjective5(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective5->SetNewCode();
	toEvalAttObjective5->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onUpdateControllerAlloc patch" << '\n';
	onUpdateControllerAlloc* toUpdateControllerAlloc = new onUpdateControllerAlloc(mem, (LPVOID)patchesForGame::onUpdateControllerAlloc, globals::dataS.gameVersion);
	toUpdateControllerAlloc->SetNewCode();
	toUpdateControllerAlloc->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAssessRequiredStrength patch" << '\n';
	onAssessRequiredStrength* toAssessRequiredStrength = new onAssessRequiredStrength(mem, (LPVOID)patchesForGame::onAssessRequiredStrength, globals::dataS.gameVersion);
	toAssessRequiredStrength->SetNewCode();
	toAssessRequiredStrength->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onRegionGroupStuff patch" << '\n';
	onRegionGroupStuff* toRegionGroupStuff = new onRegionGroupStuff(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toRegionGroupStuff->SetNewCode();
	toRegionGroupStuff->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onDrawBanner patch" << '\n';
	onDrawBanner* toDrawBanner = new onDrawBanner(mem, (LPVOID)patchesForGame::onDrawBanner, globals::dataS.gameVersion);
	toDrawBanner->SetNewCode();
	toDrawBanner->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onSetupBattleFromStrat patch" << '\n';
	onSetupBattleFromStrat* toSetupBattleFromStrat = new onSetupBattleFromStrat(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toSetupBattleFromStrat->SetNewCode();
	toSetupBattleFromStrat->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetRebelSymbol patch" << '\n';
	onGetRebelSymbol* toGetRebelSymbol = new onGetRebelSymbol(mem, (LPVOID)patchesForGame::onGetRebelSymbol, globals::dataS.gameVersion);
	toGetRebelSymbol->SetNewCode();
	toGetRebelSymbol->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetSupportingArmies patch" << '\n';
	onGetSupportingArmies* toGetSupportingArmies = new onGetSupportingArmies(mem, (LPVOID)patchesForGame::onGetSupportingArmies, globals::dataS.gameVersion);
	toGetSupportingArmies->SetNewCode();
	toGetSupportingArmies->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetSupportingArmies2 patch" << '\n';
	onGetSupportingArmies2* toGetSupportingArmies2 = new onGetSupportingArmies2(mem, (LPVOID)patchesForGame::onGetSupportingArmies, globals::dataS.gameVersion);
	toGetSupportingArmies2->SetNewCode();
	toGetSupportingArmies2->Enable();
	f1 << "Done" << '\n';
	
	
	f1 << "Start applying onCalcGarrisonStr patch" << '\n';
	onCalcGarrisonStr* toCalcGarrisonStr = new onCalcGarrisonStr(mem, (LPVOID)patchesForGame::onCalcGarrisonStr, globals::dataS.gameVersion);
	toCalcGarrisonStr->SetNewCode();
	toCalcGarrisonStr->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBrowserPicConstructed patch" << '\n';
	onGetBrowserPicConstructed* toGetBrowserPicConstructed= new onGetBrowserPicConstructed(mem, (LPVOID)patchesForGame::getBrowserPicConstructed, globals::dataS.gameVersion);
	toGetBrowserPicConstructed->SetNewCode();
	toGetBrowserPicConstructed->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBrowserPicConstruction patch" << '\n';
	onGetBrowserPicConstruction* toGetBrowserPicConstruction= new onGetBrowserPicConstruction(mem, (LPVOID)patchesForGame::getBrowserPicConstruction, globals::dataS.gameVersion);
	toGetBrowserPicConstruction->SetNewCode();
	toGetBrowserPicConstruction->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBuildingPic patch" << '\n';
	onGetBuildingPic* toGetBuildingPic = new onGetBuildingPic(mem, (LPVOID)patchesForGame::getBuildingPic, globals::dataS.gameVersion);
	toGetBuildingPic->SetNewCode();
	toGetBuildingPic->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetGuildOfferPic patch" << '\n';
	onGetGuildOfferPic* toGetGuildOfferPic = new onGetGuildOfferPic(mem, (LPVOID)patchesForGame::onGetGuildOfferPic, globals::dataS.gameVersion);
	toGetGuildOfferPic->SetNewCode();
	toGetGuildOfferPic->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetCultureEndTurnSound patch" << '\n';
	onGetCultureEndTurnSound* toGetCultureEndTurnSound = new onGetCultureEndTurnSound(mem, (LPVOID)patchesForGame::onGetCultureEndTurnSound, globals::dataS.gameVersion);
	toGetCultureEndTurnSound->SetNewCode();
	toGetCultureEndTurnSound->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBuildingPic2 patch" << '\n';
	onGetBuildingPic2* toGetBuildingPic2 = new onGetBuildingPic2(mem, (LPVOID)patchesForGame::getBuildingPic, globals::dataS.gameVersion);
	toGetBuildingPic2->SetNewCode();
	toGetBuildingPic2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBuildingPicConstruction patch" << '\n';
	onGetBuildingPicConstruction* toGetBuildingPicConstruction = new onGetBuildingPicConstruction(mem, (LPVOID)patchesForGame::getBuildingPicConstruction, globals::dataS.gameVersion);
	toGetBuildingPicConstruction->SetNewCode();
	toGetBuildingPicConstruction->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetBuildingPicConstructed patch" << '\n';
	onGetBuildingPicConstructed* toGetBuildingPicConstructed = new onGetBuildingPicConstructed(mem, (LPVOID)patchesForGame::getBuildingPicConstructed, globals::dataS.gameVersion);
	toGetBuildingPicConstructed->SetNewCode();
	toGetBuildingPicConstructed->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying toGetBuildingPicConstructed2 patch" << '\n';
	onGetBrowserPicConstructed2* toGetBuildingPicConstructed2 = new onGetBrowserPicConstructed2(mem, (LPVOID)patchesForGame::getBuildingPicConstructed, globals::dataS.gameVersion);
	toGetBuildingPicConstructed2->SetNewCode();
	toGetBuildingPicConstructed2->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onGetPortraitDbEntry patch" << '\n';
	onGetPortraitDbEntry* toGetPortraitDbEntry = new onGetPortraitDbEntry(mem, (LPVOID)patchesForGame::getPortraitDbEntry, globals::dataS.gameVersion);
	toGetPortraitDbEntry->SetNewCode();
	toGetPortraitDbEntry->Enable();
	f1 << "Done" << '\n';
	
	f1 << "Start applying onAiTurn patch" << '\n';
	onAiTurn* toAiTurn = new onAiTurn(mem, (LPVOID)patchesForGame::onAiTurn, globals::dataS.gameVersion);
	toAiTurn->SetNewCode();
	toAiTurn->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toCalculateUnitValue patch" << '\n';
	OnCalculateUnitValue* toCalculateUnitValue = new OnCalculateUnitValue(mem, (LPVOID)patchesForGame::onCalculateUnitValue, globals::dataS.gameVersion);
	toCalculateUnitValue->SetNewCode();
	toCalculateUnitValue->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toCalculateUnitValue2 patch" << '\n';
	OnCalculateUnitValue2* toCalculateUnitValue2 = new OnCalculateUnitValue2(mem, (LPVOID)patchesForGame::onCalculateUnitValue, globals::dataS.gameVersion);
	toCalculateUnitValue2->SetNewCode();
	toCalculateUnitValue2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onGetUnitByLabel patch" << '\n';
	onGetUnitByLabel* toGetUnitByLabel = new onGetUnitByLabel(mem, (LPVOID)patchesForGame::onGetUnitByLabel, globals::dataS.gameVersion);
	toGetUnitByLabel->SetNewCode();
	toGetUnitByLabel->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onGetGroupByLabel patch" << '\n';
	onGetGroupByLabel* toGetGroupByLabel = new onGetGroupByLabel(mem, (LPVOID)patchesForGame::onGetGroupByLabel, globals::dataS.gameVersion);
	toGetGroupByLabel->SetNewCode();
	toGetGroupByLabel->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onReadBuildingPool patch" << '\n';
	onReadBuildingPool* toReadBuildingPool = new onReadBuildingPool(mem, (LPVOID)patchesForGame::onEvaluateUnit2, globals::dataS.gameVersion);
	toReadBuildingPool->SetNewCode();
	toReadBuildingPool->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying onReadMercenaryPool patch" << '\n';
	onReadMercenaryPool* toReadMercenaryPool = new onReadMercenaryPool(mem, (LPVOID)patchesForGame::onEvaluateUnit2, globals::dataS.gameVersion);
	toReadMercenaryPool->SetNewCode();
	toReadMercenaryPool->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying OnCreateUnitWrapper patch" << '\n';
	OnCreateUnitWrapper* toCreateUnitWrapper = new OnCreateUnitWrapper(mem, (LPVOID)patchesForGame::onCreateUnitWrapper, globals::dataS.gameVersion);
	toCreateUnitWrapper->SetNewCode();
	toCreateUnitWrapper->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying OnFindUnitStrings patch" << '\n';
	OnFindUnitStrings* toFindUnitStrings = new OnFindUnitStrings(mem, (LPVOID)patchesForGame::onCreateUnitWrapper, globals::dataS.gameVersion); //its same func as unit wrapper on purpose not error
	toFindUnitStrings->SetNewCode();
	toFindUnitStrings->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toFindUnitStrings2 patch" << '\n';
	OnFindUnitStrings2* toFindUnitStrings2 = new OnFindUnitStrings2(mem, (LPVOID)patchesForGame::onCreateUnitWrapper, globals::dataS.gameVersion); //its same func as unit wrapper on purpose not error
	toFindUnitStrings2->SetNewCode();
	toFindUnitStrings2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying OnReligionCombatBonus patch" << '\n';
	OnReligionCombatBonus* toReligionCombatBonus = new OnReligionCombatBonus(mem, (LPVOID)patchesForGame::onReligionCombatBonus, globals::dataS.gameVersion);
	toReligionCombatBonus->SetNewCode();
	toReligionCombatBonus->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toOnBattleArmiesBug patch" << '\n';
	OnBattleArmiesBug* toOnBattleArmiesBug = new OnBattleArmiesBug(mem, nullptr, globals::dataS.gameVersion);
	toOnBattleArmiesBug->SetNewCode();
	toOnBattleArmiesBug->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying OnGetRecruitPoolUnitEntry patch" << '\n';
	OnGetRecruitPoolUnitEntry* toGetRecruitPoolUnitEntry = new OnGetRecruitPoolUnitEntry(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toGetRecruitPoolUnitEntry->SetNewCode();
	toGetRecruitPoolUnitEntry->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toLoadRecruitQueue patch" << '\n';
	loadRecruitQueue* toLoadRecruitQueue = new loadRecruitQueue(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toLoadRecruitQueue->SetNewCode();
	toLoadRecruitQueue->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toLoadRecruitQueue2 patch" << '\n';
	loadRecruitQueue2* toLoadRecruitQueue2 = new loadRecruitQueue2(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toLoadRecruitQueue2->SetNewCode();
	toLoadRecruitQueue2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying OnGetRecruitPoolUnitEntry2 patch" << '\n';
	OnGetRecruitPoolUnitEntry2* toGetRecruitPoolUnitEntry2 = new OnGetRecruitPoolUnitEntry2(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toGetRecruitPoolUnitEntry2->SetNewCode();
	toGetRecruitPoolUnitEntry2->Enable();
	f1 << "Done" << '\n';

	f1 << "Start applying toCreateMercUnitCheck patch" << '\n';
	OnCreateMercUnitCheck* toCreateMercUnitCheck = new OnCreateMercUnitCheck(mem, (LPVOID)patchesForGame::onCreateMercUnitCheck, globals::dataS.gameVersion);
	toCreateMercUnitCheck->SetNewCode();
	toCreateMercUnitCheck->Enable();
	f1 << "Done" << '\n';

	if (globals::dataS.gameCfg.isSaveBackupEnabled == true) {
		f1 << "Start applying OnQuickSave patch" << '\n';
		OnQuickSave* onQuickSave = new OnQuickSave(mem, (LPVOID)patchesForGame::onQuickSave, globals::dataS.gameVersion);
		onQuickSave->SetNewCode();
		onQuickSave->Enable();
		f1 << "Done" << '\n';

		f1 << "Start applying OnAutoSave patch" << '\n';
		OnAutoSave* onAutoSave = new OnAutoSave(mem, (LPVOID)patchesForGame::onAutoSave, globals::dataS.gameVersion);
		onAutoSave->SetNewCode();
		onAutoSave->Enable();
		f1 << "Done" << '\n';
	}

	OnPathCasheCrashPlace* onPathCasheCrashPlace = new OnPathCasheCrashPlace(mem, (LPVOID)&tacticalMapViewer::getPathCache, globals::dataS.gameVersion, (LPVOID)&globals::dataS.Modules.tacticalMapViewer);
	onPathCasheCrashPlace->SetNewCode();
	onPathCasheCrashPlace->Enable();

	toSelectForDrawPortsCas* onSelectForDrawPortsCas = new toSelectForDrawPortsCas(mem, (LPVOID)casModelsDrawer::drawModels, globals::dataS.gameVersion);
	onSelectForDrawPortsCas->SetNewCode();
	onSelectForDrawPortsCas->Enable();

	//toStartOfDrawFunction* onStartOfDrawFunction = new toStartOfDrawFunction(mem, (LPVOID)patchesForGame::onStartOfDrawFunction, globals::dataS.gameVersion);
	//onStartOfDrawFunction->SetNewCode();
	//onStartOfDrawFunction->Enable();


	if (battleCreator::isRetreatsOverhaulEnabled())
	{
		toRetreat* onRetreat = new toRetreat(mem, (LPVOID)patchesForGame::onRetreat, globals::dataS.gameVersion);
		onRetreat->SetNewCode();
		onRetreat->Enable();
	}
	if (globals::dataS.gameCfg.isBlockLaunchWithoutEop == true)
	{
		f1 << "Start applying isBlockLaunchWithoutEop patch" << '\n';
		auto blockLaunch = new blockLaunchWithoutEop(mem, globals::dataS.gameVersion);
		blockLaunch->SetNewCode();
		blockLaunch->Enable();
		f1 << "Done" << '\n';
	}
	f1 << "End." << '\n';
	f1.close();

	//DWORD instruction = 0x191468;
	//MemWork::WriteData(&instruction, 0x008B963E, 3);
	//MemWork::WriteData(&instruction, 0x004BD9C0, 3);


	f1 << "Start applying new game patch" << '\n';
	toFactionSymbolSelect* newFacSymSelect = new toFactionSymbolSelect(mem, (LPVOID)patchesForGame::onFactionSymbolSelect, globals::dataS.gameVersion);
	newFacSymSelect->SetNewFactionSymbolSelect();
	newFacSymSelect->Enable();

	f1 << "Done" << '\n';

}


void managerF::loadJsonSettings()
{
	std::string fPath = globals::dataS.modPath;
	fPath += R"(\eopData\config\gameCfg.json)";
	jsn::json json = jsonManager::loadJsonFromFile(fPath);
	try
	{
		bool jsonBoolValue;
		if (json.contains("isContextMenuNeeded"))
		{
			getJson(jsonBoolValue, "isContextMenuNeeded")
			globals::dataS.Modules.contextMenuStrat.isContextMenuNeeded = jsonBoolValue;
		}
		if (json.contains("isTacticalMapViewerNeeded"))
		{
			getJson(jsonBoolValue, "isTacticalMapViewerNeeded")
			globals::dataS.Modules.tacticalMapViewer.isTacticalMapViewerNeeded = jsonBoolValue;
		}
		if (json.contains("isDeveloperModeNeeded"))
		{
			getJson(jsonBoolValue, "isDeveloperModeNeeded")
			globals::dataS.Modules.developerMode.isDeveloperModeNeeded = jsonBoolValue;
		}
		if (json.contains("isBlockLaunchWithoutEop"))
		{
			getJson(jsonBoolValue, "isBlockLaunchWithoutEop")
			globals::dataS.gameCfg.isBlockLaunchWithoutEop = jsonBoolValue;
		}
		if (json.contains("isDiscordRichPresenceEnabled"))
		{
			getJson(jsonBoolValue, "isDiscordRichPresenceEnabled")
			globals::dataS.gameCfg.isDiscordRichPresenceEnabled = jsonBoolValue;
		}
		if (json.contains("isSaveBackupEnabled"))
		{
			getJson(jsonBoolValue, "isSaveBackupEnabled")
			globals::dataS.gameCfg.isSaveBackupEnabled = jsonBoolValue;
		}
	}
	catch (jsn::json::type_error &e)
	{
		MessageBoxA(nullptr, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}

	std::string uiPath = globals::dataS.modPath;
	uiPath += R"(\eopData\config\uiCfg.json)";
	json = jsonManager::loadJsonFromFile(uiPath);
	try
	{
		if (json.contains("launcherTheme"))
		{
			getJson(globals::dataS.gameCfg.launcherTheme, "launcherTheme")
		}
		if (json.contains("modVersion"))
		{
			getJson(globals::dataS.gameCfg.modVersion, "modVersion")
		}
	}
	catch (jsn::json::type_error& e)
	{
		MessageBoxA(nullptr, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}
}

void managerF::initThread()
{
	std::random_device dev;
	std::srand(dev());
	battleCreator::readParams();
	codes::initCodes(globals::dataS.gameVersion);
	dataOffsets::initDataOffsets(globals::dataS.gameVersion);
	execPatches();
	initLuaPlugin(false);
	globals::dataS.Modules.tacticalMapViewer.init(globals::dataS.gameVersion);
}

// void waitForDebugger() {
//     while (!IsDebuggerPresent()) {
//         std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
//     }
// }

EOP_EXPORT void managerExport::initEOP(const char* modPath, const int gameVer)
{
    // waitForDebugger();

	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		return;
	globals::dataS.gameVersion = gameVer;
	globals::dataS.modPath = modPath;
	SymSetOptions(SYMOPT_DEFERRED_LOADS);
	SymInitialize((HANDLE)GetCurrentProcess(), (PCSTR)modPath, TRUE);
	CreateDirectoryA("logs", nullptr);
	managerF::initThread();
}
