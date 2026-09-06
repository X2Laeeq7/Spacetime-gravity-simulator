@echo off
echo Compiling Gravity Sheet...
g++ src/main.cpp -o gravity_sheet.exe -I C:\raylib\include -L C:\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm
if %errorlevel% == 0 (
    echo Success! Running program...
    gravity_sheet.exe
) else (
    echo Compilation failed!
    pause
)