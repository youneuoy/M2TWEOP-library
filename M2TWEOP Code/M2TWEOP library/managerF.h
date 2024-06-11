#pragma once
#include "exportHeader.h"

class managerF
{
public:
	static void init();

	static void debug();
	static void loadJsonSettings();
private:
	static void doPachs();

	static void initThread();


};

namespace managerExport
{
	EOP_EXPORT void initEOP(const char* modPath, int gameVer);

}