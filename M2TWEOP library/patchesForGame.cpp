#include "pch.h"
#include "patchesForGame.h"
#include "tilesChange.h"
#include "graphicsD3D.h"
#include <functional>


#include "onlineThings.h"



string getRecordName(worldRecord* selectedRecord)
{
	int allocatedLen = selectedRecord->allocatedNameLen;
	char* str = nullptr;
	if (allocatedLen > 15)
	{
		str = *(char**)selectedRecord->recordName;
	}
	else
	{
		str = &selectedRecord->recordName[0];
	}

	int nameLen = selectedRecord->nameLen;

	string retS;
	for (int i = 0; i < nameLen; i++)
	{
		retS.push_back(str[i]);
	}
	retS[nameLen] = '\0';

	return retS;
}


string getRecordGroup(worldRecord* selectedRecord)
{
	int allocatedLen = selectedRecord->allocatedGroupLen;
	char* str = nullptr;
	if (allocatedLen > 15)
	{
		str = *(char**)selectedRecord->recordGroup;
	}
	else
	{
		str = &selectedRecord->recordGroup[0];
	}

	int groupLen = selectedRecord->groupLen;

	string retS;
	for (int i = 0; i < groupLen; i++)
	{
		retS.push_back(str[i]);
	}
	retS[groupLen] = '\0';

	return retS;
}

worldRecord* __fastcall patchesForGame::selectWorldpkgdesc(char* database, worldRecord* selectedRecord)
{
	string selectRecordS = getRecordName(selectedRecord);
	string selectRecordG = getRecordGroup(selectedRecord);
	string selectedWorld= plugins::onSelectWorldpkgdesc(selectRecordS.c_str(), selectRecordG.c_str());
	if (selectedWorld.empty()||selectedWorld == selectRecordS)
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
		string recName = getRecordName(currRecord);
		string recGroup=getRecordGroup(selectedRecord);
		if (recName== selectedWorldCPP&& recGroup== selectRecordG)
		{
			return currRecord;
		}


		currRecord++;
	} while (currRecord!= db->recordsEnd);

	return nullptr;
}
void __fastcall patchesForGame::clickAtTile(int* xy)
{
	plugins::onClickAtTile(xy[0],xy[1]);
}
void __stdcall patchesForGame::afterCampaignMapLoaded()
{
	plugins::onCampaignMapLoaded();
}
void __stdcall patchesForGame::onNewGameStart()
{
	plugins::onNewGameStart();

}
//#define TESTPATCHES
void __stdcall patchesForGame::afterEDUread()
{
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "afterEDUread" << endl;
	f1.close();
#endif
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

void __fastcall patchesForGame::onEvent(DWORD** vTab)
{
	std::ofstream f1("D:\\WMS\\2.txt");
	f1 << "sdsdsd" << std::endl;
	f1.close();
#if defined TESTPATCHES
	ofstream f1("logs\\TESTPATCHES.log", ios::app);

	f1 << "onEvent" << endl;
	f1.close();
#endif
	plugins::onEvent(vTab);
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
	battleCreator::startCreationThread();
}

void __stdcall patchesForGame::toReadGameDbsAtStart()
{
	plugins::onReadGameDbsAtStart();
}

void __stdcall patchesForGame::onDrawPartsOfStratObjects()
{
	graphicsD3D::onDrawPartsOfStratObjects();
}

void __stdcall patchesForGame::onDrawAll()
{
	graphicsD3D::onDrawAllGameStuff();
}

void __fastcall patchesForGame::onEndSiege(settlementStruct* sett)
{
	plugins::onEndSiege(sett);
}

void __fastcall patchesForGame::onStartSiege(settlementStruct* sett)
{
	if (sett->siege == nullptr)
	{
		return;
	}
	plugins::onStartSiege(sett);
}



void __fastcall patchesForGame::onLoadDescrBattleCharacter(stackStruct* army, general* goalGen)
{

	fastFuncts::setBodyguard(goalGen, army->units[0]);

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

