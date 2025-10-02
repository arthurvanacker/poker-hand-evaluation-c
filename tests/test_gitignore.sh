#!/bin/bash
# Test script to verify .gitignore patterns work correctly
# This script creates test files and verifies they are ignored by git

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${PROJECT_ROOT}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TESTS_PASSED=0
TESTS_FAILED=0

# Helper function to test if a file is ignored
test_ignored() {
    local file="$1"
    local description="$2"

    # Create parent directory if needed
    mkdir -p "$(dirname "$file")"

    # Create test file
    touch "$file"

    # Check if git ignores it
    if git check-ignore -q "$file"; then
        echo -e "${GREEN}✓${NC} PASS: $description ($file is ignored)"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}✗${NC} FAIL: $description ($file is NOT ignored)"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi

    # Clean up
    rm -f "$file"
}

# Helper function to test if a file is NOT ignored
test_not_ignored() {
    local file="$1"
    local description="$2"

    # Create parent directory if needed
    mkdir -p "$(dirname "$file")"

    # Create test file
    touch "$file"

    # Check if git tracks it
    if git check-ignore -q "$file"; then
        echo -e "${RED}✗${NC} FAIL: $description ($file should NOT be ignored)"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    else
        echo -e "${GREEN}✓${NC} PASS: $description ($file is tracked)"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    fi

    # Clean up
    rm -f "$file"
}

echo "Testing .gitignore patterns..."
echo "================================"

# Test object files
test_ignored "build/test.o" "Object files in build/ should be ignored"
test_ignored "src/test.o" "Object files in src/ should be ignored"

# Test compiled libraries
test_ignored "lib/test.a" "Static libraries should be ignored"
test_ignored "lib/test.so" "Shared libraries should be ignored"
test_ignored "lib/test.dylib" "macOS libraries should be ignored"

# Test executables in examples
test_ignored "examples/test_program" "Executables in examples/ should be ignored"
test_not_ignored "examples/test.c" "C source files in examples/ should be tracked"

# Test build directories
test_ignored "build/subdir/test.o" "Files in build/ subdirectories should be ignored"
test_ignored "lib/subdir/test.a" "Files in lib/ subdirectories should be ignored"

# Test test outputs
test_ignored "test.out" "Test output files should be ignored"
test_ignored "test_results/report.txt" "Files in test_results/ should be ignored"

# Test coverage files
test_ignored "src/test.gcda" "Coverage data files should be ignored"
test_ignored "src/test.gcno" "Coverage notes files should be ignored"
test_ignored "src/test.gcov" "Coverage report files should be ignored"
test_ignored "coverage/report.html" "Files in coverage/ should be ignored"

# Test OS-specific files
test_ignored ".DS_Store" "macOS .DS_Store should be ignored"
test_ignored "src/.DS_Store" ".DS_Store in subdirectories should be ignored"
test_ignored "Thumbs.db" "Windows Thumbs.db should be ignored"
test_ignored "src/Thumbs.db" "Thumbs.db in subdirectories should be ignored"

# Test IDE files
test_ignored ".vscode/settings.json" "VS Code files should be ignored"
test_ignored ".idea/workspace.xml" "IntelliJ IDEA files should be ignored"
test_ignored "src/test.swp" "Vim swap files should be ignored"

# Test that important files are NOT ignored
test_not_ignored "src/card.c" "Source files should be tracked"
test_not_ignored "include/poker/card.h" "Header files should be tracked"
test_not_ignored "Makefile" "Makefile should be tracked"
test_not_ignored "README.md" "Documentation should be tracked"

echo ""
echo "================================"
echo "Test Results:"
echo -e "${GREEN}Passed: $TESTS_PASSED${NC}"
echo -e "${RED}Failed: $TESTS_FAILED${NC}"
echo "================================"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
