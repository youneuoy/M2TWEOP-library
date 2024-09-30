#pragma once
#include"headersSTD.h"

namespace casModelsDrawer
{
	void addCasModelToDrawList(UINT32 modelId, int x, int y, float sizeMultiplier);
	void removeCasModelFromDrawList(UINT32 modelId);

	void WINAPI drawModels();
};

