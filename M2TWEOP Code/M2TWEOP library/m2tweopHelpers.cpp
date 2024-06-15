#include "m2tweopHelpers.h"

#include "FastFuncts.h"
#include "gameDataAllHelper.h"
#include "gameHelpers.h"
#include "graphicsD3D.h"
#include "m2tweopMapManager.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "faction.h"

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
	bool isTileFree(int x, int y)
	{
		int xy[2]{ x,y };
		return smallFuncs::isTileFree(xy);
	}
	
	std::tuple<int, int> getGameTileCoordsWithCursor()
	{
		int x = 0;
		int y = 0;
		fastFuncts::GetGameTileCoordsWithCursor(x, y);

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
		return smallFuncs::getStratMap()->regions[regionID].factionOwner;
	}
	bool checkDipStance(const campaign* campaignStruct, const campaignEnums::dipRelEnum dipType, const factionStruct* fac1, const factionStruct* fac2)
	{
		using namespace campaignEnums;
		if (!fac1 || !fac2)
			return false;
		const auto facDiplomacy = campaignStruct->diplomaticStandings[fac1->factionID][fac2->factionID];
		if (dipType == trade)
			return facDiplomacy.trade & 1;
		const auto state = facDiplomacy.state;
		if (dipType == war)
			return state == warState;
		if (dipType == peace)
			return state == peaceState;
		if (dipType == alliance)
			return state == allianceState;
		if (dipType == suzerain)
			return facDiplomacy.protectorate & 8u;
		return false;
	}
	
	void setDipStance(campaign* campaignStruct, campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2)
	{
		using namespace campaignEnums;
		if (!fac1 || !fac2 || !fac1->factionRecord || !fac2->factionRecord)
			return;
		const auto facOneName = std::string(fac1->factionRecord->facName);
		const auto facTwoName = std::string(fac2->factionRecord->facName);
		const std::string command = "diplomatic_stance " + facOneName + " " + facTwoName + " ";
		if (dipType == dipRelEnum::war)
			smallFuncs::scriptCommand("console_command", (command + "war").c_str());
		else if (dipType == dipRelEnum::peace)
			smallFuncs::scriptCommand("console_command", (command + "neutral").c_str());
		else if (dipType == dipRelEnum::alliance)
			smallFuncs::scriptCommand("console_command", (command + "allied").c_str());
		else if (dipType == dipRelEnum::suzerain)
			setFactionProtectorate(fac1, fac2);
		else if (dipType == dipRelEnum::trade)
			setFactionTrade(fac1, fac2);
	}

	culturesDB* getCultureDb()
	{
		DWORD offset = 0x0161F0F0;
		if (smallFuncs::getGameVersion() == 1)
			offset = 0x01666FC8;
		return reinterpret_cast<culturesDB*>(offset);
	}

	void setFactionTrade(factionStruct* factionOne, factionStruct* factionTwo)
	{
		auto campaign = gameDataAllHelper::get()->campaignData;
		DWORD funcAddr = 0x00503480;
		if (smallFuncs::getGameVersion() == 1)
			funcAddr = 0x00502EE0;
		DWORD diplomaticStuff = (reinterpret_cast<DWORD>(campaign) + 0x858);
		int facIdOne = factionOne->factionID;
		int facIdTwo = factionTwo->factionID;
		auto set = (campaign->diplomaticStandings[facIdOne][facIdTwo].trade & 1) == 0;
		_asm
		{
			push set
			push facIdTwo
			push facIdOne
			mov ecx, diplomaticStuff
			mov eax, funcAddr
			call eax
		}
	}

	void setFactionProtectorate(factionStruct* factionOne, factionStruct* factionTwo)
	{
		auto campaign = gameDataAllHelper::get()->campaignData;
		DWORD funcAddr = 0x00504F20;
		if (smallFuncs::getGameVersion() == 1)
			funcAddr = 0x00504980;
		DWORD diplomaticStuff = (reinterpret_cast<DWORD>(campaign) + 0x858);
		//
		int facIdOne = factionOne->factionID;
		int facIdTwo = factionTwo->factionID;
		_asm
		{
			push facIdTwo
			push facIdOne
			mov ecx, diplomaticStuff
			mov eax, funcAddr
			call eax
		}
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
			const coordPair* tileCoords = gameHelpers::convertTileCoords(tileIndex);
			if (tileIndex >= img->tiles.size())
				img->tiles.resize(tileIndex + 1);
			img->tiles[tileIndex] = tileColor(color, tileCoords->xCoord, tileCoords->yCoord);
			delete tileCoords;
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
			const coordPair* tileCoords = gameHelpers::convertTileCoords(tileIndex);
			if (tileIndex >= img->tiles.size())
				img->tiles.resize(tileIndex + 1);
			const auto newR = max(0, min(255, GETRED(img->tiles[tileIndex].color) + r));
			const auto newG = max(0, min(255, GETGREEN(img->tiles[tileIndex].color) + g));
			const auto newB = max(0, min(255, GETBLUE(img->tiles[tileIndex].color) + b));
			const auto newA = max(0, min(255, GETALPHA(img->tiles[tileIndex].color) + a));
			const uint32_t color = MAKECOLOR(newR, newG, newB, newA);
			img->tiles[tileIndex] = tileColor(color, tileCoords->xCoord, tileCoords->yCoord);
			delete tileCoords;
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
