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

void managerF::debug()
{
	//MessageBoxA(NULL, "TEST", "TEST", NULL);
}
static void noFunc(){}

//apply code injects
void managerF::execPatches()
{
	ofstream f1("logs\\applyingPatches.youneuoylog");
	f1 << "Log:" << endl;
	MemWork* mem = new MemWork();

	f1 << "Start applying berserkers patch" << endl;

	berserkersP* brs = new berserkersP(mem, globals::dataS.gameVersion);
	brs->SetlBersCode();
	brs->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying history battles soldier patch" << endl;
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
	f1 << "Done" << endl;

	f1 << "Start applying stop_character patch" << endl;

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
	f1 << "Done" << endl;

	//f1 << "Start applying toSizeOfBattleField patch" << endl;
	//sizeOfBattleField* toSizeOfBattleField = new sizeOfBattleField(mem, globals::dataS.gamever, 500, 500);
	//toSizeOfBattleField->SetlSizeCode();
	//toSizeOfBattleField->Enable();
	//f1 << "Done" << endl;

	f1 << "Start applying recruit eop unit 1 patch" << endl;
	recruitEOPunit* toRecruitEOPunit = new recruitEOPunit(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toRecruitEOPunit->SetNewCode();
	toRecruitEOPunit->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying recruit eop unit 2 patch" << endl;
	recruitEOPunit2* toRecruitEOPunit2 = new recruitEOPunit2(mem, (LPVOID)patchesForGame::recruitEopUnit2, globals::dataS.gameVersion);
	toRecruitEOPunit2->SetNewCode();
	toRecruitEOPunit2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying recruit merc eop unit patch" << endl;
	recruitEOPMercunit* torecruitEOPMercunit = new recruitEOPMercunit(mem, (LPVOID)patchesForGame::recruitEopMercUnit, globals::dataS.gameVersion);
	torecruitEOPMercunit->SetNewCode();
	torecruitEOPMercunit->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toSearchUnitType patch" << '\n';
	onSearchUnitType* toSearchUnitType = new onSearchUnitType(mem, (LPVOID)patchesForGame::onSearchUnitType, globals::dataS.gameVersion);
	toSearchUnitType->SetNewCode();
	toSearchUnitType->Enable();
	f1 << "Done" << '\n';


	f1 << "Start applying toLoadDescrBattleCharacter patch #1" << endl;

	toLoadDescrBattleCharacter* toDescrChar = new toLoadDescrBattleCharacter(mem, (LPVOID)patchesForGame::onLoadDescrBattleCharacter, globals::dataS.gameVersion);
	toDescrChar->SetlBattleCode();
	toDescrChar->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toBattleStateResults patch #1" << endl;

	toBattleStateResults* toBattleStats = new toBattleStateResults(mem, (LPVOID)patchesForGame::onBattleStateChange, globals::dataS.gameVersion);
	toBattleStats->SetlBattleCode();
	toBattleStats->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying unit types patch #1" << endl;

	toReadEDU* ed = new toReadEDU(mem, (LPVOID)patchesForGame::onEduParsed, globals::dataS.gameVersion);
	ed->SetlEDUCode();
	ed->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnChangePassedTurnsCount patch" << endl;

	toChangePassedTurnsCount* tcptc = new toChangePassedTurnsCount(mem, (LPVOID)patchesForGame::onChangeTurnNum, globals::dataS.gameVersion);
	tcptc->SetlTurnsCountCode();
	tcptc->Enable();
	f1 << "Done" << endl;

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
	f1 << "Done" << endl;


	f1 << "Start applying battle_start patch" << endl;
	toBattleStratScreen* bstart = new toBattleStratScreen(mem, (LPVOID)patchesForGame::onBattleStratScreen, globals::dataS.gameVersion);
	bstart->SetlBSCode();
	bstart->Enable();

	f1 << "Done" << endl;
	
	f1 << "Start applying models load patch #1" << endl;
	toStartReadModels* readModelsStart = new toStartReadModels(mem, (LPVOID)stratModelsChange::disableChecker, globals::dataS.gameVersion);
	readModelsStart->SetlStReadModelsCode();
	readModelsStart->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying unical strat models patch" << endl;
	toResourcesReadModels* readResModels = new toResourcesReadModels(mem, (LPVOID)patchesForGame::unicalStratModsRead, globals::dataS.gameVersion);
	readResModels->SetlResModelsCode();
	readResModels->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying char strat models patch" << endl;
	toCharReadModels* CharReadModels = new toCharReadModels(mem, (LPVOID)patchesForGame::loadCharStratModels, globals::dataS.gameVersion);
	CharReadModels->SetNewCode();
	CharReadModels->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying onEvents patch" << endl;
	toEvents* evnts = new toEvents(mem, (LPVOID)patchesForGame::onEvent, globals::dataS.gameVersion);
	evnts->SetlEventsCode();
	evnts->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying custom_tiles patch" << endl;
	toCustomTileSelection* ctiles = new toCustomTileSelection(mem, (LPVOID)patchesForGame::onTileCheck, globals::dataS.gameVersion);
	ctiles->SetlTilesCode();
	ctiles->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying custom_tiles file patch" << endl;
	toCustomTileFileRead* ctilesF = new toCustomTileFileRead(mem, (LPVOID)patchesForGame::readTilesFile, globals::dataS.gameVersion);
	ctilesF->SetlTilesCode();
	ctilesF->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying start fort models patch" << endl;
	toStratModelsSelect* stratModSel = new toStratModelsSelect(mem, (LPVOID)patchesForGame::checkAndChangeModels, globals::dataS.gameVersion);
	stratModSel->SetlStratModelsCode();
	stratModSel->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying load save patch" << endl;
	toLoadSaveFile* loadSave = new toLoadSaveFile(mem, (LPVOID)patchesForGame::onLoadSaveFile, globals::dataS.gameVersion);
	loadSave->SetlLoadCode();
	loadSave->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying save game patch" << endl;
	toSaveGame* saveGame = new toSaveGame(mem, (LPVOID)patchesForGame::onSaveGame, globals::dataS.gameVersion);
	saveGame->SetlSaveCode();
	saveGame->Enable();

	f1 << "Done" << endl;



	f1 << "Start applying new game patch" << endl;
	toStartNewGame* newGame = new toStartNewGame(mem, (LPVOID)patchesForGame::onNewGameStart, globals::dataS.gameVersion);
	newGame->SetlStartCode();
	newGame->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying strategical map loading patch" << endl;
	toEndReadModels* stratMapLoaded = new toEndReadModels(mem, (LPVOID)patchesForGame::afterCampaignMapLoaded, globals::dataS.gameVersion);
	stratMapLoaded->SetlEndReadModelsCode();
	stratMapLoaded->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying click on tile patch" << endl;
	toClickAtCoords* clickAtTile = new toClickAtCoords(mem, (LPVOID)patchesForGame::clickAtTile, globals::dataS.gameVersion);
	clickAtTile->SetlClickCode();
	clickAtTile->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying select worldpkgdesc from db patch" << endl;
	toSelectWorldFromDB* selWorld = new toSelectWorldFromDB(mem, (LPVOID)patchesForGame::selectWorldpkgdesc, globals::dataS.gameVersion);
	selWorld->SetlSelectCode();
	selWorld->Enable();

	f1 << "Done" << endl;



	f1 << "Start applying battle loaded patch" << endl;
	toBattleLoaded* battleLoad = new toBattleLoaded(mem, (LPVOID)patchesForGame::battleLoaded, globals::dataS.gameVersion);
	battleLoad->SetlLoadCode();
	battleLoad->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying toDrawPartsOfStratObjects patch" << endl;
	toDrawPartsOfStratObjects* toDrawPartsOfStratO = new toDrawPartsOfStratObjects(mem, (LPVOID)patchesForGame::onDrawPartsOfStratObjects, globals::dataS.gameVersion);
	toDrawPartsOfStratO->SetlDrawCode();
	toDrawPartsOfStratO->Enable();

	f1 << "Start applying toGameFormDrawImage patch" << endl;
	toGameFormDrawImage* toDrawAllSt = new toGameFormDrawImage(mem, (LPVOID)patchesForGame::onDrawAll, globals::dataS.gameVersion);
	toDrawAllSt->SetlDrawCode();
	toDrawAllSt->Enable();


	f1 << "Start applying toEndSettlementSiege patch" << endl;
	toEndSettlementSiege* toEndSiegSett = new toEndSettlementSiege(mem, (LPVOID)patchesForGame::onEndSiege, globals::dataS.gameVersion);
	toEndSiegSett->SetlSiegeCode();
	toEndSiegSett->Enable();

	f1 << "Start applying toStartSettlementSiege patch" << endl;
	toStartSettlementSiege* toStartSiegSett = new toStartSettlementSiege(mem, (LPVOID)patchesForGame::onStartSiege, globals::dataS.gameVersion);
	toStartSiegSett->SetlSiegeCode();
	toStartSiegSett->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying CastleConversionLvl patch" << endl;
	CastleConversionLvlSetter* castleConversionLvlSetter = new CastleConversionLvlSetter(mem, (LPVOID)settlementConversionLvlSetter::getConversionLvlFromCastle, globals::dataS.gameVersion);
	castleConversionLvlSetter->SetNewCode();
	castleConversionLvlSetter->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying CityConversionLvl patch" << endl;
	CityConversionLvlSetter* cityConversionLvlSetter = new CityConversionLvlSetter(mem, (LPVOID)settlementConversionLvlSetter::getConversionLvlFromCity, globals::dataS.gameVersion);
	cityConversionLvlSetter->SetNewCode();
	cityConversionLvlSetter->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toGameInitialize patch" << endl;
	onGameInitialized* toGameInitialize = new onGameInitialized(mem, (LPVOID)patchesForGame::onGameInit, globals::dataS.gameVersion);
	toGameInitialize->SetNewCode();
	toGameInitialize->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toUnloadCampaign patch" << endl;
	onUnloadCampaign* toUnloadCampaign = new onUnloadCampaign(mem, (LPVOID)patchesForGame::onUnloadCampaign, globals::dataS.gameVersion);
	toUnloadCampaign->SetNewCode();
	toUnloadCampaign->Enable();
	f1 << "Done" << endl;

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


	f1 << "Start applying mercenaryMovepointsGetGeneral patch" << endl;
	mercenaryMovepointsGetGeneral* mercP = new mercenaryMovepointsGetGeneral(mem, (LPVOID)patchesForGame::mercenaryMovePointsGetGeneral, globals::dataS.gameVersion);
	mercP->SetNewCode();
	mercP->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onGameConsoleCommandFromConsole patch" << endl;
	onGameConsoleCommandFromConsole* cons1 = new onGameConsoleCommandFromConsole(mem, (LPVOID)consolePatches::onGameConsoleCommandFromConsole, globals::dataS.gameVersion);
	cons1->SetNewCode();
	cons1->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onGameConsoleCommandFromScript patch" << endl;
	onGameConsoleCommandFromScript* cons2 = new onGameConsoleCommandFromScript(mem, (LPVOID)consolePatches::onGameConsoleCommandFromScript, globals::dataS.gameVersion);
	cons2->SetNewCode();
	cons2->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onGameConsoleCommandFromScript patch" << endl;
	OnReadLogonOrLogoff* cons3 = new OnReadLogonOrLogoff(mem, (LPVOID)consolePatches::onReadLogonOrLogoff, globals::dataS.gameVersion);
	cons3->SetNewCode();
	cons3->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnLoadSettlementWorldpkgdesc patch" << endl;
	OnLoadSettlementWorldpkgdesc* loadWordSet = new OnLoadSettlementWorldpkgdesc(mem, (LPVOID)patchesForGame::onLoadSettlementWorldpkgdesc, globals::dataS.gameVersion);
	loadWordSet->SetNewCode();
	loadWordSet->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying fortificationLevelS patch" << endl;
	fortificationlevelS* fortificationLevel = new fortificationlevelS(mem, (LPVOID)patchesForGame::onFortificationLevelS, globals::dataS.gameVersion);
	fortificationLevel->SetNewCode();
	fortificationLevel->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying LimitRecruitmentQueueToSlots patch" << endl;
	LimitRecruitmentQueueToSlotsInj* limitRecr = new LimitRecruitmentQueueToSlotsInj(mem, &globals::dataS.eopGameplaySettings.bLimitRecruitmentToSlotsAndByMoney, globals::dataS.gameVersion);
	limitRecr->SetNewCode();
	limitRecr->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnSaveEDUStringS patch" << endl;
	OnSaveEDUStringS* eduStringsSaveOn = new OnSaveEDUStringS(mem, (LPVOID)patchesForGame::onSaveEDUStringS, globals::dataS.gameVersion);
	eduStringsSaveOn->SetNewCode();
	eduStringsSaveOn->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnSaveEDUStringS2 patch" << endl;
	OnSaveEDUStringS2* eduStringsSaveOn2 = new OnSaveEDUStringS2(mem, (LPVOID)patchesForGame::onSaveEDUStringS, globals::dataS.gameVersion);
	eduStringsSaveOn2->SetNewCode();
	eduStringsSaveOn2->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying OnCreateUnit2 patch" << endl;
	OnCreateUnit2* onCreateUnit2 = new OnCreateUnit2(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	onCreateUnit2->SetNewCode();
	onCreateUnit2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toFindUnit patch" << endl;
	OnFindUnit*toFindUnit = new OnFindUnit(mem, (LPVOID)patchesForGame::onFindUnit, globals::dataS.gameVersion);
	toFindUnit->SetNewCode();
	toFindUnit->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onReadDescrStrat patch" << endl;
	onReadDescrStrat* toReadDescrStrat = new onReadDescrStrat(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toReadDescrStrat->SetNewCode();
	toReadDescrStrat->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onSpawnBrigands patch" << endl;
	onSpawnBrigands* toSpawnBrigands = new onSpawnBrigands(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnBrigands->SetNewCode();
	toSpawnBrigands->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onSpawnBrigands2 patch" << endl;
	onSpawnBrigands2* toSpawnBrigands2 = new onSpawnBrigands2(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnBrigands2->SetNewCode();
	toSpawnBrigands2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onCustomBattleCost patch" << endl;
	onCustomBattleCost* toCustomBattleCost = new onCustomBattleCost(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toCustomBattleCost->SetNewCode();
	toCustomBattleCost->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying spawnUnitsSettGift patch" << endl;
	spawnUnitsSettGift* toSpawnUnitsSettGift = new spawnUnitsSettGift(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift->SetNewCode();
	toSpawnUnitsSettGift->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying spawnUnitsSettGift2 patch" << endl;
	spawnUnitsSettGift2* toSpawnUnitsSettGift2 = new spawnUnitsSettGift2(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift2->SetNewCode();
	toSpawnUnitsSettGift2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying spawnUnitsSettGift3 patch" << endl;
	spawnUnitsSettGift3* toSpawnUnitsSettGift3 = new spawnUnitsSettGift3(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift3->SetNewCode();
	toSpawnUnitsSettGift3->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onReadDescrRebel patch" << endl;
	onReadDescrRebel* toReadDescrRebel = new onReadDescrRebel(mem, (LPVOID)patchesForGame::onReadDescrRebel, globals::dataS.gameVersion);
	toReadDescrRebel->SetNewCode();
	toReadDescrRebel->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onCustomBattleUnitCards patch" << endl;
	onCustomBattleUnitCards* toCustomBattleUnitCards = new onCustomBattleUnitCards(mem, (LPVOID)patchesForGame::onCustomBattleUnitCards, globals::dataS.gameVersion);
	toCustomBattleUnitCards->SetNewCode();
	toCustomBattleUnitCards->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onCustomBattleUnits patch" << endl;
	onCustomBattleUnits* toCustomBattleUnits = new onCustomBattleUnits(mem, (LPVOID)patchesForGame::onCustomBattleUnits, globals::dataS.gameVersion);
	toCustomBattleUnits->SetNewCode();
	toCustomBattleUnits->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onPopActionMem patch" << endl;
	onPopActionMem* toPopActionMem = new onPopActionMem(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toPopActionMem->SetNewCode();
	toPopActionMem->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onGetUnitInfoCard patch" << endl;
	onGetUnitInfoCard* toGetUnitInfoCard = new onGetUnitInfoCard(mem, (LPVOID)patchesForGame::onGetUnitInfoCard, globals::dataS.gameVersion);
	toGetUnitInfoCard->SetNewCode();
	toGetUnitInfoCard->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onGetUnitCard patch" << endl;
	onGetUnitCard* toGetUnitCard = new onGetUnitCard(mem, (LPVOID)patchesForGame::onGetUnitCard, globals::dataS.gameVersion);
	toGetUnitCard->SetNewCode();
	toGetUnitCard->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onCalculateMiningIncome patch" << endl;
	onCalculateMiningIncome* toCalculateMiningIncome = new onCalculateMiningIncome(mem, (LPVOID)patchesForGame::onCalculateMiningIncome, globals::dataS.gameVersion);
	toCalculateMiningIncome->SetNewCode();
	toCalculateMiningIncome->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onNewGameLoaded patch" << endl;
	onNewGameLoaded* toNewGameLoaded = new onNewGameLoaded(mem, (LPVOID)patchesForGame::onNewGameLoaded, globals::dataS.gameVersion);
	toNewGameLoaded->SetNewCode();
	toNewGameLoaded->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onBattleRams patch" << endl;
	onBattleRams* toBattleRams = new onBattleRams(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toBattleRams->SetNewCode();
	toBattleRams->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onPreBattlePlacement patch" << endl;
	onPreBattlePlacement* toPreBattlePlacement = new onPreBattlePlacement(mem, (LPVOID)patchesForGame::onPreBattlePlacement, globals::dataS.gameVersion);
	toPreBattlePlacement->SetNewCode();
	toPreBattlePlacement->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onDecideRamAttacks patch" << endl;
	onDecideRamAttacks* toDecideRamAttacks = new onDecideRamAttacks(mem, (LPVOID)patchesForGame::onDecideRamAttacks, globals::dataS.gameVersion);
	toDecideRamAttacks->SetNewCode();
	toDecideRamAttacks->Enable();
	f1 << "Done" << endl;
	

	f1 << "Start applying spawnUnitsSettGift4 patch" << endl;
	spawnUnitsSettGift4* toSpawnUnitsSettGift4 = new spawnUnitsSettGift4(mem, (LPVOID)patchesForGame::recruitEopUnit, globals::dataS.gameVersion);
	toSpawnUnitsSettGift4->SetNewCode();
	toSpawnUnitsSettGift4->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toCreateMercUnit patch" << endl;
	OnCreateMercUnit* toCreateMercUnit = new OnCreateMercUnit(mem, (LPVOID)patchesForGame::onCreateMercUnit, globals::dataS.gameVersion);
	toCreateMercUnit->SetNewCode();
	toCreateMercUnit->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onEvaluateUnit patch" << endl;
	onEvaluateUnit* toEvaluateUnit = new onEvaluateUnit(mem, (LPVOID)patchesForGame::onEvaluateUnit, globals::dataS.gameVersion);
	toEvaluateUnit->SetNewCode();
	toEvaluateUnit->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onEvaluateUnit2 patch" << endl;
	onEvaluateUnit2* toEvaluateUnit2 = new onEvaluateUnit2(mem, (LPVOID)patchesForGame::onEvaluateUnit2, globals::dataS.gameVersion);
	toEvaluateUnit2->SetNewCode();
	toEvaluateUnit2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toAddCultureToArray patch" << endl;
	onAddCultureToArray* toAddCultureToArray = new onAddCultureToArray(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAddCultureToArray->SetNewCode();
	toAddCultureToArray->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toParseBuildingPics patch" << endl;
	onParseBuildingPics* toParseBuildingPics = new onParseBuildingPics(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toParseBuildingPics->SetNewCode();
	toParseBuildingPics->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying onParseBuildingLevelPics patch" << endl;
	onParseBuildingLevelPics* toParseBuildingLevelPics = new onParseBuildingLevelPics(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toParseBuildingLevelPics->SetNewCode();
	toParseBuildingLevelPics->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toParseBuildingPics patch" << endl;
	onParseEventBodies* toParseEventBodies = new onParseEventBodies(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toParseEventBodies->SetNewCode();
	toParseEventBodies->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onCreatePortraitDb patch" << endl;
	onCreatePortraitDb* toCreatePortraitDb = new onCreatePortraitDb(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toCreatePortraitDb->SetNewCode();
	toCreatePortraitDb->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying toAssignGateAttacker patch" << endl;
	onAssignGateAttacker* toAssignGateAttacker = new onAssignGateAttacker(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAssignGateAttacker->SetNewCode();
	toAssignGateAttacker->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onAttackGate patch" << endl;
	onAttackGate2* toAttackGate2 = new onAttackGate2(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toAttackGate2->SetNewCode();
	toAttackGate2->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onAttackGate patch" << endl;
	onAttackGate* toAttackGate = new onAttackGate(mem, (LPVOID)patchesForGame::onAttackGate, globals::dataS.gameVersion);
	toAttackGate->SetNewCode();
	toAttackGate->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onSetExtentsTexture patch" << endl;
	onSetExtentsTexture* toSetExtentsTexture = new onSetExtentsTexture(mem, (LPVOID)patchesForGame::onSetExtentsTexture, globals::dataS.gameVersion);
	toSetExtentsTexture->SetNewCode();
	toSetExtentsTexture->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onSetExtentsTextureBorder patch" << endl;
	onSetExtentsTextureBorder* toSetExtentsTextureBorder = new onSetExtentsTextureBorder(mem, (LPVOID)patchesForGame::onSetExtentsBorder, globals::dataS.gameVersion);
	toSetExtentsTextureBorder->SetNewCode();
	toSetExtentsTextureBorder->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onSetExtentsTextureZOCAlpha patch" << endl;
	onSetExtentsTextureZOCAlpha* toSetExtentsTextureZOCAlpha = new onSetExtentsTextureZOCAlpha(mem, (LPVOID)patchesForGame::onSetExtentsZocAlpha, globals::dataS.gameVersion);
	toSetExtentsTextureZOCAlpha->SetNewCode();
	toSetExtentsTextureZOCAlpha->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onSetExtentsTextureZOC patch" << endl;
	onSetExtentsTextureZOC* toSetExtentsTextureZOC = new onSetExtentsTextureZOC(mem, (LPVOID)patchesForGame::onSetExtentsZoc, globals::dataS.gameVersion);
	toSetExtentsTextureZOC->SetNewCode();
	toSetExtentsTextureZOC->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onCreateProductionController patch" << endl;
	onCreateProductionController* toCreateProductionController = new onCreateProductionController(mem, (LPVOID)patchesForGame::onCreateProductionController, globals::dataS.gameVersion);
	toCreateProductionController->SetNewCode();
	toCreateProductionController->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onDecideNeighbours patch" << endl;
	onDecideNeighbours* toDecideNeighbours = new onDecideNeighbours(mem, (LPVOID)patchesForGame::onDecideNeighbours, globals::dataS.gameVersion);
	toDecideNeighbours->SetNewCode();
	toDecideNeighbours->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onTransferSettlement patch" << endl;
	onTransferSettlement* toTransferSettlement = new onTransferSettlement(mem, (LPVOID)patchesForGame::onTransferSettlement, globals::dataS.gameVersion);
	toTransferSettlement->SetNewCode();
	toTransferSettlement->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onTransferSettlementPort patch" << endl;
	onTransferSettlementPort* toTransferSettlementPort = new onTransferSettlementPort(mem, (LPVOID)patchesForGame::onTransferSettlementPort, globals::dataS.gameVersion);
	toTransferSettlementPort->SetNewCode();
	toTransferSettlementPort->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onCheckConstructionItem patch" << endl;
	onCheckConstructionItem* toCheckConstructionItem = new onCheckConstructionItem(mem, (LPVOID)patchesForGame::onCheckConstructionItem, globals::dataS.gameVersion);
	toCheckConstructionItem->SetNewCode();
	toCheckConstructionItem->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onCreateTakeResidenceObjective patch" << endl;
	onCreateTakeResidenceObjective* toCreateTakeResidenceObjective = new onCreateTakeResidenceObjective(mem, (LPVOID)patchesForGame::onCreateTakeResidenceObjective, globals::dataS.gameVersion);
	toCreateTakeResidenceObjective->SetNewCode();
	toCreateTakeResidenceObjective->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onInitGsd patch" << endl;
	onInitGsd* toInitGsd = new onInitGsd(mem, (LPVOID)patchesForGame::onInitGsd, globals::dataS.gameVersion);
	toInitGsd->SetNewCode();
	toInitGsd->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onInitGsd2 patch" << endl;
	onInitGsd2* toInitGsd2 = new onInitGsd2(mem, (LPVOID)patchesForGame::onInitGsd2, globals::dataS.gameVersion);
	toInitGsd2->SetNewCode();
	toInitGsd2->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onAttachRegionSettlement patch" << endl;
	onAttachRegionSettlement* toAttachRegionSettlement = new onAttachRegionSettlement(mem, (LPVOID)patchesForGame::onAttachRegionSettlement, globals::dataS.gameVersion);
	toAttachRegionSettlement->SetNewCode();
	toAttachRegionSettlement->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onCalculateSettlement patch" << endl;
	onCalculateSettlement* toCalculateSettlement = new onCalculateSettlement(mem, (LPVOID)patchesForGame::onCalculateSettlement, globals::dataS.gameVersion);
	toCalculateSettlement->SetNewCode();
	toCalculateSettlement->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onScoreBestCapital patch" << endl;
	onScoreBestCapital* toScoreBestCapital = new onScoreBestCapital(mem, (LPVOID)patchesForGame::onScoreBestCapital, globals::dataS.gameVersion);
	toScoreBestCapital->SetNewCode();
	toScoreBestCapital->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onPredictedStats patch" << endl;
	onPredictedStats* toPredictedStats = new onPredictedStats(mem, (LPVOID)patchesForGame::onPredictedStats, globals::dataS.gameVersion);
	toPredictedStats->SetNewCode();
	toPredictedStats->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onEvalAttObjective patch" << endl;
	onEvalAttObjective* toEvalAttObjective = new onEvalAttObjective(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective->SetNewCode();
	toEvalAttObjective->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onEvalAttObjective2 patch" << endl;
	onEvalAttObjective2* toEvalAttObjective2 = new onEvalAttObjective2(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective2->SetNewCode();
	toEvalAttObjective2->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onEvalAttObjective patch" << endl;
	onEvalAttObjective3* toEvalAttObjective3 = new onEvalAttObjective3(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective3->SetNewCode();
	toEvalAttObjective3->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onEvalAttObjective patch" << endl;
	onEvalAttObjective4* toEvalAttObjective4 = new onEvalAttObjective4(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective4->SetNewCode();
	toEvalAttObjective4->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onEvalAttObjective patch" << endl;
	onEvalAttObjective5* toEvalAttObjective5 = new onEvalAttObjective5(mem, (LPVOID)patchesForGame::onEvalAttObjective, globals::dataS.gameVersion);
	toEvalAttObjective5->SetNewCode();
	toEvalAttObjective5->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onUpdateControllerAlloc patch" << endl;
	onUpdateControllerAlloc* toUpdateControllerAlloc = new onUpdateControllerAlloc(mem, (LPVOID)patchesForGame::onUpdateControllerAlloc, globals::dataS.gameVersion);
	toUpdateControllerAlloc->SetNewCode();
	toUpdateControllerAlloc->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onAssessRequiredStrength patch" << endl;
	onAssessRequiredStrength* toAssessRequiredStrength = new onAssessRequiredStrength(mem, (LPVOID)patchesForGame::onAssessRequiredStrength, globals::dataS.gameVersion);
	toAssessRequiredStrength->SetNewCode();
	toAssessRequiredStrength->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onRegionGroupStuff patch" << endl;
	onRegionGroupStuff* toRegionGroupStuff = new onRegionGroupStuff(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toRegionGroupStuff->SetNewCode();
	toRegionGroupStuff->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onDrawBanner patch" << endl;
	onDrawBanner* toDrawBanner = new onDrawBanner(mem, (LPVOID)patchesForGame::onDrawBanner, globals::dataS.gameVersion);
	toDrawBanner->SetNewCode();
	toDrawBanner->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onSetupBattleFromStrat patch" << endl;
	onSetupBattleFromStrat* toSetupBattleFromStrat = new onSetupBattleFromStrat(mem, (LPVOID)noFunc, globals::dataS.gameVersion);
	toSetupBattleFromStrat->SetNewCode();
	toSetupBattleFromStrat->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetRebelSymbol patch" << endl;
	onGetRebelSymbol* toGetRebelSymbol = new onGetRebelSymbol(mem, (LPVOID)patchesForGame::onGetRebelSymbol, globals::dataS.gameVersion);
	toGetRebelSymbol->SetNewCode();
	toGetRebelSymbol->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetSupportingArmies patch" << endl;
	onGetSupportingArmies* toGetSupportingArmies = new onGetSupportingArmies(mem, (LPVOID)patchesForGame::onGetSupportingArmies, globals::dataS.gameVersion);
	toGetSupportingArmies->SetNewCode();
	toGetSupportingArmies->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetSupportingArmies2 patch" << endl;
	onGetSupportingArmies2* toGetSupportingArmies2 = new onGetSupportingArmies2(mem, (LPVOID)patchesForGame::onGetSupportingArmies, globals::dataS.gameVersion);
	toGetSupportingArmies2->SetNewCode();
	toGetSupportingArmies2->Enable();
	f1 << "Done" << endl;
	
	
	f1 << "Start applying onCalcGarrisonStr patch" << endl;
	onCalcGarrisonStr* toCalcGarrisonStr = new onCalcGarrisonStr(mem, (LPVOID)patchesForGame::onCalcGarrisonStr, globals::dataS.gameVersion);
	toCalcGarrisonStr->SetNewCode();
	toCalcGarrisonStr->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetBrowserPicConstructed patch" << endl;
	onGetBrowserPicConstructed* toGetBrowserPicConstructed= new onGetBrowserPicConstructed(mem, (LPVOID)patchesForGame::getBrowserPicConstructed, globals::dataS.gameVersion);
	toGetBrowserPicConstructed->SetNewCode();
	toGetBrowserPicConstructed->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetBrowserPicConstruction patch" << endl;
	onGetBrowserPicConstruction* toGetBrowserPicConstruction= new onGetBrowserPicConstruction(mem, (LPVOID)patchesForGame::getBrowserPicConstruction, globals::dataS.gameVersion);
	toGetBrowserPicConstruction->SetNewCode();
	toGetBrowserPicConstruction->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetBuildingPic patch" << endl;
	onGetBuildingPic* toGetBuildingPic = new onGetBuildingPic(mem, (LPVOID)patchesForGame::getBuildingPic, globals::dataS.gameVersion);
	toGetBuildingPic->SetNewCode();
	toGetBuildingPic->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetGuildOfferPic patch" << endl;
	onGetGuildOfferPic* toGetGuildOfferPic = new onGetGuildOfferPic(mem, (LPVOID)patchesForGame::onGetGuildOfferPic, globals::dataS.gameVersion);
	toGetGuildOfferPic->SetNewCode();
	toGetGuildOfferPic->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetCultureEndTurnSound patch" << endl;
	onGetCultureEndTurnSound* toGetCultureEndTurnSound = new onGetCultureEndTurnSound(mem, (LPVOID)patchesForGame::onGetCultureEndTurnSound, globals::dataS.gameVersion);
	toGetCultureEndTurnSound->SetNewCode();
	toGetCultureEndTurnSound->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetBuildingPic2 patch" << endl;
	onGetBuildingPic2* toGetBuildingPic2 = new onGetBuildingPic2(mem, (LPVOID)patchesForGame::getBuildingPic, globals::dataS.gameVersion);
	toGetBuildingPic2->SetNewCode();
	toGetBuildingPic2->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetBuildingPicConstruction patch" << endl;
	onGetBuildingPicConstruction* toGetBuildingPicConstruction = new onGetBuildingPicConstruction(mem, (LPVOID)patchesForGame::getBuildingPicConstruction, globals::dataS.gameVersion);
	toGetBuildingPicConstruction->SetNewCode();
	toGetBuildingPicConstruction->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetBuildingPicConstructed patch" << endl;
	onGetBuildingPicConstructed* toGetBuildingPicConstructed = new onGetBuildingPicConstructed(mem, (LPVOID)patchesForGame::getBuildingPicConstructed, globals::dataS.gameVersion);
	toGetBuildingPicConstructed->SetNewCode();
	toGetBuildingPicConstructed->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying toGetBuildingPicConstructed2 patch" << endl;
	onGetBrowserPicConstructed2* toGetBuildingPicConstructed2 = new onGetBrowserPicConstructed2(mem, (LPVOID)patchesForGame::getBuildingPicConstructed, globals::dataS.gameVersion);
	toGetBuildingPicConstructed2->SetNewCode();
	toGetBuildingPicConstructed2->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onGetPortraitDbEntry patch" << endl;
	onGetPortraitDbEntry* toGetPortraitDbEntry = new onGetPortraitDbEntry(mem, (LPVOID)patchesForGame::getPortraitDbEntry, globals::dataS.gameVersion);
	toGetPortraitDbEntry->SetNewCode();
	toGetPortraitDbEntry->Enable();
	f1 << "Done" << endl;
	
	f1 << "Start applying onAiTurn patch" << endl;
	onAiTurn* toAiTurn = new onAiTurn(mem, (LPVOID)patchesForGame::onAiTurn, globals::dataS.gameVersion);
	toAiTurn->SetNewCode();
	toAiTurn->Enable();
	f1 << "Done" << endl;

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

	f1 << "Start applying OnCreateUnitWrapper patch" << endl;
	OnCreateUnitWrapper* toCreateUnitWrapper = new OnCreateUnitWrapper(mem, (LPVOID)patchesForGame::onCreateUnitWrapper, globals::dataS.gameVersion);
	toCreateUnitWrapper->SetNewCode();
	toCreateUnitWrapper->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnFindUnitStrings patch" << endl;
	OnFindUnitStrings* toFindUnitStrings = new OnFindUnitStrings(mem, (LPVOID)patchesForGame::onCreateUnitWrapper, globals::dataS.gameVersion); //its same func as unit wrapper on purpose not error
	toFindUnitStrings->SetNewCode();
	toFindUnitStrings->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toFindUnitStrings2 patch" << endl;
	OnFindUnitStrings2* toFindUnitStrings2 = new OnFindUnitStrings2(mem, (LPVOID)patchesForGame::onCreateUnitWrapper, globals::dataS.gameVersion); //its same func as unit wrapper on purpose not error
	toFindUnitStrings2->SetNewCode();
	toFindUnitStrings2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnReligionCombatBonus patch" << endl;
	OnReligionCombatBonus* toReligionCombatBonus = new OnReligionCombatBonus(mem, (LPVOID)patchesForGame::onReligionCombatBonus, globals::dataS.gameVersion);
	toReligionCombatBonus->SetNewCode();
	toReligionCombatBonus->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toOnBattleArmiesBug patch" << endl;
	OnBattleArmiesBug* toOnBattleArmiesBug = new OnBattleArmiesBug(mem, nullptr, globals::dataS.gameVersion);
	toOnBattleArmiesBug->SetNewCode();
	toOnBattleArmiesBug->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnGetRecruitPoolUnitEntry patch" << endl;
	OnGetRecruitPoolUnitEntry* toGetRecruitPoolUnitEntry = new OnGetRecruitPoolUnitEntry(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toGetRecruitPoolUnitEntry->SetNewCode();
	toGetRecruitPoolUnitEntry->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toLoadRecruitQueue patch" << endl;
	loadRecruitQueue* toLoadRecruitQueue = new loadRecruitQueue(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toLoadRecruitQueue->SetNewCode();
	toLoadRecruitQueue->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toLoadRecruitQueue2 patch" << endl;
	loadRecruitQueue2* toLoadRecruitQueue2 = new loadRecruitQueue2(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toLoadRecruitQueue2->SetNewCode();
	toLoadRecruitQueue2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnGetRecruitPoolUnitEntry2 patch" << endl;
	OnGetRecruitPoolUnitEntry2* toGetRecruitPoolUnitEntry2 = new OnGetRecruitPoolUnitEntry2(mem, (LPVOID)patchesForGame::onGetRecruitPoolUnitEntry, globals::dataS.gameVersion);
	toGetRecruitPoolUnitEntry2->SetNewCode();
	toGetRecruitPoolUnitEntry2->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toCreateMercUnitCheck patch" << endl;
	OnCreateMercUnitCheck* toCreateMercUnitCheck = new OnCreateMercUnitCheck(mem, (LPVOID)patchesForGame::onCreateMercUnitCheck, globals::dataS.gameVersion);
	toCreateMercUnitCheck->SetNewCode();
	toCreateMercUnitCheck->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnQuickSave patch" << endl;
	OnQuickSave* onQuickSave = new OnQuickSave(mem, (LPVOID)patchesForGame::onQuickSave, globals::dataS.gameVersion);
	onQuickSave->SetNewCode();
	onQuickSave->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnAutoSave patch" << endl;
	OnAutoSave* onAutoSave = new OnAutoSave(mem, (LPVOID)patchesForGame::onAutoSave, globals::dataS.gameVersion);
	onAutoSave->SetNewCode();
	onAutoSave->Enable();
	f1 << "Done" << endl;


	OnPathCasheCrashPlace* onPathCasheCrashPlace = new OnPathCasheCrashPlace(mem, (LPVOID)&tacticalMapViewer::getPathCache, globals::dataS.gameVersion, (LPVOID)&globals::dataS.Modules.tacticalMapViewer);
	onPathCasheCrashPlace->SetNewCode();
	onPathCasheCrashPlace->Enable();

	toSelectForDrawPortsCas* onSelectForDrawPortsCas = new toSelectForDrawPortsCas(mem, (LPVOID)casModelsDrawer::drawModels, globals::dataS.gameVersion);
	onSelectForDrawPortsCas->SetNewCode();
	onSelectForDrawPortsCas->Enable();

	toStartOfDrawFunction* onStartOfDrawFunction = new toStartOfDrawFunction(mem, (LPVOID)patchesForGame::onStartOfDrawFunction, globals::dataS.gameVersion);
	onStartOfDrawFunction->SetNewCode();
	onStartOfDrawFunction->Enable();


	if (battleCreator::IsRetreatsOverhaulEnabled())
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
	initLuaPlugin();
	globals::dataS.Modules.tacticalMapViewer.init(globals::dataS.gameVersion);
}

EOP_EXPORT void managerExport::initEOP(const char* modPath, const int gameVer)
{
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		return;
	globals::dataS.gameVersion = gameVer;
	globals::dataS.modPath = modPath;
	CreateDirectoryA("logs", nullptr);
	managerF::initThread();
}
