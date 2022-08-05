#include "managerG.h"
#include "helpers.h"

#include "managerGHelpers.h"
#include "modSettingsUI.h"
#include <iomanip>

namespace managerG
{
	void loadJsonConfig();

	void init()
	{
		helpers::updateMetrics();
		loadTextures();
		loadFonts();
		loadJsonConfig();

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
		catch (jsn::json::exception& e)
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

		std::string fPath = ".\\eopData\\gameCfg.json";
		jsn::json json = loadJsonFromFile(fPath);

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
		std::string fPath = ".\\eopData\\uiCfg.json";
		jsn::json json = loadJsonFromFile(fPath);
		std::string jsonStringValue;
		std:bool jsonBoolValue;

		try
		{
			if (json.contains("useVanillaCfg"))
			{
				getJson(jsonBoolValue, "useVanillaCfg");
				dataG::data.modData.useVanillaConfig = jsonBoolValue;
			}
			if(json.contains("modCfgFile"))
			{
				getJson(dataG::data.modData.configName, "modCfgFile");
			}
			if(json.contains("useM2TWEOP"))
			{
				getJson(jsonBoolValue, "useM2TWEOP");
				dataG::data.modData.useM2TWEOP = jsonBoolValue;
			}
			if(json.contains("hideLauncher"))
			{
				getJson(jsonBoolValue, "hideLauncher");
				dataG::data.modData.hideLauncherAtStart = jsonBoolValue;
			}
			if(json.contains("playBackgroundMusic"))
			{
				getJson(jsonBoolValue, "playBackgroundMusic");
				dataG::data.audio.bkgMusic.isMusicNeeded = jsonBoolValue;
			}
			if (json.contains("musicVolume"))
			{
				getJson(dataG::data.audio.bkgMusic.musicVolume, "musicVolume");
			}
			if (json.contains("modTitle"))
			{
				getJson(dataG::data.gameData.modTitle, "modTitle");
			}
			//if (json.contains("runButtonColor"))
			//{
				// getJson(jsonStringValue, "buttonColor");
				jsonStringValue = "f024b6";
				dataG::data.gameData.buttonColor = stoi(jsonStringValue, NULL, 16); // Convert to hex
		//	}
			//if (json.contains("runButtonHoverColor"))
			//{
				// getJson(jsonStringValue, "runButtonHoverColor");
				jsonStringValue = "cfbfe5";
				dataG::data.gameData.buttonHoverColor = stoi(jsonStringValue, NULL, 16);  // Convert to hex
			//}
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Could not read JSON in uiCfg.json", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void loadJsonConfig()
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