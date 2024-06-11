#pragma once
#include"headersSTD.h"
#include "realGameTypes.h"
namespace eduFastFuncts
{
	int readEduFile(std::string fileName,eduEntry*entryForFilling);
	static void prepareEduEntry(eduEntry*& entryForFilling);
};

