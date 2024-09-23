#include "managerGHelpers.h"
#include "dataG.h"
#include "helpers.h"
namespace managerG
{
	void loadTextures()
	{
		dataG::data.images.logoAbout.path = "eopData/resources/images/logoAbout.png";

		helpers::loadTexture(dataG::data.images.logoAbout.path.c_str(),&dataG::data.images.logoAbout.image,
			&dataG::data.images.logoAbout.xSize, &dataG::data.images.logoAbout.ySize);

		dataG::data.images.logoBad.path = "eopData/resources/images/deathDance.png";
		helpers::loadTexture(dataG::data.images.logoBad.path.c_str(), &dataG::data.images.logoBad.image,
			&dataG::data.images.logoBad.xSize, &dataG::data.images.logoBad.ySize);
	}
	void loadFont(fontS* fnt, const wchar_t* path, const wchar_t* name, float fontSize)
	{
		std::string fntPath = helpers::wstringTostring(path);
		fnt->font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fntPath.c_str(), fontSize);
		if (fnt->font == nullptr)
		{
			ImFontConfig font_config;
			font_config.OversampleH = 1;
			font_config.OversampleV = 1;
			font_config.SizePixels = fontSize;
			font_config.PixelSnapH = 1;
			font_config.FontDataOwnedByAtlas = false;
			fnt->font = ImGui::GetIO().Fonts->AddFontDefault(&font_config);
		}
		fnt->path = path;
		fnt->name = name;
	}
	void loadFonts()
	{
		float fontsSize = 22.f;
		loadFont(&dataG::data.fonts.mainFont, L"eopData/resources/fonts/mainFont.ttf", L"mainFont", fontsSize);

		loadFont(&dataG::data.fonts.markdownH1Font, L"eopData/resources/fonts/markdownH1Font.ttf", L"markdownH1Font", 12.f);
		loadFont(&dataG::data.fonts.markdownH2Font, L"eopData/resources/fonts/markdownH2Font.ttf", L"markdownH2Font", 18.f);
		loadFont(&dataG::data.fonts.markdownH3Font, L"eopData/resources/fonts/markdownH3Font.ttf", L"markdownH3Font", 22.f);
	}
	bool isLibraryLoadable(const string& path)
	{
		bool retVal = true;
		HMODULE hmod = LoadLibraryA(path.c_str());
		if (hmod == NULL)
		{
			retVal = false;
		}
		else
		{
			FreeLibrary(hmod);
		}
		return retVal;
	}
};