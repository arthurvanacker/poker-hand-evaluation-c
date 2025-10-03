#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_one_pair function
 * Tests verify one pair detection (exactly 1 rank with 2 cards)
 */

/* Test one pair: Aces with King-Queen-Jack kickers */
void test_one_pair_aces_kqj(void) {
    printf("Testing one pair: Aces with K-Q-J kickers...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_JACK, SUIT_HEARTS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 4);
    assert(tiebreakers[0] == RANK_ACE);   /* Pair rank */
    assert(tiebreakers[1] == RANK_KING);  /* Kicker 1 */
    assert(tiebreakers[2] == RANK_QUEEN); /* Kicker 2 */
    assert(tiebreakers[3] == RANK_JACK);  /* Kicker 3 */
    printf("  Pass: One pair Aces with K-Q-J kickers detected correctly\n");
}

/* Test one pair: Twos with Ace-King-Queen kickers */
void test_one_pair_twos_akq(void) {
    printf("Testing one pair: Twos with A-K-Q kickers...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 4);
    assert(tiebreakers[0] == RANK_TWO);   /* Pair rank */
    assert(tiebreakers[1] == RANK_ACE);   /* Kicker 1 (highest) */
    assert(tiebreakers[2] == RANK_KING);  /* Kicker 2 */
    assert(tiebreakers[3] == RANK_QUEEN); /* Kicker 3 */
    printf("  Pass: One pair Twos with A-K-Q kickers detected correctly\n");
}

/* Test one pair with unordered cards (test kicker sorting) */
void test_one_pair_unordered(void) {
    printf("Testing one pair with unordered cards...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_THREE, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_SEVEN, SUIT_HEARTS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 4);
    assert(tiebreakers[0] == RANK_JACK);  /* Pair rank */
    assert(tiebreakers[1] == RANK_SEVEN); /* Highest kicker */
    assert(tiebreakers[2] == RANK_FIVE);  /* Middle kicker */
    assert(tiebreakers[3] == RANK_THREE); /* Lowest kicker */
    printf("  Pass: One pair with unordered cards sorted correctly\n");
}

/* Test one pair with pre-computed counts */
void test_one_pair_with_counts(void) {
    printf("Testing one pair with pre-computed counts...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_SIX, SUIT_HEARTS}
    };

    /* Pre-compute rank counts */
    int counts[15];
    rank_counts(cards, 5, counts);

    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, counts, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 4);
    assert(tiebreakers[0] == RANK_NINE);
    assert(tiebreakers[1] == RANK_EIGHT);
    assert(tiebreakers[2] == RANK_SIX);
    assert(tiebreakers[3] == RANK_FOUR);
    printf("  Pass: One pair with pre-computed counts detected correctly\n");
}

/* Test two pair (should NOT be one pair) */
void test_two_pair_not_one_pair(void) {
    printf("Testing two pair (not one pair)...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Two pair correctly identified as non-one-pair\n");
}

/* Test three of a kind (should NOT be one pair) */
void test_three_of_a_kind_not_one_pair(void) {
    printf("Testing three of a kind (not one pair)...\n");
    Card cards[5] = {
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_EIGHT, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Three of a kind correctly identified as non-one-pair\n");
}

/* Test full house (should NOT be one pair) */
void test_full_house_not_one_pair(void) {
    printf("Testing full house (not one pair)...\n");
    Card cards[5] = {
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Full house correctly identified as non-one-pair\n");
}

/* Test four of a kind (should NOT be one pair) */
void test_four_of_a_kind_not_one_pair(void) {
    printf("Testing four of a kind (not one pair)...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_NINE, SUIT_CLUBS},
        {RANK_NINE, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Four of a kind correctly identified as non-one-pair\n");
}

/* Test high card (no pairs) */
void test_high_card_not_one_pair(void) {
    printf("Testing high card (not one pair)...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: High card correctly identified as non-one-pair\n");
}

/* Test invalid input - NULL cards */
void test_null_cards(void) {
    printf("Testing NULL cards pointer...\n");
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(NULL, 5, NULL, tiebreakers, &num_tiebreakers);

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
        {RANK_QUEEN, SUIT_SPADES}
    };
    Rank tiebreakers[4];
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 4, NULL, tiebreakers, &num_tiebreakers);

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
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_JACK, SUIT_HEARTS}
    };
    size_t num_tiebreakers = 0;

    int result = detect_one_pair(cards, 5, NULL, NULL, &num_tiebreakers);

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
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_JACK, SUIT_HEARTS}
    };
    Rank tiebreakers[4];

    int result = detect_one_pair(cards, 5, NULL, tiebreakers, NULL);

    assert(result == 0);
    printf("  Pass: NULL num_tiebreakers handled correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_one_pair function - Issue #30\n");
    printf("==========================================\n\n");

    test_one_pair_aces_kqj();
    test_one_pair_twos_akq();
    test_one_pair_unordered();
    test_one_pair_with_counts();
    test_two_pair_not_one_pair();
    test_three_of_a_kind_not_one_pair();
    test_full_house_not_one_pair();
    test_four_of_a_kind_not_one_pair();
    test_high_card_not_one_pair();
    test_null_cards();
    test_invalid_length();
    test_null_tiebreakers();
    test_null_num_tiebreakers();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
