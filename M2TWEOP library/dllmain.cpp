// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "managerF.h"
#if _DEBUG

#pragma comment(lib,"AsmJit_debug.lib")
#else
#pragma comment(lib,"AsmJit.lib")

#endif
#pragma comment(lib,"BeaEngine_s.lib")
#pragma comment(lib,"discord_game_sdk.dll.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

