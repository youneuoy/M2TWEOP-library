![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
[![Discord Link](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)
[![Build Status](https://img.shields.io/github/v/release/youneuoy/M2TWEOP-library?label=Download&style=for-the-badge)](#download)

## **Release Notes**

### **Launcher**
- Added [Freecam](https://www.moddb.com/mods/freecam-medieval-2) integration - *Medik*
  - Enable the integration to automatically launch and close Freecam whenever you play the game
- Updated [DXVK](https://github.com/doitsujin/dxvk/releases/tag/v2.5) to v2.5 - *Medik*
- Allow users to specify a mod version which is displayed in the top left beneath the EOP version at launch - *Medik*
- Allow users to specify a website and discord link which can be clicked in the launcher - *Medik*
- Launcher no longer continues to play music if Discord Rich Presence is enabled and background music is enabled - *Medik*
- Renamed "M2TWEOP GUI.exe" to "M2TWEOP_GUI.exe" - *Medik*
- Allow users to disable the "backup saves" feature so they don't get spammed with multiple saves - *Medik*

### **Library**
- Reduced amount of EOP "branding" displayed on startup - *Medik*
- Now multiple hero abilities in generated battles works - *youneuoy*

### **Lua Plugin**
- Stopped the game still receiving keyboard/mouse inputs when the EOP Console was open - *Medik*
- Added a "Clear Input" toggle in the console - *Medik*
- Updated Lua exception message box error handling - *Medik*
  - Abort (Exits the game completely)
  - Retry (Runs `restartLua()` in the background, very useful when stuck in `draw()` error loops)
  - Ignore (Ignore the error and just continue)

### **ImGUI**
- Stopped "Planned retreat route" from appearing when it was disabled - *Medik*
- Updated default scrollbar theming - *Medik*
- `reloadLua()` and `restartLua()` no long require opening the console - *Medik*
- A toast popup appears when `reloadLua()` and `restartLua()` is called to provide feedback that the restart/reload was successful - *Medik*
- Added `Copy to clipboard` button for console output

### **Documentation**
- Updated Erken's ImGUI PDF tutorial to V5 (Thanks Erken!) - *Medik*

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
