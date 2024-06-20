#include "m2tweopHelpers.h"

#include "campaign.h"
#include "dataOffsets.h"
#include "gameDataAllHelper.h"
#include "gameHelpers.h"
#include "graphicsD3D.h"
#include "m2tweopMapManager.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "faction.h"

scriptCommand::scriptCommand(const char* name) : className(name)
{
	this->vftable = dataOffsets::offsets.scriptCommandVFT;
}
namespace m2tweopHelpers
{
	std::string getModPath()
	{
		return  plugData::data.modFolder;
	}
	std::string getLuaPath()
	{
		return plugData::data.luaAll.luaPath;
	}
	
	void loadSaveGame(const char* saveName)
	{
		UNICODE_STRING** nameMem = new UNICODE_STRING*;

		auto loadName = reinterpret_cast<UNICODE_STRING***>(dataOffsets::offsets.loadGameHandler + 0x4);
		*loadName = nameMem;
		
		smallFuncs::createUniString(*loadName, saveName);

		const auto currentHandler = reinterpret_cast<DWORD*>(dataOffsets::offsets.currentGameHandler);
		*currentHandler = dataOffsets::offsets.loadGameHandler;
	}
	
	void fireGameScriptFunc(void* scriptStruct, DWORD offset)
	{
		void* scriptStructPtr = scriptStruct;
		DWORD func = offset;
		_asm
		{
			mov ecx, scriptStructPtr
			mov eax, func
			call eax
		}
	}
	
	std::string getModString(const std::string& path)
	{
		std::string ret;
		const size_t pos = path.find("/mods/", 0);
		for (UINT32 i = 0; i < path.size(); i++)
		{
			if (i > static_cast<UINT32>(pos))
			{
				ret.push_back(path[i]);
			}
		}
		return ret;
	}
	
	std::string getPathFromMods()
	{
		const std::string path = getModPath();
		return getModString(path);
	}
	
	void logStringGame(const std::string& msg)
	{
		const auto flushRate = reinterpret_cast<int*>(dataOffsets::offsets.logFlushRate);
		const int oldRate = *flushRate;
		*flushRate = 1;
		const auto order = std::make_shared<gameLogCommand>(msg.c_str());
		fireGameScriptFunc(order.get(), codes::offsets.gameLogCommand);
		*flushRate = oldRate;
	}
	
	void logFuncError(const std::string& funcName, const std::string& error)
	{
		logStringGame(funcName + " error: " + error);
	}
	
	std::tuple<int, int> getGameTileCoordsWithCursor()
	{
		int x = 0;
		int y = 0;
		stratMapHelpers::getGameTileCoordsWithCursor(x, y);

		return std::make_tuple(x, y);
	}
	
	bool getTileVisibility(factionStruct* faction, int x, int y)
	{
		auto vis = faction->tilesFac->tilesVisiblity[faction->tilesFac->tilesXBound * (y)+x];
		// If the tile is not visible
		if (vis == 0)
		{
			return false;
		}
		// If the tile is visible
		return true;
	}
	
	factionStruct* getRegionOwner(int regionID)
	{
		return stratMapHelpers::getStratMap()->regions[regionID].factionOwner;
	}

	culturesDB* getCultureDb()
	{
		DWORD offset = 0x0161F0F0;
		if (smallFuncs::getGameVersion() == 1)
			offset = 0x01666FC8;
		return reinterpret_cast<culturesDB*>(offset);
	}

	std::shared_ptr<mapImage> makeMapImage()
	{
		return std::make_shared<mapImage>(mapImage());
	}

	void clearMapImage(mapImage* img)
	{
		img->tiles.clear();
	}
	
