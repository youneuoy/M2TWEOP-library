#pragma once
#if _DEBUG

#pragma comment(lib,"libMinHook.x86D.lib")
#else
#pragma comment(lib,"libMinHook.x86.lib")

#endif
#include "MinHook.h"
#define NOINLINE __declspec(noinline)