#include "jsonManager.h"
#include "fastFunctsHelpers.h"
#include "globals.h"

namespace jsonManager
{
    // Sets keys in a JSON file in the mod folder
    void setJSONInFile(const std::string &fpath, const std::string &jsonKey, const std::string &jsonValue)
    {
        try
        {
            std::string modPath = globals::dataS.modPath;
            string jsonPath = modPath + fpath;
            jsn::json json = fastFunctsHelpers::loadJsonFromFile(jsonPath);
            json[jsonKey]=jsonValue;

            fastFunctsHelpers::writeJsonToFile(jsonPath, json);
            json.clear();
        }
        catch (jsn::json::type_error &e)
        {
            MessageBoxA(NULL, e.what(), "Error setting data in JSON!", MB_APPLMODAL | MB_SETFOREGROUND);
        }
    }
}
