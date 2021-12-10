#pragma once
#include"headersSTD.h"
#include"realGameTypes.h"
class dataOffsets
{
public:
	static struct offsetsT
	{
		DWORD factionOffsetsStart = NULL;
		DWORD statStratModelsListOffset = NULL;


		DWORD stratMapAllOffsetStart = NULL;


		DWORD unitTypesStart = NULL;
		//if on stratmap then not 0
		DWORD someStratmapPointer = NULL;

		gameDataAllStruct* gameDataAllOffset = NULL;
		consoleCommands* consoleCommands = NULL;

		DWORD guildDataStart = NULL;
	}offsets;

	static void initDataOffsets(int gameVer);
};

