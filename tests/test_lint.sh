#!/bin/bash
# Test script for 'make lint' target
# Validates that cppcheck static analysis infrastructure is correctly configured

echo "=========================================="
echo "Testing 'make lint' target infrastructure"
echo "=========================================="
echo ""

# Test 1: Verify 'make lint' target exists
echo "Test 1: Checking if 'make lint' target exists..."
if ! make -n lint > /dev/null 2>&1; then
    echo "❌ FAIL: 'make lint' target not found in Makefile"
    exit 1
fi
echo "✓ PASS: 'make lint' target exists"
echo ""

# Test 2: Verify cppcheck command is properly configured
echo "Test 2: Checking cppcheck command configuration..."
MAKE_OUTPUT=$(make -n lint 2>&1)
if ! echo "$MAKE_OUTPUT" | grep -q "cppcheck"; then
    echo "❌ FAIL: 'make lint' does not invoke cppcheck"
    exit 1
fi
echo "✓ PASS: 'make lint' invokes cppcheck"
echo ""

# Test 3: Verify --enable=all flag is used
echo "Test 3: Checking if --enable=all flag is configured..."
if ! echo "$MAKE_OUTPUT" | grep -q "\-\-enable=all"; then
    echo "❌ FAIL: cppcheck does not use --enable=all flag"
    exit 1
fi
echo "✓ PASS: cppcheck uses --enable=all flag"
echo ""

# Test 4: Verify --error-exitcode=1 flag is used
echo "Test 4: Checking if --error-exitcode=1 flag is configured..."
if ! echo "$MAKE_OUTPUT" | grep -q "\-\-error-exitcode=1"; then
    echo "❌ FAIL: cppcheck does not use --error-exitcode=1 flag"
    exit 1
fi
echo "✓ PASS: cppcheck uses --error-exitcode=1 flag"
echo ""

# Test 5: Verify --suppress=missingIncludeSystem is used
echo "Test 5: Checking if --suppress=missingIncludeSystem is configured..."
if ! echo "$MAKE_OUTPUT" | grep -q "\-\-suppress=missingIncludeSystem"; then
    echo "❌ FAIL: cppcheck does not suppress missingIncludeSystem warnings"
    exit 1
fi
echo "✓ PASS: cppcheck suppresses missingIncludeSystem warnings"
echo ""

# Test 6: Verify -I include/ is used
echo "Test 6: Checking if -I include/ flag is configured..."
if ! echo "$MAKE_OUTPUT" | grep -q "\-I include/"; then
    echo "❌ FAIL: cppcheck does not use -I include/ flag"
    exit 1
fi
echo "✓ PASS: cppcheck uses -I include/ flag"
echo ""

# Test 7: Verify src/ directory is scanned
echo "Test 7: Checking if src/ directory is scanned..."
if ! echo "$MAKE_OUTPUT" | grep -q "src/"; then
    echo "❌ FAIL: cppcheck does not scan src/ directory"
    exit 1
fi
echo "✓ PASS: cppcheck scans src/ directory"
echo ""

# Test 8: Check if cppcheck is installed (informational only)
echo "Test 8: Checking if cppcheck is installed..."
if ! command -v cppcheck > /dev/null 2>&1; then
    echo "⚠ WARNING: cppcheck not found locally"
    echo "   This is OK - cppcheck will be installed in CI/CD"
    echo "   Install locally with: sudo apt-get install cppcheck"
    echo ""
    echo "=========================================="
    echo "✅ All infrastructure tests passed!"
    echo "   (cppcheck not installed locally - CI will run full checks)"
    echo "=========================================="
    exit 0
fi
echo "✓ PASS: cppcheck is installed"
echo ""

# Test 9: Run 'make lint' if cppcheck is available
echo "Test 9: Running 'make lint' with cppcheck..."
if ! make lint; then
    echo "❌ FAIL: 'make lint' returned non-zero exit code"
    echo "   cppcheck found issues that need to be fixed"
    exit 1
fi
echo "✓ PASS: 'make lint' completed successfully with no issues"
echo ""

echo "=========================================="
echo "✅ All lint tests passed!"
echo "=========================================="
