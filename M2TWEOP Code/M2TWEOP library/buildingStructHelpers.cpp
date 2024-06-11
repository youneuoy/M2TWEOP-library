#include "buildingStructHelpers.h"

#include "eopBuildings.h"
#include "plugData.h"
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

	void addBuildingPool(edbEntry* entry, int level, int eduIndex, float initialSize, float gainPerTurn, float maxSize, int32_t exp, std::string condition)
	{
		const char* cond = condition.c_str();
    	return eopBuildings::addBuildingPool(entry, level, eduIndex, initialSize, gainPerTurn, maxSize, exp, cond);
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