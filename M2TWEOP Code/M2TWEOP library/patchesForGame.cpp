#include "pch.h"
#include "patchesForGame.h"
#include "tilesChange.h"
#include "graphicsD3D.h"
#include <functional>


#include "onlineThings.h"

#include "eduThings.h"
#include "fastFunctsHelpers.h"
#include "PlannedRetreatRoute.h"
#include "discordManager.h"


worldRecord* __fastcall patchesForGame::selectWorldpkgdesc(char* database, worldRecord* selectedRecord)
{
	auto& battlemapWorker = globals::dataS.Modules.battlemapWorker;


	battlemapWorker.TryCreateRecodsList(reinterpret_cast<battlemapWorker::dataBaseWorlds*>(database));

	string selectRecordS = battlemapWorker.getRecordName(selectedRecord);
	string selectRecordG = battlemapWorker.getRecordGroup(selectedRecord);
	string selectedWorld = plugins::onSelectWorldpkgdesc(selectRecordS.c_str(), selectRecordG.c_str());
	if (selectedWorld.empty() || selectedWorld == selectRecordS)
	{
		selectedWorld = battleCreator::selectWorldpkgdesc(selectRecordS, selectRecordG);
		if (selectedWorld.empty() || selectedWorld == selectRecordS)
		{
			selectedWorld = globals::dataS.Modules.developerMode.SelectWorldpkgdesc(selectRecordS, selectRecordG);
		}
	}

	if (selectedWorld.empty() || selectedWorld == selectRecordS)
	{
		return nullptr;
	}



	string selectedWorldCPP = selectedWorld;
	struct dataBaseS
	{
		int something;
		int something2;
		worldRecord* records;
		worldRecord* recordsEnd;
	};

	dataBaseS* db = (dataBaseS*)database;

	worldRecord* currRecord = db->records;
	do
	{
		string recName = battlemapWorker.getRecordName(currRecord);
		string recGroup = battlemapWorker.getRecordGroup(selectedRecord);
		if (recName == selectedWorldCPP && recGroup == selectRecordG)
		{
			return currRecord;
		}


		currRecord++;
	} while (currRecord != db->recordsEnd);

	return nullptr;
}
void __fastcall patchesForGame::OnLoadSettlementWorldpkgdesc(worldRecord* selectedRecord)
{
	auto& battlemapWorker = globals::dataS.Modules.battlemapWorker;

	string selectRecordS = battlemapWorker.getRecordName(selectedRecord);
	string selectRecordG = battlemapWorker.getRecordGroup(selectedRecord);
	battleCreator::OnLoadSettlementWorldpkgdesc(selectRecordS, selectRecordG);
}

float __fastcall patchesForGame::OnCalculateUnitValue(eduEntry* entry, const DWORD value)
{
	float floatValue;
	std::memcpy(&floatValue, &value, sizeof(float));
	return plugins::OnCalculateUnitValue(entry, floatValue);
}

int __fastcall patchesForGame::onfortificationlevelS(settlementStruct* settlement, bool* isCastle)
{
	int selectedLevel = plugins::onfortificationlevelS(settlement, isCastle);
	if (selectedLevel == -2)
	{
		*isCastle = settlement->isCastle;
	}
	return selectedLevel;//use old thing
}
char* __fastcall patchesForGame::onSaveEDUStringS(eduEntry* eduEntry)
{
	char* retName = eduThings::getEopNameOfEduEntry(eduEntry);
	if (retName == nullptr)
	{
		return eduEntry->Type;
	}

	return retName;
}
int __fastcall patchesForGame::onCreateUnit(char** entryName, int* edbIndex)
{
	if (edbIndex == nullptr)
	{
		int* newEdu = eduThings::tryFindDataEopEduIndex(*entryName);

		if (newEdu == nullptr)
		{
			return  -1;
		}

		return *newEdu;
	}

	return *edbIndex;
}

int __fastcall patchesForGame::onFindUnit(char* entry, int* edbIndex)
{
	if (edbIndex == nullptr)
	{
		if (entry == nullptr)
			return  -1;

		int* newEdu = eduThings::tryFindDataEopEduIndex(entry);

		if (newEdu == nullptr)
			return  -1;

		return *newEdu;
	}

	return *edbIndex;
}

int __fastcall patchesForGame::OnReligionCombatBonus(int religionID, namedCharacter* namedChar)
{
	if (religionID > 9)
		return 0;

	return namedChar->combatVsReligion[religionID];
}

