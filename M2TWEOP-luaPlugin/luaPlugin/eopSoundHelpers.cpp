#include "eopSoundHelpers.h"
#include "plugData.h"
void* eopSoundHelpers::createEOPSound(const char* path)
{
	return (*(*plugData::data.functsEopSounds.createEOPSound))(path);
}

void eopSoundHelpers::deleteEOPSound(void* sound)
{
	(*(*plugData::data.functsEopSounds.deleteEOPSound))(sound);
}

void eopSoundHelpers::playEOPSound(void* sound)
{
	(*(*plugData::data.functsEopSounds.playEOPSound))(sound);
}

void eopSoundHelpers::setEOPSoundOffset(void* sound, unsigned int milliseconds)
{
	(*(*plugData::data.functsEopSounds.setEOPSoundOffset))(sound, milliseconds);
}

unsigned int eopSoundHelpers::getEOPSoundOffset(void* sound)
{
	return (*(*plugData::data.functsEopSounds.getEOPSoundOffset))(sound);
}

void eopSoundHelpers::pauseEOPSound(void* sound)
{
	(*(*plugData::data.functsEopSounds.pauseEOPSound))(sound);
}

void eopSoundHelpers::stopEOPSound(void* sound)
{
	(*(*plugData::data.functsEopSounds.stopEOPSound))(sound);
}
