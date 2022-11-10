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
    return (*(*plugData::data.funcsEopEdu.getEduEntry))(index);
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

void eopEduHelpers::setEntryStatPriArmour(int index, int armour, int defense, int shield)
{
    (*(*plugData::data.funcsEopEdu.setEntryStatPriArmour))(index, armour, defense, shield);
}

void eopEduHelpers::setEntryAttackCharge(int index, int attack, int charge)
{
    (*(*plugData::data.funcsEopEdu.setEntryAttackCharge))(index, attack, charge);
}

int eopEduHelpers::GetEntryAttack(int index)
{
    return (*(*plugData::data.funcsEopEdu.GetEntryAttack))(index);
}   

int eopEduHelpers::GetEntryCharge(int index)
{
    return (*(*plugData::data.funcsEopEdu.GetEntryCharge))(index);
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

int eopEduHelpers::GetArmourUpgradeLevelsNum(int idx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    return (eduEn->ArmorUpgradesEnd - eduEn->ArmorUpgradeLevels);
}

void eopEduHelpers::SetArmourUpgradeLevelsNum(int idx, int amount)
{
    if (amount > 0)
    {
        EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
        int curramount = GetArmourUpgradeLevelsNum(idx);
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

int eopEduHelpers::GetArmourUpgradeLevel(int idx, int levelidx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    int LevelsNum = eopEduHelpers::GetArmourUpgradeLevelsNum(idx);

    if (levelidx < LevelsNum)
    {
        return *(int8_t*)((eduEn->ArmorUpgradeLevels) + levelidx);
    }

}

void eopEduHelpers::SetArmourUpgradeLevel(int idx, int levelidx, int8_t newlevel)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    int LevelsNum = eopEduHelpers::GetArmourUpgradeLevelsNum(idx);

    if (levelidx < LevelsNum)
    {
        *(int8_t*)((eduEn->ArmorUpgradeLevels) + levelidx) = newlevel;
    }

}

int eopEduHelpers::GetArmourUpgradeModelsNum(int idx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    return ((eduEn->ArmorUpgradeModelsEnd - (int*)eduEn->ArmorUpgradeModels))/2;
}

void eopEduHelpers::SetArmourUpgradeModelsNum(int idx, int amount)
{
    if (amount > 0 && amount < 5)
    {
        amount = amount*2;
        EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
        int curramount = GetArmourUpgradeModelsNum(idx);
        eduEn->ArmorUpgradeModelsEnd = (int*)eduEn->ArmorUpgradeModels + (amount);
        eduEn->ArmorUpgrade5 = (int*)eduEn->ArmorUpgradeModels + (amount);
        if (amount > (curramount*2))
        {
            std::string last_upgrade = GetArmourUpgradeModel(idx, (curramount-1));
            for (int i = 0; i < amount/2 - curramount;i++)
            {
                SetArmourUpgradeModel(idx, (curramount)+i, last_upgrade);
            }
        }
    }
}

std::string eopEduHelpers::GetArmourUpgradeModel(int idx, int levelidx)
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

void eopEduHelpers::SetArmourUpgradeModel(int idx, int levelidx, std::string newModel)
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

void eopEduHelpers::setEntryStatPriAttribute(int idx, UnitEnums::stat_pri_attr attribute, bool enable)
{   
	using namespace UnitEnums;
    EduEntry* entry = eopEduHelpers::getEduEntry(idx);

    if (enable == true)
    {
        entry->StatPri = entry->StatPri | attribute;
    }
    else
    {
        entry->StatPri = entry->StatPri & (1073741823-attribute);
    }
}