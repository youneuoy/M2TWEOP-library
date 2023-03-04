#pragma once
#include"headersSTD.h"
#include "exportHeader.h"
namespace casModelsDrawer
{
	EOP_EXPORT void addCasModelToDrawList (UINT32 modelId, int x, int y, float sizeMultiplier) noexcept(true);
	EOP_EXPORT void removeCasModelFromDrawList(UINT32 modelId) noexcept(true);

	void WINAPI drawModels();
};

