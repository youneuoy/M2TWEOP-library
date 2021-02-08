#include "stratModelsChange.h"
namespace stratModelsChange
{
	struct stratModelRecord
	{
		UINT32 modelId;
		string path;
		model_Rigid* modelP = nullptr;
	};

	vector<stratModelRecord*>stratModels;

	NOINLINE EOP_EXPORT void addModelToGame(const char* path, UINT32 modelId)
	{
		stratModelRecord* modRec = new stratModelRecord();
		modRec->path = path;
		modRec->modelId = modelId;

		stratModels.push_back(modRec);
		return;
	}



	struct stratModelChangeRecord
	{
		UINT32 modelId;
		UINT32 modelId2;
		int x;
		int y;
	};

	vector<stratModelChangeRecord*>stratModelChangeList;


	void setModel(int x, int y, UINT32 modelId, UINT32 modelId2)
	{
		/*if (fastFuncts::findFort(x, y) == nullptr
			&& fastFuncts::findPort(x, y) == nullptr
			&& fastFuncts::findResource(x, y) == nullptr
			) return;
*/
		for (int i = 0; i < stratModelChangeList.size(); i++)
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
	}

	stratModelRecord* findStratModel(UINT32 modelId)
	{
		for (stratModelRecord* modRec : stratModels)
		{
			if (modRec->modelId == modelId)
				return modRec;
		}

		return nullptr;
	}

	void checkAndChangeStratModels()
	{
		for (stratModelChangeRecord* changeMod : stratModelChangeList)
		{

			stratModelRecord* mod1 = findStratModel(changeMod->modelId);
			if (mod1 == nullptr)continue;

			stratModelRecord* mod2 = nullptr;

			mod2 = findStratModel(changeMod->modelId2);

			changeModel(changeMod->x, changeMod->y, mod1->modelP, mod2->modelP);


		}
	}







	bool modelsLoaded = false;
	void __stdcall disableChecker()
	{
		modelsLoaded = false;
	}
	void loadModels()
	{
		if (modelsLoaded == true)return;


		for (stratModelRecord* modRec : stratModels)
		{
			modRec->modelP = loadModel(modRec->path.c_str());
		}
		modelsLoaded = true;
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
		modRig[*modsNum] = nullptr;

		*modsNum = *modsNum - 1;

		return reinterpret_cast<model_Rigid*>(res);
	}
}
