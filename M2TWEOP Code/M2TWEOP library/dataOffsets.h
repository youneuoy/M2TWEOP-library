#pragma once
#include"realGameTypes.h"
class dataOffsets
{
public:
	static struct offsetsT
	{
		DWORD factionOffsetsStart = NULL;
		DWORD statStratModelsListOffset = NULL;


		DWORD stratMapAllOffsetStart = NULL;
		DWORD stringTable = NULL;


		DWORD unitTypesStart = NULL;
		//if on stratmap then not 0
		DWORD someStratmapPointer = NULL;

		DWORD skeletonDatabase = NULL;
		DWORD tgaDataBase = NULL;
		DWORD stratModelArray = NULL;
		DWORD getMovePath = NULL;

		gameDataAllStruct* gameDataAllOffset = NULL;
		consoleCommands* consoleCommands = NULL;

		DWORD guildDataStart = NULL;

		DWORD edbDataStart = NULL;
		DWORD religionDatabase = NULL;
		DWORD portraitDatabase = NULL;
		DWORD cultureDatabase = NULL;
		DWORD loadGameHandler = NULL;
		DWORD selectedEra = NULL;



		//audio settings
		bool* audioEnable = nullptr;
		int* audioMaster_vol = nullptr;

		//game settings
		int* gameUnit_size = nullptr;

		descr_sm_factions_list* descr_sm_factionslist = nullptr;
		DWORD scriptCommandVFT = NULL;
		DWORD battleMapData = NULL;
		DWORD globalSett = NULL;
		DWORD currentGameHandler = NULL;
		DWORD logFlushRate = NULL;
	}offsets;

	static void initDataOffsets(int gameVer);
};
