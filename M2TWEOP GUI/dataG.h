#pragma once
#include "headersSTD.h"
class dataG
{
public:
	static struct dataGS
	{
		struct
		{
			int gameMode = 0;

			string gamePath;
			string modFolder;

			string exeName;
			string gameArgs;

			int gameVer = 0;
		}gameData;

		struct
		{
			string configName;
			bool useVanillaConfig = true;
			bool useM2TWEOP = false;

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
		}images;
		std::vector< GLImage*>staticImagesCollection =
		{
			&dataG::dataGS::images.empty,
			&dataG::dataGS::images.logoAbout
		};


	}data;
};
