#pragma once
#include "helpers.h"
#include "imgui_md.h"
#include "Shellapi.h"
namespace aboutUI
{
	//Fonts and images (ImTextureID) must be loaded in other place,
	//see https://github.com/ocornut/imgui/blob/master/docs/FONTS.md

	struct
	{
		ImFont* g_font_regular = nullptr;
		ImFont* g_font_bold = nullptr;
		ImFont* g_font_bold_large = nullptr;
	}fontsS;

	void setFonts()
	{
		fontsS.g_font_regular = helpers::findFont("markH1");
		fontsS.g_font_bold = helpers::findFont("markH2");
		fontsS.g_font_bold_large = helpers::findFont("markH3");
	}
	extern ImTextureID g_texture1;

	struct my_markdown : public imgui_md
	{


		ImFont* get_font() const override
		{
			if (m_is_table_header) {
				return fontsS.g_font_bold;
			}

			switch (m_hlevel)
			{
			case 0:
				return m_is_strong ? fontsS.g_font_bold : fontsS.g_font_regular;
			case 1:
				return fontsS.g_font_bold_large;
			default:
				return fontsS.g_font_bold;
			}
		};


		void open_url() const override
		{
			//platform dependent code
			ShellExecuteA(NULL, "open", m_href.c_str(), NULL, NULL, SW_SHOWNORMAL);

		}


		bool get_image(image_info& nfo) const override
		{

			GLImage* imgFinded = helpers::findImage(m_href.c_str(), strlen(m_href.c_str()));


			nfo.texture_id = (void*)(intptr_t)imgFinded->image;
			nfo.size = ImVec2((float)imgFinded->xSize, (float)imgFinded->ySize);
						nfo.uv0 = { 0,0 };
			nfo.uv1 = { 1,1 };
			nfo.col_tint = { 1,1,1,1 };
			nfo.col_border = { 0,0,0,0 };


			return true;
		}

		void html_div(const std::string& dclass, bool e) override
		{
			if (dclass == "red") {
				if (e) {
					m_table_border = false;
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
				}
				else {
					ImGui::PopStyleColor();
					m_table_border = true;
				}
			}
		}
	};


	//call this function to render your markdown
	void markdown(const char* str, const char* str_end)
	{
		static my_markdown s_printer;
		s_printer.print(str, str_end);
	}



