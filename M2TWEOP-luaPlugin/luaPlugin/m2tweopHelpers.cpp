#include "m2tweopHelpers.h"
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
		int xy[2]{x,y};
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
		return (*(*plugData::data.funcs.getTileRegionID))(x,y);
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
			return (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::warState);
		}
		else if (dipType == dipRelEnum::peace)
		{
			return (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::peaceState);
		}
		else if (dipType == dipRelEnum::alliance)
		{
			return (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::allianceState);
		}
		else if (dipType == dipRelEnum::suzerain)
		{
			return (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state == dipStateInternalEnum::allianceState
				&& campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].protectorate == protectorateState
				);
		}
		else if (dipType == dipRelEnum::trade)
		{
			return (campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].trade == 1);
		}
		return false;
	}
	static void disableVassalage(campaign* campaignStruct, factionStruct* fac1, factionStruct* fac2)
	{
		using namespace campaignEnums;

		campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::peaceState;
		campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::peaceState;
		if (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].protectorate == protectorateState)
		{
			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].protectorate = nonProtectorateeState;
		}

		if (campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].protectorate == protectorateState)
		{
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].protectorate = nonProtectorateeState;
		}

	}
	void setDipStance(campaign* campaignStruct, campaignEnums::dipRelEnum dipType, factionStruct* fac1, factionStruct* fac2)
	{
		using namespace campaignEnums;
		if (dipType == dipRelEnum::war)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].trade = 0;
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].trade = 0;

			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::warState;
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::warState;
		}
		else if (dipType == dipRelEnum::peace)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::peaceState;
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::peaceState;
		}
		else if (dipType == dipRelEnum::alliance)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::allianceState;
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::allianceState;
		}
		else if (dipType == dipRelEnum::suzerain)
		{
			disableVassalage(campaignStruct, fac1, fac2);

			if (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].protectorate == protectorateState)
			{
				campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].protectorate = nonProtectorateeState;
			}
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].protectorate = protectorateState;

			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state = dipStateInternalEnum::allianceState;
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].state = dipStateInternalEnum::allianceState;

			campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].trade = 1;
			campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].trade = 1;
		}
		else if (dipType == dipRelEnum::trade)
		{
			if (campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].state != dipStateInternalEnum::warState)
			{
				campaignStruct->dipArray[fac1->dipNum][fac2->dipNum].trade = 1;
				campaignStruct->dipArray[fac2->dipNum][fac1->dipNum].trade = 1;
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
		(*(*plugData::data.funcs.setEDUUnitsSize))(min,max);
	}
	std::tuple<int,int,void*> loadTextureToGame(const std::string& path)
	{
		int x = 0;
		int y = 0;
		LPDIRECT3DTEXTURE9 retTexture = (*(*plugData::data.funcs.loadTexture))(path.c_str(), &x, &y);

		sol::as_table_t int2 = sol::as_table(std::vector<int>{
			x, y
		});

		return std::make_tuple(x,y, reinterpret_cast<void*>(retTexture));
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
	void changeBattlemapCameraDistance(signed short maxZoom)
	{
		(*(*plugData::data.funcsBattle.changeBattlemapCameraDistance))(maxZoom);
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

}