#pragma once
#include "exportHeader.h"

class managerF
{
public:;
	static void initThread();
	static void debug();
	static void loadJsonSettings();
private:
	static void execPatches();
};

namespace managerExport
{
	EOP_EXPORT void initEOP(const wchar_t* modPath, int gameVer);

}