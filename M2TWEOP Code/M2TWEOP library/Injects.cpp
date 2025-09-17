#include "pch.h"
#include "Injects.h"

#include "dataOffsets.h"


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
		m_adress = 0x0044F4EF;

	else if (ver == 1)//kingdoms
		m_adress = 0x0044F1AF;
}

toResourcesReadModels::~toResourcesReadModels()
{
}

void toResourcesReadModels::SetOriginaResModelsCode()
{
	Assembler* a = new Assembler();

	a->push(0);
	a->push(0);
	a->push(0);

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
	
	a->push(0);
	a->push(0);
	a->push(0);

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
	a->mov(dword_ptr(esp, 0x10), 0);

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

onSetExtentsTexture::onSetExtentsTexture(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00991068;

	else if (ver == 1)//kingdoms
		m_adress = 0x09904F8;
}

onSetExtentsTexture::~onSetExtentsTexture()
{
}

void onSetExtentsTexture::SetOriginalCode()
{
	auto a = new Assembler();

	a->lea(ecx, dword_ptr(esp, 0xC));
	a->movzx(edx, byte_ptr(ecx));

	a->ret();
	m_originalBytes = static_cast<unsigned char*>(a->make());
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSetExtentsTexture::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, byte_ptr(esi, 0x80034));
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(dword_ptr(esp, 0x10), eax);
	a->pop(eax);
	a->lea(ecx, dword_ptr(esp, 0xC));
	a->movzx(edx, byte_ptr(ecx));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetExtentsTextureBorder::onSetExtentsTextureBorder(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00990710;

	else if (ver == 1)//kingdoms
		m_adress = 0x0098FBA0;
}

onSetExtentsTextureBorder::~onSetExtentsTextureBorder()
{
}

void onSetExtentsTextureBorder::SetOriginalCode()
{
	auto a = new Assembler();

	a->mov(byte_ptr(edi), al);
	a->add(ebp, 1);

	a->ret();
	m_originalBytes = static_cast<unsigned char*>(a->make());
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSetExtentsTextureBorder::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, dword_ptr(esp, 0x8));
	a->mov(ecx, al);
	a->pushf();
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(byte_ptr(edi), al);
	a->pop(eax);
	a->popf();
	a->pop(edx);
	a->pop(ecx);
	a->add(ebp, 1);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetExtentsTextureZOC::onSetExtentsTextureZOC(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00991201;

	else if (ver == 1)//kingdoms
		m_adress = 0x00990691;
}

onSetExtentsTextureZOC::~onSetExtentsTextureZOC()
{
}

void onSetExtentsTextureZOC::SetOriginalCode()
{
	auto a = new Assembler();

	a->mov(byte_ptr(eax, 0x3), 0);
	a->mov(byte_ptr(eax, 0x2), 0xa0);
	a->mov(byte_ptr(eax, 0x1), 0);

	a->ret();
	m_originalBytes = static_cast<unsigned char*>(a->make());
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSetExtentsTextureZOC::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, eax);
	a->pop(eax);
	a->mov(dword_ptr(eax), ecx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetExtentsTextureZOCAlpha::onSetExtentsTextureZOCAlpha(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00991321;

	else if (ver == 1)//kingdoms
		m_adress = 0x009907B1;
}

onSetExtentsTextureZOCAlpha::~onSetExtentsTextureZOCAlpha()
{
}

void onSetExtentsTextureZOCAlpha::SetOriginalCode()
{
	auto a = new Assembler();

	a->cmp(ebx, dword_ptr(esp, 0x10));
	a->mov(byte_ptr(esi, 3), al);

	a->ret();
	m_originalBytes = static_cast<unsigned char*>(a->make());
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onSetExtentsTextureZOCAlpha::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, al);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(byte_ptr(esi, 3), al);
	a->cmp(ebx, dword_ptr(esp, 0x10));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateProductionController::onCreateProductionController(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x52E7D6;

	else if (ver == 1)//kingdoms
		m_adress = 0x52E1D6;
}

onCreateProductionController::~onCreateProductionController()
{
}

void onCreateProductionController::SetOriginalCode()
{
	auto a = new Assembler();

	a->ret();
	m_originalBytes = static_cast<unsigned char*>(a->make());
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onCreateProductionController::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(eax);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onDecideNeighbours::onDecideNeighbours(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x004EFFCE;

	else if (ver == 1)//kingdoms
		m_adress = 0x004EF9BE;
}

onDecideNeighbours::~onDecideNeighbours()
{
}

void onDecideNeighbours::SetOriginalCode()
{
	auto a = new Assembler();

	a->pop(edi);
	a->pop(esi);
	a->add(esp, 0x10);

	a->ret();
	m_originalBytes = static_cast<unsigned char*>(a->make());
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void onDecideNeighbours::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edi);
	a->pop(esi);
	a->add(esp, 0x10);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onTransferSettlement::onTransferSettlement(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005E5A4F;

	else if (ver == 1)//kingdoms
		m_adress = 0x005E55DF;
}

void onTransferSettlement::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(esp, 0x874));
	a->push(ecx);
	a->push(ebx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	if (m_adress == 0x005E5A4F)
		a->mov(eax, 0x005E5AC4);
	else
		a->mov(eax, 0x005E5654);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onTransferSettlementPort::onTransferSettlementPort(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005E5AE4;

	else if (ver == 1)//kingdoms
		m_adress = 0x5E5674;
}

void onTransferSettlementPort::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCheckConstructionItem::onCheckConstructionItem(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB563;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EB103;
}

void onCheckConstructionItem::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(esp, 0x8));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onInitGsd::onInitGsd(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0052B067;

	else if (ver == 1)//kingdoms
		m_adress = 0x0052AA67;
}

void onInitGsd::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onInitGsd2::onInitGsd2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0052B07F;

	else if (ver == 1)//kingdoms
		m_adress = 0x0052AA7F;
}

void onInitGsd2::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateTakeResidenceObjective::onCreateTakeResidenceObjective(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0052731F;

	else if (ver == 1)//kingdoms
		m_adress = 0x00526D1F;
}

void onCreateTakeResidenceObjective::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(esp, 0x1C));
	a->mov(ecx, dword_ptr(esp, 0x20));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(dword_ptr(esp, 0x1C), eax);
	a->mov(edi, eax);
	a->mov(eax, dword_ptr(edi));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAttachRegionSettlement::onAttachRegionSettlement(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x004A450F;

	else if (ver == 1)//kingdoms
		m_adress = 0x004A406F;
}

