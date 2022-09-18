#include "pch.h"
#include "MemWork.h"
#include <fstream>
#include <string>
MemWork::MemWork(size_t bufSize) : m_bufSize(bufSize)
{
	m_buf = new unsigned char[bufSize];

}
void MemWork::InjectCode(void* ptr, DWORD target, DWORD cave, unsigned char* original_code, int* original_size)
{
	int size = GetASMSize((unsigned char*)ptr);

	DISASM MyDisasm = { 0 };
	MyDisasm.EIP = (UIntPtr)m_buf;
	int totallen = 0;

	while (totallen < 5)
	{

		Read(target + totallen, 15);

		int currentlen = Disasm(&MyDisasm);

		if (original_code)
			memcpy(original_code, m_buf, currentlen);
		totallen += currentlen;
	}

	if (original_size)
		*original_size = totallen;
	int nops = 0;
	if (totallen > 5)
	{
		nops = totallen - 5;
	}

	DWORD jmp_address = 0 - (target - cave) - 5;
	DWORD ret_address = 0 - (cave - target) - size + nops;
	unsigned char buffer[5] = { 0xE9, 0xE9, 0xE9, 0xE9, 0xE9 };
	memcpy(buffer + 1, &ret_address, 4);

	Write(ptr, cave, size);
	Write(buffer, cave + size, 5);



	memcpy(buffer + 1, &jmp_address, 4);

	Write(buffer, target, 5);

	if (nops > 0)
	{
		SetMemory(0x90, target + 5, nops);
	}
}

int MemWork::GetASMSize(unsigned char* ptr)
{
	DISASM MyDisasm = { 0 };
	MyDisasm.EIP = (UIntPtr)ptr;
	int totallen = 0;

	while (totallen < 1024)
	{
		int currentlen = Disasm(&MyDisasm);

		if (MyDisasm.Instruction.Opcode == 0xC3)
		{
			return totallen;
		}
		MyDisasm.EIP += currentlen;
		totallen += currentlen;
	}

	return 0;
}

void MemWork::Read(DWORD from, size_t size)
{
	HANDLE h = GetCurrentProcess();

	memset(m_buf, 0, m_bufSize);
	ReadProcessMemory(h, (LPVOID)from, m_buf, size, NULL);
	CloseHandle(h);
}


void MemWork::Write(void* ptr, DWORD to, size_t size)
{

	HANDLE h = GetCurrentProcess();
	DWORD oldMemProtect = 0;
	VirtualProtectEx(h, (LPVOID)to, size, PAGE_EXECUTE_READWRITE, &oldMemProtect);
	WriteProcessMemory(h, (LPVOID)to, ptr, size, NULL);
	VirtualProtectEx(h, (LPVOID)to, size, oldMemProtect, &oldMemProtect);

	CloseHandle(h);
}

DWORD MemWork::Alloc(DWORD pageSize, DWORD memProtect)
{
	HANDLE h = GetCurrentProcess();

	DWORD r = (DWORD)VirtualAllocEx(h, NULL, pageSize, MEM_COMMIT | MEM_RESERVE, memProtect);
	CloseHandle(h);

	return r;
}

void MemWork::Free(DWORD ptr)
{
	HANDLE h = GetCurrentProcess();
	VirtualFreeEx(h, (LPVOID)ptr, 0, MEM_RELEASE);
	CloseHandle(h);

}

void MemWork::SetMemory(unsigned char content, DWORD to, DWORD size)
{
	unsigned char* fill = new unsigned char[size];
	memset(fill, content, size);
	Write(fill, to, size);
	delete[] fill;
}

unsigned char* MemWork::getBuffer()
{
	return m_buf;

}
