#pragma once
#include "imgui_markdown.h"
#include "helpers.h"
namespace aboutUI
{
#include "ImGui.h"                // https://github.com/ocornut/imgui
#include "imgui_markdown.h"       // https://github.com/juliettef/imgui_markdown
#include "IconsFontAwesome5.h"    // https://github.com/juliettef/IconFontCppHeaders

	// Following includes for Windows LinkCallback
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Shellapi.h"
#include <string>
	void LinkCallback(ImGui::MarkdownLinkCallbackData data_);
	inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_);

	static ImGui::MarkdownConfig mdConfig;

	void LinkCallback(ImGui::MarkdownLinkCallbackData data_)
	{
		std::string url(data_.link, data_.linkLength);
		if (!data_.isImage)
		{
			ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
	}

	inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_)
	{
		// In your application you would load an image based on data_ input. Here we just use the imgui font texture.
	   // ImTextureID image = ImGui::GetIO().Fonts->TexID;
		d3dImage* imgFinded = helpers::findImage(data_.link, data_.linkLength);

		ImTextureID image = imgFinded->image;

		// > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image, ImVec2( 40.0f, 20.0f ) };
		ImGui::MarkdownImageData imageData;
		imageData.isValid = true;
		imageData.useLinkCallback = false;
		imageData.user_texture_id = image;
		imageData.size = ImVec2((float)imgFinded->xSize, (float)imgFinded->ySize);

		// For image resize when available size.x > image width, add
		ImVec2 const contentSize = ImGui::GetContentRegionAvail();
		if (imageData.size.x > contentSize.x)
		{
			float const ratio = imageData.size.y / imageData.size.x;
			imageData.size.x = contentSize.x;
			imageData.size.y = contentSize.x * ratio;
		}

		return imageData;
	}

	void ExampleMarkdownFormatCallback(const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_)
	{
		// Call the default first so any settings can be overwritten by our implementation.
		// Alternatively could be called or not called in a switch statement on a case by case basis.
		// See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
		ImGui::defaultMarkdownFormatCallback(markdownFormatInfo_, start_);

		switch (markdownFormatInfo_.type)
		{
			// example: change the colour of heading level 2
		case ImGui::MarkdownFormatType::HEADING:
		{
			if (markdownFormatInfo_.level == 2)
			{
				if (start_)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
				}
				else
				{
					ImGui::PopStyleColor();
				}
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void Markdown(const std::string& markdown_)
	{
		// You can make your own Markdown function with your prefered string container and markdown config.
		// > C++14 can use ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, ImageCallback, ICON_FA_LINK, { { H1, true }, { H2, true }, { H3, false } }, NULL };
		mdConfig.linkCallback = LinkCallback;
		mdConfig.tooltipCallback = NULL;
		mdConfig.imageCallback = ImageCallback;
		mdConfig.linkIcon = ICON_FA_LINK;
		mdConfig.headingFormats[0] = { helpers::findFont("markH1"), true };
		mdConfig.headingFormats[1] = { helpers::findFont("markH2"), true };
		mdConfig.headingFormats[2] = { helpers::findFont("markH3"), false };
		mdConfig.userData = NULL;
		mdConfig.formatCallback = ExampleMarkdownFormatCallback;
		ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
	}

	void MarkdownExample()
	{
		const std::string markdownText = R"(
![logo](youneuoyData/images/logoAbout.png)
## What is M2TW Engine Overhaul Project?

M2TW Engine Overhaul Project is designed to expand the capabilities of the game *Medieval 2: Total War*.

The functionality of the program is divided into two parts

1. Making changes to the game code when starting MTW2 (changing various engine limits, finer tuning, etc.)
2. Manipulations with the game after its launch.

### Features

* Hugely expanded scripting system
* Transfer your battles from the campaign map to online. This allows you to play out Hotseat battles online and transfer the results back to the campaign map.
* Allows you to set your own limits for the number of religions, ancillaries, block resizing units
* Set the boundaries of the number of soldiers in units, the size of the battle map and the cost of siege equipment (rams, ladders, siege towers)
* Make any characters immortal and change the way their age is displayed on the screen
* Set your own prefix for units with the *legio* parameter
* Fixed crash when using berserkers in battle
* Added the ability to repeatedly upgrade the guard of characters.

## Versions
* [M2TWEOP Library](https://github.com/youneuoy/M2TWEOP-library)
* [M2TWEOP Plugins](https://github.com/youneuoy/M2TWEOP-luaPlugin)

### Compatibility

This program works with game versions 1.5 (disk version) and 1.52 (steam version).

### Usage

All program settings are set in the config files and are applied when the game starts. You can read more about this on the [forums](https://www.twcenter.net/forums/showthread.php?803575-Download-links-important-information-and-instructions-for-the-program-in-pictures).
All changes made work correctly in an ongoing campaign, nothing breaks during updates, it’s enough just not to activate new features.

If you use an antivirus, add the program to the exceptions! There may be false positives.

## Build
M2TWEOP is developed in C++ and Assembly. You can get support for building/developing on the [Discord](https://discord.gg/AfFNeQhf).

## Documentation
Detailed documentation on all features can be found on the [Total War Center Forums](https://www.twcenter.net/forums/showthread.php?803575-Download-links-important-information-and-instructions-for-the-program-in-pictures).

You can find videos on various features on [YouTube](https://www.youtube.com/channel/UCMyHomaKeeGR4ZPGrBo9dYw).

Come join the [Discord](https://discord.gg/AfFNeQhf) if you have questions or suggestions.

Also see - **M2TW Ultimate Docudemons 4.0.xls** (included in this repo) for more information on documentation for specific functions.

### Support the project

* [Patreon](https://www.patreon.com/m2tweop)

It would be nice if all interested people supported the project in an accessible way (for example, you can test various changes, participate in planning new game mechanics or discuss them, sponsor development with money, etc.).

With the help of this program, it will probably be possible someday to give modders the opportunity to change the MTW2 gameplay beyond recognition.

### Disclaimer

Please do not include the program in your modifications without my permission (and I will allow, if you ask). I just want to have some usage statistics.

)";
		Markdown(markdownText);
	}
};
