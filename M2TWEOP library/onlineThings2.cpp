#include "onlineThings2.h"

#include "imgui_notify.h"
namespace battleCreator
{

	struct armyDataS
	{
		std::vector<unit*>units;
	};

	void to_json(jsn::json& j, const armyDataS& army)
	{
		j = jsn::json::array();

		for (size_t i = 0; i < army.units.size();i++)
		{
			unit* unit = army.units[i];

			jsn::json unitJson;
			unitJson["number in army"] = i;
			unitJson["type"] = unit->eduEntry->Type;
			if (unit->general != nullptr)
			{
				if (unit->general->ifMarkedToKill == true)
				{
					unitJson["soldiers"] = 0;
				}
				else
				{
					unitJson["soldiers"] = unit->number;
				}
			}
			else
			{
				unitJson["soldiers"] = unit->number;
			}
			unitJson["exp"] = unit->expScreen;

			j.push_back(unitJson);
		}
	}
	struct sideDataS
	{
		std::vector<shared_ptr<armyDataS>>armies;
	};
	void to_json(jsn::json& j, const sideDataS& side)
	{
		j = jsn::json::array();

		for (shared_ptr<armyDataS> army : side.armies)
		{

			j.push_back(*army);
		}
	}
	struct battleArmiesCreatorS
	{
		std::vector<shared_ptr<sideDataS>>sides;

	}battleArmies;
	void to_json(jsn::json& j, const battleArmiesCreatorS& battle)
	{
		j = jsn::json::array();

		for (shared_ptr<sideDataS> side : battle.sides)
		{
			j.push_back(*side);
		}
	}

	void addArmyToSide(shared_ptr<sideDataS> side,trackedPointerArmy army)
	{
		shared_ptr<armyDataS> armyPtr(new armyDataS);
		for (int i = 0; i < army.stack->numOfUnits; i++)
		{
			armyPtr->units.push_back(army.stack->units[i]);
		}

		side->armies.push_back(std::move(armyPtr));
	}

	void addSide(battleSide&side)
	{
		shared_ptr<sideDataS> sidePtr(new sideDataS);

		for (int i = 0; i < side.armiesNum; i++)
		{
			addArmyToSide(sidePtr,side.armies[i]);
		}

		battleArmies.sides.push_back(std::move(sidePtr));
	}
	void initStructsForResults()
	{
		battleDataS*battle=smallFuncs::getGameDataAll()->battleHandler;
		for (int i = 0; i < battle->sidesNum; i++)
		{
			addSide(battle->sides[i]);
		}
	}


	void doResultsFileCreating()
	{
		std::string fPath = globals::dataS.modPatch;
		fPath += "\\eopBattles";
		filesystem::create_directory(fPath);
		fPath += "\\lastBattleResult";
		filesystem::remove_all(fPath);
		filesystem::create_directory(fPath);

		fPath += "\\m2tweopBattleResults.json";


		ofstream f1(fPath);
		jsn::json json = battleArmies;
		f1 << setw(4) << json;
		f1.close();

	}
	void createResultsFile()
	{
		ImGuiToast* readyMsg=new ImGuiToast(ImGuiToastType_Info, 25000);
		readyMsg->set_title("Battle results generation");
		readyMsg->set_content("Started generation of results");

		ImGui::InsertNotification(*readyMsg);
		delete readyMsg;

		doResultsFileCreating();


		ImGuiToast* doneMsg = new ImGuiToast(ImGuiToastType_Success, 25000);
		doneMsg->set_title("Battle results generation");
		doneMsg->set_content("Done generation of results");
		ImGui::InsertNotification(*doneMsg);
		delete doneMsg;
	}
};