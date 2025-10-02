#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for is_flush function
 * Tests verify flush detection for valid and invalid inputs
 */

/* Test valid flush with hearts */
void test_is_flush_hearts(void) {
    printf("Testing is_flush with hearts...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };
    assert(is_flush(cards, 5) == 1);
    printf("  ✓ Flush with hearts detected correctly\n");
}

/* Test valid flush with diamonds */
void test_is_flush_diamonds(void) {
    printf("Testing is_flush with diamonds...\n");
    Card cards[5] = {
        {RANK_THREE, SUIT_DIAMONDS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_DIAMONDS}
    };
    assert(is_flush(cards, 5) == 1);
    printf("  ✓ Flush with diamonds detected correctly\n");
}

/* Test valid flush with clubs */
void test_is_flush_clubs(void) {
    printf("Testing is_flush with clubs...\n");
    Card cards[5] = {
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_TEN, SUIT_CLUBS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_ACE, SUIT_CLUBS}
    };
    assert(is_flush(cards, 5) == 1);
    printf("  ✓ Flush with clubs detected correctly\n");
}

/* Test valid flush with spades */
void test_is_flush_spades(void) {
    printf("Testing is_flush with spades...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_SPADES},
        {RANK_FOUR, SUIT_SPADES},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_JACK, SUIT_SPADES},
        {RANK_KING, SUIT_SPADES}
    };
    assert(is_flush(cards, 5) == 1);
    printf("  ✓ Flush with spades detected correctly\n");
}

/* Test not a flush - mixed suits */
void test_not_flush_mixed_suits(void) {
    printf("Testing non-flush with mixed suits...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TEN, SUIT_HEARTS}
    };
    assert(is_flush(cards, 5) == 0);
    printf("  ✓ Mixed suits correctly identified as non-flush\n");
}

/* Test not a flush - one different suit */
void test_not_flush_one_different(void) {
    printf("Testing non-flush with one different suit...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_CLUBS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_NINE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES}  /* Different suit */
    };
    assert(is_flush(cards, 5) == 0);
    printf("  ✓ One different suit correctly identified as non-flush\n");
}

/* Test invalid input - NULL pointer */
void test_is_flush_null_pointer(void) {
    printf("Testing is_flush with NULL pointer...\n");
    assert(is_flush(NULL, 5) == 0);
    printf("  ✓ NULL pointer handled correctly\n");
}

/* Test invalid input - zero length */
void test_is_flush_zero_length(void) {
    printf("Testing is_flush with zero length...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TEN, SUIT_HEARTS}
    };
    assert(is_flush(cards, 0) == 0);
    printf("  ✓ Zero length handled correctly\n");
}

/* Test invalid input - less than 5 cards */
void test_is_flush_less_than_five(void) {
    printf("Testing is_flush with less than 5 cards...\n");
    Card cards[4] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS}
    };
    assert(is_flush(cards, 4) == 0);
    printf("  ✓ Less than 5 cards handled correctly\n");
}

/* Test invalid input - more than 5 cards */
void test_is_flush_more_than_five(void) {
    printf("Testing is_flush with more than 5 cards...\n");
    Card cards[6] = {
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_NINE, SUIT_DIAMONDS}
    };
    assert(is_flush(cards, 6) == 0);
    printf("  ✓ More than 5 cards handled correctly\n");
}

/* Test edge case - all same rank and suit */
void test_is_flush_all_same_rank(void) {
    printf("Testing is_flush with all same rank and suit...\n");
    Card cards[5] = {
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_SEVEN, SUIT_SPADES}
    };
    assert(is_flush(cards, 5) == 1);
    printf("  ✓ All same rank and suit detected correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing is_flush function - Issue #18\n");
    printf("==========================================\n\n");

    test_is_flush_hearts();
    test_is_flush_diamonds();
    test_is_flush_clubs();
    test_is_flush_spades();
    test_not_flush_mixed_suits();
    test_not_flush_one_different();
    test_is_flush_null_pointer();
    test_is_flush_zero_length();
    test_is_flush_less_than_five();
    test_is_flush_more_than_five();
    test_is_flush_all_same_rank();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
