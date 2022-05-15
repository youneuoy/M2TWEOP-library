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

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c74f90);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02cbce50);

		offsets.guildDataStart = 0x018FD958;


		offsets.audioEnable = (bool*)0x016818cd;
		offsets.audioMaster_vol = (int*)0x01681910;
	}
	else if (gameVer == 2)
	{
		offsets.factionOffsetsStart= 0x02bb7088;
		offsets.statStratModelsListOffset = 0x01b155d8;

		offsets.unitTypesStart = 0x18C2BAC;


		offsets.stratMapAllOffsetStart = 0x02c2bf3c;

		offsets.someStratmapPointer = 0x0196a004;


		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c2bed8);

		offsets.consoleCommands= reinterpret_cast<consoleCommands*>(0x02c73df8);

		offsets.guildDataStart = 0x18B47E8;


		offsets.audioEnable = (bool*)0x01639f1d;
		offsets.audioMaster_vol = (int*)0x01639f60;
	}
}
