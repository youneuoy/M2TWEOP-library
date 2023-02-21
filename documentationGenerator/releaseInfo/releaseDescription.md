![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

# New beta version of M2TWEOP 2.1😋

 [![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
 [![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
 [![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**

<!-- ### **Breaking Changes** -->

<!-- ### **Launcher** -->
- New eop gameplay settings - *youneuoy*

<!-- ### **Library** -->
- Added Rtw-like Tactical Map Viewer. Works for any tile type, not only settlements. To activate click middle mouse button on any tile. - *youneuoy*
- Fix for eopdu units so they always get unique internal ID - *Fynn*
- New EOP buildings feature allowing making unique copies of buildings which you can change entirely. - *Fynn*

<!-- ### **Lua Plugin** -->
- Add EOP buildings. - *Fynn*
- Add and remove capabilities and recruitment pools from buildings, change their names, descriptions and pics. - *Fynn*
- Get EDB entry of a building by name. - *Fynn*
- Create and delete forts at will. - *Fynn*
- Added region struct, expanded a lot of other structs including mercenaries, buildings, stratmap. - *Fynn*
- Added culture ID and kings purse to faction struct. - *Fynn*
- Added ability to change faction name, settlement name, region name, region's rebel type name. - *Fynn*
- Expanded EOPDU with ability to change all primary stats, morale and upgrade models. - *Fynn*
- Added function to find edu entry by name or index. - *Fynn*
- Added removing and adding mercenaries to pools, and change the religion requirements. - *Fynn*
- Add function to get region so you can iterate over them. - *Fynn*
- Added function to merge 2 armies. - *Fynn*

### **Documentation**
- Added Fynn's documentation and examples. Now pretty much every single function is now documented! - *Fynn and Medik*
- New event `onLoadingFonts`. Use it for loading fonts for useing in IMGUI, like ` font = ImGui.GetIO().Fonts:AddFontFromFileTTF(M2TWEOP.getModPath().."/eopData/fonts/mainFont.ttf", 55,nil,nil);`  - *youneuoy*
- Imgui: added `ImGui.GetStyle();` and style settings(undocumented now!) - *youneuoy*
- Imgui: added `ImGui.PushFont(font);`, `ImGui.PopFont();`. Use it in your draw function to change fonts. - *youneuoy*
- `onPluginLoad` function is now included by default with some valid options commeneted out and `M2TWEOP.unlockConsoleCommands()` enabled by default - *Medik* 

<!-- ### **Bugfixes** -->
- Fixed battlemap highlight causing crashes. - *Fynn*

### **Links**
* [EOP Documentation](https://youneuoy.github.io/M2TWEOP-library/)
* [EOP Discord](https://discord.gg/cG2Paep9)

# Download
<a id="download"></a>
