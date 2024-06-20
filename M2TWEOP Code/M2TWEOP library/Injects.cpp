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



void fillWithNOP(DWORD address, size_t size) {
	DWORD oldProtect;
	VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	memset((LPVOID)address, 0x90, size); // 0x90 is the opcode for NOP in x86/x64

	VirtualProtect((LPVOID)address, size, oldProtect, &oldProtect);
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

toCharReadModels::toCharReadModels(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008B3D7F;

	else if (ver == 1)//kingdoms
		m_adress = 0x008B338F;
}

toCharReadModels::~toCharReadModels()
{
}

void toCharReadModels::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->push(ebx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toCharReadModels::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAdress);
	a->call(eax);

	a->popf();
	a->popad();

	a->push(ebx);
	a->push(ebp);
	a->push(esi);
	a->push(edi);

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

	a->pushad();
	a->pushf();
	a->mov(eax, dword_ptr(esp, 0x28));
	a->mov(edx, dword_ptr(esp, 0x2C));

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

	a->mov(byte_ptr(eax, 0x740), 0x0);

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

	a->lea(eax, dword_ptr(esp, 0x4));
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
	a->lea(ecx, dword_ptr(esp, 0x18));

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

	a->add(esp, 0xdc);

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

	a->mov(eax, dword_ptr(ecx, 0x4));
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
	MemWork::WriteData(nops, m_adress + 6, 5);

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

	a->mov(byte_ptr(esp, 0x18), 0x0);


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
	a->mov(dword_ptr(esp, 0x20), eax);//move eax to stored eax

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

	a->mov(byte_ptr(esp, 0x60), 0x0);

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

onGameInitialized::onGameInitialized(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00414E08;

	else if (ver == 1)//kingdoms
		m_adress = 0x00414C08;
}

onGameInitialized::~onGameInitialized()
{
}

void onGameInitialized::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->add(esp, 0x4);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGameInitialized::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->popad();
	a->add(esp, 0x4);
	a->push(0x1);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onUnloadCampaign::onUnloadCampaign(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00473338;

	else if (ver == 1)//kingdoms
		m_adress = 0x00472F78;
}

onUnloadCampaign::~onUnloadCampaign()
{
}

void onUnloadCampaign::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->mov(esi, ecx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onUnloadCampaign::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	a->popad();
	a->mov(esi, ecx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onAddCultureToArray::onAddCultureToArray(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008BD1D0;

	else if (ver == 1)//kingdoms
		m_adress = 0x008BC7E0;
}

onAddCultureToArray::~onAddCultureToArray()
{
}

void onAddCultureToArray::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onAddCultureToArray::SetNewCode()
{
	Assembler* a = new Assembler();
	

	a->push(edi);
	if (m_adress == 0x008BD1D0)
		a->mov(edi, 0x0161F0F8);
	else
		a->mov(edi, 0x01666FD0);
	a->cmp(dword_ptr(edi, 0), 31);
	a->pop(edi);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onParseBuildingPics::onParseBuildingPics(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008AC71F;

	else if (ver == 1)//kingdoms
		m_adress = 0x008ABD37;
}

onParseBuildingPics::~onParseBuildingPics()
{
}

void onParseBuildingPics::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onParseBuildingPics::SetNewCode()
{
	Assembler* a = new Assembler();

	a->cmp(ebx, 7);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onParseBuildingLevelPics::onParseBuildingLevelPics(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008A74C4;

	else if (ver == 1)//kingdoms
		m_adress = 0x008A6AE4;
}

onParseBuildingLevelPics::~onParseBuildingLevelPics()
{
}

void onParseBuildingLevelPics::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onParseBuildingLevelPics::SetNewCode()
{
	Assembler* a = new Assembler();

	a->cmp(eax, 7);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onParseEventBodies::onParseEventBodies(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008D068A;

	else if (ver == 1)//kingdoms
		m_adress = 0x008CFC6A;
}

onParseEventBodies::~onParseEventBodies()
{
}

void onParseEventBodies::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onParseEventBodies::SetNewCode()
{
	Assembler* a = new Assembler();

	a->cmp(ebp, 7);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onCreatePortraitDb::onCreatePortraitDb(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008B5173;

	else if (ver == 1)//kingdoms
		m_adress = 0x008B4783;
}

onCreatePortraitDb::~onCreatePortraitDb()
{
}

void onCreatePortraitDb::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onCreatePortraitDb::SetNewCode()
{
	//auto cultureDb = reinterpret_cast<culturesDB*>(dataOffsets::offsets.cultureDatabase);
	//int cultureCount = cultureDb->culturesCount;
	//if (cultureCount > 7)
	//	cultureCount = 7;
	
	Assembler* a = new Assembler();
	a->cmp(ebx, 7);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetCultureEndTurnSound::onGetCultureEndTurnSound(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00A5C7B2;

	else if (ver == 1)//kingdoms
		m_adress = 0x00A5B782;
}

onGetCultureEndTurnSound::~onGetCultureEndTurnSound()
{
}

void onGetCultureEndTurnSound::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, 0x0162854C);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetCultureEndTurnSound::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetBrowserPicConstructed::onGetBrowserPicConstructed(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x008A472D;
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x008A3D3B;
	}
}

onGetBrowserPicConstructed::~onGetBrowserPicConstructed()
{
}

void onGetBrowserPicConstructed::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBrowserPicConstructed::SetNewCode()
{
	Assembler* a = new Assembler();
	

	if (m_adress == 0x008A472D)
	{
		a->push(dword_ptr(esp,0));
	    a->mov(edx, ebx);
	    a->mov(ecx, eax);
		a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
		a->call(eax);
		a->ret();
	}
	else
	{
		a->push(dword_ptr(esp,0));
		a->mov(edx, dword_ptr(esp,8));
	    a->mov(ecx, eax);
		a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
		a->call(eax);
		a->ret();
	}
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetBrowserPicConstruction::onGetBrowserPicConstruction(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x008A474B;
		//fillWithNOP(0x008A472F, 0x4);
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x008A3D55;
	}
}

onGetBrowserPicConstruction::~onGetBrowserPicConstruction()
{
}

void onGetBrowserPicConstruction::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBrowserPicConstruction::SetNewCode()
{
	const auto a = new Assembler();

	if (m_adress == 0x008A474B)
	{
		a->push(dword_ptr(esp,0));
	    a->mov(edx, ebx);
		a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
		a->call(eax);
	    a->ret();
	}
	else
	{
		a->push(dword_ptr(esp,0));
		a->mov(edx, dword_ptr(esp,8));
	    a->mov(ecx, ebx);
		a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
		a->call(eax);
		a->ret();
	}
	
	m_cheatBytes = static_cast<unsigned char*>(a->make());

	delete a;
}

onGetBuildingPic::onGetBuildingPic(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005FCB93;
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005FC7A3;
	}
}

onGetBuildingPic::~onGetBuildingPic()
{
}

void onGetBuildingPic::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBuildingPic::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetBuildingPic2::onGetBuildingPic2(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005FC90D;
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005FC51D;
	}
}

onGetBuildingPic2::~onGetBuildingPic2()
{
}

void onGetBuildingPic2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBuildingPic2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetGuildOfferPic::onGetGuildOfferPic(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x00BB3F4C;
		//fillWithNOP(0x008A472F, 0x4);
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x0BB9BDC;
	}
}

onGetGuildOfferPic::~onGetGuildOfferPic()
{
}

void onGetGuildOfferPic::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetGuildOfferPic::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(ebp, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetBuildingPicConstruction::onGetBuildingPicConstruction(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005FCC13;
		//fillWithNOP(0x008A472F, 0x4);
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005FC823;
	}
}

onGetBuildingPicConstruction::~onGetBuildingPicConstruction()
{
}

void onGetBuildingPicConstruction::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBuildingPicConstruction::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetBuildingPicConstructed::onGetBuildingPicConstructed(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005FCBD3;
		//fillWithNOP(0x008A472F, 0x4);
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005FC7E3;
	}
}

onGetBuildingPicConstructed::~onGetBuildingPicConstructed()
{
}

void onGetBuildingPicConstructed::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBuildingPicConstructed::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetBrowserPicConstructed2::onGetBrowserPicConstructed2(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005FC93D;
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005FC54D;
	}
}

onGetBrowserPicConstructed2::~onGetBrowserPicConstructed2()
{
}

void onGetBrowserPicConstructed2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetBrowserPicConstructed2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetPortraitDbEntry::onGetPortraitDbEntry(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x008B4F7A;
	}
	

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x008B458A;
	}
}

