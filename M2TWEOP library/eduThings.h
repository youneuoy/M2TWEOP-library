#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include "FastFuncts.h" 
#include "eventsCodes.h" 
#include "globals.h"

#include "techFuncs.h"
#include "fastFuncts.h"
#include "smallFuncs.h"
#include <sstream>
#include <unordered_set>
#include <filesystem>
#include "realGameTypes.h"
#include "eduFastFuncts.h"
using namespace std;

namespace eduThings
{
	NOINLINE EOP_EXPORT EduEntry* addEopEduEntry(int baseIdx , int newIdx);
	NOINLINE EOP_EXPORT EduEntry* getEopEduEntry(int idx);
	//used for creatinon of new units, etc
	NOINLINE EOP_EXPORT int getDataEopEdu(int idx);



	NOINLINE EOP_EXPORT void setEntryUnitCardTga(int entryIdx,const char*newCard);
	NOINLINE EOP_EXPORT void setEntryInfoCardTga(int entryIdx,const char*newCard);
	NOINLINE EOP_EXPORT void setEntrySoldierModel(int entryIdx,const char*newModel);
	NOINLINE EOP_EXPORT void setEntryLocalizedName(int entryIdx,const char*newName);
	NOINLINE EOP_EXPORT void setEntryLocalizedDescr(int entryIdx,const char*newDesr);
	NOINLINE EOP_EXPORT void setEntryLocalizedShortDescr(int entryIdx,const char*newDecrShort);
};

