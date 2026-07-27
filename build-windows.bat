@echo off
setlocal EnableDelayedExpansion

:: ============================================================
::  SyntaxFlow — Windows Build Script
::  Compiler : MSVC (Visual Studio)
::  Qt       : 6.8.0 MSVC 2022 64-bit
:: ============================================================

:: Use CI Qt path if available, otherwise default to local path
if "%Qt6_DIR%"=="" (
    set "QT_PATH=C:\Qt\6.8.0\msvc2022_64"
) else (
    :: Extract the parent of the cmake dir or just use the bin dir
    set "QT_PATH=%Qt6_DIR%\..\..\.."
)

:: ---- Inject tools into PATH ----
set PATH=%QT_PATH%\bin;%PATH%

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

:: Use Ninja generator for faster CI builds and better integration with MSVC Dev Cmd
set CMAKE_ARGS=-G Ninja
set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_BUILD_TYPE=Release -DPRECACHE_WASM_ENGINES=ON
if not "%Qt6_DIR%"=="" (
    set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_PREFIX_PATH="%Qt6_DIR%"
) else (
    set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_PREFIX_PATH="%QT_PATH%"
)

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

:: CMake with Visual Studio generator places executables in a Release subfolder
set EXE_PATH=%BUILD_DIR%\Release\SyntaxFlow.exe
if not exist "%EXE_PATH%" (
    :: Fallback in case a single-config generator was used
    set EXE_PATH=%BUILD_DIR%\SyntaxFlow.exe
)

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
echo  Output: %BUILD_DIR%\Release\SyntaxFlow.exe
echo ============================================================
exit /b 0

:BUILD_FAILED
echo.
echo ============================================================
echo  BUILD FAILED  —  check output above for errors
echo ============================================================
exit /b 1
