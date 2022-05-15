#pragma once

#include "headersSTD.h"
#include "realGameTypes.h"
#include "fastFuncts.h"
#include "exportHeader.h"
#include "functionsOffsets.h"
#include "dataOffsets.h"
namespace tilesChange
{
	void onTilesCheck(int* coords);

	NOINLINE EOP_EXPORT void replaceTile(const char* tileLabel, UINT32 x, UINT32 y, const char* fileName, const char* weather, const char* dayTime);
};

