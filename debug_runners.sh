#!/bin/bash
# Script: debug_runners.sh
# Purpose: This script is created to automatically rebuild the SyntaxFlow backend and execute our headless test runner (test_python_runner).
# Why it's created: You've mentioned that Python and other languages still say "Already running" when executed. Since I cannot reproduce the hang directly, I have injected diagnostic trace logs into the runner components (python_runner.cpp, etc.) and created a standalone test runner. This script will build that test runner and execute it so we can see precisely where QProcess hangs during WASM execution without dealing with the UI.
# How it works: It runs cmake and make inside the build-linux directory, focusing specifically on the test_python_runner target, and then executes it.

set -e

echo "============================================================"
echo " Building test_python_runner"
echo "============================================================"
mkdir -p build-linux
cd build-linux

# Re-run cmake to pick up the new test_python_runner target
cmake ..

# Build just the test target
make test_python_runner

echo "============================================================"
echo " Executing test_python_runner"
echo "============================================================"
./test_python_runner
echo "============================================================"
echo " Finished Execution. Please copy the output above."
echo "============================================================"
