#include "pch.h"
#include "stratModelsChange.h"
#include "campaign.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include "character.h"
#include "fort.h"
#include "gameHelpers.h"
#include "strategyMap.h"

namespace stratModelsChange
{
	enum class ModelsChangeStatus
	{
		Changed = 0,
		NeedChange = 1
	};

	struct stratModelCharacterRecord
	{
		const char* modelId;
		const char* skeletonName;
		const char* casPath;
		const char* shadowCasPath;
		const char* texturePath;
		float scale;
		stratModelArrayEntry* entry;
	};

	struct stratModelChangeRecord
	{
		UINT32 modelId = 0;
		UINT32 modelId2 = 0;
		int x = 0;
		int y = 0;
	};

	struct stratModelCharacterRecordChange
	{
		character* gen;
		const char* modelId;
	};
	
	ModelsChangeStatus CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::Changed;
	
	map<UINT32, stratModelRecord*>STRAT_MODELS;
	vector<stratModelCharacterRecord*>CHARACTER_STRAT_MODELS;
	std::vector<model_Rigid*> GAME_MODELS{};
	vector<stratModelChangeRecord*> STRAT_MODEL_CHANGE_LIST;
	vector<stratModelCharacterRecordChange*> STRAT_MODEL_CHARACTER_CHANGE_LIST;

	void pushGameModel(model_Rigid* model)
	{
		GAME_MODELS.push_back(model);
	}

	void addModelToGame(const std::string& path, const UINT32 modelId, const bool isSettlement)
	{
		auto* modRec = new stratModelRecord();
		modRec->path = path;
		modRec->modelId = modelId;
		modRec->isSettlement = isSettlement;
		STRAT_MODELS[modelId] = modRec;
	}

	void addModelToGameNoBool(const std::string& path, const UINT32 modelId)
	{
		addModelToGame(path, modelId);
	}
	
	void setModel(const int x, const int y, const UINT32 modelId, const UINT32 modelId2)
	{
		if (STRAT_MODELS.find(modelId) == STRAT_MODELS.end())
			return;
		for (size_t i = 0; i < STRAT_MODEL_CHANGE_LIST.size(); i++)
		{
			if (STRAT_MODEL_CHANGE_LIST[i]->x == x
				&& STRAT_MODEL_CHANGE_LIST[i]->y == y
				)
			{
				delete STRAT_MODEL_CHANGE_LIST[i];
				STRAT_MODEL_CHANGE_LIST.erase(STRAT_MODEL_CHANGE_LIST.begin() + i);
				i--;
			}
		}
		auto* rec = new stratModelChangeRecord();
		rec->modelId = modelId;
		rec->modelId2 = modelId2;
		rec->x = x;
		rec->y = y;
		
		if (const auto tile = stratMapHelpers::getTile(x, y); tile->getFort())
			eopFortDataDb::get()->addFortData(x, y, modelId, modelId2);
		else if (const auto sett = tile->getSettlement(); sett)
			eopSettlementDataDb::get()->getSettlementData(tile->regionId, sett->minorSettlementIndex)->modelId = modelId;

		STRAT_MODEL_CHANGE_LIST.push_back(rec);
		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
	}

	void setModelOneVar(const int x, const int y, const UINT32 modelId)
	{
		setModel(x, y, modelId, modelId);
	}

	stratModelRecord* findStratModel(const UINT32 modelId)
	{
		try
		{
			return STRAT_MODELS.at(modelId);
		}
		catch (...)
		{
			return nullptr;
		}
	}
	
	model_Rigid* getModel(const UINT32 modelId)
	{
		try
		{
			return STRAT_MODELS.at(modelId)->modelP;
		}
		catch (...)
		{
			return nullptr;
		}
	}
	
	void checkAndChangeStratModels()
	{
		if (CHANGE_MODELS_NEEDED_NOW == ModelsChangeStatus::Changed)
			return;
		
		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::Changed;
		for (const stratModelChangeRecord* changeMod : STRAT_MODEL_CHANGE_LIST) //static models
		{
			const stratModelRecord* mod1 = findStratModel(changeMod->modelId);
			if (mod1 == nullptr)
				continue;
			const stratModelRecord* mod2 = findStratModel(changeMod->modelId2);
			changeModel(changeMod->x, changeMod->y, mod1->modelP, mod2->modelP);
		}
		
		for (const stratModelCharacterRecordChange* changeMod : STRAT_MODEL_CHARACTER_CHANGE_LIST) //character models
		{
			if (changeMod->gen)
				changeStratModel(changeMod->gen, changeMod->modelId);
		}

		for (UINT32 i = 0; i < STRAT_MODEL_CHARACTER_CHANGE_LIST.size(); i++) //remove character models from change list because it only has to happen once!
		{
			delete STRAT_MODEL_CHARACTER_CHANGE_LIST[i];
			STRAT_MODEL_CHARACTER_CHANGE_LIST.erase(STRAT_MODEL_CHARACTER_CHANGE_LIST.begin() + i);
			i--;
		}

	}
	