int __fastcall patchesForGame::OnCreateMercUnitCheck(char** entryName, int eduindex)
{
	if (eduindex == -1)
	{
		int* newEdu = eduThings::tryFindDataEopEdu(*entryName);

		if (newEdu == nullptr)
		{
			return  -1;
		}

		return 0;
	}

	return eduindex;
}
eduEntry* __fastcall patchesForGame::OnCreateMercUnit(char** entryName, eduEntry* entry)
{
	DWORD entryAddr = (DWORD)entry;
	DWORD mercEOPValue = codes::offsets.mercEOPValue;//this is some weird address made by subtracting a value from edu start or something I dont really remember but its necesarry
	if (entryAddr == mercEOPValue)
	{
		int* eduindex = eduThings::tryFindDataEopEduIndex(*entryName);
		eduEntry* eopentry = eduThings::getEopEduEntry(*eduindex);
		return eopentry;
	}

	return entry;
}

eduEntry* __fastcall patchesForGame::OnCreateUnitWrapper(int eduindexBase, int removeValue)
{
	int eduindex = eduindexBase - (removeValue * 8);
	eduEntry* entry = eduThings::getEduEntry(eduindex);
	if (entry == nullptr)
	{
		entry = eduThings::getEopEduEntry(eduindex);
	}
	return entry;
}

DWORD __fastcall patchesForGame::OnUnitInfo(DWORD entryAddress)
{

	DWORD eduBaseAddress = dataOffsets::offsets.unitTypesStart;
	int index = (entryAddress - eduBaseAddress) / 996; // 996 is the size of the eduEntry struct
	if (index < 500)
	{
		return entryAddress;
	}
	auto entry = eduThings::getEopEduEntry(index);
	return reinterpret_cast<DWORD>(entry);
}

eduEntry* __fastcall patchesForGame::OnGetRecruitPoolUnitEntry(int eduIndex)
{
	eduEntry* entry = eduThings::getEduEntry(eduIndex);
	if (entry == nullptr)
	{
		entry = eduThings::getEopEduEntry(eduIndex);
	}
	return entry;
}

const char* __fastcall patchesForGame::onQuickSave()
{
	static std::vector<std::string> saveNames = { u8"%S-1.sav" ,u8"%S-2.sav", u8"%S-3.sav" };
	jsn::json json;

	std::string fPath = globals::dataS.modPatch;
	fPath += "\\saves\\quickSavesM2TWEOP.json";

	int currSaveID = 0;
	int maxSaveID = 2;
	try
	{
		std::ifstream f1(fPath);
		if (f1.is_open())
		{
			f1 >> json;

			f1.close();

			json.at("saveID").get_to(currSaveID);

			if (currSaveID + 1 > maxSaveID)
			{
				json["saveID"] = 0;
			}
			else
			{
				json["saveID"] = currSaveID + 1;
			}
			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
		else
		{
			currSaveID = 0;
			json["saveID"] = 0;

			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
	}
	catch (jsn::json::type_error& e)
	{
		currSaveID = 0;
		json["saveID"] = 0;

		std::ofstream f2(fPath);

		f2 << json;

		f2.close();
	}

	return saveNames[currSaveID].c_str();
}
const char* __fastcall patchesForGame::onAutoSave()
{
	static std::vector<std::string> saveNames = { u8"%s%S%s%S-1.sav" ,u8"%s%S%s%S-2.sav", u8"%s%S%s%S-3.sav",
		 u8"%s%S%s%S-4.sav" ,u8"%s%S%s%S-5.sav", u8"%s%S%s%S-6.sav"
	};
	jsn::json json;

	std::string fPath = globals::dataS.modPatch;
	fPath += "\\saves\\autoSavesM2TWEOP.json";

	int currSaveID = 0;
	int maxSaveID = 5;
	try
	{
		std::ifstream f1(fPath);
		if (f1.is_open())
		{
			f1 >> json;

			f1.close();

			json.at("saveID").get_to(currSaveID);

			if (currSaveID + 1 > maxSaveID)
			{
				json["saveID"] = 0;
			}
			else
			{
				json["saveID"] = currSaveID + 1;
			}
			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
		else
		{
			currSaveID = 0;
			json["saveID"] = 0;

			std::ofstream f2(fPath);

			f2 << json;

			f2.close();
		}
	}
	catch (jsn::json::type_error& e)
	{
		currSaveID = 0;
		json["saveID"] = 0;

		std::ofstream f2(fPath);

		f2 << json;

		f2.close();
	}

	return saveNames[currSaveID].c_str();
}
general* __fastcall patchesForGame::mercenaryMovepointsGetGeneral(stackStruct* army)
{
	general* gen = army->gen;
	if (gen == nullptr)
	{
		if (army->settlement != nullptr)
		{
			fortStruct* ourFort = fastFuncts::findFort(army->settlement->xCoord, army->settlement->yCoord);
			gen = ourFort->gubernator;
		}

	}
	return gen;
}
void __fastcall patchesForGame::clickAtTile(int* xy)
{
	plugins::onClickAtTile(xy[0], xy[1]);
}
void __stdcall patchesForGame::afterCampaignMapLoaded()
{
	plugins::onCampaignMapLoaded();
}
void __stdcall patchesForGame::onNewGameStart()
{
	plugins::onNewGameStart();


	PlannedRetreatRoute::OnNewGameStart();


}
//#define TESTPATCHES
void __stdcall patchesForGame::afterEDUread()
{
	plugins::onReadGameDbsAtStart();
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterEDUread" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::onGameInit()
{
	plugins::onGameInit();
}

void __fastcall patchesForGame::onAiTurn(aiFaction* aifaction)
{
	plugins::onAiTurn(aifaction);
}

void __stdcall patchesForGame::onChangeTurnNum()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onChangeTurnNum" << endl;
	f1.close();


#endif

	plugins::onChangeTurnNum();
}

void __stdcall patchesForGame::onGiveTrait()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onGiveTrait" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::afterLoadGameData()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterLoadGameData" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::afterI_CompareCounter()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterI_CompareCounter" << endl;
	f1.close();
#endif
}

int __stdcall patchesForGame::checkFort()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkFort" << endl;
	f1.close();
#endif
	return 0;
}

int __stdcall patchesForGame::checkDipString()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkDipString" << endl;
	f1.close();
#endif
	return 0;
}

