#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_full_house function
 * Tests verify full house detection (3 of a kind + pair)
 */

/* Test full house: Three Aces and Two Kings */
void test_full_house_aces_over_kings(void) {
    printf("Testing full house: Aces over Kings...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_ACE);  /* Trip rank */
    assert(tiebreakers[1] == RANK_KING);  /* Pair rank */
    printf("  Pass: Full house Aces over Kings detected correctly\n");
}

/* Test full house: Three Sevens and Two Threes */
void test_full_house_sevens_over_threes(void) {
    printf("Testing full house: Sevens over Threes...\n");
    Card cards[5] = {
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_THREE, SUIT_HEARTS},
        {RANK_THREE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_SEVEN);
    assert(tiebreakers[1] == RANK_THREE);
    printf("  Pass: Full house Sevens over Threes detected correctly\n");
}

/* Test full house: Three Twos and Two Aces (weaker trip with stronger pair) */
void test_full_house_twos_over_aces(void) {
    printf("Testing full house: Twos over Aces...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS},
        {RANK_TWO, SUIT_CLUBS},
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_TWO);
    assert(tiebreakers[1] == RANK_ACE);
    printf("  Pass: Full house Twos over Aces detected correctly\n");
}

/* Test full house with unordered cards */
void test_full_house_unordered(void) {
    printf("Testing full house with unordered cards...\n");
    Card cards[5] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_FIVE, SUIT_SPADES}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_FIVE);   /* Trip rank */
    assert(tiebreakers[1] == RANK_KING);   /* Pair rank */
    printf("  Pass: Full house with unordered cards detected correctly\n");
}

/* Test full house with pre-computed counts */
void test_full_house_with_counts(void) {
    printf("Testing full house with pre-computed counts...\n");
    Card cards[5] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_FOUR, SUIT_HEARTS},
        {RANK_FOUR, SUIT_DIAMONDS}
    };

    /* Pre-compute rank counts */
    int counts[15];
    rank_counts(cards, 5, counts);

    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, counts, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 2);
    assert(tiebreakers[0] == RANK_QUEEN);
    assert(tiebreakers[1] == RANK_FOUR);
    printf("  Pass: Full house with pre-computed counts detected correctly\n");
}

/* Test four of a kind (should NOT be full house) */
void test_four_of_a_kind_not_full_house(void) {
    printf("Testing four of a kind (not full house)...\n");
    Card cards[5] = {
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Four of a kind correctly identified as non-full house\n");
}

/* Test three of a kind (no pair) */
void test_three_of_a_kind_no_pair(void) {
    printf("Testing three of a kind without pair...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_NINE, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Three of a kind without pair correctly identified as non-full house\n");
}

/* Test two pair (no three of a kind) */
void test_two_pair_not_full_house(void) {
    printf("Testing two pair (not full house)...\n");
    Card cards[5] = {
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Two pair correctly identified as non-full house\n");
}

/* Test no pairs or trips */
void test_high_card_not_full_house(void) {
    printf("Testing high card (not full house)...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: High card correctly identified as non-full house\n");
}

/* Test invalid input - NULL cards */
void test_null_cards(void) {
    printf("Testing NULL cards pointer...\n");
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(NULL, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: NULL cards handled correctly\n");
}

/* Test invalid input - wrong length */
void test_invalid_length(void) {
    printf("Testing invalid length (4 cards)...\n");
    Card cards[4] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS}
    };
    Rank tiebreakers[2];
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 4, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Invalid length handled correctly\n");
}

/* Test invalid input - NULL tiebreakers */
void test_null_tiebreakers(void) {
    printf("Testing NULL tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    size_t num_tiebreakers = 0;

    int result = detect_full_house(cards, 5, NULL, NULL, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: NULL tiebreakers handled correctly\n");
}

/* Test invalid input - NULL num_tiebreakers */
void test_null_num_tiebreakers(void) {
    printf("Testing NULL num_tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    Rank tiebreakers[2];

    int result = detect_full_house(cards, 5, NULL, tiebreakers, NULL);

    assert(result == 0);
    printf("  Pass: NULL num_tiebreakers handled correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_full_house function - Issue #25\n");
    printf("==========================================\n\n");

    test_full_house_aces_over_kings();
    test_full_house_sevens_over_threes();
    test_full_house_twos_over_aces();
    test_full_house_unordered();
    test_full_house_with_counts();
    test_four_of_a_kind_not_full_house();
    test_three_of_a_kind_no_pair();
    test_two_pair_not_full_house();
    test_high_card_not_full_house();
    test_null_cards();
    test_invalid_length();
    test_null_tiebreakers();
    test_null_num_tiebreakers();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
