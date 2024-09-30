#pragma once
#include "headersSTD.h"

namespace jsonManager
{
    void setJsonInFile(const std::string& filePath, const std::string& jsonKey, const std::string& jsonValue);
    jsn::json loadJsonFromFile(const std::string& filePath);
}