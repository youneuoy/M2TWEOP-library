#include "stratModelsChange.h"
#include "fastFuncts.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
namespace stratModelsChange
{
	enum class modelsChangeStatus
	{
		changed = 0,
		needFixHiding = 1,
		needChange = 2
	};
	modelsChangeStatus changeModelsNeededNow = modelsChangeStatus::changed;

	struct stratModelRecord
	{
		UINT32 modelId = 0;
		string path;
		model_Rigid* modelP = nullptr;
	};

	struct stratModelCharacterRecord
	{
		const char* modelId;
		const char* skeletonname;
		const char* caspath;
		const char* shadowcaspath;
		const char* texturepath;
		stratModelArrayEntry* entry;
	};

	map<UINT32, stratModelRecord*>stratModels;
	vector<stratModelCharacterRecord*>characterStratModels;

	NOINLINE EOP_EXPORT void addModelToGame(const char* path, UINT32 modelId)
	{
		stratModelRecord* modRec = new stratModelRecord();
		modRec->path = path;
		modRec->modelId = modelId;

		stratModels[modelId] = modRec;
		return;
	}



	struct stratModelChangeRecord
	{
		UINT32 modelId = 0;
		UINT32 modelId2 = 0;
		int x = 0;
		int y = 0;
	};

	struct stratModelCharacterRecordChange
	{
		general* gen;

		const char* modelId;
	};

	vector<stratModelChangeRecord*>stratModelChangeList;
	vector<stratModelCharacterRecordChange*>stratModelCharacterChangeList;


	void setModel(int x, int y, UINT32 modelId, UINT32 modelId2)
	{
		/*if (fastFuncts::findFort(x, y) == nullptr
			&& fastFuncts::findPort(x, y) == nullptr
			&& fastFuncts::findResource(x, y) == nullptr
			) return;
*/
		for (size_t i = 0; i < stratModelChangeList.size(); i++)
		{
			if (stratModelChangeList[i]->x == x
				&& stratModelChangeList[i]->y == y
				)
			{
				delete stratModelChangeList[i];
				stratModelChangeList.erase(stratModelChangeList.begin() + i);
				i--;
			}
		}
		stratModelChangeRecord* rec = new stratModelChangeRecord();
		rec->modelId = modelId;
		rec->modelId2 = modelId2;
		rec->x = x;
		rec->y = y;


		stratModelChangeList.push_back(rec);

		changeModelsNeededNow = modelsChangeStatus::needChange;
	}

	stratModelRecord* findStratModel(UINT32 modelId)
	{
		try
		{
			return stratModels.at(modelId);
		}
		catch (...)
		{
			return nullptr;
		}


		return nullptr;
	}


	model_Rigid* getModel(UINT32 modelId)
	{
		try
		{
			return stratModels.at(modelId)->modelP;
		}
		catch (...)
		{
			return nullptr;
		}


		return nullptr;
	}
	struct visibilityCrashFixS
	{
		visibilityCrashFixS(int x, int y, factionStruct* fac, int8_t vis)
			:X(x), Y(y), Fac(fac), Vis(vis)
		{

		}
		int X;
		int Y;
		factionStruct* Fac = nullptr;
		int8_t Vis = 0;
	};
	vector<visibilityCrashFixS> crashFixAr;
	//Change only when render queue is happening to avoid crashes
	void checkAndChangeStratModels()
	{
		if (changeModelsNeededNow == modelsChangeStatus::needFixHiding)
		{
			//for (stratModelChangeRecord* changeMod : stratModelChangeList) //static models
			//{
			//	fastFuncts::hideRevealedTileForEveryone(changeMod->x, changeMod->y);
			//}
			for (auto& visFix : crashFixAr)
			{
				fastFuncts::setTileVisibility(visFix.Fac, visFix.X, visFix.Y, visFix.Vis);
			}
			crashFixAr.clear();
			changeModelsNeededNow = modelsChangeStatus::changed;
			return;
		}
		if (changeModelsNeededNow == modelsChangeStatus::changed)
		{
			return;
		}

		crashFixAr.reserve(stratModelChangeList.size());
		for (stratModelChangeRecord* changeMod : stratModelChangeList) //static models
		{
			stratModelRecord* mod1 = findStratModel(changeMod->modelId);
			if (mod1 == nullptr)continue;

			stratModelRecord* mod2 = nullptr;

			mod2 = findStratModel(changeMod->modelId2);

			if (changeModel(changeMod->x, changeMod->y, mod1->modelP, mod2->modelP) == true)
			{
				UINT32 numFac = fastFuncts::getFactionsCount();
				factionStruct** listFac = fastFuncts::getFactionsList();

				for (UINT32 i = 0; i < numFac; i++)
				{
					auto vis = fastFuncts::getTileVisibility(listFac[i], changeMod->x, changeMod->y);
					if (vis == 0)
					{
						continue;
					}

					crashFixAr.emplace_back(changeMod->x, changeMod->y, listFac[i], vis);
					fastFuncts::setTileVisibility(listFac[i], changeMod->x, changeMod->y, 1);
				}

				//fastFuncts::revealTileForEveryone(changeMod->x, changeMod->y);
			}
		}
		for (stratModelCharacterRecordChange* changeMod : stratModelCharacterChangeList) //character models
		{
			changeStratModel(changeMod->gen, changeMod->modelId);
		}

		for (UINT32 i = 0; i < stratModelCharacterChangeList.size(); i++) //remove character models from change list because it only has to happen once!
		{
			delete stratModelCharacterChangeList[i];
			stratModelCharacterChangeList.erase(stratModelCharacterChangeList.begin() + i);
			i--;
		}

		changeModelsNeededNow = modelsChangeStatus::needFixHiding;
	}

