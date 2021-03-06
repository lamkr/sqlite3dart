::
:: Use this script to facilitate the package building. 
::
:: At terminal, goes to the sources main directory, and run this script.
:: Syntax:
:: 	build [<parameter>]
:: Where <parameter> can be:
::	rm 		- Remove old builded files before building the package; after, it runs "dart" (see below).
::  dart 	- Only runs the Dart test at directory 'sqlite3dart/example'.
::
::  If <parameter> is not present, the script build the package and runs the Dart test.
::

@echo off
set arg1=%1

:: https://stackoverflow.com/questions/44638166/how-to-remove-last-segment-in-filepath-in-command-prompt
:: Find Dart program on environment variable PATH
if exist %DART_SDK% goto setVSVars
for %%i in (dart.exe) do set "DART_SDK=%%~dp$PATH:i"
for %%a in ("%DART_SDK%") do for %%b in ("%%~dpa.") do set "DART_SDK=%%~dpb"

:setVSVars
:: Visual Studio 2019 Community ::
:: set CMAKE_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
for %%i in (cmake.exe) do set CMAKE_PATH="%%~dp$PATH:i"
for %%i in (MSBuild.exe) do set VSPATH="%%~dp$PATH:i"
set VSPATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin"
if not exist %VSPATH% (
	@echo # Please, set environment variable VSPATH to instalation path of 'MSBuild.exe'.
	goto fim
)

:build
set SolutionFolder=.\build
set SolutionDir=.\build
	echo "%~1"
if "%~1"=="rm" rm -rf build
if "%~1"=="dart" goto RUN_DART
%CMAKE_PATH%\cmake -S . -Bbuild
echo ERRORLEVEL=%ERRORLEVEL% cmake
if %ERRORLEVEL% neq 0 goto erro

%VSPATH%\msbuild .\build\sqlite3dart_extension.sln /p:Configuration=Release /p:Platform=x64
echo ERRORLEVEL=%ERRORLEVEL% %VSPATH%\msbuild .\build\sqlite3dart_extension.sln /p:Configuration=Release /p:Platform=x64
if %ERRORLEVEL% neq 0 goto erro

copy /Y build\Release\sqlite3dart_extension.dll .\sqlite3dart\example
:RUN_DART
	cd .\sqlite3dart\example
	%DART_SDK%\bin\dart -v sqlite3dart_test.dart
	echo ERRORLEVEL=%ERRORLEVEL% dart
	cd ..\..
	goto fim
:erro
	echo # ERRO
:fim
