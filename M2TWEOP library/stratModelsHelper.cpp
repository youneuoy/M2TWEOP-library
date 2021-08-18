#include "stratModelsChange.h"
namespace stratModelsChange
{
	void changeWatchTowerStratModel(watchTowerStruct* tower, model_Rigid* modelP)
	{
		if (tower->model->modelP == modelP)return;

		watchTowerModel* newMod = new watchTowerModel();

		newMod->modelP = modelP;

		tower->model = newMod;
	}
	void changeResourceStratModel(resStrat* resource, model_Rigid* modelP)
	{
		if (resource->stratMod->model == modelP)return;
		stratResMod* newMod = new stratResMod();

		newMod->resource_cost = 0;
		newMod->hasMine = 0;
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

		newMod->castle = modelP;
		newMod->town = modelP;
		settlement->model = newMod;

	}
	void changeFortStratModel(fortStruct* fort, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (fort->stratModel->centerModel == modelP
			&& fort->stratModel->wallsModel == modelP2
			)return;


		stratFortMod* newMod = new stratFortMod;
		memset(newMod, 0, sizeof(stratFortMod));


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
	void stratModelsChange::changeModel(int x, int y, model_Rigid* modelP, model_Rigid* modelP2)
	{
		if (modelP == nullptr)return;

		fortStruct* fort = nullptr;
		portBuildingStruct* port = nullptr;
		resStrat* resource = nullptr;
		settlementStruct* settlement = nullptr;
		watchTowerStruct* tower = nullptr;

		if (modelP2 != nullptr)
		{
			fort = fastFuncts::findFort(x, y);
			if (fort != nullptr)
			{
				changeFortStratModel(fort, modelP, modelP2);
				return;
			}


			port = fastFuncts::findPort(x, y);
			if (port != nullptr)
			{

				changePortStratModel(port, modelP, modelP2);
				return;
			}
		}

		resource = fastFuncts::findResource(x, y);
		if (resource != nullptr)
		{
			changeResourceStratModel(resource, modelP);
			return;
		}
		settlement= fastFuncts::findSettlement(x, y);
		if (settlement != nullptr)
		{
			changeSettlementStratModel(settlement, modelP);
			return;
		}
		tower = fastFuncts::findWatchTower(x, y);
		if (tower != nullptr)
		{
			changeWatchTowerStratModel(tower, modelP);
			return;
		}

		return;
	}

}