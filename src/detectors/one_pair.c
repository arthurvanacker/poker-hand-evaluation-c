/* one_pair.c - One pair detector */

#include "../../include/poker.h"
#include <stdlib.h>
#include <stddef.h>

/* External helper for rank comparison */
extern int rank_compare_desc(const void* a, const void* b);

/**
 * @brief Detect one pair
 *
 * Detects if the hand contains exactly one pair (1 rank with 2 cards).
 * Returns tiebreakers in the format: [pair_rank, kicker1, kicker2, kicker3]
 * where kickers are sorted in descending order.
 *
 * The function validates all input parameters and handles the optional counts
 * parameter by computing counts internally if NULL is provided. It excludes
 * two pairs, trips, full houses, and quads.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if one pair, 0 otherwise
 */
int detect_one_pair(const Card* const cards, const size_t len,
                    const int* const counts,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers) {
    /* Validate input parameters */
    if (cards == NULL || len != HAND_SIZE || out_tiebreakers == NULL || out_num_tiebreakers == NULL) {
        return 0;
    }

    /* Use provided counts or compute locally */
    int local_counts[RANK_ARRAY_SIZE];
    const int* rank_count_array;

    if (counts == NULL) {
        rank_counts(cards, len, local_counts);
        rank_count_array = local_counts;
    } else {
        rank_count_array = counts;
    }

    /* Check for trips or quads (count >= 3) - excludes full houses, trips, and quads */
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] >= 3) {
            return 0;
        }
    }

    /* Find exactly 1 rank with count == 2 (pair) */
    Rank pair_rank = 0;
    int pair_count = 0;

    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 2) {
            pair_rank = (Rank)rank;
            pair_count++;
        }
    }

    /* Must have exactly 1 pair */
    if (pair_count != 1) {
        return 0;
    }

    /* Find exactly 3 kickers (count == 1) */
    Rank kickers[3];
    int kicker_count = 0;

    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 1) {
            if (kicker_count < 3) {
                kickers[kicker_count++] = (Rank)rank;
            }
        }
    }

    /* Should have exactly 3 kickers */
    if (kicker_count != 3) {
        return 0;
    }

    /* Sort kickers in descending order */
    qsort(kickers, 3, sizeof(Rank), rank_compare_desc);

    /* Write tiebreakers: [pair_rank, kicker1, kicker2, kicker3] */
    out_tiebreakers[0] = pair_rank;
    out_tiebreakers[1] = kickers[0];  /* Highest kicker */
    out_tiebreakers[2] = kickers[1];  /* Middle kicker */
    out_tiebreakers[3] = kickers[2];  /* Lowest kicker */
    *out_num_tiebreakers = 4;

    return 1;
}
