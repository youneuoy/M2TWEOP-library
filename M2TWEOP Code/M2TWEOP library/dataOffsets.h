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
		DWORD stratCursorCoords = NULL;
		DWORD customTiles = NULL;
		DWORD battleCamera = NULL;
		DWORD battlePerimeters = NULL;
		DWORD perfectSpy = NULL;
		DWORD options1 = NULL;
		DWORD options2 = NULL;
		DWORD campaignDiff1 = NULL;
		DWORD campaignDiff2 = NULL;
		DWORD equipmentCosts = NULL;
		DWORD saveGameHandler = NULL;
		DWORD ancLimit = NULL;
		DWORD maxUnitSize = NULL;
		DWORD maxBgSize1 = NULL;
		DWORD maxBgSize2 = NULL;
		DWORD unlockConsoleCommands1 = NULL;
		DWORD unlockConsoleCommands2 = NULL;
		DWORD highlightUnits = NULL;
		DWORD religionLimit = NULL;
		DWORD scriptCounters = NULL;
		DWORD scriptCountersSet = NULL;
		DWORD buildingChainLimit = NULL;
		DWORD guildCooldown = NULL;


		DWORD unitTypesStart = NULL;
		//if on strat map then not 0
		DWORD someStratMapPointer = NULL;

		DWORD skeletonDatabase = NULL;
		DWORD tgaDataBase = NULL;
		DWORD stratModelArray = NULL;
		DWORD getMovePath = NULL;
		DWORD modelsDb = NULL;

		gameDataAllStruct* gameDataAllOffset = nullptr;
		consoleCommands* consoleCommands = nullptr;

		DWORD guildDataStart = NULL;

		DWORD edbDataStart = NULL;
		DWORD religionDatabase = NULL;
		DWORD portraitDatabase = NULL;
		DWORD cultureDatabase = NULL;
		DWORD loadGameHandler = NULL;
		DWORD selectedEra = NULL;
		DWORD campaignDb = NULL;
		DWORD campaignDbExtra = NULL;
		DWORD groupLabels = NULL;



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
		DWORD globalCadClass = NULL;
		DWORD constructionItems = NULL;
		DWORD recruitmentItems = NULL;
		DWORD ltgdGlobals = NULL;
		DWORD battlefieldEngines = NULL;
	}offsets;

	static void initDataOffsets(int gameVer);
};