void onAttachRegionSettlement::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(esp, 0x14));
	a->mov(ecx, dword_ptr(esp, 0x18));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x004A450F)
		a->mov(eax, 0x004A456A);
	else
		a->mov(eax, 0x004A40CA);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCalculateSettlement::onCalculateSettlement(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005F464D;

	else if (ver == 1)//kingdoms
		m_adress = 0x005F425D;
}

void onCalculateSettlement::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, dword_ptr(esi, 0x4));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, dword_ptr(esi, 0x4));
	a->push(ecx);
	if (m_adress == 0x005F464D)
		a->mov(ecx, 0x2C1B8D8);
	else
		a->mov(ecx, 0x2C64990);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onPredictedStats::onPredictedStats(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005F4577;

	else if (ver == 1)//kingdoms
		m_adress = 0x005F4187;
}

void onPredictedStats::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(eax, dword_ptr(esp, 0xe4));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCalculateMiningIncome::onCalculateMiningIncome(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5F6B18;

	else if (ver == 1)//kingdoms
		m_adress = 0x5F6728;
}

void onCalculateMiningIncome::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, edi);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->push(eax);
	a->push(0x290);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetUnitCard::onGetUnitCard(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8EBFE5;

	else if (ver == 1)//kingdoms
		m_adress = 0x8EB565;
}

void onGetUnitCard::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, dword_ptr(esp, 0x18));
	a->push(dword_ptr(esp, 0x1C));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->sub(esp, 0x130);
	a->cmp(eax, 0);
	a->jz(label);
	a->push(esi);
	a->push(edi);
	if (m_adress == 0x8EBFE5)
		a->mov(eax, 0x8EC16F);
	else
		a->mov(eax, 0x8EB6EF);
	a->jmp(eax);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetUnitInfoCard::onGetUnitInfoCard(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8EC1A5;

	else if (ver == 1)//kingdoms
		m_adress = 0x8EB725;
}

void onGetUnitInfoCard::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, dword_ptr(esp, 0x18));
	a->push(dword_ptr(esp, 0x1C));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->sub(esp, 0x130);
	a->cmp(eax, 0);
	a->jz(label);
	a->push(esi);
	a->push(edi);
	if (m_adress == 0x8EC1A5)
		a->mov(eax, 0x8EC32F);
	else
		a->mov(eax, 0x8EB8AF);
	a->jmp(eax);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetSupportingArmies::onGetSupportingArmies(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x517DAC;

	else if (ver == 1)//kingdoms
		m_adress = 0x5177BC;
}

void onGetSupportingArmies::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ebp);
	a->push(ecx);
	a->push(edx);
	a->mov(edx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->pop(ebp);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onDrawBanner::onDrawBanner(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x9D31F5;

	else if (ver == 1)//kingdoms
		m_adress = 0x9D2655;
}

void onDrawBanner::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(dword_ptr(esp, 0x1C), eax);
	a->mov(dl, byte_ptr(esp, 0x1E));
	a->pop(eax);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetRebelSymbol::onGetRebelSymbol(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x9D3893;

	else if (ver == 1)//kingdoms
		m_adress = 0x9D2CF3;
}

void onGetRebelSymbol::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->mov(edx, eax);
	a->mov(ecx, dword_ptr(esp, 0x30));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->mov(ecx, dword_ptr(eax));
	a->mov(dword_ptr(ebx), ecx);
	a->mov(edx, dword_ptr(eax, 0x4));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetupBattleFromStrat::onSetupBattleFromStrat(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x61DFC9;

	else if (ver == 1)//kingdoms
		m_adress = 0x61DC19;
}

void onSetupBattleFromStrat::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->cmp(ecx, 0);
	a->mov(al, 0);
	a->jz(label);
	if (m_adress == 0x61DFC9)
		a->mov(eax, 0x718D70);
	else
		a->mov(eax, 0x718650);
	a->call(eax);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetSupportingArmies2::onGetSupportingArmies2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x517E3C;

	else if (ver == 1)//kingdoms
		m_adress = 0x51784C;
}

void onGetSupportingArmies2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ebp);
	a->push(ecx);
	a->push(edx);
	a->mov(edx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->pop(ebp);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onEvalAttObjective::onEvalAttObjective(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0051B590;

	else if (ver == 1)//kingdoms
		m_adress = 0x0051AFA0;
}

void onEvalAttObjective::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onEvalAttObjective2::onEvalAttObjective2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0051B450;

	else if (ver == 1)//kingdoms
		m_adress = 0x0051AE60;
}

void onEvalAttObjective2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onEvalAttObjective3::onEvalAttObjective3(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0051BA60;

	else if (ver == 1)//kingdoms
		m_adress = 0x0051B470;
}

