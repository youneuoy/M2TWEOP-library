#include "helpers.h"
#include <regex>

#include "discord.h"
#include "managerG.h"
#include <thread>

using namespace std::chrono;
using namespace std::literals::chrono_literals;

discord::Core *discordCore{};

struct
{
    discord::Activity activity{};
    string status = "In Battle";
    string factionName = "Dorwinion";
    string turnNum = "5";
    bool needsUpdate = false;
    time_t last;
    time_t now;
} discordData;

namespace discordManager
{

    template <typename... Args>
    std::string string_format(const std::string &format, Args... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
        if (size_s <= 0)
        {
            throw std::runtime_error("Error during formatting.");
        }
        auto size = static_cast<size_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }

    jsn::json loadJsonFromFile(const std::string &fpath)
    {
        try
        {
            jsn::json json;

            std::ifstream f1(fpath);
            if (f1.is_open())
            {
                f1 >> json;
            }
            f1.close();
            return std::move(json);
        }
        catch (jsn::json::exception &e)
        {
            MessageBoxA(NULL, e.what(), "Could not load JSON from file!", MB_APPLMODAL | MB_SETFOREGROUND);
        }
    }

    int getModActivityDetails()
    {
        string currentPath;
        helpers::getCurrentPath(currentPath);
        discordData.activity.SetDetails("At the main menu");
        discordData.activity.GetAssets().SetSmallText("Engine Overhaul Project");
        discordData.activity.GetAssets().SetSmallImage("small");

        discordData.activity.SetType(discord::ActivityType::Playing);

        // Divide and Conquer
        if (std::tolower(currentPath).find("divide_and_conquer_ago") != string::npos || std::tolower(dataG::data.gameData.modTitle).find("ago") != string::npos)
        {
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer: AGO");
            discordData.activity.GetAssets().SetLargeImage("large");
            return 1122246166642425857;
        }
        // Tsardoms
        if (std::tolower(currentPath).find("tsardoms") != string::npos || std::tolower(dataG::data.gameData.modTitle).find("tsardoms") != string::npos)
        {
            discordData.activity.GetAssets().SetLargeText("Tsardoms: Total War");
            discordData.activity.GetAssets().SetLargeImage("large");
            return 1122265309357817967;
        }
        // AGO
        if (std::tolower(currentPath).find("divide_and_conquer") != string::npos || std::tolower(dataG::data.gameData.modTitle).find("divide and conquer") != string::npos)
        {
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer");
            discordData.activity.GetAssets().SetLargeImage("large");
            return 1123366117088432259;
        }
    }

    void readPresenceDetailsFromFile()
    {
        try
        {
            // Read the campaign data from the file (written by M2TWEOP Library)
            std::string fPath = ".\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json";
            jsn::json json = loadJsonFromFile(fPath);
            std::string jsonStringValue;
            if (json.contains("factionName"))
            {
                getJson(discordData.factionName, "factionName");
            }
            if (json.contains("turnNum"))
            {
                getJson(discordData.turnNum, "turnNum");
            }
            if (json.contains("status"))
            {
                getJson(discordData.status, "status");
            }
        }
        catch (jsn::json::exception &e)
        {
            MessageBoxA(NULL, e.what(), "Could not read presence details from file!", MB_APPLMODAL | MB_SETFOREGROUND);
        }

        try
        {
            // Create a formatted string with the campaign details
            std::string details = string_format(
                "Playing as: %s",
                discordData.factionName.c_str());

            // Update the state and details of the Rich Presence with details from the file
            discordData.activity.SetState(discordData.status.c_str());
            discordData.activity.SetDetails(details.c_str());
        }
        catch (const std::exception &e)
        {
            MessageBoxA(NULL, e.what(), "Could not update activity details from file!", MB_APPLMODAL | MB_SETFOREGROUND);
        }
    }

    void initDiscordRichPresence()
    {
        discordData.last = system_clock::to_time_t(system_clock::now());
        auto discordAppId = getModActivityDetails();
        auto response = discord::Core::Create(discordAppId, DiscordCreateFlags_Default, &discordCore);
        // readPresenceDetailsFromFile();
        discordCore->ActivityManager().UpdateActivity(discordData.activity, [](discord::Result result) {});
        ::discordCore->RunCallbacks();
    }

    void updatePresence()
    {
        if (difftime(time(&discordData.now), discordData.last) > 5)
        {
            // If it's been 30 seconds since the last update, do another update
            readPresenceDetailsFromFile();
            discordCore->ActivityManager().UpdateActivity(discordData.activity, [](discord::Result result) {});

            discordData.last = time(&discordData.now);
        }
        ::discordCore->RunCallbacks();
    }
}
