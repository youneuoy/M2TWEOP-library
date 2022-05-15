@echo off
setlocal
set LUA_PATH=%~dp0\lua/?.lua;%~dp0\lua/?/init.lua;;%~dp0\?.lua;%~dp0\lua\?.lua;%~dp0\lua\?\init.lua;%~dp0\?.lua;%~dp0\?\init.lua;%~dp0\lua\?.luac;%LUA_PATH%
set LUA_CPATH=%~dp0\clibs/?.dll;%~dp0\?.dll;%~dp0\?.dll;%~dp0\loadall.dll;%~dp0\clibs\?.dll;%~dp0\clibs\loadall.dll;%~dp0\?51.dll;%~dp0\?51.dll;%~dp0\clibs\?51.dll;%LUA_CPATH%
cd %~dp0\lua
REM %~dp0\lua.exe %~dp0\lua\luarocks\loader.lua ".\rocks/ldoc/1.4.6-2/bin/ldoc.lua" %*
%~dp0\lua -lluarocks.loader
endlocal
