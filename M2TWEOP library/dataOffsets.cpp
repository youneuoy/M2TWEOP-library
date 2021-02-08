#include "pch.h"
#include "dataOffsets.h"
dataOffsets::offsetsT dataOffsets::offsets;
void dataOffsets::initDataOffsets(int gameVer)
{
	if (gameVer == 1)
	{
		offsets.factionOffsetsStart= 0x02c00140;
		offsets.statStratModelsListOffset = 0x01b5e708;

		offsets.unitTypesStart = 0x0190bd0c;


		offsets.stratMapAllOffsetStart = 0x02c74ff4;

		offsets.someStratmapPointer = 0x019b3134;
	}
	else if (gameVer == 2)
	{
		offsets.factionOffsetsStart= 0x02bb7088;
		offsets.statStratModelsListOffset = 0x01b155d8;

		offsets.unitTypesStart = 0x18C2BAC;


		offsets.stratMapAllOffsetStart = 0x02c2bf3c;

		offsets.someStratmapPointer = 0x0196a004;
	}
}
