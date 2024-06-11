#pragma once
#include <memory>
#include <string>
#include "realGameTypes.h"

namespace cultures
{
    enum class portraitType
    {
        civilians = 0,
        generals = 1,
        rogues = 2,
        princesses = 3,
        merchants = 4,
        priests = 5,
        heretics = 6,
        witches = 7,
        inquisitors = 8,
        MAX
    };
    
    enum class portraitAge
    {
        young = 0,
        old = 1,
        dead = 2,
    };

    int getCultureCount();
    std::string getCultureName(int cultureID);
    
    struct eopPortraitDb
    {
        static std::vector<std::shared_ptr<portraitDbEntry>> portraits;
        static int entryCount;
        static portraitDbEntry* getEntry(int cultureID);
        static void createEopPortraitDb();
    };
}