onGetPortraitDbEntry::~onGetPortraitDbEntry()
{
}

void onGetPortraitDbEntry::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(dword_ptr(edi, 0), 7);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetPortraitDbEntry::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(ebx, eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetUnitByLabel::onGetUnitByLabel(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x00A9560A;
		otherFunc = 0x00A927E0;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x00A945AA;
		otherFunc = 0x00A91780;
	}
}

onGetUnitByLabel::~onGetUnitByLabel()
{
}

void onGetUnitByLabel::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(0xA927E0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetUnitByLabel::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pop(eax);
	a->push(ebx);
	a->push(edx);
	a->push(ecx);
	a->push(edi);
	a->push(esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(esi);
	a->pop(edi);
	a->pop(ecx);
	a->pop(edx);
	a->pop(ebx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onAssignGateAttacker::onAssignGateAttacker(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x006C87AE;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x006C827E;
	}
}

onAssignGateAttacker::~onAssignGateAttacker()
{
}

void onAssignGateAttacker::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(dword_ptr(eax, 0x31C));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onAssignGateAttacker::SetNewCode()
{
	Assembler* a = new Assembler();

	a->call(dword_ptr(eax, 0x320));

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onAttackGate2::onAttackGate2(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x006C8BF2;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x006C86C2;
	}
}

onAttackGate2::~onAttackGate2()
{
}

void onAttackGate2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(dword_ptr(eax, 0x31C));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onAttackGate2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, 1);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onAttackGate::onAttackGate(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x006C8E22;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x006C88F2;
	}
}

