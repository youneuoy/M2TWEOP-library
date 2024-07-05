///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "character.h"

#include "dataOffsets.h"
#include "cultures.h"
#include "gameStringHelpers.h"
#include "functionsOffsets.h"
#include "fort.h"
#include "characterRecord.h"
#include "eopdu.h"
#include "gameHelpers.h"
#include "stratModelsChange.h"
#include "faction.h"
#include "unit.h"
#include "army.h"
#include "campaignDb.h"
#include "strategyMap.h"
#include "techFuncs.h"

characterMovementExtents* character::getMoveExtents(const int searchType, const int numTurns)
{
	const auto pathfinding = campaignHelpers::getStratPathFinding();
	if (!pathfinding)
		return nullptr;
	return pathfinding->extentsManager.createMoveExtents(this, searchType, numTurns);
}

std::shared_ptr<characterMoveData> character::createMoveData(const int searchType, const int numTurns)
{
	return std::make_shared<characterMoveData>(this, searchType, numTurns);
}


int character::getTypeID()
{
	return genType->type;
}

bool character::hasFreeTilesToMove()
{
	return GAME_FUNC(bool(__thiscall*)(character*), hasFreeTilesToMove)(this);
}

void character::setTypeID(int charType)
{
	const int subFac = characterRecord->originalFaction;
	const auto fac = *faction;
	if (!fac)
		return;
	const int factionDipNum = fac->factionID;
	characterHelpers::setCharacterType(this, charType, subFac, factionDipNum);
}

namespace characterHelpers
{
	std::unordered_map<int, const char*> characterTypes = {
		{0,"spy"},
		{1,"assassin"},
		{2,"diplomat"},
		{3,"admiral"},
		{4,"merchant"},
		{5,"priest"},
		{6,"general"},
		{7,"named_character"},
		{8,"princess"},
		{9,"heretic"},
		{10,"witch"},
		{11,"inquisitor"},
		{13,"pope"}
	};

	/*----------------------------------------------------------------------------------------------------------------*\
												Getters and Setters
   \*----------------------------------------------------------------------------------------------------------------*/
	
	const char* getTypeName(const character* gen)
	{
		const auto charType = characterTypes.find(gen->genType->type);
		if (charType != characterTypes.end())
		{
			return charType->second;
		}
		return "unknown";
	}

	void setMovePoints(character* gen, float movePoints)
	{
		gen->movePointsCharacter = movePoints;
		gen->movePointsArmy = movePoints;

		if (gen->bodyguards == nullptr) return;

		gen->bodyguards->movePoints = movePoints;
	}

	float getMovePoints(const character* gen)
	{
		return gen->movePointsArmy;
	}
	
	void setCharacterType(character* character, int typeID, int subFaction, int factionID)
	{
		DWORD adrFunc = codes::offsets.setCharacterType;
		genMod* retVal = nullptr;
		__asm
		{
			push factionID
			push subFaction
			push typeID
			mov eax, adrFunc
			call eax
			mov retVal, eax
		}
		character->genType = retVal;
	}
	
