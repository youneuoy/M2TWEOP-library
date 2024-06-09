# .\buildEOP.ps1 -modFolder "E:\Steam\steamapps\common\Medieval II Total War\mods\Tsardoms-2.2"
param(
    $modFolder,
    $deletePluginScript = 'True',
    $shouldZip = 'False'
)

# BUILD EOP Script
$currentLoc=(get-location).path

function CopyFilesToFolder ($fromFolder, $toFolder) {
    Write-Host "`n`nCopying all files from '$fromFolder' to '$toFolder' ... `n" -ForegroundColor Yellow

    $childItems = Get-ChildItem $fromFolder
    $childItems | ForEach-Object {
         Copy-Item -Path $_.FullName -Destination $toFolder -Recurse -Force
    }
}

Write-Host "`n`n======== 0) Pre Cleanup ========`n" -ForegroundColor Magenta

Set-Location -Path $currentLoc
Remove-item ./logs -recurse -erroraction 'silentlycontinue'
new-item ./logs -itemtype directory -erroraction 'silentlycontinue'

# 1) Build M2TWEOP-library
Write-Host "`n`n======== 1) Build M2TWEOP-library ========`n" -ForegroundColor Magenta

msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"M2TWEOP library" /fileLogger /fileLoggerParameters:LogFile=logs\library.log /NoWarn:ALL -m
msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"M2TWEOP GUI" /fileLogger /fileLoggerParameters:LogFile=logs\gui.log /NoWarn:ALL -m
msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"d3d9" /fileLogger /fileLoggerParameters:LogFile=logs\d3d9.log /NoWarn:ALL -m

# 2) Build M2TWEOP-LuaPlugin
Write-Host "`n`n======== 2) Build M2TWEOP-LuaPlugin ========`n" -ForegroundColor Magenta
msbuild  "M2TWEOP-luaPlugin\luaPlugin.sln"/p:Configuration=Debug /p:Platform=x86 /t:"luaPlugin" /fileLogger /fileLoggerParameters:LogFile=logs\luaPlugin.log /NoWarn:ALL -m

# 3) Build Documentation
Write-Host "`n`n======== 3) Build M2TWEOP-Documentation ========`n" -ForegroundColor Magenta

cd "documentationGenerator"
&".\generateDocs.ps1"   -Wait -NoNewWindow | Write-Verbose

# 4) Copy built files
Write-Host "`n`n======== 4) Copy all created files ========`n" -ForegroundColor Magenta

Set-Location -Path $currentLoc
Remove-item ./M2TWEOPGenerated -recurse -erroraction 'silentlycontinue'
new-item ./M2TWEOPGenerated  -itemtype directory -erroraction 'silentlycontinue'

Copy-Item -Path  "M2TWEOP DataFiles\*" -Destination "./M2TWEOPGenerated" -recurse

if ($deletePluginScript) {
    Remove-Item -Path "./M2TWEOPGenerated/youneuoy_Data/plugins/lua/luaPluginScript.lua" -Force
}

Get-ChildItem -Path "documentationGenerator\EOPDocs\build\html\*" -erroraction 'continue'
CopyFilesToFolder "documentationGenerator\EOPDocs\build\html" "./M2TWEOPGenerated/eopData/helpPages"

Copy-Item -Path  "M2TWEOP-luaPlugin\Debug\luaPlugin.dll" -Destination "./M2TWEOPGenerated/youneuoy_Data/plugins"
Copy-Item -Path  "M2TWEOP Code\Debug\d3d9.dll" -Destination "./M2TWEOPGenerated"
Copy-Item -Path  "M2TWEOP Code\Debug\M2TWEOP GUI.exe" -Destination "./M2TWEOPGenerated"
# Copy-Item -Path  "M2TWEOP Code\Debug\M2TWEOP tools.exe" -Destination "./M2TWEOPGenerated"
Copy-Item -Path  "M2TWEOP Code\Debug\M2TWEOPLibrary.dll" -Destination "./M2TWEOPGenerated"

if ($shouldZip -eq 'True') {
    # 5) Generate Debug ZIP
    Write-Host "`n`n======== 5) Generate Debug ZIP ========`n" -ForegroundColor Magenta
    Remove-item M2TWEOP.zip -erroraction 'silentlycontinue'
    Compress-Archive -Path "./M2TWEOPGenerated/*"  -DestinationPath "M2TWEOP.zip"
}

if ($modFolder) {
    Write-Host "`n`n======== 6) Copy to mod folder ========`n" -ForegroundColor Magenta
    CopyFilesToFolder "./M2TWEOPGenerated" $modFolder
}

# 7) Done
Write-Host "`n`n======== Success! EOP Built Successfully! ========`n" -ForegroundColor Magenta