/* three_of_a_kind.c - Three of a kind detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect three of a kind (no pair)
 *
 * Detects if the hand contains three cards of the same rank without a pair
 * (excludes full houses). Returns tiebreakers in the format: [trip_rank, kicker1, kicker2]
 * where kickers are sorted in descending order.
 *
 * The function validates all input parameters and handles the optional counts
 * parameter by computing counts internally if NULL is provided.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if three of a kind, 0 otherwise
 */
int detect_three_of_a_kind(const Card* const cards, const size_t len,
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

    /* Check if there's a pair (which would make it a full house) */
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 2) {
            /* Full house detected - return 0 */
            return 0;
        }
    }

    /* Find exactly 2 kickers (count == 1) */
    Rank kickers[2];
    int kicker_count = 0;

    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 1) {
            kickers[kicker_count++] = (Rank)rank;
            if (kicker_count == 2) {
                break;
            }
        }
    }

    /* Should have exactly 2 kickers */
    if (kicker_count != 2) {
        return 0;
    }

    /* Sort kickers in descending order */
    if (kickers[0] < kickers[1]) {
        Rank temp = kickers[0];
        kickers[0] = kickers[1];
        kickers[1] = temp;
    }

    /* Write tiebreakers: [trip_rank, kicker1, kicker2] */
    out_tiebreakers[0] = trip_rank;
    out_tiebreakers[1] = kickers[0];
    out_tiebreakers[2] = kickers[1];
    *out_num_tiebreakers = 3;

    return 1;
}
