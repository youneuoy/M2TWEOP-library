﻿///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "pch.h"
#include "strategyMap.h"

#include "character.h"
#include "gameHelpers.h"
#include "luaPlugin.h"
#include "unit.h"
#include "faction.h"
#include "army.h"
#include "campaignAi.h"
#include "casModelsDrawer.h"
#include "dataOffsets.h"
#include "settlement.h"
#include "fort.h"
#include "globals.h"
#include "tilesChange.h"

extentColor extentColors::m_Own =         { 0   , 0xFF, 0x20, 0x28, 0xFF };
extentColor extentColors::m_Enemy =       { 0xFF, 0xFF, 0   , 0x28, 0xFF };
extentColor extentColors::m_Zoc =         { 0xA0, 0x00, 0   , 0x00, 0xc8 };
std::array<std::vector<settlementStruct*>, 200> minorSettlementDb::regionMinorSettlements = {};

oneTile* landingTile::getTile()
{
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[tileId];
}

oneTile* landingPoint::getTile()
{
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[tileIndex];
}

void oneTile::setTileHeight(float height)
{
	const auto doubleTile = tileToDoubleTile();
	if (!doubleTile)
		return;
	doubleTile->height = height;
	stratMapHelpers::updateTerrain();
}

void oneTile::setTileGroundType(const int ground)
{
	const auto doubleTile = tileToDoubleTile();
	groundType = ground;
	if (!doubleTile)
		return;
	doubleTile->groundType = static_cast<int8_t>(ground);
	stratMapHelpers::updateTerrain();
}

std::vector<settlementStruct*> minorSettlementDb::getMinorSettlements(const int regionId)
{
	return regionMinorSettlements[regionId];
}

void minorSettlementDb::addToMinorSettlements(const int regionId, settlementStruct* settlement)
{
	regionMinorSettlements[regionId].push_back(settlement);
}

settlementStruct* minorSettlementDb::getSettlement(const int regionId, const int settlementIndex)
{
	const auto setts = regionMinorSettlements[regionId];
	for (const auto& sett : setts)
	{
		if (sett->getMinorSettlementIndex() == settlementIndex)
			return sett;
	}
	gameHelpers::logStringGame("Minor settlement not found " + std::to_string(settlementIndex) + " regionId: " + std::to_string(regionId));
	return nullptr;
}

int minorSettlementDb::getSettlementCount(const int regionId)
{
	return static_cast<int>(regionMinorSettlements[regionId].size());
}

settlementStruct* minorSettlementDb::getSettlementAtIndex(const int regionId, const int index)
{
	const auto setts = regionMinorSettlements[regionId];
	if (index < 0 || index >= static_cast<int>(setts.size()))
	{
		gameHelpers::logStringGame("Minor settlement index out of bounds " + std::to_string(index) + " regionId: " + std::to_string(regionId));
		return nullptr;
	}
	return regionMinorSettlements[regionId][index];
}

void oneTile::setTileClimate(const int climate)
{
	const auto doubleTile = tileToDoubleTile();
	if (!doubleTile)
		return;
	doubleTile->climate = static_cast<int8_t>(climate);
	stratMapHelpers::updateTerrain();
}
oneTileDouble* oneTile::tileToDoubleTile()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	const int mapWidth = (map->mapWidth << 1) + 1;
	const int x = (getTileX() << 1) + 1;
	const int y = (getTileY() << 1) + 1;
	if (x < 0 || y < 0 || x >= mapWidth + 1 || y >= map->mapHeight * 2 + 1)
		return nullptr;
	return &map->climateTileArray[y * mapWidth + x];
}

void regionStruct::changeRegionName(const char* newName)
{
	const auto nameMem = new UNICODE_STRING*;
	localizedRegionName = nameMem;
	gameStringHelpers::createUniString(localizedRegionName, newName);
	eopSettlementDataDb::get()->getSettlementData(regionID, 0)->regionName = newName;
}

void regionStruct::changeRebelsName(const char* newName)
{
	const auto nameMem = new UNICODE_STRING*;
	localizedRebelsName = nameMem;
	gameStringHelpers::createUniString(localizedRebelsName, newName);
	eopSettlementDataDb::get()->getSettlementData(regionID, 0)->regionRebelsName = newName;
}

bool regionStruct::hasHiddenResource(const char* newName)
{
	const auto edb = eopBuildings::getEdb();
	if (!edb)
		return false;
	const int index = edb->getHiddenResourceIndex(newName);
	if (index < 0)
		return false;
	return hasHiddenResourceId(index);
}

void regionStruct::setHiddenResource(const char* name, const bool enable)
{
	const auto edb = eopBuildings::getEdb();
	if (!edb)
		return;
	const int index = edb->getHiddenResourceIndex(name);
	if (index < 0)
		return;
	setHiddenResourceId(index, enable);
}

std::pair<int, int> oneTile::getTileCoords()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return {-1, -1};
	const int index = this - map->tilesArr;
	const int y = index / map->mapWidth;
	const int x = index - y * map->mapWidth;
	return {x, y};
}
	
int oneTile::getTileX()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return -1;
	const int index = this - map->tilesArr;
	const int y = index / map->mapWidth;
	return index - y * map->mapWidth;
}
	
armyStruct* oneTile::getArmy(const bool onlyLead)
{
	if (const auto tileChar = getCharacter(); tileChar)
	{
		if (tileChar->armyLeaded || onlyLead)
			return tileChar->armyLeaded;
		if (tileChar->armyNotLeaded)
			return tileChar->armyNotLeaded;
	}
	if (onlyLead)
		return nullptr;
	if (const auto settPtr = getSettlement(); settPtr)
		return settPtr->army;
	if (const auto fortPtr = getFort(); fortPtr)
		return fortPtr->army;
	return nullptr;
}

int oneTile::getTileY()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return -1;
	const int index = this - map->tilesArr;
	return index / map->mapWidth;
}

oneTile* landingPoint::getSeaTile()
{
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[seaTileIndex];
}

