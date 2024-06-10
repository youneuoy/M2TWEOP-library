#include "actionsStrat.h"

#include "eduThings.h"
#include "stackStructHelpers.h"
#include "unitActions.h"

namespace actionsStrat {
	
	void idle(general* gen)
	{
		DWORD mem = fastFuncts::allocateGameMem(0x301C);
		if (mem == 0)return;

		mem = getReadyIdleAction(mem);
		getReadySiegeActionChar(mem, gen);
		finalizeAction(mem, gen);
	}
	
	void siegeSettlement(general* gen, settlementStruct* sett, bool isAttack)
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
		
		if (!isAttack)
		{
			targetSettlementAction(gen, sett, characterAction::besiege);
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
	
	void siegeFort(general* gen, fortStruct* fort, bool isAttack)
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
		
		if (!isAttack)
		{
			targetFortAction(gen, fort, characterAction::besiege);
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
		{
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, TrackedPointerFort* fortP, int a3),
				createCadAttackFortFunc)(cadClass, &trackedPointer, 0);
		}
		//CAD_ASSAULT_FORT
		else if (isAttack)
		{
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2, TrackedPointerFort* fortP, int a4),
				createCadAssaultFortFunc)(cadClass, 0, &trackedPointer, 0);
		}
		//CAD_SIEGE_FORT
		else
		{
			cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, TrackedPointerFort* fortP, int a3),
				createCadSiegeFortFunc)(cadClass, &trackedPointer, 0);
		}
		
		if (cadClass == 0) return;
		
		GAME_FUNC(void(__thiscall*)(TrackedPointerFort* trackedPointerP),
		   somethingWithTrackedPointerFortFunc)(&trackedPointer);
		
		DWORD adrFunc = codes::offsets.finalyzeActionStratmapFunc;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
	}

	void moveNormal(general* gen, int x, int y)
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

	void diplomacyCharacter(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::diplomacy);
	}

	void assassinate(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::assassinate);
	}

	void marry(general* gen, general* targetCharacter)
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
		targetCharacterAction(gen, targetCharacter, characterAction::marry);
	}

	void spyCharacter(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::spy);
	}

	void switchCharacterFaction(general* gen, factionStruct* fac, bool keepArmy, bool keepBg)
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
		if (gen->armyLeaded)
			stackStructHelpers::sortStack(gen->armyLeaded, 6, 7, 4);
	}

	void denounce(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::denounce);
	}

	void bribe(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::bribe);
	}

	void acquire(general* gen, general* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::acquire);
	}

	void targetCharacterAction(general* gen, general* targetCharacter, characterAction type)
	{
		if (!gen || !targetCharacter)
			return;
		stackStruct* army = nullptr;
		if (gen->armyLeaded)
			army = gen->armyLeaded;
		gen->armyLeaded = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(general**, general**, characterAction, int),
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

	void diplomacyFort(general* gen, fortStruct* targetFort)
	{
		if (!gen || !targetFort)
			return;
		targetFortAction(gen, targetFort, characterAction::diplomacy);
	}

	void bribeFort(general* gen, fortStruct* targetFort)
	{
		if (!gen || !targetFort)
			return;
		targetFortAction(gen, targetFort, characterAction::bribe);
	}

	void spyFort(general* gen, fortStruct* targetFort)
	{
		if (!gen || !targetFort)
			return;
		targetFortAction(gen, targetFort, characterAction::spy);
	}

	void bribeSettlement(general* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::bribe);
	}

	void spySettlement(general* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::spy);
	}

	void sabotageSettlement(general* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::sabotage);
	}

	void targetFortAction(general* gen, fortStruct* targetFort, characterAction type)
	{
		if (!gen || !targetFort)
			return;
		stackStruct* army = nullptr;
		if (gen->armyLeaded)
			army = gen->armyLeaded;
		if (type != characterAction::besiege && type != characterAction::assault && type != characterAction::captureResidence)
			gen->armyLeaded = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(general**, fortStruct**, characterAction, int),
		createCADTargetFort)(&gen, &targetFort, type, 0);
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

	void targetSettlementAction(general* gen, settlementStruct* targetSettlement, characterAction type)
	{
		if (!gen || !targetSettlement)
			return;
		stackStruct* army = nullptr;
		if (gen->armyLeaded)
			army = gen->armyLeaded;
		if (type != characterAction::besiege && type != characterAction::assault && type != characterAction::captureResidence)
			gen->armyLeaded = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(general**, settlementStruct**, characterAction, int),
		createCADTargetSettlement)(&gen, &targetSettlement, type, 0);
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
	
	bool blockadePort(stackStruct* fleet, portBuildingStruct* port)
	{
		if (!fleet || !port )
			return false;
		int success = 0;
		DWORD funcAddr = codes::offsets.assaultObject;
		_asm
		{
			push 0
			push port
			mov ecx, fleet
			mov eax, funcAddr
			call eax
			mov success, eax
		}
		return success != 0;
	}
	
	void buildWatchTower(stackStruct* army)
	{
		if (!army->gen || !army->gen->genChar || !army->gen->genChar->faction)
			return;
		int cultureID = army->gen->genChar->faction->cultureID;
		culturesDB* cultures = reinterpret_cast<culturesDB*>(dataOffsets::offsets.cultureDatabase);
		int cost = cultures->cultures[cultureID].watchTowerCost;
		if (army->gen->genChar->faction->money < cost)
			return;
		DWORD funcAddr = codes::offsets.buildWatchtower;
		_asm
		{
			mov ecx, army
			mov eax, funcAddr
			call eax
		}
	}
	
	void sendOffMap(general* gen)
	{
		if (!gen || !gen->genChar)
			return;
		if (!gen->genChar->label || gen->genChar->label == "")
		{
			unitActions::logStringGame("character.sendOffMap: character has no label, you wont be able to get him back, command cancelled.");
			return;
		}
		if (gen->ifMarkedToKill == 1)
			return;
		if ((gen->genChar->status & 8) != 0)
			return;
		unit* newUnit = nullptr;
		if (gen->armyLeaded && gen->armyLeaded->numOfUnits == 1)
		{
			const auto id = gen->bodyguards->eduEntry->Index;
			newUnit = fastFuncts::createUnitIdx(id, gen->regionID, gen->armyLeaded->faction->dipNum, 0, 0, 0);
			fastFuncts::addUnitToArmy(gen->armyLeaded, newUnit);
		}
		DWORD funcAddr = codes::offsets.sendCharacterOffMap;
		auto faction = gen->genChar->faction;
		_asm
		{
			push 1
			push 0
			push 1
			push gen
			mov ecx, faction
			mov eax, funcAddr
			call eax
		}
		if (newUnit)
			fastFuncts::killUnit(newUnit);
	}
	

	void diplomacySettlement(general* gen, settlementStruct* targetSettlement)
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

	void attackCharacter(general* attacker, general* defender)
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
		
		DWORD adrFunc = codes::offsets.finalyzeActionStratmapFunc;
		_asm
		{
			push cadClass
			mov ecx, attacker
			mov eax, adrFunc
			call eax
		}
		//Destructor -- game doesnt call it, but I think it might be a bug
		CallVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); 
	}


}
