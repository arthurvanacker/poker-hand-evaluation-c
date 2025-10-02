/* evaluator.c - Hand evaluation logic */

#include "../include/poker.h"
#include <stdio.h>
#include <string.h>

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
