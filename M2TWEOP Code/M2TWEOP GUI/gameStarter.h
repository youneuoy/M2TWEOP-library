#pragma once

#include "headersSTD.h"
#include "dataG.h"
#include "helpers.h"
class gameStarter
{
public:
	static bool startGame();
private:
	static bool runGameExe();

	static void ensureFiles(std::vector<std::wstring>& newFiles, std::vector<std::wstring>& oldFiles);
	static bool initM2TWEOP();
};
