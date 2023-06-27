#pragma once
#include "headersSTD.h"
#include "fastFunctsHelpers.h"

namespace jsonManager
{
    void setJSONInFile(const std::string &fpath, const std::string &jsonKey, const std::string &jsonValue);

    // Given a JSON file, set some variable using a key inside that file
	template<typename Targ>
    void setDataFromJSON(const std::string &fpath, const std::string &jsonKey, Targ &variableToSet)
	{
		try
		{
			jsn::json json;

			std::string modPath = globals::dataS.modPatch;
			string jsonPath = modPath+fpath;

			std::ifstream f1(jsonPath);

			f1 >> json;

			f1.close();

			json.at(jsonKey).get_to(variableToSet);
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Error setting data from JSON!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}
}