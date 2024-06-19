///
//![Lua logo](../Lua.png)
//@module LuaPlugin
//@author Fynn
//@license GPL-3.0
#include "strategyMap.h"

#include <queue>

#include "character.h"
#include "gameDataAllHelper.h"
#include "gameHelpers.h"
#include "plugData.h"
#include "smallFuncs.h"
#include "technicalHelpers.h"
#include "unit.h"
#include "faction.h"
#include "army.h"
#include "dataOffsets.h"
#include "settlement.h"
#include "fort.h"
#include "globals.h"


oneTile* landingPoint::getTile()
{
	const auto map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	return &map->tilesArr[tileIndex];
}
	
oneTileDouble* oneTile::tileToDoubleTile()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return nullptr;
	const int mapWidth = map->mapWidth * 2 + 1;
	const int x = getTileX() * 2;
	const int y = getTileY() * 2;
	if (x < 0 || y < 0 || x >= mapWidth || y >= mapWidth)
		return nullptr;
	return &map->climateTileArray[y * mapWidth + x];
}

void regionStruct::changeRegionName(const char* newName)
{
	UNICODE_STRING** nameMem = new UNICODE_STRING*;
	localizedRegionName = nameMem;
	smallFuncs::createUniString(localizedRegionName, newName);
	eopSettlementDataDb::getSettlementData(regionID).regionName = newName;
}

