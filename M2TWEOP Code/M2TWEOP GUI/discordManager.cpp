#include "helpers.h"

#include "discord.h"
discord::Core *core{};

namespace discordManager
{
    auto getModAppID()
    {
        string currentPath;
        helpers::getCurrentPath(currentPath);

        if (currentPath.find("Divide") != string::npos)
        {
            return 1122246166642425857;
        }
        // Tsardoms 2.2
        if (currentPath.find("Tsar") != string::npos)
        {
            return 1122246166642425857;
        }
    }

    auto getModActivityDetails()
    {
        string currentPath;
        helpers::getCurrentPath(currentPath);

        discord::Activity activity{};
        activity.SetDetails("At the main menu");

        // Divide and Conquer
        if (currentPath.find("Divide") != string::npos)
        {
            activity.GetAssets().SetLargeText("Divide and Conquer");
        }
        // Tsardoms 2.2
        if (currentPath.find("Tsar") != string::npos)
        {
            activity.GetAssets().SetLargeText("Tsardoms: Total War");
        }

        activity.GetAssets().SetSmallText("Engine Overhaul Project");
        activity.GetAssets().SetLargeImage("large");
        activity.GetAssets().SetSmallImage("small");

        activity.SetType(discord::ActivityType::Playing);

        return activity;
    }

    void initDiscordRichPresence()
    {
        auto discordAppId = getModAppID();
        auto response = discord::Core::Create(discordAppId, DiscordCreateFlags_Default, &core);

        auto activity = getModActivityDetails();

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
    }

    void updatePresence()
    {
        // Every 30 seconds, read a file containing the state the game and update the presence
        ::core->RunCallbacks();
    }
}
