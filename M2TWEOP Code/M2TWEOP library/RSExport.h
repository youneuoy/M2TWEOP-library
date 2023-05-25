#pragma once
#include "exportHeader.h"
#include "globals.h"
namespace RSExport
{
	typedef bool (*RecruitPointCallbackF)(const char* pointName, const char* unitName);

	EOP_EXPORT void addRecruitPoint(const char*pointName, RecruitPointCallbackF isRecruitNeeded);
	EOP_EXPORT void addUnitIcons(const char*unitName, const char*relativeIcon, const char* relativeImage);
};

