#include "resourcesHelpers.h"
#include "plugData.h"

void resourcesHelpers::setModel(const resStrat* resource, UINT32 modelId)
{
	(*(*plugData::data.funcs.setModel))(resource->xCoord, resource->yCoord, modelId, modelId);
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