onAttackGate::~onAttackGate()
{
}

void onAttackGate::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(dword_ptr(eax, 0x31C));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onAttackGate::SetNewCode()
{
	Assembler* a = new Assembler();
	auto failure = a->newLabel();

	a->cmp(ebx, 0);
	a->jnz(failure);
	a->push(eax);
	a->push(ecx);
	a->mov(edx, esi);
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(eax);
	a->bind(failure);
	a->mov(edx, dword_ptr(edi, 0x294));
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onGetGroupByLabel::onGetGroupByLabel(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x00A9562A;
		otherFunc = 0x00A927E0;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x00A945CA;
		otherFunc = 0x00A91780;
	}
}

onGetGroupByLabel::~onGetGroupByLabel()
{
}

void onGetGroupByLabel::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(0xA927E0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onGetGroupByLabel::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pop(eax);
	a->push(ebx);
	a->push(edx);
	a->push(ecx);
	a->push(edi);
	a->push(esi);
	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(esi);
	a->pop(edi);
	a->pop(ecx);
	a->pop(edx);
	a->pop(ebx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onSearchUnitType::onSearchUnitType(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0046DF88;

	else if (ver == 1)//kingdoms
		m_adress = 0x0046DBC8;
}

onSearchUnitType::~onSearchUnitType()
{
}

void onSearchUnitType::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSearchUnitType::SetNewCode()
{
	Assembler* a = new Assembler();

	Label notEop = a->newLabel();
	
	a->mov(dword_ptr(esp, 0x14), esi);
	a->mov(ebp, edx);       // Move EDX to EBP
	a->shl(ebp, 3);         // Multiply EDX by 8, storing result in EBP
	a->add(ebp, esi);       // Add ESI to EBP
	a->push(ecx);
	
	a->push(ebp);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ebp);
	
	a->cmp(eax, 0);
	a->je(notEop);
	a->mov(ebp, eax);
	a->mov(dword_ptr(esp, 0x18), ebp);
	a->mov(ebx, 0x0);
	a->bind(notEop);
	//a->mov(esi, dword_ptr(ebp, 0x0));
	a->pop(ecx);
	a->ret();
	
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onReadBuildingPool::onReadBuildingPool(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x008A9CE5;

	else if (ver == 1)//kingdoms
		m_adress = 0x008A9305;
}

onReadBuildingPool::~onReadBuildingPool()
{
}

void onReadBuildingPool::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onReadBuildingPool::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(eax);
	a->mov(ecx, edi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(ecx, eax);
	a->pop(eax);
	
	a->ret();
	
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onReadMercenaryPool::onReadMercenaryPool(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAddress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x0060CBE4;

	else if (ver == 1)//kingdoms
		m_adress = 0x0060C844;
}

onReadMercenaryPool::~onReadMercenaryPool()
{
}

void onReadMercenaryPool::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onReadMercenaryPool::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->mov(ecx, edi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	
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

	a->lea(ecx, dword_ptr(esp, 0x18));
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

	a->sub(esp, 0x0C);
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

OnCalculateUnitValue::OnCalculateUnitValue(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x0045B14B;
		otherFunc = 0x0045ABE0;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x0045ADCB;
		otherFunc = 0x0045A860;
	}
}

OnCalculateUnitValue::~OnCalculateUnitValue()
{
}

void OnCalculateUnitValue::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(0x0045B1B6);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnCalculateUnitValue::SetNewCode()
{
	const auto a = new Assembler();

	//store the edu entry in esi as it remains unaltered throughout the original
	a->mov(esi, ptr(esp));
	//call original function
	a->mov(eax, otherFunc);
	a->call(eax);
	//original function stored result in st(0)
	
	a->pushad();
	a->pushf();
	
	//move edu entry into ecx for fastcall first argument
	a->mov(ecx, esi);
	
	//get value from float stack and put in edx, we will reinterpret cast as float in function
	a->sub(esp, 0x4);
	a->fstp(dword_ptr(esp));
	a->mov(edx, dword_ptr(esp));
	a->add(esp, 0x4);
	
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());

	delete a;
}

OnCalculateUnitValue2::OnCalculateUnitValue2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x0045B1B6;
		otherFunc = 0x0045ABE0;
	}

	else if (ver == 1)//kingdoms
	{
		m_adress = 0x0045AE36;
		otherFunc = 0x0045A860;
	}
}