	std::tuple<int, int, void*> loadTextureToGame(const std::string& path)
	{
		int x = 0;
		int y = 0;
		LPDIRECT3DTEXTURE9 retTexture = graphicsExport::loadTexture(path.c_str(), &x, &y);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			x, y
		});

		return std::make_tuple(x, y, reinterpret_cast<void*>(retTexture));
	}
	
	std::tuple<int, int, void*> loadMapTexture(mapImage* mapImage, const std::string& path)
	{
		int x = 0;
		int y = 0;
		LPDIRECT3DTEXTURE9 retTexture = graphicsExport::loadTexture(path.c_str(), &x, &y);
		retTexture = m2tweopMapManager::updateRegionColors(mapImage, retTexture, x, y);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			x, y
		});

		return std::make_tuple(x, y, reinterpret_cast<void*>(retTexture));
	}

	void fillRegionColor(mapImage* img, const int id, int r, int g, int b, int a)
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		const auto sMap = gameData->stratMap;
		if (!sMap) 
			return;
		if (id >= sMap->regionsNum) 
			return;
		r = max(0, min(255, r));
		g = max(0, min(255, g));
		b = max(0, min(255, b));
		a = max(0, min(255, a));
		const auto region = &sMap->regions[id];
		for (int i = 0; i < region->tileCount; i++)
		{
			const int tileIndex = region->tiles[i];
			const uint32_t color = MAKECOLOR(r, g, b, a);
			auto [xCoord, yCoord] = stratMapHelpers::convertTileCoords(tileIndex);
			if (tileIndex >= img->tiles.size())
				img->tiles.resize(tileIndex + 1);
			img->tiles[tileIndex] = tileColor(color, xCoord, yCoord);
		}
	}

	void addRegionColor(mapImage* img, const int id, const int r, const int g, const int b, const int a)
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		const auto sMap = gameData->stratMap;
		if (!sMap) 
			return;
		if (id >= sMap->regionsNum) 
			return;
		const auto region = &sMap->regions[id];
		for (int i = 0; i < region->tileCount; i++)
		{
			const int tileIndex = region->tiles[i];
			const auto [xCoord, yCoord] = stratMapHelpers::convertTileCoords(tileIndex);
			if (tileIndex >= img->tiles.size())
				img->tiles.resize(tileIndex + 1);
			const auto newR = max(0, min(255, GETRED(img->tiles[tileIndex].color) + r));
			const auto newG = max(0, min(255, GETGREEN(img->tiles[tileIndex].color) + g));
			const auto newB = max(0, min(255, GETBLUE(img->tiles[tileIndex].color) + b));
			const auto newA = max(0, min(255, GETALPHA(img->tiles[tileIndex].color) + a));
			const uint32_t color = MAKECOLOR(newR, newG, newB, newA);
			img->tiles[tileIndex] = tileColor(color, xCoord, yCoord);
		}
	}

	void fillTileColor(mapImage* img, const int x, const int y, int r, int g, int b, int a)
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		const auto sMap = gameData->stratMap;
		if (!sMap) 
			return;
		if (x >= sMap->mapWidth || y >= sMap->mapHeight) 
			return;
		if (x < 0 || y < 0) 
			return;
		r = max(0, min(255, r));
		g = max(0, min(255, g));
		b = max(0, min(255, b));
		a = max(0, min(255, a));
		const int tileIndex = sMap->mapWidth * y + x;
		if (tileIndex >= img->tiles.size())
			img->tiles.resize(tileIndex + 1);
		img->tiles[tileIndex] = tileColor(MAKECOLOR(r, g, b, a), x, y);
	}
	
	void addTileColor(mapImage* img, const int x, const int y, int r, int g, int b, int a)
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		const auto sMap = gameData->stratMap;
		if (!sMap) 
			return;
		if (x >= sMap->mapWidth || y >= sMap->mapHeight) 
			return;
		if (x < 0 || y < 0) 
			return;
		const int tileIndex = sMap->mapWidth * y + x;
		if (tileIndex >= img->tiles.size())
			img->tiles.resize(tileIndex + 1);
		r = max(0, min(255, GETRED(img->tiles[tileIndex].color) + r));
		g = max(0, min(255, GETGREEN(img->tiles[tileIndex].color) + g));
		b = max(0, min(255, GETBLUE(img->tiles[tileIndex].color) + b));
		a = max(0, min(255, GETALPHA(img->tiles[tileIndex].color) + a));
		img->tiles[tileIndex] = tileColor(MAKECOLOR(r, g, b, a), x, y);
	}
	
	void unloadTextureFromGame(void* texture)
	{
		LPDIRECT3DTEXTURE9 tex = reinterpret_cast<LPDIRECT3DTEXTURE9>(texture);
		graphicsExport::unloadTexture(tex);
	}
	
	settlementInfoScroll* getSettlementInfoScroll()
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		if (!gameData->uiManager) 
			return nullptr;
		if (!gameData->uiManager->stratUI) 
			return nullptr;

		return gameData->uiManager->stratUI->settlementInfoScroll;
	}

	settlementTextStrings* getUIStrings(const settlementInfoScroll* settlementInfoScroll)
	{
		if (!settlementInfoScroll->settlementStatsTable) 
			return nullptr;
		return settlementInfoScroll->settlementStatsTable->settlementTextStrings;
	}

	int getLocalFactionID()
	{
		const gameDataAllStruct* gameData = gameDataAllHelper::get();
		return *gameData->localFactionID;
	}

	void setPerfectSpy(const bool set)
	{
		if (smallFuncs::getGameVersion() == 1)
			*reinterpret_cast<bool*>(0x016F0E5C) = set;
		else
			*reinterpret_cast<bool*>(0x016A7CC4) = set;
	}

	void setEquipmentCosts(const int equipType, const int cost)
	{
		struct EquipmentCosts
		{
			int ram;
			int ladder;
			int siegeTower;
		};
		EquipmentCosts* costs;
		if (smallFuncs::getGameVersion() == 1)
			costs = reinterpret_cast<EquipmentCosts*>(0x01655BB0);
		else
			costs = reinterpret_cast<EquipmentCosts*>(0x0160DCC8);
		switch (equipType)
		{
			case 0:
				costs->ram = cost;
				break;
			case 1:
				costs->ladder = cost;
				break;
			case 2:
				costs->siegeTower = cost;
				break;
			default:
				break;
		}
	}

	options1* getOptions1()
	{
		if (smallFuncs::getGameVersion() == 1)
			return reinterpret_cast<options1*>(0x02CB693C);
		
		return reinterpret_cast<options1*>(0x02C6D804);
	}

	options2* getOptions2()
	{
		if (smallFuncs::getGameVersion() == 1)
			return reinterpret_cast<options2*>(0x016818A0);
		
		return reinterpret_cast<options2*>(0x01639EF0);
	}

	campaignDifficulty1* getCampaignDifficulty1()
	{
		if (smallFuncs::getGameVersion() == 1)
			return reinterpret_cast<campaignDifficulty1*>(0x016F4760);
		
		return reinterpret_cast<campaignDifficulty1*>(0x016AB5C8);
	}

	campaignDifficulty2* getCampaignDifficulty2()
	{
		if (smallFuncs::getGameVersion() == 1)
			return reinterpret_cast<campaignDifficulty2*>(0x0164BF18);
		
		return reinterpret_cast<campaignDifficulty2*>(0x01604000);
	}

}
