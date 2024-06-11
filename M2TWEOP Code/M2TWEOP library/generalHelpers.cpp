#include "generalHelpers.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "unitActions.h"

namespace generalHelpers
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

	void sabotageBuilding(general* gen, building* build)
	{
		if (!gen || !gen->genChar || !build)
			return;
		auto generalPtr = &gen;
		auto buildingPtr = &build;
		DWORD funcAddr = 0xAAC2B0;
		if (smallFuncs::getGameVersion() == 1)
			funcAddr = 0xAAB280;
		_asm
		{
			push 0
			push buildingPtr
			mov ecx, generalPtr
			mov eax, funcAddr
			call eax
		}
		DWORD globalCadClass = 0x0162C740;
		if (smallFuncs::getGameVersion() == 1)
			globalCadClass = 0x1674570;
		DWORD cadClass2 = *reinterpret_cast<DWORD*>(globalCadClass);
		DWORD finalize = 0x0059ec70;
		if (smallFuncs::getGameVersion() == 1)
			finalize = 0x0059e790;
		_asm
		{
			push cadClass2
			mov ecx, gen
			mov eax, finalize
			call eax
		}
	}

	void setMovepoints(general* gen, float movepoints)
	{
		gen->movePointsCharacter = movepoints;
		gen->movePointsArmy = movepoints;

		if (gen->bodyguards == nullptr)return;

		gen->bodyguards->movePoints = movepoints;
	}

	float getMovepoints(const general* gen)
	{
		return gen->movePointsArmy;
	}

	const char* getTypeName(const general* gen)
	{
		const auto charType = characterTypes.find(gen->genType->type);
		if (charType != characterTypes.end())
		{
			return charType->second;
		}
		return "unknown";
	}

	int getTypeID(const general* gen)
	{
		return gen->genType->type;
	}

}
