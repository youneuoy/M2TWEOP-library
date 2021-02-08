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

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	if (m_adress == 0x0044f741)
	{
		a->mov(eax, 0x00420710);
	}
	else {
		a->mov(eax, 0x004203e0);
	}
	a->call(eax);

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
		m_adress = 0x0046c44f;

	else if (ver == 1)//kingdoms
		m_adress = 0x0046c08f;
}

toStratModelsSelect::~toStratModelsSelect()
{
}

void toStratModelsSelect::SetOriginalStratModelsCode()
{
	Assembler* a = new Assembler();

	a->lea(edi, dword_ptr(ebx, 0x228));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStratModelsSelect::SetlStratModelsCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->lea(edi, dword_ptr(ebx, 0x228));

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}