oneTile* neighbourRegion::getBorderTile(const int index)
{
	if (index < 0 || index >= borderTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[borderTiles[index]];
}

oneTile* neighbourRegion::getFrontierTile(const int index)
{
	if (index < 0 || index >= frontierTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[frontierTiles[index]];
}

oneTile* neighbourRegion::getAmbushTile(const int index)
{
	if (index < 0 || index >= ambushTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[ambushTiles[index]];
}

oneTile* neighbourRegion::getDeepFrontierTile(const int index)
{
	if (index < 0 || index >= deepFrontierTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[deepFrontierTiles[index]];
}

oneTile* neighbourRegion::getBorderCrossing(const int index)
{
	if (index < 0 || index >= passableTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[passableTiles[index]];
}

std::string regionStruct::getLocalizedName()
{
	return gameStringHelpers::uniStringToStr(localizedRegionName);
}

std::string regionStruct::getLocalizedRebelsName()
{
	return gameStringHelpers::uniStringToStr(localizedRebelsName);
}

int regionStruct::settlementCount()
{
	return minorSettlementDb::getSettlementCount(regionID);
}

int regionStruct::getResourcesValue()
{
	int value = 0;
	for (int i = 0; i < resourcesNum; i++)
		value += resources[i]->stratMod->resource_cost;
	return value;
}

void regionStruct::calculateRegionStrengths(const int factionId, regionStrengths* strengths)
{
	GAME_FUNC(void(__cdecl*)(int, int, regionStrengths*),
		calculateRegionStrengths)(factionId, regionID, strengths);
}

void regionStruct::calculateBackupRegionStrengths(regionStrengths* strengths, int* enemyNum, int* neutralNum)
{
	GAME_FUNC(void(__cdecl*)(int, regionStrengths*, int*, int*),
		calculateBackupRegionStrengths)(regionID, strengths, enemyNum, neutralNum);
}

settlementStruct* regionStruct::getSettlement(const int index)
{
	return minorSettlementDb::getSettlementAtIndex(regionID, index);
}

bool regionStruct::hasFaction(const int factionId)
{
	const int settCount = settlementCount();
	if (settCount == 1)
		return factionOwner->factionID == factionId;
	for (int i = 0; i < settCount; i++)
	{
		if (const auto sett = getSettlement(i); sett)
		{
			if (sett->faction->factionID == factionId)
				return true;
		}
	}
	return false;
}

bool regionStruct::hasAlliesToFaction(int factionId, bool trustedOnly)
{
	const int settCount = settlementCount();
	const auto campaignData = campaignHelpers::getCampaignData();
	const auto fac = campaignData->getFactionById(factionId);
	for (int i = 0; i < settCount; i++)
	{
		if (const auto sett = getSettlement(i); sett)
		{
			if (sett->faction->factionID == factionId)
				return true;
			if (trustedOnly)
				return fac->aiFaction->ltgd->isTrustedAlly(sett->faction->factionID);
			return sett->isAllyToFaction(fac);
		}
	}
	return false;
}

int regionStruct::getEnemySettsToFaction(int factionId)
{
	const auto campaign = campaignHelpers::getCampaignData();
	const int settCount = settlementCount();
	if (settCount == 1)
		return campaign->getFactionDiplomacy(factionId, factionOwner->factionID)->state > dipStance::peace ? 1 : 0;
	int enemyNum = 0;
	for (int i = 0; i < settCount; i++)
	{
		if (const auto sett = getSettlement(i); sett)
		{
			if (campaign->getFactionDiplomacy(factionId, sett->faction->factionID)->state > dipStance::peace)
				enemyNum++;
		}
	}
	return enemyNum;
}

int regionStruct::getNeutralSettsToFaction(int factionId)
{
	const auto campaign = campaignHelpers::getCampaignData();
	const int settCount = settlementCount();
	if (settCount == 1)
		return factionOwner->factionID != factionId ? 1 : 0;
	int facNum = 0;
	for (int i = 0; i < settCount; i++)
	{
		if (const auto sett = getSettlement(i); sett)
		{
			if (sett->faction->factionID != factionId)
				facNum++;
		}
	}
	return facNum;
}

oneTile* regionStruct::getPatrolPoint(const int index)
{
	if (index < 0 || index >= patrolPointsCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[patrolPoints[index]];
}

oneTile* regionStruct::getDevastatedTile(const int index)
{
	if (index < 0 || index >= devastatedTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[devastatedTiles[index]];
}

oneTile* regionStruct::getTile(int index)
{
	if (index < 0 || index >= tileCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[tiles[index]];
}

oneTile* regionStruct::getFertileTile(int index)
{
	if (index < 0 || index >= fertileTilesCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[fertileTiles[index]];
}

oneTile* regionStruct::getTileBorderingEdgeOfMap(int index)
{
	if (index < 0 || index >= tilesBorderingEdgeOfMapCount)
		return nullptr;
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[tilesBorderingEdgeOfMap[index]];
}

int oneTile::getTileHeatValue()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return -1;
	const auto climate = getTileClimate();
	return map->climates->climateArray[climate].heatValue;
}
int oneTile::getTileCharacterCount()
{
	int count = 0;
	const auto character = getCharacter();
	if (character)
		count += 1;
	else
		return 0;
	auto thisPtr = static_cast<DWORD*>(character->obj);
	while (thisPtr)
	{
		if (const int objectType = callVFunc<4, int>(thisPtr); objectType == 28)
			count += 1;
		thisPtr = reinterpret_cast<DWORD*>(*(thisPtr + 1));  // NOLINT(performance-no-int-to-ptr)
	}
	return count;
}

character* oneTile::getTileCharacterAtIndex(int index)
{
	int count = 0;
	const auto thisChar = getCharacter();
	if (index == 0)
		return thisChar;
	if (!thisChar)
		return nullptr;
	auto thisPtr = static_cast<DWORD*>(thisChar->obj);
	while (thisPtr)
	{
		if (const int objectType = callVFunc<4, int>(thisPtr); objectType == 28)
		{
			count += 1;
			if (count == index)
				return reinterpret_cast<character*>(thisPtr);
		}
		thisPtr = reinterpret_cast<DWORD*>(*(thisPtr + 1));  // NOLINT(performance-no-int-to-ptr)
	}
	return nullptr;
}

namespace stratMapHelpers
{
	stratMap* getStratMap()
	{
		return gameHelpers::getGameDataAll()->stratMap;
	}

	void updateTerrain()
	{
		GAME_FUNC(void(__cdecl*)(), updateTerrain)();
		GAME_FUNC(void(__thiscall*)(DWORD), updateRadar)(dataOffsets::offsets.uiNotify);
	}

	settlementStruct* getSettlement(stratMap* map, const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto regionId = plugData::data.luaAll.settlements.find(name);
		if (regionId == plugData::data.luaAll.settlements.end()) 
			return nullptr;
		const auto region = &map->regions[regionId->second];
		const int settCount = region->settlementCount();
		for (int j = 0; j < settCount; j++)
		{
			if (const auto settlement = region->getSettlement(j);
				settlement && strcmp(settlement->name, name.c_str()) == 0)
				return settlement;
		}
		return nullptr;
	}

	regionStruct* getRegionByName(stratMap* map, const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto regionId = plugData::data.luaAll.regions.find(name);
		if (regionId == plugData::data.luaAll.regions.end()) 
			return nullptr;
		return &map->regions[regionId->second];
	}
	
	std::pair<int, int> convertTileCoords(const DWORD arrayIndex)
	{
		const auto map = getStratMap();
		if (!map)
			return {-1, -1};
		const int x = arrayIndex % map->mapWidth;
		const int y = arrayIndex / map->mapWidth;
		return {x, y};
	}
	
	regionStruct* getRegion(const int index)
	{
		if (stratMap* map = getStratMap(); map)
			return map->getRegion(index);
		return nullptr;
	}
	
	oneTile* getTile(const int x, const int y)
	{
		if (stratMap* map = getStratMap(); map)
			return map->getTile(x, y);
		return nullptr;
	}
	
	bool isTileFreeLua(int x, int y)
	{
		int xy[2]{x, y};
		return isTileFree(xy);
	}
	
	UINT32 getTileRegionID(int x, int y)
	{
		return getTile(x, y)->regionId;
	}
	
	float getTileMoveCost(int x, int y, int destX, int destY)
	{
		if (const auto map = getStratMap();
			x < 0 || x > map->mapWidth || y < 0 || y > map->mapHeight)
			return -1;
		int start[2] = {x, y};
		int end[2] = {destX, destY};
		return GAME_FUNC(float(__stdcall*)(int*, int*), getTileMoveCost)(start, end);
	}
	
	std::queue<std::pair<int, int>> getNeighbourTiles(int x, int y)
	{
		std::queue<std::pair<int, int>> neighbours;
		neighbours.emplace(x - 1, y);
		neighbours.emplace(x + 1, y);
		neighbours.emplace(x, y - 1);
		neighbours.emplace(x, y + 1);
		neighbours.emplace(x - 1, y + 1);
		neighbours.emplace(x - 1, y - 1);
		neighbours.emplace(x + 1, y + 1);
		neighbours.emplace(x + 1, y - 1);
		return neighbours;
	}

	mapTilesDb* getMapTilesDb()
	{
		return *reinterpret_cast<mapTilesDb**>(dataOffsets::offsets.mapTilesDb);
	}

	void clearAllMapArrows()
	{
		const auto mapTiles = getMapTilesDb();
		const auto arrows = mapTiles->mapArrows;
		for (uint32_t i = 0; i < arrows->pathsNum; i++)
		{
			GAME_FUNC(void(__cdecl*)(uint32_t), clearMapPath)(i);
		}
	}
	
	void clearSundries()
	{
		const auto selectInfoPtr = gameHelpers::getGameDataAll()->selectInfo;
		GAME_FUNC(void(__thiscall*)(selectionInfo*), clearPaths)(selectInfoPtr);
		clearAllMapArrows();
	}
	
	bool isTileValidForCharacterType(const int charType, coordPair* coords)
	{
		if (!GAME_FUNC(bool(__stdcall*)(coordPair*, int, int), isTileValidForCharacter)(coords, charType, 1))
			return false;
		return isTileFree(&coords->xCoord);
	}

	coordPair* findValidTileNearTile(coordPair* coords, const int charType)
	{
		if (isTileValidForCharacterType(charType, coords))
			return coords;
		const auto startCoords = *coords;
		std::queue<std::pair<int, int>> neighbours = getNeighbourTiles(coords->xCoord, coords->yCoord);
		const std::pair<int, int> start = { coords->xCoord, coords->yCoord };
		std::vector<std::pair<int, int>> visited = { start };
		while (true)
		{
			if (neighbours.empty())
				break;
			std::pair<int, int> checkCoord = neighbours.front();
			*coords = { checkCoord.first, checkCoord.second };
			neighbours.pop();
			visited.push_back(checkCoord);
			if (isTileValidForCharacterType(charType, coords))
				return coords;
			std::queue<std::pair<int, int>>  newNeighbours = getNeighbourTiles(checkCoord.first, checkCoord.second);
			while (!newNeighbours.empty())
			{
				auto newCoord = newNeighbours.front();
				newNeighbours.pop();
				bool isVisited = false;
				for (const auto& [xCoord, yCoord] : visited)
				{
					if (xCoord == newCoord.first && yCoord == newCoord.second)
						isVisited = true;
				}
				if (isVisited)
					continue;
				neighbours.push(newCoord);
			}
		}
		*coords = startCoords;
		return coords;
	}
	
	bool isStratMap()
	{
		const auto isStratMap = reinterpret_cast<int*>(dataOffsets::offsets.someStratMapPointer);
		if (isStratMap == nullptr)
			return false;
		if (*isStratMap == 0)
			return false;
		return true;
	}

	std::tuple<int, int> getGameTileCoordsWithCursorLua()
	{
		int x = 0;
		int y = 0;
		getGameTileCoordsWithCursor(x, y);
		return std::make_tuple(x, y);
	}

	float getDistanceInTiles(const int x, const int y, const int destX, const int destY)
	{
		const int dx = x - destX;
		const int dy = y - destY;
		return static_cast<float>(sqrt(static_cast<double>(dx * dx) + static_cast<double>(dy * dy)));
	}
	
	void getGameTileCoordsWithCursor(int& x, int& y)
	{
		const auto mouseOffset = reinterpret_cast<int*>(dataOffsets::offsets.stratCursorCoords);
		x = mouseOffset[0];
		y = mouseOffset[1];
	}
	
	void viewTacticalMap(const int x, const int y)
	{
		globals::dataS.Modules.tacticalMapViewer.view(x, y);
	}
	
	void moveStratCameraSlow(int x, int y)
	{
		if (!isStratMap())
			return;
		DWORD adrFunc = codes::offsets.moveStratCameraSlowFunc;
		_asm
		{
			push y
			push x
			mov eax, adrFunc
			call eax
			add esp, 8
		}
	}

	void moveStratCameraFast(int x, int y)
	{
		if (!isStratMap())
			return;
		DWORD adrFunc = codes::offsets.moveStratCameraFastFunc;
		_asm
		{
			push y
			push x
			mov eax, adrFunc
			call eax
			add esp, 8
		}
	}

	void zoomStratCamera(float zoom)
	{
		if (!isStratMap())
			return;
		DWORD adrFunc = codes::offsets.zoomStratCameraFunc;
		_asm
		{
			push zoom
			mov eax, adrFunc
			call eax
			add esp, 4
		}
	}
	
	void* getMainStratObject(void* baseObj)
	{
		if (baseObj == nullptr)
		{
			return baseObj;
		}

		strategyObject objT = callVFunc<4, strategyObject>(baseObj);
		switch (objT)
		{
		case strategyObject::floatingGeneral:
			break;
		case strategyObject::settlement:
			break;
		case strategyObject::fort:
			break;
		case strategyObject::port:
			break;
		case strategyObject::character:
			break;
		case strategyObject::rallyPointSundry:
			{
				const auto ral = static_cast<rallyPointStruct*>(baseObj);
				if (ral->nextObject == nullptr)
					break;
				return getMainStratObject(ral->nextObject);
			}
		default:
			break;
		}
		return baseObj;
	}
	
	bool isTileFree(int* xy)
	{
		DWORD funcAdr = codes::offsets.isTileFree;
		bool retZ = false;
		_asm {
			push xy
			mov eax, funcAdr
			call eax
			mov retZ, al
		}
		return retZ;
	}

	//legacy
	factionStruct* getRegionOwner(const int regionID)
	{
		return getStratMap()->regions[regionID].factionOwner;
	}
	
    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::usertype<stratMap> stratMap;
            sol::usertype<oneTile> tileStruct;
            sol::usertype<regionStruct> regionStruct;
            sol::usertype<coordPair> coordPair;
            sol::usertype<landMass> landMass;
            sol::usertype<roadStruct> roadStruct;
            sol::usertype<landingTile> landingTile;
            sol::usertype<landingPoint> landingPoint;
            sol::usertype<seaConnectedRegion> seaConnectedRegion;
            sol::usertype<neighbourRegion> neighbourRegion;
        }typeAll;

		///Strat Map
		//@section Strat Map

		/***
		Basic strat map table.

		@tfield int mapWidth
		@tfield int mapHeight
		@tfield int regionsNum
		@tfield int volcanoesNum
		@tfield int landConnectionsNum
		@tfield int landMassNum
		@tfield getRegion getRegion
		@tfield getTile getTile
		@tfield getVolcanoCoords getVolcanoCoords
		@tfield getLandConnection getLandConnection
		@tfield getLandMass getLandMass
		@tfield getSettlement getSettlement
		@tfield getRegionByName getRegionByName
		@tfield moveCamera moveCamera
		@tfield jumpCamera jumpCamera
		@tfield zoomCamera zoomCamera
		@tfield startDrawModelAt startDrawModelAt
		@tfield stopDrawModel stopDrawModel
		@tfield replaceTile replaceTile
		@tfield setOwnExtentsColor setOwnExtentsColor
		@tfield setEnemyExtentsColor setEnemyExtentsColor
		@tfield setZocColor setZocColor

		@table stratMap
		*/
		typeAll.stratMap = luaState.new_usertype<stratMap>("stratMap");
		typeAll.stratMap.set("mapWidth", &stratMap::mapWidth);
		typeAll.stratMap.set("mapHeight", &stratMap::mapHeight);
		typeAll.stratMap.set("regionsNum", &stratMap::regionsNum);
		typeAll.stratMap.set("volcanoesNum", &stratMap::volcanoesNum);
		typeAll.stratMap.set("landConnectionsNum", &stratMap::landMassConnectionsNum);
		typeAll.stratMap.set("landMassNum", &stratMap::landMassNum);

		/***
		Get a specific region by index.
		@function stratMap.getRegion
		@tparam int index
		@treturn regionStruct region
		@usage
		local region = M2TW.stratMap.getRegion(2);
		*/
		typeAll.stratMap.set_function("getRegion", &getRegion);

		/***
		Get a specific tile by it's coordinates.
		@function stratMap.getTile
		@tparam int x
		@tparam int y
		@treturn tileStruct tile
		@usage
		local tile = M2TW.stratMap.getTile(182, 243)
		*/
		typeAll.stratMap.set_function("getTile", &getTile);

		/***
		Get a volcano's coordinates.
		@function stratMap:getVolcanoCoords
		@tparam int index
		@treturn coordPair tile
		@usage
		local volcano = M2TW.stratMap:getVolcanoCoords(0)
		*/
		typeAll.stratMap.set_function("getVolcanoCoords", &stratMap::getVolcanoCoords);

		/***
		Get a land connection's coordinates (the green arrows on the map that allow you to cross bodies of water).
		@function stratMap:getLandConnection
		@tparam int index
		@treturn coordPair tile
		@usage
		local connection = M2TW.stratMap:getLandConnection(0)
		*/
		typeAll.stratMap.set_function("getLandConnection", &stratMap::getLandConnection);

		/***
		Get a landmass (collection of regions reachable by land, like a continent or island).
		@function stratMap:getLandMass
		@tparam int index
		@treturn landMass landMass
		@usage
		local land = M2TW.stratMap:getLandMass(0)
		*/
		typeAll.stratMap.set_function("getLandMass", &stratMap::getLandMass);

		/***
		Get a settlement by its internal name. This is the intended way to get a settlement quickly by it's name.
		@function stratMap:getSettlement
		@tparam string name
		@treturn settlementStruct settlement
		@usage
		local london = M2TW.stratMap:getSettlement("London")
		*/
		typeAll.stratMap.set_function("getSettlement", &getSettlement);

		/***
		Get a region by its internal name.
		@function stratMap:getRegionByName
		@tparam string name
		@treturn regionStruct region
		@usage
		local region = M2TW.stratMap:getRegionByName("London_Province")
		*/
		typeAll.stratMap.set_function("getRegionByName", &getRegionByName);

		/***
		Slowly move the camera to the specified tile.
		@function stratMap.moveCamera
		@tparam int xCoord
		@tparam int yCoord
		@usage
		      M2TW.stratMap.moveCamera(1,2)
		*/
		typeAll.stratMap.set_function("moveCamera", &moveStratCameraSlow);

		/***
		Quickly move the camera to the specified tile.
		@function stratMap.jumpCamera
		@tparam int xCoord
		@tparam int yCoord
		@usage
		      M2TW.stratMap.jumpCamera(1,2)
		*/
		typeAll.stratMap.set_function("jumpCamera", &moveStratCameraFast);

		/***
	    Set the zoom level of the camera.
		@function stratMap.zoomCamera
		@tparam float distance
		@usage
		      M2TW.stratMap.zoomCamera(0.12)
		*/
		typeAll.stratMap.set_function("zoomCamera", &zoomStratCamera);

		/***
		Start drawing .cas campaign strategy model with a unique ID in some coords. Can be used at any time.
		@function stratMap.startDrawModelAt
		@tparam int modelId Unique ID
		@tparam int x
		@tparam int y
		@tparam int sizeMultiplier 1 is value with what it draw as same size as game objects
		@usage
		      M2TWEOP.addModelToGame("data/models_strat/residences/invisible.CAS", 1)
		      M2TW.stratMap.startDrawModelAt(1, 50, 150, 2.25)
		*/
		typeAll.stratMap.set_function("startDrawModelAt", &casModelsDrawer::addCasModelToDrawList);

		/***
		Stop drawing .cas campaign strategy model with a unique ID. Can be used at any time.
	    @function stratMap.stopDrawModel
		@tparam int modelId Unique ID
		@usage
		      M2TWEOP.addModelToGame("data/models_strat/residences/invisible.CAS", 1)
		      M2TW.stratMap.startDrawModelAt(1, 50, 150, 2.25)
		      M2TW.stratMap.stopDrawModel(1)
		*/
		typeAll.stratMap.set_function("stopDrawModel", &casModelsDrawer::removeCasModelFromDrawList);

		/***
		Replace a custom tile. Change the custom battlefield on the specified coordinates.
		@function stratMap.replaceTile
		@tparam string label  Identifier.
		@tparam int xCoord  X coordinate of tile.
		@tparam int yCoord  Y coordinate of tile.
		@tparam string filename  Just the name, not full path (.wfc)
		@tparam string weather Weather on the battle map.
		@tparam string dayTime Time of day.
		@usage
		M2TW.stratMap.replaceTile("Helms-Deep_Province",167,158,"hornburg_amb.wfc","clear","midday");
		*/
		typeAll.stratMap.set_function("replaceTile", &tilesChange::replaceTile);

		/***
		Set movement extents color.
		@function stratMap.setOwnExtentsColor
		@tparam int r Red
		@tparam int g Green
		@tparam int b Blue
		@tparam int a Alpha
		@tparam int border borderAlpha
		@usage
		     M2TW.stratMap.setOwnExtentsColor(0, 255, 32, 40, 255)
		*/
		typeAll.stratMap.set_function("setOwnExtentsColor", &extentColors::setOwnColor);

		/***
		Set enemy movement extents color.
		@function stratMap.setEnemyExtentsColor
		@tparam int r Red
		@tparam int g Green
		@tparam int b Blue
		@tparam int a Alpha
		@tparam int border borderAlpha
		@usage
		     M2TW.stratMap.setEnemyExtentsColor(255, 255, 0, 40, 255)
		*/
		typeAll.stratMap.set_function("setEnemyExtentsColor", &extentColors::setEnemyColor);

		/***
		Set Zone of Control color.
		@function stratMap.setZocColor
		@tparam int r Red
		@tparam int g Green
		@tparam int b Blue
		@tparam int a Alpha
		@usage
		     M2TW.stratMap.setZocColor(160, 0, 0, 200)
		*/
		typeAll.stratMap.set_function("setZocColor", &extentColors::setZocColor);
		
		/***
		Basic coordPair table.

		@tfield int xCoord
		@tfield int yCoord

		@table coordPair
		*/
		typeAll.coordPair = luaState.new_usertype<coordPair>("coordPair");
		typeAll.coordPair.set("xCoord", &coordPair::xCoord);
		typeAll.coordPair.set("yCoord", &coordPair::yCoord);

		/***
		Basic landMass table.

		@tfield int index
		@tfield int regionsNum
		@tfield getRegionID getRegionID

		@table landMass
		*/
		typeAll.landMass = luaState.new_usertype<landMass>("landMass");
		typeAll.landMass.set("index", &landMass::index);
		typeAll.landMass.set("regionsNum", &landMass::regionsNum);

		/***
		Get a region ID by index.
		@function landMass:getRegionID
		@tparam int index
		@treturn int regionID
		@usage
		local region = landMass:getRegionID(0);
		*/
		typeAll.landMass.set_function("getRegionID", &landMass::getRegion);

		///Road
		//@section Road

		/***
		Basic roadStruct table.

		@tfield int coordsNum
		@tfield int regionIdStart
		@tfield int regionIdEnd
		@tfield int id
		@tfield bool onlyHalf
		@tfield getCoord getCoord
		@tfield int tradeValue --only counts from 1 side (importer)

		@table roadStruct
		*/
		typeAll.roadStruct = luaState.new_usertype<roadStruct>("roadStruct");
		typeAll.roadStruct.set("coordsNum", &roadStruct::coordsNum);
		typeAll.roadStruct.set("regionIdStart", &roadStruct::regionIdStart);
		typeAll.roadStruct.set("regionIdEnd", &roadStruct::regionIdEnd);
		typeAll.roadStruct.set("tradeValue", &roadStruct::tradeValue);
		typeAll.roadStruct.set("id", &roadStruct::id);
		typeAll.roadStruct.set("onlyHalf", &roadStruct::onlyHalf);

		/***
		Get a road coord by index.
		@function roadStruct:getCoord
		@tparam int index
		@treturn coordPair coords
		@usage
		local coords = road:getCoord(0);
		*/
		typeAll.roadStruct.set_function("getCoord", &roadStruct::getCoord);

		///Tile
		//@section Tile

		/***
		Basic tile table.

		@tfield roadStruct road
		@tfield int isLand (1 = land, 0 = sea)
		@tfield int groundType
		@tfield int regionID
		@tfield int choke
		@tfield int xCoord
		@tfield int yCoord
		@tfield int factionID
		@tfield int heatValue
		@tfield int height
		@tfield int climate
		@tfield bool hasRiver
		@tfield bool hasRiverSource
		@tfield bool hasCliff
		@tfield tradeResource|nil resource
		@tfield character|nil character
		@tfield settlementStruct|nil settlement
		@tfield fortStruct|nil fort
		@tfield portStruct|nil port
		@tfield watchtowerStruct|nil watchtower
		@tfield bool hasCrossing
		@tfield bool hasVolcano 
		@tfield bool hasSeaCrossing
		@tfield bool hasRoad
		@tfield int roadLevel
		@tfield int earthquakeLevel
		@tfield int floodLevel
		@tfield int stormLevel
		@tfield int volcanoLevel
		@tfield bool isScorched
		@tfield bool isTradeRoute
		@tfield int devastationLevel
		@tfield bool isDeforested
		@tfield bool isBorder
		@tfield bool isLandCoast
		@tfield bool isSeaCoast
		@tfield bool hasSettlement
		@tfield bool hasFort
		@tfield bool hasDock
		@tfield bool hasPort
		@tfield bool hasWatchtower
		@tfield bool hasNavy
		@tfield bool hasArmy
		@tfield bool isDevastated
		@tfield bool isImpassible
		@tfield float mpModifier
		@tfield factionHasArmyNearTile factionHasArmyNearTile
		@tfield factionHasCharacterOnTile factionHasCharacterOnTile
		@tfield getTileCharacterCount getTileCharacterCount
		@tfield getTileCharacterAtIndex getTileCharacterAtIndex
		
		@table tileStruct
		*/
		typeAll.tileStruct = luaState.new_usertype<oneTile>("tileStruct");
		typeAll.tileStruct.set("road", &oneTile::road);
		typeAll.tileStruct.set("isLand", &oneTile::isLand);
		typeAll.tileStruct.set("groundType", sol::property(&oneTile::getTileGroundType, &oneTile::setTileGroundType));
		typeAll.tileStruct.set("regionID", &oneTile::regionId);
		typeAll.tileStruct.set("choke", &oneTile::choke);
		typeAll.tileStruct.set("xCoord", sol::property(&oneTile::getTileX));
		typeAll.tileStruct.set("yCoord", sol::property(&oneTile::getTileY));
		typeAll.tileStruct.set("factionID", sol::property(&oneTile::getFactionId));
		typeAll.tileStruct.set("heatValue", sol::property(&oneTile::getTileHeatValue));
		typeAll.tileStruct.set("height", sol::property(&oneTile::getTileHeight, &oneTile::setTileHeight));
		typeAll.tileStruct.set("climate", sol::property(&oneTile::getTileClimate, &oneTile::setTileClimate));
		typeAll.tileStruct.set("hasRiver", sol::property(&oneTile::isRiver));
		typeAll.tileStruct.set("hasRiverSource", sol::property(&oneTile::isRiverSource));
		typeAll.tileStruct.set("hasCliff", sol::property(&oneTile::isCliff));
		typeAll.tileStruct.set("resource", sol::property(&oneTile::getResource));
		typeAll.tileStruct.set("character", sol::property(&oneTile::getCharacter));
		typeAll.tileStruct.set("settlement", sol::property(&oneTile::getSettlement));
		typeAll.tileStruct.set("fort", sol::property(&oneTile::getFort));
		typeAll.tileStruct.set("port", sol::property(&oneTile::getPort));
		typeAll.tileStruct.set("watchtower", sol::property(&oneTile::getWatchtower));
		typeAll.tileStruct.set("hasCrossing", sol::property(&oneTile::isCrossing));
		typeAll.tileStruct.set("hasVolcano", sol::property(&oneTile::isVolcano));
		typeAll.tileStruct.set("hasSeaCrossing", sol::property(&oneTile::isSeaCrossing));
		typeAll.tileStruct.set("hasRoad", sol::property(&oneTile::isRoad));
		typeAll.tileStruct.set("roadLevel", sol::property(&oneTile::getRoadLevel));
		typeAll.tileStruct.set("earthquakeLevel", sol::property(&oneTile::getEarthQuake));
		typeAll.tileStruct.set("floodLevel", sol::property(&oneTile::getFlood));
		typeAll.tileStruct.set("stormLevel", sol::property(&oneTile::getStorm));
		typeAll.tileStruct.set("volcanoLevel", sol::property(&oneTile::getVolcanoStrength));
		typeAll.tileStruct.set("isScorched", sol::property(&oneTile::isScorched));
		typeAll.tileStruct.set("isTradeRoute", sol::property(&oneTile::isTradeRoute));
		typeAll.tileStruct.set("devastationLevel", sol::property(&oneTile::getDevastationLevel));
		typeAll.tileStruct.set("isDeforested", sol::property(&oneTile::isDeforested));
		typeAll.tileStruct.set("isBorder", sol::property(&oneTile::isBorder));
		typeAll.tileStruct.set("isLandCoast", sol::property(&oneTile::isLandCoast));
		typeAll.tileStruct.set("isSeaCoast", sol::property(&oneTile::isSeaCoast));
		typeAll.tileStruct.set("hasSettlement", sol::property(&oneTile::hasSettlement));
		typeAll.tileStruct.set("hasFort", sol::property(&oneTile::hasFort));
		typeAll.tileStruct.set("hasDock", sol::property(&oneTile::hasDock));
		typeAll.tileStruct.set("hasPort", sol::property(&oneTile::hasPort));
		typeAll.tileStruct.set("hasWatchtower", sol::property(&oneTile::hasWatchtower));
		typeAll.tileStruct.set("hasNavy", sol::property(&oneTile::hasNavy));
		typeAll.tileStruct.set("hasArmy", sol::property(&oneTile::hasArmy));
		typeAll.tileStruct.set("isImpassible", sol::property(&oneTile::isImpassible));
		typeAll.tileStruct.set("isDevastated", sol::property(&oneTile::isDevastated));
		typeAll.tileStruct.set("mpModifier", &oneTile::mpModifier);

		/***
		Check if a faction has an army near a tile.
		@function tileStruct:factionHasArmyNearTile
		@tparam int factionID
		@treturn bool hasArmyNearTile
		@usage
		local hasArmyNearTile = tile:factionHasArmyNearTile(3);
		*/
		typeAll.tileStruct.set_function("factionHasArmyNearTile", &oneTile::factionHasArmyNearTile);

		/***
		Check if a faction has a character on a tile.
		@function tileStruct:factionHasCharacterOnTile
		@tparam int factionID
		@treturn bool hasCharacterOnTile
		@usage
		local hasCharacterOnTile = tile:factionHasCharacterOnTile(3);
		*/
		typeAll.tileStruct.set_function("factionHasCharacterOnTile", &oneTile::factionHasCharacterOnTile);

		/***
		Get amount of characters on a tile.
		@function tileStruct:getTileCharacterCount
		@treturn int characterCount
		@usage
		local tileCharacterCount = tile:getTileCharacterCount();
		*/
		typeAll.tileStruct.set_function("getTileCharacterCount", &oneTile::getTileCharacterCount);

		/***
		Get a character on a tile.
		@function tileStruct:getTileCharacterAtIndex
		@tparam int index
		@treturn character char
		@usage
		local tileCharacterCount = tile:getTileCharacterCount();
		for i=0,tileCharacterCount -1 do
			local character = tile:getTileCharacterAtIndex(i);
		end
		*/
		typeAll.tileStruct.set_function("getTileCharacterAtIndex", &oneTile::getTileCharacterAtIndex);

		///Region
		//@section Region

		/***
		Basic regionStruct table.

		@tfield string regionName
		@tfield string localizedName
		@tfield string settlementName
		@tfield string legioName
		@tfield int regionID
		@tfield int roadLevel as set in descr_strat
		@tfield int farmingLevel as set in descr_strat
		@tfield int famineThreat
		@tfield int harvestSuccess
		@tfield int armyCount
		@tfield int settlementCount
		@tfield int watchtowerCount
		@tfield int resourceCount
		@tfield int fortCount
		@tfield int colorRed
		@tfield int colorGreen
		@tfield int colorBlue
		@tfield bool isSea
		@tfield bool peninsular
		@tfield landMass landMass
		@tfield roadStruct roadToPort
		@tfield int seaConnectedRegionsCount
		@tfield int loyaltyFactionID
		@tfield int seaImportRegionsCount
		@tfield int tilesBorderingEdgeOfMapCount
		@tfield int devastatedTilesCount
		@tfield settlementStruct regionCapital
		@tfield int tileCount
		@tfield int fertileTilesCount
		@tfield int neighbourRegionsNum
		@tfield int resourceTypesBitMap
		@tfield int regionCapitalX
		@tfield int regionCapitalY
		@tfield int portEntranceXCoord
		@tfield int portEntranceYCoord
		@tfield factionStruct faction
		@tfield mercPool mercPool
		@tfield string rebelType
		@tfield string localizedRebelsName
		@tfield int triumphValue
		@tfield int totalTradeValue
		@tfield int landingPointsCount
		@tfield int patrolPointsCount
		@tfield getStack getStack
		@tfield getArmy getArmy
		@tfield getFort getFort
		@tfield getWatchtower getWatchtower
		@tfield getResource getResource
		@tfield getNeighbour getNeighbour
		@tfield getHiddenResource getHiddenResource
		@tfield setHiddenResource setHiddenResource
		@tfield getSeaConnectedRegion getSeaConnectedRegion
		@tfield getSeaImportRegion getSeaImportRegion
		@tfield getRegionSeaEdge getRegionSeaEdge
		@tfield getReligionHistory getReligionHistory
		@tfield getTileBorderingEdgeOfMap getTileBorderingEdgeOfMap
		@tfield getTile getTile
		@tfield getFertileTile getFertileTile
		@tfield getDevastatedTile getDevastatedTile
		@tfield getHostileArmiesStrength getHostileArmiesStrength
		@tfield hasResourceType hasResourceType
		@tfield getLandingPoint getLandingPoint
		@tfield getSettlement getSettlement
		@tfield getResourcesValue getResourcesValue
		@tfield hasFaction hasFaction
		@tfield getEnemySettsToFaction getEnemySettsToFaction
		@tfield getNeutralSettsToFaction getNeutralSettsToFaction

		@table regionStruct
		*/
		typeAll.regionStruct = luaState.new_usertype<regionStruct>("regionStruct");
		typeAll.regionStruct.set("regionName", &regionStruct::regionName);
		typeAll.regionStruct.set("settlementName", &regionStruct::settlementName);
		typeAll.regionStruct.set("localizedName", sol::property(
			&regionStruct::getLocalizedName, &regionStruct::changeRegionName
			));
		typeAll.regionStruct.set("legioName", &regionStruct::legioName);
		typeAll.regionStruct.set("colorRed", &regionStruct::colorRed);
		typeAll.regionStruct.set("colorGreen", &regionStruct::colorGreen);
		typeAll.regionStruct.set("colorBlue", &regionStruct::colorBlue);
		typeAll.regionStruct.set("regionID", &regionStruct::regionID);
		typeAll.regionStruct.set("landingPointsCount", &regionStruct::landingPointsCount);
		typeAll.regionStruct.set("loyaltyFactionID", &regionStruct::loyaltyFactionID);
		typeAll.regionStruct.set("roadLevel", &regionStruct::roadLevel);
		typeAll.regionStruct.set("farmingLevel", &regionStruct::farmingLevel);
		typeAll.regionStruct.set("famineThreat", &regionStruct::famineThreat);
		typeAll.regionStruct.set("harvestSuccess", &regionStruct::harvestSuccess);
		typeAll.regionStruct.set("totalTradeValue", &regionStruct::totalTradeValue);
		typeAll.regionStruct.set("seaConnectedRegionsCount", &regionStruct::seaConnectedRegionsCount);
		typeAll.regionStruct.set("seaImportRegionsCount", &regionStruct::seaImportRegionsCount);
		typeAll.regionStruct.set("landMass", &regionStruct::landMass);
		typeAll.regionStruct.set("roadToPort", &regionStruct::roadToPort);
		typeAll.regionStruct.set("patrolPointsCount", &regionStruct::patrolPointsCount);
		typeAll.regionStruct.set("devastatedTilesCount", &regionStruct::devastatedTilesCount);
		typeAll.regionStruct.set("tilesBorderingEdgeOfMapCount", &regionStruct::tilesBorderingEdgeOfMapCount);
		typeAll.regionStruct.set("fertileTilesCount", &regionStruct::fertileTilesCount);
		typeAll.regionStruct.set("resourceTypesBitMap", &regionStruct::resourceTypesBitMap);
		typeAll.regionStruct.set("stacksNum", &regionStruct::stacksNum);
		typeAll.regionStruct.set("armyCount", &regionStruct::stacksNum);
		typeAll.regionStruct.set("fortsNum", &regionStruct::fortsNum);
		typeAll.regionStruct.set("fortCount", &regionStruct::fortsNum);
		typeAll.regionStruct.set("watchtowersNum", &regionStruct::watchtowersNum);
		typeAll.regionStruct.set("watchtowerCount", &regionStruct::watchtowersNum);
		typeAll.regionStruct.set("settlementCount", sol::property(&regionStruct::settlementCount));
		typeAll.regionStruct.set("isSea", &regionStruct::isSea);
		typeAll.regionStruct.set("peninsular", &regionStruct::peninsular);
		typeAll.regionStruct.set("mercPool", &regionStruct::mercPool);
		typeAll.regionStruct.set("settlement", &regionStruct::settlement);
		typeAll.regionStruct.set("regionCapital", &regionStruct::settlement);
		typeAll.regionStruct.set("tileCount", &regionStruct::tileCount);
		typeAll.regionStruct.set("neighbourRegionsNum", &regionStruct::neighbourRegionsNum);
		typeAll.regionStruct.set("resourcesNum", &regionStruct::resourcesNum);
		typeAll.regionStruct.set("resourceCount", &regionStruct::resourcesNum);
		typeAll.regionStruct.set("settlementXCoord", &regionStruct::settlementXCoord);
		typeAll.regionStruct.set("settlementYCoord", &regionStruct::settlementYCoord);
		typeAll.regionStruct.set("regionCapitalX", &regionStruct::settlementXCoord);
		typeAll.regionStruct.set("regionCapitalY", &regionStruct::settlementYCoord);
		typeAll.regionStruct.set("portEntranceXCoord", &regionStruct::portEntranceXCoord);
		typeAll.regionStruct.set("portEntranceYCoord", &regionStruct::portEntranceYCoord);
		typeAll.regionStruct.set("faction", &regionStruct::factionOwner);
		typeAll.regionStruct.set("rebelType", &regionStruct::rebelType);
		typeAll.regionStruct.set("localizedRebelsName", sol::property(
			&regionStruct::getLocalizedRebelsName, &regionStruct::changeRebelsName
			));
		typeAll.regionStruct.set("triumphValue", &regionStruct::triumphValue);

		/***
		Get an army by it's index.
		@function regionStruct:getArmy
		@tparam int index
		@treturn armyStruct army
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local army = region:getArmy(0)
		*/
		typeAll.regionStruct.set_function("getArmy", &regionStruct::getArmy);
		typeAll.regionStruct.set_function("getStack", &regionStruct::getArmy);

		/***
		Get a fort by it's index.
		@function regionStruct:getFort
		@tparam int index
		@treturn fortStruct fort
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local fort = region:getFort(0)
		*/
		typeAll.regionStruct.set_function("getFort", &regionStruct::getFort);

		/***
		Get a settlement by it's index (iterating).
		@function regionStruct:getSettlement
		@tparam int index
		@treturn settlementStruct sett
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local sett = region:getSettlement(0)
		*/
		typeAll.regionStruct.set_function("getSettlement", &regionStruct::getSettlement);

		/***
		Get a watchtower by it's index.
		@function regionStruct:getWatchtower
		@tparam int index
		@treturn watchtowerStruct watchtower
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local watch = region:getWatchtower(0)
		*/
		typeAll.regionStruct.set_function("getWatchtower", &regionStruct::getWatchTower);

		/***
		Get a resource by it's index.
		@function regionStruct:getResource
		@tparam int index
		@treturn tradeResource resource
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local res = region:getResource(0)
		*/
		typeAll.regionStruct.set_function("getResource", &regionStruct::getResource);

		/***
		Get a neighbour region by it's index.
		@function regionStruct:getNeighbour
		@tparam int index
		@treturn neighbourRegion nRegion
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local nRegion = region:getNeighbour(0)
		*/
		typeAll.regionStruct.set_function("getNeighbour", &regionStruct::getNeighbourRegion);

		/***
		Check if a region has a hidden resource.
		@function regionStruct:getHiddenResource
		@tparam string name
		@treturn bool hr
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		localhr = region:getHiddenResource("resource_name")
		*/
		typeAll.regionStruct.set_function("getHiddenResource", &regionStruct::hasHiddenResource);

		/***
		Set a region's hidden resource (reset on game restart).
		@function regionStruct:setHiddenResource
		@tparam string name
		@tparam bool enable
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		region:setHiddenResource("resource_name", false)
		*/
		typeAll.regionStruct.set_function("setHiddenResource", &regionStruct::setHiddenResource);

		/***
		Get a region that is reachable from this region.
		@function regionStruct:getSeaConnectedRegion
		@tparam int index
		@treturn seaConnectedRegion connectedRegion
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local connectedRegion = region:getSeaConnectedRegion(0)
		*/
		typeAll.regionStruct.set_function("getSeaConnectedRegion", &regionStruct::getSeaConnectedRegion);

		/***
		Get a region this region is importing trade goods from.
		@function regionStruct:getSeaImportRegion
		@tparam int index
		@treturn seaConnectedRegion seaImportRegion
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local seaImportRegion = region:getSeaImportRegion(0)
		*/
		typeAll.regionStruct.set_function("getSeaImportRegion", &regionStruct::getSeaImportRegion);

		/***
		Get a region sea edge (point where it borders both sea and another land region).
		@function regionStruct:getRegionSeaEdge
		@tparam int index
		@treturn tileStruct edge
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local edge = region:getRegionSeaEdge(0)
		*/
		typeAll.regionStruct.set_function("getRegionSeaEdge", &regionStruct::getPatrolPoint);

		/***
		Get a devastated tile.
		@function regionStruct:getDevastatedTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local tile = region:getDevastatedTile(0)
		*/
		typeAll.regionStruct.set_function("getDevastatedTile", &regionStruct::getDevastatedTile);

		/***
		Get a tile that borders the edge of the map.
		@function regionStruct:getTileBorderingEdgeOfMap
		@tparam int index
		@treturn tileStruct edge
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local edge = region:getTileBorderingEdgeOfMap(0)
		*/
		typeAll.regionStruct.set_function("getTileBorderingEdgeOfMap", &regionStruct::getTileBorderingEdgeOfMap);

		/***
		Get a tile by index.
		@function regionStruct:getTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local tile = region:getTile(0)
		*/
		typeAll.regionStruct.set_function("getTile", &regionStruct::getTile);

		/***
		Get a landing point by index.
		@function regionStruct:getLandingPoint
		@tparam int index
		@treturn landingPoint point
		@usage
		local region = M2TW.stratMap.getRegion(2)
		local point = region:getLandingPoint(0)
		*/
		typeAll.regionStruct.set_function("getLandingPoint", &regionStruct::getLandingPoint);

		/***
		Get a fertile tile by index.
		@function regionStruct:getFertileTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local tile = region:getFertileTile(0)
		*/
		typeAll.regionStruct.set_function("getFertileTile", &regionStruct::getFertileTile);

		/***
		Get religion amount from a set number of turns ago.
		@function regionStruct:getReligionHistory
		@tparam int religionID
		@tparam int turnsAgo (max 19)
		@treturn float religionAmount
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local catholicThreeTurnsAgo = region:getReligionHistory(0, 3)
		*/
		typeAll.regionStruct.set_function("getReligionHistory", &regionStruct::getReligionHistory);

		/***
		Check if region has a resource type.
		@function regionStruct:hasResourceType
		@tparam int resourceID
		@treturn bool hasResource
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local hasResource = region:hasResourceType(16)
		*/
		typeAll.regionStruct.set_function("hasResourceType", &regionStruct::hasResourceType);

		/***
		Get total value of all resources in the region.
		@function regionStruct:getResourcesValue
		@treturn int value
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(5)
		local value = region:getResourcesValue()
		*/
		typeAll.regionStruct.set_function("getResourcesValue", &regionStruct::getResourcesValue);

		/***
		Check if the region contains a settlement owned by the specified faction.
		@function regionStruct:hasFaction
		@tparam int factionID
		@treturn bool result
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(5)
		local check = region:hasFaction(3)
		*/
		typeAll.regionStruct.set_function("hasFaction", &regionStruct::hasFaction);

		/***
		Get number of settlements in the region that are hostile to specified faction.
		@function regionStruct:getEnemySettsToFaction
		@tparam int factionID
		@treturn int num
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(5)
		local num = region:getEnemySettsToFaction(3)
		*/
		typeAll.regionStruct.set_function("getEnemySettsToFaction", &regionStruct::getEnemySettsToFaction);
		
		/***
		Get number of settlements in the region that are neutral to specified faction.
		@function regionStruct:getNeutralSettsToFaction
		@tparam int factionID
		@treturn int num
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(5)
		local num = region:getNeutralSettsToFaction(3)
		*/
		typeAll.regionStruct.set_function("getNeutralSettsToFaction", &regionStruct::getNeutralSettsToFaction);

		/***
		Get the strength total of all armies in this region that are hostile to a specific faction.
		@function regionStruct:getHostileArmiesStrength
		@tparam int factionID
		@treturn int totalStrength
		@usage
		local map = M2TW.stratMap
		local region = map.getRegion(2)
		local totalStrength = region:getHostileArmiesStrength(myFac.factionID)
		*/
		typeAll.regionStruct.set_function("getHostileArmiesStrength", &regionStruct::getHostileArmiesStrength);
		
		/***
		Basic landingPoint table.

		@tfield tileStruct landTile
		@tfield float moveCost From water to land
		@tfield tileStruct seaTile

		@table landingPoint
		*/
		typeAll.landingPoint = luaState.new_usertype<landingPoint>("landingPoint");
		typeAll.landingPoint.set("landTile", sol::property(&landingPoint::getTile));
		typeAll.landingPoint.set("moveCost", &landingPoint::moveCost);
		typeAll.landingPoint.set("seaTile", sol::property(&landingPoint::getSeaTile));

		///Neighbour Region
		//@section Neighbour Region

		/***
		Basic neighbourRegion table.

		@tfield int regionID
		@tfield regionStruct region
		@tfield int tradeValue
		@tfield bool isBlocked
		@tfield float moveCost from settlement to settlement
		@tfield int borderTilesCount
		@tfield int frontierTilesCount
		@tfield int ambushTilesCount
		@tfield int deepFrontierTilesCount
		@tfield int borderCrossingCount
		@tfield roadStruct connectingRoad
		@tfield getBorderTile getBorderTile
		@tfield getBorderCrossing getBorderCrossing
		@tfield getDeepFrontierTile getDeepFrontierTile
		@tfield getAmbushTile getAmbushTile
		@tfield getFrontierTile getFrontierTile


		@table neighbourRegion
		*/
		typeAll.neighbourRegion = luaState.new_usertype<neighbourRegion>("neighbourRegion");
		typeAll.neighbourRegion.set("regionID", &neighbourRegion::regionID);
		typeAll.neighbourRegion.set("tradeValue", &neighbourRegion::tradeValue);
		typeAll.neighbourRegion.set("region", &neighbourRegion::region);
		typeAll.neighbourRegion.set("borderTilesCount", &neighbourRegion::borderTilesCount);
		typeAll.neighbourRegion.set("frontierTilesCount", &neighbourRegion::frontierTilesCount);
		typeAll.neighbourRegion.set("ambushTilesCount", &neighbourRegion::ambushTilesCount);
		typeAll.neighbourRegion.set("deepFrontierTilesCount", &neighbourRegion::deepFrontierTilesCount);
		typeAll.neighbourRegion.set("borderCrossingCount", &neighbourRegion::passableTilesCount);
		typeAll.neighbourRegion.set("connectingRoad", &neighbourRegion::connectingRoad);
		typeAll.neighbourRegion.set("isBlocked", &neighbourRegion::isBlocked);
		typeAll.neighbourRegion.set("moveCost", &neighbourRegion::moveCost);

		/***
		Get a border tile by index.
		@function neighbourRegion:getBorderTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local tile = nRegion:getBorderTile(0)
		*/
		typeAll.neighbourRegion.set_function("getBorderTile", &neighbourRegion::getBorderTile);

		/***
		Get a frontier tile by index.
		@function neighbourRegion:getFrontierTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local tile = nRegion:getFrontierTile(0)
		*/
		typeAll.neighbourRegion.set_function("getFrontierTile", &neighbourRegion::getFrontierTile);

		/***
		Get an ambush tile by index.
		@function neighbourRegion:getAmbushTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local tile = nRegion:getAmbushTile(0)
		*/
		typeAll.neighbourRegion.set_function("getAmbushTile", &neighbourRegion::getAmbushTile);

		/***
		Get a deep frontier tile by index.
		@function neighbourRegion:getDeepFrontierTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local tile = nRegion:getDeepFrontierTile(0)
		*/
		typeAll.neighbourRegion.set_function("getDeepFrontierTile", &neighbourRegion::getDeepFrontierTile);

		/***
		Get a border tile that is passable to other region.
		@function neighbourRegion:getBorderCrossing
		@tparam int index
		@treturn tileStruct tile
		@usage
		local tile = nRegion:getBorderCrossing(0)
		*/
		typeAll.neighbourRegion.set_function("getBorderCrossing", &neighbourRegion::getBorderCrossing);

		
		///Sea Connected Region
		//@section Sea Connected Region

		/***
		Basic seaConnectedRegion table.

		@tfield int regionID
		@tfield int seaExportValue
		@tfield int seaImportValue
		@tfield int tilesReachableCount
		@tfield int seaTradeLanePathCount
		@tfield getReachableTile getReachableTile
		@tfield getTradeLaneCoord getTradeLaneCoord
		

		@table seaConnectedRegion
		*/
		typeAll.seaConnectedRegion = luaState.new_usertype<seaConnectedRegion>("seaConnectedRegion");
		typeAll.seaConnectedRegion.set("regionID", &seaConnectedRegion::regionID);
		typeAll.seaConnectedRegion.set("seaExportValue", &seaConnectedRegion::seaExportValue);
		typeAll.seaConnectedRegion.set("seaImportValue", &seaConnectedRegion::seaImportValue);
		typeAll.seaConnectedRegion.set("tilesReachableCount", &seaConnectedRegion::tilesReachableCount);
		typeAll.seaConnectedRegion.set("seaTradeLanePathCount", &seaConnectedRegion::seaTradeLanePathCount);

		/***
		Get a reachable tile by index.
		@function seaConnectedRegion:getReachableTile
		@tparam int index
		@treturn landingTile tile
		@usage
		local tile = seaRegion:getReachableTile(0)
		*/
		typeAll.seaConnectedRegion.set_function("getReachableTile", &seaConnectedRegion::getReachableTile);

		/***
		Get trade lane coords by index.
		@function seaConnectedRegion:getTradeLaneCoord
		@tparam int index
		@treturn coordPair coords
		@usage
		local coords = seaRegion:getTradeLaneCoord(0)
		*/
		typeAll.seaConnectedRegion.set_function("getTradeLaneCoord", &seaConnectedRegion::getTradeLanePath);
		
		/***
		Basic landingTile table.

		@tfield tileStruct tile
		@tfield float moveCost Move points required to reach this tile
		
		@table landingTile
		*/
		typeAll.landingTile = luaState.new_usertype<landingTile>("landingTile");
		typeAll.landingTile.set("tile", sol::property(&landingTile::getTile));
		typeAll.landingTile.set("moveCost", &landingTile::moveCost);

    }
};
