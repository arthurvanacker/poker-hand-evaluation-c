#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for HandCategory Enum
 * Tests verify hand category values and ordering according to poker rules
 */

void test_hand_category_values(void) {
    printf("Testing hand category values...\n");

    // Test that all hand category values match their expected numeric values
    assert(HAND_HIGH_CARD == 1);
    assert(HAND_ONE_PAIR == 2);
    assert(HAND_TWO_PAIR == 3);
    assert(HAND_THREE_OF_A_KIND == 4);
    assert(HAND_STRAIGHT == 5);
    assert(HAND_FLUSH == 6);
    assert(HAND_FULL_HOUSE == 7);
    assert(HAND_FOUR_OF_A_KIND == 8);
    assert(HAND_STRAIGHT_FLUSH == 9);
    assert(HAND_ROYAL_FLUSH == 10);

    printf("  ✓ All hand category values correct\n");
}

void test_hand_category_ordering(void) {
    printf("Testing hand category ordering...\n");

    // Test that hand categories are properly ordered (higher is better)
    assert(HAND_HIGH_CARD < HAND_ONE_PAIR);
    assert(HAND_ONE_PAIR < HAND_TWO_PAIR);
    assert(HAND_TWO_PAIR < HAND_THREE_OF_A_KIND);
    assert(HAND_THREE_OF_A_KIND < HAND_STRAIGHT);
    assert(HAND_STRAIGHT < HAND_FLUSH);
    assert(HAND_FLUSH < HAND_FULL_HOUSE);
    assert(HAND_FULL_HOUSE < HAND_FOUR_OF_A_KIND);
    assert(HAND_FOUR_OF_A_KIND < HAND_STRAIGHT_FLUSH);
    assert(HAND_STRAIGHT_FLUSH < HAND_ROYAL_FLUSH);

    printf("  ✓ Hand category ordering correct\n");
}

void test_hand_category_comparisons(void) {
    printf("Testing hand category comparisons...\n");

    // Test specific comparisons mentioned in acceptance criteria
    assert(HAND_ROYAL_FLUSH > HAND_FLUSH);
    assert(HAND_FLUSH < HAND_ROYAL_FLUSH);

    // Test that enum can be used in comparisons
    HandCategory low = HAND_HIGH_CARD;
    HandCategory high = HAND_ROYAL_FLUSH;
    HandCategory mid = HAND_STRAIGHT;

    assert(low < high);
    assert(high > low);
    assert(mid > low);
    assert(mid < high);
    assert(low == HAND_HIGH_CARD);
    assert(high == HAND_ROYAL_FLUSH);

    printf("  ✓ Hand category comparisons work correctly\n");
}

void test_hand_category_range(void) {
    printf("Testing hand category range...\n");

    // Test boundary conditions
    assert(HAND_HIGH_CARD == 1);      // Minimum hand category
    assert(HAND_ROYAL_FLUSH == 10);   // Maximum hand category

    // Verify the range is contiguous
    HandCategory expected_value = HAND_HIGH_CARD;
    HandCategory categories[] = {
        HAND_HIGH_CARD, HAND_ONE_PAIR, HAND_TWO_PAIR,
        HAND_THREE_OF_A_KIND, HAND_STRAIGHT, HAND_FLUSH,
        HAND_FULL_HOUSE, HAND_FOUR_OF_A_KIND,
        HAND_STRAIGHT_FLUSH, HAND_ROYAL_FLUSH
    };

    for (int i = 0; i < 10; i++) {
        assert(categories[i] == expected_value);
        expected_value++;
    }

    printf("  ✓ Hand category range is valid and contiguous\n");
}

void test_hand_category_explicit_values(void) {
    printf("Testing explicit values enable direct comparison...\n");

    // Verify that explicit discriminants enable numeric comparison
    // This is important for hand evaluation logic
    assert((int)HAND_ROYAL_FLUSH == 10);
    assert((int)HAND_STRAIGHT_FLUSH == 9);
    assert((int)HAND_FOUR_OF_A_KIND == 8);
    assert((int)HAND_FULL_HOUSE == 7);
    assert((int)HAND_FLUSH == 6);
    assert((int)HAND_STRAIGHT == 5);
    assert((int)HAND_THREE_OF_A_KIND == 4);
    assert((int)HAND_TWO_PAIR == 3);
    assert((int)HAND_ONE_PAIR == 2);
    assert((int)HAND_HIGH_CARD == 1);

    printf("  ✓ Explicit values allow direct comparison\n");
}

int main(void) {
    printf("\n=== HandCategory Enum Test Suite ===\n\n");

    test_hand_category_values();
    test_hand_category_ordering();
    test_hand_category_comparisons();
    test_hand_category_range();
    test_hand_category_explicit_values();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
