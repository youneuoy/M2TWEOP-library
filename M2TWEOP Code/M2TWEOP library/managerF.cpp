#include "pch.h"
#include "managerF.h"

#include "onlineThings.h"
#include "settlementConversionLvlSetter.h"
void managerF::init()
{

	initThread();
}


//apply code injects
void managerF::doPachs()
{
	MemWork* mem;
	ofstream f1("logs\\applyingPatches.youneuoylog");
	f1 << "Log:" << endl;
	mem = new MemWork();

	//f1 << "Start applying age patch" << endl;
/*	f1 << "first step" << endl;
	Age* inj = new Age(mem, (LPVOID)ageFunc, globals::dataS.gamever);
	f1 << "step 2" << endl;
	inj->SetlAgeCode();
	f1 << "step 3" << endl;
	inj->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying visual age patch" << endl;

	AgeVisual* agV = new AgeVisual(mem, (LPVOID)ageVisualFunc, globals::dataS.gamever);
	agV->SetlAgeCode();
	agV->Enable();
	f1 << "Done" << endl;*/


	f1 << "Start applying berserkers patch" << endl;

	berserkersP* brs = new berserkersP(mem, globals::dataS.gamever);
	brs->SetlBersCode();
	brs->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying history battles soldier patch" << endl;
	{
		unsigned char disSold[2] = { 0x90, 0x90 };
		DWORD nPtr;
		DWORD nbPtr;


		if (globals::dataS.gamever == 2)
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
	{
		unsigned char ret[1] = { 0xC3};
		DWORD nPtr;


		if (globals::dataS.gamever == 2)
		{

			nPtr = 0x00ab5170;

		}
		else
		{
			nPtr = 0x00ab4140;

		}

		MemWork::WriteData(ret, nPtr,1);
	}
	f1 << "Done" << endl;


	f1 << "Start applying toLoadDescrBattleCharacter patch #1" << endl;

	toLoadDescrBattleCharacter* toDescrChar = new toLoadDescrBattleCharacter(mem, (LPVOID)patchesForGame::onLoadDescrBattleCharacter, globals::dataS.gamever);
	toDescrChar->SetlBattleCode();
	toDescrChar->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying toBattleStateResults patch #1" << endl;

	toBattleStateResults* toBattleStats = new toBattleStateResults(mem, (LPVOID)patchesForGame::onBattleStateChange, globals::dataS.gamever);
	toBattleStats->SetlBattleCode();
	toBattleStats->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying unit types patch #1" << endl;

	toReadEDU* ed = new toReadEDU(mem, (LPVOID)patchesForGame::afterEDUread, globals::dataS.gamever);
	ed->SetlEDUCode();
	ed->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnChangePassedTurnsCount patch" << endl;

	toChangePassedTurnsCount* tcptc = new toChangePassedTurnsCount(mem, (LPVOID)patchesForGame::onChangeTurnNum, globals::dataS.gamever);
	tcptc->SetlTurnsCountCode();
	tcptc->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying label script patch" << endl;
	toCheckLabelResults* labResults = new toCheckLabelResults(mem, (LPVOID)patchesForGame::onGiveTrait, globals::dataS.gamever);
	labResults->SetlCheckLabCode();
	labResults->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying start campaign and battle start patch" << endl;
	toCreateMap* stCampaign = new toCreateMap(mem, (LPVOID)patchesForGame::afterLoadGameData, globals::dataS.gamever);
	stCampaign->SetlCreateCode();
	stCampaign->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying I_CompareCounter command patch" << endl;
	toI_CompareCounter* iccmp = new toI_CompareCounter(mem, (LPVOID)patchesForGame::afterI_CompareCounter, globals::dataS.gamever);
	iccmp->SetltoI_CompareCounterCode();
	iccmp->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying fort patch" << endl;
	toFortsDelCheck* frt = new toFortsDelCheck(mem, (LPVOID)patchesForGame::checkFort, globals::dataS.gamever);
	frt->SetlFortsDelCode();
	frt->Enable();

	f1 << "Done" << endl;

	/*f1 << "Start applying DiplomaticStanceFromFaction patch #1" << endl;
	toDSFromFacCh* dsff1 = new toDSFromFacCh(mem, (LPVOID)patchesForGame::checkDipString, globals::dataS.gamever);
	dsff1->SetlDsFromFacCode();
	dsff1->Enable();

	f1 << "Done" << endl;*/

	/*f1 << "Start applying DiplomaticStanceFromFaction patch #2" << endl;
	toDiplomaticStanceCheck* dsff2 = new toDiplomaticStanceCheck(mem, (LPVOID)patchesForGame::checkFacDip, globals::dataS.gamever);
	dsff2->SetlDsCheckCode();
	dsff2->Enable();

	f1 << "Done" << endl;*/


	f1 << "Start applying spawn_army coords patch" << endl;
	toSpawnArmy* spwnArm = new toSpawnArmy(mem, (LPVOID)patchesForGame::spawnArmyCoords, globals::dataS.gamever);
	spwnArm->SetlSpawnCode();
	spwnArm->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying spawn_army coords patch end" << endl;
	toEndOfSpawnArmy* endSpwnArm = new toEndOfSpawnArmy(mem, (LPVOID)patchesForGame::spawnEndArmyCoords, globals::dataS.gamever);
	endSpwnArm->SetlEndSpawnCode();
	endSpwnArm->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying spawn_character coords patch" << endl;
	toSpawnCharacter* spwnChar = new toSpawnCharacter(mem, (LPVOID)patchesForGame::spawnCharacterCoords, globals::dataS.gamever);
	spwnChar->SetlSpawnCode();
	spwnChar->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying battle_start patch" << endl;
	toBattleStratScreen* bstart = new toBattleStratScreen(mem, (LPVOID)patchesForGame::onBattleStratScreen, globals::dataS.gamever);
	bstart->SetlBSCode();
	bstart->Enable();

	f1 << "Done" << endl;



	/*f1 << "Start applying strat models onload patch" << endl;
	toStartReadCas* readCas = new toStartReadCas(mem, (LPVOID)stratModelsPatch::readModel, globals::dataS.gamever);
	readCas->SetlStReadCasCode();
	readCas->Enable();

	f1 << "Done" << endl;*/


	f1 << "Start applying models load patch #1" << endl;
	toStartReadModels* readModelsStart = new toStartReadModels(mem, (LPVOID)stratModelsChange::disableChecker, globals::dataS.gamever);
	readModelsStart->SetlStReadModelsCode();
	readModelsStart->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying unical strat models patch" << endl;
	toResourcesReadModels* readResModels = new toResourcesReadModels(mem, (LPVOID)patchesForGame::unicalStratModsRead, globals::dataS.gamever);
	readResModels->SetlResModelsCode();
	readResModels->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying onEvents patch" << endl;
	toEvents* evnts = new toEvents(mem, (LPVOID)patchesForGame::onEvent, globals::dataS.gamever);
	evnts->SetlEventsCode();
	evnts->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying custom_tiles patch" << endl;
	toCustomTileSelection* ctiles = new toCustomTileSelection(mem, (LPVOID)patchesForGame::onTileCheck, globals::dataS.gamever);
	ctiles->SetlTilesCode();
	ctiles->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying custom_tiles file patch" << endl;
	toCustomTileFileRead* ctilesF = new toCustomTileFileRead(mem, (LPVOID)patchesForGame::readTilesFile, globals::dataS.gamever);
	ctilesF->SetlTilesCode();
	ctilesF->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying start fort models patch" << endl;
	toStratModelsSelect* stratModSel = new toStratModelsSelect(mem, (LPVOID)patchesForGame::checkAndChangeModels, globals::dataS.gamever);
	stratModSel->SetlStratModelsCode();
	stratModSel->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying load save patch" << endl;
	toLoadSaveFile* loadSave = new toLoadSaveFile(mem, (LPVOID)patchesForGame::onLoadSaveFile, globals::dataS.gamever);
	loadSave->SetlLoadCode();
	loadSave->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying save game patch" << endl;
	toSaveGame* saveGame = new toSaveGame(mem, (LPVOID)patchesForGame::onSaveGame, globals::dataS.gamever);
	saveGame->SetlSaveCode();
	saveGame->Enable();

	f1 << "Done" << endl;



	f1 << "Start applying new game patch" << endl;
	toStartNewGame* newGame = new toStartNewGame(mem, (LPVOID)patchesForGame::onNewGameStart, globals::dataS.gamever);
	newGame->SetlStartCode();
	newGame->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying strategical map loading patch" << endl;
	toEndReadModels* stratMapLoaded = new toEndReadModels(mem, (LPVOID)patchesForGame::afterCampaignMapLoaded, globals::dataS.gamever);
	stratMapLoaded->SetlEndReadModelsCode();
	stratMapLoaded->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying click on tile patch" << endl;
	toClickAtCoords* clickAtTile = new toClickAtCoords(mem, (LPVOID)patchesForGame::clickAtTile, globals::dataS.gamever);
	clickAtTile->SetlClickCode();
	clickAtTile->Enable();

	f1 << "Done" << endl;

	f1 << "Start applying select worldpkgdesc from db patch" << endl;
	toSelectWorldFromDB* selWorld = new toSelectWorldFromDB(mem, (LPVOID)patchesForGame::selectWorldpkgdesc, globals::dataS.gamever);
	selWorld->SetlSelectCode();
	selWorld->Enable();

	f1 << "Done" << endl;



	f1 << "Start applying battle loaded patch" << endl;
	toBattleLoaded* battleLoad = new toBattleLoaded(mem, (LPVOID)patchesForGame::battleLoaded, globals::dataS.gamever);
	battleLoad->SetlLoadCode();
	battleLoad->Enable();

	f1 << "Done" << endl;


	f1 << "Start applying toReadGameDBsAtGameStart patch" << endl;
	toReadGameDBsAtGameStart* toReadGameDbsAtStart = new toReadGameDBsAtGameStart(mem, (LPVOID)patchesForGame::toReadGameDbsAtStart, globals::dataS.gamever);
	toReadGameDbsAtStart->SetlReadCode();
	toReadGameDbsAtStart->Enable();

	f1 << "Done" << endl;
	


	f1 << "Start applying toDrawPartsOfStratObjects patch" << endl;
	toDrawPartsOfStratObjects* toDrawPartsOfStratO = new toDrawPartsOfStratObjects(mem, (LPVOID)patchesForGame::onDrawPartsOfStratObjects, globals::dataS.gamever);
	toDrawPartsOfStratO->SetlDrawCode();
	toDrawPartsOfStratO->Enable();

	f1 << "Start applying toGameFormDrawImage patch" << endl;
	toGameFormDrawImage* toDrawAllSt = new toGameFormDrawImage(mem, (LPVOID)patchesForGame::onDrawAll, globals::dataS.gamever);
	toDrawAllSt->SetlDrawCode();
	toDrawAllSt->Enable();
	

	f1 << "Start applying toEndSettlementSiege patch" << endl;
	toEndSettlementSiege* toEndSiegSett = new toEndSettlementSiege(mem, (LPVOID)patchesForGame::onEndSiege, globals::dataS.gamever);
	toEndSiegSett->SetlSiegeCode();
	toEndSiegSett->Enable();	

	f1 << "Start applying toStartSettlementSiege patch" << endl;
	toStartSettlementSiege* toStartSiegSett = new toStartSettlementSiege(mem, (LPVOID)patchesForGame::onStartSiege, globals::dataS.gamever);
	toStartSiegSett->SetlSiegeCode();
	toStartSiegSett->Enable();

	f1 << "Done" << endl;

	/*f1 << "Start replacing cas loading function" << endl;
	toLoadCas* loadCasF = new toLoadCas(mem, (LPVOID)stratModelsOptimise::loadCasProc, globals::dataS.gamever);
	loadCasF->SetlCasCode();
	loadCasF->Enable(true);

	f1 << "Done" << endl;*/


	f1 << "Start applying CastleConversionLvl patch" << endl;
	CastleConversionLvlSetter* castleConversionLvlSetter = new CastleConversionLvlSetter(mem, (LPVOID)settlementConversionLvlSetter::getConversionLvlFromCastle, globals::dataS.gamever);
	castleConversionLvlSetter->SetNewCode();
	castleConversionLvlSetter->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying CityConversionLvl patch" << endl;
	CityConversionLvlSetter* cityConversionLvlSetter = new CityConversionLvlSetter(mem, (LPVOID)settlementConversionLvlSetter::getConversionLvlFromCity, globals::dataS.gamever);
	cityConversionLvlSetter->SetNewCode();
	cityConversionLvlSetter->Enable();
	f1 << "Done" << endl;



	f1 << "Start applying mercenaryMovepointsGetGeneral patch" << endl;
	mercenaryMovepointsGetGeneral* mercP = new mercenaryMovepointsGetGeneral(mem, (LPVOID)patchesForGame::mercenaryMovepointsGetGeneral, globals::dataS.gamever);
	mercP->SetNewCode();
	mercP->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying onGameConsoleCommandFromConsole patch" << endl;
	onGameConsoleCommandFromConsole* cons1 = new onGameConsoleCommandFromConsole(mem, (LPVOID)patchessForConsole::onGameConsoleCommandFromConsole, globals::dataS.gamever);
	cons1->SetNewCode();
	cons1->Enable();
	f1 << "Done" << endl;
	

	f1 << "Start applying onGameConsoleCommandFromScript patch" << endl;
	onGameConsoleCommandFromScript* cons2 = new onGameConsoleCommandFromScript(mem, (LPVOID)patchessForConsole::onGameConsoleCommandFromScript, globals::dataS.gamever);
	cons2->SetNewCode();
	cons2->Enable();
	f1 << "Done" << endl;	


	f1 << "Start applying onGameConsoleCommandFromScript patch" << endl;
	OnReadLogonOrLogoff* cons3 = new OnReadLogonOrLogoff(mem, (LPVOID)patchessForConsole::OnReadLogonOrLogoff, globals::dataS.gamever);
	cons3->SetNewCode();
	cons3->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnLoadSettlementWorldpkgdesc patch" << endl;
	OnLoadSettlementWorldpkgdesc* loadWordSet = new OnLoadSettlementWorldpkgdesc(mem, (LPVOID)patchesForGame::OnLoadSettlementWorldpkgdesc, globals::dataS.gamever);
	loadWordSet->SetNewCode();
	loadWordSet->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying fortificationlevelS patch" << endl;
	fortificationlevelS* fortificationlevel = new fortificationlevelS(mem, (LPVOID)patchesForGame::onfortificationlevelS, globals::dataS.gamever);
	fortificationlevel->SetNewCode();
	fortificationlevel->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying LimitRecruitmentQueueToSlots patch" << endl;
	LimitRecruitmentQueueToSlotsInj* limitRecr = new LimitRecruitmentQueueToSlotsInj(mem, &globals::dataS.eopGameplaySettings.bLimitRecruitmentToSlotsAndByMoney, globals::dataS.gamever);
	limitRecr->SetNewCode();
	limitRecr->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnSaveEDUStringS patch" << endl;
	OnSaveEDUStringS* eduStringsSaveOn = new OnSaveEDUStringS(mem, (LPVOID)patchesForGame::onSaveEDUStringS, globals::dataS.gamever);
	eduStringsSaveOn->SetNewCode();
	eduStringsSaveOn->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnSaveEDUStringS2 patch" << endl;
	OnSaveEDUStringS2* eduStringsSaveOn2 = new OnSaveEDUStringS2(mem, (LPVOID)patchesForGame::onSaveEDUStringS, globals::dataS.gamever);
	eduStringsSaveOn2->SetNewCode();
	eduStringsSaveOn2->Enable();
	f1 << "Done" << endl;


	f1 << "Start applying OnCreateUnit patch" << endl;
	OnCreateUnit* onCreateUnit = new OnCreateUnit(mem, (LPVOID)patchesForGame::onCreateUnit, globals::dataS.gamever);
	onCreateUnit->SetNewCode();
	onCreateUnit->Enable();
	f1 << "Done" << endl;

	f1 << "Start applying OnQuickSave patch" << endl;
	OnQuickSave* onQuickSave = new OnQuickSave(mem, (LPVOID)patchesForGame::onQuickSave, globals::dataS.gamever);
	onQuickSave->SetNewCode();
	onQuickSave->Enable();
	f1 << "Done" << endl;

	
	f1 << "Start applying OnAutoSave patch" << endl;
	OnAutoSave* onAutoSave = new OnAutoSave(mem, (LPVOID)patchesForGame::onAutoSave, globals::dataS.gamever);
	onAutoSave->SetNewCode();
	onAutoSave->Enable();
	f1 << "Done" << endl;


	if (globals::dataS.gameCfg.isBlockLaunchWithoutEop == true)
	{

		f1 << "Start applying isBlockLaunchWithoutEop patch" << endl;
		blockLaunchWithoutEop* blockLaunch = new blockLaunchWithoutEop(mem, globals::dataS.gamever);
		blockLaunch->SetNewCode();
		blockLaunch->Enable();
		f1 << "Done" << endl;
	}


	f1 << "End." << endl;

	f1.close();
}

jsn::json loadJsonFromFile(const std::string& fpath)
{
	jsn::json json;


	std::ifstream f1(fpath);
	if (f1.is_open())
	{
		f1 >> json;
	}
	f1.close();

	return std::move(json);
}

void loadJsonSettings()
{
	std::string fPath = globals::dataS.modPatch;
	fPath += "\\eopData\\gameCfg.json";
	jsn::json json = loadJsonFromFile(fPath);

	try
	{
		if (json.contains("isBlockLaunchWithoutEop"))
		{
			getJson(globals::dataS.gameCfg.isBlockLaunchWithoutEop, "isBlockLaunchWithoutEop");
		}
	}
	catch (jsn::json::type_error& e)
	{
		MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
	}
}
//#include "tests.h"
void managerF::initThread()
{
	//read_modConfig();
	codes::initCodes(globals::dataS.gamever);
	dataOffsets::initDataOffsets(globals::dataS.gamever);
	//read_limits();



	loadJsonSettings();
	doPachs();

	plugins::init();

	battleCreator::readParams();
	//stratResTest::test();
}


#include "stratModelsOptimise.h"
NOINLINE EOP_EXPORT void managerExport::initEOP(const char* modPath, int gameVer)
{	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return;
	}
	
	globals::dataS.gamever = gameVer;
	globals::dataS.modPatch = modPath;

	CreateDirectoryA("logs", NULL);

	managerF::init();

}
