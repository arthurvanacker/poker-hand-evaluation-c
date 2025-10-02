#!/bin/bash
# Test script to verify C project directory structure
# This validates that all required directories exist per issue #1

set -e

echo "Testing C project directory structure..."

# Test: Required directories exist
REQUIRED_DIRS=("src" "include" "tests" "examples" "lib" "build" "docs" "analysis")
FAILED=0

for dir in "${REQUIRED_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        echo "✓ $dir/ exists"
    else
        echo "✗ $dir/ missing"
        FAILED=1
    fi
done

# Test: Directories are tracked in git (have .gitkeep files)
echo ""
echo "Verifying git tracking..."
for dir in "${REQUIRED_DIRS[@]}"; do
    if [ -f "$dir/.gitkeep" ]; then
        echo "✓ $dir/.gitkeep exists"
    else
        echo "✗ $dir/.gitkeep missing"
        FAILED=1
    fi
done

if [ $FAILED -eq 0 ]; then
    echo ""
    echo "All tests passed! Project structure is complete."
    exit 0
else
    echo ""
    echo "Some tests failed. Please review the structure."
    exit 1
fi
