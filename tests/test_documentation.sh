#!/bin/bash
# Test script to verify documentation structure
# This validates acceptance criteria for issue #6

set -e

echo "Testing project documentation structure..."
echo ""

FAILED=0

# Test 1: Verify /docs/ folder exists (created in issue #47)
echo "Test 1: Verifying /docs/ folder exists"
if [ -d "docs" ]; then
    echo "✓ docs/ folder exists"
else
    echo "✗ docs/ folder missing"
    FAILED=1
fi
echo ""

# Test 2: Verify /analysis/ folder exists (created in issue #47)
echo "Test 2: Verifying /analysis/ folder exists"
if [ -d "analysis" ]; then
    echo "✓ analysis/ folder exists"
else
    echo "✗ analysis/ folder missing"
    FAILED=1
fi
echo ""

# Test 3: Verify ALGORITHM_EXPLAINED.md placeholder exists
echo "Test 3: Verifying ALGORITHM_EXPLAINED.md placeholder exists"
if [ -f "docs/ALGORITHM_EXPLAINED.md" ]; then
    echo "✓ docs/ALGORITHM_EXPLAINED.md exists"
    # Check that it mentions Phase 05
    if grep -q "Phase 05" "docs/ALGORITHM_EXPLAINED.md"; then
        echo "✓ ALGORITHM_EXPLAINED.md mentions Phase 05"
    else
        echo "✗ ALGORITHM_EXPLAINED.md does not mention Phase 05"
        FAILED=1
    fi
else
    echo "✗ docs/ALGORITHM_EXPLAINED.md missing"
    FAILED=1
fi
echo ""

# Test 4: Verify MEMORY_SAFETY.md placeholder exists
echo "Test 4: Verifying MEMORY_SAFETY.md placeholder exists"
if [ -f "docs/MEMORY_SAFETY.md" ]; then
    echo "✓ docs/MEMORY_SAFETY.md exists"
    # Check that it mentions Phase 05
    if grep -q "Phase 05" "docs/MEMORY_SAFETY.md"; then
        echo "✓ MEMORY_SAFETY.md mentions Phase 05"
    else
        echo "✗ MEMORY_SAFETY.md does not mention Phase 05"
        FAILED=1
    fi
else
    echo "✗ docs/MEMORY_SAFETY.md missing"
    FAILED=1
fi
echo ""

# Test 5: Verify README has documentation section
echo "Test 5: Verifying README has documentation section"
if [ -f "README.md" ]; then
    echo "✓ README.md exists"
    # Check for documentation section
    if grep -q "## Documentation" "README.md"; then
        echo "✓ README.md contains ## Documentation section"
        # Check that it mentions future documentation
        if grep -qi "documentation.*complete\|documentation.*phase\|documentation.*added" "README.md"; then
            echo "✓ README.md mentions future documentation"
        else
            echo "✗ README.md does not mention future documentation"
            FAILED=1
        fi
    else
        echo "✗ README.md missing ## Documentation section"
        FAILED=1
    fi
else
    echo "✗ README.md missing"
    FAILED=1
fi
echo ""

# Final result
if [ $FAILED -eq 0 ]; then
    echo "======================================"
    echo "All tests passed! ✓"
    echo "Documentation structure is complete."
    echo "======================================"
    exit 0
else
    echo "======================================"
    echo "Some tests failed. ✗"
    echo "Please review the documentation structure."
    echo "======================================"
    exit 1
fi
