rd /S /Q output
mkdir output

copy build\game\Debug\Game.exe output
copy res output

python scripts/client.py post