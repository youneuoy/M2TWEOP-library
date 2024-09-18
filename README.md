
<div align="center">
    <a href="https://www.twcenter.net/forums/forumdisplay.php?2296-M2TW-Engine-Overhaul-Project"><img src="https://youneuoy.github.io/M2TWEOP-library/_static/M2TWEOP.png" width="1920" alt="EOP" /></a>
    <br>
    <br>
</div>

<div align="center">

[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?style=for-the-badge&color=3d1d00)](https://github.com/youneuoy/M2TWEOP-library/releases)
[![Build Status](https://img.shields.io/github/downloads/youneuoy/M2TWEOP-library/total?style=for-the-badge&color=a87300)](https://github.com/youneuoy/M2TWEOP-library/releases)
[![Build Status](https://img.shields.io/discord/713369537948549191?style=for-the-badge&label=Discord&color=b33102)](https://discord.gg/Epqjm8u2WK)

</div>

<div align="center">

## What is M2TWEOP?

The M2TW Engine Overhaul Project (M2TWEOP) is designed to expand the capabilities of the strategy game *Medieval 2: Total War*. EOP uses C++ and Assembly to modify the game's code in-memory and exposes this to the end user via a Lua API.
</div>

## Features

* [Lua scripting system with 1000+ functions](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html) 
  * Full documentation with code examples and VSCode Intellisense
  * Seamless integration with existing campaign_script scripts
  * Hot-reload
  * Custom console
  * Debugging support (VSCode)
  * [ImgGui bindings](https://github.com/ocornut/imgui)
* Removes engine limitations for
  * Number of unit types and unit sizes
  * Number of religions and cultures
  * Number of buildings and their chains
  * Number of ancillaries
  * Number of custom settlements
  * Number of custom cas models (characters and settlements)
* Unlocks all vanilla console commands
* Fixes many engine bugs, crashes and oversights such as
  * Berserker units instantly crashing
  * Elephant units breaking sieges
  * Elephant cannons unable to attack gates
* Draw cas models at any scale, anywhere on the campaign map
* Fully customizable and configurable launcher
* Discord Rich Presence support for 30+ mods
* Rome: Total War style [Tactical Map Viewer](https://www.youtube.com/watch?v=RrGi4zxr7bU)
* Play out Hotseat battles online and transfer the results back to the campaign map
* Custom keybinding support
* Play custom sounds or music with support for WAV, OGG/Vorbis and FLAC
* Supports both disk (1.5) and Steam (1.52) versions

## Contributions
This project wouldn't be possible without all the amazing contributions from the Medieval 2 modding community. Thank you to all of our contributors so far!
<div align=center>
  <a href="https://github.com/youneuoy/M2TWEOP-library/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=youneuoy/M2TWEOP-library" />
  </a>
  </p>
</div>

## Lua Examples

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
function onGeneralAssaultsGeneral(eventData)
    local atatcker, defender = eventData.attacker, eventData.defender
    -- Check if the character is a Nazgul
    if (hasTrait(attacker, "NazgulRace")) then
        -- Play a sound
        playSound(nazgulScream)
        -- Display some text
        showWindow("The Nazgul have arrived.")
        -- Spawn a new unit
        spawnUnit("Gondor Infantry", "Anorien", "Minas Tirith", 3, 4, 1, 1)
    end
end
```
For a full list of EOP Events, see [here](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html)

## Getting Started with M2TWEOP Lua Scripting
[![M2TWEOP-Tutorials](https://i.imgur.com/ByebdSI.png)](https://youtu.be/hUvbcz8i7Wk)

## Building Locally

**Requirements**

* [Microsoft Visual Studio 2022](https://community.chocolatey.org/packages/visualstudio2022community)
* [DirectX SDK 2009](https://community.chocolatey.org/packages/directx-sdk)

Use [Choco](https://chocolatey.org/install) for easy installation of the DirectX SDK.

**How to build**
1. Clone the project
2. Run Developer Powershell for Visual Studio 2022 in Admin mode
3. Navigate to where you have cloned/extracted the project
4. Run a build script like

```powershell
# Default build script for full release
.\buildEOP.ps1 -modFolder "E:\Steam\steamapps\common\Medieval II Total War\mods\Tsardoms-2.2"

# Release build script without config or luaPluginScript.lua
.\buildEOPDev_release.ps1 -modFolder "E:\Steam\steamapps\common\Medieval II Total War\mods\Tsardoms-2.2"

# Debug build script without config or luaPluginScript.lua
.\buildEOPDev_debug.ps1 -modFolder "E:\Steam\steamapps\common\Medieval II Total War\mods\Tsardoms-2.2"
```

You can get support for building/developing on our [Discord](https://discord.gg/Epqjm8u2WK) server.

## Disclaimer

* Do not include the project in a mod without prior permission (this is simply to kepe track of usage)
* Do not release your mod with an unreleased version of M2TWEOP

## Acknowledgments
* To all users participating in the discussion and improvement of the project.
* Users and administration of the forum <https://gamehacklab.ru/>. Without them, I would not have learned to do this kind of thing.
* d3d9.dll wrapper based on this repository: <https://github.com/elishacloud/DirectX-Wrappers>
* Used GUI library: <https://github.com/ocornut/imgui>
* Used LUA binding: <https://github.com/ThePhD/sol2>
* ImGui lua binding is based on this repository:
<https://github.com/MSeys/sol2_ImGui_Bindings>

## Version History
* **v.2+**
<https://github.com/youneuoy/M2TWEOP-library/releases>
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

### License
The project uses the [GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.html).