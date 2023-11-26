param(
    $modFolder
)

. .\buildScripts\Helpers.ps1

$currentLoc=(get-location).path

Write-Host "`n`n======== 0) Pre Cleanup ========`n" -ForegroundColor Magenta

Set-Location -Path $currentLoc
Remove-item ./logs -recurse -erroraction 'silentlycontinue'
new-item ./logs -itemtype directory -erroraction 'silentlycontinue'

# 1) Build M2TWEOP-library
Write-Host "`n`n======== 1) Build D3D9.dll ========`n" -ForegroundColor Magenta

msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"d3d9" /fileLogger /fileLoggerParameters:LogFile=logs\d3d9.log /nowarn:ALL -m

# 2) Copy built files
Write-Host "`n`n======== 2) Copy all created files ========`n" -ForegroundColor Magenta

Set-Location -Path $currentLoc

if ($modFolder) {
    Write-Host "`n`n======== 6) Copy to mod folder ========`n" -ForegroundColor Magenta
    CopyFilesToFolder "M2TWEOP Code\Debug\d3d9.dll" $modFolder
    Write-Host "`n`n======== 3) Success! EOP Built Successfully! ========`n" -ForegroundColor Magenta
}