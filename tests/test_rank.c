#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for Rank Enum
 * Tests verify rank values and ordering according to poker rules
 */

void test_rank_values(void) {
    printf("Testing rank values...\n");

    // Test that all rank values match their expected numeric values
    assert(RANK_TWO == 2);
    assert(RANK_THREE == 3);
    assert(RANK_FOUR == 4);
    assert(RANK_FIVE == 5);
    assert(RANK_SIX == 6);
    assert(RANK_SEVEN == 7);
    assert(RANK_EIGHT == 8);
    assert(RANK_NINE == 9);
    assert(RANK_TEN == 10);
    assert(RANK_JACK == 11);
    assert(RANK_QUEEN == 12);
    assert(RANK_KING == 13);
    assert(RANK_ACE == 14);

    printf("  ✓ All rank values correct\n");
}

void test_rank_ordering(void) {
    printf("Testing rank ordering...\n");

    // Test that ranks are properly ordered
    assert(RANK_TWO < RANK_THREE);
    assert(RANK_THREE < RANK_FOUR);
    assert(RANK_FOUR < RANK_FIVE);
    assert(RANK_FIVE < RANK_SIX);
    assert(RANK_SIX < RANK_SEVEN);
    assert(RANK_SEVEN < RANK_EIGHT);
    assert(RANK_EIGHT < RANK_NINE);
    assert(RANK_NINE < RANK_TEN);
    assert(RANK_TEN < RANK_JACK);
    assert(RANK_JACK < RANK_QUEEN);
    assert(RANK_QUEEN < RANK_KING);
    assert(RANK_KING < RANK_ACE);

    printf("  ✓ Rank ordering correct\n");
}

void test_rank_comparisons(void) {
    printf("Testing rank comparisons...\n");

    // Test that enum can be used in comparisons
    Rank low = RANK_TWO;
    Rank high = RANK_ACE;
    Rank mid = RANK_SEVEN;

    assert(low < high);
    assert(high > low);
    assert(mid > low);
    assert(mid < high);
    assert(low == RANK_TWO);
    assert(high == RANK_ACE);

    printf("  ✓ Rank comparisons work correctly\n");
}

void test_rank_range(void) {
    printf("Testing rank range...\n");

    // Test boundary conditions
    assert(RANK_TWO == 2);   // Minimum rank
    assert(RANK_ACE == 14);  // Maximum rank

    // Verify the range is contiguous
    Rank expected_value = RANK_TWO;
    Rank ranks[] = {
        RANK_TWO, RANK_THREE, RANK_FOUR, RANK_FIVE, RANK_SIX,
        RANK_SEVEN, RANK_EIGHT, RANK_NINE, RANK_TEN, RANK_JACK,
        RANK_QUEEN, RANK_KING, RANK_ACE
    };

    for (int i = 0; i < 13; i++) {
        assert(ranks[i] == expected_value);
        expected_value++;
    }

    printf("  ✓ Rank range is valid and contiguous\n");
}

int main(void) {
    printf("\n=== Rank Enum Test Suite ===\n\n");

    test_rank_values();
    test_rank_ordering();
    test_rank_comparisons();
    test_rank_range();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
