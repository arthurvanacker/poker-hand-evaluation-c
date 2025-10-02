#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for HandCategory Enum
 * Tests verify hand category values and ordering according to poker hand rankings
 */

void test_handcategory_values(void) {
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

void test_handcategory_ordering(void) {
    printf("Testing hand category ordering...\n");

    // Test that hand categories are properly ordered (better hands have higher values)
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

void test_handcategory_comparisons(void) {
    printf("Testing hand category comparisons...\n");

    // Test specific comparisons mentioned in acceptance criteria
    assert(HAND_ROYAL_FLUSH > HAND_FLUSH);

    // Test other important comparisons
    assert(HAND_STRAIGHT_FLUSH > HAND_FOUR_OF_A_KIND);
    assert(HAND_FULL_HOUSE > HAND_THREE_OF_A_KIND);
    assert(HAND_TWO_PAIR > HAND_ONE_PAIR);

    // Test variables
    HandCategory weak = HAND_HIGH_CARD;
    HandCategory strong = HAND_ROYAL_FLUSH;
    HandCategory mid = HAND_FLUSH;

    assert(weak < strong);
    assert(strong > weak);
    assert(mid > weak);
    assert(mid < strong);
    assert(weak == HAND_HIGH_CARD);
    assert(strong == HAND_ROYAL_FLUSH);

    printf("  ✓ Hand category comparisons work correctly\n");
}

void test_handcategory_range(void) {
    printf("Testing hand category range...\n");

    // Test boundary conditions
    assert(HAND_HIGH_CARD == 1);    // Minimum value
    assert(HAND_ROYAL_FLUSH == 10); // Maximum value

    // Verify the range is contiguous (1-10)
    HandCategory expected_value = HAND_HIGH_CARD;
    HandCategory categories[] = {
        HAND_HIGH_CARD, HAND_ONE_PAIR, HAND_TWO_PAIR, HAND_THREE_OF_A_KIND,
        HAND_STRAIGHT, HAND_FLUSH, HAND_FULL_HOUSE, HAND_FOUR_OF_A_KIND,
        HAND_STRAIGHT_FLUSH, HAND_ROYAL_FLUSH
    };

    for (int i = 0; i < 10; i++) {
        assert(categories[i] == expected_value);
        expected_value++;
    }

    printf("  ✓ Hand category range is valid and contiguous\n");
}

void test_handcategory_explicit_discriminants(void) {
    printf("Testing explicit discriminants enable direct comparison...\n");

    // Verify that explicit values allow direct numeric comparison
    // This is the key feature mentioned in the issue notes
    int royal_value = HAND_ROYAL_FLUSH;
    int flush_value = HAND_FLUSH;

    assert(royal_value > flush_value);
    assert(royal_value == 10);
    assert(flush_value == 6);

    // Test that we can use the enum in arithmetic operations
    int difference = HAND_ROYAL_FLUSH - HAND_HIGH_CARD;
    assert(difference == 9);

    printf("  ✓ Explicit discriminants work correctly\n");
}

int main(void) {
    printf("\n=== HandCategory Enum Test Suite ===\n\n");

    test_handcategory_values();
    test_handcategory_ordering();
    test_handcategory_comparisons();
    test_handcategory_range();
    test_handcategory_explicit_discriminants();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
