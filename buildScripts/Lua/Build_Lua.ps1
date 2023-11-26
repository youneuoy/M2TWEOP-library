param(
    $modFolder
)

. .\buildScripts\Helpers.ps1

$currentLoc=(get-location).path

Set-Location -Path $currentLoc

# 1) Build M2TWEOP-LuaPlugin
Write-Host "`n`n======== 1) Build M2TWEOP-LuaPlugin ========`n" -ForegroundColor Magenta

msbuild  "M2TWEOP-luaPlugin\luaPlugin.sln"/p:Configuration=Release /p:Platform=x86 /t:"luaPlugin" /fileLogger /fileLoggerParameters:LogFile=logs\luaPlugin.log /nowarn:ALL -m

# 2) Copy built files
if ($modFolder) {
    Set-Location -Path $currentLoc
    Write-Host "`n`n======== 6) Copy to mod folder ========`n" -ForegroundColor Magenta
    CopyFilesToFolder "M2TWEOP-luaPlugin\Release\luaPlugin.dll" $modFolder
    Write-Host "`n`n======== 3) Success! EOP Built Successfully! ========`n" -ForegroundColor Magenta
}