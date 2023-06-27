#include "jsonManager.h"

namespace jsonManager
{
    // Sets keys in a JSON file in the mod folder
    void setJSONInFile(const std::string &fpath, const std::string &jsonKey, const std::string &jsonValue)
    {
        std::string modPath = globals::dataS.modPatch;

		jsn::json json = loadJsonFromFile(modPath+=fpath);

		setJson(jsonKey, jsonValue);

		fastFunctsHelpers::writeJsonToFile(fpath, json);
		json.clear();
    }

    // Given a JSON file, set some variable using a key inside that file
    void setDataFromJSON(const std::string &fpath, const std::string &jsonKey, const std::string variableToSet)
	{
		jsn::json json;

        std::string modPath = globals::dataS.modPatch;

		std::ifstream f1(modPath+=fpath);

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