	void loadModels()
	{
		// ReSharper disable once CppUseElementsView
		for (const auto& [index, record] : STRAT_MODELS)
		{
			record->modelP = loadModel(record->path, record->isSettlement);
		}
		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
	}

	void loadCharModels() //rebuild character CAS entries to be sure no pointers were cleaned up
	{
		for (const stratModelCharacterRecord* modRec : CHARACTER_STRAT_MODELS)
		{
			*modRec->entry = *buildCharacterCas(modRec->skeletonName, modRec->casPath, modRec->shadowCasPath, modRec->modelId, modRec->texturePath, modRec->scale);
		}
	}

	model_Rigid* loadModel(const std::string& path, const bool isSettlement)
	{
		struct simpleCas
		{
			void* cas = nullptr;
		}cas;
		const char* casPath = GAME_FUNC(const char*(__cdecl*)(const char*), getRelativePath)(path.c_str());
		gameHelpers::logStringGame("Loading model: " + string(casPath));
		GAME_FUNC(bool(__thiscall*)(simpleCas*, const char*), parseCas)(&cas, casPath);
		auto modelRigid = techFuncs::createGameClass<model_Rigid>();
		modelRigid = GAME_FUNC(model_Rigid*(__thiscall*)(model_Rigid*), createModelRigid)(modelRigid);
		char textureName[MAX_PATH];
		strncpy(textureName, casPath, sizeof(textureName));
		textureName[sizeof(textureName) -1] = '\0';
		GAME_FUNC(void(__cdecl*)(char*), appendTexturesFolder)(textureName);
		const DWORD casModel = GAME_FUNC(DWORD(__thiscall*)(simpleCas*), getCasModel)(&cas);
		const uint32_t flags = isSettlement ? 2428 : 2332;
		GAME_FUNC(char(__thiscall*)(model_Rigid*, DWORD, char*, uint32_t, int ,int, int, bool)
			, loadModelRigid)(modelRigid, casModel, textureName, flags, 2, 1, 1, true);
		pushGameModel(modelRigid);
		GAME_FUNC(void(__thiscall*)(simpleCas*), closeCas)(&cas);
		GAME_FUNC(void(__thiscall*)(simpleCas*), simpleCasDestructor)(&cas);
		return modelRigid;
	}

	void closeModel(model_Rigid* model)
	{
		if (!model)
			return;
		callClassFunc<model_Rigid*, void>(model, 0x0);
		GAME_FUNC(void(__thiscall*)(model_Rigid*), modelRigidDestructor)(model);
	}

	void clearModels()
	{
		for (model_Rigid* model : GAME_MODELS)
		{
			closeModel(model);
		}
		GAME_MODELS.clear();
		*reinterpret_cast<int*>(dataOffsets::offsets.modelRigidCounts) = 0;
	}

	void setCharacterModel(character* gen, const char* model) //add character to be changed to the queue
	{
		if (!gen || !model)
			return;
		const size_t stringSize = strlen(model);
		for (UINT32 i = 0; i < STRAT_MODEL_CHARACTER_CHANGE_LIST.size(); i++)
		{
			if (STRAT_MODEL_CHARACTER_CHANGE_LIST[i]->gen == gen)
			{
				delete STRAT_MODEL_CHARACTER_CHANGE_LIST[i];
				STRAT_MODEL_CHARACTER_CHANGE_LIST.erase(STRAT_MODEL_CHARACTER_CHANGE_LIST.begin() + i);
				i--;
			}
		}
		auto* rec = new stratModelCharacterRecordChange();
		rec->gen = gen;
		const auto modelNameCopy = new char[stringSize];
		strcpy(modelNameCopy, model);
		rec->modelId = modelNameCopy;
		STRAT_MODEL_CHARACTER_CHANGE_LIST.push_back(rec);
		const auto charData = eopCharacterDataDb::get()->getOrCreateData(gen->characterRecord->giveValidLabel(), gen->getTypeID());
		charData->model = model;

		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
	}

