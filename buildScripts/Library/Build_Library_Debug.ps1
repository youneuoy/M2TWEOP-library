# BUILD EOP Script
$currentLoc=(get-location).path

# Mod folder to copy the files to
$modFolder = "E:\Steam\steamapps\common\Medieval II Total War\mods\MOD_FOLDER"

function CopyFilesToFolder ($fromFolder, $toFolder) {
    $childItems = Get-ChildItem $fromFolder
    $childItems | ForEach-Object {
         Copy-Item -Path $_.FullName -Destination $toFolder -Recurse -Force
    }
}

Write-Output "======== 0) Pre Cleanup ========"

Set-Location -Path $currentLoc
Remove-item ./logs -recurse -erroraction 'silentlycontinue'
new-item ./logs -itemtype directory -erroraction 'silentlycontinue'

# 1) Build M2TWEOP-library
Write-Output "======== 1) Build M2TWEOP-GUI ========"

devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Debug|x86" /project "M2TWEOP library" /out "logs\library.log"

# 2) Copy built files
Write-Output "======== 4) Copy all created files ========"

Set-Location -Path $currentLoc

CopyFilesToFolder "M2TWEOP Code\Release\M2TWEOPLibrary.dll" $modFolder

# 6) Done
Write-Output "======== 6) Success! EOP Built Successfully! ========"