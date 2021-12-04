
#include "codeInjects.h"
#include "globalData.h"
#include "patchesForGame.h"
#include "settlementConversionLvlSetter.h"
namespace codeInjects
{
	void berserkersPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x007D3D04;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x007d34a4;
		}

		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID toberserkers = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_none, 5);
		Assembler* a = patchesManager.prepareForCoding(toberserkers);


		//code
		{
			Label rett = a->newLabel();
			Label retmet = a->newLabel();

			a->pushf();

			a->cmp(ecx, 0);
			a->je(rett);
			a->mov(eax, dword_ptr(ecx, 0x4));
			a->jmp(retmet);

			a->bind(rett);
			a->mov(eax, 0);

			a->bind(retmet);
			a->popf();
		}

		patchesManager.createCode(toberserkers);
		patchesManager.setPatchedCode(toberserkers);
	}
	void historyBattlesSoldierPatch(int gameVer)
	{
		unsigned char disSold[2] = { 0x90, 0x90 };
		void* nPtr;
		void* nbPtr;


		if (gameVer == 2)
		{

			nPtr = (void*)0x008eebbd;
			nbPtr = (void*)0x008EEBB5;

		}
		else
		{
			nPtr = (void*)0x008ee13d;
			nbPtr = (void*)0x008ee135;

		}
		memHelpers::nopBytes(nPtr, 2);
		memHelpers::nopBytes(nbPtr, 2);
	}
	void toLoadDescrBattleCharacterPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x0047bb66;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x0047b786;
		}
		void* nopAdr = (void*)((DWORD)injectAdr + 6);
		memHelpers::nopBytes(nopAdr, 5);


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_none, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			Assembler* a = new Assembler();

			a->mov(eax, dword_ptr(ecx));
			//a->push(eax);
			a->mov(ecx, esi);


			a->pushad();
			a->pushf();

			a->mov(edx, ecx);//character
			a->mov(ecx, edi);//army
			a->mov(eax, patchesForGame::onLoadDescrBattleCharacter);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toBattleStateResultsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x00618f26;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00618b86;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			Assembler* a = new Assembler();


			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::onBattleStateChange);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toReadEDUPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x008ef0bf;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x008ee63f;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::afterEDUread);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toChangePassedTurnsCountPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x00476fd3;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00476be3;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::onChangeTurnNum);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toCheckLabelResultsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x00D2899F;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00d2e61f;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(edx, eax);
			a->mov(esi, ebp);
			a->mov(eax, patchesForGame::onGiveTrait);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toCreateMapPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x00436172;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00435e31;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{

			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::afterLoadGameData);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toStartReadModelsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x0044f460;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x0044f120;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, stratModelsChange::disableChecker);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toResourcesReadModelsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x008e432d;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x008e38a9;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::unicalStratModsRead);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toEventsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x008969b0;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00895fb0;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->mov(eax, dword_ptr(esp, 0x4));
			a->pushad();
			a->pushf();

			a->mov(ecx, eax);
			a->mov(eax, patchesForGame::onEvent);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toCustomTileSelectionPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x0049ede4;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x0049e974;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(ecx, ebx);
			a->mov(eax, patchesForGame::onTileCheck);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toStratModelsSelectPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x0044f861;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x0044f521;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::checkAndChangeModels);

			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toLoadSaveFilePatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x004347fe;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x004344be;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(ecx, dword_ptr(esp, 0x34));
			a->mov(eax, patchesForGame::onLoadSaveFile);

			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toSaveGamePatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x0043530a;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00434fca;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(ecx, dword_ptr(esp, 0x26));
			a->mov(eax, patchesForGame::onSaveGame);

			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toStartNewGamePatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*) 0x0047e815;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x0047e435;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::onNewGameStart);

			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toEndReadModelsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x0044f741;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x0044f401;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_none, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			if (injectAdr == (void*)0x0044f741)
			{
				a->mov(eax, 0x00420710);
			}
			else {
				a->mov(eax, 0x004203e0);
			}
			a->call(eax);

			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::afterCampaignMapLoaded);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toClickAtCoordsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x00c2e9a4;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00c345d4;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->mov(eax, dword_ptr(esp, 0x18));
			a->pushad();
			a->pushf();

			a->mov(ecx, eax);
			a->mov(eax, patchesForGame::clickAtTile);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toSelectWorldFromDBPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x010b8ab5;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x010b82c5;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_none, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			Label overrideWorld = a->newLabel();
			Label standardWorld = a->newLabel();
			Label exLab = a->newLabel();

			a->mov(ecx, esi);
			a->pushad();
			a->pushf();

			a->mov(ecx, edi);//pointer to db
			a->mov(edx, esi);//selected record
			a->mov(eax, patchesForGame::selectWorldpkgdesc);
			a->call(eax);

			a->test(eax, eax);

			//if 0, when standard world, else not
			a->jnz(overrideWorld);

			a->bind(standardWorld);
			a->popf();
			a->popad();
			a->push(esi);
			a->jmp(exLab);


			a->bind(overrideWorld);
			a->popf();
			a->mov(dword_ptr(esp, 0x4), eax);//mov our eax to esi
			a->popad();

			a->push(esi);
			a->mov(esi, ecx);
			a->bind(exLab);

			a->lea(ecx, dword_ptr(esp, 0x18));
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toBattleLoadedPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x00441f01;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x00441bc1;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::battleLoaded);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toReadGameDBsAtGameStartPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x004122ec;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x004120ec;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::toReadGameDbsAtStart);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toDrawPartsOfStratObjectsPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x009e2a00;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x009e1e60;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtEnd, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::onDrawPartsOfStratObjects);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toGameFormDrawImagePatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x008fbd6a;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x008fb2da;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(eax, patchesForGame::onDrawAll);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toEndSettlementSiegePatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x004bf8af;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x004bf31f;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(ecx, esi);//settlement
			a->mov(eax, patchesForGame::onEndSiege);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toStartSettlementSiegePatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x004bf80d;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x004bf27d;
		}


		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_insertOriginalCodeAtStart, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(ecx, edi);//settlement
			a->mov(eax, patchesForGame::onStartSiege);
			a->call(eax);

			a->popf();
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toCastleConversionLvlSetterPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x005E9304;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x005E8EA4;
		}
		void* nopAdr = (void*)((DWORD)injectAdr + 5);
		memHelpers::nopBytes(nopAdr, 9);

		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_none, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{
			a->pushad();
			a->pushf();

			a->mov(ecx, eax);
			a->mov(eax, settlementConversionLvlSetter::getConversionLvlFromCastle);
			a->call(eax);

			a->popf();
			a->mov(dword_ptr(esp, 0x1C), eax);
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
	void toCityConversionLvlSetterPatch(const char* patchName, int gameVer)
	{
		void* injectAdr = nullptr;
		if (gameVer == 2)//steam
		{
			injectAdr = (void*)0x005E9344;
		}
		else if (gameVer == 1)//cd
		{
			injectAdr = (void*)0x005E8EE4;
		}
		void* nopAdr = (void*)((DWORD)injectAdr + 5);
		memHelpers::nopBytes(nopAdr, 15);

		patchesWorker& patchesManager = globals::dataS.patchesManager;


		codePatchID patchID = patchesManager.addPatch(patchName, injectAdr, codePatchFlag_none, 5);
		Assembler* a = patchesManager.prepareForCoding(patchID);


		//code
		{

			a->pushad();
			a->pushf();

			a->mov(ecx, eax);
			a->mov(eax, settlementConversionLvlSetter::getConversionLvlFromCity);
			a->call(eax);

			a->popf();
			a->mov(dword_ptr(esp, 0x1C), eax);
			a->popad();
		}

		patchesManager.createCode(patchID);
		patchesManager.setPatchedCode(patchID);
	}
};