int __stdcall patchesForGame::checkFacDip()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkFacDip" << endl;
	f1.close();
#endif
	return 0;
}

int __stdcall patchesForGame::spawnArmyCoords()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "spawnArmyCoords" << endl;
	f1.close();
#endif
	return 0;
}

void __stdcall patchesForGame::spawnEndArmyCoords()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "spawnEndArmyCoords" << endl;
	f1.close();
#endif
}

int __stdcall patchesForGame::spawnCharacterCoords()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "spawnCharacterCoords" << endl;
	f1.close();
#endif
	return 0;
}

void __stdcall patchesForGame::onBattleStratScreen()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onBattleStratScreen" << endl;
	f1.close();
#endif
	//battleCreator::onBattleStratScreen();

}

void __fastcall patchesForGame::onEvent(DWORD** vTab, DWORD arg2)
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onEvent" << endl;
	f1.close();
#endif
	plugins::onEvent(vTab, arg2);
}

void __fastcall patchesForGame::onLoadSaveFile(UNICODE_STRING**& savePath)
{
	plugins::onLoadGame(savePath);
}

void __fastcall patchesForGame::onSaveGame(UNICODE_STRING**& savePath)
{
	plugins::onSaveGame(savePath);
}

void __fastcall patchesForGame::onTileCheck(int* coords)
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onTileCheck" << endl;
	f1.close();
#endif
	if (coords == nullptr)
	{
		MessageBoxA(NULL, "Error in onTileCheck", "Attention", NULL);
		return;
	}

	tilesChange::onTilesCheck(coords);
}

void __stdcall patchesForGame::readTilesFile()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "readTilesFile" << endl;
	f1.close();
#endif
}

void __stdcall patchesForGame::unicalStratModsRead()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "unicalStratModsRead" << endl;
	f1.close();
#endif
	stratModelsChange::loadModels();
}

void __stdcall patchesForGame::loadCharStratModels()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "loadCharStratModels" << endl;
	f1.close();
#endif
	stratModelsChange::loadCharModels();
}


void __stdcall patchesForGame::checkAndChangeModels()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "checkAndChangeModels" << endl;
	f1.close();
#endif
	stratModelsChange::checkAndChangeStratModels();
}

void __stdcall patchesForGame::battleLoaded()
{
	discordManager::battleLoaded();
	battleCreator::startCreationThread();
}

void __stdcall patchesForGame::toReadGameDbsAtStart()
{
	//	plugins::onReadGameDbsAtStart();
}

void __stdcall patchesForGame::onDrawPartsOfStratObjects()
{
	graphicsD3D::onDrawPartsOfStratObjects();
}

