#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_three_of_a_kind function
 * Tests verify three of a kind detection (3 cards of same rank, no pair)
 */

/* Test three of a kind: Three Aces with King and Queen kickers */
void test_three_aces_kq_kickers(void) {
    printf("Testing three of a kind: Aces with K-Q kickers...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_ACE);    /* Trip rank */
    assert(tiebreakers[1] == RANK_KING);   /* Kicker 1 (higher) */
    assert(tiebreakers[2] == RANK_QUEEN);  /* Kicker 2 (lower) */
    printf("  Pass: Three Aces with K-Q kickers detected correctly\n");
}

/* Test three of a kind: Three Sevens with Ace and Two kickers */
void test_three_sevens_a2_kickers(void) {
    printf("Testing three of a kind: Sevens with A-2 kickers...\n");
    Card cards[5] = {
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_ACE, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_SEVEN);
    assert(tiebreakers[1] == RANK_ACE);
    assert(tiebreakers[2] == RANK_TWO);
    printf("  Pass: Three Sevens with A-2 kickers detected correctly\n");
}

/* Test three of a kind: Three Twos with unordered kickers (test sorting) */
void test_three_twos_unordered_kickers(void) {
    printf("Testing three of a kind with unordered kickers...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_TWO, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TWO, SUIT_SPADES}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_TWO);
    assert(tiebreakers[1] == RANK_JACK);  /* Kickers sorted descending */
    assert(tiebreakers[2] == RANK_FIVE);
    printf("  Pass: Three Twos with unordered kickers sorted correctly\n");
}

/* Test three of a kind with pre-computed counts */
void test_three_of_a_kind_with_counts(void) {
    printf("Testing three of a kind with pre-computed counts...\n");
    Card cards[5] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_NINE, SUIT_HEARTS},
        {RANK_FOUR, SUIT_DIAMONDS}
    };

    /* Pre-compute rank counts */
    int counts[15];
    rank_counts(cards, 5, counts);

    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, counts, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 3);
    assert(tiebreakers[0] == RANK_QUEEN);
    assert(tiebreakers[1] == RANK_NINE);
    assert(tiebreakers[2] == RANK_FOUR);
    printf("  Pass: Three of a kind with pre-computed counts detected correctly\n");
}

/* Test full house (should NOT be three of a kind) */
void test_full_house_not_three_of_a_kind(void) {
    printf("Testing full house (not three of a kind)...\n");
    Card cards[5] = {
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Full house correctly identified as non-three-of-a-kind\n");
}

/* Test four of a kind (should NOT be three of a kind) */
void test_four_of_a_kind_not_three_of_a_kind(void) {
    printf("Testing four of a kind (not three of a kind)...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_NINE, SUIT_CLUBS},
        {RANK_NINE, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Four of a kind correctly identified as non-three-of-a-kind\n");
}

/* Test two pair (no three of a kind) */
void test_two_pair_not_three_of_a_kind(void) {
    printf("Testing two pair (not three of a kind)...\n");
    Card cards[5] = {
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: Two pair correctly identified as non-three-of-a-kind\n");
}

/* Test one pair (not three of a kind) */
void test_one_pair_not_three_of_a_kind(void) {
    printf("Testing one pair (not three of a kind)...\n");
    Card cards[5] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: One pair correctly identified as non-three-of-a-kind\n");
}

/* Test high card (no pairs) */
void test_high_card_not_three_of_a_kind(void) {
    printf("Testing high card (not three of a kind)...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, &num_tiebreakers);

    assert(result == 0);
    printf("  Pass: High card correctly identified as non-three-of-a-kind\n");
}

/* Test invalid input - NULL cards */
void test_null_cards(void) {
    printf("Testing NULL cards pointer...\n");
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(NULL, 5, NULL, tiebreakers, &num_tiebreakers);

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
    Rank tiebreakers[3];
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 4, NULL, tiebreakers, &num_tiebreakers);

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
        {RANK_QUEEN, SUIT_DIAMONDS}
    };
    size_t num_tiebreakers = 0;

    int result = detect_three_of_a_kind(cards, 5, NULL, NULL, &num_tiebreakers);

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
        {RANK_QUEEN, SUIT_DIAMONDS}
    };
    Rank tiebreakers[3];

    int result = detect_three_of_a_kind(cards, 5, NULL, tiebreakers, NULL);

    assert(result == 0);
    printf("  Pass: NULL num_tiebreakers handled correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_three_of_a_kind function - Issue #28\n");
    printf("==========================================\n\n");

    test_three_aces_kq_kickers();
    test_three_sevens_a2_kickers();
    test_three_twos_unordered_kickers();
    test_three_of_a_kind_with_counts();
    test_full_house_not_three_of_a_kind();
    test_four_of_a_kind_not_three_of_a_kind();
    test_two_pair_not_three_of_a_kind();
    test_one_pair_not_three_of_a_kind();
    test_high_card_not_three_of_a_kind();
    test_null_cards();
    test_invalid_length();
    test_null_tiebreakers();
    test_null_num_tiebreakers();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
