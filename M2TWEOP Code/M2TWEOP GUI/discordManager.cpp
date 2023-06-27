#include "helpers.h"
#include <regex>

#include "discord.h"
#include "managerG.h"
#include <thread>

discord::Core *discordCore{};

struct
{
    discord::Activity activity{};
    string status = "In Battle";
    string factionName = "Dorwinion";
    string turnNum = "5";
    bool needsUpdate = false;
    time_t last = system_clock::to_time_t(system_clock::now());
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

    auto setModActivityDetails()
    {
        string currentPath;
        helpers::getCurrentPath(currentPath);

        discordData.activity.SetDetails("At the main menu");

        // Divide and Conquer
        if (currentPath.find("Divide") != string::npos)
        {
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer");
        }
        // Tsardoms 2.2
        if (currentPath.find("Tsar") != string::npos)
        {
            discordData.activity.GetAssets().SetLargeText("Tsardoms: Total War");
        }

        discordData.activity.GetAssets().SetSmallText("Engine Overhaul Project");
        discordData.activity.GetAssets().SetLargeImage("large");
        discordData.activity.GetAssets().SetSmallImage("small");

        discordData.activity.SetType(discord::ActivityType::Playing);
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
    void updateActivity()
    {
        discordCore->ActivityManager().UpdateActivity(discordData.activity, [](discord::Result result) {});
    }
    void initDiscordRichPresence()
    {
        auto discordAppId = getModAppID();
        auto response = discord::Core::Create(discordAppId, DiscordCreateFlags_Default, &discordCore);
        setModActivityDetails();
        // readPresenceDetailsFromFile();

        updateActivity();
    }

void updatePresence()
    {
        using namespace  std::chrono;
        using namespace std::literals::chrono_literals;
        time_t currentTime =  system_clock::to_time_t(system_clock::now());

        if(discordData.last+30s> currentTime ) {
            return;
        }

        // If it's been 30 seconds since the last update, do another update
       readPresenceDetailsFromFile();
       updateActivity();
       discordData.last = currentTime;
       discordCore->RunCallbacks();
    }
}
