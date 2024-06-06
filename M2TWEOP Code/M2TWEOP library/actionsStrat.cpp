#include "actionsStrat.h"

#include "eduThings.h"
#include "unitActions.h"

namespace actionsStrat {
	NOINLINE EOP_EXPORT void Idle(general* gen)
	{
		DWORD mem = fastFuncts::allocateGameMem(0x301C);
		if (mem == 0)return;

		mem = getReadyIdleAction(mem);
		getReadySiegeActionChar(mem, gen);
		finalizeAction(mem, gen);
	}
	
	NOINLINE EOP_EXPORT void siegeSettlement(general* gen, settlementStruct* sett, bool isAttack)
	{
		//if general or namedchar or sett is nil or general isnt alive return
		if (!gen || !gen->genChar || !sett || (gen->genChar->age & 1) == 0)
			return;

		//if maintain option is on and general is leading an army and the army is sieging the settlement then do maintain stuff
		if (!isAttack && gen->armyLeaded && gen->armyLeaded->siege && gen->armyLeaded->siege->goal == sett)
		{
			//Some array where it will put what kind of siege equipment it wants
			DWORD pref[5] {};
			
			//Game function to fill that array
			if (!GAME_FUNC(bool(__cdecl*)(stackStruct* armyP, DWORD* prefP, settlementStruct* settlementP),
				getEquipmentPreferencesFunc)(gen->armyLeaded, &pref[0], sett))
			{
				//Loop through the array and decide what equipment to use
				for (int i = 0; pref[i] != 3 && i < 5; i++)
				{
					GAME_FUNC(char(__thiscall*)(siegeS* siegeP, DWORD currPrefP),
						decideEquipmentFunc)(gen->armyLeaded->siege, pref[i]);
				}
			}
			return;
		}

		//Allocate memory for the CAD class
		auto cadClass = fastFuncts::allocateGameMem(isAttack && sett->army ? 0x3028 : 0x3024);
		if (cadClass == 0) return;

		//Select appropriate function to create the relevant CAD class
		//CAD_ATTACK_SETTLEMENT
		if (!sett->army)
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, settlementStruct* settlementP, int a3),
				createCadAttackSettlementFunc)(cadClass, sett, 0);
		
		//CAD_ASSAULT_SETTLEMENT
		else if (isAttack)
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2, settlementStruct* settlementP, int a4),
				createCadAssaultSettlementFunc)(cadClass, 0, sett, 0);
		
		//CAD_SIEGE_SETTLEMENT
		else
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, settlementStruct* settlementP, int a3),
				createCadSiegeSettlementFunc)(cadClass, sett, 0);
		
		if (cadClass == 0) return;
		
		GAME_FUNC(DWORD(__thiscall*)(general* character, DWORD cadClassP, int a3),
			getReadySiegeCharacterGarrisonedFunc)(gen, cadClass, 0);
		
		finalizeAction(cadClass, gen);
		CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
	}
	
	NOINLINE EOP_EXPORT void siegeFort(general* gen, fortStruct* fort, bool isAttack)
	{
		//if general or namedchar or sett is nil or general isnt alive return
		if (!gen || !gen->genChar || !fort || (gen->genChar->age & 1) == 0)
			return;

		//if maintain option is on and general is leading an army and the army is sieging the settlement then do maintain stuff
		if (!isAttack && gen->armyLeaded && gen->armyLeaded->siege && reinterpret_cast<fortStruct*>(gen->armyLeaded->siege->goal) == fort)
		{
			//Some array where it will put what kind of siege equipment it wants
			DWORD pref[5] {};
			
			//Game function to fill that array
			if (!GAME_FUNC(bool(__cdecl*)(stackStruct*, DWORD*, fortStruct*),
				getEquipmentPreferencesFunc)(gen->armyLeaded, &pref[0], fort))
			{
				//Loop through the array and decide what equipment to use
				for (int i = 0; pref[i] != 3 && i < 5; i++)
				{
					GAME_FUNC(char(__thiscall*)(siegeS* siegeP, DWORD currPrefP),
						decideEquipmentFunc)(gen->armyLeaded->siege, pref[i]);
				}
			}
			return;
		}

		//Allocate memory for the CAD class
		auto cadClass = fastFuncts::allocateGameMem(isAttack && fort->army ? 0x302C : 0x3028);
		if (cadClass == 0) return;
		
		struct TrackedPointerFort { void* vtable; fortStruct* fort; };
		
		TrackedPointerFort trackedPointer{};
		trackedPointer = *GAME_FUNC(TrackedPointerFort*(__thiscall*)(TrackedPointerFort* trackedFort, fortStruct* fortP),
		makeTrackedPointerFortFunc)(&trackedPointer, fort);
		
		//Select appropriate function to create the relevant CAD class
		//CAD_ATTACK_FORT
		if (!fort->army)
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, TrackedPointerFort* fortP, int a3),
				createCadAttackFortFunc)(cadClass, &trackedPointer, 0);
		
		//CAD_ASSAULT_FORT
		else if (isAttack)
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2, TrackedPointerFort* fortP, int a4),
				createCadAssaultFortFunc)(cadClass, 0, &trackedPointer, 0);
		
		//CAD_SIEGE_FORT
		else
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, TrackedPointerFort* fortP, int a3),
				createCadSiegeFortFunc)(cadClass, &trackedPointer, 0);
		
		if (cadClass == 0) return;
		
		GAME_FUNC(void(__thiscall*)(TrackedPointerFort* trackedPointerP),
		   somethingWithTrackedPointerFortFunc)(&trackedPointer);
		
		finalizeAction(cadClass, gen);
		CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
	}

	NOINLINE EOP_EXPORT void moveNormal(general* gen, int x, int y)
	{
		if (!gen)
			return;
		DWORD cadClass = fastFuncts::allocateGameMem(0x301C);
		if (cadClass == 0) return;
		cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2),
			createCadMovingNormalFunc)(cadClass, 0);
		if (cadClass == 0) return;
		
		if (!GAME_FUNC(bool(__thiscall*)(general* character, DWORD cadClassP, int xCoord, int yCoord, int a5, int a6),
			getReadyForMoving2Func)(gen, cadClass, x, y, 0, 0))
		{
			CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
			return;
		}
		DWORD adrFunc = codes::offsets.finalyzeActionStratmapFunc;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
	}

	NOINLINE EOP_EXPORT void diplomacyCharacter(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, 20);
	}

	NOINLINE EOP_EXPORT void assassinate(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, 0x1A);
	}

	NOINLINE EOP_EXPORT void marry(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter || !gen->genType || !targetCharacter->genType)
			return;
		if (gen->genType->type != characterTypeStrat::princess)
		{
			fastFuncts::logFuncError("character.marry", "character is not princess");
			return;
		}
		if (targetCharacter->genType->type != characterTypeStrat::namedCharacter)
		{
			fastFuncts::logFuncError("character.marry", "target character is not family member");
			return;
		}
		targetCharacterAction(gen, targetCharacter, 0x22);
	}

	NOINLINE EOP_EXPORT void spyCharacter(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, 0x19);
	}

	NOINLINE EOP_EXPORT void switchCharacterFaction(general* gen, factionStruct* fac, bool keepArmy, bool keepBg)
	{
		std::string functionName = "character.switchFaction";
		if (!gen || !fac || !gen->genType)
		{
			fastFuncts::logFuncError(functionName, "gen or fac or genType is nil");
			return;
		}
		if (gen->genType->type == characterTypeStrat::admiral)
		{
			fastFuncts::logFuncError(functionName, "unsupported character type admiral");
			return;
		}
		if (gen->genType->type == characterTypeStrat::princess)
		{
			fastFuncts::logFuncError(functionName, "unsupported character type princess");
			return;
		}
		if (gen->armyNotLeaded)
		{
			fastFuncts::logFuncError(functionName, "character is part of an army");
			return;
		}
		if (gen->armyLeaded && (gen->armyLeaded->shipArmy || gen->armyLeaded->charactersNum != 0))
		{
			fastFuncts::logFuncError(functionName, "character is boarded or has auxiliary characters");
			return;
		}
		if (gen->genType->type == characterTypeStrat::namedCharacter && !gen->armyLeaded)
		{
			fastFuncts::logFuncError(functionName, "character is in a residence or off-map");
			return;
		}
		
		struct unitInfo
		{
			int xp{};
			int armour{};
			int weapon{};
			int soldierCount{};
			UNICODE_STRING** alias{};
			int supplies{};
			int eduType{};
			unitInfo() = default;
			explicit unitInfo(const unit* unit)
			{
				xp = unit->expScreen;
				armour = (unit->stats >> 8) & 0x1F;
				weapon = (unit->stats >> 13) & 0x1F;
				soldierCount = unit->SoldierCountStrat;
				alias = unit->alias;
				supplies = unit->foodRequirement;
				eduType = unit->eduEntry->Index;
			}
			[[nodiscard]] unit* CreateUnit(const factionStruct* faction, const general* character) const
			{
				unit* newUnit;
				if (eduThings::getDataEopEdu(eduType))
					 newUnit = fastFuncts::createUnitEDB(eduType, character->regionID, faction->dipNum, xp, armour, weapon);
				else
					 newUnit = fastFuncts::createUnitIdx(eduType, character->regionID, faction->dipNum, xp, armour, weapon);
				newUnit->alias = alias;
				newUnit->foodRequirement = supplies;
				newUnit->SoldierCountStrat = soldierCount;
				return newUnit;
			}
		};
		
		unitInfo bgInfo{};
		std::vector<unitInfo> unitsInfo;
		if ((gen->genType->type == characterTypeStrat::namedCharacter
			|| gen->genType->type == characterTypeStrat::general) && gen->armyLeaded)
		{
			if (keepBg && gen->bodyguards)
				bgInfo = unitInfo(gen->bodyguards);
			if (keepArmy)
			{
				for (int i = 0; i < gen->armyLeaded->numOfUnits; ++i)
				{
					auto un = gen->armyLeaded->units[i];
					if (un->generalInfo || un == gen->bodyguards)
						continue;
					if ((un->eduEntry->ownership & (1 << fac->dipNum)) == 0)
						unitsInfo.emplace_back(un);
				}
			}
		}

		GAME_FUNC(void(__thiscall*)(general*), changeCharacterTileStuff)(gen);

		DWORD funcAddr = codes::offsets.switchCharacterFaction;
		_asm
		{
			push 0
			push 0
			push fac
			mov ecx, gen
			mov eax, funcAddr
			call eax
		}

		if (gen->genType->type == characterTypeStrat::namedCharacter)
		{
			DWORD funcAddr2 = codes::offsets.switchNamedCharacterFaction;
			_asm
			{
				push 0
				push fac
				mov ecx, gen
				mov eax, funcAddr2
				call eax
			}
		}
		
		if ((gen->genType->type == characterTypeStrat::namedCharacter
			|| gen->genType->type == characterTypeStrat::general) && gen->armyLeaded)
		{
			if (keepBg && gen->genType->type == characterTypeStrat::namedCharacter)
			{
				auto newUnit = bgInfo.CreateUnit(fac, gen);
				if (gen->armyLeaded && gen->armyLeaded->numOfUnits >= 20)
				{
					auto bodyguard = gen->bodyguards;
					auto randomUnit = gen->armyLeaded->units[17];
					fastFuncts::setBodyguard(gen, randomUnit);
					fastFuncts::killUnit(bodyguard);
					fastFuncts::addUnitToArmy(gen->armyLeaded, newUnit);
					fastFuncts::setBodyguard(gen, newUnit);
				}
				else
				{
					auto bodyguard = gen->bodyguards;
					fastFuncts::setBodyguard(gen, newUnit);
					fastFuncts::addUnitToArmy(gen->armyLeaded, newUnit);
					fastFuncts::killUnit(bodyguard);
				}
			}
			if (keepArmy)
			{
				for (auto& un : unitsInfo)
				{
					auto newUnit = un.CreateUnit(fac, gen);
					fastFuncts::addUnitToArmy(gen->armyLeaded, newUnit);
				}
			}
		}
		GAME_FUNC(void(__thiscall*)(general*), initPlaceCharacter)(gen);
	}

	NOINLINE EOP_EXPORT void denounce(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, 0x23);
	}

	NOINLINE EOP_EXPORT void bribe(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, 0x15);
	}

	NOINLINE EOP_EXPORT void acquire(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, 0x21);
	}

	NOINLINE EOP_EXPORT void targetCharacterAction(general* gen, general* targetCharacter, int type)
	{
		if (!gen || !targetCharacter)
			return;
		stackStruct* army = nullptr;
		if (gen->armyLeaded)
			army = gen->armyLeaded;
		gen->armyLeaded = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(general**, general**, int, int),
		createCADTargetCharacter)(&gen, &targetCharacter, type, 0);
		gen->armyLeaded = army;
		DWORD cadClass =0x0162C740;
		if (smallFuncs::getGameVersion() == 1)
			cadClass = 0x1674570;
		cadClass = *reinterpret_cast<DWORD*>(cadClass);
		DWORD adrFunc = codes::offsets.finalyzeActionStratmapFunc;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
	}

	NOINLINE EOP_EXPORT void diplomacySettlement(general* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		
		DWORD cadClass = fastFuncts::allocateGameMem(0x3024);
		if (cadClass == 0) return;
		cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, settlementStruct* sett, int a3),
			makeCadDiplomacySettlementFunc)(cadClass, targetSettlement, 0);
		if (cadClass == 0) return;
		
		GAME_FUNC(DWORD(__thiscall*)(general* character, DWORD cadClassP, int a3),
			getReadySiegeCharacterGarrisonedFunc)(gen, cadClass, 0);

		DWORD adrFunc = codes::offsets.finalyzeActionStratmapFunc;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
	}

	NOINLINE EOP_EXPORT void attackCharacter(general* attacker, general* defender)
	{
		if (!attacker || !defender)
			return;
		
		DWORD cadClass = fastFuncts::allocateGameMem(0x302C);
		if (cadClass == 0) return;
		
		
		struct TrackedPointerCharacter { void* vtable; general* character;};
		TrackedPointerCharacter trackedPointer{};
		trackedPointer = *GAME_FUNC(TrackedPointerCharacter*(__thiscall*)(TrackedPointerCharacter* trackedChar, general* character),
		makeTrackedPointerToAttackFunc)(&trackedPointer, defender);
		
		cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2, TrackedPointerCharacter* trackedPointerP, int a4),
			makeCadAttackFunc)(cadClass, 0, &trackedPointer, 0);
		
		GAME_FUNC(void(__thiscall*)(TrackedPointerCharacter* trackedPointerP),
		   somethingWithTrackedPointerAttackFunc)(&trackedPointer);
		
		finalizeAction(cadClass, attacker);
		//Destructor -- game doesnt call it, but I think it might be a bug
		CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); 
	}


}
