@echo off
setlocal enabledelayedexpansion

set "ACTION=%~1"
if "%ACTION%"=="" set "ACTION=all"

:: Locate Visual Studio / MSVC Build Tools via vswhere
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
        set "VS_PATH=%%i"
    )
)

if not defined VS_PATH (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools" (
        set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community" (
        set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise" (
        set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional" (
        set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional"
    ) else (
        echo [ERROR] Visual Studio / MSVC Build Tools not found.
        exit /b 1
    )
)

set "VCVARS=%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
if not exist "%VCVARS%" (
    echo [ERROR] vcvarsall.bat not found at "%VCVARS%"
    exit /b 1
)

echo [INFO] Initializing MSVC x64 environment from %VS_PATH%...
call "%VCVARS%" x64 >nul || (
    echo [ERROR] Failed to initialize MSVC environment.
    exit /b 1
)

if /i "%ACTION%"=="clean" (
    echo [INFO] Cleaning build directory...
    if exist build rmdir /s /q build
    echo [INFO] Clean complete.
    exit /b 0
)

if /i "%ACTION%"=="config" (
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
    exit /b %ERRORLEVEL%
)

if /i "%ACTION%"=="build" (
    if not exist build\build.ninja (
        cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1
    )
    cmake --build build
    exit /b %ERRORLEVEL%
)

if /i "%ACTION%"=="test" (
    ctest --test-dir build --output-on-failure
    exit /b %ERRORLEVEL%
)

if /i "%ACTION%"=="audit" (
    cmake --build build --target v4_audit
    exit /b %ERRORLEVEL%
)

if /i "%ACTION%"=="boot" (
    cmake --build build --target boot_image
    exit /b %ERRORLEVEL%
)

if /i "%ACTION%"=="bootsmoke" (
    cmake --build build --target boot_smoke_virtualbox
    exit /b %ERRORLEVEL%
)

if /i "%ACTION%"=="all" (
    echo [1/5] Configuring CMake with MSVC...
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1
    echo [2/5] Building all targets...
    cmake --build build || exit /b 1
    echo [3/5] Running CTest suite...
    ctest --test-dir build --output-on-failure || exit /b 1
    echo [4/5] Running DOS 4.0 mapper audit...
    cmake --build build --target v4_audit || exit /b 1
    echo [5/5] Building bootable FAT12 image...
    cmake --build build --target boot_image || exit /b 1
    echo [SUCCESS] MSVC build, tests, audit, and boot image completed successfully!
    exit /b 0
)

echo Unknown action: %ACTION%
echo Usage: build-msvc.bat [all^|config^|build^|test^|audit^|boot^|bootsmoke^|clean]
exit /b 1

