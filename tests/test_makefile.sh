#!/bin/bash

# Test script for Makefile functionality
# This script verifies all acceptance criteria for issue #2

set -e

PROJECT_ROOT="/home/puupa/Projects/Poker/HandEvaluation_C"
cd "$PROJECT_ROOT"

echo "=========================================="
echo "Testing Makefile - Issue #2"
echo "=========================================="
echo ""

# Test 1: Makefile exists
echo "[TEST 1] Checking Makefile exists..."
if [ ! -f "Makefile" ]; then
    echo "FAIL: Makefile not found"
    exit 1
fi
echo "PASS: Makefile exists"
echo ""

# Test 2: Check for required variables
echo "[TEST 2] Checking required variables..."
if ! grep -q "CC = gcc" Makefile; then
    echo "FAIL: CC = gcc not found"
    exit 1
fi
echo "PASS: CC = gcc found"

if ! grep -q "CFLAGS.*-Wall.*-Wextra.*-std=c99.*-Iinclude" Makefile; then
    echo "FAIL: CFLAGS not properly defined"
    exit 1
fi
echo "PASS: CFLAGS properly defined"
echo ""

# Test 3: Check for source files definition
echo "[TEST 3] Checking source files definition..."
if ! grep -q "SRC.*card.c.*deck.c.*evaluator.c" Makefile; then
    echo "FAIL: SRC not properly defined"
    exit 1
fi
echo "PASS: SRC definition found"
echo ""

# Test 4: Check for object files definition
echo "[TEST 4] Checking object files definition..."
if ! grep -q "OBJ.*=.*\$(SRC:src/%.c=build/%.o)" Makefile; then
    echo "FAIL: OBJ not properly defined"
    exit 1
fi
echo "PASS: OBJ definition found"
echo ""

# Test 5: Check for required targets
echo "[TEST 5] Checking required targets..."
required_targets=("all" "test" "examples" "clean" "install")
for target in "${required_targets[@]}"; do
    if ! grep -q "^${target}:" Makefile; then
        echo "FAIL: Target '${target}:' not found"
        exit 1
    fi
    echo "PASS: Target '${target}:' found"
done
echo ""

# Test 6: Clean before build
echo "[TEST 6] Cleaning build artifacts..."
make clean > /dev/null 2>&1 || true
echo "PASS: Clean executed"
echo ""

# Test 7: Test 'make all' target
echo "[TEST 7] Testing 'make all' target..."
if ! make all 2>&1; then
    echo "FAIL: 'make all' failed"
    exit 1
fi
echo "PASS: 'make all' succeeded"
echo ""

# Test 8: Check library was created
echo "[TEST 8] Checking lib/libpoker.a was created..."
if [ ! -f "lib/libpoker.a" ]; then
    echo "FAIL: lib/libpoker.a not created"
    exit 1
fi
echo "PASS: lib/libpoker.a created"
echo ""

# Test 9: Check object files in build directory
echo "[TEST 9] Checking object files in build directory..."
for src_file in card deck evaluator; do
    if [ ! -f "build/${src_file}.o" ]; then
        echo "FAIL: build/${src_file}.o not created"
        exit 1
    fi
    echo "PASS: build/${src_file}.o created"
done
echo ""

# Test 10: Test clean removes artifacts
echo "[TEST 10] Testing 'make clean' removes artifacts..."
make clean > /dev/null 2>&1
if [ -f "lib/libpoker.a" ] || [ -f "build/card.o" ]; then
    echo "FAIL: 'make clean' did not remove artifacts"
    exit 1
fi
echo "PASS: 'make clean' removed all artifacts"
echo ""

echo "=========================================="
echo "ALL TESTS PASSED!"
echo "=========================================="
