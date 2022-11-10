#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace UnitEnums
{
    enum stat_pri_attr :int
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
};
namespace eopEduHelpers
{
	EduEntry* addEopEduEntry(int baseIndex, int newIndex);
	EduEntry* addEopEduEntryFromFile(const char* fileName, int newIndex);
	EduEntry* getEopEduEntry(int index);
	EduEntry* getEduEntry(int index);



	int getDataEopDu(int index);


	void setEntryUnitCardTga(int index, const char* newCard);
	void setEntryInfoCardTga(int index, const char* newCard);
	void setEntrySoldierModel(int index, const char* newModel);
	void setEntryStatPriArmour(int index, int armour, int defense, int shield);
	void setEntryAttackCharge(int index, int attack, int charge);
	int GetEntryAttack(int index);
	int GetEntryCharge(int index);
	int GetArmourUpgradeLevelsNum(int index);
	void SetArmourUpgradeLevelsNum(int index, int amount);
	int GetArmourUpgradeLevel(int idx, int levelidx);
	void SetArmourUpgradeLevel(int idx, int levelidx, int8_t newlevel);
	int GetArmourUpgradeModelsNum(int index);
	void SetArmourUpgradeModelsNum(int index, int amount);
	std::string GetArmourUpgradeModel(int index, int levelidx);
	void SetArmourUpgradeModel(int index, int levelidx, std::string newModel);
	void setEntryStatPriAttribute(int idx, UnitEnums::stat_pri_attr attribute, bool enable);

	void setEntryLocalizedName(int index, const char* newLocName);
	void setEntryLocalizedDescr(int index, const char* newLocDescr);
	void setEntryLocalizedShortDescr(int index, const char* newLocShortDescr);


	//edu things
	bool haveAttributeLegioGet(EduEntry* eduEn);
	void haveAttributeLegioSet(EduEntry* eduEn, bool isHaveLegio);
};



