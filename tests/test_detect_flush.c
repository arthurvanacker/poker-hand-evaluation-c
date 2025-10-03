#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_flush function
 * Tests verify flush detection (5 suited cards, non-sequential)
 */

/* Test flush in hearts (Ace-high) */
void test_flush_hearts_ace_high(void) {
    printf("Testing flush in hearts (Ace-high)...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS},
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_TWO, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Tiebreakers should be in descending order */
    assert(tiebreakers[0] == RANK_ACE);
    assert(tiebreakers[1] == RANK_JACK);
    assert(tiebreakers[2] == RANK_NINE);
    assert(tiebreakers[3] == RANK_FIVE);
    assert(tiebreakers[4] == RANK_TWO);
    printf("  Pass: Flush in hearts detected with correct tiebreakers\n");
}

/* Test flush in diamonds */
void test_flush_diamonds(void) {
    printf("Testing flush in diamonds...\n");
    Card cards[5] = {
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_DIAMONDS},
        {RANK_THREE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    assert(tiebreakers[0] == RANK_KING);
    assert(tiebreakers[1] == RANK_TEN);
    assert(tiebreakers[2] == RANK_SEVEN);
    assert(tiebreakers[3] == RANK_FOUR);
    assert(tiebreakers[4] == RANK_THREE);
    printf("  Pass: Flush in diamonds detected with correct tiebreakers\n");
}

/* Test flush in clubs (unordered cards) */
void test_flush_clubs_unordered(void) {
    printf("Testing flush in clubs (unordered)...\n");
    Card cards[5] = {
        {RANK_THREE, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_SIX, SUIT_CLUBS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_CLUBS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Should be sorted descending regardless of input order */
    assert(tiebreakers[0] == RANK_ACE);
    assert(tiebreakers[1] == RANK_QUEEN);
    assert(tiebreakers[2] == RANK_EIGHT);
    assert(tiebreakers[3] == RANK_SIX);
    assert(tiebreakers[4] == RANK_THREE);
    printf("  Pass: Unordered flush detected and sorted correctly\n");
}

/* Test straight flush should return 0 (excluded) */
void test_straight_flush_excluded(void) {
    printf("Testing straight flush exclusion...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_SPADES},
        {RANK_TEN, SUIT_SPADES},
        {RANK_JACK, SUIT_SPADES},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_KING, SUIT_SPADES}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Straight flush correctly excluded\n");
}

/* Test royal flush should return 0 (excluded) */
void test_royal_flush_excluded(void) {
    printf("Testing royal flush exclusion...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Royal flush correctly excluded\n");
}

/* Test wheel straight flush should return 0 */
void test_wheel_straight_flush_excluded(void) {
    printf("Testing wheel straight flush exclusion...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_CLUBS},
        {RANK_TWO, SUIT_CLUBS},
        {RANK_THREE, SUIT_CLUBS},
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_FIVE, SUIT_CLUBS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Wheel straight flush correctly excluded\n");
}

/* Test not a flush - mixed suits */
void test_not_flush_mixed_suits(void) {
    printf("Testing non-flush (mixed suits)...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Mixed suits correctly identified as non-flush\n");
}

/* Test invalid input - wrong length */
void test_invalid_length(void) {
    printf("Testing invalid length (4 cards)...\n");
    Card cards[4] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 4, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Invalid length handled correctly\n");
}

/* Test invalid input - NULL tiebreakers */
void test_null_tiebreakers(void) {
    printf("Testing NULL tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, NULL, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: NULL tiebreakers handled correctly\n");
}

/* Test invalid input - NULL num_tiebreakers */
void test_null_num_tiebreakers(void) {
    printf("Testing NULL num_tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    int result = detect_flush(cards, 5, tiebreakers, NULL);
    assert(result == 0);
    printf("  Pass: NULL num_tiebreakers handled correctly\n");
}

/* Test flush with all low cards */
void test_flush_low_cards(void) {
    printf("Testing flush with low cards...\n");
    Card cards[5] = {
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_FIVE, SUIT_SPADES},
        {RANK_FOUR, SUIT_SPADES},
        {RANK_THREE, SUIT_SPADES},
        {RANK_TWO, SUIT_SPADES}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_flush(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    assert(tiebreakers[0] == RANK_SEVEN);
    assert(tiebreakers[1] == RANK_FIVE);
    assert(tiebreakers[2] == RANK_FOUR);
    assert(tiebreakers[3] == RANK_THREE);
    assert(tiebreakers[4] == RANK_TWO);
    printf("  Pass: Low flush detected with correct tiebreakers\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_flush function - Issue #26\n");
    printf("==========================================\n\n");

    test_flush_hearts_ace_high();
    test_flush_diamonds();
    test_flush_clubs_unordered();
    test_straight_flush_excluded();
    test_royal_flush_excluded();
    test_wheel_straight_flush_excluded();
    test_not_flush_mixed_suits();
    test_invalid_length();
    test_null_tiebreakers();
    test_null_num_tiebreakers();
    test_flush_low_cards();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
