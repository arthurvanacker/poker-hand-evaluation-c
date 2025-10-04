/* helpers.c - Helper functions for hand evaluation */

#include "../include/poker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Static helper: Compare ranks in descending order for qsort
 * Uses conditional logic instead of subtraction to avoid integer overflow
 * Safety: Subtraction-based comparators can overflow when (b - a) > INT_MAX
 */
int rank_compare_desc(const void* a, const void* b) {
    Rank rank_a = *(Rank*)a;
    Rank rank_b = *(Rank*)b;

    /* Descending order: larger ranks come first */
    if (rank_b > rank_a) {
        return 1;   /* b is greater, so b comes first */
    } else if (rank_b < rank_a) {
        return -1;  /* a is greater, so a comes first */
    } else {
        return 0;   /* Equal ranks */
    }
}

/**
 * @brief Check if cards form a straight
 *
 * Detects straights in a HAND_SIZE-card hand, including special handling for
 * wheel straight (A-2-3-4-5). Sorts ranks internally before checking.
 *
 * @param cards Array of cards to check
 * @param len Number of cards (must be HAND_SIZE)
 * @param out_high_card Optional pointer to receive high card rank
 * @return 1 if straight detected, 0 otherwise
 */
int is_straight(const Card* const cards, const size_t len, Rank* const out_high_card) {
    if (len != HAND_SIZE) {
        return 0;
    }

    /* Extract ranks into array */
    Rank ranks[HAND_SIZE];
    for (size_t i = 0; i < HAND_SIZE; i++) {
        ranks[i] = (Rank)cards[i].rank;
    }

    /* Sort ranks in descending order */
    qsort(ranks, HAND_SIZE, sizeof(Rank), rank_compare_desc);

    /* Check for wheel straight: A-5-4-3-2 (sorted descending) */
    if (ranks[0] == RANK_ACE && ranks[1] == RANK_FIVE &&
        ranks[2] == RANK_FOUR && ranks[3] == RANK_THREE &&
        ranks[4] == RANK_TWO) {
        if (out_high_card != NULL) {
            *out_high_card = RANK_FIVE;  /* Wheel: high card is 5 */
        }
        return 1;
    }

    /* Check for regular straight: each rank = previous - 1 */
    for (size_t i = 1; i < HAND_SIZE; i++) {
        if (ranks[i] != ranks[i-1] - 1) {
            return 0;
        }
    }

    /* Regular straight found */
    if (out_high_card != NULL) {
        *out_high_card = ranks[0];  /* Highest rank is first after sort */
    }
    return 1;
}

/**
 * @brief Check if all cards are the same suit
 *
 * Validates that all cards in the array have the same suit, which is required
 * for flush detection in poker hand evaluation.
 *
 * @param cards Array of cards to check
 * @param len Number of cards (must be HAND_SIZE for valid poker hand)
 * @return 1 if all cards have same suit, 0 otherwise
 */
int is_flush(const Card* const cards, const size_t len) {
    /* Validate input parameters */
    if (cards == NULL || len != HAND_SIZE) {
        return 0;
    }

    /* Get suit of first card */
    uint8_t first_suit = cards[0].suit;

    /* Check if all other cards have same suit */
    for (size_t i = 1; i < len; i++) {
        if (cards[i].suit != first_suit) {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief Count frequency of each rank
 *
 * Initializes counts array to zero, then iterates through cards and
 * increments the count for each card's rank. The counts array is indexed
 * by Rank values (2-14), so it must be at least RANK_ARRAY_SIZE elements.
 *
 * Validates all input parameters and performs bounds checking on rank values
 * to prevent buffer overruns and undefined behavior.
 *
 * @param cards Array of cards to count (can be NULL if len is 0)
 * @param len Number of cards in array
 * @param counts Output array[RANK_ARRAY_SIZE] to receive counts (indexed by Rank, cannot be NULL)
 */
void rank_counts(const Card* const cards, const size_t len, int* const counts) {
    /* Validate counts pointer */
    if (counts == NULL) {
        return;
    }

    /* Initialize all counts to zero */
    memset(counts, 0, RANK_ARRAY_SIZE * sizeof(int));

    /* Handle empty or NULL cards array */
    if (cards == NULL || len == 0) {
        return;
    }

    /* Count occurrences of each rank with bounds checking */
    for (size_t i = 0; i < len; i++) {
        uint8_t rank = cards[i].rank;
        /* Validate rank is within bounds (2-14) */
        if (rank >= RANK_TWO && rank <= RANK_ACE) {
            counts[rank]++;
        }
    }
}
