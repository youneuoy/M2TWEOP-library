#include "jsonManager.h"
#include <iomanip>
#include "gameStringHelpers.h"
#include "globals.h"

namespace jsonManager
{
    
    void writeJsonToFile(const std::string& fpath, const jsn::json& json)
    {
        ofstream f1(fpath);
        f1 << setw(4) << json;
        f1.close();
    }
    
    // Sets keys in a JSON file in the mod folder
    void setJsonInFile(const std::string &filePath, const std::string &jsonKey, const std::string &jsonValue)
    {
        try
        {
            const std::string modPath = globals::dataS.modPath;
            const string jsonPath = modPath + filePath;
            jsn::json json = loadJsonFromFile(jsonPath);
            json[jsonKey]=jsonValue;

            writeJsonToFile(jsonPath, json);
            json.clear();
        }
        catch (jsn::json::type_error &e)
        {
            MessageBoxA(nullptr, e.what(), "Error setting data in JSON!", MB_APPLMODAL | MB_SETFOREGROUND);
        }
    }
    
    jsn::json loadJsonFromFile(const std::string& filePath)
    {
        jsn::json json;
        try
        {
            std::ifstream f1(filePath);
            if (f1.is_open())
                f1 >> json;
            f1.close();
            return json;
        }
        catch (jsn::json::exception& e)
        {
            MessageBoxA(nullptr, e.what(), "Could not load JSON from file!", MB_APPLMODAL | MB_SETFOREGROUND);
            return json;
        }
    }
    
    jsn::json loadJsonFromFile(const std::wstring& filePath)
    {
        jsn::json json;
        try
        {

            std::ifstream f1(filePath);
            if (f1.is_open())
                f1 >> json;
            f1.close();
            return json;
        }
        catch (jsn::json::exception& e)
        {
            MessageBoxA(nullptr, e.what(), "Could not load JSON from file!", MB_APPLMODAL | MB_SETFOREGROUND);
            return json;
        }
    }
}
