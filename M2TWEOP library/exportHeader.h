#pragma once
#ifdef M2TWEOPLIBRARY_EXPORTS
#define EOP_EXPORT extern "C" __declspec(dllexport)
#else
#define EOP_EXPORT extern "C" __declspec(dllimport)
#endif