rd /S /Q output
mkdir output

copy build\game\Release\Game.exe output
xcopy /E res output

python scripts/client.py post