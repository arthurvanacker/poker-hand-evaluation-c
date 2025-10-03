#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_two_pair function
 * Tests verify two pair detection (2 ranks with 2 cards each)
 */

/* Test two pair: Aces and Kings with Queen kicker */
void test_two_pair_aces_kings_queen(void) {
    printf("Testing two pair: Aces and Kings with Queen kicker...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_ACE);    /* High pair */
    assert(tiebreakers[1] == RANK_KING);   /* Low pair */
    assert(tiebreakers[2] == RANK_QUEEN);  /* Kicker */
    printf("  Pass: Two pair Aces-Kings with Queen kicker detected correctly\n");
}

/* Test two pair: Tens and Twos with Ace kicker */
void test_two_pair_tens_twos_ace(void) {
    printf("Testing two pair: Tens and Twos with Ace kicker...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_TWO, SUIT_CLUBS},
        {RANK_TWO, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_TEN);  /* High pair */
    assert(tiebreakers[1] == RANK_TWO);  /* Low pair */
    assert(tiebreakers[2] == RANK_ACE);  /* Kicker */
    printf("  Pass: Two pair Tens-Twos with Ace kicker detected correctly\n");
}

/* Test two pair with unordered cards (test sorting) */
void test_two_pair_unordered(void) {
    printf("Testing two pair with unordered cards...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_THREE, SUIT_SPADES},
        {RANK_JACK, SUIT_HEARTS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_JACK);  /* High pair (sorted) */
    assert(tiebreakers[1] == RANK_FIVE);  /* Low pair (sorted) */
    assert(tiebreakers[2] == RANK_THREE); /* Kicker */
    printf("  Pass: Two pair with unordered cards sorted correctly\n");
}

/* Test two pair with pre-computed counts */
void test_two_pair_with_counts(void) {
    printf("Testing two pair with pre-computed counts...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_FOUR, SUIT_SPADES},
        {RANK_SEVEN, SUIT_HEARTS}
    };

    /* Pre-compute rank counts */
    int counts[15];
    rank_counts(cards, 5, counts);

    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, counts, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_NINE);
    assert(tiebreakers[1] == RANK_FOUR);
    assert(tiebreakers[2] == RANK_SEVEN);
    printf("  Pass: Two pair with pre-computed counts detected correctly\n");
}

/* Test full house (should NOT be two pair) */
void test_full_house_not_two_pair(void) {
    printf("Testing full house (not two pair)...\n");
    Card cards[5] = {
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Full house correctly identified as non-two-pair\n");
}

/* Test three of a kind (should NOT be two pair) */
void test_three_of_a_kind_not_two_pair(void) {
    printf("Testing three of a kind (not two pair)...\n");
    Card cards[5] = {
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_EIGHT, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Three of a kind correctly identified as non-two-pair\n");
}

/* Test four of a kind (should NOT be two pair) */
void test_four_of_a_kind_not_two_pair(void) {
    printf("Testing four of a kind (not two pair)...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_NINE, SUIT_CLUBS},
        {RANK_NINE, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Four of a kind correctly identified as non-two-pair\n");
}

/* Test one pair (should NOT be two pair) */
void test_one_pair_not_two_pair(void) {
    printf("Testing one pair (not two pair)...\n");
    Card cards[5] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: One pair correctly identified as non-two-pair\n");
}

/* Test high card (no pairs) */
void test_high_card_not_two_pair(void) {
    printf("Testing high card (not two pair)...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: High card correctly identified as non-two-pair\n");
}

/* Test invalid input - NULL cards */
void test_null_cards(void) {
    printf("Testing NULL cards pointer...\n");
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(NULL, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: NULL cards handled correctly\n");
}

/* Test invalid input - wrong length */
void test_invalid_length(void) {
    printf("Testing invalid length (4 cards)...\n");
    Card cards[4] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 4, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Invalid length handled correctly\n");
}

/* Test invalid input - NULL tiebreakers */
void test_null_tiebreakers(void) {
    printf("Testing NULL tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };
    size_t num_tiebreakers = 0;

    int result = detect_two_pair(cards, 5, NULL, NULL, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: NULL tiebreakers handled correctly\n");
}

/* Test invalid input - NULL num_tiebreakers */
void test_null_num_tiebreakers(void) {
    printf("Testing NULL num_tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };
    Rank tiebreakers[3];

    int result = detect_two_pair(cards, 5, NULL, tiebreakers, NULL);

    assert(result == 0);
    printf("  Pass: NULL num_tiebreakers handled correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_two_pair function - Issue #29\n");
    printf("==========================================\n\n");

    test_two_pair_aces_kings_queen();
    test_two_pair_tens_twos_ace();
    test_two_pair_unordered();
    test_two_pair_with_counts();
    test_full_house_not_two_pair();
    test_three_of_a_kind_not_two_pair();
    test_four_of_a_kind_not_two_pair();
    test_one_pair_not_two_pair();
    test_high_card_not_two_pair();
    test_null_cards();
    test_invalid_length();
    test_null_tiebreakers();
    test_null_num_tiebreakers();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
