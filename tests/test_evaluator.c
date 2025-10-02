#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for Evaluator Functions
 * Tests verify rank counting and hand evaluation logic
 */

/* ========================================
 * Test Suite: rank_counts Function
 * ======================================== */

void test_rank_counts_four_of_a_kind(void) {
    printf("Testing rank_counts with four of a kind...\n");

    /* Create 5 cards: four aces and one king */
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    /* Verify counts */
    assert(counts[RANK_ACE] == 4);
    assert(counts[RANK_KING] == 1);
    assert(counts[RANK_QUEEN] == 0);
    assert(counts[RANK_JACK] == 0);

    printf("  ✓ Four of a kind counts correct\n");
}

void test_rank_counts_full_house(void) {
    printf("Testing rank_counts with full house...\n");

    /* Create 5 cards: three queens and two jacks */
    Card cards[5] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_SPADES}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    /* Verify counts */
    assert(counts[RANK_QUEEN] == 3);
    assert(counts[RANK_JACK] == 2);
    assert(counts[RANK_ACE] == 0);
    assert(counts[RANK_KING] == 0);

    printf("  ✓ Full house counts correct\n");
}

void test_rank_counts_empty_array(void) {
    printf("Testing rank_counts with empty array...\n");

    int counts[15];
    rank_counts(NULL, 0, counts);

    /* Verify all counts are zero */
    for (int i = 0; i < 15; i++) {
        assert(counts[i] == 0);
    }

    printf("  ✓ Empty array counts correct\n");
}

void test_rank_counts_initialization(void) {
    printf("Testing rank_counts initializes all ranks to zero...\n");

    /* Create 2 cards */
    Card cards[2] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS}
    };

    int counts[15];
    rank_counts(cards, 2, counts);

    /* Verify specified ranks */
    assert(counts[RANK_TEN] == 1);
    assert(counts[RANK_TWO] == 1);

    /* Verify all other ranks are zero */
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank != RANK_TEN && rank != RANK_TWO) {
            assert(counts[rank] == 0);
        }
    }

    printf("  ✓ Initialization to zero works correctly\n");
}

void test_rank_counts_null_counts_pointer(void) {
    printf("Testing rank_counts with NULL counts pointer...\n");

    Card cards[3] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS}
    };

    /* Should not crash with NULL counts pointer */
    rank_counts(cards, 3, NULL);

    printf("  ✓ NULL counts pointer handled safely\n");
}

void test_rank_counts_invalid_rank_bounds(void) {
    printf("Testing rank_counts with invalid rank values...\n");

    /* Create cards with invalid ranks (out of bounds) */
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},        /* Valid: rank 14 */
        {0, SUIT_DIAMONDS},              /* Invalid: rank 0 */
        {1, SUIT_CLUBS},                 /* Invalid: rank 1 */
        {15, SUIT_SPADES},               /* Invalid: rank 15 */
        {RANK_KING, SUIT_HEARTS}         /* Valid: rank 13 */
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    /* Only valid ranks should be counted */
    assert(counts[RANK_ACE] == 1);
    assert(counts[RANK_KING] == 1);

    /* Invalid ranks should not cause buffer overrun */
    /* counts[0], counts[1], counts[15] should not be accessed */

    printf("  ✓ Invalid rank bounds handled safely\n");
}

int main(void) {
    printf("\n=== Evaluator Test Suite ===\n\n");

    /* Test rank_counts function */
    test_rank_counts_four_of_a_kind();
    test_rank_counts_full_house();
    test_rank_counts_empty_array();
    test_rank_counts_initialization();
    test_rank_counts_null_counts_pointer();
    test_rank_counts_invalid_rank_bounds();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
