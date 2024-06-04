#include "dataOffsets.h"
dataOffsets::offsetsT dataOffsets::offsets;
void dataOffsets::initDataOffsets(int gameVer)
{
	if (gameVer == 1) //kingdoms
	{
		offsets.factionOffsetsStart = 0x02c00140;
		
		offsets.battleMapData = 0x02c00130;
		
		offsets.statStratModelsListOffset = 0x01b5e708;

		offsets.unitTypesStart = 0x0190bd0c;

		offsets.stratMapAllOffsetStart = 0x02c74ff4;

		offsets.stringTable = 0x02CB97E8;

		offsets.someStratmapPointer = 0x019b3134;

		offsets.skeletonDatabase = 0x01B59B08;

		offsets.getMovePath = 0x004D8390;

		offsets.tgaDataBase = 0x019875E8;

		offsets.stratModelArray = 0x01B617D8;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c74f90);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02cbce50);

		offsets.guildDataStart = 0x018FD958;

		//gets used in adding building capabilities function for some reason just following what game does
		offsets.edbDataStart = 0x018AA5A8;
		
		offsets.scriptCommandVFT = 0x0134E3A0;
		
		offsets.religionDatabase = 0x016E9DC0;
		
		offsets.portraitDatabase = 0x18B0340;

		offsets.cultureDatabase = 0x01666FC8;
		
		offsets.audioEnable = (bool*)0x016818cd;
		offsets.audioMaster_vol = (int*)0x01681910;
		offsets.gameUnit_size = (int*)0x0016818fc;

		offsets.descr_sm_factionslist = reinterpret_cast<descr_sm_factions_list*>(0x01667288);
	}
	else if (gameVer == 2) //steam
	{
		offsets.factionOffsetsStart = 0x02bb7088;
		
		offsets.battleMapData = 0x02BB7078;
		
		offsets.statStratModelsListOffset = 0x01b155d8;

		offsets.unitTypesStart = 0x18C2BAC;

		offsets.stratMapAllOffsetStart = 0x02c2bf3c;

		offsets.stringTable = 0x02C70760;

		offsets.someStratmapPointer = 0x0196a004;

		offsets.skeletonDatabase = 0x01B109D8;

		offsets.tgaDataBase = 0x0193E498;

		offsets.stratModelArray = 0x01B186A8;

		offsets.getMovePath = 0x004D8390;
		
		offsets.globalSett = 0x016A7F38;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c2bed8);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02c73df8);

		offsets.guildDataStart = 0x18B47E8;
		
		offsets.scriptCommandVFT = 0x013933D0;
		
		offsets.religionDatabase = 0x016A0B90;
		
		offsets.portraitDatabase = 0x018671D0;

		offsets.cultureDatabase = 0x0161F0F0;
		offsets.loadGameHandler = 0x02C6DC60;
		offsets.currentGameHandler = 0x016A2FA4;

		//gets used in adding building capabilities function for some reason just following what game does
		offsets.edbDataStart = 0x01861438;

		offsets.audioEnable = (bool*)0x01639f1d;
		offsets.audioMaster_vol = (int*)0x01639f60;
		offsets.gameUnit_size = (int*)0x01639f4c;

		offsets.descr_sm_factionslist = reinterpret_cast<descr_sm_factions_list*>(0x0161f3b0);
	}
}
