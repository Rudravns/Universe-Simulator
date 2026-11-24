[CmdletBinding()]
param(
    [string]$Compiler = "",
    [string]$RaylibRoot = "",
    [switch]$NoRun
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$ProjectRoot = $PSScriptRoot
Set-Location $ProjectRoot

function Find-CommandPath {
    param([string[]]$Names)

    foreach ($name in $Names) {
        $command = Get-Command $name -CommandType Application -ErrorAction SilentlyContinue |
            Select-Object -First 1
        if ($command) {
            return $command.Source
        }
    }

    return $null
}

function Find-CompilerPath {
    $compiler = Find-CommandPath @("g++.exe", "clang++.exe")
    if ($compiler) {
        return $compiler
    }

    $compilerCandidates = New-Object System.Collections.Generic.List[string]
    foreach ($root in @($env:MINGW_HOME, $env:MSYS2_ROOT, $env:MSYS2_PATH)) {
        if ($root) {
            $compilerCandidates.Add((Join-Path $root "mingw64\bin\g++.exe"))
            $compilerCandidates.Add((Join-Path $root "ucrt64\bin\g++.exe"))
            $compilerCandidates.Add((Join-Path $root "clang64\bin\clang++.exe"))
        }
    }

    foreach ($candidate in @(
        "C:\msys64\mingw64\bin\g++.exe",
        "C:\msys64\ucrt64\bin\g++.exe",
        "C:\msys64\clang64\bin\clang++.exe",
        "C:\mingw64\bin\g++.exe",
        "C:\tools\mingw64\bin\g++.exe",
        (Join-Path $env:USERPROFILE "scoop\apps\mingw\current\bin\g++.exe"),
        (Join-Path $env:USERPROFILE "scoop\apps\msys2\current\mingw64\bin\g++.exe")
    )) {
        if ($candidate) {
            $compilerCandidates.Add($candidate)
        }
    }

    foreach ($candidate in $compilerCandidates) {
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            return $candidate
        }
    }

    return $null
}

$compilerPath = $Compiler
if (-not $compilerPath) {
    $compilerPath = Find-CompilerPath
} elseif (-not (Test-Path -LiteralPath $compilerPath -PathType Leaf)) {
    $resolved = Get-Command $compilerPath -CommandType Application -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($resolved) {
        $compilerPath = $resolved.Source
    }
}

if (-not $compilerPath -or -not (Test-Path -LiteralPath $compilerPath -PathType Leaf)) {
    throw "No GCC/Clang C++ compiler was found. Install MinGW-w64 (for example via MSYS2), add its bin directory to PATH, or pass -Compiler 'C:\path\to\g++.exe'."
}

$raylibInclude = $null
$raylibLib = $null

if ($RaylibRoot) {
    $candidateInclude = Join-Path $RaylibRoot "include"
    $candidateLib = Join-Path $RaylibRoot "lib"
    if (-not (Test-Path (Join-Path $candidateInclude "raylib.h"))) {
        throw "Raylib headers were not found at '$candidateInclude'."
    }
    if (-not (Test-Path (Join-Path $candidateLib "libraylib.a"))) {
        throw "Raylib MinGW library was not found at '$candidateLib'."
    }
    $raylibInclude = $candidateInclude
    $raylibLib = $candidateLib
}
else {
    $raylibCandidates = New-Object System.Collections.Generic.List[string]

    if ($env:RAYLIB_ROOT) { $raylibCandidates.Add($env:RAYLIB_ROOT) }
    if ($env:RAYLIB_PATH) { $raylibCandidates.Add($env:RAYLIB_PATH) }

    $propertiesPath = Join-Path $ProjectRoot ".vscode/c_cpp_properties.json"
    if (Test-Path $propertiesPath) {
        $properties = Get-Content $propertiesPath -Raw | ConvertFrom-Json
        foreach ($configuration in $properties.configurations) {
            foreach ($includePath in $configuration.includePath) {
                $expanded = $includePath
                foreach ($match in [regex]::Matches($expanded, '\$\{env:([^}]+)\}')) {
                    $value = [Environment]::GetEnvironmentVariable($match.Groups[1].Value)
                    if (-not $value) {
                        $expanded = $null
                        break
                    }
                    $expanded = $expanded.Replace($match.Value, $value)
                }
                if (-not $expanded) { continue }

                $expanded = $expanded.Replace('${workspaceFolder}', $ProjectRoot)
                if ($expanded.Contains('*') -or $expanded.Contains('?')) { continue }
                $raylibCandidates.Add($expanded)
                if ((Split-Path $expanded -Leaf) -ieq "include") {
                    $raylibCandidates.Add((Split-Path $expanded -Parent))
                }
            }
        }
    }

    foreach ($root in @(
        (Join-Path $env:USERPROFILE "raylib"),
        (Join-Path $env:USERPROFILE "raylib-6.0"),
        "C:/raylib",
        "C:\msys64\mingw64",
        "C:\msys64\ucrt64"
    )) {
        if ($root) { $raylibCandidates.Add($root) }
    }

    foreach ($candidate in $raylibCandidates) {
        if (-not $candidate) { continue }

        $possibleInclude = @(
            (Join-Path $candidate "include"),
            $candidate
        )
        $possibleLib = @(
            (Join-Path $candidate "lib"),
            (Join-Path $candidate "lib64"),
            $candidate
        )

        foreach ($include in $possibleInclude) {
            if (-not (Test-Path (Join-Path $include "raylib.h"))) { continue }

            foreach ($lib in $possibleLib) {
                if (Test-Path (Join-Path $lib "libraylib.a")) {
                    $raylibInclude = $include
                    $raylibLib = $lib
                    break
                }
            }
            if ($raylibInclude) { break }
        }
        if ($raylibInclude) { break }
    }
}

if (-not $raylibInclude -or -not $raylibLib) {
    throw "Raylib was not found. Set RAYLIB_ROOT/RAYLIB_PATH, pass -RaylibRoot, or install Raylib with its MinGW include and lib directories."
}

$sourceFiles = Get-ChildItem -Path (Join-Path $ProjectRoot "src") -Filter *.cpp -File -Recurse |
    Sort-Object FullName | ForEach-Object { $_.FullName }

if ($sourceFiles.Count -eq 0) {
    throw "No C++ source files were found under '$ProjectRoot\src'."
}

$outputPath = Join-Path $ProjectRoot "main.exe"

$compilerDirectory = Split-Path -Parent $compilerPath
if (($env:PATH -split ';') -notcontains $compilerDirectory) {
    $env:PATH = "$compilerDirectory;$env:PATH"
}

$args = @("-std=c++20", "-g", "-O0", "-Wall", "-Wextra")
$args += $sourceFiles
$args += "-o"
$args += $outputPath
$args += "-I$(Join-Path $ProjectRoot 'include')"
$args += "-I$raylibInclude"
$args += "-L$raylibLib"
$args += "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm"


Write-Host "Compiling with $(Split-Path $compilerPath -Leaf)..." -ForegroundColor Cyan
& $compilerPath @args

if ($LASTEXITCODE -ne 0) {
    throw "Compilation failed with exit code $LASTEXITCODE."
}

if (-not (Test-Path $outputPath)) {
    throw "Compilation succeeded but '$outputPath' was not created."
}

Write-Host "Compilation successful: $outputPath" -ForegroundColor Green

if (-not $NoRun) {
    Write-Host "Starting Universe Simulator..." -ForegroundColor Green
    & $outputPath
}

