# .\buildEOP.ps1 -modFolder "E:\Steam\steamapps\common\Medieval II Total War\mods\Tsardoms-2.2"
param(
    $modFolder
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
if ($LASTEXITCODE -ne 0) { Write-Host "`n`n!!! Failure detected. Stopping the build process. !!!" -ForegroundColor DarkRed ; exit $LASTEXITCODE }

msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"M2TWEOP GUI" /fileLogger /fileLoggerParameters:LogFile=logs\gui.log /NoWarn:ALL -m
if ($LASTEXITCODE -ne 0) { Write-Host "`n`n!!! Failure detected. Stopping the build process. !!!" -ForegroundColor DarkRed ; exit $LASTEXITCODE }

msbuild  "M2TWEOP Code\M2TWEOP library.sln"/p:Configuration=Debug /p:Platform=x86 /t:"d3d9" /fileLogger /fileLoggerParameters:LogFile=logs\d3d9.log /NoWarn:ALL -m
if ($LASTEXITCODE -ne 0) { Write-Host "`n`n!!! Failure detected. Stopping the build process. !!!" -ForegroundColor DarkRed ; exit $LASTEXITCODE }

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

Remove-Item -Path "./M2TWEOPGenerated/eopData/eopScripts/luaPluginScript.lua" -Force
Remove-Item -Path "./M2TWEOPGenerated/eopData/config" -recurse -erroraction 'silentlycontinue' -Force
Remove-Item -Path "./.vs" -recurse -erroraction 'silentlycontinue' -Force

Get-ChildItem -Path "documentationGenerator\EOPDocs\build\html\*" -erroraction 'continue'
CopyFilesToFolder "documentationGenerator\EOPDocs\build\html" "./M2TWEOPGenerated/eopData/documentation"

Copy-Item -Path  "M2TWEOP Code\Debug\d3d9.dll" -Destination "./M2TWEOPGenerated"
Copy-Item -Path  "M2TWEOP Code\Debug\M2TWEOP GUI.exe" -Destination "./M2TWEOPGenerated"
Rename-Item -Path "./M2TWEOPGenerated/M2TWEOP GUI.exe" -NewName "M2TWEOP_GUI.exe"
Copy-Item -Path  "M2TWEOP Code\Debug\M2TWEOPLibrary.dll" -Destination "./M2TWEOPGenerated"
Copy-Item -Path  "M2TWEOP Code\Debug\M2TWEOPLibrary.pdb" -Destination "./M2TWEOPGenerated"


if ($modFolder) {
    Write-Host "`n`n======== 6) Copy to mod folder ========`n" -ForegroundColor Magenta
    CopyFilesToFolder "./M2TWEOPGenerated" $modFolder
}

# 7) Done
Write-Host "`n`n======== Success! EOP Built Successfully! ========`n" -ForegroundColor Magenta