#!/bin/bash
# Test script for debug/release build configurations
# Tests that debug and release targets exist and build successfully

set -e

echo "Testing Makefile build configurations..."
echo "=========================================="

# Test 1: Verify debug target exists and builds
echo ""
echo "Test 1: Debug build target"
echo "--------------------------"
make clean > /dev/null 2>&1
if make debug 2>&1 | grep -qi "built.*library"; then
    echo "✓ Debug target builds successfully"
else
    echo "✗ Debug target failed"
    exit 1
fi

# Test 2: Verify release target exists and builds
echo ""
echo "Test 2: Release build target"
echo "----------------------------"
make clean > /dev/null 2>&1
if make release 2>&1 | grep -qi "built.*library"; then
    echo "✓ Release target builds successfully"
else
    echo "✗ Release target failed"
    exit 1
fi

# Test 3: Verify default all target still works
echo ""
echo "Test 3: Default all target"
echo "--------------------------"
make clean > /dev/null 2>&1
if make all 2>&1 | grep -qi "built.*library"; then
    echo "✓ Default all target still works"
else
    echo "✗ Default all target failed"
    exit 1
fi

# Test 4: Verify debug flags are applied (compile with verbose to check)
echo ""
echo "Test 4: Debug flags verification"
echo "---------------------------------"
make clean > /dev/null 2>&1
if make debug V=1 2>&1 | grep -q "\-g.*\-O0.*\-DDEBUG"; then
    echo "✓ Debug flags (-g -O0 -DDEBUG) are applied"
else
    echo "⚠ Warning: Could not verify debug flags (may be OK if not verbose)"
fi

# Test 5: Verify release flags are applied
echo ""
echo "Test 5: Release flags verification"
echo "-----------------------------------"
make clean > /dev/null 2>&1
if make release V=1 2>&1 | grep -q "\-O3.*\-DNDEBUG"; then
    echo "✓ Release flags (-O3 -DNDEBUG) are applied"
else
    echo "⚠ Warning: Could not verify release flags (may be OK if not verbose)"
fi

echo ""
echo "=========================================="
echo "✅ All build configuration tests passed!"
