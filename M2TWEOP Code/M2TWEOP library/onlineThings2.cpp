#include "onlineThings2.h"

#include "imgui_notify.h"
#include "fastFunctsHelpers.h"
namespace battleCreator
{
	struct genDataS
	{
		bool isCharacterDied = false;
		std::string genName;
		int age = 16;
		std::string faction;
		int subfaction=0;
		std::string portrait;
		std::string battle_model;
		std::string hero_ability;

		std::vector<std::pair<std::string, int>>traits;
		std::vector<std::string>ancillaries;
	};

	struct unitDataS
	{
		unitDataS() {};
		unitDataS(unit*un,int pnumberInArmy)
		{
			numberInArmy = pnumberInArmy;
			type = un->eduEntry->Type;
			soldiersNumber = un->SoldierCountStrat;
			exp = un->expScreen;


			isHaveCharacter = (un->general != nullptr);
		}
		size_t numberInArmy = 0;
		std::string type;
		int soldiersNumber = 0;
		int exp = 0;

		genDataS gen;
		bool isHaveCharacter = false;
	};

	genDataS getCharacter(const jsn::json& j)
	{
		genDataS retGen;

		j.at("isCharacterDied").get_to(retGen.isCharacterDied);
		j.at("name").get_to(retGen.genName);
		j.at("age").get_to(retGen.age);
		j.at("faction").get_to(retGen.faction);
		j.at("subfaction").get_to(retGen.subfaction);
		j.at("portrait").get_to(retGen.portrait);
		j.at("battle_model").get_to(retGen.battle_model);
		j.at("hero_ability").get_to(retGen.hero_ability);
		if (j.at("traits").is_number()==false)
		{
			j.at("traits").get_to(retGen.traits);
		}		
		
		if (j.at("ancillaries").is_number()==false)
		{
			j.at("ancillaries").get_to(retGen.ancillaries);
		}



		return retGen;
	}
	void from_json(const jsn::json& j, unitDataS& unit)
	{
		j.at("number in army").get_to(unit.numberInArmy);
		j.at("type").get_to(unit.type);
		j.at("soldiers").get_to(unit.soldiersNumber);
		j.at("exp").get_to(unit.exp);
		if (j.at("general").is_number())
		{
			unit.isHaveCharacter = false;
		}
		else
		{
			unit.isHaveCharacter = true;
			unit.gen = getCharacter(j.at("general"));
		}
	}
	struct armyDataS
	{
		std::vector<unit*>units;

		std::vector<shared_ptr<unitDataS>>unitsForTransfer;

		shared_ptr<unitDataS> getUnitWithID(int id)
		{
			shared_ptr<unitDataS> retUn = nullptr;
			for (auto& un : unitsForTransfer)
			{
				if (un->numberInArmy == id)
				{
					retUn = un;
					break;
				}
			}
			return retUn;
		}
	};


