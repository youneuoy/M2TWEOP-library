#pragma once
#include <string>
#include "realGameTypes.h"
#include "luaGetSetFuncs.h"
namespace UnitEnums
{
    enum attackAttr :int
    {
		nothing = 0,
        spear = 1<<2, //4
        light_spear = 1<<3, //8
        prec = 1<<4, //16
        ap = 1<<5, //32
        bp = 1<<6, //64
        area = 1<<7, //128
        fire = 1<<8, //256
        launching = 1<<9, //512
        thrown = 1<<10, //1024
        short_pike = 1<<11, //2048
        long_pike = 1<<12, //4096
        spear_bonus_12 = 1<<13, //8192
        spear_bonus_10 = 1<<14, //16384
        spear_bonus_8 = 1<<15, //32768
        spear_bonus_6 = 1<<16, //65536
        spear_bonus_4 = 1<<17 //131072
    };
    enum eduStat :int
    {
		none = 0,
        armour = 1<<1, //2
        defense = 1<<7, //128
        shield = 1<<13, //8192
        attack = 1<<18, //262144
        charge = 1<<24 //16777216
    };
};
namespace eopEduHelpers
{
	int hasOwnership(eduEntry* entry, int factionID);
	int getDataEopDu(int index);
	void setOwnerShip(eduEntry* entry, int factionID, bool set);


	int getArmourUpgradeLevelsNum(int index);
	void setArmourUpgradeLevelsNum(int index, int amount);
	int getArmourUpgradeLevel(int idx, int levelidx);
	void setArmourUpgradeLevel(int idx, int levelidx, int8_t newlevel);
	int getArmourUpgradeModelsNum(int index);
	void setArmourUpgradeModelsNum(int index, int amount);
	std::string getArmourUpgradeModel(int index, int levelidx);
	void setArmourUpgradeModel(int index, int levelidx, const std::string& newModel);
	void setEntryAttackAttribute(int idx, UnitEnums::attackAttr attribute, bool enable, int sec);
	bool getEntryAttackAttribute(int idx, UnitEnums::attackAttr attribute, int sec);
	void setEntryStat(int idx, UnitEnums::eduStat stat, int value, int sec);
	int getEntryStat(int idx, UnitEnums::eduStat stat, int sec);
	int multiplexor(int n1, int n2, int sel);
	std::string getPrimaryAnim(const eduEntry* entry);
	std::string getSecondaryAnim(const eduEntry* entry);
	bool hasAttribute(unit* unit, const char* attributeName);
	bool hasAttributeEdu(eduEntry* entry, const char* attributeName);
	
	//edu things
	bool haveAttributeLegioGet(eduEntry* eduEn);
	void haveAttributeLegioSet(eduEntry* eduEn, bool isHaveLegio);
};



