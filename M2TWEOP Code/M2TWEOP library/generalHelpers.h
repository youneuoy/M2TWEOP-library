#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace generalHelpers
{
	void sendOffMap(general* gen);
	void sabotageBuilding(general* gen, building* build);
	void diplomacyFort(general* gen, fortStruct* targetFort);
	void bribeFort(general* gen, fortStruct* targetFort);
	void spyFort(general* gen, fortStruct* targetFort);
	void bribeSettlement(general* gen, settlementStruct* targetSettlement);
	void spySettlement(general* gen, settlementStruct* targetSettlement);
	void sabotageSettlement(general* gen, settlementStruct* targetSettlement);

	const char* getTypeName(const general* gen);
	void killGeneral(general* gen);
	void setBodyguard(general* gen, unit* un);
	void setMovepoints(general* gen, float movepoints);
	float getMovepoints(const general* gen);
	int getTypeID(const general* gen);
	void setTypeID(general* gen, int newType);
	void setCharacterModel(general* gen, const char* model);
	void addCharacterCas(const char* skeletonname, const char* caspath, const char* shadowcaspath, const char* typeName, const char* texturepath, float scale);
}