#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
class helpers
{
public:
	static std::vector<std::string> splitString(const std::string& phrase, const std::string& delimiter);
	static void doEOPPipe(std::string& result, int waitSeconds);
};