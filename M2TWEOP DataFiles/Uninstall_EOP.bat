@echo off
echo WARNING!!! THIS FILE WILL DELETE ALL EOP FILES. MAKE BACKUPS OF ANY SCRIPTS YOU WISH TO PRESERVE FIRST BEFORE CONTUINING!

set /p userResponse=Do you really want to uninstall all EOP files? (Y/N): 

if /i "%userResponse%"=="Y" (
    echo You chose to continue.
    rmdir /s /q  "eopData"
    DEL /F /Q "M2TWEOP GUI.exe"
    DEL /F /Q "d3d9.dll"
    DEL /F /Q "discord_game_sdk.dll"
    DEL /F /Q "libfbxsdk.dll"
    DEL /F /Q "lua5.1.dll"
    DEL /F /Q "M2TWEOPLibrary.dll"
    DEL /F /Q "openal32.dll"
    DEL /F /Q "d3d9_vk.dll"
) else (
    echo You chose to cancel.
)

pause