/* full_house.c - Full house detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect full house
 *
 * Detects if the hand contains a full house (three of a kind + pair).
 * Returns tiebreakers in the format: [trip_rank, pair_rank].
 *
 * The function validates all input parameters and handles the optional counts
 * parameter by computing counts internally if NULL is provided.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if full house, 0 otherwise
 */
int detect_full_house(const Card* const cards, const size_t len,
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

    /* Find the trip rank (count == 3) */
    Rank trip_rank = 0;
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 3) {
            trip_rank = (Rank)rank;
            break;
        }
    }

    /* No trip found */
    if (trip_rank == 0) {
        return 0;
    }

    /* Find the pair rank (count == 2) */
    Rank pair_rank = 0;
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 2) {
            pair_rank = (Rank)rank;
            break;
        }
    }

    /* No pair found */
    if (pair_rank == 0) {
        return 0;
    }

    /* Write tiebreakers: [trip_rank, pair_rank] */
    out_tiebreakers[0] = trip_rank;
    out_tiebreakers[1] = pair_rank;
    *out_num_tiebreakers = 2;

    return 1;
}