OnCalculateUnitValue2::~OnCalculateUnitValue2()
{
}

void OnCalculateUnitValue2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->call(0x0045B1B6);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnCalculateUnitValue2::SetNewCode()
{
	const auto a = new Assembler();

	//store the edu entry in esi as it remains unaltered throughout the original
	a->mov(esi, ptr(esp));
	//call original function
	a->mov(eax, otherFunc);
	a->call(eax);
	//original function stored result in st(0)
	
	a->pushad();
	a->pushf();
	
	//move edu entry into ecx for fastcall first argument
	a->mov(ecx, esi);
	
	//get value from float stack and put in edx, we will reinterpret cast as float in function
	a->sub(esp, 0x4);
	a->fstp(dword_ptr(esp));
	a->mov(edx, dword_ptr(esp));
	a->add(esp, 0x4);
	
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);

	a->popf();
	a->popad();


	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());

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
LimitRecruitmentQueueToSlotsInj::~LimitRecruitmentQueueToSlotsInj() {}

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
	a->test(ax, ax);
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
	a->mov(al, byte_ptr(eax));
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
	a->mov(dword_ptr(esp, 0xC), 0);

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

	a->mov(ecx, edi);
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

onEvaluateUnit::onEvaluateUnit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0052F52F;

	else if (ver == 1)//kingdoms
		m_adress = 0x0052EF2F;
}

onEvaluateUnit::~onEvaluateUnit()
{
}

void onEvaluateUnit::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, 0x018C2BF0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onEvaluateUnit::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, eax);

	a->popf();
	a->popad();

	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());

	delete a;
}

onEvaluateUnit2::onEvaluateUnit2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0052F53D;

	else if (ver == 1)//kingdoms
		m_adress = 0x0052EF3D;
}

onEvaluateUnit2::~onEvaluateUnit2()
{
}

void onEvaluateUnit2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, 0x018C2BF0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onEvaluateUnit2::SetNewCode()
{
	Assembler* a = new Assembler();
	
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);

	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());

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
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);
	a->push(esi);

	a->pushf();

	a->mov(edx, eax);
	a->mov(ecx, edi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();
	
	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(edx);





	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnCreateUnit2::OnCreateUnit2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008EEB10;

	else if (ver == 1)//kingdoms
		m_adress = 0x008EE090;
}

OnCreateUnit2::~OnCreateUnit2()
{
}

void OnCreateUnit2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnCreateUnit2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, ebx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onReadDescrStrat::onReadDescrStrat(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x047A700;

	else if (ver == 1)//kingdoms
		m_adress = 0x0047A320;
}

onReadDescrStrat::~onReadDescrStrat()
{
}

void onReadDescrStrat::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onReadDescrStrat::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edi, eax);
	a->add(eax, 0x3AF);
	a->test(eax, 0x20);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onNewGameLoaded::onNewGameLoaded(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0047F9BD;

	else if (ver == 1)//kingdoms
		m_adress = 0x0047A320;
}

