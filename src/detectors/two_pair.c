/* two_pair.c - Two pair detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect two pair
 *
 * Detects if the hand contains exactly two pairs (2 ranks with 2 cards each).
 * Returns tiebreakers in the format: [high_pair, low_pair, kicker]
 * where pairs are sorted in descending order.
 *
 * The function validates all input parameters and handles the optional counts
 * parameter by computing counts internally if NULL is provided. It excludes
 * full houses (trips with a pair) and quads.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if two pair, 0 otherwise
 */
int detect_two_pair(const Card* const cards, const size_t len,
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

    /* Check for trips or quads (count >= 3) - excludes full houses and quads */
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] >= 3) {
            return 0;
        }
    }

    /* Find exactly 2 ranks with count == 2 (pairs) */
    Rank pairs[2];
    int pair_count = 0;

    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 2) {
            if (pair_count < 2) {
                pairs[pair_count++] = (Rank)rank;
            }
        }
    }

    /* Must have exactly 2 pairs */
    if (pair_count != 2) {
        return 0;
    }

    /* Sort pairs in descending order (high pair first) */
    if (pairs[0] < pairs[1]) {
        Rank temp = pairs[0];
        pairs[0] = pairs[1];
        pairs[1] = temp;
    }

    /* Find the kicker (count == 1) */
    Rank kicker = 0;
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (rank_count_array[rank] == 1) {
            kicker = (Rank)rank;
            break;
        }
    }

    /* Write tiebreakers: [high_pair, low_pair, kicker] */
    out_tiebreakers[0] = pairs[0];  /* High pair */
    out_tiebreakers[1] = pairs[1];  /* Low pair */
    out_tiebreakers[2] = kicker;
    *out_num_tiebreakers = 3;

    return 1;
}
