#include "actionsStrat.h"


#define GAME_FUNC(funcType, funcAddr) reinterpret_cast<funcType>(codes::offsets.funcAddr)

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
			GAME_FUNC(void(__cdecl*)(stackStruct* armyP, DWORD* prefP, settlementStruct* settlementP),
				getEquipmentPreferencesFunc)(gen->armyLeaded, &pref[0], sett);

			//Loop through the array and decide what equipment to use
			for (int i = 0; pref[i] != 3 && i < 5; ++i)
				GAME_FUNC(char(__thiscall*)(siegeS* siegeP, DWORD* currPrefP),
					decideEquipmentFunc)(gen->armyLeaded->siege, &pref[i]);
			
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
			GAME_FUNC(void(__cdecl*)(stackStruct* armyP, DWORD* prefP, fortStruct* settlementP),
				getEquipmentPreferencesFunc)(gen->armyLeaded, &pref[0], fort);

			//Loop through the array and decide what equipment to use
			for (int i = 0; pref[i] != 3 && i < 5; ++i)
				GAME_FUNC(char(__thiscall*)(siegeS* siegeP, DWORD* currPrefP),
					decideEquipmentFunc)(gen->armyLeaded->siege, &pref[i]);
			
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

		finalizeAction(cadClass, gen);
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