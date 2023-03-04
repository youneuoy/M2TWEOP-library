#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace objectsHelpers
{
	void startDrawModelAt(UINT32 modelId, int x,int y, float size);
	void stopDrawModel(UINT32 modelId);

	void setModel(int x,int y, UINT32 modelId, UINT32 modelId2);
	void setModelOneVar(int x,int y, UINT32 modelId);
	void addModelToGame(const std::string& path,UINT32 modelId);


	void replaceTile(const std::string & tileLabel, UINT32 x, UINT32 y, const std::string &fileName, const std::string &weather, const std::string &dayTime);
};