	void MarkdownExample()
	{
		const std::string markdownText = R"(
![logo](eopData/images/logoAbout.png)
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
* Fixed crash when using berserkers in battle
* Added ability to write your own add-ons in C ++. The library exports many different functions.
* and many many more

### Creators
* **youneuoy** - project founder, main developer
* **Jojo00182** - did a lot of great stuff and much more to come
  
### Contributors
* **Edmond** - project idea, first attempts to implement hotseats with online battles
* **Xamax** - lots of ideas and testing
* **Erken** - lots of ideas and testing
* **DinarMayor** - ideas and testing
* **Medik** - help in creating documentation

## Project composition
* [M2TWEOP Library](https://github.com/youneuoy/M2TWEOP-library) - main project
* [M2TWEOP LUA plugin](https://github.com/youneuoy/M2TWEOP-luaPlugin) - a plugin that adds the ability to script in the lua language (from game logic to extending the interface), add new console to the game and much more.

### Compatibility

This program works with game versions 1.5 (disk version) and 1.52 (steam version).

### Usage

All program settings are set in the config files and scripts and are applied when the game starts. You can read more about this on the [forums](https://www.twcenter.net/forums/showthread.php?803575-Download-links-important-information-and-instructions-for-the-program-in-pictures).
All changes made work correctly in an ongoing campaign, nothing breaks during updates, it’s enough just not to activate new features.

If you use an antivirus, add the program to the exceptions! There may be false positives.

### License 
The project uses the [GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.html).

## Build
M2TWEOP is developed in C++ and Assembly. You can get support for building/developing on the our [Discord](https://discord.gg/Epqjm8u2WK) server.

## Support the project

It would be nice if all interested people supported the project in an accessible way (for example, you can test various changes, participate in planning new game mechanics or discuss them, sponsor development with money, etc.).
* Come join the our [Discord](https://discord.gg/Epqjm8u2WK) server if you have questions or suggestions.
* [Patreon](https://www.patreon.com/m2tweop)


With the help of this program, it will probably be possible someday to give modders the opportunity to change the MTW2 gameplay beyond recognition.


## Disclaimer

Please do not include the program in your modifications without my permission (and I will allow, if you ask). I just want to have some usage statistics.

## Acknowledgments
* To all users participating in the discussion and improvement of the project.
* Users and administration of the forum https://gamehacklab.ru/. Without them, I would not have learned to do this kind of thing.
* d3d9.dll wrapper based on this repository: https://github.com/elishacloud/DirectX-Wrappers
* Used GUI library: https://github.com/ocornut/imgui
* Used LUA binding: https://github.com/ThePhD/sol2
* ImGui lua binding is based on this repository:
https://github.com/MSeys/sol2_ImGui_Bindings

## Version History
* **v.2.0 test1:**
Added lua plugin and a huge number of functions for it.
Fixed many crashes and issues.
Optimization.
* **v.1.18:**
Completion of the diplomacy system, adding the ability to check vassal relationships using scripts
The ability to make forts part of the eternal, and part - temporary
The ability of change the heir
The ability to automatically create historical battles
The ability to automatically edit armies on a strategic map in accordance with the results of online battle
Fixed incorrect display of the age of the characters on the disk version of the game
Now you can run the program and the game using the .bat file (not all of the new launch methods worked correctly)
* **v.1.17:**
fixed crash if the character has no name in the game
added ability to partially disable logging
added the ability to use the console command "change faction" in a single player game
improved scripting system
added ability to make factions vassals of other factions
Added the ability to strategically view the battle map (Z key during the battle)
* **v.1.16:**
added launch of the game from the program
added ability to replace legio prefix for units
improved scripting system
added ability to change characters labels
* **v.1.15:**
fixed several bugs
added the ability to upgrade guard of characters
* **v.1.14:**
Added the ability to set the cost of siege equipment (rams, ladders, siege towers).
* **v.1.13:**
Fixed crash when using berserkers in the game.
Added the ability to quickly modify an additional file.
* **v.1.12:**
Improved editing descr_buildings (extended range of possible values)
Added the ability to make immortal characters and change the display settings for age.
* **v.1.11:**
Added several patches that apply to the game when the program starts, as well as the ability to configure them in the config file.
* **v.1.10:**
parameters from the game were not always correctly read, fixed.
* **v.1.09:**
improved generation of descr_battle.txt. Added the conclusion of the battle season (winter or summer) and the time of the battle, as well as the portrait, model and special ability of the general.
* **v.1.08:**
improved generation of descr_battle.txt. Now coordinates of deployment of armies are displayed.
* **v.1.07:**
improved generation of descr_battle.txt.
* **v.1.06:**
now, when generating descr_battle.txt, a list of fractions, the year of the battle and a little more standard information is written.
* **v.1.05:**
Added display of coordinates of each of the fighting armies when generating descr_battle.txt
* **v1.04:**
The system of auto-calculation of losses has been changed, the units of the generals are removed at the same time, but the generals themselves can be considered live for a certain number of moves.
This is done to avoid crashes.
It is recommended that when manually killing generals, this is always done with a daw "mark if there is only one general in the stack." Otherwise, departures during the campaign are possible.
* **v1.03:**
Now the program only works with the Steam version of MTW2.
'Temporarily' ;) cut out the Russian language.
* **v1.02:**
Fixed crash during loss auto transfer.
Fixed auto-accrual of losses, they could not be accrued to all units.
* **v1.01:**
Added general traits (including hidden traits) when generating descr_battle.txt.
Added the ability to automatically transfer losses after an online battle.
Improving the stability of the program.
* **v1.0:**
First release.
)";
		const char* end_ptr = markdownText.c_str() + markdownText.size();
		markdown(markdownText.c_str(), end_ptr);
	}
};