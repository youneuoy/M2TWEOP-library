#include "pch.h"
#include "fastFuncts.h"

#include "fastFunctsHelpers.h"
namespace fastFuncts
{


	NOINLINE EOP_EXPORT factionStruct* getRegionOwner(int regionID)
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
	NOINLINE EOP_EXPORT void setSettlementOwner(settlementStruct* sett, factionStruct newOwner)
	{
		DWORD adrFunc = 0x0;
		int* vTableAdr = (int*)sett->vTable;
		adrFunc = vTableAdr[39];

		//give settlement
		__asm
		{
			push 5
			push newOwner
			push 0
			mov ecx, sett
			mov eax, adrFunc
			call eax
		}

		adrFunc = 0x0;

		//release sieges
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x004bf8d0;
		}
		else
		{
			adrFunc = 0x004bf340;
		}
		__asm
		{
			push 1
			mov ecx, sett
			mov eax, adrFunc
			call eax
		}

	}
	NOINLINE EOP_EXPORT void setCharacterType(general* character, int typeID, int subFaction, int factionDipNum)
	{
		DWORD adrFunc = 0x0;
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x008b4200;
		}
		else
		{
			adrFunc = 0x008b3810;
		}

		genMod* retVal = nullptr;

		__asm
		{
			push factionDipNum
			push subFaction
			push typeID
			mov eax, adrFunc
			call eax
			mov retVal, eax
		}
		character->genType = retVal;
	}
	NOINLINE EOP_EXPORT UINT32 getTileRegionID(int x, int y)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);
		UINT32 redID = gameDataAll->stratMap->tilesArr[gameDataAll->stratMap->xBound * y + x].regionId;


		return redID;
	}
	NOINLINE EOP_EXPORT regionStruct* getRegionByID(UINT32 regionID)
	{
		gameDataAllStruct* gameDataAll = reinterpret_cast<gameDataAllStruct*>(dataOffsets::offsets.gameDataAllOffset);

		return &gameDataAll->stratMap->regionsArr[regionID];
	}

	NOINLINE EOP_EXPORT factionStruct** getFactionsList()
	{
		factionStruct** list;
		list = reinterpret_cast<factionStruct**>(dataOffsets::offsets.factionOffsetsStart + 0x22c);
		return list;
	}

	NOINLINE EOP_EXPORT UINT32 getPassedTurnsNum()
	{
		UINT32 count = 0;
		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x378, &count);

		return count;
	}

	NOINLINE EOP_EXPORT UINT32 getSeason()
	{
		UINT32 season = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x710, &season);

		return season;
	}

	NOINLINE EOP_EXPORT UINT32 getYear()
	{
		UINT32 year = 0;
		float t = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x70c, &t);

		year = (UINT32)t;
		return year;
	}

	NOINLINE EOP_EXPORT void setHeir(generalCharacterictics* gen, bool isJustSet)
	{
		factionStruct* fac = gen->faction;
		if (isJustSet == true)
		{
			for (UINT32 i = 0; i < fac->numOfCharacters; i++)
			{
				if (fac->characters[i]->genChar->status & 2)
				{
					fac->characters[i]->genChar->status = 0;
				}
			}

			fac->heir = gen;

			gen->status = 2;
		}

		else
		{
			_asm {
				push gen
				mov ecx, fac
				mov eax, [fac]
				mov eax, [eax]
				mov eax, [eax + 0x20]
				call eax
			}
		}


		return;
	}

	NOINLINE EOP_EXPORT UINT32 getFactionsCount()
	{
		UINT32 count = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x324, &count);

		return count;
	}

	NOINLINE EOP_EXPORT guild* getGuild(unsigned char index)
	{
		uintptr_t currentOffsett = dataOffsets::offsets.guildDataStart;
		UINT32 count = 0;
		UINT32 maxCount = 0;
		UINT8 currID = 0;

		do {
			techFuncs::Read(currentOffsett + 0x10, &count);
			techFuncs::Read(currentOffsett + 0xC, &maxCount);
			uintptr_t guilds = *(uintptr_t*)currentOffsett;

			if (guilds == 0)
			{
				return nullptr;
			}

			for (int i = 0; i < count; ++i)
			{
				if (index == currID)
				{
					return  (guild*)(guilds + 0x4C * i);
				}
				++currID;
			}

			if (count < maxCount || *(uintptr_t*)(currentOffsett + 0x4) == 0)
			{
				return nullptr;
			}
			currentOffsett = *(uintptr_t*)(currentOffsett + 0x4);

		} while (*(uintptr_t*)currentOffsett != 0);

		return nullptr;
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

	NOINLINE EOP_EXPORT stackStruct* findArmy(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->stackNum; j++)
			{
				auto armyCoords = getArmyCoords(listFac[i]->stacks[j]);
				if (armyCoords.first == x
					&&
					armyCoords.second == y
					)
				{
					return listFac[i]->stacks[j];
				}
			}
		}

		return nullptr;
	}

	NOINLINE EOP_EXPORT fortStruct* findFort(int x, int y)
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

	NOINLINE EOP_EXPORT portBuildingStruct* findPort(int x, int y)
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

	NOINLINE EOP_EXPORT resStrat* findResource(int x, int y)
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

	NOINLINE EOP_EXPORT settlementStruct* findSettlement(int x, int y)
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

	NOINLINE EOP_EXPORT watchTowerStruct* findWatchTower(int x, int y)
	{
		UINT32 numFac = fastFuncts::getFactionsCount();
		factionStruct** listFac = fastFuncts::getFactionsList();

		for (UINT32 i = 0; i < numFac; i++)
		{
			for (int j = 0; j < listFac[i]->wathtowersNum; j++)
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

	NOINLINE EOP_EXPORT void moveStratCameraSlow(int x, int y)
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

	NOINLINE EOP_EXPORT void moveStratCameraFast(int x, int y)
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

	NOINLINE EOP_EXPORT void zoomStratCamera(float zoom)
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

	NOINLINE EOP_EXPORT void teleportCharacter(general* gen, int x, int y)
	{
		DWORD adrFunc = codes::offsets.teleportCharacterFunc;
		_asm
		{
			push y
			push x

			mov ecx, gen
			mov eax, adrFunc
			call eax
		}

		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x00599900;
		}
		else
		{
			adrFunc = 0x00599410;
		}

		_asm
		{
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		return;
	}

	NOINLINE EOP_EXPORT void addTrait(generalCharacterictics* character, const char* traitName, int traitLevel)
	{

		DWORD adrFunc = 0;



		//getTrait
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x008b7070;
		}
		else
		{
			adrFunc = 0x008b6680;
		}
		char** cryptS = fastFunctsHelpers::makeCryptedString(traitName);


		void* resTrait = nullptr;

		auto oneArg = cryptS[0];
		auto nextArg = cryptS[1];
		_asm
		{
			push nextArg
			push oneArg

			mov eax, adrFunc
			call eax

			mov resTrait, eax
			add esp, 0x8
		}
		if (resTrait == nullptr)
		{
			return;
		}

		//set trait
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x005a7bf0;
		}
		else
		{
			adrFunc = 0x005a7710;
		}

		_asm
		{
			push 1
			push traitLevel
			push resTrait
			mov ecx, character

			mov eax, adrFunc
			call eax
		}
	}

	NOINLINE EOP_EXPORT void removeTrait(generalCharacterictics* character, const char* traitName)
	{
		DWORD adrFunc = 0;



		//getTrait
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x008b7070;
		}
		else
		{
			adrFunc = 0x008b6680;
		}
		char** cryptS = fastFunctsHelpers::makeCryptedString(traitName);


		void* resTrait = nullptr;

		auto oneArg = cryptS[0];
		auto nextArg = cryptS[1];
		_asm
		{
			push nextArg
			push oneArg

			mov eax, adrFunc
			call eax

			mov resTrait, eax
			add esp, 0x8
		}
		if (resTrait == nullptr)
		{
			return;
		}

		//set trait
		if (globals::dataS.gamever == 2)//steam
		{
			adrFunc = 0x005a5880;
		}
		else
		{
			adrFunc = 0x005a53a0;
		}

		_asm
		{
			push resTrait
			mov ecx, character

			mov eax, adrFunc
			call eax
		}
	}

	EOP_EXPORT int addAnchillary(generalCharacterictics* character, anchillary* anch)
	{
		if (character == nullptr || anch == nullptr)return 0;

		DWORD adr = 0;
		int retr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			adr = 0x005a5d50;
		}
		else
		{
			adr = 0x005a5870;
		}
		_asm
		{
			mov ecx, character
			push anch
			mov eax, adr
			call eax
			mov retr, eax
		}


		return retr;
	}

	EOP_EXPORT void removeAnchillary(generalCharacterictics* character, anchillary* anch)
	{
		if (character == nullptr || anch == nullptr)return;

		DWORD adr = 0;
		int ret = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			adr = 0x005a5e70;
		}
		else
		{
			adr = 0x005a5990;
		}
		_asm
		{
			mov ecx, character
			push anch
			mov eax, adr
			call eax
		}

		return;
	}

	EOP_EXPORT anchillary* findAnchillary(char* anchName)
	{
		if (anchName == nullptr)return 0;

		DWORD adr = 0;
		anchillary* retr = nullptr;
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
			push anchName
			mov eax, adr
			call eax
			add esp, 4
			mov retr, eax
		}


		return retr;
	}

	NOINLINE EOP_EXPORT void setSoldiersCountAndExp(unit* un, int count, int exp)
	{
		un->expScreen = exp;
		setSoldiersCount(un, count);
	}
	NOINLINE EOP_EXPORT void setUnitMovepoints(unit* un, float movepoints)
	{
		un->movePoints = movepoints;


		if (un->general == nullptr)
		{
			general* gen = un->army->gen;
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
		un->general->movepoints1 = movepoints;
		un->general->movepoints2 = movepoints;


		general* gen = un->general;

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
	NOINLINE EOP_EXPORT void setSoldiersCount(unit* un, int count)
	{
		if (count == 0)
		{
			killUnit(un);
			return;
		}



		int diff = count - un->number;
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
			un->number = count;
		}



		return;
	}

	NOINLINE EOP_EXPORT void killUnit(unit* un)
	{
		if (un->general)
		{
			killCharacter(un->general);
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

	NOINLINE EOP_EXPORT void killCharacter(general* gen)
	{
		DWORD adr = codes::offsets.killCharStratMapFunc;

		_asm {
			push 0x1
			mov ecx, [gen]
			mov eax, [adr]
			call eax
		}
	}

	NOINLINE EOP_EXPORT void destroyBuilding(settlementStruct* sett, const char* typeName, bool isReturnMoney)
	{

		DWORD adr = codes::offsets.destroyBuildingFunc;
		char** buildTypeS = fastFunctsHelpers::makeCryptedString(typeName);
		if (buildTypeS == nullptr)return;
		char* buildType = buildTypeS[0];
		char* hash = buildTypeS[1];
		_asm {
			push isReturnMoney
			push hash
			push buildType
			mov ecx, [sett]
			mov eax, [adr]
			call eax
		}
		return;
	}

	NOINLINE EOP_EXPORT bool useButton(const char* buttonName)
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
	NOINLINE EOP_EXPORT uiElement* getUiElement(const char* elementName)
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
	NOINLINE EOP_EXPORT void useUiElement(uiElement* element)
	{
		DWORD adrF = codes::offsets.useButtonFunc;
		_asm
		{
			mov ecx, element
			mov eax, adrF
			call eax
		}
	}
	NOINLINE EOP_EXPORT void autoResolve()
	{
		DWORD adrFunc = codes::offsets.autoResolveFunc;

		_asm
		{
			mov eax, adrFunc
			call eax
		}
	}
	NOINLINE EOP_EXPORT bool autoWin(const char* winnerSide)
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
	NOINLINE EOP_EXPORT bool callGameConsoleCommand(const char* name, const char* arg, char* errorBuffer)
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
	NOINLINE EOP_EXPORT void createBuilding(settlementStruct* sett, const char* building_level_id)
	{
		DWORD adrFunc = codes::offsets.createBuildingFunc;
		string command = sett->name;
		command.push_back(' ');
		command += building_level_id;
		char buffer[100]{};
		const char* cmdC = command.c_str();
		_asm
		{
			lea eax, buffer
			push eax
			push cmdC
			mov eax, adrFunc
			call eax
		}
	}

	NOINLINE EOP_EXPORT general* createCharacterWithoutSpawning(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y)
	{
		DWORD adrFunc = codes::offsets.createCharacterFunc;

		general* gen = nullptr;

		char** cryptS = fastFunctsHelpers::makeCryptedString(type);

		DWORD adrType = (DWORD)cryptS;
		_asm
		{
			push portrait
			push subFaction
			push name2
			push name
			push age
			push fac
			push adrType
			mov eax, adrFunc
			call eax
			mov gen, eax
			add esp, 0x1c
		}
		struct xyS
		{
			int x = 0;
			int y = 0;
		}xy;
		xy.x = x;
		xy.y = y;


		adrFunc = codes::offsets.spawnCreatedCharacterFunc;
		xyS* xyP = &xy;

		_asm
		{
			push xyP
			push gen
			mov eax, adrFunc
			call eax
		}

		return gen;
	}
	NOINLINE EOP_EXPORT general* createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y)
	{
		DWORD adrFunc = codes::offsets.createCharacterFunc;

		general* gen = nullptr;

		char** cryptS = fastFunctsHelpers::makeCryptedString(type);

		DWORD adrType = (DWORD)cryptS;
		_asm
		{
			push portrait
			push subFaction
			push name2
			push name
			push age
			push fac
			push adrType
			mov eax, adrFunc
			call eax
			mov gen, eax
			add esp, 0x1c
		}
		struct xyS
		{
			int x = 0;
			int y = 0;
		}xy;
		xy.x = x;
		xy.y = y;


		adrFunc = codes::offsets.spawnCreatedCharacterFunc;
		xyS* xyP = &xy;

		_asm
		{
			push xyP
			push gen
			mov eax, adrFunc
			call eax
		}

		adrFunc = codes::offsets.doSomeWithCharacterFunc;
		DWORD some = fac->someForSpawnCharacter;
		_asm
		{
			push 0
			push gen
			mov ecx, some
			mov eax, adrFunc
			call eax
		}

		return gen;
	}

	NOINLINE EOP_EXPORT stackStruct* createArmy(general* character)
	{
		stackStruct* stack = nullptr;

		factionStruct* fac = character->genChar->faction;
		DWORD adrFunc = codes::offsets.createArmyFunc;
		_asm
		{
			push 0
			push character
			mov ecx, fac
			mov eax, adrFunc
			call eax
			mov stack, eax
		}


		return stack;
	}

	NOINLINE EOP_EXPORT stackStruct* createArmyInSettlement(settlementStruct* sett)
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

	NOINLINE EOP_EXPORT unit* createUnitN(const char* type, int regionID, int facNum, int exp, int arm, int weap)
	{
		int unitIndex = fastFunctsHelpers::getEDUIndex(type);


		return createUnitIdx(unitIndex, regionID, facNum, exp, arm, weap);
	}

	NOINLINE EOP_EXPORT unit* createUnitIdx(int index, int regionID, int facNum, int exp, int arm, int weap)
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

	NOINLINE EOP_EXPORT unit* createUnitEDB(int edb, int regionID, int facNum, int exp, int arm, int weap)
	{
		if (edb == 0)
		{
			return nullptr;
		}
		unit* res = nullptr;

		DWORD adr = codes::offsets.createUnitFunc;

		regionStruct* region = getRegionByID(regionID);
		_asm {
			mov ecx, edb;

			push weap
				push arm
				push - 1
				push exp
				push facNum
				push 0
				push region

				mov eax, [adr]
				call eax
				mov res, eax
		}

		return res;
	}

	NOINLINE EOP_EXPORT int addUnitToArmy(stackStruct* army, unit* un)
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

	NOINLINE EOP_EXPORT void setBodyguard(general* gen, unit* un)
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

	NOINLINE EOP_EXPORT ModelDbEntry* findBattleModel(const char* modelName)
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

	NOINLINE EOP_EXPORT void setUnitParams(unit* un, int count, int exp, int armor, int weap)
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



	EOP_EXPORT DWORD allocateGameMem(size_t amount)
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

}