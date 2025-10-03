#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_royal_flush function
 * Tests verify royal flush detection (10-J-Q-K-A all same suit)
 */

/* Test royal flush in hearts */
void test_royal_flush_hearts(void) {
    printf("Testing royal flush in hearts...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };
    assert(detect_royal_flush(cards, 5) == 1);
    printf("  Pass: Royal flush in hearts detected correctly\n");
}

/* Test royal flush in diamonds */
void test_royal_flush_diamonds(void) {
    printf("Testing royal flush in diamonds...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_DIAMONDS}
    };
    assert(detect_royal_flush(cards, 5) == 1);
    printf("  Pass: Royal flush in diamonds detected correctly\n");
}

/* Test royal flush in clubs */
void test_royal_flush_clubs(void) {
    printf("Testing royal flush in clubs...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_ACE, SUIT_CLUBS}
    };
    assert(detect_royal_flush(cards, 5) == 1);
    printf("  Pass: Royal flush in clubs detected correctly\n");
}

/* Test royal flush in spades */
void test_royal_flush_spades(void) {
    printf("Testing royal flush in spades...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_SPADES},
        {RANK_JACK, SUIT_SPADES},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_SPADES}
    };
    assert(detect_royal_flush(cards, 5) == 1);
    printf("  Pass: Royal flush in spades detected correctly\n");
}

/* Test non-royal straight flush (9-high) */
void test_nine_high_straight_flush(void) {
    printf("Testing 9-high straight flush (not royal)...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };
    assert(detect_royal_flush(cards, 5) == 0);
    printf("  Pass: 9-high straight flush correctly identified as non-royal\n");
}

/* Test King-high straight flush (not royal) */
void test_king_high_straight_flush(void) {
    printf("Testing K-high straight flush (not royal)...\n");
    Card cards[5] = {
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_KING, SUIT_DIAMONDS}
    };
    assert(detect_royal_flush(cards, 5) == 0);
    printf("  Pass: K-high straight flush correctly identified as non-royal\n");
}

/* Test regular flush (not straight) */
void test_regular_flush(void) {
    printf("Testing regular flush (not straight)...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_CLUBS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_ACE, SUIT_CLUBS}
    };
    assert(detect_royal_flush(cards, 5) == 0);
    printf("  Pass: Regular flush correctly identified as non-royal\n");
}

/* Test straight but not flush */
void test_straight_not_flush(void) {
    printf("Testing straight but not flush...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };
    assert(detect_royal_flush(cards, 5) == 0);
    printf("  Pass: Straight but not flush correctly identified as non-royal\n");
}

/* Test neither straight nor flush */
void test_random_cards(void) {
    printf("Testing random cards...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };
    assert(detect_royal_flush(cards, 5) == 0);
    printf("  Pass: Random cards correctly identified as non-royal\n");
}

/* Test invalid input - wrong length */
void test_invalid_length(void) {
    printf("Testing invalid length (4 cards)...\n");
    Card cards[4] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS}
    };
    assert(detect_royal_flush(cards, 4) == 0);
    printf("  Pass: Invalid length handled correctly\n");
}

/* Test royal flush with cards in different order */
void test_royal_flush_unordered(void) {
    printf("Testing royal flush with unordered cards...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_SPADES},
        {RANK_TEN, SUIT_SPADES},
        {RANK_KING, SUIT_SPADES},
        {RANK_JACK, SUIT_SPADES},
        {RANK_QUEEN, SUIT_SPADES}
    };
    assert(detect_royal_flush(cards, 5) == 1);
    printf("  Pass: Unordered royal flush detected correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_royal_flush function - Issue #22\n");
    printf("==========================================\n\n");

    test_royal_flush_hearts();
    test_royal_flush_diamonds();
    test_royal_flush_clubs();
    test_royal_flush_spades();
    test_nine_high_straight_flush();
    test_king_high_straight_flush();
    test_regular_flush();
    test_straight_not_flush();
    test_random_cards();
    test_invalid_length();
    test_royal_flush_unordered();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
