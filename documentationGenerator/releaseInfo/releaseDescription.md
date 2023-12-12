![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
[![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes - 3.0.2**
These are fixes released in the 3.0.2 version

### ImGUI
- Restored default styles found in previous versions for in-game ImGUI elements

## **Release Notes - 3.0.1**
These are fixes released in the 3.0.1 version

### **Library**
- When enable planned retreat routes, also overwrite all retreats(even AI ones).
- Fixed add building pools for disk version.
- Fixed launching the game with installed M2TWEOP.
- Fixed save files with non-english letters.
- Fixed hotseat settings.

### **Lua Plugin**
- Fixed reading condition requirements for pools.
- Fixed getting stratCharacter from eventData.

## **Release Notes - 3.0.0**

### **Launcher**
- Enabling Discord Rich Presence no longer opens an extra window, it now seamlessly toggles on and off. You just need to use the launcher if you want to enable it. - *Medik*
- Added a new default theme for the launcher, console and all other EOP GUI elements. - *Medik*
- Added the ability to configure ImGUI elements (Launcher, Console, ImGUI Lua etc.) via  a single [toml](https://toml.io/en/) file. See [configuring the EOP launcher](https://youneuoy.github.io/M2TWEOP-library/M2TWEOP_LAUNCHER_SETTINGS.html) tutorial for more information - *Medik*
- Added an executable GUI that allows you to create the [toml](https://toml.io/en/) files with a GUI tool. Thank you [ImThemes](https://github.com/Patitotective/ImThemes). - *Medik*
- Added tooltips that explain what all the EOP launcher settings do - *Medik*
- M2TWEOP GUI automatically selects the mod .cfg file if it detects one - *Fynn*
- Added an uninstaller batch file that deletes all EOP files in the mod folder- *Medik*

![](https://i.imgur.com/Pl02p1W.png)
![](https://i.imgur.com/h8UlYMT.png)

### **Library**
- Added support for [DXVK project](https://github.com/doitsujin/dxvk) (Vulcan wrapper over game d3d9 graphics). To try this out simply enable the option in the launcher. For some hardware, this can massively improve performance  - *youneuoy and Medik*
- "Show planned retreat route" no longer shows on the Stratmap Context Menu if the option is disabled in the launcher - *Medik*

### **Lua Plugin**

**Note: The following changes were all made by Fynn**

- Added ability to recruit EOPEDU units from buildings
  - With this change, the unit cap has now been broken in it's entirety
- Added event that fires after game loads to main menu
- Added `onAiTurn` event as an insertion point for campaign ai altering code
- Fixed EOP EDU units as mercenaries causing crashes when gifting or rebelling settlements
- uimanager struct added to game data all where you can get selected units, character, fort, settlement
- Check if unit has an attribute (from edu attributes line)
- Get and set military access between factions
- Get and set faction standing between factions
- College of cardinals struct added
- Global array of forts, ports and watchtowers added to campaign struct
- Get settlement by name function added to campaign struct
- Get religion name added
- Get and set guard mode, fire at will, skirmish of units in battle
- Expansion to unit struct, including many in battle properties
- Added unit position data struct to get some more unit information during battles
- Expansion of campaign struct with difficulty per faction, crusade/jihad structs, fog of war and more
- Added most game options from the game cfg file to lua
- Expansion to character struct with new fields like inEnemyZOC, numTurnsIdle, timeInRegion and much more
- Expansion to faction struct with new fields like battles won/lost, neighbour regions, faction economy, faction rankings, interfaction battle statistics
- Added win conditions like regions to hold and factions to outlive
- Added faction economy with all the information from the financial panel
- Expanded sm factions struct
- Expanded settlement struct with capabilities, recruit pools, tax level, base fertility and more
- Expanded army struct with siege equipment and more
- Added ability to add conditions to recruit pools in the format of edb
- Added ability to use any game condition in lua with M2TWEOP.condition, which can use event context through the eventData
- Expanded tile struct with easy way to locate if it contains characters, settlements, resources and much more
- Expanded region struct with roads, sea trade, harvest success and more
- Added many enums to the luaDocs file for easier coding
- Fixed and expanded battleArmy and battleUnit structs
- Added ability to change some hardcoded campaign difficulty modifiers
- Expanded edu and renamed many fields for consistency
- Added ai structs like the long term goal director, ai production controller and ai personality with changeable values
- Added much of the information the campaign ai uses like free strength, military balance and much more
- All game events improved performance and added to the game, there is a change in how game events provide their parameters (See more below)

### **Breaking Changes**
- `regionNumber` in settlement struct -> `regionID`
- Event function has different parameters (see more below)
- Lot of info like different public order and growth types inside settlement struct moved to their own struct
- Many renamed fields in edu entry struct
- `dipNum` in factionStruct -> `factionID` (dipNum still works for compatibility but it is not in docs, you should change it where you use it)

**Note:** Please use the Lua docs to find problems with obsolete namings
**Note:** `eopLuaHelpers.lua` has not been updated in accordance with the breaking changes.

### New Event Structure Example
In the below example the eventData parameter contains 4 fields with information, which you can see in the documentation under "Exports":

```lua
---A settlement is being processed for the start of its faction's turn.
-- Exports: settlement, faction, regionID, religion
---@param eventData eventTrigger 
function onSettlementTurnStart(eventData)
    local faction = eventData.faction
    local religion = eventData.religion
end 
```
<!-- ### **ImGUI** -->

<!-- ### **Documentation** -->

### **Links**

- [Website](https://youneuoy.github.io/M2TWEOP-library/)
- [FAQ](https://youneuoy.github.io/M2TWEOP-library/faq.html)
- [Lua Documentation](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html)
- [ImGUI Documentation](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/extra/readme_imgui.md.html)
- [Discord](https://discord.gg/Epqjm8u2WK)
- [Installation Video](https://youtu.be/caOiB0NaGGI?t=67)
- [M2TWEOP Tutorials](https://www.youtube.com/playlist?list=PLi6V3nVH22N7ZfjfOuivGKHnNRAlBaTQd)
- [M2TWEOP Features](https://www.youtube.com/playlist?list=PLi6V3nVH22N6R7IGupVDwfyiPm6-d6rlU)

### **Download**

<a id="download"></a>