void onEvalAttObjective3::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onEvalAttObjective4::onEvalAttObjective4(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0051B8D0;

	else if (ver == 1)//kingdoms
		m_adress = 0x0051B2E0;
}

void onEvalAttObjective4::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onEvalAttObjective5::onEvalAttObjective5(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0051B720;

	else if (ver == 1)//kingdoms
		m_adress = 0x0051B130;
}

void onEvalAttObjective5::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onRegionGroupStuff::onRegionGroupStuff(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00537B09;

	else if (ver == 1)//kingdoms
		m_adress = 0x005374D9;
}

void onRegionGroupStuff::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->test(eax, eax);
	a->jne(label);
	if (m_adress == 0x00537B09)
		a->mov(eax, 0x537D13);
	else
		a->mov(eax, 0x5376E3);
	a->jmp(eax);
	a->bind(label);
	a->mov(esi, eax);
	a->mov(edx, dword_ptr(esi));
	a->mov(ecx, esi);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onPopActionMem::onPopActionMem(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005B819E;

	else if (ver == 1)//kingdoms
		m_adress = 0x005B7CBE;
}

void onPopActionMem::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->mov(eax, dword_ptr(ecx));
	a->cmp(eax, 0x401000);
	a->jle(label);
	a->mov(eax, dword_ptr(eax));
	a->cmp(eax, 0x401000);
	a->jle(label);
	a->push(1);
	a->call(eax);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetSettlementModel::onSetSettlementModel(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5DB759;

	else if (ver == 1)//kingdoms
		m_adress = 0x5DB299;
}

void onSetSettlementModel::SetNewCode()
{
	const auto a = new Assembler();
	a->pushad();
	a->pushf();
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->popf();
	a->popad();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onRemoveFromUnitQueue::onRemoveFromUnitQueue(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5EA313;

	else if (ver == 1)//kingdoms
		m_adress = 0x5E9EB3;
}

void onRemoveFromUnitQueue::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(esp, 0x4));
	a->pushad();
	a->pushf();
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->popf();
	a->popad();
	if (m_adress == 0x5EA313)
		a->mov(eax, 0x5E9A60);
	else
		a->mov(eax, 0x5E9600);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onDecideMissionTarget::onDecideMissionTarget(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0057096B;

	else if (ver == 1)//kingdoms
		m_adress = 0x005704AB;
}

void onDecideMissionTarget::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->mov(edx, dword_ptr(esp, 0x70));
	a->push(ecx);
	a->push(eax);
	a->mov(ecx, ebp);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ebp, eax);
	a->pop(eax);
	a->pop(ecx);
	a->pop(edx);
	a->test(ebp, ebp);
	a->mov(dword_ptr(esp, 0x1c), ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCanWithdrawPreBattle::onCanWithdrawPreBattle(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x714744;

	else if (ver == 1)//kingdoms
		m_adress = 0x714044;
}

void onCanWithdrawPreBattle::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddSettlementToDiplomacyScroll::onAddSettlementToDiplomacyScroll(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xBC7491;

	else if (ver == 1)//kingdoms
		m_adress = 0xBCD131;
}

void onAddSettlementToDiplomacyScroll::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->mov(edx, dword_ptr(ecx, 0x128));
	a->mov(edi, ebp);
	a->imul(edi, 4);
	a->add(edi, edx);
	a->mov(edi, dword_ptr(edi, 0));
	a->push(edi);
	a->push(eax);
	a->push(ebx);
	a->push(ebp);
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->pop(ebp);
	a->pop(ebx);
	a->cmp(eax, 1);
	a->pop(eax);
	a->pop(edi);
	a->jz(label);
	if (m_adress == 0xBC7491)
		a->mov(eax, 0xBC753C);
	else
		a->mov(eax, 0x00BCD1DC);
	a->jmp(eax);
	a->bind(label);
	a->cmp(byte_ptr(eax, 0x91), bl);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onPartialConstructionPush::onPartialConstructionPush(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EA9B0;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EA550;
}

void onPartialConstructionPush::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(eax, 0x38));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onLoadBuilding::onLoadBuilding(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EE0EB;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EDC8B;
}

void onLoadBuilding::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, dword_ptr(eax, 0x38));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onLoadModelRigid::onLoadModelRigid(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xA04FEE;

	else if (ver == 1)//kingdoms
		m_adress = 0xA0446E;
}

void onLoadModelRigid::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onUnloadModels::onUnloadModels(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xA0506C;

	else if (ver == 1)//kingdoms
		m_adress = 0xA044EC;
}

void onUnloadModels::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(eax, 0);
	a->test(eax, eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onOffMapModelThing::onOffMapModelThing(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x009D0194;

	else if (ver == 1)//kingdoms
		m_adress = 0x9CF5F4;
}

void onOffMapModelThing::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->push(ebp);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ebp);
	a->pop(ecx);
	a->pop(edx);
	a->mov(dword_ptr(esp, 0x78), eax);
	a->lea(eax, dword_ptr(esp, 0x28));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onOffMapModelThing2::onOffMapModelThing2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x9D0000;

	else if (ver == 1)//kingdoms
		m_adress = 0x9CF460;
}

void onOffMapModelThing2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->push(ebp);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ebp);
	a->pop(ecx);
	a->pop(edx);
	a->mov(edi, eax);
	a->lea(eax, dword_ptr(esp, 0x38));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddBuilding::onAddBuilding(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5EAE1D;

	else if (ver == 1)//kingdoms
		m_adress = 0x5EA9BD;
}

