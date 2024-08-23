![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
[![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**
***NOTE : NOT SAVEGAME COMPATIBLE!!!!***

#### **M2TWEOP-3-3-1**
- Fixed custom mod building UI in `data/ui/generic` not being read correctly when EOP was installed - *Medik*

<!-- ### **Launcher** -->

#### **M2TWEOP-3-3-0**

(all changes by Fynn)
### **Library**
- Increased culture limit to 31. NOTE: the 8th culture (index 7) is NOT valid and you need to place a dummy culture there you won't actually use.
- Do not use cultures over limit in offmap_models file, event_bodies and sound files.
- Added ability to use EOP units in custom battle.
- Added ability to use EOP units in game files like export_descr_buildings, descr_strat, descr_rebel_factions etc
- Can spawn EOP units by name instead of index. (Both this and using in game files only works for file-added EOP units)
- EOP unit type names need to be unique now.
- Fixed maintain option for siegeSettlement/siegeFort.
- Fixed "random_name" in spawnArmy
- Fixed armies not attacking in sieges with Elephants in their army.
- Fixed elephant artillery not attacking gates.
- Fixed crashes relating to EOP mercenaries.


### **Lua Plugin**
- [Added new `mapImage` struct](https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html#mapImage)
  - Enables users to render their own versions of the campaign strategy map
  - Includes options for filling certain tiles and regions with color and blurring options for smoothing out jagged lines
- Added all the unit actions that were possible to execute in campaign_script, and expanded the unit struct.
- Added get unit by label
- Added 2 extra unit functions: Attack building and collect engine.
- Added unit groups, getting group by label, defining groups, and every script function relating to groups.
- Added siegeEngine struct and ability to get all engines in battle or get the engine a unit is carrying.
- Fixed checking if unit/eduEntry has an attribute.
- Added creating watchtowers.
- Added all character stratmap actions, diplomacy, spying, assassination, blockading, sabotage etc
- Added ability to split armies, disembark and embark on ships.
- Added ability to merge by movement instead of by teleportation.
- Added new functions to switch a characters or fort faction, and added option to convert the settlement garrison when switching a settlement's faction.
- Added loading saves.
- Added function to log something to game log (flushes log immediately).
- Expanded EDU entry struct, and added ability to get localized name, description.
- CreateFortXY does not cost money any more, and both fort construction functions now will always work even if you have buildable forts off.
- Expanded `coords` to `xCoord` and `yCoord` in the `tileStruct` and added many new fields.
- Added `aiFaction` to `factionStruct`
- Fixed `battleBuilding:getBuilding()` returning the wrong thing
- Fixed a bug where right clicking an EOP unit in the building description would do nothing
- Added ability to get the character/settlement/fort the cursor is hovering over.
- Added getting merc pool by name.
- Added global roads db to campaign struct.
- Fixed setting diplomatic stances and getting them.
- Added sending characters off map through lua.
- Added ability to get various info on the battlemap, like height at any point, street deployment positions, hill positions, terrain lines, zones etc.
- Added get culture/climate id by name, and getting their name by id.
- Added recruitment queue info
- Added ability to check all the available construction/recruitment items in a settlement and add them to the queue.
- Added ability to see which unit/building is being looked at in the info scroll.
- Added all the options from the campaign DB.
- Many new fields in many structs.


### **ImGUI**
- Added all the drawList functions to draw stuff on the screen.
- Added optional extra parameters to image to draw only parts of it.

NOT SAVEGAME COMPATIBLE!!!!

<!--### **Documentation**-->

### **Links**
- [Known Bugs](https://github.com/youneuoy/M2TWEOP-library/issues/71)
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
