#include "managerG.h"
#include "helpers.h"

#include "managerGHelpers.h"
namespace managerG
{
	void loadSettings();

	void init()
	{
		helpers::updateMetrics();
		loadTextures();
		loadFonts();
		loadSettings();
	}


	void loadSettings()
	{
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

	void saveSettings()
	{
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