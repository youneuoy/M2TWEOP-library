![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
[![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**
- added event that fires after game loads to main menu
- added onAiTurn event as an insertion point for campaign ai altering code
- Fixed eop units as mercenaries causing crashes when gifting or rebelling settlements

Breaking changes to watch out for:
- regionNumber in settlement struct -> regionID
- event function has different parameters
- lot of info like different public order and growth types inside settlement struct moved to their own struct
- many renamed fields in edu entry struct
- USE THE LUA DOCS TO FIND ISSUES WITH OBSOLOTE NAMINGS
   
### **Launcher**
- Enabling Discord Rich Presence no longer opens an extra window, it now seamlessly toggles on and off. You just need to use the launcher if you want to enable it.
- M2TWEOP GUI automatically selects the cfg file if it detects one

### **Library**
- Added support for [DXVK project](https://github.com/doitsujin/dxvk) (Vulcan wrapper over game d3d9 graphics). For use unpack dxvk d3d9.dll to game folder and rename it to d3d9_vk.dll  - *youneuoy*

### **Lua Plugin**
- uimanager struct added to game data all where you can get selected units, character, fort, settlement
- check if unit has an attribute (from edu attributes line)
- get and set military access between factions
- get and set faction standing between factions
- college of cardinals struct added
- global array of forts, ports and watchtowers added to campaign struct
- get settlement by name function added to campaign struct
- get religion name added
- get and set guard mode, fire at will, skirmish of units in battle
- expansion to unit struct, including many in battle properties
- added unit position data struct to get some more unit information during battles
- expansion of campaign struct with difficulty per faction, crusade/jihad structs, fog of war and more
- added most game options from the game cfg file to lua
- expansion to character struct with new fields like inEnemyZOC, numTurnsIdle, timeInRegion and much more
- expansion to faction struct with new fields like battles won/lost, neighbour regions, faction economy, faction rankings, interfaction battle statistics
- added win conditions like regions to hold and factions to outlive
- added faction economy with all the information from the financial panel
- expanded sm factions struct
- expanded settlement struct with capabilities, recruit pools, tax level, base fertility and more
- expanded army struct with siege equipment and more
- added ability to recruit EOPDU units from buildings
- added ability to add conditions to recruit pools in the format of edb
- added ability to use any game condition in lua with M2TWEOP.condition, which can use event context through the eventData
- expanded tile struct with easy way to locate if it contains characters, settlements, resources and much more
- expanded region struct with roads, sea trade, harvest success and more
- added many enums to the luaDocs file for easier coding
- fixed and expanded battleArmy and battleUnit structs
- added ability to change some hardcoded campaign difficulty modifiers
- expanded edu and renamed many fields for consistency
- added ai structs like the long term goal director, ai production controller and ai personality with changeable values
- added much of the information the campaign ai uses like free strength, military balance and much more
- All game events improved performance and added to the game, there is a change in how game events provide their parameters:
In the below example the eventData parameter contains 4 fields with information, which you can see in the documentation under "Exports"

```lua
---A settlement is being processed for the start of its faction's turn.
-- Exports: settlement, faction, regionID, religion
---@param eventData eventTrigger 
function onSettlementTurnStart(eventData)
    local faction = eventData.faction
    local religion = eventData.religion
end 
```

![image](https://github.com/youneuoy/M2TWEOP-library/assets/22448079/99194af1-1ca3-44b9-9382-a5dd0a080b3b)


<!-- ### **ImGUI** -->

### **Documentation**


![image](https://github.com/youneuoy/M2TWEOP-library/assets/22448079/ea6c89c5-8d30-4284-8288-3bafafc56361)


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
