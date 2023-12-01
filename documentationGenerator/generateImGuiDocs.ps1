#remove old lua doc files
Write-Host "`n`n======== 3.1) Remove old documentation ========`n" -ForegroundColor Magenta
Remove-item generatedLuaImGuiDocs -recurse  -erroraction 'silentlycontinue'

#create folder for temporary place scripting docs
new-item ./generatedLuaImGuiDocs -itemtype directory

Write-Host "`n`n======== 3.2) Generate documentation config ========`n" -ForegroundColor Magenta

# Using LuaRocks, call ldoc.lua and pass in the config file
.\luarocks\lua.exe ./luarocks/rocks/ldoc/1.4.6-2/bin/ldoc.lua . -c configImGui.ld
Start-Process -FilePath ".\makeLuaImGuiDocs.bat" -Wait -NoNewWindow