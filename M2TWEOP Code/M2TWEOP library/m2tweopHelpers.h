#pragma once
#include <string>

#include "lua/sol.hpp"

#include "realGameTypes.h"

namespace m2tweopHelpers
{


	std::string getModPath();
	std::string getLuaPath();
	std::string getPathFromMods();
	std::string getModString(const std::string& path);
	std::shared_ptr<mapImage> makeMapImage();
	void clearMapImage(mapImage* img);
	void loadSaveGame(const char* saveName);
	
	bool isTileFree(int x, int y);
	std::tuple<int, int> getGameTileCoordsWithCursor();
	bool getTileVisibility(factionStruct* faction, int x, int y);
	factionStruct* getRegionOwner(int regionID);
	void fireGameScriptFunc(void* scriptStruct, DWORD offset);
	void setPerfectSpy(bool set);
	int getLocalFactionID();
	
	culturesDB* getCultureDb();
	void setEquipmentCosts(int equipType, int cost);
	void fillRegionColor(mapImage* img, int id, int r, int g, int b, int a);
	void addRegionColor(mapImage* img, int id, int r, int g, int b, int a);
	void fillTileColor(mapImage* img, int x, int y, int r, int g, int b, int a);
	void addTileColor(mapImage* img, int x, int y, int r, int g, int b, int a);
	void logStringGame(const std::string& msg);
	void logFuncError(const std::string& funcName, const std::string& error);
	options1* getOptions1();
	options2* getOptions2();
	campaignDifficulty1* getCampaignDifficulty1();
	campaignDifficulty2* getCampaignDifficulty2();
	void setFactionTrade(factionStruct* factionOne, factionStruct* factionTwo);
	void setFactionProtectorate(factionStruct* factionOne, factionStruct* factionTwo);

	std::tuple<int, int, void*>  loadTextureToGame(const std::string& path);
	std::tuple<int, int, void*>  loadMapTexture(mapImage* mapImage, const std::string& path);
	void unloadTextureFromGame(void* texture);

	settlementTextStrings* getUIStrings(const settlementInfoScroll* settlementInfoScroll);
	settlementInfoScroll* getSettlementInfoScroll();
};

