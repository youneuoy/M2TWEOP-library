#include "eopEduHelpers.h"
#include <string>
#include "luaGetSetFuncs.h"
#include "plugData.h"



EduEntry* eopEduHelpers::addEopEduEntry(int baseIndex, int newIndex)
{
    return (*(*plugData::data.funcsEopEdu.addEopEduEntry))(baseIndex, newIndex);
}
EduEntry* eopEduHelpers::addEopEduEntryFromFile(const char* fileName, int newIndex)
{
    return (*(*plugData::data.funcsEopEdu.addEopEduEntryFromFile))(fileName, newIndex);
}

EduEntry* eopEduHelpers::getEopEduEntry(int index)
{
    return (*(*plugData::data.funcsEopEdu.getEopEduEntry))(index);
}
EduEntry* eopEduHelpers::getEduEntry(int index)
{
    if (index < 500)
    {
        return (*(*plugData::data.funcsEopEdu.getEduEntry))(index);
    }
    else
    {
        return (*(*plugData::data.funcsEopEdu.getEopEduEntry))(index);
    }
}
EduEntry* eopEduHelpers::getEduEntryByType(const char* type)
{
    return (*(*plugData::data.funcsEopEdu.getEduEntryByType))(type);
}
int eopEduHelpers::getEduIndexByType(const char* type)
{
    return (*(*plugData::data.funcsEopEdu.getEduIndexByType))(type);
}

int eopEduHelpers::getDataEopDu(int index)
{
    return (*(*plugData::data.funcsEopEdu.getDataEopEdu))(index);
}

void eopEduHelpers::setEntryUnitCardTga(int index, const char* newCard)
{
    (*(*plugData::data.funcsEopEdu.setEntryUnitCardTga))(index, newCard);

}

void eopEduHelpers::setEntryInfoCardTga(int index, const char* newCard)
{
    (*(*plugData::data.funcsEopEdu.setEntryInfoCardTga))(index, newCard);
}

void eopEduHelpers::setEntrySoldierModel(int index, const char* newModel)
{
    (*(*plugData::data.funcsEopEdu.setEntrySoldierModel))(index, newModel);
}

void eopEduHelpers::setEntryLocalizedName(int index, const char* newLocName)
{
    (*(*plugData::data.funcsEopEdu.setEntryLocalizedName))(index, newLocName);
}

void eopEduHelpers::setEntryLocalizedDescr(int index, const char* newLocDescr)
{
    (*(*plugData::data.funcsEopEdu.setEntryLocalizedDescr))(index, newLocDescr);
}

void eopEduHelpers::setEntryLocalizedShortDescr(int index, const char* newLocShortDescr)
{
    (*(*plugData::data.funcsEopEdu.setEntryLocalizedShortDescr))(index, newLocShortDescr);
}

bool eopEduHelpers::haveAttributeLegioGet(EduEntry* eduEn)
{
    return ((eduEn->Attributes6&10) != 0);
}

void eopEduHelpers::haveAttributeLegioSet(EduEntry* eduEn, bool isHaveLegio)
{
    if (isHaveLegio)
    {
        eduEn->Attributes6 = eduEn->Attributes6 | 0x10;
    }
    else
    {
        eduEn->Attributes6 = eduEn->Attributes6 & 0x10;
    }
}

int eopEduHelpers::getArmourUpgradeLevelsNum(int idx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    return (eduEn->ArmorUpgradesEnd - eduEn->ArmorUpgradeLevels);
}

void eopEduHelpers::setArmourUpgradeLevelsNum(int idx, int amount)
{
    if (amount > 0)
    {
        EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
        int curramount = getArmourUpgradeLevelsNum(idx);
        eduEn->ArmorUpgradesEnd = eduEn->ArmorUpgradeLevels + amount;
        eduEn->ArmorUpgrade2 = eduEn->ArmorUpgradeLevels + amount;
        if (amount > curramount)
        {
            int8_t last_upgrade = *(int8_t*)((eduEn->ArmorUpgradeLevels) + (curramount-1));
            for (int i = 0; i < amount - curramount;i++)
            {
                *(int8_t*)((eduEn->ArmorUpgradeLevels) + curramount + i) = last_upgrade;
            }
        }
    }
}

int eopEduHelpers::getArmourUpgradeLevel(int idx, int levelidx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    int LevelsNum = eopEduHelpers::getArmourUpgradeLevelsNum(idx);

    if (levelidx < LevelsNum)
    {
        return *(int8_t*)((eduEn->ArmorUpgradeLevels) + levelidx);
    }

}

void eopEduHelpers::setArmourUpgradeLevel(int idx, int levelidx, int8_t newlevel)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    int LevelsNum = eopEduHelpers::getArmourUpgradeLevelsNum(idx);

    if (levelidx < LevelsNum)
    {
        *(int8_t*)((eduEn->ArmorUpgradeLevels) + levelidx) = newlevel;
    }

}

