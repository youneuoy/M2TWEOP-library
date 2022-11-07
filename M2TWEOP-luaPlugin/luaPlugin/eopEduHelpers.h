#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace eopEduHelpers
{
	EduEntry* addEopEduEntry(int baseIndex, int newIndex);
	EduEntry* addEopEduEntryFromFile(const char* fileName, int newIndex);
	EduEntry* getEopEduEntry(int index);



	int getDataEopDu(int index);


	void setEntryUnitCardTga(int index, const char* newCard);
	void setEntryInfoCardTga(int index, const char* newCard);
	void setEntrySoldierModel(int index, const char* newModel);

	void setEntryLocalizedName(int index, const char* newLocName);
	void setEntryLocalizedDescr(int index, const char* newLocDescr);
	void setEntryLocalizedShortDescr(int index, const char* newLocShortDescr);


	//edu things
	bool haveAttributeLegioGet(EduEntry* eduEn);
	void haveAttributeLegioSet(EduEntry* eduEn, bool isHaveLegio);
};

