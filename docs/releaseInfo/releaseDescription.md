![EOP Logo](https://i.imgur.com/jqzoYoQ.png)

# New beta version of M2TWEOP 2.1😋

 [![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D4DZTHG)
 [![Build Status](https://img.shields.io/discord/713369537948549191?color=red&label=Discord&style=for-the-badge)](https://discord.gg/Epqjm8u2WK)

## **Release Notes**

### **Breaking Changes**
- Renamed "`etResourceCode`" to "`getResourceID`" for uniformity :: WARNING: backwards incompatibility! - *Callistonian*

### **Launcher**
- Added M2TWEOP health checker (this will appear automatically if there are potential startup problems) - *youneuoy*
- Removed `M2TWEOPUI.cfg` (this file is now obsolete, you may remove it) - *Medik*
- Added new file `eopData/uiCfg.json` to configure the launcher  - *Medik*
- Added new config options to change the launcher window title, "Run mod" message and button colors - *Medik*
- EOP is now enabled by default in the launcher config options - *Medik*
- [Added a new tutorial on configuring the launcher](https://youneuoy.github.io/M2TWEOP-library/M2TWEOP_LAUNCHER_SETTINGS.html) - *Medik*

#### **Examples of the new launcher options**
![](https://i.imgur.com/Zfr6M1k.png)
![](https://i.imgur.com/LAiOigh.png)
![](https://i.imgur.com/0ctfDQi.png)

### **Library**
- Hosteat: select battle winner without casualties - *youneuoy*
- DeveloperMode: activated by ctrl+2 - *youneuoy*
- DeveloperMode: BattleMapOverrider, useful for custom maps testing. - *youneuoy*

### **Lua Plugin**
- EduEntry table: new fields - *youneuoy*
- M2TWEOPDU table: now `setEntrySoldierModel` works better - *youneuoy*
- namedCharacter table: removed `localizedSaveName`, `localizedSaveName2`, `localizedSaveNickName` - *youneuoy*
- namedCharacter table: added rest of the attribute members - *Callistonian*
- Added event functions: `onShortcutTriggered()`, `onCharacterMarriesPrincess()`, `onBecomesFactionLeader()`, `onBecomesFactionHeir()` - *Callistonian*
- Added structures: `portStruct`, `watchtowerStruct`, `factionStratMap`, `buildingsQueue`, `buildingInQueue` - *Callistonian*
- Renamed "`getResourceCode`" to "`getResourceID`" for uniformity :: WARNING: backwards incompatibility! - *Callistonian*
- Added trade resource getters for `hasMine` and image (icon) - *Callistonian*

### **Documentation**
- Updated Erken's ImGUI tutorial to V3 (Thanks again Erken!) - *Medik*
- Lua documentation corrected and improved with many usage examples - *Callistonian*

### **Bugfixes**
- Fixes for launch process
- Iterating through sub elements from 0 to (subElementsnum-1) now returns the correct results - *Callistonian*
- Setting resource strat models no longer sets their cost and hasMine fields to 0 - *Callistonian*
- `soldierCountStratMapMax` and `soldierCountBattleMap` return the correct values - *Callistonian*

### **Links**
* [EOP Documentation](https://youneuoy.github.io/M2TWEOP-library/)
* [EOP Discord](https://discord.gg/cG2Paep9)
