# BUILD EOP Script
$currentLoc=(get-location).path

# Mod folder to copy the files to
$modFolder = "E:\Steam\steamapps\common\Medieval II Total War\mods\ago_beta"

function CopyFilesToFolder ($fromFolder, $toFolder) {
    $childItems = Get-ChildItem $fromFolder
    $childItems | ForEach-Object {
         Copy-Item -Path $_.FullName -Destination $toFolder -Recurse -Force
    }
}

Write-Output "======== 0) Pre Cleanup ======== "


Set-Location -Path $currentLoc
Remove-item ./logs -recurse -erroraction 'silentlycontinue'
new-item ./logs -itemtype directory -erroraction 'silentlycontinue'

# 1) Build M2TWEOP-library
Write-Output "======== 1) Build M2TWEOP-GUI ======== "

# devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP library" /out "logs\library.log"
devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP GUI" /out "logs\GUI.log" /debugexe
#devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP tools"  /out "logs\tools.log"
#devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "d3d9"  /out "logs\d3d9.log"

# 2) Build M2TWEOP-LuaPlugin
# Write-Output "======== 2) Build M2TWEOP-LuaPlugin ======== "

# devenv  "M2TWEOP-luaPlugin\luaPlugin.sln" /build "Release|x86" /project "luaPlugin"  /out "logs\luaPlugin.log"

# 3) Build Documentation
#Write-Output "======== 3) Build M2TWEOP-Documentation ======== "
#
#cd "documentationGenerator"
#&".\generateDocs.ps1"   -Wait -NoNewWindow | Write-Verbose

# 4) Copy built files
Write-Output "======== 4) Copy all created files ======== "

Set-Location -Path $currentLoc
Remove-item ./M2TWEOPGenerated -recurse -erroraction 'silentlycontinue'
new-item ./M2TWEOPGenerated  -itemtype directory -erroraction 'silentlycontinue'

# new-item ./M2TWEOPGenerated/youneuoy_Data/plugins  -itemtype directory -erroraction 'silentlycontinue'
# Copy-Item -Path  "M2TWEOP-luaPlugin\Release\luaPlugin.dll" -Destination "./M2TWEOPGenerated/youneuoy_Data/plugins"
#Copy-Item -Path  "M2TWEOP Code\Release\d3d9.dll" -Destination "./M2TWEOPGenerated"
# Copy-Item -Path  "M2TWEOP Code\Release\M2TWEOP GUI.exe" -Destination "./M2TWEOPGenerated"
#Copy-Item -Path  "M2TWEOP Code\Release\M2TWEOP tools.exe" -Destination "./M2TWEOPGenerated"
# Copy-Item -Path  "M2TWEOP Code\Release\M2TWEOPLibrary.dll" -Destination "./M2TWEOPGenerated"
CopyFilesToFolder "M2TWEOP Code\Release\M2TWEOP GUI.exe" $modFolder

# 6) Done
Write-Output "======== 6) Success! EOP Built Successfully! ======== "

"$modFolder\M2TWEOP GUI.exe"