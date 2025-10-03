#!/bin/bash

# Test cppcheck configuration in Makefile
# Verifies that unusedFunction warnings are suppressed for library code

set -e

# Get project root directory (parent of tests/)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

echo "=========================================="
echo "Testing cppcheck Configuration"
echo "=========================================="
echo ""

# Test 1: Verify lint target exists
echo "[TEST 1] Checking lint target exists..."
if ! grep -q "^lint:" Makefile; then
    echo "FAIL: lint target not found in Makefile"
    exit 1
fi
echo "PASS: lint target exists"
echo ""

# Test 2: Verify cppcheck is configured
echo "[TEST 2] Checking cppcheck is configured..."
if ! grep -q "cppcheck.*--enable=all" Makefile; then
    echo "FAIL: cppcheck not properly configured"
    exit 1
fi
echo "PASS: cppcheck configured with --enable=all"
echo ""

# Test 3: Verify unusedFunction suppression is present
echo "[TEST 3] Checking unusedFunction suppression..."
if ! grep -q "suppress=unusedFunction" Makefile; then
    echo "FAIL: unusedFunction suppression not found in Makefile"
    echo "Expected: --suppress=unusedFunction in cppcheck command"
    exit 1
fi
echo "PASS: unusedFunction suppression configured"
echo ""

# Test 4: Verify missingIncludeSystem suppression still present
echo "[TEST 4] Checking missingIncludeSystem suppression..."
if ! grep -q "suppress=missingIncludeSystem" Makefile; then
    echo "FAIL: missingIncludeSystem suppression missing"
    exit 1
fi
echo "PASS: missingIncludeSystem suppression still configured"
echo ""

# Test 5: Verify error-exitcode is set
echo "[TEST 5] Checking error-exitcode configuration..."
if ! grep -q "error-exitcode=1" Makefile; then
    echo "FAIL: error-exitcode=1 not found"
    exit 1
fi
echo "PASS: error-exitcode=1 configured"
echo ""

echo "=========================================="
echo "ALL CPPCHECK CONFIG TESTS PASSED!"
echo "=========================================="
