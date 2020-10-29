cls
echo "Beginning Build (Clean)"

rd /S /Q d:\ChemmoditiesParticleSim\build
rd /S /Q d:\ChemmoditiesParticleSim\output
mkdir output

"D:\Program Files\CMake\bin\cmake.EXE" --no-warn-unused-cli "-DCMAKE_CXX_COMPILER:STRING=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x64/cl.exe" "-DCMAKE_C_COMPILER:STRING=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x64/cl.exe" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -Hd:/ChemmoditiesParticleSim -Bd:/ChemmoditiesParticleSim/build -G "Visual Studio 16 2019"
"D:\Program Files\CMake\bin\cmake.EXE" --build d:/ChemmoditiesParticleSim/build --config Debug --target ALL_BUILD -- /maxcpucount:1

move d:\ChemmoditiesParticleSim\build\game\Debug\game.exe d:\ChemmoditiesParticleSim\output
copy d:\ChemmoditiesParticleSim\res output\

"D:\Program Files\7-Zip\7z.exe" a game.zip output/*
