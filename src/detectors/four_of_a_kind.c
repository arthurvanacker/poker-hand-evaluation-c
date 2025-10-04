/* four_of_a_kind.c - Four of a kind detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect four of a kind
 *
 * Detects if the hand contains four cards of the same rank (four of a kind).
 * Returns tiebreakers in the format: [quad_rank, kicker].
 *
 * The function validates all input parameters and handles the optional counts
 * parameter by computing counts internally if NULL is provided.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if four of a kind, 0 otherwise
 */
int detect_four_of_a_kind(const Card* const cards, const size_t len,
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
