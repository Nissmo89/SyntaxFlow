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

# BUILD-02: check_cmd uses "exit 1" which terminates the whole script from a
# function, so "check_cmd qmake6 || check_cmd qmake" never evaluates the rhs.
# Use command -v directly for the fallback detection.
QMAKE_CMD=$(command -v qmake6 2>/dev/null || command -v qmake 2>/dev/null)
if [ -z "$QMAKE_CMD" ]; then
    echo "[ERROR] Neither 'qmake6' nor 'qmake' found."
    echo "        Install Qt6: sudo apt install qt6-base-dev"
    exit 1
fi
echo " qmake   : $QMAKE_CMD"

# ---- Step 1: QScintilla (SKIPPED — Replaced by WebEngine & CodeMirror) ----
echo ""
echo "============================================================"
echo " Step 1: QScintilla (SKIPPED — Removed)"
echo "============================================================"
echo "  [SKIP] QScintilla replaced by QWebEngine & CodeMirror."

# ---- Step 2: Skip TinyCC (Removed from CMake) ----
echo ""
echo "============================================================"
echo " Step 2: TinyCC (SKIPPED — Removed from CMake)"
echo "============================================================"
echo "  [SKIP] TinyCC is disabled."

# ---- Step 3: CMake Configure ----
echo ""
echo "============================================================"
echo " Step 3: CMake Configure"
echo "============================================================"

mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR" > /dev/null

CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release -DPRECACHE_WASM_ENGINES=ON"

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
