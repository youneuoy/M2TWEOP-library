# Colors
#remove old lua doc files
Write-Host "`n`n======== 3.1) Remove old documentation ========`n" -ForegroundColor Magenta
Remove-item generatedLuaDocs -recurse  -erroraction 'silentlycontinue'

#create folder for temporary place scripting docs
new-item ./generatedLuaDocs -itemtype directory

Write-Host "`n`n======== 3.2) Generate documentation config ========`n" -ForegroundColor Magenta

# Using LuaRocks, call ldoc.lua and pass in the config file
.\luarocks\lua.exe ./luarocks/rocks/ldoc/1.4.6-2/bin/ldoc.lua . -c config.ld
Start-Process -FilePath ".\makeLuaDocs.bat" -Wait -NoNewWindow

Write-Host "`n`n======== 3.3) Remove old files ========`n" -ForegroundColor Magenta

# Remove old scripting files in main docs path
Remove-item EOPDocs/source/_static/LuaLib/* -recurse  -erroraction 'silentlycontinue'

# Remove old build
Remove-item EOPDocs/build/* -recurse  -erroraction 'silentlycontinue'

Write-Host "`n`n======== 3.4) Build documentation files  ========`n" -ForegroundColor Magenta

#copy created scripting docs to main docs source folder
Copy-Item -Path "generatedLuaDocs/*" -Destination "EOPDocs/source/_static/LuaLib" -recurse
Start-Process -FilePath ".\EOPDocs\WPy32-3890\scripts\cmdEOPDOCS.bat" -Wait -NoNewWindow

Write-Host "`n`n======== 3.5) Success! Documentation built successfully.  ========`n" -ForegroundColor Magenta