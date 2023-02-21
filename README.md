
<div align="center">
    <a href="https://www.twcenter.net/forums/forumdisplay.php?2296-M2TW-Engine-Overhaul-Project"><img src="https://cdn.discordapp.com/attachments/744306199075225627/819869043866468382/unknown.png" width="1920" alt="EOP" /></a>
    <br>
    <br>
  <p>
    <a href="https://discord.gg/Epqjm8u2WK"><img src="https://i.imgur.com/lWD9kdU.png" alt="Discord server" width="250" height="70"></a>
    <a href="https://www.twcenter.net/forums/forumdisplay.php?2296-M2TW-Engine-Overhaul-Project"><img src="https://i.imgur.com/rvo91ZR.png" alt="TWC" width="250" height="70"/></a>
    <a href="https://www.youtube.com/channel/UCMyHomaKeeGR4ZPGrBo9dYw"><img src="https://i.imgur.com/iwypXWd.png" alt="YouTube" width="250" height="70"/></a>
  </p>
</div>

[![Build Status](https://img.shields.io/github/downloads/youneuoy/M2TWEOP-library/total?style=for-the-badge)](https://github.com/youneuoy/M2TWEOP-library/releases)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?style=for-the-badge)](https://github.com/youneuoy/M2TWEOP-library/releases)
[![Build Status](https://img.shields.io/github/license/youneuoy/M2TWEOP-library?style=for-the-badge)](https://github.com/youneuoy/M2TWEOP-library/blob/master/LICENSE)
[![Build Status](https://img.shields.io/discord/713369537948549191?style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)


## What is M2TW Engine Overhaul Project?

M2TW Engine Overhaul Project is designed to expand the capabilities of the game *Medieval 2: Total War*.

## How does it work?

EOP uses C++ and Assembly to modify the game's code in-memory and exposes this to the end user via Lua scripts and the [EOP Lua Library](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html).

## Features

* Hugely expanded Lua scripting system that integrates seamlessly with existing campaign_script scripts, hot-reload, new custom console, debugger and much more
* Place .fbx models anywhere on the strategy or battle map with full animation and texture support
* Play custom sounds or music with support for WAV, OGG/Vorbis and FLAC
* Unlocks all vanilla console commands
* Create your own GUIs and overlays using the popular [ImgGui](https://github.com/ocornut/imgui)
* New custom EDU system that allows the creation and modification of new units without the need to restart the game
* Play out Hotseat battles online and transfer the results back to the campaign map
* Set your own limits for the number of religions, ancillaries, bodyguard units, building chains, max unit size
* Set the boundaries of the number of soldiers in units and the size of the battle map
* Fixed many engine bugs, crashes and oversights (e.g crashes when using berserkers in battle)
* Edit worldpkgdesc for any settlement on the fly without the need to restart the game
* Custom keybinding support
* Added ability to write your own add-ons in C++. The library exports many different functions.
* Many, many more

## Examples

### Removing engine limits
```lua
function onPluginLoad()
    M2TWEOP.unlockGameConsoleCommands();
    M2TWEOP.setAncillariesLimit(16);
    M2TWEOP.setMaxBgSize(100);
    M2TWEOP.setReligionsLimit(50);
    M2TWEOP.setBuildingChainLimit(40);
    M2TWEOP.setGuildCooldown(3);
end
```
### Playing sounds, displaying GUIs and spawning units
```lua
-- onGeneralAssaultsGeneral is a custom EOP event
function onGeneralAssaultsGeneral(attacker, defender)
        if(isNotNull(attacker)) then
            -- Check if the character is a Nazgul
            if(hasTrait(attacker, "NazgulRace")) then
                -- Play a sound
                playSound(nazgulScream);
                -- Display some text
                showWindow("The Nazgul have arrived.");
                -- Spawn a new unit
                spawnUnit('Gondor Infantry', 'Anorien' , 'Minas Tirith' ,3,4,1,1);
        end
    end
end
```
For a full list of EOP Events, see [here](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html)

## Getting Started
[![M2TWEOP-Tutorials](https://i.imgur.com/ByebdSI.png)](https://youtu.be/hUvbcz8i7Wk)

### Developers
* **youneuoy** - Project founder, Main Developer
* **Jojo00182** - Great code contributions, engine bugfixes, reversing structs and much more
* **Medik** - Creating documentation and website, setting up continuous integration
* **Fynn** - Testing, early adopter, great suggestions and many new events/documentation added
* **Callistonian** - Editing of documentation, Lua examples, feedback and new events

### Contributors
* **Edmond** - Project idea, first attempts to implement hotseats with online battles
* **Xamax** - Video tutorials, testing and many great suggestions
* **Erken** - Excellent IMGUI tutorials, Lua examples and feedback
* **DinarMayor** - Testing, great suggestions and examples

## Projects
* [M2TWEOP Library](https://github.com/youneuoy/M2TWEOP-library) - Main project (C++ and Assembly)
* [M2TWEOP LUA plugin](https://github.com/youneuoy/M2TWEOP-luaPlugin) - Allows you to write custom scripts in the Lua programming using M2TWEOP-library

### Compatibility

This program works with game versions 1.5 (disk version) and 1.52 (steam version).

### Usage
* Check the [F.A.Q](https://youneuoy.github.io/M2TWEOP-library/faq.html#questions-and-answers)

If you use an antivirus, add the program to the exceptions! There may be false positives.

### License
The project uses the [GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.html).

## Build

**Requirements**

M2TWEOP is developed in C++ and Assembly.

* Microsoft Visual Studio 2019 (https://community.chocolatey.org/packages/visualstudio2019community)
* DirectX SDK 2009 (https://community.chocolatey.org/packages/directx-sdk)

Use Choco (https://chocolatey.org/install) for easy install.

**How to build**
1. Clone or download the project and extract it to a folder
2. Run Developer Powershell for Visual Studio 2019 in Admin mode
3. Navigate to where you have cloned/extracted the project
4. Run buildEOP.ps1

You can get support for building/developing on our [Discord](https://discord.gg/Epqjm8u2WK) server.

## Support the project

It would be nice if all interested people supported the project in an accessible way (for example, you can test various changes, participate in planning new game mechanics or discuss them, sponsor development with money, etc.).
* Come join the our [Discord](https://discord.gg/Epqjm8u2WK) server if you have questions or suggestions.

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
* **v.2+**
https://github.com/youneuoy/M2TWEOP-library/releases
* **v.2.0:**
A huge number of fixes and optimizations. Lots of new features in the lua plugin, as well as features exported for use in other plugins.
* **v.2.0 test2:**
Changed initializing way
Added an option to hide the launcher at startup
Added a window that will appear in case of some probable problems with the initialization of the game. The launcher will not freeze when launching the game.
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
