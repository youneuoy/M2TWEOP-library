#include "mmbMover.h"
#include "dataOffsets.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
void MmbMover::Start(LPARAM lParam)
{
	isMmb = true;

	int xPosAbsolute = GET_X_LPARAM(lParam);
	int yPosAbsolute = GET_Y_LPARAM(lParam);
	oldCoords.x = xPosAbsolute;
	oldCoords.y = yPosAbsolute;
}

void MmbMover::Stop()
{
	isMmb = false;
}



void MmbMover::Update(LPARAM lParam)
{
	if (isMmb == false)
	{
		return;
	}
	int xPosAbsolute = GET_X_LPARAM(lParam);
	int yPosAbsolute = GET_Y_LPARAM(lParam);

	float** isStratMap = reinterpret_cast<float**>(dataOffsets::offsets.someStratMapPointer);
	if (*(int**)isStratMap == 0)return;



	float* xOffset =(*isStratMap + 0x7148/4);
	float* yOffset = (*isStratMap + 0x7150/4);

	float* xOffset2 =(*isStratMap + 0x7154/4);
	float* yOffset2 = (*isStratMap + 0x715C/4);

	int* intOffset1 = (*(int**)isStratMap + 0x7612/4);
	int* intOffset2 = (*(int**)isStratMap + 0x762C/4);
	*intOffset1 = 1;
	if (*intOffset2 != 0)
	{
		*intOffset2 = 3;
	}

	auto relativeX = oldCoords.x - xPosAbsolute;
	auto relativeY = oldCoords.y - yPosAbsolute;

	oldCoords.x = xPosAbsolute;
	oldCoords.y = yPosAbsolute;

	*isStratMap[0x7148/4] += (float)relativeX / 10;
	*isStratMap[0x7150/4] += (float)relativeY / 10;

	*isStratMap[0x7154/4] += (float)relativeX / 10;
	*isStratMap[0x715c/4] += (float)relativeY / 10;

	*xOffset2 = *xOffset;
	*yOffset2 = *yOffset;


	int* intOffsetx = (*(int**)isStratMap + 0x11bc / 4);
	int* intOffsety = (*(int**)isStratMap + 0x11c0 / 4);

	*intOffsetx = *((int*)0x2C3DA48);
	*intOffsety = *((int*)0x2C3DA4C);
}
