#!/usr/bin/env bash
# ============================================================
#  SyntaxFlow — Linux Build Script
#  Requires: Qt6, CMake 3.19+, GCC/Clang, Python3 dev
#
#  Usage:
#    chmod +x build-linux.sh
#    ./build-linux.sh
#
#  Optional env vars:
#    QT_PATH   — path to Qt6 install (default: system Qt)
#    BUILD_DIR — output directory (default: ./build-linux)
# ============================================================
set -e  # exit on first error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${BUILD_DIR:-$SCRIPT_DIR/build-linux}"

echo ""
echo "============================================================"
echo " SyntaxFlow Linux Build"
echo "============================================================"
echo " Source  : $SCRIPT_DIR"
echo " Output  : $BUILD_DIR"
echo ""

# ---- Check prerequisites ----
check_cmd() {
    if ! command -v "$1" &>/dev/null; then
        echo "[ERROR] '$1' not found. Please install it and try again."
        exit 1
    fi
}

check_cmd cmake
check_cmd make
check_cmd qmake6 || check_cmd qmake

QMAKE_CMD=$(command -v qmake6 2>/dev/null || command -v qmake)
echo " qmake   : $QMAKE_CMD"

# ---- Step 1: Build QScintilla (if not already present) ----
echo ""
echo "============================================================"
echo " Step 1: QScintilla"
echo "============================================================"

QSCI_SRC="$SCRIPT_DIR/libs/qscintilla/src"
QSCI_LIB="$QSCI_SRC/libqscintilla2.so"

if [ -f "$QSCI_LIB" ]; then
    echo "  [SKIP] Local QScintilla .so already exists."
elif pkg-config --exists qscintilla2-qt6 2>/dev/null; then
    echo "  [SKIP] System qscintilla2-qt6 found via pkg-config."
else
    echo "  Building QScintilla from source..."
    pushd "$QSCI_SRC" > /dev/null
    "$QMAKE_CMD" qscintilla.pro CONFIG+=release QT+=widgets
    make -j"$(nproc)"
    popd > /dev/null
    echo "  [OK] QScintilla built."
fi

# ---- Step 2: Ensure TinyCC Linux libs are built ----
echo ""
echo "============================================================"
echo " Step 2: TinyCC"
echo "============================================================"

TINYCC_DIR="$SCRIPT_DIR/libs/tinycc"
TINYCC_LIB="$TINYCC_DIR/linux/libtcc.a"

if [ -f "$TINYCC_LIB" ]; then
    echo "  [SKIP] libtcc.a already exists at libs/tinycc/linux/"
else
    echo "  Building TinyCC from source..."
    pushd "$TINYCC_DIR" > /dev/null
    ./configure --prefix="$TINYCC_DIR/linux" \
                --sysincludepaths="$TINYCC_DIR/include"
    make libtcc.a libtcc1.a -j"$(nproc)"
    mkdir -p linux
    cp libtcc.a libtcc1.a \
       runmain.o bt-exe.o bt-log.o bcheck.o \
       linux/ 2>/dev/null || true
    popd > /dev/null
    echo "  [OK] TinyCC built."
fi

# ---- Step 3: CMake Configure ----
echo ""
echo "============================================================"
echo " Step 3: CMake Configure"
echo "============================================================"

mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR" > /dev/null

CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release"

if [ -n "$QT_PATH" ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_PREFIX_PATH=$QT_PATH"
    echo "  Qt prefix: $QT_PATH"
fi

cmake $CMAKE_ARGS "$SCRIPT_DIR"

# ---- Step 4: Build ----
echo ""
echo "============================================================"
echo " Step 4: Build"
echo "============================================================"

make -j"$(nproc)"

popd > /dev/null

echo ""
echo "============================================================"
echo "  BUILD COMPLETE"
echo "  Binary: $BUILD_DIR/SyntaxFlow"
echo "============================================================"
