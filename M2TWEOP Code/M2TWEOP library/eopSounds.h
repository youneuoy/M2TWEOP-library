#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include "realGameTypes.h"

#include <SFML/Audio.hpp>
using namespace std;
namespace eopSounds
{

	NOINLINE EOP_EXPORT void* createEOPSound(const char* path);
	NOINLINE EOP_EXPORT void deleteEOPSound(void* sound);

	NOINLINE EOP_EXPORT void playEOPSound(void* sound);
	NOINLINE EOP_EXPORT void setEOPSoundOffset(void* sound, unsigned int milliseconds);
	NOINLINE EOP_EXPORT unsigned int getEOPSoundOffset(void* sound);
	NOINLINE EOP_EXPORT void pauseEOPSound(void* sound);
	NOINLINE EOP_EXPORT void stopEOPSound(void* sound);
};