void __stdcall patchesForGame::onDrawAll()
{
	graphicsD3D::onDrawAllGameStuff();
}

void __stdcall patchesForGame::onStartOfDrawFunction()
{
	stratModelsChange::update();
}

void __stdcall patchesForGame::onRetreat()
{
	PlannedRetreatRoute::OnRetreat();
}

void __fastcall patchesForGame::OnStopCharacter(general* character)
{
	auto& campaign = smallFuncs::getGameDataAll()->campaignData;
	if (campaign->humanPlayers < 2)
	{
		character->isStopCharacterNeeded = 1;
	}
}

eduEntry* __fastcall patchesForGame::recruitEOPunit(int eduIndex)
{
	eduEntry* entry = eduThings::getEduEntry(eduIndex);
	if (entry == nullptr)
	{
		entry = eduThings::getEopEduEntry(eduIndex);
	}
	return entry;
}

void __fastcall patchesForGame::recruitEOPunit2(int eduIndex)
{
	eduEntry* entry = eduThings::getEduEntry(eduIndex);
	if (entry == nullptr)
	{
		entry = eduThings::getEopEduEntry(eduIndex);
	}
	if (entry)
	{
		entry->UnitCreatedCounter++;
	}
}

void __fastcall patchesForGame::recruitEOPMercunit(DWORD pad, DWORD pad2, regionStruct* region, int eduindex, int factionid, int exp)
{
	int regionID = region->regionID;
	if (eduindex > 499)
	{
		int eopIDX = eduThings::getDataEopEdu(eduindex);
		fastFuncts::createUnitEDB(eopIDX, regionID, factionid, exp, 0, 0);
	}
	else
	{
		fastFuncts::createUnitIdx(eduindex, regionID, factionid, exp, 0, 0);
	}
}

void WINAPI patchesForGame::OnMoveRecruitQueue()
{
	auto& campaign = smallFuncs::getGameDataAll()->campaignData;
	if (campaign->humanPlayers < 2)
	{
		return;
	}

	MessageBoxA(NULL, "Moving queue disabled in hotseat mode. EXIT NOW!!!!!", "ATTENTION!", NULL);
	std::terminate();
}

void __fastcall patchesForGame::onEndSiege(settlementStruct* sett)
{
	plugins::onEndSiege(sett);
}

void __fastcall patchesForGame::onStartSiege(settlementStruct* sett)
{
	if (sett->siegesNumber == 0)
	{
		return;
	}
	plugins::onStartSiege(sett);
}



void __fastcall patchesForGame::onLoadDescrBattleCharacter(stackStruct* army, general* goalGen)
{
	fastFuncts::setBodyguard(goalGen, army->units[0]);//we replace game function what set army leader character.

	std::string relativePath = techFuncs::uniToANSI(smallFuncs::getGameDataAll()->campaignData->currentDescrFile);

	if (relativePath.find("battle") != std::string::npos)
	{
		battleCreator::addCharactersToCustomBattleArmy(army, relativePath);
	}
	//general*newGen=fastFuncts::createCharacter("named character",army->faction,25,"testGen", "testGen",0,nullptr, 0,0);
}

void __stdcall patchesForGame::onBattleStateChange()
{
	int battleState = smallFuncs::getGameDataAll()->battleHandler->battleState;
	//results screen
	if (battleState == 9)
	{
		battleCreator::onBattleResultsScreen();
	}
}
/*
int* __fastcall patchesForGame::ontryFindTypeIdInListRecruitPoolEDB(char* unitTypeString)
{
	return  eduThings::tryFindDataEopEduIndex(unitTypeString);
}

EduEntry* __fastcall patchesForGame::onrecruitPoolFillFromFile(int eduIndex)
{
	EduEntry*retEntry =	fastFunctsHelpers::getEDUEntryById(eduIndex);
	if (retEntry != nullptr)
	{
		return retEntry;
	}

	return eduThings::getEopEduEntry(eduIndex);
}
*/

struct
{
	bool isComingFromConsole = false;
}logonConsoleD;
void __stdcall patchessForConsole::onGameConsoleCommandFromConsole()
{
	logonConsoleD.isComingFromConsole = true;
}

void __stdcall patchessForConsole::onGameConsoleCommandFromScript()
{
	logonConsoleD.isComingFromConsole = false;
}

int __fastcall patchessForConsole::OnReadLogonOrLogoff(int isLogonNow)
{
	if (logonConsoleD.isComingFromConsole == true)
	{
		return isLogonNow;
	}
	return true;
}
