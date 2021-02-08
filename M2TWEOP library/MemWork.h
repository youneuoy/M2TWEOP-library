#pragma once

//disasm
#define BEA_ENGINE_STATIC


#include <windows.h>
#include <TlHelp32.h>
#include <BeaEngine.h>
using namespace std;
class MemWork
{
public:
	MemWork(size_t bufSize = 32);
	void InjectCode(void* ptr, DWORD target, DWORD cave, unsigned char* original_code = 0, int* original_size = 0);

	int GetASMSize(unsigned char* ptr);
	void Read(DWORD from, size_t size);
	void Write(void* ptr, DWORD to, size_t size);

	DWORD Alloc(DWORD pageSize = 0x1000, DWORD memProtect = PAGE_EXECUTE_READWRITE);
	void Free(DWORD ptr);

	void SetMemory(unsigned char content, DWORD to, DWORD size);


	unsigned char* getBuffer();

	int m_bufSize;

	unsigned char* m_buf;
};

