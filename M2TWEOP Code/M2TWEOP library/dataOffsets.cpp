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
		
		offsets.globalSett = 0x016F10D0;

		offsets.someStratMapPointer = 0x019b3134;

		offsets.skeletonDatabase = 0x01B59B08;

		offsets.getMovePath = 0x004D8390;

		offsets.tgaDataBase = 0x019875E8;

		offsets.stratModelArray = 0x01B617D8;

		offsets.gameDataAllOffset = reinterpret_cast<gameDataAllStruct*>(0x02c74f90);

		offsets.consoleCommands = reinterpret_cast<consoleCommands*>(0x02cbce50);

		offsets.guildDataStart = 0x018FD958;

		offsets.edbDataStart = 0x018AA5A8;
		offsets.selectedEra = 0x2CB9210;
		offsets.campaignDb = 0x16666BC;
		offsets.campaignDbExtra = 0x18AA87C;
		
		offsets.scriptCommandVFT = 0x0134E3A0;
		
		offsets.religionDatabase = 0x016E9DC0;
		
		offsets.portraitDatabase = 0x18B0340;

		offsets.cultureDatabase = 0x01666FC8;
		offsets.loadGameHandler = 0x02CB6D78;
		offsets.currentGameHandler = 0x016EC1C4;
		offsets.logFlushRate = 0x16E445C;
		
		offsets.audioEnable = reinterpret_cast<bool*>(0x016818cd);
		offsets.audioMaster_vol = reinterpret_cast<int*>(0x01681910);
		offsets.gameUnit_size = reinterpret_cast<int*>(0x0016818fc);
		offsets.globalCadClass = 0x1674570;
		offsets.constructionItems = 0x016F4CAC;
		offsets.recruitmentItems = 0x16F4E00;
		offsets.ltgdGlobals = 0x016F115C;
		offsets.groupLabels = 0x001BA96A0;
		offsets.stratCursorCoords = 0x02c86c28;
		offsets.modelsDb = 0x016e9dc8;
		offsets.customTiles = 0x02C3BF50;

		offsets.descr_sm_factionslist = reinterpret_cast<descr_sm_factions_list*>(0x01667288);
	}
	else if (gameVer == 2) //steam
	{
		offsets.factionOffsetsStart = 0x02bb7088;
		
		offsets.battleMapData = 0x02BB7078;
		offsets.customTiles = 0x02BF2E98;
		
		offsets.statStratModelsListOffset = 0x01b155d8;

		offsets.unitTypesStart = 0x18C2BAC;

		offsets.stratMapAllOffsetStart = 0x02c2bf3c;

		offsets.stringTable = 0x02C70760;

		offsets.someStratMapPointer = 0x0196a004;

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
		offsets.logFlushRate = 0x169CF94;

		offsets.edbDataStart = 0x01861438;
		offsets.selectedEra = 0x02C70178;
		offsets.campaignDb = 0x0161E7E4;
		offsets.campaignDbExtra = 0x0186170C;
		offsets.globalCadClass = 0x0162C740;
		offsets.constructionItems = 0x016ABB14;
		offsets.recruitmentItems = 0x0016ABC68;
		offsets.ltgdGlobals = 0x016A7Fc4;
		offsets.groupLabels = 0x01B60580;
		offsets.stratCursorCoords = 0x02c3da48;
		offsets.modelsDb = 0x016a0b98;

		offsets.audioEnable = reinterpret_cast<bool*>(0x01639f1d);
		offsets.audioMaster_vol = reinterpret_cast<int*>(0x01639f60);
		offsets.gameUnit_size = reinterpret_cast<int*>(0x01639f4c);

		offsets.descr_sm_factionslist = reinterpret_cast<descr_sm_factions_list*>(0x0161f3b0);
	}
}
