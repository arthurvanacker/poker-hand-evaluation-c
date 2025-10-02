#!/bin/bash
#
# Valgrind Memory Leak Test Script for deck_free()
#
# This script runs the deck test suite under Valgrind to verify
# that deck_free() properly deallocates all memory without leaks.
#
# Usage: ./tests/valgrind_test.sh
#
# Expected output: "All heap blocks were freed -- no leaks are possible"
#

set -e

# Build the project
echo "Building project..."
make clean
make

# Build test executable
echo "Building test executable..."
gcc -Wall -Wextra -std=c99 -Iinclude tests/test_deck.c lib/libpoker.a -o build/test_deck

# Run tests under Valgrind
echo ""
echo "Running tests under Valgrind..."
echo "================================"
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --error-exitcode=1 \
         ./build/test_deck

echo ""
echo "================================"
echo "Valgrind verification complete!"
echo ""
echo "If you see 'All heap blocks were freed -- no leaks are possible'"
echo "then deck_free() correctly deallocates all memory."
