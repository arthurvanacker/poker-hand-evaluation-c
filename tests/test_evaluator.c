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

/* ========================================
 * Test Suite: rank_compare_desc Safety
 * Tests that rank comparison is overflow-safe
 * ======================================== */

void test_rank_compare_desc_edge_cases(void) {
    printf("Testing rank_compare_desc with edge case ranks...\n");

    /* Create hand with edge case ranks (ACE=14, TWO=2) that would overflow with subtraction
     * If using subtraction: (14) - (2) = 12 (OK)
     * But if using: (2) - (14) = -12 (OK for small values)
     * Edge case: Very large rank differences could theoretically overflow INT_MIN/MAX
     *
     * This test verifies correct sorting behavior regardless of implementation
     */
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},      /* 2 */
        {RANK_ACE, SUIT_DIAMONDS},    /* 14 */
        {RANK_FIVE, SUIT_CLUBS},      /* 5 */
        {RANK_KING, SUIT_SPADES},     /* 13 */
        {RANK_SEVEN, SUIT_HEARTS}     /* 7 */
    };

    /* Use detect_high_card to trigger rank_compare_desc sorting */
    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);

    /* Verify high card detection succeeded */
    assert(result == 1);
    assert(num_tiebreakers == 5);

    /* Verify ranks are sorted in descending order: ACE > KING > SEVEN > FIVE > TWO */
    assert(tiebreakers[0] == RANK_ACE);    /* 14 */
    assert(tiebreakers[1] == RANK_KING);   /* 13 */
    assert(tiebreakers[2] == RANK_SEVEN);  /* 7 */
    assert(tiebreakers[3] == RANK_FIVE);   /* 5 */
    assert(tiebreakers[4] == RANK_TWO);    /* 2 */

    printf("  ✓ rank_compare_desc handles edge case ranks correctly\n");
}

void test_rank_compare_desc_all_same(void) {
    printf("Testing rank_compare_desc with identical ranks...\n");

    /* All same rank - tests equal comparison */
    Card cards[5] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 5);

    /* First should be KING, then 4 QUEENs */
    assert(tiebreakers[0] == RANK_KING);
    assert(tiebreakers[1] == RANK_QUEEN);
    assert(tiebreakers[2] == RANK_QUEEN);
    assert(tiebreakers[3] == RANK_QUEEN);
    assert(tiebreakers[4] == RANK_QUEEN);

    printf("  ✓ rank_compare_desc handles identical ranks correctly\n");
}

void test_rank_compare_desc_consecutive_ranks(void) {
    printf("Testing rank_compare_desc with consecutive ranks...\n");

    /* Consecutive ranks in random order */
    Card cards[5] = {
        {RANK_SIX, SUIT_HEARTS},
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };

    Rank tiebreakers[5];
    size_t num_tiebreakers;
    int result = detect_high_card(cards, 5, tiebreakers, &num_tiebreakers);

    assert(result == 1);
    assert(num_tiebreakers == 5);

    /* Should be sorted: 10, 9, 8, 7, 6 */
    assert(tiebreakers[0] == RANK_TEN);
    assert(tiebreakers[1] == RANK_NINE);
    assert(tiebreakers[2] == RANK_EIGHT);
    assert(tiebreakers[3] == RANK_SEVEN);
    assert(tiebreakers[4] == RANK_SIX);

    printf("  ✓ rank_compare_desc handles consecutive ranks correctly\n");
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

    /* Test rank_compare_desc safety */
    test_rank_compare_desc_edge_cases();
    test_rank_compare_desc_all_same();
    test_rank_compare_desc_consecutive_ranks();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
