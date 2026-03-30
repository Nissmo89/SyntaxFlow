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

:: Optional: path to Python 3 Windows installation (for embedded runner)
:: Set to empty string to disable Python support.
set PYTHON_PATH=C:\Python312

:: ---- Inject tools into PATH ----
set PATH=%QT_PATH%\bin;%MINGW_PATH%\bin;%PATH%

echo.
echo ============================================================
echo  Step 1: Build TinyCC for Windows (x86_64-mingw32)
echo ============================================================

set TINYCC_DIR=%~dp0libs\tinycc
set TINYCC_WIN=%~dp0libs\tinycc\windows

if exist "%TINYCC_WIN%\libtcc.a" (
    echo   [SKIP] libtcc.a already exists at libs\tinycc\windows\
) else (
    echo   Building TinyCC from source with MinGW...
    pushd "%TINYCC_DIR%"

    :: Patch: configure wants 'cc', redirect to mingw gcc
    mingw32-make ^
        CC=x86_64-w64-mingw32-gcc ^
        AR=x86_64-w64-mingw32-ar ^
        CONFIG_WIN32=yes ^
        CONFIG_i386=no ^
        CONFIG_x86_64=yes ^
        libtcc.a libtcc1.a 2>&1

    if errorlevel 1 (
        echo.
        echo   [WARN] mingw32-make with cross prefix failed.
        echo   Trying native MinGW build via configure...
        sh configure --prefix="%TINYCC_WIN%" --sysincludepaths="%TINYCC_WIN%\include"
        sh -c "make libtcc.a libtcc1.a"
        if errorlevel 1 (
            echo   [ERROR] TinyCC build failed. Install MSYS2 bash or Git Bash.
            echo   Alternatively, pre-built libtcc.a can be obtained from:
            echo     https://github.com/bellard/tcc/releases
            echo   Place libtcc.a + libtcc1.a in: libs\tinycc\windows\
            popd
            goto :BUILD_FAILED
        )
    )

    :: Move outputs to windows/ folder
    if not exist "%TINYCC_WIN%" mkdir "%TINYCC_WIN%"
    copy /Y "libtcc.a"  "%TINYCC_WIN%\libtcc.a"  >nul
    copy /Y "libtcc1.a" "%TINYCC_WIN%\libtcc1.a" >nul

    popd
    echo   [OK] TinyCC built successfully.
)

echo.
echo ============================================================
echo  Step 2: Build QScintilla (static, Qt6, MinGW)
echo ============================================================

set QSCI_SRC=%~dp0libs\qscintilla\src
set QSCI_LIB=%QSCI_SRC%\release\libqscintilla2_qt6.a

if exist "%QSCI_LIB%" (
    echo   [SKIP] QScintilla already built at libs\qscintilla\src\release\
) else (
    echo   Building QScintilla with qmake...
    pushd "%QSCI_SRC%"
    qmake qscintilla.pro ^
        CONFIG+=staticlib ^
        CONFIG+=release ^
        CONFIG+=qt ^
        QT+=widgets
    if errorlevel 1 ( popd & goto :BUILD_FAILED )

    mingw32-make -j%NUMBER_OF_PROCESSORS%
    if errorlevel 1 ( popd & goto :BUILD_FAILED )

    popd
    echo   [OK] QScintilla built.
)

echo.
echo ============================================================
echo  Step 3: Configure SyntaxFlow with CMake
echo ============================================================

set BUILD_DIR=%~dp0build-windows

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
pushd "%BUILD_DIR%"

set CMAKE_ARGS=-G "MinGW Makefiles"
set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_BUILD_TYPE=Release
set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_PREFIX_PATH="%QT_PATH%"

if not "%PYTHON_PATH%"=="" (
    if exist "%PYTHON_PATH%\python.exe" (
        echo   Python found at %PYTHON_PATH%, enabling python_runner.
        set CMAKE_ARGS=%CMAKE_ARGS% -DPython3_ROOT_DIR="%PYTHON_PATH%"
    ) else (
        echo   [WARN] PYTHON_PATH set but python.exe not found. Python support disabled.
    )
)

cmake %CMAKE_ARGS% ..
if errorlevel 1 ( popd & goto :BUILD_FAILED )

echo.
echo ============================================================
echo  Step 4: Build SyntaxFlow
echo ============================================================

cmake --build . --config Release --parallel %NUMBER_OF_PROCESSORS%
if errorlevel 1 ( popd & goto :BUILD_FAILED )

echo.
echo ============================================================
echo  Step 5: Deploy Qt DLLs (windeployqt)
echo ============================================================

set EXE_PATH=%BUILD_DIR%\SyntaxFlow.exe
if exist "%EXE_PATH%" (
    windeployqt --release --no-translations "%EXE_PATH%"
    echo   [OK] Qt DLLs deployed.
) else (
    echo   [WARN] SyntaxFlow.exe not found at expected path: %EXE_PATH%
)

:: ---- Copy Python DLL if wanted ----
if not "%PYTHON_PATH%"=="" (
    if exist "%PYTHON_PATH%\python312.dll" (
        echo   Copying python312.dll...
        copy /Y "%PYTHON_PATH%\python312.dll" "%BUILD_DIR%\" >nul
        echo   [OK] Python DLL copied.
    )
)

popd

echo.
echo ============================================================
echo  BUILD COMPLETE
echo  Output: %BUILD_DIR%\SyntaxFlow.exe
echo ============================================================
pause
exit /b 0

:BUILD_FAILED
echo.
echo ============================================================
echo  BUILD FAILED  —  check output above for errors
echo ============================================================
pause
exit /b 1
