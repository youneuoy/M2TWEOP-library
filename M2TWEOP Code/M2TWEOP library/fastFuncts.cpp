#include "pch.h"
#include "fastFuncts.h"

#include "exportHeader.h"
#include "globals.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include <queue>
#include <set>

#include "actionsStrat.h"
#include "eduThings.h"
#include "smallFuncs.h"

#include "fastFunctsHelpers.h"
#include "gameHelpers.h"
#include "headersMEM.h"
#include "m2tweopHelpers.h"
#include "plugData.h"
#include "techFuncs.h"
#include "unitActions.h"
#include "settlement.h"
#include "character.h"
#include "characterRecord.h"
#include "faction.h"

namespace fastFuncts
{

	//unique per faction!
	void loadSaveGame(const char* saveName)
	{
		UNICODE_STRING** nameMem = new UNICODE_STRING*;

		auto loadName = reinterpret_cast<UNICODE_STRING***>(dataOffsets::offsets.loadGameHandler + 0x4);
		*loadName = nameMem;
		
		smallFuncs::createUniString(*loadName, saveName);

		const auto currentHandler = reinterpret_cast<DWORD*>(dataOffsets::offsets.currentGameHandler);
		*currentHandler = dataOffsets::offsets.loadGameHandler;
	}

	factionStruct* getRegionOwner(int regionID)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->regionsNum; j++)
			{
				if (regionID == listFac[i]->regionsID[j])
				{
					return listFac[i];
				}
			}
		}



		return nullptr;
	}
	factionStruct* GetCurrentFaction()
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		if (gameDataAll == nullptr)
		{
			return nullptr;
		}
		if (gameDataAll->campaignData == nullptr)
		{
			return nullptr;
		}

		return gameDataAll->campaignData->currentFactionTurn;
	}
	std::string GetModPath()
	{
		return globals::dataS.modPath;
	}
	
	float GetMovepointsForReachNearTile(int x, int y, int destX, int destY)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		if (x > gameDataAll->stratMap->mapWidth)
		{
			return -1;
		}
		if (y > gameDataAll->stratMap->mapHeight)
		{
			return -1;
		}
		if (x < 0)
		{
			return -1;
		}
		if (y < 0)
		{
			return -1;
		}
		typedef float(__stdcall* GetMovepointsForReachTileF)(int* xy, int* destxy);

		GetMovepointsForReachTileF getMovepointsForReachTileF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			getMovepointsForReachTileF = (GetMovepointsForReachTileF)0x004c7bd0;
		}
		else
		{
			getMovepointsForReachTileF = (GetMovepointsForReachTileF)0x004c7620;
		}

		int xy[2] = { x,y };
		int destxy[2] = { destX, destY };

		return getMovepointsForReachTileF(xy, destxy);
	}
	void revealTile(factionStruct* faction, int x, int y)
	{
		if (IsStratMap() == false)
		{
			return;
		}
		typedef int* (__thiscall* RevealTileF)(void* tilesFac, int* xy, int some, float some2);

		RevealTileF revealTileF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			revealTileF = (RevealTileF)0x004baea0;
		}
		else
		{
			revealTileF = (RevealTileF)0x004ba910;
		}

		int coords[2] = { x,y };
		revealTileF(faction->tilesFac, coords, 1, -1.0);
	}
	
	void hideRevealedTile(factionStruct* faction, int x, int y)
	{
		if (IsStratMap() == false)
		{
			return;
		}
		struct SomeArgForHiding
		{
			int x;
			int y;
			int some = 2;
			float some2 = -1;
		};
		typedef int* (__thiscall* UnRevealTileF)(void* tilesFac, SomeArgForHiding* someArgForHiding, unsigned char isDeleteXYArray);

		UnRevealTileF unrevealTileF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			unrevealTileF = (UnRevealTileF)0x004baf80;
		}
		else
		{
			unrevealTileF = (UnRevealTileF)0x004ba9f0;
		}

		SomeArgForHiding someArg;
		someArg.x = x;
		someArg.y = y;

		void** revealedTiles = (void**)faction->tilesFac;
		SomeArgForHiding** tilesArr = (SomeArgForHiding**)revealedTiles[10];
		int num = (int)revealedTiles[12];
		for (int i = 0; i < num; i++)
		{
			if (tilesArr[i]->x == x && tilesArr[i]->y == y)
			{
				unrevealTileF(faction->tilesFac, tilesArr[i], 1);
				return;
			}
		}
	}
	
	int8_t getTileVisibility(factionStruct* faction, int x, int y)
	{
		return faction->tilesFac->tilesVisiblity[faction->tilesFac->tilesXBound * (y)+x];
	}
	
	void setTileVisibility(factionStruct* faction, int x, int y, int8_t vis)
	{
		faction->tilesFac->tilesVisiblity[faction->tilesFac->tilesXBound * (y)+x] = vis;
	}

	std::queue<coordPair> getNeighbourTiles(int x, int y)
	{
		std::queue<coordPair> neighbours;
		neighbours.push({ x - 1, y });
		neighbours.push({ x + 1, y });
		neighbours.push({ x, y - 1 });
		neighbours.push({ x, y + 1 });
		neighbours.push({ x - 1, y + 1 });
		neighbours.push({ x - 1, y - 1 });
		neighbours.push({ x + 1, y + 1 });
		neighbours.push({ x + 1, y - 1 });
		return neighbours;
	}

	coordPair* findValidTileNearTile(coordPair* coords, int charType)
	{
		if (isTileValidForCharacterType(charType, coords))
			return coords;
		const auto startCoords = *coords;
		std::queue<coordPair> neighbours = getNeighbourTiles(coords->xCoord, coords->yCoord);
		std::vector<coordPair> visited = { *coords };
		while (true)
		{
			if (neighbours.empty())
				break;
			coordPair checkCoord = neighbours.front();
			*coords = { checkCoord.xCoord, checkCoord.yCoord };
			neighbours.pop();
			visited.push_back(checkCoord);
			if (isTileValidForCharacterType(charType, coords))
				return coords;
			std::queue<coordPair> newNeighbours = getNeighbourTiles(checkCoord.xCoord, checkCoord.yCoord);
			while (!newNeighbours.empty())
			{
				auto newCoord = newNeighbours.front();
				newNeighbours.pop();
				bool isVisited = false;
				for (const auto& [xCoord, yCoord] : visited)
				{
					if (xCoord == newCoord.xCoord && yCoord == newCoord.yCoord)
						isVisited = true;
				}
				if (isVisited)
					continue;
				neighbours.push(newCoord);
			}
		}
		*coords = startCoords;
		return coords;
	}

	void GetGameTileCoordsWithCursor(int& x, int& y)
	{
		int* mouseOffset = 0x0;
		if (globals::dataS.gamever == 2)//steam
		{
			mouseOffset = (int*)0x02c3da48;
		}
		else
		{
			mouseOffset = (int*)0x02c86c28;
		}
		x = mouseOffset[0];
		y = mouseOffset[1];
	}
	void ViewTacticalMap(int x, int y)
	{
		globals::dataS.Modules.tacticalMapVeiwer.View(x, y);
	}
	bool IsStratMap()
	{
		int* isStratMap = reinterpret_cast<int*>(dataOffsets::offsets.someStratmapPointer);
		if (isStratMap == 0)
			return false;
		if (*isStratMap == 0)
			return false;
		return true;
	}
	
	UINT32 getTileRegionID(int x, int y)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		UINT32 redID = gameDataAll->stratMap->tilesArr[gameDataAll->stratMap->mapWidth * y + x].regionId;


		return redID;
	}
	oneTile* getTileStruct(int x, int y)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		return &(gameDataAll->stratMap->tilesArr[gameDataAll->stratMap->mapWidth * y + x]);
	}
	regionStruct* getRegionByID(UINT32 regionID)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);

		return &gameDataAll->stratMap->regions[regionID];
	}

	factionStruct** getFactionsList()
	{
		factionStruct** list;
		list = reinterpret_cast<factionStruct**>(dataOffsets::offsets.factionOffsetsStart + 0x22c);
		return list;
	}

	UINT32 getPassedTurnsNum()
	{
		UINT32 count = 0;
		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x378, &count);

		return count;
	}

	UINT32 getSeason()
	{
		UINT32 season = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x710, &season);

		return season;
	}

	UINT32 getYear()
	{
		UINT32 year = 0;
		float t = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x70c, &t);

		year = (UINT32)t;
		return year;
	}
	UINT32 getFactionsCount()
	{
		UINT32 count = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x324, &count);

		return count;
	}


	std::pair<int, int> getArmyCoords(stackStruct* army)
	{
		if (army->gen != nullptr)
		{
			return { army->gen->xCoord, army->gen->yCoord };
		}
		if (army->settlement != nullptr)
		{
			return { army->settlement->xCoord, army->settlement->yCoord };

		}
		return { -1,-1 };
	}

	stackStruct* findArmy(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->stackNum; j++)
			{
				auto armyCoords = getArmyCoords(listFac[i]->armies[j]);
				if (armyCoords.first == x
					&&
					armyCoords.second == y
					)
				{
					return listFac[i]->armies[j];
				}
			}
		}

		return nullptr;
	}

	fortStruct* findFort(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->fortsNum; j++)
			{
				if (listFac[i]->forts[j]->xCoord == x
					&&
					listFac[i]->forts[j]->yCoord == y
					)
				{
					return listFac[i]->forts[j];
				}
			}
		}

		return nullptr;
	}

	portBuildingStruct* findPort(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->portBuildingsNum; j++)
			{
				if (listFac[i]->portBuildings[j]->xCoord == x
					&&
					listFac[i]->portBuildings[j]->yCoord == y
					)
				{
					return listFac[i]->portBuildings[j];
				}
			}
		}

		return nullptr;
	}

	resStrat* findResource(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->settlementsNum; j++)
			{
				settlementStruct* setS = listFac[i]->settlements[j];
				for (int k = 0; k < setS->resourcesNum; k++)
				{
					if (setS->resources[k]->xCoord == x
						&&
						setS->resources[k]->yCoord == y
						)
					{
						return setS->resources[k];
					}
				}
			}
		}

		return nullptr;
	}

	settlementStruct* findSettlement(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->settlementsNum; j++)
			{
				settlementStruct* setS = listFac[i]->settlements[j];
				if (setS->xCoord == x && setS->yCoord == y)
				{
					return setS;
				}
			}
		}
		return nullptr;
	}

	watchTowerStruct* findWatchTower(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->watchtowersNum; j++)
			{
				watchTowerStruct* towS = listFac[i]->watchTowers[j];
				if (towS->xCoord == x && towS->yCoord == y)
				{
					return towS;
				}
			}
		}



		return nullptr;
	}

	void moveStratCameraSlow(int x, int y)
	{
		int* isStratMap = reinterpret_cast<int*>(dataOffsets::offsets.someStratmapPointer);
		if (isStratMap == 0)return;

		DWORD adrFunc = codes::offsets.moveStratCameraSlowFunc;
		_asm
		{
			push y
			push x

			mov eax, adrFunc
			call eax

			add esp, 8
		}


		return;
	}

	void moveStratCameraFast(int x, int y)
	{
		int* isStratMap = reinterpret_cast<int*>(dataOffsets::offsets.someStratmapPointer);
		if (isStratMap == 0)return;

		DWORD adrFunc = codes::offsets.moveStratCameraFastFunc;
		_asm
		{
			push y
			push x

			mov eax, adrFunc
			call eax

			add esp, 8
		}


		return;
	}

	void zoomStratCamera(float zoom)
	{
		int* isStratMap = reinterpret_cast<int*>(dataOffsets::offsets.someStratmapPointer);
		if (isStratMap == 0)return;

		DWORD adrFunc = codes::offsets.zoomStratCameraFunc;
		_asm
		{
			push zoom

			mov eax, adrFunc
			call eax

			add esp, 4
		}

		return;
	}

	factionRecord* GetFactSmDescrById(int id)
	{
		if (dataOffsets::offsets.descr_sm_factionslist == nullptr)
		{
			return nullptr;
		}
		int facsNum = dataOffsets::offsets.descr_sm_factionslist->size;

		for (int i = 0; i < facsNum; ++i)
		{
			if (dataOffsets::offsets.descr_sm_factionslist->facDescrs[i].id==id)
			{
				return &dataOffsets::offsets.descr_sm_factionslist->facDescrs[i];
			}
		}
		return nullptr;
	}

	bool isTileValidForCharacterType(int charType, coordPair* coords)
	{
		auto stratMap = smallFuncs::getGameDataAll()->stratMap;
		if (!stratMap)
			return false;
		if (!GAME_FUNC(bool(__stdcall*)(coordPair*, int, int), isTileValidForCharacter)(coords, charType, 1))
			return false;
		const auto tile = getTileStruct(coords->xCoord, coords->yCoord);
		return tile->nonPassable != -1 && ((tile->objectTypes & 107) == 0);
	}
	
	ancillary* findAncillary(const char* ancName)
	{
		if (ancName == nullptr)return 0;

		DWORD adr = 0;
		ancillary* retr = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			adr = 0x008b1d30;
		}
		else
		{
			adr = 0x008b1340;
		}

		_asm
		{
			push ancName
			mov eax, adr
			call eax
			add esp, 4
			mov retr, eax
		}


		return retr;
	}

	void setSoldiersCountAndExp(unit* un, int count, int exp)
	{
		un->expScreen = exp;
		setSoldiersCount(un, count);
	}
	void setUnitMovepoints(unit* un, float movepoints)
	{
		un->movePoints = movepoints;


		if (un->general == nullptr)
		{
			character* gen = un->army->gen;
			if (gen != 0)
			{
				DWORD adrFunc = 0;
				if (globals::dataS.gamever == 2)//steam
				{
					adrFunc = 0x00597c60;
				}
				else
				{
					adrFunc = 0x00597770;
				}
				_asm
				{
					mov ecx, gen

					mov eax, adrFunc
					call eax
				}
			}
			return;
		}
		un->general->movePointsCharacter = movepoints;
		un->general->movePointsArmy = movepoints;


		character* gen = un->general;

		DWORD adrFunc = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x00597c60;
		}
		else
		{
			adrFunc = 0x00597770;
		}
		_asm
		{
			mov ecx, gen

			mov eax, adrFunc
			call eax
		}

	}
	void NuullifyMovepoints(stackStruct* army)
	{
		if (army == nullptr)
		{
			return;
		}

		for (int i = 0; i < army->numOfUnits; ++i)
		{
			setUnitMovepoints(army->units[i], 0);
		}
	}

	void setSoldiersCount(unit* un, int count)
	{
		if (count == 0)
		{
			killUnit(un);
			return;
		}
		
		int diff = count - un->SoldierCountStrat;
		if (diff == 0)
		{
			return;
		}

		int exp = un->expScreen;
		if (diff > 0)
		{
			DWORD adrFunc = codes::offsets.replenishUnitFunc;
			_asm
			{
				push exp
				push diff
				mov ecx, un
				mov eax, adrFunc
				call eax
			}
		}
		else
		{
			un->SoldierCountStrat = count;
		}



		return;
	}

	void killUnit(unit* un)
	{
		if (un->general)
		{
			characterHelpers::killCharacter(un->general);
			return;
		}
		DWORD adr = codes::offsets.killUnitStratMapFunc;

		_asm {
			push 0x1
			mov ecx, [un]
			mov eax, [adr]
			call eax
		}
	}

	bool useButton(const char* buttonName)
	{
		DWORD findedButton = 0;
		char** cryptS = fastFunctsHelpers::makeCryptedString(buttonName);
		DWORD adrF = codes::offsets.getUiElementFunc;
		_asm
		{
			push cryptS
			mov eax, adrF
			call eax
			mov findedButton, eax
			add esp, 0x4
		}
		if (findedButton == 0)
		{
			return false;
		}

		adrF = codes::offsets.useButtonFunc;
		_asm
		{
			mov ecx, findedButton
			mov eax, adrF
			call eax
		}
		return true;
	}
	uiElement* getUiElement(const char* elementName)
	{
		uiElement* resElement = nullptr;
		char** cryptS = fastFunctsHelpers::makeCryptedString(elementName);
		DWORD adrF = codes::offsets.getUiElementFunc;
		_asm
		{
			push cryptS
			mov eax, adrF
			call eax
			mov resElement, eax
			add esp, 0x4
		}

		return resElement;
	}
	void useUiElement(uiElement* element)
	{
		DWORD adrF = codes::offsets.useButtonFunc;
		_asm
		{
			mov ecx, element
			mov eax, adrF
			call eax
		}
	}
	void autoResolve()
	{
		DWORD adrFunc = codes::offsets.autoResolveFunc;

		_asm
		{
			mov eax, adrFunc
			call eax
		}
	}
	bool autoWin(const char* winnerSide)
	{
		DWORD adrFunc = codes::offsets.autoWinFunc;
		string command = winnerSide;
		char buffer[100]{};
		const char* cmdC = command.c_str();

		bool result = false;
		_asm
		{
			lea eax, buffer
			push eax
			push cmdC
			mov eax, adrFunc
			call eax
			mov result, al
		}
		return result;
	}
	bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer)
	{
		auto cmd = dataOffsets::offsets.consoleCommands;
		for (int i = 0; i < cmd->size; i++)
		{
			auto currCom = cmd->commands[i];
			if (strcmp(currCom->name, name) != 0)
			{
				continue;
			}

			return (**currCom->function)(arg, errorBuffer);
		}

		return false;
	}
	stackStruct* createArmy(character* character)
	{
		stackStruct* stack = nullptr;

		factionStruct* fac = character->characterRecord->faction;
		bool isAdmiral = character->genType->type == 3;
		DWORD adrFunc = codes::offsets.createArmyFunc;
		_asm
		{
			push isAdmiral
			push character
			mov ecx, fac
			mov eax, adrFunc
			call eax
			mov stack, eax
		}


		return stack;
	}

	stackStruct* createArmyInSettlement(settlementStruct* sett)
	{
		stackStruct* stack = nullptr;

		DWORD adrFunc = codes::offsets.createArmyInSettlementFunc;
		_asm
		{
			mov ecx, sett
			mov eax, adrFunc
			call eax
			mov stack, eax
		}


		return stack;
	}

	unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap)
	{
		int unitIndex = fastFunctsHelpers::getEduIndex(type);


		return createUnitIdx(unitIndex, regionID, facNum, exp, arm, weap);
	}

	unit* createUnitIdx(int index, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (index == -1)return nullptr;

		unit* res = nullptr;

		DWORD EDB = dataOffsets::offsets.unitTypesStart - 4;
		DWORD adr = codes::offsets.createUnitFunc;

		regionStruct* region = getRegionByID(regionID);
		_asm {
			mov ecx, EDB;

			push weap
				push arm
				push - 1
				push exp
				push facNum
				push index
				push region

				mov eax, [adr]
				call eax
				mov res, eax
		}

		return res;
	}

	unit* createUnitEDB(int edb, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (edb == 0)
		{
			return nullptr;
		}
		unit* res = nullptr;

		DWORD adr = codes::offsets.createUnitFunc;

		auto* entry = reinterpret_cast<eduEntry*>(edb);
		DWORD eduIndex = entry->UnitCreatedCounter;

		regionStruct* region = getRegionByID(regionID);
		_asm {
				//mov ecx, edb;

				push weap
				push arm
				push - 1
				push exp
				push facNum
				push eduIndex
				push region

				mov eax, [adr]
				call eax
				mov res, eax
		}

		return res;
	}

	bool StopSiege(stackStruct* army)
	{
		bool retVal = (army->siege == nullptr);
		if (retVal)return false;

		typedef void(__thiscall* StopSiegeF)(stackStruct* army, int some);

		StopSiegeF stopSiegeF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			stopSiegeF = (StopSiegeF)0x00711830;
		}
		else
		{
			stopSiegeF = (StopSiegeF)0x007110f0;
		}

		stopSiegeF(army, 1);

		return retVal;
	}

	bool StopBlockPort(stackStruct* army)
	{
		bool retVal = (army->blockedPort == nullptr);
		if (retVal)return false;

		typedef void(__thiscall* StopBlockPortF)(stackStruct* army);

		StopBlockPortF stopBlockPortF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			stopBlockPortF = (StopBlockPortF)0x00711a50;
		}
		else
		{
			stopBlockPortF = (StopBlockPortF)0x00711310;
		}

		stopBlockPortF(army);

		return retVal;
	}

	int addUnitToArmy(stackStruct* army, unit* un)
	{
		if (army->numOfUnits == 20)return 0;

		DWORD adr = codes::offsets.addUnitToArmyFunc;
		_asm {
			push 0
			push un
			mov ecx, army
			mov eax, adr
			call eax
		}
		return 1;
	}

	void setBodyguard(character* gen, unit* un)
	{
		if (gen->bodyguards != nullptr && gen->bodyguards->trackedUnitPointerP != nullptr)
		{
			un->general = gen;
			un->trackedUnitPointerP = gen->bodyguards->trackedUnitPointerP;
			gen->bodyguards->trackedUnitPointerP = nullptr;///
			gen->bodyguards->general = nullptr;

			gen->bodyguards = un;

			(*un->trackedUnitPointerP)->unit = un;
			return;

		}
		DWORD adr = codes::offsets.setBodyguard;
		_asm {
			push un
			mov ecx, gen
			mov eax, adr
			call eax
		}

		return;
	}

	void setBodyguardStart(character* gen, unit* un)
	{
		DWORD adr = codes::offsets.setBodyguard;
		_asm {
			push un
			mov ecx, gen
			mov eax, adr
			call eax
		}

		return;
	}

	void AddToSettlement(stackStruct* army, settlementStruct* set)
	{
		if (army->settlement != nullptr)
		{
			UngarisonSetOrFort(army->settlement);
		}

		if (set->army != nullptr)
		{
			mergeArmies(army, set->army);
			return;
		}

		typedef void(__thiscall* AddToSettlementF)(settlementStruct* set, character* gen);

		AddToSettlementF addToSettlementF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			addToSettlementF = (AddToSettlementF)0x005e1dc0;
		}
		else
		{
			addToSettlementF = (AddToSettlementF)0x005e18f0;
		}
		if (army->gen==nullptr)
		{
			MessageBoxA(NULL, "AddToSettlement error", "ERROR", NULL);
			return;
		}
		addToSettlementF(set, army->gen);
	}

	void AddToFort(stackStruct* army, fortStruct* fort)
	{
		if (army->settlement != nullptr)
		{
			UngarisonSetOrFort(army->settlement);
		}

		if (fort->army != nullptr)
		{
			mergeArmies(army, fort->army);
			return;
		}

		typedef void(__thiscall* AddToFortF)(fortStruct* fort, character* gen);

		AddToFortF addToFortF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			addToFortF = (AddToFortF)0x004bcc20;
		}
		else
		{
			addToFortF = (AddToFortF)0x004bc690;
		}
		if (army->gen == nullptr)
		{
			MessageBoxA(NULL, "AddToFort error", "ERROR", NULL);
			return;
		}
		addToFortF(fort, army->gen);
	}

	void UngarisonSetOrFort(void* setOrFort)
	{
		typedef void(__thiscall* UngarisonSetOrFortF)(void* setOrFort);

		UngarisonSetOrFortF ungarisonSetOrFortF = nullptr;
		if (globals::dataS.gamever == 2)//steam
		{
			ungarisonSetOrFortF = (UngarisonSetOrFortF)0x004bff50;
		}
		else
		{
			ungarisonSetOrFortF = (UngarisonSetOrFortF)0x004bf9c0;
		}

		ungarisonSetOrFortF(setOrFort);
	}

	ModelDbEntry* findBattleModel(const char* modelName)
	{

		DWORD funcAdr = 0;
		DWORD paramAdr = 0;

		if (globals::dataS.gamever == 2)//steam
		{
			funcAdr = 0x010e8ee0;

			paramAdr = *(int*)0x016a0b98;
			paramAdr += 0x84;
		}
		else
		{
			funcAdr = 0x010e86c0;

			paramAdr = *(int*)0x016e9dc8;
			paramAdr += 0x84;
		}


		ModelDbEntry* res = nullptr;
		_asm {
			mov ecx, paramAdr

			push modelName

			mov eax, [funcAdr]
			call eax
			mov res, eax
		}


		return res;
	}

	campaignDb* getCampaignDb()
	{
		return reinterpret_cast<campaignDb*>(dataOffsets::offsets.campaignDb);
	}

	campaignDbExtra* getCampaignDbExtra()
	{
		return reinterpret_cast<campaignDbExtra*>(dataOffsets::offsets.campaignDbExtra);
	}

	void setUnitParams(unit* un, int count, int exp, int armor, int weap)
	{
		setSoldiersCountAndExp(un, count, exp);
		DWORD adrFunc = codes::offsets.setUnitArmorFunc;
		_asm
		{
			push armor
			mov ecx, un
			mov eax, adrFunc
			call eax
		}
		adrFunc = codes::offsets.setUnitWeapFunc;
		_asm
		{
			push weap
			mov ecx, un
			mov eax, adrFunc
			call eax
		}


		return;
	}



	DWORD allocateGameMem(size_t amount)
	{
		DWORD retMem = 0;

		DWORD adrFunc = codes::offsets.allocMemFunc;
		_asm
		{
			push amount
			mov eax, adrFunc
			call eax
			add esp, 0x4
			mov retMem, eax
		}


		return retMem;
	}

	void logFuncError(const std::string& funcName, const std::string& error)
	{
		unitActions::logStringGame(funcName + " error: " + error);
	}

	void mergeArmies(stackStruct* army, stackStruct* targetArmy)
	{
		if (army->numOfUnits + targetArmy->numOfUnits > 20)
		{
			return;
		}
		DWORD codeOffset = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			codeOffset = 0x007155F0;
		}
		else
		{
			codeOffset = 0x00714EF0;
		}
		_asm
		{
			push army
			mov ecx, targetArmy
			mov eax, codeOffset
			call eax
		}
	}

	
	std::unordered_map<int, const char*> characterTypes = {
		{0,"spy"},
		{1,"assassin"},
		{2,"diplomat"},
		{3,"admiral"},
		{4,"merchant"},
		{5,"priest"},
		{6,"general"},
		{7,"named character"},
		{8,"princess"},
		{9,"heretic"},
		{10,"witch"},
		{11,"inquisitor"},
		{13,"pope"}
	};
	
	stackStruct* spawnArmy(
		factionStruct* faction,
		const char* name,
		const char* name2,
		int characterType,
		const char* label,
		const char* portrait,
		int x,
		int y,
		int age,
		bool family,
		int subFaction,
		int unitIndex,
		int exp,
		int wpn,
		int armour
		)
	{
		if (!faction)
			return nullptr;
		if (portrait != nullptr && strlen(portrait) == 0)
			portrait = nullptr;
		if (label != nullptr && strlen(label) == 0)
			label = nullptr;
		queue<std::pair<int, int>> q;
		set<std::pair<int, int>> visited;
		q.emplace(x, y);
		visited.insert({x, y});
		stratMap* map = smallFuncs::getGameDataAll()->stratMap;
		auto spawnCoords = new coords;
		while (!q.empty()) {
			auto [curX, curY] = q.front();
			q.pop();
			if (const oneTile* tile = getTileStruct(curX, curY);
				smallFuncs::isTileFree(&curX)
				&& tile->isLand == (characterType != 3)
				&& !getTileObject(tile, 28)
				&& !getTileObject(tile, 29)
				&& !getTileObject(tile, 30))
				{
					spawnCoords->xCoord = curX;
					spawnCoords->yCoord = curY;
					break;
				}
			for (int dx = -1; dx <= 1; ++dx)
			{
				for (int dy = -1; dy <= 1; ++dy)
				{
					if (dx == 0 && dy == 0) continue;

					int newX = curX + dx;
					int newY = curY + dy;
					
					if (newX >= 0 && newX < map->mapWidth && newY >= 0 && newY < map->mapHeight
						&& !visited.count({newX, newY}))
					{
						q.emplace(newX, newY);
						visited.insert({newX, newY});
					}
				}
			}
		}
		if (!spawnCoords)
		{
			delete spawnCoords;
			return nullptr;
		}
		character* gen = nullptr;
		const char* typeName = characterTypes.find(characterType)->second;
		int factionNum = getFactionsCount();
		campaign* campaign = smallFuncs::getGameDataAll()->campaignData;
		if (label && !std::string(label).empty())
		{
			for (int i = 0; i < campaign->numberOfFactionsWithSlave; i++)
			{
				auto fac = campaign->factionsSortedByDescrStrat[i];
				for(int j = 0; j < fac->characterRecordNum; j++)
				{
					if (auto namedChar = fac->characterRecords[j]; namedChar->label != nullptr && std::string(namedChar->label) == std::string(label))
					{
						if ((namedChar->status & 8) != 0)
						{

							char** cryptS = fastFunctsHelpers::makeCryptedString(typeName);
							DWORD adrType = reinterpret_cast<DWORD>(cryptS);
							gen = GAME_FUNC(character*(__cdecl*)(DWORD, int, characterRecord*, const char*), respawnOffMapCharacterFunc)
							(adrType, faction->factionID, namedChar, portrait);
							if (gen)
							{
								DWORD adrFunc = codes::offsets.spawnCreatedObject;
							
								_asm
								{
									push spawnCoords
									push gen
									mov eax, adrFunc
									call eax
								}
							}
							break;
						}
						delete spawnCoords;
						return nullptr;
					}
				}
				if (gen)
					break;
			}
		}
		if (!gen)
		{
			int nameFaction = faction->agentNameFactionId[characterType];
			if (subFaction != 31)
				nameFaction = subFaction;
			if (const auto nameS = std::string(name); nameS == "random_name")
			{
				const int checkCountMax = faction->characterRecordNum * 2;
				int checkCount = 0;
				int firstNameIndex = 0;
				int secondNameIndex = 0;
				while (checkCount < checkCountMax)
				{
					GAME_FUNC(int(__cdecl*)(int*, int, int, int*, int*), getRandomNameFunc)
					(&campaign->lastrandomseed, nameFaction, 0, &firstNameIndex, &secondNameIndex);
					bool research = false;
					for(int i = 0; i < faction->characterRecordNum; i++)
					{
						if (const int nameIndex = GAME_FUNC(int(__thiscall*)(characterRecord*), getNameIndexFunc)(faction->characterRecords[i]);
							firstNameIndex == nameIndex)
						{
							research = true;
							break;
						}
					}
					checkCount++;
					if (research)
						continue;
					break;
				}

				name = GAME_FUNC(const char*(__cdecl*)(int, int, int), getCharacterName)(0, nameFaction, firstNameIndex);
				name2 = GAME_FUNC(const char*(__cdecl*)(int, int, int), getCharacterName)(2, nameFaction, secondNameIndex);
			}
			gen = characterHelpers::createCharacterWithoutSpawning(typeName, faction, age, name, name2, subFaction, portrait, x, y);
			auto namedChar = gen->characterRecord;
			namedChar->age ^= (namedChar->age ^ (family << 13)) & 0x2000u;
			if (!faction->leader)
			{
				namedChar->age |= 0x2000u;
				DWORD codeOffset = codes::offsets.setFactionLeaderFunc;
				_asm
				{
					push namedChar
					mov ecx, faction
					mov eax, codeOffset
					call eax
				}
			}
			if ( (namedChar->age & 0x2000) != 0 )
			{
				DWORD facDWORD = reinterpret_cast<DWORD>(faction);
				facDWORD += 0xEC8;
				auto parent = GAME_FUNC(characterRecord*(__thiscall*)(DWORD, characterRecord*), findParentForAdoptionFunc)
				(facDWORD, namedChar);
				if (parent)
				{
					namedChar->parent = parent;
					int childNum = parent->numberOfChildren;
					parent->numberOfChildren = childNum + 1;
					namedChar->parent->childs[childNum] = namedChar;
				}
			}
		}
		if (gen)
		{
			
			DWORD adrFunc = codes::offsets.doSomeWithCharacterFunc;
			void* some = faction->tilesFac;
			_asm
			{
				push 0
				push gen
				mov ecx, some
				mov eax, adrFunc
				call eax
			}
				
			stackStruct* army = createArmy(gen);
			const oneTile* tile = getTileStruct(spawnCoords->xCoord, spawnCoords->yCoord);
			int regionId = tile->regionId;
			GAME_FUNC(void(__thiscall*)(stratMap*, stackStruct*, int), setArmyRegionEntriesFunc)(map, army, regionId);
			unit* bgUnit = nullptr;
			if (eduThings::getEduEntry(unitIndex))
			{
				bgUnit = createUnitIdx(unitIndex, regionId, faction->factionID, exp, armour, wpn);
			}
			else
			{
				int eopIDX = eduThings::getDataEopEdu(unitIndex);
				if (eopIDX == 0)
					bgUnit = createUnitIdx(0, regionId, faction->factionID, exp, armour, wpn);
				else
					bgUnit = createUnitEDB(eopIDX, regionId, faction->factionID, exp, armour, wpn);
			}
			addUnitToArmy(army, bgUnit);
			if (characterType == 7)
				setBodyguard(gen, bgUnit);
			adrFunc = codes::offsets.factionRessurectStuffFunc;
			_asm
			{
				mov ecx, faction
				push spawnCoords
				mov eax, adrFunc
				call eax
			}
			delete spawnCoords;
			if (army && label && strcmp(label, "") == 0)
				fastFunctsHelpers::setCryptedString(&gen->characterRecord->label, label);
			return army;
		}
		delete spawnCoords;
		return nullptr;
	}
	
	DWORD* getTileObject(const oneTile* tile, int type)
	{
		DWORD* object = tile->object;

		while (object)
		{
			int objectType = CallVFunc<4, int>(object);
			if (objectType == type || (type == 0 && objectType < 28))
				return object;
			object = reinterpret_cast<DWORD*>(*(object + 1));
		}

		return nullptr;
	}
	void toggleDeveloperMode()
	{
		auto& developerMode = globals::dataS.Modules.developerMode;
		developerMode.toggleDeveloperModeBase();
	}
}
