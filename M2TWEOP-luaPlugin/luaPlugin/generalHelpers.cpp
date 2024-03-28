#include "generalHelpers.h"
#include "plugData.h"

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

	void reposition(general* gen, int x, int y)
	{
		(*(*plugData::data.funcs.teleportCharacter))(gen, x, y);
	}


	void killGeneral(general* gen)
	{
		(*(*plugData::data.funcs.killCharacter))(gen);
	}

	void setBodyguard(general* gen, unit* un)
	{
		if (gen->bodyguards != nullptr)
		{
			un->general = gen;
			un->trackedUnitPointerP = gen->bodyguards->trackedUnitPointerP;
			gen->bodyguards->trackedUnitPointerP = 0;///
			gen->bodyguards->general = 0;

			gen->bodyguards = un;

			(*un->trackedUnitPointerP)->unit = un;
			return;

		}
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