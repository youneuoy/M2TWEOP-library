#include "managerG.h"
#include "helpers.h"

#include "managerGHelpers.h"
#include "modSettingsUI.h"
namespace managerG
{
	void loadSettings();

	void init()
	{
		helpers::updateMetrics();
		loadTextures();
		loadFonts();
		loadSettings();


		modSettingsUI::initSettingsUI();
	}

	void loadJsonSettings()
	{
		jsn::json json;

		std::string fPath = ".\\eopBattles\\battlesCfg.json";


		std::ifstream f1(fPath);
		if(f1.is_open())
		{ 
		f1 >> json;
		}
		f1.close();

		try
		{
			if (json.contains("enableAutoGeneration"))
			{
				getJson(dataG::data.battlesData.isGenerationNeeded, "enableAutoGeneration");
			}
			if (json.contains("enableResultsTransfer"))
			{
				getJson(dataG::data.battlesData.isResultTransferNeeded, "enableResultsTransfer");
			}
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}
	void loadSettings()
	{
		loadJsonSettings();
		ifstream f1("M2TWEOPGUI.cfg");


		string s;
		while (f1.good())
		{
			getline(f1, s);

			if (s == "Use_vanilla_cfg:")
			{
				getline(f1, s);
				dataG::data.modData.useVanillaConfig = stoi(s);
			}
			else if (s == "Mod_cfg_file:")
			{
				getline(f1, dataG::data.modData.configName);
			}
			else if (s == "Use_M2TWEOP:")
			{
				getline(f1, s);
				dataG::data.modData.useM2TWEOP = stoi(s);
			}
			else if (s == "Hide_launcher:")
			{
				getline(f1, s);
				dataG::data.modData.hideLauncherAtStart = stoi(s);
			}
			else if (s == "Play_background_music:")
			{
				getline(f1, s);
				dataG::data.audio.bkgMusic.isMusicNeeded = stoi(s);
			}
			else if (s == "Music_volume:")
			{
				getline(f1, s);
				dataG::data.audio.bkgMusic.musicVolume = stoi(s);
			}
		}


		f1.close();
	}
	void saveJsonSettings()
	{
		std::string fPath = ".\\eopBattles\\battlesCfg.json";
		ofstream f1(fPath);
		jsn::json json;
		setJson("enableAutoGeneration", dataG::data.battlesData.isGenerationNeeded);
		setJson("enableResultsTransfer", dataG::data.battlesData.isResultTransferNeeded);

		f1 << setw(4) << json;
		f1.close();
	}
	void saveSettings()
	{
		saveJsonSettings();
		ofstream f1("M2TWEOPGUI.cfg");

		f1 << "Use_vanilla_cfg:" << endl;
		f1 << dataG::data.modData.useVanillaConfig << endl;
		f1 << "Mod_cfg_file:" << endl;
		f1 << dataG::data.modData.configName << endl;
		f1 << "Use_M2TWEOP:" << endl;
		f1 << dataG::data.modData.useM2TWEOP << endl;
		f1 << "Hide_launcher:" << endl;
		f1 << dataG::data.modData.hideLauncherAtStart << endl;
		f1 << "Play_background_music:" << endl;
		f1 << dataG::data.audio.bkgMusic.isMusicNeeded << endl;
		f1 << "Music_volume:" << endl;
		f1 << dataG::data.audio.bkgMusic.musicVolume << endl;


		f1.close();
	}
};