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
	enum class ModelsChangeStatus : uint8_t
	{
		Changed = 0,
		NeedChange = 1
	};

	struct stratModelCharacterRecord
	{
		std::string modelId;
		std::string skeletonName;
		std::string casPath;
		std::string shadowCasPath;
		std::string texturePath;
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
		std::string modelId;
	};

	namespace
	{
		auto CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::Changed;
		map<UINT32, std::shared_ptr<stratModelRecord>> STRAT_MODELS;
		std::vector<std::shared_ptr<stratModelCharacterRecord>> CHARACTER_STRAT_MODELS;
		std::vector<model_Rigid*> GAME_MODELS{};
		vector<std::shared_ptr<stratModelChangeRecord>> STRAT_MODEL_CHANGE_LIST;
		vector<std::shared_ptr<stratModelCharacterRecordChange>> STRAT_MODEL_CHARACTER_CHANGE_LIST;
	}
	
	void pushGameModel(model_Rigid* model)
	{
		GAME_MODELS.push_back(model);
	}

	void addModelToGame(const std::string& path, const UINT32 modelId, const bool isSettlement)
	{
		STRAT_MODELS[modelId] = std::make_shared<stratModelRecord>();
		STRAT_MODELS[modelId]->path = path;
		STRAT_MODELS[modelId]->modelId = modelId;
		STRAT_MODELS[modelId]->isSettlement = isSettlement;
	}

	void addModelToGameNoBool(const std::string& path, const UINT32 modelId)
	{
		addModelToGame(path, modelId);
	}
	
	void setModel(const int x, const int y, const UINT32 modelId, const UINT32 modelId2)
	{
		if (STRAT_MODELS.find(modelId) == STRAT_MODELS.end())
		{
			gameHelpers::logStringGame("setModel: modelId not found in STRAT_MODELS" + std::to_string(modelId));
			return;
		}
		for (size_t i = 0; i < STRAT_MODEL_CHANGE_LIST.size(); i++)
		{
			if (STRAT_MODEL_CHANGE_LIST[i]->x == x
				&& STRAT_MODEL_CHANGE_LIST[i]->y == y
				)
			{
				STRAT_MODEL_CHANGE_LIST.erase(STRAT_MODEL_CHANGE_LIST.begin() + i);
				i--;
			}
		}
		const auto rec = std::make_shared<stratModelChangeRecord>();
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

	std::shared_ptr<stratModelRecord> findStratModel(const UINT32 modelId)
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
		for (const auto& changeMod : STRAT_MODEL_CHANGE_LIST) //static models
		{
			const auto mod1 = findStratModel(changeMod->modelId);
			if (mod1 == nullptr)
				continue;
			const auto mod2 = findStratModel(changeMod->modelId2);
			changeModel(changeMod->x, changeMod->y, mod1->modelP, mod2->modelP);
		}
		
		for (const auto& changeMod : STRAT_MODEL_CHARACTER_CHANGE_LIST) //character models
		{
			if (changeMod->gen)
				changeStratModel(changeMod->gen, changeMod->modelId);
		}

		for (UINT32 i = 0; i < STRAT_MODEL_CHARACTER_CHANGE_LIST.size(); i++) //remove character models from change list because it only has to happen once!
		{
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
		for (const auto& modRec : CHARACTER_STRAT_MODELS)
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

	void setCharacterModel(character* gen, const std::string& model) //add character to be changed to the queue
	{
		if (!gen || model.empty())
			return;
		for (UINT32 i = 0; i < STRAT_MODEL_CHARACTER_CHANGE_LIST.size(); i++)
		{
			if (STRAT_MODEL_CHARACTER_CHANGE_LIST[i]->gen == gen)
			{
				STRAT_MODEL_CHARACTER_CHANGE_LIST.erase(STRAT_MODEL_CHARACTER_CHANGE_LIST.begin() + i);
				i--;
			}
		}
		const auto rec = std::make_shared<stratModelCharacterRecordChange>();
		rec->gen = gen;
		rec->modelId = model;
		STRAT_MODEL_CHARACTER_CHANGE_LIST.push_back(rec);
		const auto charData = eopCharacterDataDb::get()->getOrCreateData(gen->characterRecord->giveValidLabel(), gen->getTypeID());
		charData->model = model;

		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
	}

	void changeStratModel(character* gen, const std::string& model)
	{
		gameHelpers::logStringGame("assigning new model to character: " + model, false);
		if (!gen || !gen->genType) { //maybe captain dont exist anymore
			return;
		}
		stratModelArrayEntry* entry = findCharacterStratModel(model);
		if (entry == nullptr) {
			gameHelpers::logStringGame("stratModelsChange::changeStratModel: model not found: " + model);
			return;
		}
		auto characterFacEntry = gen->genType;
		if (characterFacEntry->isEopEntry != 1)
		{
			characterFacEntry = techFuncs::createGameClass<genMod>(); //make new descr character faction entry
			*characterFacEntry = *gen->genType; //get data of old entry and copy it in
		}
		else
		{
			if ( &characterFacEntry->stratInfo->stratModelsArray[0]
				&& characterFacEntry->stratInfo->stratModelsArray[0].stratModelEntry == entry
				&& strcmp(characterFacEntry->stratInfo->stratModelsArray[0].modelName.name, model.c_str()) == 0
				)
			{
				CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
				return;
			}
		}
		characterFacEntry->isEopEntry = 1;
		
		auto* modelArray = techFuncs::createGameClass<descrCharacterStratModelArray>(); //make new model array
		*modelArray = *gen->genType->stratInfo; //get data of old model array
		characterFacEntry->stratInfo = modelArray; //assign new model array to new descr character faction entry
		
		for (int i = 0; i < characterFacEntry->modelCount; i++) //change all models to new one in this array
		{
			if (&characterFacEntry->stratInfo->stratModelsArray[i] != nullptr)
			{
				characterFacEntry->stratInfo->stratModelsArray[i].modelName = *gameStringHelpers::createHashedStringGame(model.c_str());
				characterFacEntry->stratInfo->stratModelsArray[i].stratModelEntry = entry;
			}
		}
		gen->genType = characterFacEntry; //assign new array to general
		CHANGE_MODELS_NEEDED_NOW = ModelsChangeStatus::NeedChange;
	}


	stratModelArrayEntry* findCharacterStratModel(const std::string& modelId) //find eop model from vector
	{
		for (const auto& newRec : CHARACTER_STRAT_MODELS)
		{
			if (newRec->modelId == modelId) {
				return newRec->entry;
			}
		}
		return getStratModelEntry(modelId.c_str());
	}

	stratModelArrayEntry* getStratModelEntry(const char* name) //find vanilla model from array at offset
	{
		auto* array = reinterpret_cast<stratModelArrayEntry*>(dataOffsets::offsets.stratModelArray);
		for (int i = 0; i < 255; i++)
		{
			if (&array[i] && array[i].typeName && array[i].models[0])
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


	void addCharacterCas(const std::string& skeletonName, const std::string& casPath, const std::string& shadowCasPath, const std::string& typeName, const std::string& texturePath, const float scale)
	{
		//add entry to vector, using all info that is needed to rebuild cas
		const auto newRec = std::make_shared<stratModelCharacterRecord>();
		newRec->modelId = typeName;
		newRec->skeletonName = skeletonName;
		newRec->casPath = casPath;
		newRec->shadowCasPath = shadowCasPath;
		newRec->texturePath = texturePath;
		newRec->scale = scale;
		newRec->entry = buildCharacterCas(newRec->skeletonName, newRec->casPath, newRec->shadowCasPath, newRec->modelId, newRec->texturePath, newRec->scale);
		CHARACTER_STRAT_MODELS.push_back(newRec);
	}


	stratModelArrayEntry* buildCharacterCas(const std::string& skeletonName, const std::string& casPath, const std::string& shadowCasPath, const std::string& typeName, const std::string& texturePath, const float scale)
	{
		//build new cas file
		const DWORD skeleton = getCasAnimSet(skeletonName.c_str());

		size_t pos = texturePath.find_last_of("\\/") + 1;
		auto textureString = texturePath.substr(0, pos);
		for (int i = 0; i < 2; i++)
		{
			pos = textureString.find_first_of("\\/") + 1;
			textureString = textureString.substr(pos);
		}
		const int texIndex = readTGAfile(texturePath.c_str());
		
		const DWORD newModelFlexi = createModelFlexi(false);
		const DWORD newModelFlexiShadow = createModelFlexi(true);
		const DWORD stratModel = loadStratCas(casPath.c_str(), false) + 0x64;
		const DWORD stratModelShadow = loadStratCas(shadowCasPath.c_str(), true) + 0x64;

		fixModelFlexi(false, stratModel, newModelFlexi, textureString.c_str(), skeleton, scale);
		fixModelFlexi(true, stratModelShadow, newModelFlexiShadow, textureString.c_str(), skeleton, scale);

		auto* newEntry = techFuncs::createGameClass<stratModelArrayEntry>();
		newEntry->models[0] = newModelFlexi;
		newEntry->lodRanges[0] = 1000000.0f;
		for (auto& [textureIndex, glossTextureIndex] : newEntry->modelTexturesFactionArray)
		{ //assigning texture for every faction
			textureIndex = texIndex;
			glossTextureIndex = -1;
		}
		newEntry->diffuse = -1;
		newEntry->gloss = -1;
		newEntry->triRange = 1000000.0f;
		newEntry->skeleton[0] = skeleton;
		newEntry->modelCount = 1;
		newEntry->indivRangeSquared = 160000;
		newEntry->shadowModels[0] = newModelFlexiShadow;
		newEntry->shadowModelRanges[0] = 1000000.0f;
		newEntry->shadowModelCount = 1;
		newEntry->typeName = *reinterpret_cast<char**>(gameStringHelpers::createHashedStringGame(typeName.c_str()));
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
