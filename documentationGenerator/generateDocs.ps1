# Colors
$color = "`e[$(35)m"
$endColor = "`e[0m`e[30;"

#remove old lua doc files
Write-Output "$color ======== 3.1) Remove old documentation ======== $endColor"
Remove-item generatedLuaDocs -recurse  -erroraction 'silentlycontinue'

#create folder for temporary place scripting docs
new-item ./generatedLuaDocs -itemtype directory

Write-Output "$color ======== 3.2) Generate documentation config ======== $endColor"
#.\luarocks\ldoc.lua.bat . -c ../luaPlugin/config.ld
.\luarocks\lua.exe ./luarocks/rocks/ldoc/1.4.6-2/bin/ldoc.lua . -c config.ld

Write-Output "$color ======== 3.3) Remove old files ======== $endColor"
#remove old scripting files in main docs path
Remove-item EOPDocs/source/_static/LuaLib/* -recurse  -erroraction 'silentlycontinue'
#erase build path
Remove-item EOPDocs/build/* -recurse  -erroraction 'silentlycontinue'

Write-Output "$color ======== 3.4) Build documentation files  ======== $endColor"
#copy created scripting docs to main docs source folder
Copy-Item -Path "generatedLuaDocs/*" -Destination "EOPDocs/source/_static/LuaLib" -recurse
Start-Process -FilePath ".\EOPDocs\WPy32-3890\scripts\cmdEOPDOCS.bat" -Wait -NoNewWindow

Write-Output "$color ======== 3.5) Success! Documentation built successfully.  ======== $endColor"