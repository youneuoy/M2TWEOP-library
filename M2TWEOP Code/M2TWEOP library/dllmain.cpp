// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "managerF.h"

//sfml(for audio)
#if _DEBUG
#pragma comment(lib,"sfml-system-s-d.lib")
#pragma comment(lib,"sfml-audio-s-d.lib")
#else
#pragma comment(lib,"sfml-system-s.lib")
#pragma comment(lib,"sfml-audio-s.lib")

#endif
#pragma comment(lib,"vorbisfile.lib")
#pragma comment(lib,"vorbisenc.lib")
#pragma comment(lib,"vorbis.lib")
#pragma comment(lib,"openal32.lib")
#pragma comment(lib,"ogg.lib")
#pragma comment(lib,"flac.lib")



#if _DEBUG

#pragma comment(lib,"AsmJit_debug.lib")
#else
#pragma comment(lib,"AsmJit.lib")

#endif
#pragma comment(lib,"BeaEngine_s.lib")
#pragma comment(lib,"libfbxsdk.lib")
//#pragma comment(lib,"discord_game_sdk.dll.lib")
//
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

