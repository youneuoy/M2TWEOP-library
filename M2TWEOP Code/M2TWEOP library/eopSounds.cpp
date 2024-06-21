#include "pch.h"
#include "eopSounds.h"
#include "realGameTypes.h"

#include <SFML/Audio.hpp>

#include "dataOffsets.h"

void* eopSounds::createEOPSound(const char* path)
{
    sf::SoundBuffer* buffer=new sf::SoundBuffer();
    if (!buffer->loadFromFile(path))
    {
        delete buffer;
        return nullptr;
    }

    return new sf::Sound(*buffer);
}

void eopSounds::deleteEOPSound(void* sound)
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

void eopSounds::playEOPSound(void* sound)
{
    if (*dataOffsets::offsets.audioEnable == false)
    {
        return;
    }
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->setVolume((float)*dataOffsets::offsets.audioMaster_vol);
    snd->play();
}

void eopSounds::setEOPSoundOffset(void* sound, unsigned int milliseconds)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->setVolume((float)*dataOffsets::offsets.audioMaster_vol);
    snd->setPlayingOffset(sf::milliseconds(milliseconds));
}

unsigned int eopSounds::getEOPSoundOffset(void* sound)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    return snd->getPlayingOffset().asMilliseconds();
}

void eopSounds::pauseEOPSound(void* sound)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->pause();
}

void eopSounds::stopEOPSound(void* sound)
{
    sf::Sound* snd = reinterpret_cast<sf::Sound*>(sound);
    snd->stop();
}
        