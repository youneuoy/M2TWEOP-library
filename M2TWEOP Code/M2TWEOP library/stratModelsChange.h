#pragma once

#include "headersSTD.h"
#include "realGameTypes.h"
#include "fastFuncts.h"
#include "exportHeader.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
namespace stratModelsChange
{
	model_Rigid* getModel(UINT32 modelId);
	void checkAndChangeStratModels();
	void loadModels();
	model_Rigid* loadModel(const char* path);

	//add model to a game
	//pass path to model and its id(used for change models)
	//example of path - youneuoy_Data/models_strat/northern_european_large_castle.CAS
	EOP_EXPORT void addModelToGame(const char* path, UINT32 modelId);
	EOP_EXPORT void setModel(int x, int y, UINT32 modelId, UINT32 modelId2);
	EOP_EXPORT void changeStratModel(general* gen, const char* model);
	DWORD getCasAnimSet(const char* name);
	DWORD createModelFlexi(bool shadow);
	void fixModelFlexi(bool shadow, DWORD stratmodel, DWORD modelflexi, const char* texturepath, DWORD skeleton);
	DWORD loadStratCAS(const char* caspath, bool shadow);
	EOP_EXPORT void addCharacterCas(const char* skeletonname, const char* caspath, const char* shadowcaspath, const char* typeName, const char* texturepath);
	stratModelArrayEntry* buildCharacterCas(const char* skeletonname, const char* caspath, const char* shadowcaspath, const char* typeName, const char* texturepath);
	stratModelArrayEntry* getStratModelEntry(const char* name);
	stratModelArrayEntry* findCharacterStratModel(const char* modelId);
	EOP_EXPORT void setCharacterModel(general* gen, const char* model);
	int readTGAfile(const char* path);
	void loadCharModels();
	//for internal use only
	void changeModel(int x, int y, model_Rigid* modelP, model_Rigid* modelP2);
	void WINAPI disableChecker();
};