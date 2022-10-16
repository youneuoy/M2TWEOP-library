# BUILD EOP Script
$currentLoc=(get-location).path

# Colors
# https://duffney.io/usingansiescapesequencespowershell/
$color = "`e[$(35)m"
$endColor = "`e[0m`e[30;"

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
Write-Output "$color======== 1) Build M2TWEOP-library ======== $endColor"

devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP library" /out "logs\library.log"
devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP GUI" /out "logs\GUI.log"
devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "M2TWEOP tools"  /out "logs\tools.log"
devenv  "M2TWEOP Code\M2TWEOP library.sln" /build "Release|x86" /project "d3d9"  /out "logs\d3d9.log"

# 2) Copy built files
Write-Output "$color======== 2) Copy all created files ======== $endColor"

Set-Location -Path $currentLoc
Remove-item ./M2TWEOPGenerated -recurse -erroraction 'silentlycontinue'
new-item ./M2TWEOPGenerated  -itemtype directory -erroraction 'silentlycontinue'

Copy-Item -Path  "M2TWEOP Code\Release\M2TWEOPLibrary.dll" -Destination "./M2TWEOPGenerated"

# 3) Generate Release ZIP
Write-Output "$color======== 3) Generate Release ZIP ======== $endColor"
Remove-item M2TWEOP.zip -erroraction 'silentlycontinue'
Compress-Archive -Path "./M2TWEOPGenerated/*"  -DestinationPath "M2TWEOP-Library.zip" -Force
Remove-item ./M2TWEOPGenerated -recurse -erroraction 'silentlycontinue'

# 4) Done
Write-Output "$color======== 4) Success! EOP Built Successfully! ======== $endColor"
explorer .