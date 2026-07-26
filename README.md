# SyntaxFlow

SyntaxFlow is an offline-first, blazing fast programming sandbox and LeetCode-like problem solver. It runs your Python, C++, and Javascript code in a totally isolated, secure WebAssembly sandbox using Wasmer, eliminating the need for Docker containers.

## 🚀 Features
- **Offline First:** Code runs entirely on your local machine using WASM, ensuring privacy and speed.
- **WASM Powered Security:** Your code is executed safely in a sandbox using `wasmer`, `syrusakbary/quickjs`, `clang/clang`, and `python/python`.
- **Modern UI:** Built using Qt6 and WebEngine, offering a responsive CodeMirror-based editor with rich syntax highlighting.
- **Rich Output Verification:** Generates beautiful split-diffs mapping your actual vs expected outputs and stack traces natively mapped to your code.

## 🏗️ Building the Project

SyntaxFlow is built using CMake and requires Qt6. 

### Prerequisites
- CMake 3.19+
- Qt6 (Components: Core, Gui, Widgets, Network, WebEngineWidgets, WebChannel)
- C++17 Compatible Compiler (GCC, Clang, or MSVC/MinGW)

### Build Instructions (Linux / macOS)

```bash
git clone https://github.com/Nissmo89/SyntaxFlow.git
cd SyntaxFlow

mkdir build && cd build

# Important: To download Wasmer language engines (Python, Clang, JS) at build time for offline use, pass -DPRECACHE_WASM_ENGINES=ON
cmake -DPRECACHE_WASM_ENGINES=ON ..

# Compile the project
cmake --build . -j$(nproc)

# Run the app
./SyntaxFlow
```

### Build Instructions (Windows - MinGW)

```bash
git clone https://github.com/Nissmo89/SyntaxFlow.git
cd SyntaxFlow

mkdir build
cd build

# Generate Makefiles with CMake
cmake -G "MinGW Makefiles" -DPRECACHE_WASM_ENGINES=ON ..

# Compile the project
mingw32-make -j4
```

## 📦 Important: Language Engines & Offline Use

SyntaxFlow runs code via Wasmer language packages (`python/python`, `clang/clang`, `syrusakbary/quickjs`). 

**First Run Downloads:**
By default, these packages are dynamically downloaded from the Wasmer registry the *first time* you run a code snippet. Since the engines can be quite large, this first execution might take up to a minute depending on your connection. 

**True Offline Mode (Pre-caching):**
To avoid downloading engines at runtime and ensure the app is 100% ready for offline use immediately after building, configure your CMake build with the `-DPRECACHE_WASM_ENGINES=ON` flag (or check the box in Qt Creator). This adds a build step to pre-download and cache the WASM engines during compilation. Once cached, code execution takes milliseconds.
