#include "buildingStructHelpers.h"

namespace buildingStructHelpers
{
	std::string getType(building* build)
	{
		return build->bData->type;
	}
	std::string getName(building* build)
	{
		return (build->bData->drawInfo + build->level)->name;
	}
	std::string getQueueType(buildingInQueue* build)
	{
		return build->buildingData->type;
	}
	std::string getQueueName(buildingInQueue* build)
	{
		return (build->buildingData->drawInfo + build->currentLevel)->name;
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