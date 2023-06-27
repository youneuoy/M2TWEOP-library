#pragma once
#include "headersSTD.h"
#include "fastFunctsHelpers.h"

namespace jsonManager
{
    void setJSONInFile(const std::string &fpath, const std::string &jsonKey, const std::string &jsonValue);
	void setDataFromJSON(const std::string &fpath, const std::string &jsonKey, const std::string variableToSet);
}