onNewGameLoaded::~onNewGameLoaded()
{
}

void onNewGameLoaded::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onNewGameLoaded::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(dword_ptr(esi, 0x24), 0);
	a->pushf();
	a->pushad();
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->popad();
	a->popf();
	
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onCustomBattleCost::onCustomBattleCost(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00632848;

	else if (ver == 1)//kingdoms
		m_adress = 0x006324A8;
}

onCustomBattleCost::~onCustomBattleCost()
{
}

void onCustomBattleCost::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onCustomBattleCost::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(eax);
	a->mov(ecx, dword_ptr(esp, 0x8));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, eax);
	a->pop(eax);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onSpawnBrigands::onSpawnBrigands(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x004D2B08;

	else if (ver == 1)//kingdoms
		m_adress = 0x004D24F3;
}

onSpawnBrigands::~onSpawnBrigands()
{
}

void onSpawnBrigands::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSpawnBrigands::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(esi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onSpawnBrigands2::onSpawnBrigands2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x004D2AA3;

	else if (ver == 1)//kingdoms
		m_adress = 0x004D2558;
}

onSpawnBrigands2::~onSpawnBrigands2()
{
}

void onSpawnBrigands2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSpawnBrigands2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(esi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

spawnUnitsSettGift::spawnUnitsSettGift(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0071A8EC;

	else if (ver == 1)//kingdoms
		m_adress = 0x0071A1BC;
}

spawnUnitsSettGift::~spawnUnitsSettGift()
{
}

void spawnUnitsSettGift::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void spawnUnitsSettGift::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(esi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

spawnUnitsSettGift2::spawnUnitsSettGift2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0071AA91;

	else if (ver == 1)//kingdoms
		m_adress = 0x0071A361;
}

spawnUnitsSettGift2::~spawnUnitsSettGift2()
{
}

void spawnUnitsSettGift2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void spawnUnitsSettGift2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(esi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


spawnUnitsSettGift3::spawnUnitsSettGift3(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0071ACB0;

	else if (ver == 1)//kingdoms
		m_adress = 0x0071A580;
}

spawnUnitsSettGift3::~spawnUnitsSettGift3()
{
}

void spawnUnitsSettGift3::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void spawnUnitsSettGift3::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(esi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


spawnUnitsSettGift4::spawnUnitsSettGift4(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0071AE66;

	else if (ver == 1)//kingdoms
		m_adress = 0x0071A736;
}

spawnUnitsSettGift4::~spawnUnitsSettGift4()
{
}

void spawnUnitsSettGift4::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void spawnUnitsSettGift4::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(esi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


onReadDescrRebel::onReadDescrRebel(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008E2692;

	else if (ver == 1)//kingdoms
		m_adress = 0x008E1C12;
}

onReadDescrRebel::~onReadDescrRebel()
{
}

void onReadDescrRebel::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onReadDescrRebel::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->push(eax);
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edi, eax);
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onCustomBattleUnitCards::onCustomBattleUnitCards(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00C7D729;

	else if (ver == 1)//kingdoms
		m_adress = 0x00C83339;
}

onCustomBattleUnitCards::~onCustomBattleUnitCards()
{
}

void onCustomBattleUnitCards::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onCustomBattleUnitCards::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(edx, ebx);
	a->lea(ecx, dword_ptr(esp, 0x14));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->push(eax);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onCustomBattleUnits::onCustomBattleUnits(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00CBB786;

	else if (ver == 1)//kingdoms
		m_adress = 0x00CC1256;
}

onCustomBattleUnits::~onCustomBattleUnits()
{
}

void onCustomBattleUnits::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->test(eax, eax);
	a->push(edx);
	a->mov(eax, dword_ptr(eax));

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onCustomBattleUnits::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->mov(edx, ebx);
	a->mov(ecx, dword_ptr(esp, 0x18));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ebx, eax);
	a->mov(dword_ptr(esp, 0x14), ebx);
	a->test(ebx, ebx);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}



OnFindUnit::OnFindUnit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0047543A;

	else if (ver == 1)//kingdoms
		m_adress = 0x0047504A;
}

OnFindUnit::~OnFindUnit()
{
}

void OnFindUnit::SetOriginialCode()
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

