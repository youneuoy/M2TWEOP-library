![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
[![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**

<!-- ### **Launcher** -->

<!-- ![](https://i.imgur.com/Pl02p1W.png) -->
<!-- ![](https://i.imgur.com/h8UlYMT.png) -->

### **Library**
- Fixed crashing on startup in some mods - *Fynn*
- Fixed some issues with `CombatVsReligion` attributes. 
  - **Note:** Do not use religions > 10 in CombatVsReligion attributes! - *Fynn*

### **Lua Plugin**
- Removed default `CTRL+1` and `CTRL+2` binds for the Console/Developer Mode.
  - These are now bindable in Lua, see `M2TWEOP.toggleConsole()` (Bound to `CTRL + ~` by default) and `M2TWEOP.toggleDeveloperMode()` (Bound to `ALT + ~` by default)
- Added `M2TWEOP.setEquipmentCosts()`. Allows you to set the siege points required to make different siege equipment. *Fynn*
- Fixed `battleObjective:getType()` to properly return the right type *Fynn*
- Added `battleBuildings` field to `battleResidence` struct - *Fynn*

### **ImGUI**
- Updated to the latest version of the ImGUI library - *Fynn*
  - This version bump will likely have some very small breaking changes but also includes a bunch of bugixes, performance enhancements and new features! Please consult the docs and report any bugs you find. 
  - Previous Version: [1.86.0](https://github.com/ocornut/imgui/releases/tag/v1.86)
  - Current Version: [1.90.2](https://github.com/ocornut/imgui/releases/tag/v1.90.2)
- Added a bunch of new keys available to be used within ImGUI - *Fynn*


### **Documentation**
- Added proper ImGUI documentation in the same format as the normal Lua documentation. - *Fynn*
  - This enables proper Intellisense for all ImGUI Lua provided you are using Visual Studio Code and the correct [Lua Extension](https://marketplace.visualstudio.com/items?itemName=sumneko.lua)

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
