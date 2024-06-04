#pragma once
#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#include "lua/sol.hpp"

#include "realGameTypes.h"
namespace campaignEnums
{
	enum dipRelEnum :int
	{
		war = 1,
		peace = 2,
		alliance = 3,
		suzerain = 4,
		trade = 6
	};

	enum dipStateInternalEnum :int
	{
		peaceState = 200,
		warState = 600,
		allianceState = 0
	};
	constexpr int protectorateState = 15;
	constexpr int nonProtectorateeState = 6;
}
namespace m2tweopHelpers
{

	std::string getModPath();
	std::string getLuaPath();
	void toggleDeveloperMode();
	std::shared_ptr<mapImage> makeMapImage();
	void clearMapImage(mapImage* img);
	
	bool isTileFree(int x, int y);
	std::tuple<int, int> getGameTileCoordsWithCursor();
	int getTileRegionID(int x, int y);
	bool getTileVisibility(factionStruct* faction, int x, int y);
	factionStruct* getRegionOwner(int regionID);
	
	bool checkDipStance(const campaign* campaignStruct, campaignEnums::dipRelEnum dipType, const factionStruct* fac1, const factionStruct* fac2);
	void setDipStance(campaign* campaignStruct, campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2);
	int GetUnitSize();

	void setAncLimit(unsigned char limit);
	void seReligionsLimit(unsigned char limit);
	void setMaxBgSize(unsigned char maxBgSize);
	void unlockGameConsoleCommands();
	void setPerfectSpy(bool set);
	int getLocalFactionID();
	void setEquipmentCosts(int equipType, int cost);
	void fillRegionColor(mapImage* img, int id, int r, int g, int b, int a);
	void addRegionColor(mapImage* img, int id, int r, int g, int b, int a);
	void fillTileColor(mapImage* img, int x, int y, int r, int g, int b, int a);
	void addTileColor(mapImage* img, int x, int y, int r, int g, int b, int a);


	void setEDUUnitsSize(signed short min, signed short max);
	int getGameVersion();
	options1* getOptions1();
	options2* getOptions2();
	campaignDifficulty1* getCampaignDifficulty1();
	campaignDifficulty2* getCampaignDifficulty2();
	void setFactionTrade(factionStruct* factionOne, factionStruct* factionTwo);
	void setFactionProtectorate(factionStruct* factionOne, factionStruct* factionTwo);
	void loadSaveGame(const std::string& path);

	std::tuple<int, int, void*>  loadTextureToGame(const std::string& path);
	std::tuple<int, int, void*>  loadMapTexture(mapImage* mapImage, const std::string& path);
	void unloadTextureFromGame(void* texture);
	void logStringGame(const std::string& msg);

	void toggleUnitsBMapHighlight();
	battleCameraStruct* getBattleCamCoords();
	settlementTextStrings* getUIStrings(const settlementInfoScroll* settlementInfoScroll);
	settlementInfoScroll* getSettlementInfoScroll();

	void setConversionLvlFromCastle(int castleLvl, int convertToLvl);
	void setConversionLvlFromCity(int cityLvl, int convertToLvl);

	void setBuildingChainLimit(unsigned int limit);

	void setGuildCooldown(unsigned char turns);
};