	jsn::json addCharacter(general* gen)
	{
		jsn::json genJson;

		if (gen->ifMarkedToKill == true)
		{
			genJson["isCharacterDied"] = true;
		}
		else
		{
			genJson["isCharacterDied"] = false;
		}

		std::string genName;
		if (gen->genChar->fullName == nullptr || string(gen->genChar->fullName).size() == 0)
		{
			genName="default";
		}
		else
		{
			genName=gen->genChar->fullName;
		}
		genJson["name"] = genName;
		
		int age = (gen->genChar->age >> 3) & 0x7f;
		genJson["index"] = gen->genChar->index;
		genJson["age"] = age;
		genJson["faction"] = gen->genChar->faction->factSmDescr->facName;
		genJson["subfaction"] = gen->genChar->subFaction;

		std::string portrait;
		if (gen->genChar->portrait_custom)
		{
			portrait = gen->genChar->portrait_custom;
		}
		else
		{
			portrait = "default";
		}
		genJson["portrait"] = portrait;

		std::string battle_model;
		if (gen->genChar->modelName)
		{
			battle_model = gen->genChar->modelName;
		}
		else
		{
			battle_model = "default";
		}
		genJson["battle_model"] = battle_model;

		std::string hero_ability;
		if (gen->ability)
		{
			hero_ability = gen->ability;
		}
		else
		{
			hero_ability = "default";
		}
		genJson["hero_ability"] = hero_ability;


		if (gen->genChar->traits != nullptr)
		{
			jsn::json jTraits=jsn::json::array();

			traitContainer* traitCont = gen->genChar->traits;
			while (traitCont != nullptr)
			{
				std::pair<std::string, int> traitData;
				traitData.first= traitCont->trait->nameS->name;
				traitData.second= *traitCont->trait->level;

				traitCont = traitCont->next;
				jTraits.push_back(traitData);
			};
			genJson["traits"] = jTraits;
		}
		else
		{
			genJson["traits"] = 0;
		}


		if (gen->genChar->ancNum != 0)
		{
			jsn::json jAncs= jsn::json::array();

			UINT32 anchNum = gen->genChar->ancNum;
			for (UINT32 i = 0; i < anchNum; i++)
			{
				jAncs.push_back(gen->genChar->ancillaries[i]->dataAnch->ancName);
			}

			genJson["ancillaries"] = jAncs;
		}
		else
		{
			genJson["ancillaries"] = 0;
		}

		return genJson;
	}
	void to_json(jsn::json& j, const armyDataS& army)
	{
		j = jsn::json::array();


		for (size_t i = 0; i < army.units.size(); i++)
		{
			unit* unit = army.units[i];

			jsn::json unitJson;

			unitJson["number in army"] = i;
			unitJson["index"] = unit->ID;
			unitJson["type"] = unit->eduEntry->Type;
			if (unit->general != nullptr)
			{
				unitJson["general"] = addCharacter(unit->general);
			}
			else
			{
				unitJson["general"] = 0;
			}
			unitJson["soldiers"] = unit->SoldierCountStrat;
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


	struct fortDataS
	{
		bool isFort = false;
		std::string worldRecord;

	};
	struct battleSettlementCreatorS
	{
		fortDataS fort;
	}battleSett;

	void to_json(jsn::json& j, const battleSettlementCreatorS& sett)
	{
		j = jsn::json();


		if (sett.fort.isFort == false)
		{
			j["fort"] = 0;
		}
		else
		{
			jsn::json jFort = jsn::json();

			jFort["worldRecord"] = sett.fort.worldRecord;


			j["fort"] = jFort;
		}
	}

	void from_json(const jsn::json& j, battleSettlementCreatorS& sett)
	{
		if (j.at("fort").is_number())
		{
			sett.fort.isFort = false;
		}
		else
		{
			sett.fort.isFort = true;
			sett.fort.worldRecord = j.at("fort").at("worldRecord");
		}
	}




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
		battleArmies.sides.clear();
		battleDataS* battle = smallFuncs::getGameDataAll()->battleHandler;
		for (int i = 0; i < battle->sidesNum; i++)
		{
			addSide(battle->sides[i]);
		}
	}

	void fillStructsForResults()
	{
		initStructsForResults();
		for (auto& side : battleArmies.sides)
		{
			for (size_t i=0;i< side->armies.size();i++)
			{
				auto& army = side->armies[i];
				for (size_t j = 0; j < army->units.size();j++)
				{
					army->unitsForTransfer.push_back(make_shared<unitDataS>(army->units[j], j));
				}
			}
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



	struct
	{
		int numOfArmy = 0;
	}charactersParams;
	void writeCharactersJson(const std::string& filePath)
	{
		charactersParams.numOfArmy = 0;
		ofstream f1(filePath);
		jsn::json json = battleArmies;
		f1 << setw(4) << json;
		f1.close();
	}

	void writeSettlementJson(const std::string& filePath, const std::string& lastSettlementWorldRec)
	{
		battleDataS* battleData = smallFuncs::getGameDataAll()->battleHandler;
		fortStruct* isFort = fastFuncts::findFort(battleData->xCoord, battleData->yCoord);
		if (isFort == nullptr)
		{
			battleSett.fort.isFort = false;
		}
		else
		{
			battleSett.fort.isFort = true;
			battleSett.fort.worldRecord = lastSettlementWorldRec;
		}

		ofstream f1(filePath);
		jsn::json json = battleSett;
		f1 << setw(4) << json;
		f1.close();
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
		Sleep(200);
		battleDataS* battle = smallFuncs::getGameDataAll()->battleHandler;

		std::vector<std::pair<shared_ptr<unitDataS>, unit*>>transferPairs;
		if (battleArmies.sides.size() != battle->sidesNum)
		{
			MessageBoxA(NULL, "The number of sides in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
			
			clearStructs();
			return false;
		}

		for (size_t sideI = 0; sideI < battleArmies.sides.size(); sideI++)
		{
			auto& side = battleArmies.sides[sideI];
			auto& gameSide = battle->sides[sideI];
			if (side->armies.size() != gameSide.armiesNum)
			{
				MessageBoxA(NULL, "The number of armies in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);

				clearStructs();
				return false;
			}
			for (size_t armyI = 0; armyI < side->armies.size(); armyI++)
			{
				auto& army = side->armies[armyI];
				auto& gameArmy = gameSide.armies[armyI];
				if (army->unitsForTransfer.size() != gameArmy.stack->numOfUnits)
				{
					MessageBoxA(NULL, "The number of units in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);

					clearStructs();
					return false;
				}

				for (size_t unitI = 0; unitI < army->unitsForTransfer.size(); unitI++)
				{
					auto& gameUnit = gameArmy.stack->units[unitI];


					auto ourUnit = army->getUnitWithID(unitI);
					if (ourUnit == nullptr)
					{
						MessageBoxA(NULL, "M2TWEOP can`t find correct unit in json!", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
						
						clearStructs();
						return false;
					}
					if (strcmp(ourUnit->type.c_str(), gameUnit->eduEntry->Type) != 0)
					{
						MessageBoxA(NULL, "The unit types in the results file and in the battle does not match.", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);

						clearStructs();
						return false;
					}
					transferPairs.push_back({ ourUnit ,gameUnit });

				}
			}
		}

		for (auto& unitPair : transferPairs)
		{
			auto& unit = unitPair.first;
			auto& gameUnit = unitPair.second;
			if (gameUnit->general!=nullptr&& unit->gen.isCharacterDied)
			{
				fastFuncts::setSoldiersCountAndExp(gameUnit, 0, unit->exp);
			}				
			else
			{
				fastFuncts::setSoldiersCountAndExp(gameUnit, unit->soldiersNumber, unit->exp);
			}
		}
		transferPairs.clear();

		clearStructs();
		return true;
	}

	bool transferResults(const std::string& resultsPath, int selectedWinner, bool noCasualities)
	{
		if (noCasualities==true)
		{
			fillStructsForResults();
		}
		else
		{
			jsn::json json2;
			try
			{
				std::ifstream f2(std::filesystem::u8path(resultsPath));


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
		else//draw
		{
			transferResults2();
		}

		return true;
		bool transferResult = doTransfer();
		if (transferResult == false)
		{
			MessageBoxA(NULL, "Something goes wrong!", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
			return false;
		}
		return transferResult;
	}

	void transferResults2()
	{
		std::thread thrUrl(
			doTransfer);
		thrUrl.detach();
	}

	void onLoadCharacter(stackStruct* army, const std::filesystem::path& relativePath)
	{
		charactersParams.numOfArmy++;
		jsn::json json2;
		try
		{

			std::ifstream f2(relativePath);

			if (f2.is_open()==false)
			{
				return;
			}
			f2 >> json2;

			f2.close();
		}
		catch (jsn::json::parse_error& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}


		try
		{
			battleArmies = json2;
		}
		catch (jsn::json::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}

		
		int currArmy = 0;
		for (auto& side : battleArmies.sides)
		{
			for (auto& armySide : side->armies)
			{
				currArmy++;
				if (currArmy == charactersParams.numOfArmy)
				{
					if (armySide->unitsForTransfer.size() != army->numOfUnits)
					{
						MessageBoxA(NULL, "M2TWEOP characters creating error!", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
						return;
					}
					if (army->units[0]->general != nullptr)
					{
						army->units[0]->general->genChar->index = armySide->unitsForTransfer[0]->numberInArmy;
					}

					for (int i = 0; i < army->numOfUnits; i++)
					{
						army->units[i]->ID = armySide->unitsForTransfer[i]->numberInArmy;
					}
					if (army->numOfUnits <= 1)
					{
						return;
					}
					for (int i = 1; i < army->numOfUnits; i++)
					{
						if (strcmp(army->units[i]->eduEntry->Type, armySide->unitsForTransfer[i]->type.c_str()) != 0)
						{
							MessageBoxA(NULL, "M2TWEOP characters creating error!", "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
							return;
						}

						if (armySide->unitsForTransfer[i]->isHaveCharacter == false)
						{
							continue;
						}

						auto& newGen = armySide->unitsForTransfer[i]->gen;

						const char* portrait = nullptr;
						if (newGen.portrait != "default")
						{
							portrait = newGen.portrait.c_str();
						}
	
						general*newGeneral=fastFuncts::createCharacterWithoutSpawning("named character",army->faction, newGen.age
							, newGen.genName.c_str(), newGen.genName.c_str(), newGen.subfaction
							, portrait, 0,0);
						fastFuncts::setBodyguard(newGeneral, army->units[i]);
						newGeneral->genChar->index = armySide->unitsForTransfer[i]->numberInArmy;
						for (std::string& anc : newGen.ancillaries)
						{
							auto* resAnc=fastFuncts::findAnchillary((char*)anc.c_str());
							if (resAnc != nullptr)
							{
								fastFuncts::addAnchillary(newGeneral->genChar, resAnc);
							}
						}

						for (auto& trait : newGen.traits)
						{
							fastFuncts::addTrait(newGeneral->genChar, trait.first.c_str(), trait.second);
						}
					}


					return;
				}
			}
		}
	}
	string onLoadSettlementWord(const std::filesystem::path& relativePath)
	{
		jsn::json json2;
		try
		{

			std::ifstream f2(relativePath);

			if (f2.is_open() == false)
			{
				return "";
			}
			f2 >> json2;

			f2.close();
		}
		catch (jsn::json::parse_error& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}


		try
		{
			battleSett = json2;
		}
		catch (jsn::json::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}


		if (battleSett.fort.isFort == false)
		{
			return "";
		}

		else
		{
			return battleSett.fort.worldRecord;
		}
	}

	void clearStructs()
	{
		battleArmies.sides.clear();
	}




};