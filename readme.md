## Platform-specific builds

### Windows (With Mingw)
```bash
git clone https://github.com/Nissmo89/SyntaxFlow.git

cd SyntaxFlow

cd libs/qscintilla/src/

qmake qscintilla.pro

mingw32-make

cd ../../../

mkdir build

cd build

cmake -G "MinGW Makefiles" ..

mingw32-make


#may you also need to copy some Qscintilla .dll file into project build folder
```
