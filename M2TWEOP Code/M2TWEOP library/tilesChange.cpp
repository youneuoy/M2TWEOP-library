#include "tilesChange.h"
#include "realGameTypes.h"
#include "dataOffsets.h"
#include "functionsOffsets.h"
#include "globals.h"
#include "strategyMap.h"

namespace tilesChange
{
	vector<customTile*> EOP_CUSTOM_TILES{};
	
	void onTilesCheck(int* coords)
	{
		const int x = coords[0];
		const int y = coords[1];
		for (const customTile* ct : EOP_CUSTOM_TILES)
		{
			if (ct->x == x && ct->y == y)
			{
				const auto ctRes = reinterpret_cast<customTile*>(dataOffsets::offsets.customTiles);
				ctRes->name = ct->name;
				ctRes->nameCrypt = ct->nameCrypt;
				ctRes->pathToModel_UNICODE = ct->pathToModel_UNICODE;
				ctRes->timeCode = ct->timeCode;
				ctRes->weather = ct->weather;
				ctRes->weatherCrypt = ct->weatherCrypt;
				ctRes->x = ct->x;
				ctRes->y = ct->y;
				coords[0] = ct->x;
				coords[1] = ct->y;
				return;
			}
		}
	}
	
	int getTimeCode(const char* timeS)
	{
		if (strcmp("afternoon", timeS) == 0)
		{
			return 3;
		}
		if (strcmp("midday", timeS) == 0)
		{
			return 2;
		}
		if (strcmp("morning", timeS) == 0)
		{
			return 1;
		}
		if (strcmp("night", timeS) == 0)
		{
			return 5;
		}
		if (strcmp("sunrise", timeS) == 0)
		{
			return 0;
		}
		if (strcmp("sunset", timeS) == 0)
		{
			return 4;
		}
		return 6;
	}

	void replaceAll2(string& s, const string& search, const string& replace) {
		for (size_t pos = 0; ; pos += replace.length())
		{
			pos = s.find(search, pos);
			if (pos == string::npos) break;

			s.erase(pos, search.length());
			s.insert(pos, replace);
		}
	}

	void replaceTile(const char* tileLabel, int x, int y, const char* fileName, const char* weather, const char* dayTime)
	{
		auto newTile = new customTile();
		newTile->x = x;
		newTile->y = y;
		UINT32 nameOffset = reinterpret_cast<UINT32>(&newTile->name);
		UINT32 functionOffset = codes::offsets.stringCryptFunc;
		_asm {
			push tileLabel
			mov ecx, nameOffset
			mov eax, functionOffset
			call eax
		}
		UINT32 weatherOffset = reinterpret_cast<UINT32>(&newTile->weather);
		_asm {
			push weather
			mov ecx, weatherOffset
			mov eax, functionOffset
			call eax
		}
		newTile->timeCode = getTimeCode(dayTime);
		string pathName = globals::dataS.modPath;
		string f = "\\";
		string r = "/";
		replaceAll2(pathName, f, r);
		pathName = m2tweopHelpers::getModString(pathName);
		pathName = pathName + "/data/world/maps/campaign/imperial_campaign/custom_tiles/";
		pathName = pathName + fileName;
		auto pathTemp = new char[pathName.length() + 1];
		memcpy(pathTemp, pathName.c_str(), pathName.length() + 1);
		DWORD funcAdr = codes::offsets.uniStrCustomTile;
		newTile->pathToModel_UNICODE = reinterpret_cast<char**>(new char[(pathName.length() + 1) * 2]);
		auto tileTest = (char**)newTile->pathToModel_UNICODE;
		_asm {
			mov ecx, tileTest
			push pathTemp
			mov eax, funcAdr
			call eax
		}
		newTile->pathToModel_UNICODE = reinterpret_cast<char**>(*tileTest);
		for (size_t i = 0; i < EOP_CUSTOM_TILES.size(); i++)
		{
			if (EOP_CUSTOM_TILES[i]->x == x && EOP_CUSTOM_TILES[i]->y == y)
			{
				delete EOP_CUSTOM_TILES[i];
				EOP_CUSTOM_TILES.erase(EOP_CUSTOM_TILES.begin() + i);
				break;
			}
		}
		EOP_CUSTOM_TILES.push_back(newTile);
		return;
	}
}
