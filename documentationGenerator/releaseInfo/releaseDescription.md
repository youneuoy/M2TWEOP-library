![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
[![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**

<!-- ### **Launcher** -->

### **Library**
- Rewrote and improved moveToTile, siegeSettlement, attackCharacter (Fynn)
- Fixed some more potential crashes with EOP unit recruitment due to reports provided to me, thanks to perry123 (Fynn)

### **Lua Plugin**
- Added settlementInfoScroll struct. Allows you to set colours of some elements ![](https://github.com/youneuoy/M2TWEOP-library/assets/22448079/6fd79330-6c82-4498-9962-3fd3612a6da2) (Medik)
- Added spawnArmy, works similar to game spawn_army command allowing you to spawn admirals, respawn off-map characters and use random_name (Fynn)
- Added siegeFort, works pretty much the same way as siegeSettlement but for forts (Fynn)
- Added getFaction, getSettlement and getRegionByName which use internal hash tables for instant retrieval of the selected types by their names. (Fynn)
- Added onUnloadCampaign event so you can for example detect when a user quits back to main menu from the campaign. (Fynn)
- Expanded eduEntry struct with many new fields like getting the animation names, checking if unit has an attribute and getting the mount/projectile the entry uses (Fynn)
- Incompatibility: siegeSettlement now takes 3 arguments, third one being a boolean isAttack which does the same as the game's siege_settlement command maintain|attack option. (Fynn)

<!--### **ImGUI** -->

### **Documentation**
- Some more fixes and missing fields added (Fynn)

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
