#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for rank_counts Helper Function
 * Tests verify rank frequency counting functionality
 */

void test_rank_counts_four_of_a_kind(void) {
    printf("Testing rank_counts with four of a kind...\n");

    // Four Aces and a King: AAAA K
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify counts
    assert(counts[RANK_ACE] == 4);
    assert(counts[RANK_KING] == 1);

    // Verify all other ranks are zero
    for (int i = 2; i <= 14; i++) {
        if (i != RANK_ACE && i != RANK_KING) {
            assert(counts[i] == 0);
        }
    }

    printf("  ✓ Four of a kind counted correctly\n");
}

void test_rank_counts_full_house(void) {
    printf("Testing rank_counts with full house...\n");

    // Three Queens and two Jacks: QQQ JJ
    Card cards[5] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify counts
    assert(counts[RANK_QUEEN] == 3);
    assert(counts[RANK_JACK] == 2);

    // Verify all other ranks are zero
    for (int i = 2; i <= 14; i++) {
        if (i != RANK_QUEEN && i != RANK_JACK) {
            assert(counts[i] == 0);
        }
    }

    printf("  ✓ Full house counted correctly\n");
}

void test_rank_counts_all_different(void) {
    printf("Testing rank_counts with all different ranks...\n");

    // Five different ranks: A K Q J T
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_TEN, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify each rank appears once
    assert(counts[RANK_ACE] == 1);
    assert(counts[RANK_KING] == 1);
    assert(counts[RANK_QUEEN] == 1);
    assert(counts[RANK_JACK] == 1);
    assert(counts[RANK_TEN] == 1);

    // Verify all other ranks are zero
    for (int i = 2; i <= 14; i++) {
        if (i != RANK_ACE && i != RANK_KING && i != RANK_QUEEN &&
            i != RANK_JACK && i != RANK_TEN) {
            assert(counts[i] == 0);
        }
    }

    printf("  ✓ All different ranks counted correctly\n");
}

void test_rank_counts_pair(void) {
    printf("Testing rank_counts with one pair...\n");

    // One pair of Tens: TT A K Q
    Card cards[5] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify counts
    assert(counts[RANK_TEN] == 2);
    assert(counts[RANK_ACE] == 1);
    assert(counts[RANK_KING] == 1);
    assert(counts[RANK_QUEEN] == 1);

    printf("  ✓ One pair counted correctly\n");
}

void test_rank_counts_two_pair(void) {
    printf("Testing rank_counts with two pair...\n");

    // Two pairs: AA KK Q
    Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify counts
    assert(counts[RANK_ACE] == 2);
    assert(counts[RANK_KING] == 2);
    assert(counts[RANK_QUEEN] == 1);

    printf("  ✓ Two pair counted correctly\n");
}

void test_rank_counts_three_of_a_kind(void) {
    printf("Testing rank_counts with three of a kind...\n");

    // Three Sevens: 777 A K
    Card cards[5] = {
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify counts
    assert(counts[RANK_SEVEN] == 3);
    assert(counts[RANK_ACE] == 1);
    assert(counts[RANK_KING] == 1);

    printf("  ✓ Three of a kind counted correctly\n");
}

void test_rank_counts_empty_array(void) {
    printf("Testing rank_counts with empty array...\n");

    int counts[15];
    rank_counts(NULL, 0, counts);

    // Verify all counts are zero
    for (int i = 0; i < 15; i++) {
        assert(counts[i] == 0);
    }

    printf("  ✓ Empty array handled correctly\n");
}

void test_rank_counts_single_card(void) {
    printf("Testing rank_counts with single card...\n");

    Card cards[1] = {{RANK_ACE, SUIT_HEARTS}};
    int counts[15];
    rank_counts(cards, 1, counts);

    // Verify only ace is counted
    assert(counts[RANK_ACE] == 1);

    // Verify all other ranks are zero
    for (int i = 0; i < 15; i++) {
        if (i != RANK_ACE) {
            assert(counts[i] == 0);
        }
    }

    printf("  ✓ Single card counted correctly\n");
}

void test_rank_counts_low_ranks(void) {
    printf("Testing rank_counts with low ranks...\n");

    // Low ranks: 2222 3
    Card cards[5] = {
        {RANK_TWO, SUIT_HEARTS},
        {RANK_TWO, SUIT_DIAMONDS},
        {RANK_TWO, SUIT_CLUBS},
        {RANK_TWO, SUIT_SPADES},
        {RANK_THREE, SUIT_HEARTS}
    };

    int counts[15];
    rank_counts(cards, 5, counts);

    // Verify counts
    assert(counts[RANK_TWO] == 4);
    assert(counts[RANK_THREE] == 1);

    printf("  ✓ Low ranks counted correctly\n");
}

int main(void) {
    printf("\n=== Rank Counts Test Suite ===\n\n");

    test_rank_counts_four_of_a_kind();
    test_rank_counts_full_house();
    test_rank_counts_all_different();
    test_rank_counts_pair();
    test_rank_counts_two_pair();
    test_rank_counts_three_of_a_kind();
    test_rank_counts_empty_array();
    test_rank_counts_single_card();
    test_rank_counts_low_ranks();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
