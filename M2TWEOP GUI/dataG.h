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
			string exeName;
			string gameArgs;

			int gameVer = 0;
		}gameData;

		struct
		{
			string configName;
			bool useVanillaConfig = false;

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
			d3dImage empty{ .path = "empty" };
			d3dImage logoAbout;
		}images;
		std::vector< d3dImage*>staticImagesCollection =
		{
			&dataG::dataGS::images.empty,
			&dataG::dataGS::images.logoAbout
		};

		struct
		{
			LPDIRECT3D9              g_pD3D = NULL;
			LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
			D3DPRESENT_PARAMETERS    g_d3dpp = {};
		}d3d;
	}data;
};
