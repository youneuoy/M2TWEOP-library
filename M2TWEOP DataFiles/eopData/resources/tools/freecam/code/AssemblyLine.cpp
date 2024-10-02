#include "AssemblyLine.h"




AssemblyLine::AssemblyLine(LPVOID addr)
{
	this->address = addr;
	this->bytes = (int)RW::readMemoryByte(addr) == 243 ? 5 : 3;
	//The 243 or F3 byte means that the operatation in total is 5 bytes long.
	//Otherwise the operation is 3 bytes long. This works for this program as these are the only possibilities
	this->original = new byte[this->bytes];

	for (int i = 0; i < this->bytes; i++)
		original[i] = RW::readMemoryByte((LPVOID)((int)(address)+i));
}

void AssemblyLine::nop()
{
	byte nop = (byte)144;
	for (int i = 0; i < bytes; i++)
		RW::writeMemory((LPVOID)((int)(address)+i), nop);
}

void AssemblyLine::reset()
{
	for (int i = 0; i < bytes; i++)
		RW::writeMemory((LPVOID)((int)(address)+i), original[i]);
}

void AssemblyLine::printOriginal()
{
	std::cout << "Original = ";
	for (int i = 0; i < bytes; i++)
		std::cout << (int)original[i] << " ";
	std::cout << "\n";
}

void AssemblyLine::printCurrent()
{
	std::cout << address << " is = ";
	for (int i = 0; i < bytes; i++)
		std::cout << (int)RW::readMemoryByte((LPVOID)((int)(address)+i)) << " ";
	std::cout << "\n";
}

