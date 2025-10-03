#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_high_card function
 * Tests verify high card detection (fallback when no other category matches)
 */

/* Test high card with Ace-high */
void test_high_card_ace_high(void) {
    printf("Testing high card with Ace-high...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Tiebreakers should be in descending order: A K Q J 9 */
    assert(tiebreakers[0] == RANK_ACE);
    assert(tiebreakers[1] == RANK_KING);
    assert(tiebreakers[2] == RANK_QUEEN);
    assert(tiebreakers[3] == RANK_JACK);
    assert(tiebreakers[4] == RANK_NINE);
    printf("  Pass: Ace-high detected with correct tiebreakers\n");
}

/* Test high card with King-high */
void test_high_card_king_high(void) {
    printf("Testing high card with King-high...\n");
    Card cards[5] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_EIGHT, SUIT_CLUBS},
        {RANK_SIX, SUIT_SPADES},
        {RANK_FOUR, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Tiebreakers should be in descending order: K 10 8 6 4 */
    assert(tiebreakers[0] == RANK_KING);
    assert(tiebreakers[1] == RANK_TEN);
    assert(tiebreakers[2] == RANK_EIGHT);
    assert(tiebreakers[3] == RANK_SIX);
    assert(tiebreakers[4] == RANK_FOUR);
    printf("  Pass: King-high detected with correct tiebreakers\n");
}

/* Test high card with unordered cards */
void test_high_card_unordered(void) {
    printf("Testing high card with unordered cards...\n");
    Card cards[5] = {
        {RANK_THREE, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Should be sorted descending regardless of input order: A Q 9 7 3 */
    assert(tiebreakers[0] == RANK_ACE);
    assert(tiebreakers[1] == RANK_QUEEN);
    assert(tiebreakers[2] == RANK_NINE);
    assert(tiebreakers[3] == RANK_SEVEN);
    assert(tiebreakers[4] == RANK_THREE);
    printf("  Pass: Unordered cards detected and sorted correctly\n");
}

/* Test high card with low cards */
void test_high_card_low_cards(void) {
    printf("Testing high card with low cards...\n");
    Card cards[5] = {
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_CLUBS},
        {RANK_THREE, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Tiebreakers should be: 7 5 4 3 2 */
    assert(tiebreakers[0] == RANK_SEVEN);
    assert(tiebreakers[1] == RANK_FIVE);
    assert(tiebreakers[2] == RANK_FOUR);
    assert(tiebreakers[3] == RANK_THREE);
    assert(tiebreakers[4] == RANK_TWO);
    printf("  Pass: Low cards detected with correct tiebreakers\n");
}

/* Test high card with all face cards except suited */
void test_high_card_mixed_suits(void) {
    printf("Testing high card with mixed suits...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Tiebreakers should be: A K Q J 10 */
    assert(tiebreakers[0] == RANK_ACE);
    assert(tiebreakers[1] == RANK_KING);
    assert(tiebreakers[2] == RANK_QUEEN);
    assert(tiebreakers[3] == RANK_JACK);
    assert(tiebreakers[4] == RANK_TEN);
    printf("  Pass: Mixed suits detected with correct tiebreakers\n");
}

/* Test invalid input - wrong length (4 cards) */
void test_invalid_length_too_few(void) {
    printf("Testing invalid length (4 cards)...\n");
    Card cards[4] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 4, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Invalid length (4 cards) handled correctly\n");
}

/* Test invalid input - wrong length (6 cards) */
void test_invalid_length_too_many(void) {
    printf("Testing invalid length (6 cards)...\n");
    Card cards[6] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 6, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: Invalid length (6 cards) handled correctly\n");
}

/* Test invalid input - NULL cards */
void test_null_cards(void) {
    printf("Testing NULL cards pointer...\n");
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(NULL, 5, tiebreakers, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: NULL cards handled correctly\n");
}

/* Test invalid input - NULL tiebreakers */
void test_null_tiebreakers(void) {
    printf("Testing NULL tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, NULL, &num_tiebreakers);
    assert(result == 0);
    printf("  Pass: NULL tiebreakers handled correctly\n");
}

/* Test invalid input - NULL num_tiebreakers */
void test_null_num_tiebreakers(void) {
    printf("Testing NULL num_tiebreakers pointer...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    int result = detect_high_card(cards, 5, tiebreakers, NULL);
    assert(result == 0);
    printf("  Pass: NULL num_tiebreakers handled correctly\n");
}

/* Test high card with duplicate ranks (should still work - extracts all 5 ranks) */
void test_high_card_with_pair(void) {
    printf("Testing high card with pair (should still extract 5 ranks)...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);
    assert(result == 1);
    assert(num_tiebreakers == 5);
    /* Should extract all 5 ranks in descending order: A A Q J 10 */
    assert(tiebreakers[0] == RANK_ACE);
    assert(tiebreakers[1] == RANK_ACE);
    assert(tiebreakers[2] == RANK_QUEEN);
    assert(tiebreakers[3] == RANK_JACK);
    assert(tiebreakers[4] == RANK_TEN);
    printf("  Pass: Pair handled - all 5 ranks extracted\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_high_card function - Issue #31\n");
    printf("==========================================\n\n");

    test_high_card_ace_high();
    test_high_card_king_high();
    test_high_card_unordered();
    test_high_card_low_cards();
    test_high_card_mixed_suits();
    test_invalid_length_too_few();
    test_invalid_length_too_many();
    test_null_cards();
    test_null_tiebreakers();
    test_null_num_tiebreakers();
    test_high_card_with_pair();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
