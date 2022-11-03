## Configuring the EOP launcher and settings

### Configure the UI

#### ```eopData/uiCfg.json```

```
{
    "hideLauncher": false, // Hide the launcher completely and just load the mod directly
    "modCfgFile": "", // The default config file to use (e.g TATW.cfg)
    "musicVolume": 15, // The volume level of the background music
    "playBackgroundMusic": false, // Whether music should play automatically when starting the launcher
    "useM2TWEOP": true, // Whether EOP should be enabled when running the mod
    "useVanillaCfg": true, // Don't read the modCfgFile but just read medieval2.preference.cfg
    "modTitle": "Divide and Conquer: AGO", // Sets the title of the launcher window and the "Run" button
    "runButtonColor": "123,40,145,185", // Color of the run button in this format (red(0-255), green(0-255), blue(0-255), alpha(0-255))
    "runButtonHoverColor": "145,40,145,175" // Hover/highlight color of the run button in this format (red(0-255), green(0-255), blue(0-255), alpha(0-255))
}
```

### Configure the Battles

#### ```eopBattles/battlesCfg.json```

```
{
    "enableAutoGeneration": false, // Whether we should automatically generate the details of a battle to play online when we load into a battle
    "enableResultsTransfer": false // Whether we should automatically prompt the player to transfer the results of a battle to the campaign map
}
```


### Configure the Game

#### ```eopData/gameCfg.json```

```
{
    "isBlockLaunchWithoutEop": false // Should we stop the mod running if EOP is not enabled
}
```