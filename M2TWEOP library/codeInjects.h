#pragma once
namespace codeInjects
{
	void berserkersPatch(const char*patchName,int gameVer);
	void historyBattlesSoldierPatch(int gameVer);
	void toLoadDescrBattleCharacterPatch(const char* patchName, int gameVer);
	void toBattleStateResultsPatch(const char* patchName, int gameVer);
	void toReadEDUPatch(const char* patchName, int gameVer);
	void toChangePassedTurnsCountPatch(const char* patchName, int gameVer);
	void toCheckLabelResultsPatch(const char* patchName, int gameVer);
	//after read campaign or battle files
	void toCreateMapPatch(const char* patchName, int gameVer);

	void toStartReadModelsPatch(const char* patchName, int gameVer);

	void toResourcesReadModelsPatch(const char* patchName, int gameVer);
	void toEventsPatch(const char* patchName, int gameVer);
	void toCustomTileSelectionPatch(const char* patchName, int gameVer);
	void toStratModelsSelectPatch(const char* patchName, int gameVer);
	void toLoadSaveFilePatch(const char* patchName, int gameVer);
	void toSaveGamePatch(const char* patchName, int gameVer);
	void toStartNewGamePatch(const char* patchName, int gameVer);
	void toEndReadModelsPatch(const char* patchName, int gameVer);
	void toClickAtCoordsPatch(const char* patchName, int gameVer);
	void toSelectWorldFromDBPatch(const char* patchName, int gameVer);
	void toBattleLoadedPatch(const char* patchName, int gameVer);
	void toReadGameDBsAtGameStartPatch(const char* patchName, int gameVer);
	void toDrawPartsOfStratObjectsPatch(const char* patchName, int gameVer);
	void toGameFormDrawImagePatch(const char* patchName, int gameVer);
	void toEndSettlementSiegePatch(const char* patchName, int gameVer);
	void toStartSettlementSiegePatch(const char* patchName, int gameVer);
	void toCastleConversionLvlSetterPatch(const char* patchName, int gameVer);
	void toCityConversionLvlSetterPatch(const char* patchName, int gameVer);
};


