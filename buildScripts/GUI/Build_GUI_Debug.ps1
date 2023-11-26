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
Write-Host "`n`n======== 1) Build M2TWEOP-GUI ========`n" -ForegroundColor Magenta

msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"M2TWEOP GUI" /fileLogger /fileLoggerParameters:LogFile=logs\gui.log /nowarn:ALL -m

# 2) Copy built files
if ($modFolder) {
    Set-Location -Path $currentLoc
    Write-Host "`n`n======== 6) Copy to mod folder ========`n" -ForegroundColor Magenta
    CopyFilesToFolder "M2TWEOP Code\Debug\M2TWEOP GUI.exe" $modFolder
    Start-Process $modFolder"\M2TWEOP GUI.exe"
    Write-Host "`n`n======== 3) Success! EOP Built Successfully! ========`n" -ForegroundColor Magenta
}