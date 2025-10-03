#!/bin/bash

# Test script portability - verifies test_makefile.sh doesn't use hardcoded paths
# This test ensures the script works in any environment (local, CI, etc.)

set -e

echo "=========================================="
echo "Testing Script Portability"
echo "=========================================="
echo ""

# Test 1: Verify no hardcoded absolute paths in script
echo "[TEST 1] Checking for hardcoded absolute paths..."
if grep -q "PROJECT_ROOT=\"/home/" tests/test_makefile.sh; then
    echo "FAIL: Found hardcoded absolute path in test_makefile.sh"
    grep "PROJECT_ROOT=" tests/test_makefile.sh
    exit 1
fi
echo "PASS: No hardcoded absolute paths found"
echo ""

# Test 2: Verify script uses relative path logic
echo "[TEST 2] Verifying script uses relative paths..."
if ! grep -q 'cd.*dirname.*BASH_SOURCE' tests/test_makefile.sh; then
    echo "FAIL: Script doesn't use BASH_SOURCE for relative path resolution"
    exit 1
fi
echo "PASS: Script uses relative path resolution"
echo ""

# Test 3: Run from tests directory
echo "[TEST 3] Running test_makefile.sh from tests/ directory..."
cd tests || exit 1
if ! ./test_makefile.sh > /dev/null 2>&1; then
    echo "FAIL: Script failed when run from tests/ directory"
    exit 1
fi
cd .. || exit 1
echo "PASS: Script works from tests/ directory"
echo ""

# Test 4: Run from project root
echo "[TEST 4] Running test_makefile.sh from project root..."
if ! ./tests/test_makefile.sh > /dev/null 2>&1; then
    echo "FAIL: Script failed when run from project root"
    exit 1
fi
echo "PASS: Script works from project root"
echo ""

echo "=========================================="
echo "ALL PORTABILITY TESTS PASSED!"
echo "=========================================="
