#include "managerGHelpers.h"
#include "dataG.h"
#include "helpers.h"
namespace managerG
{
	void loadTextures()
	{
		dataG::data.images.logoAbout.path = "eopData/images/logoAbout.png";

		helpers::loadTexture(dataG::data.images.logoAbout.path.c_str(),&dataG::data.images.logoAbout.image,
			&dataG::data.images.logoAbout.xSize, &dataG::data.images.logoAbout.ySize);
	}
	void loadFont(fontS* fnt, const char* path, const char* name, float fontSize)
	{
		fnt->font = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, fontSize);
		fnt->path = path;
		fnt->name = name;
	}
	void loadFonts()
	{
		float fontsSize = 22.f;
		loadFont(&dataG::data.fonts.mainFont, "eopData/fonts/mainFont.ttf", "mainFont", fontsSize);

		loadFont(&dataG::data.fonts.markdownH1Font, "eopData/fonts/markdownH1Font.ttf", "markdownH1Font", 12.f);
		loadFont(&dataG::data.fonts.markdownH2Font, "eopData/fonts/markdownH2Font.ttf", "markdownH2Font", 18.f);
		loadFont(&dataG::data.fonts.markdownH3Font, "eopData/fonts/markdownH3Font.ttf", "markdownH3Font", 22.f);
	}
};