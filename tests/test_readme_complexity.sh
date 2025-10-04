#!/bin/bash
# Test script to verify README contains algorithm complexity documentation

set -e

README_PATH="README.md"
EXIT_CODE=0

echo "Testing README for algorithm complexity documentation..."

# Check if README exists
if [ ! -f "$README_PATH" ]; then
    echo "FAIL: README.md not found"
    exit 1
fi

# Check for "Algorithm Complexity" section header
if ! grep -q "## Algorithm Complexity" "$README_PATH"; then
    echo "FAIL: Missing '## Algorithm Complexity' section header"
    EXIT_CODE=1
fi

# Check for table format
if ! grep -q "| Function | Time | Space | Notes |" "$README_PATH"; then
    echo "FAIL: Missing complexity table header"
    EXIT_CODE=1
fi

# Check for specific function documentation (at least a few key functions)
REQUIRED_FUNCTIONS=(
    "deck_shuffle"
    "deck_new"
    "deck_deal"
    "is_flush"
    "is_straight"
    "rank_counts"
    "detect_royal_flush"
    "detect_straight_flush"
    "detect_four_of_a_kind"
    "detect_full_house"
    "detect_flush"
    "detect_straight"
    "detect_three_of_a_kind"
    "detect_two_pair"
    "detect_one_pair"
    "detect_high_card"
    "card_to_string"
    "parse_card"
)

for func in "${REQUIRED_FUNCTIONS[@]}"; do
    if ! grep -q "$func" "$README_PATH"; then
        echo "FAIL: Missing complexity documentation for $func()"
        EXIT_CODE=1
    fi
done

# Check for O-notation (time complexity)
if ! grep -q "O(" "$README_PATH"; then
    echo "FAIL: Missing O-notation for complexity analysis"
    EXIT_CODE=1
fi

if [ $EXIT_CODE -eq 0 ]; then
    echo "PASS: All algorithm complexity documentation checks passed"
else
    echo "FAIL: Some algorithm complexity documentation checks failed"
fi

exit $EXIT_CODE