	void update()
	{
	}

	bool modelsLoaded = false;
	void __stdcall disableChecker()
	{
		modelsLoaded = false;
	}
	void loadModels()
	{
		if (modelsLoaded == true)return;

		for (auto& modRec : stratModels)
		{
			modRec.second->modelP = loadModel(modRec.second->path.c_str());
		}
		modelsLoaded = true;
		changeModelsNeededNow = modelsChangeStatus::needChange;
	}

	void loadCharModels() //rebuild character CAS entries to be sure no pointers were cleaned up
	{
		if (modelsLoaded == true)return;

		for (stratModelCharacterRecord* modRec : characterStratModels)
		{
			*modRec->entry = *buildCharacterCas(modRec->skeletonname, modRec->caspath, modRec->shadowcaspath, modRec->modelId, modRec->texturepath);
		}
	}

	model_Rigid* loadModel(const char* path)
	{
		DWORD adr = codes::offsets.loadStratCasModelFunc;
		DWORD res = 0;
		_asm {
			push 0x1
			push 0x1
			push 0x1
			push 0x2
			push 0x97c
			push path

			mov eax, [adr]
			call eax
			add esp, 0x18
			mov[res], eax
		}
		int* modsNum = reinterpret_cast<int*>(dataOffsets::offsets.statStratModelsListOffset + 0x8dc);

		model_Rigid** modRig = reinterpret_cast<model_Rigid**>(dataOffsets::offsets.statStratModelsListOffset);
		modRig[*modsNum - 1] = nullptr;

		*modsNum = *modsNum - 1;

		return reinterpret_cast<model_Rigid*>(res);
	}

	NOINLINE EOP_EXPORT void setCharacterModel(general* gen, const char* model) //add character to be changed to the queue
	{
		int stringsize = strlen(model);
		for (UINT32 i = 0; i < stratModelCharacterChangeList.size(); i++)
		{
			if (stratModelCharacterChangeList[i]->gen == gen)
			{
				delete stratModelCharacterChangeList[i];
				stratModelCharacterChangeList.erase(stratModelCharacterChangeList.begin() + i);
				i--;
			}
		}
		stratModelCharacterRecordChange* rec = new stratModelCharacterRecordChange();
		rec->gen = gen;
		char* modelNameCopy = new char[stringsize];
		strcpy(modelNameCopy, model);
		rec->modelId = modelNameCopy;
		stratModelCharacterChangeList.push_back(rec);

		changeModelsNeededNow = modelsChangeStatus::needChange;
	}

