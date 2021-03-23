#pragma once
#include "graphicsD3D.h"
#include "MemWork.h"
#include "Injects.h"
#include "technicalStringsFuncs.h"
#include "patchesForGame.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
#include "plugins.h"


#include "exportHeader.h"
class managerF
{
public:
	static void init();
private:
	static void doPachs();

	static void initThread();

	static bool read_modConfig();
	static bool read_limits();
};

namespace managerExport
{
	NOINLINE EOP_EXPORT void initEOP(const char*modPath,int gameVer);

}