#pragma once
#include <windows.h>
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <list>
#include "RW.h"


class AssemblyLine
{
private:
	LPVOID address;
	int bytes;
	byte *original;

public:
	AssemblyLine() {};
	AssemblyLine(LPVOID addr);

	void nop();

	void reset();

	void printOriginal();

	void printCurrent();
};

