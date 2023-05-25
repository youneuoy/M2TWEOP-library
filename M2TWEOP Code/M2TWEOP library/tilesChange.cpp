#include "tilesChange.h"

namespace tilesChange
{
	vector<custom_tile*>eopCustomTiles;
	void onTilesCheck(int* coords)
	{
		int x = coords[0];
		int y = coords[1];
		for (custom_tile* ct : eopCustomTiles)
		{
			if (ct->x == x
				&&
				ct->y == y
				)
			{
				DWORD ctOffset = 0;
				if (globals::dataS.gamever == 2)//steam
				{
					ctOffset = 0x02BF2E98;
				}
				else
				{
					ctOffset = 0x02C3BF50;
				}
				custom_tile* ctRes = (custom_tile*)ctOffset;

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
		else if (strcmp("midday", timeS) == 0)
		{
			return 2;
		}
		else if (strcmp("morning", timeS) == 0)
		{
			return 1;
		}
		else if (strcmp("night", timeS) == 0)
		{
			return 5;
		}
		else if (strcmp("sunrise", timeS) == 0)
		{
			return 0;
		}
		else if (strcmp("sunset", timeS) == 0)
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

	string getModString(string& path)
	{
		string ret;
		size_t pos = path.find("/mods/", 0);
		for (UINT32 i = 0; i < path.size(); i++)
		{
			if (i > (UINT32)pos)
			{
				ret.push_back(path[i]);
			}
		}
		return ret;
	}

	EOP_EXPORT void replaceTile(const char* tileLabel, UINT32 x, UINT32 y, const char* fileName, const char* weather, const char* dayTime)
	{
		custom_tile* newTile = new custom_tile();
		newTile->x = x;
		newTile->y = y;
		UINT32 nameOffset = (UINT32)&newTile->name;
		UINT32 functionOffset = codes::offsets.stringCryptFunc;
		_asm {
			push tileLabel
			mov ecx, nameOffset
			mov eax, functionOffset
			call eax
		}

		UINT32 weatherOffset = (UINT32)&newTile->weather;
		_asm {
			push weather
			mov ecx, weatherOffset
			mov eax, functionOffset
			call eax
		}
		newTile->timeCode = getTimeCode(dayTime);

		string pathName = globals::dataS.modPatch;
		string f = "\\";
		string r = "/";
		replaceAll2(pathName, f, r);
		pathName = getModString(pathName);

		pathName = pathName + "/data/world/maps/campaign/imperial_campaign/custom_tiles/";

		pathName = pathName + fileName;

		char* pathTemp = new char[pathName.length() + 1];
		memcpy(pathTemp, pathName.c_str(), pathName.length() + 1);

		DWORD funcAdr = 0;
		if (globals::dataS.gamever == 2)//steam
		{
			funcAdr = 0x00f01eb0;
		}
		else
		{
			funcAdr = 0x00f018e0;
		}

		newTile->pathToModel_UNICODE = (char**)new char[(pathName.length() + 1) * 2];
		char** tileTest = (char**)newTile->pathToModel_UNICODE;
		_asm {
			mov ecx, tileTest
			push pathTemp
			mov eax, funcAdr
			call eax
		}
		newTile->pathToModel_UNICODE = (char**)*tileTest;


		for (size_t i = 0; i < eopCustomTiles.size(); i++)
		{
			if (eopCustomTiles[i]->x == x
				&& eopCustomTiles[i]->y == y
				)
			{
				delete eopCustomTiles[i];
				eopCustomTiles.erase(eopCustomTiles.begin() + i);
				break;
			}

		}

		eopCustomTiles.push_back(newTile);

		return;
	}

}