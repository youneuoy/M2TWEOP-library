#include "pch.h"
#include "stratModelsChange.h"
#include "fort.h"
#include "settlement.h"
#include "functionsOffsets.h"
#include "strategyMap.h"
#include "campaign.h"
#include "cultures.h"

namespace stratModelsChange
{

	cultureCasEntry* newCasEntry(const cultureCasEntry* old)
	{
		const auto newMod = techFuncs::createGameClass<cultureCasEntry>();
		GAME_FUNC(void(__thiscall*)(cultureCasEntry*), createCultureCasEntry)(newMod);
		if (old)
			*newMod = *old;
		return newMod;
	}
	

	bool isVanillaCultureWatchtower(const DWORD addr)
	{
		const auto cultureDb = cultures::getCultureDb();
		const auto cultureCount = cultureDb->culturesCount;
		for (int i = 0; i < cultureCount; i++)
		{
			const auto culture = &cultureDb->cultures[i];
			if (reinterpret_cast<DWORD>(&culture->watchTower) == addr)
				return true;
		}
		return false;
	}
	
	void changeWatchTowerStratModel(watchTowerStruct* tower, model_Rigid* modelP)
	{
		if (tower->model->model == modelP)
			return;

		if (isVanillaCultureWatchtower(reinterpret_cast<DWORD>(tower->model)))
			tower->model = newCasEntry(tower->model);
		tower->model->model = modelP;
	}
	void changeResourceStratModel(resourceStruct* resource, model_Rigid* modelP)
	{
		if (resource->stratMod->model == modelP)
			return;
		stratResMod* newMod = new stratResMod();
		memcpy(newMod, resource->stratMod, sizeof(stratResMod));
		newMod->resource_cost = resource->stratMod->resource_cost;
		newMod->hasMine = resource->stratMod->hasMine;
		newMod->resource_num = resource->stratMod->resource_num;
		newMod->model = modelP;
		resource->stratMod = newMod;
	}
	void changeSettlementStratModel(settlementStruct* settlement, model_Rigid* modelP)
	{
		if (settlement->model->castleModel == modelP
			&& settlement->model->cityModel == modelP
			)return;

		cultureSettlement* newMod = settlement->model;
		if (settlement->model && settlement->model->index != 9999)
		{
			const auto oldMod = cultures::getCultureSettlement(settlement->level, settlement->fac_creatorModNum);
			newMod = techFuncs::createGameClass<cultureSettlement>();
			GAME_FUNC(void(__thiscall*)(cultureSettlement*), createCultureSettlement)(newMod);
			*newMod = *oldMod;
			newMod->index = 9999;
		}
		newMod->cityModel = modelP;
		newMod->castleModel = modelP;
		for (int i = 0; i < 31; i++)
		{
			newMod->settlementModels[i] = modelP;
			newMod->castleModels[i] = modelP;
		}
		settlement->model = newMod;
	}

	bool isVanillaCultureFort(const DWORD addr)
	{
		const auto cultureDb = cultures::getCultureDb();
		const auto cultureCount = cultureDb->culturesCount;
		for (int i = 0; i < cultureCount; i++)
		{
			const auto culture = &cultureDb->cultures[i];
			if (reinterpret_cast<DWORD>(&culture->fort) == addr)
				return true;
		}
		return false;
	}
	
	void changeFortStratModel(fortStruct* fort, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (fort->stratModel->model == modelP
			&& fort->stratModel->wallsModel == modelP2)
			return;
		
		cultureFort* newMod = fort->stratModel;
		
		if (const DWORD addr = reinterpret_cast<DWORD>(fort->stratModel);
			fort->stratModel && isVanillaCultureFort(addr))
		{
			const auto oldMod = fort->stratModel;
			newMod = techFuncs::createGameClass<cultureFort>();
			GAME_FUNC(void(__thiscall*)(cultureFort*), createCultureFort)(newMod);
			if (oldMod)
				*newMod = *oldMod;
		}
		newMod->model = modelP;
		newMod->wallsModel = modelP2;
		for (int i = 0; i < 31; i++)
		{
			newMod->models[i] = modelP;
			newMod->wallModels[i] = modelP2;
		}
		fort->stratModel = newMod;
	}
	
	bool isVanillaCulturePort(const DWORD addr)
	{
		const auto cultureDb = cultures::getCultureDb();
		const auto cultureCount = cultureDb->culturesCount;
		for (int i = 0; i < cultureCount; i++)
		{
			const auto culture = &cultureDb->cultures[i];
			for (auto& portBuilding : culture->portBuildings)
			{
				if (reinterpret_cast<DWORD>(&portBuilding) == addr)
					return true;
			}
		}
		return false;
	}
	
	bool isVanillaCulturePortDock(const DWORD addr)
	{
		const auto cultureDb = cultures::getCultureDb();
		const auto cultureCount = cultureDb->culturesCount;
		for (int i = 0; i < cultureCount; i++)
		{
			const auto culture = &cultureDb->cultures[i];
			for (auto& portBuilding : culture->portWalls)
			{
				if (reinterpret_cast<DWORD>(&portBuilding) == addr)
					return true;
			}
		}
		return false;
	}
	
	void changePortStratModel(portBuildingStruct* port, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (!port->portDock && port->portStratModel->model == modelP)
			return;
		if (port->portDock && port->portDock->dockStratModel->model == modelP2 && port->portStratModel->model == modelP)
			return;
		if (isVanillaCulturePort(reinterpret_cast<DWORD>(port->portStratModel)))
			port->portStratModel = newCasEntry(port->portStratModel);
		port->portStratModel->model = modelP;
		
		if (port->portDock->dockStratModel->model == modelP2)
			return;
		if (isVanillaCulturePortDock(reinterpret_cast<DWORD>(port->portDock->dockStratModel)))
			port->portDock->dockStratModel = newCasEntry(port->portDock->dockStratModel);
		port->portDock->dockStratModel->model = modelP2;
	}
	
	bool changeModel(const int x, const int y, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (!modelP)
			return false;
		const auto tile = stratMapHelpers::getTile(x, y);
		if (modelP2)
		{
			if (fortStruct* fort = tile->getFort(); fort)
			{
				changeFortStratModel(fort, modelP, modelP2);
				return true;
			}
			if (portBuildingStruct* port = tile->getPort(); port)
			{

				changePortStratModel(port, modelP, modelP2);
				return true;
			}
		}
		if (resourceStruct* resource = tile->getResource(); resource)
		{
			changeResourceStratModel(resource, modelP);
			return true;
		}
		if (settlementStruct* settlement = tile->getSettlement(); settlement)
		{
			changeSettlementStratModel(settlement, modelP);
			return true;
		}
		if (watchTowerStruct* tower = tile->getWatchtower(); tower)
		{
			changeWatchTowerStratModel(tower, modelP);
			return true;
		}
		return false;
	}

}
