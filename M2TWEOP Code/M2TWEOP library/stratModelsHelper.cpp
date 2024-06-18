#include "stratModelsChange.h"
#include "fort.h"
#include "settlement.h"
#include "functionsOffsets.h"
#include "strategyMap.h"
#include "campaign.h"

namespace stratModelsChange
{
	void changeWatchTowerStratModel(watchTowerStruct* tower, model_Rigid* modelP)
	{
		if (tower->model->modelP == modelP)return;

		watchTowerModel* newMod = new watchTowerModel();

		newMod->modelP = modelP;

		tower->model = newMod;
	}
	void changeResourceStratModel(resourceStruct* resource, model_Rigid* modelP)
	{
		if (resource->stratMod->model == modelP)return;
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
		if (settlement->model->castle == modelP
			&& settlement->model->town == modelP
			)return;

		settMod* newMod = new settMod();
		memset(newMod, 0, sizeof(settMod));
		newMod->castle = modelP;
		newMod->town = modelP;
		settlement->model = newMod;

	}
	void changeFortStratModel(fortStruct* fort, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (fort->stratModel->centerModel == modelP
			&& fort->stratModel->wallsModel == modelP2
			)return;


		stratFortMod* newMod = new stratFortMod[33];
		memset(newMod, 0, sizeof(stratFortMod) * 33);


		newMod->centerModel = modelP;

		/*newMod->centerModelPath = model->stratFortModel->centerModelPath;
		newMod->centerModelPathCrypt = model->stratFortModel->centerModelPathCrypt;*/

		newMod->wallsModel = modelP2;

		/*newMod->wallsModelPath = model->stratFortModel->wallsModelPath;
		newMod->wallsModelPathCrypt = model->stratFortModel->wallsModelPathCrypt;*/
		fort->stratModel = newMod;

	}

	void changePortStratModel(portBuildingStruct* port, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (port->portDock == nullptr)
		{
			if (port->portStratModel->model_rigid == modelP)
			{
				return;
			}
		}
		else
		{
			if (port->portStratModel->model_rigid == modelP
				&& port->portDock->dockStratModel->model_rigid == modelP2
				)return;
		}


		stratPortModel* newMod = new stratPortModel();
		memset(newMod, 0, sizeof(stratPortModel));


		newMod->model_rigid = modelP;


		port->portStratModel = newMod;


		if (port->portDock == nullptr)return;
		stratPortModel* newDockMod = new stratPortModel();
		memset(newDockMod, 0, sizeof(stratPortModel));

		newDockMod->model_rigid = modelP2;


		port->portDock->dockStratModel = newDockMod;

	}
	bool changeModel(int x, int y, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (modelP == nullptr)
			return false;
		const auto tile = stratMapHelpers::getTile(x, y);
		if (modelP2 != nullptr)
		{
			fortStruct* fort = tile->getFort();
			if (fort != nullptr)
			{
				changeFortStratModel(fort, modelP, modelP2);
				return true;
			}
			portBuildingStruct* port = tile->getPort();
			if (port != nullptr)
			{

				changePortStratModel(port, modelP, modelP2);
				return true;
			}
		}
		resourceStruct* resource = tile->getResource();
		if (resource != nullptr)
		{
			changeResourceStratModel(resource, modelP);
			return true;
		}
		settlementStruct* settlement = tile->getSettlement();
		if (settlement != nullptr)
		{
			changeSettlementStratModel(settlement, modelP);
			return true;
		}
		watchTowerStruct* tower = tile->getWatchtower();
		if (tower != nullptr)
		{
			changeWatchTowerStratModel(tower, modelP);
			return true;
		}

		return false;
	}

}
