# Clear screen     
Clear-Host

# Compile all source files across subdirectories
g++ -std=c++20 `
    src/main.cpp `
    src/Planets/planet.cpp `
    src/windows/window.cpp `
    src/Planets/celestial.cpp `
    src/Physics/Planets/planethandler.cpp src/windows/plane.cpp `
    -o main.exe `
    -I"include" `
    -I"C:/Users/kumar1272/raylib-6.0_win64_mingw-w64/raylib-6.0_win64_mingw-w64/include" `
    -L"C:/Users/kumar1272/raylib-6.0_win64_mingw-w64/raylib-6.0_win64_mingw-w64/lib" `
    -lraylib -lopengl32 -lgdi32 -lwinmm

# Run if compilation succeeds
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilation successful! Running main.exe..." -ForegroundColor Green
    .\main.exe
} else {
    Write-Host "Compilation failed." -ForegroundColor Red
}

# PlayStation 1