	void changeStratModel(general* gen, const char* model)
	{
		if (gen == nullptr) { //maybe captain dont exist anymore
			return;
		}
		stratModelArrayEntry* modelentry = findCharacterStratModel(model); //get eop strat model from vector
		if (modelentry == nullptr) {
			stratModelArrayEntry* modelentry = getStratModelEntry(model); //get vanilla strat model from 255 array
		}
		if (modelentry == nullptr) {
			return;
		}
		int stringsize = strlen(model);
		genMod* characterFacEntry = new genMod; //make new descr character faction entry
		*characterFacEntry = *gen->genType; //get data of old entry and copy it in
		descrCharacterStratModelArray* modelArray = new descrCharacterStratModelArray; //make new model array
		*modelArray = *gen->genType->stratInfo; //get data of old model array
		characterFacEntry->stratInfo = modelArray; //assign new model array to new descr character faction entry
		for (int i = 0; i < characterFacEntry->modelCount; i++) //change all models to new one in this array
		{
			if (&characterFacEntry->stratInfo->stratModelsArray[i] != nullptr)
			{
				char* modelNameCopy = new char[stringsize];
				strcpy(modelNameCopy, model);
				characterFacEntry->stratInfo->stratModelsArray[i].modelName = modelNameCopy;
				characterFacEntry->stratInfo->stratModelsArray[i].stratModelEntry = modelentry;
			}
		}
		gen->genType = characterFacEntry; //assign new array to general

		changeModelsNeededNow = modelsChangeStatus::needChange;
	}


	stratModelArrayEntry* findCharacterStratModel(const char* modelId) //find eop model from vector
	{
		for (stratModelCharacterRecord* newRec : characterStratModels)
		{
			if (strcmp(modelId, newRec->modelId) == 0) {
				return newRec->entry;
			}
		}

		return nullptr;
	}

