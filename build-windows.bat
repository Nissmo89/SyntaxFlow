@echo off
setlocal EnableDelayedExpansion

:: ============================================================
::  SyntaxFlow — Windows Build Script
::  Compiler : MinGW-w64 (x86_64) bundled with Qt 6.2.0
::  Qt       : 6.2.0 MinGW 64-bit
::
::  Edit the two paths below to match your installation.
:: ============================================================

set QT_PATH=C:\Qt\6.2.0\mingw_64
set MINGW_PATH=C:\Qt\Tools\mingw810_64

:: ---- Inject tools into PATH ----
set PATH=%QT_PATH%\bin;%MINGW_PATH%\bin;%PATH%

echo.
echo ============================================================
echo  Step 1: QScintilla (SKIPPED — Replaced by WebEngine ^& CodeMirror)
echo ============================================================
echo   [SKIP] QScintilla replaced by QWebEngine ^& CodeMirror.

echo.
echo ============================================================
echo  Step 2: Configure SyntaxFlow with CMake
echo ============================================================

set BUILD_DIR=%~dp0build-windows

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
pushd "%BUILD_DIR%"

set CMAKE_ARGS=-G "MinGW Makefiles"
set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_BUILD_TYPE=Release
set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_PREFIX_PATH="%QT_PATH%"

cmake %CMAKE_ARGS% ..
if errorlevel 1 ( popd & goto :BUILD_FAILED )

echo.
echo ============================================================
echo  Step 3: Build SyntaxFlow
echo ============================================================

cmake --build . --config Release --parallel %NUMBER_OF_PROCESSORS%
if errorlevel 1 ( popd & goto :BUILD_FAILED )

echo.
echo ============================================================
echo  Step 4: Deploy Qt DLLs (windeployqt)
echo ============================================================

set EXE_PATH=%BUILD_DIR%\SyntaxFlow.exe
if exist "%EXE_PATH%" (
    windeployqt --release --no-translations "%EXE_PATH%"
    echo   [OK] Qt DLLs deployed.
) else (
    echo   [WARN] SyntaxFlow.exe not found at expected path: %EXE_PATH%
)

popd

echo.
echo ============================================================
echo  BUILD COMPLETE
echo  Output: %BUILD_DIR%\SyntaxFlow.exe
echo ============================================================
exit /b 0

:BUILD_FAILED
echo.
echo ============================================================
echo  BUILD FAILED  —  check output above for errors
echo ============================================================
exit /b 1
