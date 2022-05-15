#pragma once
#include "headersSTD.h"
class dataG
{
public:
	static struct dataGS
	{


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
			GLImage empty{  "empty" };
			GLImage logoAbout;
			GLImage logoBad;
		}images;
		std::vector< GLImage*>staticImagesCollection =
		{
			&dataG::dataGS::images.empty,
			&dataG::dataGS::images.logoAbout,
			&dataG::dataGS::images.logoBad
		};


	}data;
};
