#include "pch.h"
#include "fastFuncts.h"

#include "fastFunctsHelpers.h"
namespace fastFuncts
{
	NOINLINE EOP_EXPORT void setCryptedString(char* targetS, const char* newS)
	{
		UINT32 functionOffset = codes::offsets.stringCryptFunc;
		_asm {
			push newS
			mov ecx, targetS
			mov eax, functionOffset
			call eax
		}
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

	NOINLINE EOP_EXPORT UINT32 getFactionsCount()
	{
		UINT32 count = 0;

		techFuncs::Read(dataOffsets::offsets.factionOffsetsStart + 0x324, &count);

		return count;
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
				if (listFac[i]->portBuildings[j]->x == x
					&&
					listFac[i]->portBuildings[j]->y == y
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
						return setS->resources[k];
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
			push x
			push y

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
			push x
			push y

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

		return;
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

	NOINLINE EOP_EXPORT void setSoldiersCount(unit* un, int count)
	{
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

	NOINLINE EOP_EXPORT general* createCharacter(char* type, factionStruct* fac, int age, char* name, char* name2, int subFaction, char* portrait, int x, int y)
	{
		DWORD adrFunc = codes::offsets.createCharacterFunc;

		general* gen = nullptr;

		char** cryptS = (char**)malloc(8);
		if (cryptS == nullptr)
		{
			return gen;
		}
		cryptS[0] = _strdup(type);
		setCryptedString((char*)cryptS, type);
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

	NOINLINE EOP_EXPORT unit* createUnitN(const char* type, int facNum, int exp, int arm, int weap)
	{
		int unitIndex = fastFunctsHelpers::getEDUIndex(type);


		return createUnitIdx(unitIndex, facNum, exp, arm, weap);
	}

	NOINLINE EOP_EXPORT unit* createUnitIdx(int index, int facNum, int exp, int arm, int weap)
	{
		if (index == -1)return nullptr;

		unit* res = nullptr;

		DWORD EDB = dataOffsets::offsets.unitTypesStart - 4;
		DWORD adr = codes::offsets.createUnitFunc;
		_asm {
				mov ecx, EDB;

				push weap
				push arm
				push - 1
				push exp
				push facNum
				push index
				push 0

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

	NOINLINE EOP_EXPORT void setUnitParams(unit* un, int count, int exp, int armor, int weap)
	{
		setSoldiersCount(un, count);
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