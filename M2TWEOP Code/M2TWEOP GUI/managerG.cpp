#include "managerG.h"
#include "helpers.h"

#include "managerGHelpers.h"
#include "modSettingsUI.h"
#include <iomanip>
#include <regex>

namespace managerG
{
	void loadJsonConfig();

	void init()
	{
		helpers::updateMetrics();
		loadTextures();
		loadFonts();
		// loadJsonConfig();

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
		std::string fPath = ".\\eopData\\config\\battlesCfg.json";
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
			if (json.contains("isPlannedRetreatRoute"))
			{
				getJson(dataG::data.battlesData.isPlannedRetreatRoute, "isPlannedRetreatRoute");
			}
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Could not read JSON in battlesCfg.json!", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void loadGameConfig()
	{

		std::string fPath = ".\\eopData\\config\\gameCfg.json";
		jsn::json json = loadJsonFromFile(fPath);

		try
		{
			if (json.contains("isDXVKEnabled"))
			{
				getJson(dataG::data.modulesData.isDXVKEnabled, "isDXVKEnabled");
			}
			if (json.contains("isContextMenuNeeded"))
			{
				getJson(dataG::data.modulesData.isContextMenuNeeded, "isContextMenuNeeded");
			}
			if (json.contains("isTacticalMapViewerNeeded"))
			{
				getJson(dataG::data.modulesData.isTacticalMapViewerNeeded, "isTacticalMapViewerNeeded");
			}
			if (json.contains("isDeveloperModeNeeded"))
			{
				getJson(dataG::data.modulesData.isDeveloperModeNeeded, "isDeveloperModeNeeded");
			}
			if (json.contains("isBlockLaunchWithoutEop"))
			{
				getJson(dataG::data.gameData.isBlockLaunchWithoutEop, "isBlockLaunchWithoutEop");
			}
			if (json.contains("IsOverrideBattleCamera"))
			{
				getJson(dataG::data.gameData.IsOverrideBattleCamera, "IsOverrideBattleCamera");
			}
			if (json.contains("isDiscordRichPresenceEnabled"))
			{
				getJson(dataG::data.gameData.isDiscordRichPresenceEnabled, "isDiscordRichPresenceEnabled");
			}
			if (json.contains("isFreecamIntegrationEnabled"))
			{
				getJson(dataG::data.gameData.freecamIntegration, "isFreecamIntegrationEnabled");
			}
		}
		catch (jsn::json::type_error& e)
		{
			MessageBoxA(NULL, e.what(), "Could not read JSON in gameCfg.json", MB_APPLMODAL | MB_SETFOREGROUND);
		}
	}

	void loadUIConfig()
	{
		std::string fPath = ".\\eopData\\config\\uiCfg.json";
		std::regex regex{ R"([\,\s]+)" };
		jsn::json json = loadJsonFromFile(fPath);
		std::string jsonStringValue;
		bool jsonBoolValue;

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
			if (json.contains("discordServerLink"))
			{
				getJson(dataG::data.gameData.discordServerLink, "discordServerLink");
			}
			if (json.contains("websiteLink"))
			{
				getJson(dataG::data.gameData.websiteLink, "websiteLink");
			}
			if (json.contains("modVersion"))
			{
				getJson(dataG::data.gameData.modVersion, "modVersion");
			}
			if (json.contains("runButtonColor"))
			{
				getJson(dataG::data.gameData.buttonColorString, "runButtonColor");

				if (dataG::data.gameData.buttonColorString != "") {
					std::sregex_token_iterator it{ dataG::data.gameData.buttonColorString.begin(), dataG::data.gameData.buttonColorString.end(), regex, -1 };
					std::vector<std::string> colorValues{ it, {} };
					dataG::data.gameData.buttonColor.r = stoi(colorValues[0]);
					dataG::data.gameData.buttonColor.g = stoi(colorValues[1]);
					dataG::data.gameData.buttonColor.b = stoi(colorValues[2]);
					dataG::data.gameData.buttonColor.a = stoi(colorValues[3]);
				}

			}
			if (json.contains("runButtonHoverColor"))
			{
				getJson(dataG::data.gameData.buttonHoverColorString, "runButtonHoverColor");
				if (dataG::data.gameData.buttonHoverColorString != "") {
					std::sregex_token_iterator it_2{ dataG::data.gameData.buttonHoverColorString.begin(), dataG::data.gameData.buttonHoverColorString.end(), regex, -1 };
					std::vector<std::string> hoverColorValues{ it_2, {} };
					dataG::data.gameData.buttonHoverColor.r = stoi(hoverColorValues[0]);
					dataG::data.gameData.buttonHoverColor.g = stoi(hoverColorValues[1]);
					dataG::data.gameData.buttonHoverColor.b = stoi(hoverColorValues[2]);
					dataG::data.gameData.buttonHoverColor.a = stoi(hoverColorValues[3]);
				}
			}
			if (json.contains("launcherTheme"))
			{
				getJson(dataG::data.modData.themeName, "launcherTheme");
			}
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
		fPath = ".\\eopData\\config\\battlesCfg.json";
		setJson("enableAutoGeneration", dataG::data.battlesData.isGenerationNeeded);
		setJson("enableResultsTransfer", dataG::data.battlesData.isResultTransferNeeded);
		setJson("isPlannedRetreatRoute", dataG::data.battlesData.isPlannedRetreatRoute);
		writeJsonToFile(fPath, json);
		json.clear();

		// Save game config
		fPath = ".\\eopData\\config\\gameCfg.json";
		setJson("isDXVKEnabled", dataG::data.modulesData.isDXVKEnabled);
		setJson("isContextMenuNeeded", dataG::data.modulesData.isContextMenuNeeded);
		setJson("isTacticalMapViewerNeeded", dataG::data.modulesData.isTacticalMapViewerNeeded);
		setJson("isDeveloperModeNeeded", dataG::data.modulesData.isDeveloperModeNeeded);
		setJson("isBlockLaunchWithoutEop", dataG::data.gameData.isBlockLaunchWithoutEop);
		setJson("IsOverrideBattleCamera", dataG::data.gameData.IsOverrideBattleCamera);
		setJson("isDiscordRichPresenceEnabled", dataG::data.gameData.isDiscordRichPresenceEnabled);
		setJson("isFreecamIntegrationEnabled", dataG::data.gameData.freecamIntegration);
		writeJsonToFile(fPath, json);
		json.clear();

		// Save UI config
		fPath = ".\\eopData\\config\\uiCfg.json";
		setJson("useVanillaCfg", dataG::data.modData.useVanillaConfig);
		setJson("modCfgFile", dataG::data.modData.configName);
		setJson("useM2TWEOP", dataG::data.modData.useM2TWEOP);
		setJson("hideLauncher", dataG::data.modData.hideLauncherAtStart);
		setJson("playBackgroundMusic", dataG::data.audio.bkgMusic.isMusicNeeded);
		setJson("musicVolume", dataG::data.audio.bkgMusic.musicVolume);
		setJson("websiteLink", dataG::data.gameData.websiteLink);
		setJson("discordServerLink", dataG::data.gameData.discordServerLink);
		setJson("modTitle", dataG::data.gameData.modTitle);
		setJson("modVersion", dataG::data.gameData.modVersion);
		setJson("runButtonColor", dataG::data.gameData.buttonColorString);
		setJson("runButtonHoverColor", dataG::data.gameData.buttonHoverColorString);
		setJson("launcherTheme", dataG::data.modData.themeName);
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