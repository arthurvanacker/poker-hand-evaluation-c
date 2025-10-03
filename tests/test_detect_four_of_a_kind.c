#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_four_of_a_kind function
 * Tests verify four of a kind detection with proper tiebreakers
 */

/* Test four aces with king kicker */
void test_four_aces_king_kicker(void) {
    printf("Testing four aces with king kicker...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_ACE);   /* Quad rank */
    assert(tiebreakers[1] == RANK_KING);  /* Kicker */
    printf("  ✓ Four aces with king kicker detected correctly\n");
}

/* Test four kings with ace kicker */
void test_four_kings_ace_kicker(void) {
    printf("Testing four kings with ace kicker...\n");
    Card cards[5] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_KING);  /* Quad rank */
    assert(tiebreakers[1] == RANK_ACE);   /* Kicker */
    printf("  ✓ Four kings with ace kicker detected correctly\n");
}

/* Test four twos with three kicker */
void test_four_twos_three_kicker(void) {
    printf("Testing four twos with three kicker...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS},
        {RANK_TWO, SUIT_CLUBS},
        {RANK_TWO, SUIT_SPADES},
        {RANK_THREE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_TWO);    /* Quad rank */
    assert(tiebreakers[1] == RANK_THREE);  /* Kicker */
    printf("  ✓ Four twos with three kicker detected correctly\n");
}

/* Test with pre-computed counts */
void test_with_precomputed_counts(void) {
    printf("Testing four of a kind with pre-computed counts...\n");
    Card cards[5] = {
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_JACK, SUIT_HEARTS}
    };

    /* Pre-compute counts */
    int counts[15];
    rank_counts(cards, 5, counts);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, counts, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_SEVEN);  /* Quad rank */
    assert(tiebreakers[1] == RANK_JACK);   /* Kicker */
    printf("  ✓ Four of a kind with pre-computed counts detected correctly\n");
}

/* Test not four of a kind - three of a kind */
void test_not_four_of_a_kind_three_of_a_kind(void) {
    printf("Testing not four of a kind - three of a kind...\n");
    Card cards[5] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ Three of a kind correctly identified as not four of a kind\n");
}

/* Test not four of a kind - full house */
void test_not_four_of_a_kind_full_house(void) {
    printf("Testing not four of a kind - full house...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_TEN, SUIT_CLUBS},
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ Full house correctly identified as not four of a kind\n");
}

/* Test not four of a kind - two pair */
void test_not_four_of_a_kind_two_pair(void) {
    printf("Testing not four of a kind - two pair...\n");
    Card cards[5] = {
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_FOUR, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ Two pair correctly identified as not four of a kind\n");
}

/* Test not four of a kind - high card */
void test_not_four_of_a_kind_high_card(void) {
    printf("Testing not four of a kind - high card...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ High card correctly identified as not four of a kind\n");
}

/* Test invalid input - NULL cards */
void test_invalid_null_cards(void) {
    printf("Testing invalid input - NULL cards...\n");
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(NULL, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ NULL cards handled correctly\n");
}

/* Test invalid input - wrong length */
void test_invalid_wrong_length(void) {
    printf("Testing invalid input - wrong length...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 4, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ Wrong length handled correctly\n");
}

/* Test invalid input - NULL tiebreakers */
void test_invalid_null_tiebreakers(void) {
    printf("Testing invalid input - NULL tiebreakers...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, NULL, &num_tiebreakers);

    assert(result == 0);
    printf("  ✓ NULL tiebreakers handled correctly\n");
}

/* Test invalid input - NULL num_tiebreakers */
void test_invalid_null_num_tiebreakers(void) {
    printf("Testing invalid input - NULL num_tiebreakers...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, NULL);

    assert(result == 0);
    printf("  ✓ NULL num_tiebreakers handled correctly\n");
}

/* Test four of a kind with different suit arrangements */
void test_four_queens_mixed_suits(void) {
    printf("Testing four queens with mixed suits...\n");
    Card cards[5] = {
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_QUEEN, SUIT_DIAMONDS}
    };
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers = 0;

    int result = detect_four_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_QUEEN);  /* Quad rank */
    assert(tiebreakers[1] == RANK_SIX);    /* Kicker */
    printf("  ✓ Four queens with mixed suits detected correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_four_of_a_kind - Issue #24\n");
    printf("==========================================\n\n");

    test_four_aces_king_kicker();
    test_four_kings_ace_kicker();
    test_four_twos_three_kicker();
    test_with_precomputed_counts();
    test_not_four_of_a_kind_three_of_a_kind();
    test_not_four_of_a_kind_full_house();
    test_not_four_of_a_kind_two_pair();
    test_not_four_of_a_kind_high_card();
    test_invalid_null_cards();
    test_invalid_wrong_length();
    test_invalid_null_tiebreakers();
    test_invalid_null_num_tiebreakers();
    test_four_queens_mixed_suits();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
