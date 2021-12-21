#include "onlineThings.h"
#include "imgui_notify.h"

#include "onlineThings2.h"

#include <ImFileDialog.h>
#include <imgui_internal.h>
namespace battleCreator
{
	struct
	{
		bool isGenerationNeeded = true;
		bool isResultTransferNeeded = true;

		atomic_bool isRunStarted{ false };
		atomic_bool isRunEnded{ false };
	}data;

	void readParams()
	{
		jsn::json json;

		std::string fPath = globals::dataS.modPatch;
		fPath += "\\eopBattles\\battlesCfg.json";


		std::ifstream f1(fPath);

		f1 >> json;

		f1.close();

		try
		{
			json.at("enableAutoGeneration").get_to(data.isGenerationNeeded);
			json.at("enableResultsTransfer").get_to(data.isResultTransferNeeded);
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void setCreationNeeded(bool isNeeded)
	{
		data.isGenerationNeeded = isNeeded;
	}

	string makeWeatherString()
	{
		string retS;
		if (fastFuncts::getSeason() == 0)
		{
			retS = "summer";
		}
		else
		{
			retS = "winter";
		}

		return retS;
	}
	string makeBattleName()
	{
		string battleName = makeWeatherString();

		battleName += "_T";
		battleName += to_string(fastFuncts::getPassedTurnsNum());
		battleName += "_";
		factionStruct* currFac = smallFuncs::getGameDataAll()->campaignData->currentFactionTurn;
		battleName += currFac->factSmDescr->facName;

		return battleName;
	}


	set<string>getFacsNamesInBattle()
	{
		set<string>retSet;
		battleDataS* battleData = smallFuncs::getGameDataAll()->battleHandler;

		for (int i = 0; i < battleData->sidesNum; i++)
		{
			battleSide& side = battleData->sides[i];


			for (int j = 0; j < side.armiesNum; j++)
			{
				stackStruct* army = side.armies[j].stack;

				retSet.emplace(army->faction->factSmDescr->facName);
			}
		}

		return retSet;
	}


	string getSettlLevelString(int lev)
	{
		string s;
		if (lev == 0)
			s = "village";
		else if (lev == 1)
		{
			s = "town";
		}
		else if (lev == 2)
		{
			s = "large_town";
		}
		else if (lev == 3)
		{
			s = "city";
		}
		else if (lev == 4)
		{
			s = "large_city";
		}
		else if (lev == 5)
		{
			s = "huge_city";
		}
		else
		{
			//MessageBoxA(NULL, "Not correct town level :-(", "Not good, see descr_battle.txt", NULL);
			s = "unknown level :-) = ";
			s.append(to_string(lev));
		}
		return s;
	}
	void writeSettlementInfo(vector<string>& fileStrings, settlementStruct* sett)
	{
		string tempS;
		if (sett->isCastle == 1)
		{
			fileStrings.push_back("settlement castle");
		}
		else
		{
			fileStrings.push_back("settlement");
		}

		fileStrings.push_back("{");

		fileStrings.push_back(string("	level ").append(getSettlLevelString(sett->level)));
		fileStrings.push_back(string("	tile ").append(to_string(sett->xCoord).append(" ").append(to_string(sett->yCoord))));
		fileStrings.push_back("	year_founded 0");
		fileStrings.push_back("	population 999");
		fileStrings.push_back("	plan_set default_set");

		factionStruct* facCreator = smallFuncs::getGameDataAll()->campaignData->factionsSortedByID[sett->fac_creatorModNum];
		fileStrings.push_back(string("	faction_creator ").append(facCreator->factSmDescr->facName));
		for (int bnum = 0; bnum < sett->buldingsNum; bnum++)
		{
			fileStrings.push_back("	building");

			string buildType = sett->buildings[bnum]->bData->type;
			string buildName = sett->buildings[bnum]->bData->drawInfo[sett->buildings[bnum]->level].name;
			fileStrings.push_back("	{");

			tempS = "		type ";
			tempS.append(buildType);
			tempS.append(" ");
			tempS.append(buildName);
			fileStrings.push_back(tempS);

			fileStrings.push_back("	}");
		}
		fileStrings.push_back("}");
	}

	void writeCharacterInfo(vector<string>& fileStrings, general* gen)
	{
		int age = (gen->genChar->age >> 3) & 0x7f;

		string tempS = "character	";
		if (gen->genChar->fullName == nullptr || string(gen->genChar->fullName).size() == 0)
		{
			tempS.append("default");
		}
		else
		{
			tempS.append(gen->genChar->fullName);
		}
		tempS.append(", named character, male");
		if (gen->genChar->status & 2)
		{
			tempS.append(", heir");
		}
		else if (gen->genChar->status & 5)
		{
			tempS.append(", leader");
		}
		tempS.append(", age ").append(to_string(age));
		tempS.append(", x ").append(to_string(gen->xCoord));
		tempS.append(", y ").append(to_string(gen->yCoord));
		if (gen->genChar->portrait_custom)
		{
			tempS.append(", portrait ").append(gen->genChar->portrait_custom);
		}
		if (gen->genChar->modelName)
		{
			tempS.append(", battle_model ").append(gen->genChar->modelName);
		}
		if (gen->ability)
		{
			tempS.append(", hero_ability ").append(gen->ability);
		}
		if (gen->genChar->label)
		{
			tempS.append(", label ").append(gen->genChar->label);
		}




		fileStrings.push_back(tempS);

		if (gen->genChar->traits != nullptr)
		{
			tempS = "traits";
			traitContainer* traitCont = gen->genChar->traits;
			while (traitCont != nullptr)
			{
				tempS.append(" ").append(traitCont->trait->nameS->name).append(" ").append(to_string(*traitCont->trait->level)).append(" ,");

				traitCont = traitCont->next;
			};
			fileStrings.push_back(tempS);
		}

		if (gen->genChar->anchNum != 0)
		{
			tempS = "ancillaries";
			UINT32 anchNum = gen->genChar->anchNum;
			for (UINT32 i = 0; i < anchNum; i++)
			{
				tempS.append(" ").append(gen->genChar->anchillaries[i]->dataAnch->anchName).append(" ,");
			}
			fileStrings.push_back(tempS);
		}
	}
	void createHeaderSection(vector<string>& fileStrings)
	{
		string battleName = makeBattleName();
		string tempS;

		fileStrings.push_back(";Generated by new M2TW Engine Overhaul Project\n");

		tempS = "battle		";
		tempS.append(battleName);
		tempS.append("		multiplayer");
		fileStrings.push_back(tempS);

		fileStrings.push_back("playable");

		set<string>facsNames = getFacsNamesInBattle();
		for (const string& fName : facsNames)
		{
			fileStrings.push_back(string("	").append(fName));
		}
		fileStrings.push_back("end");
		fileStrings.push_back("nonplayable");
		fileStrings.push_back("end");
		fileStrings.push_back("\n");
		fileStrings.push_back("\n");

		tempS = "start_date	";
		tempS.append(to_string(fastFuncts::getYear())).append(" ").append(makeWeatherString());
		fileStrings.push_back(tempS);

		tempS = "end_date	";
		tempS.append(to_string(fastFuncts::getYear())).append(" ").append(makeWeatherString());
		fileStrings.push_back(tempS);
		fileStrings.push_back("\n");
		fileStrings.push_back("\n");
		fileStrings.push_back(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");

	}
	void createFactionsSection(vector<string>& fileStrings)
	{
		battleDataS* battleData = smallFuncs::getGameDataAll()->battleHandler;
		string tempS;

		fileStrings.push_back("; >>>> start of factions section <<<<\n");

		for (int i = 0; i < battleData->sidesNum; i++)
		{
			battleSide& side = battleData->sides[i];

			set<string> facsNames = getFacsNamesInBattle();


			for (int j = 0; j < side.armiesNum; j++)
			{
				stackStruct* army = side.armies[j].stack;
				if (facsNames.count(army->faction->factSmDescr->facName))
				{
					facsNames.erase(army->faction->factSmDescr->facName);

					fileStrings.push_back(string("faction	").append(army->faction->factSmDescr->facName));
				}

				if (army->gen != nullptr && army->gen->xCoord == battleData->xCoord
					&& army->gen->yCoord == battleData->yCoord
					)
				{
					settlementStruct* sett = fastFuncts::findSettlement(army->gen->xCoord, army->gen->yCoord);
					if (sett != nullptr)
					{
						writeSettlementInfo(fileStrings, sett);
					}
				}

				if (army->gen != nullptr)
				{
					general* gen = army->gen;
					writeCharacterInfo(fileStrings, gen);
				}
				else
				{
					fileStrings.push_back("character	default, general, male");
				}
				fileStrings.push_back("\n");

				fileStrings.push_back("army");
				for (int k = 0; k < army->numOfUnits; k++)
				{
					unit* un = army->units[k];
					tempS = "unit		";
					tempS.append(un->eduEntry->Type);
					tempS.append("				soldiers ").append(to_string(un->number));
					tempS.append(" exp ").append(to_string(un->expScreen));
					tempS.append(" armour ").append(to_string(un->stats >> 0xd & 0x1f));
					tempS.append(" weapon_lvl ").append(to_string(un->stats >> 0x8 & 0x1f));

					fileStrings.push_back(tempS);
				}
				fileStrings.push_back("\n");
			}
		}
		fileStrings.push_back(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");
	}
	void createBattleSection(vector<string>& fileStrings)
	{
		battleDataS* battleData = smallFuncs::getGameDataAll()->battleHandler;
		string tempS;

		fileStrings.push_back("; >>>> start of battle section <<<<\n");

		tempS = "battle	";
		tempS.append(to_string(battleData->xCoord)).append(", ").append(to_string(battleData->yCoord));
		fileStrings.push_back(tempS);

		tempS = "battle_time	";
		tempS.append(to_string(smallFuncs::getGameDataAll()->campaignData->timeAtStartBattle));
		tempS.append("	0.00");
		fileStrings.push_back(tempS);

		fileStrings.push_back("weather	clear\n");

		tempS = "home_faction	";
		tempS.append(battleData->sides[0].armies[0].stack->faction->factSmDescr->facName);
		fileStrings.push_back(tempS);


		for (int i = 0; i < battleData->sidesNum; i++)
		{
			battleSide& side = battleData->sides[i];
			tempS = "alliance	";
			if (side.isCanDeploy == true)
			{
				tempS.append("can_deploy	");
			}
			tempS.append("can_view	");
			set<string>sideFactions;
			for (int j = 0; j < side.armiesNum; j++)
			{
				stackStruct* army = side.armies[j].stack;

				sideFactions.emplace(army->faction->factSmDescr->facName);
			}
			for (const string& fac : sideFactions)
			{
				tempS.append(fac).append(", ");
			}
			if (side.isDefender == true)
			{
				tempS.append("	defending");
			}
			else
			{
				tempS.append("	attacking");
			}
			fileStrings.push_back(tempS);
		}



		for (int i = 0; i < battleData->sidesNum; i++)
		{
			battleSide& side = battleData->sides[i];
			map<string, int>facArms;

			for (int j = 0; j < side.armiesNum; j++)
			{
				stackStruct* army = side.armies[j].stack;
				char* armyFacName = army->faction->factSmDescr->facName;

				fileStrings.push_back("");
				tempS = "army	";
				tempS.append(armyFacName);



				int ifHere = facArms.count(armyFacName);
				int newCount = 0;
				if (ifHere > 0)
				{
					auto res = facArms.find(armyFacName);

					res->second++;
					newCount = res->second;
				}

				facArms.insert(pair<string, int>(armyFacName, newCount));


				tempS.append(", ");
				tempS.append(to_string(newCount));
				tempS.append(", no_withdraw, supporting_armies 0, reform_point ");
				tempS.append(to_string(army->reform_point_x));
				tempS.append(", ");
				tempS.append(to_string(army->reform_point_y));
				fileStrings.push_back(tempS);
				fileStrings.push_back("\n");

				int coordsPairsNum = side.armies[j].deploymentArea->coordsNum;
				float* coords = side.armies[j].deploymentArea->coordsPairs;
				for (int k = 0; k < coordsPairsNum; k++)
				{
					tempS = "deployment_area_point	";
					tempS.append(to_string(coords[k * 2]));
					tempS.append(", ");
					tempS.append(to_string(coords[k * 2 + 1]));
					fileStrings.push_back(tempS);
				}
				fileStrings.push_back("\n");

				for (int k = 0; k < army->numOfUnits; k++)
				{
					unit* un = army->units[k];
					while (1)
					{
						if (un->soldiersBattleArr == nullptr)
						{
							continue;
						}
						if (un->soldiersBattleArr[0]->xCoord == 0 && un->soldiersBattleArr[0]->yCoord == 0 && un->soldiersBattleArr[0]->zCoord == 0)
						{
							continue;
						}

						tempS = "unit			";
						tempS.append(to_string(k));
						tempS.append(", ");
						tempS.append(to_string(un->soldiersBattleArr[0]->xCoord));
						tempS.append(", ");
						tempS.append(to_string(un->soldiersBattleArr[0]->yCoord));
						tempS.append(", ");
						tempS.append("0");
						tempS.append(", formation_type square, formation_width ");
						tempS.append("50");


						if (un->eduEntry->Category == 0 && un->engineRec != nullptr)
						{
							engineRecord* enRec = un->engineRec;
							if (enRec->classID == 18)//ram
							{
								tempS.append(", attach_engine ram");
							}
							else if (enRec->classID == 19)//ladder
							{
								tempS.append(", attach_engine ladder");
							}
							else if (enRec->classID == 17)//tower
							{
								tempS.append(", attach_engine tower");
							}
						}

						fileStrings.push_back(tempS);
						break;
					}

				}
			}

		}
		fileStrings.push_back(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");
	}

	void createObjectivesSection(vector<string>& fileStrings)
	{
		battleDataS* battleData = smallFuncs::getGameDataAll()->battleHandler;
		string tempS;

		fileStrings.push_back("; >>>> start of objectives scripting section <<<<\n");

		fileStrings.push_back("\nobjectives");

		for (int i = 0; i < battleData->sidesNum; i++)
		{
			battleSide& side = battleData->sides[i];
			tempS = "\nalliance ";
			tempS.append(to_string(i));

			fileStrings.push_back(tempS);

			for (int i = 0; i < 4; i++)
			{
				DWORD winCond = side.winConditions[i];
				if (winCond != 0)
				{
					tempS = "condition ";
					tempS.append(smallFuncs::getWinConditionS(winCond));
					fileStrings.push_back(tempS);
				}
			}


		}
		fileStrings.push_back("\n;Generation completed");
	}



	void createBattle()
	{

		std::string bName = makeBattleName();

		ImGuiToast bMsg(ImGuiToastType_Info, 25000);

		bMsg.set_title("Battle generation");
		bMsg.set_content("Started generation of %s", bName.c_str());
		ImGui::InsertNotification(bMsg);


		vector<string>fileStrings;
		createHeaderSection(fileStrings);


		bMsg = ImGuiToast(ImGuiToastType_Info, 10000);
		bMsg.set_title("Battle generation");
		bMsg.set_content("Header section of %s done", bName.c_str());
		ImGui::InsertNotification(bMsg);


		createFactionsSection(fileStrings);

		bMsg = ImGuiToast(ImGuiToastType_Info, 10000);
		bMsg.set_title("Battle generation");
		bMsg.set_content("Factions section of %s done", bName.c_str());
		ImGui::InsertNotification(bMsg);

		createBattleSection(fileStrings);

		bMsg = ImGuiToast(ImGuiToastType_Info, 10000);
		bMsg.set_title("Battle generation");
		bMsg.set_content("Battle section of %s done", bName.c_str());
		ImGui::InsertNotification(bMsg);

		createObjectivesSection(fileStrings);

		bMsg = ImGuiToast(ImGuiToastType_Info, 10000);
		bMsg.set_title("Battle generation");
		bMsg.set_content("Objectives section of %s done", bName.c_str());
		ImGui::InsertNotification(bMsg);

		std::string fPath = globals::dataS.modPatch;
		fPath += "\\eopBattles";
		filesystem::create_directory(fPath);
		fPath += "\\lastBattle";
		filesystem::remove_all(fPath);
		filesystem::create_directory(fPath);
		fPath += "\\";
		fPath += bName;


		filesystem::create_directory(fPath);
		writeCharactersJson(fPath + "\\m2tweopDescr1.json");

		fPath += "\\descr_battle.txt";
		ofstream f1(fPath);
		for (string& s : fileStrings)
		{
			f1 << s << endl;
		}
		f1.close();

		bMsg = ImGuiToast(ImGuiToastType_Success, 25000);
		bMsg.set_title("Battle generation");
		bMsg.set_content("Done generation of %s", bName.c_str());
		ImGui::InsertNotification(bMsg);
	}




	void doWork(std::atomic_bool& isStarted)
	{
		if (isStarted == true)
		{
			return;
		}

		if (data.isGenerationNeeded == false)
		{
			return;
		}

		isStarted = true;


		initStructsForResults();
		createBattle();


		isStarted = false;
	}
	void startCreationThread()
	{


		if (data.isGenerationNeeded == false)
		{
			return;
		}




		std::thread thrUrl(
			doWork, std::ref(data.isRunStarted));
		thrUrl.detach();
	}
	void onBattleResultsScreen()
	{
		std::thread thrUrl(createResultsFile);
		thrUrl.detach();
	}



	bool battleResultsMenuOpened = false;
	bool battleResultsSuccessOpened = false;
	void onBattleStratScreen()
	{
		if (data.isResultTransferNeeded == false)
		{
			return;
		}

		battleResultsMenuOpened = true;
	}

	void draw(LPDIRECT3DDEVICE9 pDevice)
	{
		if (battleResultsMenuOpened == true)
		{
			ImGui::OpenPopup("Select winner##Online battles results transfering");
		}
		if (battleResultsSuccessOpened == true)
		{
			ImGui::OpenPopup("Succes##Online battles results transfer");
		}


		ImGui::SetNextWindowSize({300,0}, ImGuiCond_Always);
		if (ImGui::BeginPopupModal("Select winner##Online battles results transfering", &battleResultsMenuOpened, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
		{
			static int resolveVariant = 0;

			const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;
			static float defenderButtonWidth = 10.0f;
			static float drawButtonWidth = 10.0f;
			float pos = 0;




			ImGui::RadioButton("attacker", &resolveVariant, 0); 

			ImGui::SameLine(ImGui::GetWindowWidth()/2- defenderButtonWidth/2);
			ImGui::RadioButton("defender", &resolveVariant, 1); 
			defenderButtonWidth = ImGui::GetItemRectSize().x;

			pos =drawButtonWidth + ItemSpacing;
			ImGui::SameLine(ImGui::GetWindowWidth() - pos);
			ImGui::RadioButton("draw", &resolveVariant, 2);
			drawButtonWidth = ImGui::GetItemRectSize().x;



			ImGui::NewLine();

			static std::string selectedFile;

			if (ImGui::Button("select results file", { -1.0f,0.0f }))
			{
				ifd::FileDialog::Instance().Open("select results file", "select results file", "results file (*.json;){.json},.*");
			}
			if (ifd::FileDialog::Instance().IsDone("select results file")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					selectedFile = ifd::FileDialog::Instance().GetResult().u8string();
				}
				ifd::FileDialog::Instance().Close();
			}

			if (!selectedFile.empty())
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				ImGui::Separator();
				ImGui::TextWrapped(selectedFile.c_str());
				ImGui::Separator();
				//ImGui::InputText("Selected results file", &selectedFile);
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();

				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::NewLine();
				if (ImGui::Button("Transfer results now", { -1.0f,0.0f }))
				{
					bool res=transferResults(selectedFile, resolveVariant);
					if (res == true)
					{

						battleResultsMenuOpened = false;
						battleResultsSuccessOpened = true;
						ImGui::CloseCurrentPopup();
						selectedFile.clear();
						resolveVariant = 0;
					}
				}
			}
			else
			{
				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

				ImGui::Button("Transfer results now", { -1.0f,0.0f });

				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			ImGui::NewLine();

			/*if (ImGui::Button("decline"))
			{
				ImGui::CloseCurrentPopup();
				battleResultsMenuOpened = false;
			}*/

			ImGui::EndPopup();
		}


		ImGui::SetNextWindowSize({ 300,0 }, ImGuiCond_Always);
		if (ImGui::BeginPopupModal("Succes##Online battles results transfer", &battleResultsSuccessOpened, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
		{

			if (ImGui::Button("Continue the game", { -1.0f,0.0f }))
			{
				ImGui::CloseCurrentPopup();
				battleResultsSuccessOpened = false;
			}
			ImGui::EndPopup();
		}
	}

	void addCharactersToCustomBattleArmy(stackStruct* army, const std::string& relativePath)
	{

		std::filesystem::path charactersPath = relativePath;
		charactersPath.replace_filename("m2tweopDescr1.json");
		onLoadCharacter(army, charactersPath);
	}
};