void onAddBuilding::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(edx, 0x14));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onRemoveBuilding::onRemoveBuilding(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5EA9F6;

	else if (ver == 1)//kingdoms
		m_adress = 0x5EA596;
}

void onRemoveBuilding::SetNewCode()
{
	const auto a = new Assembler();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetBuildingById::onGetBuildingById(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EA950;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EA4F0;
}

void onGetBuildingById::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->mov(edx, dword_ptr(esp, 0x8));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCheckSettHasBuilding::onCheckSettHasBuilding(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB234;

	else if (ver == 1)//kingdoms
		m_adress = 0x5EADD4;
}

void onCheckSettHasBuilding::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(ecx);
	a->push(eax);
	a->mov(edx, dword_ptr(eax, 0x14));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->pop(eax);
	a->pop(ecx);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetBuildingByIdConst::onGetBuildingByIdConst(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EA960;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EA500;
}

void onGetBuildingByIdConst::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->mov(edx, dword_ptr(esp, 0x8));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

getPossibleConstructions::getPossibleConstructions(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8A8D70;

	else if (ver == 1)//kingdoms
		m_adress = 0x8A8390;
}

void getPossibleConstructions::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ebx);
	a->push(esi);
	a->push(edi);
	a->push(dword_ptr(esp, 0x24));
	a->push(dword_ptr(esp, 0x24));
	a->push(dword_ptr(esp, 0x24));
	a->push(dword_ptr(esp, 0x24));
	a->push(dword_ptr(esp, 0x24));
	a->mov(edx, dword_ptr(esp, 0x24));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edi);
	a->pop(esi);
	a->pop(ebx);
	a->sub(esp, 0x0A4C);
	if (m_adress == 0x8A8D70)
		a->mov(eax, 0x8A8EFF);
	else
		a->mov(eax, 0x8A8520);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onConflictTest::onConflictTest(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB5B2;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EB152;
}

void onConflictTest::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, esi);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->pop(esi);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onConflictTest2::onConflictTest2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB36C;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EAF0C;
}

void onConflictTest2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCheckBuildUpgrade::onCheckBuildUpgrade(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB774;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EB314;
}

void onCheckBuildUpgrade::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ebx, eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetCqFlags::onSetCqFlags(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB3C0;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EAF60;
}

void onSetCqFlags::SetNewCode()
{
	const auto a = new Assembler();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetCqFlags2::onSetCqFlags2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB3F7;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EAF97;
}