	void changeStratModel(character* gen, const char* model)
	{
		gameHelpers::logStringGame("assigning new model to character: " + string(model));
		if (gen == nullptr) { //maybe captain dont exist anymore
			return;
		}
		stratModelArrayEntry* entry = findCharacterStratModel(model);
		if (entry == nullptr) {
			return;
		}
		const size_t stringSize = strlen(model);

		const auto characterFacEntry = techFuncs::createGameClass<genMod>(); //make new descr character faction entry
		*characterFacEntry = *gen->genType; //get data of old entry and copy it in

		auto* modelArray = techFuncs::createGameClass<descrCharacterStratModelArray>(); //make new model array
		*modelArray = *gen->genType->stratInfo; //get data of old model array
		characterFacEntry->stratInfo = modelArray; //assign new model array to new descr character faction entry

		for (int i = 0; i < characterFacEntry->modelCount; i++) //change all models to new one in this array
		{
			if (&characterFacEntry->stratInfo->stratModelsArray[i] != nullptr)
			{
				const auto modelNameCopy = new char[stringSize];
				strcpy(modelNameCopy, model);
				characterFacEntry->stratInfo->stratModelsArray[i].modelName = modelNameCopy;
				characterFacEntry->stratInfo->stratModelsArray[i].stratModelEntry = entry;
			}
		}

		gen->genType = characterFacEntry; //assign new array to general

		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
	}


	stratModelArrayEntry* findCharacterStratModel(const char* modelId) //find eop model from vector
	{
		for (const stratModelCharacterRecord* newRec : CHARACTER_STRAT_MODELS)
		{
			if (strcmp(modelId, newRec->modelId) == 0) {
				return newRec->entry;
			}
		}
		return getStratModelEntry(modelId);
	}

	stratModelArrayEntry* getStratModelEntry(const char* name) //find vanilla model from array at offset
	{
		auto* array = reinterpret_cast<stratModelArrayEntry*>(dataOffsets::offsets.stratModelArray);
		for (int i = 0; i < 255; i++)
		{
			if (&array[i] && array[i].typeName && array[i].model_flexi_m)
			{
				if (strcmp(name, array[i].typeName) == 0)
				{
					return &array[i];
				}
			}
		}
		return nullptr;
	}

	DWORD getCasAnimSet(const char* name) //function game uses to find an animation set by name
	{
		DWORD getSkelFunc = codes::offsets.getSkeleton;
		const char** stringPointer = &name;
		DWORD skeleton = 0;
		DWORD skeletonDatabasePointer = dataOffsets::offsets.skeletonDatabase;

		_asm
		{
			push stringPointer
			mov ecx, skeletonDatabasePointer
			mov eax, getSkelFunc
			call eax
			mov ecx, [eax + 0x14]
			mov skeleton, ecx
		}

		return skeleton;
	}


	DWORD createModelFlexi(bool shadow) //create new empty model flexi using game's allocation function
	{
		DWORD assignFunc = codes::offsets.allocMemFunc;
		DWORD newModelFlexi = 0;
		DWORD createModelFlexi = shadow == true ? codes::offsets.createModelFlexiShadow : codes::offsets.createModelFlexi;
		DWORD memSize = shadow == true ? 0x11c : 0x12c;

		_asm
		{
			push memSize
			mov eax, assignFunc
			call eax
			add esp, 0x4
			mov ecx, eax
			mov eax, createModelFlexi
			call eax
			mov newModelFlexi, eax
		}

		return newModelFlexi;
	}

	void fixModelFlexi(bool shadow, DWORD stratModel, DWORD modelFlexi, const char* texturePath, DWORD skeleton, float scale) //what game does to apply loaded char cas to model flexi
	{
		DWORD funcAddr = codes::offsets.assignCasToFlexi;
		if (shadow == true)
		{
			funcAddr = codes::offsets.assignShadowCasToFlexi;
			texturePath = nullptr;
		}
		float pushScale = scale;
		_asm
		{
			push 1
			push 0
			push 1
			push pushScale
			push texturePath
			push skeleton
			push stratModel
			mov ecx, modelFlexi
			mov eax, funcAddr
			call eax
		}
	}

	DWORD loadStratCas(const char* casPath, bool shadow) //load the cas
	{
		size_t count = strlen(casPath);
		int zero = 0;
		int* zeroPointer = &zero;
		DWORD loadFunc = codes::offsets.loadStratCharModel;
		DWORD stratModel = 0;
		if (shadow) {  // NOLINT(bugprone-branch-clone)
			_asm
			{
				push count
				push casPath
				mov ecx, zeroPointer
				mov eax, loadFunc
				call eax
			}
		}
		else {
			_asm
			{
				push count
				push casPath
				mov ecx, zeroPointer
				mov eax, loadFunc
				call eax
			}
		}

		return *zeroPointer;
	}


