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
 * @brief Detect four of a kind
 *
 * Detects if the hand contains four cards of the same rank (four of a kind).
 * Returns tiebreakers in the format: [quad_rank, kicker].
 *
 * The function validates all input parameters and handles the optional counts
 * parameter by computing counts internally if NULL is provided.
 *
 * @param cards Array of exactly 5 cards
 * @param len Must be 5
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if four of a kind, 0 otherwise
 */
int detect_four_of_a_kind(const Card* cards, size_t len,
                           const int* counts,
                           Rank* out_tiebreakers,
                           size_t* out_num_tiebreakers) {
    /* Validate input parameters */
    if (cards == NULL || len != 5 || out_tiebreakers == NULL || out_num_tiebreakers == NULL) {
        return 0;
    }

    /* Use provided counts or compute locally */
    int local_counts[15];
    const int* rank_count_array;

    if (counts == NULL) {
        rank_counts(cards, len, local_counts);
        rank_count_array = local_counts;
    } else {
        rank_count_array = counts;
    }

    /* Find the quad rank (count == 4) */
    Rank quad_rank = 0;
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 4) {
            quad_rank = (Rank)rank;
            break;
        }
    }

    /* No quad found */
    if (quad_rank == 0) {
        return 0;
    }

    /* Find the kicker (count == 1) */
    Rank kicker = 0;
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 1) {
            kicker = (Rank)rank;
            break;
        }
    }

    /* Write tiebreakers: [quad_rank, kicker] */
    out_tiebreakers[0] = quad_rank;
    out_tiebreakers[1] = kicker;
    *out_num_tiebreakers = 2;

    return 1;
}

/**
 * @brief Detect straight flush
 *
 * Detects if the hand is a straight flush (5 sequential suited cards).
 * Combines is_flush() and is_straight() checks. Returns the high card
 * for tiebreakers (RANK_FIVE for wheel straight flush).
 *
 * @param cards Array of exactly 5 cards
 * @param len Must be 5
 * @param out_high_card Pointer to receive high card rank (can be NULL)
 * @return 1 if straight flush, 0 otherwise
 */
int detect_straight_flush(const Card* cards, size_t len, Rank* out_high_card) {
    /* Validate input length */
    if (len != 5) {
        return 0;
    }

    /* Check if all cards are the same suit (flush) */
    if (!is_flush(cards, len)) {
        return 0;
    }

    /* Check if cards form a straight */
    if (!is_straight(cards, len, out_high_card)) {
        return 0;
    }

    /* Both flush and straight confirmed - it's a straight flush */
    return 1;
}

int evaluate_hand(void) {
    /* Placeholder for hand evaluation */
    return 0;
}
