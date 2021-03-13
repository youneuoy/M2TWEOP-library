#include "managerGHelpers.h"
#include "dataG.h"
namespace managerG
{
	void loadTextures()
	{
		dataG::data.images.logoAbout.path = "youneuoyData/images/logoAbout.png";
		helpers::loadTexture(&dataG::data.images.logoAbout);


	}
	void loadFont(fontS* fnt,const char*path,const char*name,float fontSize)
	{
		fnt->font=ImGui::GetIO().Fonts->AddFontFromFileTTF(path, fontSize);
		fnt->path = path;
		fnt->name = name;
	}
	void loadFonts()
	{
		float fontsSize = 22.f;
		loadFont(&dataG::data.fonts.mainFont,"youneuoyData/fonts/mainFont.ttf","mainFont", fontsSize);

		loadFont(&dataG::data.fonts.markdownH1Font,"youneuoyData/fonts/markdownH1Font.ttf","markdownH1Font", 12.f);
		loadFont(&dataG::data.fonts.markdownH2Font,"youneuoyData/fonts/markdownH2Font.ttf","markdownH2Font", 18.f);
		loadFont(&dataG::data.fonts.markdownH3Font,"youneuoyData/fonts/markdownH3Font.ttf","markdownH3Font", 22.f);

	}

};