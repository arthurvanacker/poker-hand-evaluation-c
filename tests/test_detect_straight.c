#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_straight function
 * Tests verify straight detection (5 sequential cards, non-flush only)
 */

/* Test basic straight with mixed suits (non-flush) */
void test_basic_straight_mixed_suits(void) {
    printf("Testing basic straight with mixed suits...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 1);
    assert(tiebreakers[0] == RANK_NINE);
    printf("  Pass: Basic straight detected with correct tiebreaker\n");
}

/* Test wheel straight (A-2-3-4-5) with mixed suits */
void test_wheel_straight_mixed_suits(void) {
    printf("Testing wheel straight (A-2-3-4-5) with mixed suits...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS},
        {RANK_THREE, SUIT_CLUBS},
        {RANK_FOUR, SUIT_SPADES},
        {RANK_FIVE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 1);
    assert(tiebreakers[0] == RANK_FIVE);  /* Wheel: high card is 5 */
    printf("  Pass: Wheel straight detected with correct tiebreaker (5)\n");
}

/* Test ace-high straight (T-J-Q-K-A) with mixed suits */
void test_ace_high_straight_mixed_suits(void) {
    printf("Testing ace-high straight with mixed suits...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 1);
    assert(tiebreakers[0] == RANK_ACE);
    printf("  Pass: Ace-high straight detected with correct tiebreaker\n");
}

/* Test straight with unordered cards */
void test_straight_unordered_mixed_suits(void) {
    printf("Testing straight with unordered cards (mixed suits)...\n");
    Card cards[5] = {
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_SIX, SUIT_SPADES},
        {RANK_FOUR, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 1);
    assert(tiebreakers[0] == RANK_EIGHT);
    printf("  Pass: Unordered straight detected with correct tiebreaker\n");
}

/* Test straight flush should return 0 (must exclude flushes) */
void test_straight_flush_excluded(void) {
    printf("Testing straight flush is excluded...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 0);  /* Should return 0 for straight flush */
    printf("  Pass: Straight flush correctly excluded\n");
}

/* Test royal flush should return 0 (must exclude flushes) */
void test_royal_flush_excluded(void) {
    printf("Testing royal flush is excluded...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_SPADES},
        {RANK_JACK, SUIT_SPADES},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_SPADES}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 0);  /* Should return 0 for royal flush */
    printf("  Pass: Royal flush correctly excluded\n");
}

/* Test non-straight cards */
void test_non_straight(void) {
    printf("Testing non-straight cards...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Non-straight correctly identified\n");
}

/* Test pair (not a straight) */
void test_pair_not_straight(void) {
    printf("Testing pair (not a straight)...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Pair correctly identified as non-straight\n");
}

/* Test flush (not a straight) */
void test_flush_not_straight(void) {
    printf("Testing flush (not a straight)...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_CLUBS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_ACE, SUIT_CLUBS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Flush (not straight) correctly identified\n");
}

/* Test invalid length (4 cards) */
void test_invalid_length_four(void) {
    printf("Testing invalid length (4 cards)...\n");
    Card cards[4] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 4, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Invalid length (4) handled correctly\n");
}

/* Test invalid length (6 cards) */
void test_invalid_length_six(void) {
    printf("Testing invalid length (6 cards)...\n");
    Card cards[6] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 6, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Invalid length (6) handled correctly\n");
}

/* Test low straight (2-3-4-5-6) with mixed suits */
void test_low_straight_mixed_suits(void) {
    printf("Testing low straight (2-3-4-5-6) with mixed suits...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_THREE, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_FIVE, SUIT_SPADES},
        {RANK_SIX, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 1);
    assert(tiebreakers[0] == RANK_SIX);
    printf("  Pass: Low straight detected with correct tiebreaker\n");
}

/* Test king-high straight (9-T-J-Q-K) with mixed suits */
void test_king_high_straight_mixed_suits(void) {
    printf("Testing king-high straight (9-T-J-Q-K) with mixed suits...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    int result = detect_straight(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 1);
    assert(tiebreakers[0] == RANK_KING);
    printf("  Pass: King-high straight detected with correct tiebreaker\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_straight function - Issue #27\n");
    printf("==========================================\n\n");

    test_basic_straight_mixed_suits();
    test_wheel_straight_mixed_suits();
    test_ace_high_straight_mixed_suits();
    test_straight_unordered_mixed_suits();
    test_straight_flush_excluded();
    test_royal_flush_excluded();
    test_non_straight();
    test_pair_not_straight();
    test_flush_not_straight();
    test_invalid_length_four();
    test_invalid_length_six();
    test_low_straight_mixed_suits();
    test_king_high_straight_mixed_suits();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
