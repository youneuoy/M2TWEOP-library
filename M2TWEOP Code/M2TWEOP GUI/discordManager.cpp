#include "helpers.h"
#include <regex>

#include "discord.h"
#include "managerG.h"
#include <chrono>

using namespace std::chrono;
using namespace std::literals::chrono_literals;

discord::Core *discordCore{};

struct
{
    discord::Activity activity{};
    string status = "At the main menu";
    string factionName = "Unknown";
    string turnNum = "Unknown";
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
        discordData.activity.GetAssets().SetLargeImage("large");

        discordData.activity.SetType(discord::ActivityType::Playing);

        // Divide and Conquer (AGO)
        if (findString(dataG::data.gameData.modTitle, "Divide and Conquer: AGO") || findString(currentPath, "divide_and_conquer_ago") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer: AGO");
            return 1123366117088432259;
        }
        // Third Age: Reforged
        else if (findString(dataG::data.gameData.modTitle, "reforged") || findString(currentPath, "reforged")  == true)
        {
            discordData.activity.GetAssets().SetLargeText("Third Age: Reforged");
            return 1123946532085969009;
        }
        // Third Age: Total War Extended (4.0)
        else if ( findString(dataG::data.gameData.modTitle, "third age extended")  || findString(currentPath, "third_age_4") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Third Age: Total War Extended (4.0)");
            return 1123942808609374270;
        }
        // Third Age: Total War (Vanilla)
        else if (findString(dataG::data.gameData.modTitle, "third age") || findString(currentPath, "tatw") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Third Age: Total War");
            return 1123960645092651009;
        }
        // DCI: Last Alliance
        else if (findString(dataG::data.gameData.modTitle, "dci") || findString(currentPath, "dci") == true)
        {
            discordData.activity.GetAssets().SetLargeText("DCI: Last Alliance");
            return 1123960587660038214;
        }
        // Tsardoms
        else if (findString(dataG::data.gameData.modTitle, "tsardoms") || findString(currentPath, "tsardoms") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Tsardoms: Total War");
            return 1122265309357817967;
        }
        // Divide and Conquer (Vanilla)
        else if (findString(dataG::data.gameData.modTitle, "divide and conquer") || findString(currentPath, "divide_and_conquer")  == true)
        {
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer");
            return 1122246166642425857;
        }
        // Insularis Draco
        else if (findString(dataG::data.gameData.modTitle, "insularis")  || findString(currentPath, "insularis") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Insularis Draco");
            return 1123941224819200000;
        }
        // Elder Scrolls TW
        else if (findString(dataG::data.gameData.modTitle, "elder scrolls")  || findString(currentPath, "elder_scrolls") == true)
        {
            discordData.activity.GetAssets().SetLargeText("The Elder Scrolls: Total War");
            return 1123941440939102300;
        }
        // Call of Warhammer: Beginning of The End Times
        else if (findString(dataG::data.gameData.modTitle, "warhammer") || findString(currentPath, "botet") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Call of Warhammer: Beginning of The End Times");
            return 1123941826789900288;
        }
        // Stainless Steel
        else if (findString(dataG::data.gameData.modTitle, "stainless steel") || findString(currentPath, "ss6.3")  == true)
        {
            discordData.activity.GetAssets().SetLargeText("Stainless Steel");
            return 1123943657477779538;
        }
        // Game of Thrones: Total War
        else if ( findString(dataG::data.gameData.modTitle, "game of thrones")  || findString(currentPath, "got") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Game of Thrones: Total War");
            return 1123943733470179488;
        }
        // Warcraft: Total War
        else if (findString(dataG::data.gameData.modTitle, "warcraft") || findString(currentPath, "wtw") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Warcraft: Total War");
            return 1124054222720208947;
        }
        // Europa Barbarorum II
        else if (findString(dataG::data.gameData.modTitle, "europa")  || findString(currentPath, "ebii") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Europa Barbarorum II");
            return 1123960787040469012;
        }
        // Rusichi: Total War
        else if (findString(dataG::data.gameData.modTitle, "rusichi") || findString(currentPath, "rusichi") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Rusichi: Total War");
            return 1123991624096161955;
        }
        // De Bello Mundi
        else if (findString(dataG::data.gameData.modTitle, "mundi")  || findString(currentPath, "dbm") == true)
        {
            discordData.activity.GetAssets().SetLargeText("De Bello Mundi: Total War");
            return 1124055063518789722;
        }
        // Chivalry II: The Sicilian Vespers
        else if (findString(dataG::data.gameData.modTitle, "vespers") || findString(currentPath, "sicilianvespers") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Chivalry II: The Sicilian Vespers");
            return 1124057160662405231;
        }
        // Divide and Conquer: AGO (SuS)
        else if (findString(dataG::data.gameData.modTitle, "SuS") || findString(currentPath, "SuS") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Divide and Conquer: SUS");
            return 1123978207302004806;
        }
        // Medieval 2: DenMod
        else if (findString(dataG::data.gameData.modTitle, "DenMod") || findString(currentPath, "DenMod") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Medieval 2: Total War (DenMod)");
            return 1123977758335320134;
        }
        // Rise of Three Kingdoms
        else if (findString(dataG::data.gameData.modTitle, "Three Kingdoms") || findString(currentPath, "rotk") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Rise of Three Kingdoms: Total War");
            return 1123977966544764960;
        }
        // Dragon Age: Total War
        else if (findString(dataG::data.gameData.modTitle, "Dragon Age") || findString(currentPath, "dragon_age") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Dragon Age: Total War");
            return 1123981386727051405;
        }
        // Broken Crescent
        else if (findString(dataG::data.gameData.modTitle, "Broken Crescent") || findString(currentPath, "broken_crescent") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Broken Crescent");
            return 1123991572472676362;
        }
        // Silmarillion: Total War
        else if (findString(dataG::data.gameData.modTitle, "Silmarillion") || findString(currentPath, "Silmarillion") == true)
        {
            discordData.activity.GetAssets().SetLargeText("Silmarillion: Total War");
            return 1123978014082990140;
        }
        // The Great Conflicts
        else if (findString(dataG::data.gameData.modTitle, "Great Conflicts") || findString(currentPath, "tgc ") == true)
        {
            discordData.activity.GetAssets().SetLargeText("The Great Conflicts");
            return 1124058190682791997;
        }
        // Default Medieval II
        else
        {
            discordData.activity.GetAssets().SetLargeText("Medieval II: Total War");
            return 1123940609783234670;
        }
    }

    void readPresenceDetailsFromFile()
    {
        try
        {
            // Read the campaign data from the file (written by M2TWEOP Library)
            std::string fPath = ".\\eopData\\config\\discordRichPresenceCfg.json";
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
                "%s - Turn %s",
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

        // Check if Discord is running, if it isn't just turn off the Rich Presence
        discord::Result response = discord::Core::Create(discordAppId, DiscordCreateFlags_NoRequireDiscord, &discordCore);
        // If Discord is running, start doing Rich Presence stuff
        if (discordCore) {
            readPresenceDetailsFromFile();
            discordCore->ActivityManager().UpdateActivity(discordData.activity, [](discord::Result result) {});
            ::discordCore->RunCallbacks();
        }    
    }

    void updatePresence()
    {
        if (!discordCore) {
            return;
        }

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
