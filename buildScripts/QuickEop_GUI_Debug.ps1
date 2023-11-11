# BUILD EOP Script
$currentLoc=(get-location).path

# Colors
# https://duffney.io/usingansiescapesequencespowershell/
$color = "`e[$(35)m"
$endColor = "`e[0m`e[30;"

# Mod folder to copy the files to
$modFolder = "E:\Steam\steamapps\common\Medieval II Total War\mods\ago_beta"

function CopyFilesToFolder ($fromFolder, $toFolder) {
    $childItems = Get-ChildItem $fromFolder
    $childItems | ForEach-Object {
         Copy-Item -Path $_.FullName -Destination $toFolder -Recurse -Force
    }
}

Write-Output "$color======== 0) Pre Cleanup ======== $endColor"


Set-Location -Path $currentLoc
Remove-item ./logs -recurse -erroraction 'silentlycontinue'
new-item ./logs -itemtype directory -erroraction 'silentlycontinue'

# 1) Build M2TWEOP-library
Write-Output "$color======== 1) Build M2TWEOP-GUI ======== $endColor"

# devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP library" /out "logs\library.log"
devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Debug|x86" /project "M2TWEOP GUI" /out "logs\GUI.log"
#devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP tools"  /out "logs\tools.log"
#devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "d3d9"  /out "logs\d3d9.log"

# 2) Build M2TWEOP-LuaPlugin
# Write-Output "$color======== 2) Build M2TWEOP-LuaPlugin ======== $endColor"

# devenv  "M2TWEOP-luaPlugin\luaPlugin.sln" /build "Release|x86" /project "luaPlugin"  /out "logs\luaPlugin.log"

# 3) Build Documentation
#Write-Output "$color======== 3) Build M2TWEOP-Documentation ======== $endColor"
#
#cd "documentationGenerator"
#&".\generateDocs.ps1"   -Wait -NoNewWindow | Write-Verbose

# 4) Copy built files
Write-Output "$color======== 4) Copy all created files ======== $endColor"

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
Write-Output "$color======== 6) Success! EOP Built Successfully! ======== $endColor"