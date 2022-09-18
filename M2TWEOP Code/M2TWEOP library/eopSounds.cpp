#include "eopSounds.h"

NOINLINE EOP_EXPORT void* eopSounds::createEOPSound(const char* path)
{
    sf::SoundBuffer* buffer=new sf::SoundBuffer();
    if (!buffer->loadFromFile(path))
    {
        delete buffer;
        return nullptr;
    }

    return new sf::Sound(*buffer);
}

NOINLINE EOP_EXPORT void eopSounds::deleteEOPSound(void* sound)
{
    if (sound == nullptr)
    {
        return;
    }
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    sf::SoundBuffer* buffer = (sf::SoundBuffer*)snd->getBuffer();

    delete snd;
    delete buffer;
}

NOINLINE EOP_EXPORT void eopSounds::playEOPSound(void* sound)
{
    if (*dataOffsets::offsets.audioEnable == false)
    {
        return;
    }
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->setVolume((float)*dataOffsets::offsets.audioMaster_vol);
    snd->play();
}

NOINLINE EOP_EXPORT void eopSounds::setEOPSoundOffset(void* sound, unsigned int milliseconds)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->setVolume((float)*dataOffsets::offsets.audioMaster_vol);
    snd->setPlayingOffset(sf::milliseconds(milliseconds));
}

NOINLINE EOP_EXPORT unsigned int eopSounds::getEOPSoundOffset(void* sound)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    return snd->getPlayingOffset().asMilliseconds();
}

NOINLINE EOP_EXPORT void eopSounds::pauseEOPSound(void* sound)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->pause();
}

NOINLINE EOP_EXPORT void eopSounds::stopEOPSound(void* sound)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->stop();
}
        