	character* createCharacterWithoutSpawning(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y)
	{
		DWORD adrFunc = codes::offsets.createCharacterFunc;

		character* gen = nullptr;

		char** cryptS = gameStringHelpers::createHashedString(type);

		DWORD adrType = reinterpret_cast<DWORD>(cryptS);
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


		adrFunc = codes::offsets.spawnCreatedObject;
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
	
	character* createCharacter(const char* type, factionStruct* fac, int age, const char* name, const char* name2, int subFaction, const char* portrait, int x, int y)
	{
		if (portrait != nullptr && strlen(portrait) == 0)
			portrait = nullptr;
		if (name != nullptr && strlen(name) == 0)
			name = nullptr;
		if (name2 != nullptr && strlen(name2) == 0)
			name2 = nullptr;
		DWORD adrFunc = codes::offsets.createCharacterFunc;

		character* gen = nullptr;

		char** cryptS = gameStringHelpers::createHashedString(type);

		DWORD adrType = reinterpret_cast<DWORD>(cryptS);
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


		adrFunc = codes::offsets.spawnCreatedObject;
		xyS* xyP = &xy;

		_asm
		{
			push xyP
			push gen
			mov eax, adrFunc
			call eax
		}

		adrFunc = codes::offsets.doSomeWithCharacterFunc;
		void* some = fac->tilesFac;
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


	/*----------------------------------------------------------------------------------------------------------------*\
												Character actions
   \*----------------------------------------------------------------------------------------------------------------*/
	
	void siegeSettlement(character* gen, settlementStruct* sett, bool isAttack)
	{
		//if general or namedchar or sett is nil or general isnt alive return
		if (!gen || !gen->characterRecord || !sett || !gen->characterRecord->isAlive || gen->characterRecord->faction->factionID == sett->faction->factionID)
			return;

		//if maintain option is on and general is leading an army and the army is sieging the settlement then do maintain stuff
		if (!isAttack && gen->army && gen->army->siege && gen->army->siege->getSiegedSettlement() == sett)
		{
			//Some array where it will put what kind of siege equipment it wants
			DWORD pref[5] {};
			
			//Game function to fill that array
			if (!GAME_FUNC(bool(__cdecl*)(armyStruct* armyP, DWORD* prefP, settlementStruct* settlementP),
				getEquipmentPreferencesFunc)(gen->army, &pref[0], sett))
			{
				//Loop through the array and decide what equipment to use
				for (int i = 0; pref[i] != 3 && i < 5; i++)
				{
					GAME_FUNC(char(__thiscall*)(siegeS* siegeP, DWORD currPrefP),
						decideEquipmentFunc)(gen->army->siege, pref[i]);
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
		auto cadClass = techFuncs::allocateGameMem(isAttack && sett->army ? 0x3028 : 0x3024);
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
		
		GAME_FUNC(DWORD(__thiscall*)(character* character, DWORD cadClassP, int a3),
			getReadySiegeCharacterGarrisonedFunc)(gen, cadClass, 0);
		
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		callVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
		stratMapHelpers::clearSundries();
	}
	
	void siegeFort(character* gen, fortStruct* fort, bool isAttack)
	{
		
		//if general or namedchar or sett is nil or general isnt alive return
		if (!gen || !gen->characterRecord || !fort || (gen->characterRecord->age & 1) == 0)
			return;

		//if maintain option is on and general is leading an army and the army is sieging the settlement then do maintain stuff
		if (!isAttack && gen->army && gen->army->siege && gen->army->siege->getSiegedFort() == fort)
		{
			//Some array where it will put what kind of siege equipment it wants
			DWORD pref[5] {};
			
			//Game function to fill that array
			if (!GAME_FUNC(bool(__cdecl*)(armyStruct*, DWORD*, fortStruct*),
				getEquipmentPreferencesFunc)(gen->army, &pref[0], fort))
			{
				//Loop through the array and decide what equipment to use
				for (int i = 0; pref[i] != 3 && i < 5; i++)
				{
					GAME_FUNC(char(__thiscall*)(siegeS* siegeP, DWORD currPrefP),
						decideEquipmentFunc)(gen->army->siege, pref[i]);
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
		auto cadClass = techFuncs::allocateGameMem(isAttack && fort->army ? 0x302C : 0x3028);
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
		
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		stratMapHelpers::clearSundries();
	}
	
	void attackCharacter(character* attacker, character* defender)
	{
		if (!attacker || !defender)
			return;
		
		DWORD cadClass = techFuncs::allocateGameMem(0x302C);
		if (cadClass == 0) return;
		
		
		struct TrackedPointerCharacter { void* vtable; character* character;};
		TrackedPointerCharacter trackedPointer{};
		trackedPointer = *GAME_FUNC(TrackedPointerCharacter*(__thiscall*)(TrackedPointerCharacter* trackedChar, character* character),
		makeTrackedPointerToAttackFunc)(&trackedPointer, defender);
		
		cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2, TrackedPointerCharacter* trackedPointerP, int a4),
			makeCadAttackFunc)(cadClass, 0, &trackedPointer, 0);
		
		GAME_FUNC(void(__thiscall*)(TrackedPointerCharacter* trackedPointerP),
		   somethingWithTrackedPointerAttackFunc)(&trackedPointer);
		
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, attacker
			mov eax, adrFunc
			call eax
		}
		//Destructor -- game doesnt call it, but I think it might be a bug
		callVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); 
		stratMapHelpers::clearSundries();
	}

	void moveNormal(character* gen, int x, int y)
	{
		if (!gen)
			return;
		if (!gen->hasFreeTilesToMove())
			return;
		DWORD cadClass = techFuncs::allocateGameMem(0x301C);
		if (cadClass == 0) return;
		cadClass = GAME_FUNC(DWORD(__thiscall*)(DWORD cadClassP, int a2),
			createCadMovingNormalFunc)(cadClass, 0);
		if (cadClass == 0) return;
		
		if (!GAME_FUNC(bool(__thiscall*)(character* character, DWORD cadClassP, int xCoord, int yCoord, int a5, int a6),
			getReadyForMoving2Func)(gen, cadClass, x, y, 0, 0))
		{
			callVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
			return;
		}
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		callVFunc<0, void>(reinterpret_cast<void*>(cadClass), 1); //Destructor
		stratMapHelpers::clearSundries();
	}

	///Target character

	void targetCharacterAction(character* gen, character* targetCharacter, characterAction type)
	{
		if (!gen || !targetCharacter)
			return;
		armyStruct* army = nullptr;
		if (gen->army)
			army = gen->army;
		gen->army = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(character**, character**, characterAction, int),
		createCADTargetCharacter)(&gen, &targetCharacter, type, 0);
		gen->army = army;
		DWORD cadClass = dataOffsets::offsets.globalCadClass;
		cadClass = *reinterpret_cast<DWORD*>(cadClass);
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		stratMapHelpers::clearSundries();
	}

	void denounce(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::denounce);
	}

	void bribe(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::bribe);
	}

	void acquire(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::acquire);
	}
	
