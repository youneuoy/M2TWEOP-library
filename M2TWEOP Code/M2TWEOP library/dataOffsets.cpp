#include "pch.h"
#include "dataOffsets.h"
dataOffsets::offsetsT dataOffsets::offsets;
void dataOffsets::initDataOffsets(int gameVer)
{
	if (gameVer == 1) //kingdoms
	{
		offsets.factionOffsetsStart = 0x02c00140;
		offsets.statStratModelsListOffset = 0x01b5e708;

		offsets.unitTypesStart = 0x0190bd0c;

		offsets.stratMapAllOffsetStart = 0x02c74ff4;

		offsets.stringTable = 0x02CB97E8;

		offsets.someStratmapPointer = 0x019b3134;

		offsets.skeletonDatabase = 0x01B59B08;

		offsets.tgaDataBase = 0x019875E8;

		offsets.stratModelArray = 0x01B617D8;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c74f90);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02cbce50);

		offsets.guildDataStart = 0x018FD958;

		//gets used in adding building capabilities function for some reason just following what game does
		offsets.edbDataStart = 0x018940C0;


		offsets.audioEnable = (bool*)0x016818cd;
		offsets.audioMaster_vol = (int*)0x01681910;
		offsets.gameUnit_size = (int*)0x0016818fc;
	}
	else if (gameVer == 2) //steam
	{
		offsets.factionOffsetsStart = 0x02bb7088;
		offsets.statStratModelsListOffset = 0x01b155d8;

		offsets.unitTypesStart = 0x18C2BAC;

		offsets.stratMapAllOffsetStart = 0x02c2bf3c;

		offsets.stringTable = 0x02C70760;

		offsets.someStratmapPointer = 0x0196a004;

		offsets.skeletonDatabase = 0x01B109D8;

		offsets.tgaDataBase = 0x0193E498;

		offsets.stratModelArray = 0x01B186A8;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c2bed8);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02c73df8);

		offsets.guildDataStart = 0x18B47E8;

		//gets used in adding building capabilities function for some reason just following what game does
		offsets.edbDataStart = 0x01861438;

		offsets.audioEnable = (bool*)0x01639f1d;
		offsets.audioMaster_vol = (int*)0x01639f60;
		offsets.gameUnit_size = (int*)0x01639f4c;
	}
}
