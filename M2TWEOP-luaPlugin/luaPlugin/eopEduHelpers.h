#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace UnitEnums
{
    enum attackAttr :int
    {
        spear = 1<<2,
        light_spear = 1<<3,
        prec = 1<<4,
        ap = 1<<5,
        bp = 1<<6,
        area = 1<<7,
        fire = 1<<8,
        launching = 1<<9,
        thrown = 1<<10,
        short_pike = 1<<11,
        long_pike = 1<<12,
        spear_bonus_12 = 1<<13,
        spear_bonus_10 = 1<<14,
        spear_bonus_8 = 1<<15,
        spear_bonus_6 = 1<<16,
        spear_bonus_4 = 1<<17
    };
    enum eduStat :int
    {
        armour = 1<<1,
        defense = 1<<7,
        shield = 1<<13,
        attack = 1<<18,
        charge = 1<<24
    };
};
namespace eopEduHelpers
{
	EduEntry* addEopEduEntry(int baseIndex, int newIndex);
	EduEntry* addEopEduEntryFromFile(const char* fileName, int newIndex);
	EduEntry* getEopEduEntry(int index);
	EduEntry* getEduEntry(int index);
	EduEntry* getEduEntryByType(const char* type);
	int getEduIndexByType(const char* type);



	int getDataEopDu(int index);


	void setEntryUnitCardTga(int index, const char* newCard);
	void setEntryInfoCardTga(int index, const char* newCard);
	void setEntrySoldierModel(int index, const char* newModel);
	int getArmourUpgradeLevelsNum(int index);
	void setArmourUpgradeLevelsNum(int index, int amount);
	int getArmourUpgradeLevel(int idx, int levelidx);
	void setArmourUpgradeLevel(int idx, int levelidx, int8_t newlevel);
	int getArmourUpgradeModelsNum(int index);
	void setArmourUpgradeModelsNum(int index, int amount);
	std::string getArmourUpgradeModel(int index, int levelidx);
	void setArmourUpgradeModel(int index, int levelidx, std::string newModel);
	void setEntryAttackAttribute(int idx, UnitEnums::attackAttr attribute, bool enable, int sec);
	bool getEntryAttackAttribute(int idx, UnitEnums::attackAttr attribute, int sec);
	void setEntryStat(int idx, UnitEnums::eduStat stat, int value, int sec);
	int getEntryStat(int idx, UnitEnums::eduStat stat, int sec);

	void setEntryLocalizedName(int index, const char* newLocName);
	void setEntryLocalizedDescr(int index, const char* newLocDescr);
	void setEntryLocalizedShortDescr(int index, const char* newLocShortDescr);


	//edu things
	bool haveAttributeLegioGet(EduEntry* eduEn);
	void haveAttributeLegioSet(EduEntry* eduEn, bool isHaveLegio);
};



