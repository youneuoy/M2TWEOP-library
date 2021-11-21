#pragma once

#include "headersSTD.h"

#include <BeaEngine.h>


#include "asmjit\asmjit.h"
namespace memHelpers
{
	void writeToMem(void* src, void* dst, size_t size);
	void nopOneInstruction(void* address);
	void nopBytes(void* address, size_t size);


	void readFromMem(void* src, void* dst, size_t size);
	//5 bytes needed for jmp
	void getInstructionsFromMem(void* src, std::vector<uint8_t>&dest, int minBytes = 5);
};

