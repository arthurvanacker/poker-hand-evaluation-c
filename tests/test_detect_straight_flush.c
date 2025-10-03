#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for detect_straight_flush function
 * Tests verify straight flush detection (5 sequential suited cards)
 */

/* Test 9-high straight flush (5-6-7-8-9 of hearts) */
void test_nine_high_straight_flush(void) {
    printf("Testing 9-high straight flush (5-6-7-8-9 hearts)...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank high_card = 0;
    assert(detect_straight_flush(cards, 5, &high_card) == 1);
    assert(high_card == RANK_NINE);
    printf("  ✓ 9-high straight flush detected correctly (high card = 9)\n");
}

/* Test wheel straight flush (A-2-3-4-5 of diamonds) */
void test_wheel_straight_flush(void) {
    printf("Testing wheel straight flush (A-2-3-4-5 diamonds)...\n");
    Card cards[5] = {
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_TWO, SUIT_DIAMONDS},
        {RANK_THREE, SUIT_DIAMONDS},
        {RANK_FOUR, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_DIAMONDS}
    };
    Rank high_card = 0;
    assert(detect_straight_flush(cards, 5, &high_card) == 1);
    assert(high_card == RANK_FIVE);  /* Wheel: high card is 5, not Ace */
    printf("  ✓ Wheel straight flush detected correctly (high card = 5)\n");
}

/* Test not a straight flush - flush but not straight */
void test_flush_not_straight(void) {
    printf("Testing flush but not straight...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_SPADES},
        {RANK_FIVE, SUIT_SPADES},
        {RANK_SEVEN, SUIT_SPADES},
        {RANK_JACK, SUIT_SPADES},
        {RANK_ACE, SUIT_SPADES}
    };
    Rank high_card = 0;
    assert(detect_straight_flush(cards, 5, &high_card) == 0);
    printf("  ✓ Flush but not straight correctly identified as non-straight-flush\n");
}

/* Test not a straight flush - straight but not flush */
void test_straight_not_flush(void) {
    printf("Testing straight but not flush...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS}
    };
    Rank high_card = 0;
    assert(detect_straight_flush(cards, 5, &high_card) == 0);
    printf("  ✓ Straight but not flush correctly identified as non-straight-flush\n");
}

/* Test not a straight flush - neither straight nor flush */
void test_neither_straight_nor_flush(void) {
    printf("Testing neither straight nor flush...\n");
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_FIVE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };
    Rank high_card = 0;
    assert(detect_straight_flush(cards, 5, &high_card) == 0);
    printf("  ✓ Neither straight nor flush correctly identified as non-straight-flush\n");
}

/* Test royal flush (10-J-Q-K-A of clubs) */
void test_royal_flush(void) {
    printf("Testing royal flush (10-J-Q-K-A clubs)...\n");
    Card cards[5] = {
        {RANK_TEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_ACE, SUIT_CLUBS}
    };
    Rank high_card = 0;
    assert(detect_straight_flush(cards, 5, &high_card) == 1);
    assert(high_card == RANK_ACE);
    printf("  ✓ Royal flush detected correctly (high card = Ace)\n");
}

/* Test NULL output parameter */
void test_null_output_parameter(void) {
    printf("Testing NULL output parameter...\n");
    Card cards[5] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };
    assert(detect_straight_flush(cards, 5, NULL) == 1);
    printf("  ✓ NULL output parameter handled correctly\n");
}

/* Test invalid input - wrong length */
void test_invalid_length(void) {
    printf("Testing invalid length...\n");
    Card cards[4] = {
        {RANK_FIVE, SUIT_HEARTS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_HEARTS}
    };
    assert(detect_straight_flush(cards, 4, NULL) == 0);
    printf("  ✓ Invalid length handled correctly\n");
}

int main(void) {
    printf("\n==========================================\n");
    printf("Testing detect_straight_flush function - Issue #23\n");
    printf("==========================================\n\n");

    test_nine_high_straight_flush();
    test_wheel_straight_flush();
    test_flush_not_straight();
    test_straight_not_flush();
    test_neither_straight_nor_flush();
    test_royal_flush();
    test_null_output_parameter();
    test_invalid_length();

    printf("\n==========================================\n");
    printf("ALL TESTS PASSED!\n");
    printf("==========================================\n");

    return 0;
}
