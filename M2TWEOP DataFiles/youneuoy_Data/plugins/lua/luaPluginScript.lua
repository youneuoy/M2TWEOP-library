	require('myconfigs')
    --helper for saveing and loading tables
	require('helpers/tableSave')
	
	--uncomment to use external debugger
	--require('helpers/mobdebug').start()

    --our campaign config table.
    campaignConfig={["someConfigValue"]=5};


    --load save file
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

    --event on create save file. Return to m2tweop list of saved files.
    function onCreateSaveFile()    
        local savefiles = {};
        currentPath=M2TWEOP.getPluginPath();

        --function from helper, save our table
        persistence.store(currentPath.."configTable.lua",campaignConfig);

        savefiles[1]=currentPath.."configTable.lua";
        
        return savefiles;
    end
