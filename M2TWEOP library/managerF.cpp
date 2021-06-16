#include "pch.h"
#include "managerF.h"
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

	f1 << "Start applying age patch" << endl;
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

	if (globals::dataS.modConfig.bersPatch == 1) {
		f1 << "Start applying berserkers patch" << endl;

		berserkersP* brs = new berserkersP(mem, globals::dataS.gamever);
		brs->SetlBersCode();
		brs->Enable();
		f1 << "Done" << endl;
	}

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
	toBattleStratScreen* bstart = new toBattleStratScreen(mem, (LPVOID)patchesForGame::checkAndChangeCharacterBattleModels, globals::dataS.gamever);
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



	f1 << "End." << endl;

	f1.close();
}

//#include "tests.h"
void managerF::initThread()
{
	read_modConfig();
	codes::initCodes(globals::dataS.gamever);
	dataOffsets::initDataOffsets(globals::dataS.gamever);
	read_limits();




	doPachs();

	plugins::init();

	//stratResTest::test();
}


bool managerF::read_modConfig()
{
	string s = globals::dataS.modPatch + "\\modConfig.youneuoycfg";
	string ls;
	ifstream f2(s);
	while (f2.good())
	{

		getline(f2, s);

		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.fullLogging, s, "FullLoggingEnabled");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.unlockChangeFaction, s, "UnlockConsoleChangeFaction");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.bersPatch, s, "CorrectionOfBerserkers");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.histBattleFix, s, "HistoryBattlesSoldiersNumberFix");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.DCI, s, "DCI");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.spawnArmyCoords, s, "SpawnArmyCoordsPatch");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.characterUIMenus, s, "CharacterUIMenus");
		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.unlockConsoleCommands, s, "UnlockConsoleCommands");

		technicalStringsFuncs::findOWParam(&globals::dataS.modConfig.maxBodSize, s, "MaxBodyguardSize");

		technicalStringsFuncs::readStringInQuotes(&ls, s, "LegioString");
		technicalStringsFuncs::readStringInQuotes(&globals::dataS.modConfig.langP, s, "Lang");
	}


	f2.close();
	globals::dataS.modConfig.langP = globals::dataS.modPatch + "\\youneuoy_Data\\localisation\\" + globals::dataS.modConfig.langP;

	if (ls.empty())
		ls = "Legio ";
	globals::dataS.modConfig.legioString = new char[ls.length() + 1];
	memcpy(globals::dataS.modConfig.legioString, ls.c_str(), ls.length() + 1);


	return true;
}

bool managerF::read_limits()
{
	string s = globals::dataS.modPatch + "\\limits.youneuoycfg";
	ifstream f1(s);
	while (f1.good())
	{
		getline(f1, s);

		technicalStringsFuncs::findOWParam(&globals::dataS.limits.religionLimit, s, "Religion_Limit");

		technicalStringsFuncs::findOWParam(&globals::dataS.limits.ancillariesLimit, s, "Ancillaries_Limit");

		technicalStringsFuncs::findOWParam(&globals::dataS.limits.lockSizeOfUnit, s, "Lock_Size_Of_Unit");
		technicalStringsFuncs::findOWParam(&globals::dataS.limits.sizeOfUnit, s, "Size_Of_Unit");

		findIntParam(&globals::dataS.limits.minSoldiersEdu, s, "Min_Soldiers_In_Edu");
		findIntParam(&globals::dataS.limits.maxSoldiersEdu, s, "Max_Soldiers_In_Edu");

		technicalStringsFuncs::findFloatParam(&globals::dataS.limits.battlefieldMapSizeX, s, "Battlefield_Map_Size_X");
		technicalStringsFuncs::findFloatParam(&globals::dataS.limits.battlefieldMapSizeY, s, "Battlefield_Map_Size_Y");

		findIntParam(&globals::dataS.limits.siegeRamCost, s, "Siege_Ram_Cost");
		findIntParam(&globals::dataS.limits.siegeLadderCost, s, "Siege_Ladder_Cost");
		findIntParam(&globals::dataS.limits.siegeTowerCost, s, "Siege_Tower_Cost");
	}
	f1.close();

	return true;
}

NOINLINE EOP_EXPORT void managerExport::initEOP(const char* modPath,int gameVer)
{
	globals::dataS.gamever = gameVer;
	globals::dataS.modPatch = modPath;

	CreateDirectoryA("logs", NULL);

	managerF::init();
}
