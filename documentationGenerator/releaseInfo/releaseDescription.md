![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

 [![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
 [![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
 [![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**

### **Launcher**
- Added options in the launcher to enable/disable the following - *youneuoy*
    - `M2TWEOP developer mode` (CTRL + 2)
    - `Tactical map viewer`
    - `Stratmap context menu`

### **Library**
- Added Rome: Total War like Tactical Map Viewer. Works for any tile type, not only settlements. To activate click 'Middle Mouse Button' on any tile. You can preview it [here!](https://www.youtube.com/watch?v=RrGi4zxr7bU) - *youneuoy*
- New EOP buildings feature allowing making unique copies of buildings which you can change entirely. - *Fynn*

### **Lua Plugin**
- Added new EOP buildings system. - *Fynn*
- Add and remove capabilities and recruitment pools from buildings, change their names, descriptions and pics. - *Fynn*
- Get EDB entry of a building by name. - *Fynn*
- Create and delete forts at will. - *Fynn*
- Added region struct, expanded a lot of other structs including mercenaries, buildings, stratmap. - *Fynn*
- Added culture ID and kings purse to faction struct. - *Fynn*
- Added ability to change faction name, settlement name, region name, region's rebel type name. - *Fynn*
- Expanded EOPDU with ability to change all primary stats, morale and upgrade models. - *Fynn*
- Added function to find edu entry by name or index. - *Fynn*
- Added removing and adding mercenaries to pools, and change the religion requirements. - *Fynn*
- Added function to get region so you can iterate over them. - *Fynn*
- Added function to merge 2 armies. - *Fynn*

### **ImGUI**
- New event `onLoadingFonts`. Use it for loading fonts for useing in IMGUI - *youneuoy*
```lua
function onLoadingFonts()
    myCustomFont = ImGui.GetIO().Fonts:AddFontFromFileTTF(M2TWEOP.getModPath().."/eopData/fonts/customFont.ttf", 55, nil, nil);
end
```
- Added `ImGui.GetStyle();` and style settings (undocumented for now!) - *youneuoy*
- Added `ImGui.PushFont(font);`, `ImGui.PopFont();`. Use it in your draw function to change fonts. - *youneuoy*

![Screenshot 2023-02-21 at 14 05 23](https://user-images.githubusercontent.com/22448079/220367433-a825bd36-02a7-484f-a523-a58e4524f651.png)


### **Documentation**
- Added Fynn's documentation and examples. Now pretty much every single function is now documented! - *Fynn and Medik*
- `onPluginLoad` function is now included by default to `luaPluginScript.lua` with some valid options commented out and `M2TWEOP.unlockConsoleCommands()` enabled by default - *Medik*

### **Bugfixes**
- Fixed Battlemap highlight causing crashes. - *Fynn*
- Fix for EOPDU units so they always get unique internal ID - *Fynn*

### **Links**
* [EOP Documentation](https://youneuoy.github.io/M2TWEOP-library/)
* [EOP Discord](https://discord.gg/Epqjm8u2WK)

### **Download**
<a id="download"></a>
