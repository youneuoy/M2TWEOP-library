# Colors
$color =`n" -ForegroundColor Magenta`e[$(35)m"
$endColor =`n" -ForegroundColor Magenta`e[0m`e[30;"

#remove old lua doc files
Write-Output "$color ======== 3.1) Remove old documentation ======== $endColor"
Remove-item generatedLuaImGuiDocs -recurse  -erroraction 'silentlycontinue'

#create folder for temporary place scripting docs
new-item ./generatedLuaImGuiDocs -itemtype directory

Write-Output "$color ======== 3.2) Generate documentation config ======== $endColor"

# Using LuaRocks, call ldoc.lua and pass in the config file
.\luarocks\lua.exe ./luarocks/rocks/ldoc/1.4.6-2/bin/ldoc.lua . -c configImGui.ld
Start-Process -FilePath ".\makeLuaImGuiDocs.bat" -Wait -NoNewWindow