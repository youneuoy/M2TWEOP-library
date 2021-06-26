#include "stratModelsOptimise.h"
#include "realGameTypes.h"
#include "fastFuncts.h"

#include <ctime>
namespace stratModelsOptimise
{
	template<typename T>
	T FnCast(uint32_t fnToCast, T pFnCastTo) {
		(void)pFnCastTo;
		return (T)fnToCast;
	}

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
	static NOINLINE model_Rigid* APIENTRY hkLoadCasProc(char* fileName, int param_2, int param_3, int param_4, int param_5, int param_6)
	{

		// здесь должен быть фрагмент кода, время выполнения которого нужно измерить


				//model_Rigid* modelP = constructObject();
				//someCasS* someCas = createSomeCas();


				//string pathWithModFolder = "mods/teutonic/";
				//pathWithModFolder += fileName;

			//	return modelP;


		return FnCast((uint32_t)ourData.oldLoadCasProc, &hkLoadCasProc)(fileName, param_2, param_3, param_4, param_5, param_6);
	}




	void createHook()
	{
		DWORD adr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			adr = 0x00a04f00;
		}
		else
		{
			adr = 0x00a04380;
		}

		if (MH_CreateHook((char*)adr, &hkLoadCasProc,
			reinterpret_cast<LPVOID*>(&ourData.oldLoadCasProc)) != MH_OK)
		{
			return;
		}
		if (MH_EnableHook((char*)adr) != MH_OK)
		{
			return;
		}
	}

};