#include "stratModelsOptimise.h"
#include "fastFuncts.h"

#include <ctime>
namespace stratModelsOptimise
{

//	typedef  model_Rigid* (APIENTRY *loadCasF)(char*, int, int, int, int, int);
	struct
	{
		LPVOID oldLoadCasProc = nullptr;
	}ourData;


	model_Rigid* constructObject()
	{
		DWORD mem = fastFuncts::allocateGameMem(0x164);

		DWORD constructorAdr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			constructorAdr = 0x00934190;
		}
		else
		{
			//constructorAdr = 0x00a04380;
		}


		model_Rigid* retModel = nullptr;
		_asm
		{

			mov ecx, mem
			mov eax, constructorAdr
			call eax
			mov retModel, eax
		}

	}

#pragma pack(push,1) 
	struct someCasS
	{

	};

#pragma pack(pop)
	someCasS* createSomeCas()
	{
		return nullptr;
	}




	model_Rigid* _fastcall loadCasProc()
	{
		char* fileName;
		int param_2, param_3,  param_4,  param_5,  param_6;

		model_Rigid* modelP = constructObject();
		someCasS* someCas = createSomeCas();

		return modelP;




		//return ourData.oldLoadCasProc(fileName, param_2, param_3, param_4, param_5, param_6);
		//return FnCast((uint32_t)ourData.oldLoadCasProc, &hkLoadCasProc)(fileName, param_2, param_3, param_4, param_5, param_6);
	}

};