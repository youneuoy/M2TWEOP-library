#pragma once
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <list>
#include "assemblyLine.h"

static HWND hwnd;
static DWORD pId;
static HANDLE hProc;
static HANDLE *hThreads;
static unsigned int hThread_length;

class RW
{
public:
    // INIT
    static void init(LPCWSTR  proc);

    static void terminate(std::string reason);

    //readers and writers
    static byte readMemoryByte(LPVOID address);
    static float readMemoryFloat(LPVOID address);
    static int readMemoryInt(LPVOID address);
    static bool readMemoryBool(LPVOID address);

    static bool writeMemory(LPVOID address, byte data);
    static bool writeMemory(LPVOID address, float data);
    static bool writeMemory(LPVOID address, int data);

    static void suspend();
    static void resume();

    //Preferable, but does not work :( so dont use
    //template<class T>
    //static T readMemory(LPVOID address);
    //template<class G>
    //static bool writeMemory(LPVOID address, G data);

    

};