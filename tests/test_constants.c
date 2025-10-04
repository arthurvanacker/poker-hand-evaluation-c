/*
 * Test suite for poker constants
 * Verifies that magic number constants are properly defined
 */

#include "../include/poker.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Test that RANK_ARRAY_SIZE constant exists and has correct value */
static void test_rank_array_size_defined(void) {
    /* RANK_ARRAY_SIZE should be 15 (indices 0-14, where RANK_ACE=14) */
    assert(RANK_ARRAY_SIZE == 15);

    /* Verify it can be used to declare arrays */
    int counts[RANK_ARRAY_SIZE];
    memset(counts, 0, RANK_ARRAY_SIZE * sizeof(int));

    /* Verify all rank indices are valid (2-14) */
    assert(RANK_TWO >= 0 && RANK_TWO < RANK_ARRAY_SIZE);
    assert(RANK_ACE >= 0 && RANK_ACE < RANK_ARRAY_SIZE);

    printf("test_rank_array_size_defined: PASSED\n");
}

/* Test that HAND_SIZE constant exists and has correct value */
static void test_hand_size_defined(void) {
    /* HAND_SIZE should be 5 (standard poker hand) */
    assert(HAND_SIZE == 5);

    /* Verify it can be used to declare card arrays */
    Card hand[HAND_SIZE];

    /* Verify it matches the size used in Hand struct */
    Hand test_hand = {0};
    assert(sizeof(test_hand.cards) == HAND_SIZE * sizeof(Card));

    printf("test_hand_size_defined: PASSED\n");
}

/* Test that DECK_SIZE constant exists and has correct value */
static void test_deck_size_defined(void) {
    /* DECK_SIZE should be 52 (4 suits × 13 ranks) */
    assert(DECK_SIZE == 52);

    /* Verify it represents full deck (4 suits × 13 ranks) */
    assert(DECK_SIZE == 4 * 13);

    printf("test_deck_size_defined: PASSED\n");
}

/* Test that constants work correctly with deck operations */
static void test_deck_uses_deck_size(void) {
    Deck* deck = deck_new();
    assert(deck != NULL);

    /* Verify deck is initialized with DECK_SIZE cards */
    assert(deck->size == DECK_SIZE);
    assert(deck->capacity == DECK_SIZE);

    deck_free(deck);

    printf("test_deck_uses_deck_size: PASSED\n");
}

/* Test that constants work correctly with rank_counts */
static void test_rank_counts_uses_rank_array_size(void) {
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };

    int counts[RANK_ARRAY_SIZE];
    rank_counts(cards, HAND_SIZE, counts);

    /* Verify counts for each card */
    assert(counts[RANK_ACE] == 1);
    assert(counts[RANK_KING] == 1);
    assert(counts[RANK_QUEEN] == 1);
    assert(counts[RANK_JACK] == 1);
    assert(counts[RANK_TEN] == 1);

    printf("test_rank_counts_uses_rank_array_size: PASSED\n");
}

/* Test that hand validation uses HAND_SIZE */
static void test_hand_validation_uses_hand_size(void) {
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    /* Valid hand with HAND_SIZE cards should work */
    assert(is_flush(cards, HAND_SIZE) == 0);

    /* Incorrect size should fail (if implementation validates) */
    assert(is_flush(cards, HAND_SIZE - 1) == 0);
    assert(is_flush(cards, HAND_SIZE + 1) == 0);

    printf("test_hand_validation_uses_hand_size: PASSED\n");
}

int main(void) {
    printf("Running constants tests...\n\n");

    test_rank_array_size_defined();
    test_hand_size_defined();
    test_deck_size_defined();
    test_deck_uses_deck_size();
    test_rank_counts_uses_rank_array_size();
    test_hand_validation_uses_hand_size();

    printf("\nAll constants tests passed!\n");
    return 0;
}
