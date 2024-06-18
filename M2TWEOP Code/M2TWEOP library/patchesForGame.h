#pragma once
#include "stratModelsChange.h"
#include "graphicsD3D.h"
#include "unit.h"


struct regionStruct;
struct aiFaction;

struct aiTacticAssault
{
	DWORD *vftable /*VFT*/;
	struct aiDetachment *aiDetachment;
	char hasBeenConsidered;
	char isExecuting;
	char hasExecuted;
	char field_B;
	int phaseStarted;
	int arrayOfTenRandomValues[10];
	char startedTactic;
	char differentUnitsThisUpdate;
	char pad[2];
	unit **units;
	int unitsSize;
	int unitNum;
	void* fsm;
	DWORD fsmState;
	struct aiDetachment *detachment;
	struct aiUnitGroup siegeUnitGroup;
	struct aiUnitGroup aiUnitGroup;
	bool started;
	char field_2ACd[3];
	struct buildingBattle *building;
	struct unit *attackUnit;
	int formationIndex;
	float advanceX;
	float advanceY;
	int16_t angle;
	int16_t angle2;
	int advanceDirectionX;
	float advanceDirectionY;
	char firstAdvanceOrders;
	char docking;
	char engineLeft;
	char finished;
	float priority;
};

class patchesForGame
{
public:
	//select worldpkgdesc from db
	static worldRecord* __fastcall selectWorldpkgdesc(char* database, worldRecord* selectedRecord);


	static void __fastcall onLoadSettlementWorldpkgdesc(worldRecord* selectedRecord);
	static int __fastcall onfortificationlevelS(settlementStruct* settlement, bool* isCastle);
	static char* __fastcall onSaveEDUStringS(eduEntry* eduEntry);
	static int __fastcall onCreateUnit(char** entryName, int* edbIndex);
	static int __fastcall OnCreateMercUnitCheck(char** entryName, int eduindex);
	static void __fastcall onPreBattlePlacement(aiTacticAssault* aiTactic);
	static bool __thiscall onPreBattlePlacement2(aiUnitGroup* group, DWORD formationTemplate, bool force_issue_order);
	static eduEntry* __fastcall OnCreateMercUnit(char** entryName, eduEntry* entry);
	static eduEntry* __fastcall OnCreateUnitWrapper(int eduindexBase, int removeValue);
	static bool __fastcall onDecideRamAttacks(buildingBattle* gate, aiDetachment* detachment, int numRamsLeft);
	static const char* __fastcall onQuickSave();
	static const char* __fastcall onAutoSave();
	static eduEntry* __fastcall OnGetRecruitPoolUnitEntry(int eduIndex);
	static int __fastcall onFindUnit(char* entry, int* edbIndex);
	static DWORD __fastcall OnUnitInfo(DWORD entryAddress);
	static float __fastcall onCalculateUnitValue(eduEntry* entry, DWORD value);
	static int __fastcall onEvaluateUnit(int eduIndex);
	static DWORD __fastcall onCustomBattleUnitCards(DWORD cardArrayThing, int factionID);
	static int __fastcall onCustomBattleUnits(eduEntry** unitArray, int currentCount, int factionID);
	static int __fastcall onAttackGate(unit* unit, void* tactic);
	static eduEntry* __fastcall onEvaluateUnit2(int eduIndex);
	static eduEntry* __fastcall onReadDescrRebel(DWORD value);
	static DWORD __fastcall onSearchUnitType(char* typeName);
	static int __fastcall OnReligionCombatBonus(int religionID, characterRecord* namedChar);
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


	static character* __fastcall mercenaryMovepointsGetGeneral(armyStruct* army);

	//click at tile
	static void __fastcall clickAtTile(coordPair* xy);

	//after loading of campaign map
	static void WINAPI afterCampaignMapLoaded();

	//after new game start
	static void WINAPI onNewGameStart();
	//after reading EDU
	static void WINAPI afterEDUread();
	static void WINAPI onGameInit();
	static void WINAPI onUnloadCampaign();
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


	static void __fastcall OnStopCharacter(character* character);
	static void WINAPI onMoveRecruitQueue();
	static eduEntry* __fastcall recruitEOPunit(int eduIndex);
	static void __fastcall recruitEOPunit2(int eduIndex);
	static void __fastcall recruitEopMercUnit(DWORD pad, DWORD pad2, regionStruct* region, int eduindex, int factionid, int exp);


	static void __fastcall onEndSiege(settlementStruct* sett);
	static void __fastcall onStartSiege(settlementStruct* sett);
	static void __fastcall onLoadDescrBattleCharacter(armyStruct* army, character* goalGen);


	//called not in all cases!
	static void WINAPI onBattleStateChange();

	/*
	static int* __fastcall ontryFindTypeIdInListRecruitPoolEDB(char* unitTypeString);
	static EduEntry* __fastcall onrecruitPoolFillFromFile(int eduIndex);
	*/
};

class patchessForConsole
{
public:
	static void WINAPI onGameConsoleCommandFromConsole();
	static void WINAPI onGameConsoleCommandFromScript();

	static int __fastcall OnReadLogonOrLogoff(int isLogonNow);
};