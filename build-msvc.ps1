<#
.SYNOPSIS
    Builds and tests the msdos-cpp project using the MSVC++ toolchain.
.DESCRIPTION
    Automatically locates the Visual Studio / MSVC Build Tools installation,
    imports x64 environment variables into the current session, and coordinates
    CMake, Ninja, CTest, DOS 4.0 mapper audits, and FAT12 boot image creation.
.PARAMETER Action
    Specifies the task to execute: All, Config, Build, Test, Audit, BootImage, BootSmoke, Regenerate, Clean.
    Default is 'All'.
#>
[CmdletBinding()]
param(
    [ValidateSet('All', 'Config', 'Build', 'Test', 'Audit', 'BootImage', 'BootSmoke', 'Regenerate', 'Clean')]
    [string]$Action = 'All'
)

$ErrorActionPreference = 'Stop'

function Initialize-VcEnvironment {
    if (Get-Command cl.exe -ErrorAction SilentlyContinue) {
        Write-Host "[INFO] cl.exe already available in PATH." -ForegroundColor Cyan
        return
    }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vsPath = $null
    if (Test-Path $vswhere) {
        $vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    }

    if (-not $vsPath -or -not (Test-Path $vsPath)) {
        $candidates = @(
            "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools",
            "C:\Program Files\Microsoft Visual Studio\2022\Community",
            "C:\Program Files\Microsoft Visual Studio\2022\Enterprise",
            "C:\Program Files\Microsoft Visual Studio\2022\Professional",
            "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools"
        )
        foreach ($c in $candidates) {
            if (Test-Path $c) { $vsPath = $c; break }
        }
    }

    if (-not $vsPath) {
        throw "Could not locate a Visual Studio or MSVC Build Tools installation."
    }

    $vcvars = Join-Path $vsPath "VC\Auxiliary\Build\vcvarsall.bat"
    if (-not (Test-Path $vcvars)) {
        throw "vcvarsall.bat was not found at $vcvars"
    }

    Write-Host "[INFO] Importing MSVC x64 environment from $vsPath..." -ForegroundColor Cyan
    $envLines = cmd.exe /c "`"$vcvars`" x64 >nul && set"
    foreach ($line in $envLines) {
        if ($line -match "^([^=]+)=(.*)$") {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
        }
    }

    if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
        throw "Failed to initialize MSVC compiler in PATH."
    }
    Write-Host "[INFO] MSVC Compiler: $((Get-Command cl.exe).Source)" -ForegroundColor Green
}

$projectRoot = $PSScriptRoot

switch ($Action) {
    'Clean' {
        $buildDir = Join-Path $projectRoot 'build'
        if (Test-Path $buildDir) {
            Write-Host "[INFO] Removing $buildDir..." -ForegroundColor Yellow
            Remove-Item -Recurse -Force $buildDir
        }
        Write-Host "[SUCCESS] Clean complete." -ForegroundColor Green
    }

    'Regenerate' {
        Write-Host "[INFO] Regenerating C++ translations from assembly..." -ForegroundColor Cyan
        & node (Join-Path $projectRoot 'tools\convert.mjs') generate
        if ($LASTEXITCODE -ne 0) { throw "convert.mjs generate failed" }
    }

    'Config' {
        Initialize-VcEnvironment
        & cmake -S $projectRoot -B (Join-Path $projectRoot 'build') -G Ninja -DCMAKE_BUILD_TYPE=Release
        if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }
    }

    'Build' {
        Initialize-VcEnvironment
        $ninjaFile = Join-Path $projectRoot 'build\build.ninja'
        if (-not (Test-Path $ninjaFile)) {
            & cmake -S $projectRoot -B (Join-Path $projectRoot 'build') -G Ninja -DCMAKE_BUILD_TYPE=Release
            if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }
        }
        & cmake --build (Join-Path $projectRoot 'build')
        if ($LASTEXITCODE -ne 0) { throw "CMake build failed" }
    }

    'Test' {
        Initialize-VcEnvironment
        & ctest --test-dir (Join-Path $projectRoot 'build') --output-on-failure
        if ($LASTEXITCODE -ne 0) { throw "CTest suite failed" }
    }

    'Audit' {
        Initialize-VcEnvironment
        & cmake --build (Join-Path $projectRoot 'build') --target v4_audit
        if ($LASTEXITCODE -ne 0) { throw "DOS 4.0 audit failed" }
    }

    'BootImage' {
        Initialize-VcEnvironment
        & cmake --build (Join-Path $projectRoot 'build') --target boot_image
        if ($LASTEXITCODE -ne 0) { throw "Boot image creation failed" }
    }

    'BootSmoke' {
        Initialize-VcEnvironment
        & cmake --build (Join-Path $projectRoot 'build') --target boot_smoke_virtualbox
        if ($LASTEXITCODE -ne 0) { throw "Boot smoke test failed" }
    }

    'All' {
        Initialize-VcEnvironment

        Write-Host "`n[1/5] Configuring CMake with MSVC (Ninja, Release)..." -ForegroundColor Cyan
        & cmake -S $projectRoot -B (Join-Path $projectRoot 'build') -G Ninja -DCMAKE_BUILD_TYPE=Release
        if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }

        Write-Host "`n[2/5] Building all targets with MSVC..." -ForegroundColor Cyan
        & cmake --build (Join-Path $projectRoot 'build')
        if ($LASTEXITCODE -ne 0) { throw "CMake build failed" }

        Write-Host "`n[3/5] Running CTest suite..." -ForegroundColor Cyan
        & ctest --test-dir (Join-Path $projectRoot 'build') --output-on-failure
        if ($LASTEXITCODE -ne 0) { throw "CTest suite failed" }

        Write-Host "`n[4/5] Running DOS 4.0 mapper audit..." -ForegroundColor Cyan
        & cmake --build (Join-Path $projectRoot 'build') --target v4_audit
        if ($LASTEXITCODE -ne 0) { throw "DOS 4.0 mapper audit failed" }

        Write-Host "`n[5/5] Building bootable FAT12 image..." -ForegroundColor Cyan
        & cmake --build (Join-Path $projectRoot 'build') --target boot_image
        if ($LASTEXITCODE -ne 0) { throw "Boot image creation failed" }

        Write-Host "`n[SUCCESS] All steps completed successfully with MSVC++!" -ForegroundColor Green
    }
}

