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
    if (amount <= 0)
    {
        return;
    }
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    int curramount = getArmourUpgradeLevelsNum(idx);
    eduEn->ArmorUpgradesEnd = eduEn->ArmorUpgradeLevels + amount;
    eduEn->ArmorUpgrade2 = eduEn->ArmorUpgradeLevels + amount;
    if (amount <= curramount)
    {
        return;
    }
    int8_t last_upgrade = eduEn->ArmorUpgradeLevels[curramount-1];
    for (int i = 0; i < amount - curramount;i++)
    {
        eduEn->ArmorUpgradeLevels[curramount + i] = last_upgrade;
    }
}

int eopEduHelpers::getArmourUpgradeLevel(int idx, int levelidx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    int LevelsNum = eopEduHelpers::getArmourUpgradeLevelsNum(idx);

    if (levelidx >= LevelsNum)
    {
        return -1;
    }
    return (int8_t)eduEn->ArmorUpgradeLevels[levelidx];
}

void eopEduHelpers::setArmourUpgradeLevel(int idx, int levelidx, int8_t newlevel)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    int LevelsNum = eopEduHelpers::getArmourUpgradeLevelsNum(idx);
    if (levelidx >= LevelsNum)
    {
        return;
    }
    eduEn->ArmorUpgradeLevels[levelidx] = newlevel;
}

int eopEduHelpers::getArmourUpgradeModelsNum(int idx)
{
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    return ((eduEn->ArmorUpgradeModelsEnd - (int*)eduEn->ArmorUpgradeModels))/2;
}

void eopEduHelpers::setArmourUpgradeModelsNum(int idx, int amount)
{
    if (amount <= 0 || amount >= 5)
    {
        return;
    }
    amount = amount*2;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    int curramount = getArmourUpgradeModelsNum(idx);
    ArmourUpgModels* newModels = new ArmourUpgModels;
    ArmourUpgModels* oldModels = eduEn->ArmorUpgradeModels;
    *newModels = *oldModels;
    eduEn->ArmorUpgradeModels = newModels;
    eduEn->ArmorUpgradeModelsEnd = (int*)eduEn->ArmorUpgradeModels + (amount);
    eduEn->ArmorUpgrade5 = (int*)eduEn->ArmorUpgradeModels + (amount);
    if (amount <= (curramount*2))
    {
        return;
    }
    const std::string& last_upgrade = getArmourUpgradeModel(idx, (curramount-1));
    for (int i = 0; i < amount/2 - curramount;i++)
    {
        setArmourUpgradeModel(idx, (curramount)+i, last_upgrade);
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
    return "";  
}

void eopEduHelpers::setArmourUpgradeModel(int idx, int levelidx, const std::string& newModel)
{
    
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    int modelsNum = getArmourUpgradeModelsNum(idx);
    ArmourUpgModels* newModels = new ArmourUpgModels;
    ArmourUpgModels* oldModels = eduEn->ArmorUpgradeModels;
    *newModels = *oldModels;
    eduEn->ArmorUpgradeModels = newModels;
    eduEn->ArmorUpgradeModelsEnd = (int*)eduEn->ArmorUpgradeModels + (modelsNum * 2);
    eduEn->ArmorUpgrade5 = (int*)eduEn->ArmorUpgradeModels + (modelsNum * 2);
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

int eopEduHelpers::multiplexor(int n1, int n2, int sel)
{
    return (((~sel) & n1) | (n2 & sel));
}

void eopEduHelpers::setEntryStat(int idx, UnitEnums::eduStat stat, int value, int sec)
{   
	using namespace UnitEnums;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    int newStat = value*stat;
    if (stat < UnitEnums::attack)
    {
        eduEn->StatPriArmour = (eopEduHelpers::multiplexor(eduEn->StatPriArmour, newStat, stat*63));
    }
    else
    {
        if ( sec == 1)
        {

            eduEn->StatPri = eopEduHelpers::multiplexor(eduEn->StatPri, newStat, stat*63);
        }
        else
        {
            eduEn->StatSec = eopEduHelpers::multiplexor(eduEn->StatSec, newStat, stat*63);
        }
    }
}

int eopEduHelpers::getEntryStat(int idx, UnitEnums::eduStat stat, int sec)
{   
	using namespace UnitEnums;
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);

    if (stat < UnitEnums::attack)
    {
        return (eduEn->StatPriArmour & (stat*63))/stat;
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

//pretty much ignore this
void eopEduHelpers::addUnitToRQ(int idx, settlementStruct* sett)
{   
    int startIndexRQ = sett->startIndexRQ;
    int endIndexRQ = sett->endIndexRQ;
    if (startIndexRQ == 0 && endIndexRQ == 9)
    {
        return;
    }
    EduEntry* eduEn = eopEduHelpers::getEduEntry(idx);
    unitRQ* newUnit = new unitRQ;
    int unitsize = m2tweopHelpers::GetUnitSize();
    float unitModifier = 1;
    switch(unitsize)
    {
        case 3:
            unitModifier = 2.5;
            break;
        case 2:
            unitModifier = 1.88;
            break;
        case 1:
            unitModifier = 1.25;
            break;
        case 0:
            unitModifier = 1;
            break;
    }
    newUnit->eduEntry = eduEn;
    newUnit->FourtySeven = 47;
    newUnit->settlement = sett;
    newUnit->N0001082F = 0;
    newUnit->threeIfRetrainingFourIfBoth = 0;
    newUnit->experience = 0;
    newUnit->N00010830 = 0;
    newUnit->Minus1 = -1;
    newUnit->turnsTrainedAlready = 0;
    newUnit->procentFinished = 0;
    newUnit->turnsToTrain = eduEn->StatCost1;
    newUnit->cost = eduEn->StatCost2;
    newUnit->unitCount = (int)(eduEn->SoldierCount*unitModifier);
    newUnit->N00010833 = 0;
    newUnit->isNotFrozen = 1;
    newUnit->recruitmentPoolSizeBeforeOrRetrainingNumbersBefore = 1;
    newUnit->isRetraining = 0;
    int newIndex = 0;
    if (sett->countRQ == 0)
    {
        newIndex = 0;
    }
    else
    {
        if (startIndexRQ > 0)
        {
            newIndex = startIndexRQ-1;
            sett->startIndexRQ--;
        }
        else
        {
            newIndex = endIndexRQ+1;
            sett->endIndexRQ++;
        }
    }
    sett->countRQ++;
    sett->unitQueue[newIndex] = *newUnit;
}

