#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace eopSoundHelpers
{
	void* createEOPSound(const char* path);
	void deleteEOPSound(void* sound);

	void playEOPSound(void* sound);
	void setEOPSoundOffset(void* sound, unsigned int milliseconds);
	unsigned int getEOPSoundOffset(void* sound);
	void pauseEOPSound(void* sound);
	void stopEOPSound(void* sound);

};