	stratModelArrayEntry* getStratModelEntry(const char* name) //find vanilla model from array at offset
	{
		stratModelArrayEntry* array = (stratModelArrayEntry*)dataOffsets::offsets.stratModelArray;
		for (int i = 0; i < 255; i++)
		{
			if (&array[i] != nullptr)
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
		const char** stringpointer = &name;
		DWORD skeleton = 0;
		DWORD skeletonDatabasePointer = dataOffsets::offsets.skeletonDatabase;

		_asm
		{
			push stringpointer
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
		DWORD assignfunc = codes::offsets.allocMemFunc;
		DWORD newModelFlexi = 0;
		DWORD createModelFlexi = codes::offsets.createModelFlexi;
		DWORD memsize = 0x12c;

		if (shadow == true) {
			memsize = 0x11c;
			createModelFlexi = codes::offsets.createModelFlexiShadow;
		}

		_asm
		{
			push memsize
			mov eax, assignfunc
			call eax
			add esp, 0x4
			mov ecx, eax
			mov eax, createModelFlexi
			call eax
			mov newModelFlexi, eax
		}

		return newModelFlexi;
	}

	void fixModelFlexi(bool shadow, DWORD stratmodel, DWORD modelflexi, const char* texturepath, DWORD skeleton) //what game does to apply loaded char cas to model flexi
	{
		DWORD funcAddr = codes::offsets.assignCasToFlexi;
		if (shadow == true)
		{
			funcAddr = codes::offsets.assignShadowCasToFlexi;
			texturepath = nullptr;
		}
		float scale = 1.0;
		_asm
		{
			push 1
			push 0
			push 1
			push scale
			push texturepath
			push skeleton
			mov eax, stratmodel
			add eax, 0x64
			push eax
			mov ecx, modelflexi
			mov eax, funcAddr
			call eax
		}
	}

	DWORD loadStratCAS(const char* caspath, bool shadow) //load the cas
	{
		int count = strlen(caspath);
		int zero = 0;
		int* zeropointer = &zero;
		DWORD loadFunc = codes::offsets.loadStratCharModel;
		DWORD stratModel = 0;
		DWORD stackOffset = 0x178;
		if (shadow) {
			_asm
			{
				push count
				push caspath
				mov ecx, zeropointer
				mov eax, loadFunc
				call eax
				test al, al
				mov eax, [esp + 0x174]
				mov stratModel, eax
			}
		}
		else {
			_asm
			{
				push count
				push caspath
				mov ecx, zeropointer
				mov eax, loadFunc
				call eax
				test al, al
				mov eax, [esp + 0x178]
				mov stratModel, eax
			}
		}

		return stratModel;
	}


	void addCharacterCas(const char* skeletonname, const char* caspath, const char* shadowcaspath, const char* typeName, const char* texturepath)
	{
		//add entry to vector, using all info that is needed to rebuild cas
		int stringsize = strlen(typeName);
		stratModelCharacterRecord* newRec = new stratModelCharacterRecord();
		char* typeNameCopy2 = new char[stringsize];
		strcpy(typeNameCopy2, typeName);
		newRec->modelId = typeNameCopy2;
		char* skeletonCopy = new char[strlen(skeletonname)];
		strcpy(skeletonCopy, skeletonname);
		newRec->skeletonname = skeletonCopy;
		char* caspathCopy = new char[strlen(caspath)];
		strcpy(caspathCopy, caspath);
		newRec->caspath = caspathCopy;
		char* shadowpathCopy = new char[strlen(shadowcaspath)];
		strcpy(shadowpathCopy, shadowcaspath);
		newRec->shadowcaspath = shadowpathCopy;
		char* textureCopy = new char[strlen(texturepath)];
		strcpy(textureCopy, texturepath);
		newRec->texturepath = textureCopy;
		newRec->entry = buildCharacterCas(newRec->skeletonname, newRec->caspath, newRec->shadowcaspath, newRec->modelId, newRec->texturepath);
		characterStratModels.push_back(newRec);
	}


	stratModelArrayEntry* buildCharacterCas(const char* skeletonname, const char* caspath, const char* shadowcaspath, const char* typeName, const char* texturepath)
	{
		//build new cas file
		int stringsize = strlen(typeName);
		DWORD skeleton = getCasAnimSet(skeletonname);
		DWORD newModelFlexi = createModelFlexi(false);
		DWORD newModelFlexiShadow = createModelFlexi(true);
		DWORD stratmodel = loadStratCAS(caspath, false);//this is very fickle with the stack position the data gets pulled from, watch out!
		DWORD stratmodelShadow = loadStratCAS(shadowcaspath, true);
		std::string texturepathString = texturepath;
		size_t pos = texturepathString.find_last_of("\\/") + 1;
		texturepathString = texturepathString.substr(0, pos);
		pos = texturepathString.find_first_of("\\/") + 1;
		texturepathString = texturepathString.substr(pos);
		pos = texturepathString.find_first_of("\\/") + 1;
		texturepathString = texturepathString.substr(pos);
		fixModelFlexi(false, stratmodel, newModelFlexi, texturepathString.c_str(), skeleton);
		fixModelFlexi(true, stratmodelShadow, newModelFlexiShadow, texturepathString.c_str(), skeleton);
		int textureindex = readTGAfile(texturepath);
		stratModelArrayEntry* newEntry = new stratModelArrayEntry;
		newEntry->model_flexi_m = newModelFlexi;
		newEntry->zeropoint = 0; //just filling the 0 stuff with loops and assignments so that there isn't random crap in there, we don't know what's used as 0
		for (int i = 0; i < 6; i++) {
			newEntry->zeroarray[i] = 0;
		}
		newEntry->lodRange = (float)1000000.0;
		for (int i = 0; i < 9; i++) {
			newEntry->zeroarray2[i] = 0;
		}
		for (int i = 0; i < 31; i++) { //assigning texture for every faction
			newEntry->modelTexturesFactionArray[i].textureIndex = textureindex;
			newEntry->modelTexturesFactionArray[i].glossTextureIndex = -1;
		}
		for (int i = 0; i < 33; i++) {
			newEntry->somearray[i] = -1;
		}
		for (int i = 0; i < 3; i++) {
			newEntry->somearray2[i] = 0;
		}
		newEntry->lod2 = (float)1000000.0;
		newEntry->scale = 0;
		newEntry->skeleton = skeleton;
		newEntry->modelCountNotShadow = 1;
		newEntry->indevRangeSquared = 160000;
		newEntry->shadow_model_flexi = newModelFlexiShadow;
		for (int i = 0; i < 7; i++) {
			newEntry->zeroarray3[i] = 0;
			newEntry->zeroarray4[i] = 0;
		}
		newEntry->lod3 = (float)1000000.0;
		newEntry->modelCountShadow = 1;
		newEntry->zeroint = 0;
		char* typeNameCopy = new char[stringsize];
		strcpy(typeNameCopy, typeName);
		newEntry->typeName = typeNameCopy;
		newEntry->N54788969 = 54788969;
		return newEntry;
	}

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
