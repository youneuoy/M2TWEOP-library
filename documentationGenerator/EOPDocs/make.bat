@ECHO OFF

pushd %~dp0

REM Command file for Sphinx documentation


set SOURCEDIR=source
set BUILDDIR=build




start WPy32-3890/WinPython Command Prompt.exe/sphinx-build -M %1 %SOURCEDIR% %BUILDDIR% %SPHINXOPTS% %O%
goto end

:help
%SPHINXBUILD% -M help %SOURCEDIR% %BUILDDIR% %SPHINXOPTS% %O%

:end
popd
