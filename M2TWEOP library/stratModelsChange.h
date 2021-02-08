#pragma once

#include "headersSTD.h"
#include "realGameTypes.h"
#include "fastFuncts.h"
#include "exportHeader.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
namespace stratModelsChange
{
	void checkAndChangeStratModels();
	void loadModels();
	model_Rigid* loadModel(const char* path);

	//add model to a game
	//pass path to model and its id(used for change models)
	//example of path - youneuoy_Data/models_strat/northern_european_large_castle.CAS
	NOINLINE EOP_EXPORT void addModelToGame(const char* path,UINT32 modelId);
	NOINLINE EOP_EXPORT void setModel(int x, int y, UINT32 modelId, UINT32 modelId2);

	//for internal use only
	void changeModel(int x, int y, model_Rigid* modelP, model_Rigid* modelP2);
	void WINAPI disableChecker();
};

