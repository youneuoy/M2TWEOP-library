#include "memHelpers.h"

namespace memHelpers
{
	void writeToMem(void* src, void* dst, size_t size)
	{
		HANDLE h = GetCurrentProcess();
		DWORD oldMemProtect = 0;
		VirtualProtectEx(h, dst, size, PAGE_EXECUTE_READWRITE, &oldMemProtect);
		WriteProcessMemory(h, dst, src, size, NULL);
		VirtualProtectEx(h, dst, size, oldMemProtect, &oldMemProtect);

		CloseHandle(h);
	}
	void nopOneInstruction(void* address)
	{
		DISASM MyDisasm = {};
		MyDisasm.EIP = (UIntPtr)address;

		int currentlen = Disasm(&MyDisasm);

		std::vector<uint8_t> nops(currentlen, 0x90);

		writeToMem(nops.data(), address, currentlen);
	}

	void nopBytes(void* address, size_t size)
	{
		std::vector<uint8_t> nops(size, 0x90);
		writeToMem(nops.data(), address, size);
	}


	void readFromMem(void* src, void* dst, size_t size)
	{
		HANDLE h = GetCurrentProcess();

		ReadProcessMemory(h, src, dst, size, NULL);
		CloseHandle(h);
	}

	void getInstructionsFromMem(void* src, std::vector<uint8_t>& dest, int minBytes)
	{
		dest.clear();

		DISASM myDisasm = { 0 };
		myDisasm.EIP = (UIntPtr)src;


		int readedBytes = 0;
		do
		{
			int currentlen = Disasm(&myDisasm);


			myDisasm.EIP += currentlen;
			readedBytes += currentlen;
		} while (readedBytes < minBytes);


		dest.reserve(readedBytes);
		dest.resize(readedBytes);

		readFromMem(src, dest.data(), readedBytes);


		dest.shrink_to_fit();
	}
};