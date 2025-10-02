/* evaluator.c - Hand evaluation logic */

#include "../include/poker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Static helper: Compare ranks in descending order for qsort */
static int rank_compare_desc(const void* a, const void* b) {
    return (*(Rank*)b) - (*(Rank*)a);
}

/**
 * @brief Check if cards form a straight
 *
 * Detects straights in a 5-card hand, including special handling for
 * wheel straight (A-2-3-4-5). Sorts ranks internally before checking.
 *
 * @param cards Array of cards to check
 * @param len Number of cards (must be 5)
 * @param out_high_card Optional pointer to receive high card rank
 * @return 1 if straight detected, 0 otherwise
 */
int is_straight(const Card* cards, size_t len, Rank* out_high_card) {
    if (len != 5) {
        return 0;
    }

    /* Extract ranks into array */
    Rank ranks[5];
    for (size_t i = 0; i < 5; i++) {
        ranks[i] = (Rank)cards[i].rank;
    }

    /* Sort ranks in descending order */
    qsort(ranks, 5, sizeof(Rank), rank_compare_desc);

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
    for (size_t i = 1; i < 5; i++) {
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
 * @brief Count frequency of each rank
 *
 * Initializes counts array to zero, then iterates through cards and
 * increments the count for each card's rank. The counts array is indexed
 * by Rank values (2-14), so it must be at least 15 elements.
 *
 * Validates all input parameters and performs bounds checking on rank values
 * to prevent buffer overruns and undefined behavior.
 *
 * @param cards Array of cards to count (can be NULL if len is 0)
 * @param len Number of cards in array
 * @param counts Output array[15] to receive counts (indexed by Rank, cannot be NULL)
 */
void rank_counts(const Card* cards, size_t len, int* counts) {
    /* Validate counts pointer */
    if (counts == NULL) {
        return;
    }

    /* Initialize all counts to zero */
    memset(counts, 0, 15 * sizeof(int));

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

/**
 * @brief Check if all cards are the same suit
 *
 * Validates that all cards in the array have the same suit, which is required
 * for flush detection in poker hand evaluation.
 *
 * @param cards Array of cards to check
 * @param len Number of cards (must be 5 for valid poker hand)
 * @return 1 if all cards have same suit, 0 otherwise
 */
int is_flush(const Card* cards, size_t len) {
    /* Validate input parameters */
    if (cards == NULL || len != 5) {
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

int evaluate_hand(void) {
    /* Placeholder for hand evaluation */
    return 0;
}