void regionStruct::changeRebelsName(const char* newName)
{
	UNICODE_STRING** nameMem = new UNICODE_STRING*;
	localizedRebelsName = nameMem;
	smallFuncs::createUniString(localizedRebelsName, newName);
	eopSettlementDataDb::getSettlementData(regionID).regionRebelsName = newName;
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
	
int oneTile::getTileX()
{
	const stratMap* map = stratMapHelpers::getStratMap();
	if (!map)
		return -1;
	const int index = this - map->tilesArr;
	return index % map->mapWidth;
}
	
armyStruct* oneTile::getArmy()
{
	if (const auto tileChar = getCharacter(); tileChar)
	{
		if (tileChar->armyLeaded)
			return tileChar->armyLeaded;
		if (tileChar->armyNotLeaded)
			return tileChar->armyNotLeaded;
	}
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
	return technicalHelpers::uniStringToStr(localizedRegionName);
}

std::string regionStruct::getLocalizedRebelsName()
{
	return technicalHelpers::uniStringToStr(localizedRebelsName);
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
		const auto gameData = gameDataAllHelper::get();
		if (!gameData)
			return nullptr;
		return gameData->stratMap;
	}

	settlementStruct* getSettlement(const stratMap* map, const std::string& name)
	{
		if (!plugData::data.luaAll.hashLoaded)
			plugData::data.luaAll.fillHashMaps();
		const auto regionId = plugData::data.luaAll.settlements.find(name);
		if (regionId == plugData::data.luaAll.settlements.end()) 
			return nullptr;
		return map->regions[regionId->second].settlement;
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
	
	std::queue<coordPair> getNeighbourTiles(int x, int y)
	{
		std::queue<coordPair> neighbours;
		neighbours.push({ x - 1, y });
		neighbours.push({ x + 1, y });
		neighbours.push({ x, y - 1 });
		neighbours.push({ x, y + 1 });
		neighbours.push({ x - 1, y + 1 });
		neighbours.push({ x - 1, y - 1 });
		neighbours.push({ x + 1, y + 1 });
		neighbours.push({ x + 1, y - 1 });
		return neighbours;
	}

	bool isTileValidForCharacterType(int charType, coordPair* coords)
	{
		if (!GAME_FUNC(bool(__stdcall*)(coordPair*, int, int), isTileValidForCharacter)(coords, charType, 1))
			return false;
		return isTileFree(&coords->xCoord);
	}

	coordPair* findValidTileNearTile(coordPair* coords, int charType)
	{
		if (isTileValidForCharacterType(charType, coords))
			return coords;
		const auto startCoords = *coords;
		std::queue<coordPair> neighbours = getNeighbourTiles(coords->xCoord, coords->yCoord);
		std::vector<coordPair> visited = { *coords };
		while (true)
		{
			if (neighbours.empty())
				break;
			coordPair checkCoord = neighbours.front();
			*coords = { checkCoord.xCoord, checkCoord.yCoord };
			neighbours.pop();
			visited.push_back(checkCoord);
			if (isTileValidForCharacterType(charType, coords))
				return coords;
			queue<coordPair> newNeighbours = getNeighbourTiles(checkCoord.xCoord, checkCoord.yCoord);
			while (!newNeighbours.empty())
			{
				auto newCoord = newNeighbours.front();
				newNeighbours.pop();
				bool isVisited = false;
				for (const auto& [xCoord, yCoord] : visited)
				{
					if (xCoord == newCoord.xCoord && yCoord == newCoord.yCoord)
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
				RallyPointSundry* ral = (RallyPointSundry*)baseObj;
				if (ral->object == nullptr)
				{
					break;
				}
				return getMainStratObject(ral->object);

				break;
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
            sol::usertype<seaConnectedRegion> seaConnectedRegion;
            sol::usertype<neighbourRegion> neighbourRegion;
        }typeAll;
    	
		/// coords
		//@section coordPair

		/***
		Basic coordPair table.

		@tfield int xCoord
		@tfield int yCoord

		@table coordPair
		*/
		typeAll.coordPair = luaState.new_usertype<coordPair>("coordPair");
		typeAll.coordPair.set("xCoord", &coordPair::xCoord);
		typeAll.coordPair.set("yCoord", &coordPair::yCoord);

		/// Strat Map
		//@section stratMap

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
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		*/
		typeAll.stratMap.set_function("getRegion", &getRegion);

		/***
		Get a specific tile by it's coordinates.
		@function stratMap.getTile
		@tparam int x
		@tparam int y
		@treturn tileStruct tile
		@usage
		local sMap = gameDataAll.get().stratMap;
		local tile = sMap.getTile(182, 243);
		*/
		typeAll.stratMap.set_function("getTile", &getTile);

		/***
		Get a volcano's coordinates.
		@function stratMap:getVolcanoCoords
		@tparam int index
		@treturn coordPair tile
		@usage
		local sMap = gameDataAll.get().stratMap;
		local volcano = sMap:getVolcanoCoords(0);
		*/
		typeAll.stratMap.set_function("getVolcanoCoords", &stratMap::getVolcanoCoords);

		/***
		Get a land connection's coordinates (the green arrows on the map that allow you to cross bodies of water).
		@function stratMap:getLandConnection
		@tparam int index
		@treturn coordPair tile
		@usage
		local sMap = gameDataAll.get().stratMap;
		local connection = sMap:getLandConnection(0);
		*/
		typeAll.stratMap.set_function("getLandConnection", &stratMap::getLandConnection);

		/***
		Get a landmass (collection of regions reachable by land, like a continent or island).
		@function stratMap:getLandMass
		@tparam int index
		@treturn landMass landMass
		@usage
		local sMap = gameDataAll.get().stratMap;
		local tile = sMap:getLandMass(0);
		*/
		typeAll.stratMap.set_function("getLandMass", &stratMap::getLandMass);

		/***
		Get a settlement by its internal name. This is the intended way to get a settlement quickly by it's name.
		@function stratMap:getSettlement
		@tparam string name
		@treturn settlementStruct settlement
		@usage
		local STRAT_MAP = gameDataAll.get().stratMap;
		local london = STRAT_MAP:getSettlement("London");
		*/
		typeAll.stratMap.set_function("getSettlement", &getSettlement);

		/***
		Get a region by its internal name.
		@function stratMap:getRegionByName
		@tparam string name
		@treturn regionStruct region
		@usage
		local STRAT_MAP = gameDataAll.get().stratMap;
		local londonRegion = STRAT_MAP:getRegionByName("London_Province");
		*/
		typeAll.stratMap.set_function("getRegionByName", &getRegionByName);

		/// landMass
		//@section landMass

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

		/// roadStruct
		//@section roadStruct

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
		//@section tileStruct

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

		///RegionStruct
		//@section RegionStruct

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
		@tfield int stacksNum
		@tfield int fortsNum
		@tfield int colorRed
		@tfield int colorGreen
		@tfield int colorBlue
		@tfield int watchtowersNum
		@tfield bool isSea
		@tfield bool peninsular
		@tfield landMass landMass
		@tfield roadStruct roadToPort
		@tfield int seaConnectedRegionsCount
		@tfield int loyaltyFactionID
		@tfield int seaImportRegionsCount
		@tfield int tilesBorderingEdgeOfMapCount
		@tfield int devastatedTilesCount
		@tfield settlementStruct settlement
		@tfield int tileCount
		@tfield int fertileTilesCount
		@tfield int neighbourRegionsNum
		@tfield int resourcesNum
		@tfield int resourceTypesBitMap
		@tfield int settlementXCoord
		@tfield int settlementYCoord
		@tfield int portEntranceXCoord
		@tfield int portEntranceYCoord
		@tfield factionStruct faction
		@tfield mercPool mercPool
		@tfield string rebelType
		@tfield string localizedRebelsName
		@tfield int triumphValue
		@tfield int totalTradeValue
		@tfield int patrolPointsCount
		@tfield getStack getStack
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
		typeAll.regionStruct.set("fortsNum", &regionStruct::fortsNum);
		typeAll.regionStruct.set("watchtowersNum", &regionStruct::watchtowersNum);
		typeAll.regionStruct.set("isSea", &regionStruct::isSea);
		typeAll.regionStruct.set("peninsular", &regionStruct::peninsular);
		typeAll.regionStruct.set("mercPool", &regionStruct::mercPool);
		typeAll.regionStruct.set("settlement", &regionStruct::settlement);
		typeAll.regionStruct.set("tileCount", &regionStruct::tileCount);
		typeAll.regionStruct.set("neighbourRegionsNum", &regionStruct::neighbourRegionsNum);
		typeAll.regionStruct.set("resourcesNum", &regionStruct::resourcesNum);
		typeAll.regionStruct.set("settlementXCoord", &regionStruct::settlementXCoord);
		typeAll.regionStruct.set("settlementYCoord", &regionStruct::settlementYCoord);
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
		@function regionStruct:getStack
		@tparam int index
		@treturn armyStruct army
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local army = region:getStack(0)
		*/
		typeAll.regionStruct.set_function("getStack", &regionStruct::getArmy);

		/***
		Get a fort by it's index.
		@function regionStruct:getFort
		@tparam int index
		@treturn fortStruct fort
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local fort = region:getFort(0)
		*/
		typeAll.regionStruct.set_function("getFort", &regionStruct::getFort);

		/***
		Get a watchtower by it's index.
		@function regionStruct:getWatchtower
		@tparam int index
		@treturn watchtowerStruct watchtower
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local watch = region:getWatchtower(0)
		*/
		typeAll.regionStruct.set_function("getWatchtower", &regionStruct::getWatchTower);

		/***
		Get a resource by it's index.
		@function regionStruct:getResource
		@tparam int index
		@treturn tradeResource resource
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local res = region:getResource(0)
		*/
		typeAll.regionStruct.set_function("getResource", &regionStruct::getResource);

		/***
		Get a neighbour region by it's index.
		@function regionStruct:getNeighbour
		@tparam int index
		@treturn neighbourRegion nRegion
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local nRegion = region:getNeighbour(0)
		*/
		typeAll.regionStruct.set_function("getNeighbour", &regionStruct::getNeighbourRegion);

		/***
		Check if a region has a hidden resource.
		@function regionStruct:getHiddenResource
		@tparam string name
		@treturn bool hr
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		localhr = region:getHiddenResource("resource_name")
		*/
		typeAll.regionStruct.set_function("getHiddenResource", &regionStruct::hasHiddenResource);

		/***
		Set a region's hidden resource (reset on game restart).
		@function regionStruct:setHiddenResource
		@tparam string name
		@tparam bool enable
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		region:setHiddenResource("resource_name", false)
		*/
		typeAll.regionStruct.set_function("setHiddenResource", &regionStruct::setHiddenResource);

		/***
		Get a region that is reachable from this region.
		@function regionStruct:getSeaConnectedRegion
		@tparam int index
		@treturn seaConnectedRegion connectedRegion
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local connectedRegion = region:getSeaConnectedRegion(0)
		*/
		typeAll.regionStruct.set_function("getSeaConnectedRegion", &regionStruct::getSeaConnectedRegion);

		/***
		Get a region this region is importing trade goods from.
		@function regionStruct:getSeaImportRegion
		@tparam int index
		@treturn seaConnectedRegion seaImportRegion
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local seaImportRegion = region:getSeaImportRegion(0)
		*/
		typeAll.regionStruct.set_function("getSeaImportRegion", &regionStruct::getSeaImportRegion);

		/***
		Get a region sea edge (point where it borders both sea and another land region).
		@function regionStruct:getRegionSeaEdge
		@tparam int index
		@treturn tileStruct edge
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local edge = region:getRegionSeaEdge(0)
		*/
		typeAll.regionStruct.set_function("getRegionSeaEdge", &regionStruct::getPatrolPoint);

		/***
		Get a devastated tile.
		@function regionStruct:getDevastatedTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local tile = region:getDevastatedTile(0)
		*/
		typeAll.regionStruct.set_function("getDevastatedTile", &regionStruct::getDevastatedTile);

		/***
		Get a tile that borders the edge of the map.
		@function regionStruct:getTileBorderingEdgeOfMap
		@tparam int index
		@treturn tileStruct edge
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local edge = region:getTileBorderingEdgeOfMap(0)
		*/
		typeAll.regionStruct.set_function("getTileBorderingEdgeOfMap", &regionStruct::getTileBorderingEdgeOfMap);

		/***
		Get a tile by index.
		@function regionStruct:getTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local tile = region:getTile(0)
		*/
		typeAll.regionStruct.set_function("getTile", &regionStruct::getTile);

		/***
		Get a fertile tile by index.
		@function regionStruct:getFertileTile
		@tparam int index
		@treturn tileStruct tile
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
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
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local catholicThreeTurnsAgo = region:getReligionHistory(0, 3)
		*/
		typeAll.regionStruct.set_function("getReligionHistory", &regionStruct::getReligionHistory);

		/***
		Check if region has a resource type.
		@function regionStruct:hasResourceType
		@tparam int resourceID
		@treturn bool hasResource
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local hasResource = region:hasResourceType(16)
		*/
		typeAll.regionStruct.set_function("hasResourceType", &regionStruct::hasResourceType);

		/***
		Get the strength total of all armies in this region that are hostile to a specific faction.
		@function regionStruct:getHostileArmiesStrength
		@tparam int factionID
		@treturn int totalStrength
		@usage
		local sMap = gameDataAll.get().stratMap;
		local region = sMap.getRegion(2);
		local totalStrength = region:getHostileArmiesStrength(myFac.factionID)
		*/
		typeAll.regionStruct.set_function("getHostileArmiesStrength", &regionStruct::getHostileArmiesStrength);

		///neighbourRegion
		//@section neighbourRegion

		/***
		Basic neighbourRegion table.

		@tfield int regionID
		@tfield regionStruct region
		@tfield int tradeValue
		@tfield bool isBlocked
		@tfield float moveCost
		@tfield int borderTilesCount
		@tfield roadStruct connectingRoad
		@tfield getBorderTile getBorderTile


		@table neighbourRegion
		*/
		typeAll.neighbourRegion = luaState.new_usertype<neighbourRegion>("neighbourRegion");
		typeAll.neighbourRegion.set("regionID", &neighbourRegion::regionID);
		typeAll.neighbourRegion.set("tradeValue", &neighbourRegion::tradeValue);
		typeAll.neighbourRegion.set("region", &neighbourRegion::region);
		typeAll.neighbourRegion.set("borderTilesCount", &neighbourRegion::borderTilesCount);
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



		///seaConnectedRegion
		//@section seaConnectedRegion

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
		@treturn tileStruct tile
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

    }
};

