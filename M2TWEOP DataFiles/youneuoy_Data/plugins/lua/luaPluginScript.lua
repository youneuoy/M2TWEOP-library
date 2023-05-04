	require('myconfigs')
    -- Helper for managing persistence of tables across save/load
	require('helpers/tableSave')

    -- Uncomment to use EOP Helper functions
	-- require('helpers/EopLuaHelpers')

	-- Uncomment to use external debugger
	-- require('helpers/mobdebug').start()

    --our campaign config table.
    campaignConfig={["someConfigValue"]=5};

    -- Fires when loading a save file
    function onLoadSaveFile(paths)
        campaignPopup=true;

        for index, path in pairs(paths) do

         if (string.find(path, "configTable.lua"))
         then
            --function from helper, load saved table
            campaignConfig=persistence.load(path);
         end

        end
    end

    -- Fires when creating a save file
    -- Returns a list of M2TWEOP save files
    function onCreateSaveFile()
        local savefiles = {};
        currentPath=M2TWEOP.getPluginPath();

        --function from helper, save our table
        persistence.store(currentPath.."configTable.lua",campaignConfig);

        savefiles[1]=currentPath.."configTable.lua";
        return savefiles;
    end

    -- Fires when the plugin is first loaded at game start or reloded with CTRL+9+1
    function onPluginLoad()
        M2TWEOP.unlockGameConsoleCommands();
        -- M2TWEOP.setAncillariesLimit(8);
        -- M2TWEOP.setMaxBgSize(100);
        -- M2TWEOP.setReligionsLimit(50);
        -- M2TWEOP.setBuildingChainLimit(40);
        -- M2TWEOP.setGuildCooldown(3);
    end