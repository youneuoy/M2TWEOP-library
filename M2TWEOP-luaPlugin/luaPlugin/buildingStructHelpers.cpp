#include "buildingStructHelpers.h"

namespace buildingStructHelpers
{
	std::string getType(building* build)
	{
		return build->edbEntry->type;
	}
	std::string getName(building* build)
	{
		buildingLevel* level = &build->edbEntry->buildingLevel[build->level];
		return level->name;
	}
	std::string getQueueType(buildingInQueue* build)
	{
		return build->edbEntry->type;
	}
	std::string getQueueName(buildingInQueue* build)
	{
		buildingLevel* level = &build->edbEntry->buildingLevel[build->currentLevel];
		return level->name;
	}
	
	void addCapability(building* building, int capability, int16_t value, bool bonus)
	{
		BuildingLvlCapability* cap = new BuildingLvlCapability;
		cap->capabilityType = 0;
		if (bonus)
		{
			cap->bonus = value;
			cap->capabilityLvl = -1;
		}
		else
		{
			cap->bonus = 0;
			cap->capabilityLvl = value;
		}
		cap->capabilityID = capability;
		buildingLevel* level = &building->edbEntry->buildingLevel[building->level];
		cap->nextCapability = level->capabilities;
		level->capabilities = cap;

	}

	edbEntry* addEopBuildEntry(edbEntry* oldEntry, int newIndex)
	{
    	return (*(*plugData::data.funcs.addEopBuildEntry))(oldEntry, newIndex);
	}

	edbEntry* getEopBuildEntry(int idx)
	{
    	return (*(*plugData::data.funcs.getEopBuildEntry))(idx);
	}

	void setBuildingPic(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
    	return (*(*plugData::data.funcs.setBuildingPic))(entry, newPic, level, cultureID);
	}

	void setBuildingPicConstructed(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
    	return (*(*plugData::data.funcs.setBuildingPicConstructed))(entry, newPic, level, cultureID);
	}

	void setBuildingPicConstruction(edbEntry* entry, const char* newPic, int level, int cultureID)
	{
    	return (*(*plugData::data.funcs.setBuildingPicConstruction))(entry, newPic, level, cultureID);
	}

	void setBuildingLocalizedName(edbEntry* entry, const char* newName, int level, int facnum)
	{
    	return (*(*plugData::data.funcs.setBuildingLocalizedName))(entry, newName, level, facnum);
	}

	void setBuildingLocalizedDescr(edbEntry* entry, const char* newName, int level, int facnum)
	{
    	return (*(*plugData::data.funcs.setBuildingLocalizedDescr))(entry, newName, level, facnum);
	}

	void setBuildingLocalizedDescrShort(edbEntry* entry, const char* newName, int level, int facnum)
	{
    	return (*(*plugData::data.funcs.setBuildingLocalizedDescrShort))(entry, newName, level, facnum);
	}

	void addBuildingCapability(edbEntry* entry, int level, int capability, int16_t value, bool bonus)
	{
    	return (*(*plugData::data.funcs.addBuildingCapability))(entry, level, capability, value, bonus);
	}

	void removeBuildingCapability(edbEntry* entry, int level, int index)
	{
    	return (*(*plugData::data.funcs.removeBuildingCapability))(entry, level, index);
	}

	BuildingLvlCapability* getBuildingCapability(edbEntry* entry, int level, int index)
	{
    	return (*(*plugData::data.funcs.getBuildingCapability))(entry, level, index);
	}

	int getBuildingCapabilityNum(edbEntry* entry, int level)
	{
    	return (*(*plugData::data.funcs.getBuildingCapabilityNum))(entry, level);
	}

	void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp)
	{
    	return (*(*plugData::data.funcs.addBuildingPool))(entry, level, eduIndex, initialSize, gainPerTurn, maxSize, exp);
	}

	void removeBuildingPool(edbEntry* entry, int level, int index)
	{
    	return (*(*plugData::data.funcs.removeBuildingPool))(entry, level, index);
	}

	recruitPool* getBuildingPool(edbEntry* entry, int level, int index)
	{
    	return (*(*plugData::data.funcs.getBuildingPool))(entry, level, index);
	}

	int getBuildingPoolNum(edbEntry* entry, int level)
	{
    	return (*(*plugData::data.funcs.getBuildingPoolNum))(entry, level);
	}

	void createEOPBuilding(settlementStruct* sett, int edbIdx, int level)
	{
    	return (*(*plugData::data.funcs.createEOPBuilding))(sett, edbIdx, level);
	}

	edbEntry* getBuildingByName(const char* name)
	{
    	return (*(*plugData::data.funcs.getBuildingByName))(name);
	}

}

buildingInQueue* buildingStructHelpers::getBuildingInQueue(buildingsQueue* queue, int position)
{
	if (position > 0 && position <= queue->buildingsInQueue) {
		int index = queue->firstIndex + position - 1;
		if (index > 5) { index = index - 6; }
		return &(queue->items[index]);
	}
	return nullptr;
}