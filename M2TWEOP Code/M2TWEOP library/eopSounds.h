#pragma once

namespace eopSounds
{

    void* createEOPSound(const char* path);
    void deleteEOPSound(void* sound);

    void playEOPSound(void* sound);
    void setEOPSoundOffset(void* sound, unsigned int milliseconds);
    unsigned int getEOPSoundOffset(void* sound);
    void pauseEOPSound(void* sound);
    void stopEOPSound(void* sound);
};