void OnFindUnit::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	a->mov(ecx, dword_ptr(esp, 0x8));

	a->push(edx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);
	a->push(esi);

	a->pushf();

	a->mov(edx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();

	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(edx);
	a->pop(ecx);




	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnUnitInfo::OnUnitInfo(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00BB4C8B;

	else if (ver == 1)//kingdoms
		m_adress = 0x00BBA91B;
}

OnUnitInfo::~OnUnitInfo()
{
}

void OnUnitInfo::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->push(ecx);
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnUnitInfo::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(eax);
	a->push(ebx);
	a->push(ecx);
	a->push(esi);
	a->push(edi);
	a->push(ebp);
	a->pushf();

	a->mov(ecx,edx);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(edx, eax);

	a->popf();
	a->pop(ebp);
	a->pop(edi);
	a->pop(esi);
	a->pop(ecx);
	a->pop(ebx);
	a->pop(eax);

	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(ecx, ebx);


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnReligionCombatBonus::OnReligionCombatBonus(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005A65EF;

	else if (ver == 1)//kingdoms
		m_adress = 0x005A610F;
}

OnReligionCombatBonus::~OnReligionCombatBonus()
{
}

void OnReligionCombatBonus::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, dword_ptr(edi, eax, 4, 0x118));
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnReligionCombatBonus::SetNewCode()
{
	Assembler* a = new Assembler();


	a->push(ebp);
	a->push(ebx);
	a->push(ecx);
	a->push(edx);
	a->push(esi);
	a->push(edi);

	a->mov(edx, edi);
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->pop(edi);
	a->pop(esi);
	a->pop(edx);
	a->pop(ecx);
	a->pop(ebx);
	a->pop(ebp);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnBattleArmiesBug::OnBattleArmiesBug(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0062218A;

	else if (ver == 1)//kingdoms
		m_adress = 0x00621DCA;
}

OnBattleArmiesBug::~OnBattleArmiesBug()
{
}

void OnBattleArmiesBug::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->call(0x0046D580);
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnBattleArmiesBug::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pop(eax);

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


onAiTurn::onAiTurn(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x0052B074;
		otherFunc = 0x005D2F00;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x0052AA74;
		otherFunc = 0x005D2A70;
	}
}

onAiTurn::~onAiTurn()
{
}

void onAiTurn::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->cmp(eax, ecx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onAiTurn::SetNewCode()
{
	Assembler* a = new Assembler();


	a->pushad();
	a->pushf();

	a->mov(ecx, esi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->popf();
	a->popad();
	a->mov(ecx, dword_ptr(esi, 0x24));
	a->mov(eax, (DWORD)otherFunc);
	a->call(eax);


	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnCreateMercUnit::OnCreateMercUnit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x0060D568;
		otherFunc = 0x00D45D40;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x0060D1C8;
		otherFunc = 0x00D4B9E0;
	}
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

OnGetRecruitPoolUnitEntry::OnGetRecruitPoolUnitEntry(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005E61A6;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005E5D46;
	}
}

OnGetRecruitPoolUnitEntry::~OnGetRecruitPoolUnitEntry()
{
}

void OnGetRecruitPoolUnitEntry::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}
void OnGetRecruitPoolUnitEntry::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->lea(esi, dword_ptr(eax));

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



loadRecruitQueue::loadRecruitQueue(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005D82F5;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005D7E65;
	}
}

loadRecruitQueue::~loadRecruitQueue()
{
}

void loadRecruitQueue::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}
void loadRecruitQueue::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);
	a->push(esi);

	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();

	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(edx);



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


loadRecruitQueue2::loadRecruitQueue2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005D835D;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005D7ECD;
	}
}

loadRecruitQueue2::~loadRecruitQueue2()
{
}

void loadRecruitQueue2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}
void loadRecruitQueue2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);
	a->push(esi);

	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();

	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(edx);



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnGetRecruitPoolUnitEntry2::OnGetRecruitPoolUnitEntry2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x00AB92A8;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x00AB8278;
	}
}

OnGetRecruitPoolUnitEntry2::~OnGetRecruitPoolUnitEntry2()
{
}

void OnGetRecruitPoolUnitEntry2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}
void OnGetRecruitPoolUnitEntry2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

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

OnFindUnitStrings::OnFindUnitStrings(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005F6412;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005F6022;
	}
}

OnFindUnitStrings::~OnFindUnitStrings()
{
}


