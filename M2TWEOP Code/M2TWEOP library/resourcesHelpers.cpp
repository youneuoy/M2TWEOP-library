#include "resourcesHelpers.h"
#include "plugData.h"
#include "stratModelsChange.h"

void resourcesHelpers::setModel(const resStrat* resource, UINT32 modelId)
{
	stratModelsChange::setModel(resource->xCoord, resource->yCoord, modelId, modelId);
}

int resourcesHelpers::getResourceCode(const resStrat* resource)
{
	return resource->stratMod->resource_num;
}

int resourcesHelpers::getResourceCost(const resStrat* resource)
{
	return resource->stratMod->resource_cost;
}

int resourcesHelpers::getResourceHasMine(const resStrat* resource)
{
	return resource->stratMod->hasMine;
}

namespace resourcesHelpers
{
	std::string getImage(const resStrat* resource)
	{
		return resource->stratMod->tga;
	}
}
