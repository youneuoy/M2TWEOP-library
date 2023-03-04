#pragma once
#include"headersSTD.h"
#include "exportHeader.h"
namespace casModelsDrawer
{
	EOP_EXPORT void addModelToDrawList(UINT32 modelId, int x, int y, float sizeMultiplier);
	EOP_EXPORT void removeModelFromDrawList(UINT32 modelId);

	void WINAPI drawModels();
};

