#include "pch.h"
#include "Injects.h"

Injects::Injects(MemWork* mem)
	:AATemplate(mem)
{
	m_adress = 0x0074227A;
}

Injects::~Injects()
{
}

void Injects::SetOriginalUnitCode()
{
	Assembler* a = new Assembler();
	a->imul(edx, dword_ptr(esi, 0x50C));
	a->test(edx, edx);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void Injects::SetlUnitCode()
{
	Assembler* a = new Assembler();
	a->mov(dword_ptr(esi, 0x50C), 50);

	a->imul(edx, dword_ptr(esi, 0x50C));
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

Age::Age(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x005a7d81;
	else if (ver == 1)//kingdoms
		m_adress = 0x005a78a1;
}

Age::~Age()
{
}

void Age::SetOriginaAgeCode()
{
	Assembler* a = new Assembler();
	a->mov(esi, ecx);
	a->mov(eax, dword_ptr(esi, 0x218));
	a->cmp(byte_ptr(eax, 0x200), 00);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void Age::SetlAgeCode()
{
	Assembler* a = new Assembler();
	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->mov(esi, ecx);
	a->mov(eax, dword_ptr(esi, 0x218));
	//a->cmp(byte_ptr(eax, 0x200), 00);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

AgeVisual::AgeVisual(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00beb81B;
	else if (ver == 1)//kingdoms
		m_adress = 0x00bf148b;
}

AgeVisual::~AgeVisual()
{
}

void AgeVisual::SetOriginaAgeCode()
{
	Assembler* a = new Assembler();
	a->mov(dword_ptr(esp, 0x54), esp);
	a->push(0x012FDB48);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void AgeVisual::SetlAgeCode()
{
	if (m_adress == 0x00beb81B)
	{
		Assembler* a = new Assembler();
		a->pushad();
		a->pushf();

		a->mov(eax, (DWORD)funcAdress);
		a->call(eax);

		a->popf();
		a->popad();

		a->mov(dword_ptr(esp, 0x54), esp);
		a->push(0x012FDB48);

		a->ret();
		m_cheatBytes = (unsigned char*)a->make();

		delete a;
	}
	else if (m_adress == 0x00bf148b)
	{
		Assembler* a = new Assembler();
		a->pushad();
		a->pushf();

		a->mov(eax, (DWORD)funcAdress);
		a->call(eax);

		a->popf();
		a->popad();

		a->mov(dword_ptr(esp, 0x54), esp);
		a->push(0x01342b68);

		a->ret();
		m_cheatBytes = (unsigned char*)a->make();

		delete a;
	}
}

berserkersP::berserkersP(MemWork* mem, int ver)
	:AATemplate(mem)
{
	if (ver == 2)//steam
		m_adress = 0x007D3D04;
	else if (ver == 1)//kingdoms
		m_adress = 0x007d34a4;
}

berserkersP::~berserkersP()
{
}

void berserkersP::SetOriginaBersCode()
{
	Assembler* a = new Assembler();
	a->mov(eax, dword_ptr(ecx, 0x4));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void berserkersP::SetlBersCode()
{
	Assembler* a = new Assembler();
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

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toReadEDU::toReadEDU(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008ef0bf;
	else if (ver == 1)//kingdoms
		m_adress = 0x008ee63f;
}

toReadEDU::~toReadEDU()
{
}

void toReadEDU::SetOriginalEDUCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, ebx);
	a->call(dword_ptr(eax, 0x14));
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toReadEDU::SetlEDUCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, ebx);
	a->call(dword_ptr(eax, 0x14));

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toChangePassedTurnsCount::toChangePassedTurnsCount(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00476fd3;
	else if (ver == 1)//kingdoms
		m_adress = 0x00476be3;
}

toChangePassedTurnsCount::~toChangePassedTurnsCount()
{
}

void toChangePassedTurnsCount::SetOriginalTurnsCountCode()
{
	Assembler* a = new Assembler();
	//medieval2.exe + 76FD3 - add dword ptr[esi + 00000374], 01 { 1 }


	a->add(dword_ptr(esi, 0x374), 01);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toChangePassedTurnsCount::SetlTurnsCountCode()
{
	Assembler* a = new Assembler();

	a->add(dword_ptr(esi, 0x374), 01);


	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toBattleResults::toBattleResults(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00aa90b2;
	//m_adress = 0x00629d0a;
	//m_adress = 0x00627109;
	else if (ver == 1)//kingdoms
		m_adress = 0x00626d69;
}

toBattleResults::~toBattleResults()
{

}

void toBattleResults::SetOriginalBattleResCode()
{

	Assembler* a = new Assembler();
	//medieval2.exe + 6A90C4 - pop esi
	//	medieval2.exe + 6A90C5 - pop ebx
	//	medieval2.exe + 6A90C6 - add esp, 48 { 72 }
	//call dword ptr[012E73B0]
	a->mov(eax, 0x012E73B0);
	a->mov(eax, dword_ptr(eax));
	a->call(eax);


	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;

}

void toBattleResults::SetlBattleResCode()
{
	Assembler* a = new Assembler();
	a->mov(eax, 0x012E73B0);
	a->mov(eax, dword_ptr(eax));
	a->call(eax);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

/*
void toBattleResults::SetOriginalBattleResCode()
{
	//medieval2.exe + 227109 - cmp[esi + 18], edi
	//	medieval2.exe + 22710C - mov[esp + 10], edi
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(esi, 0x18),edi);
	a->mov(dword_ptr(esp, 0x10),edi);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;

}

void toBattleResults::SetlBattleResCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(esi, 0x18), edi);
	a->mov(dword_ptr(esp, 0x10), edi);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}
*/

toCheckLabelResults::toCheckLabelResults(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00D2899F;
	else if (ver == 1)//kingdoms
		m_adress = 0x00d2e61f;
}

toCheckLabelResults::~toCheckLabelResults()
{
}

void toCheckLabelResults::SetOriginalCheckLabCode()
{
	Assembler* a = new Assembler();
	//mov edi, eax
	//add esp, 08

	a->mov(edi, eax);
	a->add(esp, 0x8);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toCheckLabelResults::SetlCheckLabCode()
{
	Assembler* a = new Assembler();

	a->mov(edi, eax);
	a->add(esp, 0x8);

	a->pushad();
	a->pushf();

	a->mov(edx, eax);
	a->mov(esi, ebp);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toCreateMap::toCreateMap(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00436172;
	else if (ver == 1)//kingdoms
		m_adress = 0x00435e31;
}

toCreateMap::~toCreateMap()
{
}

void toCreateMap::SetOriginalCreateCode()
{
	Assembler* a = new Assembler();

	//add esp, x3c04

	a->add(esp, 0x3c04);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toCreateMap::SetlCreateCode()
{
	Assembler* a = new Assembler();

	a->add(esp, 0x3c04);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toI_CompareCounter::toI_CompareCounter(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00a9b505;
	else if (ver == 1)//kingdoms
		m_adress = 0x00a9a4b0;
}

toI_CompareCounter::~toI_CompareCounter()
{
}

void toI_CompareCounter::SetOriginaltoI_CompareCounterCode()
{
	Assembler* a = new Assembler();

	//mov ecx,[esi+14]
	//mov esi, [esi + 08]


	a->mov(ecx, dword_ptr(esi, 0x14));
	a->mov(esi, dword_ptr(esi, 0x8));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toI_CompareCounter::SetltoI_CompareCounterCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->mov(ecx, dword_ptr(esi, 0x14));
	a->mov(esi, dword_ptr(esi, 0x8));

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toFortsDelCheck::toFortsDelCheck(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0047d116;
	else if (ver == 1)//kingdoms
		m_adress = 0x0047cd36;
}


toFortsDelCheck::~toFortsDelCheck()
{
}

void toFortsDelCheck::SetOriginalFortsDelCode()
{
	Assembler* a = new Assembler();

	// MOV        EDI ,dword ptr [EDX  + EBX *0x4 ]
	//CMP        dword ptr [EDI  + 0x44 ],0x0

	a->imul(ebx, 0x4);
	a->mov(edi, dword_ptr(edx, ebx));
	a->cmp(dword_ptr(edi, 0x44), 0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toFortsDelCheck::SetlFortsDelCode()
{
	Assembler* a = new Assembler();

	a->add(edx, ebx);
	a->add(edx, ebx);
	a->add(edx, ebx);
	a->add(edx, ebx);
	a->mov(edi, dword_ptr(edx));


	a->push(eax);

	a->pushf();

	a->push(ecx);
	a->push(edx);
	a->push(ebx);
	a->push(ebp);
	a->push(esi);
	a->push(edi);


	a->mov(edx, edi);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->pop(edi);
	a->pop(esi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(edx);
	a->pop(ecx);

	a->popf();

	a->test(eax, eax);

	a->pop(eax);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toDSFromFacCh::toDSFromFacCh(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00d59169;
	else if (ver == 1)//kingdoms
		m_adress = 0x00d5edf9;
}

toDSFromFacCh::~toDSFromFacCh()
{
}

void toDSFromFacCh::SetOriginalDsFromFacCode()
{
	Assembler* a = new Assembler();

	a->add(eax, 0x259);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toDSFromFacCh::SetlDsFromFacCode()
{
	Assembler* a = new Assembler();

	a->push(esi);
	a->mov(esi, dword_ptr(esp, 0x8));

	a->pushf();

	a->push(ecx);
	a->push(edx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->mov(edx, esi);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(edx);
	a->pop(ecx);

	a->popf();
	a->pop(esi);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toDiplomaticStanceCheck::toDiplomaticStanceCheck(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0086eb42;

	else if (ver == 1)//kingdoms
		m_adress = 0x0086e27d;
}

toDiplomaticStanceCheck::~toDiplomaticStanceCheck()
{
}

void toDiplomaticStanceCheck::SetOriginalDsCheckCode()
{
	Assembler* a = new Assembler();
	//mov ecx,00000001
	a->mov(ecx, 00000001);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toDiplomaticStanceCheck::SetlDsCheckCode()
{
	Assembler* a = new Assembler();

	a->push(esi);

	a->mov(esi, dword_ptr(esp, 0x8));
	a->pushf();

	a->push(ecx);
	a->push(edx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->mov(edx, ebp);

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(edx);
	a->pop(ecx);

	a->popf();
	a->pop(esi);


	a->mov(ecx, 00000001);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

DCICrashHard::DCICrashHard(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x009bee86;

	else if (ver == 1)//kingdoms
		m_adress = 0x009be306;
}

DCICrashHard::~DCICrashHard()
{
}

void DCICrashHard::SetOriginaDCICode()
{
	Assembler* a = new Assembler();
	//mov ecx,00000001
	a->mov(ecx, 00000001);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void DCICrashHard::SetlDCICode()
{
	Assembler* a = new Assembler();

	a->pop(ecx);
	a->pop(ecx);
	a->pop(ecx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toSpawnArmy::toSpawnArmy(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00a8cdd7;

	else if (ver == 1)//kingdoms
		m_adress = 0x00a8bd77;
}

toSpawnArmy::~toSpawnArmy()
{
}

void toSpawnArmy::SetOriginaSpawnCode()
{
	Assembler* a = new Assembler();
	//mov edx,[esp+30]
	//mov eax, [esp + 34]

	a->mov(edx, dword_ptr(esp, 0x30));
	a->mov(eax, dword_ptr(esp, 0x34));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toSpawnArmy::SetlSpawnCode()
{
	Assembler* a = new Assembler();
	a->pushad();
	a->pushf();
	Label overrideCoords = a->newLabel();
	Label standardCoords = a->newLabel();
	Label exLab = a->newLabel();

	a->mov(edx, dword_ptr(esp, 0x68));
	a->mov(esi, dword_ptr(esp, 0x6c));
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->test(eax, eax);

	//if 0, when standard coords, else not
	a->jnz(overrideCoords);

	a->bind(standardCoords);

	a->jmp(exLab);

	a->bind(overrideCoords);

	a->mov(edx, dword_ptr(esp, 0x68));
	a->mov(eax, dword_ptr(esp, 0x6c));
	a->mov(dword_ptr(esp, 0x54), edx);
	a->mov(dword_ptr(esp, 0x58), eax);

	a->bind(exLab);

	a->popf();
	a->popad();
	a->mov(edx, dword_ptr(esp, 0x30));
	a->mov(eax, dword_ptr(esp, 0x34));



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toEndOfSpawnArmy::toEndOfSpawnArmy(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00a8d6db;

	else if (ver == 1)//kingdoms
		m_adress = 0x00a8c67b;
}

toEndOfSpawnArmy::~toEndOfSpawnArmy()
{
}

void toEndOfSpawnArmy::SetOriginalEndSpawnCode()
{
	Assembler* a = new Assembler();
	//mov edx,[esp+30]
	//mov eax, [esp + 34]

	a->add(eax, 0x1);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toEndOfSpawnArmy::SetlEndSpawnCode()
{
	Assembler* a = new Assembler();
	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();
	a->add(eax, 0x1);



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


toSpawnCharacter::toSpawnCharacter(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00a8b3a9;

	else if (ver == 1)//kingdoms
		m_adress = 0x00a8a349;
}

toSpawnCharacter::~toSpawnCharacter()
{

}

void toSpawnCharacter::SetOriginaSpawnCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, dword_ptr(esp, 0x1c));
	a->mov(ecx, dword_ptr(esp, 0x20));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toSpawnCharacter::SetlSpawnCode()
{
	Assembler* a = new Assembler();
	a->pushad();
	a->pushf();
	Label overrideCoords = a->newLabel();
	Label standardCoords = a->newLabel();
	Label exLab = a->newLabel();

	a->mov(edx, dword_ptr(esp, 0x58));
	a->mov(esi, dword_ptr(esp, 0x5C));
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

		a->test(eax, eax);

		//if 0, when standard coords, else not
		a->jnz(overrideCoords);

	a->bind(standardCoords);

	a->jmp(exLab);

	a->bind(overrideCoords);

	a->mov(edx, dword_ptr(esp, 0x58));
	a->mov(eax, dword_ptr(esp, 0x5C));
	a->mov(dword_ptr(esp, 0x40), edx);
	a->mov(dword_ptr(esp, 0x44), eax);

	a->bind(exLab);

	a->popf();
	a->popad();
	a->mov(eax, dword_ptr(esp, 0x1C));
	a->mov(ecx, dword_ptr(esp, 0x20));



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}



toBattleStratScreen::toBattleStratScreen(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0061b3a3;

	else if (ver == 1)//kingdoms
		m_adress = 0x0061b003;
}

toBattleStratScreen::~toBattleStratScreen()
{
}

void toBattleStratScreen::SetOriginaBSCode()
{
	Assembler* a = new Assembler();
	//mov ecx,00000001
	a->push(ebx);
	a->push(esi);
	a->push(edi);
	a->mov(esi, ecx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toBattleStratScreen::SetlBSCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->push(ebx);
	a->push(esi);
	a->push(edi);
	a->mov(esi, ecx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toStartReadCas::toStartReadCas(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00a04f15;

	else if (ver == 1)//kingdoms
		m_adress = 0x00a04395;
}

toStartReadCas::~toStartReadCas()
{
}

void toStartReadCas::SetOriginaStReadCasCode()
{
	Assembler* a = new Assembler();
	//mov ecx,00000001
	a->sub(esp, 0x10C);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStartReadCas::SetlStReadCasCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(edi, dword_ptr(esp, 0x34));
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->sub(esp, 0x10C);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toStartReadModels::toStartReadModels(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0044f460;

	else if (ver == 1)//kingdoms
		m_adress = 0x0044f120;
}

toStartReadModels::~toStartReadModels()
{
}

void toStartReadModels::SetOriginaStReadModelsCode()
{
	Assembler* a = new Assembler();

	a->push(esi);
	a->mov(esi, ecx);
	a->push(0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStartReadModels::SetlStReadModelsCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->push(esi);
	a->mov(esi, ecx);
	a->push(0);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toEndReadModels::toEndReadModels(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0044f741;

	else if (ver == 1)//kingdoms
		m_adress = 0x0044f401;
}

toEndReadModels::~toEndReadModels()
{
}

void toEndReadModels::SetOriginaEndReadModelsCode()
{
	Assembler* a = new Assembler();
	if (m_adress == 0x0044f741)
	{
		a->mov(eax, 0x00420710);
	}
	else {
		a->mov(eax, 0x004203e0);
	}
	a->call(eax);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toEndReadModels::SetlEndReadModelsCode()
{
	Assembler* a = new Assembler();

	if (m_adress == 0x0044f741)
	{
		a->mov(eax, 0x00420710);
	}
	else {
		a->mov(eax, 0x004203e0);
	}
	a->call(eax);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toResourcesReadModels::toResourcesReadModels(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008e432d;

	else if (ver == 1)//kingdoms
		m_adress = 0x008e38a9;
}

toResourcesReadModels::~toResourcesReadModels()
{
}

void toResourcesReadModels::SetOriginaResModelsCode()
{
	Assembler* a = new Assembler();

	a->add(eax, 0x1);
	a->cmp(eax, 0x1c);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toResourcesReadModels::SetlResModelsCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->add(eax, 0x1);
	a->cmp(eax, 0x1c);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


sizeOfBattleField::sizeOfBattleField(MemWork* mem, UINT32 gamever, float sizeX, float sizeY)
	:AATemplate(mem), sizeX(sizeX), sizeY(sizeY)
{
	if (gamever == 1)//disk ver
		m_adress = 0x00441335;
	else if (gamever == 2)//steam ver
		m_adress = 0x00441675;
}

sizeOfBattleField::~sizeOfBattleField()
{
}

void sizeOfBattleField::SetOriginalSizeCode()
{
	Assembler* a = new Assembler();
	a->mov(ecx, dword_ptr(eax, 0x00000080));
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void sizeOfBattleField::SetlSizeCode()
{
	Assembler* a = new Assembler();
	a->mov(dword_ptr(eax, 0x00000080), *(reinterpret_cast<int*>(&sizeX)));
	a->mov(dword_ptr(eax, 0x0000007C), *(reinterpret_cast<int*>(&sizeY)));


	a->mov(ecx, dword_ptr(eax, 0x00000080));
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toEvents::toEvents(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008969b0;

	else if (ver == 1)//kingdoms
		m_adress = 0x00895fb0;
}

toEvents::~toEvents()
{
}

void toEvents::SetOriginalEventsCode()
{
	Assembler* a = new Assembler();

	a->push(ebp);
	a->mov(ebp, esp);
	a->push(-0x1);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toEvents::SetlEventsCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, dword_ptr(esp, 0x4));
	a->pushad();
	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->push(ebp);
	a->mov(ebp, esp);
	a->push(-0x1);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toCustomTileSelection::toCustomTileSelection(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0049ede4;

	else if (ver == 1)//kingdoms
		m_adress = 0x0049e974;
}

toCustomTileSelection::~toCustomTileSelection()
{
}

void toCustomTileSelection::SetOriginalTilesCode()
{
	Assembler* a = new Assembler();

	a->mov(edi, dword_ptr(ebx));
	a->add(ecx, 0x0001A7C0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toCustomTileSelection::SetlTilesCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, ebx);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->mov(edi, dword_ptr(ebx));
	a->add(ecx, 0x0001A7C0);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toCustomTileFileRead::toCustomTileFileRead(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x004a1900;

	else if (ver == 1)//kingdoms
		m_adress = 0x004a1460;
}

toCustomTileFileRead::~toCustomTileFileRead()
{
}

void toCustomTileFileRead::SetOriginalTilesCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, dword_ptr(edi, 0x10));
	a->mov(eax, dword_ptr(edi, 0x8));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toCustomTileFileRead::SetlTilesCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->mov(eax, dword_ptr(edi, 0x10));
	a->cmp(eax, dword_ptr(edi, 0x8));

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toStratModelsSelect::toStratModelsSelect(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0044f861;

	else if (ver == 1)//kingdoms
		m_adress = 0x0044f521;
}

toStratModelsSelect::~toStratModelsSelect()
{
}

void toStratModelsSelect::SetOriginalStratModelsCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(eax,0x740), 0x0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStratModelsSelect::SetlStratModelsCode()
{
	Assembler* a = new Assembler();


	a->mov(byte_ptr(eax, 0x740), 0x0);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);

	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toLoadSaveFile::toLoadSaveFile(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x004347fe;

	else if (ver == 1)//kingdoms
		m_adress = 0x004344be;
}

toLoadSaveFile::~toLoadSaveFile()
{
}

void toLoadSaveFile::SetOriginalLoadCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, 0x3bc8);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toLoadSaveFile::SetlLoadCode()
{
	Assembler* a = new Assembler();


	a->pushad();
	a->pushf();

	a->mov(ecx, dword_ptr(esp, 0x34));
	a->mov(eax, (DWORD)funcAdress);

	a->call(eax);

	a->popf();
	a->popad();

	a->mov(eax, 0x3bc8);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toSaveGame::toSaveGame(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0043530a;

	else if (ver == 1)//kingdoms
		m_adress = 0x00434fca;
}

toSaveGame::~toSaveGame()
{
}

void toSaveGame::SetOriginalSaveCode()
{
	Assembler* a = new Assembler();

	a->add(esp, 0x3be4);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toSaveGame::SetlSaveCode()
{
	Assembler* a = new Assembler();

	a->add(esp, 0x3be4);

	a->pushad();
	a->pushf();

	a->mov(ecx, dword_ptr(esp, 0x28));
	a->mov(eax, (DWORD)funcAdress);

	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toStartNewGame::toStartNewGame(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0047e815;

	else if (ver == 1)//kingdoms
		m_adress = 0x0047e435;
}

toStartNewGame::~toStartNewGame()
{
}

void toStartNewGame::SetOriginalStartCode()
{
	Assembler* a = new Assembler();

	a->sub(esp, 0xbc);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStartNewGame::SetlStartCode()
{
	Assembler* a = new Assembler();

	a->sub(esp, 0xbc);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);

	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toLoadCas::toLoadCas(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{

	if (ver == 2)//steam
		m_adress = 0x00a04f00;

	else if (ver == 1)//kingdoms
		m_adress = 0x00a04380;
}

toLoadCas::~toLoadCas()
{
}

void toLoadCas::SetOriginalStartCode()
{
	//nothing xD
}

void toLoadCas::SetlCasCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();


	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);


	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toClickAtCoords::toClickAtCoords(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00c2e9a4;

	else if (ver == 1)//kingdoms
		m_adress = 0x00c345d4;
}

toClickAtCoords::~toClickAtCoords()
{
}

void toClickAtCoords::SetOriginalClickCode()
{
	Assembler* a = new Assembler();

	a->lea(eax,dword_ptr(esp,0x4));
	a->mov(esi, ecx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toClickAtCoords::SetlClickCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, dword_ptr(esp, 0x18));
	a->pushad();
	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->lea(eax, dword_ptr(esp, 0x4));
	a->mov(esi, ecx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toSelectWorldFromDB::toSelectWorldFromDB(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x010b8ab5;

	else if (ver == 1)//kingdoms
		m_adress = 0x010b82c5;
}

toSelectWorldFromDB::~toSelectWorldFromDB()
{
}

void toSelectWorldFromDB::SetOriginalSelectCode()
{
	Assembler* a = new Assembler();

	a->push(esi);
	a->lea(ecx, dword_ptr(esp,0x18));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toSelectWorldFromDB::SetlSelectCode()
{
	Assembler* a = new Assembler();
	Label overrideWorld = a->newLabel();
	Label standardWorld = a->newLabel();
	Label exLab = a->newLabel();

	a->mov(ecx, esi);
	a->pushad();
	a->pushf();

	a->mov(ecx, edi);//pointer to db
	a->mov(edx, esi);//selected record
	a->mov(eax, (DWORD)funcAdress);
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

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toBattleLoaded::toBattleLoaded(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00441f01;

	else if (ver == 1)//kingdoms
		m_adress = 0x00441bc1;
}

toBattleLoaded::~toBattleLoaded()
{
}

void toBattleLoaded::SetOriginalLoadCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, dword_ptr(esp, 0xb4));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toBattleLoaded::SetlLoadCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, dword_ptr(esp, 0xb4));
	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toReadGameDBsAtGameStart::toReadGameDBsAtGameStart(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x004122ec;

	else if (ver == 1)//kingdoms
		m_adress = 0x004120ec;
}

toReadGameDBsAtGameStart::~toReadGameDBsAtGameStart()
{
}

void toReadGameDBsAtGameStart::SetOriginalReadCode()
{
	Assembler* a = new Assembler();

	a->add(esp,0xdc);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toReadGameDBsAtGameStart::SetlReadCode()
{
	Assembler* a = new Assembler();

	a->add(esp, 0xdc);
	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toDrawPartsOfStratObjects::toDrawPartsOfStratObjects(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x009e2a00;

	else if (ver == 1)//kingdoms
		m_adress = 0x009e1e60;
}

toDrawPartsOfStratObjects::~toDrawPartsOfStratObjects()
{
}

void toDrawPartsOfStratObjects::SetOriginalDrawCode()
{
	Assembler* a = new Assembler();

	a->sub(esp, 0x10);
	a->push(ebx);
	a->push(ebp);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toDrawPartsOfStratObjects::SetlDrawCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->sub(esp, 0x10);
	a->push(ebx);
	a->push(ebp);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toEndSettlementSiege::toEndSettlementSiege(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x004bf8af;

	else if (ver == 1)//kingdoms
		m_adress = 0x004bf31f;
}

toEndSettlementSiege::~toEndSettlementSiege()
{
}

void toEndSettlementSiege::SetOriginalSiegeCode()
{
	Assembler* a = new Assembler();

	a->cmp(byte_ptr(esi, 0x88), bl);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toEndSettlementSiege::SetlSiegeCode()
{
	Assembler* a = new Assembler();

	a->cmp(byte_ptr(esi, 0x88), bl);

	a->pushad();
	a->pushf();

	a->mov(ecx, esi);//settlement
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toStartSettlementSiege::toStartSettlementSiege(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x004bf80d;

	else if (ver == 1)//kingdoms
		m_adress = 0x004bf27d;
}


toStartSettlementSiege::~toStartSettlementSiege()
{
}

void toStartSettlementSiege::SetOriginalSiegeCode()
{
	Assembler* a = new Assembler();

	a->mov(dword_ptr(esi, 0x4), eax);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStartSettlementSiege::SetlSiegeCode()
{
	Assembler* a = new Assembler();

	a->mov(dword_ptr(esi, 0x4), eax);

	a->pushad();
	a->pushf();

	a->mov(ecx, edi);//settlement
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toLoadDescrBattleCharacter::toLoadDescrBattleCharacter(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0047bb66;

	else if (ver == 1)//kingdoms
		m_adress = 0x0047b786;
}
toLoadDescrBattleCharacter::~toLoadDescrBattleCharacter()
{
}

void toLoadDescrBattleCharacter::SetOriginalBattleCode()
{

	Assembler* a = new Assembler();

	a->mov(eax,dword_ptr(ecx,0x4));
	a->push(eax);
	a->mov(ecx, esi);

	//call of set bodyguard here
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toLoadDescrBattleCharacter::SetlBattleCode()
{
	unsigned char nops[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	MemWork::WriteData(nops, m_adress+6, 5);

	Assembler* a = new Assembler();

	a->mov(eax, dword_ptr(ecx));
	//a->push(eax);
	a->mov(ecx, esi);


	a->pushad();
	a->pushf();

	a->mov(edx, ecx);//character
	a->mov(ecx, edi);//army
	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toBattleStateResults::toBattleStateResults(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00618f26;

	else if (ver == 1)//kingdoms
		m_adress = 0x00618b86;
}

toBattleStateResults::~toBattleStateResults()
{
}

void toBattleStateResults::SetOriginalBattleCode()
{
	Assembler* a = new Assembler();

	a->mov(dword_ptr(esi, 0x4), 0x9);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toBattleStateResults::SetlBattleCode()
{
	Assembler* a = new Assembler();

	a->mov(dword_ptr(esi, 0x4), 0x9);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

toGameFormDrawImage::toGameFormDrawImage(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008fbd6a;

	else if (ver == 1)//kingdoms
		m_adress = 0x008fb2da;
}

toGameFormDrawImage::~toGameFormDrawImage()
{
}

void toGameFormDrawImage::SetOriginalDrawCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(esp,0x18),0x0);


	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toGameFormDrawImage::SetlDrawCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(esp, 0x18), 0x0);


	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

CastleConversionLvlSetter::CastleConversionLvlSetter(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005E9304;
	else if (ver == 1)
		m_adress = 0x005E8EA4;
}

CastleConversionLvlSetter::~CastleConversionLvlSetter()
{

}

void CastleConversionLvlSetter::SetOriginalCode()
{
	Assembler* a = new Assembler();


	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	// Unfinished!
	delete a;
}

void CastleConversionLvlSetter::SetNewCode()
{
	unsigned char nops[9] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 , 0x90 , 0x90 , 0x90 };
	MemWork::WriteData(nops, m_adress + 0x5, 9);
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->mov(dword_ptr(esp, 0x1C), eax);
	a->popad();

	a->ret();

	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

CityConversionLvlSetter::CityConversionLvlSetter(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005E9344;
	else if (ver == 1)
		m_adress = 0x005E8EE4;
}

CityConversionLvlSetter::~CityConversionLvlSetter()
{

}

void CityConversionLvlSetter::SetOriginalCode()
{
	Assembler* a = new Assembler();


	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	// Unfinished!
	delete a;
}

void CityConversionLvlSetter::SetNewCode()
{
	unsigned char nops[15] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 , 0x90 , 0x90 , 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	MemWork::WriteData(nops, m_adress + 0x5, 15);
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->mov(dword_ptr(esp, 0x1C), eax);
	a->popad();

	a->ret();

	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

mercenaryMovepointsGetGeneral::mercenaryMovepointsGetGeneral(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0060bc93;
	else if (ver == 1)
		m_adress = 0x0060b8f3;
}

mercenaryMovepointsGetGeneral::~mercenaryMovepointsGetGeneral()
{
}

void mercenaryMovepointsGetGeneral::SetOriginalCode()
{
	Assembler* a = new Assembler();

	//call func

	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	// Unfinished!
	delete a;
}

void mercenaryMovepointsGetGeneral::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	//stack struct in ecx
	a->mov(eax, (DWORD)funcAddress);//here we must specify general or 0
	a->call(eax);
	//return general
	a->mov(dword_ptr(esp,0x20),eax);//move eax to stored eax

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGameConsoleCommandFromConsole::onGameConsoleCommandFromConsole(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00b025ec;

	else if (ver == 1)//kingdoms
		m_adress = 0x00b082bc;
}

onGameConsoleCommandFromConsole::~onGameConsoleCommandFromConsole()
{
}

void onGameConsoleCommandFromConsole::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(esp,0x60), 0x0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGameConsoleCommandFromConsole::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(esp, 0x60), 0x0);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGameConsoleCommandFromScript::onGameConsoleCommandFromScript(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00a903f5;

	else if (ver == 1)//kingdoms
		m_adress = 0x00a8f395;
}

onGameConsoleCommandFromScript::~onGameConsoleCommandFromScript()
{
}

void onGameConsoleCommandFromScript::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->lea(ecx,dword_ptr(esp, 0x18));
	a->push(ecx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGameConsoleCommandFromScript::SetNewCode()
{
	Assembler* a = new Assembler();

	a->lea(ecx, dword_ptr(esp, 0x18));
	a->push(ecx);

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnReadLogonOrLogoff::OnReadLogonOrLogoff(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00467e60;

	else if (ver == 1)//kingdoms
		m_adress = 0x00467ae0;
}

OnReadLogonOrLogoff::~OnReadLogonOrLogoff()
{
}

void OnReadLogonOrLogoff::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->mov(al, byte_ptr(ecx, 0x6f9));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnReadLogonOrLogoff::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(al, byte_ptr(ecx, 0x6f9));//is admin logged in

	a->pushad();
	a->pushf();

	a->xor_(ecx, ecx);
	a->mov(ecx, al);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->mov(byte_ptr(esp, 0x20), al);//move eax to stored eax

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnLoadSettlementWorldpkgdesc::OnLoadSettlementWorldpkgdesc(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x010b7ee5;

	else if (ver == 1)//kingdoms
		m_adress = 0x010b76f5;
}

OnLoadSettlementWorldpkgdesc::~OnLoadSettlementWorldpkgdesc()
{
}

void OnLoadSettlementWorldpkgdesc::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->sub(esp,  0x0C);
	a->push(ebp);
	a->push(esi);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnLoadSettlementWorldpkgdesc::SetNewCode()
{
	Assembler* a = new Assembler();

	a->sub(esp, 0x0C);
	a->push(ebp);
	a->push(esi);

	a->pushad();
	a->pushf();

	//our record in ecx
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

LimitRecruitmentQueueToSlotsInj::LimitRecruitmentQueueToSlotsInj(MemWork* mem, int* gameplaySettingAddr, int ver)
	:AATemplate(mem)
{
	if (ver == 2) //steam
		m_adress = 0x5E451F;
	else if (ver == 1)
		m_adress = 0x5E40AF;

	m_version = ver;
	settingAddr = gameplaySettingAddr;
}
LimitRecruitmentQueueToSlotsInj::~LimitRecruitmentQueueToSlotsInj(){}

void LimitRecruitmentQueueToSlotsInj::SetOriginialCode()
{
	Assembler* a = new Assembler();

	if (m_version == 2)
	{
		a->call(0xb461);		
	}
	else if (m_version == 1)
	{
		a->call(0xb4e1);
	}

	a->ret();

	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void LimitRecruitmentQueueToSlotsInj::SetNewCode()
{
	Assembler* a = new Assembler();

	Label noSlots = a->newLabel();
	Label end = a->newLabel();
	Label gameFunctionCall = a->newLabel();

	a->push(eax);

	a->mov(eax, (DWORD)settingAddr);
	a->cmp(ptr(eax), 0);
	a->jz(gameFunctionCall);

	a->xor_(eax, eax);
	a->mov(ax, ptr(esi, 0xF34)); //recruitmentSlots 
	a->test(ax,ax);
	a->jz(noSlots);
	a->add(ax, ptr(esi, 0xF36)); //recruitmentSlots bonus
	a->cmp(ax, ptr(esi, 0x320));
	a->jle(noSlots);
	a->mov(eax, ptr(esi, 0x170)); //settlement->faction
	a->mov(eax, ptr(eax, 0xAEC)); //faction->treasury
	a->push(edi);
	a->movzx(edi, word_ptr(edi, 0x16));
	a->cmp(eax, edi);
	a->pop(edi);
	a->jl(noSlots);

	a->bind(gameFunctionCall);
	a->pop(eax);
	if (m_version == 2)
		a->mov(eax, (DWORD)0x005EF980);
	else
		a->mov(eax, (DWORD)0x005EF590);

	a->call(eax);
	a->jmp(end);
	a->bind(noSlots);
	a->pop(eax);
	a->pop(eax);
	a->xor_(eax, eax);
	a->bind(end);
	a->ret();

	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

fortificationlevelS::fortificationlevelS(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005e3806;

	else if (ver == 1)//kingdoms
		m_adress = 0x005e3336;
}

fortificationlevelS::~fortificationlevelS()
{
}

void fortificationlevelS::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, dword_ptr(ecx, 0x198));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void fortificationlevelS::SetNewCode()
{
	static bool fakeSettlementTypePointer;

	Assembler* a = new Assembler();
	Label override = a->newLabel();
	Label standard = a->newLabel();
	Label exLab = a->newLabel();

	a->pushad();
	a->pushf();

	a->mov(edx, (int)&fakeSettlementTypePointer);

	//settlement in ecx now
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->cmp(eax, -2);

	a->jnz(override);

	a->bind(standard);
	a->popf();
	a->popad();
	a->mov(ecx, dword_ptr(ecx, 0x198));
	a->jmp(exLab);

	a->bind(override);
	a->popf();
	a->mov(dword_ptr(esp, 0x18), eax);//mov our eax to ecx
	a->popad();

	a->mov(eax, (int)&fakeSettlementTypePointer);
	a->mov(al,byte_ptr(eax));
	//a->mov(al, 0x0);
	a->bind(exLab);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnSaveEDUStringS::OnSaveEDUStringS(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00d47558;

	else if (ver == 1)//kingdoms
		m_adress = 0x00d4d1f8;
}

OnSaveEDUStringS::~OnSaveEDUStringS()
{
}

void OnSaveEDUStringS::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, dword_ptr(edi));
	a->test(ecx, ecx);
	a->mov(dword_ptr(esp,0xC), 0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnSaveEDUStringS::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(eax);
	a->push(edx);
	a->push(ebx);
	a->push(ebp);
	a->push(esi);
	a->push(edi);

	a->pushf();

	a->mov(ecx,edi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(ecx, eax);//here must be pointer to char* string  - type for writing to save file

	a->popf();

	a->pop(edi);
	a->pop(esi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(edx);
	a->pop(eax);

	a->test(ecx, ecx);
	a->mov(dword_ptr(esp, 0xC), 0);


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnSaveEDUStringS2::OnSaveEDUStringS2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00d475af;

	else if (ver == 1)//kingdoms
		m_adress = 0x00d4d24f;
}

OnSaveEDUStringS2::~OnSaveEDUStringS2()
{
}

void OnSaveEDUStringS2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(edx, dword_ptr(edi));
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, esi);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnSaveEDUStringS2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(eax);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(esi);
	a->push(edi);

	a->pushf();

	a->mov(ecx, edi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(edx, eax);//here must be pointer to char* string  - type for writing to save file

	a->popf();

	a->pop(edi);
	a->pop(esi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(eax);

	a->push(ecx);
	a->push(edx);
	a->mov(ecx, esi);


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnCreateUnit::OnCreateUnit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008ee8e6;

	else if (ver == 1)//kingdoms
		m_adress = 0x008ede66;
}

OnCreateUnit::~OnCreateUnit()
{
}

void OnCreateUnit::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
//jz

	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnCreateUnit::SetNewCode()
{
	static int fakeEDBPointer;
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->pushf();


	a->mov(ecx,(int)&fakeEDBPointer);
	a->mov(dword_ptr(ecx),esi);

	a->mov(edx, ecx);//here pointer to edb
	a->mov(ecx, eax);//here pointer to index in edb
	a->push(edi);//entry name
	a->mov(eax, (DWORD)funcAddress);//we must return index
	a->call(eax);

	//here in eax we must put -1 if not find
	a->mov(esi, (int)&fakeEDBPointer);
	a->mov(esi, dword_ptr(esi));
	a->popf();

	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(edx);





	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

//need to overwrite some code so it passes check with non standard edu index, and this way also flags it as eop unit for later
OnCreateMercUnitCheck::OnCreateMercUnitCheck(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0060D54B;

	else if (ver == 1)//kingdoms
		m_adress = 0x0060D1AB;
}

OnCreateMercUnitCheck::~OnCreateMercUnitCheck()
{
}

void OnCreateMercUnitCheck::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->cmp(eax, ecx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

//dont ask me what this does exactly been too long
void OnCreateMercUnitCheck::SetNewCode()
{
	Assembler* a = new Assembler();


	a->pushad();
	a->lea(ecx, dword_ptr(esp, 0x60));
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(ecx, -1);
	a->cmp(eax, ecx);
	a->popad();
	a->lea(ecx, dword_ptr(esp, 0x40));



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnCreateMercUnit::OnCreateMercUnit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0060D568;

	else if (ver == 1)//kingdoms
		m_adress = 0x0060D1C8;
}

OnCreateMercUnit::~OnCreateMercUnit()
{
}

void OnCreateMercUnit::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnCreateMercUnit::SetNewCode()
{
	DWORD otherFunc; //different funcs dependant on disk/steam version
	if (m_adress = 0x0060D568)
	{
		otherFunc = 0x00D45D40;
	}
	else
	{
		otherFunc = 0x00D4B9E0;
	}
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(dword_ptr(edi), eax);

	a->popf();
	a->popad();
	a->mov(eax, otherFunc);
	a->call(eax);


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnQuickSave::OnQuickSave(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00c30a83;

	else if (ver == 1)//kingdoms
		m_adress = 0x00c366b3;
}

OnQuickSave::~OnQuickSave()
{
}

void OnQuickSave::SetOriginialCode()
{
	Assembler* a = new Assembler();

	//push pointer to "%S.sav" string

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnQuickSave::SetNewCode()
{
	Assembler* a = new Assembler();


	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->mov(byte_ptr(esp, 0x20), eax);//move eax to stored eax
	a->popf();
	a->popad();

	a->push(eax);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnAutoSave::OnAutoSave(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0047cd72;

	else if (ver == 1)//kingdoms
		m_adress = 0x0047c992;
}

OnAutoSave::~OnAutoSave()
{
}

void OnAutoSave::SetOriginialCode()
{
	Assembler* a = new Assembler();

	//push pointer to "%s%S%s%S.sav" string

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnAutoSave::SetNewCode()
{
	Assembler* a = new Assembler();


	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->mov(byte_ptr(esp, 0x20), eax);//move eax to stored eax
	a->popf();
	a->popad();

	a->push(eax);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

blockLaunchWithoutEop::blockLaunchWithoutEop(MemWork* mem, int ver)
	:AATemplate(mem)
{
	if (ver == 2)//steam
		m_adress = 0x008efe32;

	else if (ver == 1)//kingdoms
		m_adress = 0x008ef3b2;
}

blockLaunchWithoutEop::~blockLaunchWithoutEop()
{
}

void blockLaunchWithoutEop::SetOriginialCode()
{
	Assembler* a = new Assembler();

	//push pointer to "type" string

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void blockLaunchWithoutEop::SetNewCode()
{
	static char newTypeString[] = "eopOnlyType";
	Assembler* a = new Assembler();

	a->mov(ecx, (int)newTypeString);
	a->push(ecx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

/*
OntryFindTypeIdInListRecruitPoolEDB::OntryFindTypeIdInListRecruitPoolEDB(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0046e0e8;

	else if (ver == 1)//kingdoms
		m_adress = 0x0046dd28;
}

OntryFindTypeIdInListRecruitPoolEDB::~OntryFindTypeIdInListRecruitPoolEDB()
{
}

void OntryFindTypeIdInListRecruitPoolEDB::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->pop(edi);
	a->pop(esi);
	a->pop(ebp);
	a->xor_(eax,eax);
	//push pointer to "%S.sav" string

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OntryFindTypeIdInListRecruitPoolEDB::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, ebp);//just to indicate - string pointer in ebp
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->mov(dword_ptr(esp, 0x1c), eax);//move eax to stored eax
	a->popad();

	a->pop(edi);
	a->pop(esi);
	a->pop(ebp);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnrecruitPoolFillFromFile::OnrecruitPoolFillFromFile(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008a9ce5;

	else if (ver == 1)//kingdoms
		m_adress = 0x008a9305;
}

OnrecruitPoolFillFromFile::~OnrecruitPoolFillFromFile()
{
}

void OnrecruitPoolFillFromFile::SetOriginialCode()
{
	Assembler* a = new Assembler();
	a->xor_(eax, eax);
	//        008a9ce5 8d  0c  85       LEA        this ,[typeId *0x4  + eduEntries ] 

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnrecruitPoolFillFromFile::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, edi);//just to indicate - string pointer in ecx
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->mov(dword_ptr(esp, 0x18), eax);//move eax to stored ecx
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnrecruitPoolFillFromFile2::OnrecruitPoolFillFromFile2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008a9c19;

	else if (ver == 1)//kingdoms
		m_adress = 0x008a9239;
}

OnrecruitPoolFillFromFile2::~OnrecruitPoolFillFromFile2()
{
}

void OnrecruitPoolFillFromFile2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	//        008a9c19 8d  14  8d       LEA        EDX ,[this *0x4  + eduEntries ]
	a->xor_(eax, eax);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnrecruitPoolFillFromFile2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, edi);//just to indicate - string pointer in ecx
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->mov(dword_ptr(esp, 0x14), eax);//move eax to stored edx
	a->popad();


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}
*/

OnStopCharacter::OnStopCharacter(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00ab51a5;

	else if (ver == 1)//kingdoms
		m_adress = 0x00ab4175;
}

OnStopCharacter::~OnStopCharacter()
{
}

void OnStopCharacter::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(eax, 0xD0), 01);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnStopCharacter::SetNewCode()
{
	Assembler* a = new Assembler();


	a->pushad();
	a->pushf();

	//character here
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnMoveRecruitQueue::OnMoveRecruitQueue(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00ab4966;
	else if (ver == 1)//kingdoms
		m_adress = 0x00ab3936;
}
OnMoveRecruitQueue::~OnMoveRecruitQueue()
{
}
void OnMoveRecruitQueue::SetOriginialCode()
{
	Assembler* a = new Assembler();
	
	a->mov(byte_ptr(eax, 0xD0), 01);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	delete a;
}
void OnMoveRecruitQueue::SetNewCode()
{
	Assembler* a = new Assembler();
	a->pushad();
	a->pushf();
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(byte_ptr(esp, 0x20), eax);//move eax to stored eax
	a->popf();
	a->popad();
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();
	delete a;
}
OnPathCasheCrashPlace::OnPathCasheCrashPlace(MemWork* mem, LPVOID addr, int ver, LPVOID cbObj)
	:AATemplate(mem), funcAddress(addr),callbackObject(cbObj)
{
	if (ver == 2)//steam
		m_adress = 0x0043cbae;
	else if (ver == 1)//kingdoms
		m_adress = 0x0043c86e;
}
OnPathCasheCrashPlace::~OnPathCasheCrashPlace()
{
}
void OnPathCasheCrashPlace::SetOriginalCode()
{
	return;
	Assembler* a = new Assembler();
	//filler, not work!
	a->mov(ecx,ecx);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	delete a;
}
void OnPathCasheCrashPlace::SetNewCode()
{
	Assembler* a = new Assembler();
	a->pushad();
	a->pushf();
	a->mov(ecx, (int)callbackObject);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->popf();
	a->mov(dword_ptr(esp, 0x18), eax);//move eax to stored ecx
	a->popad();
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();
	delete a;
}

//this function allows recruiting eop unit in rq but it is not really functional, as it doesnt work with saving, will not be bound to lua, leaving it here for future maybe
recruitEOPunit::recruitEOPunit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EEEA0;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EEAC1;
}

recruitEOPunit::~recruitEOPunit()
{
}

void recruitEOPunit::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->call(m_adress);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void recruitEOPunit::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

recruitEOPMercunit::recruitEOPMercunit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0060BC67;

	else if (ver == 1)//kingdoms
		m_adress = 0x0060BB5C;
}

recruitEOPMercunit::~recruitEOPMercunit()
{
}

void recruitEOPMercunit::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->call(m_adress);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void recruitEOPMercunit::SetNewCode()
{	
	Assembler* a = new Assembler();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->add(esp, 0x4);

	a->ret();

	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}