	void addCharacterCas(const char* skeletonName, const char* casPath, const char* shadowCasPath, const char* typeName, const char* texturePath, const float scale)
	{
		//add entry to vector, using all info that is needed to rebuild cas
		const size_t size = strlen(typeName);
		auto* newRec = new stratModelCharacterRecord();
		const auto typeNameCopy2 = new char[size];
		strcpy(typeNameCopy2, typeName);
		newRec->modelId = typeNameCopy2;
		const auto skeletonCopy = new char[strlen(skeletonName)];
		strcpy(skeletonCopy, skeletonName);
		newRec->skeletonName = skeletonCopy;
		const auto copy = new char[strlen(casPath)];
		strcpy(copy, casPath);
		newRec->casPath = copy;
		const auto shadowCopy = new char[strlen(shadowCasPath)];
		strcpy(shadowCopy, shadowCasPath);
		newRec->shadowCasPath = shadowCopy;
		const auto textureCopy = new char[strlen(texturePath)];
		strcpy(textureCopy, texturePath);
		newRec->texturePath = textureCopy;
		newRec->scale = scale;
		newRec->entry = buildCharacterCas(newRec->skeletonName, newRec->casPath, newRec->shadowCasPath, newRec->modelId, newRec->texturePath, newRec->scale);
		CHARACTER_STRAT_MODELS.push_back(newRec);
	}


	stratModelArrayEntry* buildCharacterCas(const char* skeletonName, const char* casPath, const char* shadowCasPath, const char* typeName, const char* texturePath, const float scale)
	{
		//build new cas file
		const size_t size = strlen(typeName);
		const DWORD skeleton = getCasAnimSet(skeletonName);

		std::string textureString = texturePath;
		size_t pos = textureString.find_last_of("\\/") + 1;
		textureString = textureString.substr(0, pos);
		for (int i = 0; i < 2; i++)
		{
			pos = textureString.find_first_of("\\/") + 1;
			textureString = textureString.substr(pos);
		}
		const int texIndex = readTGAfile(texturePath);
		
		const DWORD newModelFlexi = createModelFlexi(false);
		const DWORD newModelFlexiShadow = createModelFlexi(true);
		const DWORD stratModel = loadStratCas(casPath, false) + 0x64;
		const DWORD stratModelShadow = loadStratCas(shadowCasPath, true) + 0x64;

		fixModelFlexi(false, stratModel, newModelFlexi, textureString.c_str(), skeleton, scale);
		fixModelFlexi(true, stratModelShadow, newModelFlexiShadow, textureString.c_str(), skeleton, scale);

		auto* newEntry = new stratModelArrayEntry;
		newEntry->model_flexi_m = newModelFlexi;
		newEntry->zeropoint = 0; //just filling the 0 stuff with loops and assignments so that there isn't random crap in there, we don't know what's used as 0
		for (int& i : newEntry->zeroarray)
		{
			i = 0;
		}
		newEntry->lodRange = 1000000.0f;
		for (int& i : newEntry->zeroarray2)
		{
			i = 0;
		}
		for (auto& [textureIndex, glossTextureIndex] : newEntry->modelTexturesFactionArray)
		{ //assigning texture for every faction
			textureIndex = texIndex;
			glossTextureIndex = -1;
		}
		for (int& i : newEntry->somearray)
		{
			i = -1;
		}
		for (int& i : newEntry->somearray2)
		{
			i = 0;
		}
		newEntry->lod2 = 1000000.0f;
		newEntry->scale = 0;
		newEntry->skeleton = skeleton;
		newEntry->modelCountNotShadow = 1;
		newEntry->indevRangeSquared = 160000;
		newEntry->shadow_model_flexi = newModelFlexiShadow;
		for (int i = 0; i < 7; i++) {
			newEntry->zeroarray3[i] = 0;
			newEntry->zeroarray4[i] = 0;
		}
		newEntry->lod3 = 1000000.0f;
		newEntry->modelCountShadow = 1;
		newEntry->zeroint = 0;
		const auto typeNameCopy = new char[size];
		strcpy(typeNameCopy, typeName);
		newEntry->typeName = typeNameCopy;
		newEntry->N54788969 = 54788969;
		return newEntry;
	}

	// ReSharper disable once CppInconsistentNaming
	int readTGAfile(const char* path) //read new tga file and add to game tga database
	{
		DWORD funcAddr = codes::offsets.readTgaFile;
		DWORD tgaDataBase = dataOffsets::offsets.tgaDataBase;
		int dbIndex = 0;
		_asm
		{
			push 2
			push 2
			push path
			mov ecx, tgaDataBase
			mov eax, funcAddr
			call eax
			mov dbIndex, eax
		}

		return dbIndex;
	}
}
