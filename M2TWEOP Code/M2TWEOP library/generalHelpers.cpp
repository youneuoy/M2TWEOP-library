#include "generalHelpers.h"
#include "plugData.h"
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
	void moveToTile(general* gen, int x, int y)
	{
		(*(*plugData::data.funcs.moveNormal))(gen, x, y);
	}

	void sabotageBuilding(general* gen, building* build)
	{
		if (!gen || !gen->genChar || !build)
			return;
		auto generalPtr = &gen;
		auto buildingPtr = &build;
		DWORD funcAddr = 0xAAC2B0;
		if (m2tweopHelpers::getGameVersion() == 1)
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
		if (m2tweopHelpers::getGameVersion() == 1)
			globalCadClass = 0x1674570;
		DWORD cadClass2 = *reinterpret_cast<DWORD*>(globalCadClass);
		DWORD finalize = 0x0059ec70;
		if (m2tweopHelpers::getGameVersion() == 1)
			finalize = 0x0059e790;
		_asm
		{
			push cadClass2
			mov ecx, gen
			mov eax, finalize
			call eax
		}
	}

	void reposition(general* gen, int x, int y)
	{
		(*(*plugData::data.funcs.teleportCharacter))(gen, x, y);
	}

	bool teleport(general* gen, int x, int y)
	{
		return (*(*plugData::data.funcs.teleportCharacterClose))(gen, x, y);
	}

	void diplomacyCharacter(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.diplomacyCharacter))(gen, targetCharacter);
	}

	void assassinate(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.assassinate))(gen, targetCharacter);
	}

	void marry(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.marry))(gen, targetCharacter);
	}

	void spyCharacter(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.spyCharacter))(gen, targetCharacter);
	}

	void denounce(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.denounce))(gen, targetCharacter);
	}

	void bribe(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.bribe))(gen, targetCharacter);
	}

	void acquire(general* gen, general* targetCharacter)
	{
		(*(*plugData::data.funcs.acquire))(gen, targetCharacter);
	}
	
	void switchCharacterFaction(general* gen, factionStruct* fac, bool keepArmy, bool keepBg)
	{
		(*(*plugData::data.funcs.switchCharacterFaction))(gen, fac, keepArmy, keepBg);
		if (gen->armyLeaded)
			stackStructHelpers::sortStack(gen->armyLeaded, 6, 7, 4);
	}

	void diplomacySettlement(general* gen, settlementStruct* targetSettlement)
	{
		(*(*plugData::data.funcs.diplomacySettlement))(gen, targetSettlement);
	}

	void diplomacyFort(general* gen, fortStruct* targetFort)
	{
		(*(*plugData::data.funcs.diplomacyFort))(gen, targetFort);
	}

	void bribeFort(general* gen, fortStruct* targetFort)
	{
		(*(*plugData::data.funcs.bribeFort))(gen, targetFort);
	}

	void spyFort(general* gen, fortStruct* targetFort)
	{
		(*(*plugData::data.funcs.spyFort))(gen, targetFort);
	}

	void bribeSettlement(general* gen, settlementStruct* targetSettlement)
	{
		(*(*plugData::data.funcs.bribeSettlement))(gen, targetSettlement);
	}

	void spySettlement(general* gen, settlementStruct* targetSettlement)
	{
		(*(*plugData::data.funcs.spySettlement))(gen, targetSettlement);
	}

	void sabotageSettlement(general* gen, settlementStruct* targetSettlement)
	{
		(*(*plugData::data.funcs.sabotageSettlement))(gen, targetSettlement);
	}


	void killGeneral(general* gen)
	{
		(*(*plugData::data.funcs.killCharacter))(gen);
	}

	void setBodyguard(general* gen, unit* un)
	{
		(*(*plugData::data.funcs.setBodyguard))(gen, un);
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

	void setTypeID(general* gen, int newType)
	{
		(*(*plugData::data.funcs.setCharacterType))(gen, newType, gen->genChar->subFaction, gen->genChar->faction->dipNum);
	}

	void setCharacterModel(general* gen, const char* model)
	{
		(*(*plugData::data.funcs.setCharacterModel))(gen, model);
	}

	void addCharacterCas(const char* skeletonname, const char* caspath, const char* shadowcaspath, const char* typeName, const char* texturepath, float scale)
	{
		(*(*plugData::data.funcs.addCharacterCas))(skeletonname, caspath, shadowcaspath, typeName, texturepath, scale);
	}

}