void onSetCqFlags2::SetNewCode()
{
	const auto a = new Assembler();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onRemoveCqFlags::onRemoveCqFlags(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB450;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EAFF0;
}

void onRemoveCqFlags::SetNewCode()
{
	const auto a = new Assembler();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onRemoveCqFlags2::onRemoveCqFlags2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB495;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EB035;
}

void onRemoveCqFlags2::SetNewCode()
{
	const auto a = new Assembler();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onDamageCoreBuilding::onDamageCoreBuilding(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EE19A;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EDD3A;
}

void onDamageCoreBuilding::SetNewCode()
{
	const auto a = new Assembler();
	a->pushad();
	a->mov(edx, eax);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->xor_(ebp, ebp);
	a->cmp(eax, ebp);
	a->popad();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onDamageCoreBuilding2::onDamageCoreBuilding2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EE1D0;

	else if (ver == 1)//kingdoms
		m_adress = 0x005EDD70;
}

void onDamageCoreBuilding2::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, dword_ptr(eax, 0x14));
	a->pushad();
	a->mov(edx, ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->xor_(ebp, ebp);
	a->cmp(eax, ebp);
	a->popad();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onDamageCoreBuilding3::onDamageCoreBuilding3(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x05EDA92;

	else if (ver == 1)//kingdoms
		m_adress = 0x05ED632;
}

void onDamageCoreBuilding3::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(eax, dword_ptr(eax, 0x14));
	a->push(eax);
	a->push(ebx);
	a->push(ecx);
	a->push(edx);
	a->push(esp);
	a->push(ebp);
	a->push(esi);
	a->mov(edx, eax);
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edi, eax);
	a->pop(esi);
	a->pop(ebp);
	a->pop(esp);
	a->pop(edx);
	a->pop(ecx);
	a->pop(ebx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetWatchTowerRange::onGetWatchTowerRange(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x004DD330;

	else if (ver == 1)//kingdoms
		m_adress = 0x4DCD80;
}

void onGetWatchTowerRange::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ebx);
	a->push(ecx);
	a->push(edx);
	a->push(esp);
	a->push(ebp);
	a->push(edi);
	a->push(esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(esp);
	a->pop(edx);
	a->pop(ecx);
	a->pop(ebx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCheckGarrison::onCheckGarrison(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5506BA;

	else if (ver == 1)//kingdoms
		m_adress = 0x55022A;
}

void onCheckGarrison::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ebx);
	a->push(ecx);
	a->push(esp);
	a->push(ebp);
	a->push(edi);
	a->push(esi);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edx, eax);
	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(esp);
	a->pop(ecx);
	a->pop(ebx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onValidateGarrison::onValidateGarrison(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x55067D;

	else if (ver == 1)//kingdoms
		m_adress = 0x5501ED;
}

void onValidateGarrison::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(edx);
	a->push(ebx);
	a->push(ecx);
	a->push(esp);
	a->push(ebp);
	a->push(edi);
	a->push(esi);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edx, 1);
	a->cmp(edx, eax);
	a->pop(esi);
	a->pop(edi);
	a->pop(ebp);
	a->pop(esp);
	a->pop(ecx);
	a->pop(ebx);
	a->pop(edx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGeneralAssaultAction::onGeneralAssaultAction(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x05BEDA1;

	else if (ver == 1)//kingdoms
		m_adress = 0x5BE8C1;
}

void onGeneralAssaultAction::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(edx);
	a->push(ebx);
	a->push(ecx);
	a->push(esp);
	a->push(ebp);
	a->push(edi);
	a->mov(esi, ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->test(byte_ptr(esi, 0x4), 2);
	a->pop(edi);
	a->pop(ebp);
	a->pop(esp);
	a->pop(ecx);
	a->pop(ebx);
	a->pop(edx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddBuildingCapsAfterConstruction::onAddBuildingCapsAfterConstruction(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005ED4E2;

	else if (ver == 1)//kingdoms
		m_adress = 0x005ED082;
}

void onAddBuildingCapsAfterConstruction::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(dx, eax);
	a->pop(ecx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddBuildingCapsAfterConstruction2::onAddBuildingCapsAfterConstruction2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5ED507;

	else if (ver == 1)//kingdoms
		m_adress = 0x005ED0A7;
}

void onAddBuildingCapsAfterConstruction2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(edx);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(cx, eax);
	a->pop(edx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddBuildingCapsAfterConstruction3::onAddBuildingCapsAfterConstruction3(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005ED60D;

	else if (ver == 1)//kingdoms
		m_adress = 0x005ED1AD;
}

void onAddBuildingCapsAfterConstruction3::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(dx, eax);
	a->pop(ecx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddBuildingCapsAfterConstruction4::onAddBuildingCapsAfterConstruction4(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005ED632;

	else if (ver == 1)//kingdoms
		m_adress = 0x005ED1D2;
}

void onAddBuildingCapsAfterConstruction4::SetNewCode()
{
	const auto a = new Assembler();
	a->push(eax);
	a->push(edx);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(cx, eax);
	a->pop(edx);
	a->pop(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAddBuildingCapsAfterConstruction5::onAddBuildingCapsAfterConstruction5(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5EDA2A;

	else if (ver == 1)//kingdoms
		m_adress = 0x005ED5CA;
}

void onAddBuildingCapsAfterConstruction5::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, eax);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ax, eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetDefensesBuilding::onGetDefensesBuilding(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5EAEA3;

	else if (ver == 1)//kingdoms
		m_adress = 0x5EAA43;
}

void onGetDefensesBuilding::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(edx, dword_ptr(eax, 0x14));
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetKhakiText::onSetKhakiText(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00B18307;

	else if (ver == 1)//kingdoms
		m_adress = 0x00B1DFF7;
}

void onSetKhakiText::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->mov(dword_ptr(edi, 0x4), edx);
	a->mov(byte_ptr(edi, 0x8), bl);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetKhakiText2::onSetKhakiText2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xB18457;

	else if (ver == 1)//kingdoms
		m_adress = 0xB1E147;
}

void onSetKhakiText2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->mov(dword_ptr(edi, 0x4), edx);
	a->mov(byte_ptr(edi, 0x8), bl);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetKhakiText3::onSetKhakiText3(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xB185CC;

	else if (ver == 1)//kingdoms
		m_adress = 0xB1E2BC;
}

void onSetKhakiText3::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->mov(dword_ptr(edi, 0x4), edx);
	a->mov(byte_ptr(edi, 0x8), bl);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetKhakiText4::onSetKhakiText4(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xB1AA8C;

	else if (ver == 1)//kingdoms
		m_adress = 0xB2077C;
}

void onSetKhakiText4::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(dword_ptr(esi, 0x6C), eax);
	a->mov(eax, dword_ptr(esp, 0x20));
	a->push(ecx);
	a->push(edx);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetKhakiText5::onSetKhakiText5(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0xC44B8A;

	else if (ver == 1)//kingdoms
		m_adress = 0xC4A7BA;
}

void onSetKhakiText5::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(ecx, dword_ptr(esp, 0x28));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->sub(esp, 0x2C);
	a->cmp(dword_ptr(eax, 0x34), 0);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetRegionHiddenResources2::onSetRegionHiddenResources2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8D95A3;

	else if (ver == 1)//kingdoms
		m_adress = 0x8D8B43;
}

void onSetRegionHiddenResources2::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(edx);
	a->push(ecx);
	a->push(edx);
	a->push(ebx);
	a->mov(ecx, edx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ebx);
	a->pop(edx);
	a->pop(ecx);
	a->cmp(eax, ebx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onHiddenResourceConditionParse::onHiddenResourceConditionParse(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8A79AA;

	else if (ver == 1)//kingdoms
		m_adress = 0x8A6FCA;
}

void onHiddenResourceConditionParse::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(ecx);
	a->push(edx);
	a->push(ebp);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ebp);
	a->pop(edx);
	a->mov(edi, eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onHiddenResourceCheck::onHiddenResourceCheck(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x609E54;

	else if (ver == 1)//kingdoms
		m_adress = 0x609AA4;
}

void onHiddenResourceCheck::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(esi);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x609E54)
		a->mov(ecx, 0x609E84);
	else
		a->mov(ecx, 0x609AD4);
	a->jmp(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onHiddenResourceCheck2::onHiddenResourceCheck2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x609F52;

	else if (ver == 1)//kingdoms
		m_adress = 0x609BA2;
}

void onHiddenResourceCheck2::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(esi);
	a->mov(ecx, edx);
	a->mov(edx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x609F52)
		a->mov(ecx, 0x609F81);
	else
		a->mov(ecx, 0x609BD1);
	a->jmp(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateMarriageOption::onCreateMarriageOption(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x4F15F3;

	else if (ver == 1)//kingdoms
		m_adress = 0x4F0FD3;
}

void onCreateMarriageOption::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onEnemyClicked::onEnemyClicked(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x004DCDD4;

	else if (ver == 1)//kingdoms
		m_adress = 0x4F0FD3;
}

void onEnemyClicked::SetNewCode()
{
	const auto a = new Assembler();
	a->add(ecx, 0x88);
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCampaignTick::onCampaignTick(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x47D5B6;

	else if (ver == 1)//kingdoms
		m_adress = 0x47D1D6;
}

void onCampaignTick::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, eax);
	a->pop(eax);
	a->pop(edx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onBattleTick::onBattleTick(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x60FE6F;

	else if (ver == 1)//kingdoms
		m_adress = 0x60FABF;
}

void onBattleTick::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->mov(eax, dword_ptr(esi, 0x4));
	a->cmp(eax, 5);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCalculateLTGD::onCalculateLTGD(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x53FCB1;

	else if (ver == 1)//kingdoms
		m_adress = 0x53F891;
}

void onCalculateLTGD::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(esi);
	if (m_adress == 0x53FCB1)
		a->mov(eax,0x53FCFA);
	else
		a->mov(eax,0x53F8DA);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onStartProductionTurn::onStartProductionTurn(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x532502;

	else if (ver == 1)//kingdoms
		m_adress = 0x531EF2;
}

void onStartProductionTurn::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x532502)
		a->mov(eax,0x532544);
	else
		a->mov(eax,0x531F34);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetBuildPolicies::onSetBuildPolicies(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x52E841;

	else if (ver == 1)//kingdoms
		m_adress = 0x52E241;
}

void onSetBuildPolicies::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, dword_ptr(esp, 0x8));
	a->push(dword_ptr(esp, 0xC));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x52E841)
		a->mov(eax,0x52E894);
	else
		a->mov(eax,0x52E294);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onUpdateProdControllers::onUpdateProdControllers(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x532553;

	else if (ver == 1)//kingdoms
		m_adress = 0x531F43;
}

void onUpdateProdControllers::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(esi);
	a->pop(ebp);
	a->add(esp, 0x4);
	if (m_adress == 0x532553)
		a->mov(eax,0x5322A0);
	else
		a->mov(eax,0x531C90);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetProdPriorities::onSetProdPriorities(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0532724;

	else if (ver == 1)//kingdoms
		m_adress = 0x532114;
}

void onSetProdPriorities::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(edx);
	a->pop(edx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onGetTrueBuildingCapabilities::onGetTrueBuildingCapabilities(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5FC1A7;

	else if (ver == 1)//kingdoms
		m_adress = 0x5FBDB7;
}

void onGetTrueBuildingCapabilities::SetNewCode()
{
	const auto a = new Assembler();
	const Label skipJump = a->newLabel();
	a->mov(dword_ptr(ebx, ecx, 2), esi);
	a->add(dword_ptr(edi), 1);
	a->mov(edx, ebp); //stackCapabilities has sett ptr
	a->mov(ecx, dword_ptr(edi)); //counter
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->test(eax, eax);
	a->jz(skipJump);
	//early exit
	if (m_adress == 0x5FC1A7)
		a->mov(eax, 0x005FC1B4);
	else
		a->mov(eax, 0x5FBDC4);
	a->jmp(eax);
	a->bind(skipJump);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}
onCheckMountedEngineValid::onCheckMountedEngineValid(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8ED625;

	else if (ver == 1)//kingdoms
		m_adress = 0x8ECBA5;
}

void onCheckMountedEngineValid::SetNewCode()
{
	const auto a = new Assembler();
	a->pop(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}
onParseMountedEngines::onParseMountedEngines(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8CF233;

	else if (ver == 1)//kingdoms
		m_adress = 0x8CE813;
}

void onParseMountedEngines::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ebp);
	a->push(esi);
	a->push(edi);
	a->mov(edx, ebx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x8CF233)
		a->mov(esi, 0x8CF2D6);
	else
		a->mov(esi, 0x8CE8B6);
	a->jmp(esi);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onParseEdu::onParseEdu(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8F3300;

	else if (ver == 1)//kingdoms
		m_adress = 0x8F2880;
}

void onParseEdu::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(edx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	if (m_adress == 0x8F3300)
		a->mov(esi, 0x8F33B0);
	else
		a->mov(esi, 0x8F2930);
	a->jmp(esi);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onApplyKillChance::onApplyKillChance(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x008249A0;

	else if (ver == 1)//kingdoms
		m_adress = 0x824130;
}

void onApplyKillChance::SetNewCode()
{
	const auto a = new Assembler();
	const Label notZero = a->newLabel();
	a->mov(edx, dword_ptr(ebx, 0x10)); //heroability kill chance
	a->test(edx, edx); //test if killchance is 0
	a->jnz(notZero);
	a->mov(edx, 0x3F800000);
	a->movd(xmm0, edx); // Move 1 into xmm0
	a->bind(notZero);
	a->mulss(xmm0, dword_ptr(eax, 0x2d8));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onInitUnitBattle::onInitUnitBattle(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x752B02;

	else if (ver == 1)//kingdoms
		m_adress = 0x752372;
}

void onInitUnitBattle::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->mov(ecx, 0x3F800000);
	a->movd(xmm0, ecx); // Move 1 into xmm0
	a->pop(ecx);
	a->movss(dword_ptr(esi, 0x2d8), xmm0); //move result into unit
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateUnitKillChance::onCreateUnitKillChance(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x74C1D8;

	else if (ver == 1)//kingdoms
		m_adress = 0x74BA58;
}

void onCreateUnitKillChance::SetNewCode()
{
	const auto a = new Assembler();
	a->push(edx);
	a->mov(edx, 0x3F800000);
	a->movd(xmm0, edx); // Move 1 into xmm0
	a->pop(edx);
	a->movss(dword_ptr(esi, 0x2d8), xmm0); //move result into unit
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onRemoveKillChance::onRemoveKillChance(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00824A6B;

	else if (ver == 1)//kingdoms
		m_adress = 0x8241FB;
}

void onRemoveKillChance::SetNewCode()
{
	const auto a = new Assembler();
	const Label isZero = a->newLabel();
	a->mov(edx, 0x3F800000);
	a->push(eax);
	a->movd(xmm0, edx); // Move 1 into xmm0
	a->mov(eax, dword_ptr(ebx, 0x10)); //heroability kill chance
	a->test(eax, eax); //test if killchance is 0
	a->pop(eax);
	a->jz(isZero); //if 0 then just jump to setting xmm0 which contains 1 into unit kill chance
	a->movss(xmm1, dword_ptr(ebx, 0x10)); //move heroability chance into float register
	a->divss(xmm0, xmm1); //divide 1 by hero ability
	a->mulss(xmm0, dword_ptr(eax, 0x2d8)); //multiply with unit kill chance
	a->bind(isZero);
	a->movss(dword_ptr(eax, 0x2d8), xmm0); //move result into unit
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}
onGetMountedEngine::onGetMountedEngine(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8ED561;

	else if (ver == 1)//kingdoms
		m_adress = 0x8ECAE1;
}

void onGetMountedEngine::SetNewCode()
{
	const auto a = new Assembler();
	const Label error = a->newLabel();
	a->pop(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->test(eax, eax);
	a->jz(error);
	a->pop(edi);
	if (m_adress == 0x8ED561)
		a->mov(esi, 0x8ED57B);
	else
		a->mov(esi, 0x8ECAFB);
	a->jmp(esi);
	a->bind(error);
	if (m_adress == 0x8ED561)
		a->mov(eax, 0x8ED58D);
	else
		a->mov(eax, 0x8ECB0D);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateMarriageOption2::onCreateMarriageOption2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x4F1600;

	else if (ver == 1)//kingdoms
		m_adress = 0x4F0FE0;
}

void onCreateMarriageOption2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 1);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateBirthOption::onCreateBirthOption(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x59CA3B;

	else if (ver == 1)//kingdoms
		m_adress = 0x59C54B;
}

void onCreateBirthOption::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCreateBirthOption2::onCreateBirthOption2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x59CA48;

	else if (ver == 1)//kingdoms
		m_adress = 0x59C558;
}

void onCreateBirthOption2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 1);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onSetRegionHiddenResources::onSetRegionHiddenResources(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8D964B;

	else if (ver == 1)//kingdoms
		m_adress = 0x8D8BEB;
}

void onSetRegionHiddenResources::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->push(eax);
	a->mov(edx, esi);
	a->mov(ecx, eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->pop(eax);
	a->jbe(label);
	if (m_adress == 0x8D964B)
		a->mov(eax, 0x008D9677);
	else
		a->mov(eax, 0x008D8C17);
	a->jmp(eax);
	a->bind(label);
	a->mov(ecx, eax);
	a->mov(edx, eax);
	a->and_(eax, 0x1F);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onReadHiddenResources::onReadHiddenResources(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x8AD0AD;

	else if (ver == 1)//kingdoms
		m_adress = 0x8AC6BD;
}

void onReadHiddenResources::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(edx, ecx);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->jbe(label);
	if (m_adress == 0x8AD0AD)
		a->mov(eax, 0x8AD0CA);
	else
		a->mov(eax, 0x8AC6DA);
	a->jmp(eax);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onIsBuildingTypePresentAtMinLevel::onIsBuildingTypePresentAtMinLevel(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5EB254;

	else if (ver == 1)//kingdoms
		m_adress = 0x5EADF4;
}

void onIsBuildingTypePresentAtMinLevel::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->mov(edx, dword_ptr(eax, 0x14));
	a->push(edx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCheckBuildUpgrade2::onCheckBuildUpgrade2(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005EB722;

	else if (ver == 1)//kingdoms
		m_adress = 0x5EB2C2;
}

void onCheckBuildUpgrade2::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(edx);
	a->push(eax);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->pop(eax);
	a->pop(edx);
	a->pop(ecx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCalculateCommand::onCalculateCommand(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x5A6521;

	else if (ver == 1)//kingdoms
		m_adress = 0x5A6041;
}

void onCalculateCommand::SetNewCode()
{
	const auto a = new Assembler();
	a->pushad();
	a->pushf();
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edx, dword_ptr(esp, 0x10));
	a->add(edx, eax);
	a->mov(dword_ptr(esp, 0x10), edx);
	a->popf();
	a->popad();
	a->setz(al);
	a->cmp(byte_ptr(ebp, 0), 0);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onSetArmyGeneralsUnit::onSetArmyGeneralsUnit(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x71B5F4;

	else if (ver == 1)//kingdoms
		m_adress = 0x71AEC4;
}

void onSetArmyGeneralsUnit::SetNewCode()
{
	const auto a = new Assembler();
	a->pushad();
	a->pushf();
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->popf();
	a->popad();
	a->mov(ecx, dword_ptr(esi, 0xD8));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onWriteSoldiersToStrat::onWriteSoldiersToStrat(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0075307B;

	else if (ver == 1)//kingdoms
		m_adress = 0x007528EB;
}

void onWriteSoldiersToStrat::SetNewCode()
{
	const auto a = new Assembler();
	a->pushad();
	a->pushf();
	a->mov(dword_ptr(esi, 0x50c), eax);
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->popf();
	a->popad();
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onArmyDecimate::onArmyDecimate(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0071489D;

	else if (ver == 1)//kingdoms
		m_adress = 0x00714198;
}

void onArmyDecimate::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(eax, 0);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}


onUnitMerge::onUnitMerge(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00749EC1;

	else if (ver == 1)//kingdoms
		m_adress = 0x00749741;
}

void onUnitMerge::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->mov(edi, dword_ptr(ebx, 0x50c));
	a->cmp(edi, 1);
	a->jge(label);
	if (m_adress == 0x00749EC1)
		a->mov(ecx, 0x00749F18);
	else
		a->mov(ecx, 0x00749798);
	a->jmp(ecx);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onUpdateControllerAlloc::onUpdateControllerAlloc(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0051C77E;

	else if (ver == 1)//kingdoms
		m_adress = 0x51C18E;
}

void onUpdateControllerAlloc::SetNewCode()
{
	const auto a = new Assembler();
	a->mov(ecx, esi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, dword_ptr(esp, 0x14));
	a->pop(edi);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onCalcGarrisonStr::onCalcGarrisonStr(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x00515B70;

	else if (ver == 1)//kingdoms
		m_adress = 0x515580;
}

void onCalcGarrisonStr::SetNewCode()
{
	const auto a = new Assembler();
	a->push(ecx);
	a->push(ebx);
	a->push(ebp);
	a->push(esi);
	a->push(edi);
	a->mov(ecx, dword_ptr(esp, 0x18));
	a->mov(edx, dword_ptr(esp, 0x1C));
	a->push(dword_ptr(esp, 0x20));
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, eax);
	if (m_adress == 0x00515B70)
		a->mov(eax, 0x00515D20);
	else
		a->mov(eax, 0x00515730);
	a->jmp(eax);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onAssessRequiredStrength::onAssessRequiredStrength(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x005505D0;

	else if (ver == 1)//kingdoms
		m_adress = 0x550140;
}

void onAssessRequiredStrength::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->push(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->cmp(eax, 0);
	a->jnz(label);
	if (m_adress == 0x005505D0)
		a->mov(ecx, 0x55061D);
	else
		a->mov(ecx, 0x55018D);
	a->jmp(ecx);
	a->bind(label);
	a->push(esi);
	a->mov(esi, ecx);
	a->mov(eax, dword_ptr(esi, 0x20));
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
	delete a;
}

onScoreBestCapital::onScoreBestCapital(MemWork* mem, LPVOID addr, int ver)
	:AATemplate(mem), funcAddress(addr)
{
	if (ver == 2)//steam
		m_adress = 0x0053D61A;

	else if (ver == 1)//kingdoms
		m_adress = 0x0053D1FA;
}

void onScoreBestCapital::SetNewCode()
{
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->mov(ecx, dword_ptr(eax, 0x4));
	a->push(eax);
	a->push(ecx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->cmp(eax, 0);
	a->jnz(label);
	a->pop(edi);
	a->pop(edi);
	if (m_adress == 0x0053D61A)
		a->mov(edi, 0x53D919);
	else
		a->mov(edi, 0x53D4F9);
	a->jmp(edi);
	a->bind(label);
	a->pop(ecx);
	a->pop(eax);
	a->mov(edi, dword_ptr(eax, 0x4));
	a->xor_(ebx, ebx);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
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
		m_adress = 0x8EEB0D;

	else if (ver == 1)//kingdoms
		m_adress = 0x8EE08D;
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

	a->pushf();
	a->mov(ecx, ebx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(edi, eax);
	a->popf();
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
	a->mov(eax, byte_ptr(eax, 0x3AF));
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
		m_adress = 0x0047F5DD;
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
	a->push(edx);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->mov(ecx, eax);
	a->pop(edx);
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
	const auto a = new Assembler();
	const auto label = a->newLabel();
	a->push(edx);
	a->push(ecx);
	a->mov(ecx, dword_ptr(esp, 0x34));
	a->push(ecx);
	a->mov(edx, esi);
	a->mov(ecx, edi);
	a->mov(eax, reinterpret_cast<DWORD>(funcAddress));
	a->call(eax);
	a->pop(ecx);
	a->pop(edx);
	a->cmp(eax, 0);
	a->jnz(label);
	a->mov(eax, m_adress + 0x3C);
	a->jmp(eax);
	a->bind(label);
	a->ret();
	m_cheatBytes = static_cast<unsigned char*>(a->make());
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

toFactionSymbolSelect::toFactionSymbolSelect(MemWork* mem, LPVOID adr, int ver)
	:AATemplate(mem), funcAdress(adr)
{
	if (ver == 2)//steam
		m_adress = 0x00cbfed5;

	else if (ver == 1)//kingdoms
		m_adress = 0x00cc59b5;
}

toFactionSymbolSelect::~toFactionSymbolSelect()
{
}

void toFactionSymbolSelect::SetOriginalFactionSymbolSelect()
{
	Assembler* a = new Assembler();

	a->sub(esp, 0x84);

	a->ret();
	m_originalBytes = (unsigned char*)a->make();
	m_originalSize = m_memory->GetASMSize(m_originalBytes);

	delete a;
}

void toFactionSymbolSelect::SetNewFactionSymbolSelect()
{
	Assembler* a = new Assembler();

	a->sub(esp, 0x84);

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

