function CopyFilesToFolder ($fromFolder, $toFolder) {
    Write-Host "`n`nCopying all files from '$fromFolder' to '$toFolder' ... `n" -ForegroundColor Yellow

    $childItems = Get-ChildItem $fromFolder
    $childItems | ForEach-Object {
         Copy-Item -Path $_.FullName -Destination $toFolder -Recurse -Force
    }
}