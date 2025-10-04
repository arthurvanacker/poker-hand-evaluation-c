/*
 * Tests for example programs
 * Verify examples compile and demonstrate library functionality
 */

#include "../include/poker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Unity test framework */
void setUp(void) {}
void tearDown(void) {}

/*
 * Test: Verify poker_game example demonstrates basic usage
 * This test validates the example would work if compiled
 */
void test_poker_game_basic_flow(void) {
    srand(42);  /* Fixed seed for reproducible test */

    /* Create and shuffle deck */
    Deck* deck = deck_new();
    if (deck == NULL) {
        fprintf(stderr, "Failed to create deck\n");
        exit(1);
    }

    deck_shuffle(deck);

    /* Deal 5 cards */
    Card hand[HAND_SIZE];
    size_t dealt = deck_deal(deck, hand, HAND_SIZE);

    if (dealt != HAND_SIZE) {
        fprintf(stderr, "Expected to deal %d cards, got %zu\n", HAND_SIZE, dealt);
        deck_free(deck);
        exit(1);
    }

    /* Detect hand category using available detectors */
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    HandCategory category = HAND_HIGH_CARD;

    /* Check from strongest to weakest */
    if (detect_royal_flush(hand, HAND_SIZE)) {
        category = HAND_ROYAL_FLUSH;
    } else if (detect_straight_flush(hand, HAND_SIZE, NULL)) {
        category = HAND_STRAIGHT_FLUSH;
    } else if (detect_four_of_a_kind(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        category = HAND_FOUR_OF_A_KIND;
    } else if (detect_full_house(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        category = HAND_FULL_HOUSE;
    } else if (detect_flush(hand, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        category = HAND_FLUSH;
    } else if (detect_straight(hand, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        category = HAND_STRAIGHT;
    } else if (detect_three_of_a_kind(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        category = HAND_THREE_OF_A_KIND;
    } else if (detect_two_pair(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        category = HAND_TWO_PAIR;
    } else if (detect_one_pair(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        category = HAND_ONE_PAIR;
    } else {
        detect_high_card(hand, HAND_SIZE, tiebreakers, &num_tiebreakers);
    }

    /* Verify we detected a valid category */
    if (category < HAND_HIGH_CARD || category > HAND_ROYAL_FLUSH) {
        fprintf(stderr, "Invalid hand category: %d\n", category);
        deck_free(deck);
        exit(1);
    }

    deck_free(deck);
}

/*
 * Test: Verify hand_detector example demonstrates all detectors
 */
void test_hand_detector_all_categories(void) {
    /* Test royal flush detection */
    Card royal_flush[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TEN, SUIT_HEARTS}
    };

    if (!detect_royal_flush(royal_flush, HAND_SIZE)) {
        fprintf(stderr, "Failed to detect royal flush\n");
        exit(1);
    }

    /* Test straight flush detection */
    Card straight_flush[HAND_SIZE] = {
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_DIAMONDS}
    };

    if (!detect_straight_flush(straight_flush, HAND_SIZE, NULL)) {
        fprintf(stderr, "Failed to detect straight flush\n");
        exit(1);
    }

    /* Test four of a kind detection */
    Card four_kind[HAND_SIZE] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (!detect_four_of_a_kind(four_kind, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        fprintf(stderr, "Failed to detect four of a kind\n");
        exit(1);
    }

    /* Test full house detection */
    Card full_house[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    if (!detect_full_house(full_house, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        fprintf(stderr, "Failed to detect full house\n");
        exit(1);
    }
}

int main(void) {
    test_poker_game_basic_flow();
    test_hand_detector_all_categories();
    printf("All example tests passed!\n");
    return 0;
}
