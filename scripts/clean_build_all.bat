cls
@echo off
echo Loading Config
call ".\config.bat"

echo CMAKE_LOCATION = %CMAKE_LOCATION%
echo PROJECTDIR = %PROJECTDIR%
echo BUILDCONFIG =%BUILDCONFIG%
echo _7ZIP_EXE = %_7ZIP_EXE%

cd "%PROJECTDIR%"

echo Beginning Build (Clean)

rd /S /Q "%PROJECTDIR%\build"
echo Removed build directory

rd /S /Q "%PROJECTDIR%\output"
mkdir "%PROJECTDIR%\output"
echo Reset Output Directory

echo Running Main Build
"%CMAKE_LOCATION%" --no-warn-unused-cli "-DCMAKE_CXX_COMPILER:STRING=%PROGRAMFILES86%/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x64/cl.exe" "-DCMAKE_C_COMPILER:STRING=%PROGRAMFILES86%/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x64/cl.exe" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -H"%PROJECTDIR%" -B"%PROJECTDIR%/build" -G "Visual Studio 16 2019"
"%CMAKE_LOCATION%" --build "%PROJECTDIR%/build" --config %BUILDCONFIG% --target ALL_BUILD -- /maxcpucount:1

echo Constructing Output Folder
copy d:\ChemmoditiesParticleSim\build\game\%BUILDCONFIG%\game.exe output\
copy d:\ChemmoditiesParticleSim\res output\

echo Zipping Output
"%_7ZIP_EXE%" a game.zip output/*

echo Build Complete
