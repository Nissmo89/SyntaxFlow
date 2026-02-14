@echo off

set QT_PATH=C:\Qt\6.2.0\mingw_64
set MINGW_PATH=C:\Qt\Tools\mingw810_64

set PATH=%QT_PATH%\bin;%MINGW_PATH%\bin;%PATH%

echo ===== Building QScintilla =====

cd external\QScintilla\Qt6Qt5
qmake CONFIG+=release
mingw32-make
mingw32-make install

cd ..\..\..

echo ===== Building SyntaxFlow =====

if not exist build mkdir build
cd build

cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=%QT_PATH% ..
cmake --build .

echo ===== Build Finished =====
pause

