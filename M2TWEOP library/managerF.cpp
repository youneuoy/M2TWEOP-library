#include "pch.h"
#include "managerF.h"

#include "onlineThings.h"
#include "settlementConversionLvlSetter.h"
#include "codeInjects.h"
void managerF::init()
{

	initThread();
}


//apply code injects
void managerF::doPachs()
{
	 codeInjects::berserkersPatch("berserkersPatch", globals::dataS.gamever);
	 codeInjects::historyBattlesSoldierPatch(globals::dataS.gamever);
	 codeInjects::toLoadDescrBattleCharacterPatch("toLoadDescrBattleCharacterPatch", globals::dataS.gamever);
	 codeInjects::toBattleStateResultsPatch("toBattleStateResultsPatch", globals::dataS.gamever);
	 codeInjects::toReadEDUPatch("toReadEDUPatch", globals::dataS.gamever);
	 codeInjects::toChangePassedTurnsCountPatch("toChangePassedTurnsCountPatch", globals::dataS.gamever);
	 codeInjects::toCheckLabelResultsPatch("toCheckLabelResultsPatch", globals::dataS.gamever);

	 codeInjects::toCreateMapPatch("toCreateMapPatch", globals::dataS.gamever);

	 codeInjects::toStartReadModelsPatch("toStartReadModelsPatch", globals::dataS.gamever);

	 codeInjects::toResourcesReadModelsPatch("toResourcesReadModelsPatch", globals::dataS.gamever);
	 codeInjects::toEventsPatch("toEventsPatch", globals::dataS.gamever);
	 codeInjects::toCustomTileSelectionPatch("toCustomTileSelectionPatch", globals::dataS.gamever);
	 codeInjects::toStratModelsSelectPatch("toStratModelsSelectPatch", globals::dataS.gamever);
	 codeInjects::toLoadSaveFilePatch("toLoadSaveFilePatch", globals::dataS.gamever);
	 codeInjects::toSaveGamePatch("toSaveGamePatch", globals::dataS.gamever);
	 codeInjects::toStartNewGamePatch("toStartNewGamePatch", globals::dataS.gamever);
	 codeInjects::toEndReadModelsPatch("toEndReadModelsPatch", globals::dataS.gamever);
	 codeInjects::toClickAtCoordsPatch("toClickAtCoordsPatch", globals::dataS.gamever);
	 codeInjects::toSelectWorldFromDBPatch("toSelectWorldFromDBPatch", globals::dataS.gamever);
	 codeInjects::toBattleLoadedPatch("toBattleLoadedPatch", globals::dataS.gamever);
	 codeInjects::toReadGameDBsAtGameStartPatch("toReadGameDBsAtGameStartPatch", globals::dataS.gamever);
	 codeInjects::toDrawPartsOfStratObjectsPatch("toDrawPartsOfStratObjectsPatch", globals::dataS.gamever);
	 codeInjects::toGameFormDrawImagePatch("toGameFormDrawImagePatch", globals::dataS.gamever);
	 codeInjects::toEndSettlementSiegePatch("toEndSettlementSiegePatch", globals::dataS.gamever);
	 codeInjects::toStartSettlementSiegePatch("toStartSettlementSiegePatch", globals::dataS.gamever);
	 codeInjects::toCastleConversionLvlSetterPatch("toCastleConversionLvlSetterPatch", globals::dataS.gamever);
	 codeInjects::toCityConversionLvlSetterPatch("toCityConversionLvlSetterPatch", globals::dataS.gamever);
}

//#include "tests.h"
void managerF::initThread()
{
	//read_modConfig();
	codes::initCodes(globals::dataS.gamever);
	dataOffsets::initDataOffsets(globals::dataS.gamever);
	//read_limits();




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
