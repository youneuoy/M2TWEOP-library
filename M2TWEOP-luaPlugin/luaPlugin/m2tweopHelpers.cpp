#include "m2tweopHelpers.h"

#include "gameDataAllHelper.h"
#include "plugData.h"
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
		return (*(*plugData::data.funcs.isTileFree))(xy);
	}
	std::tuple<int, int> getGameTileCoordsWithCursor()
	{
		int x = 0;
		int y = 0;
		(*(*plugData::data.funcs.getGameTileCoordsWithCursor))(x, y);

		return std::make_tuple(x, y);
	}
	int getTileRegionID(int x, int y)
	{
		return (*(*plugData::data.funcs.getTileRegionID))(x, y);
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
		return (*(*plugData::data.funcs.getRegionOwner))(regionID);
	}
	bool checkDipStance(campaign* campaignStruct, campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2)
	{
		using namespace campaignEnums;
		if (dipType == dipRelEnum::war)
		{
			return (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::warState);
		}
		else if (dipType == dipRelEnum::peace)
		{
			return (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::peaceState);
		}
		else if (dipType == dipRelEnum::alliance)
		{
			return (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::allianceState);
		}
		else if (dipType == dipRelEnum::suzerain)
		{
			return (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::allianceState
				&& campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].protectorate == protectorateState
				);
		}
		else if (dipType == dipRelEnum::trade)
		{
			return (campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].trade == 1);
		}
		return false;
	}
	static void disableVassalage(campaign* campaignStruct, factionStruct* fac1, factionStruct* fac2)
	{
		using namespace campaignEnums;

		campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::peaceState;
		campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::peaceState;
		if (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].protectorate == protectorateState)
		{
			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].protectorate = nonProtectorateeState;
		}

		if (campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].protectorate == protectorateState)
		{
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].protectorate = nonProtectorateeState;
		}

	}
	void setDipStance(campaign* campaignStruct, campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2)
	{
		using namespace campaignEnums;
		if (dipType == dipRelEnum::war)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade = 0;
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].trade = 0;

			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::warState;
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::warState;
		}
		else if (dipType == dipRelEnum::peace)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::peaceState;
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::peaceState;
		}
		else if (dipType == dipRelEnum::alliance)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::allianceState;
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::allianceState;
		}
		else if (dipType == dipRelEnum::suzerain)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			if (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].protectorate == protectorateState)
			{
				campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].protectorate = nonProtectorateeState;
			}
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].protectorate = protectorateState;

			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::allianceState;
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::allianceState;

			campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade = 1;
			campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].trade = 1;
		}
		else if (dipType == dipRelEnum::trade)
		{
			if (campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].state != dipStateInternalEnum::warState)
			{
				campaignStruct->diplomaticStandings[fac1->dipNum][fac2->dipNum].trade = 1;
				campaignStruct->diplomaticStandings[fac2->dipNum][fac1->dipNum].trade = 1;
			}
		}

	}
	int GetUnitSize()
	{
		return (*(*plugData::data.funcs.GetUnitSize))();
	}
	void setAncLimit(unsigned char limit)
	{
		(*(*plugData::data.funcs.setAncLimit))(limit);
	}
	void toggleDeveloperMode()
	{
		(*(*plugData::data.funcs.toggleDeveloperMode))();
	}
	void seReligionsLimit(unsigned char limit)
	{
		(*(*plugData::data.funcs.setReligionsLimit))(limit);
	}
	void setMaxBgSize(unsigned char maxBgSize)
	{
		(*(*plugData::data.funcs.setMaxBgSize))(maxBgSize);
	}
	void unlockGameConsoleCommands()
	{
		(*(*plugData::data.funcs.unlockConsoleCommands))();
	}
	void setEDUUnitsSize(signed short min, signed short max)
	{
		(*(*plugData::data.funcs.setEDUUnitsSize))(min, max);
	}
	void setBattlemapSize(signed short x, signed short y)
	{
		(*(*plugData::data.funcs.setBattlemapSize))(x, y);
	}
	std::tuple<int, int, void*> loadTextureToGame(const std::string& path)
	{
		int x = 0;
		int y = 0;
		LPDIRECT3DTEXTURE9 retTexture = (*(*plugData::data.funcs.loadTexture))(path.c_str(), &x, &y);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			x, y
		});

		return std::make_tuple(x, y, reinterpret_cast<void*>(retTexture));
	}
	void unloadTextureFromGame(void* texture)
	{
		LPDIRECT3DTEXTURE9 tex = reinterpret_cast<LPDIRECT3DTEXTURE9>(texture);
		(*(*plugData::data.funcs.unloadTexture))(tex);

	}
	void toggleUnitsBMapHighlight()
	{
		(*(*plugData::data.funcsBattle.swUnBMapHighlight))();
	}
	battleCameraStruct* getBattleCamCoords()
	{
		battleCameraStruct* battleCamStruct =  (*(*plugData::data.funcsBattle.getBattleCamCoords))();
		return battleCamStruct;
	}
	void setConversionLvlFromCastle(int castleLvl, int convertToLvl)
	{
		(*(*plugData::data.funcs.setConversionLvlFromCastle))(castleLvl, convertToLvl);

	}
	void setConversionLvlFromCity(int cityLvl, int convertToLvl)
	{
		(*(*plugData::data.funcs.setConversionLvlFromCity))(cityLvl, convertToLvl);
	}

	void setBuildingChainLimit(unsigned int limit)
	{
		(*(*plugData::data.funcs.setBuildingChainLimit))(limit);
	}

	void setGuildCooldown(unsigned char turns)
	{
		(*(*plugData::data.funcs.setGuildCooldown))(turns);
	}

	int getGameVersion()
	{
		return (*(*plugData::data.funcs.getGameVersion))();
	}

	int getLocalFactionID()
	{
		gameDataAllStruct* gameData = gameDataAllHelper::get();
		return *gameData->localFactionID;
	}

	void setPerfectSpy(bool set)
	{
		if (getGameVersion() == 1)
		{
			*reinterpret_cast<bool*>(0x016F0E5C) = set;
		}
		else
		{
			*reinterpret_cast<bool*>(0x016A7CC4) = set;
		}
	}

	void setEquipmentCosts(int equipType, int cost)
	{
		struct equipmentCosts
		{
			int ram;
			int ladder;
			int siegeTower;
		};
		equipmentCosts* costs;
		if (getGameVersion() == 1)
		{
			costs = reinterpret_cast<equipmentCosts*>(0x01655BB0);
		}
		else
		{
			costs = reinterpret_cast<equipmentCosts*>(0x0160DCC8);
		}
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
		if (getGameVersion() == 1)
		{
			return reinterpret_cast<options1*>(0x02CB693C);
		}
		return reinterpret_cast<options1*>(0x02C6D804);
	}

	options2* getOptions2()
	{
		if (getGameVersion() == 1)
		{
			return reinterpret_cast<options2*>(0x016818A0);
		}
		return reinterpret_cast<options2*>(0x01639EF0);
	}

	campaignDifficulty1* getCampaignDifficulty1()
	{
		if (getGameVersion() == 1)
		{
			return reinterpret_cast<campaignDifficulty1*>(0x016F4760);
		}
		return reinterpret_cast<campaignDifficulty1*>(0x016AB5C8);
	}

	campaignDifficulty2* getCampaignDifficulty2()
	{
		if (getGameVersion() == 1)
		{
			return reinterpret_cast<campaignDifficulty2*>(0x0164BF18);
		}
		return reinterpret_cast<campaignDifficulty2*>(0x01604000);
	}

}