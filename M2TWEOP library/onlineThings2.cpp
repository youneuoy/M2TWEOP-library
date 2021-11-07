#include "onlineThings2.h"

#include "imgui_notify.h"
#include "fastFunctsHelpers.h"
namespace battleCreator
{
	struct unitDataS
	{
		size_t numberInArmy = 0;
		std::string type;
		int soldiersNumber = 0;
		int exp = 0;
		bool isCharacterDied = false;
	};
	void from_json(const jsn::json& j, unitDataS& unit)
	{
		j.at("number in army").get_to(unit.numberInArmy);
		j.at("type").get_to(unit.type);
		j.at("soldiers").get_to(unit.soldiersNumber);
		j.at("exp").get_to(unit.exp);
		j.at("isCharacterDied").get_to(unit.isCharacterDied);
	}
	struct armyDataS
	{
		std::vector<unit*>units;

		std::vector<shared_ptr<unitDataS>>unitsForTransfer;
	};

	void to_json(jsn::json& j, const armyDataS& army)
	{
		j = jsn::json::array();

		for (size_t i = 0; i < army.units.size(); i++)
		{
			unit* unit = army.units[i];

			jsn::json unitJson;
			unitJson["number in army"] = i;
			unitJson["type"] = unit->eduEntry->Type;
			if (unit->general != nullptr)
			{
				if (unit->general->ifMarkedToKill == true)
				{
					unitJson["isCharacterDied"] =true;
				}
				else
				{
					unitJson["isCharacterDied"] = false;
				}
			}
			else
			{
				unitJson["isCharacterDied"] = false;
			}
			unitJson["soldiers"] = unit->number;
			unitJson["exp"] = unit->expScreen;

			j.push_back(unitJson);
		}
	}
	void from_json(const jsn::json& j, armyDataS& army)
	{
		for (auto it = j.begin(); it != j.end(); ++it)
		{
			shared_ptr<unitDataS> unitPtr(new unitDataS);

			unitDataS newUnit = it.value();
			*unitPtr = newUnit;

			army.unitsForTransfer.push_back(std::move(unitPtr));
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

	void from_json(const jsn::json& j, sideDataS& side)
	{
		for (auto it = j.begin(); it != j.end(); ++it)
		{
			shared_ptr<armyDataS> armyPtr(new armyDataS);

			armyDataS newArmy = it.value();
			*armyPtr = newArmy;

			side.armies.push_back(std::move(armyPtr));
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
	void from_json(const jsn::json& j, battleArmiesCreatorS& battle)
	{
		for (auto it = j.begin(); it != j.end(); ++it)
		{
			shared_ptr<sideDataS> sidePtr(new sideDataS);

			sideDataS newSide = it.value();
			*sidePtr = newSide;

			battle.sides.push_back(std::move(sidePtr));
		}
	}


	void addArmyToSide(shared_ptr<sideDataS> side, trackedPointerArmy army)
	{
		shared_ptr<armyDataS> armyPtr(new armyDataS);
		for (int i = 0; i < army.stack->numOfUnits; i++)
		{
			armyPtr->units.push_back(army.stack->units[i]);
		}

		side->armies.push_back(std::move(armyPtr));
	}

	void addSide(battleSide& side)
	{
		shared_ptr<sideDataS> sidePtr(new sideDataS);

		for (int i = 0; i < side.armiesNum; i++)
		{
			addArmyToSide(sidePtr, side.armies[i]);
		}

		battleArmies.sides.push_back(std::move(sidePtr));
	}
	void initStructsForResults()
	{
		battleDataS* battle = smallFuncs::getGameDataAll()->battleHandler;
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
		ImGuiToast* readyMsg = new ImGuiToast(ImGuiToastType_Info, 25000);
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

	void setWinner(int selectedWinner)
	{
		if (selectedWinner == 2)//draw
		{
			return;
		}

		else if (selectedWinner == 0)
		{
			fastFuncts::autoWin("attacker");
		}
		else if (selectedWinner == 1)
		{
			fastFuncts::autoWin("defender");
		}
	}


	bool doTransfer()
	{
		battleDataS* battle = smallFuncs::getGameDataAll()->battleHandler;

		std::vector<std::pair<shared_ptr<unitDataS>, unit*>>transferPairs;
		if (battleArmies.sides.size() != battle->sidesNum)
		{
			MessageBoxA(NULL, "The number of sides in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
			return false;
		}

		for (size_t sideI = 0; sideI < battleArmies.sides.size(); sideI++)
		{
			auto& side = battleArmies.sides[sideI];
			auto& gameSide = battle->sides[sideI];
			if (side->armies.size() != gameSide.armiesNum)
			{
				MessageBoxA(NULL, "The number of armies in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);

				return false;
			}
			for (size_t armyI = 0; armyI < side->armies.size(); armyI++)
			{
				auto& army = side->armies[armyI];
				auto& gameArmy = gameSide.armies[armyI];
				if (army->unitsForTransfer.size() != gameArmy.stack->numOfUnits)
				{
					MessageBoxA(NULL, "The number of units in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);

					return false;
				}

				for (int unitI = 0; unitI < army->unitsForTransfer.size(); unitI++)
				{
					auto& unit = army->unitsForTransfer[unitI];
					auto& gameUnit = gameArmy.stack->units[unitI];
					if (strcmp(unit->type.c_str(), gameUnit->eduEntry->Type) != 0)
					{
						MessageBoxA(NULL, "The unit types in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);

						return false;
					}

					transferPairs.push_back({ unit ,gameUnit });
				}
			}
		}

		for (auto& unitPair : transferPairs)
		{
			auto& unit = unitPair.first;
			auto& gameUnit = unitPair.second;
			if (gameUnit->general!=nullptr&& unit->isCharacterDied)
			{
				fastFuncts::setSoldiersCountAndExp(gameUnit, 0, unit->exp);
			}				
			else
			{
				fastFuncts::setSoldiersCountAndExp(gameUnit, unit->soldiersNumber, unit->exp);
			}
		}
		transferPairs.clear();

		return true;
	}

	bool transferResults(const std::string& resultsPath, int selectedWinner)
	{
		jsn::json json2;
		try
		{
			std::ifstream f2(resultsPath);


			f2 >> json2;

			f2.close();
		}
		catch (jsn::json::parse_error& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
			return false;
		}


		try
		{
			battleArmies = json2;
		}
		catch (jsn::json::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
			return false;
		}


		if (selectedWinner != 2)//draw
		{
			setWinner(selectedWinner);
			bool res = fastFuncts::useButton("prebattle_auto_resolve_button");
			if (res == false)
			{
				MessageBoxA(NULL, "Something goes wrong!", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				return false;
			}
		}
		bool transferResult = doTransfer();
		if (transferResult == false)
		{
			MessageBoxA(NULL, "Something goes wrong!", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
			return false;
		}
		return transferResult;
	}
};