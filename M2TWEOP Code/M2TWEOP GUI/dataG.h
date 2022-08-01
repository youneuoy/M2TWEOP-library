#pragma once
#include "headersSTD.h"
#include "../M2TWEOP library/m2tweopConstData.h"



class dataG
{
public:
	static struct dataGS
	{
		struct
		{
			bool isGenerationNeeded = false;
			bool isResultTransferNeeded = false;
		}battlesData;


		struct
		{
			int gameMode = 0;

			string gamePath;
			string modFolder;

			string exeName;
			string gameArgs;

			// Customization Options
			string modTitle;
			ImU32 buttonColor;

			int gameVer = 0;

			bool isBlockLaunchWithoutEop = false;
		}gameData;

		struct
		{
			string configName;
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
