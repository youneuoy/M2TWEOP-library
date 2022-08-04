#include "managerG.h"
#include "helpers.h"

#include "managerGHelpers.h"
#include "modSettingsUI.h"
#include <iomanip>
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

	jsn::json loadJsonFromFile(const std::string&fpath)
	{
		try
		{
			jsn::json json;

			std::ifstream f1(fpath);
			if (f1.is_open())
			{
				f1 >> json;
			}
			f1.close();
			return std::move(json);
		}
		catch
		{
			MessageBoxA(NULL, e.what(), "Could not load JSON from file!", MB_APPLMODAL | MB_SETFOREGROUND);
		}

	}

	void loadBattleConfig()
	{
		std::string fPath = ".\\eopBattles\\battlesCfg.json";
		jsn::json json= loadJsonFromFile(fPath);

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
			MessageBoxA(NULL, e.what(), "Could not read JSON in battlesCfg.json!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void loadGameConfig()
	{

		fPath = ".\\eopData\\gameCfg.json";
		json= loadJsonFromFile(fPath);

		try
		{
			if (json.contains("isBlockLaunchWithoutEop"))
			{
				getJson(dataG::data.gameData.isBlockLaunchWithoutEop, "isBlockLaunchWithoutEop");
			}
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Could not read JSON in gameCfg.json", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void loadUIConfig()
	{
		fPath = ".\\eopData\\uiCfg.json";
		json= loadJsonFromFile(fPath);

		try
		{
			if (json.contains("useVanillaCfg"))
			{
				getJson(dataG::data.modData.useVanillaConfig, "useVanillaCfg");
			}
			if(json.contains("modCfgFile"))
			{
				getJson(dataG::data.modData.configName, "modCfgFile");
			}
			if(json.contains("useM2TWEOP"))
			{
				getJson(dataG::data.modData.useM2TWEOP, "useM2TWEOP");
			}
			if(json.contains("hideLauncher"))
			{
				getJson(dataG::data.modData.hideLauncherAtStart, "hideLauncher");
			}
			if(json.contains("playBackgroundMusic"))
			{
				getJson(dataG::data.audio.bkgMusic.isMusicNeeded, "playBackgroundMusic");
			}
			if (json.contains("musicVolume"))
			{
				getJson(dataG::data.audio.bkgMusic.musicVolume, "musicVolume");
			}
			else if (s == "Mod_title:")
			{
				getline(f1, dataG::data.gameData.modTitle);
			}
			else if (s == "Button_color:")
			{
				getline(f1, s);
				try
				{
					dataG::data.gameData.buttonColor = stoi(s, 0, 16);
				}
				catch (std::exception& e)
				{
					MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}
			}
			else if (s == "Button_hover_color:")
			{
				getline(f1, s);
				try
				{
					dataG::data.gameData.buttonHoverColor = stoi(s, 0, 16);
				}
				catch (std::exception& e)
				{
					MessageBoxA(NULL, e.what(), "Warning!", MB_APPLMODAL | MB_SETFOREGROUND);
				}
			}
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Could not read JSON in uiCfg.json", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void loadJsonSettings()
	{
		loadBattleConfig();
		loadGameConfig();
		loadUIConfig();
	}

	void writeJsonToFile(const std::string& fpath,const jsn::json& json)
	{
		ofstream f1(fpath);
		f1 << setw(4) << json;
		f1.close();
	}

	void saveJsonSettings()
	{
		jsn::json json;
		std::string fPath;

		// Save battle config
		fPath = ".\\eopBattles\\battlesCfg.json";
		setJson("enableAutoGeneration", dataG::data.battlesData.isGenerationNeeded);
		setJson("enableResultsTransfer", dataG::data.battlesData.isResultTransferNeeded);
		writeJsonToFile(fPath, json);
		json.clear();

		// Save game config
		fPath = ".\\eopData\\gameCfg.json";
		setJson("isBlockLaunchWithoutEop", dataG::data.gameData.isBlockLaunchWithoutEop);
		writeJsonToFile(fPath, json);
		json.clear();

		// Save UI config
		fPath = ".\\eopData\\uiCfg.json";
		setJson("useVanillaCfg", dataG::data.modData.useVanillaConfig);
		setJson("modCfgFile", dataG::data.modData.configName);
		setJson("useM2TWEOP", dataG::data.modData.useM2TWEOP);
		setJson("hideLauncher", dataG::data.modData.hideLauncherAtStart);
		setJson("playBackgroundMusic", dataG::data.audio.bkgMusic.isMusicNeeded);
		setJson("musicVolume", dataG::data.audio.bkgMusic.musicVolume);
		writeJsonToFile(fPath, json);
		json.clear();
	}

	bool isRedistsInstallNeeded()
	{
		return (!isLibraryLoadable("M2TWEOPLibrary.dll") || !isLibraryLoadable("d3d9.dll"));

		//variant 2
		//
		//
		//std::string fileVerName;

		//bool retVal = false;

		//std::string fPath = "..\\..\\eopVer.json";
		//jsn::json json = loadJsonFromFile(fPath);

		//try
		//{
		//	if (json.contains("fileVerName"))
		//	{
		//		getJson(fileVerName, "fileVerName");
		//	}
		//	if (fileVerName != std::string(eopConstData::eopVersionName))
		//	{
		//		retVal = true;
		//	}
		//}
		//catch (...)
		//{
		//	retVal = true;
		//}
		//

		//return retVal;
	}
};