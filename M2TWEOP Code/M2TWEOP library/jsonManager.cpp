#include "json.hpp"

using namespace std;
namespace jsn = nlohmann;

namespace jsonManager
{
    jsn::json loadJsonFromFile(const std::string &fpath)
    {
        try
        {
            nlohmann::json json;

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
            // MessageBoxA(NULL, e.what(), "Could not load JSON from file!", MB_APPLMODAL | MB_SETFOREGROUND);
        }
    }

    // Sets keys in a JSON file in the mod folder
    void setJSONInFile(const std::string &fpath, const std::string &jsonKey, const std::string &jsonValue)
    {
	    // std::string fPath = ".\\youneuoy_Data\\m2tweop_temp\\discordRichPresenceData.json";
        std::string modPath = globals::dataS.modPatch;

		jsn::json json = loadJsonFromFile(modPath+=fpath);

		setJson(jsonKey, jsonValue);

		writeJsonToFile(fpath, json);
		json.clear();
    }

    // Given a JSON file, set some variable using a key inside that file
    void setDataFromJSON(const std::string &fpath, const std::string &jsonKey, const std::string variableToSet)
	{
		jsn::json json;

		std::ifstream f1(fpath);

		f1 >> json;

		f1.close();

		try
		{
			json.at(jsonKey).get_to(variableToSet);
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Error setting data from JSON!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}
}