/*
 * Test suite for is_straight helper function
 *
 * Tests straight detection in 5-card hands including:
 * - Regular straights (consecutive ranks)
 * - Wheel straight (A-2-3-4-5)
 * - Non-straight hands
 * - High card output parameter
 */

#include "../include/poker.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Test helper: create card with rank and suit */
static Card make_card(Rank rank, Suit suit) {
    Card c;
    c.rank = (uint8_t)rank;
    c.suit = (uint8_t)suit;
    return c;
}

/* Test: Regular straight (9-8-7-6-5) */
static void test_regular_straight_nine_high(void) {
    Card cards[5] = {
        make_card(RANK_NINE, SUIT_HEARTS),
        make_card(RANK_EIGHT, SUIT_DIAMONDS),
        make_card(RANK_SEVEN, SUIT_CLUBS),
        make_card(RANK_SIX, SUIT_SPADES),
        make_card(RANK_FIVE, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 1);
    assert(high_card == RANK_NINE);
    printf("PASS: test_regular_straight_nine_high\n");
}

/* Test: Regular straight (K-Q-J-T-9) */
static void test_regular_straight_king_high(void) {
    Card cards[5] = {
        make_card(RANK_KING, SUIT_HEARTS),
        make_card(RANK_QUEEN, SUIT_DIAMONDS),
        make_card(RANK_JACK, SUIT_CLUBS),
        make_card(RANK_TEN, SUIT_SPADES),
        make_card(RANK_NINE, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 1);
    assert(high_card == RANK_KING);
    printf("PASS: test_regular_straight_king_high\n");
}

/* Test: Regular straight (6-5-4-3-2) */
static void test_regular_straight_six_high(void) {
    Card cards[5] = {
        make_card(RANK_SIX, SUIT_HEARTS),
        make_card(RANK_FIVE, SUIT_DIAMONDS),
        make_card(RANK_FOUR, SUIT_CLUBS),
        make_card(RANK_THREE, SUIT_SPADES),
        make_card(RANK_TWO, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 1);
    assert(high_card == RANK_SIX);
    printf("PASS: test_regular_straight_six_high\n");
}

/* Test: Wheel straight (A-2-3-4-5) - special case */
static void test_wheel_straight_ace_low(void) {
    Card cards[5] = {
        make_card(RANK_ACE, SUIT_HEARTS),
        make_card(RANK_TWO, SUIT_DIAMONDS),
        make_card(RANK_THREE, SUIT_CLUBS),
        make_card(RANK_FOUR, SUIT_SPADES),
        make_card(RANK_FIVE, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 1);
    assert(high_card == RANK_FIVE);  /* Wheel straight: high card is 5 */
    printf("PASS: test_wheel_straight_ace_low\n");
}

/* Test: Ace-high straight (A-K-Q-J-T) */
static void test_ace_high_straight(void) {
    Card cards[5] = {
        make_card(RANK_ACE, SUIT_HEARTS),
        make_card(RANK_KING, SUIT_DIAMONDS),
        make_card(RANK_QUEEN, SUIT_CLUBS),
        make_card(RANK_JACK, SUIT_SPADES),
        make_card(RANK_TEN, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 1);
    assert(high_card == RANK_ACE);
    printf("PASS: test_ace_high_straight\n");
}

/* Test: Not a straight - random ranks */
static void test_not_straight_random(void) {
    Card cards[5] = {
        make_card(RANK_ACE, SUIT_HEARTS),
        make_card(RANK_KING, SUIT_DIAMONDS),
        make_card(RANK_EIGHT, SUIT_CLUBS),
        make_card(RANK_FIVE, SUIT_SPADES),
        make_card(RANK_TWO, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 0);
    printf("PASS: test_not_straight_random\n");
}

/* Test: Not a straight - one gap */
static void test_not_straight_one_gap(void) {
    Card cards[5] = {
        make_card(RANK_NINE, SUIT_HEARTS),
        make_card(RANK_EIGHT, SUIT_DIAMONDS),
        make_card(RANK_SEVEN, SUIT_CLUBS),
        make_card(RANK_SIX, SUIT_SPADES),
        make_card(RANK_FOUR, SUIT_HEARTS)  /* Gap: missing 5 */
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 0);
    printf("PASS: test_not_straight_one_gap\n");
}

/* Test: Not a straight - pair included */
static void test_not_straight_with_pair(void) {
    Card cards[5] = {
        make_card(RANK_NINE, SUIT_HEARTS),
        make_card(RANK_NINE, SUIT_DIAMONDS),
        make_card(RANK_SEVEN, SUIT_CLUBS),
        make_card(RANK_SIX, SUIT_SPADES),
        make_card(RANK_FIVE, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 0);
    printf("PASS: test_not_straight_with_pair\n");
}

/* Test: High card output parameter is optional (NULL) */
static void test_null_output_parameter(void) {
    Card cards[5] = {
        make_card(RANK_NINE, SUIT_HEARTS),
        make_card(RANK_EIGHT, SUIT_DIAMONDS),
        make_card(RANK_SEVEN, SUIT_CLUBS),
        make_card(RANK_SIX, SUIT_SPADES),
        make_card(RANK_FIVE, SUIT_HEARTS)
    };

    int result = is_straight(cards, 5, NULL);

    assert(result == 1);
    printf("PASS: test_null_output_parameter\n");
}

/* Test: Unsorted input (function should sort internally) */
static void test_unsorted_input(void) {
    Card cards[5] = {
        make_card(RANK_FIVE, SUIT_HEARTS),
        make_card(RANK_NINE, SUIT_DIAMONDS),
        make_card(RANK_SEVEN, SUIT_CLUBS),
        make_card(RANK_SIX, SUIT_SPADES),
        make_card(RANK_EIGHT, SUIT_HEARTS)
    };

    Rank high_card = 0;
    int result = is_straight(cards, 5, &high_card);

    assert(result == 1);
    assert(high_card == RANK_NINE);
    printf("PASS: test_unsorted_input\n");
}

int main(void) {
    printf("Running is_straight tests...\n\n");

    test_regular_straight_nine_high();
    test_regular_straight_king_high();
    test_regular_straight_six_high();
    test_wheel_straight_ace_low();
    test_ace_high_straight();
    test_not_straight_random();
    test_not_straight_one_gap();
    test_not_straight_with_pair();
    test_null_output_parameter();
    test_unsorted_input();

    printf("\nAll tests passed!\n");
    return 0;
}
