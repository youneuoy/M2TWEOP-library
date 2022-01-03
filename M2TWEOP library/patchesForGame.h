#pragma once
#include "plugins.h"
#include "stratModelsChange.h"
#include "graphicsD3D.h"
class patchesForGame
{
public:
	//select worldpkgdesc from db
	static worldRecord* __fastcall selectWorldpkgdesc(char* database, worldRecord* selectedRecord);


	static general* __fastcall mercenaryMovepointsGetGeneral(stackStruct*army);

	//click at tile
	static void __fastcall clickAtTile(int* xy);

	//after loading of campaign map
	static void WINAPI afterCampaignMapLoaded();

	//after new game start
	static void WINAPI onNewGameStart();
	//after reading EDU
	static void WINAPI afterEDUread();


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
	static void __fastcall onEvent(DWORD** vTab);

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
	//moment before game start select stratmap models for drawing
	//replace models for the settlements, forts, ports, etc here without craches
	static void WINAPI checkAndChangeModels();



	static void WINAPI battleLoaded();
	static void WINAPI toReadGameDbsAtStart();


	static void WINAPI onDrawPartsOfStratObjects();
	static void WINAPI onDrawAll();


	static void __fastcall onEndSiege(settlementStruct*sett);
	static void __fastcall onStartSiege(settlementStruct*sett);
	static void __fastcall onLoadDescrBattleCharacter(stackStruct*army,general*goalGen);


	//called not in all cases!
	static void WINAPI onBattleStateChange();



};

class patchessForConsole
{
public:
	static void WINAPI onGameConsoleCommandFromConsole();
	static void WINAPI onGameConsoleCommandFromScript();

	static int __fastcall OnReadLogonOrLogoff(int isLogonNow);
};