#pragma once
#include "headersSTD.h"
#include "../M2TWEOP Common/m2tweopConstData.h"



class dataG
{
public:
	static struct dataGS
	{
		struct
		{
			bool isContextMenuNeeded = true;
			bool isDXVKEnabled = true;
			bool isTacticalMapViewerNeeded = true;
			bool isDeveloperModeNeeded = true;

		}modulesData;
		struct
		{
			bool isGenerationNeeded = false;
			bool isResultTransferNeeded = false;
			bool isPlannedRetreatRoute = false;
		}battlesData;


		struct
		{
			int gameMode = 0;

			string gamePath;
			string modFolder;

			// Freecam Integration
			string freecamExeName = "Freecam.exe";
			bool freecamIntegration = false;
			string freecamFolder = "<folder_with_freecam.exe>";

			string exeName;
			string gameArgs;

			// Discord Rich Presence
			bool isDiscordRichPresenceEnabled = true;

			// Customization Options
			string modTitle = "";
			string modVersion = "";
			string buttonColorString = "";
			string buttonHoverColorString = "";

			struct
			{
				int r = 0;
				int g = 0;
				int b = 0;
				int a = 0;
			}buttonColor;

			struct
			{
				int r = 0;
				int g = 0;
				int b = 0;
				int a = 0;
			}buttonHoverColor;


			int gameVer = 0;

			bool isBlockLaunchWithoutEop = false;
			bool IsOverrideBattleCamera = false;
		}gameData;

		struct
		{
			string configName;
			string themeName = "default.toml";
			bool useVanillaConfig = true;
			bool useM2TWEOP = false;
			bool hideLauncherAtStart = false;

			bool useCustomD3D = false;
		}modData;

		struct
		{
			fontS mainFont;

			fontS markdownH1Font;
			fontS markdownH2Font;
			fontS markdownH3Font;
		}fonts;
		std::vector<fontS*>staticFontsCollection =
		{
			&dataG::dataGS::fonts.mainFont,
			&dataG::dataGS::fonts.markdownH1Font,
			&dataG::dataGS::fonts.markdownH2Font,
			&dataG::dataGS::fonts.markdownH3Font,
		};
		screenS screen;
		struct
		{
			GLImage empty{ .path = "empty" };
			GLImage logoAbout;
			GLImage logoBad;
		}images;

		struct
		{
			struct
			{
				bool musicLoaded = false;
				sf::Music* music;

				bool isMusicNeeded = false;
				int musicVolume = 20;
			}bkgMusic;

		}audio;

		std::vector< GLImage*>staticImagesCollection =
		{
			&dataG::dataGS::images.empty,
			&dataG::dataGS::images.logoAbout,
			&dataG::dataGS::images.logoBad
		};


	}data;
};