	void diplomacyCharacter(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::diplomacy);
	}

	void assassinate(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::assassinate);
	}

	void marry(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter || !gen->genType || !targetCharacter->genType)
			return;
		if (gen->genType->type != characterTypeStrat::princess)
		{
			gameHelpers::logFuncError("character.marry", "character is not princess");
			return;
		}
		if (targetCharacter->genType->type != characterTypeStrat::namedCharacter)
		{
			gameHelpers::logFuncError("character.marry", "target character is not family member");
			return;
		}
		targetCharacterAction(gen, targetCharacter, characterAction::marry);
	}

	void spyCharacter(character* gen, character* targetCharacter)
	{
		if (!gen || !targetCharacter)
			return;
		targetCharacterAction(gen, targetCharacter, characterAction::spy);
	}

	///Target settlement
	void targetSettlementAction(character* gen, settlementStruct* targetSettlement, characterAction type)
	{
		if (!gen || !targetSettlement)
			return;
		armyStruct* army = nullptr;
		if (gen->army)
			army = gen->army;
		if (type != characterAction::besiege && type != characterAction::assault && type != characterAction::captureResidence)
			gen->army = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(character**, settlementStruct**, characterAction, int),
		createCADTargetSettlement)(&gen, &targetSettlement, type, 0);
		gen->army = army;
		DWORD cadClass = dataOffsets::offsets.globalCadClass;
		cadClass = *reinterpret_cast<DWORD*>(cadClass);
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		stratMapHelpers::clearSundries();
	}

	void bribeSettlement(character* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::bribe);
	}

	void diplomacySettlement(character* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::diplomacy);
	}

	void spySettlement(character* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::spy);
	}

	void sabotageSettlement(character* gen, settlementStruct* targetSettlement)
	{
		if (!gen || !targetSettlement)
			return;
		targetSettlementAction(gen, targetSettlement, characterAction::sabotage);
	}
	
	void sabotageBuilding(character* gen, building* build)
	{
		if (!gen || !gen->characterRecord || !build)
			return;
		auto generalPtr = &gen;
		auto buildingPtr = &build;
		DWORD funcAddr = codes::offsets.sabotageBuilding;
		_asm
		{
			push 0
			push buildingPtr
			mov ecx, generalPtr
			mov eax, funcAddr
			call eax
		}
		DWORD cadClass2 = *reinterpret_cast<DWORD*>(dataOffsets::offsets.globalCadClass);
		DWORD finalize = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass2
			mov ecx, gen
			mov eax, finalize
			call eax
		}
		stratMapHelpers::clearSundries();
	}

	//Target fort
	void targetFortAction(character* gen, fortStruct* targetFort, characterAction type)
	{
		if (!gen || !targetFort)
			return;
		armyStruct* army = nullptr;
		if (gen->army)
			army = gen->army;
		if (type != characterAction::besiege && type != characterAction::assault && type != characterAction::captureResidence)
			gen->army = nullptr;
		GAME_FUNC(DWORD(__thiscall*)(character**, fortStruct**, characterAction, int),
		createCADTargetFort)(&gen, &targetFort, type, 0);
		gen->army = army;
		DWORD cadClass = dataOffsets::offsets.globalCadClass;
		cadClass = *reinterpret_cast<DWORD*>(cadClass);
		DWORD adrFunc = codes::offsets.finalizeActionStrat;
		_asm
		{
			push cadClass
			mov ecx, gen
			mov eax, adrFunc
			call eax
		}
		stratMapHelpers::clearSundries();
	}

	void diplomacyFort(character* gen, fortStruct* targetFort)
	{
		if (!gen || !targetFort)
			return;
		targetFortAction(gen, targetFort, characterAction::diplomacy);
	}

	void bribeFort(character* gen, fortStruct* targetFort)
	{
		if (!gen || !targetFort)
			return;
		targetFortAction(gen, targetFort, characterAction::bribe);
	}

	void spyFort(character* gen, fortStruct* targetFort)
	{
		if (!gen || !targetFort)
			return;
		targetFortAction(gen, targetFort, characterAction::spy);
	}

	/*----------------------------------------------------------------------------------------------------------------*\
												Other helpers
   \*----------------------------------------------------------------------------------------------------------------*/
	
	void sendOffMap(character* gen)
	{
		if (!gen || !gen->characterRecord)
			return;
		if (!gen->characterRecord->label || gen->characterRecord->labelCrypt == 0 || strcmp(gen->characterRecord->label, "") == 0)
		{
			gameHelpers::logStringGame("character.sendOffMap: character has no label, you wont be able to get him back, command cancelled.");
			return;
		}
		if (gen->ifMarkedToKill == 1)
			return;
		if ((gen->characterRecord->status & 8) != 0)
			return;
		unit* newUnit = nullptr;
		if (gen->army && gen->army->numOfUnits == 1)
		{
			const auto id = gen->bodyguards->eduEntry->index;
			newUnit = unitHelpers::createUnitIdx(id, gen->regionID, gen->army->faction->factionID, 0, 0, 0);
			armyHelpers::addUnitToArmy(gen->army, newUnit);
		}
		DWORD funcAddr = codes::offsets.sendCharacterOffMap;
		auto faction = gen->characterRecord->faction;
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
			unitHelpers::killUnit(newUnit);
	}
	
	void switchCharacterFaction(character* gen, factionStruct* fac, bool keepArmy, bool keepBg)
	{
		std::string functionName = "character.switchFaction";
		if (!gen || !fac || !gen->genType)
		{
			gameHelpers::logFuncError(functionName, "gen or fac or genType is nil");
			return;
		}
		if (gen->genType->type == characterTypeStrat::admiral)
		{
			gameHelpers::logFuncError(functionName, "unsupported character type admiral");
			return;
		}
		if (gen->genType->type == characterTypeStrat::princess)
		{
			gameHelpers::logFuncError(functionName, "unsupported character type princess");
			return;
		}
		if (gen->visitingArmy)
		{
			gameHelpers::logFuncError(functionName, "character is part of an army");
			return;
		}
		if (gen->army && (gen->army->shipArmy || gen->army->charactersNum != 0))
		{
			gameHelpers::logFuncError(functionName, "character is boarded or has auxiliary characters");
			return;
		}
		if (gen->genType->type == characterTypeStrat::namedCharacter && !gen->army)
		{
			gameHelpers::logFuncError(functionName, "character is in a residence or off-map");
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
				armour = unit->avgArmourUpg;
				weapon = unit->avgWeaponUpg;
				soldierCount = unit->SoldierCountStrat;
				alias = unit->alias;
				supplies = unit->foodRequirement;
				eduType = unit->eduEntry->index;
			}
			[[nodiscard]] unit* CreateUnit(const factionStruct* faction, const character* character) const
			{
				unit* newUnit = unitHelpers::createUnitIdx(eduType, character->regionID, faction->factionID, xp, armour, weapon);
				newUnit->alias = alias;
				newUnit->foodRequirement = supplies;
				newUnit->SoldierCountStrat = soldierCount;
				return newUnit;
			}
		};
		
		unitInfo bgInfo{};
		std::vector<unitInfo> unitsInfo;
		if ((gen->genType->type == characterTypeStrat::namedCharacter
			|| gen->genType->type == characterTypeStrat::general) && gen->army)
		{
			if (keepBg && gen->bodyguards)
				bgInfo = unitInfo(gen->bodyguards);
			if (keepArmy)
			{
				for (int i = 0; i < gen->army->numOfUnits; ++i)
				{
					auto un = gen->army->units[i];
					if (un->generalInfo || un == gen->bodyguards)
						continue;
					if ((un->eduEntry->ownership & (1 << fac->factionID)) == 0)
						unitsInfo.emplace_back(un);
				}
			}
		}

		GAME_FUNC(void(__thiscall*)(character*), changeCharacterTileStuff)(gen);

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
			|| gen->genType->type == characterTypeStrat::general) && gen->army)
		{
			if (keepBg && gen->genType->type == characterTypeStrat::namedCharacter)
			{
				auto newUnit = bgInfo.CreateUnit(fac, gen);
				if (gen->army && gen->army->numOfUnits >= 20)
				{
					auto bodyguard = gen->bodyguards;
					auto randomUnit = gen->army->units[17];
					setBodyguard(gen, randomUnit);
					unitHelpers::killUnit(bodyguard);
					armyHelpers::addUnitToArmy(gen->army, newUnit);
					setBodyguard(gen, newUnit);
				}
				else
				{
					auto bodyguard = gen->bodyguards;
					setBodyguard(gen, newUnit);
					armyHelpers::addUnitToArmy(gen->army, newUnit);
					unitHelpers::killUnit(bodyguard);
				}
			}
			if (keepArmy)
			{
				for (auto& un : unitsInfo)
				{
					auto newUnit = un.CreateUnit(fac, gen);
					armyHelpers::addUnitToArmy(gen->army, newUnit);
				}
			}
		}
		GAME_FUNC(void(__thiscall*)(character*), initPlaceCharacter)(gen);
		if (gen->army)
			gen->army->sortStack(6, 7, 4);
	}
	
	void teleportCharacter(character* gen, int x, int y)
	{
		if (gen->residence)
		{
			gameHelpers::logStringGame("character.teleport: Character is in residence, cancelled to avoid bugs");
			return;
		}
		
		if (gen->army != nullptr)
		{
			armyHelpers::stopSiege(gen->army);
			armyHelpers::stopBlockPort(gen->army);
		}

		auto charArray = new characterArray();
		auto targetCoords = new int[2]{x,y};
		GAME_FUNC(void(__stdcall*)(int*, characterArray*, int), getTileCharactersFunc)(targetCoords, charArray, 12);
		if ((!charArray->charactersNum && stratMapHelpers::isTileValidForCharacterType(gen->genType->type, x, y)) ||
			GAME_FUNC(bool(__thiscall*)(regionStruct*, int*, char), getValidRegionTile)(stratMapHelpers::getRegion(stratMapHelpers::getTile(x, y)->regionId), &targetCoords[0], 0))
		{
			
			GAME_FUNC(void(__thiscall*)(character*), changeCharacterTileStuff)(gen);
			DWORD adrFunc = codes::offsets.teleportCharacterFunc;
			int xCoord = targetCoords[0];
			int yCoord = targetCoords[1];
			_asm
			{
				push yCoord
				push xCoord

				mov ecx, gen
				mov eax, adrFunc
				call eax
			}
			DWORD moveExtentThing = reinterpret_cast<DWORD>(gameHelpers::getGameDataAll()->stratPathFinding) + 0x88;
		    GAME_FUNC(void(__thiscall*)(DWORD), deleteMoveExtents)(moveExtentThing);
			auto selectInfoPtr = gameHelpers::getGameDataAll()->selectionInfoPtr2;
		    GAME_FUNC(void(__thiscall*)(selectionInfo**, character*), someSelectionStuff)(selectInfoPtr, gen);
		    GAME_FUNC(void(__thiscall*)(character*), initPlaceCharacter)(gen);
		}
		delete[] targetCoords;
		delete charArray;
	}

	bool teleportCharacterClose(character* gen, int x, int y)
	{
		if (gen->residence)
		{
			gameHelpers::logStringGame("character.teleport: Character is in residence, cancelled to avoid bugs");
			return false;
		}
		bool isTeleported = false;
		if (gen->army != nullptr)
		{
			armyHelpers::stopSiege(gen->army);
			armyHelpers::stopBlockPort(gen->army);
		}

		if (const auto [newX, newY] = stratMapHelpers::findValidTileNearTile(x, y, gen->genType->type);
			stratMapHelpers::isTileValidForCharacterType(gen->genType->type, newX, newY))
		{
			GAME_FUNC(void(__thiscall*)(character*), changeCharacterTileStuff)(gen);
			DWORD adrFunc = codes::offsets.teleportCharacterFunc;
			int xCoord = newX;
			int yCoord = newY;
			_asm
			{
				push yCoord
				push xCoord

				mov ecx, gen
				mov eax, adrFunc
				call eax
			}
			DWORD moveExtentThing = reinterpret_cast<DWORD>(gameHelpers::getGameDataAll()->stratPathFinding) + 0x88;
		    GAME_FUNC(void(__thiscall*)(DWORD), deleteMoveExtents)(moveExtentThing);
			auto selectInfoPtr = gameHelpers::getGameDataAll()->selectionInfoPtr2;
		    GAME_FUNC(void(__thiscall*)(selectionInfo**, character*), someSelectionStuff)(selectInfoPtr, gen);
		    GAME_FUNC(void(__thiscall*)(character*), initPlaceCharacter)(gen);
			isTeleported = true;
		}
		return isTeleported;
	}

	
	fortStruct* createFort(character* gen)
	{
		if (!gen || !gen->characterRecord || !gen->characterRecord->faction || !gen->army)
			return nullptr;
		if (auto faction = gen->characterRecord->faction;
			faction->money < cultures::getCultureDb()->cultures[faction->cultureID].fortCost)
			return nullptr;
		armyStruct* newArmy = gen->army;
		auto oldOption = campaignHelpers::getCampaignDb()->campaignDbSettlement.canBuildForts;
		campaignHelpers::getCampaignDb()->campaignDbSettlement.canBuildForts = true;
		DWORD adrFunc = codes::offsets.createFortFunc;
		_asm
		{
			mov ecx, newArmy
			mov eax, adrFunc
			call eax
		}
		campaignHelpers::getCampaignDb()->campaignDbSettlement.canBuildForts = oldOption;
		return stratMapHelpers::getTile(gen->xCoord, gen->yCoord)->getFort();
	}
	
	void killCharacter(character* gen)
	{
		DWORD adr = codes::offsets.killCharStratMapFunc;

		_asm {
			push 0x1
			mov ecx, [gen]
			mov eax, [adr]
			call eax
		}
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
	
	//general
	template <char fieldIndex>
	std::string getStringPropertyGen(const character* gen)
	{
		char* retS = nullptr;
		
		if (fieldIndex == generalStruct_abilityID)
			retS = gen->ability;

		if (retS != nullptr)
			return std::string(retS);
		
		return std::string("");
	}

	template <char fieldIndex>
	void setStringPropertyGen(character* gen, const std::string& newS)
	{
		if (fieldIndex == generalStruct_abilityID)
			gameStringHelpers::setHashedString(&gen->ability, newS.c_str());
	}
	
    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::usertype<character>character;
        }types;
    	
	///Character
	//@section Character

	/***
	characters as they exist on the strategy map - dead characters, wives, children, and off-map characters do not have these fields.

	@tfield int characterType use characterType enum
	@tfield int xCoord
	@tfield int yCoord
	@tfield characterRecord characterRecord
	@tfield factionStruct faction
	@tfield int isMarkedToKill can check if the character died before the game updates he is dead like post battle event
	@tfield bool inEnemyZOC
	@tfield bool inEnemyTerritory
	@tfield int sedentaryTurns
	@tfield bool isPlagued
	@tfield bool hasEopOrders
	@tfield int plaguedDuration
	@tfield unit bodyguards
	@tfield armyStruct army
	@tfield armyStruct visitingArmy in the army but not leading it
	@tfield int actionType
	@tfield bool unusedMovePoints
	@tfield bool infiniteMovePoints
	@tfield int regionID
	@tfield float movePointsCharacter
	@tfield float movePointsModifier
	@tfield float movePointsMaxCharacter
	@tfield float movePointsMaxArmy
	@tfield float movePointsArmy
	@tfield float movePoints sets both army and character move points.
	@tfield int turnJoinedCrusade
	@tfield int numTurnsIdle
	@tfield float percentCharacterReligionInRegion 0 to 1
	@tfield float popConvertedThisTurn
	@tfield int timeInRegion
	@tfield int timeWithArmy for auxiliary generals, not leading general
	@tfield settlementStruct settlement
	@tfield fortStruct fort
	@tfield portStruct dockedPort
	@tfield portStruct blockedPort
	@tfield crusadeStruct crusade
	@tfield jihadStruct jihad
	@tfield int currentTurn
	@tfield float distanceToCrusadeTarget
	@tfield int disbandProgress
	@tfield int isCrusadeDisbandActive
	@tfield string ability see descr\_hero\_abilities.xml
	@tfield getTypeID getTypeID
	@tfield getTypeName getTypeName
	@tfield setTypeID setTypeID
	@tfield moveToTile moveToTile
	@tfield reposition reposition
	@tfield teleport teleport
	@tfield diplomacyCharacter diplomacyCharacter
	@tfield diplomacySettlement diplomacySettlement
	@tfield kill kill
	@tfield createFort createFort
	@tfield assassinate assassinate
	@tfield marry marry
	@tfield spyCharacter spyCharacter
	@tfield denounce denounce
	@tfield bribe bribe
	@tfield acquire acquire
	@tfield setBodyguardUnit setBodyguardUnit
	@tfield setCharacterModel setCharacterModel
	@tfield switchFaction switchFaction
	@tfield sendOffMap sendOffMap
	@tfield sabotage sabotage
	@tfield diplomacyFort diplomacyFort
	@tfield bribeFort bribeFort
	@tfield spyFort spyFort
	@tfield bribeSettlement bribeSettlement
	@tfield spySettlement spySettlement
	@tfield sabotageSettlement sabotageSettlement
	@tfield getMovementExtents getMovementExtents
	@tfield createMoveData createMoveData
	@tfield joinCrusade joinCrusade
	@tfield leaveCrusade leaveCrusade
	@tfield joinJihad joinJihad
	@tfield leaveJihad leaveJihad
	@tfield hasFreeTilesToMove hasFreeTilesToMove
	@tfield isGeneral isGeneral

	@table character
	*/
	types.character = luaState.new_usertype<character>("character");
	types.character.set("characterType", sol::property(&character::getTypeID, &character::setTypeID));
	types.character.set("xCoord", &character::xCoord);
	types.character.set("yCoord", &character::yCoord);
	types.character.set("namedCharacter", &character::characterRecord);
	types.character.set("characterRecord", &character::characterRecord);
	types.character.set("faction", sol::property(&character::getFaction));
	types.character.set("isMarkedToKill", &character::ifMarkedToKill);
	types.character.set("inEnemyZOC", &character::inEnemyZOC);
	types.character.set("inEnemyTerritory", &character::inEnemyTerritory);
	types.character.set("sedentaryTurns", &character::sedentaryTurns);
	types.character.set("isPlagued", &character::isPlagued);
	types.character.set("plaguedDuration", &character::plaguedDuration);
	types.character.set("ambushState", &character::actionType);
	types.character.set("actionType", &character::actionType);
	types.character.set("unusedMovePoints", &character::unusedMovePoints);
    types.character.set("infiniteMovePoints", &character::infiniteMovePoints);
	types.character.set("movePointsCharacter", &character::movePointsCharacter);
	types.character.set("numTurnsIdle", &character::numTurnsIdle);
	types.character.set("regionID", &character::regionID);
	types.character.set("hasEopOrders", &character::hasEopOrders);
	types.character.set("percentCharacterReligionInRegion", &character::percentCharacterReligionInRegion);
    types.character.set("popConvertedThisTurn", &character::popConvertedThisTurn);
    types.character.set("timeInRegion", &character::timeInRegion);
    types.character.set("timeWithArmy", &character::timeWithArmy);
    types.character.set("settlement", sol::property(&character::getSettlement));
    types.character.set("fort", sol::property(&character::getFort));
    types.character.set("dockedPort", &character::dockedPort);
    types.character.set("blockedPort", &character::blockadedPort);
    types.character.set("army", &character::army);
    types.character.set("jihad", sol::property(&character::getJihad));
    types.character.set("armyLeaded", &character::army);
    types.character.set("bodyguards", &character::bodyguards);
    types.character.set("armyNotLeaded", &character::visitingArmy);
    types.character.set("visitingArmy", &character::visitingArmy);
    types.character.set("movePointsModifier", &character::movePointsModifier);
    types.character.set("movePointsMaxArmy", &character::movePointsMaxArmy);
    types.character.set("movePointsMaxCharacter", &character::movePointsMax);
    types.character.set("movePointsArmy", &character::movePointsArmy);
    types.character.set("movePoints", sol::property(&getMovePoints, &setMovePoints));
	types.character.set("crusade", &character::currentCrusade);
	types.character.set("turnJoinedCrusade", &character::turnJoinedCrusade);
	types.character.set("currentTurn", &character::currentTurn);
	types.character.set("distanceToCrusadeTarget", &character::distanceToCrusadeTarget);
	types.character.set("crusadeNoProgressTurns", &character::crusadeNoProgressTurns);
	types.character.set("noCrusadeProgressThisTurn", &character::noCrusadeProgressThisTurn);
	types.character.set("ability", sol::property(&getStringPropertyGen<generalStruct_abilityID>, &setStringPropertyGen<generalStruct_abilityID>));
	/***
	Get the character type. Use characterType enum.
	@function character:getTypeID
	@treturn int typeId
	@usage
	local ourType=ourCharacter:getTypeID();
	*/
	types.character.set_function("getTypeID", &character::getTypeID);
	/***
	Get the character type. Use characterType enum.
	@function character:getTypeName
	@treturn string type
	@usage
	local ourType=ourCharacter:getTypeName();
	*/
	types.character.set_function("getTypeName", &getTypeName);
	/***
	Set the character type. Use characterType enum.
	@function character:setTypeID
	@tparam int typeId
	@usage
	ourCharacter:setTypeID(2);
	*/
	types.character.set_function("setTypeID", &character::setTypeID);
	/***
	Issue regular move command, character must have movement points.
	@function character:moveToTile
	@tparam int xCoord
	@tparam int yCoord
	@usage
	ourCharacter:moveToTile(11,25);
	*/
	types.character.set_function("moveToTile", &moveNormal);
	/***
	Issue diplomacy command.
	@function character:diplomacyCharacter
	@tparam character targetChar
	@usage
	ourCharacter:diplomacyCharacter(targetChar);
	*/
	types.character.set_function("diplomacyCharacter", &diplomacyCharacter);
	/***
	Issue assassination command.
	@function character:assassinate
	@tparam character targetChar
	@usage
	ourCharacter:assassinate(targetChar);
	*/
	types.character.set_function("assassinate", &assassinate);
	/***
	Issue marry command.
	@function character:marry
	@tparam character targetChar
	@usage
	ourCharacter:marry(targetChar);
	*/
	types.character.set_function("marry", &marry);
	/***
	Issue spyCharacter command.
	@function character:spyCharacter
	@tparam character targetChar
	@usage
	ourCharacter:spyCharacter(targetChar);
	*/
	types.character.set_function("spyCharacter", &spyCharacter);
	/***
	Issue denounce command.
	@function character:denounce
	@tparam character targetChar
	@usage
	ourCharacter:denounce(targetChar);
	*/
	types.character.set_function("denounce", &denounce);
	/***
	Issue bribe command.
	@function character:bribe
	@tparam character targetChar
	@usage
	ourCharacter:bribe(targetChar);
	*/
	types.character.set_function("bribe", &bribe);
	/***
	Issue acquire command.
	@function character:acquire
	@tparam character targetChar
	@usage
	ourCharacter:acquire(targetChar);
	*/
	types.character.set_function("acquire", &acquire);
	/***
	Issue sabotage command.
	@function character:sabotage
	@tparam building target
	@usage
	ourCharacter:sabotage(targetBuilding);
	*/
	types.character.set_function("sabotage", &sabotageBuilding);
	/***
	Switch character faction.
	@function character:switchFaction
	@tparam factionStruct newFac
	@tparam bool keepArmy
	@tparam bool keepUnit
	@usage
	ourCharacter:switchFaction(otherFac, true, true);
	*/
	types.character.set_function("switchFaction", &switchCharacterFaction);
	/***
	Issue diplomacy command.
	@function character:diplomacySettlement
	@tparam settlementStruct targetSettlement
	@usage
	ourCharacter:diplomacySettlement(targetSettlement);
	*/
	types.character.set_function("diplomacySettlement", &diplomacySettlement);
	/***
	Issue bribe command.
	@function character:bribeSettlement
	@tparam settlementStruct targetSettlement
	@usage
	ourCharacter:bribeSettlement(targetSettlement);
	*/
	types.character.set_function("bribeSettlement", &bribeSettlement);
	/***
	Issue spy command.
	@function character:spySettlement
	@tparam settlementStruct targetSettlement
	@usage
	ourCharacter:spySettlement(targetSettlement);
	*/
	types.character.set_function("spySettlement", &spySettlement);
	/***
	Issue sabotage command.
	@function character:sabotageSettlement
	@tparam settlementStruct targetSettlement
	@usage
	ourCharacter:sabotageSettlement(targetSettlement);
	*/
	types.character.set_function("sabotageSettlement", &sabotageSettlement);
	/***
	Issue diplomacy command.
	@function character:diplomacyFort
	@tparam fortStruct targetFort
	@usage
	ourCharacter:diplomacyFort(targetFort);
	*/
	types.character.set_function("diplomacyFort", &diplomacyFort);
	/***
	Issue bribe command.
	@function character:bribeFort
	@tparam fortStruct targetFort
	@usage
	ourCharacter:bribeFort(targetFort);
	*/
	types.character.set_function("bribeFort", &bribeFort);
	/***
	Issue spy command.
	@function character:spyFort
	@tparam fortStruct targetFort
	@usage
	ourCharacter:spyFort(targetFort);
	*/
	types.character.set_function("spyFort", &spyFort);
	/***
	Instantly teleport character to the coordinates, if occupied a random place in the same region is chosen.
	@function character:reposition
	@tparam int xCoord
	@tparam int yCoord
	@usage
	ourCharacter:reposition(11,25);
	*/
	types.character.set_function("reposition",  &teleportCharacter);
	/***
	Instantly teleport character to the coordinates, if occupied the closest valid place to the coordinates is chosen.
	@function character:teleport
	@tparam int xCoord
	@tparam int yCoord
	@treturn bool hasTeleported
	@usage
	 local success = ourCharacter:teleport(11,25);
	*/
	types.character.set_function("teleport", &teleportCharacterClose);
	/***
	Delete this character
	@function character:kill
	@usage
	ourCharacter:kill();
	*/
	types.character.set_function("kill", &killCharacter);
	/***
	Create a fort at the character's coordinates
	@function character:createFort
	@treturn fortStruct newFort
	@usage
	 local newFort = ourCharacter:createFort()
	*/
	types.character.set_function("createFort", &createFort);
	/***
	Set bodyguard. Do this only for characters without it, such as immediately after creating a character.
	@function character:setBodyguardUnit
	@tparam unit unit
	@usage
	ourCharacter:setBodyguardUnit(unit);
	*/
	types.character.set_function("setBodyguardUnit", &setBodyguard);
	/***
	Set a character's model to a new one.
	@function character:setCharacterModel
	@tparam string model
	@usage
	ourCharacter:setCharacterModel("saruman");
	*/
	types.character.set_function("setCharacterModel", &stratModelsChange::setCharacterModel);
	/***
	Send a character off map (command will not fire if the general does not have a label, as you wouldn't be able to respawn him). It automatically acounts for the bug relating to sending characters off map that are leading a 1 unit army, by adding and killing a unit.
	@function character:sendOffMap
	@usage
	       ourCharacter:sendOffMap();
	*/
	types.character.set_function("sendOffMap", &sendOffMap);
	/***
	Get the movement extents for the character.
	@function character:getMovementExtents
	@tparam int searchType
	@tparam int turns
	@treturn characterMovementExtents extents
	@usage
	       local extents = ourCharacter:getMovementExtents(searchType.avoidZoc, 1)
	*/
	types.character.set_function("getMovementExtents", &character::getMoveExtents);
		
	/***
	Get pathfinding information for this character (try to create and then assign to a variable and use it, not constantly call). But beware the info is out of data quickly if any movement happens on the strategy map.
	@function character:createMoveData
	@tparam int searchType
	@tparam int turns
	@treturn characterMoveData moveData
	@usage
	       local moveData = ourCharacter:createMoveData(searchType.avoidZoc, 1)
	*/
	types.character.set_function("createMoveData", &character::createMoveData);
		
	/***
	Join an ongoing crusade.
	@function character:joinCrusade
	@usage
	       ourCharacter:joinCrusade()
	*/
	types.character.set_function("joinCrusade", &character::joinCrusade);
		
	/***
	Abandon a joined crusade.
	@function character:leaveCrusade
	@tparam bool triggerEvent Fire general abandons crusade event
	@usage
	       ourCharacter:leaveCrusade(true)
	*/
	types.character.set_function("leaveCrusade", &character::leaveCrusade);
		
	/***
	Join an ongoing jihad.
	@function character:joinJihad
	@usage
	       ourCharacter:joinJihad()
	*/
	types.character.set_function("joinJihad", &character::joinJihad);
		
	/***
	Abandon a joined jihad.
	@function character:leaveJihad
	@tparam bool triggerEvent Fire general abandons crusade event
	@usage
	       ourCharacter:leaveJihad(true)
	*/
	types.character.set_function("leaveJihad", &character::leaveJihad);
		
	/***
	Check if character can move out of his position.
	@function character:hasFreeTilesToMove
	@treturn bool canMove
	@usage
	       local canMove = ourCharacter:hasFreeTilesToMove()
	*/
	types.character.set_function("hasFreeTilesToMove", &character::hasFreeTilesToMove);
		
	/***
	Checks if character is either named_character or general.
	@function character:isGeneral
	@treturn bool isGen
	@usage
	       local isGen = ourCharacter:isGeneral()
	*/
	types.character.set_function("isGeneral", &character::isGeneral);
		
    }
}


void character::joinCrusade()
{
	const auto newCrusade = &campaignHelpers::getCampaignData()->crusade;
	GAME_FUNC(void(__thiscall*)(character*, crusade*), generalJoinCrusade)(this, newCrusade);
}

void character::leaveCrusade(bool triggerEvent)
{
	GAME_FUNC(void(__thiscall*)(character*, bool), generalLeaveCrusade)(this, triggerEvent);
}

void character::joinJihad()
{
	const auto newJihad = &campaignHelpers::getCampaignData()->jihad;
	GAME_FUNC(void(__thiscall*)(character*, jihad*), generalJoinCrusade)(this, newJihad);
}

void character::leaveJihad(bool triggerEvent)
{
	GAME_FUNC(void(__thiscall*)(character*, bool), generalLeaveCrusade)(this, triggerEvent);
}

settlementStruct* character::getSettlement()
{
	const auto tile = stratMapHelpers::getTile(xCoord, yCoord);
	if (!tile)
		return nullptr;
	return tile->getSettlement();
}
fortStruct* character::getFort()
{
	const auto tile = stratMapHelpers::getTile(xCoord, yCoord);
	if (!tile)
		return nullptr;
	return tile->getFort();
}