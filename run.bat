@echo off
set arg1=%1
rem home set VSPATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin"
set VSPATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin"
set SolutionFolder=.\build
set SolutionDir=.\build
if "%~1"=="rm" rm -rf build
if "%~1"=="dart" goto RUN_DART
cmake -S . -B build -DCMAKE_GENERATOR_PLATFORM=x64
echo ERRORLEVEL=%ERRORLEVEL% cmake
if %ERRORLEVEL% neq 0 goto erro
REM cd build 
%VSPATH%\msbuild .\build\sqlite3dart_extension.sln /p:Configuration=Release /p:Platform=x64
echo ERRORLEVEL=%ERRORLEVEL% %VSPATH%\msbuild .\build\sqlite3dart_extension.sln /p:Configuration=Release /p:Platform=x64
if %ERRORLEVEL% neq 0 goto erro
REM cd ..
copy /Y build\Release\sqlite3dart_extension.dll .\dart
:RUN_DART
	cd dart 
	dart sqlite3dart_test.dart
	echo ERRORLEVEL=%ERRORLEVEL% dart
	cd ..
	goto fim
:erro
	echo # ERRO
:fim
