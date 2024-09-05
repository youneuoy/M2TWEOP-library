---@diagnostic disable: lowercase-global

-- Uncomment to use VSCode Lua Debugger
-- json = require "helpers/dkjson"
-- local mobdebug = require "helpers/mobdebug"
-- mobdebug.start('127.0.0.1', 8818)

-- Fires when the plugin is first loaded at game start or restarted with restartLua()
function onPluginLoad()
    M2TWEOP.unlockGameConsoleCommands();
    -- UNCOMMENT LINES BELOW TO ENABLE THEM
    --M2TWEOP.setAncillariesLimit(8);
    --M2TWEOP.setMaxBgSize(31);
    --M2TWEOP.setReligionsLimit(10);
    --M2TWEOP.setBuildingChainLimit(9);
    --M2TWEOP.setGuildCooldown(3);
end

--- Called every time an image is rendered for display
--- Change hotkeys here
---@param pDevice LPDIRECT3DDEVICE9
function draw(pDevice)
    if (ImGui.IsKeyPressed(ImGuiKey.GraveAccent))
        and (ImGui.IsKeyDown(ImGuiKey.LeftCtrl))
    then
        M2TWEOP.toggleConsole()
    elseif (ImGui.IsKeyPressed(ImGuiKey.GraveAccent))
        and (ImGui.IsKeyDown(ImGuiKey.LeftAlt))
    then
        M2TWEOP.toggleDeveloperMode()
    elseif (ImGui.IsKeyPressed(ImGuiKey.R))
        and (ImGui.IsKeyDown(ImGuiKey.LeftCtrl))
        and (ImGui.IsKeyDown(ImGuiKey.LeftShift))
    then
        M2TWEOP.restartLua()
    end
end

--- Called after loading the campaign map
function onCampaignMapLoaded()
    CAMPAIGN   = M2TW.campaign
    STRAT_MAP  = M2TW.stratMap
    BATTLE     = M2TW.battle
    UI_MANAGER = M2TW.uiCardManager
end