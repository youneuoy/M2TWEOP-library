# BUILD EOP Script
$currentLoc=(get-location).path

# Mod folder to copy the files to
$modFolder = "E:\Steam\steamapps\common\Medieval II Total War\mods\MOD_FOLDER\youneuoy_Data\plugins"

function CopyFilesToFolder ($fromFolder, $toFolder) {
    $childItems = Get-ChildItem $fromFolder
    $childItems | ForEach-Object {
         Copy-Item -Path $_.FullName -Destination $toFolder -Recurse -Force
    }
}

Set-Location -Path $currentLoc

# 1) Build M2TWEOP-LuaPlugin
Write-Output "======== 1) Build M2TWEOP-LuaPlugin ======== "

devenv  "M2TWEOP-luaPlugin\luaPlugin.sln" /build "Debug|x86" /project "luaPlugin"  /out "logs\luaPlugin.log"

# 4) Copy built files
Write-Output "======== 2) Copy all created files ======== "

Set-Location -Path $currentLoc

CopyFilesToFolder "M2TWEOP-luaPlugin\Release\luaPlugin.dll" $modFolder

# 6) Done
Write-Output "======== 3) Success! EOP Built Successfully! ======== "

"$modFolder\M2TWEOP GUI.exe"