int eopEduHelpers::getArmourUpgradeModelsNum(int idx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    return ((eduEn->ArmorUpgradeModelsEnd - (int*)eduEn->ArmorUpgradeModels))/2;
}

void eopEduHelpers::setArmourUpgradeModelsNum(int idx, int amount)
{
    if (amount > 0 && amount < 5)
    {
        amount = amount*2;
        EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
        int curramount = getArmourUpgradeModelsNum(idx);
        eduEn->ArmorUpgradeModelsEnd = (int*)eduEn->ArmorUpgradeModels + (amount);
        eduEn->ArmorUpgrade5 = (int*)eduEn->ArmorUpgradeModels + (amount);
        if (amount > (curramount*2))
        {
            std::string last_upgrade = getArmourUpgradeModel(idx, (curramount-1));
            for (int i = 0; i < amount/2 - curramount;i++)
            {
                setArmourUpgradeModel(idx, (curramount)+i, last_upgrade);
            }
        }
    }
}

std::string eopEduHelpers::getArmourUpgradeModel(int idx, int levelidx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    char* modelname = nullptr;
    switch(levelidx)
    {
        case 3:
            modelname = eduEn->ArmorUpgradeModels->UpgradeThree;
            break;
        case 2:
            modelname = eduEn->ArmorUpgradeModels->UpgradeTwo;
            break;
        case 1:
            modelname = eduEn->ArmorUpgradeModels->UpgradeOne;
            break;
        case 0:
            modelname = eduEn->ArmorUpgradeModels->BaseModel;
            break;
    }
    if (modelname != nullptr)
    {
        return std::string(modelname);
    }
}

void eopEduHelpers::setArmourUpgradeModel(int idx, int levelidx, std::string newModel)
{
    
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    char* modelname = nullptr;
    switch(levelidx)
    {
        case 3:
            modelname = reinterpret_cast<char*>(&eduEn->ArmorUpgradeModels->UpgradeThree);
            break;
        case 2:
            modelname = reinterpret_cast<char*>(&eduEn->ArmorUpgradeModels->UpgradeTwo);
            break;
        case 1:
            modelname = reinterpret_cast<char*>(&eduEn->ArmorUpgradeModels->UpgradeOne);
            break;
        case 0:
            modelname = reinterpret_cast<char*>(&eduEn->ArmorUpgradeModels->BaseModel);
            break;
    }
    if (modelname != nullptr)
    {
        luaGetSetFuncs::setGameString(modelname, newModel.c_str());
    }
}

void eopEduHelpers::setEntryAttackAttribute(int idx, UnitEnums::attackAttr attribute, bool enable, int sec)
{   
	using namespace UnitEnums;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    if ( sec == 1)
    {
        if (enable == true)
        {
            eduEn->StatPri = eduEn->StatPri | attribute;
        }
        else
        {
            eduEn->StatPri = eduEn->StatPri & (1073741823-attribute);
        }
    }
    else
    {
        if (enable == true)
        {
            eduEn->StatSec = eduEn->StatSec | attribute;
        }
        else
        {
            eduEn->StatSec = eduEn->StatSec & (1073741823-attribute);
        }
    }
}

bool eopEduHelpers::getEntryAttackAttribute(int idx, UnitEnums::attackAttr attribute, int sec)
{   
	using namespace UnitEnums;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    if ( sec == 1)
    {
        return ((eduEn->StatPri & attribute) != 0);
    }
    else
    {
        return ((eduEn->StatSec & attribute) != 0);
    }
}

void eopEduHelpers::setEntryStat(int idx, UnitEnums::eduStat stat, int value, int sec)
{   
	using namespace UnitEnums;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    if (stat < UnitEnums::attack)
    {
        eduEn->StatPriArmour = eduEn->StatPriArmour - (eduEn->StatPriArmour & (stat*63)) | value*stat;
    }
    else
    {
        if ( sec == 1)
        {
            eduEn->StatPri = eduEn->StatPri - (eduEn->StatPri & (stat*63)) | value*stat;
        }
        else
        {
            eduEn->StatSec = eduEn->StatSec - (eduEn->StatSec & (stat*63)) | value*stat;
        }
    }
}

int eopEduHelpers::getEntryStat(int idx, UnitEnums::eduStat stat, int sec)
{   
	using namespace UnitEnums;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    if (stat < UnitEnums::attack)
    {
        return (eduEn->StatPriArmour & (stat*63))/stat ;
    }
    else
    {
    if ( sec == 1)
        {
            return (eduEn->StatPri & (stat*63))/stat;
        }
        else
        {
            return (eduEn->StatSec & (stat*63))/stat;
        }
    }
}

