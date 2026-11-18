$raylib = if ([string]::IsNullOrWhiteSpace($env:RAYLIB_PATH)) {
    Join-Path $env:USERPROFILE "raylib-6.0_win64_mingw-w64\raylib-6.0_win64_mingw-w64"
} else {
    $env:RAYLIB_PATH
}
$raylibInclude = Join-Path $raylib "include"
$raylibLib = Join-Path $raylib "lib"
$raylibHeader = Join-Path $raylibInclude "raylib.h"
$output = Join-Path $PSScriptRoot "main.exe"
$compilerCommand = Get-Command g++ -ErrorAction SilentlyContinue
$compiler = if ($compilerCommand) {
    $compilerCommand.Source
} else {
    Join-Path $env:USERPROFILE "cpp\mingw64\bin\g++.exe"
}

Clear-Host

Write-Host "Building Universe Simulator..." -ForegroundColor Cyan

if (-not (Test-Path $compiler)) {
    Write-Error "GCC compiler not found. Add g++ to PATH or install it under '$env:USERPROFILE\cpp\mingw64\bin'."
    exit 1
}

if (-not (Test-Path $raylibHeader)) {
    Write-Error "Raylib header not found at '$raylibHeader'. Set RAYLIB_PATH to the Raylib installation directory."
    exit 1
}

Write-Host "Using Raylib headers: $raylibInclude"

# Compile
& $compiler -std=c++20 -Wall -Wextra -g `
    -I (Join-Path $PSScriptRoot "include") `
    -I $raylibInclude `
    (Join-Path $PSScriptRoot "src\main.cpp") `
    (Join-Path $PSScriptRoot "src\windows\window.cpp") `
    (Join-Path $PSScriptRoot "src\Planets\planet.cpp") `
    -o $output `
    -L $raylibLib `
    -lraylib -lopengl32 -lgdi32 -lwinmm

# Check compilation result
if ($LASTEXITCODE -eq 0) {
    Write-Host "`nBuild successful!" -ForegroundColor Green
    Write-Host "Starting Universe Simulator..." -ForegroundColor Cyan

    & $output
}
else {
    Write-Host "`nBuild failed. Check the errors above." -ForegroundColor Red
    exit $LASTEXITCODE
}