void OnFindUnitStrings::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnFindUnitStrings::SetNewCode()
{
	Assembler* a = new Assembler();

	a->push(esi);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->pushf();

	a->mov(edx, ecx);
	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->mov(edx, dword_ptr(eax, 0x30));

	a->popf();

	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(esi);



	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnFindUnitStrings2::OnFindUnitStrings2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005F6419;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005F6029;
	}
}

OnFindUnitStrings2::~OnFindUnitStrings2()
{
}


void OnFindUnitStrings2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnFindUnitStrings2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onBuildingUnitAttributes::onBuildingUnitAttributes(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x005F789F;
		fillWithNOP(0x005F78A2, 0x7);
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x005F74AF;
		fillWithNOP(0x005F74B2, 0x7);
	}
}

onBuildingUnitAttributes::~onBuildingUnitAttributes()
{
}


void onBuildingUnitAttributes::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, 0x18C2BAC);
	
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onBuildingUnitAttributes::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);

	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	
	delete a;
}

onBuildingUnitAttributes2::onBuildingUnitAttributes2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x0060E866;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x0060E4B6;
	}
}

onBuildingUnitAttributes2::~onBuildingUnitAttributes2()
{
}


void onBuildingUnitAttributes2::SetOriginalCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, 0x18C2BAC);
	
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onBuildingUnitAttributes2::SetNewCode()
{
	Assembler* a = new Assembler();

	
	a->push(eax);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edi, eax);
	a->pop(ecx);
	a->pop(eax);

	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());

	delete a;
}

OnCreateUnitWrapper::OnCreateUnitWrapper(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
	{
		m_adress = 0x008EC39F;
	}
	else if (ver == 1)//kingdoms
	{
		m_adress = 0x008EB91F;
	}
}

OnCreateUnitWrapper::~OnCreateUnitWrapper()
{
}

void OnCreateUnitWrapper::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void OnCreateUnitWrapper::SetNewCode()
{
	Assembler* a = new Assembler();
	auto skip = a->newLabel();
	auto skip2 = a->newLabel();

	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->pushf();

	a->mov(ecx, eax);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->lea(esi, dword_ptr(eax));

	a->popf();

	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(edx);

	a->pushf();
	a->cmp(esi, 0);
	a->je(skip);
	
	a->mov(eax, dword_ptr(esi, 0x3C));

	a->bind(skip);
	a->cmp(esi, 0);
	a->jne(skip2);
	a->mov(eax, 0x10);

	a->bind(skip2);
	a->popf();
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

	a->mov(dword_ptr(esp, 0x20), eax);//move eax to stored eax
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

	a->mov(dword_ptr(esp, 0x20), eax);//move eax to stored eax
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

onBattleRams::onBattleRams(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x006A2CAC;

	else if (ver == 1)//kingdoms
		m_adress = 0x00ab4175;
}

onBattleRams::~onBattleRams()
{
}

void onBattleRams::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(eax, 0xD0), 01);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onBattleRams::SetNewCode()
{
	Assembler* a = new Assembler();
	
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onDecideRamAttacks::onDecideRamAttacks(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x006A324F;

	else if (ver == 1)//kingdoms
		m_adress = 0x006A2D3F;
}

onDecideRamAttacks::~onDecideRamAttacks()
{
}

void onDecideRamAttacks::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(eax, 0xD0), 01);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onDecideRamAttacks::SetNewCode()
{
	Assembler* a = new Assembler();

	auto label = a->newLabel();
	
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, dword_ptr(esp, 0x34));
	a->push(ecx);
	a->mov(edx, esi);
	a->mov(ecx, edi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->cmp(eax, 0);
	a->jnz(label);
	//a->mov(eax, 0x6A328B);
	if (m_adress == 0x006A324F)
		a->jmp(0x2A424B);
	else
		a->jmp(0x2A3D3B);
	a->bind(label);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onPreBattlePlacement::onPreBattlePlacement(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x006C8D9D;

	else if (ver == 1)//kingdoms
		m_adress = 0x006C886D;
}

onPreBattlePlacement::~onPreBattlePlacement()
{
}

void onPreBattlePlacement::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(esi, 0x2AF0), 0);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onPreBattlePlacement::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(byte_ptr(esi, 0x2AF0), 0);
	a->mov(ecx, esi);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

onPreBattlePlacement2::onPreBattlePlacement2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x006C823F;

	else if (ver == 1)//kingdoms
		m_adress = 0x006C7D0F;
}

onPreBattlePlacement2::~onPreBattlePlacement2()
{
}

