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
    string status = "At the main menu";
    string factionName = "";
    string turnNum = "";
    bool needsUpdate = false;
    time_t last;
    time_t now;
} discordData;

namespace discordManager
{

    /// Try to find in the Haystack the Needle - ignore case
    bool findString(const std::string &strHaystack, const std::string &strNeedle)
    {
        auto it = std::search(
            strHaystack.begin(), strHaystack.end(),
            strNeedle.begin(), strNeedle.end(),
            [](unsigned char ch1, unsigned char ch2)
            { return std::toupper(ch1) == std::toupper(ch2); });
        return (it != strHaystack.end());
    }

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

    auto getModActivityDetails()
    {
        string currentPath;
        helpers::getCurrentPath(currentPath);
        discordData.activity.GetTimestamps().SetStart(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        discordData.activity.SetDetails("At the main menu");
        discordData.activity.GetAssets().SetSmallText("Engine Overhaul Project");
        discordData.activity.GetAssets().SetSmallImage("small");

        discordData.activity.SetType(discord::ActivityType::Playing);

        // Divide and Conquer
        if (findString(currentPath, "divide_and_conquer_ago") || findString(dataG::data.gameData.modTitle, "ago") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer: AGO");
            return 1123366117088432259;
        }
        // Reforged
        else if (findString(currentPath, "reforged") || findString(dataG::data.gameData.modTitle, "reforged") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Third Age: Reforged");
            return 1123946532085969009;
        }
        // Third Age: Total War (Vanilla)
        else if (findString(currentPath, "tatw") || findString(dataG::data.gameData.modTitle, "third age") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Third Age: Total War");
            return 1123960645092651009;
        }
        // DCI: Last Alliance
        else if (findString(currentPath, "reforged") || findString(dataG::data.gameData.modTitle, "reforged") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("DCI: Last Alliance");
            return 1123960587660038214;
        }
        // Tsardoms
        else if (findString(currentPath, "tsardoms") || findString(dataG::data.gameData.modTitle, "tsardoms") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Tsardoms: Total War");
            return 1122265309357817967;
        }
        // AGO
        else if (findString(currentPath, "divide_and_conquer") || findString(dataG::data.gameData.modTitle, "divide and conquer") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer");
            return 1122246166642425857;
        }
        // Insularis Draco
        else if (findString(currentPath, "insularis") || findString(dataG::data.gameData.modTitle, "insularis") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Insularis Draco");
            return 1123941224819200000;
        }
        // Elder Scrolls TW
        else if (findString(currentPath, "elder_scrolls") || findString(dataG::data.gameData.modTitle, "elder scrolls") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("The Elder Scrolls: Total War");
            return 1123941440939102300;
        }
        // BOTET
        else if (findString(currentPath, "botet") || findString(dataG::data.gameData.modTitle, "call of warhammer") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Call of Warhammer: Beginning of The End Times");
            return 1123941826789900288;
        }
        // TATW 4.0
        else if (findString(currentPath, "third_age_4") || findString(dataG::data.gameData.modTitle, "third age 4") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Third Age: Total War Extended (4.0)");
            return 1123942808609374270;
        }
        // Stainless Steel
        else if (findString(currentPath, "SS6.3") || findString(dataG::data.gameData.modTitle, "stainless steel") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Stainless Steel");
            return 1123943657477779538;
        }
        // Game of Thrones: Total War
        else if (findString(currentPath, "GoT") || findString(dataG::data.gameData.modTitle, "game of thrones") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Game of Thrones: Total War");
            return 1123943733470179488;
        }
        // Warcraft: Total War
        else if (findString(currentPath, "WTW") || findString(dataG::data.gameData.modTitle, "warcraft") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Westeros: Total War");
            return 1123943733470179488;
        }
        // EB2
        else if (findString(currentPath, "ebii") || findString(dataG::data.gameData.modTitle, "europa") == true)
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Europa Barbarorum II");
            return 1123960787040469012;
        }
        // Default Medieval II with Rich Presence
        else
        {
            discordData.activity.GetAssets().SetLargeImage("large");
            discordData.activity.GetAssets().SetLargeText("Medieval II: Total War");
            return 1123940609783234670;
        }
    }

    void readPresenceDetailsFromFile()
    {
        try
        {
            // Read the campaign data from the file (written by M2TWEOP Library)
            std::string fPath = ".\\eopData\\discordRichPresenceCfg.json";
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
                "Playing as '%s' on Turn %s",
                discordData.factionName.c_str(),
                discordData.turnNum.c_str());

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