void onPreBattlePlacement2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->mov(dword_ptr(esi, 0x2AE8), edx);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onPreBattlePlacement2::SetNewCode()
{
	Assembler* a = new Assembler();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}

OnMoveRecruitQueue::OnMoveRecruitQueue(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00ab495f;
	else if (ver == 1)//kingdoms
		m_adress = 0x00ab392f;
}
OnMoveRecruitQueue::~OnMoveRecruitQueue()
{
}
void OnMoveRecruitQueue::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->lea(edx, dword_ptr(esp, 0xC));
	a->push(edx);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	delete a;
}
void OnMoveRecruitQueue::SetNewCode()
{
	Assembler* a = new Assembler();

	a->lea(edx, dword_ptr(esp, 0xC));
	a->push(edx);

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
OnPathCasheCrashPlace::OnPathCasheCrashPlace(MemWork* mem, LPVOID addr, int ver, LPVOID cbObj)
	:AATemplate(mem), funcAddress(addr), callbackObject(cbObj)
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
	/*
	* 
	Assembler* a = new Assembler();
	//filler, not work!
	a->mov(ecx, ecx);
	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);
	delete a;
	 */
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

recruitEOPunit::recruitEOPunit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008ECFFE;

	else if (ver == 1)//kingdoms
		m_adress = 0x008EC57E;
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


	a->push(edx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);

	a->pushf();

	a->push(ecx);
	a->mov(ecx, ebp);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->lea(esi, dword_ptr(eax));
	a->pop(ecx);
	a->mov(dword_ptr(esp, 0x20), ecx);

	a->popf();

	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(edx);

	a->ret();
	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}


recruitEOPunit2::recruitEOPunit2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008ECFF2;

	else if (ver == 1)//kingdoms
		m_adress = 0x008EC572;
}

recruitEOPunit2::~recruitEOPunit2()
{
}

void recruitEOPunit2::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->call(m_adress);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void recruitEOPunit2::SetNewCode()
{
	Assembler* a = new Assembler();


	a->push(esi);
	a->push(edx);
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(edi);
	a->push(eax);

	a->pushf();

	a->mov(ecx, ebp);
	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);

	a->popf();

	a->pop(eax);
	a->pop(edi);
	a->pop(ebp);
	a->pop(ebx);
	a->pop(ecx);
	a->pop(edx);
	a->pop(esi);

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
		m_adress = 0x0060B8C7;
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

toSelectForDrawPortsCas::toSelectForDrawPortsCas(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0046c471;

	else if (ver == 1)//kingdoms
		m_adress = 0x0046c0b1;
}

toSelectForDrawPortsCas::~toSelectForDrawPortsCas()
{
}

void toSelectForDrawPortsCas::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->push(esi);
	a->mov(ebx, ecx);
	a->xor_(esi, esi);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toSelectForDrawPortsCas::SetNewCode()
{
	Assembler* a = new Assembler();
	a->push(esi);
	a->mov(ebx, ecx);
	a->xor_(esi, esi);

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

toStartOfDrawFunction::toStartOfDrawFunction(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008fb3c0;

	else if (ver == 1)//kingdoms
		m_adress = 0x008fa930;
}

toStartOfDrawFunction::~toStartOfDrawFunction()
{
}

void toStartOfDrawFunction::SetOriginialCode()
{
	Assembler* a = new Assembler();

	a->sub(esp,0x18);
	a->push(ebx);
	a->push(ebp);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toStartOfDrawFunction::SetNewCode()
{
	Assembler* a = new Assembler();
	a->sub(esp, 0x18);
	a->push(ebx);
	a->push(ebp);

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

toRetreat::toRetreat(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005a0c20;

	else if (ver == 1)//kingdoms
		m_adress = 0x005a0740;
}

toRetreat::~toRetreat()
{
}

void toRetreat::SetOriginialCode()
{
	Assembler* a = new Assembler();

	//retreat logic
	a->nop();
	a->nop();
	a->nop();
	a->nop();
	a->nop();

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toRetreat::SetNewCode()
{
	Assembler* a = new Assembler();

	a->pushad();
	a->pushf();

	a->mov(eax, (DWORD)funcAddress);
	a->call(eax);
	a->popf();
	a->popad();


	a->ret(0xC);
	a->ret();

	m_cheatBytes = (unsigned char*)a->make();

	delete a;
}
